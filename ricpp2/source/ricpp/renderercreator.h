#ifndef _RICPP_RICPP_RENDERERCREATOR_H
#define _RICPP_RICPP_RENDERERCREATOR_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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

#ifndef _RICPP_BASERENDERER_RIRENDERER_H
#include "baserenderer/rirenderer.h"
#endif // _RICPP_BASERENDERER_RIRENDERER_H

namespace RiCPP {

/** Create a renderer, either by dynamic loading or instanciating
 */	 
class IRendererCreator
{
public:
	inline virtual ~IRendererCreator() {}

	/** Renderer creation (may throw ERendererException)
	 */
	virtual IRiRenderer *beginRenderer(RtString name) = 0;

	/** Renderer ending
	 *  Do not throw an exception here
	 */
	virtual RtVoid endRenderer(IRiRenderer *renderer) = 0;

	/** Renderer abords, called if a severe (RIE_SEVERE) error occurs
	 *  Do not throw an exception here
	 *  if an renderer is aborted endRenderer() will not be called
	 */
	virtual RtVoid abortRenderer(IRiRenderer *renderer) = 0;

	/** Options called before the renderer begins (may throw ERendererException)
	 *  that is before IRi::begin() is called or between
	 *  IRi::end() and (IRi::context() or IRi::begin())
	 */
	virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
}; // IRendererCreator

} // namespace RiCPP

#endif // _RICPP_RICPP_RENDERERCREATOR_H