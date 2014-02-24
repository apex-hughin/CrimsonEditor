#include "stdafx.h"
#include "cedtHeader.h"



INT CCedtView::GetLastIdxY()
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetLastIdxY();
}

INT CCedtView::GetLastPosY()
{
	INT nLineHeight = GetLineHeight();
	return nLineHeight * (m_clsFormatedScreenText.GetCount()-1);
}

INT CCedtView::GetFirstIdxX(CAnalyzedString & rLine)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetFirstIdxX( rLine );
}

INT CCedtView::GetFirstIdxX(CFormatedString & rLine)
{
	FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[0];
	return rWord.m_sIndex;
}

INT CCedtView::GetLastIdxX(CAnalyzedString & rLine)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetLastIdxX( rLine );
}

INT CCedtView::GetLastIdxX(CFormatedString & rLine)
{
	FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[rLine.m_sWordCount-1];
	return rWord.m_sIndex + rWord.m_sLength;
}

INT CCedtView::GetFirstNonBlankIdxX(CAnalyzedString & rLine)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetFirstNonBlankIdxX( rLine );
}

INT CCedtView::GetTrailingBlankIdxX(CAnalyzedString & rLine)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetTrailingBlankIdxX( rLine );
}


INT CCedtView::GetNextTabPosition(INT nPosX)
{
	INT nSpaceWidth = GetSpaceWidth();
	INT nTabWidth = m_nTabSize * nSpaceWidth;

	INT nNextPosX = ((nPosX + nSpaceWidth - 1) / nTabWidth + 1) * nTabWidth;
	return nNextPosX;
}

INT CCedtView::GetPrevTabPosition(INT nPosX)
{
	INT nSpaceWidth = GetSpaceWidth();
	INT nTabWidth = m_nTabSize * nSpaceWidth;

	INT nPrevPosX = ((nPosX - 1) / nTabWidth) * nTabWidth;
	return (nPrevPosX > 0) ? nPrevPosX : 0;
}


INT CCedtView::GetFirstPosX(CFormatedString & rLine) 
{
	FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[0];
	return rWord.m_nPosition;
}

INT CCedtView::GetLastPosX(CFormatedString & rLine) 
{
	FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[rLine.m_sWordCount-1];
	return rWord.m_nPosition + rWord.m_nWidth;
}

INT CCedtView::GetFirstNonBlankPosX(CFormatedString & rLine) 
{
	SHORT i, sWordCount = rLine.m_sWordCount;
	for(i = 0; i < sWordCount; i++) {
		FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return rWord.m_nPosition;
	}
	return GetLastPosX(rLine);
}

INT CCedtView::GetTrailingBlankPosX(CFormatedString & rLine)
{
	SHORT i, sWordCount = rLine.m_sWordCount;
	for(i = sWordCount-1; i >= 0; i--) {
		FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return rWord.m_nPosition + rWord.m_nWidth;
	}
	return GetFirstPosX(rLine);
}

CAnalyzedString & CCedtView::GetLineFromIdxY(INT nIdxY)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetLineFromIdxY(nIdxY);
}

CFormatedString & CCedtView::GetLineFromPosY(INT nPosY)
{
	INT nLineIndex = nPosY / GetLineHeight();
	POSITION pos = m_clsFormatedScreenText.FindIndex(nLineIndex);
	if( pos ) return m_clsFormatedScreenText.GetAt(pos);
	return m_clsFormatedScreenText.GetTail();
}

ANALYZEDWORDINFO & CCedtView::GetWordFromIdxX(CAnalyzedString & rLine, INT nIdxX)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	return pDoc->GetWordFromIdxX(rLine, nIdxX);
}

FORMATEDWORDINFO & CCedtView::GetWordFromPosX(CFormatedString & rLine, INT nPosX)
{
	SHORT i, sWordCount = rLine.m_sWordCount;
	for(i = 0; i < sWordCount; i++) {
		FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		if( rWord.m_nPosition + rWord.m_nWidth > nPosX ) return rLine.m_pWordInfo[i];
	}
	return rLine.m_pWordInfo[sWordCount-1];
}

FORMATEDWORDINFO & CCedtView::GetWordFromIdxX(CFormatedString & rLine, INT nIdxX)
{
	SHORT i, sWordCount = rLine.m_sWordCount;
	for(i = 0; i < sWordCount; i++) {
		FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) return rLine.m_pWordInfo[i];
	}
	return rLine.m_pWordInfo[sWordCount-1];
}

INT CCedtView::GetIdxYFromPosY(INT nPosY)
{
	INT nLineIndex = nPosY / GetLineHeight();
	INT nParaCount = 0, nLineCount = 0;

	POSITION pos = m_clsFormatedScreenText.GetHeadPosition();
	while( pos ) {
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		nLineCount++; if( rLine.m_sLineSplitIndex == 0 ) nParaCount++;
		if( nLineIndex == nLineCount-1 ) return nParaCount-1;
	}
	return nParaCount-1;
}

INT CCedtView::GetIdxXFromPosX(CFormatedString & rLine, INT nPosX, BOOL bAdjust)
{
	INT nFirstPosX, nLastPosX;

	if( nPosX < (nFirstPosX = GetFirstPosX(rLine) ) ) {
		return GetFirstIdxX( rLine );
	} else if( nPosX < (nLastPosX = GetLastPosX(rLine) ) ) {
		FORMATEDWORDINFO & rWord = GetWordFromPosX(rLine, nPosX);
		return GetIdxXFromPosX(rLine, rWord, nPosX, bAdjust);
	} else {
		if( bAdjust ) return GetLastIdxX(rLine);
		else return GetLastIdxX(rLine) + (nPosX - nLastPosX) / GetAveCharWidth();
	}
}

INT CCedtView::GetIdxXFromPosX(CFormatedString & rLine, FORMATEDWORDINFO & rWord, INT nPosX, BOOL bAdjust)
{
	if( rWord.m_nPosition + rWord.m_nWidth <= nPosX ) return rWord.m_sIndex + rWord.m_sLength;
	if( rWord.m_cType == WT_RETURN || rWord.m_cType == WT_LINEFEED || rWord.m_cType == WT_TAB || rWord.m_nPosition >= nPosX ) return rWord.m_sIndex;
	if( rWord.m_cType == WT_DBCHAR ) return bAdjust ? rWord.m_sIndex : rWord.m_sIndex + (nPosX - rWord.m_nPosition) / GetAveCharWidth();
	return rWord.m_sIndex + GetWordIndex((LPCTSTR)rLine + rWord.m_sIndex, rWord.m_sLength, nPosX - rWord.m_nPosition);
}

INT CCedtView::GetPosYFromIdxY(INT nIdxX, INT nIdxY, BOOL bAdjust)
{
	INT nLineHeight = GetLineHeight();
	INT nParaCount = 0, nLineCount = 0;

	POSITION pos = m_clsFormatedScreenText.GetHeadPosition();
	while( pos ) {
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		nLineCount++; if( rLine.m_sLineSplitIndex == 0 ) nParaCount++;
		if( nIdxY <  nParaCount-1 ) return (nLineCount-2) * nLineHeight;
		if( nIdxY == nParaCount-1 && GetLastIdxX( rLine ) > nIdxX ) return (nLineCount-1) * nLineHeight;
	}
	return (nLineCount-1) * nLineHeight;
}

INT CCedtView::GetPosXFromIdxX(CFormatedString & rLine, INT nIdxX, BOOL bAdjust)
{
	INT nFirstIdxX, nLastIdxX;

	if( nIdxX < (nFirstIdxX = GetFirstIdxX(rLine) ) ) {
		return GetFirstPosX( rLine );
	} else if( nIdxX < (nLastIdxX = GetLastIdxX(rLine) ) ) {
		FORMATEDWORDINFO & rWord = GetWordFromIdxX(rLine, nIdxX);
		return GetPosXFromIdxX(rLine, rWord, nIdxX, bAdjust);
	} else {
		if( bAdjust ) return GetLastPosX(rLine);
		else return GetLastPosX(rLine) + (nIdxX - nLastIdxX) * GetAveCharWidth();
	}
}

INT CCedtView::GetPosXFromIdxX(CFormatedString & rLine, FORMATEDWORDINFO & rWord, INT nIdxX, BOOL bAdjust)
{
	if( rWord.m_sIndex + rWord.m_sLength <= nIdxX ) return rWord.m_nPosition + rWord.m_nWidth;
	if( rWord.m_cType == WT_RETURN || rWord.m_cType == WT_LINEFEED || rWord.m_cType == WT_TAB || rWord.m_sIndex >= nIdxX ) return rWord.m_nPosition;
	if( rWord.m_cType == WT_DBCHAR ) return bAdjust ? rWord.m_nPosition : rWord.m_nPosition + (nIdxX - rWord.m_sIndex) * GetAveCharWidth();
	return rWord.m_nPosition + GetWordWidth((LPCTSTR)rLine + rWord.m_sIndex, nIdxX - rWord.m_sIndex, rWord.m_nPosition, rWord.m_cType);
}

