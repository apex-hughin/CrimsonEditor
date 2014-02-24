// FindInFilesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FindInFilesDialog.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindInFilesDialog dialog


CFindInFilesDialog::CFindInFilesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindInFilesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindInFilesDialog)
	m_bRegularExpression = FALSE;
	m_bWholeWord = FALSE;
	m_bMatchCase = FALSE;
	m_bLookSubfolders = FALSE;
	//}}AFX_DATA_INIT

	m_bLogFindSelection = FALSE;
	m_nFindSelBeg = m_nFindSelEnd = 0;
}


void CFindInFilesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindInFilesDialog)
	DDX_Control(pDX, IDC_FIND, m_btnFind);
	DDX_Control(pDX, IDC_FIND_STRING_MENU, m_btnFindString);
	DDX_Control(pDX, IDC_FILE_TYPE, m_cmbFileType);
	DDX_Control(pDX, IDC_FOLDER, m_cmbFolder);
	DDX_Control(pDX, IDC_FIND_STRING, m_cmbFindString);
	DDX_Check(pDX, IDC_REGULAR_EXPRESSION, m_bRegularExpression);
	DDX_Check(pDX, IDC_WHOLE_WORD, m_bWholeWord);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_LOOK_SUBFOLDERS, m_bLookSubfolders);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindInFilesDialog, CDialog)
	//{{AFX_MSG_MAP(CFindInFilesDialog)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_CBN_SETFOCUS(IDC_FIND_STRING, OnSetfocusFindString)
	ON_CBN_KILLFOCUS(IDC_FIND_STRING, OnKillfocusFindString)
	ON_BN_CLICKED(IDC_FIND_STRING_MENU, OnFindStringMenu)
	ON_COMMAND(ID_FIND_TEXT_TAB_CHAR, OnFindTextTabChar)
	ON_COMMAND(ID_FIND_TEXT_ANY_CHAR, OnFindTextAnyChar)
	ON_COMMAND(ID_FIND_TEXT_BEGIN_OF_LINE, OnFindTextBeginOfLine)
	ON_COMMAND(ID_FIND_TEXT_END_OF_LINE, OnFindTextEndOfLine)
	ON_COMMAND(ID_FIND_TEXT_ZERO_OR_MORE, OnFindTextZeroOrMore)
	ON_COMMAND(ID_FIND_TEXT_ONE_OR_MORE, OnFindTextOneOrMore)
	ON_COMMAND(ID_FIND_TEXT_ZERO_OR_ONE, OnFindTextZeroOrOne)
	ON_COMMAND(ID_FIND_TEXT_OR, OnFindTextOr)
	ON_COMMAND(ID_FIND_TEXT_IN_RANGE, OnFindTextInRange)
	ON_COMMAND(ID_FIND_TEXT_NOT_IN_RANGE, OnFindTextNotInRange)
	ON_COMMAND(ID_FIND_TEXT_ALPHA_CHAR, OnFindTextAlphaChar)
	ON_COMMAND(ID_FIND_TEXT_ALNUM_CHAR, OnFindTextAlnumChar)
	ON_COMMAND(ID_FIND_TEXT_DEC_DIGIT, OnFindTextDecDigit)
	ON_COMMAND(ID_FIND_TEXT_HEX_DIGIT, OnFindTextHexDigit)
	ON_COMMAND(ID_FIND_TEXT_TAGGED_EXP, OnFindTextTaggedExp)
	ON_CBN_EDITCHANGE(IDC_FIND_STRING, OnEditchangeFindString)
	ON_COMMAND(ID_FIND_TEXT_WHITE_SPACE, OnFindTextWhiteSpace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindInFilesDialog message handlers

BOOL CFindInFilesDialog::OnInitDialog() 
{
	static BOOL bImageLoaded = FALSE;

	CDialog::OnInitDialog();

	if( ! bImageLoaded ) {
		m_lstButtonImage.Create(IDB_GENERAL_BUTTONS, 9, 0, RGB(255, 0, 255));
		bImageLoaded = TRUE;
	}

	m_btnFindString.SetIcon( m_lstButtonImage.ExtractIcon(2) );

	TCHAR szCurrentDirectory[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

	
	m_cmbFindString.ResetContent();
	POSITION pos1 = m_lstFindString.GetHeadPosition();
	while( pos1 ) m_cmbFindString.AddString( m_lstFindString.GetNext(pos1) );

	if( ! m_szFindString.GetLength() && m_lstFindString.GetCount() ) m_szFindString = m_lstFindString.GetHead();
	m_cmbFindString.SetWindowText( m_szFindString );

	m_btnFind.EnableWindow( m_szFindString.GetLength() );

	m_cmbFileType.ResetContent();
	POSITION pos2 = m_lstFileType.GetHeadPosition();
	while( pos2 ) m_cmbFileType.AddString( m_lstFileType.GetNext(pos2) );

	if( ! m_szFileType.GetLength() ) m_szFileType = "*.*";
	m_cmbFileType.SetWindowText( m_szFileType );

	m_cmbFolder.ResetContent();
	POSITION pos3 = m_lstFolder.GetHeadPosition();
	while( pos3 ) m_cmbFolder.AddString( m_lstFolder.GetNext(pos3) );

	if( ! m_szFolder.GetLength() ) m_szFolder = szCurrentDirectory;
	m_cmbFolder.SetWindowText( m_szFolder );

	return TRUE;
}

void CFindInFilesDialog::OnFind() 
{
	m_cmbFindString.GetWindowText( m_szFindString );
	m_cmbFileType.GetWindowText( m_szFileType );
	m_cmbFolder.GetWindowText( m_szFolder );

	if( ! m_szFindString.GetLength() ) return;
	if( ! m_szFileType.GetLength() ) return;
	if( ! m_szFolder.GetLength() ) return;

	POSITION pos1 = m_lstFindString.Find( m_szFindString );
	if( pos1 ) m_lstFindString.RemoveAt( pos1 );
	while( m_lstFindString.GetCount() >= 30 ) m_lstFindString.RemoveTail();
	m_lstFindString.AddHead( m_szFindString );

	POSITION pos2 = m_lstFileType.Find( m_szFileType );
	if( pos2 ) m_lstFileType.RemoveAt( pos2 );
	if( m_lstFileType.GetCount() >= 30 ) m_lstFileType.RemoveTail();
	m_lstFileType.AddHead( m_szFileType );

	POSITION pos3 = m_lstFolder.Find( m_szFolder );
	if( pos3 ) m_lstFolder.RemoveAt( pos3 );
	if( m_lstFolder.GetCount() >= 30 ) m_lstFolder.RemoveTail();
	m_lstFolder.AddHead( m_szFolder );

	CDialog::OnOK();
}

void CFindInFilesDialog::OnSetfocusFindString() 
{
	m_bLogFindSelection = TRUE;
}

void CFindInFilesDialog::OnKillfocusFindString() 
{
	m_bLogFindSelection = FALSE;
}

BOOL CFindInFilesDialog::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYUP || pMsg->message == WM_LBUTTONUP ) {
		if( m_bLogFindSelection ) {
			DWORD dwSelect = m_cmbFindString.GetEditSel();
			m_nFindSelBeg = LOWORD(dwSelect);
			m_nFindSelEnd = HIWORD(dwSelect);
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CFindInFilesDialog::OnFindStringMenu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_SEARCH_DIALOG);
	pMenu = context.GetSubMenu(0);

	CRect rect; m_btnFindString.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CFindInFilesDialog::OnBrowse() 
{
	CString szDirectory; m_cmbFolder.GetWindowText( szDirectory );
	CString szText( LPCTSTR(IDS_CHOOSE_DIR_TO_SEARCH) );
	CFolderDialog dlg(szText, szDirectory, NULL, this);
	if( dlg.DoModal() != IDOK ) return;
	m_cmbFolder.SetWindowText( dlg.GetPathName() );
}

void CFindInFilesDialog::OnEditchangeFindString() 
{
	CString szFindString; m_cmbFindString.GetWindowText( szFindString );
	m_btnFind.EnableWindow( szFindString.GetLength() );
}

/////////////////////////////////////////////////////////////////////////////
// Context Menu Handlers

void CFindInFilesDialog::ReplaceFindSelection(LPCTSTR lpszString, INT nIncrement)
{
	CString szText; m_cmbFindString.GetWindowText( szText );
	szText = szText.Left( m_nFindSelBeg ) + CString(lpszString) + szText.Mid( m_nFindSelEnd );
	m_cmbFindString.SetWindowText( szText ); m_cmbFindString.SetFocus();
	m_cmbFindString.SetEditSel( m_nFindSelBeg + nIncrement, m_nFindSelBeg + nIncrement );

	((CButton *)GetDlgItem(IDC_REGULAR_EXPRESSION))->SetCheck(1);
	m_btnFind.EnableWindow( TRUE );
}

void CFindInFilesDialog::OnFindTextTabChar() 
{
	ReplaceFindSelection( "\\t", 2 );
}

void CFindInFilesDialog::OnFindTextAnyChar() 
{
	ReplaceFindSelection( ".", 1 );
}

void CFindInFilesDialog::OnFindTextBeginOfLine() 
{
	ReplaceFindSelection( "^", 1 );
}

void CFindInFilesDialog::OnFindTextEndOfLine() 
{
	ReplaceFindSelection( "$", 1 );
}

void CFindInFilesDialog::OnFindTextZeroOrMore() 
{
	ReplaceFindSelection( "*", 1 );
}

void CFindInFilesDialog::OnFindTextOneOrMore() 
{
	ReplaceFindSelection( "+", 1 );
}

void CFindInFilesDialog::OnFindTextZeroOrOne() 
{
	ReplaceFindSelection( "?", 1 );
}

void CFindInFilesDialog::OnFindTextOr() 
{
	ReplaceFindSelection( "|", 1 );
}

void CFindInFilesDialog::OnFindTextInRange() 
{
	ReplaceFindSelection( "[]", 1 );
}

void CFindInFilesDialog::OnFindTextNotInRange() 
{
	ReplaceFindSelection( "[^]", 2 );
}

void CFindInFilesDialog::OnFindTextWhiteSpace() 
{
	ReplaceFindSelection( "\\s", 2 );
}

void CFindInFilesDialog::OnFindTextAlnumChar() 
{
	ReplaceFindSelection( "\\w", 2 );
}

void CFindInFilesDialog::OnFindTextAlphaChar() 
{
	ReplaceFindSelection( "\\a", 2 );
}

void CFindInFilesDialog::OnFindTextDecDigit() 
{
	ReplaceFindSelection( "\\d", 2 );
}

void CFindInFilesDialog::OnFindTextHexDigit() 
{
	ReplaceFindSelection( "\\h", 2 );
}

void CFindInFilesDialog::OnFindTextTaggedExp() 
{
	ReplaceFindSelection( "()", 1 );
}
