// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__FFCA2B88_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_)
#define AFX_MAINFRM_H__FFCA2B88_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();

protected:
	void OnSizeMainWindow(int cx, int cy);

public: // status bar
	void BeginProgress(LPCTSTR lpszMessage) { m_wndStatusBar.BeginProgress(lpszMessage); }
	void EndProgress() { m_wndStatusBar.EndProgress(); }
	void SetProgress(INT nPercent) { m_wndStatusBar.SetProgress(nPercent); }

	void SetCaretPositionInfo(INT nLine, INT nCol, INT nMax);
	void SetFileStatusInfo(INT nEncodingType, INT nFileFormat, BOOL bReadOnly);
	void SetMacroRecordingFlag(BOOL bRecording);
	void SetColumnModeFlag(BOOL bColumnMode);
	void SetOverwriteFlag(BOOL bOverwrite);

public: // file selection tab
	void InsertMDIFileTab(CMDIChildWnd * pChild);
	void DeleteMDIFileTab(CMDIChildWnd * pChild);

	void UpdateMDIFileTab(CMDIChildWnd * pChild);
	void SetActiveFileTab(CMDIChildWnd * pChild);

public: // get windows
	CWnd * GetFileWindow() { return & m_wndFileWindow; }
	CWnd * GetOutputWindow() { return & m_wndOutputWindow; }

public: // file window
	BOOL IsFileWindowVisible() { return m_wndFileWindow.IsWindowVisible(); }
	void ShowFileWindow(BOOL bShow) { ShowControlBar( & m_wndFileWindow, bShow, FALSE ); }

	void ShowFileWindowCategory(INT nCategory);
	void ToggleFileWindowCategory(INT nCategory);

public: // output window
	BOOL IsOutputWindowVisible() { return m_wndOutputWindow.IsWindowVisible(); }
	void ShowOutputWindow(BOOL bShow) { ShowControlBar( & m_wndOutputWindow, bShow, FALSE ); }

	void ApplyOutputWindowFont(BOOL bRedraw = TRUE) { m_wndOutputWindow.ApplyOutputFont(bRedraw); }
	void EnableOutputWindowInput(BOOL bEnable) { m_wndOutputWindow.EnableInputConsole(bEnable); }

	void SetOutputWindowOccupied(BOOL bOccupy) { m_wndOutputWindow.SetOccupied(bOccupy); }
	BOOL CanUseOutputWindow() { return m_wndOutputWindow.CanUseNow(); }
	void ClearOutputWindowContents() { m_wndOutputWindow.ClearAllTheContents(); }

	BOOL AddStringToOutputWindow(LPCTSTR lpszString) { return m_wndOutputWindow.AddStringToTheLast(lpszString); }
	BOOL ReplaceStringOfOutputWindow(LPCTSTR lpszString) { return m_wndOutputWindow.ReplaceTheLastString(lpszString); }
	BOOL GetStringOfOutputWindow(CString & szString) { return m_wndOutputWindow.GetTheLastString(szString); }

public: // child window
	INT  GetChildFrameCount() { return m_lstChildFrame.GetCount(); }
	CMDIChildWnd * GetNextChildFrame() { return m_wndFileTab.GetNextChildFrame(); }
	CMDIChildWnd * GetPrevChildFrame() { return m_wndFileTab.GetPrevChildFrame(); }

	CDocument * MDIGetActiveDocument();
	CView * MDIGetActiveView();

	void ActivateNextChildFrame();
	void ActivatePrevChildFrame();
	void ActivateChildFrameInZOrder(INT nIndex);
	void BringChildFrameToTop(CMDIChildWnd * pChild);

public: // others
	void OnUpdateFrameTitle(BOOL bAddToTitle);
	BOOL RefreshSyntaxTypeMenu();
	BOOL RefreshLoadUserToolsMenu();
	BOOL RefreshLoadUserMacrosMenu();

protected: // find menu item
	CMenu * FindParentMenuContaing(CMenu * pParentMenu, UINT nFindMenuID);

public: // window placement
	BOOL SaveWindowPlacement(LPCTSTR lpszProfileName);
	BOOL LoadWindowPlacement(LPCTSTR lpszProfileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: // Attributes
	CList<CMDIChildWnd *, CMDIChildWnd *> m_lstChildFrame;
	BOOL m_bSwitchingChildFrame;
	BOOL m_bPrintPreviewMode;

protected: // control bar embedded members
	CStatusBarEx	m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CMDIFileTab		m_wndFileTab;

	CFileWindow		m_wndFileWindow;
	COutputWindow	m_wndOutputWindow;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFileTab();
	afx_msg void OnUpdateViewFileTab(CCmdUI* pCmdUI);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnClose();
	afx_msg void OnViewDirectory();
	afx_msg void OnUpdateViewDirectory(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnViewProject();
	afx_msg void OnUpdateViewProject(CCmdUI* pCmdUI);
	afx_msg void OnWindowLastVisited();
	afx_msg void OnWindowNext();
	afx_msg void OnWindowPrev();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileWindowHide();
	afx_msg void OnFileWindowSync();
	afx_msg void OnOutputWindowHide();
	afx_msg void OnOutputWindowCopy();
	afx_msg void OnOutputWindowClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__FFCA2B88_F9C5_11D4_A6F1_0050CE184C9B__INCLUDED_)
