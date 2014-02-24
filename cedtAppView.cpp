#include "stdafx.h"
#include "cedtHeader.h"



void CCedtApp::ApplyPreferencesToAllViews()
{
	INT nFont = CCedtView::m_nCurrentScreenFont;
	if( CCedtView::m_bColumnMode ) { // check if it need to use column mode font
		if( CCedtView::IsFixedPitchScreenFont(nFont) ) CCedtView::m_bUsingColumnModeFont = FALSE;
		else CCedtView::m_bUsingColumnModeFont = TRUE;
	} else CCedtView::m_bUsingColumnModeFont = FALSE;

	// apply preferences to all views
	SaveCaretAndAnchorPosAllViews();

	CCedtView::CreateScreenFontObject();
	CCedtView::ApplyCurrentScreenFont();
	FormatScreenTextAllViews();

	RestoreCaretAndAnchorPosAllViews();
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::UpdateAllViews()
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		POSITION posView = pDoc->GetFirstViewPosition();
		while( posView ) {
			CCedtView * pView = (CCedtView *)pDoc->GetNextView( posView );
			pView->Invalidate();
		}
	}
}

void CCedtApp::SaveCaretAndAnchorPosAllViews()
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		POSITION posView = pDoc->GetFirstViewPosition();
		while( posView ) {
			CCedtView * pView = (CCedtView *)pDoc->GetNextView( posView );
			pView->SaveCaretAndAnchorPos();
		}
	}
}

void CCedtApp::RestoreCaretAndAnchorPosAllViews()
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		POSITION posView = pDoc->GetFirstViewPosition();
		while( posView ) {
			CCedtView * pView = (CCedtView *)pDoc->GetNextView( posView );
			pView->RestoreCaretAndAnchorPos();
		}
	}
}

void CCedtApp::FormatScreenTextAllViews()
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		POSITION posView = pDoc->GetFirstViewPosition();
		while( posView ) {
			CCedtView * pView = (CCedtView *)pDoc->GetNextView( posView );
			pView->FormatScreenText();
		}
	}
}

void CCedtApp::TurnOffWordWrapModeAllViews()
{
	POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc );
		POSITION posView = pDoc->GetFirstViewPosition();
		while( posView ) {
			CCedtView * pView = (CCedtView *)pDoc->GetNextView( posView );
			if( pView->IsWordWrapOn() ) pView->TurnOffWordWrapMode();
		}
	}
}


void CCedtApp::OnEditColumnMode() 
{
	CCedtView::m_bColumnMode = ! CCedtView::m_bColumnMode;

	// turn off word wrap mode of all views
	if( CCedtView::m_bColumnMode ) TurnOffWordWrapModeAllViews();

	BOOL bNeedFormatByFontChange = FALSE;
	if( CCedtView::m_bColumnMode ) { // check if it need to use column mode font
		if( CCedtView::IsUsingFixedPitchFont() ) CCedtView::m_bUsingColumnModeFont = FALSE;
		else { bNeedFormatByFontChange = TRUE; CCedtView::m_bUsingColumnModeFont = TRUE; }
	} else {
		if( CCedtView::m_bUsingColumnModeFont ) bNeedFormatByFontChange = TRUE;
		CCedtView::m_bUsingColumnModeFont = FALSE;
	}

	if( bNeedFormatByFontChange ) {
		// create screen font object and apply to all views
		SaveCaretAndAnchorPosAllViews();

		CCedtView::CreateScreenFontObject();
		CCedtView::ApplyCurrentScreenFont();
		FormatScreenTextAllViews();

		RestoreCaretAndAnchorPosAllViews();
		UpdateAllViews();
	} else {
		// just need to redraw all views...
		UpdateAllViews();
	}

	// set indicators of main frame window
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetColumnModeFlag(CCedtView::m_bColumnMode);

	// we need to update caret position info of main frame window
	CCedtView * pView = (CCedtView *)pMainFrame->MDIGetActiveView();
	if( pView ) pView->UpdateCaretPosition();
}

void CCedtApp::OnUpdateEditColumnMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CCedtView::m_bColumnMode); 
}

void CCedtApp::OnIndicatorOvr() 
{
	CCedtView::m_bOverwriteMode = ! CCedtView::m_bOverwriteMode;

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetOverwriteFlag(CCedtView::m_bOverwriteMode);

	CCedtView * pView = (CCedtView *)pMainFrame->MDIGetActiveView();
	if( pView ) pView->UpdateCaretPosition();
}

void CCedtApp::OnViewLineNumbers() 
{
	CCedtView::m_bShowLineNumbers = ! CCedtView::m_bShowLineNumbers;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewLineNumbers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bShowLineNumbers );
}

void CCedtApp::OnViewLineSpacing100() { OnViewLineSpacing(100); }
void CCedtApp::OnViewLineSpacing110() { OnViewLineSpacing(110); }
void CCedtApp::OnViewLineSpacing120() { OnViewLineSpacing(120); }
void CCedtApp::OnViewLineSpacing150() { OnViewLineSpacing(150); }
void CCedtApp::OnViewLineSpacing200() { OnViewLineSpacing(200); }

void CCedtApp::OnViewLineSpacing(INT nSpacing)
{
	SaveCaretAndAnchorPosAllViews();

	CCedtView::m_nLineSpacing = nSpacing;

	RestoreCaretAndAnchorPosAllViews();
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewLineSpacing100(CCmdUI* pCmdUI) { OnUpdateViewLineSpacing(100, pCmdUI); }
void CCedtApp::OnUpdateViewLineSpacing110(CCmdUI* pCmdUI) { OnUpdateViewLineSpacing(110, pCmdUI); }
void CCedtApp::OnUpdateViewLineSpacing120(CCmdUI* pCmdUI) { OnUpdateViewLineSpacing(120, pCmdUI); }
void CCedtApp::OnUpdateViewLineSpacing150(CCmdUI* pCmdUI) { OnUpdateViewLineSpacing(150, pCmdUI); }
void CCedtApp::OnUpdateViewLineSpacing200(CCmdUI* pCmdUI) { OnUpdateViewLineSpacing(200, pCmdUI); }

void CCedtApp::OnUpdateViewLineSpacing(INT nSpacing, CCmdUI * pCmdUI)
{
	pCmdUI->SetRadio( CCedtView::m_nLineSpacing == nSpacing );
}

void CCedtApp::OnViewScreenFont0() { OnViewScreenFont(0); }
void CCedtApp::OnViewScreenFont1() { OnViewScreenFont(1); }
void CCedtApp::OnViewScreenFont2() { OnViewScreenFont(2); }
void CCedtApp::OnViewScreenFont3() { OnViewScreenFont(3); }
void CCedtApp::OnViewScreenFont4() { OnViewScreenFont(4); }
void CCedtApp::OnViewScreenFont5() { OnViewScreenFont(5); }

void CCedtApp::OnViewScreenFont(INT nFont)
{
	CCedtView::m_nCurrentScreenFont = nFont;
	if( CCedtView::m_bColumnMode ) { // check if it need to use column mode font
		if( CCedtView::IsFixedPitchScreenFont(nFont) ) CCedtView::m_bUsingColumnModeFont = FALSE;
		else CCedtView::m_bUsingColumnModeFont = TRUE;
	} else CCedtView::m_bUsingColumnModeFont = FALSE;

	// apply preferences to al views
	SaveCaretAndAnchorPosAllViews();

	CCedtView::CreateScreenFontObject();
	CCedtView::ApplyCurrentScreenFont();
	FormatScreenTextAllViews();

	RestoreCaretAndAnchorPosAllViews();
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewScreenFont0(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(0, pCmdUI); }
void CCedtApp::OnUpdateViewScreenFont1(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(1, pCmdUI); }
void CCedtApp::OnUpdateViewScreenFont2(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(2, pCmdUI); }
void CCedtApp::OnUpdateViewScreenFont3(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(3, pCmdUI); }
void CCedtApp::OnUpdateViewScreenFont4(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(4, pCmdUI); }
void CCedtApp::OnUpdateViewScreenFont5(CCmdUI* pCmdUI) { OnUpdateViewScreenFont(5, pCmdUI); }

void CCedtApp::OnUpdateViewScreenFont(INT nFont, CCmdUI * pCmdUI)
{
	if( pCmdUI->m_pMenu ) {
		CString szMenuText; LOGFONT lf; memcpy( & lf, & CCedtView::m_lfScreen[nFont], sizeof(LOGFONT) );
		if( nFont == 0 ) szMenuText.Format(IDS_MENU_VIEW_FONT_DEFAULT, lf.lfFaceName, lf.lfHeight / 10);
		else szMenuText.Format(IDS_MENU_VIEW_FONT_CUSTOM, nFont, lf.lfFaceName, lf.lfHeight / 10);
		pCmdUI->SetText(szMenuText);
	}
	pCmdUI->SetRadio( CCedtView::m_nCurrentScreenFont == nFont );
}

void CCedtApp::OnViewPrinterFont0() { OnViewPrinterFont(0); }
void CCedtApp::OnViewPrinterFont1() { OnViewPrinterFont(1); }
void CCedtApp::OnViewPrinterFont2() { OnViewPrinterFont(2); }
void CCedtApp::OnViewPrinterFont3() { OnViewPrinterFont(3); }

void CCedtApp::OnViewPrinterFont(INT nFont) 
{
	CCedtView::m_nCurrentPrinterFont = nFont;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewPrinterFont0(CCmdUI* pCmdUI) { OnUpdateViewPrinterFont(0, pCmdUI); }
void CCedtApp::OnUpdateViewPrinterFont1(CCmdUI* pCmdUI) { OnUpdateViewPrinterFont(1, pCmdUI); }
void CCedtApp::OnUpdateViewPrinterFont2(CCmdUI* pCmdUI) { OnUpdateViewPrinterFont(2, pCmdUI); }
void CCedtApp::OnUpdateViewPrinterFont3(CCmdUI* pCmdUI) { OnUpdateViewPrinterFont(3, pCmdUI); }

void CCedtApp::OnUpdateViewPrinterFont(INT nFont, CCmdUI* pCmdUI) 
{
	if( pCmdUI->m_pMenu ) {
		CString szMenuText; LOGFONT lf; memcpy( & lf, & CCedtView::m_lfPrinter[nFont], sizeof(LOGFONT) );
		if( nFont == 0 ) szMenuText.Format(IDS_MENU_VIEW_FONT_DEFAULT, lf.lfFaceName, lf.lfHeight / 10);
		else szMenuText.Format(IDS_MENU_VIEW_FONT_CUSTOM, nFont, lf.lfFaceName, lf.lfHeight / 10);
		pCmdUI->SetText(szMenuText);
	}
	pCmdUI->SetRadio( CCedtView::m_nCurrentPrinterFont == nFont );
}


void CCedtApp::OnViewTabSize02() { OnViewTabSize( 2); }
void CCedtApp::OnViewTabSize04() { OnViewTabSize( 4); }
void CCedtApp::OnViewTabSize08() { OnViewTabSize( 8); }
void CCedtApp::OnViewTabSize16() { OnViewTabSize(16); }

void CCedtApp::OnViewTabSize(INT nTabSize)
{
	SaveCaretAndAnchorPosAllViews();

	CCedtView::m_nTabSize = nTabSize;
	FormatScreenTextAllViews();

	RestoreCaretAndAnchorPosAllViews();
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewTabSize02(CCmdUI* pCmdUI) { OnUpdateViewTabSize( 2, pCmdUI); }
void CCedtApp::OnUpdateViewTabSize04(CCmdUI* pCmdUI) { OnUpdateViewTabSize( 4, pCmdUI); }
void CCedtApp::OnUpdateViewTabSize08(CCmdUI* pCmdUI) { OnUpdateViewTabSize( 8, pCmdUI); }
void CCedtApp::OnUpdateViewTabSize16(CCmdUI* pCmdUI) { OnUpdateViewTabSize(16, pCmdUI); }

void CCedtApp::OnUpdateViewTabSize(INT nTabSize, CCmdUI * pCmdUI)
{
	pCmdUI->SetRadio( CCedtView::m_nTabSize == nTabSize );
}

void CCedtApp::OnViewEmboldenKeywords() 
{
	CCedtView::m_bEmboldenKeywords = ! CCedtView::m_bEmboldenKeywords;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewEmboldenKeywords(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bEmboldenKeywords );
}

void CCedtApp::OnViewItalicizeComment() 
{
	CCedtView::m_bItalicizeComment = ! CCedtView::m_bItalicizeComment;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewItalicizeComment(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bItalicizeComment );
}

void CCedtApp::OnViewShowSpaces() 
{
	CCedtView::m_bShowSpaces = ! CCedtView::m_bShowSpaces;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewShowSpaces(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bShowSpaces );
}

void CCedtApp::OnViewShowTabChars() 
{
	CCedtView::m_bShowTabChars = ! CCedtView::m_bShowTabChars;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewShowTabChars(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bShowTabChars );
}

void CCedtApp::OnViewShowLineBreak() 
{
	CCedtView::m_bShowLineBreak = ! CCedtView::m_bShowLineBreak;
	UpdateAllViews(); SaveUserConfiguration();
}

void CCedtApp::OnUpdateViewShowLineBreak(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CCedtView::m_bShowLineBreak );
}
