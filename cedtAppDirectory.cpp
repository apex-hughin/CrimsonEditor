#include "stdafx.h"
#include "cedtHeader.h"


void CCedtApp::OnDirectoryItemOpen() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->OpenDirectoryItem();
}

void CCedtApp::OnDirectoryItemExecute() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->ExecuteDirectoryItem();
}

void CCedtApp::OnDirectoryItemExplore() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->ExploreDirectoryItem();
}

void CCedtApp::OnDirectoryItemFind() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->FindInDirectoryItem();
}

void CCedtApp::OnDirectoryItemSetWorkdir() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->SetWorkdirDirectoryItem();

	CCedtApp::SaveUserConfiguration();
}

void CCedtApp::OnDirectoryItemMove() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->MoveDirectoryItem();
}

void CCedtApp::OnDirectoryItemCopy() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->CopyDirectoryItem();
}

void CCedtApp::OnDirectoryItemRename() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->RenameDirectoryItem();
}

void CCedtApp::OnDirectoryItemDelete() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->DeleteDirectoryItem();
}

void CCedtApp::OnDirectoryItemRefresh() 
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();
	pFileWindow->RefreshDirectoryItem();
}
