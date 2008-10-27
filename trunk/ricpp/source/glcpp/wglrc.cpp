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
    m_hdcMem = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
    m_size.cx = -1;
    m_size.cy = -1;
}

TWGLRenderContext::~TWGLRenderContext() {
		try {
			deleteMemory();
		} catch (...) {
		}
		m_hglrc = NULL;
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

void TWGLRenderContext::createRC(HDC ahdc)
{
    if ( ahdc != m_hdcMem )
        deleteMemory();

    // get the device context's best-available-match pixel format and
    // make that the device context's current pixel format
    m_iPixelFormat = m_pfd.enterPixelFormat(ahdc);

    m_hglrc = ::wglCreateContext(ahdc);

    if ( !m_hglrc )
        throw TSystemException(__FILE__, __LINE__);
}

void TWGLRenderContext::deleteRC()
{
	HGLRC cc = NULL;
	DWORD d = 0;
	BOOL flag = TRUE;
	if ( m_hglrc != NULL ) {

		::wglMakeCurrent(NULL,NULL);

		flag = ::wglDeleteContext(m_hglrc);
	}
	if ( !flag ) {
		d = GetLastError();
		cc = wglGetCurrentContext();
	}
	m_hglrc = NULL;
}

HDC TWGLRenderContext::createMemory(HDC ahdc, long w, long h)
{
    if ( m_hdcMem && m_size.cx == w && m_size.cy == h )
		return m_hdcMem;
    
    deleteMemory();
    m_pfd.setDoubleBuffer(FALSE); // DoubleBuffer makes no sense in memory contexts

	m_pfd.m_pfd.iPixelType |= PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI | PFD_GENERIC_FORMAT;
	m_pfd.m_pfd.iPixelType &= ~PFD_DRAW_TO_WINDOW;

    m_hdcMem = ::CreateCompatibleDC(ahdc);
    if ( !m_hdcMem )
		throw TSystemException(__FILE__, __LINE__);

    m_hBitmap = ::CreateCompatibleBitmap(ahdc, w, h);
    if ( !m_hBitmap ) {
		long err = ::GetLastError();
		::DeleteDC(m_hdcMem);
		m_hdcMem = NULL;
		throw TSystemException(err, __FILE__, __LINE__);
	}

    m_hOldBitmap = (HBITMAP)::SelectObject(m_hdcMem, m_hBitmap);
    ::SetMapMode(m_hdcMem, ::GetMapMode(ahdc));

    m_size.cx = w;
    m_size.cy = h;

    createRC(m_hdcMem);
    return m_hdcMem;
}

void TWGLRenderContext::deleteMemory()
{
    deleteRC();

    if ( m_hBitmap ) {
        if ( m_hdcMem )
			::SelectObject(m_hdcMem, m_hOldBitmap);
        ::DeleteObject(m_hBitmap);
    }

    m_hBitmap = NULL;
    if ( m_hdcMem )
		::DeleteDC(m_hdcMem);
    m_hdcMem = NULL;

    m_pfd.setDoubleBuffer(TRUE);
	m_pfd.m_pfd.iPixelType &= ~(PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI | PFD_GENERIC_FORMAT);
	m_pfd.m_pfd.iPixelType |= PFD_DRAW_TO_WINDOW;
}

HDC TWGLRenderContext::getMemory() { return m_hdcMem; }
HBITMAP TWGLRenderContext::getBitmap() { return m_hBitmap; }

void TWGLRenderContext::makeCurrentRC(HDC ahdc) {
    if ( !ahdc ) return;
    m_iPixelFormat = m_pfd.enterPixelFormat(ahdc);
    if ( !::wglMakeCurrent(ahdc, m_hglrc) )
        throw TSystemException(__FILE__, __LINE__);
    m_hdc = ahdc;
}

/*
BOOL TWGLRenderContext::releaseThisRC() {
    if ( m_hglrc == NULL )
		return FALSE;
    
    HGLRC chglrc = getCurrentRC();
    if ( chglrc == m_hglrc ) {
        if ( !::wglMakeCurrent(NULL, NULL) )
            throw TSystemException(__FILE__, __LINE__);
        m_hdc = NULL;
        return TRUE;
    }

    return FALSE;
}
*/

void TWGLRenderContext::releaseCurrentRC() {
    // HGLRC chglrc = getCurrentRC();
    // BOOL selfCurrent = chglrc == m_hglrc;
	try {
		if ( !::wglMakeCurrent(NULL, NULL) ) {
			m_hdc = NULL;
			throw TSystemException(__FILE__, __LINE__);
		}
	} catch ( ... ) {
		m_hdc = NULL;
		throw TSystemException(__FILE__, __LINE__);
	}
	m_hdc = NULL;
}

HGLRC TWGLRenderContext::getCurrentRC() const {
	return ::wglGetCurrentContext();
}

HDC TWGLRenderContext::getCurrentDC() const {
    return ::wglGetCurrentDC();
}

void TWGLRenderContext::swapBuffers(HDC ahdc) const {
    if ( !m_hdcMem ) {
        if ( !::SwapBuffers(ahdc) )
            throw TSystemException(__FILE__, __LINE__);
    } else {

        if ( m_pfd.isDoubleBuffer() ) {
            // ::SwapBuffers calls ::glFlush() implicitly
            if ( !::SwapBuffers(m_hdcMem) )
                throw TSystemException(__FILE__, __LINE__);
        } {
			TGL::flush();
        }
        
        BITMAP bm;
        POINT ptOrg, ptSize, ptStart;

        ::GetObject(m_hBitmap, sizeof(BITMAP), (LPVOID) &bm);
        ptSize.x = bm.bmWidth;
        ptSize.y = bm.bmHeight;
        ::DPtoLP(ahdc, &ptSize, 1);
        ptStart.x = 0;
        ptStart.y = 0;
        ::DPtoLP(ahdc, &ptStart, 1);
        ptOrg.x = 0;
        ptOrg.y = 0;
        ::DPtoLP(m_hdcMem, &ptSize, 1);
        ::BitBlt(ahdc, ptStart.x, ptStart.y, ptSize.x, ptSize.y, m_hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);
    }
}

PROC TWGLRenderContext::getProcAddress(LPCSTR lpszProc) const {
    PROC rval = ::wglGetProcAddress(lpszProc);
    if ( !rval )
        throw TSystemException(__FILE__, __LINE__);
    return rval;
}

void TWGLRenderContext::shareLists(HGLRC ahglrc) {
    if ( !::wglShareLists(m_hglrc, ahglrc) )
        throw TSystemException(__FILE__, __LINE__);
}

void TWGLRenderContext::shareLists(TWGLRenderContext aContext) {
    shareLists(aContext.getRC());
}

void TWGLRenderContext::useFontBitmaps(HDC ahdc, DWORD first, DWORD count, DWORD listBase) const {
    if ( !::wglUseFontBitmaps(ahdc, first, count, listBase) )
        throw TSystemException(__FILE__, __LINE__);
}

void TWGLRenderContext::useFontBitmaps(DWORD first, DWORD count, DWORD listBase) {
    useFontBitmaps(m_hdc, first, count, listBase);
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
    if ( !::wglUseFontOutlines(ahdc, first, count, listBase, deviation, extrusion, format, lpgmf) )
        throw TSystemException(__FILE__, __LINE__);
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
    useFontOutlines(m_hdc, first, count, listBase, deviation, extrusion, format, lpgmf);
}

