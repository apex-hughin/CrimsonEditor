#if !defined(AFX_FILETAB_H__CF314FB5_538C_11D3_978A_00104B151BA8__INCLUDED_)
#define AFX_FILETAB_H__CF314FB5_538C_11D3_978A_00104B151BA8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIFileTab window

class CMDIFileTab : public CDialogBar
{
// Construction
public:
	CMDIFileTab();

// Attributes
protected:
	CImageList m_lstImage;

// Operations
public:
	void InsertMDIFileTab(CMDIChildWnd * pChild);
	void DeleteMDIFileTab(CMDIChildWnd * pChild);

	void UpdateMDIFileTab(CMDIChildWnd * pChild);
	void SetActiveFileTab(CMDIChildWnd * pChild);

protected:
	void AdjustTabString(CString & szFileName);

public:
	CMDIChildWnd * GetNextChildFrame();
	CMDIChildWnd * GetPrevChildFrame();

protected:
	int  FindTabIndex(CMDIChildWnd * pChild);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIFileTab)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
    virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
    virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDIFileTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIFileTab)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchangeFileTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETAB_H__CF314FB5_538C_11D3_978A_00104B151BA8__INCLUDED_)
