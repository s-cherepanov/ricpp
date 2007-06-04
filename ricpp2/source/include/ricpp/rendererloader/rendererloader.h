#ifndef _RICPP_RENDERERLOADER_RENDERERLOADER_H
#define _RICPP_RENDERERLOADER_RENDERERLOADER_H

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

/** @file rendererloader.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declares a class to provide a renderer creator by loading from dynamic library.
 *         A RIB writer creator is obtained by instanciating CRibWriter.
 */

#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#include "ricpp/baserenderer/contextcreator.h"
#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H

namespace RiCPP {

/** @brief Creates a renderer context creator.
 *
 *  It is created either by dynamic loading or instanciating
 */	 
class CRendererLoader : protected TPluginHandler<CContextCreator>
{
protected:
	/** @brief Return real renderername
	 *
	 * The null string stands for the ribwriter
	 *
	 * @param name
	 * @return The renderer name for \a name
	 */
	inline virtual const char *rendererName(RtString name) const
	{
		return emptyStr(name) ? "ribwriter" : name;
	}
	
public:
	/** @brief Virtual destructor
	 */
	inline virtual ~CRendererLoader() {}

	/** @brief CContextCreator creation.
	 * 
	 * May throw ERiCPPException for more error information.
	 *
	 * @param name The argument of IRiRoot::beginV(), indicates the
	 *        name of the renderer context creator.
	 * @return A context creator, new or already loaded.
	 * @exception ERiCPPException
	 */
	inline virtual CContextCreator *getContextCreator(RtString name) { return lastPlugin(rendererName(name)); }

	/** @brief Do not remove, context creator is cached
	 *
	 * Context creators are destroyed, when the frontend is destroyed. If Context creator
	 * is not cached, this function can call deletePlugin(). end calls this function
	 * so uncached context can be used in principle.
	 *
	 * @param cc ContextCreator is not removed
	 */
	inline virtual void removeContextCreator(CContextCreator *cc) {}

	inline virtual bool registerFactory(const char *name, TPluginFactory<CContextCreator> *f) { return TPluginHandler<CContextCreator>::registerFactory(name, f); }

	/** @brief Sets a new searchpath.
	 *
	 * @param aSearchpath New searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline virtual void searchpath(RtString aSearchpath) { TPluginHandler<CContextCreator>::searchpath(aSearchpath); }
}; // IRendererCreator

} // namespace RiCPP

#endif // _RICPP_RENDERERLOADER_RENDERERLOADER_H
