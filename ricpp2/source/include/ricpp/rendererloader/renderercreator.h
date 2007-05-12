#ifndef _RICPP_RENDERERLOADER_RENDERERCREATOR_H
#define _RICPP_RENDERERLOADER_RENDERERCREATOR_H

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

/** @file renderercreator.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Interface to provide a CContextCreator
 * 
 *  The renderer will be created either by loading a dynamic
 *  library or by dynamically allocate one. A concrete IRendererCreator
 *  will manage the dynamically created CContextCreator witch in turn
 *  can create contexts the concrete implementations of a renderer. CRendererLoader
 *  implements this interface.
 */

#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#include "ricpp/baserenderer/contextcreator.h"
#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H

namespace RiCPP {

/** Create a renderer context creator, either by dynamic loading or instanciating
 */	 
class IRendererCreator
{
public:
	/** Virtual destructor
	 */
	inline virtual ~IRendererCreator() {}

	/** CContextCreator creation, may throw ERendererException for more error information
	 * @param name The argument of IRi::begin(RtString name), indicates the
	 *        name of the renderer creator with parameters appended
	 * @return A context creator, new or already loaded
	 */
	virtual CContextCreator *getContextCreator(RtString name) = 0;

	/** Sets a new searchpath
	 *
	 * @param aSearchpath New searchpath, directory seperator '/', pathes separated by ';'
	 */
	virtual void searchpath(RtString aSearchpath) = 0;
}; // IRendererCreator

} // namespace RiCPP

#endif // _RICPP_RENDERERLOADER_RENDERERCREATOR_H