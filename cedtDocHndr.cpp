#include "stdafx.h"
#include "cedtHeader.h"
#include "DocumentSummary.h"


// ON UPDATE DOCUMENT SYNTAX TYPE
void CCedtDoc::OnUpdateDocuSyntaxType0 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  0); }
void CCedtDoc::OnUpdateDocuSyntaxType1 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  1); }
void CCedtDoc::OnUpdateDocuSyntaxType2 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  2); }
void CCedtDoc::OnUpdateDocuSyntaxType3 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  3); }
void CCedtDoc::OnUpdateDocuSyntaxType4 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  4); }
void CCedtDoc::OnUpdateDocuSyntaxType5 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  5); }
void CCedtDoc::OnUpdateDocuSyntaxType6 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  6); }
void CCedtDoc::OnUpdateDocuSyntaxType7 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  7); }
void CCedtDoc::OnUpdateDocuSyntaxType8 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  8); }
void CCedtDoc::OnUpdateDocuSyntaxType9 (CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI,  9); }
void CCedtDoc::OnUpdateDocuSyntaxType10(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 10); }
void CCedtDoc::OnUpdateDocuSyntaxType11(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 11); }
void CCedtDoc::OnUpdateDocuSyntaxType12(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 12); }
void CCedtDoc::OnUpdateDocuSyntaxType13(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 13); }
void CCedtDoc::OnUpdateDocuSyntaxType14(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 14); }
void CCedtDoc::OnUpdateDocuSyntaxType15(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 15); }
void CCedtDoc::OnUpdateDocuSyntaxType16(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 16); }
void CCedtDoc::OnUpdateDocuSyntaxType17(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 17); }
void CCedtDoc::OnUpdateDocuSyntaxType18(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 18); }
void CCedtDoc::OnUpdateDocuSyntaxType19(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 19); }
void CCedtDoc::OnUpdateDocuSyntaxType20(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 20); }
void CCedtDoc::OnUpdateDocuSyntaxType21(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 21); }
void CCedtDoc::OnUpdateDocuSyntaxType22(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 22); }
void CCedtDoc::OnUpdateDocuSyntaxType23(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 23); }
void CCedtDoc::OnUpdateDocuSyntaxType24(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 24); }
void CCedtDoc::OnUpdateDocuSyntaxType25(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 25); }
void CCedtDoc::OnUpdateDocuSyntaxType26(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 26); }
void CCedtDoc::OnUpdateDocuSyntaxType27(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 27); }
void CCedtDoc::OnUpdateDocuSyntaxType28(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 28); }
void CCedtDoc::OnUpdateDocuSyntaxType29(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 29); }
void CCedtDoc::OnUpdateDocuSyntaxType30(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 30); }
void CCedtDoc::OnUpdateDocuSyntaxType31(CCmdUI* pCmdUI) { OnUpdateDocuSyntaxType(pCmdUI, 31); }

void CCedtDoc::OnUpdateDocuSyntaxAuto(CCmdUI* pCmdUI) 
{
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->RefreshSyntaxTypeMenu();

	BOOL bFound = FALSE;
	for(INT i = 0; i < MAX_SYNTAX_TYPE; i++) {
		CSyntaxType & rSyntaxType = m_clsSyntaxTypes[i];
		if( rSyntaxType.m_szDescription.GetLength() && ! rSyntaxType.m_szLangSpecFile.CompareNoCase(m_szLangSpecFile)
			&& ! rSyntaxType.m_szKeywordsFile.CompareNoCase(m_szKeywordsFile) ) { bFound = TRUE; break; }
	}
	pCmdUI->SetRadio( m_szLangSpecFile.GetLength() && m_szKeywordsFile.GetLength() && ! bFound );
}

void CCedtDoc::OnUpdateDocuSyntaxText(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio( ! m_szLangSpecFile.GetLength() || ! m_szKeywordsFile.GetLength() );
}

void CCedtDoc::OnUpdateDocuSyntaxType(CCmdUI * pCmdUI, INT nSyntaxType)
{
	CSyntaxType & rSyntaxType = m_clsSyntaxTypes[nSyntaxType];
	pCmdUI->SetRadio( rSyntaxType.m_szDescription.GetLength() && ! rSyntaxType.m_szLangSpecFile.CompareNoCase(m_szLangSpecFile)
					  && ! rSyntaxType.m_szKeywordsFile.CompareNoCase(m_szKeywordsFile) );
}

// ON DOCUMENT SYNTAX TYPE
void CCedtDoc::OnDocuSyntaxType0 () { OnDocuSyntaxType( 0); }
void CCedtDoc::OnDocuSyntaxType1 () { OnDocuSyntaxType( 1); }
void CCedtDoc::OnDocuSyntaxType2 () { OnDocuSyntaxType( 2); }
void CCedtDoc::OnDocuSyntaxType3 () { OnDocuSyntaxType( 3); }
void CCedtDoc::OnDocuSyntaxType4 () { OnDocuSyntaxType( 4); }
void CCedtDoc::OnDocuSyntaxType5 () { OnDocuSyntaxType( 5); }
void CCedtDoc::OnDocuSyntaxType6 () { OnDocuSyntaxType( 6); }
void CCedtDoc::OnDocuSyntaxType7 () { OnDocuSyntaxType( 7); }
void CCedtDoc::OnDocuSyntaxType8 () { OnDocuSyntaxType( 8); }
void CCedtDoc::OnDocuSyntaxType9 () { OnDocuSyntaxType( 9); }
void CCedtDoc::OnDocuSyntaxType10() { OnDocuSyntaxType(10); }
void CCedtDoc::OnDocuSyntaxType11() { OnDocuSyntaxType(11); }
void CCedtDoc::OnDocuSyntaxType12() { OnDocuSyntaxType(12); }
void CCedtDoc::OnDocuSyntaxType13() { OnDocuSyntaxType(13); }
void CCedtDoc::OnDocuSyntaxType14() { OnDocuSyntaxType(14); }
void CCedtDoc::OnDocuSyntaxType15() { OnDocuSyntaxType(15); }
void CCedtDoc::OnDocuSyntaxType16() { OnDocuSyntaxType(16); }
void CCedtDoc::OnDocuSyntaxType17() { OnDocuSyntaxType(17); }
void CCedtDoc::OnDocuSyntaxType18() { OnDocuSyntaxType(18); }
void CCedtDoc::OnDocuSyntaxType19() { OnDocuSyntaxType(19); }
void CCedtDoc::OnDocuSyntaxType20() { OnDocuSyntaxType(20); }
void CCedtDoc::OnDocuSyntaxType21() { OnDocuSyntaxType(21); }
void CCedtDoc::OnDocuSyntaxType22() { OnDocuSyntaxType(22); }
void CCedtDoc::OnDocuSyntaxType23() { OnDocuSyntaxType(23); }
void CCedtDoc::OnDocuSyntaxType24() { OnDocuSyntaxType(24); }
void CCedtDoc::OnDocuSyntaxType25() { OnDocuSyntaxType(25); }
void CCedtDoc::OnDocuSyntaxType26() { OnDocuSyntaxType(26); }
void CCedtDoc::OnDocuSyntaxType27() { OnDocuSyntaxType(27); }
void CCedtDoc::OnDocuSyntaxType28() { OnDocuSyntaxType(28); }
void CCedtDoc::OnDocuSyntaxType29() { OnDocuSyntaxType(29); }
void CCedtDoc::OnDocuSyntaxType30() { OnDocuSyntaxType(30); }
void CCedtDoc::OnDocuSyntaxType31() { OnDocuSyntaxType(31); }

void CCedtDoc::OnDocuSyntaxAuto() 
{
	m_szLangSpecFile = ""; m_clsLangSpec.ResetContents(); 
	m_szKeywordsFile = ""; m_clsKeywords.ResetContents();
	m_bAutomaticSyntaxType = TRUE;

	if( DetectSyntaxType(GetPathName(), m_clsAnalyzedText.GetHead()) ) LoadSyntaxInformation();
	AnalyzeText();

	FormatScreenText();
	UpdateAllViews(NULL);
}

void CCedtDoc::OnDocuSyntaxText() 
{
	m_szLangSpecFile = ""; m_clsLangSpec.ResetContents(); 
	m_szKeywordsFile = ""; m_clsKeywords.ResetContents();
	m_bAutomaticSyntaxType = FALSE;

//	LoadSyntaxInformation();
	AnalyzeText();

	FormatScreenText();
	UpdateAllViews(NULL);
}

void CCedtDoc::OnDocuSyntaxType(INT nSyntaxType)
{
	CSyntaxType & rSyntaxType = m_clsSyntaxTypes[nSyntaxType];
	if( rSyntaxType.m_szDescription.GetLength() && rSyntaxType.m_szLangSpecFile.GetLength() && rSyntaxType.m_szKeywordsFile.GetLength() ) {
		m_szLangSpecFile = rSyntaxType.m_szLangSpecFile; m_clsLangSpec.ResetContents(); 
		m_szKeywordsFile = rSyntaxType.m_szKeywordsFile; m_clsKeywords.ResetContents();
		m_bAutomaticSyntaxType = FALSE;

		LoadSyntaxInformation();
		AnalyzeText();

		FormatScreenText();
		UpdateAllViews(NULL);
	}
}

// ON UPDATE DOCUMENT FILE FORMAT
void CCedtDoc::OnUpdateDocuFormatDos(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nFileFormat == FILE_FORMAT_DOS ); }
void CCedtDoc::OnUpdateDocuFormatUnix(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nFileFormat == FILE_FORMAT_UNIX ); }
void CCedtDoc::OnUpdateDocuFormatMac(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nFileFormat == FILE_FORMAT_MAC ); }

// ON DOCUMENT FILE FORMAT
void CCedtDoc::OnDocuFormatDos() { OnDocuFileFormat(FILE_FORMAT_DOS); }
void CCedtDoc::OnDocuFormatUnix() { OnDocuFileFormat(FILE_FORMAT_UNIX); }
void CCedtDoc::OnDocuFormatMac() { OnDocuFileFormat(FILE_FORMAT_MAC); }

void CCedtDoc::OnDocuFileFormat(INT nFileFormat)
{
	if( m_nFileFormat == nFileFormat ) return;
	m_nFileFormat = nFileFormat; 

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetFileStatusInfo( m_nEncodingType, m_nFileFormat, IsReadOnlyFile() );
}

// ON UPDATE DOCUMENT ENCODING TYPE
void CCedtDoc::OnUpdateDocuEncodingAscii(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nEncodingType == ENCODING_TYPE_ASCII ); }
void CCedtDoc::OnUpdateDocuEncodingUnicodeLE(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nEncodingType == ENCODING_TYPE_UNICODE_LE ); }
void CCedtDoc::OnUpdateDocuEncodingUnicodeBE(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nEncodingType == ENCODING_TYPE_UNICODE_BE ); }
void CCedtDoc::OnUpdateDocuEncodingUtf8WBOM(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nEncodingType == ENCODING_TYPE_UTF8_WBOM ); }
void CCedtDoc::OnUpdateDocuEncodingUtf8XBOM(CCmdUI* pCmdUI) { pCmdUI->SetRadio( m_nEncodingType == ENCODING_TYPE_UTF8_XBOM ); }

void CCedtDoc::OnDocuEncodingAscii() { OnDocuEncodingType(ENCODING_TYPE_ASCII); }
void CCedtDoc::OnDocuEncodingUnicodeLE() { OnDocuEncodingType(ENCODING_TYPE_UNICODE_LE); }
void CCedtDoc::OnDocuEncodingUnicodeBE() { OnDocuEncodingType(ENCODING_TYPE_UNICODE_BE); }
void CCedtDoc::OnDocuEncodingUtf8WBOM() { OnDocuEncodingType(ENCODING_TYPE_UTF8_WBOM); }
void CCedtDoc::OnDocuEncodingUtf8XBOM() { OnDocuEncodingType(ENCODING_TYPE_UTF8_XBOM); }

void CCedtDoc::OnDocuEncodingType(INT nEncodingType)
{
	if( m_nEncodingType == nEncodingType ) return;
	INT nResult = IDNO; // default - do not reload this document

	if( ! IsNewFileNotSaved() ) { // if it is a disk file then ask user to reload this document
		nResult = AfxMessageBox(IDS_MSG_ASK_RELOAD_DOCUMENT, MB_YESNOCANCEL | MB_ICONQUESTION);
		if( nResult == IDCANCEL ) return;
	}

	m_nEncodingType = nEncodingType;

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetFileStatusInfo( m_nEncodingType, m_nFileFormat, IsReadOnlyFile() );

	if( nResult == IDYES ) pMainFrame->PostMessage(WM_COMMAND, ID_FILE_RELOAD, 0L);
}


// ON DOCUMENT SUMMARY
void CCedtDoc::OnDocuSummary() 
{
	CDocumentSummary dlg;

	dlg.m_szPathName = GetTitle();

	dlg.m_szEncodingType = ENCODING_TYPE_DESCRIPTION_SHORT[m_nEncodingType];
	dlg.m_szFileFormat = FILE_FORMAT_DESCRIPTION_SHORT[m_nFileFormat];
	dlg.m_nFileSize = m_clsFileStatus.m_size;

	dlg.m_nLineCount = GetLineCount();
	dlg.m_nWordCount = GetWordCount();
	dlg.m_nByteCount = GetByteCount();

	dlg.m_bAttrReadOnly = m_dwFileAttribute & FILE_ATTRIBUTE_READONLY;
	dlg.m_bAttrHidden = m_dwFileAttribute & FILE_ATTRIBUTE_HIDDEN;
	dlg.m_bAttrSystem = m_dwFileAttribute & FILE_ATTRIBUTE_SYSTEM;

	dlg.m_szModDate = m_clsFileStatus.m_mtime.Format("%B %d, %Y   %I:%M:%S %p");

	dlg.DoModal();
}
