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
 *  @brief The Win32 stub for the dynamic library of a CRibWriter
 */

#include "ricpp/ribwriter/ribwriter.h"
#include "ricpp/tools/platform.h"

using namespace RiCPP;

#ifdef WIN32

#ifdef _MANAGED
#pragma managed(push, off)
#endif

/** @brief Defines the entry point for the DLL.
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif // WIN32

extern "C" {

/** @fn CContextCreator *newContextCreator(unsigned long majorversion)
 *  @brief Returns an instance of a new renderer
 *  @param majorversion Major version number of the returnd IRiContext
 *  @return Returns a context if \a majorversion has the right value
 */
EXPORT
CContextCreator * CDECL newContextCreator(unsigned long majorversion)
{
	if ( majorversion != IRiContext::riContextMajorVersion )
		return NULL;
	return new CRibWriterCreator;
}

/** @fn void deleteContextCreator(CContextCreator *cc)
 *  @brief Deletes a renderer created by newContextCreator()
 *  @param cc Context creator that is deletetd, must have been constructed by newContextCreator()
 */
EXPORT
void CDECL deleteContextCreator(CContextCreator *cc)
{
	if ( cc )
		delete cc;
}

/** @fn unsigned long majorInterfaceVer()
 *  @brief Returns the major version number of the interface (changes if the IRiContext interface changes)
 *  @return The major version of IRiContext
 */
EXPORT
unsigned long CDECL majorInterfaceVer()
{
	return IRiContext::riContextMajorVersion;
}

/** @fn unsigned long minorInterfaceVer()
 *  @brief Returns the minor version number of the interface (since IRiContext is pure virtual, the concrete renderer has the minor version)
 *  @return The minor version of CRibWriter
 */
EXPORT
unsigned long CDECL minorInterfaceVer()
{
	return CRibWriter::ribWriterMinorVersion;
}

/** @fn unsigned long interfaceRevision()
 *  @brief Returns the revision of the interface (since IRiContext is pure virtual, the concrete renderer has the revision number)
 *  @return The revision number of CRibWriter
 */
EXPORT
unsigned long CDECL interfaceRevision()
{
	return CRibWriter::ribWriterRevision;
}

/** @fn RtToken rendererType()
 *  @brief Returns the type of the renderer
 *  @return The renderer type of CRibWriter
 */
EXPORT
RtToken CDECL rendererType()
{
	return CRibWriter::myRendererType();
}

/** @fn RtToken rendererName()
 *  @brief Returns the name of the renderer
 *  @return The name of CRibWriter
 */
EXPORT
RtToken CDECL rendererName()
{
	return CRibWriter::myRendererName();
}

}
