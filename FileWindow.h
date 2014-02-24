#if !defined(AFX_FILEWINDOW_H__EF837327_15B9_11D5_A6F1_0050CE184C9B__INCLUDED_)
#define AFX_FILEWINDOW_H__EF837327_15B9_11D5_A6F1_0050CE184C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream.h>
#include "SizeCBar.h"


#define FILE_WINDOW_DIRECTORY		0x00
#define FILE_WINDOW_REMOTE			0x02
#define FILE_WINDOW_PROJECT			0x01

#define PROJECT_ITEM_PROJECT		0x00
#define PROJECT_ITEM_CATEGORY		0x01
#define PROJECT_ITEM_LOCAL_FILE		0x02
#define PROJECT_ITEM_REMOTE_FILE	0x03


typedef void (* CALLBACK_SELCHANGE)(INT);

typedef struct _PROJECTITEMINFO {
	INT nItemType;
	INT nFtpAccount;
	CString szPathName;
	LONG nFileSize;
} PROJECTITEMINFO, * LPPROJECTITEMINFO;


class CFileWindow : public CSizingControlBar
{
public: // Construction
	CFileWindow();
	virtual ~CFileWindow();

protected: // Resources & Controls
	TBBUTTON m_tbiToolbarDirectory[10];
	TBBUTTON m_tbiToolbarProject[10];
	TBBUTTON m_tbiWinButtons[10];

	CImageList m_imgToolbarDirectory;
	CImageList m_imgToolbarProject;
	CImageList m_imgWinButtons;

	CToolBarCtrl m_btnToolbarDirectory;
	CToolBarCtrl m_btnToolbarProject;
	CToolBarCtrl m_btnWinButtons;

	CImageList m_imgCategoryTab;
	CImageList m_imgDirectoryTree;
	CImageList m_imgProjectTree;

	CFont m_fontControl;
	CTabCtrl m_tabCategory;

	CComboBox m_cmbLocalDrive;
	CTreeCtrl m_treDirectoryTree;
	CComboBox m_cmbFileFilter;

	CComboBox m_cmbFtpAccount;
	CTreeCtrl m_treRemoteTree;

	CTreeCtrl m_treProjectTree;

	BOOL m_bLabelEditing;
	CString m_szPrevDriveName;

	CStringArray m_arrFilterDescription, m_arrFilterExtensions;
	CALLBACK_SELCHANGE m_fcnCallbackSelchangeFileFilter;

public:
	INT  GetActiveCategory();
	void SetActiveCategory(INT nSelect);

protected:
	void InitAllFileWindowControls();
	void ResizeAllFileWindowControls(INT cx, INT cy);
	void ShowProperFileWindowControls(INT nSelect);

	void ShowOrHideCommonControls(INT nSelect);
	void ShowOrHideDirectoryControls(INT nSelect);
	void ShowOrHideRemoteControls(INT nSelect);
	void ShowOrHideProjectControls(INT nSelect);

protected: // initialize directory image list
	BOOL InitDirectoryImageList( CImageList & imglst );


public: // *** FileWindowDirectory.cpp ***
	BOOL InitLocalDriveList(LPCTSTR lpszInitialDriveName);
	BOOL InitFileFilterList(LPCTSTR lpszComposedFilter, INT nInitialFilter, CALLBACK_SELCHANGE fcnCallback = NULL);

	BOOL GetBrowsingDirectory(CString & szDirectory);
	BOOL SetBrowsingDirectory(LPCTSTR lpszDirectory);

	BOOL IsSelectedDirectoryItemRoot();
	CString GetSelectedDirectoryItemText();

	BOOL OpenDirectoryItem();
	BOOL ExecuteDirectoryItem();
	BOOL ExploreDirectoryItem();
	BOOL FindInDirectoryItem();

	BOOL MoveDirectoryItem();
	BOOL CopyDirectoryItem();
	BOOL RenameDirectoryItem();
	BOOL DeleteDirectoryItem();

	BOOL SetWorkdirDirectoryItem();
	BOOL RefreshDirectoryItem();

protected: // inner functions
	BOOL SelectLocalDrive(LPCTSTR lpszDriveName);
	CString GetActiveLocalDriveName();

	INT GetActiveFileFilterIndex();
	CString GetActiveFileFilterString();

	BOOL DoesDirectoryHaveChildren(LPCTSTR lpszPath);
	CString GetDirectoryItemPathName(HTREEITEM hItem);

	BOOL RefreshDirectoryTree(LPCTSTR lpszPath);
	BOOL AskDestinationDirectory(CString & szDirectory);

	HTREEITEM GetSelectedDirectoryItem() { return m_treDirectoryTree.GetSelectedItem(); }
	HTREEITEM GetClickedDirectoryItem();

	HTREEITEM InsertDirectoryItem(HTREEITEM hParent, LPCTSTR lpszPath);
	HTREEITEM RemoveChildDirectoryItems(HTREEITEM hParent);
	HTREEITEM FindChildDirectoryItem(HTREEITEM hParent, LPCTSTR lpszText);

private:
	void DisplayDirectoryDrive(LPCTSTR lpszDrive);
	void DisplayDirectoryPath(HTREEITEM hParent, LPCTSTR lpszPath);

protected: // action functions
	BOOL OpenDirectoryContextMenu(HTREEITEM hItem);

	BOOL SetAsCurrentDirectory(HTREEITEM hItem);
	BOOL SetAsWorkingDirectory(HTREEITEM hItem);
	BOOL ShowDirectoryItemProperties(HTREEITEM hItem);

	BOOL OpenDirectoryItem(HTREEITEM hItem);
	BOOL ExecuteDirectoryItem(HTREEITEM hItem);
	BOOL ExploreDirectoryItem(HTREEITEM hItem);
	BOOL FindInDirectoryItem(HTREEITEM hItem);

	BOOL MoveToDirectoryItem(HTREEITEM hItem, LPCTSTR lpszDestination);
	BOOL CopyToDirectoryItem(HTREEITEM hItem, LPCTSTR lpszDestination);
	BOOL RenameDirectoryItem(HTREEITEM hItem, LPCTSTR lpszNewName);
	BOOL DeleteDirectoryItem(HTREEITEM hItem);
	BOOL RefreshDirectoryItem(HTREEITEM hItem);


public: // *** FileWindowRemote.cpp ***


public: // *** FileWindowProject.cpp ***
	BOOL InitProjectWorkspace();

	BOOL NewProjectWorkspace(LPCTSTR lpszPathName);
	BOOL SaveProjectWorkspace(LPCTSTR lpszPathName);
	BOOL OpenProjectWorkspace(LPCTSTR lpszPathName);

	BOOL SaveRegularWorkspace(LPCTSTR lpszPathName);
	BOOL OpenRegularWorkspace(LPCTSTR lpszPathName);

	BOOL AddCategoryToProject(LPCTSTR lpszCategory);
	BOOL AddLocalFileToProject(LPCTSTR lpszPathName);
	BOOL AddRemoteFileToProject(INT nAccount, LPCTSTR lpszPathName, DWORD dwSize);

	BOOL OpenProjectItem();
	BOOL ExecuteProjectItem();
	BOOL RenameProjectItem();
	BOOL RemoveProjectItem();

	BOOL IsSelectedProjectItemRoot();
	CString GetSelectedProjectItemText();

protected: // inner functions
	BOOL SaveProjectItem(ostream & os, INT nLevel, HTREEITEM hItem);
	BOOL LoadProjectItem(istream & is, TCHAR szText[], HTREEITEM hParent);

	BOOL SaveWorkspaceItem(ostream & os, INT nLevel, CDocument * pDoc);
	BOOL LoadWorkspaceItem(istream & is, TCHAR szText[], CWinApp * pApp);

	BOOL ParseProjectItemAttr(LPCTSTR lpszText, CMapStringToString & mapAttr);

	LPPROJECTITEMINFO GetProjectItemInfo(HTREEITEM hItem);
	CString GetProjectItemPathName(HTREEITEM hItem);

	HTREEITEM GetSelectedProjectItem() { return m_treProjectTree.GetSelectedItem(); }
	HTREEITEM GetClickedProjectItem();

	BOOL EnableAllProjectButtons(BOOL bEnable);
	static INT CALLBACK CompareProjectItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	HTREEITEM InsertProjectItem(HTREEITEM hParent, LPCTSTR lpszText, INT nType, INT nAccount, LPCTSTR lpszPathName, LONG nSize);
	HTREEITEM FindChildProjectItem(HTREEITEM hParent, LPCTSTR lpszText);

protected: // action functions
	BOOL OpenProjectContextMenu(HTREEITEM hItem);
	BOOL RemoveAllProjectItems();

	BOOL OpenProjectItem(HTREEITEM hItem);
	BOOL ExecuteProjectItem(HTREEITEM hItem);

	BOOL RenameProjectItem(HTREEITEM hItem, LPCTSTR lpszNewName);
	BOOL RemoveProjectItem(HTREEITEM hItem);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Attributes
protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CFileWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCategoryTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropdownLocalDrive();
	afx_msg void OnSelchangeLocalDrive();
	afx_msg void OnSelchangeFileFilter();
	afx_msg void OnItemexpandedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickProjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkProjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickProjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditProjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditProjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEWINDOW_H__EF837327_15B9_11D5_A6F1_0050CE184C9B__INCLUDED_)
