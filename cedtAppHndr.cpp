#include "stdafx.h"
#include "cedtHeader.h"

#include "FtpSettingsDialog.h"
#include "FindInFilesDialog.h"
#include "PrefDialog.h"
#include "AboutDialog.h"



void CCedtApp::OnUpdateFileMruFile1(CCmdUI* pCmdUI) 
{
	if( pCmdUI->m_pSubMenu != NULL ) return;
	CWinApp::OnUpdateRecentFileMenu( pCmdUI );
}

void CCedtApp::OnFileFtpSettings() 
{
	CFtpSettingsDialog dlg;
	dlg.SetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );

	if( dlg.DoModal() == IDOK ) {
		dlg.GetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );
		SaveFtpAccountInfo(m_szInstallDirectory + "\\cedt.ftp");
	}
}

void CCedtApp::OnSearchFindInFiles() 
{
	// static dialog box to remember last settings...
	static CFindInFilesDialog dlg;

	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pFrame );
	if( ! pFrame->CanUseOutputWindow() ) {
		AfxMessageBox(IDS_ERR_OUTPUT_WINDOW_OCCUPIED, MB_OK | MB_ICONSTOP); return;
	}

	CCedtView * pView = (CCedtView *)pFrame->MDIGetActiveView();
	CCedtDoc * pDoc = (pView) ? (CCedtDoc *)pView->GetDocument() : NULL;

	if( pDoc && pView ) {
		dlg.m_szFindString = pView->GetCurrentWord();
		if( pView->IsSelected() && ! pView->GetSelectedLineCount() ) dlg.m_szFindString = pView->GetSelectedString();

		if( ! dlg.m_szFolder.GetLength() ) {
			CString szPathName = pDoc->GetPathName();
			if( szPathName.GetLength() ) dlg.m_szFolder = GetFileDirectory( szPathName );
		}
	} else {
		if( ! dlg.m_szFolder.GetLength() ) {
			TCHAR szTemp[MAX_PATH]; GetCurrentDirectory(MAX_PATH, szTemp);
			dlg.m_szFolder = szTemp;
		}
	}

	if( dlg.DoModal() != IDOK ) return;

	CString szFindString = dlg.m_szFindString;
	CString szFileType = dlg.m_szFileType;
	CString szFolder = dlg.m_szFolder;

	BOOL bLookSubfolders = dlg.m_bLookSubfolders;
	UINT nOptions = COMPOSE_SEARCH_OPTION( dlg.m_bWholeWord, dlg.m_bMatchCase, dlg.m_bRegularExpression);

	CRegExp clsTestRegExp; // compile regular expression for test
	if( HAS_REG_EXP(nOptions) && ! clsTestRegExp.RegComp(szFindString) ) {
		CString szMessage; szMessage.Format(IDS_ERR_REG_COMP_FAILED, szFindString);
		AfxMessageBox(szMessage); return; // test failed
	}

	DoFindInFiles(szFindString, szFileType, szFolder, bLookSubfolders, nOptions); 
}

void CCedtApp::OnViewColumnMarkers() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_VISUAL);
}

void CCedtApp::OnViewSetScreenFonts() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_FONTS, 0);
}

void CCedtApp::OnViewSetPrinterFonts() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_FONTS, 1);
}

void CCedtApp::OnViewSetLineSpacing() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_VISUAL);
}

void CCedtApp::OnViewSetTabSize() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_GENERAL);
}

void CCedtApp::OnViewSetColors() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_COLORS);
}

void CCedtApp::OnDocuSyntaxCustomize() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_SYNTAX);
}

void CCedtApp::OnToolPreferences() 
{
	CPreferenceDialog dlg;
	dlg.DoModal();
}

void CCedtApp::OnUpdateCommandUserFile0(CCmdUI* pCmdUI) 
{
	CCedtView::RefreshUserCommandFilePathForMenu();
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->RefreshLoadUserToolsMenu();
	pCmdUI->Enable( CCedtView::m_szUserCommandFilePath[0].GetLength() );
}

void CCedtApp::OnCommandUserFile0() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[0] ); }
void CCedtApp::OnCommandUserFile1() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[1] ); }
void CCedtApp::OnCommandUserFile2() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[2] ); }
void CCedtApp::OnCommandUserFile3() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[3] ); }
void CCedtApp::OnCommandUserFile4() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[4] ); }
void CCedtApp::OnCommandUserFile5() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[5] ); }
void CCedtApp::OnCommandUserFile6() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[6] ); }
void CCedtApp::OnCommandUserFile7() { LoadUserCommands( CCedtView::m_szUserCommandFilePath[7] ); }

void CCedtApp::OnCommandConfigure() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_COMMANDS);
}

void CCedtApp::OnUpdateMacroUserFile0(CCmdUI* pCmdUI) 
{
	CCedtView::RefreshMacroBufferFilePathForMenu();
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->RefreshLoadUserMacrosMenu();
	pCmdUI->Enable( CCedtView::m_szMacroBufferFilePath[0].GetLength() );
}

void CCedtApp::OnMacroUserFile0() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[0] ); }
void CCedtApp::OnMacroUserFile1() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[1] ); }
void CCedtApp::OnMacroUserFile2() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[2] ); }
void CCedtApp::OnMacroUserFile3() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[3] ); }
void CCedtApp::OnMacroUserFile4() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[4] ); }
void CCedtApp::OnMacroUserFile5() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[5] ); }
void CCedtApp::OnMacroUserFile6() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[6] ); }
void CCedtApp::OnMacroUserFile7() { LoadMacroBuffers( CCedtView::m_szMacroBufferFilePath[7] ); }

void CCedtApp::OnMacroConfigure() 
{
	CPreferenceDialog dlg;
	dlg.DoModal(PREF_CATEGORY_MACROS);
}

void CCedtApp::OnHelpTopics() 
{
	GotoURL(m_szInstallDirectory + "\\docs\\index.html", SW_SHOW);
}

void CCedtApp::OnHelpVisitHomepage() 
{
	GotoURL(STRING_HOMEPAGEURL, SW_SHOW);
}

void CCedtApp::OnHelpFeedback() 
{
	GotoURL(STRING_EMAILADDRESS, SW_SHOW);
}

void CCedtApp::OnHelpSponsoring() 
{
	GotoURL(STRING_SPONSORURL, SW_SHOW);
}

void CCedtApp::OnAppAbout() 
{
	CAboutDialog dlg;
	dlg.DoModal();
}
