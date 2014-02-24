#include "stdafx.h"
#include "cedtHeader.h"
#include "SortStringArray.h"
#include "FolderDialog.h"


BOOL CFileWindow::InitLocalDriveList(LPCTSTR lpszInitialDriveName)
{
	TCHAR szBuffer[1024];
	TCHAR * pDrive = szBuffer;

	if( ! GetLogicalDriveStrings(sizeof(szBuffer), szBuffer) ) return FALSE;
	m_cmbLocalDrive.ResetContent();

	CString szDriveName; INT nDrive = 0;
	while( * pDrive ) {
		TCHAR szVolumeName[1024], szFileSystemName[1024];
		DWORD dwSerialNumber, dwMaxComponentLength, dwFileSystemFlags;

		if( GetVolumeInformation(pDrive, szVolumeName, 1024, & dwSerialNumber,
			& dwMaxComponentLength, & dwFileSystemFlags, szFileSystemName, 1024) ) {
			szDriveName.Format("[%c:] %s", toupper(pDrive[0]), szVolumeName);
		} else {
			switch( GetDriveType(pDrive) ) {
			case DRIVE_FIXED: szDriveName.Format("[%c:] Local Drive", toupper(pDrive[0])); break;
			case DRIVE_RAMDISK: szDriveName.Format("[%c:] RAM Disk", toupper(pDrive[0])); break;
			case DRIVE_REMOTE: szDriveName.Format("[%c:] Network Drive", toupper(pDrive[0])); break;
			case DRIVE_CDROM: szDriveName.Format("[%c:] CD-ROM", toupper(pDrive[0])); break;
			case DRIVE_REMOVABLE: szDriveName.Format("[%c:] Removable", toupper(pDrive[0])); break;
			default: szDriveName.Format("[%c:] Unknown Type", toupper(pDrive[0])); break;
			}
		}

		m_cmbLocalDrive.InsertString(nDrive, szDriveName);
		nDrive++; pDrive += strlen(pDrive) + 1;
	}

	if( lpszInitialDriveName ) {
		nDrive = m_cmbLocalDrive.FindString(-1, lpszInitialDriveName);
		if( nDrive != CB_ERR ) m_cmbLocalDrive.SetCurSel(nDrive);
	}

	return TRUE;
}

BOOL CFileWindow::InitFileFilterList(LPCTSTR lpszComposedFilter, INT nInitialFilter, CALLBACK_SELCHANGE fcnCallback)
{
	m_cmbFileFilter.ResetContent(); if( nInitialFilter < 0 ) nInitialFilter = 0; 
	ParseFileFilter(m_arrFilterDescription, m_arrFilterExtensions, lpszComposedFilter);

	INT nSize = m_arrFilterDescription.GetSize();
	for(INT i = 0; i < nSize; i++) m_cmbFileFilter.InsertString( i, m_arrFilterDescription.GetAt(i) );

	m_cmbFileFilter.SetCurSel(nInitialFilter);
	m_fcnCallbackSelchangeFileFilter = fcnCallback;

	return TRUE;
}

BOOL CFileWindow::GetBrowsingDirectory(CString & szDirectory)
{
	HTREEITEM hItem = m_treDirectoryTree.GetSelectedItem();
	CString szFullPath = GetDirectoryItemPathName( hItem );

	if( ! VerifyFilePath( szFullPath ) ) szDirectory = szFullPath;
	else szDirectory = GetFileDirectory( szFullPath );

	return TRUE;
}

BOOL CFileWindow::SetBrowsingDirectory(LPCTSTR lpszDirectory)
{
	CString szDriveName( lpszDirectory, 2 );
	SelectLocalDrive( szDriveName );

	DisplayDirectoryDrive( szDriveName );
	RefreshDirectoryTree( lpszDirectory );

	return TRUE;
}

BOOL CFileWindow::IsSelectedDirectoryItemRoot()
{
	HTREEITEM hItem = m_treDirectoryTree.GetSelectedItem();
	HTREEITEM hRoot = m_treDirectoryTree.GetRootItem();
	return (hItem == hRoot);
}

CString CFileWindow::GetSelectedDirectoryItemText()
{
	HTREEITEM hItem = m_treDirectoryTree.GetSelectedItem();
	if( ! hItem ) { AfxMessageBox(IDS_ERR_NO_DIR_ITEM_SELECTED); return ""; }
	return m_treDirectoryTree.GetItemText(hItem);
}

BOOL CFileWindow::OpenDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return OpenDirectoryItem(hItem);
}

BOOL CFileWindow::ExecuteDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return ExecuteDirectoryItem(hItem);
}

BOOL CFileWindow::ExploreDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return ExploreDirectoryItem(hItem);
}

BOOL CFileWindow::FindInDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return FindInDirectoryItem(hItem);
}

BOOL CFileWindow::MoveDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem && m_treDirectoryTree.GetParentItem(hItem) ) {
		CString szPathName = GetDirectoryItemPathName(hItem);
		CString szDirectory = GetFileDirectory(szPathName);
		if( AskDestinationDirectory( szDirectory ) ) {
			return MoveToDirectoryItem(hItem, szDirectory);
		} else return FALSE;
	} else return FALSE;
}

BOOL CFileWindow::CopyDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem && m_treDirectoryTree.GetParentItem(hItem) ) {
		CString szPathName = GetDirectoryItemPathName(hItem);
		CString szDirectory = GetFileDirectory(szPathName);
		if( AskDestinationDirectory( szDirectory ) ) {
			return CopyToDirectoryItem(hItem, szDirectory);
		} else return FALSE;
	} else return FALSE;
}

BOOL CFileWindow::RenameDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem && m_treDirectoryTree.GetParentItem(hItem) ) {
		CEdit * pEdit = m_treDirectoryTree.EditLabel(hItem);
		return (pEdit != NULL);
	} else return FALSE;
}

BOOL CFileWindow::DeleteDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem && m_treDirectoryTree.GetParentItem(hItem) ) {
		return DeleteDirectoryItem(hItem);
	} else return FALSE;
}

BOOL CFileWindow::SetWorkdirDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return SetAsWorkingDirectory(hItem);
}

BOOL CFileWindow::RefreshDirectoryItem()
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	return RefreshDirectoryItem(hItem);
}



/////////////////////////////////////////////////////////////////////////////
// Inner Functions
BOOL CFileWindow::SelectLocalDrive(LPCTSTR lpszDriveName)
{
	INT nDrive = m_cmbLocalDrive.FindString(-1, lpszDriveName);
	if( nDrive == CB_ERR ) return FALSE;
	m_cmbLocalDrive.SetCurSel(nDrive);
	return TRUE;
}

CString CFileWindow::GetActiveLocalDriveName()
{
	INT nDrive = m_cmbLocalDrive.GetCurSel();
	if( nDrive == CB_ERR ) return "C:";
	CString szDrive; m_cmbLocalDrive.GetLBText(nDrive, szDrive);
	return szDrive.Mid(1, 2);
}

INT CFileWindow::GetActiveFileFilterIndex()
{
	INT nFilter = m_cmbFileFilter.GetCurSel();
	if( nFilter == CB_ERR ) return 0;
	return nFilter;
}

CString CFileWindow::GetActiveFileFilterString()
{
	INT nFilter = m_cmbFileFilter.GetCurSel();
	if( nFilter == CB_ERR ) return "*.*";
	return m_arrFilterExtensions.GetAt(nFilter);
}


BOOL CFileWindow::DoesDirectoryHaveChildren(LPCTSTR lpszPath)
{
	CString szPath = lpszPath;
	if( szPath.Right(1) == "\\" ) szPath += "*.*";
	else szPath += "\\*.*";

	CString szFilter = GetActiveFileFilterString();
	CFileFind find; BOOL bFound = find.FindFile(szPath);

	while( bFound ) {
		bFound = find.FindNextFile();
		CString szFilePath = find.GetFilePath();

		if( find.IsDirectory() && ! find.IsDots() ) return TRUE;
		if( ! find.IsDirectory() && ! find.IsHidden() && MatchFileFilter(szFilePath, szFilter) ) return TRUE;
	}

	return FALSE;
}

CString CFileWindow::GetDirectoryItemPathName(HTREEITEM hItem)
{
	CString szTemp, szReturn = "";

	while( hItem ) {
		szTemp = m_treDirectoryTree.GetItemText( hItem );

		if( ! szReturn.GetLength() ) szReturn = szTemp;
		else szReturn = szTemp + "\\" + szReturn;

		hItem = m_treDirectoryTree.GetParentItem( hItem );
	}

	return szReturn;
}

BOOL CFileWindow::RefreshDirectoryTree(LPCTSTR lpszPathName)
{
	TCHAR szPathName[MAX_PATH]; strcpy( szPathName, lpszPathName );
	if( szPathName[strlen(szPathName)-1] != '\\' ) strcat( szPathName, "\\" );

	HTREEITEM hItem = TVI_ROOT;
	TCHAR * pText = szPathName;

	INT nLen = strlen(szPathName);
	for(INT i = 0; i < nLen; i++) {
		if( szPathName[i] == '\\' ) {
			szPathName[i] = '\0';

			hItem = FindChildDirectoryItem(hItem, pText);
			if( ! hItem ) break; // not found

			CString szPath = GetDirectoryItemPathName(hItem);
			RemoveChildDirectoryItems(hItem);
			DisplayDirectoryPath(hItem, szPath);
			m_treDirectoryTree.Expand(hItem, TVE_EXPAND);

			pText += strlen(pText) + 1;
		}
	}

	if( hItem ) m_treDirectoryTree.SelectItem(hItem);

	return TRUE;
}

BOOL CFileWindow::AskDestinationDirectory(CString & szDirectory)
{
	CString szText( LPCTSTR(IDS_CHOOSE_DIR_DESTINATION) );
	CFolderDialog dlg(szText, szDirectory, NULL, AfxGetMainWnd());
	if( dlg.DoModal() != IDOK ) return FALSE;
	szDirectory = dlg.GetPathName();
	return TRUE;
}


HTREEITEM CFileWindow::GetClickedDirectoryItem()
{
	CPoint point; GetCursorPos( & point ); 
	m_treDirectoryTree.ScreenToClient( & point );

	HTREEITEM hItem; UINT nFlags;
	hItem = m_treDirectoryTree.HitTest( point, & nFlags );

	if( nFlags & TVHT_ONITEM ) return hItem;
	return NULL;
}

HTREEITEM CFileWindow::InsertDirectoryItem(HTREEITEM hParent, LPCTSTR lpszPath)
{
	TCHAR szTemp[MAX_PATH]; strcpy(szTemp, lpszPath); INT nLen = strlen(lpszPath);
	if( szTemp[nLen-1] != '\\' ) { szTemp[nLen] = '\\'; nLen++; szTemp[nLen] = '\0'; }

	SHFILEINFO shFinfo; INT iIcon, iIconSel;
	if( ! SHGetFileInfo(szTemp, 0, &shFinfo, sizeof(shFinfo), SHGFI_SYSICONINDEX) ) return NULL;
	iIcon = iIconSel = shFinfo.iIcon;

	if( szTemp[nLen-1] == '\\' ) { szTemp[nLen-1] = '\0'; }
	return m_treDirectoryTree.InsertItem( GetFileName(szTemp ), iIcon, iIconSel, hParent );
}

HTREEITEM CFileWindow::RemoveChildDirectoryItems(HTREEITEM hParent)
{
	HTREEITEM hChild = m_treDirectoryTree.GetChildItem( hParent );
	while( hChild ) {
		m_treDirectoryTree.DeleteItem( hChild );
		hChild = m_treDirectoryTree.GetChildItem( hParent );
	}
	return hParent;
}

HTREEITEM CFileWindow::FindChildDirectoryItem(HTREEITEM hParent, LPCTSTR lpszText)
{
	HTREEITEM hFound = m_treDirectoryTree.GetChildItem(hParent);
	while( hFound ) {
		CString szText = m_treDirectoryTree.GetItemText( hFound );
		if( ! szText.CompareNoCase(lpszText) ) return hFound;
		hFound = m_treDirectoryTree.GetNextSiblingItem( hFound );
	}
	return NULL;
}


void CFileWindow::DisplayDirectoryDrive(LPCTSTR lpszDrive)
{
	m_treDirectoryTree.DeleteAllItems();

	HTREEITEM hItem = InsertDirectoryItem(TVI_ROOT, lpszDrive);
	if( DoesDirectoryHaveChildren(lpszDrive) ) {
		DisplayDirectoryPath(hItem, lpszDrive);
		m_treDirectoryTree.Expand(hItem, TVE_EXPAND);
	}
}

void CFileWindow::DisplayDirectoryPath(HTREEITEM hParent, LPCTSTR lpszPath)
{
	CString szPath = lpszPath;
	if( szPath.Right(1) == "\\" ) szPath += "*.*";
	else szPath += "\\*.*";

	CSortStringArray arrDirectories, arrFiles;
	INT i, nSize;

	CString szFilter = GetActiveFileFilterString();
	CFileFind find; BOOL bFound = find.FindFile(szPath);

	while( bFound ) {
		bFound = find.FindNextFile();
		CString szFilePath = find.GetFilePath();

		if( find.IsDirectory() && ! find.IsDots() ) arrDirectories.Add( szFilePath );
		if( ! find.IsDirectory() && ! find.IsHidden() && MatchFileFilter(szFilePath, szFilter) ) arrFiles.Add( szFilePath );
	}

	arrDirectories.Sort(); nSize = arrDirectories.GetSize();
	for(i = 0; i < nSize; i++) {
		szPath = arrDirectories.GetAt(i);
		HTREEITEM hItem = InsertDirectoryItem(hParent, szPath);
		if( DoesDirectoryHaveChildren(szPath) ) m_treDirectoryTree.InsertItem("NULL", hItem);
	}

	arrFiles.Sort(); nSize = arrFiles.GetSize();
	for(i = 0; i < nSize; i++) {
		szPath = arrFiles.GetAt(i);
		HTREEITEM hItem = InsertDirectoryItem(hParent, szPath);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Handlers
void CFileWindow::OnDropdownLocalDrive() 
{
	CString szDriveName = GetActiveLocalDriveName();
	m_szPrevDriveName = szDriveName;

	InitLocalDriveList(szDriveName);
}

void CFileWindow::OnSelchangeLocalDrive() 
{
	CWaitCursor wait;
	m_treDirectoryTree.SetRedraw(FALSE);

	CString szDriveName = GetActiveLocalDriveName();
	if( szDriveName.Compare(m_szPrevDriveName) ) DisplayDirectoryDrive(szDriveName);

	m_treDirectoryTree.SetRedraw(TRUE);
}

void CFileWindow::OnSelchangeFileFilter() 
{
	CWaitCursor wait;
	m_treDirectoryTree.SetRedraw(FALSE);

	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem ) RefreshDirectoryTree( GetDirectoryItemPathName(hItem) );
	else RefreshDirectoryTree( GetActiveLocalDriveName() ); // no selected item

	m_treDirectoryTree.SetRedraw(TRUE);
	if( m_fcnCallbackSelchangeFileFilter ) m_fcnCallbackSelchangeFileFilter( GetActiveFileFilterIndex() );
}

void CFileWindow::OnItemexpandedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	UINT state = pNMTreeView->itemNew.state;
	CString szPath = GetDirectoryItemPathName( hItem );

	CWaitCursor wait;
	m_treDirectoryTree.SetRedraw(FALSE);

	if( state & TVIS_EXPANDED ) { // expand
		RemoveChildDirectoryItems( hItem );
		DisplayDirectoryPath(hItem, szPath);
	} else { // collapse
		RemoveChildDirectoryItems( hItem );
		if( DoesDirectoryHaveChildren(szPath) ) m_treDirectoryTree.InsertItem("NULL", hItem);
	}

	m_treDirectoryTree.SetRedraw(TRUE);
	*pResult = 0;
}

void CFileWindow::OnSelchangedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetSelectedDirectoryItem();
	if( hItem ) SetAsCurrentDirectory(hItem);
	*pResult = 0;
}

void CFileWindow::OnClickDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CFileWindow::OnDblclkDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetClickedDirectoryItem();
	if( hItem ) OpenDirectoryItem(hItem);
	*pResult = 0;
}

void CFileWindow::OnRclickDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetClickedDirectoryItem();
	if( hItem ) m_treDirectoryTree.SelectItem(hItem);
	OpenDirectoryContextMenu(hItem);
	*pResult = 0;
}

void CFileWindow::OnBeginlabeleditDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	HTREEITEM hParent = m_treDirectoryTree.GetParentItem(pTVDispInfo->item.hItem);

	m_bLabelEditing = hParent ? TRUE : FALSE;
	* pResult = m_bLabelEditing ? 0 : 1;
}

void CFileWindow::OnEndlabeleditDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CString szText = m_treDirectoryTree.GetItemText(pTVDispInfo->item.hItem);
	if( pTVDispInfo->item.pszText && szText.CompareNoCase(pTVDispInfo->item.pszText) ) {
		RenameDirectoryItem( pTVDispInfo->item.hItem, pTVDispInfo->item.pszText );
	}

	m_bLabelEditing = FALSE;
	* pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// Action Functions
BOOL CFileWindow::OpenDirectoryContextMenu(HTREEITEM hItem)
{
	CPoint point; GetCursorPos( & point ); 
	CMenu * pMenu, context; context.LoadMenu(IDR_FILE_WINDOW);

	if( hItem ) {
		if( ! IsSelectedDirectoryItemRoot() ) {
			CString szPathName = GetDirectoryItemPathName(hItem);
			if( VerifyFilePath(szPathName) ) {
				pMenu = GetSubMenuByText( & context, "DIR_FILE" );
			} else pMenu = GetSubMenuByText( & context, "DIR_FOLDER" );
		} else pMenu = GetSubMenuByText( & context, "DIR_ROOT" );
	} else pMenu = GetSubMenuByText( & context, "DIR_NULL" );

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	return pMenu->TrackPopupMenu(nFlags, point.x, point.y, AfxGetMainWnd());
}

BOOL CFileWindow::SetAsCurrentDirectory(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szDirName, szPathName = GetDirectoryItemPathName(hItem);
	if( ! VerifyFilePath(szPathName) ) {
		szDirName = szPathName;
		return ::SetCurrentDirectory(szDirName + "\\");
	} else {
		szDirName = GetFileDirectory(szPathName);
		return ::SetCurrentDirectory(szDirName + "\\");
	}
}

BOOL CFileWindow::SetAsWorkingDirectory(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	if( ! VerifyFilePath(szPathName) ) {
		CCedtApp::m_szInitialWorkingDirectory = szPathName;
		return TRUE;
	} else return FALSE;
}

BOOL CFileWindow::ShowDirectoryItemProperties(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	HINSTANCE hResult = ::ShellExecute(NULL, "properties", szPathName, NULL, NULL, SW_SHOWNORMAL);
	return ((UINT)hResult > 32) ? TRUE : FALSE;

/*	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	SHObjectProperties( pFrame->m_hWnd, SHOP_FILEPATH, szPathName, NULL ); 
	// this function is available only in Win2000
	return TRUE; */
}

BOOL CFileWindow::OpenDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	if( VerifyFilePath(szPathName) ) {
		CCedtApp * pApp = (CCedtApp *)AfxGetApp(); if( ! pApp ) return FALSE;
		return pApp->PostOpenDocumentFile( szPathName, 0 );
	} else return FALSE;
}

BOOL CFileWindow::ExecuteDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	if( VerifyFilePath(szPathName) ) {
		CWnd * pWnd = AfxGetMainWnd(); if( ! pWnd ) return FALSE;
		HINSTANCE hResult = ::ShellExecute(NULL, "open", szPathName, NULL, NULL, SW_SHOWNORMAL);
		return ((UINT)hResult > 32) ? TRUE : FALSE;
	} else return FALSE;
}

BOOL CFileWindow::ExploreDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	if( ! VerifyFilePath(szPathName) ) {
		CWnd * pWnd = AfxGetMainWnd(); if( ! pWnd ) return FALSE;
		HINSTANCE hResult = ::ShellExecute(NULL, "open", szPathName, NULL, NULL, SW_SHOWNORMAL);
		return ((UINT)hResult > 32) ? TRUE : FALSE;
	} else return FALSE;
}

BOOL CFileWindow::FindInDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	CString szPathName = GetDirectoryItemPathName(hItem);
	if( ! VerifyFilePath(szPathName) ) {
		CWnd * pWnd = AfxGetMainWnd(); if( ! pWnd ) return FALSE;
		HINSTANCE hResult = ::ShellExecute(NULL, "find", szPathName, NULL, NULL, SW_SHOWNORMAL);
		return ((UINT)hResult > 32) ? TRUE : FALSE;
	} else return FALSE;
}

BOOL CFileWindow::MoveToDirectoryItem(HTREEITEM hItem, LPCTSTR lpszDestination)
{
	if( ! hItem ) return FALSE;

	TCHAR szFrom[MAX_PATH]; memset(szFrom, 0x00, sizeof(szFrom)); strcpy(szFrom, GetDirectoryItemPathName(hItem));
	TCHAR szDest[MAX_PATH]; memset(szDest, 0x00, sizeof(szDest)); strcpy(szDest, lpszDestination);
	CWnd * pWnd = AfxGetMainWnd();

	SHFILEOPSTRUCT fo; memset( & fo, 0x00, sizeof(SHFILEOPSTRUCT) );
	fo.hwnd = pWnd->m_hWnd;		fo.wFunc = FO_MOVE;
	fo.pFrom = szFrom;			fo.pTo = szDest;
	fo.fFlags = FOF_ALLOWUNDO;

	if( ! SHFileOperation( & fo ) && ! fo.fAnyOperationsAborted && ! VerifyPathName(szFrom) ) {
		HTREEITEM hParent = m_treDirectoryTree.GetParentItem(hItem);
		m_treDirectoryTree.SelectItem(hParent);
		m_treDirectoryTree.DeleteItem(hItem);
		return TRUE;
	} else return FALSE;
}

BOOL CFileWindow::CopyToDirectoryItem(HTREEITEM hItem, LPCTSTR lpszDestination)
{
	if( ! hItem ) return FALSE;

	TCHAR szFrom[MAX_PATH]; memset(szFrom, 0x00, sizeof(szFrom)); strcpy(szFrom, GetDirectoryItemPathName(hItem));
	TCHAR szDest[MAX_PATH]; memset(szDest, 0x00, sizeof(szDest)); strcpy(szDest, lpszDestination);
	CWnd * pWnd = AfxGetMainWnd();

	SHFILEOPSTRUCT fo; memset( & fo, 0x00, sizeof(SHFILEOPSTRUCT) );
	fo.hwnd = pWnd->m_hWnd;		fo.wFunc = FO_COPY;
	fo.pFrom = szFrom;			fo.pTo = szDest;
	fo.fFlags = FOF_ALLOWUNDO;

	if( ! SHFileOperation( & fo ) && ! fo.fAnyOperationsAborted ) {
		return TRUE;
	} else return FALSE;
}

BOOL CFileWindow::RenameDirectoryItem(HTREEITEM hItem, LPCTSTR lpszNewName)
{
	if( ! hItem ) return FALSE;

	TCHAR szFrom[MAX_PATH]; memset(szFrom, 0x00, sizeof(szFrom)); strcpy(szFrom, GetDirectoryItemPathName(hItem));
	TCHAR szDest[MAX_PATH]; memset(szDest, 0x00, sizeof(szDest)); strcpy(szDest, GetFileDirectory(szFrom) + "\\" + lpszNewName);
	CWnd * pWnd = AfxGetMainWnd();

	SHFILEOPSTRUCT fo; memset( & fo, 0x00, sizeof(SHFILEOPSTRUCT) );
	fo.hwnd = pWnd->m_hWnd;		fo.wFunc = FO_RENAME;
	fo.pFrom = szFrom;			fo.pTo = szDest;
	fo.fFlags = FOF_ALLOWUNDO;

	if( ! SHFileOperation( & fo ) && ! fo.fAnyOperationsAborted && ! VerifyPathName(szFrom) ) {
		return m_treDirectoryTree.SetItemText(hItem, lpszNewName);
	} else return FALSE;
}

BOOL CFileWindow::DeleteDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;

	TCHAR szFrom[MAX_PATH]; memset(szFrom, 0x00, sizeof(szFrom)); strcpy(szFrom, GetDirectoryItemPathName(hItem));
	CWnd * pWnd = AfxGetMainWnd();

	SHFILEOPSTRUCT fo; memset( & fo, 0x00, sizeof(SHFILEOPSTRUCT) );
	fo.hwnd = pWnd->m_hWnd;		fo.wFunc = FO_DELETE;
	fo.pFrom = szFrom;			fo.fFlags = FOF_ALLOWUNDO;

	if( ! SHFileOperation( & fo ) && ! fo.fAnyOperationsAborted && ! VerifyPathName(szFrom) ) {
		HTREEITEM hParent = m_treDirectoryTree.GetParentItem(hItem);
		m_treDirectoryTree.SelectItem(hParent);
		m_treDirectoryTree.DeleteItem(hItem);
		return TRUE;
	} else return FALSE;
}

BOOL CFileWindow::RefreshDirectoryItem(HTREEITEM hItem)
{
	if( ! hItem ) return FALSE;
	if( VerifyFilePath( GetDirectoryItemPathName(hItem) ) ) hItem = m_treDirectoryTree.GetParentItem(hItem);

	CWaitCursor wait;
	m_treDirectoryTree.SetRedraw(FALSE);

	CString szPath = GetDirectoryItemPathName(hItem);
	RemoveChildDirectoryItems( hItem );
	DisplayDirectoryPath(hItem, szPath);
	m_treDirectoryTree.Expand(hItem, TVE_EXPAND);

	m_treDirectoryTree.SelectItem( hItem );
	m_treDirectoryTree.SetRedraw(TRUE);

	return TRUE;
}
