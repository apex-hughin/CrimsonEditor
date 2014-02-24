#include "stdafx.h"
#include "cedtHeader.h"


BOOL CCedtView::ActionMoveLeft(UINT nFlags) 
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );

	if( m_nCaretPosX > GetLastPosX( rLine ) ) {
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphBegin();
		else if( nFlags & KEYSTATE_CONTROL    ) MoveCaretLineEnd();
		else   /* no special key pressed     */ MoveCaretLeft();
		return ! IsCaretVisible();
	} else if( m_nCaretPosX > GetFirstPosX( rLine ) ) {
		INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
		FORMATEDWORDINFO & rWord = GetWordFromIdxX( rLine, nIdxX-1 );
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphBegin();
		else if( nFlags & KEYSTATE_CONTROL    ) MoveCaretWordLeft();
		else if( rWord.m_cType == WT_DBCHAR   ) MoveCaretLeft( 2 );
		else   /* no special key pressed     */ MoveCaretLeft();
		return ! IsCaretVisible();
	} else if( ! m_bColumnMode && m_nCaretPosY > 0 ) {
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphBegin();
		else   /* not pressed with menu      */ { MoveCaretUp(); MoveCaretLineEnd(); }
		return ! IsCaretVisible();
	} else return FALSE;
}

BOOL CCedtView::ActionMoveRight(UINT nFlags) 
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );

	if( m_nCaretPosX < GetLastPosX( rLine ) ) {
		INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
		FORMATEDWORDINFO & rWord = GetWordFromIdxX( rLine, nIdxX );
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphEnd();
		else if( nFlags & KEYSTATE_CONTROL    ) MoveCaretWordRight();
		else if( rWord.m_cType == WT_DBCHAR   ) MoveCaretRight( 2 );
		else   /* no special key pressed     */ MoveCaretRight();
		return ! IsCaretVisible();
	} else if( m_bColumnMode ) {
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphEnd();
		else if( nFlags & KEYSTATE_CONTROL    ) MoveCaretLineEnd();
		else   /* no special key pressed     */ MoveCaretRight();
		return ! IsCaretVisible();
	} else if( m_nCaretPosY < GetLastPosY() ) {
		if     ( nFlags & KEYSTATE_MENU       ) MoveCaretParagraphEnd();
		else   /* not pressed with menu      */ { MoveCaretDown(); MoveCaretLineBegin(); }
		return ! IsCaretVisible();
	} else return FALSE;
}

BOOL CCedtView::ActionMoveUp(UINT nFlags) 
{
	if( nFlags & KEYSTATE_CONTROL && nFlags & KEYSTATE_SHIFT ) {
		if( m_nScrollPosY > 0 ) { ScrollScreenUpLimited(); return TRUE; }
		else return FALSE;
	} else if( nFlags & KEYSTATE_CONTROL ) {
		if( m_nScrollPosY > 0 ) { ScrollScreenUp(); return TRUE; }
		else return FALSE;
	} else if( nFlags & KEYSTATE_MENU ) {
		MoveCaretScreenTop();
		return ! IsCaretVisible();
	} else /* no special key pressed */ {
		if( m_nCaretPosY  > 0 ) MoveCaretUp(); 
		return ! IsCaretVisible();
	}
}

BOOL CCedtView::ActionMoveDown(UINT nFlags) 
{
	if( nFlags & KEYSTATE_CONTROL && nFlags & KEYSTATE_SHIFT ) {
		if( m_nScrollPosY < GetLastPosY() ) { ScrollScreenDownLimited(); return TRUE; }
		else return FALSE;
	} else if( nFlags & KEYSTATE_CONTROL ) {
		if( m_nScrollPosY < GetLastPosY() ) { ScrollScreenDown(); return TRUE; }
		else return FALSE;
	} else if( nFlags & KEYSTATE_MENU ) {
		MoveCaretScreenBottom(); 
		return ! IsCaretVisible();
	} else /* no special key pressed */ {
		if( m_nCaretPosY  < GetLastPosY() ) MoveCaretDown(); 
		return ! IsCaretVisible();
	}
}

BOOL CCedtView::ActionMoveHome(UINT nFlags)
{
	if( nFlags & KEYSTATE_CONTROL ) {
		MoveCaretDocumentBegin();
		return ! IsCaretVisible();
	} else if( nFlags & KEYSTATE_MENU ) {
		MoveCaretScreenBegin();
		return ! IsCaretVisible();
	} else if( m_bHomeKeyGoesToFirstPosition ) {
		MoveCaretLineBegin();
		return ! IsCaretVisible();
	} else /* no special key pressed */ {
		CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
		if( m_nCaretPosX != GetFirstNonBlankPosX(rLine) ) MoveCaretLineBeginNonBlank();
		else MoveCaretLineBegin();
		return ! IsCaretVisible();
	}
}

BOOL CCedtView::ActionMoveEnd(UINT nFlags)
{
	if( nFlags & KEYSTATE_CONTROL ) {
		MoveCaretDocumentEnd();
		return ! IsCaretVisible();
	} else if( nFlags & KEYSTATE_MENU ) {
		MoveCaretScreenEnd();
		return ! IsCaretVisible(); 
	} else /* no special key pressed */ {
		MoveCaretLineEnd();
		return ! IsCaretVisible();
	}
}

BOOL CCedtView::ActionMovePrior(UINT nFlags)
{
	if( nFlags & KEYSTATE_CONTROL ) {
		/* do nothing here */
		return FALSE;
	} else if( nFlags & KEYSTATE_MENU ) {
		if( m_nCaretPosY > 0 ) { MoveCaretHalfPageUp(); return TRUE; }
		else return FALSE;
	} else /* no special key pressed */ {
		if( m_nCaretPosY > 0 ) { MoveCaretPageUp(); return TRUE; }
		else return FALSE;
	}
}

BOOL CCedtView::ActionMoveNext(UINT nFlags)
{
	if( nFlags & KEYSTATE_CONTROL ) {
		/* do nothing here */
		return FALSE;
	} else if( nFlags & KEYSTATE_MENU ) {
		if( m_nCaretPosY < GetLastPosY() ) { MoveCaretHalfPageDown(); return TRUE; }
		else return FALSE;
	} else /* no special key pressed */ {
		if( m_nCaretPosY < GetLastPosY() ) { MoveCaretPageDown(); return TRUE; }
		else return FALSE;
	}
}


void CCedtView::MoveCaretLeft(INT nCount)
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	if( m_nCaretPosX <= GetLastPosX( rLine ) ) {
		INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
		if( nIdxX >= nCount ) SetCaretPosX( GetPosXFromIdxX( rLine, nIdxX-nCount ) );
	} else if( m_bColumnMode ) {
		INT nIdxX = m_nCaretPosX / GetAveCharWidth();
		if( nIdxX >= nCount ) SetCaretPosX( (nIdxX-nCount) * GetAveCharWidth() );
	} else SetCaretPosX( GetLastPosX( rLine ) );
}

void CCedtView::MoveCaretRight(INT nCount)
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	if( m_nCaretPosX < GetLastPosX( rLine ) ) {
		INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
		SetCaretPosX( GetPosXFromIdxX( rLine, nIdxX+nCount ) );
	} else if( m_bColumnMode ) {
		INT nIdxX = m_nCaretPosX / GetAveCharWidth();
		SetCaretPosX( (nIdxX+nCount) * GetAveCharWidth() );
	} else SetCaretPosX( GetLastPosX( rLine ) );
}

void CCedtView::MoveCaretWordLeft()
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
	if( nIdxX > 0 ) nIdxX = GetPrevWordIdxX( rLine, nIdxX-1 );
	SetCaretPosX( GetPosXFromIdxX( rLine, nIdxX ) );
}

void CCedtView::MoveCaretWordRight()
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	INT nIdxX = GetIdxXFromPosX( rLine, m_nCaretPosX );
	nIdxX = GetNextWordIdxX( rLine, nIdxX );
	SetCaretPosX( GetPosXFromIdxX( rLine, nIdxX ) );
}

void CCedtView::MoveCaretLineBegin()
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	SetCaretPosX( GetFirstPosX( rLine ) );
}

void CCedtView::MoveCaretLineEnd()
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	SetCaretPosX( GetLastPosX( rLine ) );
}

void CCedtView::MoveCaretLineBeginNonBlank()
{
	CFormatedString & rLine = GetLineFromPosY( m_nCaretPosY );
	SetCaretPosX( GetFirstNonBlankPosX( rLine ) );
}

void CCedtView::MoveCaretParagraphBegin()
{
	INT nIdxY = GetIdxYFromPosY( m_nCaretPosY );
	CAnalyzedString & rLine = GetLineFromIdxY( nIdxY );
	INT nIdxX = GetFirstIdxX( rLine );

	SetCaretPosY( GetPosYFromIdxY( nIdxX, nIdxY ) );
	CFormatedString & rLne2 = GetLineFromPosY( m_nCaretPosY );
	SetCaretPosX( GetPosXFromIdxX( rLne2, nIdxX ) );
}

void CCedtView::MoveCaretParagraphEnd()
{
	INT nIdxY = GetIdxYFromPosY( m_nCaretPosY );
	CAnalyzedString & rLine = GetLineFromIdxY( nIdxY );
	INT nIdxX = GetLastIdxX( rLine );

	SetCaretPosY( GetPosYFromIdxY( nIdxX, nIdxY ) );
	CFormatedString & rLne2 = GetLineFromPosY( m_nCaretPosY );
	SetCaretPosX( GetPosXFromIdxX( rLne2, nIdxX ) );
}

void CCedtView::MoveCaretUp()
{
	SetCaretPosY( m_nCaretPosY - GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretDown()
{
	SetCaretPosY( m_nCaretPosY + GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretScreenTop()
{
	SetCaretPosY( m_nScrollPosY );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretScreenBottom()
{
	SetCaretPosY( m_nScrollPosY + (GetLinesPerPage() - 1) * GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretPageUp()
{
	SetScrollPosY( m_nScrollPosY - GetLinesPerPage() * GetLineHeight() );
	SetCaretPosY( m_nCaretPosY - GetLinesPerPage() * GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretPageDown()
{
	SetScrollPosY( m_nScrollPosY + GetLinesPerPage() * GetLineHeight() );
	SetCaretPosY( m_nCaretPosY + GetLinesPerPage() * GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretHalfPageUp()
{
	SetScrollPosY( m_nScrollPosY - (GetLinesPerPage() / 2) * GetLineHeight() );
	SetCaretPosY( m_nCaretPosY - (GetLinesPerPage() / 2) * GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretHalfPageDown()
{
	SetScrollPosY( m_nScrollPosY + (GetLinesPerPage() / 2) * GetLineHeight() );
	SetCaretPosY( m_nCaretPosY + (GetLinesPerPage() / 2) * GetLineHeight() );
	SetCaretPosX( m_nCaretPosX, FALSE );
}

void CCedtView::MoveCaretDocumentBegin()
{
	SetCaretPosY( 0 );
	MoveCaretLineBegin();
}

void CCedtView::MoveCaretDocumentEnd()
{
	SetCaretPosY( GetLastPosY() );
	MoveCaretLineEnd();
}

void CCedtView::MoveCaretScreenBegin()
{
	SetCaretPosY( m_nScrollPosY );
	MoveCaretLineBegin();
}

void CCedtView::MoveCaretScreenEnd()
{
	SetCaretPosY( m_nScrollPosY + (GetLinesPerPage() - 1) * GetLineHeight() );
	MoveCaretLineEnd();
}

void CCedtView::ScrollScreenUp()
{
	SetScrollPosY( m_nScrollPosY - GetLineHeight() );
	if( m_nCaretPosY > m_nScrollPosY + GetLinesPerPage() * GetLineHeight() - GetLineHeight() ) {
		SetCaretPosY( m_nScrollPosY + GetLinesPerPage() * GetLineHeight() - GetLineHeight() );
		SetCaretPosX( m_nCaretPosX, FALSE );
	}
}

void CCedtView::ScrollScreenDown()
{
	SetScrollPosY( m_nScrollPosY + GetLineHeight() );
	if( m_nCaretPosY < m_nScrollPosY ) {
		SetCaretPosY( m_nScrollPosY );
		SetCaretPosX( m_nCaretPosX, FALSE );
	}
}


void CCedtView::ScrollScreenUpLimited()
{
	INT nSavedScrollPosY = m_nScrollPosY;
	SetScrollPosY( m_nScrollPosY - GetLineHeight() );
	if( m_nCaretPosY > m_nScrollPosY + GetLinesPerPage() * GetLineHeight() - GetLineHeight() ) {
		SetScrollPosY( nSavedScrollPosY );
	}
}

void CCedtView::ScrollScreenDownLimited()
{
	INT nSavedScrollPosY = m_nScrollPosY;
	SetScrollPosY( m_nScrollPosY + GetLineHeight() );
	if( m_nCaretPosY < m_nScrollPosY ) {
		SetScrollPosY( nSavedScrollPosY );
	}
}



