// FileTab.cpp : implementation file
//

#include "stdafx.h"
#include "cedtHeader.h"
#include "MDIFileTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIFileTab

CMDIFileTab::CMDIFileTab()
{
}

CMDIFileTab::~CMDIFileTab()
{
}


BEGIN_MESSAGE_MAP(CMDIFileTab, CDialogBar)
	//{{AFX_MSG_MAP(CMDIFileTab)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TCN_SELCHANGE, IDC_FILE_TAB, OnSelchangeFileTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIFileTab message handlers

void CMDIFileTab::InsertMDIFileTab(CMDIChildWnd * pChild)
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	TC_ITEM item; item.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	item.pszText = "";
	item.iImage = 0;
	item.lParam = (LPARAM)pChild;

	INT nTab = pTabCtrl->GetItemCount();
	pTabCtrl->InsertItem(nTab, & item);
}

void CMDIFileTab::DeleteMDIFileTab(CMDIChildWnd * pChild)
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	INT nTab = FindTabIndex(pChild); if(nTab < 0) return;
	pTabCtrl->DeleteItem(nTab);
}

void CMDIFileTab::UpdateMDIFileTab(CMDIChildWnd * pChild)
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	CCedtDoc * pDoc = (CCedtDoc *)pChild->GetActiveDocument();
	CString szText; pChild->GetWindowText( szText );

	// check string size of window text (can have zero length if it is not visible)
	if( ! szText.GetLength() ) szText = pDoc->GetTitle();

	// process window text to adjust into tab string
	szText = GetFileName(szText); AdjustTabString(szText); szText += " ";

	TC_ITEM item; item.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	item.pszText = (LPTSTR)(LPCTSTR)szText;
	item.iImage = pDoc->IsModified() ? 1 : 0;
	item.lParam = (LPARAM)pChild;

	TCHAR szTxt2[1024]; 
	TC_ITEM itm2; itm2.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	itm2.pszText = szTxt2; itm2.cchTextMax = sizeof(szTxt2);

	INT nTab = FindTabIndex(pChild); if(nTab < 0) return;
	pTabCtrl->GetItem(nTab, & itm2);

	if( strcmp(item.pszText, itm2.pszText) || item.iImage != itm2.iImage )
		pTabCtrl->SetItem(nTab, & item);
}

void CMDIFileTab::SetActiveFileTab(CMDIChildWnd * pChild)
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	INT nTab = FindTabIndex(pChild); if(nTab < 0) return;
	pTabCtrl->SetCurSel(nTab);
}

void CMDIFileTab::AdjustTabString(CString & szFileName)
{
	INT nFileTabLength = CCedtView::m_nFileTabLength;
	INT nLength = szFileName.GetLength();

	if( nLength > CCedtView::m_nFileTabLength ) {
		INT nFound = szFileName.ReverseFind('.');

		if( nFound >= 0 ) nFound = nFound+1; // ignore '.' character
		else nFound = szFileName.Find(':'); // try to get view counter

		if( nFound >= 0 ) {
			CString szExtension = szFileName.Mid(nFound);
			szFileName = szFileName.Mid(0, nFileTabLength-(nLength-nFound)-3);
			szFileName += CString("...") + szExtension;
		} else {
			szFileName = szFileName.Mid(0, nFileTabLength-3);
			szFileName += "...";
		}
	}
}

CMDIChildWnd * CMDIFileTab::GetNextChildFrame()
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	INT nCount = pTabCtrl->GetItemCount();
	if( nCount == 0 ) return NULL;

	INT nTab = pTabCtrl->GetCurSel();
	if( nTab < 0 ) return NULL;

	INT nNext = nTab + 1;
	if( nNext > nCount-1 ) nNext = 0;

	TC_ITEM item; item.mask = TCIF_PARAM;
	pTabCtrl->GetItem(nNext, & item);

	return (CMDIChildWnd *)item.lParam;
}

CMDIChildWnd * CMDIFileTab::GetPrevChildFrame()
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	INT nCount = pTabCtrl->GetItemCount();
	if( nCount == 0 ) return NULL;

	INT nTab = pTabCtrl->GetCurSel();
	if( nTab < 0 ) return NULL;

	INT nPrev = nTab - 1;
	if( nPrev < 0 ) nPrev = nCount-1;

	TC_ITEM item; item.mask = TCIF_PARAM;
	pTabCtrl->GetItem(nPrev, & item);

	return (CMDIChildWnd *)item.lParam;
}

int CMDIFileTab::FindTabIndex(CMDIChildWnd * pChild)
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	TC_ITEM item; item.mask = TCIF_PARAM;
	INT i, nCount = pTabCtrl->GetItemCount();

	for(i = 0; i < nCount; i++) {
		pTabCtrl->GetItem(i, & item);
		if( item.lParam == (LPARAM)pChild ) return i;
	}

	return -1;
}

BOOL CMDIFileTab::Create(CWnd * pParentWnd)
{
	CDialogBar::Create(pParentWnd, IDD_MDI_FILE_TAB, WS_CHILD | WS_VISIBLE 
		| CBRS_TOP | CBRS_SIZE_DYNAMIC, IDD_MDI_FILE_TAB);

	m_lstImage.Create(IDB_MDI_FILE_TAB, 12, 0, RGB(255, 0, 255));

	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	if( pTabCtrl ) pTabCtrl->SetImageList( & m_lstImage );
	
	return TRUE;
}

CSize CMDIFileTab::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect rect; AfxGetMainWnd()->GetWindowRect(& rect);
	return CSize(rect.Width()-4, 30);
}

CSize CMDIFileTab::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CRect rect; AfxGetMainWnd()->GetWindowRect(& rect);
	return CSize(rect.Width()-4, 30);
}

void CMDIFileTab::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);

	RECT rect; GetClientRect( & rect );
	rect.left += 2; rect.right -= 2;
	rect.top += 5; rect.bottom -= 0;

	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	if( pTabCtrl ) pTabCtrl->MoveWindow( & rect );
}

void CMDIFileTab::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	TC_HITTESTINFO info;
	info.pt = point; pTabCtrl->ScreenToClient( & info.pt );
	INT nCurrTab = pTabCtrl->HitTest( & info );
	
	if( nCurrTab >= 0 ) {
		TC_ITEM item; item.mask = TCIF_PARAM;
		pTabCtrl->GetItem(nCurrTab, & item);

		CMDIChildWnd * pChild = (CMDIChildWnd *)item.lParam;
		pChild->MDIActivate();
	}

	CMenu * pMenu, context; context.LoadMenu(IDR_FILE_TAB);
	if( nCurrTab < 0 ) pMenu = context.GetSubMenu(0);
	else pMenu = context.GetSubMenu(1);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, AfxGetMainWnd()); 
}

void CMDIFileTab::OnSelchangeFileTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_FILE_TAB);
	ASSERT( pTabCtrl );

	INT nTab = pTabCtrl->GetCurSel();
	if( nTab < 0 ) return;

	TC_ITEM item; item.mask = TCIF_PARAM;
	pTabCtrl->GetItem(nTab, & item);

	CMDIChildWnd * pChild = (CMDIChildWnd *)item.lParam;
	pChild->MDIActivate();

	*pResult = 0;
}
