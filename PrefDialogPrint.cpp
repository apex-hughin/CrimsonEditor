#include "stdafx.h"
#include "cedtHeader.h"
#include "PrefDialog.h"


void CPreferenceDialog::InitPrintPage()
{
	m_btnPrintHeader0.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_btnPrintHeader1.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_btnPrintHeader2.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_btnPrintFooter0.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_btnPrintFooter1.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_btnPrintFooter2.SetIcon( m_lstButtonImage.ExtractIcon(2) );
}

void CPreferenceDialog::SizePrintPage()
{
	INT nPosY;

	nPosY  = 26; m_stcPageMarginBox.MoveWindow(170, nPosY, 360, 75);
	nPosY += 10; m_stcPageMarginTitle.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_stcPageMarginLeft.MoveWindow(180, nPosY, 60, 14);	m_edtPageMarginLeft.MoveWindow(250, nPosY-3, 70, 18);
	nPosY +=  0; m_stcPageMarginRight.MoveWindow(360, nPosY, 60, 14);	m_edtPageMarginRight.MoveWindow(430, nPosY-3, 70, 18);
	nPosY += 20; m_stcPageMarginTop.MoveWindow(180, nPosY, 60, 14);		m_edtPageMarginTop.MoveWindow(250, nPosY-3, 70, 18);
	nPosY +=  0; m_stcPageMarginBottom.MoveWindow(360, nPosY, 60, 14);	m_edtPageMarginBottom.MoveWindow(430, nPosY-3, 70, 18);

	nPosY += 35; m_stcPrintHeaderBox.MoveWindow(170, nPosY, 360, 95);
	nPosY += 10; m_chkPrintHeader.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_stcPrintHeader0.MoveWindow(180, nPosY, 60, 14);		m_edtPrintHeader0.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintHeader0.MoveWindow(505, nPosY-3, 20, 18);
	nPosY += 20; m_stcPrintHeader1.MoveWindow(180, nPosY, 60, 14);		m_edtPrintHeader1.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintHeader1.MoveWindow(505, nPosY-3, 20, 18);
	nPosY += 20; m_stcPrintHeader2.MoveWindow(180, nPosY, 60, 14);		m_edtPrintHeader2.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintHeader2.MoveWindow(505, nPosY-3, 20, 18);

	nPosY += 35; m_stcPrintFooterBox.MoveWindow(170, nPosY, 360, 95);
	nPosY += 10; m_chkPrintFooter.MoveWindow(180, nPosY, 300, 14);
	nPosY += 20; m_stcPrintFooter0.MoveWindow(180, nPosY, 60, 14);		m_edtPrintFooter0.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintFooter0.MoveWindow(505, nPosY-3, 20, 18);
	nPosY += 20; m_stcPrintFooter1.MoveWindow(180, nPosY, 60, 14);		m_edtPrintFooter1.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintFooter1.MoveWindow(505, nPosY-3, 20, 18);
	nPosY += 20; m_stcPrintFooter2.MoveWindow(180, nPosY, 60, 14);		m_edtPrintFooter2.MoveWindow(250, nPosY-3, 250, 18);	m_btnPrintFooter2.MoveWindow(505, nPosY-3, 20, 18);

	nPosY += 35; m_stcPrintOptionsBox.MoveWindow(170, nPosY, 360, 35);
	nPosY += 10; m_chkPrintLineNumbers.MoveWindow(180, nPosY, 160, 14);	m_chkPrintSyntaxHighlight.MoveWindow(360, nPosY, 160, 14);
}

void CPreferenceDialog::ShowPrintPage()
{
	INT nCmdShow = (m_nActiveCategory == PREF_CATEGORY_PRINT) ? SW_SHOW : SW_HIDE;

	m_stcPageMarginBox.ShowWindow(nCmdShow);
	m_stcPageMarginTitle.ShowWindow(nCmdShow);
	m_stcPageMarginLeft.ShowWindow(nCmdShow);	m_edtPageMarginLeft.ShowWindow(nCmdShow);
	m_stcPageMarginRight.ShowWindow(nCmdShow);	m_edtPageMarginRight.ShowWindow(nCmdShow);
	m_stcPageMarginTop.ShowWindow(nCmdShow);	m_edtPageMarginTop.ShowWindow(nCmdShow);
	m_stcPageMarginBottom.ShowWindow(nCmdShow);	m_edtPageMarginBottom.ShowWindow(nCmdShow);

	m_stcPrintHeaderBox.ShowWindow(nCmdShow);
	m_chkPrintHeader.ShowWindow(nCmdShow);
	m_stcPrintHeader0.ShowWindow(nCmdShow);		m_edtPrintHeader0.ShowWindow(nCmdShow);		m_btnPrintHeader0.ShowWindow(nCmdShow);
	m_stcPrintHeader1.ShowWindow(nCmdShow);		m_edtPrintHeader1.ShowWindow(nCmdShow);		m_btnPrintHeader1.ShowWindow(nCmdShow);
	m_stcPrintHeader2.ShowWindow(nCmdShow);		m_edtPrintHeader2.ShowWindow(nCmdShow);		m_btnPrintHeader2.ShowWindow(nCmdShow);

	m_stcPrintFooterBox.ShowWindow(nCmdShow);
	m_chkPrintFooter.ShowWindow(nCmdShow);
	m_stcPrintFooter0.ShowWindow(nCmdShow);		m_edtPrintFooter0.ShowWindow(nCmdShow);		m_btnPrintFooter0.ShowWindow(nCmdShow);
	m_stcPrintFooter1.ShowWindow(nCmdShow);		m_edtPrintFooter1.ShowWindow(nCmdShow);		m_btnPrintFooter1.ShowWindow(nCmdShow);
	m_stcPrintFooter2.ShowWindow(nCmdShow);		m_edtPrintFooter2.ShowWindow(nCmdShow);		m_btnPrintFooter2.ShowWindow(nCmdShow);

	m_stcPrintOptionsBox.ShowWindow(nCmdShow);
	m_chkPrintLineNumbers.ShowWindow(nCmdShow);
	m_chkPrintSyntaxHighlight.ShowWindow(nCmdShow);

}


BOOL CPreferenceDialog::LoadPrintSettings()
{
	CString str;

	str.Format("%d", CCedtView::m_rectPageMargin.left / 10);
	m_edtPageMarginLeft.SetWindowText( str );

	str.Format("%d", CCedtView::m_rectPageMargin.right / 10);
	m_edtPageMarginRight.SetWindowText( str );

	str.Format("%d", CCedtView::m_rectPageMargin.top / 10);
	m_edtPageMarginTop.SetWindowText( str );

	str.Format("%d", CCedtView::m_rectPageMargin.bottom / 10);
	m_edtPageMarginBottom.SetWindowText( str );

	m_chkPrintHeader.SetCheck( CCedtView::m_bPrintHeader ? 1 : 0 );
	m_edtPrintHeader0.SetWindowText( CCedtView::m_szHeaderFormat[0] );
	m_edtPrintHeader1.SetWindowText( CCedtView::m_szHeaderFormat[1] );
	m_edtPrintHeader2.SetWindowText( CCedtView::m_szHeaderFormat[2] );
	m_edtPrintHeader0.EnableWindow( CCedtView::m_bPrintHeader );
	m_edtPrintHeader1.EnableWindow( CCedtView::m_bPrintHeader );
	m_edtPrintHeader2.EnableWindow( CCedtView::m_bPrintHeader );
	m_btnPrintHeader0.EnableWindow( CCedtView::m_bPrintHeader );
	m_btnPrintHeader1.EnableWindow( CCedtView::m_bPrintHeader );
	m_btnPrintHeader2.EnableWindow( CCedtView::m_bPrintHeader );

	m_chkPrintFooter.SetCheck( CCedtView::m_bPrintFooter ? 1 : 0 );
	m_edtPrintFooter0.SetWindowText( CCedtView::m_szFooterFormat[0] );
	m_edtPrintFooter1.SetWindowText( CCedtView::m_szFooterFormat[1] );
	m_edtPrintFooter2.SetWindowText( CCedtView::m_szFooterFormat[2] );
	m_edtPrintFooter0.EnableWindow( CCedtView::m_bPrintFooter );
	m_edtPrintFooter1.EnableWindow( CCedtView::m_bPrintFooter );
	m_edtPrintFooter2.EnableWindow( CCedtView::m_bPrintFooter );
	m_btnPrintFooter0.EnableWindow( CCedtView::m_bPrintFooter );
	m_btnPrintFooter1.EnableWindow( CCedtView::m_bPrintFooter );
	m_btnPrintFooter2.EnableWindow( CCedtView::m_bPrintFooter );

	m_chkPrintLineNumbers.SetCheck( CCedtView::m_bPrintLineNumbers ? 1 : 0 );
	m_chkPrintSyntaxHighlight.SetCheck( CCedtView::m_bPrintSyntaxHighlight ? 1 : 0 );

	return TRUE;
}

BOOL CPreferenceDialog::SavePrintSettings()
{
	TCHAR buf[1024];

	m_edtPageMarginLeft.GetWindowText(buf, 1024); CCedtView::m_rectPageMargin.left = 10 * atoi(buf);
	ASSURE_BOUND_VALUE(CCedtView::m_rectPageMargin.left, 0, 1000);

	m_edtPageMarginRight.GetWindowText(buf, 1024); CCedtView::m_rectPageMargin.right = 10 * atoi(buf);
	ASSURE_BOUND_VALUE(CCedtView::m_rectPageMargin.right, 0, 1000);

	m_edtPageMarginTop.GetWindowText(buf, 1024); CCedtView::m_rectPageMargin.top = 10 * atoi(buf);
	ASSURE_BOUND_VALUE(CCedtView::m_rectPageMargin.top, 0, 1000);

	m_edtPageMarginBottom.GetWindowText(buf, 1024); CCedtView::m_rectPageMargin.bottom = 10 * atoi(buf);
	ASSURE_BOUND_VALUE(CCedtView::m_rectPageMargin.bottom, 0, 1000);

	CCedtView::m_bPrintHeader = m_chkPrintHeader.GetCheck();
	m_edtPrintHeader0.GetWindowText( CCedtView::m_szHeaderFormat[0] );
	m_edtPrintHeader1.GetWindowText( CCedtView::m_szHeaderFormat[1] );
	m_edtPrintHeader2.GetWindowText( CCedtView::m_szHeaderFormat[2] );

	CCedtView::m_bPrintFooter = m_chkPrintFooter.GetCheck();
	m_edtPrintFooter0.GetWindowText( CCedtView::m_szFooterFormat[0] );
	m_edtPrintFooter1.GetWindowText( CCedtView::m_szFooterFormat[1] );
	m_edtPrintFooter2.GetWindowText( CCedtView::m_szFooterFormat[2] );

	CCedtView::m_bPrintLineNumbers = m_chkPrintLineNumbers.GetCheck();
	CCedtView::m_bPrintSyntaxHighlight = m_chkPrintSyntaxHighlight.GetCheck();

	return TRUE;
}

void CPreferenceDialog::OnPrintHeader() 
{
	BOOL bEnable = m_chkPrintHeader.GetCheck();
	m_edtPrintHeader0.EnableWindow( bEnable );
	m_edtPrintHeader1.EnableWindow( bEnable );
	m_edtPrintHeader2.EnableWindow( bEnable );
	m_btnPrintHeader0.EnableWindow( bEnable );
	m_btnPrintHeader1.EnableWindow( bEnable );
	m_btnPrintHeader2.EnableWindow( bEnable );
}

void CPreferenceDialog::OnPrintFooter() 
{
	BOOL bEnable = m_chkPrintFooter.GetCheck();
	m_edtPrintFooter0.EnableWindow( bEnable );
	m_edtPrintFooter1.EnableWindow( bEnable );
	m_edtPrintFooter2.EnableWindow( bEnable );
	m_btnPrintFooter0.EnableWindow( bEnable );
	m_btnPrintFooter1.EnableWindow( bEnable );
	m_btnPrintFooter2.EnableWindow( bEnable );
}

void CPreferenceDialog::OnPrintHeader0Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(2);

	CRect rect; m_btnPrintHeader0.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnPrintHeader1Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(3);

	CRect rect; m_btnPrintHeader1.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnPrintHeader2Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(4);

	CRect rect; m_btnPrintHeader2.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnPrintFooter0Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(5);

	CRect rect; m_btnPrintFooter0.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnPrintFooter1Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(6);

	CRect rect; m_btnPrintFooter1.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnPrintFooter2Menu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_PREF_DIALOG);
	pMenu = context.GetSubMenu(7);

	CRect rect; m_btnPrintFooter2.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CPreferenceDialog::OnHeader0FilePath() { m_edtPrintHeader0.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnHeader0FileName() { m_edtPrintHeader0.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnHeader0PageNumber() {	m_edtPrintHeader0.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnHeader0TotalPage() { m_edtPrintHeader0.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnHeader0CurrentDate() { m_edtPrintHeader0.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnHeader0CurrentTime() { m_edtPrintHeader0.ReplaceSel("$(CurrTime)"); }

void CPreferenceDialog::OnHeader1FilePath() { m_edtPrintHeader1.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnHeader1FileName() { m_edtPrintHeader1.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnHeader1PageNumber() { m_edtPrintHeader1.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnHeader1TotalPage() { m_edtPrintHeader1.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnHeader1CurrentDate() { m_edtPrintHeader1.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnHeader1CurrentTime() { m_edtPrintHeader1.ReplaceSel("$(CurrTime)"); }

void CPreferenceDialog::OnHeader2FilePath() { m_edtPrintHeader2.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnHeader2FileName() { m_edtPrintHeader2.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnHeader2PageNumber() { m_edtPrintHeader2.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnHeader2TotalPage() { m_edtPrintHeader2.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnHeader2CurrentDate() { m_edtPrintHeader2.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnHeader2CurrentTime() { m_edtPrintHeader2.ReplaceSel("$(CurrTime)"); }

void CPreferenceDialog::OnFooter0FilePath() { m_edtPrintFooter0.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnFooter0FileName() { m_edtPrintFooter0.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnFooter0PageNumber() { m_edtPrintFooter0.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnFooter0TotalPage() { m_edtPrintFooter0.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnFooter0CurrentDate() { m_edtPrintFooter0.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnFooter0CurrentTime() { m_edtPrintFooter0.ReplaceSel("$(CurrTime)"); }

void CPreferenceDialog::OnFooter1FilePath() { m_edtPrintFooter1.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnFooter1FileName() { m_edtPrintFooter1.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnFooter1PageNumber() { m_edtPrintFooter1.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnFooter1TotalPage() { m_edtPrintFooter1.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnFooter1CurrentDate() { m_edtPrintFooter1.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnFooter1CurrentTime() { m_edtPrintFooter1.ReplaceSel("$(CurrTime)"); }

void CPreferenceDialog::OnFooter2FilePath() { m_edtPrintFooter2.ReplaceSel("$(FilePath)"); }
void CPreferenceDialog::OnFooter2FileName() { m_edtPrintFooter2.ReplaceSel("$(FileName)"); }
void CPreferenceDialog::OnFooter2PageNumber() { m_edtPrintFooter2.ReplaceSel("$(PageNumber)"); }
void CPreferenceDialog::OnFooter2TotalPage() { m_edtPrintFooter2.ReplaceSel("$(TotalPage)"); }
void CPreferenceDialog::OnFooter2CurrentDate() { m_edtPrintFooter2.ReplaceSel("$(CurrDate)"); }
void CPreferenceDialog::OnFooter2CurrentTime() { m_edtPrintFooter2.ReplaceSel("$(CurrTime)"); }

