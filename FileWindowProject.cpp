#include "stdafx.h"
#include "cedtHeader.h"


BOOL CFileWindow::InitProjectWorkspace()
{
	if( ! RemoveAllProjectItems() ) return FALSE;
	CString szItemText; szItemText.LoadString(IDS_MENU_NO_PROJECT_AVAILABLE);
	if( ! InsertProjectItem(TVI_ROOT, szItemText, PROJECT_ITEM_PROJECT, 0, "", 0) ) return FALSE;

	if( ! EnableAllProjectButtons(FALSE) ) return FALSE;
	return TRUE;
}

BOOL CFileWindow::NewProjectWorkspace(LPCTSTR lpszPathName)
{
	if( ! RemoveAllProjectItems() ) return FALSE;
	if( ! InsertProjectItem(TVI_ROOT, GetFileName(lpszPathName), PROJECT_ITEM_PROJECT, 0, lpszPathName, 0) ) return FALSE;

	if( ! EnableAllProjectButtons(TRUE) ) return FALSE;
	return TRUE;
}

BOOL CFileWindow::SaveProjectWorkspace(LPCTSTR lpszPathName)
{
	ofstream fout(lpszPathName, ios::out);
	if( ! fout.is_open() ) return FALSE;

	CString szContents;

	// save project
	szContents.Format("<project version=\"%s\">", STRING_PROJECTFILEVER);
	fout << szContents << endl;

	HTREEITEM hRoot = m_treProjectTree.GetRootItem();
	HTREEITEM hChild = m_treProjectTree.GetChildItem( hRoot );
	while( hChild ) { // recursive call to all child items
		if( ! SaveProjectItem(fout, 1, hChild) ) return FALSE;
		hChild = m_treProjectTree.GetNextSiblingItem( hChild );
	}

	fout << "</project>" << endl << endl;

	// save workspace
	szContents.Format("<workspace version=\"%s\">", STRING_PROJECTFILEVER);
	fout << szContents << endl;

	CCedtApp * pApp = (CCedtApp *)AfxGetApp();
	POSITION posDoc = pApp->GetFirstDocPosition();
	while( posDoc ) { // recursive call to all open documents
		CCedtDoc * pDoc = (CCedtDoc *)pApp->GetNextDoc( posDoc );
		if( ! SaveWorkspaceItem(fout, 1, pDoc) ) return FALSE;
	}

	fout << "</workspace>" << endl << endl;

	fout.close();
	return TRUE;
}

BOOL CFileWindow::OpenProjectWorkspace(LPCTSTR lpszPathName)
{
	if( ! RemoveAllProjectItems() ) return FALSE;
	ifstream fin(lpszPathName, ios::in | ios::nocreate);

	CMapStringToString mapAttr; TCHAR szText[4096]; 
	fin >> szText; // get first tocken

	// load project
	if( ! _stricmp(szText, "<project") ) {
		fin.getline(szText, 4096, '>'); // get attributes
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szVersion; BOOL bLookup = mapAttr.Lookup("version", szVersion);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		HTREEITEM hItem = InsertProjectItem(TVI_ROOT, GetFileName(lpszPathName), PROJECT_ITEM_PROJECT, 0, lpszPathName, 0);
		fin >> szText; // get next tocken

		while( _stricmp(szText, "</project>") ) {
			if( ! LoadProjectItem(fin, szText, hItem) ) return FALSE;
		}
		fin >> szText; // get next tocken

		// expand and select root item
		if( ! m_treProjectTree.Expand( hItem, TVE_EXPAND ) ) ; // there could be only one root item;
		if( ! m_treProjectTree.SelectItem( hItem ) ) return FALSE;

	} else { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

	// load workspace
	if( ! _stricmp(szText, "<workspace") ) {
		fin.getline(szText, 4096, '>'); // get attributes
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szVersion; BOOL bLookup = mapAttr.Lookup("version", szVersion);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CCedtApp * pApp = (CCedtApp *)AfxGetApp();
		fin >> szText; // get next tocken

		while( _stricmp(szText, "</workspace>") ) {
			if( ! LoadWorkspaceItem(fin, szText, pApp) ) return FALSE; 
		}
		fin >> szText; // get next tocken

	} else { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

	if( ! EnableAllProjectButtons(TRUE) ) return FALSE;

	fin.close();
	return TRUE;
}

BOOL CFileWindow::SaveRegularWorkspace(LPCTSTR lpszPathName)
{
	ofstream fout(lpszPathName, ios::out);
	if( ! fout.is_open() ) return FALSE;

	CString szContents;

	// save workspace
	szContents.Format("<workspace version=\"%s\">", STRING_PROJECTFILEVER);
	fout << szContents << endl;

	CCedtApp * pApp = (CCedtApp *)AfxGetApp();
	POSITION posDoc = pApp->GetFirstDocPosition();
	while( posDoc ) { // recursive call to all open documents
		CCedtDoc * pDoc = (CCedtDoc *)pApp->GetNextDoc( posDoc );
		if( ! SaveWorkspaceItem(fout, 1, pDoc) ) return FALSE;
	}

	fout << "</workspace>" << endl << endl;

	fout.close();
	return TRUE;
}

BOOL CFileWindow::OpenRegularWorkspace(LPCTSTR lpszPathName)
{
	if( ! RemoveAllProjectItems() ) return FALSE;
	ifstream fin(lpszPathName, ios::in | ios::nocreate);

	CMapStringToString mapAttr; TCHAR szText[4096]; 
	fin >> szText; // get first tocken

	// load workspace
	if( ! _stricmp(szText, "<workspace") ) {
		fin.getline(szText, 4096, '>'); // get attributes
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szVersion; BOOL bLookup = mapAttr.Lookup("version", szVersion);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CCedtApp * pApp = (CCedtApp *)AfxGetApp();
		fin >> szText; // get next tocken

		while( _stricmp(szText, "</workspace>") ) {
			if( ! LoadWorkspaceItem(fin, szText, pApp) ) return FALSE; 
		}
		fin >> szText; // get next tocken
	} else { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

	if( ! EnableAllProjectButtons(FALSE) ) return FALSE;

	fin.close();
	return TRUE;
}

BOOL CFileWindow::AddCategoryToProject(LPCTSTR lpszCategory)
{
	HTREEITEM hParent = m_treProjectTree.GetSelectedItem();
	if( ! hParent ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	// if selected item is not a category, then get parent item
	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hParent, nImage, nSelectedImage);
	if( nImage != PROJECT_ITEM_PROJECT && nImage != PROJECT_ITEM_CATEGORY ) hParent = m_treProjectTree.GetParentItem(hParent);

	HTREEITEM hFound = FindChildProjectItem(hParent, lpszCategory);
	if( hFound ) { AfxMessageBox(IDS_ERR_DUPLICATE_PRJ_ITEM); return FALSE; }

	HTREEITEM hInsert = InsertProjectItem(hParent, lpszCategory, PROJECT_ITEM_CATEGORY, 0, "", 0);
	if( ! hInsert ) { AfxMessageBox(IDS_ERR_INSERT_PRJ_ITEM_FAILED); return FALSE; }

	// expand parent category item
	if( ! m_treProjectTree.Expand( hParent, TVE_EXPAND ) ) return FALSE;

	return TRUE;
}

BOOL CFileWindow::AddLocalFileToProject(LPCTSTR lpszPathName)
{
	HTREEITEM hParent = m_treProjectTree.GetSelectedItem();
	if( ! hParent ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	// if selected item is not a category, then get parent item
	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hParent, nImage, nSelectedImage);
	if( nImage != PROJECT_ITEM_PROJECT && nImage != PROJECT_ITEM_CATEGORY ) hParent = m_treProjectTree.GetParentItem(hParent);

	HTREEITEM hFound = FindChildProjectItem(hParent, GetFileName(lpszPathName));
	if( hFound ) { /* AfxMessageBox(IDS_ERR_DUPLICATE_PROJECT_ITEM); */ return FALSE; }

	HTREEITEM hInsert = InsertProjectItem(hParent, GetFileName(lpszPathName), PROJECT_ITEM_LOCAL_FILE, 0, lpszPathName, 0);
	if( ! hInsert ) { AfxMessageBox(IDS_ERR_INSERT_PRJ_ITEM_FAILED); return FALSE; }

	// expand parent category item
	if( ! m_treProjectTree.Expand( hParent, TVE_EXPAND ) ) return FALSE;

	return TRUE;
}

BOOL CFileWindow::AddRemoteFileToProject(INT nAccount, LPCTSTR lpszPathName, DWORD dwSize)
{
	HTREEITEM hParent = m_treProjectTree.GetSelectedItem();
	if( ! hParent ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	// if selected item is not a category, then get parent item
	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hParent, nImage, nSelectedImage);
	if( nImage != PROJECT_ITEM_PROJECT && nImage != PROJECT_ITEM_CATEGORY ) hParent = m_treProjectTree.GetParentItem(hParent);

	HTREEITEM hFound = FindChildProjectItem(hParent, GetFileName(lpszPathName));
	if( hFound ) { /* AfxMessageBox(IDS_ERR_DUPLICATE_PROJECT_ITEM); */ return FALSE; }

	HTREEITEM hInsert = InsertProjectItem(hParent, GetFileName(lpszPathName), PROJECT_ITEM_REMOTE_FILE, nAccount, lpszPathName, dwSize);
	if( ! hInsert ) { AfxMessageBox(IDS_ERR_INSERT_PRJ_ITEM_FAILED); return FALSE; }

	// expand parent category item
	if( ! m_treProjectTree.Expand( hParent, TVE_EXPAND ) ) return FALSE;

	return TRUE;
}

BOOL CFileWindow::OpenProjectItem()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hItem, nImage, nSelectedImage);
	if( nImage != PROJECT_ITEM_LOCAL_FILE && nImage != PROJECT_ITEM_REMOTE_FILE ) return FALSE;

	return OpenProjectItem(hItem);
}

BOOL CFileWindow::ExecuteProjectItem()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hItem, nImage, nSelectedImage);
	if( nImage != PROJECT_ITEM_LOCAL_FILE ) return FALSE;

	return ExecuteProjectItem(hItem);
}

BOOL CFileWindow::RenameProjectItem()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hItem, nImage, nSelectedImage);
	if( nImage == PROJECT_ITEM_PROJECT ) return FALSE;

	CEdit * pEdit = m_treProjectTree.EditLabel(hItem);
	TRACE0("Begin EditLabel\n");
	return (pEdit != NULL);
}

BOOL CFileWindow::RemoveProjectItem()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return FALSE; }

	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hItem, nImage, nSelectedImage);
	if( nImage == PROJECT_ITEM_PROJECT ) return FALSE;
	
	return RemoveProjectItem(hItem);
}

BOOL CFileWindow::IsSelectedProjectItemRoot()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	HTREEITEM hRoot = m_treProjectTree.GetRootItem();
	return (hItem == hRoot);
}

CString CFileWindow::GetSelectedProjectItemText()
{
	HTREEITEM hItem = m_treProjectTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_PRJ_ITEM_SELECTED); return ""; }
	return m_treProjectTree.GetItemText(hItem);
}

/////////////////////////////////////////////////////////////////////////////
// Inner Functions
BOOL CFileWindow::SaveProjectItem(ostream & os, INT nLevel, HTREEITEM hItem)
{
	INT nImage, nSelectedImage; m_treProjectTree.GetItemImage( hItem, nImage, nSelectedImage );
	CString szContents, szIndent('\t', nLevel), szText = m_treProjectTree.GetItemText( hItem );
	CString szExpanded = (m_treProjectTree.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) ? "yes" : "no";

	if( nImage == PROJECT_ITEM_CATEGORY ) {
		szContents.Format("<category name=\"%s\" expanded=\"%s\">", szText, szExpanded);
		os << szIndent << szContents << endl;

		HTREEITEM hChild = m_treProjectTree.GetChildItem( hItem );
		while( hChild ) { // recursive call to all child items
			SaveProjectItem(os, nLevel+1, hChild);
			hChild = m_treProjectTree.GetNextSiblingItem( hChild );
		}

		os << szIndent << "</category>" << endl;

	} else if( nImage == PROJECT_ITEM_LOCAL_FILE ) {
		CString szPath = GetProjectItemPathName( hItem );
		szContents.Format("<localfile path=\"%s\" />", szPath);

		os << szIndent << szContents << endl;

	} else if( nImage == PROJECT_ITEM_REMOTE_FILE ) {
		LPPROJECTITEMINFO lpInfo = GetProjectItemInfo( hItem );
		INT nAccount = lpInfo->nFtpAccount;
		CString szPath = lpInfo->szPathName;
		LONG nFileSize = lpInfo->nFileSize;

		szContents.Format("<remotefile account=\"%d\" path=\"%s\" filesize=\"%d\" />", nAccount, szPath, nFileSize);

		os << szIndent << szContents << endl;

	} else return FALSE;

	return TRUE;
}

BOOL CFileWindow::LoadProjectItem(istream & is, TCHAR szText[], HTREEITEM hParent)
{
	CMapStringToString mapAttr; 

	if( ! _stricmp(szText, "<category") ) {
		is.getline(szText, 4096, '>'); // get attributes
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szName; BOOL bLookup = mapAttr.Lookup("name", szName);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szExpanded; bLookup = mapAttr.Lookup("expanded", szExpanded);
		if( ! bLookup ) { szExpanded = "no"; }

		HTREEITEM hItem = InsertProjectItem(hParent, szName, PROJECT_ITEM_CATEGORY, 0, "", 0);
		is >> szText; // get next tocken

		while( _stricmp(szText, "</category>") ) {
			if( ! LoadProjectItem(is, szText, hItem) ) return FALSE;
		}
		is >> szText; // get next tocken

		// expand category item if it is checked
		if( ! szExpanded.Compare("yes") ) m_treProjectTree.Expand( hItem, TVE_EXPAND );

	} else if( ! _stricmp(szText, "<localfile") ) {
		is.getline(szText, 4096, '>'); // get attributes
		INT nLen = strlen(szText); if( szText[nLen-1] == '/' ) szText[nLen-1] = '\0';
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szPath; BOOL bLookup = mapAttr.Lookup("path", szPath);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		HTREEITEM hItem = InsertProjectItem(hParent, GetFileName(szPath), PROJECT_ITEM_LOCAL_FILE, 0, szPath, 0);
		is >> szText; // get next tocken

	} else if( ! _stricmp(szText, "<remotefile") ) {
		is.getline(szText, 4096, '>'); // get attributes
		INT nLen = strlen(szText); if( szText[nLen-1] == '/' ) szText[nLen-1] = '\0';
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szAccount; BOOL bLookup = mapAttr.Lookup("account", szAccount);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szPath; bLookup = mapAttr.Lookup("path", szPath);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szFileSize; bLookup = mapAttr.Lookup("filesize", szFileSize);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		HTREEITEM hItem = InsertProjectItem(hParent, GetFileName(szPath), PROJECT_ITEM_REMOTE_FILE, atoi(szAccount), szPath, atol(szFileSize));
		is >> szText; // get next tocken

	} else { // not recognized item
		is.getline(szText, 4096, '>'); // skip attributes
		is >> szText; // get next tocken
	}

	return TRUE;
}

BOOL CFileWindow::SaveWorkspaceItem(ostream & os, INT nLevel, CDocument * pDoc)
{
	CCedtDoc * pCedtDoc = (CCedtDoc *)pDoc;
	if( pCedtDoc->IsNewFileNotSaved() ) return TRUE;

	CString szContents, szIndent('\t', nLevel);

	if( pCedtDoc->IsRemoteFile() ) {
		INT nAccount = pCedtDoc->GetFtpAccountNumber();
		CString szPath = pCedtDoc->GetRemotePathName();
		LONG nFileSize = pCedtDoc->GetFileSize();

		CCedtView * pView = (CCedtView *)pCedtDoc->GetFirstView();
		INT nLineNum = pView->GetCurrentLineNumber();

		CChildFrame * pFrame = (CChildFrame *)pView->GetParentFrame();
		WINDOWPLACEMENT wndpl; pFrame->GetWindowPlacement( & wndpl );

		szContents.Format("<remotefile account=\"%d\" path=\"%s\" filesize=\"%d\" linenum=\"%d\" placement=\"%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\" />",
			nAccount, szPath, nFileSize, nLineNum, (INT)wndpl.flags, (INT)wndpl.showCmd, 
			wndpl.ptMinPosition.x, wndpl.ptMinPosition.y, wndpl.ptMaxPosition.x, wndpl.ptMaxPosition.y,
			wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, wndpl.rcNormalPosition.right, wndpl.rcNormalPosition.bottom);
		os << szIndent << szContents << endl;

	} else {
		CString szPath = pCedtDoc->GetPathName();

		CCedtView * pView = (CCedtView *)pCedtDoc->GetFirstView();
		INT nLineNum = pView->GetCurrentLineNumber();

		CChildFrame * pFrame = (CChildFrame *)pView->GetParentFrame();
		WINDOWPLACEMENT wndpl; pFrame->GetWindowPlacement( & wndpl );

		szContents.Format("<localfile path=\"%s\" linenum=\"%d\" placement=\"%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\" />", 
			szPath, nLineNum, (INT)wndpl.flags, (INT)wndpl.showCmd, 
			wndpl.ptMinPosition.x, wndpl.ptMinPosition.y, wndpl.ptMaxPosition.x, wndpl.ptMaxPosition.y,
			wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top, wndpl.rcNormalPosition.right, wndpl.rcNormalPosition.bottom);
		os << szIndent << szContents << endl;
	}

	return TRUE;
}

BOOL CFileWindow::LoadWorkspaceItem(istream & is, TCHAR szText[], CWinApp * pApp)
{
	CCedtApp * pCedtApp = (CCedtApp *)pApp;
	CMapStringToString mapAttr;

	if( ! _stricmp(szText, "<remotefile") ) {
		is.getline(szText, 4096, '>'); // get attributes
		INT nLen = strlen(szText); if( szText[nLen-1] == '/' ) szText[nLen-1] = '\0';
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szAccount; BOOL bLookup = mapAttr.Lookup("account", szAccount);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szPath; bLookup = mapAttr.Lookup("path", szPath);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szFileSize; bLookup = mapAttr.Lookup("filesize", szFileSize);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szLineNum; bLookup = mapAttr.Lookup("linenum", szLineNum);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szPlacement; bLookup = mapAttr.Lookup("placement", szPlacement);
		if( ! bLookup ) { szPlacement = ""; }

		WINDOWPLACEMENT * pwndpl = NULL;
		if( szPlacement.GetLength() ) {
			WINDOWPLACEMENT wndpl; wndpl.length = sizeof(wndpl);
			INT nFound = 0; pwndpl = & wndpl;

			wndpl.flags = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.showCmd = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMinPosition.x = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMinPosition.y = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMaxPosition.x = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMaxPosition.y = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.left = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.top = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.right = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.bottom = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
		}

		CDocument * pDoc = pCedtApp->OpenRemoteDocumentFile(atoi(szAccount), szPath, atol(szFileSize), atoi(szLineNum), pwndpl);
		is >> szText; // get next tocken

	} else if( ! _stricmp(szText, "<localfile") ) {
		is.getline(szText, 4096, '>'); // get attributes
		INT nLen = strlen(szText); if( szText[nLen-1] == '/' ) szText[nLen-1] = '\0';
		if( ! ParseProjectItemAttr( szText, mapAttr ) ) return FALSE;

		CString szPath; BOOL bLookup = mapAttr.Lookup("path", szPath);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szLineNum; bLookup = mapAttr.Lookup("linenum", szLineNum);
		if( ! bLookup ) { AfxMessageBox(IDS_ERR_WRONG_PRJ_FILE); return FALSE; }

		CString szPlacement; bLookup = mapAttr.Lookup("placement", szPlacement);
		if( ! bLookup ) { szPlacement = ""; }

		WINDOWPLACEMENT * pwndpl = NULL;
		if( szPlacement.GetLength() ) {
			WINDOWPLACEMENT wndpl; wndpl.length = sizeof(wndpl);
			INT nFound = 0; pwndpl = & wndpl;

			wndpl.flags = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.showCmd = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMinPosition.x = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMinPosition.y = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMaxPosition.x = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.ptMaxPosition.y = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.left = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.top = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.right = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
			wndpl.rcNormalPosition.bottom = atoi( szPlacement.Mid(nFound) ); nFound = szPlacement.Find(':', nFound) + 1;
		}

		CCedtDoc * pDoc = (CCedtDoc *)pCedtApp->OpenDocumentFile(szPath, atoi(szLineNum), pwndpl);
		is >> szText; // get next tocken

	} else { // not recognized item
		is.getline(szText, 4096, '>'); // skip attributes
		is >> szText; // get next tocken
	}

	return TRUE;
}

BOOL CFileWindow::ParseProjectItemAttr(LPCTSTR lpszText, CMapStringToString & mapAttr)
{
	TCHAR * pBeg, * pEnd = (TCHAR *)lpszText;
	CString szAttrName, szAttrValue;

	mapAttr.RemoveAll();
	while( * pEnd && isspace(* pEnd) ) pEnd++;

	while( * pEnd ) {
		pBeg = pEnd; while( * pEnd && ! isspace(* pEnd) && * pEnd != '=' ) pEnd++;
		szAttrName = CString(pBeg, pEnd-pBeg);

		while( * pEnd && isspace(* pEnd) ) pEnd++;

		if( * pEnd == '=' ) { pEnd++; } 
		else { AfxMessageBox(IDS_ERR_PARSE_PRJ_ATTRIBUTE); return FALSE; }

		while( * pEnd && isspace(* pEnd) ) pEnd++;

		if( * pEnd == '"' ) { pEnd++; } 
		else { AfxMessageBox(IDS_ERR_PARSE_PRJ_ATTRIBUTE); return FALSE; }

		pBeg = pEnd; while( * pEnd && * pEnd != '"' ) pEnd++;
		szAttrValue = CString(pBeg, pEnd-pBeg);

		if( * pEnd == '"' ) { pEnd++; } 
		else { AfxMessageBox(IDS_ERR_PARSE_PRJ_ATTRIBUTE); return FALSE; }

		mapAttr.SetAt( szAttrName, szAttrValue );
		while( * pEnd && isspace(* pEnd) ) pEnd++;
	}

	return TRUE;
}

LPPROJECTITEMINFO CFileWindow::GetProjectItemInfo(HTREEITEM hItem)
{
	if( ! hItem ) return NULL;
	return (LPPROJECTITEMINFO)m_treProjectTree.GetItemData(hItem); 
}

CString CFileWindow::GetProjectItemPathName(HTREEITEM hItem)
{
	if( ! hItem ) return "";
	LPPROJECTITEMINFO lpInfo = (LPPROJECTITEMINFO)m_treProjectTree.GetItemData(hItem); 
	return lpInfo->szPathName;
}

HTREEITEM CFileWindow::GetClickedProjectItem()
{
	CPoint point; GetCursorPos( & point ); 
	m_treProjectTree.ScreenToClient( & point );

	HTREEITEM hItem; UINT nFlags;
	hItem = m_treProjectTree.HitTest( point, & nFlags );

	if( nFlags & TVHT_ONITEM ) return hItem;
	return NULL;
}

BOOL CFileWindow::EnableAllProjectButtons(BOOL bEnable)
{
	m_btnToolbarProject.EnableButton(ID_PROJECT_NEW_CATEGORY, bEnable);
	m_btnToolbarProject.EnableButton(ID_PROJECT_ADD_FILES_TO, bEnable);
	m_btnToolbarProject.EnableButton(ID_PROJECT_ADD_ACTIVE_FILE, bEnable);
	m_btnToolbarProject.EnableButton(ID_PROJECT_ADD_OPEN_FILES, bEnable);
	m_btnToolbarProject.EnableButton(ID_PROJECT_ITEM_REMOVE, bEnable);

	return TRUE;
}

INT CALLBACK CFileWindow::CompareProjectItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	LPPROJECTITEMINFO lpInfo1 = (LPPROJECTITEMINFO)lParam1;
	LPPROJECTITEMINFO lpInfo2 = (LPPROJECTITEMINFO)lParam2;

	if( lpInfo1->nItemType == lpInfo2->nItemType ) {
		CString szFileName1 = GetFileName( lpInfo1->szPathName );
		CString szFileName2 = GetFileName( lpInfo2->szPathName );
		return szFileName1.CompareNoCase( szFileName2 );
	} else return (lpInfo1->nItemType - lpInfo2->nItemType);
}

HTREEITEM CFileWindow::InsertProjectItem(HTREEITEM hParent, LPCTSTR lpszText, INT nType, INT nAccount, LPCTSTR lpszPathName, LONG nSize)
{
	LPPROJECTITEMINFO lpInfo = new PROJECTITEMINFO;
	lpInfo->nItemType = nType;			lpInfo->nFtpAccount = nAccount;
	lpInfo->szPathName = lpszPathName;	lpInfo->nFileSize = nSize;

	HTREEITEM hItem = m_treProjectTree.InsertItem( lpszText, nType, nType, hParent );
	BOOL bReturn = m_treProjectTree.SetItemData( hItem, (DWORD)lpInfo );

	TVSORTCB sort; sort.hParent = hParent; sort.lpfnCompare = CompareProjectItem; sort.lParam = 0L;
	BOOL bSorted = m_treProjectTree.SortChildrenCB( & sort );

//	BOOL bExpand = m_treProjectTree.Expand( hParent, TVE_EXPAND );
//	BOOL bSelect = m_treProjectTree.SelectItem( hItem );

	return hItem;
}

HTREEITEM CFileWindow::FindChildProjectItem(HTREEITEM hParent, LPCTSTR lpszText)
{
	HTREEITEM hFound = m_treProjectTree.GetChildItem(hParent);
	while( hFound ) {
		CString szText = m_treProjectTree.GetItemText( hFound );
		if( ! szText.CompareNoCase(lpszText) ) return hFound;
		hFound = m_treProjectTree.GetNextSiblingItem( hFound );
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Handlers
void CFileWindow::OnClickProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CFileWindow::OnDblclkProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetClickedProjectItem();
	if( hItem ) OpenProjectItem(hItem);
	*pResult = 0;
}

void CFileWindow::OnRclickProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetClickedProjectItem();
	if( hItem ) m_treProjectTree.SelectItem(hItem);
	OpenProjectContextMenu(hItem);
	*pResult = 0;
}

void CFileWindow::OnBeginlabeleditProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	HTREEITEM hItem = pTVDispInfo->item.hItem;

	LPPROJECTITEMINFO lpInfo = GetProjectItemInfo( hItem );
	if( lpInfo->nItemType == PROJECT_ITEM_CATEGORY ) m_bLabelEditing = TRUE;

	* pResult = m_bLabelEditing ? 0 : 1;
}

void CFileWindow::OnEndlabeleditProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	HTREEITEM hItem = pTVDispInfo->item.hItem;

	CString szText = m_treProjectTree.GetItemText( hItem );
	if( pTVDispInfo->item.pszText && szText.CompareNoCase(pTVDispInfo->item.pszText) ) {
		RenameProjectItem( pTVDispInfo->item.hItem, pTVDispInfo->item.pszText );
	}

	m_bLabelEditing = FALSE;
	* pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// Action Functions
BOOL CFileWindow::OpenProjectContextMenu(HTREEITEM hItem)
{
	CPoint point; GetCursorPos( & point ); 
	CMenu * pMenu, context; context.LoadMenu(IDR_FILE_WINDOW);

	if( hItem ) {
		INT nImage, nSelectedImage; m_treProjectTree.GetItemImage(hItem, nImage, nSelectedImage);
		CCedtApp * pApp = (CCedtApp *)AfxGetApp();

		if( ! IsSelectedProjectItemRoot() ) {
			switch( nImage ) {
			case PROJECT_ITEM_LOCAL_FILE:
				pMenu = GetSubMenuByText( & context, "PRJ_LOCAL"    ); break;
			case PROJECT_ITEM_REMOTE_FILE:
				pMenu = GetSubMenuByText( & context, "PRJ_REMOTE"   ); break;
			case PROJECT_ITEM_CATEGORY:
				pMenu = GetSubMenuByText( & context, "PRJ_CATEGORY" ); break;
			default: // this should not occur !!!
				pMenu = GetSubMenuByText( & context, "PRJ_NULL"     ); break;
			}
		} else if( pApp->IsProjectLoaded() ) {
			pMenu = GetSubMenuByText( & context, "PRJ_ROOT1" );
		} else pMenu = GetSubMenuByText( & context, "PRJ_ROOT0" );
	} else pMenu = GetSubMenuByText( & context, "PRJ_NULL" );

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	return pMenu->TrackPopupMenu(nFlags, point.x, point.y, AfxGetMainWnd());
}

BOOL CFileWindow::RemoveAllProjectItems()
{
	HTREEITEM hRoot = m_treProjectTree.GetRootItem();
	if( hRoot ) return RemoveProjectItem( hRoot );
	else return TRUE; // nothing to delete
}

BOOL CFileWindow::OpenProjectItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	LPPROJECTITEMINFO lpInfo = GetProjectItemInfo( hItem );
	if( lpInfo->nItemType == PROJECT_ITEM_REMOTE_FILE ) { // remote file
		CCedtApp * pApp = (CCedtApp *)AfxGetApp(); if( ! pApp ) return FALSE;
		return pApp->PostOpenRemoteDocumentFile( lpInfo->nFtpAccount, lpInfo->szPathName, lpInfo->nFileSize, 0 );
	} else if( lpInfo->nItemType == PROJECT_ITEM_LOCAL_FILE ) { // local file
		if( ! VerifyFilePath( lpInfo->szPathName ) ) return FALSE;
		CCedtApp * pApp = (CCedtApp *)AfxGetApp(); if( ! pApp ) return FALSE;
		return pApp->PostOpenDocumentFile( lpInfo->szPathName, 0 );
	} else return FALSE;
}

BOOL CFileWindow::ExecuteProjectItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	LPPROJECTITEMINFO lpInfo = GetProjectItemInfo( hItem );
	if( lpInfo->nItemType == PROJECT_ITEM_LOCAL_FILE ) { // local file
		if( ! VerifyFilePath( lpInfo->szPathName ) ) return FALSE;
		CWnd * pWnd = AfxGetMainWnd(); if( ! pWnd ) return FALSE;
		HINSTANCE hResult = ::ShellExecute(NULL, "open", lpInfo->szPathName, NULL, NULL, SW_SHOWNORMAL);
		return ((UINT)hResult > 32) ? TRUE : FALSE;
	} else return FALSE;
}

BOOL CFileWindow::RenameProjectItem(HTREEITEM hItem, LPCTSTR lpszNewName)
{
	return m_treProjectTree.SetItemText(hItem, lpszNewName);
}

BOOL CFileWindow::RemoveProjectItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	HTREEITEM hChild = m_treProjectTree.GetChildItem( hItem );
	while( hChild ) { // delete childrens
		if( ! RemoveProjectItem( hChild ) ) return FALSE; // recursive call
		hChild = m_treProjectTree.GetChildItem( hItem );
	}

	delete GetProjectItemInfo( hItem ); // delete item data first
	return m_treProjectTree.DeleteItem( hItem );
}
