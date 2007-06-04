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

/** @file win32passthrough.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The Win32 stub for the dynamic library of a CRibFilter (demo)
 */

// passthrough.cpp : Defines the entry point for the DLL application.
//

#include "ricpp/ribfilter/ribfilter.h"
#include "ricpp/tools/platform.h"

using namespace RiCPP;

#ifdef WIN32

#ifdef _MANAGED
#pragma managed(push, off)
#endif

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

/** @fn CRibFilter *newPlugin(unsigned long majorversion, const char *type)
 *  @brief Returns an instance of a new plugin
 *  @param majorversion Major version number of the returnd type
 *  @param type Type must match the type of the plugin
 *  @return Returns a plugin if \a majorversion has the right value
 */
EXPORT
CRibFilter * CDECL newPlugin(unsigned long majorversion, const char *type)
{
	if ( majorversion != CRibFilter::myMajorVersion() || !type || strcmp(type, CRibFilter::myType()) != 0 )
		return NULL;
	return new CRibFilter;
}

/** @fn void deletePlugin(CRibFilter *p)
 *  @brief Deletes a plugin created by newPlugin()
 *  @param p Plugin that is deletetd, must have been constructed by newPlugin()
 */
EXPORT
void CDECL deletePlugin(CRibFilter *p)
{
	if ( p )
		delete p;
}

/** @fn unsigned long majorVersion()
 *  @brief Returns the major version number of the plugin
 *  @return The major version of the plugin
 */
EXPORT
unsigned long CDECL majorVersion()
{
	return CRibFilter::myMajorVersion();
}

/** @fn unsigned long minorVersion()
 *  @brief Returns the minor version number of the plugin
 *  @return The minor version of the plugin
 */
EXPORT
unsigned long CDECL minorVersion()
{
	return CRibFilter::myMinorVersion();
}

/** @fn unsigned long revision()
 *  @brief Returns the revision of the plugin
 *  @return The revision number of the plugin
 */
EXPORT
unsigned long CDECL revision()
{
	return CRibFilter::myRevision();
}

/** @fn RtToken type()
 *  @brief Returns the type of the plugin
 *  @return The renderer type the plugin
 */
EXPORT
const char * CDECL type()
{
	return CRibFilter::myType();
}

/** @fn RtToken name()
 *  @brief Returns the name of the plugin
 *  @return The name of the plugin
 */
EXPORT
const char * CDECL name()
{
	return CRibFilter::myName();
}

}
