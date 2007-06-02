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

/** @brief Creates a renderer context creator.
 *
 *  It is created either by dynamic loading or instanciating
 */	 
class IRendererCreator
{
	/** @brief Error handler used by CContextCreator, it is returned by the virtual ricppErrHandler().
	 *
	 * This error handler throws an ERendererError exception. The exception is
	 * catched by the front end and bridged to the RenderMan error handler.
	 * Member shall be never used directly, so it can be changed by an overwritten ricppErrHandler()
	 * at a child class.
	 */
	CErrorExceptionHandler m_errorHandler;

protected:
	/** @brief Returns the error handler to use.
	 *
	 * It can but usually won't be overwritten in this framework.
	 *
	 *  @return m_errorHandler, the default CErrorExceptionHandler is returned.
	 */
	inline virtual IRiCPPErrorHandler &ricppErrHandler() { return m_errorHandler; }

public:
	/** @brief Virtual destructor
	 */
	inline virtual ~IRendererCreator() {}

	/** @brief CContextCreator creation.
	 * 
	 * May throw ERendererException for more error information.
	 *
	 * @param name The argument of IRiRoot::beginV(), indicates the
	 *        name of the renderer context creator.
	 * @return A context creator, new or already loaded.
	 */
	virtual CContextCreator *getContextCreator(RtString name) = 0;

	/** @brief Removes renderer context creator.
	 *
	 * @param cc ContextCreator to remove
	 */
	virtual void removeContextCreator(CContextCreator *cc) = 0;

	/** @brief Sets a new searchpath.
	 *
	 * @param aSearchpath New searchpath, directory seperator '/', pathes separated by ';'.
	 */
	virtual void searchpath(RtString aSearchpath) = 0;
}; // IRendererCreator

} // namespace RiCPP

#endif // _RICPP_RENDERERLOADER_RENDERERCREATOR_H