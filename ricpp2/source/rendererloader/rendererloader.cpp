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

/** @file rendererloader.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements a renderer creator by loading an instance from a dynamic library
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */

#include "ricpp/rendererloader/rendererloader.h"
#include "ricpp/ribwriter/ribwriter.h"

using namespace RiCPP;


CRendererLoader::CRendererLib::CRendererLib(CDynLib *dynLib) {
	m_contextCreator = NULL;
	m_newContextCreator = NULL;
	m_deleteContextCreator = NULL;
	m_majorInterfaceVer = NULL;
	m_minorInterfaceVer = NULL;
	m_interfaceRevision = NULL;
	m_rendererType = NULL;

	m_lib = dynLib;
	if ( m_lib ) {
		m_newContextCreator = m_lib->getFunc("newContextCreator");
		m_deleteContextCreator = m_lib->getFunc("deleteContextCreator");
		m_majorInterfaceVer = m_lib->getFunc("majorInterfaceVer");
		m_minorInterfaceVer = m_lib->getFunc("minorInterfaceVer");
		m_interfaceRevision = m_lib->getFunc("interfaceRevision");
		m_rendererType = m_lib->getFunc("rendererType");
		m_rendererName = m_lib->getFunc("rendererName");
	}
}

CRendererLoader::CRendererLib::~CRendererLib() {
	if ( m_contextCreator )
		deleteContextCreator(m_contextCreator);
	m_contextCreator = NULL;

	if ( m_lib ) {
		m_lib->deleteFunc(m_newContextCreator);
		m_lib->deleteFunc(m_deleteContextCreator);
		m_lib->deleteFunc(m_majorInterfaceVer);
		m_lib->deleteFunc(m_minorInterfaceVer);
		m_lib->deleteFunc(m_interfaceRevision);
		m_lib->deleteFunc(m_rendererType);
		m_lib->deleteFunc(m_rendererName);

		CDynLibFactory::deleteDynLib(m_lib);
	}
	m_lib = 0;
}

CContextCreator *CRendererLoader::CRendererLib::getContextCreator(unsigned long majorVersion) {
	if ( m_contextCreator ) {
		if ( m_contextCreator->majorVersion() == majorVersion )
			return m_contextCreator;
		return 0;
	}
	m_contextCreator = newContextCreator(majorVersion);
	return m_contextCreator;
}

bool CRendererLoader::CRendererLib::valid() {
	if ( !validDLL() )
		return false;

	if ( !m_newContextCreator || !m_newContextCreator->valid() )
		return false;
	if ( !m_deleteContextCreator || !m_deleteContextCreator->valid() )
		return false;
	if ( !m_majorInterfaceVer || !m_majorInterfaceVer->valid() )
		return false;
	if ( !m_minorInterfaceVer || !m_minorInterfaceVer->valid() )
		return false;
	if ( !m_interfaceRevision || !m_interfaceRevision->valid() )
		return false;
	if ( !m_rendererType || !m_rendererType->valid() )
		return false;
	if ( !m_rendererName || !m_rendererName->valid() )
		return false;

	if ( IRiContext::riContextMajorVersion != majorInterfaceVer() )
		return false;

	return true;
}

/* Platformdependent */
/*
CContextCreator *newContextrCreator();
void deleteContextCreator(CContextCrteator *);
unsigned long majorInterfaceVer();
unsigned long minorInterfaceVer();
unsigned long interfaceRevision();
const char *rendererType();
*/

CRendererLoader::CRendererLoader() : m_libs(true) {
	m_ribWriterCreator = 0;
	m_searchpath = "$PROGDIR";
}

CRendererLoader::~CRendererLoader() {
	if ( m_ribWriterCreator )
		delete m_ribWriterCreator;
	m_ribWriterCreator = 0;
}

CContextCreator *CRendererLoader::getRibWriterCreator() {
	return new CRibWriterCreator;
}

const char *CRendererLoader::getRibWritername() {
	return CRibWriter::myRendererName();
}

CContextCreator *CRendererLoader::loadContextCreator(const char *name) {
	// While testing load the ribwriter dll (since there is only one dll)
	// const char *ribWriterName = getRibWritername();
	if ( !name || !name[0] /* || ribWriterName && !strcasecmp(name, ribWriterName) */ ) {
		if ( !m_ribWriterCreator ) {
			m_ribWriterCreator = getRibWriterCreator();
		}

		return m_ribWriterCreator;
	}

	CDynLib *dynLib = CDynLibFactory::newDynLib(name, m_searchpath.c_str(), IRiContext::riContextMajorVersion);
	if ( dynLib ) {
		std::string key = dynLib->libname();
		CRendererLib *lib = m_libs.findObj(key);
		if ( lib ) {
			delete dynLib;
			return lib->getContextCreator(IRiContext::riContextMajorVersion);
		}
		if ( dynLib->load() && dynLib->valid() ) {
			lib = new CRendererLib(dynLib);
			if ( lib ) {
				if ( lib->valid() ) {
					m_libs.registerObj(key, lib);
					return lib->getContextCreator(IRiContext::riContextMajorVersion);
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

CContextCreator *CRendererLoader::getContextCreator(RtString name) {
	const char *ptr;
	CStringList stringList;
	stringList.explode(' ', name, true);
	if ( !stringList.empty() ) {
		CStringList::const_iterator first = stringList.begin();
		ptr = strrchr((*first).c_str(), '.');
		if ( !(ptr && !strcmp(ptr, ".rib")) ) {
			return loadContextCreator((*first).c_str());
		}
	}

	return loadContextCreator(0);
}

RtVoid CRendererLoader::doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !name )
		return;

	// The searchpath for the dynamic renderer libraries
	if ( !strcmp(name, "searchpath") ) {
		if ( n < 1 )
			return;
		if ( !strcmp(tokens[0], "renderer") ) {
			m_searchpath = (const char *)params[0];
		}
	}
}
