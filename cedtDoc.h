// cedtDoc.h : interface of the CCedtDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CEDTDOC_H__FFCA2B8C_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_)
#define AFX_CEDTDOC_H__FFCA2B8C_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCedtDoc : public CDocument
{
protected: // create from serialization only
	CCedtDoc();
	DECLARE_DYNCREATE(CCedtDoc)

public: // Attributes
	static BOOL m_bConvertTabsToSpacesBeforeSaving;
	static BOOL m_bRemoveTrailingSpacesBeforeSaving;

	static BOOL m_bSaveFilesInUnixFormat;
	static BOOL m_bSaveRemoteFilesInUnixFormat;

	static UINT m_nMakeBackupFile;
	static UINT m_nBackupMethod;
	static CString m_szBackupExtension;
	static CString m_szBackupDirectory;

public: // syntax types
	static CSyntaxType m_clsSyntaxTypes[MAX_SYNTAX_TYPE];

protected: // dictionary
	static CDictionary m_clsDictionary;
	static BOOL m_bDictionaryLoaded;

public: // file contents & status
	CAnalyzedText m_clsAnalyzedText;
	INT  m_nEncodingType; // ASCII/Unicode/UTF-8
	INT  m_nFileFormat;   // DOS/UNIX/MAC
	
	CFileStatus m_clsFileStatus;
	DWORD m_dwFileAttribute;

	BOOL m_bDocumentSaved;
	INT  m_nSavedUndoCount;

public: // shared remote path name
	static INT m_nCurrentFtpAccount;
	static CString m_szCurrentRemotePathName;

protected: // remote path name
	INT m_nFtpAccount;
	CString m_szRemotePathName;

public: // syntax type
	BOOL m_bAutomaticSyntaxType;
	CString m_szLangSpecFile;
	CString m_szKeywordsFile;

protected: // language specification
	CLangSpec m_clsLangSpec;
	CKeywords m_clsKeywords;

protected: // saved composition string
	CString m_szSavedCompositionString;
	BOOL m_bCompositionStringSaved;

protected: // undo & redo buffer
	CUndoBuffer m_clsUndoBuffer;
	CUndoBuffer m_clsRedoBuffer;

	BOOL m_bRecordingUndo;
	INT  m_nRecordingCount;


public: // *** cedtDoc.cpp ***
	BOOL IsModifiedOutside();
	void GoToLineNumber(INT nLineNumber);

	BOOL IsReadOnlyFile() const { return (m_dwFileAttribute & FILE_ATTRIBUTE_READONLY) ? TRUE : FALSE; }
	BOOL IsHiddenFile() const { return (m_dwFileAttribute & FILE_ATTRIBUTE_HIDDEN) ? TRUE : FALSE; }
	BOOL IsSystemFile() const { return (m_dwFileAttribute & FILE_ATTRIBUTE_SYSTEM) ? TRUE : FALSE; }

	BOOL IsRemoteFile() const { return (m_nFtpAccount >= 0) ? TRUE : FALSE; }
	BOOL IsNewFileNotSaved() const { return GetPathName().IsEmpty(); }

	BOOL HaveAnyOverflowLine() { return m_clsAnalyzedText.HaveAnyOverflowLine(); }
	BOOL MultiLineStringConstant() const { return m_clsLangSpec.m_bMultiLineStringConstant; }
	BOOL VariableHighlightInString() const { return m_clsLangSpec.m_bVariableHighlightInString; }

	BOOL HasLineCommentDelimiter() const { return m_clsLangSpec.m_szLineComment1.GetLength(); }
	BOOL HasBlockCommentDelimiter() const { return (m_clsLangSpec.m_szBlockComment1On.GetLength() && m_clsLangSpec.m_szBlockComment1Off.GetLength()); }

	INT GetFtpAccountNumber() const { return m_nFtpAccount; }
	CString GetRemotePathName() const { return m_szRemotePathName; }

	CString GetFullRemotePathName() const;
	LONG GetFileSize() const { return m_clsFileStatus.m_size; }

protected:
//	BOOL OnNewDocument();
	BOOL OnReloadDocument(LPCTSTR lpszPathName, INT nEncodingType);
//	BOOL OnOpenDocument(LPCTSTR lpszPathName);
//	BOOL OnSaveDocument(LPCTSTR lpszPathName);


public: // *** cedtDocView.cpp ***
	CView * GetFirstView();
	INT GetViewCount();

	void ReinitializeAllViews();
	void UpdateMDIFileTab();

	void FormatScreenText();
	void FormatScreenText(INT nIndex, INT nCount);

	void RemoveScreenText(INT nIndex, INT nCount);
	void InsertScreenText(INT nIndex, INT nCount);


public: // *** cedtDocFile.cpp ***
	BOOL FileSave();
	BOOL FileSaveAs();
	BOOL FileSaveAsRemote();
	BOOL FileReload(INT nEncodingType);

	BOOL SaveDocumentFile(LPCTSTR lpszPathName);
	BOOL SaveRemoteDocumentFile(INT nFtpAccount, LPCTSTR lpszPathName);

	BOOL ReloadDocumentFile(LPCTSTR lpszPathName, INT nLineNum, INT nEncodingType);
	BOOL ReloadRemoteDocumentFile(INT nFtpAccount, LPCTSTR lpszPathName, DWORD dwFileSize, INT nLineNum, INT nEncodingType);

	BOOL UpdateFileStatus();
	BOOL BackupDocument(LPCTSTR lpszPathName);


public: // *** cedtDocSyntax.cpp ***
	BOOL DetectSyntaxType(LPCTSTR lpszPathName, LPCTSTR lpszFirstLine);
	BOOL SearchLinkFileWithFirstLine(LPCTSTR lpszFirstLine, CString & szLinkFilePath);
	BOOL SearchLinkFileWithPathName(LPCTSTR lpszPathName, CString & szLinkFilePath);
	BOOL GetContainsPartOfLinkFile(LPCTSTR lpszLinkFilePath, CString & szContains);
	BOOL ReadExtensionLinkFile(LPCTSTR lpszLinkFilePath);
	BOOL LoadSyntaxInformation();


public: // *** cedtDocDictionary.cpp ***
	static BOOL IsDictionaryLoaded() { return m_bDictionaryLoaded; }
	static BOOL LoadDictionary();
	static BOOL LoadingProgress(UINT nTotalRead);


public: // *** cedtDocAnal.cpp ***
	INT  GetCharType(TCHAR nChar);
	void AnalyzeText() { AnalyzeText(0, GetLineCount()); }
	void AnalyzeText(INT nIndex, INT nCount);


public: // *** cedtDocHndr.cpp ***
	void OnUpdateDocuSyntaxType(CCmdUI * pCmdUI, INT nSyntaxType);
	void OnDocuSyntaxType(INT nSyntaxType);

	void OnDocuFileFormat(INT nFileFormat);
	void OnDocuEncodingType(INT nEncodingType);


public: // *** cedtDocMap.cpp ***
	INT GetLineCount() { return m_clsAnalyzedText.GetCount(); }
	INT GetLastIdxY() { return m_clsAnalyzedText.GetCount()-1; }
	INT GetWordCount();
	INT GetByteCount();

	INT GetFirstIdxX(CAnalyzedString & rLine);
	INT GetLastIdxX(CAnalyzedString & rLine);
	INT GetFirstNonBlankIdxX(CAnalyzedString & rLine);
	INT GetTrailingBlankIdxX(CAnalyzedString & rLine);

	CAnalyzedString & GetLineFromIdxY(INT nIdxY);
	ANALYZEDWORDINFO & GetWordFromIdxX(CAnalyzedString & rLine, INT nIdxX);

	BOOL IsBlankLine(CAnalyzedString & rLine);
	BOOL IsBlankLineFromIdxY(INT nIdxY);


public: // *** cedtDocEdit.cpp ***
	BOOL IsCompositionStringSaved() { return m_bCompositionStringSaved; }
	void EmptySavedCompositionString();
	void SaveCurrentCompositionString(INT nIdxY);
	void RestoreCurrentCompositionString(INT nIdxY);

	void InsertCompositionString(INT nIdxX, INT nIdxY, LPCTSTR lpszString);
	void DeleteCompositionString(INT nIdxX, INT nIdxY, INT nLength);

	void InsertChar(INT nIdxX, INT nIdxY, UINT nChar);
	void DeleteChar(INT nIdxX, INT nIdxY);
	void InsertString(INT nIdxX, INT nIdxY, LPCTSTR lpszString);
	void DeleteString(INT nIdxX, INT nIdxY, INT nLength);

	void InsertBlock(INT nBegX, INT nBegY, INT & nEndX, INT & nEndY, CMemText & rBlock);
	void DeleteBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY);
	void SplitLine(INT nIdxX, INT nIdxY);
	void JoinLines(INT nIdxX, INT nIdxY);

	void CopyToString(CString & rString, INT nBegX, INT nBegY, INT nLength);
	void CopyToBlock(CMemText & rBlock, INT nBegX, INT nBegY, INT nEndX, INT nEndY);

protected:
	void FastInsertChar(CAnalyzedString & rLine, INT nIdxX, INT nIdxY, UINT nChar);
	void FastDeleteChar(CAnalyzedString & rLine, INT nIdxX, INT nIdxY);
	void FastInsertString(CAnalyzedString & rLine, INT nIdxX, INT nIdxY, LPCTSTR lpszString);
	void FastDeleteString(CAnalyzedString & rLine, INT nIdxX, INT nIdxY, INT nLength);


public: // *** cedtDocEditAdv.cpp
	// return number of characters inserted or deleted
	INT  IndentLine(INT nIdxY, INT nTabSize, BOOL bUseSpaces);
	INT  UnindentLine(INT nIdxY, INT nTabSize);
	INT  MakeCommentLine(INT nIdxY);			INT  ReleaseCommentLine(INT nIdxY);

	// return number characters converted or deleted
	INT  ConvertTabsToSpaces(INT nIdxY);		INT  ConvertSpacesToTabs(INT nIdxY);
	INT  LeadingSpacesToTabs(INT nIdxY);
	INT  DeleteLeadingSpaces(INT nIdxY);		INT  DeleteTrailingSpaces(INT nIdxY);

	void IndentBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY, INT nTabSize, BOOL bUseSpaces);
	void UnindentBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY, INT nTabSize);
	void MakeCommentBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY);
	void ReleaseCommentBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY);

	void ConvertTabsToSpacesDocument();			void ConvertSpacesToTabsDocument();
	void LeadingSpacesToTabsDocument();			void DeleteTrailingSpacesDocument();

protected:
	INT  FastIndentLine(CAnalyzedString & rLine, INT nIdxY, INT nTabSize, BOOL bUseSpaces);
	INT  FastUnindentLine(CAnalyzedString & rLine, INT nIdxY, INT nTabSize);
	INT  FastMakeCommentLine(CAnalyzedString & rLine, INT nIdxY);
	INT  FastReleaseCommentLine(CAnalyzedString & rLine, INT nIdxY);

	INT  FastConvertTabsToSpaces(CAnalyzedString & rLine, INT nIdxY);
	INT  FastConvertSpacesToTabs(CAnalyzedString & rLine, INT nIdxY);
	INT  FastLeadingSpacesToTabs(CAnalyzedString & rLine, INT nIdxY);

	INT  FastDeleteLeadingSpaces(CAnalyzedString & rLine, INT nIdxY);
	INT  FastDeleteTrailingSpaces(CAnalyzedString & rLine, INT nIdxY);


public: // *** cedtDocUndo.cpp ***
	INT  GetUndoBufferCount() { return m_clsUndoBuffer.GetCount(); }
	INT  GetRedoBufferCount() { return m_clsRedoBuffer.GetCount(); }

	void EmptyUndoBuffer() { m_clsUndoBuffer.EmptyBuffer(); }
	void EmptyRedoBuffer() { m_clsRedoBuffer.EmptyBuffer(); }

	void GetLastEditingIndex(INT & nIdxX, INT & nIdxY) { m_clsUndoBuffer.GetRecentIndex(nIdxX, nIdxY); }
	void CheckIfAllActionsCanBeUndone() { if( m_nSavedUndoCount > GetUndoBufferCount() ) m_nSavedUndoCount = -1; }
	void CheckIfAllActionsAreUndone() { if( m_nSavedUndoCount == GetUndoBufferCount() ) SetModifiedFlag(FALSE); }

	void BeginActionRecording(BOOL bRecordingUndo);
	void EndActionRecording();

	void RecordInsertChar(INT nIdxX, INT nIdxY);
	void RecordDeleteChar(INT nIdxX, INT nIdxY, UINT nChar);
	void RecordInsertString(INT nIdxX, INT nIdxY, INT nLength);
	void RecordDeleteString(INT nIdxX, INT nIdxY, LPCTSTR lpszString);
	void RecordInsertBlock(INT nBegX, INT nBegY, INT nEndX, INT nEndY);
	void RecordDeleteBlock(INT nBegX, INT nBegY, CMemText & rBlock);
	void RecordSplitLine(INT nIdxX, INT nIdxY);
	void RecordJoinLines(INT nIdxX, INT nIdxY);

	INT PopUndoBuffer() { return m_clsUndoBuffer.RemoveHead(); }
	INT PopUndoAction() { return m_clsUndoBuffer.m_lstAction.RemoveHead(); }
	INT PopUndoIdxX() { return m_clsUndoBuffer.m_lstIdxX.RemoveHead(); }
	INT PopUndoIdxY() { return m_clsUndoBuffer.m_lstIdxY.RemoveHead(); }
	INT PopUndoParam() { return m_clsUndoBuffer.m_lstParam.RemoveHead(); }
	UINT PopUndoChar() { return m_clsUndoBuffer.m_lstChar.RemoveHead(); }
	CString PopUndoString() { return m_clsUndoBuffer.m_lstString.RemoveHead(); }
	CMemText PopUndoBlock() { return m_clsUndoBuffer.m_lstBlock.RemoveHead(); }

	INT PopRedoBuffer() { return m_clsRedoBuffer.RemoveHead(); }
	INT PopRedoAction() { return m_clsRedoBuffer.m_lstAction.RemoveHead(); }
	INT PopRedoIdxX() { return m_clsRedoBuffer.m_lstIdxX.RemoveHead(); }
	INT PopRedoIdxY() { return m_clsRedoBuffer.m_lstIdxY.RemoveHead(); }
	INT PopRedoParam() { return m_clsRedoBuffer.m_lstParam.RemoveHead(); }
	UINT PopRedoChar() { return m_clsRedoBuffer.m_lstChar.RemoveHead(); }
	CString PopRedoString() { return m_clsRedoBuffer.m_lstString.RemoveHead(); }
	CMemText PopRedoBlock() { return m_clsRedoBuffer.m_lstBlock.RemoveHead(); }


public: // *** cedtDocSearch.cpp ***
	BOOL OnePassFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp);
	BOOL ForwardFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp);
	BOOL ReverseFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp);

	void ToggleBookmark(INT nIdxY);
	BOOL FindNextBookmark(INT & nIdxY);
	BOOL FindPrevBookmark(INT & nIdxY);

	BOOL IsThisIndentOnChar(INT nIdxX, INT nIdxY);
	BOOL IsThisIndentOffChar(INT nIdxX, INT nIdxY);

	BOOL IsThisOneOfPairs(INT nIdxX, INT nIdxY, BOOL & bBeginning);
	BOOL FindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY);

	BOOL ForwardFindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY);
	BOOL ReverseFindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY);

	BOOL ForwardFindEndingPair(INT & nIdxX, INT & nIdxY);
	BOOL ReverseFindBeginningPair(INT & nIdxX, INT & nIdxY);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCedtDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCedtDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCedtDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSaveAll();
	afx_msg void OnFileSaveAsRemote();
	afx_msg void OnFileReload();
	afx_msg void OnFileReloadAs();
	afx_msg void OnUpdateDocuSyntaxAuto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType9(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType11(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType12(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType13(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType14(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType15(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType16(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType17(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType18(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType19(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType20(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType21(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType22(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType23(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType24(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType25(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType26(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType27(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType28(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType29(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType30(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuSyntaxType31(CCmdUI* pCmdUI);
	afx_msg void OnDocuSyntaxType0();
	afx_msg void OnDocuSyntaxType1();
	afx_msg void OnDocuSyntaxType2();
	afx_msg void OnDocuSyntaxType3();
	afx_msg void OnDocuSyntaxType4();
	afx_msg void OnDocuSyntaxType5();
	afx_msg void OnDocuSyntaxType6();
	afx_msg void OnDocuSyntaxType7();
	afx_msg void OnDocuSyntaxType8();
	afx_msg void OnDocuSyntaxType9();
	afx_msg void OnDocuSyntaxType10();
	afx_msg void OnDocuSyntaxType11();
	afx_msg void OnDocuSyntaxType12();
	afx_msg void OnDocuSyntaxType13();
	afx_msg void OnDocuSyntaxType14();
	afx_msg void OnDocuSyntaxType15();
	afx_msg void OnDocuSyntaxType16();
	afx_msg void OnDocuSyntaxType17();
	afx_msg void OnDocuSyntaxType18();
	afx_msg void OnDocuSyntaxType19();
	afx_msg void OnDocuSyntaxType20();
	afx_msg void OnDocuSyntaxType21();
	afx_msg void OnDocuSyntaxType22();
	afx_msg void OnDocuSyntaxType23();
	afx_msg void OnDocuSyntaxType24();
	afx_msg void OnDocuSyntaxType25();
	afx_msg void OnDocuSyntaxType26();
	afx_msg void OnDocuSyntaxType27();
	afx_msg void OnDocuSyntaxType28();
	afx_msg void OnDocuSyntaxType29();
	afx_msg void OnDocuSyntaxType30();
	afx_msg void OnDocuSyntaxType31();
	afx_msg void OnDocuSyntaxAuto();
	afx_msg void OnDocuSyntaxText();
	afx_msg void OnUpdateDocuFormatDos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuFormatUnix(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuFormatMac(CCmdUI* pCmdUI);
	afx_msg void OnDocuFormatDos();
	afx_msg void OnDocuFormatUnix();
	afx_msg void OnDocuFormatMac();
	afx_msg void OnUpdateDocuEncodingAscii(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuEncodingUnicodeLE(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuEncodingUnicodeBE(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuEncodingUtf8WBOM(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDocuEncodingUtf8XBOM(CCmdUI* pCmdUI);
	afx_msg void OnDocuEncodingAscii();
	afx_msg void OnDocuEncodingUnicodeLE();
	afx_msg void OnDocuEncodingUnicodeBE();
	afx_msg void OnDocuEncodingUtf8WBOM();
	afx_msg void OnDocuEncodingUtf8XBOM();
	afx_msg void OnDocuSummary();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEDTDOC_H__FFCA2B8C_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_)
