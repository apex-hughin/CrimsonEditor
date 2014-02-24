#include "stdafx.h"
#include "cedtHeader.h"


#define IS_COUNTABLE_WORD(rWord) ( \
  ( (rWord.m_cType) >= WT_KEYWORD0 && (rWord.m_cType) <= WT_KEYWORD9 ) || \
	(rWord.m_cType) == WT_CONSTANT || (rWord.m_cType) == WT_VARIABLE   || \
	(rWord.m_cType) == WT_WORD     || (rWord.m_cType) == WT_WRONGWORD  || \
	(rWord.m_cType) == WT_DBCHAR \
)

INT CCedtDoc::GetWordCount()
{
	INT nWordCount = 0;
	POSITION pos = m_clsAnalyzedText.GetHeadPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext( pos );
		for(SHORT i = 0; i < rLine.m_sWordCount; i++) {
			ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
			if( IS_COUNTABLE_WORD(rWord) ) nWordCount++;
		}
	}

	return nWordCount;
}

INT CCedtDoc::GetByteCount()
{
	INT nByteCount = 0, nLineFeed = 2;
	if( m_nFileFormat != FILE_FORMAT_DOS ) nLineFeed = 1;

	POSITION pos = m_clsAnalyzedText.GetHeadPosition();

	while( pos ) {
		CAnalyzedString & rLine = m_clsAnalyzedText.GetNext( pos );
		nByteCount += rLine.GetLength();
		if( pos ) nByteCount += nLineFeed;
	}

	return nByteCount;
}

INT CCedtDoc::GetFirstIdxX(CAnalyzedString & rLine)
{
	ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[0];
	return rWord.m_sIndex;
}

INT CCedtDoc::GetLastIdxX(CAnalyzedString & rLine)
{
	ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[rLine.m_sWordCount-1];
	return rWord.m_sIndex + rWord.m_sLength;
}

INT CCedtDoc::GetFirstNonBlankIdxX(CAnalyzedString & rLine)
{
	SHORT i, sCount = rLine.m_sWordCount;
	for(i = 0; i < sCount; i++) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return rWord.m_sIndex;
	}
	return GetLastIdxX(rLine);
}

INT CCedtDoc::GetTrailingBlankIdxX(CAnalyzedString & rLine)
{
	SHORT i, sCount = rLine.m_sWordCount;
	for(i = sCount-1; i >= 0; i--) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return rWord.m_sIndex + rWord.m_sLength;
	}
	return GetFirstIdxX(rLine);
}

CAnalyzedString & CCedtDoc::GetLineFromIdxY(INT nIdxY)
{
	POSITION pos = m_clsAnalyzedText.FindIndex(nIdxY);
	if( pos ) return m_clsAnalyzedText.GetAt(pos);
	return m_clsAnalyzedText.GetTail();
}

ANALYZEDWORDINFO & CCedtDoc::GetWordFromIdxX(CAnalyzedString & rLine, INT nIdxX)
{
	SHORT i, sCount = rLine.m_sWordCount;
	for(i = 0; i < sCount; i++) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i];
		if( rWord.m_sIndex + rWord.m_sLength > nIdxX ) return rLine.m_pWordInfo[i];
	}
	return rLine.m_pWordInfo[sCount-1];
}

BOOL CCedtDoc::IsBlankLine(CAnalyzedString & rLine)
{
	SHORT i, sCount = rLine.m_sWordCount;
	for(i = 0; i < sCount; i++) {
		ANALYZEDWORDINFO & rWord = rLine.m_pWordInfo[i]; UCHAR cType = rWord.m_cType;
		if( cType != WT_TAB && cType != WT_SPACE && cType != WT_RETURN && cType != WT_LINEFEED ) return FALSE;
	}
	return TRUE;
}

BOOL CCedtDoc::IsBlankLineFromIdxY(INT nIdxY)
{
	CAnalyzedString & rLine = GetLineFromIdxY( nIdxY );
	return IsBlankLine( rLine );
}
