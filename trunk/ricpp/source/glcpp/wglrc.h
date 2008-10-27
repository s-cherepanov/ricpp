#ifndef _RICPP_GLCPP_WGLRC_H
#define _RICPP_GLCPP_WGLRC_H

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

/*! \file wglrc.h
 *  \brief Conainer for a windows OpenGL render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "opengl.h"
#include "pixform.h"

// Class to handle wgl functions (binds windows to OpenGL)
class TWGLRenderContext
{
protected:
    int m_iPixelFormat;
    HGLRC m_hglrc;
    HDC m_hdc;

    HDC m_hdcMem;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    SIZE m_size;
#ifdef LINUX
    HWND m_hwnd;
#endif // LINUX
public:
    TPixelFormat m_pfd;

    TWGLRenderContext(BOOL useDoubleBuffer=TRUE);
    virtual ~TWGLRenderContext();
    
    virtual HGLRC getRC() const;
    virtual HDC getActiveDC() const;

    virtual int getPixelFormatIndex() const;
#ifdef WIN32
    virtual void createRC(HDC);
#endif // WIN32
#ifdef LINUX
    virtual void createRC(HDC, XVisualInfo *);
#endif // LINUX
    virtual void deleteRC();

    virtual HDC createMemory(HDC, long w, long h);
    virtual void deleteMemory();
    virtual HDC getMemory();
    virtual HBITMAP getBitmap();

#ifdef WIN32
    virtual void makeCurrentRC(HDC);
#endif // WIN32
#ifdef LINUX
    virtual void makeCurrentRC(HDC, HWND);
#endif // LINUX
    // virtual BOOL  releaseThisRC();
    virtual void  releaseCurrentRC();
    virtual HGLRC getCurrentRC() const;
    virtual HDC   getCurrentDC() const;

#ifdef WIN32
    virtual void swapBuffers(HDC) const;
#endif // WIN32
#ifdef LINUX
    virtual void swapBuffers(HDC, HWND) const;
#endif // LINUX
    virtual PROC getProcAddress(LPCSTR) const;
    virtual void shareLists(HGLRC);
    virtual void shareLists(TWGLRenderContext);
    virtual void useFontBitmaps(DWORD first, DWORD count, DWORD listBase);
    virtual void useFontBitmaps(HDC ahdc, DWORD first, DWORD count, DWORD listBase) const;
    virtual void useFontOutlines(HDC ahdc, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf) const;
    virtual void useFontOutlines(DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
};

#endif // _RICPP_GLCPP_WGLRC_H
