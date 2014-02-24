#include "stdafx.h"
#include "cedtHeader.h"


static BOOL _CheckRange(BOOL bRange1, BOOL bRange2, UCHAR cRange)
{
	switch( cRange ) {
	case RT_GLOBAL: return TRUE;
	case RT_RANGE1: return bRange1;
	case RT_RANGE2: return bRange2;
	case RT_NR1AR2: return (! bRange1 && bRange2);
	case RT_NRNGE1: return (! bRange1);
	case RT_NRNGE2: return (! bRange2);
	case RT_R1ORR2: return (bRange1 || bRange2);
	default:        return FALSE;
	}
}

static UCHAR _CheckType(UCHAR cType, UCHAR cRange, BOOL & bQuotation1, BOOL & bQuotation2, BOOL & bQuotation3, BOOL & bLineComment, BOOL & bBlockComment, BOOL & bHighlight, BOOL & bShadow, BOOL & bRange1, BOOL & bRange2)
{
	switch( cType ) {
	case WT_COMMENTON:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment  && _CheckRange(bRange1, bRange2, cRange) ) { bBlockComment = TRUE;  return WT_LINECOMMENT; } else return WT_DELIMITER;
	case WT_COMMENTOFF:   if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 &&   bBlockComment && _CheckRange(bRange1, bRange2, cRange) ) { bBlockComment = FALSE; return WT_LINECOMMENT; } else return WT_DELIMITER;
	case WT_LINECOMMENT:  if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bBlockComment && _CheckRange(bRange1, bRange2, cRange) ) { bLineComment  = TRUE;  return WT_LINECOMMENT; } else return WT_DELIMITER;
	case WT_QUOTATION1:   if( ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment && _CheckRange(bRange1, bRange2, cRange) ) { bQuotation1 = ! bQuotation1; return WT_QUOTATION1; } else return WT_DELIMITER;
	case WT_QUOTATION2:   if( ! bQuotation1 && ! bQuotation3 && ! bLineComment && ! bBlockComment && _CheckRange(bRange1, bRange2, cRange) ) { bQuotation2 = ! bQuotation2; return WT_QUOTATION1; } else return WT_DELIMITER;
	case WT_QUOTATION3:   if( ! bQuotation1 && ! bQuotation2 && ! bLineComment && ! bBlockComment && _CheckRange(bRange1, bRange2, cRange) ) { bQuotation3 = ! bQuotation3; return WT_QUOTATION1; } else return WT_DELIMITER;
	case WT_SHADOWON:     if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bShadow = TRUE;  return WT_SHADOWON;
	case WT_SHADOWOFF:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bShadow = FALSE; return WT_SHADOWON;
	case WT_HIGHLIGHTON:  if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bHighlight = TRUE;  return WT_HIGHLIGHTON;
	case WT_HIGHLIGHTOFF: if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bHighlight = FALSE; return WT_HIGHLIGHTON;
	case WT_RANGE1BEG:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bRange1 = TRUE;  return WT_DELIMITER;
	case WT_RANGE1END:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment ) bRange1 = FALSE; return WT_DELIMITER;
	case WT_RANGE2BEG:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment && ! bRange1 ) bRange2 = TRUE;  return WT_DELIMITER;
	case WT_RANGE2END:    if( ! bQuotation1 && ! bQuotation2 && ! bQuotation3 && ! bLineComment && ! bBlockComment && ! bRange1 ) bRange2 = FALSE; return WT_DELIMITER;
	default:			  return cType;
	}
}


void CCedtView::DrawScreenBackgroundAndText() 
{
	CRect rect; GetClientRect( & rect );
	INT nCharHeight = GetCharHeight();		INT nLineHeight = GetLineHeight();
	INT nLeftMargin = GetLeftMargin();		INT nAveCharWidth = GetAveCharWidth();

	INT nNumWidth = GetNumberWidth();
	INT nSpaceWidth = GetSpaceWidth();
	CSize size; CString szLineNumber;

	BOOL bMultiLineStringConstant = MultiLineStringConstant();
	BOOL bVariableHighlightInString = VariableHighlightInString();
	BOOL bQuotation1 = FALSE, bQuotation2 = FALSE, bQuotation3 = FALSE, bLineComment = FALSE, bBlockComment = FALSE;
	BOOL bHighlight = FALSE, bShadow = FALSE, bRange1 = FALSE, bRange2 = FALSE;
	BOOL bQuotation1Save, bQuotation2Save, bQuotation3Save, bLineCommentSave, bBlockCommentSave;
	BOOL bHighlightSave, bShadowSave, bRange1Save, bRange2Save;

	INT nParaCount = 0, nLineCount = 0; SHORT sWordCount, sSpaceCount;
	INT nParaMax = GetLastIdxY(), nLineMax = GetLastPosY() / nLineHeight;
	SHORT sInfoFlags, sLineSplitIndex; UCHAR cType, cType2, cRange; INT nPosition, nWidth;
	INT nPosX = 0, nPosY = 0; BOOL bLineVisible, bWordVisible;

	// fill background
	m_dcScreen.SelectClipRgn( NULL, RGN_COPY );
	if( m_bShowLineNumbers || m_bShowSelectionMargin ) {
		m_dcScreen.FillSolidRect(0, 0, nLeftMargin-1, rect.bottom, m_crBkgrColor[1]);
		m_dcScreen.FillSolidRect(nLeftMargin-1, 0, rect.right-nLeftMargin+1, rect.bottom, m_crBkgrColor[0]);
	} else {
		m_dcScreen.FillSolidRect(0, 0, rect.right, rect.bottom, m_crBkgrColor[0]);
	}

	POSITION pos = m_clsFormatedScreenText.GetHeadPosition();
	while( pos ) {
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
		sInfoFlags = rLine.m_sInfoFlags; sLineSplitIndex = rLine.m_sLineSplitIndex;

		nPosY = nLineCount * nLineHeight - m_nScrollPosY;
		bLineVisible = (nPosY >= -nLineHeight) && (nPosY <= rect.Height());

		if( sLineSplitIndex == 0 ) bLineComment = FALSE;
		if( sLineSplitIndex == 0 && ! bMultiLineStringConstant ) bQuotation1 = bQuotation2 = bQuotation3 = FALSE;

		if( bLineVisible ) {
			// draw line number
			if( m_bShowLineNumbers && sLineSplitIndex == 0 ) {
				szLineNumber.Format("%d", nParaCount+1);
				size = m_dcScreen.GetTextExtent(szLineNumber);
				nPosX = nLeftMargin - size.cx - nNumWidth / 2;
				m_dcScreen.SetTextColor( m_crTextColor[WT_LINEFEED] );
				m_dcScreen.TextOut(nPosX, nPosY, szLineNumber);
			}

			// draw bookmark
			if( (sInfoFlags & LT_HASBOOKMARK) && sLineSplitIndex == 0 ) {
				if( m_bShowLineNumbers || m_bShowSelectionMargin ) {
					DrawScreenBookmark(nLeftMargin-5*nNumWidth/2+1, nPosY+1, 2*nNumWidth-2, nCharHeight-2);
				} else {
					COLORREF clr = RGB(0, 128, 128) ^ m_crBkgrColor[0];
					m_dcScreen.FillSolidRect(nLeftMargin, nPosY, rect.right-nLeftMargin, nCharHeight, clr);
				}
			}

			// exclude left margin from drawing
			m_dcScreen.ExcludeClipRect(0, nPosY, nLeftMargin, nPosY + nLineHeight);

			// save status of before drawing background
			bQuotation1Save = bQuotation1;	bQuotation2Save = bQuotation2;	bQuotation3Save = bQuotation3;	bLineCommentSave = bLineComment;	bBlockCommentSave = bBlockComment;
			bHighlightSave = bHighlight;	bShadowSave = bShadow;			bRange1Save = bRange1;			bRange2Save = bRange2;

			// drawing background - the reason why we split from drawing background from drawing text is that
			//                      when the font is italic drawing next background will erase part of previous text
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 
				nPosition = rWord.m_nPosition; nWidth = rWord.m_nWidth;

				nPosX = nLeftMargin + nPosition - m_nScrollPosX;
				if( cType == WT_RETURN || cType == WT_LINEFEED ) nWidth = rect.right - nPosX;
				bWordVisible = nPosX + nWidth >= nLeftMargin && nPosX <= rect.right;

				cType2 = _CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);

				if( m_bSyntaxHighlight && bWordVisible && (bRange1 || cType == WT_RANGE1END) && m_crBkgrColor[0] != m_crBkgrColor[3] ) {
					if( sWordCount == 0 && nPosition != 0 ) m_dcScreen.FillSolidRect(nLeftMargin, nPosY-(nLineHeight-nCharHeight)/2, nPosX-nLeftMargin, nLineHeight, m_crBkgrColor[0x03]);
					m_dcScreen.FillSolidRect(nPosX, nPosY-(nLineHeight-nCharHeight)/2, nWidth, nLineHeight, m_crBkgrColor[0x03]);
				}
				if( m_bSyntaxHighlight && bWordVisible && (bRange2 || cType == WT_RANGE2END) && ! bRange1 && m_crBkgrColor[0] != m_crBkgrColor[4] ) {
					if( sWordCount == 0 && nPosition != 0 ) m_dcScreen.FillSolidRect(nLeftMargin, nPosY-(nLineHeight-nCharHeight)/2, nPosX-nLeftMargin, nLineHeight, m_crBkgrColor[0x04]);
					m_dcScreen.FillSolidRect(nPosX, nPosY-(nLineHeight-nCharHeight)/2, nWidth, nLineHeight, m_crBkgrColor[0x04]);
				}
			}

			// restore status of before drawing text
			bQuotation1 = bQuotation1Save;	bQuotation2 = bQuotation2Save;	bQuotation3 = bQuotation3Save;	bLineComment = bLineCommentSave;	bBlockComment = bBlockCommentSave;
			bHighlight = bHighlightSave;	bShadow = bShadowSave;			bRange1 = bRange1Save;			bRange2 = bRange2Save;

			// drawing text
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 
				nPosition = rWord.m_nPosition; nWidth = rWord.m_nWidth;

				nPosX = nLeftMargin + nPosition - m_nScrollPosX;
				if( cType == WT_RETURN || cType == WT_LINEFEED ) nWidth = rect.right - nPosX;
				bWordVisible = nPosX + nWidth >= nLeftMargin && nPosX <= rect.right;
				cType2 = _CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);

				if( bWordVisible && cType != WT_LINEFEED ) {
					if( ! m_bSyntaxHighlight ) m_dcScreen.SetTextColor(m_crTextColor[WT_WORD]);
					else if( bLineComment || bBlockComment ) m_dcScreen.SetTextColor(m_crTextColor[WT_LINECOMMENT]);
					else if( bHighlight ) m_dcScreen.SetTextColor(m_crTextColor[WT_HIGHLIGHTON]);
					else if( bShadow ) m_dcScreen.SetTextColor(m_crTextColor[WT_SHADOWON]);
					else if( bQuotation1 || bQuotation2 || bQuotation3 ) {
						if( bVariableHighlightInString && cType2 == WT_VARIABLE ) m_dcScreen.SetTextColor(m_crTextColor[WT_VARIABLE] / 2 + m_crTextColor[WT_QUOTATION1] / 2);
						else m_dcScreen.SetTextColor(m_crTextColor[WT_QUOTATION1]);
					} else if( _IS_BET(WT_KEYWORD0, cType, WT_KEYWORD9) && cRange != RT_GLOBAL ) {
						if( _CheckRange(bRange1, bRange2, cRange) ) m_dcScreen.SetTextColor(m_crTextColor[cType2]);
						else m_dcScreen.SetTextColor(m_crTextColor[WT_WORD]);
					} else m_dcScreen.SetTextColor(m_crTextColor[cType2]);

					if( m_bShowLineBreak && cType == WT_RETURN && nParaCount != nParaMax ) {
						DrawScreenLineBreak(nPosX, nPosY, nSpaceWidth, nLineHeight);
					} else if( m_bShowTabChars && cType == WT_TAB ) {
						DrawScreenTabChar(nPosX, nPosY, nSpaceWidth, nLineHeight);
					} else if( m_bShowSpaces && cType == WT_SPACE ) {
						for(sSpaceCount = 0; sSpaceCount < rWord.m_sLength; sSpaceCount++) {
							DrawScreenSpace(nPosX + sSpaceCount * nSpaceWidth, nPosY, nSpaceWidth, nLineHeight);
						}
					} else if( cType != WT_RETURN && cType != WT_TAB && cType != WT_SPACE ) {
						// set font to italic
						if( m_bItalicizeComment && (bLineComment || bBlockComment || cType2 == WT_LINECOMMENT) ) m_dcScreen.SelectObject( & m_fontScreenIt );

						m_dcScreen.TextOut(nPosX, nPosY, (LPCTSTR)rLine + rWord.m_sIndex, rWord.m_sLength);

						if( m_bEmboldenKeywords && ! bLineComment && ! bBlockComment && ! bShadow && ! bQuotation1 && ! bQuotation2 && ! bQuotation3
							&& _IS_BET(WT_KEYWORD0, cType, WT_KEYWORD9) && _CheckRange(bRange1, bRange2, cRange) ) 
							m_dcScreen.TextOut(nPosX+1, nPosY, (LPCTSTR)rLine + rWord.m_sIndex, rWord.m_sLength);

						if( m_bLocalSpellCheck /* && ! bLineComment && ! bBlockComment && ! bQuotation1 && ! bQuotation2 */ && cType == WT_WRONGWORD ) 
							DrawScreenUnderbar(nPosX, nPosY+nCharHeight-1, rWord.m_nWidth, 1);

						// restore font from italic
						if( m_bItalicizeComment && (bLineComment || bBlockComment || cType2 == WT_LINECOMMENT) ) m_dcScreen.SelectObject( & m_fontScreen );
					}
				}
			}
		} else if( sInfoFlags & LT_HASRANGE || sInfoFlags & LT_HASCOMMENT || sInfoFlags & LT_HASHIGHLIGHT || sInfoFlags & LT_HASQUOTATION ) {
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 

				if( _IS_BET(WT_RANGE1BEG, cType, WT_RANGE2END) ||
					_IS_BET(WT_LINECOMMENT, cType, WT_COMMENTOFF) ||
					_IS_BET(WT_SHADOWON, cType, WT_HIGHLIGHTOFF) ||
					_IS_BET(WT_QUOTATION1, cType, WT_QUOTATION3) )
					_CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);
			}
		}

		nLineCount++;
		if( sLineSplitIndex == 0 ) nParaCount++;
	}
}


void CCedtView::InvertScreenSelected()
{
	RECT rect; GetClientRect( & rect );
	INT nCharHeight = GetCharHeight();		INT nLineHeight = GetLineHeight();
	INT nLeftMargin = GetLeftMargin();		INT nAveCharWidth = GetAveCharWidth();

	INT nBegX, nBegY, nEndX, nEndY;			
	GetSelectedPosition( nBegX, nBegY, nEndX, nEndY );

	RECT rectInvert;
	INT nParaCount = 0, nLineCount = 0;
//	SHORT siLineSplitIndex; 
	INT nCurY, nPosY; BOOL bLineVisible;

	POSITION pos = m_clsFormatedScreenText.GetHeadPosition();
	while( pos ) {
		CFormatedString & rLine = m_clsFormatedScreenText.GetNext( pos );
	//	siLineSplitIndex = rLine.m_siLineSplitIndex;

		nPosY = nLineCount * nLineHeight - m_nScrollPosY;
		bLineVisible = (nPosY >= -nLineHeight) && (nPosY <= rect.bottom);

		if( bLineVisible ) {
			nCurY = nLineCount * nLineHeight;

			if( nCurY >= nBegY && nCurY <= nEndY ) { // in block
				rectInvert.top = nPosY;
				rectInvert.bottom = nPosY + nCharHeight;

				if( m_bColumnMode ) {
					rectInvert.left = nBegX - m_nScrollPosX + nLeftMargin;
					if( nBegX == nEndX && nCurY != m_nCaretPosY ) {
						INT nCaretWidth = 25 * nAveCharWidth / 100; // default caret width
						rectInvert.right = rectInvert.left + nCaretWidth;
					} else rectInvert.right = nEndX - m_nScrollPosX + nLeftMargin;
				} else {
					if( nCurY == nBegY && nCurY == nEndY ) {
						rectInvert.left = nBegX - m_nScrollPosX + nLeftMargin;
						rectInvert.right = nEndX - m_nScrollPosX + nLeftMargin;
					} else if( nCurY == nBegY ) {
						rectInvert.left = nBegX - m_nScrollPosX + nLeftMargin;
						rectInvert.right = GetLastPosX(rLine) - m_nScrollPosX + nLeftMargin + nAveCharWidth;
					} else if( nCurY == nEndY ) {
						rectInvert.left = GetFirstPosX(rLine) - m_nScrollPosX + nLeftMargin;
						rectInvert.right = nEndX - m_nScrollPosX + nLeftMargin;
					} else {
						rectInvert.left = GetFirstPosX(rLine) - m_nScrollPosX + nLeftMargin;
						rectInvert.right = GetLastPosX(rLine) - m_nScrollPosX + nLeftMargin + nAveCharWidth;
					}
				}

				if( rectInvert.top    < rect.top    ) rectInvert.top    = rect.top;
				if( rectInvert.bottom > rect.bottom ) rectInvert.bottom = rect.bottom;
				if( rectInvert.left   < rect.left   ) rectInvert.left   = rect.left;
				if( rectInvert.right  > rect.right  ) rectInvert.right  = rect.right;

				m_dcScreen.InvertRect( & rectInvert );
			}
		}

		nLineCount++;
	//	if( siLineSplitIndex == 0 ) nParaCount++;
	}
}


void CCedtView::DrawPrintPageOutline(CDC * pDC, RECT rectDraw, INT nCurPage)
{
	CRect rect( rectDraw );
	INT nLineHeight = GetLineHeight( pDC );
	INT nPosX, nPosY; CSize size;

	CPen pen( PS_SOLID, 1, RGB(0, 0, 0) );
	CPen * pPenOld = pDC->SelectObject( & pen );
	pDC->SetTextColor( RGB(0, 0, 0) );

	if( m_bPrintHeader ) {
		nPosY = - rect.top;
		nPosX = rect.left;
		pDC->TextOut(nPosX, nPosY, m_szHeaderString[0]);

		size = pDC->GetTextExtent(m_szHeaderString[1]);
		nPosX = rect.left + (rect.Width() - size.cx) / 2;
		pDC->TextOut(nPosX, nPosY, m_szHeaderString[1]);

		size = pDC->GetTextExtent(m_szHeaderString[2]);
		nPosX = rect.right - size.cx;
		pDC->TextOut(nPosX, nPosY, m_szHeaderString[2]);

		nPosY = - rect.top - 4 * nLineHeight / 3;
		pDC->MoveTo(rect.left, nPosY);
		pDC->LineTo(rect.right, nPosY);
	}

	if( m_bPrintFooter ) {
		nPosY = - rect.bottom + 4 * nLineHeight / 3;
		pDC->MoveTo(rect.left, nPosY);
		pDC->LineTo(rect.right, nPosY);

		nPosY = - rect.bottom + nLineHeight;
		nPosX = rect.left;
		pDC->TextOut(nPosX, nPosY, m_szFooterString[0]);

		size = pDC->GetTextExtent(m_szFooterString[1]);
		nPosX = rect.left + (rect.Width() - size.cx) / 2;
		pDC->TextOut(nPosX, nPosY, m_szFooterString[1]);

		size = pDC->GetTextExtent(m_szFooterString[2]);
		nPosX = rect.right - size.cx;
		pDC->TextOut(nPosX, nPosY, m_szFooterString[2]);
	}

	pDC->SelectObject( pPenOld );
}


void CCedtView::DrawPrintPageBackgroundAndText(CDC * pDC, RECT rectDraw, INT nCurPage)
{
	CRect rect( rectDraw );
	INT nCharHeight = GetCharHeight(pDC);	INT nLineHeight = GetLineHeight(pDC);
	INT nLeftMargin = GetLeftMargin(pDC);	INT nAveCharWidth = GetAveCharWidth(pDC);

	INT nLinesPerPage = rect.Height() / nLineHeight;
	if( m_bPrintHeader ) { nLinesPerPage = nLinesPerPage - 2; rect.top = rect.top + 2 * nLineHeight; }
	if( m_bPrintFooter ) { nLinesPerPage = nLinesPerPage - 2; rect.bottom = rect.bottom - 2 * nLineHeight; }
	INT nScrollPosY = (nCurPage - 1) * nLinesPerPage * nLineHeight;

	INT nNumWidth = GetNumberWidth(pDC);
	INT nSpaceWidth = GetSpaceWidth(pDC);
	CSize size; CString szLineNumber;

	BOOL bMultiLineStringConstant = MultiLineStringConstant();
	BOOL bVariableHighlightInString = VariableHighlightInString();
	BOOL bQuotation1 = FALSE, bQuotation2 = FALSE, bQuotation3 = FALSE, bLineComment = FALSE, bBlockComment = FALSE;
	BOOL bHighlight = FALSE, bShadow = FALSE, bRange1 = FALSE, bRange2 = FALSE;
	BOOL bQuotation1Save, bQuotation2Save, bQuotation3Save, bLineCommentSave, bBlockCommentSave;
	BOOL bHighlightSave, bShadowSave, bRange1Save, bRange2Save;

	INT nParaCount = m_nFormatedPrintTextStartIdxY, nLineCount = 0; SHORT sWordCount;
	INT nParaMax = GetLastIdxY(), nLineMax = GetLastPosY() / nLineHeight;
	SHORT sInfoFlags, sLineSplitIndex; UCHAR cType, cType2, cRange; INT nPosition, nWidth;
	INT nPosX = 0, nPosY = 0; BOOL bLineVisible, bWordVisible;

	POSITION pos = m_clsFormatedPrintText.GetHeadPosition();
	while( pos ) {
		CFormatedString & rLine = m_clsFormatedPrintText.GetNext( pos );
		sInfoFlags = rLine.m_sInfoFlags; sLineSplitIndex = rLine.m_sLineSplitIndex;

		nPosY = nLineCount * nLineHeight - nScrollPosY;
		bLineVisible = (nPosY >= 0) && (nPosY <= rect.Height() - nLineHeight);

		if( sLineSplitIndex == 0 ) bLineComment = FALSE;
		if( sLineSplitIndex == 0 && ! bMultiLineStringConstant ) bQuotation1 = bQuotation2 = bQuotation3 = FALSE;

		if( bLineVisible ) {
			// draw line number
			if( m_bPrintLineNumbers && sLineSplitIndex == 0 ) {
				szLineNumber.Format("%d:", nParaCount+1);
				size = pDC->GetTextExtent(szLineNumber);
				nPosX = nLeftMargin - size.cx - nNumWidth / 1;
				pDC->SetTextColor( RGB(0, 0, 0) );
				pDC->TextOut(rect.left+nPosX, -rect.top-nPosY, szLineNumber);
			}

			// save status of before drawing background
			bQuotation1Save = bQuotation1;	bQuotation2Save = bQuotation2;	bQuotation3Save = bQuotation3;	bLineCommentSave = bLineComment;	bBlockCommentSave = bBlockComment;
			bHighlightSave = bHighlight;	bShadowSave = bShadow;			bRange1Save = bRange1;			bRange2Save = bRange2;

			// drawing background - the reason why we split from drawing background from drawing text is that
			//                      when the font is italic drawing next background will erase part of previous text
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 
				nPosition = rWord.m_nPosition; nWidth = rWord.m_nWidth;

				nPosX = nLeftMargin + nPosition;
				if( cType == WT_RETURN || cType == WT_LINEFEED ) nWidth = rect.right - rect.left - nPosX;
				bWordVisible = nPosX + nWidth >= nLeftMargin && nPosX <= rect.right;

				cType2 = _CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);

				if( m_bPrintSyntaxHighlight && bWordVisible && (bRange1 || cType == WT_RANGE1END) && m_crBkgrColor[0] != m_crBkgrColor[3] ) {
					if( sWordCount == 0 && nPosition != 0 ) pDC->FillSolidRect(rect.left+nLeftMargin, -(rect.top+nPosY+nCharHeight)-(nLineHeight-nCharHeight)/2, nPosX-nLeftMargin, nLineHeight, m_crBkgrColor[0x03]);
					pDC->FillSolidRect(rect.left+nPosX, -(rect.top+nPosY+nCharHeight)-(nLineHeight-nCharHeight)/2, nWidth, nLineHeight, m_crBkgrColor[0x03]);
				}
				if( m_bPrintSyntaxHighlight && bWordVisible && (bRange2 || cType == WT_RANGE2END) && ! bRange1 && m_crBkgrColor[0] != m_crBkgrColor[4] ) {
					if( sWordCount == 0 && nPosition != 0 ) pDC->FillSolidRect(rect.left+nLeftMargin, -(rect.top+nPosY+nCharHeight)-(nLineHeight-nCharHeight)/2, nPosX-nLeftMargin, nLineHeight, m_crBkgrColor[0x04]);
					pDC->FillSolidRect(rect.left+nPosX, -(rect.top+nPosY+nCharHeight)-(nLineHeight-nCharHeight)/2, nWidth, nLineHeight, m_crBkgrColor[0x04]);
				}
			}

			// restore status of before drawing text
			bQuotation1 = bQuotation1Save;	bQuotation2 = bQuotation2Save;	bQuotation3 = bQuotation3Save;	bLineComment = bLineCommentSave;	bBlockComment = bBlockCommentSave;
			bHighlight = bHighlightSave;	bShadow = bShadowSave;			bRange1 = bRange1Save;			bRange2 = bRange2Save;

			// drawing text
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 
				nPosition = rWord.m_nPosition; nWidth = rWord.m_nWidth;

				nPosX = nLeftMargin + nPosition;
				if( cType == WT_RETURN || cType == WT_LINEFEED ) nWidth = rect.right - rect.left - nPosX;
				bWordVisible = nPosX + nWidth >= nLeftMargin && nPosX <= rect.right;
				cType2 = _CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);

				if( bWordVisible && cType != WT_LINEFEED ) {
					if( ! m_bPrintSyntaxHighlight ) pDC->SetTextColor(RGB(0, 0, 0));
					else if( bLineComment || bBlockComment ) pDC->SetTextColor(m_crTextColor[WT_LINECOMMENT]);
					else if( bHighlight ) pDC->SetTextColor(m_crTextColor[WT_HIGHLIGHTON]);
					else if( bShadow ) pDC->SetTextColor(m_crTextColor[WT_SHADOWON]);
					else if( bQuotation1 || bQuotation2 || bQuotation3 ) {
						if( bVariableHighlightInString && cType2 == WT_VARIABLE ) m_dcScreen.SetTextColor(m_crTextColor[WT_VARIABLE] / 2 + m_crTextColor[WT_QUOTATION1] / 2);
						else pDC->SetTextColor(m_crTextColor[WT_QUOTATION1]);
					} else if( _IS_BET(WT_KEYWORD0, cType, WT_KEYWORD9) && cRange != RT_GLOBAL ) {
						if( _CheckRange(bRange1, bRange2, cRange) ) pDC->SetTextColor(m_crTextColor[cType2]);
						else pDC->SetTextColor(m_crTextColor[WT_WORD]);
					} else pDC->SetTextColor(m_crTextColor[cType2]);

					if( cType != WT_RETURN && cType != WT_TAB && cType != WT_SPACE ) {
						// set font to italic
						if( m_bItalicizeComment && (bLineComment || bBlockComment || cType2 == WT_LINECOMMENT) ) pDC->SelectObject( & m_fontPrinterIt );

						pDC->TextOut(rect.left+nPosX, -(rect.top+nPosY), (LPCTSTR)rLine + rWord.m_sIndex, rWord.m_sLength);

						if( m_bEmboldenKeywords && ! bLineComment && ! bBlockComment && ! bShadow && ! bQuotation1 && ! bQuotation2 && ! bQuotation3
							&& _IS_BET(WT_KEYWORD0, cType, WT_KEYWORD9) && _CheckRange(bRange1, bRange2, cRange) ) 
							pDC->TextOut(rect.left+nPosX+nAveCharWidth/8, -(rect.top+nPosY), (LPCTSTR)rLine + rWord.m_sIndex, rWord.m_sLength);

						// restore font from italic
						if( m_bItalicizeComment && (bLineComment || bBlockComment || cType2 == WT_LINECOMMENT) ) pDC->SelectObject( & m_fontPrinter );
					}
				}
			}
		} else if( sInfoFlags & LT_HASRANGE || sInfoFlags & LT_HASCOMMENT || sInfoFlags & LT_HASHIGHLIGHT || sInfoFlags & LT_HASQUOTATION ) {
			for(sWordCount = 0; sWordCount < rLine.m_sWordCount; sWordCount++) {
				FORMATEDWORDINFO & rWord = rLine.m_pWordInfo[sWordCount];
				cType = rWord.m_cType; cRange = rWord.m_cRange; 

				if( _IS_BET(WT_RANGE1BEG, cType, WT_RANGE2END) ||
					_IS_BET(WT_LINECOMMENT, cType, WT_COMMENTOFF) ||
					_IS_BET(WT_SHADOWON, cType, WT_HIGHLIGHTOFF) ||
					_IS_BET(WT_QUOTATION1, cType, WT_QUOTATION3) )
					_CheckType(cType, cRange, bQuotation1, bQuotation2, bQuotation3, bLineComment, bBlockComment, bHighlight, bShadow, bRange1, bRange2);
			}
		}

		nLineCount++;
		if( sLineSplitIndex == 0 ) nParaCount++;
	}
}


void CCedtView::DrawActiveLineHighlight()
{
	RECT rect; GetClientRect( & rect );
	INT nCharHeight = GetCharHeight();
	INT nLeftMargin = GetLeftMargin();
	INT nPosY = m_nCaretPosY - m_nScrollPosY;

	if( nPosY >= 0 && nPosY <= rect.bottom ) {
		INT nLineWidth  = rect.right - nLeftMargin;
	//	INT nLineHeight = GetLineHeight();

		m_dcActiveLine.BitBlt(nLeftMargin, nCharHeight+1, nLineWidth, nCharHeight+1, & m_dcScreen, nLeftMargin, nPosY, SRCCOPY);

		m_dcActiveLine.FillSolidRect(nLeftMargin, 0, nLineWidth, nCharHeight, m_crBkgrColor[0]);
		m_dcActiveLine.BitBlt(nLeftMargin, nCharHeight+1, nLineWidth, nCharHeight+1, & m_dcActiveLine, nLeftMargin, 0, SRCINVERT);
		m_dcActiveLine.FillSolidRect(nLeftMargin, 0, nLineWidth, nCharHeight, m_crBkgrColor[2]);
		m_dcActiveLine.BitBlt(nLeftMargin, nCharHeight+1, nLineWidth, nCharHeight+1, & m_dcActiveLine, nLeftMargin, 0, SRCINVERT);

		if( m_bHighlightActiveLine ) {
			for(INT i = nLeftMargin; i < rect.right; i += 2) {
				m_dcActiveLine.SetPixelV(i, nCharHeight+1, m_crTextColor[WT_WORD]);
				m_dcActiveLine.SetPixelV(i, nCharHeight+1+nCharHeight, m_crTextColor[WT_WORD]);
			}
		}

		m_dcActiveLine.BitBlt(nLeftMargin, 0, nLineWidth, nCharHeight+1, & m_dcScreen, nLeftMargin, nPosY, SRCCOPY);
		m_dcScreen.BitBlt(nLeftMargin, nPosY, nLineWidth, nCharHeight+1, & m_dcActiveLine, nLeftMargin, nCharHeight+1, SRCCOPY);

		m_bActiveLineHighlighted = TRUE;
		m_nActiveLineHighlightedPosY = nPosY;
	} else {
		m_bActiveLineHighlighted = FALSE;
		m_nActiveLineHighlightedPosY = 0;
	}
}

void CCedtView::DrawColumnMarkerHighlight()
{
	RECT rect; GetClientRect( & rect );
	INT nLeftMargin = GetLeftMargin();
	INT nAveCharWidth = GetAveCharWidth();
	INT nPosX, nPosY;

	nPosX = nLeftMargin + m_nColumnMarker1Pos * nAveCharWidth - m_nScrollPosX;
	if( m_bShowColumnMarker1 && (nPosX >= nLeftMargin && nPosX <= rect.right) ) {
		for( nPosY = 0; nPosY < rect.bottom; nPosY += 2 ) m_dcScreen.SetPixelV(nPosX, nPosY, m_crTextColor[WT_WORD]);
	}

	nPosX = nLeftMargin + m_nColumnMarker2Pos * nAveCharWidth - m_nScrollPosX;
	if( m_bShowColumnMarker2 && (nPosX >= nLeftMargin && nPosX <= rect.right) ) {
		for( nPosY = 0; nPosY < rect.bottom; nPosY += 2 ) m_dcScreen.SetPixelV(nPosX, nPosY, m_crTextColor[WT_WORD]);
	}
}

void CCedtView::DrawScreenBookmark(INT nPosX, INT nPosY, INT nWidth, INT nHeight)
{
	POINT points[4];
	points[0].x = nPosX;			points[0].y = nPosY;
	points[1].x = nPosX + nWidth;	points[1].y = nPosY + nHeight / 2;
	points[2].x = nPosX;			points[2].y = nPosY + nHeight;

	CPen * penOld, pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush * brushOld, brush(RGB(0, 255, 0));
	penOld = m_dcScreen.SelectObject( & pen );
	brushOld = m_dcScreen.SelectObject( & brush );
	m_dcScreen.FillSolidRect(nPosX, nPosY, nWidth, nHeight, m_crBkgrColor[1]);
	m_dcScreen.Polygon( points, 3 );
	m_dcScreen.SelectObject( penOld );
	m_dcScreen.SelectObject( brushOld );
}

void CCedtView::DrawScreenUnderbar(INT nPosX, INT nPosY, INT nWidth, INT nHeight)
{
	CPen * penOld, pen(PS_SOLID, 1, RGB(255, 0, 0));
	penOld = m_dcScreen.SelectObject( & pen );

	if( nHeight == 0 ) nHeight = 1;
	INT i, nCount = nWidth / nHeight;

	m_dcScreen.MoveTo(nPosX, nPosY);
	for(i = 1; i <= nCount; i++) {
		m_dcScreen.LineTo(nPosX + i * nHeight, (i % 2) ? nPosY - nHeight : nPosY);
	}

	m_dcScreen.SelectObject( penOld );
}

void CCedtView::DrawScreenSpace(INT nPosX, INT nPosY, INT nWidth, INT nHeight)
{
	m_dcScreen.SetPixelV( nPosX + nWidth / 2,     nPosY + nHeight / 2,     RGB(127, 127, 127) );
	m_dcScreen.SetPixelV( nPosX + nWidth / 2,     nPosY + nHeight / 2 - 1, RGB(127, 127, 127) );
	m_dcScreen.SetPixelV( nPosX + nWidth / 2 - 1, nPosY + nHeight / 2 - 1, RGB(127, 127, 127) );
	m_dcScreen.SetPixelV( nPosX + nWidth / 2 - 1, nPosY + nHeight / 2,     RGB(127, 127, 127) );
}

void CCedtView::DrawScreenTabChar(INT nPosX, INT nPosY, INT nWidth, INT nHeight)
{
	if( nWidth < 8 ) nWidth = 8;
	INT nCenY = nPosY + nHeight / 2;
	INT nDelY = 4 * nWidth / 8;

	CPen * penOld, pen(PS_SOLID, 1, RGB(127, 127, 127));
	penOld = m_dcScreen.SelectObject( & pen );

	m_dcScreen.MoveTo(nPosX + 4 * nWidth / 8, nCenY);
	m_dcScreen.LineTo(nPosX + 0 * nWidth / 8, nCenY - nDelY);
	m_dcScreen.MoveTo(nPosX + 4 * nWidth / 8, nCenY);
	m_dcScreen.LineTo(nPosX + 0 * nWidth / 8, nCenY + nDelY);

	m_dcScreen.MoveTo(nPosX + 7 * nWidth / 8, nCenY);
	m_dcScreen.LineTo(nPosX + 3 * nWidth / 8, nCenY - nDelY);
	m_dcScreen.MoveTo(nPosX + 7 * nWidth / 8, nCenY);
	m_dcScreen.LineTo(nPosX + 3 * nWidth / 8, nCenY + nDelY);

	m_dcScreen.SelectObject( penOld );
}

void CCedtView::DrawScreenLineBreak(INT nPosX, INT nPosY, INT nWidth, INT nHeight)
{
	if( nWidth < 8 ) nWidth = 8;

	CPen * penOld, pen(PS_SOLID, 1, RGB(127, 127, 127));
	penOld = m_dcScreen.SelectObject( & pen );

	m_dcScreen.Arc(nPosX +  1 * nWidth / 8, nPosY + 2 * nHeight / 10,
				   nPosX +  9 * nWidth / 8, nPosY + 6 * nHeight / 10,
				   nPosX +  6 * nWidth / 8, nPosY + 2 * nHeight / 10,
				   nPosX +  7 * nWidth / 8, nPosY + 6 * nHeight / 10);

	m_dcScreen.MoveTo(nPosX +  6 * nWidth / 8, nPosY + 2 * nHeight / 10);
	m_dcScreen.LineTo(nPosX +  6 * nWidth / 8, nPosY + 8 * nHeight / 10);
	m_dcScreen.MoveTo(nPosX +  8 * nWidth / 8, nPosY + 2 * nHeight / 10);
	m_dcScreen.LineTo(nPosX +  8 * nWidth / 8, nPosY + 8 * nHeight / 10);

	m_dcScreen.MoveTo(nPosX +  9 * nWidth / 8, nPosY + 2 * nHeight / 10);
	m_dcScreen.LineTo(nPosX +  6 * nWidth / 8, nPosY + 2 * nHeight / 10); 
	m_dcScreen.MoveTo(nPosX +  9 * nWidth / 8, nPosY + 8 * nHeight / 10);
	m_dcScreen.LineTo(nPosX +  3 * nWidth / 8, nPosY + 8 * nHeight / 10); 

	m_dcScreen.SelectObject( penOld );
}


void CCedtView::ParsePageHeaderAndFooter(INT nCurPage, INT nMaxPage)
{
	CString szPathName, szFileName, szPageNumber, szTotalPage, szCurrTime, szCurrDate;
	CCedtDoc * pDoc = (CCedtDoc *)GetDocument();

	szPathName = pDoc->GetTitle();
	szFileName = GetFileName( szPathName );

	szPageNumber.Format("%d", nCurPage );
	szTotalPage.Format("%d", nMaxPage );
	szCurrDate = GetCurrentDate();
	szCurrTime = GetCurrentTime();

	for(INT i = 0; i < 3; i++) {
		m_szHeaderString[i] = m_szHeaderFormat[i];
		m_szFooterString[i] = m_szFooterFormat[i];

		ParsePageHeaderAndFooter(m_szHeaderString[i], szPathName, szFileName, szPageNumber, szTotalPage, szCurrDate, szCurrTime);
		ParsePageHeaderAndFooter(m_szFooterString[i], szPathName, szFileName, szPageNumber, szTotalPage, szCurrDate, szCurrTime);
	}
}

void CCedtView::ParsePageHeaderAndFooter(CString & szArgument, LPCTSTR lpszFilePath, LPCTSTR lpszFileName, 
										 LPCTSTR lpszPageNumber, LPCTSTR lpszTotalPage, LPCTSTR lpszCurrDate, LPCTSTR lpszCurrTime)
{
	INT nFound;
	while( (nFound = szArgument.Find("$(FilePath)"  )) >= 0 ) szArgument = szArgument.Left(nFound) + lpszFilePath   + szArgument.Mid(nFound + 11);
	while( (nFound = szArgument.Find("$(FileName)"  )) >= 0 ) szArgument = szArgument.Left(nFound) + lpszFileName   + szArgument.Mid(nFound + 11);
	while( (nFound = szArgument.Find("$(PageNumber)")) >= 0 ) szArgument = szArgument.Left(nFound) + lpszPageNumber + szArgument.Mid(nFound + 13);
	while( (nFound = szArgument.Find("$(TotalPage)" )) >= 0 ) szArgument = szArgument.Left(nFound) + lpszTotalPage  + szArgument.Mid(nFound + 12);
	while( (nFound = szArgument.Find("$(CurrDate)"  )) >= 0 ) szArgument = szArgument.Left(nFound) + lpszCurrDate   + szArgument.Mid(nFound + 11);
	while( (nFound = szArgument.Find("$(CurrTime)"  )) >= 0 ) szArgument = szArgument.Left(nFound) + lpszCurrTime   + szArgument.Mid(nFound + 11);
}

