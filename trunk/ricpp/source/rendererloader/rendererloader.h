#ifndef _RICPP_RENDERLOADER_RENDERERLOADER_H
#define _RICPP_RENDERLOADER_RENDERERLOADER_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file rendererloader.h
 *  \brief Declaration of classes to handle libraries and renderer context instances
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ricpp.h"
#include "sysexcept.h"

#include <deque>

//! Function prototype, defined in each RiCPP Renderer DLL.

extern "C" {
	/*! \return  The function should return the instance of the rendercontext implemented with the DLL.
	 */
	typedef TRi * (CDECL * _GetRenderer)();
	/*! \return  Delete a renderer created with _GetRenderer()
	 */
	typedef void (CDECL * _DeleteRenderer)(TRi *);
}

//! Handles the loading and unloading of the render contexts.

/*! Normally only one instance of this class is created. If this instance
 *  is destroyed, all renderer contexts are deleted and all DLL's are unloaded.
 *  For this purpose a deque of all used DLL's in TRendererLibSlot instances
 *  (m_rendererDLLs) and all created renderer instances (m_renderers) is
 *  maintained. The path where the DLL's are searched is stored in m_strSearchPath and
 *  is defined by calling the constructor.
 */
class TRendererLoader {

	//! Is a private helper class (contains data for a renderer DLL) for TRendererLoader
	/*! TRendererLibSlot is not keeping track of instanciated renderers.
	 *  TRendererLoader is doing this.
	 *  \sa TRendererLoader
	 */
	class TRendererLibSlot {
		friend class TRendererLoader;   //!< The outer class TRenderLoader can use the data members for easier access
		friend class TRendererLib;		//!< Container of renderers may call the deletion and creation functions of a renderer

		std::string m_strCurrPath;		//!< Path of the renderer DLL.
		std::string m_strRendererName;	//!< Name of the renderer (without extensions).
		HINSTANCE m_hinst;				//!< DLL for renderer.
		_GetRenderer m_getRendererFunc;	//!< Returns new instance of a renderer.
		_DeleteRenderer m_deleteRendererFunc;	//!< Deletes an instance of a renderer created by m_getRendererFunc.

	public:
		TRendererLibSlot(); //!< Initialization, zeros all members

		//! Copy constructor
		/*! Copies the path m_strCurrPath and the
		 *  renderer name m_strRendererName, so that the library gets
		 *  reloaded by the next call of TRendererLibSlot::load()
		 *  \param rs Source to copy from
		 *  \sa TRendererLibSlot::load()
		 */
		TRendererLibSlot(const TRendererLibSlot &rs);

		~TRendererLibSlot(); //!< Destructor, unloads the library

		//! Instance duplication
		/*! Returns a copy of the instance, the library is reloaded if
		 *  load of the returned instance is called.
		 *  \exception TSystemException Throws a
		 *  TSystemException if not enough memory.
		 *  \return Returns a pointer to the new instance.
		 */
		TRendererLibSlot *duplicate() const throw(TSystemException &);

		//! Assigns a copy of the slot rs, unloads library of 'this' first.
		/*! \param  rs The TRendererLibSlot to copy
		 *  \return A reference to this.
		 */
		TRendererLibSlot &operator=(const TRendererLibSlot &rs);

		//! Load a DLL
		/*! Loads the DLL if called the first time and returns a new renderer context
		 *  on every call.
		 *  \exception TSystemException Throws a TSystemException, if the DLL could not be loaded,
		 *  the _GetRenderer() Function could not be found or the renderer could
		 *  not be instanciated.
		 *  \exception TSystemException DLL could not be loaded for some reason
		 *  \return An instance of the new renderer
		 */
		TRi *load() throw(TSystemException &);
		void unload(TRi *);

		inline TRendererLibSlot *myAddr() { return this; }

		//! Unloads the DLL referenced by m_hinst
		void releaseAll();
	};

	std::string m_strSearchPath;					//!< Path to search for renderers.
	std::deque<TRendererLibSlot *> m_rendererDLLs;	//!< Deque of all loaded renderer libraries.

	class TRendererLib {
		friend class TRendererLoader;   //!< The outer class TRenderLoader can use the data members for easier access
		TRi *m_renderer;
		TRendererLibSlot *m_lib;
	public:
		inline TRendererLib(): m_renderer(NULL), m_lib(NULL) {}
		inline TRendererLib(const TRendererLib &rl): m_renderer(NULL), m_lib(NULL) {*this = rl;}
		inline TRendererLib(TRendererLibSlot *lib): m_renderer(NULL), m_lib(lib) {
			if ( m_lib )
				m_renderer = m_lib->load();
		}
		inline ~TRendererLib() {
			if ( m_lib && m_renderer ) {
				m_lib->unload(m_renderer);
			}
		}
		inline TRendererLib &operator = (const TRendererLib &rl) {
			if ( this == &rl )
				return *this;
			if ( m_lib && m_renderer ) {
				m_lib->unload(m_renderer);
			}
			m_renderer = NULL;
			m_lib = rl.m_lib;
			if ( m_lib )
				m_renderer = m_lib->load();
			return *this;
		}
	};
	std::deque<TRendererLib *> m_renderers;					//!< Deque of Pointer to all renderer instances.

public:
	//!Constructor, initialize with searchpath
	/* \param cpSearchPath The path to search the libraries. defaults to NULL, the current path.
	 */
	TRendererLoader(const char *cpSearchPath=NULL);

	//! Destroys all renderers and closes all libraries.
	virtual ~TRendererLoader();

	//! Frees all resources
	void freeAll();

	//! Loads a specific renderer
	/*! Loads a renderer library only the first time
	 *  after loaded, it returns a pointer instance of the renderer context, either
	 *  new loaded or found in the deque od previously loaded DLLs.
	 *  \exception TSystemException Error loading Library, not enough memory, etc.
	 *  \param cpRendererName Name of a renderer without an extension like .dll
	 *  \return Pointer to new renderer context
	 */
	virtual TRi *loadRenderer(const char *cpRendererName) throw(TSystemException &);

	//! Deletes a renderer context and removes it from the list of instanciated contexts.
	/*! \param  ri Renderer context that will be deleted.
	 *  \return true if renderer context is deleted, false otherwise (renderer contect not found).
	 */
	virtual bool deleteRenderer(TRi *ri);
};

#endif // _RICPP_RENDERLOADER_RENDERERLOADER_H
