// cedtDoc.cpp : implementation of the CCedtDoc class
//

#include "stdafx.h"
#include "cedtHeader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCedtDoc
IMPLEMENT_DYNCREATE(CCedtDoc, CDocument)

BEGIN_MESSAGE_MAP(CCedtDoc, CDocument)
	//{{AFX_MSG_MAP(CCedtDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_COMMAND(ID_FILE_SAVE_AS_REMOTE, OnFileSaveAsRemote)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
	ON_COMMAND(ID_FILE_RELOAD_AS, OnFileReloadAs)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_AUTO, OnUpdateDocuSyntaxAuto)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TEXT, OnUpdateDocuSyntaxText)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE0, OnUpdateDocuSyntaxType0)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE1, OnUpdateDocuSyntaxType1)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE2, OnUpdateDocuSyntaxType2)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE3, OnUpdateDocuSyntaxType3)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE4, OnUpdateDocuSyntaxType4)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE5, OnUpdateDocuSyntaxType5)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE6, OnUpdateDocuSyntaxType6)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE7, OnUpdateDocuSyntaxType7)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE8, OnUpdateDocuSyntaxType8)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE9, OnUpdateDocuSyntaxType9)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE10, OnUpdateDocuSyntaxType10)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE11, OnUpdateDocuSyntaxType11)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE12, OnUpdateDocuSyntaxType12)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE13, OnUpdateDocuSyntaxType13)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE14, OnUpdateDocuSyntaxType14)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE15, OnUpdateDocuSyntaxType15)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE16, OnUpdateDocuSyntaxType16)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE17, OnUpdateDocuSyntaxType17)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE18, OnUpdateDocuSyntaxType18)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE19, OnUpdateDocuSyntaxType19)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE20, OnUpdateDocuSyntaxType20)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE21, OnUpdateDocuSyntaxType21)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE22, OnUpdateDocuSyntaxType22)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE23, OnUpdateDocuSyntaxType23)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE24, OnUpdateDocuSyntaxType24)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE25, OnUpdateDocuSyntaxType25)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE26, OnUpdateDocuSyntaxType26)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE27, OnUpdateDocuSyntaxType27)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE28, OnUpdateDocuSyntaxType28)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE29, OnUpdateDocuSyntaxType29)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE30, OnUpdateDocuSyntaxType30)
	ON_UPDATE_COMMAND_UI(ID_DOCU_SYNTAX_TYPE31, OnUpdateDocuSyntaxType31)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE0, OnDocuSyntaxType0)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE1, OnDocuSyntaxType1)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE2, OnDocuSyntaxType2)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE3, OnDocuSyntaxType3)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE4, OnDocuSyntaxType4)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE5, OnDocuSyntaxType5)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE6, OnDocuSyntaxType6)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE7, OnDocuSyntaxType7)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE8, OnDocuSyntaxType8)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE9, OnDocuSyntaxType9)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE10, OnDocuSyntaxType10)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE11, OnDocuSyntaxType11)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE12, OnDocuSyntaxType12)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE13, OnDocuSyntaxType13)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE14, OnDocuSyntaxType14)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE15, OnDocuSyntaxType15)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE16, OnDocuSyntaxType16)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE17, OnDocuSyntaxType17)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE18, OnDocuSyntaxType18)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE19, OnDocuSyntaxType19)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE20, OnDocuSyntaxType20)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE21, OnDocuSyntaxType21)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE22, OnDocuSyntaxType22)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE23, OnDocuSyntaxType23)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE24, OnDocuSyntaxType24)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE25, OnDocuSyntaxType25)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE26, OnDocuSyntaxType26)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE27, OnDocuSyntaxType27)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE28, OnDocuSyntaxType28)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE29, OnDocuSyntaxType29)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE30, OnDocuSyntaxType30)
	ON_COMMAND(ID_DOCU_SYNTAX_TYPE31, OnDocuSyntaxType31)
	ON_COMMAND(ID_DOCU_SYNTAX_AUTO, OnDocuSyntaxAuto)
	ON_COMMAND(ID_DOCU_SYNTAX_TEXT, OnDocuSyntaxText)
	ON_UPDATE_COMMAND_UI(ID_DOCU_FORMAT_DOS, OnUpdateDocuFormatDos)
	ON_UPDATE_COMMAND_UI(ID_DOCU_FORMAT_UNIX, OnUpdateDocuFormatUnix)
	ON_UPDATE_COMMAND_UI(ID_DOCU_FORMAT_MAC, OnUpdateDocuFormatMac)
	ON_COMMAND(ID_DOCU_FORMAT_DOS, OnDocuFormatDos)
	ON_COMMAND(ID_DOCU_FORMAT_UNIX, OnDocuFormatUnix)
	ON_COMMAND(ID_DOCU_FORMAT_MAC, OnDocuFormatMac)
	ON_UPDATE_COMMAND_UI(ID_DOCU_ENCODING_ASCII, OnUpdateDocuEncodingAscii)
	ON_UPDATE_COMMAND_UI(ID_DOCU_ENCODING_UNICODE_LE, OnUpdateDocuEncodingUnicodeLE)
	ON_UPDATE_COMMAND_UI(ID_DOCU_ENCODING_UNICODE_BE, OnUpdateDocuEncodingUnicodeBE)
	ON_UPDATE_COMMAND_UI(ID_DOCU_ENCODING_UTF8_WBOM, OnUpdateDocuEncodingUtf8WBOM)
	ON_UPDATE_COMMAND_UI(ID_DOCU_ENCODING_UTF8_XBOM, OnUpdateDocuEncodingUtf8XBOM)
	ON_COMMAND(ID_DOCU_ENCODING_ASCII, OnDocuEncodingAscii)
	ON_COMMAND(ID_DOCU_ENCODING_UNICODE_LE, OnDocuEncodingUnicodeLE)
	ON_COMMAND(ID_DOCU_ENCODING_UNICODE_BE, OnDocuEncodingUnicodeBE)
	ON_COMMAND(ID_DOCU_ENCODING_UTF8_WBOM, OnDocuEncodingUtf8WBOM)
	ON_COMMAND(ID_DOCU_ENCODING_UTF8_XBOM, OnDocuEncodingUtf8XBOM)
	ON_COMMAND(ID_DOCU_SUMMARY, OnDocuSummary)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCedtDoc static member variables
BOOL CCedtDoc::m_bConvertTabsToSpacesBeforeSaving;
BOOL CCedtDoc::m_bRemoveTrailingSpacesBeforeSaving;

BOOL CCedtDoc::m_bSaveFilesInUnixFormat;
BOOL CCedtDoc::m_bSaveRemoteFilesInUnixFormat;

UINT CCedtDoc::m_nMakeBackupFile;
UINT CCedtDoc::m_nBackupMethod;
CString CCedtDoc::m_szBackupExtension;
CString CCedtDoc::m_szBackupDirectory;

CSyntaxType CCedtDoc::m_clsSyntaxTypes[MAX_SYNTAX_TYPE];

CDictionary CCedtDoc::m_clsDictionary;
BOOL CCedtDoc::m_bDictionaryLoaded = FALSE;

INT CCedtDoc::m_nCurrentFtpAccount = -1;
CString CCedtDoc::m_szCurrentRemotePathName = "";


/////////////////////////////////////////////////////////////////////////////
// CCedtDoc construction/destruction
CCedtDoc::CCedtDoc()
{
	m_szSavedCompositionString = "";
	m_bCompositionStringSaved = FALSE;
}

CCedtDoc::~CCedtDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCedtDoc diagnostics

#ifdef _DEBUG
void CCedtDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCedtDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Operations

BOOL CCedtDoc::IsModifiedOutside()
{
	CString szPathName = GetPathName();
	if( ! szPathName.GetLength() || IsRemoteFile() ) return FALSE;

	CFileStatus status;	
	if( ! CFile::GetStatus(szPathName, status) ) return FALSE;

	return( status.m_mtime != m_clsFileStatus.m_mtime );
}

void CCedtDoc::GoToLineNumber(INT nLineNumber)
{
	POSITION pos = GetFirstViewPosition();
	while( pos ) {
		CCedtView * pView = (CCedtView *)GetNextView( pos );
		pView->OnSearchGoTo(nLineNumber);
	}
}

void CCedtDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	m_strPathName = lpszPathName;
	ASSERT( ! m_strPathName.IsEmpty() );
	m_bEmbedded = FALSE;

	// set the document title based on path name
	if( ! IsRemoteFile() ) SetTitle( GetPathName() );
	else SetTitle( GetFullRemotePathName() );

	// add it to the file MRU list
	if( bAddToMRU && ! IsRemoteFile() )
		AfxGetApp()->AddToRecentFileList(lpszPathName);
}

CString CCedtDoc::GetFullRemotePathName() const
{
	CString szFullRemotePathName = CCedtApp::m_clsFtpAccounts[m_nFtpAccount].GetFullAccountName();
	szFullRemotePathName += m_szRemotePathName;
	return szFullRemotePathName;
}

/////////////////////////////////////////////////////////////////////////////
// CCedtDoc commands

BOOL CCedtDoc::OnNewDocument()
{
	m_nFtpAccount = -1;
	m_szRemotePathName = "";

	// This code segment replaces CDocument::OnNewDocument()
	if( IsModified() ) TRACE0("Warning: OnNewDocument replaces an unsaved document.\n");

	DeleteContents();
	m_strPathName.Empty();  // no path name yet
	SetModifiedFlag(FALSE); // make clean
	// End of code segment CDocument::OnNewDocument()

	m_bDocumentSaved = FALSE;
	m_nSavedUndoCount = 0;

	EmptyUndoBuffer();
	EmptyRedoBuffer();

	m_bRecordingUndo = TRUE;
	m_nRecordingCount = 0;

	m_nEncodingType = ENCODING_TYPE_ASCII; m_nFileFormat = FILE_FORMAT_DOS;
	if( CCedtApp::m_bOpenTemplate ) m_clsAnalyzedText.FileLoad(CCedtApp::m_szOpenTemplatePathName, m_nEncodingType, m_nFileFormat);
	else m_clsAnalyzedText.AddTail("");

	ZeroMemory( & m_clsFileStatus, sizeof(m_clsFileStatus) );
	m_dwFileAttribute = FILE_ATTRIBUTE_NORMAL;

	m_szLangSpecFile = ""; m_clsLangSpec.ResetContents(); 
	m_szKeywordsFile = ""; m_clsKeywords.ResetContents();
	m_bAutomaticSyntaxType = TRUE;

	if( CCedtApp::m_bOpenTemplate && DetectSyntaxType(CCedtApp::m_szOpenTemplatePathName, m_clsAnalyzedText.GetHead()) ) LoadSyntaxInformation();
	AnalyzeText();

	CCedtApp::m_szOpenTemplatePathName = "";
	CCedtApp::m_bOpenTemplate = FALSE;

//	ReinitializeAllViews();
//	FormatScreenText();
//	UpdateAllViews(NULL);

	return TRUE;
}

BOOL CCedtDoc::OnReloadDocument(LPCTSTR lpszPathName, INT nEncodingType)
{
	if( ! strlen(lpszPathName) ) return FALSE;

//	m_nFtpAccount = m_nCurrentFtpAccount;
//	m_szRemotePathName = m_szCurrentRemotePathName;

	// This code segment imitate CDocument::OnOpenDocument(lpszPathName)
	if( IsModified() ) TRACE0("Warning: OnReloadDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile * pFile = GetFile(lpszPathName, CFile::modeRead | CFile::shareDenyNone, &fe);
	if( pFile == NULL ) {
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	ReleaseFile(pFile, FALSE);

	DeleteContents();
	SetModifiedFlag(FALSE);
	// End of code segment CDocument::OnOpenDocument(lpszPathName)

	m_bDocumentSaved = FALSE;
	m_nSavedUndoCount = 0;

	EmptyUndoBuffer();
	EmptyRedoBuffer();

	m_bRecordingUndo = TRUE;
	m_nRecordingCount = 0;

	DetectEncodingTypeAndFileFormat(lpszPathName, m_nEncodingType, m_nFileFormat);
	if( nEncodingType != ENCODING_TYPE_UNKNOWN ) m_nEncodingType = nEncodingType;
	m_clsAnalyzedText.FileLoad(lpszPathName, m_nEncodingType, m_nFileFormat);

	CFile::GetStatus(lpszPathName, m_clsFileStatus);
	m_dwFileAttribute = GetFileAttributes(lpszPathName);

//	m_szLangSpecFileName = ""; m_clsLangSpec.Reset(); 
//	m_szKeywordsFileName = ""; m_clsKeywords.Reset();
//	m_bAutomaticSyntaxType = TRUE;

//	if( DetectSyntaxType(lpszPathName, m_clsAnalyzedText.GetHead()) ) LoadSyntaxInformation();
	AnalyzeText();

	ReinitializeAllViews();
//	FormatScreenText();
	UpdateAllViews(NULL);
	
	return TRUE;
}

BOOL CCedtDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_nFtpAccount = m_nCurrentFtpAccount;
	m_szRemotePathName = m_szCurrentRemotePathName;

	// make backup file
	if( m_nMakeBackupFile == 2 && ! IsRemoteFile() && VerifyFilePath(lpszPathName) ) {
		if( ! BackupDocument(lpszPathName) ) AfxMessageBox( IDS_ERR_FILE_BACKUP_FAILED );
	}

	// This code segment replaces CDocument::OnOpenDocument(lpszPathName)
	if( IsModified() ) TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile * pFile = GetFile(lpszPathName, CFile::modeRead | CFile::shareDenyNone, &fe);
	if( pFile == NULL ) {
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	ReleaseFile(pFile, FALSE);

	DeleteContents();
	SetModifiedFlag(FALSE);
	// End of code segment CDocument::OnOpenDocument(lpszPathName)

	m_bDocumentSaved = FALSE;
	m_nSavedUndoCount = 0;

	EmptyUndoBuffer();
	EmptyRedoBuffer();

	m_bRecordingUndo = TRUE;
	m_nRecordingCount = 0;

	DetectEncodingTypeAndFileFormat(lpszPathName, m_nEncodingType, m_nFileFormat);
	m_clsAnalyzedText.FileLoad(lpszPathName, m_nEncodingType, m_nFileFormat);

	CFile::GetStatus(lpszPathName, m_clsFileStatus);
	m_dwFileAttribute = GetFileAttributes(lpszPathName);

	m_szLangSpecFile = ""; m_clsLangSpec.ResetContents(); 
	m_szKeywordsFile = ""; m_clsKeywords.ResetContents();
	m_bAutomaticSyntaxType = TRUE;

	if( DetectSyntaxType(lpszPathName, m_clsAnalyzedText.GetHead()) ) LoadSyntaxInformation();
	AnalyzeText();

//	ReinitializeAllViews();
//	FormatScreenText();
//	UpdateAllViews(NULL);
	
	return TRUE;
}

BOOL CCedtDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// retrieve old path name
	CString szOldPathName = GetPathName();
	if( szOldPathName.IsEmpty() ) szOldPathName = GetTitle();

	// check if file can be written
	DWORD dwAttrib = 0x00000000;
	if( VerifyFilePath(lpszPathName) ) dwAttrib = GetFileAttributes(lpszPathName);

	if( dwAttrib == 0xFFFFFFFF ) { // GetFileAttributes() error
		INT nReturn = AfxMessageBox( IDS_ERR_FILE_ATTR_CHECK, MB_YESNO );
		if( nReturn != IDYES ) return FALSE;
	} else if( (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) || (dwAttrib & FILE_ATTRIBUTE_READONLY) ) {
		CString szMessage; szMessage.Format( IDS_ERR_FILE_SAVE_DENIED1, lpszPathName );
		AfxMessageBox( szMessage, MB_OK | MB_ICONSTOP ); return FALSE;
	} else if( (dwAttrib & FILE_ATTRIBUTE_HIDDEN) || (dwAttrib & FILE_ATTRIBUTE_SYSTEM) ) {
		CString szMessage; szMessage.Format( IDS_ERR_FILE_SAVE_DENIED2, lpszPathName );
		AfxMessageBox( szMessage, MB_OK | MB_ICONSTOP ); return FALSE;
	}

	m_nFtpAccount = m_nCurrentFtpAccount;
	m_szRemotePathName = m_szCurrentRemotePathName;

	// make backup file
	if( m_nMakeBackupFile == 1 && ! IsRemoteFile() && VerifyFilePath(lpszPathName) ) {
		if( ! BackupDocument(lpszPathName) ) AfxMessageBox( IDS_ERR_FILE_BACKUP_FAILED );
	}

	// check if it need to remove trailing spaces
	if( m_bRemoveTrailingSpacesBeforeSaving ) {
		CCedtView * pView = (CCedtView *)GetFirstView();
		pView->SendMessage(WM_COMMAND, ID_EDIT_REMOVE_TRAILING_SPACES, 0L);
	}

	// check if it need to convert tabs to spaces
	if( m_bConvertTabsToSpacesBeforeSaving ) {
		CCedtView * pView = (CCedtView *)GetFirstView();
		pView->SendMessage(WM_COMMAND, ID_EDIT_CONVERT_TABS_TO_SPACES, 0L);
	}

	// This code segment replaces CDocument::OnSaveDocument(lpszPathName)
	CFileException fe;
	CFile * pFile = GetFile(lpszPathName, CFile::modeReadWrite | CFile::modeCreate | CFile::shareExclusive, &fe);
	if( pFile == NULL ) {
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
	ReleaseFile(pFile, FALSE);

	SetModifiedFlag(FALSE); // back to unmodified
	// End of code segment CDocument::OnSaveDocument(lpszPathName)

	m_bDocumentSaved = TRUE;
	m_nSavedUndoCount = GetUndoBufferCount();

//	EmptyUndoBuffer(); - do not empty undo buffer
//	EmptyRedoBuffer(); - do not empty redo buffer

	m_bRecordingUndo = TRUE;
	m_nRecordingCount = 0;

	if( m_bSaveFilesInUnixFormat ) m_nFileFormat = FILE_FORMAT_UNIX;
	if( IsRemoteFile() && m_bSaveRemoteFilesInUnixFormat ) m_nFileFormat = FILE_FORMAT_UNIX;
	m_clsAnalyzedText.FileSave(lpszPathName, m_nEncodingType, m_nFileFormat);

	CFile::GetStatus(lpszPathName, m_clsFileStatus);
	m_dwFileAttribute = GetFileAttributes(lpszPathName);

	CString szOldExtension = GetFileExtension(szOldPathName);
	CString szNewExtension = GetFileExtension(lpszPathName);

	if( m_bAutomaticSyntaxType && szNewExtension != szOldExtension ) {
		m_szLangSpecFile = ""; m_clsLangSpec.ResetContents(); 
		m_szKeywordsFile = ""; m_clsKeywords.ResetContents();
		m_bAutomaticSyntaxType = TRUE;

		if( DetectSyntaxType(lpszPathName, m_clsAnalyzedText.GetHead()) ) LoadSyntaxInformation();
		AnalyzeText();

	//	ReinitializeAllViews();
		FormatScreenText();
		UpdateAllViews(NULL);
	} else {
	//	ReinitializeAllViews();
	//	FormatScreenText();
		UpdateAllViews(NULL);
	}

	return TRUE;
}
