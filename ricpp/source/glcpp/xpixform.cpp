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
    int snglBuf[] = {
        GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,
	GLX_DEPTH_SIZE, 12, None
    };

    int dblBuf[] = {
        GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,
	GLX_DEPTH_SIZE, 12, GLX_DOUBLEBUFFER, None
    };
    const PIXELFORMATDESCRIPTOR ps = &snglBuf[0];
    const PIXELFORMATDESCRIPTOR pd = &dblBuf[0];

    putPixelFormat(useDoubleBuffer ? ps : pd);
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
    return 0;
}

void TPixelFormat::setPixelFormat(HDC hdc, int iPixelFormat) {

}

int  TPixelFormat::enterPixelFormat(HDC hdc) {
    return 0;
}

int TPixelFormat::describePixelFormat(HDC hdc, int iPixelFormat) {
    return 0;
}

void TPixelFormat::getPixelFormat(PIXELFORMATDESCRIPTOR &apfd) const { 
//	apfd = m_pfd; 
}

void TPixelFormat::putPixelFormat(const PIXELFORMATDESCRIPTOR &apfd) {
//	m_pfd = apfd;
}

void TPixelFormat::setDoubleBuffer(BOOL useDoubleBuffer) {
//    if ( useDoubleBuffer ) m_pfd.dwFlags |= PFD_DOUBLEBUFFER;
//    else  m_pfd.dwFlags &= ~PFD_DOUBLEBUFFER;
}

BOOL TPixelFormat::isDoubleBuffer() const {
//    return (m_pfd.dwFlags & PFD_DOUBLEBUFFER) ? TRUE : FALSE;
    return 1;
}
