// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
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

/** @file win32rendererlib.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The Win32 specific functions of the CRendererLoader::CRendererLib
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */

#include "ricpp/rendererloader/rendererloader.h"
#include "ricpp/gendynlib/win32dynlib.h"

using namespace RiCPP;

extern "C" {
typedef CContextCreator *(CDECL *TNewContextCreatorFunc)(unsigned long);
typedef void (CDECL *TDeleteContextCreatorFunc)(CContextCreator *);
typedef unsigned long (CDECL *TMajorInterfaceVerFunc)();
typedef unsigned long (CDECL *TMinorInterfaceVerFunc)();
typedef unsigned long (CDECL *TInterfaceRevisionFunc)();
typedef const char *(CDECL *TRendererTypeFunc)();
typedef const char *(CDECL *TRendererNameFunc)();
}

bool CRendererLoader::CRendererLib::validDLL() {
	if ( !m_lib )
		return false;
	if ( strcmp(m_lib->className(), CWin32DynLib::staticClassName()) != 0 )
		return false;
	return true;
}

CContextCreator *CRendererLoader::CRendererLib::newContextCreator(unsigned long majorversion) {
	CContextCreator *cc = ((TNewContextCreatorFunc)((CWin32LibFunc *)m_newContextCreator)->funcPtr())(majorversion);
	return cc;
}
void CRendererLoader::CRendererLib::deleteContextCreator(CContextCreator *cc) {
	((TDeleteContextCreatorFunc)((CWin32LibFunc *)m_deleteContextCreator)->funcPtr())(cc);
}

unsigned long CRendererLoader::CRendererLib::majorInterfaceVer() {
	return ((TMajorInterfaceVerFunc)((CWin32LibFunc *)m_majorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::minorInterfaceVer() {
	return ((TMinorInterfaceVerFunc)((CWin32LibFunc *)m_minorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::interfaceRevision() {
	return ((TInterfaceRevisionFunc)((CWin32LibFunc *)m_interfaceRevision)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererType() {
	return ((TRendererTypeFunc)((CWin32LibFunc *)m_rendererType)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererName() {
	return ((TRendererNameFunc)((CWin32LibFunc *)m_rendererName)->funcPtr())();
}
