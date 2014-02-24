#include "stdafx.h"
#include "cedtHeader.h"



BOOL CCedtView::IsBlankLine(CAnalyzedString & rLine)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->IsBlankLine(rLine);
}

BOOL CCedtView::IsBlankLineFromIdxY(INT nIdxY)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->IsBlankLineFromIdxY(nIdxY);
}

BOOL CCedtView::IsBlankLine(CFormatedString & rLine)
{
	SHORT i, sWordCount = rLine.m_sWordCount;
	for(i = 0; i < sWordCount; i++) {
		FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return FALSE;
	}
	return TRUE;
}

BOOL CCedtView::IsBlankLineFromPosY(INT nPosY)
{
	CFormatedString & rLine = GetLineFromPosY(nPosY);
	return IsBlankLine(rLine);
}


void CCedtView::PositionToIndex(INT nPosX, INT nPosY, INT & nIdxX, INT & nIdxY)
{
	nIdxY = GetIdxYFromPosY( nPosY );
	CFormatedString & rLine = GetLineFromPosY( nPosY );
	nIdxX = GetIdxXFromPosX( rLine, nPosX );
}

void CCedtView::IndexToPosition(INT nIdxX, INT nIdxY, INT & nPosX, INT & nPosY)
{
	nPosY = GetPosYFromIdxY( nIdxX, nIdxY );
	CFormatedString & rLine = GetLineFromPosY( nPosY );
	nPosX = GetPosXFromIdxX( rLine, nIdxX );
}

TCHAR CCedtView::GetCharFromPosX(CFormatedString & rLine, INT nPosX)
{
	FORMATEDWORDINFO & rWord = GetWordFromPosX(rLine, nPosX);
	INT nIdxX = GetIdxXFromPosX(rLine, rWord, nPosX);
	return rLine[nIdxX];
}

TCHAR CCedtView::GetCharFromIdxX(CFormatedString & rLine, INT nIdxX)
{
	return rLine[nIdxX];
}


INT CCedtView::GetNextSegmentIdxX(CFormatedString & rLine, INT nIdxX) 
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();

	TCHAR nCh = GetCharFromIdxX(rLine, nIdxX);
	INT nType = pDoc->GetCharType(nCh);

	INT i, nEndX = GetLastIdxX(rLine);
	for(i = nIdxX + 1; i <= nEndX; i++) {
		nCh = GetCharFromIdxX(rLine, i);
		if( nType != pDoc->GetCharType(nCh) ) return i;
	}
	return nEndX;
}

INT CCedtView::GetPrevSegmentIdxX(CFormatedString & rLine, INT nIdxX) 
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();

	TCHAR nCh = GetCharFromIdxX(rLine, nIdxX);
	INT nType = pDoc->GetCharType(nCh);

	INT i, nBegX = GetFirstIdxX(rLine);
	for(i = nIdxX - 1; i >= nBegX; i--) {
		nCh = GetCharFromIdxX(rLine, i);
		if( nType != pDoc->GetCharType(nCh) ) return i+1;
	}
	return nBegX;
}

INT CCedtView::GetNextWordIdxX(CFormatedString & rLine, INT nIdxX)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	nIdxX = GetNextSegmentIdxX(rLine, nIdxX);

	TCHAR nCh = GetCharFromIdxX(rLine, nIdxX);
	INT nType = pDoc->GetCharType(nCh);

	if( nType != CH_WHITESPACE ) return nIdxX;
	else return GetNextSegmentIdxX(rLine, nIdxX);
}

INT CCedtView::GetPrevWordIdxX(CFormatedString & rLine, INT nIdxX)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	nIdxX = GetPrevSegmentIdxX(rLine, nIdxX);

	TCHAR nCh = GetCharFromIdxX(rLine, nIdxX);
	INT nType = pDoc->GetCharType(nCh);

	if( nType != CH_WHITESPACE || nIdxX == 0 ) return nIdxX;
	else return GetPrevSegmentIdxX(rLine, nIdxX-1);
}

INT CCedtView::GetCurrWordRange(CFormatedString & rLine, INT nIdxX, INT & nBegX, INT & nEndX)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();

	TCHAR nCh = GetCharFromIdxX( rLine, nIdxX );
	INT nType = pDoc->GetCharType(nCh);

	if( nType == CH_WHITESPACE && nIdxX > 0 ) {
		nCh   = GetCharFromIdxX( rLine, nIdxX-1 );
		nType = pDoc->GetCharType(nCh);
		if( nType == CH_CHARACTER ) {
			nBegX = GetPrevSegmentIdxX( rLine, nIdxX-1 );
			nEndX = GetNextSegmentIdxX( rLine, nIdxX-1 );
		} else nBegX = nEndX = nIdxX;
	} else if( nType == CH_CHARACTER ) {
		nBegX = GetPrevSegmentIdxX( rLine, nIdxX );
		nEndX = GetNextSegmentIdxX( rLine, nIdxX );
	} else if( nType == CH_DELIMITER ) {
		nBegX = nIdxX;
		nEndX = nIdxX + 1;
	} else nBegX = nEndX = nIdxX;

	return nEndX - nBegX;
}


