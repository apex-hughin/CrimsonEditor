#include "stdafx.h"
#include "cedtHeader.h"


BOOL CCedtApp::DoFindInFiles(LPCTSTR lpszFindString, LPCTSTR lpszFileType, LPCTSTR lpszFolder, BOOL bLookSubfolders, UINT nOptions)
{
	CWaitCursor wait; CRegExp clsRegExp; 

	if( HAS_REG_EXP(nOptions) ) { // compile regular expression
		CString szExpression = lpszFindString; 			szExpression.Replace( "\\\\", "\x1B" );
		szExpression.Replace( "\\s" , "[ \t\r\n]" );	szExpression.Replace( "\\S" , "[^ \t\r\n]" );
		szExpression.Replace( "\\w" , "[A-Za-z0-9]" );	szExpression.Replace( "\\W" , "[^A-Za-z0-9]" );
		szExpression.Replace( "\\a" , "[A-Za-z]" );		szExpression.Replace( "\\A" , "[^A-Za-z]" );
		szExpression.Replace( "\\d" , "[0-9]" );		szExpression.Replace( "\\D" , "[^0-9]" );
		szExpression.Replace( "\\h" , "[A-Fa-f0-9]" );	szExpression.Replace( "\\H" , "[^A-Fa-f0-9]" );
		szExpression.Replace( "\\t" , "\t" );			szExpression.Replace( "\x1B", "\\\\" );

		if( ! HAS_MATCH_CASE(nOptions) ) szExpression.MakeLower();
		if( ! clsRegExp.RegComp( szExpression ) ) return FALSE;
	}

	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pFrame );
	if( ! pFrame->IsOutputWindowVisible() ) pFrame->ShowOutputWindow(TRUE);

	pFrame->SetOutputWindowOccupied(TRUE);
	pFrame->EnableOutputWindowInput(FALSE);

	pFrame->ClearOutputWindowContents();

	CString szMessage; szMessage.LoadString(IDS_OUT_SEARCH_TITLE);
	pFrame->AddStringToOutputWindow( szMessage );

	szMessage.Format(IDS_OUT_SEARCH_BEGIN, lpszFindString);
	pFrame->AddStringToOutputWindow( szMessage );

	INT nFound = FindInFilesInFolder(lpszFindString, lpszFileType, lpszFolder, bLookSubfolders, nOptions, clsRegExp);

	if( nFound ) szMessage.Format(IDS_OUT_SEARCH_RESULT, nFound);
	else szMessage.Format(IDS_OUT_SEARCH_NOT_FOUND, lpszFindString);
	pFrame->AddStringToOutputWindow( szMessage );

	pFrame->SetOutputWindowOccupied(FALSE);
	pFrame->EnableOutputWindowInput(FALSE);

	return TRUE;
}

INT CCedtApp::FindInFilesInFolder(LPCTSTR lpszFindString, LPCTSTR lpszFileType, LPCTSTR lpszFolder, BOOL bLookSubfolders, UINT nOptions, CRegExp & clsRegExp)
{
	CFileFind find; BOOL bFound; INT nFound = 0;

	CString szFolder = lpszFolder; INT nLen = strlen(lpszFolder);
	if( szFolder[nLen-1] != '\\' ) szFolder += "\\";

	bFound = find.FindFile(szFolder + "*.*");
	while( bFound ) {
		bFound = find.FindNextFile();
		if( ! find.IsDirectory() && ! find.IsDots() && ! find.IsHidden() ) {
			CString szFilePath = find.GetFilePath();
			if( ! MatchFileFilter(szFilePath, lpszFileType) ) continue;
			nFound += FindInFilesInFile(lpszFindString, szFilePath, nOptions, clsRegExp);
		}
	}

	// do not look in sub folders
	if( ! bLookSubfolders ) return nFound;

	bFound = find.FindFile(szFolder + "*.*");
	while( bFound ) {
		bFound = find.FindNextFile();
		if( find.IsDirectory() && ! find.IsDots() && ! find.IsHidden() ) {
			CString szSubFolder = find.GetFilePath();
			nFound += FindInFilesInFolder(lpszFindString, lpszFileType, szSubFolder, bLookSubfolders, nOptions, clsRegExp);
		}
	}

	// return total sum of the count
	return nFound;
}

INT CCedtApp::FindInFilesInFile(LPCTSTR lpszFindString, LPCTSTR lpszFilePath, UINT nOptions, CRegExp & clsRegExp)
{
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd(); ASSERT( pFrame );
	CFile file; CString szMessage; INT nFound = 0;

	if( file.Open( lpszFilePath, CFile::modeRead | CFile::shareDenyNone ) ) {
		CArchive archive( & file, CArchive::load );
		CString szString; INT nIdxX, nIdxY = 0;

		while( archive.ReadString( szString ) ) {
			if( szString.FindOneOf( CONTROL_CHARS1 ) >= 0 ) {
			//	szMessage.Format(IDS_OUT_SEARCH_SKIP_BINARY, lpszFilePath);
			//	pFrame->AddStringToOutputWindow( szMessage ); // do not display message
				break;
			} else if( szString.GetLength() >= MAX_STRING_SIZE ) {
				szMessage.Format(IDS_OUT_SEARCH_LINE_EXCEED, MAX_STRING_SIZE, lpszFilePath, nIdxY+1);
				pFrame->AddStringToOutputWindow( szMessage );
			} else {
				if( ! HAS_REG_EXP(nOptions) ) nIdxX = ::ForwardFindString(szString, lpszFindString, 0, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
				else nIdxX = ::ForwardFindStringRegExp(szString, lpszFindString, clsRegExp, 0, HAS_WHOLE_WORD(nOptions), HAS_MATCH_CASE(nOptions));
				if( nIdxX >= 0 ) { // found it!
					szMessage.Format("%s(%d,%d): %s", lpszFilePath, nIdxY+1, nIdxX+1, szString);
					pFrame->AddStringToOutputWindow( szMessage ); nFound++;
				}
			}
			nIdxY++;
		}

		archive.Close();
		file.Close();
	} else {
		szMessage.Format(IDS_OUT_SEARCH_CANNOT_OPEN, lpszFilePath);
		pFrame->AddStringToOutputWindow( szMessage );
	}

	return nFound;
}

