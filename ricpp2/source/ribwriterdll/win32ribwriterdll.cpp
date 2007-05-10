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

/** @file win32ribwriterdll.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The Win32 stub for a DLL.
 */

#include "ricpp/ribwriter/ribwriter.h"
#include <windows.h>

using namespace RiCPP;
#define EXPORT __declspec ( dllexport )

/**  @brief Defines the entry point for the DLL.
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extern "C" {

/**  @brief Returns an instance of a new renderer
 */
EXPORT
CContextCreator * CDECL newContextCreator(unsigned long majorversion) {
	if ( majorversion != IRiContext::riContextMajorVersion )
		return NULL;
	return new CRibWriterCreator;
}

/**  @brief Deletes a renderer created by newContextCreator()
 */
EXPORT
void CDECL deleteContextCreator(CContextCreator *cc) {
	if ( cc )
		delete cc;
}

/**  @brief Returns the major version number of the interface (changes if the IRiContext interface changes)
 */
EXPORT
unsigned long CDECL majorInterfaceVer() {
	return IRiContext::riContextMajorVersion;
}

/**  @brief Returns the minor version number of the interface (since IRiContext is pure virtual, the concrete renderer has the minor version)
 */
EXPORT
unsigned long CDECL minorInterfaceVer() {
	return CRibWriter::ribWriterMinorVersion;
}

/**  @brief Returns the revision of the interface (since IRiContext is pure virtual, the concrete renderer has the revision number)
 */
EXPORT
unsigned long CDECL interfaceRevision() {
	return CRibWriter::ribWriterRevision;
}

/**  @brief Returns the type of the renderer
 */
EXPORT
RtToken CDECL rendererType() {
	return CRibWriter::myRendererType();
}

/**  @brief Returns the name of the renderer
 */
EXPORT
RtToken CDECL rendererName() {
	return CRibWriter::myRendererName();
}

}

