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
 *  @brief Implements the creation of a renderer
 */

#include "ricpp/rendererloader/rendererloader.h"
#include "ricpp/ribwriter/ribwriter.h"
#include "ricpp/tools/platform.h"

using namespace RiCPP;


CContextCreator *CRendererLoader::CRendererLib::newContextCreator(unsigned long majorversion) {
	CContextCreator *cc = ((TypeNewContextCreatorFunc)m_newContextCreator->funcPtr())(majorversion);
	return cc;
}
void CRendererLoader::CRendererLib::deleteContextCreator(CContextCreator *cc) {
	((TypeDeleteContextCreatorFunc)m_deleteContextCreator->funcPtr())(cc);
}

unsigned long CRendererLoader::CRendererLib::majorInterfaceVer() {
	return ((TypeMajorInterfaceVerFunc)m_majorInterfaceVer->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::minorInterfaceVer() {
	return ((TypeMinorInterfaceVerFunc)m_minorInterfaceVer->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::interfaceRevision() {
	return ((TypeInterfaceRevisionFunc)m_interfaceRevision->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererType() {
	return ((TypeRendererTypeFunc)m_rendererType->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererName() {
	return ((TypeRendererNameFunc)m_rendererName->funcPtr())();
}

CRendererLoader::CRendererLib::CRendererLib(CDynLib *dynLib)
{
	m_contextCreator = 0;
	m_newContextCreator = 0;
	m_deleteContextCreator = 0;
	m_majorInterfaceVer = 0;
	m_minorInterfaceVer = 0;
	m_interfaceRevision = 0;
	m_rendererType = 0;

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

CRendererLoader::CRendererLib::~CRendererLib()
{
	if ( m_contextCreator )
		deleteContextCreator(m_contextCreator);
	m_contextCreator = 0;

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

CContextCreator *CRendererLoader::CRendererLib::getContextCreator(unsigned long majorVersion)
// throw ERiCPPError
{
	if ( m_contextCreator ) {
		if ( m_contextCreator->majorVersion() == majorVersion )
			return m_contextCreator;
		throw ERiCPPError(RIE_VERSION, RIE_SEVERE, "Major version of context creator does not match.", __LINE__, __FILE__);
		return 0;
	}
	m_contextCreator = newContextCreator(majorVersion);
	return m_contextCreator;
}

bool CRendererLoader::CRendererLib::valid()
{
	if ( !m_lib || !m_lib->valid() )
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


/*
// Platformdependent
CContextCreator *newContextrCreator();
void deleteContextCreator(CContextCrteator *);
unsigned long majorInterfaceVer();
unsigned long minorInterfaceVer();
unsigned long interfaceRevision();
const char *rendererType();
*/

CRendererLoader::CRendererLoader()
	: m_libs(true)
{
	m_ribWriterCreator = 0;
	m_searchpath = "$PROGDIR";
}

CRendererLoader::~CRendererLoader()
{
	if ( m_ribWriterCreator )
		delete m_ribWriterCreator;
	m_ribWriterCreator = 0;
}

CContextCreator *CRendererLoader::getRibWriterCreator()
// throw ERiCPPError
{
	CContextCreator *cc = 0;
	try {
		cc = new CRibWriterCreator;
	} catch (...) {
		// cc not created
	}

	if ( !cc ) {
		ricppErrHandler().handleErrorV(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a new CRibWriterCreator");
	}
	return cc;
}

const char *CRendererLoader::getRibWritername()
{
	return CRibWriter::myRendererName();
}

CContextCreator *CRendererLoader::loadContextCreator(const char *name)
// throw ERiCPPError
{
	// name: if there is no name, create RIB output
	const char *ribWriterName = getRibWritername();

	if ( !ribWriterName ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "Could not get the name for the RIB writer.");
		return 0;
	}

	if ( !name || !*name ) {
		name = ribWriterName;
	}

	// Step 1: Gets the Rib Writer
	// name: if it is the name of the RibWriter return the RibWriter
	// commented out to test the loading of libraries
	/*
	if ( ribWriterName && !strcasecmp(name, ribWriterName) ) {
		if ( !m_ribWriterCreator ) {
			try {
				m_ribWriterCreator = getRibWriterCreator();
			} catch (ERiCPPError &err) {
				m_ribWriterCreator = 0;
				ricppErrHandler().handleError(err);
			}
			return 0;
		}
	}
	// name does not stand for the standard RibWriter
	*/

	// Step 2: Gets the loader for dynamic libraries
	CDynLib *dynLib = 0;
	try {
		dynLib = CDynLibFactory::newDynLib(name, m_searchpath.c_str(), IRiContext::riContextMajorVersion);
	} catch ( ... ) {
		// dynlib not created
	}
	if ( !dynLib ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create a new CDynLib for %s", !name && !*name ? "<no libname>" : name);
		return 0;
	}

	// Step 3: Library already loaded?
	CRendererLib *lib = 0;
	// use libname as key, the path is not used for identification
	std::string key = dynLib->libname();
	lib = m_libs.findObj(key);

	if ( lib ) {
		// Lib already loaded
		CDynLibFactory::deleteDynLib(dynLib);
		try {
			CContextCreator *cc = lib->getContextCreator(IRiContext::riContextMajorVersion);
			return cc;
		} catch (ERiCPPError &err) {
			ricppErrHandler().handleError(err);
		}
		return 0;
	}

	// Step 4: Loads Library
	try {
		dynLib->load();
	} catch ( ERiCPPError &err ) {
		CDynLibFactory::deleteDynLib(dynLib);
		ricppErrHandler().handleError(err);
		return 0;
	}

	if ( !dynLib->valid() ) {
		CDynLibFactory::deleteDynLib(dynLib);
		ricppErrHandler().handleError(RIE_BADFILE, RIE_SEVERE, "Invalid library %s", key.empty() ? "<no libname>" : key.c_str());
		return 0;
	}

	// Step 5: Creates new library instance
	try {
		lib = new CRendererLib(dynLib);
	} catch ( ... ) {
		// lib not created
	}
	if ( !lib ) {
		CDynLibFactory::deleteDynLib(dynLib);
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create a new CRendererLib for %s", key.empty() ? "<no libname>" : key.c_str());
		return 0;
	}

	if ( !lib->valid() ) {
		// also deletes dynLib
		delete lib;
		ricppErrHandler().handleError(RIE_BADFILE, RIE_SEVERE, __LINE__, __FILE__ "Library of %s not of expected type", key.empty() ? "<no libname>" : key.c_str());
		return 0;
	}

	CContextCreator *cc = 0;

	try {
		cc = lib->getContextCreator(IRiContext::riContextMajorVersion);
	} catch (ERiCPPError &err) {
		// also deletes dynLib
		delete lib;
		ricppErrHandler().handleError(err);
		return 0;
	}

	// Step 5: Registers library
	m_libs.registerObj(key, lib);
	return cc;
}

CContextCreator *CRendererLoader::getContextCreator(RtString name)
// throw ERiCPPError
{
	CContextCreator *cc = 0;
	try {
		cc = loadContextCreator(name);
	} catch ( ERiCPPError &err ) {
		ricppErrHandler().handleError(err);
		return 0;
	}
	return cc;
}
