#include "stdafx.h"
#include "cedtHeader.h"
#include "PrefDialog.h"
#include "FolderDialog.h"


void CPreferenceDialog::InitFilePage()
{
}

void CPreferenceDialog::SizeFilePage()
{
	INT nPosY;

	nPosY  = 26; m_stcWorkingDirectoryBox.MoveWindow(170, nPosY, 360, 55);
	nPosY += 10; m_stcWorkingDirectory.MoveWindow(180, nPosY, 320, 14);
	nPosY += 20; m_edtWorkingDirectory.MoveWindow(180, nPosY-3, 320, 18);	m_btnWorkingDirectory.MoveWindow(505, nPosY-3, 20, 18);

	nPosY += 35; m_stcRemoteDirectoryBox.MoveWindow(170, nPosY, 360, 75);
	nPosY += 10; m_stcRemoteDirectory.MoveWindow(180, nPosY, 320, 14);
	nPosY += 20; m_edtRemoteDirectory.MoveWindow(180, nPosY-3, 320, 18);	m_btnRemoteDirectory.MoveWindow(505, nPosY-3, 20, 18);
	nPosY += 20; m_stcRemoteDirectoryDesc.MoveWindow(180, nPosY, 320, 14);

	nPosY += 35; m_stcFileSettingsBox.MoveWindow(170, nPosY, 360, 145);
	nPosY += 10; m_chkCreateNewDocument.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_chkReloadWorkingFiles.MoveWindow(180, nPosY, 300, 14);

	nPosY += 25; m_chkConvertTabsToSpaces.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_chkRemoveTrailingSpaces.MoveWindow(180, nPosY, 300, 14);

	nPosY += 25; m_chkSaveFilesUnix.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_chkSaveRemoteFilesUnix.MoveWindow(180, nPosY, 300, 14);
}

void CPreferenceDialog::ShowFilePage()
{
	INT nCmdShow = (m_nActiveCategory == PREF_CATEGORY_FILE) ? SW_SHOW : SW_HIDE;

	m_stcWorkingDirectoryBox.ShowWindow(nCmdShow);
	m_stcWorkingDirectory.ShowWindow(nCmdShow);	
	m_edtWorkingDirectory.ShowWindow(nCmdShow);		m_btnWorkingDirectory.ShowWindow(nCmdShow);

	m_stcRemoteDirectoryBox.ShowWindow(nCmdShow);
	m_stcRemoteDirectory.ShowWindow(nCmdShow);
	m_edtRemoteDirectory.ShowWindow(nCmdShow);		m_btnRemoteDirectory.ShowWindow(nCmdShow);
	m_stcRemoteDirectoryDesc.ShowWindow(nCmdShow);

	m_stcFileSettingsBox.ShowWindow(nCmdShow);
	m_chkCreateNewDocument.ShowWindow(nCmdShow);
	m_chkReloadWorkingFiles.ShowWindow(nCmdShow);

	m_chkConvertTabsToSpaces.ShowWindow(nCmdShow);
	m_chkRemoveTrailingSpaces.ShowWindow(nCmdShow);

	m_chkSaveFilesUnix.ShowWindow(nCmdShow);
	m_chkSaveRemoteFilesUnix.ShowWindow(nCmdShow);
}


BOOL CPreferenceDialog::LoadFileSettings()
{
	m_edtWorkingDirectory.SetWindowText( CCedtApp::m_szInitialWorkingDirectory );
	m_edtRemoteDirectory.SetWindowText( CCedtApp::m_szRemoteBackupDirectory );

	m_chkCreateNewDocument.SetCheck( CCedtApp::m_bCreateNewDocumentOnStartup ? 1 : 0 );
	m_chkReloadWorkingFiles.SetCheck( CCedtApp::m_bReloadWorkingFilesOnStartup ? 1 : 0 );

	m_chkConvertTabsToSpaces.SetCheck( CCedtDoc::m_bConvertTabsToSpacesBeforeSaving ? 1 : 0 );
	m_chkRemoveTrailingSpaces.SetCheck( CCedtDoc::m_bRemoveTrailingSpacesBeforeSaving ? 1 : 0 );

	m_chkSaveFilesUnix.SetCheck( CCedtDoc::m_bSaveFilesInUnixFormat ? 1 : 0 );
	m_chkSaveRemoteFilesUnix.SetCheck( CCedtDoc::m_bSaveRemoteFilesInUnixFormat ? 1 : 0 );

	return TRUE;
}

BOOL CPreferenceDialog::SaveFileSettings()
{
	m_edtWorkingDirectory.GetWindowText( CCedtApp::m_szInitialWorkingDirectory );
	m_edtRemoteDirectory.GetWindowText( CCedtApp::m_szRemoteBackupDirectory );

	CCedtApp::m_bCreateNewDocumentOnStartup = m_chkCreateNewDocument.GetCheck();
	CCedtApp::m_bReloadWorkingFilesOnStartup = m_chkReloadWorkingFiles.GetCheck();

	CCedtDoc::m_bConvertTabsToSpacesBeforeSaving = m_chkConvertTabsToSpaces.GetCheck();
	CCedtDoc::m_bRemoveTrailingSpacesBeforeSaving = m_chkRemoveTrailingSpaces.GetCheck();

	CCedtDoc::m_bSaveFilesInUnixFormat = m_chkSaveFilesUnix.GetCheck();
	CCedtDoc::m_bSaveRemoteFilesInUnixFormat = m_chkSaveRemoteFilesUnix.GetCheck();

	return TRUE;
}

void CPreferenceDialog::OnWorkingDirectoryBrowse() 
{
	CString szDirectory; m_edtWorkingDirectory.GetWindowText( szDirectory );
	CString szText( (LPCTSTR)IDS_CHOOSE_DIRECTORY );
	CFolderDialog dlg(szText, szDirectory, NULL, this);
	if( dlg.DoModal() != IDOK ) return;
	m_edtWorkingDirectory.SetWindowText( dlg.GetPathName() );
}


void CPreferenceDialog::OnRemoteDirectoryBrowse() 
{
	CString szDirectory; m_edtRemoteDirectory.GetWindowText( szDirectory );
	CString szText( (LPCTSTR)IDS_CHOOSE_DIRECTORY );
	CFolderDialog dlg(szText, szDirectory, NULL, this);
	if( dlg.DoModal() != IDOK ) return;
	m_edtRemoteDirectory.SetWindowText( dlg.GetPathName() );
}
