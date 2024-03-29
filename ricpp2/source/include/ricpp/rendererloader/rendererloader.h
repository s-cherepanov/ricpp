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

#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H
#include "ricpp/ricontext/contextcreator.h"
#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H

namespace RiCPP {

/** @brief Creates a renderer context creator.
 *
 *  It is created either by dynamic loading or instanciating
 */	 
class CRendererLoader : protected TemplPluginHandler<CContextCreator>
{
protected:
	/**
	 * Name of the standard renderer, standard string is "ribwriter"
	 */
	std::string m_standardRendererName;

	/** @brief Return real renderername
	 *
	 * The null string stands for the standard renderer
	 *
	 * @param name
	 * @return The renderer name for @a name
	 */
	virtual const char *rendererName(RtString name) const;
	
public:
	/** @brief Standard constructor
	 */
	CRendererLoader();

	/** @brief Virtual destructor
	 */
	virtual ~CRendererLoader();

	/** @brief CContextCreator creation.
	 * 
	 * May throw ERiCPPException for more error information.
	 *
	 * @param name The argument of IRiRoot::beginV(), indicates the
	 *        name of the renderer context creator.
	 * @return A context creator, new or already loaded.
	 * @exception ERiCPPException
	 */
	virtual CContextCreator *getContextCreator(RtString name);

	/** @brief Do not remove, context creator is cached
	 *
	 * Context creators are destroyed, when the frontend is destroyed. If Context creator
	 * is not cached, this function can call deletePlugin(). end calls this function
	 * so uncached context can be used in principle.
	 *
	 * @param cc ContextCreator is not removed
	 */
	inline virtual void removeContextCreator(CContextCreator *cc) {}

	/** @brief Registers a plugin factory
	 *
	 *  Registers a plugin factory for a specific name. Normally
	 *  TemplPluginFactory are registered to create specific plugins with
	 *  @c new instead of loading them from a dynamic library.
	 *
	 * @param name Name of the plugins
	 * @param f Factory to create the plugins
	 * @return true, if the plugin factory could be registerd
	 */
	inline virtual bool registerFactory(
		const char *name,
		TemplPluginFactory<CContextCreator> *f)
	{
		return TemplPluginHandler<CContextCreator>::registerFactory(name, f);
	}

	/** @brief unregisters a plugin factory
	 * @param name Name of the plugins
	 * @return true, if factory was unregistered
	 */
	inline virtual bool unregisterFactory(const char *name)
	{
		return TemplPluginHandler<CContextCreator>::unregisterFactory(name);
	}

	/** @brief Sets a new searchpath.
	 *
	 * @param aSearchpath New searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline virtual void searchpath(RtString aSearchpath)
	{
		TemplPluginHandler<CContextCreator>::searchpath(aSearchpath);
	}

	/** @brief Gets the current searchpath.
	 *
	 * @return Searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline const char *searchpath() const
	{
		return TemplPluginHandler<CContextCreator>::searchpath();
	}

	/** @brief Gets the name of the standard renderer.
	 *
	 * @return Name of the standard renderer
	 */
	inline const char *standardRendererName() const
	{
		return m_standardRendererName.c_str();
	}

	/** @brief Sets the name of the standard renderer.
	 *
	 * @param name Name of the standard renderer, 0 is substituted by "ribwriter"
	 */
	void standardRendererName(const char *name);

}; // IRendererCreator

} // namespace RiCPP

#endif // _RICPP_RENDERERLOADER_RENDERERLOADER_H
