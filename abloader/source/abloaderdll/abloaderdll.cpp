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

#include "abloader/abloader/abloader.h"

#pragma GCC visibility push(hidden)

#ifndef _ABLOADER_TOOLS_TRACE
#include "abloader/tools/trace.h"
#endif // _ABLOADER_TOOLS_TRACE

#pragma GCC visibility pop

using namespace RiCPP;

#if defined(WIN32)

#if defined(_MANAGED)
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

#if defined(_MANAGED)
#pragma managed(pop)
#endif

#endif // WIN32

extern "C" {
	
/** @fn CContextCreator *newPlugin(unsigned long majorversion, const char *type)
 *  @brief Returns an instance of a new plugin
 *  @param majorversion Major version number of the returned interface (base class CContextCreator)
 *  @param type Type must match the type of the plugins interface (base class CContextCreator)
 *  @return Returns a plugin if @a majorversion has the right value
 */
EXPORT
CContextCreator * CDECL newPlugin(unsigned long majorversion, const char *type)
{
	trace("-> newPlugin()");
	if ( majorversion != CContextCreator::myMajorVersion() || !type || strcmp(type, CContextCreator::myType()) != 0 )
	{
		trace("<- newPlugin(), NULL");
		return NULL;
	}
	CABLoaderCreator *ret = new CABLoaderCreator;
	trace(ret ? "<- newPlugin(), regular" : "<- newPlugin(), CABLoaderCreator not created");
	return ret;
}

/** @fn void deletePlugin(CContextCreator *p)
 *  @brief Deletes a plugin created by newPlugin()
 *  @param p Plugin that is deletetd, must have been constructed by newPlugin()
 */
EXPORT
void CDECL deletePlugin(CContextCreator *p)
{
	trace("-> deletePlugin()");
	if ( p )
		delete p;
	trace("<- deletePlugin()");
}

/** @fn unsigned long majorVersion()
 *  @brief Returns the major version number of the plugin
 *  @return The major version of the plugin
 */
EXPORT
unsigned long CDECL majorVersion()
{
	trace("<> majorVersion()");
	return CABLoaderCreator::myMajorVersion();
}

/** @fn unsigned long minorVersion()
 *  @brief Returns the minor version number of the plugin
 *  @return The minor version of the plugin
 */
EXPORT
unsigned long CDECL minorVersion()
{
	trace("<> myMinorVersion()");
	return CABLoaderCreator::myMinorVersion();
}

/** @fn unsigned long revision()
 *  @brief Returns the revision of the plugin
 *  @return The revision number of the plugin
 */
EXPORT
unsigned long CDECL revision()
{
	trace("<> revision()");
	return CABLoaderCreator::myRevision();
}

/** @fn RtToken type()
 *  @brief Returns the type of the plugin
 *  @return The renderer type the plugin
 */
EXPORT
const char * CDECL type()
{
	trace("-> type()");
	trace(std::string("-- type = ")+std::string(CABLoaderCreator::myType()));
	trace("<- type()");
	return CABLoaderCreator::myType();
}

/** @fn RtToken name()
 *  @brief Returns the name of the plugin
 *  @return The name of the plugin
 */
EXPORT
const char * CDECL name()
{
	trace("<> name()");
	return CABLoaderCreator::myName();
}

}
