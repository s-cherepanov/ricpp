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

/*! \file wglrc.cpp
 *  \brief Conainer for a windows OpenGL render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "wglrc.h"

TWGLRenderContext::TWGLRenderContext(BOOL useDoubleBuffer) : m_pfd(useDoubleBuffer) {
    m_iPixelFormat = 0;
    m_hglrc = NULL;
    m_hdc = NULL;
    m_hwnd = 0L;
    m_hdcMem = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
    m_size.cx = -1;
    m_size.cy = -1;
}

TWGLRenderContext::~TWGLRenderContext() {
}
    
HGLRC TWGLRenderContext::getRC() const {
    return m_hglrc;
}
    
HDC TWGLRenderContext::getActiveDC() const {
    return m_hdc;
}

int TWGLRenderContext::getPixelFormatIndex() const {
    return m_iPixelFormat;
}

void TWGLRenderContext::createRC(HDC ahdc, XVisualInfo *visinfo)
{
    m_hglrc = glXCreateContext( ahdc, visinfo, NULL, True );
}

void TWGLRenderContext::deleteRC()
{
}

HDC TWGLRenderContext::createMemory(HDC ahdc, long w, long h)
{
    return NULL;
}

void TWGLRenderContext::deleteMemory()
{
}

HDC TWGLRenderContext::getMemory() { return m_hdcMem; }
HBITMAP TWGLRenderContext::getBitmap() { return m_hBitmap; }

void TWGLRenderContext::makeCurrentRC(HDC ahdc, HWND win) {
    m_hdc = ahdc;
    m_hwnd = win;
    glXMakeCurrent(ahdc, win, m_hglrc);
}


void TWGLRenderContext::releaseCurrentRC() {
    // Crashes: Speicherzugriffsfehler
    // glXMakeCurrent(NULL, 0UL, m_hglrc);
}

HGLRC TWGLRenderContext::getCurrentRC() const {
    return NULL;
}

HDC TWGLRenderContext::getCurrentDC() const {
    return NULL;
}

void TWGLRenderContext::swapBuffers(HDC ahdc, HWND ahwnd) const {
    glXSwapBuffers( ahdc,  ahwnd);
}

PROC TWGLRenderContext::getProcAddress(LPCSTR lpszProc) const {
    return NULL;
}

void TWGLRenderContext::shareLists(HGLRC ahglrc) {
}

void TWGLRenderContext::shareLists(TWGLRenderContext aContext) {
}

void TWGLRenderContext::useFontBitmaps(HDC ahdc, DWORD first, DWORD count, DWORD listBase) const {
}

void TWGLRenderContext::useFontBitmaps(DWORD first, DWORD count, DWORD listBase) {
}

void TWGLRenderContext::useFontOutlines(
    HDC  ahdc,  //Device context of the outline font
    DWORD  first,   //First glyph to be turned into a display list
    DWORD  count,   //Number of glyphs to be turned into display lists
    DWORD  listBase,    //Specifies the starting display list
    FLOAT  deviation,   //Specifies the maximum chordal deviation from the true outlines
    FLOAT  extrusion,   //Extrusion value in the negative z direction
    int  format,    //Specifies line segments or polygons in display lists
    LPGLYPHMETRICSFLOAT  lpgmf  //Address of buffer to receive glyphs metric data
) const {
}

void TWGLRenderContext::useFontOutlines(
    DWORD  first,   //First glyph to be turned into a display list
    DWORD  count,   //Number of glyphs to be turned into display lists
    DWORD  listBase,    //Specifies the starting display list
    FLOAT  deviation,   //Specifies the maximum chordal deviation from the true outlines
    FLOAT  extrusion,   //Extrusion value in the negative z direction
    int  format,    //Specifies line segments or polygons in display lists
    LPGLYPHMETRICSFLOAT  lpgmf  //Address of buffer to receive glyphs metric data
) {
}
