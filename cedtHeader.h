// Crimson Editor Main Header File

#ifndef __CEDT_HEADER_H_
#define __CEDT_HEADER_H_


#define STRING_COMPANYNAME					"Crimson System"
#define STRING_PROJECTFILEVER				"Crimson Editor 3.60"
#define STRING_CONFIGURATIONVER				"Configuration 3.70 Beta"
#define STRING_COLORSETTINGSVER				"Color Settings 3.45 Release"
#define STRING_FTPACCOUNTVER				"FTP Account 3.70 Beta"
#define STRING_USERTOOLSVER					"User Command 3.40"
#define STRING_USERMACROVER					"User Macro 3.50 Beta"

#define STRING_HOMEPAGEURL					"http://www.crimsoneditor.com/"
#define STRING_SPONSORURL					"http://www.crimsoneditor.com/english/sponsor.html"
#define STRING_EMAILADDRESS					"mailto:crimsonware@yahoo.com"

#define REGPATH_INSTALL_DIRECTORY			"Software\\Crimson System\\Crimson Editor"
#define REGPATH_USEININTERNETEXPLORER		"Software\\Microsoft\\Internet Explorer\\View Source Editor\\Editor Name"
#define REGPATH_SHELLEXTENSIONAPPROVED		"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"
#define REGPATH_ADDTORIGHTMOUSEBUTTON		"*\\shellex\\ContextMenuHandlers\\Crimson Editor"

#define CLSID_SHELLEXT_CRIMSONEDITOR		"{475A9681-F01B-11d5-BC5E-0050CE184C9B}"

#define PROGID_SHELLEXT_CRIMSONEDITOR		"CrimsonEditor.ShellExt"
#define PROGID_FILEASSOC_CRIMSONEDITOR		"CrimsonEditor"

#define MUTEX_NAME_CMDLINE					"CrimsonEditor.CmdLine"

#define REGKEY_ALLOW_MULTI					"Allow Multi-Instances"
#define REGKEY_BROWSING_DIR					"Browsing Directory"
#define REGKEY_WORKING_DIR					"Working Directory"
#define REGKEY_LAST_WORKSPACE				"Last Workspace File"
#define REGKEY_WINDOW_PLACEMENT				"Window Placement"
#define REGKEY_BAR_STATE					"Bar State"


// USER MESSSAGE
#define WM_ANOTHER_INSTANCE					(WM_USER+1)
#define WM_SIZE_MAIN_WINDOW					(WM_USER+2)


// MAXIMUM CONSTANTS
#define MAX_BUFFER_SIZE						4096
#define MAX_STRING_SIZE						32767

#define MAX_WORD_LENGTH						255
#define MAX_WORDS_COUNT						32767

#define MAX_FTP_ACCOUNT						16
#define MAX_FILE_FILTER						32
#define MAX_SYNTAX_TYPE						32

// MACROS
#define _IS_BET(min, x, max)				((x) >= (min) && (x) <= (max))
#define _MY_MAX(x, y)						((x) > (y) ? (x) : (y))
#define _MY_MIN(x, y)						((x) < (y) ? (x) : (y))

#define ASSURE_BOUND_VALUE(x, min, max)	{ \
	x = (x < (min)) ? (min) : x; \
	x = (x > (max)) ? (max) : x; \
}


// HEADERS
#include "resource.h"
#include "PathName.h"
#include "RegExp.h"
#include "Utility.h"

#include "StatusBarEx.h"
#include "SplitterWndEx.h"
#include "MDIFileTab.h"
#include "FileWindow.h"
#include "OutputWindow.h"

#include "cedtElement.h"
#include "cedtApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "cedtDoc.h"
#include "cedtView.h"


#endif // __CEDT_HEADER_H_