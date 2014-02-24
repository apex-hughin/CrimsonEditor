#if !defined(AFX_OUTPUTWINDOW_H__0217A261_BFCA_11D5_B61D_00E0298ED004__INCLUDED_)
#define AFX_OUTPUTWINDOW_H__0217A261_BFCA_11D5_B61D_00E0298ED004__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SizeCBar.h"
#include "VerticalStatic.h"


#define OUTPUT_MAX_LINE_COUNT	1024


class COutputWindow : public CSizingControlBar
{
// Construction
public:
	COutputWindow();

// Resources & Controls
protected:
	TBBUTTON m_tbiToolbarOutput[10];
	TBBUTTON m_tbiWinButtons[10];

	CImageList m_imgToolbarOutput;
	CImageList m_imgWinButtons;

	CToolBarCtrl m_btnToolbarOutput;
	CToolBarCtrl m_btnWinButtons;

	CListBox m_lstConsoleOutput;
	CEdit m_edtConsoleInput;
	CVerticalStatic m_stcCaptionOutput;

	CFont m_fontControl, m_fontOutput;

	BOOL m_bOccupied;

// Operations
public:
	void ApplyOutputFont(BOOL bRedraw = TRUE);
	void EnableInputConsole(BOOL bEnable);

	void SetOccupied(BOOL bOccupy) { m_bOccupied = bOccupy; }
	BOOL CanUseNow() { return ! m_bOccupied; }

	void CopyAllTheContents();
	void ClearAllTheContents();

	BOOL AddStringToTheLast(LPCTSTR lpszString);
	BOOL ReplaceTheLastString(LPCTSTR lpszString);
	BOOL GetTheLastString(CString & szString);

protected:
	void OnKeyReturn();
	void OnKeyEscape();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkOutput();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTWINDOW_H__0217A261_BFCA_11D5_B61D_00E0298ED004__INCLUDED_)
