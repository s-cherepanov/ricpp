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

#ifndef _RICPP_RENDERERLOADER_RENDERERCREATOR_H
#include "ricpp/rendererloader/renderercreator.h"
#endif // _RICPP_RENDERERLOADER_RENDERERCREATOR_H

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "ricpp/tools/objptrregistry.h"
#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H

namespace RiCPP {

/** @brief Provides a renderer creator.
 *
 *  The creator is loaded a dynamic library or an allocated
 *  instanciating CRibWriter.
 */
class CRendererLoader : public IRendererCreator
{
private:
	/** @brief A rib writer creator, created by getRibWriterCreator().
	 */
	CContextCreator *m_ribWriterCreator;
	/** @brief The searchpath for the dynamic libraries.
	 *
	 * Can be set by searchpath(), a user can specify a searchpath by
	 * - Option "renderer" "searchpath" "path"
	 */
	std::string m_searchpath;

	/** @brief Class to represent a dynamic library of a renderer creator.
	 *
	 * The dynamic library can deliver one context creator object of a specific renderer.
	 */
	class CRendererLib {
		/** @brief The object to handle dynamic libraries in general.
		 */
		CDynLib *m_lib;

		/** @brief The contextcreator obtained from the library by calling newContextCreator().
		 */
		CContextCreator *m_contextCreator;

		/** @brief The functions loaded from the library
		 */
		//@{
		ILibFunc *m_newContextCreator;    ///< Gets a new context creator
		ILibFunc *m_deleteContextCreator; ///< Deletes a context creator (created by newContextCreator() of the same library)
		ILibFunc *m_majorInterfaceVer;    ///< Gets the major imterface version (the version of IRendererContext
		ILibFunc *m_minorInterfaceVer;    ///< Gets the minor version of a concrete context
		ILibFunc *m_interfaceRevision;    ///< Gets the revision of a concrete context
		ILibFunc *m_rendererType;         ///< Gets the renderer type of a concrete context
		ILibFunc *m_rendererName;         ///< Gets the renderer name of a concrete context
		//@}

		/** @brief Gets a new context creator, if of the right major version
		 *
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp.
		 * @param majorVersion The major version the conext creator shall support
		 * @return A Context creator of the version majorVersion
		 */
		CContextCreator *newContextCreator(unsigned long majorVersion);

		/** @brief Deletes a context creator that was created by newContextCreator() of the same library.
		 *
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp.
		 * @param cc The context creator that should be deleted.
		 */
		void deleteContextCreator(CContextCreator *cc);

	public:
		/** @brief Constructor loads the functions from the dynamic library dynLib
		 *
		 * @param dynLib Representation of the dynamic library that should be used as renderer library
		 */
		CRendererLib(CDynLib *dynLib);

		/** @brief UInloads all symbols and deletes the dynamic library m_lib
		 */
		~CRendererLib();

		/** @brief Tests if loading of the library is ok (all functions found)
		 *
		 * @return All functions m_newContextCreator etc. could be loaded
		 */
		bool valid();

		/** @brief Gets a context creator, if of the right major version
		 *
		 * If there is nor context creator loaded (m_contextCreator is still 0),
		 * newContextCreator() is called to get a new one, it is stored at m_contextCreator.
		 *
		 * @param majorVersion The major version the conext creator shall support
		 * @return A Context creator of the version majorVersion
		 */
		CContextCreator *getContextCreator(unsigned long majorVersion);

		/** @brief Gets the major interface version (the version of IRendererContext
		 *
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp
		 * @return The major interface version
		 */
		unsigned long majorInterfaceVer();

		/** @brief Gets the minor version of a concrete context
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp
		 * @return The minor interface version
		 */
		unsigned long minorInterfaceVer();

		/** @brief Gets the revision of a concrete context
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp
		 * @return The revision version
		 */
		unsigned long interfaceRevision();

		/** @brief Gets the renderer type of a concrete context
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp
		 * @return The type of the renderer context
		 */
		const char *rendererType();

		/** @brief Gets the renderer name of a concrete context
		 * This function is OS dependent and declared in win32rendererlib.cpp or macrendererlib.cpp
		 * @return The name of the renderer context
		 */
		const char *rendererName();
	};

	/** @brief Maps stores the renderer name/renderer library pairs
	 */
	TObjPtrRegistry<std::string, class CRendererLib *> m_libs;

protected:
	/** @brief Gets a new rib writer creator.
	 *
	 * getContextCreator() stores the result in m_ribWriterCreator and reuses it.
	 * @return a new CContextCreator used as rib writer (here an isntance of CRibWriterCreator)
	 */
	virtual CContextCreator *getRibWriterCreator();

	/** @brief Gets the name of the rib writer creator
	 *
	 * Used to compare with the renderer name. If they are the same, the rib writer creator
	 * m_ribWriterCreator can be returned from getContextCreator()
	 * @return a new CContextCreator used as rib writer (here an isntance of CRibWriterCreator)
	 */
	virtual const char *getRibWritername();

	/** @brief Gets a new render context creator CContextCreator
	 *
	 * Used to load a new context creator for the name (from CRi::begin(RtString name))
	 * @param name name of the renderer, can be NULL to name a rib writer context creator
	 * created by getRibWriterCreator() (is also compared to getRibWritername() for this purpose).
	 * @return A CContextCreator instance new or cached.
	 */
	virtual CContextCreator *loadContextCreator(const char *name);

public:
	/** @brief Initializes an empty CRendererLoader
	 */
	CRendererLoader();

	/** @brief Destroys a CRendererLoader, librariers are unloaded.
	*/
	virtual ~CRendererLoader();

	/** @brief Gets a renderer context creator.
	 *
	 * The context creators library-name is obtained from the string name.
	 * name is forwarded from IRiCPPBridge::begin(name). The first word
	 * of this string can contain a ribfile or the renderer name.
	 *
	 * @param name String that contains the name of the renderer (or a RIB file) in front
	 * @return A renderer context creator (cached or new) for name.
	 */
	virtual CContextCreator *getContextCreator(RtString name);

	/** @brief Sets a new searchpath
	 * 
	 * @param aSearchpath Searchpath from Option "renderer" "searchpath", can contain
	 * (pseudo) shell variables $PROG, $PATH, etc.
	 * @see env.h, stringlist.h
	 */
	inline virtual void searchpath(RtString aSearchpath)
	{
		m_searchpath = aSearchpath ? m_searchpath : "";
	}
}; // CRendererLoader

} // namespace RiCPP;

#endif // _RICPP_RENDERERLOADER_RENDERERLOADER_H
