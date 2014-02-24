#include "stdafx.h"
#include "cedtHeader.h"



static TCHAR _nPairCancelWith, _nPairLookingFor;
static BOOL _bPairFindForward;


BOOL CCedtDoc::OnePassFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp)
{
	INT nLine = nIdxY; POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		INT nFound, nFrom = (nLine == nIdxY) ? nIdxX : -1;

		if( ! HAS_REG_EXP(nOptions) ) nFound = ::ForwardFindString(rLine, lpszFindString, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
		else nFound = ::ForwardFindStringRegExp(rLine, lpszFindString, clsRegExp, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));

		if( nFound >= 0 ) { nIdxY = nLine; nIdxX = nFound; return TRUE; }
		else nLine++;
	}

	return FALSE;
}

BOOL CCedtDoc::ForwardFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp)
{
	INT nLine = nIdxY; POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		INT nFound, nFrom = (nLine == nIdxY) ? nIdxX : -1;

		if( ! HAS_REG_EXP(nOptions) ) nFound = ::ForwardFindString(rLine, lpszFindString, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
		else nFound = ::ForwardFindStringRegExp(rLine, lpszFindString, clsRegExp, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));

		if( nFound >= 0 ) { nIdxY = nLine; nIdxX = nFound; return TRUE; }
		else nLine++;
	}

	if( ! CCedtView::m_bSearchWrapAtEndOfFile ) return FALSE;

	nLine = 0; pos = m_clsAnalyzedText.GetHeadPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		INT nFound, nFrom = -1;

		if( ! HAS_REG_EXP(nOptions) ) nFound = ::ForwardFindString(rLine, lpszFindString, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
		else nFound = ::ForwardFindStringRegExp(rLine, lpszFindString, clsRegExp, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));

		if( nFound >= 0 ) { nIdxY = nLine; nIdxX = nFound; return TRUE; }
		else nLine++;
	}

	return FALSE;
}

BOOL CCedtDoc::ReverseFindString(INT & nIdxX, INT & nIdxY, LPCTSTR lpszFindString, UINT nOptions, CRegExp & clsRegExp)
{
	INT nLine = nIdxY; POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		INT nFound, nFrom = (nLine == nIdxY) ? nIdxX : -1;

		if( ! HAS_REG_EXP(nOptions) ) nFound = ::ReverseFindString(rLine, lpszFindString, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
		else nFound = ::ReverseFindStringRegExp(rLine, lpszFindString, clsRegExp, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));

		if( nFound >= 0 ) { nIdxY = nLine; nIdxX = nFound; return TRUE; }
		else nLine--;
	}

	if( ! CCedtView::m_bSearchWrapAtEndOfFile ) return FALSE;

	nLine = m_clsAnalyzedText.GetCount()-1; pos = m_clsAnalyzedText.GetTailPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		INT nFound, nFrom = -1;

		if( ! HAS_REG_EXP(nOptions) ) nFound = ::ReverseFindString(rLine, lpszFindString, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
		else nFound = ::ReverseFindStringRegExp(rLine, lpszFindString, clsRegExp, nFrom, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));

		if( nFound >= 0 ) { nIdxY = nLine; nIdxX = nFound; return TRUE; }
		else nLine--;
	}

	return FALSE;
}

void CCedtDoc::ToggleBookmark(INT nIdxY)
{
	CAnalyzedString & rLine = GetLineFromIdxY(nIdxY);

	if( rLine.m_sInfoFlags & LT_HASBOOKMARK ) rLine.m_sInfoFlags &= ~LT_HASBOOKMARK;
	else rLine.m_sInfoFlags |= LT_HASBOOKMARK;
}

BOOL CCedtDoc::FindNextBookmark(INT & nIdxY)
{
	INT nLine = nIdxY+1; POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY+1);

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		if( rLine.m_sInfoFlags & LT_HASBOOKMARK ) { nIdxY = nLine; return TRUE; }
		else nLine++;
	}

	nLine = 0; pos = m_clsAnalyzedText.GetHeadPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		if( rLine.m_sInfoFlags & LT_HASBOOKMARK ) { nIdxY = nLine; return TRUE; }
		else nLine++;
	}

	return FALSE;
}

BOOL CCedtDoc::FindPrevBookmark(INT & nIdxY)
{
	INT nLine = nIdxY-1; POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY-1);

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		if( rLine.m_sInfoFlags & LT_HASBOOKMARK ) { nIdxY = nLine; return TRUE; }
		else nLine--;
	}

	nLine = m_clsAnalyzedText.GetCount()-1; pos = m_clsAnalyzedText.GetTailPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		if( rLine.m_sInfoFlags & LT_HASBOOKMARK ) { nIdxY = nLine; return TRUE; }
		else nLine--;
	}

	return FALSE;
}


BOOL CCedtDoc::IsThisIndentOnChar(INT nIdxX, INT nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	CAnalyzedString & rLine = m_clsAnalyzedText.GetAt(pos);
	ANALYZEDWORDINFO & rWord = GetWordFromIdxX( rLine, nIdxX );

	if( rWord.m_cType != WT_DELIMITER ) return FALSE;
	TCHAR nChar = rLine[(INT)rWord.m_sIndex];

	if( nChar == m_clsLangSpec.m_chIndentationOn ) return TRUE;
	return FALSE;
}

BOOL CCedtDoc::IsThisIndentOffChar(INT nIdxX, INT nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	CAnalyzedString & rLine = m_clsAnalyzedText.GetAt(pos);
	ANALYZEDWORDINFO & rWord = GetWordFromIdxX( rLine, nIdxX );

	if( rWord.m_cType != WT_DELIMITER ) return FALSE;
	TCHAR nChar = rLine[(INT)rWord.m_sIndex];

	if( nChar == m_clsLangSpec.m_chIndentationOff ) return TRUE;
	return FALSE;
}


BOOL CCedtDoc::IsThisOneOfPairs(INT nIdxX, INT nIdxY, BOOL & bBeginning)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	CAnalyzedString & rLine = m_clsAnalyzedText.GetAt(pos);
	ANALYZEDWORDINFO & rWord = GetWordFromIdxX( rLine, nIdxX );

//	if( rWord.m_cType != WT_DELIMITER ) return FALSE;
	if( rWord.m_sLength != 1 ) return FALSE;
	TCHAR nChar = rLine.GetAt(rWord.m_sIndex);

	LPCTSTR PAIRS[3];
	PAIRS[0] = m_clsLangSpec.m_szPairs1;
	PAIRS[1] = m_clsLangSpec.m_szPairs2;
	PAIRS[2] = m_clsLangSpec.m_szPairs3;

	for(INT i = 0; i < 3; i++) {
		if( strlen(PAIRS[i]) != 2 ) continue;

		if( nChar == PAIRS[i][0] ) {
			_nPairCancelWith = PAIRS[i][0];	_nPairLookingFor = PAIRS[i][1];
			_bPairFindForward = TRUE;		bBeginning = TRUE;
			return TRUE;
		} 
		
		if( nChar == PAIRS[i][1] ) {
			_nPairCancelWith = PAIRS[i][1];	_nPairLookingFor = PAIRS[i][0];
			_bPairFindForward = FALSE;		bBeginning = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCedtDoc::FindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY)
{
	if( _bPairFindForward ) 
		return ForwardFindAnotherOneOfPairs(nIdxX, nIdxY);
	else 
		return ReverseFindAnotherOneOfPairs(nIdxX, nIdxY);
}


BOOL CCedtDoc::ForwardFindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	SHORT i, sWordCount, sBegin;
	INT nLine = nIdxY, nDepth = 0;

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		sWordCount = rLine.m_sWordCount;

		if( nLine == nIdxY ) {
			for(sBegin = sWordCount-1, i = 0; i < sWordCount; i++) {
				ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
				if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) { sBegin = i; break; }
			}
		} else sBegin = 0;

		for(i = sBegin; i < sWordCount; i++) {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		//	if( rWord.m_cType != WT_DELIMITER ) continue;
			if( rWord.m_sLength != 1 ) continue;

			if( rLine[(INT)rWord.m_sIndex] == _nPairCancelWith ) nDepth++;
			if( rLine[(INT)rWord.m_sIndex] == _nPairLookingFor ) nDepth--;

			if( nDepth == 0 ) {
				nIdxY = nLine;
				nIdxX = rWord.m_sIndex;
				return TRUE;
			}
		}
		nLine++;
	}

	return FALSE;
}

BOOL CCedtDoc::ReverseFindAnotherOneOfPairs(INT & nIdxX, INT & nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	SHORT i, sWordCount, sBegin;
	INT nLine = nIdxY, nDepth = 0;

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		sWordCount = rLine.m_sWordCount;

		if( nLine == nIdxY ) {
			for(sBegin = sWordCount-1, i = 0; i < sWordCount; i++) {
				ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
				if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) { sBegin = i; break; }
			}
		} else sBegin = sWordCount-1;

		for(i = sBegin; i >= 0; i--) {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		//	if( rWord.m_cType != WT_DELIMITER ) continue;
			if( rWord.m_sLength != 1 ) continue;

			if( rLine[(INT)rWord.m_sIndex] == _nPairCancelWith ) nDepth++;
			if( rLine[(INT)rWord.m_sIndex] == _nPairLookingFor ) nDepth--;

			if( nDepth == 0 ) {
				nIdxY = nLine;
				nIdxX = rWord.m_sIndex;
				return TRUE;
			}
		}
		nLine--;
	}

	return FALSE;
}

BOOL CCedtDoc::ForwardFindEndingPair(INT & nIdxX, INT & nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	SHORT i, sWordCount, sBegin; INT j, nLine = nIdxY;

	LPCTSTR PAIRS[3]; INT nPairs[3]; INT nDepth[3];
	PAIRS[0] = m_clsLangSpec.m_szPairs1; nPairs[0] = strlen(PAIRS[0]); nDepth[0] = 0;
	PAIRS[1] = m_clsLangSpec.m_szPairs2; nPairs[1] = strlen(PAIRS[1]); nDepth[1] = 0;
	PAIRS[2] = m_clsLangSpec.m_szPairs3; nPairs[2] = strlen(PAIRS[2]); nDepth[2] = 0;

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext(pos);
		sWordCount = rLine.m_sWordCount;

		if( nLine == nIdxY ) {
			for(sBegin = sWordCount-1, i = 0; i < sWordCount; i++) {
				ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
				if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) { sBegin = i; break; }
			}
		} else sBegin = 0;

		for(i = sBegin; i < sWordCount; i++) {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		//	if( rWord.m_cType != WT_DELIMITER ) continue;
			if( rWord.m_sLength != 1 ) continue;

			for(j = 0; j < 3; j++) {
				if( nPairs[j] != 2 ) continue;
				if( rLine[(INT)rWord.m_sIndex] == PAIRS[j][0] ) nDepth[j]++;
			}

			for(j = 0; j < 3; j++) {
				if( nPairs[j] != 2 ) continue;
				if( rLine[(INT)rWord.m_sIndex] == PAIRS[j][1] ) nDepth[j]--;

				if( nDepth[j] == -1 ) {
					nIdxY = nLine;
					nIdxX = rWord.m_sIndex;
					return TRUE;
				}
			}
		}
		nLine++;
	}

	return FALSE;
}

BOOL CCedtDoc::ReverseFindBeginningPair(INT & nIdxX, INT & nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	SHORT i, sWordCount, sBegin; INT j, nLine = nIdxY;

	LPCTSTR PAIRS[3]; INT nPairs[3]; INT nDepth[3];
	PAIRS[0] = m_clsLangSpec.m_szPairs1; nPairs[0] = strlen(PAIRS[0]); nDepth[0] = 0;
	PAIRS[1] = m_clsLangSpec.m_szPairs2; nPairs[1] = strlen(PAIRS[1]); nDepth[1] = 0;
	PAIRS[2] = m_clsLangSpec.m_szPairs3; nPairs[2] = strlen(PAIRS[2]); nDepth[2] = 0;

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetPrev(pos);
		sWordCount = rLine.m_sWordCount;

		if( nLine == nIdxY ) {
			for(sBegin = sWordCount-1, i = 0; i < sWordCount; i++) {
				ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
				if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) { sBegin = i; break; }
			}
		} else sBegin = sWordCount-1;

		for(i = sBegin; i >= 0; i--) {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		//	if( rWord.m_cType != WT_DELIMITER ) continue;
			if( rWord.m_sLength != 1 ) continue;

			for(j = 0; j < 3; j++) {
				if( nPairs[j] != 2 ) continue;
				if( rLine[(INT)rWord.m_sIndex] == PAIRS[j][1] ) nDepth[j]++;
			}

			for(j = 0; j < 3; j++) {
				if( nPairs[j] != 2 ) continue;
				if( rLine[(INT)rWord.m_sIndex] == PAIRS[j][0] ) nDepth[j]--;

				if( nDepth[j] == -1 ) {
					nIdxY = nLine;
					nIdxX = rWord.m_sIndex;
					return TRUE;
				}
			}
		}
		nLine--;
	}

	return FALSE;
}
