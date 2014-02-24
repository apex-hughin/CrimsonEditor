#include "stdafx.h"
#include "cedtHeader.h"


static FORMATEDWORDINFO _words[MAX_WORDS_COUNT+1];

// width setting global variables
static BOOL  _bFixedWidth;
static INT   _nSpaceWidth, _nTabWidth, _nTabMargin;
static INT   _nWrapWidth, _nIndentWidth, _nMinWidth;

// temporary global variables for format text
static BOOL  _bWordSplit; 
static INT   _nLeadingSpaceWidth;
static SHORT _sLineSplitIndex, _sWordIndex;

static UCHAR _cType, _cRange;
static SHORT _sIndex, _sLength;



static INT _GetLeadingSpaceWidth(CAnalyzedString & rLine, CDC * pDC) 
{
	SHORT i, sLength; INT nWidth, nPosition = 0;
	UCHAR cType;

	for(i = 0; i < rLine.m_sWordCount; i++) {
		ANALYZEDWORDINFO & rWordInfo = rLine.m_pWordInfo[i];
		cType = rWordInfo.m_cType; sLength = rWordInfo.m_sLength;

		if( cType == WT_TAB ) {
			nWidth = ((nPosition + _nSpaceWidth - _nTabMargin) / _nTabWidth + 1) * _nTabWidth - nPosition;
		} else if( cType == WT_SPACE ) {
			nWidth = _nSpaceWidth * sLength;
		} else break;

		nPosition += nWidth;
	}
	return nPosition;
}

static INT _GetWordWidth(LPCTSTR pWord, SHORT sLength, INT nPosition, UCHAR cType, CDC * pDC)
{
	INT nWidth; CSize size;

	if( cType == WT_TAB ) {
		nWidth = ((nPosition + _nSpaceWidth - _nTabMargin) / _nTabWidth + 1) * _nTabWidth - nPosition;
	} else if( cType == WT_SPACE ) {
		nWidth = _nSpaceWidth * sLength;
	} else if( _bFixedWidth ) {
		nWidth = _nSpaceWidth * sLength;
	} else {
		size = pDC->GetTextExtent(pWord, sLength);
		nWidth = (SHORT)size.cx;
	}
	return nWidth;
}

static SHORT _GetWordIndex(LPCTSTR pWord, SHORT sLength, INT nWidth, CDC * pDC)
{
	SHORT i, sIndex; CSize size;
	for(i = 0; i <= sLength; i++) {
		if( _bFixedWidth ) size.cx = _nSpaceWidth * i;
		else size = pDC->GetTextExtent(pWord, i);

		if( size.cx <= nWidth ) sIndex = i;
		else break;
	}
	return sIndex;
}


static void _WordFound(SHORT windex, UCHAR cType, UCHAR cRange, SHORT sIndex, SHORT sLength, INT nPosition, INT nWidth)
{
	_words[windex].m_cType = cType;
	_words[windex].m_cRange = cRange;
	_words[windex].m_sIndex = sIndex;
	_words[windex].m_sLength = sLength;
	_words[windex].m_nPosition = nPosition;
	_words[windex].m_nWidth = nWidth;
}

static void _FinishLine(SHORT wcount, SHORT sLineSplitIndex, CFormatedString & rFmtLine, CAnalyzedString & rLine)
{
	delete [] rFmtLine.m_pWordInfo;
	rFmtLine.m_pWordInfo = new FORMATEDWORDINFO[wcount];
	rFmtLine.m_sWordCount = wcount;
	memcpy(rFmtLine.m_pWordInfo, _words, wcount * sizeof(FORMATEDWORDINFO));

	rFmtLine.m_pString = (LPCTSTR)rLine;
	rFmtLine.m_sLineSplitIndex = sLineSplitIndex;
	rFmtLine.m_sInfoFlags = rLine.m_sInfoFlags;
}


static BOOL _FormatLineWrap(CFormatedString & rFmtLine, CAnalyzedString & rLine, CDC * pDC)
{
	
	SHORT i, wcount, sIndex, sLength, sSplit; 
	INT nPosition, nWidth; UCHAR cType, cRange;

	_nLeadingSpaceWidth = _GetLeadingSpaceWidth(rLine, pDC);
	_sLineSplitIndex = 0;

	wcount = nPosition = 0;
	for(i = 0; i < rLine.m_sWordCount; i++) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		cType = rWord.m_cType; cRange = rWord.m_cRange;
		sIndex = rWord.m_sIndex; sLength = rWord.m_sLength;

		nWidth = _GetWordWidth((LPCTSTR)rLine + sIndex, sLength, nPosition, cType, pDC);

		if( _nWrapWidth <= _nLeadingSpaceWidth + _nIndentWidth + _nMinWidth ) {
			// not enough space - add in the same line
			_WordFound(wcount++, cType, cRange, sIndex, sLength, nPosition, nWidth);
			nPosition += nWidth; _bWordSplit = FALSE;
		} else if( nPosition + nWidth <= _nWrapWidth || (cType == WT_RETURN || cType == WT_TAB || cType == WT_SPACE) ) {
			// add in the same line
			_WordFound(wcount++, cType, cRange, sIndex, sLength, nPosition, nWidth);
			nPosition += nWidth; _bWordSplit = FALSE;
		} else if( _nLeadingSpaceWidth + _nIndentWidth + nWidth <= _nWrapWidth ) {
			// go next line
			_WordFound(wcount++, WT_LINEFEED, RT_GLOBAL, sIndex, 0, nPosition, 0);
			_bWordSplit = FALSE; _sWordIndex = i; break;
		} else {
			// split word - go next line
			sSplit = _GetWordIndex((LPCTSTR)rLine + sIndex, sLength, _nWrapWidth - nPosition, pDC);
			nWidth = _GetWordWidth((LPCTSTR)rLine + sIndex, sSplit, nPosition, cType, pDC);

			_WordFound(wcount++, cType, cRange, sIndex, sSplit, nPosition, nWidth);
			sIndex += sSplit; sLength -= sSplit; nPosition += nWidth;

			_WordFound(wcount++, WT_LINEFEED, RT_GLOBAL, sIndex, 0, nPosition, 0);
			_cType = cType; _cRange = cRange; _sIndex = sIndex; _sLength = sLength;
			_bWordSplit = TRUE; _sWordIndex = i; break;
		}
	}

	_FinishLine(wcount, _sLineSplitIndex++, rFmtLine, rLine);
	return ( i < rLine.m_sWordCount ) ? TRUE : FALSE;
}


static BOOL _FormatLineWrapContinue(CFormatedString & rFmtLine, CAnalyzedString & rLine, CDC * pDC)
{
	SHORT i, wcount, sIndex, sLength, sSplit;
	INT nPosition, nWidth; UCHAR cType, cRange;

	wcount = 0; nPosition = _nLeadingSpaceWidth + _nIndentWidth;
	for(i = _sWordIndex; i < rLine.m_sWordCount; i++) {
		if( _bWordSplit ) {
			cType = _cType; cRange = _cRange; 
			sIndex = _sIndex; sLength = _sLength;
		} else {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
			cType = rWord.m_cType; cRange = rWord.m_cRange;
			sIndex = rWord.m_sIndex; sLength = rWord.m_sLength;
		}

		nWidth = _GetWordWidth((LPCTSTR)rLine + sIndex, sLength, nPosition, cType, pDC);

		if( nPosition + nWidth <= _nWrapWidth || (cType == WT_RETURN || cType == WT_TAB || cType == WT_SPACE) ) {
			// add in the same line
			_WordFound(wcount++, cType, cRange, sIndex, sLength, nPosition, nWidth);
			nPosition += nWidth; _bWordSplit = FALSE;
		} else if( _nLeadingSpaceWidth + _nIndentWidth + nWidth <= _nWrapWidth ) {
			// go next line
			_WordFound(wcount++, WT_LINEFEED, RT_GLOBAL, sIndex, 0, nPosition, 0);
			_bWordSplit = FALSE; _sWordIndex = i; break;
		} else {
			// split word - go next line
			sSplit = _GetWordIndex((LPCTSTR)rLine + sIndex, sLength, _nWrapWidth - nPosition, pDC);
			nWidth = _GetWordWidth((LPCTSTR)rLine + sIndex, sSplit, nPosition, cType, pDC);

			_WordFound(wcount++, cType, cRange, sIndex, sSplit, nPosition, nWidth);
			sIndex += sSplit; sLength -= sSplit; nPosition += nWidth;

			_WordFound(wcount++, WT_LINEFEED, RT_GLOBAL, sIndex, 0, nPosition, 0);
			_cType = cType; _cRange = cRange; _sIndex = sIndex; _sLength = sLength;
			_bWordSplit = TRUE; _sWordIndex = i; break;
		}
	}

	_FinishLine(wcount, _sLineSplitIndex++, rFmtLine, rLine);
	return ( i < rLine.m_sWordCount ) ? TRUE : FALSE;
}


static BOOL _FormatLineNoWrap(CFormatedString & rFmtLine, CAnalyzedString & rLine, CDC * pDC)
{
	SHORT i, wcount, sIndex, sLength;
	INT nPosition, nWidth; UCHAR cType, cRange;

	wcount = nPosition = 0;
	for(i = 0; i < rLine.m_sWordCount; i++) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		cType = rWord.m_cType; cRange = rWord.m_cRange;
		sIndex = rWord.m_sIndex; sLength = rWord.m_sLength;

		nWidth = _GetWordWidth((LPCTSTR)rLine + sIndex, sLength, nPosition, cType, pDC);

		_WordFound(wcount++, cType, cRange, sIndex, sLength, nPosition, nWidth);
		nPosition += nWidth;
	}

	_FinishLine(wcount, 0, rFmtLine, rLine);
	return FALSE;
}


void CCedtView::FormatScreenText() 
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	INT nLineCount = pDoc->GetLineCount();

	m_clsFormatedScreenText.RemoveAll(); CFormatedString dummyLine;
	for(INT i = 0; i < nLineCount; i++) m_clsFormatedScreenText.AddTail( dummyLine );

	FormatScreenText(0, nLineCount);
}


void CCedtView::FormatScreenText(INT nIndex, INT nCount) 
{
	CRect rect; GetClientRect( & rect );
	INT nLeftMargin = GetLeftMargin();
	INT nAveCharWidth = GetAveCharWidth();
	INT nMaxCharWidth = GetMaxCharWidth();

	// save width setting to global variables
	_bFixedWidth = IsUsingFixedPitchFont();
	_nSpaceWidth = GetSpaceWidth();
	_nTabWidth = m_nTabSize * _nSpaceWidth;
	_nTabMargin = 1;

	// save wrap setting to gloabl variables
	if( m_nFixedWrapWidth ) _nWrapWidth = m_nFixedWrapWidth * nAveCharWidth;
	else _nWrapWidth = rect.Width() - nLeftMargin - nMaxCharWidth;
	_nIndentWidth = m_nWrapIndentation * _nSpaceWidth;
	_nMinWidth = 8 * nAveCharWidth;


	CCedtDoc * pDoc = (CCedtDoc *)GetDocument(); ASSERT( pDoc );
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pMainFrame );
	INT nProcess = 0; CWaitCursor * pWait = NULL;

	if( nCount > 1000 ) {
		pWait = new CWaitCursor;
		pMainFrame->BeginProgress("Formatting...");
	}

	POSITION po1 = pDoc->m_clsAnalyzedText.FindIndex(nIndex);
	POSITION po2 = FindScreenTextIndex(nIndex);
	if( m_bLocalWordWrap ) {
		while( po1 && nProcess < nCount ) {
			CAnalyzedString & rLine = pDoc->m_clsAnalyzedText.GetNext(po1); FlattenScreenTextAt(po2);
			BOOL bContinue = _FormatLineWrap( m_clsFormatedScreenText.GetAt(po2), rLine, & m_dcScreen );
			while( bContinue ) {
				CFormatedString dummyLine; po2 = m_clsFormatedScreenText.InsertAfter(po2, dummyLine);
				bContinue = _FormatLineWrapContinue( m_clsFormatedScreenText.GetAt(po2), rLine, & m_dcScreen );
			}
			m_clsFormatedScreenText.GetNext(po2);

			if( nCount > 1000 && ! (nProcess % 20) ) pMainFrame->SetProgress( 100 * nProcess / nCount );
			nProcess++;
		}
	} else {
		while( po1 && nProcess < nCount ) {
			CAnalyzedString & rLine = pDoc->m_clsAnalyzedText.GetNext(po1); FlattenScreenTextAt(po2);
			_FormatLineNoWrap( m_clsFormatedScreenText.GetNext(po2), rLine, & m_dcScreen );

			if( nCount > 1000 && ! (nProcess % 20) ) pMainFrame->SetProgress( 100 * nProcess / nCount );
			nProcess++;
		}
	}	

	if( nCount > 1000 ) {
		delete pWait;
		pMainFrame->EndProgress();
	}
}


void CCedtView::FormatPrintText(CDC * pDC, RECT rectDraw)
{
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();
	FormatPrintText(pDC, rectDraw, 0, pDoc->GetLineCount());
}


void CCedtView::FormatPrintText(CDC * pDC, RECT rectDraw, INT nIdxY, INT nCount)
{
	CRect rect( rectDraw );
	INT nLeftMargin = GetLeftMargin( pDC );
	INT nAveCharWidth = GetAveCharWidth( pDC );
	INT nMaxCharWidth = GetMaxCharWidth( pDC );

	// save width setting to global variables
	_bFixedWidth = IsUsingFixedPitchFont( pDC );
	_nSpaceWidth = GetSpaceWidth( pDC );
	_nTabWidth = m_nTabSize * _nSpaceWidth;
	_nTabMargin = _MY_MAX(_nSpaceWidth / 10, 1);

	// save wrap setting to gloabl variables
	_nWrapWidth = rect.Width() - nLeftMargin - nMaxCharWidth;
	_nIndentWidth = m_nWrapIndentation * _nSpaceWidth;
	_nMinWidth = 8 * nAveCharWidth;


	m_clsFormatedPrintText.RemoveAll(); CFormatedString dummyLine;
	for(INT i = 0; i < nCount; i++) m_clsFormatedPrintText.AddTail( dummyLine );

	CCedtDoc * pDoc = (CCedtDoc *)GetDocument(); ASSERT( pDoc );
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pMainFrame );
	INT nProcess = 0; CWaitCursor * pWait = NULL;

	pWait = new CWaitCursor;
	if( nCount > 100 ) pMainFrame->BeginProgress("Formatting...");

	POSITION po1 = pDoc->m_clsAnalyzedText.FindIndex(nIdxY);
	POSITION po2 = m_clsFormatedPrintText.GetHeadPosition();
	while( po1 && nProcess < nCount ) {
		CAnalyzedString & rLine = pDoc->m_clsAnalyzedText.GetNext(po1);
		BOOL bContinue = _FormatLineWrap( m_clsFormatedPrintText.GetAt(po2), rLine, pDC );
		while( bContinue ) {
			po2 = m_clsFormatedPrintText.InsertAfter(po2, dummyLine);
			bContinue = _FormatLineWrapContinue( m_clsFormatedPrintText.GetAt(po2), rLine, pDC );
		}
		m_clsFormatedPrintText.GetNext(po2);

		if( nCount > 100 && ! (nProcess % 2) ) pMainFrame->SetProgress( 100 * nProcess / nCount );
		nProcess++;
	}

	delete pWait;
	if( nCount > 100 ) pMainFrame->EndProgress();
}


void CCedtView::RemoveScreenText(INT nIndex, INT nCount)
{
	POSITION posRemove = FindScreenTextIndex( nIndex );
	while( nCount-- ) posRemove = RemoveScreenTextAt( posRemove );
}

void CCedtView::InsertScreenText(INT nIndex, INT nCount)
{
	CFormatedString dummyLine;
	POSITION posInsert = (nIndex < 0) ? NULL : FindScreenTextIndex( nIndex );

	if( posInsert ) while( nCount-- ) m_clsFormatedScreenText.InsertAfter( posInsert, dummyLine );
	else while( nCount-- ) m_clsFormatedScreenText.AddHead( dummyLine );
}


INT CCedtView::GetWordWidth(LPCTSTR lpWord, SHORT siLength, INT nPosition, UCHAR ucType, CDC * pDC)
{
	// save width setting to global variables
	_bFixedWidth = IsUsingFixedPitchFont( pDC );
	_nSpaceWidth = GetSpaceWidth( pDC );
	_nTabWidth = m_nTabSize * _nSpaceWidth;
	_nTabMargin = _MY_MAX(_nSpaceWidth / 10, 1);

	if( pDC ) return _GetWordWidth(lpWord, siLength, nPosition, ucType, pDC);
	else return _GetWordWidth(lpWord, siLength, nPosition, ucType, & m_dcScreen);
}

SHORT CCedtView::GetWordIndex(LPCTSTR lpWord, SHORT siLength, INT nWidth, CDC * pDC)
{
	// save width setting to global variables
	_bFixedWidth = IsUsingFixedPitchFont( pDC );
	_nSpaceWidth = GetSpaceWidth( pDC );
	_nTabWidth = m_nTabSize * _nSpaceWidth;
	_nTabMargin = _MY_MAX(_nSpaceWidth / 10, 1);

	if( pDC ) return _GetWordIndex(lpWord, siLength, nWidth, pDC);
	else return _GetWordIndex(lpWord, siLength, nWidth, & m_dcScreen);
}


POSITION CCedtView::FindScreenTextIndex(INT nIndex)
{
	INT nParaCount = 0;
	POSITION posFound, pos = m_clsFormatedScreenText.GetHeadPosition();
	while( pos ) {
		posFound = pos;
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		if( rLine.m_sLineSplitIndex == 0 ) nParaCount++;
		if( nParaCount-1 == nIndex ) break;
	}
	return posFound;
}

POSITION CCedtView::FlattenScreenTextAt(POSITION pos)
{
	INT nParaCount = 0;
	POSITION posFlatten;
	while( pos ) {
		posFlatten = pos;
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		if( rLine.m_sLineSplitIndex == 0 ) nParaCount++;
		if( nParaCount-1 == 0 ) {
			if( rLine.m_sLineSplitIndex ) m_clsFormatedScreenText.RemoveAt( posFlatten );
		} else break;
	}
	return posFlatten;
}

POSITION CCedtView::RemoveScreenTextAt(POSITION pos)
{
	INT nParaCount = 0;
	POSITION posRemove;
	while( pos ) {
		posRemove = pos;
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		if( rLine.m_sLineSplitIndex == 0 ) nParaCount++;
		if( nParaCount-1 == 0 ) m_clsFormatedScreenText.RemoveAt( posRemove );
		else break;
	}
	return posRemove;
}
