// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__43D3DE3B_BF6C_4E8B_85E3_FB1CB7FD6FF6__INCLUDED_)
#define AFX_STDAFX_H__43D3DE3B_BF6C_4E8B_85E3_FB1CB7FD6FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

// #ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
// #define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
// #endif						

// #ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
// #define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//b #endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#endif // _MSC_VER > 1000

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "tools/ricppplatform.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

// Warnings disable for STL
#pragma warning(disable:4786)
// disable throw() warning
#pragma warning(disable:4290)

#endif // !defined(AFX_STDAFX_H__43D3DE3B_BF6C_4E8B_85E3_FB1CB7FD6FF6__INCLUDED_)
