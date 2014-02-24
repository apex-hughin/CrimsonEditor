#include "stdafx.h"
#include "cedtHeader.h"
#include "OpenRemoteDialog.h"
#include "FtpPasswordDialog.h"
#include "FtpTransferDialog.h"


void CCedtApp::OnFileOpen() 
{
	if( ! m_bPostOpenDocument ) { // normal file open command
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
		CString szFilter = GetComposedFileFilter();
		CFileDialog dlg(TRUE, NULL, NULL, dwFlags, szFilter);

		TCHAR szCurrentDirectory[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

		TCHAR szBuffer[4096]; memset(szBuffer, 0x00, sizeof(szBuffer));
		dlg.m_ofn.lpstrFile = szBuffer; dlg.m_ofn.nMaxFile = 4096; 
		dlg.m_ofn.lpstrInitialDir = szCurrentDirectory;
		dlg.m_ofn.nFilterIndex = GetFilterIndexDialog() + 1;
		if( dlg.DoModal() != IDOK ) return;

		SetFilterIndexDialog(dlg.m_ofn.nFilterIndex - 1);

		POSITION pos = dlg.GetStartPosition();
		while( pos ) OpenDocumentFile( dlg.GetNextPathName( pos ), 0, NULL );
	} else {
		OpenDocumentFile( m_szPostOpenPathName, m_nPostOpenLineNum, NULL );
		m_bPostOpenDocument = FALSE;
	}
}

void CCedtApp::OnFileOpenTemplate() 
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CString szFilter = GetComposedFileFilter();
	CFileDialog dlg(TRUE, NULL, NULL, dwFlags, szFilter);

	CString szTitle; szTitle.LoadString(IDS_DLG_OPEN_TEMPLATE);
	CString szInitialDirectory = m_szInstallDirectory + "\\template";

	TCHAR szCurrentDirectory[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

	dlg.m_ofn.lpstrTitle = szTitle; dlg.m_ofn.lpstrInitialDir = szInitialDirectory;
	dlg.m_ofn.nFilterIndex = GetFilterIndexDialog() + 1;
	if( dlg.DoModal() != IDOK ) return;

	SetFilterIndexDialog(dlg.m_ofn.nFilterIndex - 1);
	SetCurrentDirectory( szCurrentDirectory );

	m_szOpenTemplatePathName = dlg.GetPathName();
	m_bOpenTemplate = TRUE;

	CWnd * pWnd = AfxGetMainWnd();
	pWnd->PostMessage( WM_COMMAND, ID_FILE_NEW );
}

void CCedtApp::OnFileOpenRemote() 
{
	if( ! m_bPostOpenDocument ) { // normal open remote command
		static nFtpAccount = 0;
		CString szFilter = GetComposedFileFilter();
		COpenRemoteDialog dlg(TRUE, NULL, szFilter);

		dlg.SetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );
		dlg.SetCurrentFtpAccount( nFtpAccount );
		dlg.SetCurrentFilterIndex( GetFilterIndexDialog() );
		INT nResponse = dlg.DoModal();

		dlg.GetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );
		SaveFtpAccountInfo(m_szInstallDirectory + "\\cedt.ftp");
		if( nResponse != IDOK ) return;

		nFtpAccount = dlg.GetCurrentFtpAccount();
		SetFilterIndexDialog( dlg.GetCurrentFilterIndex() );

		POSITION pos = dlg.GetFirstFileInfoPosition();
		while( pos ) {
			CString szFileInfo = dlg.GetNextFileInfo( pos );
			INT nFound = szFileInfo.Find('\n'); ASSERT(nFound >= 0);

			CString szPathName = szFileInfo.Mid(0, nFound);
			DWORD dwFileSize = atol( szFileInfo.Mid(nFound+1) );
			OpenRemoteDocumentFile( nFtpAccount, szPathName, dwFileSize, 0, NULL );
		}
	} else {
		OpenRemoteDocumentFile( m_nPostOpenFtpAccount, m_szPostOpenPathName, m_dwPostOpenFileSize, m_nPostOpenLineNum, NULL );
		m_bPostOpenDocument = FALSE;
	}
}

void CCedtApp::OnFileCloseAll() 
{
	if( SaveAllModified() ) CloseAllDocuments(FALSE);
}


BOOL CCedtApp::ReloadLastWorkingFiles()
{
	if( ! m_szPrevWorkspacePathName.GetLength() ) return FALSE;
	CString szExtension = GetFileExtension( m_szPrevWorkspacePathName );

	if( ! szExtension.CompareNoCase(".prj") ) {
		// activate project tab if file window is visible
		CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
		if( pFrame->IsFileWindowVisible() ) pFrame->ShowFileWindowCategory( FILE_WINDOW_PROJECT );

		return OpenProjectWorkspace(m_szPrevWorkspacePathName);
	} else if( ! szExtension.CompareNoCase(".wks") ) {
		return OpenRegularWorkspace(m_szPrevWorkspacePathName);
	} else return FALSE;
}


BOOL CCedtApp::CreateNewDocumentIfNothingLoaded()
{
	POSITION posDoc = GetFirstDocPosition();
	if( ! posDoc ) {
		CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->PostMessage( WM_COMMAND, ID_FILE_NEW, (LPARAM)pFrame->m_hWnd );
	}
	return TRUE;
}


BOOL CCedtApp::PromptSaveFileName(CString & szPathName)
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CString szFilter = GetComposedFileFilter(); 
	CFileDialog dlg(FALSE, NULL, szPathName, dwFlags, szFilter);

	TCHAR szCurrentDirectory[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

	dlg.m_ofn.lpstrInitialDir = szCurrentDirectory;
	dlg.m_ofn.nFilterIndex = GetFilterIndexDialog() + 1;
	if( dlg.DoModal() != IDOK ) return FALSE;

	SetFilterIndexDialog(dlg.m_ofn.nFilterIndex - 1);
	szPathName = dlg.GetPathName();

	// check to append default extension
	CString szExtension = GetFileExtension(szPathName);
	if( ! szExtension.GetLength() ) {
		szExtension = GetDefaultFileExtension(); 
		if( szExtension.GetLength() ) szPathName += CString(".") + szExtension;
	}

	return TRUE;
}

BOOL CCedtApp::PromptSaveRemoteFileName(INT & nFtpAccount, CString & szPathName)
{
	CString szFileName = GetFileName(szPathName);
	CString szFilter = GetComposedFileFilter();
	COpenRemoteDialog dlg(FALSE, szFileName, szFilter);

	dlg.SetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );
	dlg.SetCurrentFtpAccount( nFtpAccount );
	dlg.SetCurrentFilterIndex( GetFilterIndexDialog() );
	INT nResponse = dlg.DoModal();

	dlg.GetFtpAccounts( MAX_FTP_ACCOUNT, m_clsFtpAccounts );
	SaveFtpAccountInfo(m_szInstallDirectory + "\\cedt.ftp");
	if( nResponse != IDOK ) return FALSE;

	SetFilterIndexDialog( dlg.GetCurrentFilterIndex() );
	nFtpAccount = dlg.GetCurrentFtpAccount();
	szPathName = dlg.GetPathName();

	// check to append default extension
	CString szExtension = GetFileExtension(szPathName);
	if( ! szExtension.GetLength() ) {
		szExtension = GetDefaultFileExtension();
		if( szExtension.GetLength() ) szPathName += CString(".") + szExtension;
	}

	return TRUE;
}


CDocument * CCedtApp::OpenDocumentFile(LPCTSTR lpszPathName, INT nLineNum, WINDOWPLACEMENT * lpwndpl)
{
	CCedtDoc::m_nCurrentFtpAccount = -1;
	CCedtDoc::m_szCurrentRemotePathName = "";

	// check if the document is already open
	CCedtDoc * pDocExist = (CCedtDoc *)GetDocumentFromLocalPathName(lpszPathName);

	CCedtView::m_lpWindowPlacement = lpwndpl; // this will be used in CCedtView::OnInitialUpdate();
	CCedtDoc * pDoc = (CCedtDoc *)CWinApp::OpenDocumentFile(lpszPathName);

	// restore CCedtView::m_lpWindowPlacement
	CCedtView::m_lpWindowPlacement = NULL;

	if( pDocExist ) ASSERT( pDocExist == pDoc );
	if( pDoc && nLineNum ) pDoc->GoToLineNumber( nLineNum );

	if( pDoc ) {
		CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->UpdateWindow();

		CCedtView * pView = (CCedtView *)pDoc->GetFirstView();
		pView->SetFocus();

		if( pDoc->HaveAnyOverflowLine() ) {
			CString szMessage; szMessage.Format(IDS_MSG_HAVE_LINE_OVERFLOW, MAX_STRING_SIZE);
			AfxMessageBox( szMessage, MB_OK | MB_ICONINFORMATION );
		}
	}

	return pDoc;
}

CDocument * CCedtApp::OpenRemoteDocumentFile(INT nAccount, LPCTSTR lpszPathName, DWORD dwFileSize, INT nLineNum, WINDOWPLACEMENT * lpwndpl)
{
	CString szLocalPath = m_szRemoteBackupDirectory + "\\";
	if( szLocalPath.GetLength() < 2 ) szLocalPath = m_szInstallDirectory + "\\remote\\";
	szLocalPath += m_clsFtpAccounts[nAccount].GetShortAccountName();
	szLocalPath += RemotePathToLocalPath(lpszPathName);
	if( ! TouchFile(szLocalPath) ) {
		CString szMessage; szMessage.Format(IDS_ERR_OPEN_REMOTE_BACKUP, szLocalPath);
		AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP); return NULL;
	}

	CCedtDoc::m_nCurrentFtpAccount = nAccount;
	CCedtDoc::m_szCurrentRemotePathName = lpszPathName;

	// check if the document is already open
	CCedtDoc * pDocExist = (CCedtDoc *)GetDocumentFromLocalPathName(szLocalPath);

	if( ! pDocExist ) {
		CFtpAccount & rFtpAccount = m_clsFtpAccounts[nAccount];
		if( ! rFtpAccount.IsValid() ) return NULL;

		// if account password is not given, ask user
		if( ! rFtpAccount.m_bSavePassword && ! rFtpAccount.m_bPasswordVerified ) {
			CFtpPasswordDialog dlg; dlg.m_szAccountInfo = rFtpAccount.GetShortAccountName();
			if( dlg.DoModal() != IDOK ) return NULL;
			rFtpAccount.m_szPassword = dlg.m_szPassword;
		}

		// download the remote file
		CFtpTransferDialog dlg(TRUE, rFtpAccount, lpszPathName, szLocalPath, dwFileSize);

		if( dlg.DoModal() != IDOK ) { // operation is canceled by the user
			CString szMessage; szMessage.Format(IDS_ERR_OPEN_REMOTE_CANCEL);
			AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP); return NULL;
		} else if( ! dlg.IsTransferCompleted() ) {
			CString szMessage; szMessage.Format(IDS_ERR_OPEN_REMOTE_FILE, lpszPathName);
			AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP); return NULL;
		}

		// operation successful, password verified
		if( ! rFtpAccount.m_bSavePassword ) rFtpAccount.m_bPasswordVerified = TRUE;
	}

	CCedtView::m_lpWindowPlacement = lpwndpl; // this will be used in CCedtView::OnInitialUpdate();
	CCedtDoc * pDoc = (CCedtDoc *)CWinApp::OpenDocumentFile(szLocalPath);

	// restore CCedtView::m_lpWindowPlacement
	CCedtView::m_lpWindowPlacement = NULL;

	if( pDocExist ) ASSERT( pDocExist == pDoc );
	if( pDoc && nLineNum ) pDoc->GoToLineNumber( nLineNum );

	if( pDoc ) {
		CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->UpdateWindow();

		CCedtView * pView = (CCedtView *)pDoc->GetFirstView();
		pView->SetFocus();

		if( pDoc->HaveAnyOverflowLine() ) {
			CString szMessage; szMessage.Format(IDS_MSG_HAVE_LINE_OVERFLOW, MAX_STRING_SIZE);
			AfxMessageBox(szMessage, MB_OK | MB_ICONINFORMATION );
		}
	}

	return pDoc;
}

BOOL CCedtApp::PostOpenDocumentFile(LPCTSTR lpszPathName, INT nLineNum)
{
	m_bPostOpenDocument = TRUE;
	m_szPostOpenPathName = lpszPathName;
	m_nPostOpenLineNum = nLineNum;

	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	return pFrame->PostMessage(WM_COMMAND, ID_FILE_OPEN, (LPARAM)pFrame->m_hWnd);
}

BOOL CCedtApp::PostOpenRemoteDocumentFile(INT nFtpAccount, LPCTSTR lpszPathName, DWORD dwFileSize, INT nLineNum)
{
	m_bPostOpenDocument = TRUE;
	m_nPostOpenFtpAccount = nFtpAccount;
	m_szPostOpenPathName = lpszPathName;
	m_dwPostOpenFileSize = dwFileSize;
	m_nPostOpenLineNum = nLineNum;

	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	return pFrame->PostMessage(WM_COMMAND, ID_FILE_OPEN_REMOTE, (LPARAM)pFrame->m_hWnd);
}

BOOL CCedtApp::CheckIfFilesModifiedOutside()
{
	BOOL bModified = FALSE;

	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		if( ! pDoc->IsNewFileNotSaved() && pDoc->IsModifiedOutside() ) {
			CString message; message.Format(IDS_MSG_FILE_MODIFIED_OUTSIDE, pDoc->GetPathName());
			INT nResult = AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION);

			if( nResult == IDYES ) pDoc->FileReload( pDoc->m_nEncodingType );
			else pDoc->UpdateFileStatus();

			bModified = TRUE;
		}
	}

	return bModified;
}

CDocument * CCedtApp::GetDocumentFromLocalPathName(LPCTSTR lpszPathName)
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		if( ! pDoc->IsNewFileNotSaved() ) {
			CString szPathName = pDoc->GetPathName();
			if( szPathName.CompareNoCase(lpszPathName) == 0 ) return pDoc;
		}
	}

	return NULL;
}
