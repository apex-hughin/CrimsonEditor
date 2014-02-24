#include "stdafx.h"
#include "cedtHeader.h"


void CMainFrame::OnViewFileTab() 
{
	BOOL bVisible = m_wndFileTab.IsWindowVisible();
	ShowControlBar( & m_wndFileTab, ! bVisible, FALSE );
}

void CMainFrame::OnUpdateViewFileTab(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndFileTab.IsWindowVisible();
	pCmdUI->SetCheck( bVisible );
}

void CMainFrame::OnViewDirectory() 
{
	ToggleFileWindowCategory( FILE_WINDOW_DIRECTORY );
}

void CMainFrame::OnUpdateViewDirectory(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndFileWindow.IsWindowVisible();
	INT nCategory = m_wndFileWindow.GetActiveCategory();
	pCmdUI->SetCheck( bVisible && (nCategory == FILE_WINDOW_DIRECTORY) );
}

void CMainFrame::OnViewProject() 
{
	ToggleFileWindowCategory( FILE_WINDOW_PROJECT );
}

void CMainFrame::OnUpdateViewProject(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndFileWindow.IsWindowVisible();
	INT nCategory = m_wndFileWindow.GetActiveCategory();
	pCmdUI->SetCheck( bVisible && (nCategory == FILE_WINDOW_PROJECT) );
}

void CMainFrame::OnViewOutput() 
{
	BOOL bVisible = m_wndOutputWindow.IsWindowVisible();
	ShowControlBar( & m_wndOutputWindow, ! bVisible, FALSE );
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndOutputWindow.IsWindowVisible();
	pCmdUI->SetCheck( bVisible );
}

void CMainFrame::OnWindowLastVisited() 
{
	m_bSwitchingChildFrame = TRUE;
	ActivateChildFrameInZOrder(1);
	m_bSwitchingChildFrame = FALSE;
	BringChildFrameToTop(NULL);
}

void CMainFrame::OnWindowNext() 
{
	m_bSwitchingChildFrame = TRUE;
	ActivateNextChildFrame();
	m_bSwitchingChildFrame = FALSE;
	BringChildFrameToTop(NULL);
}

void CMainFrame::OnWindowPrev() 
{
	m_bSwitchingChildFrame = TRUE;
	ActivatePrevChildFrame();
	m_bSwitchingChildFrame = FALSE;
	BringChildFrameToTop(NULL);
}

void CMainFrame::OnFileWindowHide() 
{
	ShowControlBar( & m_wndFileWindow, FALSE, FALSE );
}

void CMainFrame::OnFileWindowSync() 
{
	CCedtDoc * pDoc = (CCedtDoc *)MDIGetActiveDocument(); if( ! pDoc ) return;

	if( ! pDoc->IsRemoteFile() && ! pDoc->IsNewFileNotSaved() ) {
		CString szPathName = pDoc->GetPathName();
		m_wndFileWindow.SetBrowsingDirectory( szPathName );
	}
}

void CMainFrame::OnOutputWindowHide() 
{
	ShowControlBar( & m_wndOutputWindow, FALSE, FALSE );
}

void CMainFrame::OnOutputWindowCopy() 
{
	m_wndOutputWindow.CopyAllTheContents();
}

void CMainFrame::OnOutputWindowClear() 
{
	m_wndOutputWindow.ClearAllTheContents();
}
