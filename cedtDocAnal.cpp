#include "stdafx.h"
#include "cedtHeader.h"


// temporary variables
static TCHAR _buffer[MAX_WORD_LENGTH+1];
static ANALYZEDWORDINFO _words[MAX_WORDS_COUNT+1];

// global settings
static CMap<CString, LPCTSTR, SHORT, SHORT &> * _pKEY;
static CMap<CString, LPCTSTR, SHORT, SHORT &> * _pDIC;
static BOOL _bCASESENSITIVE, _bDICLOADED;

// language specification
static TCHAR ESC, QU1, QU2, QU3;
static UCHAR rngQU1, rngLC1, rngBC1;

static LPCTSTR LF1, LF2, LC1, LC2, C1B, C1E, C2B, C2E;
static LPCTSTR SDB, SDE, HLB, HLE, R1B, R1E, R2B, R2E;
static LPCTSTR DEL, PRE, VAR, VEB, SVC, HEX;

static INT lenLF1, lenLF2, lenLC1, lenLC2, lenC1B, lenC1E, lenC2B, lenC2E;
static INT lenSDB, lenSDE, lenHLB, lenHLE, lenR1B, lenR1E, lenR2B, lenR2E;
static INT lenHEX;


static BOOL _KeyLookup(UCHAR & cType, UCHAR & cRange, TCHAR * pWord, SHORT sLength) 
{
	if( sLength > MAX_WORD_LENGTH ) return FALSE;

	strncpy(_buffer, pWord, sLength); _buffer[sLength] = '\0';
	if( ! _bCASESENSITIVE ) _strlwr(_buffer);

	BOOL bFound; SHORT sValue = 0x00;
	bFound = _pKEY->Lookup(_buffer, sValue);
	cType = LOBYTE((WORD)sValue); cRange = HIBYTE((WORD)sValue);

	return bFound;
}

static BOOL _DicLookup(TCHAR * pWord, SHORT sLength) 
{
	if( sLength > MAX_WORD_LENGTH ) return FALSE;

	strncpy(_buffer, pWord, sLength); _buffer[sLength] = '\0';
	_strlwr(_buffer);

	BOOL bFound; SHORT sValue = 0x00;
	bFound = _pDIC->Lookup(_buffer, sValue);

	return bFound;
}

static void _WordFound(SHORT windex, UCHAR cType, UCHAR cRange, SHORT sIndex, SHORT sLength)
{
	_words[windex].m_cType = cType;
	_words[windex].m_cRange = cRange;
	_words[windex].m_sIndex = sIndex;
	_words[windex].m_sLength = sLength;

//	TRACE2("Word Found: %d, %d\n", windex, sLength);
}

static void _FinishLine(SHORT wcount, BOOL bOverflow, CAnalyzedString & rLine)
{
	delete [] rLine.m_pWordInfo;
	rLine.m_pWordInfo = new ANALYZEDWORDINFO[wcount];
	memcpy(rLine.m_pWordInfo, _words, wcount * sizeof(ANALYZEDWORDINFO));

	rLine.m_sWordCount = wcount;
	rLine.m_sInfoFlags = rLine.m_sInfoFlags & LT_HASBOOKMARK;
	if( bOverflow ) rLine.m_sInfoFlags |= LT_LINEOVERFLOW;

	for(SHORT i = 0; i < wcount; i++) {
		switch( _words[i].m_cType ) {
		case WT_RANGE1BEG:		case WT_RANGE1END:		case WT_RANGE2BEG:		case WT_RANGE2END: 
			rLine.m_sInfoFlags |= LT_HASRANGE; break;
		case WT_LINECOMMENT:	case WT_COMMENTON:		case WT_COMMENTOFF: 
			rLine.m_sInfoFlags |= LT_HASCOMMENT; break;
		case WT_SHADOWON:		case WT_SHADOWOFF:		case WT_HIGHLIGHTON:	case WT_HIGHLIGHTOFF:
			rLine.m_sInfoFlags |= LT_HASHIGHLIGHT; break;
		case WT_QUOTATION1:		case WT_QUOTATION2:		case WT_QUOTATION3:
			rLine.m_sInfoFlags |= LT_HASQUOTATION; break;
		}
	}
}


#define _NEXT_WORD(addr)		{ beg = fwd; state = (addr); }
#define _ROLL_BACK(addr)		{ fwd = beg; state = (addr); }
#define _JUMP_ADDR(addr)		{            state = (addr); }

#define _CHCK_DBCS(ptr)			( bDBCS && IsDBCSLeadByte(* ptr) )
#define _CHCK_SIZE(ptr, len)	( ptr - str < MAX_STRING_SIZE - (len) )


static void _AnalyzeLine(CAnalyzedString & rLine, BOOL bDBCS) 
{
	INT is_finished = 0, state = 0; SHORT wcount = 0;
	UCHAR type, range; TCHAR * str, * beg, * fwd;

	str = beg = fwd = (TCHAR *)(LPCTSTR)rLine;

	while( ! is_finished ) {
		switch( state ) {
		
		case 0x0000: // CHECK EOS, OVERFLOW AND WHITE SPACE
			if( * fwd == '\0' ) {
				// end of line
				_WordFound(wcount++, WT_RETURN, RT_GLOBAL, beg-str, fwd-beg);
				is_finished =  1;
			} else if( fwd - str >= MAX_STRING_SIZE-1 ) {
				// line overflow
				_WordFound(wcount++, WT_RETURN, RT_GLOBAL, beg-str, fwd-beg);
				is_finished = -1;
			} else if( * fwd == '\t' && _CHCK_SIZE(fwd, 1) ) {
				fwd++; 
				_WordFound(wcount++, WT_TAB, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( * fwd == ' ' && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && * fwd == ' ' && _CHCK_SIZE(fwd, 1) ) fwd++;
				_WordFound(wcount++, WT_SPACE, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0100);
			}
			break;


		case 0x0100: // CHECK DOUBLE BYTE CHARACTERS AND ESCAPE CHARACTERS
			if( bDBCS && IsDBCSLeadByte(* fwd) && * (fwd+1) && _CHCK_SIZE(fwd, 2) ) {
				fwd += 2;
				_WordFound(wcount++, WT_DBCHAR, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( ESC && * fwd == ESC && * (fwd+1) && * (fwd+1) != '\t' && * (fwd+1) != ' ' && _CHCK_SIZE(fwd, 2) ) {
				fwd += 2; 
				_WordFound(wcount++, WT_CONSTANT, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0200);
			}
			break;


		case 0x0200: // CHECK QUOTATION MARK
			if( QU1 && * fwd == QU1 && _CHCK_SIZE(fwd, 1) ) {
				fwd++; 
				_WordFound(wcount++, WT_QUOTATION1, rngQU1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( QU2 && * fwd == QU2 && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_WordFound(wcount++, WT_QUOTATION2, rngQU1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( QU3 && * fwd == QU3 && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_WordFound(wcount++, WT_QUOTATION3, rngQU1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0300);
			}
			break;


		case 0x0300: // CHECK VARIOUS DELIMITERS

			// block comment delimiters
			if( C1B[0] && ! _strnicmp(fwd, C1B, lenC1B) && _CHCK_SIZE(fwd, lenC1B) ) { 
				fwd += lenC1B; 
				_WordFound(wcount++, WT_COMMENTON, rngBC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( C1E[0] && ! _strnicmp(fwd, C1E, lenC1E) && _CHCK_SIZE(fwd, lenC1E) ) {
				fwd += lenC1E; 
				_WordFound(wcount++, WT_COMMENTOFF, rngBC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( C2B[0] && ! _strnicmp(fwd, C2B, lenC2B) && _CHCK_SIZE(fwd, lenC2B) ) {
				fwd += lenC2B; 
				_WordFound(wcount++, WT_COMMENTON, rngBC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( C2E[0] && ! _strnicmp(fwd, C2E, lenC2E) && _CHCK_SIZE(fwd, lenC2E) ) {
				fwd += lenC2E; 
				_WordFound(wcount++, WT_COMMENTOFF, rngBC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);

			// line comment delimiters
			} else if( LC1[0] && isalpha(LC1[0]) && ! _strnicmp(fwd, LC1, lenLC1) && ! isalpha(fwd[lenLC1]) && _CHCK_SIZE(fwd, lenLC1) ) {
				fwd += lenLC1; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( LC1[0] && ! isalpha(LC1[0]) && ! _strnicmp(fwd, LC1, lenLC1) && _CHCK_SIZE(fwd, lenLC1) ) {
				fwd += lenLC1; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( LC2[0] && isalpha(LC2[0]) && ! _strnicmp(fwd, LC2, lenLC2) && ! isalpha(fwd[lenLC2]) && _CHCK_SIZE(fwd, lenLC2) ) {
				fwd += lenLC2; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( LC2[0] && ! isalpha(LC2[0]) && ! _strnicmp(fwd, LC2, lenLC2) && _CHCK_SIZE(fwd, lenLC2) ) {
				fwd += lenLC2; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( LF1[0] && fwd == str && ! _strnicmp(fwd, LF1, lenLF1) && _CHCK_SIZE(fwd, lenLF1) ) {
				fwd += lenLF1; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( LF2[0] && fwd == str && ! _strnicmp(fwd, LF2, lenLF2) && _CHCK_SIZE(fwd, lenLF2) ) {
				fwd += lenLF2; 
				_WordFound(wcount++, WT_LINECOMMENT, rngLC1, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);

			// highlight & shadow delimiters
			} else if( HLB[0] && ! _strnicmp(fwd, HLB, lenHLB) && _CHCK_SIZE(fwd, lenHLB) ) {
				fwd += lenHLB; 
				_WordFound(wcount++, WT_HIGHLIGHTON, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( HLE[0] && ! _strnicmp(fwd, HLE, lenHLE) && _CHCK_SIZE(fwd, lenHLE) ) {
				fwd += lenHLE; 
				_WordFound(wcount++, WT_HIGHLIGHTOFF, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( SDB[0] && ! _strnicmp(fwd, SDB, lenSDB) && _CHCK_SIZE(fwd, lenSDB) ) {
				fwd += lenSDB; 
				_WordFound(wcount++, WT_SHADOWON, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( SDE[0] && ! _strnicmp(fwd, SDE, lenSDE) && _CHCK_SIZE(fwd, lenSDE) ) {
				fwd += lenSDE; 
				_WordFound(wcount++, WT_SHADOWOFF, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);

			// range delimiters
			} else if( R1B[0] && ! _strnicmp(fwd, R1B, lenR1B) && _CHCK_SIZE(fwd, lenR1B) ) {
				fwd += lenR1B; 
				_WordFound(wcount++, WT_RANGE1BEG, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( R1E[0] && ! _strnicmp(fwd, R1E, lenR1E) && _CHCK_SIZE(fwd, lenR1E) ) {
				fwd += lenR1E; 
				_WordFound(wcount++, WT_RANGE1END, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( R2B[0] && ! _strnicmp(fwd, R2B, lenR2B) && _CHCK_SIZE(fwd, lenR2B) ) {
				fwd += lenR2B; 
				_WordFound(wcount++, WT_RANGE2BEG, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( R2E[0] && ! _strnicmp(fwd, R2E, lenR2E) && _CHCK_SIZE(fwd, lenR2E) ) {
				fwd += lenR2E; 
				_WordFound(wcount++, WT_RANGE2END, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0400);
			}
			break;


		case 0x0400: // CHECK HEXADECIMAL NUMBERS
			if( HEX[0] && ! _strnicmp(fwd, HEX, lenHEX) && _CHCK_SIZE(fwd, lenHEX) ) {
				fwd += lenHEX; while( * fwd && isxdigit(* fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_WordFound(wcount++, WT_CONSTANT, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0450);
			}
			break;


		case 0x0450: // NUMBERS AND FLOATING POINT NUMBERS
			if( isdigit(* fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && (isdigit(* fwd) || * fwd == '.') && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_JUMP_ADDR(0x0451);
			} else {
				_ROLL_BACK(0x0500);
			}
			break;

		case 0x0451:
			if( * fwd && (* fwd == 'E' || * fwd == 'e') && _CHCK_SIZE(fwd, 1) ) {
				fwd++; 
				_JUMP_ADDR(0x0452);
			} else {
				_WordFound(wcount++, WT_CONSTANT, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			}
			break;

		case 0x0452:
			if( * fwd && (* fwd == '+' || * fwd == '-' || isdigit(* fwd)) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && (isdigit(* fwd)) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_WordFound(wcount++, WT_CONSTANT, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_WordFound(wcount++, WT_CONSTANT, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			}
			break;


		case 0x0500: // CHECK KEYWORD BEGINNING WITH PREFIX
			if( PRE[0] && strchr(PRE, * fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; 
				_JUMP_ADDR(0x0501);
			} else {
				_ROLL_BACK(0x0600);
			}
			break;

		case 0x0501:
			if( _KeyLookup(type, range, beg, fwd-beg) ) {
				_WordFound(wcount++, type, range, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( * fwd && strchr( PRE, * fwd ) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_JUMP_ADDR(0x0501);
			} else {
				while( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_JUMP_ADDR(0x0502);
			}
			break;

		case 0x0502:
			if( _KeyLookup(type, range, beg, fwd-beg) ) {
				_WordFound(wcount++, type, range, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0600);
			}
			break;


		case 0x0600: // CHECK VARIABLE BEGINING WITH PREFIX
			if( VAR[0] && strchr(VAR, * fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_JUMP_ADDR(0x0601);
			} else {
				_ROLL_BACK(0x0700);
			}
			break;

		case 0x0601:
			if( * fwd && VEB[0] && * fwd == VEB[0] && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && ! isspace(* fwd) && ! strchr(VEB, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_JUMP_ADDR(0x0602);
			} else if( * fwd && SVC[0] && strchr(SVC, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && strchr(SVC, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_JUMP_ADDR(0x0603);
			} else if( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_WordFound(wcount++, WT_VARIABLE, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0700);
			}
			break;

		case 0x0602:
			if( * fwd && VEB[1] && * fwd == VEB[1] && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_WordFound(wcount++, WT_VARIABLE, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_ROLL_BACK(0x0700);
			}
			break;

		case 0x0603:
			if( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_WordFound(wcount++, WT_VARIABLE, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_WordFound(wcount++, WT_VARIABLE, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			}
			break;


		case 0x0700: // CHECK IDENTIFIER
			if( ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++; while( * fwd && ! isspace(* fwd) && ! strchr(DEL, * fwd) && ! _CHCK_DBCS(fwd) && _CHCK_SIZE(fwd, 1) ) fwd++;
				_JUMP_ADDR(0x0701);
			} else {
				_ROLL_BACK(0x0800);
			}
			break;

		case 0x0701:
			if( _KeyLookup(type, range, beg, fwd-beg) ) {
				_WordFound(wcount++, type, range, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else if( _bDICLOADED && ! _DicLookup(beg, fwd-beg) ) {
				_WordFound(wcount++, WT_WRONGWORD, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				_WordFound(wcount++, WT_WORD, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			}
			break;


		case 0x0800: // CHECK DELIMITERS
			if( isprint(* fwd) && strchr(DEL, * fwd) && _CHCK_SIZE(fwd, 1) ) {
				fwd++;
				_WordFound(wcount++, WT_DELIMITER, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			} else {
				fwd++;
				_WordFound(wcount++, WT_GRAPH, RT_GLOBAL, beg-str, fwd-beg);
				_NEXT_WORD(0x0000);
			}
			break;
		}
	}

	BOOL bOverflow = (is_finished < 0) ? TRUE : FALSE;
	_FinishLine(wcount, bOverflow, rLine);
}


INT CCedtDoc::GetCharType(TCHAR nChar)
{
	BOOL bDBCS = CCedtApp::m_bDoubleByteCharacterSet;
	DEL = m_clsLangSpec.m_szDelimiters;

	if( bDBCS && IsDBCSLeadByte(nChar)  ) return CH_CHARACTER;
	else if( isspace(nChar) || ! nChar  ) return CH_WHITESPACE;
	else if( isprint(nChar) && strchr(DEL, nChar) ) return CH_DELIMITER;
	else return CH_CHARACTER;
}


void CCedtDoc::AnalyzeText(INT nIndex, INT nCount) 
{
	// save settings to global variables
	_pKEY = & m_clsKeywords;						_bCASESENSITIVE = m_clsLangSpec.m_bCaseSensitive;
	_pDIC = & m_clsDictionary;						_bDICLOADED = m_bDictionaryLoaded;

	// langage specification
	ESC = m_clsLangSpec.m_chEscapeChar;				QU1 = m_clsLangSpec.m_chQuotationMark1;
	QU2 = m_clsLangSpec.m_chQuotationMark2;			QU3 = m_clsLangSpec.m_chQuotationMark3;

	rngQU1 = m_clsLangSpec.m_ucQuotationMarkRange;
	rngLC1 = m_clsLangSpec.m_ucLineCommentRange;
	rngBC1 = m_clsLangSpec.m_ucBlockCommentRange;

	LF1 = m_clsLangSpec.m_szLineComment1OnFirstPosition;
	LF2 = m_clsLangSpec.m_szLineComment2OnFirstPosition;
	LC1 = m_clsLangSpec.m_szLineComment1;			LC2 = m_clsLangSpec.m_szLineComment2;
	C1B = m_clsLangSpec.m_szBlockComment1On;		C1E = m_clsLangSpec.m_szBlockComment1Off;
	C2B = m_clsLangSpec.m_szBlockComment2On;		C2E = m_clsLangSpec.m_szBlockComment2Off;

	SDB = m_clsLangSpec.m_szShadowOn;				SDE = m_clsLangSpec.m_szShadowOff;
	HLB = m_clsLangSpec.m_szHighlightOn;			HLE = m_clsLangSpec.m_szHighlightOff;
	R1B = m_clsLangSpec.m_szRange1Beg;				R1E = m_clsLangSpec.m_szRange1End;
	R2B = m_clsLangSpec.m_szRange2Beg;				R2E = m_clsLangSpec.m_szRange2End;

	DEL = m_clsLangSpec.m_szDelimiters;				PRE = m_clsLangSpec.m_szKeywordPrefix;
	VAR = m_clsLangSpec.m_szVariablePrefix;			VEB = m_clsLangSpec.m_szVariableEnclosedBy;
	SVC = m_clsLangSpec.m_szSpecialVariableChars;	HEX = m_clsLangSpec.m_szHexaDecimalMark;

	lenLF1 = strlen(LF1);	lenLF2 = strlen(LF2);	lenLC1 = strlen(LC1);	lenLC2 = strlen(LC2);
	lenC1B = strlen(C1B);	lenC1E = strlen(C1E);	lenC2B = strlen(C2B);	lenC2E = strlen(C2E);
	lenSDB = strlen(SDB);	lenSDE = strlen(SDE);	lenHLB = strlen(HLB);	lenHLE = strlen(HLE);
	lenR1B = strlen(R1B);	lenR1E = strlen(R1E);	lenR2B = strlen(R2B);	lenR2E = strlen(R2E);
	lenHEX = strlen(HEX);

	// now start analyzing text
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pMainFrame );
	INT nProcess = 0; CWaitCursor * pWait = NULL;

	if( nCount > 1000 ) {
		pWait = new CWaitCursor;
		pMainFrame->BeginProgress("Analyzing...");
	}

	POSITION pos = m_clsAnalyzedText.FindIndex( nIndex );
	while( pos && nProcess < nCount ) {
		_AnalyzeLine( m_clsAnalyzedText.GetNext( pos ), CCedtApp::m_bDoubleByteCharacterSet );

		if( nCount > 1000 && ! (nProcess % 20) ) pMainFrame->SetProgress(100 * nProcess / nCount);
		nProcess++; 
	}

	if( nCount > 1000 ) {
		delete pWait;
		pMainFrame->EndProgress();
	}
}

