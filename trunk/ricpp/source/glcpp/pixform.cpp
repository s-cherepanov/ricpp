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

/*! \file pixform.cpp
 *  \brief Container for a pixel format descriptor
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "pixform.h"

TPixelFormat::TPixelFormat(BOOL useDoubleBuffer)
{
    PIXELFORMATDESCRIPTOR tempPfd = {
        sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
        1,                     // version number
        PFD_DRAW_TO_WINDOW     // support window
		// | PFD_GENERIC_FORMAT   // supported by GDI generic implementation
        | (useDoubleBuffer ? PFD_DOUBLEBUFFER  : 0 )  // support doublebuffer
        | PFD_SUPPORT_OPENGL    // support OpenGL
        ,
		PFD_TYPE_RGBA,         // RGBA type
        32,                    // max. 32-bit color depth
        0, 0, 0, 0, 0, 0,      // color bits ignored
        0,                     // no alpha buffer
        0,                     // shift bit ignored
        0,                     // no accumulation buffer
        0, 0, 0, 0,            // accum bits ignored
        32,                    // 32-bit z-buffer
        32,                    // stencil buffer
        0,                     // no auxiliary buffer
        PFD_MAIN_PLANE,        // main layer (only one supported)
        0,                     // reserved
        0, 0, 0                // layer masks ignored
    };

    putPixelFormat(tempPfd);
}

TPixelFormat::TPixelFormat(const PIXELFORMATDESCRIPTOR &aPfd)
{
    putPixelFormat(aPfd);
}

TPixelFormat::TPixelFormat(HDC hdc, int iPixelFormat)
{
    describePixelFormat(hdc, iPixelFormat);
}

// TPixelFormat::~TPixelFormat() { }

int  TPixelFormat::choosePixelFormat(HDC hdc) {
	return ::ChoosePixelFormat(hdc, &m_pfd);
}

void TPixelFormat::setPixelFormat(HDC hdc, int iPixelFormat) {
    if ( !::SetPixelFormat(hdc, iPixelFormat, &m_pfd) ) {
		throw TSystemException(__FILE__, __LINE__);
	}
}

int  TPixelFormat::enterPixelFormat(HDC hdc) {
    int iPixelFormat = choosePixelFormat(hdc);
	describePixelFormat(hdc, iPixelFormat);
    setPixelFormat(hdc, iPixelFormat);
    return iPixelFormat;
}

int TPixelFormat::describePixelFormat(HDC hdc, int iPixelFormat) {
	return ::DescribePixelFormat(hdc, iPixelFormat, sizeof(m_pfd), &m_pfd);
}

void TPixelFormat::getPixelFormat(PIXELFORMATDESCRIPTOR &apfd) const { 
	apfd = m_pfd; 
}

void TPixelFormat::putPixelFormat(const PIXELFORMATDESCRIPTOR &apfd) {
	m_pfd = apfd;
}

void TPixelFormat::setDoubleBuffer(BOOL useDoubleBuffer) {
    if ( useDoubleBuffer ) m_pfd.dwFlags |= PFD_DOUBLEBUFFER;
    else  m_pfd.dwFlags &= ~PFD_DOUBLEBUFFER;
}

BOOL TPixelFormat::isDoubleBuffer() const {
    return (m_pfd.dwFlags & PFD_DOUBLEBUFFER) ? TRUE : FALSE;
}

