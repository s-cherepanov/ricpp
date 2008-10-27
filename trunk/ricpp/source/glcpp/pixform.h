#ifndef _RICPP_GLCPP_PIXFORM_H
#define _RICPP_GLCPP_PIXFORM_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! \file pixform.h
 *  \brief Container for a pixel format descriptor
 *  \author Andreas Pidde (andreas@pidde.de)
 */


#include "rendererloader/sysexcept.h"

#ifdef LINUX
#  include <X11/Xlib.h>
#  include <GL/glx.h>
#  define BOOL Bool
#  define HDC Display *
#  define HWND Window
#  define HBITMAP void *
#  define HGLRC GLXContext
#  define PIXELFORMATDESCRIPTOR int *
#  define DWORD long
#  define LONG long

typedef struct tagSize {
    LONG cx;
    LONG cy;
} SIZE;
#define LPSIZE SIZE *

typedef struct tagRect {
    LONG left;
    LONG right;
    LONG top;
    LONG bottom;
} RECT;
#define LPRECT RECT *

#  define PROC void *
#  define FLOAT float
#  define LPCSTR const char *
#  define LPGLYPHMETRICSFLOAT void *
#  define __stdcall
inline void GetClientRect(HWND hwns, LPRECT r) {}
#define HORZSIZE  4
#define VERTSIZE  6
#define HORZRES   8
#define VERTRES  10
inline int GetDeviceCaps(HDC hdc, int caps) { return 0; }
#endif // LINUX

class TPixelFormat {

public:
    PIXELFORMATDESCRIPTOR m_pfd;

    TPixelFormat(BOOL useDoubleBuffer=TRUE);
    TPixelFormat(const PIXELFORMATDESCRIPTOR &);
    TPixelFormat(HDC, int iPixelFormat);
    virtual ~TPixelFormat() {}

    virtual void getPixelFormat(PIXELFORMATDESCRIPTOR &) const;
    virtual void putPixelFormat(const PIXELFORMATDESCRIPTOR &);

    virtual int  choosePixelFormat(HDC);
    virtual void setPixelFormat(HDC, int iPixelFormat);
    virtual int  enterPixelFormat(HDC); // Choose + Set
    virtual int  describePixelFormat(HDC, int iPixelFormat);

    virtual void setDoubleBuffer(BOOL useDoubleBuffer=TRUE);
    virtual BOOL isDoubleBuffer() const;
};

#endif // _RICPP_GLCPP_PIXFORM_H
