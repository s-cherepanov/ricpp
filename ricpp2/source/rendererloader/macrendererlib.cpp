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

/** @file macrendererlib.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The Win32 specific functions of the representation
 *         of a dynamic library for the factory of renderer context creators
 */

#include "ricpp/rendererloader/rendererloader.h"
#include "ricpp/gendynlib/macdynlib.h"

using namespace RiCPP;

extern "C" {
typedef CContextCreator *(*TypeNewContextCreatorFunc)(unsigned long majorversion);
typedef void (*TypeDeleteContextCreatorFunc)(CContextCreator *);
typedef unsigned long (*TypeMajorInterfaceVerFunc)();
typedef unsigned long (*TypeMinorInterfaceVerFunc)();
typedef unsigned long (*TypeInterfaceRevisionFunc)();
typedef const char *(*TypeRendererTypeFunc)();
typedef const char *(*TypeRendererNameFunc)();
}


CContextCreator *CRendererLoader::CRendererLib::newContextCreator(unsigned long majorversion) {
	CContextCreator *ri = ((TypeNewContextCreatorFunc)((CMacLibFunc *)m_newContextCreator)->funcPtr())(majorversion);
	return ri;
}
void CRendererLoader::CRendererLib::deleteContextCreator(CContextCreator *cc) {
	((TypeDeleteContextCreatorFunc)((CMacLibFunc *)m_deleteContextCreator)->funcPtr())(cc);
}

unsigned long CRendererLoader::CRendererLib::majorInterfaceVer() {
	return ((TypeMajorInterfaceVerFunc)((CMacLibFunc *)m_majorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::minorInterfaceVer() {
	return ((TypeMinorInterfaceVerFunc)((CMacLibFunc *)m_minorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::interfaceRevision() {
	return ((TypeInterfaceRevisionFunc)((CMacLibFunc *)m_interfaceRevision)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererType() {
	return ((TypeRendererTypeFunc)((CMacLibFunc *)m_rendererType)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererName() {
	return ((TypeRendererNameFunc)((CMacLibFunc *)m_rendererName)->funcPtr())();
}
