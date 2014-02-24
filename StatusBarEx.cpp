// CedtStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "StatusBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define PANE_WIDTH_PROGRESS		200

/////////////////////////////////////////////////////////////////////////////
// CStatusBarEx

CStatusBarEx::CStatusBarEx()
{
	m_fontPane1.CreateStockObject( ANSI_VAR_FONT );
	m_fontPane2.CreateStockObject( DEFAULT_GUI_FONT );
	memset( m_crPaneTextColors, 0x00, sizeof(m_crPaneTextColors));
	memset( m_bPaneHighlight, 0x00, sizeof(m_bPaneHighlight));
}

CStatusBarEx::~CStatusBarEx()
{
	m_fontPane1.DeleteObject();
	m_fontPane2.DeleteObject();
}


BEGIN_MESSAGE_MAP(CStatusBarEx, CStatusBar)
	//{{AFX_MSG_MAP(CStatusBarEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusBarEx message handlers

int CStatusBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CStatusBarEx::BeginProgress(LPCTSTR lpszMessage)
{
	CStatusBar::SetPaneText(0, "", FALSE);
	m_szPaneText = lpszMessage;
}

void CStatusBarEx::EndProgress()
{
	CString szPaneText( (LPCTSTR)AFX_IDS_IDLEMESSAGE );

	CStatusBar::SetPaneText(0, szPaneText, FALSE);
	m_szPaneText = szPaneText;

	Invalidate();
}

void CStatusBarEx::SetProgress(INT nPercent)
{
	CFont * pFontOld; CBitmap * pBmpOld;
	CRect rect; GetItemRect(0, & rect);
	rect.right = rect.left + PANE_WIDTH_PROGRESS;

	CDC * pDC = GetDC();
	m_dcPane.CreateCompatibleDC( pDC );

	m_dcPane.SetBkMode(TRANSPARENT);
	pFontOld = m_dcPane.SelectObject( & m_fontPane1 );

	m_bmpPane.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height() );
	pBmpOld = m_dcPane.SelectObject( & m_bmpPane );

	// draw border
	CRect border; border.left = border.top = 0;
	border.right = rect.Width();
	border.bottom = rect.Height();
	DrawPaneBorder( & m_dcPane, border );

	// draw pane
	CString szPaneText; szPaneText.Format("%s %i%%", m_szPaneText, nPercent);
	DrawPaneText( & m_dcPane, border, szPaneText, RGB(0, 0, 0) );

	INT nSplit = border.Width() * nPercent / 100;
	m_dcPane.ExcludeClipRect(nSplit, 0, rect.Width(), rect.Height());

	m_dcPane.FillSolidRect(1, 1, border.Width()-2, border.Height()-2, RGB(0, 0, 180));
	DrawPaneText( & m_dcPane, border, szPaneText, RGB(255, 255, 255) );

	// bit built
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		& m_dcPane, 0, 0, SRCCOPY);

	m_dcPane.SelectObject( pFontOld );
	m_dcPane.SelectObject( pBmpOld );

	m_bmpPane.DeleteObject();
	m_dcPane.DeleteDC();

	ReleaseDC( pDC );
}

void CStatusBarEx::SetPaneTextColor(INT nIndex, COLORREF crTextColor)
{
	if( m_crPaneTextColors[nIndex] == crTextColor ) return;
	m_crPaneTextColors[nIndex] = crTextColor;

	CRect rect; GetItemRect(nIndex, & rect);
	InvalidateRect(& rect, FALSE);
}

void CStatusBarEx::SetPaneHighlight(INT nIndex, BOOL bHighlight)
{
	if( m_bPaneHighlight[nIndex] == bHighlight ) return;
	m_bPaneHighlight[nIndex] = bHighlight;

	CRect rect; GetItemRect(nIndex, & rect);
	InvalidateRect(& rect, FALSE);
}

void CStatusBarEx::OnPaint() 
{
	CStatusBar::OnPaint(); // call parent class OnPaint member function
	CRect rect; GetClientRect( & rect );

	CDC * pDC = GetDC();
	m_dcPane.CreateCompatibleDC( pDC );

	m_dcPane.SetBkMode(TRANSPARENT);
	CFont * pFontOld = m_dcPane.SelectObject( & m_fontPane2 );

	m_bmpPane.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height() );
	CBitmap * pBmpOld = m_dcPane.SelectObject( & m_bmpPane );

	CStatusBarCtrl & rStatusBarCtrl = GetStatusBarCtrl();
	INT nCount = rStatusBarCtrl.GetParts(0, NULL);

	for(INT nPane = 1; nPane < nCount; nPane++) {
		GetItemRect(nPane, & rect);

		CRect border; border.left = border.top = 0;
		border.right = rect.Width();
		border.bottom = rect.Height();

		COLORREF crBgColor = m_bPaneHighlight[nPane] ? m_crPaneTextColors[nPane] : GetSysColor(COLOR_3DFACE);
		m_dcPane.FillSolidRect( & border, crBgColor);
		
		CString szPaneText; GetPaneText(nPane, szPaneText);
		COLORREF crTextColor = m_bPaneHighlight[nPane] ? GetSysColor(COLOR_3DFACE) : m_crPaneTextColors[nPane];
		DrawPaneText( & m_dcPane, border, szPaneText, crTextColor );

		pDC->BitBlt(rect.left+1, rect.top+1, rect.Width()-2, rect.Height()-2, & m_dcPane, 1, 1, SRCCOPY);
	}

	m_dcPane.SelectObject( pFontOld );
	m_dcPane.SelectObject( pBmpOld );

	m_bmpPane.DeleteObject();
	m_dcPane.DeleteDC();
	ReleaseDC( pDC );
}

void CStatusBarEx::DrawPaneBorder(CDC * pDC, CRect rect)
{
	CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DDKSHADOW));
	CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	CPen * pPenOld;

	pPenOld = pDC->SelectObject( & penShadow );
	pDC->MoveTo(rect.right-2, rect.top);
	pDC->LineTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom-2);

	pDC->SelectObject( & penLight );
	pDC->MoveTo(rect.left, rect.bottom-1);
	pDC->LineTo(rect.right-1, rect.bottom-1);
	pDC->LineTo(rect.right-1, rect.top);

	pDC->FillSolidRect(rect.left+1, rect.top+1, rect.right-2, rect.bottom-2, GetSysColor(COLOR_3DFACE));
	pDC->SelectObject( pPenOld );
}

void CStatusBarEx::DrawPaneText(CDC * pDC, CRect rect, LPCTSTR lpszPaneText, COLORREF crTextColor)
{
	INT nLength = strlen(lpszPaneText);
	CSize size = pDC->GetTextExtent(lpszPaneText, nLength);
	INT nPosX = rect.left + (rect.Width() - size.cx) / 2;
	INT nPosY = rect.top + (rect.Height() - size.cy) / 2;

	pDC->SetTextColor(crTextColor);
	pDC->TextOut(nPosX, nPosY, lpszPaneText, nLength);
}
