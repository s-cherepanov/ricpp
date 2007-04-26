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

#include "rendererloader/rendererloader.h"
#include "ribwriter/ribwriter.h"

using namespace RiCPP;


CRendererLoader::CRendererLib::CRendererLib(CDynLib *dynLib) {
	m_renderer = NULL;
	m_newRenderer = NULL;
	m_deleteRenderer = NULL;
	m_majorInterfaceVer = NULL;
	m_minorInterfaceVer = NULL;
	m_rendererType = NULL;

	m_lib = dynLib;
	if ( m_lib ) {
		m_newRenderer = m_lib->getFunc("newRenderer");
		m_deleteRenderer = m_lib->getFunc("deleteRenderer");
		m_majorInterfaceVer = m_lib->getFunc("majorInterfaceVer");
		m_minorInterfaceVer = m_lib->getFunc("minorInterfaceVer");
		m_rendererType = m_lib->getFunc("rendererType");
		m_rendererName = m_lib->getFunc("rendererName");
	}
}

CRendererLoader::CRendererLib::~CRendererLib() {
	if ( m_renderer )
		deleteRenderer(m_renderer);
	m_renderer = NULL;

	if ( m_lib ) {
		m_lib->deleteFunc(m_newRenderer);
		m_lib->deleteFunc(m_deleteRenderer);
		m_lib->deleteFunc(m_majorInterfaceVer);
		m_lib->deleteFunc(m_minorInterfaceVer);
		m_lib->deleteFunc(m_rendererType);
		m_lib->deleteFunc(m_rendererName);

		CDynLibFactory::deleteDynLib(m_lib);
	}
	m_lib = NULL;
}

IRiRenderer *CRendererLoader::CRendererLib::getRenderer() {
	if ( m_renderer )
		return m_renderer;
	m_renderer = newRenderer();
	return m_renderer;
}

bool CRendererLoader::CRendererLib::valid() {
	if ( !validDLL() )
		return false;

	if ( !m_newRenderer || !m_newRenderer->valid() )
		return false;
	if ( !m_deleteRenderer || !m_deleteRenderer->valid() )
		return false;
	if ( !m_majorInterfaceVer || !m_majorInterfaceVer->valid() )
		return false;
	if ( !m_minorInterfaceVer || !m_minorInterfaceVer->valid() )
		return false;
	if ( !m_rendererType || !m_rendererType->valid() )
		return false;
	if ( !m_rendererName || !m_rendererName->valid() )
		return false;

	if ( IRiRenderer::majorVersion > majorInterfaceVer() )
		return false;

	/* // Should be ok if only minor version differ
	if ( IRiRenderer::majorVersion == majorInterfaceVer() )
		if ( IRiRenderer::minorVersion > minorInterfaceVer() )
			return false;
	*/
	return true;
}

/* Platformdependent */
/*
IRiRenderer *newRenderer();
void deleteRenderer(IRiRenderer *);
unsigned long majorInterfaceVer();
unsigned long minorInterfaceVer();
const char *rendererType();
*/

CRendererLoader::CRendererLoader() : m_libs(true) {
	m_ribWriter = 0;
	m_searchpath = ".";
}

CRendererLoader::~CRendererLoader() {
	if ( m_ribWriter )
		delete m_ribWriter;
	m_ribWriter = 0;
}

IRiRenderer *CRendererLoader::getRibWriter() {
	return new CRibWriter;
}

IRiRenderer *CRendererLoader::beginRenderer(RtString name) {
	const char *ptr;
	ptr = name ? strrchr(name, '.') : NULL;
	if ( name && !(ptr && !strcmp(ptr, ".rib")) ) {
		CDynLib *dynLib = CDynLibFactory::newDynLib(name, m_searchpath.c_str());
		if ( dynLib ) {
			std::string key = dynLib->findLib();
			CRendererLib *lib = m_libs.findObj(key);
			if ( lib ) {
				delete dynLib;
				return lib->getRenderer();
			}
			if ( dynLib->load() && dynLib->valid() ) {
				lib = new CRendererLib(dynLib);
				if ( lib ) {
					if ( lib->valid() ) {
						m_libs.registerObj(key, lib);
						return lib->getRenderer();
					} else {
						delete lib;
					}
				}
			} else {
				delete dynLib;
			}
		}
		return NULL;
	}

	if ( !m_ribWriter ) {
		m_ribWriter = getRibWriter();
	}

	return m_ribWriter;
}

RtVoid CRendererLoader::endRenderer(IRiRenderer *renderer) {
	renderer = renderer;
	// do nothing with the renderer, the renderer will be destroyed at the end
}

RtVoid CRendererLoader::abortRenderer(IRiRenderer *renderer) {
	renderer = renderer;
	// do nothing with the renderer, the renderer will be destroyed at the end
}

RtVoid CRendererLoader::doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	RtInt i;

	// The only option will be the searchpath for the dynamic renderer libraries
	name = name;
	for ( i=0; i<n; ++i ) {
		tokens[i] = tokens[i];
		params[i] = params[i];
	}
}
