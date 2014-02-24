// cedt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "cedtHeader.h"
#include "registry.h"
#include "HtmlHelp.h"
#include <afxmt.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Shared data segment
#pragma data_seg("Shared")
DWORD	nFirstThreadID = 0;
HWND	hFirstWindow = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:Shared,rws")


/////////////////////////////////////////////////////////////////////////////
// CCedtApp static member variables
CString CCedtApp::m_szInstallDirectory;
CString CCedtApp::m_szLoadingDirectory;

CString CCedtApp::m_szInitialWorkingDirectory;
CString CCedtApp::m_szRemoteBackupDirectory;

BOOL CCedtApp::m_bAllowMultiInstances = FALSE;
BOOL CCedtApp::m_bFirstInstance = TRUE;
BOOL CCedtApp::m_bDoubleByteCharacterSet = FALSE;

BOOL CCedtApp::m_bCreateNewDocumentOnStartup;
BOOL CCedtApp::m_bReloadWorkingFilesOnStartup;

// global cursor resources
HCURSOR CCedtApp::m_hCursorArrow, CCedtApp::m_hCursorIBeam, CCedtApp::m_hCursorCross; 
HCURSOR CCedtApp::m_hCursorRightArrow, CCedtApp::m_hCursorArrowMacro, CCedtApp::m_hCursorIBeamMacro;

// ftp accounts and file filters
CFtpAccount CCedtApp::m_clsFtpAccounts[MAX_FTP_ACCOUNT];
CFileFilter CCedtApp::m_clsFileFilters[MAX_FILE_FILTER];
INT CCedtApp::m_nFilterIndexDialog = 0;
INT CCedtApp::m_nFilterIndexPannel = 0;

// html help initialization
BOOL CCedtApp::m_bHtmlHelpInitialized = FALSE;
DWORD CCedtApp::m_dwHtmlHelpCookie = 0x00;

// open template
CString CCedtApp::m_szOpenTemplatePathName;
BOOL CCedtApp::m_bOpenTemplate;

/////////////////////////////////////////////////////////////////////////////
// CCedtApp

BEGIN_MESSAGE_MAP(CCedtApp, CWinApp)
	//{{AFX_MSG_MAP(CCedtApp)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile1)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_TOPICS, OnHelpTopics)
	ON_COMMAND(ID_HELP_VISIT_HOMEPAGE, OnHelpVisitHomepage)
	ON_COMMAND(ID_HELP_FEEDBACK, OnHelpFeedback)
	ON_COMMAND(ID_TOOL_PREFERENCES, OnToolPreferences)
	ON_COMMAND(ID_MACRO_CONFIGURE, OnMacroConfigure)
	ON_COMMAND(ID_COMMAND_CONFIGURE, OnCommandConfigure)
	ON_COMMAND(ID_VIEW_SET_COLORS, OnViewSetColors)
	ON_COMMAND(ID_FILE_FTP_SETTINGS, OnFileFtpSettings)
	ON_COMMAND(ID_FILE_OPEN_REMOTE, OnFileOpenRemote)
	ON_COMMAND(ID_FILE_OPEN_TEMPLATE, OnFileOpenTemplate)
	ON_COMMAND(ID_VIEW_SET_LINE_SPACING, OnViewSetLineSpacing)
	ON_COMMAND(ID_VIEW_SET_TAB_SIZE, OnViewSetTabSize)
	ON_COMMAND(ID_FILE_CLOSE_ALL, OnFileCloseAll)
	ON_COMMAND(ID_DOCU_SYNTAX_CUSTOMIZE, OnDocuSyntaxCustomize)
	ON_COMMAND(ID_VIEW_COLUMN_MARKERS, OnViewColumnMarkers)
	ON_COMMAND(ID_SEARCH_FIND_IN_FILES, OnSearchFindInFiles)
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_COMMAND(ID_PROJECT_CLOSE, OnProjectClose)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_PROJECT_NEW_CATEGORY, OnProjectNewCategory)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW_CATEGORY, OnUpdateProjectNewCategory)
	ON_COMMAND(ID_PROJECT_ADD_FILES_TO, OnProjectAddFilesTo)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_ADD_FILES_TO, OnUpdateProjectAddFilesTo)
	ON_COMMAND(ID_PROJECT_ADD_ACTIVE_FILE, OnProjectAddActiveFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_ADD_ACTIVE_FILE, OnUpdateProjectAddActiveFile)
	ON_COMMAND(ID_PROJECT_ADD_OPEN_FILES, OnProjectAddOpenFiles)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_ADD_OPEN_FILES, OnUpdateProjectAddOpenFiles)
	ON_COMMAND(ID_VIEW_SET_SCREEN_FONTS, OnViewSetScreenFonts)
	ON_COMMAND(ID_VIEW_SET_PRINTER_FONTS, OnViewSetPrinterFonts)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_USER_FILE0, OnUpdateCommandUserFile0)
	ON_UPDATE_COMMAND_UI(ID_MACRO_USER_FILE0, OnUpdateMacroUserFile0)
	ON_COMMAND(ID_COMMAND_USER_FILE0, OnCommandUserFile0)
	ON_COMMAND(ID_COMMAND_USER_FILE1, OnCommandUserFile1)
	ON_COMMAND(ID_COMMAND_USER_FILE2, OnCommandUserFile2)
	ON_COMMAND(ID_COMMAND_USER_FILE3, OnCommandUserFile3)
	ON_COMMAND(ID_COMMAND_USER_FILE4, OnCommandUserFile4)
	ON_COMMAND(ID_COMMAND_USER_FILE5, OnCommandUserFile5)
	ON_COMMAND(ID_COMMAND_USER_FILE6, OnCommandUserFile6)
	ON_COMMAND(ID_COMMAND_USER_FILE7, OnCommandUserFile7)
	ON_COMMAND(ID_MACRO_USER_FILE0, OnMacroUserFile0)
	ON_COMMAND(ID_MACRO_USER_FILE1, OnMacroUserFile1)
	ON_COMMAND(ID_MACRO_USER_FILE2, OnMacroUserFile2)
	ON_COMMAND(ID_MACRO_USER_FILE3, OnMacroUserFile3)
	ON_COMMAND(ID_MACRO_USER_FILE4, OnMacroUserFile4)
	ON_COMMAND(ID_MACRO_USER_FILE5, OnMacroUserFile5)
	ON_COMMAND(ID_MACRO_USER_FILE6, OnMacroUserFile6)
	ON_COMMAND(ID_MACRO_USER_FILE7, OnMacroUserFile7)
	ON_COMMAND(ID_EDIT_COLUMN_MODE, OnEditColumnMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COLUMN_MODE, OnUpdateEditColumnMode)
	ON_COMMAND(ID_INDICATOR_OVR, OnIndicatorOvr)
	ON_COMMAND(ID_VIEW_LINE_NUMBERS, OnViewLineNumbers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_NUMBERS, OnUpdateViewLineNumbers)
	ON_COMMAND(ID_VIEW_LINE_SPACING100, OnViewLineSpacing100)
	ON_COMMAND(ID_VIEW_LINE_SPACING110, OnViewLineSpacing110)
	ON_COMMAND(ID_VIEW_LINE_SPACING120, OnViewLineSpacing120)
	ON_COMMAND(ID_VIEW_LINE_SPACING150, OnViewLineSpacing150)
	ON_COMMAND(ID_VIEW_LINE_SPACING200, OnViewLineSpacing200)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_SPACING100, OnUpdateViewLineSpacing100)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_SPACING110, OnUpdateViewLineSpacing110)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_SPACING120, OnUpdateViewLineSpacing120)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_SPACING150, OnUpdateViewLineSpacing150)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_SPACING200, OnUpdateViewLineSpacing200)
	ON_COMMAND(ID_VIEW_SCREEN_FONT0, OnViewScreenFont0)
	ON_COMMAND(ID_VIEW_SCREEN_FONT1, OnViewScreenFont1)
	ON_COMMAND(ID_VIEW_SCREEN_FONT2, OnViewScreenFont2)
	ON_COMMAND(ID_VIEW_SCREEN_FONT3, OnViewScreenFont3)
	ON_COMMAND(ID_VIEW_SCREEN_FONT4, OnViewScreenFont4)
	ON_COMMAND(ID_VIEW_SCREEN_FONT5, OnViewScreenFont5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT0, OnUpdateViewScreenFont0)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT1, OnUpdateViewScreenFont1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT2, OnUpdateViewScreenFont2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT3, OnUpdateViewScreenFont3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT4, OnUpdateViewScreenFont4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_FONT5, OnUpdateViewScreenFont5)
	ON_COMMAND(ID_VIEW_PRINTER_FONT0, OnViewPrinterFont0)
	ON_COMMAND(ID_VIEW_PRINTER_FONT1, OnViewPrinterFont1)
	ON_COMMAND(ID_VIEW_PRINTER_FONT2, OnViewPrinterFont2)
	ON_COMMAND(ID_VIEW_PRINTER_FONT3, OnViewPrinterFont3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRINTER_FONT0, OnUpdateViewPrinterFont0)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRINTER_FONT1, OnUpdateViewPrinterFont1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRINTER_FONT2, OnUpdateViewPrinterFont2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRINTER_FONT3, OnUpdateViewPrinterFont3)
	ON_COMMAND(ID_VIEW_TAB_SIZE02, OnViewTabSize02)
	ON_COMMAND(ID_VIEW_TAB_SIZE04, OnViewTabSize04)
	ON_COMMAND(ID_VIEW_TAB_SIZE08, OnViewTabSize08)
	ON_COMMAND(ID_VIEW_TAB_SIZE16, OnViewTabSize16)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_SIZE02, OnUpdateViewTabSize02)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_SIZE04, OnUpdateViewTabSize04)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_SIZE08, OnUpdateViewTabSize08)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_SIZE16, OnUpdateViewTabSize16)
	ON_COMMAND(ID_VIEW_EMBOLDEN_KEYWORDS, OnViewEmboldenKeywords)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EMBOLDEN_KEYWORDS, OnUpdateViewEmboldenKeywords)
	ON_COMMAND(ID_VIEW_ITALICIZE_COMMENT, OnViewItalicizeComment)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITALICIZE_COMMENT, OnUpdateViewItalicizeComment)
	ON_COMMAND(ID_VIEW_SHOW_SPACES, OnViewShowSpaces)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_SPACES, OnUpdateViewShowSpaces)
	ON_COMMAND(ID_VIEW_SHOW_TAB_CHARS, OnViewShowTabChars)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_TAB_CHARS, OnUpdateViewShowTabChars)
	ON_COMMAND(ID_VIEW_SHOW_LINE_BREAK, OnViewShowLineBreak)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_LINE_BREAK, OnUpdateViewShowLineBreak)
	ON_COMMAND(ID_HELP_SPONSORING, OnHelpSponsoring)
	ON_COMMAND(ID_DIRECTORY_ITEM_OPEN, OnDirectoryItemOpen)
	ON_COMMAND(ID_DIRECTORY_ITEM_EXECUTE, OnDirectoryItemExecute)
	ON_COMMAND(ID_DIRECTORY_ITEM_EXPLORE, OnDirectoryItemExplore)
	ON_COMMAND(ID_DIRECTORY_ITEM_FIND, OnDirectoryItemFind)
	ON_COMMAND(ID_DIRECTORY_ITEM_SETWORKDIR, OnDirectoryItemSetWorkdir)
	ON_COMMAND(ID_DIRECTORY_ITEM_MOVE, OnDirectoryItemMove)
	ON_COMMAND(ID_DIRECTORY_ITEM_COPY, OnDirectoryItemCopy)
	ON_COMMAND(ID_DIRECTORY_ITEM_RENAME, OnDirectoryItemRename)
	ON_COMMAND(ID_DIRECTORY_ITEM_DELETE, OnDirectoryItemDelete)
	ON_COMMAND(ID_DIRECTORY_ITEM_REFRESH, OnDirectoryItemRefresh)
	ON_COMMAND(ID_PROJECT_ITEM_OPEN, OnProjectItemOpen)
	ON_COMMAND(ID_PROJECT_ITEM_EXECUTE, OnProjectItemExecute)
	ON_COMMAND(ID_PROJECT_ITEM_REMOVE, OnProjectItemRemove)
	ON_COMMAND(ID_PROJECT_ITEM_RENAME, OnProjectItemRename)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCedtApp construction
CCedtApp::CCedtApp()
{
	m_bPostOpenDocument = FALSE;
	m_nPostOpenFtpAccount = -1;
	m_szPostOpenPathName = "";
	m_dwPostOpenFileSize = 0;
	m_nPostOpenLineNum = 0;

	m_szPrevWorkspacePathName = "";
	m_bProjectLoaded = FALSE;
	m_szProjectPathName = "";
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCedtApp object
CCedtApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCedtApp initialization
BOOL CCedtApp::InitInstance()
{
	// Change the registry key under which our settings are stored.
	SetRegistryKey(STRING_COMPANYNAME);

	// read InstallDir from the registry
	if( ! GetRegKeyValue(HKEY_LOCAL_MACHINE, REGPATH_INSTALL_DIRECTORY, "InstallDir", m_szInstallDirectory) ) {
		CString szMessage; szMessage.Format(IDS_ERR_REGISTRY, REGPATH_INSTALL_DIRECTORY);
		AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP); return FALSE;
	}

	// get current working directory
	TCHAR szTemp[MAX_PATH]; GetCurrentDirectory(MAX_PATH, szTemp);
	m_szLoadingDirectory = ChopDirectory(szTemp);

	// load multi-instance flag
	m_bAllowMultiInstances = FALSE;
	if( ! LoadMultiInstancesFlag(REGKEY_ALLOW_MULTI) ) return FALSE;

	// is it first instance ?
	m_bFirstInstance = TRUE; 
	if( nFirstThreadID ) m_bFirstInstance = FALSE;

	if( ! m_bAllowMultiInstances && ! m_bFirstInstance ) {
		// do not allow multi-instance
		CMutex mutex(FALSE, MUTEX_NAME_CMDLINE);
		CSingleLock lock( & mutex ); lock.Lock();

		ofstream fout(m_szInstallDirectory + "\\cmdline.txt", ios::out | ios::app);
		fout << "/D:\"" << m_szLoadingDirectory << "\"";
		fout << " " << m_lpCmdLine << endl;
		fout.close(); 
		
		lock.Unlock();

		if( hFirstWindow && ::IsIconic(hFirstWindow) ) ShowWindow(hFirstWindow, SW_RESTORE);
		if( hFirstWindow ) ::SetForegroundWindow(hFirstWindow);
		::PostThreadMessage(nFirstThreadID, WM_ANOTHER_INSTANCE, 0, 0L);

		// ghost exit here !!!
		return FALSE; 

	} else {
		// otherwise save thread id for later use
		nFirstThreadID = m_nThreadID; 
	}

	// Initialize OLE 2.0 libraries
	if( ! AfxOleInit() ) {
		AfxMessageBox(IDS_ERR_OLE_INIT);
		return FALSE;
	}

	// Initialize common control library for XP Look
	InitCommonControls();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// check multi-byte language
	m_bDoubleByteCharacterSet = FALSE;
	CheckDoubleByteCharacterSet();

	// load user configuration
	if( ! LoadUserConfiguration() ) {
		AfxMessageBox(IDS_ERR_CONFIG_FILE, MB_OK | MB_ICONEXCLAMATION);
		SetDefaultConfiguration();
		SaveUserConfiguration();
	}

	// load color settings
	if( ! LoadColorSettings() ) {
		SetDefaultColorSettings();
		SaveColorSettings();
	}

	// load FTP account information
	LoadFtpAccountInfo(m_szInstallDirectory + "\\cedt.ftp");

	// load command & macro
	LoadUserCommands(m_szInstallDirectory + "\\cedt.cmd");
	LoadMacroBuffers(m_szInstallDirectory + "\\cedt.mac");

//	SetRegistryKey(STRING_COMPANYNAME); // Change the registry key under which our settings are stored.
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_CEDTTYPE,
		RUNTIME_CLASS(CCedtDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CCedtView));
	AddDocTemplate(pDocTemplate);

	// Create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if ( ! pMainFrame->LoadFrame(IDR_MAINFRAME) ) return FALSE;
	m_pMainWnd = pMainFrame;

	// save first instance window handle 
	if( ! hFirstWindow ) hFirstWindow = pMainFrame->m_hWnd;

	// Load cursor resources
	m_hCursorArrow = LoadStandardCursor(IDC_ARROW);
	m_hCursorIBeam = LoadStandardCursor(IDC_IBEAM);
	m_hCursorCross = LoadStandardCursor(IDC_CROSS);
	m_hCursorRightArrow = LoadCursor(IDC_RIGHT_ARROW);
	m_hCursorArrowMacro = LoadCursor(IDC_ARROW_MACRO);
	m_hCursorIBeamMacro = LoadCursor(IDC_IBEAM_MACRO);

	// Accept files from drag and drop
	pMainFrame->DragAcceptFiles(TRUE);

	// File window initialization
	CFileWindow * pFileWindow = (CFileWindow *)pMainFrame->GetFileWindow();

	pFileWindow->InitLocalDriveList(NULL);
	pFileWindow->InitFileFilterList( GetComposedFileFilter(), m_nFilterIndexPannel, CallbackFilterIndexPannel );
	pFileWindow->InitProjectWorkspace();

	// File window browsing directory & current working directory
	if( m_szInitialWorkingDirectory.GetLength() ) {
		pFileWindow->SetBrowsingDirectory( m_szInitialWorkingDirectory );
		// redundant code - SetBrowsingDirectory() will set current working directory
		::SetCurrentDirectory( m_szInitialWorkingDirectory );
	} else {
		LoadBrowsingDirectory(REGKEY_BROWSING_DIR);
		// current working directory might differ from browsing directory
		LoadWorkingDirectory(REGKEY_WORKING_DIR);
	}

	// Load prev workspace pathname
	m_szPrevWorkspacePathName = "";
	LoadWorkspaceFilePath(REGKEY_LAST_WORKSPACE);

	// Load window placement & bar state
	pMainFrame->LoadWindowPlacement(REGKEY_WINDOW_PLACEMENT);
	pMainFrame->LoadBarState(REGKEY_BAR_STATE);

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOW);
	pMainFrame->UpdateWindow();

	// Reload last working files
	if( m_bReloadWorkingFilesOnStartup && m_bFirstInstance ) ReloadLastWorkingFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCmdLine cmdLine(__argc, __argv);
	ProcessShellCommand(cmdLine);

	// Create new document on start up
	if( m_bCreateNewDocumentOnStartup ) CreateNewDocumentIfNothingLoaded();

	return TRUE;
}

int CCedtApp::ExitInstance() 
{
	// User configuration is saved when mainframe is closed and whenever there is a change
	// SaveUserConfiguration();

	// Uninitialize HtmlHelp if it was initialized
	if( m_bHtmlHelpInitialized ) {
		::HtmlHelp(NULL, NULL, HH_UNINITIALIZE, (DWORD)&m_dwHtmlHelpCookie);
		m_bHtmlHelpInitialized = FALSE;
	}

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CCedtApp operations

BOOL CCedtApp::CheckDoubleByteCharacterSet()
{
	if( GetSystemMetrics(SM_DBCSENABLED) ) m_bDoubleByteCharacterSet = TRUE;
	else m_bDoubleByteCharacterSet = FALSE;
	return TRUE;
}

BOOL CCedtApp::ProcessShellCommand(CCmdLine & rCmdLine)
{
	CString szOption, szDirectory; 
	BOOL bNoCreate = FALSE; INT nLineNumber = 1;

	if( rCmdLine.HasSwitch("NC", szOption) ) bNoCreate = TRUE;
	if( rCmdLine.HasSwitch("L" , szOption) ) nLineNumber = atoi(szOption);
	if( rCmdLine.HasSwitch("D" , szOption) ) szDirectory = ChopDirectory(szOption);

	CString szPrefix = ChopDirectory(m_szLoadingDirectory);
	if( szDirectory.GetLength() ) szPrefix = szDirectory;

	for(INT i = 0; i < rCmdLine.GetArgumentCount(); i++) {
		CString szArgument = rCmdLine.GetArgument(i);

		// check if it is absolute path without drive name, then append drive name
		if( szArgument[0] == '\\' && szArgument[1] != '\\' ) szArgument = szPrefix.Mid(0, 2) + szArgument;

		BOOL bAbsolute = FALSE, bWildCard = FALSE;
		if( szArgument.Mid(0, 2) == "\\\\" || szArgument.Mid(1, 2) == ":\\" ) bAbsolute = TRUE;
		if( szArgument.Find('*') >= 0 || szArgument.Find('?') >= 0 ) bWildCard = TRUE;

		CString szPathName = szArgument; 
		if( ! bAbsolute ) szPathName = szPrefix + "\\" + szArgument;

		CStringArray arrPathName; 
		BOOL bFound = FindAllFilePath(arrPathName, szPathName);

		if( bFound ) {
			for(INT j = 0; j < arrPathName.GetSize(); j++) {
				OpenDocumentFile( GetLongPathName(arrPathName[j]), nLineNumber, NULL );
			}
		} else if( ! bWildCard ) {
			if( ! bNoCreate ) {
				BOOL bCreated = TouchFile( szPathName ); // create a file
				OpenDocumentFile( GetLongPathName(szPathName), nLineNumber, NULL );
			} else { // if /NC is specified
			//	CString szMessage; szMessage.Format(IDS_FILE_NOT_FOUND_ERROR, szPathName);
			//	AfxMessageBox(szMessage, MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}

	return TRUE;
}

void CCedtApp::OnAnotherInstance()
{
/*	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd(); if( ! pMainFrame ) return;
	if( pMainFrame->IsIconic() ) pMainFrame->ShowWindow( SW_RESTORE );
	else pMainFrame->ShowWindow( SW_SHOW );

//	pMainFrame->BringWindowToTop();
	if( pMainFrame->SetForegroundWindow() ) {
		pMainFrame->ShowOwnedPopups( TRUE );
	} else {
	//	pMainFrame->ShowWindow( SW_MINIMIZE );
	//	pMainFrame->ShowWindow( SW_RESTORE );
	} */

	TCHAR szBuffer[4096]; CStringArray arrCmdLine;

	CMutex mutex(FALSE, MUTEX_NAME_CMDLINE);
	CSingleLock lock( & mutex ); lock.Lock();

	ifstream fin(m_szInstallDirectory + "\\cmdline.txt", ios::in | ios::nocreate);
	while( fin.good() ) {
		fin.getline(szBuffer, 4096);
		if( strlen(szBuffer) ) arrCmdLine.Add(szBuffer);
	}
	fin.close();

	ofstream fout(m_szInstallDirectory + "\\cmdline.txt", ios::out );
	fout.close();

	lock.Unlock();

	for(INT i = 0; i < arrCmdLine.GetSize(); i++) {
		CCmdLine cmdLine( arrCmdLine[i] );
		ProcessShellCommand(cmdLine);
	}
}

POSITION CCedtApp::GetFirstDocPosition()
{
	POSITION posTemplate = GetFirstDocTemplatePosition();
	CDocTemplate * pTemplate = GetNextDocTemplate( posTemplate );
	return pTemplate->GetFirstDocPosition();
}

CDocument * CCedtApp::GetNextDoc(POSITION & rPos)
{
	POSITION posTemplate = GetFirstDocTemplatePosition();
	CDocTemplate * pTemplate = GetNextDocTemplate( posTemplate );
	return pTemplate->GetNextDoc( rPos );
}

INT CCedtApp::GetDocumentCount()
{
	INT nCount = 0; POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) { 
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc ); 
		nCount++; 
	}
	return nCount;
}

INT CCedtApp::GetViewCount()
{
	INT nCount = 0; POSITION posDoc = GetFirstDocPosition();
	while( posDoc ) {
		CCedtDoc * pDoc = (CCedtDoc *)GetNextDoc( posDoc ); 
		nCount += pDoc->GetViewCount();
	}
	return nCount;
}

/////////////////////////////////////////////////////////////////////////////
// CCedtApp message handlers
BOOL CCedtApp::PreTranslateMessage(MSG* pMsg) 
{
	switch( pMsg->message ) {
	case WM_ANOTHER_INSTANCE:
		OnAnotherInstance();
		return TRUE;
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}

