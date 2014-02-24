// PreferenceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "cedtHeader.h"
#include "PrefDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


INT CPreferenceDialog::m_nActiveCategory = PREF_CATEGORY_GENERAL;

INT CPreferenceDialog::m_nActiveFontsPannel = FONTS_PANNEL_SCREEN;
INT CPreferenceDialog::m_nActiveScreenFont = 0;
INT CPreferenceDialog::m_nActivePrinterFont = 0;
INT CPreferenceDialog::m_nActiveMiscelFont = 0;

INT CPreferenceDialog::m_nActiveColorsPannel = COLORS_PANNEL_GENERAL;
INT CPreferenceDialog::m_nActiveColorScheme = 0; // default color scheme

CString CPreferenceDialog::m_szActiveAssocExtension = ""; // first extension in the registry

INT CPreferenceDialog::m_nActiveSyntaxType = 0;
INT CPreferenceDialog::m_nActiveFileFilter = 0;

INT CPreferenceDialog::m_nActiveUserCommand = 0;
INT CPreferenceDialog::m_nActiveMacroBuffer = 0;


BEGIN_MESSAGE_MAP(CPreferenceDialog, CDialog)
	//{{AFX_MSG_MAP(CPreferenceDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CATEGORIES, OnSelchangedCategories)
	ON_NOTIFY(NM_DBLCLK, IDC_SCREEN_FONTS, OnDblclkScreenFonts)
	ON_NOTIFY(NM_DBLCLK, IDC_PRINTER_FONTS, OnDblclkPrinterFonts)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_FIXED_WRAP_WIDTH_CHECK, OnFixedWrapWidthCheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SCREEN_FONTS, OnItemchangedScreenFonts)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PRINTER_FONTS, OnItemchangedPrinterFonts)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COMMAND_LIST, OnItemchangedCommandList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MACRO_LIST, OnItemchangedMacroList)
	ON_EN_CHANGE(IDC_MACRO_TEXT, OnChangeMacroText)
	ON_BN_CLICKED(IDC_MACRO_REMOVE, OnMacroRemove)
	ON_BN_CLICKED(IDC_MACRO_MOVE_UP, OnMacroMoveUp)
	ON_BN_CLICKED(IDC_MACRO_MOVE_DOWN, OnMacroMoveDown)
	ON_EN_CHANGE(IDC_COMMAND_TEXT, OnChangeCommandText)
	ON_EN_CHANGE(IDC_COMMAND_COMMAND, OnChangeCommandCommand)
	ON_EN_CHANGE(IDC_COMMAND_ARGUMENT, OnChangeCommandArgument)
	ON_EN_CHANGE(IDC_COMMAND_DIRECTORY, OnChangeCommandDirectory)
	ON_BN_CLICKED(IDC_COMMAND_CLOSE_ON_EXIT, OnCommandCloseOnExit)
	ON_BN_CLICKED(IDC_COMMAND_CAPTURE_OUTPUT, OnCommandCaptureOutput)
	ON_BN_CLICKED(IDC_COMMAND_SAVE_BEFORE, OnCommandSaveBefore)
	ON_BN_CLICKED(IDC_COMMAND_REMOVE, OnCommandRemove)
	ON_BN_CLICKED(IDC_COMMAND_MOVE_UP, OnCommandMoveUp)
	ON_BN_CLICKED(IDC_COMMAND_MOVE_DOWN, OnCommandMoveDown)
	ON_BN_CLICKED(IDC_COMMAND_COMMAND_BROWSE, OnCommandCommandBrowse)
	ON_BN_CLICKED(IDC_COMMAND_ARGUMENT_MENU, OnCommandArgumentMenu)
	ON_BN_CLICKED(IDC_COMMAND_DIRECTORY_MENU, OnCommandDirectoryMenu)
	ON_COMMAND(ID_ARGUMENT_FILE_PATH, OnArgumentFilePath)
	ON_COMMAND(ID_ARGUMENT_FILE_DIRECTORY, OnArgumentFileDirectory)
	ON_COMMAND(ID_ARGUMENT_FILE_NAME, OnArgumentFileName)
	ON_COMMAND(ID_ARGUMENT_FILE_TITLE, OnArgumentFileTitle)
	ON_COMMAND(ID_ARGUMENT_CURRENT_WORD, OnArgumentCurrentWord)
	ON_COMMAND(ID_DIRECTORY_FILE_DIRECTORY, OnDirectoryFileDirectory)
	ON_COMMAND(ID_DIRECTORY_BROWSE, OnDirectoryBrowse)
	ON_BN_CLICKED(IDC_PRINT_HEADER, OnPrintHeader)
	ON_BN_CLICKED(IDC_PRINT_FOOTER, OnPrintFooter)
	ON_BN_CLICKED(IDC_PRINT_HEADER1_MENU, OnPrintHeader1Menu)
	ON_BN_CLICKED(IDC_PRINT_HEADER2_MENU, OnPrintHeader2Menu)
	ON_BN_CLICKED(IDC_PRINT_HEADER0_MENU, OnPrintHeader0Menu)
	ON_BN_CLICKED(IDC_PRINT_FOOTER0_MENU, OnPrintFooter0Menu)
	ON_BN_CLICKED(IDC_PRINT_FOOTER1_MENU, OnPrintFooter1Menu)
	ON_BN_CLICKED(IDC_PRINT_FOOTER2_MENU, OnPrintFooter2Menu)
	ON_COMMAND(ID_HEADER0_FILE_PATH, OnHeader0FilePath)
	ON_COMMAND(ID_HEADER0_FILE_NAME, OnHeader0FileName)
	ON_COMMAND(ID_HEADER0_PAGE_NUMBER, OnHeader0PageNumber)
	ON_COMMAND(ID_HEADER0_TOTAL_PAGE, OnHeader0TotalPage)
	ON_COMMAND(ID_HEADER0_CURRENT_DATE, OnHeader0CurrentDate)
	ON_COMMAND(ID_HEADER0_CURRENT_TIME, OnHeader0CurrentTime)
	ON_COMMAND(ID_HEADER1_FILE_PATH, OnHeader1FilePath)
	ON_COMMAND(ID_HEADER1_FILE_NAME, OnHeader1FileName)
	ON_COMMAND(ID_HEADER1_PAGE_NUMBER, OnHeader1PageNumber)
	ON_COMMAND(ID_HEADER1_TOTAL_PAGE, OnHeader1TotalPage)
	ON_COMMAND(ID_HEADER1_CURRENT_DATE, OnHeader1CurrentDate)
	ON_COMMAND(ID_HEADER1_CURRENT_TIME, OnHeader1CurrentTime)
	ON_COMMAND(ID_HEADER2_FILE_PATH, OnHeader2FilePath)
	ON_COMMAND(ID_HEADER2_FILE_NAME, OnHeader2FileName)
	ON_COMMAND(ID_HEADER2_PAGE_NUMBER, OnHeader2PageNumber)
	ON_COMMAND(ID_HEADER2_TOTAL_PAGE, OnHeader2TotalPage)
	ON_COMMAND(ID_HEADER2_CURRENT_DATE, OnHeader2CurrentDate)
	ON_COMMAND(ID_HEADER2_CURRENT_TIME, OnHeader2CurrentTime)
	ON_COMMAND(ID_FOOTER0_FILE_PATH, OnFooter0FilePath)
	ON_COMMAND(ID_FOOTER0_FILE_NAME, OnFooter0FileName)
	ON_COMMAND(ID_FOOTER0_PAGE_NUMBER, OnFooter0PageNumber)
	ON_COMMAND(ID_FOOTER0_TOTAL_PAGE, OnFooter0TotalPage)
	ON_COMMAND(ID_FOOTER0_CURRENT_DATE, OnFooter0CurrentDate)
	ON_COMMAND(ID_FOOTER0_CURRENT_TIME, OnFooter0CurrentTime)
	ON_COMMAND(ID_FOOTER1_FILE_PATH, OnFooter1FilePath)
	ON_COMMAND(ID_FOOTER1_FILE_NAME, OnFooter1FileName)
	ON_COMMAND(ID_FOOTER1_PAGE_NUMBER, OnFooter1PageNumber)
	ON_COMMAND(ID_FOOTER1_TOTAL_PAGE, OnFooter1TotalPage)
	ON_COMMAND(ID_FOOTER1_CURRENT_DATE, OnFooter1CurrentDate)
	ON_COMMAND(ID_FOOTER1_CURRENT_TIME, OnFooter1CurrentTime)
	ON_COMMAND(ID_FOOTER2_FILE_PATH, OnFooter2FilePath)
	ON_COMMAND(ID_FOOTER2_FILE_NAME, OnFooter2FileName)
	ON_COMMAND(ID_FOOTER2_PAGE_NUMBER, OnFooter2PageNumber)
	ON_COMMAND(ID_FOOTER2_TOTAL_PAGE, OnFooter2TotalPage)
	ON_COMMAND(ID_FOOTER2_CURRENT_DATE, OnFooter2CurrentDate)
	ON_COMMAND(ID_FOOTER2_CURRENT_TIME, OnFooter2CurrentTime)
	ON_BN_CLICKED(IDC_WORKING_DIRECTORY_BROWSE, OnWorkingDirectoryBrowse)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILTERS, OnItemchangedFilters)
	ON_EN_CHANGE(IDC_FILTER_DESCRIPTION, OnChangeFilterDescription)
	ON_EN_CHANGE(IDC_FILTER_EXTENSION, OnChangeFilterExtension)
	ON_BN_CLICKED(IDC_FILTER_REMOVE, OnFilterRemove)
	ON_BN_CLICKED(IDC_FILTER_MOVE_UP, OnFilterMoveUp)
	ON_BN_CLICKED(IDC_FILTER_MOVE_DOWN, OnFilterMoveDown)
	ON_BN_CLICKED(IDC_COMMAND_SHORT_FILE_NAME, OnCommandShortFileName)
	ON_COMMAND(ID_ARGUMENT_LINE_NUMBER, OnArgumentLineNumber)
	ON_COMMAND(ID_ARGUMENT_USER_INPUT, OnArgumentUserInput)
	ON_BN_CLICKED(IDC_BACKGROUND_COLOR, OnBackgroundColor)
	ON_BN_CLICKED(IDC_LEFT_MARGIN_COLOR, OnLeftMarginColor)
	ON_BN_CLICKED(IDC_WORD_COLOR, OnWordColor)
	ON_BN_CLICKED(IDC_VARIABLE_COLOR, OnVariableColor)
	ON_BN_CLICKED(IDC_COMMENT_COLOR, OnCommentColor)
	ON_BN_CLICKED(IDC_CONSTANT_COLOR, OnConstantColor)
	ON_BN_CLICKED(IDC_DELIMITER_COLOR, OnDelimiterColor)
	ON_BN_CLICKED(IDC_LINE_NUMBER_COLOR, OnLineNumberColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_KEYWORD0_COLOR, OnKeyword0Color)
	ON_BN_CLICKED(IDC_KEYWORD1_COLOR, OnKeyword1Color)
	ON_BN_CLICKED(IDC_KEYWORD2_COLOR, OnKeyword2Color)
	ON_BN_CLICKED(IDC_KEYWORD3_COLOR, OnKeyword3Color)
	ON_BN_CLICKED(IDC_KEYWORD4_COLOR, OnKeyword4Color)
	ON_BN_CLICKED(IDC_KEYWORD5_COLOR, OnKeyword5Color)
	ON_BN_CLICKED(IDC_KEYWORD6_COLOR, OnKeyword6Color)
	ON_BN_CLICKED(IDC_KEYWORD7_COLOR, OnKeyword7Color)
	ON_BN_CLICKED(IDC_KEYWORD8_COLOR, OnKeyword8Color)
	ON_BN_CLICKED(IDC_KEYWORD9_COLOR, OnKeyword9Color)
	ON_BN_CLICKED(IDC_MAKE_BACKUP_FILE0, OnMakeBackupFile0)
	ON_BN_CLICKED(IDC_MAKE_BACKUP_FILE1, OnMakeBackupFile1)
	ON_BN_CLICKED(IDC_MAKE_BACKUP_FILE2, OnMakeBackupFile2)
	ON_BN_CLICKED(IDC_BACKUP_METHOD1, OnBackupMethod1)
	ON_BN_CLICKED(IDC_BACKUP_METHOD2, OnBackupMethod2)
	ON_BN_CLICKED(IDC_BACKUP_DIRECTORY_BROWSE, OnBackupDirectoryBrowse)
	ON_LBN_SELCHANGE(IDC_ASSOC_EXTENSIONS, OnSelchangeAssocExtensions)
	ON_LBN_SELCHANGE(IDC_ASSOC_ASSOCIATED, OnSelchangeAssocAssociated)
	ON_BN_CLICKED(IDC_ASSOC_ASSOCIATE, OnAssocAssociate)
	ON_BN_CLICKED(IDC_ASSOC_RESTORE, OnAssocRestore)
	ON_EN_KILLFOCUS(IDC_ASSOC_DESCRIPTION, OnKillfocusAssocDescription)
	ON_EN_KILLFOCUS(IDC_ASSOC_PROGRAM, OnKillfocusAssocProgram)
	ON_EN_KILLFOCUS(IDC_ASSOC_DEFAULTICON, OnKillfocusAssocDefaulticon)
	ON_BN_CLICKED(IDC_ACTIVE_LINE_COLOR, OnActiveLineColor)
	ON_BN_CLICKED(IDC_LOAD_COLOR_SCHEME, OnLoadColorScheme)
	ON_BN_CLICKED(IDC_HIGHLIGHTED_COLOR, OnHighlightedColor)
	ON_BN_CLICKED(IDC_SHADOWED_COLOR, OnShadowedColor)
	ON_BN_CLICKED(IDC_RANGE1_BKGR_COLOR, OnRange1BkgrColor)
	ON_BN_CLICKED(IDC_RANGE2_BKGR_COLOR, OnRange2BkgrColor)
	ON_BN_CLICKED(IDC_COLUMN_MARKER1_CHECK, OnColumnMarker1Check)
	ON_BN_CLICKED(IDC_COLUMN_MARKER2_CHECK, OnColumnMarker2Check)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SYNTAX_TYPES, OnItemchangedSyntaxTypes)
	ON_EN_CHANGE(IDC_SYNTAX_DESCRIPTION, OnChangeSyntaxDescription)
	ON_EN_CHANGE(IDC_SYNTAX_LANG_SPEC, OnChangeSyntaxLangSpec)
	ON_EN_CHANGE(IDC_SYNTAX_KEYWORDS, OnChangeSyntaxKeywords)
	ON_BN_CLICKED(IDC_SYNTAX_REMOVE, OnSyntaxRemove)
	ON_BN_CLICKED(IDC_SYNTAX_MOVE_UP, OnSyntaxMoveUp)
	ON_BN_CLICKED(IDC_SYNTAX_MOVE_DOWN, OnSyntaxMoveDown)
	ON_BN_CLICKED(IDC_SYNTAX_LANG_SPEC_BROWSE, OnSyntaxLangSpecBrowse)
	ON_BN_CLICKED(IDC_SYNTAX_KEYWORDS_BROWSE, OnSyntaxKeywordsBrowse)
	ON_BN_CLICKED(IDC_STRING_COLOR, OnStringColor)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MISCEL_FONTS, OnItemchangedMiscelFonts)
	ON_NOTIFY(NM_DBLCLK, IDC_MISCEL_FONTS, OnDblclkMiscelFonts)
	ON_NOTIFY(TCN_SELCHANGE, IDC_COLORS_PANNEL, OnSelchangeColorsPannel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FONTS_PANNEL, OnSelchangeFontsPannel)
	ON_EN_CHANGE(IDC_FILTER_DEFAULT_EXT, OnChangeFilterDefaultExt)
	ON_BN_CLICKED(IDC_SAVE_COLOR_SCHEME, OnSaveColorScheme)
	ON_CBN_SELCHANGE(IDC_COLOR_SCHEME_LIST, OnSelchangeColorSchemeList)
	ON_BN_CLICKED(IDC_COMMAND_SAVE_TOOLS, OnCommandSaveTools)
	ON_BN_CLICKED(IDC_MACRO_SAVE_MACROS, OnMacroSaveMacros)
	ON_COMMAND(ID_ARGUMENT_SELECT_PATH1, OnArgumentSelectPath1)
	ON_COMMAND(ID_ARGUMENT_SELECT_PATH2, OnArgumentSelectPath2)
	ON_COMMAND(ID_ARGUMENT_SELECT_DIRECTORY1, OnArgumentSelectDirectory1)
	ON_COMMAND(ID_ARGUMENT_SELECT_DIRECTORY2, OnArgumentSelectDirectory2)
	ON_BN_CLICKED(IDC_REMOTE_DIRECTORY_BROWSE, OnRemoteDirectoryBrowse)
	ON_BN_CLICKED(IDC_SHOW_LINE_NUMBERS, OnShowLineNumbers)
	ON_BN_CLICKED(IDC_COMMAND_LOAD_TOOLS, OnCommandLoadTools)
	ON_BN_CLICKED(IDC_MACRO_LOAD_MACROS, OnMacroLoadMacros)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog dialog
CPreferenceDialog::CPreferenceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferenceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferenceDialog)
	//}}AFX_DATA_INIT
}


void CPreferenceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferenceDialog)
	DDX_Control(pDX, IDC_MISCEL_FONTS, m_lstMiscelFonts);
	DDX_Control(pDX, IDC_MISCEL_FONTS_CAPTION, m_stcMiscelFonts);
	DDX_Control(pDX, IDC_MACRO_ITEM_BOX, m_stcMacroItemBox);
	DDX_Control(pDX, IDC_MACRO_SETTINGS_BOX, m_stcMacroSettingsBox);
	DDX_Control(pDX, IDC_COMMAND_ITEM_BOX, m_stcCommandItemBox);
	DDX_Control(pDX, IDC_COMMAND_SETTINGS_BOX, m_stcCommandSettingsBox);
	DDX_Control(pDX, IDC_SYNTAX_TYPE, m_lstSyntaxTypes);
	DDX_Control(pDX, IDC_TOOL_SETTINGS_BOX, m_stcToolSettingsBox);
	DDX_Control(pDX, IDC_FILTER_ITEM_BOX, m_stcFilterItemBox);
	DDX_Control(pDX, IDC_FILTER_SETTINGS_BOX, m_stcFilterSettingsBox);
	DDX_Control(pDX, IDC_SYNTAX_ITEM_BOX, m_stcSyntaxItemBox);
	DDX_Control(pDX, IDC_SYNTAX_SETTINGS_BOX, m_stcSyntaxSettingsBox);
	DDX_Control(pDX, IDC_BACKUP_METHOD_BOX, m_stcBackupMethodBox);
	DDX_Control(pDX, IDC_BACKUP_DIRECTORY_BOX, m_stcBackupDirectoryBox);
	DDX_Control(pDX, IDC_BACKUP_EXTENSION_BOX, m_stcBackupExtensionBox);
	DDX_Control(pDX, IDC_BACKUP_SETTINGS_BOX, m_stcBackupSettingsBox);
	DDX_Control(pDX, IDC_ASSOC_ITEM_BOX, m_stcAssocItemBox);
	DDX_Control(pDX, IDC_ASSOC_SETTINGS_BOX, m_stcAssocSettingsBox);
	DDX_Control(pDX, IDC_SAVE_REMOTE_FILES_UNIX, m_chkSaveRemoteFilesUnix);
	DDX_Control(pDX, IDC_SAVE_FILES_UNIX, m_chkSaveFilesUnix);
	DDX_Control(pDX, IDC_RELOAD_WORKING_FILES, m_chkReloadWorkingFiles);
	DDX_Control(pDX, IDC_CREATE_NEW_DOCUMENT, m_chkCreateNewDocument);
	DDX_Control(pDX, IDC_FILE_SETTINGS_BOX, m_stcFileSettingsBox);
	DDX_Control(pDX, IDC_REMOTE_DIRECTORY_DESC, m_stcRemoteDirectoryDesc);
	DDX_Control(pDX, IDC_REMOTE_DIRECTORY_BROWSE, m_btnRemoteDirectory);
	DDX_Control(pDX, IDC_REMOTE_DIRECTORY, m_edtRemoteDirectory);
	DDX_Control(pDX, IDC_REMOTE_DIRECTORY_CAPTION, m_stcRemoteDirectory);
	DDX_Control(pDX, IDC_REMOTE_DIRECTORY_BOX, m_stcRemoteDirectoryBox);
	DDX_Control(pDX, IDC_WORKING_DIRECTORY, m_edtWorkingDirectory);
	DDX_Control(pDX, IDC_WORKING_DIRECTORY_CAPTION, m_stcWorkingDirectory);
	DDX_Control(pDX, IDC_WORKING_DIRECTORY_BROWSE, m_btnWorkingDirectory);
	DDX_Control(pDX, IDC_WORKING_DIRECTORY_BOX, m_stcWorkingDirectoryBox);
	DDX_Control(pDX, IDC_PAGE_MARGIN_TITLE, m_stcPageMarginTitle);
	DDX_Control(pDX, IDC_PRINT_OPTIONS_BOX, m_stcPrintOptionsBox);
	DDX_Control(pDX, IDC_PRINT_FOOTER_BOX, m_stcPrintFooterBox);
	DDX_Control(pDX, IDC_PRINT_HEADER_BOX, m_stcPrintHeaderBox);
	DDX_Control(pDX, IDC_PAGE_MARGIN_BOX, m_stcPageMarginBox);
	DDX_Control(pDX, IDC_COLOR_SCHEME_BOX, m_stcColorSchemeBox);
	DDX_Control(pDX, IDC_VISUAL_OPTIONS_BOX, m_stcVisualOptionsBox);
	DDX_Control(pDX, IDC_COLUMN_MARKERS_BOX, m_stcColumnMarkersBox);
	DDX_Control(pDX, IDC_LINE_NUMBER_BOX, m_stcLineNumberBox);
	DDX_Control(pDX, IDC_LINE_SPACING_BOX, m_stcLineSpacingBox);
	DDX_Control(pDX, IDC_GENERAL_OPTIONS_BOX, m_stcGeneralOptionsBox);
	DDX_Control(pDX, IDC_WORD_WRAP_BOX, m_stcWordWrapBox);
	DDX_Control(pDX, IDC_TAB_SIZE_BOX, m_stcTabSizeBox);
	DDX_Control(pDX, IDC_MACRO_LOAD_MACROS, m_btnMacroLoadMacros);
	DDX_Control(pDX, IDC_COMMAND_LOAD_TOOLS, m_btnCommandLoadTools);
	DDX_Control(pDX, IDC_PRINT_LINE_NUMBERS, m_chkPrintLineNumbers);
	DDX_Control(pDX, IDC_SHOW_LINE_NUMBERS, m_chkShowLineNumbers);
	DDX_Control(pDX, IDC_SHOW_SELECTION_MARGIN, m_chkShowSelectionMargin);
	DDX_Control(pDX, IDC_SEARCH_WRAP_AT_END_OF_FILE, m_chkSearchWrapAtEndOfFile);
	DDX_Control(pDX, IDC_OPEN_DOCUMENT_WORD_WRAPPED, m_chkOpenDocumentWordWrapped);
	DDX_Control(pDX, IDC_ENABLE_DRAG_AND_DROP_EDITING, m_chkEnableDragAndDropEditing);
	DDX_Control(pDX, IDC_HOME_KEY_GOES_TO_FIRST_POSITION, m_chkHomeKeyGoesToFirstPosition);
	DDX_Control(pDX, IDC_ENABLE_AUTO_INDENT, m_chkEnableAutoIndent);
	DDX_Control(pDX, IDC_ENABLE_PAIRS_MATCHING, m_chkEnablePairsMatching);
	DDX_Control(pDX, IDC_SHOW_TAB_CHARS, m_chkShowTabChars);
	DDX_Control(pDX, IDC_MACRO_LIST, m_lstMacroList);
	DDX_Control(pDX, IDC_COMMAND_LIST, m_lstCommandList);
	DDX_Control(pDX, IDC_MACRO_SAVE_MACROS, m_btnMacroSaveMacros);
	DDX_Control(pDX, IDC_COMMAND_SAVE_TOOLS, m_btnCommandSaveTools);
	DDX_Control(pDX, IDC_ASSOC_ASSOCIATE_EXTENSION, m_edtAssocAssociate);
	DDX_Control(pDX, IDC_ASSOC_DEFAULTICON_CAPTION, m_stcAssocDefaultIcon);
	DDX_Control(pDX, IDC_ASSOC_DEFAULTICON_BROWSE, m_btnAssocDefaultIcon);
	DDX_Control(pDX, IDC_ASSOC_DEFAULTICON, m_edtAssocDefaultIcon);
	DDX_Control(pDX, IDC_ASSOC_RESTORE, m_btnAssocRestore);
	DDX_Control(pDX, IDC_ASSOC_PROGRAM_CAPTION, m_stcAssocProgram);
	DDX_Control(pDX, IDC_ASSOC_PROGRAM_BROWSE, m_btnAssocProgram);
	DDX_Control(pDX, IDC_ASSOC_PROGRAM, m_edtAssocProgram);
	DDX_Control(pDX, IDC_ASSOC_EXTENSIONS, m_lstAssocExtensions);
	DDX_Control(pDX, IDC_ASSOC_DESCRIPTION_CAPTION, m_stcAssocDescription);
	DDX_Control(pDX, IDC_ASSOC_DESCRIPTION, m_edtAssocDescription);
	DDX_Control(pDX, IDC_ASSOC_ASSOCIATED_CAPTION, m_stcAssocAssociated);
	DDX_Control(pDX, IDC_ASSOC_ASSOCIATED, m_lstAssocAssociated);
	DDX_Control(pDX, IDC_ASSOC_ASSOCIATE, m_btnAssocAssociate);
	DDX_Control(pDX, IDC_SAVE_COLOR_SCHEME, m_btnSaveColorScheme);
	DDX_Control(pDX, IDC_SAVE_COLOR_SCHEME_CAPTION, m_stcSaveColorScheme);
	DDX_Control(pDX, IDC_FILTER_DEFAULT_EXT, m_edtFilterDefaultExt);
	DDX_Control(pDX, IDC_FILTER_DEFAULT_EXT_CAPTION, m_stcFilterDefaultExt);
	DDX_Control(pDX, IDC_USE_SPACES_IN_PLACE_OF_TAB, m_chkUseSpacesInPlaceOfTab);
	DDX_Control(pDX, IDC_EMBOLDEN_KEYWORDS, m_chkEmboldenKeywords);
	DDX_Control(pDX, IDC_LINE_NUMBER_DIGITS, m_edtLineNumberDigits);
	DDX_Control(pDX, IDC_LINE_NUMBER_DIGITS_CAPTION, m_stcLineNumberDigits);
	DDX_Control(pDX, IDC_HIGHLIGHTED_COLOR_CAPTION, m_stcHighlightedColor);
	DDX_Control(pDX, IDC_RANGE_BKGR_COLOR_CAPTION, m_stcRangeBkgrColor);
	DDX_Control(pDX, IDC_LEFT_MARGIN_COLOR_CAPTION, m_stcLeftMarginColor);
	DDX_Control(pDX, IDC_FONTS_PANNEL, m_tabFontsPannel);
	DDX_Control(pDX, IDC_COLORS_PANNEL, m_tabColorsPannel);
	DDX_Control(pDX, IDC_STRING_COLOR, m_btnStringColor);
	DDX_Control(pDX, IDC_ITALICIZE_COMMENT, m_chkItalicizeComment);
	DDX_Control(pDX, IDC_FILTER_EXTENSIONS_CAPTION, m_stcFilterExtensions);
	DDX_Control(pDX, IDC_FILTER_EXTENSIONS, m_edtFilterExtensions);
	DDX_Control(pDX, IDC_FILE_FILTERS, m_lstFileFilters);
	DDX_Control(pDX, IDC_SYNTAX_MOVE_UP, m_btnSyntaxMoveUp);
	DDX_Control(pDX, IDC_SYNTAX_KEYWORDS_BROWSE, m_btnSyntaxKeywords);
	DDX_Control(pDX, IDC_SYNTAX_KEYWORDS, m_edtSyntaxKeywords);
	DDX_Control(pDX, IDC_SYNTAX_KEYWORDS_CAPTION, m_stcSyntaxKeywords);
	DDX_Control(pDX, IDC_SYNTAX_LANG_SPEC_BROWSE, m_btnSyntaxLangSpec);
	DDX_Control(pDX, IDC_SYNTAX_LANG_SPEC, m_edtSyntaxLangSpec);
	DDX_Control(pDX, IDC_SYNTAX_LANG_SPEC_CAPTION, m_stcSyntaxLangSpec);
	DDX_Control(pDX, IDC_SYNTAX_DESCRIPTION, m_edtSyntaxDescription);
	DDX_Control(pDX, IDC_SYNTAX_DESCRIPTION_CAPTION, m_stcSyntaxDescription);
	DDX_Control(pDX, IDC_SYNTAX_REMOVE, m_btnSyntaxRemove);
	DDX_Control(pDX, IDC_SYNTAX_MOVE_DOWN, m_btnSyntaxMoveDown);
	DDX_Control(pDX, IDC_COLUMN_MARKER2_POS, m_edtColumnMarker2);
	DDX_Control(pDX, IDC_COLUMN_MARKER1_POS, m_edtColumnMarker1);
	DDX_Control(pDX, IDC_COLUMN_MARKER2_CHECK, m_chkColumnMarker2);
	DDX_Control(pDX, IDC_COLUMN_MARKER1_CHECK, m_chkColumnMarker1);
	DDX_Control(pDX, IDC_RANGE2_BKGR_COLOR, m_btnRange2BkgrColor);
	DDX_Control(pDX, IDC_RANGE1_BKGR_COLOR, m_btnRange1BkgrColor);
	DDX_Control(pDX, IDC_SHADOWED_COLOR, m_btnShadowedColor);
	DDX_Control(pDX, IDC_HIGHLIGHTED_COLOR, m_btnHighlightedColor);
	DDX_Control(pDX, IDC_COLOR_SCHEME_LIST, m_cmbColorSchemeList);
	DDX_Control(pDX, IDC_LOAD_COLOR_SCHEME, m_btnLoadColorScheme);
	DDX_Control(pDX, IDC_LOAD_COLOR_SCHEME_CAPTION, m_stcLoadColorScheme);
	DDX_Control(pDX, IDC_ACTIVE_LINE_COLOR, m_btnActiveLineColor);
	DDX_Control(pDX, IDC_ADD_TO_RIGHT_MOUSE_BUTTON_DESC, m_stcAddToRightMouseButton);
	DDX_Control(pDX, IDC_ADD_TO_RIGHT_MOUSE_BUTTON, m_chkAddToRightMouseButton);
	DDX_Control(pDX, IDC_TEXT_COLOR_CAPTION, m_stcTextColor);
	DDX_Control(pDX, IDC_KEYWORD_COLOR_CAPTION, m_stcKeywordColor);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR_CAPTION, m_stcBackgroundColor);
	DDX_Control(pDX, IDC_BACKUP_DIRECTORY_BROWSE, m_btnBackupDirectory);
	DDX_Control(pDX, IDC_BACKUP_METHOD2, m_btnBackupMethod2);
	DDX_Control(pDX, IDC_BACKUP_METHOD1, m_btnBackupMethod1);
	DDX_Control(pDX, IDC_BACKUP_METHOD_CAPTION, m_stcBackupMethod);
	DDX_Control(pDX, IDC_BACKUP_DIRECTORY_DESC, m_stcBackupDirectoryDesc);
	DDX_Control(pDX, IDC_BACKUP_DIRECTORY, m_edtBackupDirectory);
	DDX_Control(pDX, IDC_BACKUP_DIRECTORY_CAPTION, m_stcBackupDirectory);
	DDX_Control(pDX, IDC_BACKUP_EXTENSION_DESC2, m_stcBackupExtensionDesc2);
	DDX_Control(pDX, IDC_BACKUP_EXTENSION_DESC1, m_stcBackupExtensionDesc1);
	DDX_Control(pDX, IDC_BACKUP_EXTENSION, m_edtBackupExtension);
	DDX_Control(pDX, IDC_BACKUP_EXTENSION_CAPTION, m_stcBackupExtension);
	DDX_Control(pDX, IDC_MAKE_BACKUP_FILE2, m_btnMakeBackupFile2);
	DDX_Control(pDX, IDC_MAKE_BACKUP_FILE1, m_btnMakeBackupFile1);
	DDX_Control(pDX, IDC_MAKE_BACKUP_FILE0, m_btnMakeBackupFile0);
	DDX_Control(pDX, IDC_KEYWORD9_COLOR, m_btnKeyword9Color);
	DDX_Control(pDX, IDC_KEYWORD8_COLOR, m_btnKeyword8Color);
	DDX_Control(pDX, IDC_KEYWORD7_COLOR, m_btnKeyword7Color);
	DDX_Control(pDX, IDC_KEYWORD6_COLOR, m_btnKeyword6Color);
	DDX_Control(pDX, IDC_KEYWORD5_COLOR, m_btnKeyword5Color);
	DDX_Control(pDX, IDC_KEYWORD4_COLOR, m_btnKeyword4Color);
	DDX_Control(pDX, IDC_KEYWORD3_COLOR, m_btnKeyword3Color);
	DDX_Control(pDX, IDC_KEYWORD2_COLOR, m_btnKeyword2Color);
	DDX_Control(pDX, IDC_KEYWORD1_COLOR, m_btnKeyword1Color);
	DDX_Control(pDX, IDC_KEYWORD0_COLOR, m_btnKeyword0Color);
	DDX_Control(pDX, IDC_LINE_NUMBER_COLOR, m_btnLineNumberColor);
	DDX_Control(pDX, IDC_DELIMITER_COLOR, m_btnDelimiterColor);
	DDX_Control(pDX, IDC_CONSTANT_COLOR, m_btnConstantColor);
	DDX_Control(pDX, IDC_COMMENT_COLOR, m_btnCommentColor);
	DDX_Control(pDX, IDC_VARIABLE_COLOR, m_btnVariableColor);
	DDX_Control(pDX, IDC_WORD_COLOR, m_btnWordColor);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR, m_btnBackgroundColor);
	DDX_Control(pDX, IDC_LEFT_MARGIN_COLOR, m_btnLeftMarginColor);
	DDX_Control(pDX, IDC_REMOVE_TRAILING_SPACES, m_chkRemoveTrailingSpaces);
	DDX_Control(pDX, IDC_CONVERT_TABS_TO_SPACES, m_chkConvertTabsToSpaces);
	DDX_Control(pDX, IDC_SHOW_LINE_BREAK, m_chkShowLineBreak);
	DDX_Control(pDX, IDC_FILE_TAB_LENGTH_CAPTION, m_stcFileTabLength);
	DDX_Control(pDX, IDC_FILE_TAB_LENGTH, m_edtFileTabLength);
	DDX_Control(pDX, IDC_CARET_WIDTH_DESC, m_stcCaretWidthDesc);
	DDX_Control(pDX, IDC_CARET_WIDTH, m_edtCaretWidth);
	DDX_Control(pDX, IDC_CARET_WIDTH_CAPTION, m_stcCaretWidth);
	DDX_Control(pDX, IDC_MACRO_HOTKEY_CAPTION, m_stcMacroHotKey);
	DDX_Control(pDX, IDC_COMMAND_HOTKEY_CAPTION, m_stcCommandHotKey);
	DDX_Control(pDX, IDC_MACRO_HOTKEY, m_hkyMacroHotKey);
	DDX_Control(pDX, IDC_COMMAND_HOTKEY, m_hkyCommandHotKey);
	DDX_Control(pDX, IDC_WRAP_INDENTATION_CAPTION, m_stcWrapIndentation);
	DDX_Control(pDX, IDC_TAB_SIZE_CAPTION, m_stcTabSize);
	DDX_Control(pDX, IDC_SCREEN_FONTS_CAPTION, m_stcScreenFonts);
	DDX_Control(pDX, IDC_PRINTER_FONTS_CAPTION, m_stcPrinterFonts);
	DDX_Control(pDX, IDC_PRINT_HEADER2_CAPTION, m_stcPrintHeader2);
	DDX_Control(pDX, IDC_PRINT_HEADER1_CAPTION, m_stcPrintHeader1);
	DDX_Control(pDX, IDC_PRINT_HEADER0_CAPTION, m_stcPrintHeader0);
	DDX_Control(pDX, IDC_PRINT_FOOTER2_CAPTION, m_stcPrintFooter2);
	DDX_Control(pDX, IDC_PRINT_FOOTER1_CAPTION, m_stcPrintFooter1);
	DDX_Control(pDX, IDC_PRINT_FOOTER0_CAPTION, m_stcPrintFooter0);
	DDX_Control(pDX, IDC_PAGE_MARGIN_TOP_ST, m_stcPageMarginTop);
	DDX_Control(pDX, IDC_PAGE_MARGIN_RIGHT_ST, m_stcPageMarginRight);
	DDX_Control(pDX, IDC_PAGE_MARGIN_LEFT_ST, m_stcPageMarginLeft);
	DDX_Control(pDX, IDC_PAGE_MARGIN_BOTTOM_ST, m_stcPageMarginBottom);
	DDX_Control(pDX, IDC_MACRO_TEXT_CAPTION, m_stcMacroText);
	DDX_Control(pDX, IDC_LINE_SPACING_CAPTION, m_stcLineSpacing);
	DDX_Control(pDX, IDC_COMMAND_TEXT_CAPTION, m_stcCommandText);
	DDX_Control(pDX, IDC_COMMAND_DIRECTORY_CAPTION, m_stcCommandDirectory);
	DDX_Control(pDX, IDC_COMMAND_COMMAND_CAPTION, m_stcCommandCommand);
	DDX_Control(pDX, IDC_COMMAND_ARGUMENT_CAPTION, m_stcCommandArgument);
	DDX_Control(pDX, IDC_CATEGORIES, m_treCategories);
	DDX_Control(pDX, IDC_CATEGORIES_CAPTION, m_stcCategories);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_PRINT_SYNTAX_HIGHLIGHT, m_chkPrintSyntaxHighlight);
	DDX_Control(pDX, IDC_COMMAND_SHORT_FILE_NAME, m_chkCommandShortFileName);
	DDX_Control(pDX, IDC_COMMAND_DIRECTORY_MENU, m_btnCommandDirectory);
	DDX_Control(pDX, IDC_FILTER_DESCRIPTION_CAPTION, m_stcFilterDescription);
	DDX_Control(pDX, IDC_FILTER_DESCRIPTION, m_edtFilterDescription);
	DDX_Control(pDX, IDC_FILTER_MOVE_DOWN, m_btnFilterMoveDown);
	DDX_Control(pDX, IDC_FILTER_MOVE_UP, m_btnFilterMoveUp);
	DDX_Control(pDX, IDC_FILTER_REMOVE, m_btnFilterRemove);
	DDX_Control(pDX, IDC_PRINT_HEADER0, m_edtPrintHeader0);
	DDX_Control(pDX, IDC_PRINT_FOOTER2_MENU, m_btnPrintFooter2);
	DDX_Control(pDX, IDC_PRINT_FOOTER1_MENU, m_btnPrintFooter1);
	DDX_Control(pDX, IDC_PRINT_FOOTER0_MENU, m_btnPrintFooter0);
	DDX_Control(pDX, IDC_PRINT_FOOTER2, m_edtPrintFooter2);
	DDX_Control(pDX, IDC_PRINT_FOOTER1, m_edtPrintFooter1);
	DDX_Control(pDX, IDC_PRINT_FOOTER0, m_edtPrintFooter0);
	DDX_Control(pDX, IDC_PRINT_HEADER2_MENU, m_btnPrintHeader2);
	DDX_Control(pDX, IDC_PRINT_HEADER1_MENU, m_btnPrintHeader1);
	DDX_Control(pDX, IDC_PRINT_HEADER0_MENU, m_btnPrintHeader0);
	DDX_Control(pDX, IDC_PRINT_HEADER2, m_edtPrintHeader2);
	DDX_Control(pDX, IDC_PRINT_HEADER1, m_edtPrintHeader1);
	DDX_Control(pDX, IDC_PRINT_FOOTER, m_chkPrintFooter);
	DDX_Control(pDX, IDC_PRINT_HEADER, m_chkPrintHeader);
	DDX_Control(pDX, IDC_PAGE_MARGIN_BOTTOM, m_edtPageMarginBottom);
	DDX_Control(pDX, IDC_PAGE_MARGIN_TOP, m_edtPageMarginTop);
	DDX_Control(pDX, IDC_PAGE_MARGIN_RIGHT, m_edtPageMarginRight);
	DDX_Control(pDX, IDC_PAGE_MARGIN_LEFT, m_edtPageMarginLeft);
	DDX_Control(pDX, IDC_WRAP_INDENTATION, m_edtWrapIndentation);
	DDX_Control(pDX, IDC_TAB_SIZE, m_edtTabSize);
	DDX_Control(pDX, IDC_MACRO_TEXT, m_edtMacroText);
	DDX_Control(pDX, IDC_LINE_SPACING, m_edtLineSpacing);
	DDX_Control(pDX, IDC_FIXED_WRAP_WIDTH, m_edtFixedWrapWidth);
	DDX_Control(pDX, IDC_COMMAND_TEXT, m_edtCommandText);
	DDX_Control(pDX, IDC_COMMAND_DIRECTORY, m_edtCommandDirectory);
	DDX_Control(pDX, IDC_COMMAND_COMMAND, m_edtCommandCommand);
	DDX_Control(pDX, IDC_COMMAND_ARGUMENT, m_edtCommandArgument);
	DDX_Control(pDX, IDC_USE_IN_INTERNET_EXPLORER, m_chkUseInInternetExplorer);
	DDX_Control(pDX, IDC_HIGHLIGHT_ACTIVE_LINE, m_chkHighlightActiveLine);
	DDX_Control(pDX, IDC_COMMAND_MOVE_DOWN, m_btnCommandMoveDown);
	DDX_Control(pDX, IDC_COMMAND_MOVE_UP, m_btnCommandMoveUp);
	DDX_Control(pDX, IDC_MACRO_MOVE_DOWN, m_btnMacroMoveDown);
	DDX_Control(pDX, IDC_MACRO_MOVE_UP, m_btnMacroMoveUp);
	DDX_Control(pDX, IDC_COMMAND_ARGUMENT_MENU, m_btnCommandArgument);
	DDX_Control(pDX, IDC_COMMAND_COMMAND_BROWSE, m_btnCommandCommand);
	DDX_Control(pDX, IDC_COMMAND_SAVE_BEFORE, m_chkCommandSaveBefore);
	DDX_Control(pDX, IDC_COMMAND_CLOSE_ON_EXIT, m_chkCommandCloseOnExit);
	DDX_Control(pDX, IDC_COMMAND_CAPTURE_OUTPUT, m_chkCommandCaptureOutput);
	DDX_Control(pDX, IDC_COMMAND_REMOVE, m_btnCommandRemove);
	DDX_Control(pDX, IDC_MACRO_REMOVE, m_btnMacroRemove);
	DDX_Control(pDX, IDC_PRINTER_FONTS, m_lstPrinterFonts);
	DDX_Control(pDX, IDC_SCREEN_FONTS, m_lstScreenFonts);
	DDX_Control(pDX, IDC_ALLOW_MULTIPLE_INSTANCE, m_chkAllowMultiInstances);
	DDX_Control(pDX, IDC_FIXED_WRAP_WIDTH_CHECK, m_chkFixedWrapWidth);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog dialog
void CPreferenceDialog::InitAllPrefPages()
{
	m_lstButtonImage.Create(IDB_GENERAL_BUTTONS, 9, 0, RGB(255, 0, 255));

	m_ctlSeparator.SubclassDlgItem(IDC_SEPARATOR, this);
	m_ctlSeparator.SetWindowText("");

	// categories
	m_hItemGeneral = InsertCategoryItem( IDS_PREF_CATEGORY_GENERAL, TVI_ROOT, PREF_CATEGORY_GENERAL );
	m_hItemVisual = InsertCategoryItem( IDS_PREF_CATEGORY_VISUAL, m_hItemGeneral, PREF_CATEGORY_VISUAL );
	m_hItemColors = InsertCategoryItem( IDS_PREF_CATEGORY_COLORS, m_hItemGeneral, PREF_CATEGORY_COLORS );
	m_hItemFonts = InsertCategoryItem( IDS_PREF_CATEGORY_FONTS, m_hItemGeneral, PREF_CATEGORY_FONTS );
	m_hItemPrint = InsertCategoryItem( IDS_PREF_CATEGORY_PRINT, m_hItemGeneral, PREF_CATEGORY_PRINT );

	m_hItemFile = InsertCategoryItem( IDS_PREF_CATEGORY_FILE, TVI_ROOT, PREF_CATEGORY_FILE );
	m_hItemBackup = InsertCategoryItem( IDS_PREF_CATEGORY_BACKUP, m_hItemFile, PREF_CATEGORY_BACKUP );
	m_hItemAssoc = InsertCategoryItem( IDS_PREF_CATEGORY_ASSOC, m_hItemFile, PREF_CATEGORY_ASSOC );
	m_hItemFilters = InsertCategoryItem( IDS_PREF_CATEGORY_FILTERS, m_hItemFile, PREF_CATEGORY_FILTERS );
	m_hItemSyntax = InsertCategoryItem( IDS_PREF_CATEGORY_SYNTAX, m_hItemFile, PREF_CATEGORY_SYNTAX );

	m_hItemTools = InsertCategoryItem( IDS_PREF_CATEGORY_TOOLS, TVI_ROOT, PREF_CATEGORY_TOOLS );
	m_hItemCommands = InsertCategoryItem( IDS_PREF_CATEGORY_COMMANDS, m_hItemTools, PREF_CATEGORY_COMMANDS );
	m_hItemMacros = InsertCategoryItem( IDS_PREF_CATEGORY_MACROS, m_hItemTools, PREF_CATEGORY_MACROS );

	// init each page
	InitGeneralPage();			InitVisualPage();			InitFontsPage();
	InitColorsPage();			InitPrintPage();
	InitFilePage();				InitAssocPage();			InitBackupPage();
	InitSyntaxPage();			InitFiltersPage();
	InitToolsPage();			InitCommandsPage();			InitMacrosPage();

}

void CPreferenceDialog::SizeAllPrefPages()
{
	RECT rect; GetWindowRect( & rect ); 
	MoveWindow( rect.left, rect.top, 546, 440 );

	// categories & buttons
	INT nPosY;

	nPosY  =   8; m_stcCategories.MoveWindow(8, nPosY, 150, 14);
	nPosY +=  18; m_treCategories.MoveWindow(8, nPosY, 150, 330);

	nPosY  = 360; m_ctlSeparator.MoveWindow(8, nPosY, 530, 14);
	nPosY +=  16; m_btnOK.MoveWindow(240, nPosY, 90, 22);
	nPosY +=   0; m_btnCancel.MoveWindow(340, nPosY, 90, 22);
	nPosY +=   0; m_btnApply.MoveWindow(440, nPosY, 90, 22);

	// resize each page
	SizeGeneralPage();			SizeVisualPage();			SizeFontsPage();
	SizeColorsPage();			SizePrintPage();
	SizeFilePage();				SizeAssocPage();			SizeBackupPage();
	SizeSyntaxPage();			SizeFiltersPage();
	SizeToolsPage();			SizeCommandsPage();			SizeMacrosPage();
}

void CPreferenceDialog::ShowProperPrefPages()
{
	ShowGeneralPage();			ShowVisualPage();			ShowFontsPage();
	ShowColorsPage();			ShowPrintPage();
	ShowFilePage();				ShowAssocPage();			ShowBackupPage();
	ShowSyntaxPage();			ShowFiltersPage();
	ShowToolsPage();			ShowCommandsPage();			ShowMacrosPage();
}

BOOL CPreferenceDialog::LoadAllPrefSettings()
{
	LoadGeneralSettings();		LoadVisualSettings();		LoadFontSettings();
	LoadColorSettings();		LoadPrintSettings();
	LoadFileSettings();		/*	LoadAssocSettings(); */		LoadBackupSettings();
	LoadSyntaxTypes();			LoadFileFilters();
	LoadToolsSettings();		LoadUserCommands();			LoadMacroBuffers();

	return TRUE;
}

BOOL CPreferenceDialog::ApplyAllPrefSettings()
{
	SaveGeneralSettings();		SaveVisualSettings();		SaveFontSettings();
	SaveColorSettings();		SavePrintSettings();
	SaveFileSettings();			SaveAssocSettings();		SaveBackupSettings();
	SaveSyntaxTypes();			SaveFileFilters();
	SaveToolsSettings();		SaveUserCommands();			SaveMacroBuffers();

	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->ApplyOutputWindowFont(TRUE);

	CCedtApp * pApp = (CCedtApp *)AfxGetApp();
	pApp->ApplyFileFilterToFileWindow();

	pApp->SaveMultiInstancesFlag(REGKEY_ALLOW_MULTI);
	pApp->ApplyPreferencesToAllViews(); // CCedtApp::SaveUserConfiguration();

	CCedtApp::SaveColorSettings();
	CCedtApp::SaveUserCommands(CCedtApp::m_szInstallDirectory + "\\cedt.cmd");
	CCedtApp::SaveMacroBuffers(CCedtApp::m_szInstallDirectory + "\\cedt.mac");

	return TRUE;
}

HTREEITEM CPreferenceDialog::InsertCategoryItem(UINT nResourceID, HTREEITEM hParent, UINT nCategory)
{
	CString szText; szText.LoadString( nResourceID );
	HTREEITEM hItem = m_treCategories.InsertItem( szText, hParent );
	BOOL bDataSet = m_treCategories.SetItemData( hItem, nCategory );
	m_treCategories.Expand( hParent, TVE_EXPAND );
	return hItem;
}


/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog message handlers
BOOL CPreferenceDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitAllPrefPages();
	SizeAllPrefPages();
	LoadAllPrefSettings();

	switch( m_nActiveCategory ) {
	case PREF_CATEGORY_GENERAL:		m_treCategories.SelectItem(m_hItemGeneral);		break;
	case PREF_CATEGORY_VISUAL:		m_treCategories.SelectItem(m_hItemVisual);		break;
	case PREF_CATEGORY_FONTS:		m_treCategories.SelectItem(m_hItemFonts);		break;
	case PREF_CATEGORY_COLORS:		m_treCategories.SelectItem(m_hItemColors);		break;
	case PREF_CATEGORY_PRINT:		m_treCategories.SelectItem(m_hItemPrint);		break;
	case PREF_CATEGORY_FILE:		m_treCategories.SelectItem(m_hItemFile);		break;
	case PREF_CATEGORY_ASSOC:		m_treCategories.SelectItem(m_hItemAssoc);		break;
	case PREF_CATEGORY_BACKUP:		m_treCategories.SelectItem(m_hItemBackup);		break;
	case PREF_CATEGORY_SYNTAX:		m_treCategories.SelectItem(m_hItemSyntax);		break;
	case PREF_CATEGORY_FILTERS:		m_treCategories.SelectItem(m_hItemFilters);		break;
	case PREF_CATEGORY_TOOLS:		m_treCategories.SelectItem(m_hItemTools);		break;
	case PREF_CATEGORY_COMMANDS:	m_treCategories.SelectItem(m_hItemCommands);	break;
	case PREF_CATEGORY_MACROS:		m_treCategories.SelectItem(m_hItemMacros);		break;
	}

	CenterWindow();
	// ensure preference window appear center of the editor

	return TRUE;
}

void CPreferenceDialog::OnApply() 
{
	ApplyAllPrefSettings();
}

void CPreferenceDialog::OnOK() 
{
	ApplyAllPrefSettings();
	CDialog::OnOK();
}

void CPreferenceDialog::OnSelchangedCategories(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW * pNMTreeView = (NM_TREEVIEW *)pNMHDR;
	m_nActiveCategory = pNMTreeView->itemNew.lParam;

	if( m_nActiveCategory == PREF_CATEGORY_ASSOC ) LoadAssocSettings();
	ShowProperPrefPages();

	*pResult = 0;
}

BOOL CPreferenceDialog::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_SYSKEYUP || pMsg->message == WM_KEYUP ) {
		CWnd * pWnd = GetFocus();
		if( pWnd == (CWnd *)(& m_hkyCommandHotKey) ) OnChangeCommandHotKey();
		else if( pWnd == (CWnd *)(& m_hkyMacroHotKey) ) OnChangeMacroHotKey();
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CPreferenceDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	static CBrush brush;

	if( pWnd == & m_btnBackgroundColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] ); 
		pDC->SetTextColor( m_crTextColor[WT_WORD] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnActiveLineColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x02] );
		pDC->SetTextColor( m_crTextColor[WT_WORD] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnWordColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_WORD] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnConstantColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_CONSTANT] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnCommentColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_LINECOMMENT] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnStringColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_QUOTATION1] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnDelimiterColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_DELIMITER] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnVariableColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_VARIABLE] ); pDC->SetBkMode( TRANSPARENT );
		return brush;

	} else if( pWnd == & m_btnKeyword0Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD0] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword1Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD1] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword2Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD2] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword3Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD3] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword4Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD4] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword5Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD5] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword6Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD6] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword7Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD7] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword8Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD8] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnKeyword9Color ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_KEYWORD9] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnLeftMarginColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x01] );
		pDC->SetTextColor( m_crTextColor[WT_LINEFEED] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnLineNumberColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x01] );
		pDC->SetTextColor( m_crTextColor[WT_LINEFEED] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnRange1BkgrColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x03] );
		pDC->SetTextColor( m_crTextColor[WT_WORD] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnRange2BkgrColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x04] );
		pDC->SetTextColor( m_crTextColor[WT_WORD] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnHighlightedColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_HIGHLIGHTON] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else if( pWnd == & m_btnShadowedColor ) {
		brush.DeleteObject(); brush.CreateSolidBrush( m_crBkgrColor[0x00] );
		pDC->SetTextColor( m_crTextColor[WT_SHADOWON] ); pDC->SetBkMode( TRANSPARENT );
		return brush;
	} else return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
