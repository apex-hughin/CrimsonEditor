// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FindDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDialog dialog


CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDialog)
	m_bMatchCase = FALSE;
	m_bRegularExpression = FALSE;
	m_bWholeWord = FALSE;
	m_nDirection = 1;
	//}}AFX_DATA_INIT

	m_bLogFindSelection = FALSE;
	m_nFindSelBeg = m_nFindSelEnd = 0;
}


void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDialog)
	DDX_Control(pDX, IDC_FIND, m_btnFind);
	DDX_Control(pDX, IDC_FIND_STRING_MENU, m_btnFindString);
	DDX_Control(pDX, IDC_FIND_STRING, m_cmbFindString);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_REGULAR_EXPRESSION, m_bRegularExpression);
	DDX_Check(pDX, IDC_WHOLE_WORD, m_bWholeWord);
	DDX_Radio(pDX, IDC_DIRECTION0, m_nDirection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	//{{AFX_MSG_MAP(CFindDialog)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_FIND_STRING_MENU, OnFindStringMenu)
	ON_COMMAND(ID_FIND_TEXT_ALNUM_CHAR, OnFindTextAlnumChar)
	ON_COMMAND(ID_FIND_TEXT_ALPHA_CHAR, OnFindTextAlphaChar)
	ON_COMMAND(ID_FIND_TEXT_ANY_CHAR, OnFindTextAnyChar)
	ON_COMMAND(ID_FIND_TEXT_BEGIN_OF_LINE, OnFindTextBeginOfLine)
	ON_COMMAND(ID_FIND_TEXT_DEC_DIGIT, OnFindTextDecDigit)
	ON_COMMAND(ID_FIND_TEXT_END_OF_LINE, OnFindTextEndOfLine)
	ON_COMMAND(ID_FIND_TEXT_HEX_DIGIT, OnFindTextHexDigit)
	ON_COMMAND(ID_FIND_TEXT_IN_RANGE, OnFindTextInRange)
	ON_COMMAND(ID_FIND_TEXT_NOT_IN_RANGE, OnFindTextNotInRange)
	ON_COMMAND(ID_FIND_TEXT_ONE_OR_MORE, OnFindTextOneOrMore)
	ON_COMMAND(ID_FIND_TEXT_OR, OnFindTextOr)
	ON_COMMAND(ID_FIND_TEXT_TAB_CHAR, OnFindTextTabChar)
	ON_COMMAND(ID_FIND_TEXT_TAGGED_EXP, OnFindTextTaggedExp)
	ON_COMMAND(ID_FIND_TEXT_ZERO_OR_MORE, OnFindTextZeroOrMore)
	ON_COMMAND(ID_FIND_TEXT_ZERO_OR_ONE, OnFindTextZeroOrOne)
	ON_CBN_SETFOCUS(IDC_FIND_STRING, OnSetfocusFindString)
	ON_CBN_KILLFOCUS(IDC_FIND_STRING, OnKillfocusFindString)
	ON_CBN_EDITCHANGE(IDC_FIND_STRING, OnEditchangeFindString)
	ON_COMMAND(ID_FIND_TEXT_WHITE_SPACE, OnFindTextWhiteSpace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDialog message handlers
BOOL CFindDialog::OnInitDialog() 
{
	static BOOL bImageLoaded = FALSE;

	CDialog::OnInitDialog();

	if( ! bImageLoaded ) {
		m_lstButtonImage.Create(IDB_GENERAL_BUTTONS, 9, 0, RGB(255, 0, 255));
		bImageLoaded = TRUE;
	}

	m_btnFindString.SetIcon( m_lstButtonImage.ExtractIcon(2) );
	m_cmbFindString.ResetContent();

	POSITION pos = m_lstFindString.GetHeadPosition();
	while( pos ) m_cmbFindString.AddString( m_lstFindString.GetNext(pos) );

	if( ! m_szFindString.GetLength() && m_lstFindString.GetCount() ) m_szFindString = m_lstFindString.GetHead();
	m_cmbFindString.SetWindowText( m_szFindString );

	m_btnFind.EnableWindow( m_szFindString.GetLength() );

	return TRUE;
}

void CFindDialog::OnFind() 
{
	m_cmbFindString.GetWindowText( m_szFindString );
	if( ! m_szFindString.GetLength() ) return;

	POSITION pos = m_lstFindString.Find( m_szFindString );
	if( pos ) m_lstFindString.RemoveAt( pos );
	while( m_lstFindString.GetCount() >= 30 ) m_lstFindString.RemoveTail();
	m_lstFindString.AddHead( m_szFindString );

	CDialog::OnOK();
}

void CFindDialog::OnSetfocusFindString() 
{
	m_bLogFindSelection = TRUE;
}

void CFindDialog::OnKillfocusFindString() 
{
	m_bLogFindSelection = FALSE;
}

BOOL CFindDialog::PreTranslateMessage(MSG* pMsg) 
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

void CFindDialog::OnFindStringMenu() 
{
	CMenu * pMenu, context; context.LoadMenu(IDR_SEARCH_DIALOG);
	pMenu = context.GetSubMenu(0);

	CRect rect; m_btnFindString.GetWindowRect( & rect );
	CPoint point(rect.right, rect.top);

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	pMenu->TrackPopupMenu(nFlags, point.x, point.y, this);
}

void CFindDialog::OnEditchangeFindString() 
{
	CString szFindString; m_cmbFindString.GetWindowText( szFindString );
	m_btnFind.EnableWindow( szFindString.GetLength() );
}

/////////////////////////////////////////////////////////////////////////////
// Context Menu Handlers

void CFindDialog::ReplaceFindSelection(LPCTSTR lpszString, INT nIncrement)
{
	CString szText; m_cmbFindString.GetWindowText( szText );
	szText = szText.Left( m_nFindSelBeg ) + CString(lpszString) + szText.Mid( m_nFindSelEnd );
	m_cmbFindString.SetWindowText( szText ); m_cmbFindString.SetFocus();
	m_cmbFindString.SetEditSel( m_nFindSelBeg + nIncrement, m_nFindSelBeg + nIncrement );

	((CButton *)GetDlgItem(IDC_REGULAR_EXPRESSION))->SetCheck(1);
	m_btnFind.EnableWindow( TRUE );
}

void CFindDialog::OnFindTextTabChar() 
{
	ReplaceFindSelection( "\\t", 2 );
}

void CFindDialog::OnFindTextAnyChar() 
{
	ReplaceFindSelection( ".", 1 );
}

void CFindDialog::OnFindTextBeginOfLine() 
{
	ReplaceFindSelection( "^", 1 );
}

void CFindDialog::OnFindTextEndOfLine() 
{
	ReplaceFindSelection( "$", 1 );
}

void CFindDialog::OnFindTextZeroOrMore() 
{
	ReplaceFindSelection( "*", 1 );
}

void CFindDialog::OnFindTextOneOrMore() 
{
	ReplaceFindSelection( "+", 1 );
}

void CFindDialog::OnFindTextZeroOrOne() 
{
	ReplaceFindSelection( "?", 1 );
}

void CFindDialog::OnFindTextOr() 
{
	ReplaceFindSelection( "|", 1 );
}

void CFindDialog::OnFindTextInRange() 
{
	ReplaceFindSelection( "[]", 1 );
}

void CFindDialog::OnFindTextNotInRange() 
{
	ReplaceFindSelection( "[^]", 2 );
}

void CFindDialog::OnFindTextWhiteSpace() 
{
	ReplaceFindSelection( "\\s", 2 );
}

void CFindDialog::OnFindTextAlnumChar() 
{
	ReplaceFindSelection( "\\w", 2 );
}

void CFindDialog::OnFindTextAlphaChar() 
{
	ReplaceFindSelection( "\\a", 2 );
}

void CFindDialog::OnFindTextDecDigit() 
{
	ReplaceFindSelection( "\\d", 2 );
}

void CFindDialog::OnFindTextHexDigit() 
{
	ReplaceFindSelection( "\\h", 2 );
}

void CFindDialog::OnFindTextTaggedExp() 
{
	ReplaceFindSelection( "()", 1 );
}
