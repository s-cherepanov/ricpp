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

/*! \file rendererloader.cpp
 *  \brief Implementation of objects to handle libraries and renderer context instances
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "rendererloader.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// TRendererLibSlot: Holds instance of a renderer Library (not of the renderer)
//

///////////////////////////////////////////////////////////////////////////////
// TRendererLibSlot - Initialization
//
TRendererLoader::TRendererLibSlot::TRendererLibSlot() {
	m_hinst = NULL;
	m_getRendererFunc = NULL;
	m_deleteRendererFunc = NULL;
	m_strCurrPath = "";
	m_strRendererName = "";
}

///////////////////////////////////////////////////////////////////////////////
// TRendererLibSlot - Copy constructor
//
TRendererLoader::TRendererLibSlot::TRendererLibSlot(const TRendererLibSlot &rs) {
	m_hinst = NULL;
	m_getRendererFunc = NULL;
	m_deleteRendererFunc = NULL;
	m_strCurrPath = "";
	m_strRendererName = "";
	*this = rs;
}

///////////////////////////////////////////////////////////////////////////////
// ~TRendererLibSlot - unloads DLL (renderers are not discarded -
//                  it's done by loader)
//
TRendererLoader::TRendererLibSlot::~TRendererLibSlot() {
	releaseAll();
}
	
///////////////////////////////////////////////////////////////////////////////
// releaseAll - unloads DLL (renderers are not discarded)
//
void TRendererLoader::TRendererLibSlot::releaseAll() {
	m_deleteRendererFunc = NULL;
	m_getRendererFunc = NULL;
	if ( m_hinst )
		FreeLibrary(m_hinst);
	m_hinst = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// duplicate - returns a new instance of the slot
//
// Returns:
//   a duplication of *this
//
// Throws:
//   TSystemException (not enough memory)
//
TRendererLoader::TRendererLibSlot *TRendererLoader::TRendererLibSlot::duplicate() const throw(TSystemException &) {
	TRendererLibSlot *rs = new TRendererLibSlot(*this);
	if ( !rs )
		throw TSystemException(__FILE__, __LINE__);
	return rs;
}

///////////////////////////////////////////////////////////////////////////////
// operator= : Assigning a slot to another, previous loaded library is closed
//
// Returns:
//   *this
//
TRendererLoader::TRendererLibSlot &TRendererLoader::TRendererLibSlot::operator=(const TRendererLibSlot &rs) {
	// Do nothing if assigned to the same object
	if ( this == &rs )
		return *this;

	// close library
	releaseAll();

	// copy path
	m_strCurrPath = rs.m_strCurrPath;
	m_strRendererName = rs.m_strRendererName;

	// Library opening is deffered to next load
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// load(): returns a new renderer, loads library if needed
//
// Returns:
//   Pointer of a new renderer (must be deleted by caller)
//
// Throws:
//   TSystemException : Error loading Library, not enough memory, etc.
//
TRi *TRendererLoader::TRendererLibSlot::load() throw(TSystemException &) {
	if ( m_strCurrPath.length() <= 0 ) {
		return NULL;
	}

	// DLL is not loaded, so try to load it
	if ( !m_hinst ) {
		m_getRendererFunc = NULL;
		if ( (m_hinst = LoadLibrary(m_strCurrPath.c_str())) == 0L ) {
			throw TSystemException(__FILE__, __LINE__);
		}
	}

	// Get a pointer to the renderer instanciation function
	if ( !m_getRendererFunc ) {
		m_getRendererFunc = (_GetRenderer)GetProcAddress(m_hinst, "GetRenderer");
		if ( !m_getRendererFunc ) {
			int err = GetLastError();
			FreeLibrary(m_hinst);
			m_hinst = NULL;
			throw TSystemException(err, __FILE__, __LINE__);
		}
	}

	// Get a pointer to the renderer deletion function
	if ( !m_deleteRendererFunc ) {
		m_deleteRendererFunc = (_DeleteRenderer)GetProcAddress(m_hinst, "DeleteRenderer");
		if ( !m_deleteRendererFunc ) {
			int err = GetLastError();
			FreeLibrary(m_hinst);
			m_getRendererFunc = NULL;
			m_hinst = NULL;
			throw TSystemException(err, __FILE__, __LINE__);
		}
	}

	// Get a new renderer
	TRi *renderer = m_getRendererFunc();
	if ( !renderer )
		throw TSystemException(__FILE__, __LINE__);

	return renderer;
}

void TRendererLoader::TRendererLibSlot::unload(TRi *renderer) {
	if ( m_deleteRendererFunc && renderer )
	 	m_deleteRendererFunc(renderer);
	// if ( renderer )
	//	delete renderer;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// TRendererLoader - Objekt to load renderers


///////////////////////////////////////////////////////////////////////////////
// TRendererLoader - Initializes object with a search path
//
// Parameters:
//    cpSearchPath - Pointer to a searchpath for renderers
//
TRendererLoader::TRendererLoader(const char *cpSearchPath)
{
	if ( cpSearchPath ) {
		m_strSearchPath = cpSearchPath;
		if ( cpSearchPath[strlen(cpSearchPath)-1] != '\\' )
			m_strSearchPath += "\\";
	}

}

///////////////////////////////////////////////////////////////////////////////
// ~TRendererLoader - destructor frees all Libraries, deletes all renderers
//
TRendererLoader::~TRendererLoader() {
	freeAll();
}

void TRendererLoader::freeAll() {
    // std::cout << "TRendererLoader::freeAll() start\n";

	std::deque<TRendererLib *>::iterator i;
	for ( i = m_renderers.begin(); i != m_renderers.end(); ++i  ) {
		if ( *i ) {
			delete *i;
			*i = NULL;
		}
	}

	std::deque<TRendererLibSlot *>::iterator iter;
	for ( iter = m_rendererDLLs.begin(); iter != m_rendererDLLs.end(); ++iter  ) {
	    if ( *iter ) {
	        delete *iter;
	        *iter = NULL;
	    }
	}

	m_renderers.erase(m_renderers.begin(), m_renderers.end());
	m_rendererDLLs.erase(m_rendererDLLs.begin(), m_rendererDLLs.end());
}

///////////////////////////////////////////////////////////////////////////////
// loadRenderer - loads the renderer with basename cpRendererName
//
// Returns:
//   pointer to an instance a new renderer or NULL
//
// Throws:
//   TSystemException : Error loading Library, not enough memory, etc.
//
TRi *TRendererLoader::loadRenderer(const char *cpRendererName) throw(TSystemException &) {
	std::deque<TRendererLibSlot *>::iterator i;
	for ( i = m_rendererDLLs.begin(); i != m_rendererDLLs.end(); ++i  ) {
		if ( !stricmp((*i)->m_strRendererName.c_str(), cpRendererName) ) {
			TRendererLib *rl = new TRendererLib((*i)->myAddr());
			if ( rl ) {
				m_renderers.push_back(rl);
				return rl->m_renderer;
			}
			return NULL;
		}
	}

	TRendererLibSlot *rs = new TRendererLibSlot;
	if ( !rs )
		return NULL;

	m_rendererDLLs.push_back(rs);
	rs->m_strCurrPath = m_strSearchPath;
	rs->m_strCurrPath += cpRendererName;
	rs->m_strCurrPath += ".dll";
	rs->m_strRendererName = cpRendererName;
    
	TRendererLib *rl = new TRendererLib(rs);
	if ( rl ) {
		m_renderers.push_back(rl);
		return rl->m_renderer;
	}
  
   	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// deleteRenderer - delete the renderer ri, if found in list m_renderers
//
// Parameters:
//    ri - pointer to renderer to be freed
//
// Returns:
//   false: renderer not found and not deleted
//   true:  renderer is deleted (changes pointer in m_renderers to NULL)
//
bool TRendererLoader::deleteRenderer(TRi *ri) {
	std::deque<TRendererLib *>::iterator i;
	for ( i = m_renderers.begin(); i != m_renderers.end(); ++i  ) {
		if ( (*i)->m_renderer == ri ) {
			delete *i;
			*i = NULL;
			// m_renderers.erase(i, i+1);
			return true;
		}
	}

	return false;
}
