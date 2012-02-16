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

/** @file macdynlib.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a class to handle dynamic libraries in MacOS.
 */
#if !(defined _WIN32)

#include "ricpp/gendynlib/macdynlib.h"

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

#include <dlfcn.h>
#include <iostream>

using namespace RiCPP;

CMacLibFunc::CMacLibFunc(void *aFuncPtr) {
	m_funcPtr = aFuncPtr;
}

CMacLibFunc::~CMacLibFunc() {
}

bool CMacLibFunc::valid() {
	return m_funcPtr != 0;
}

FARPROC CMacLibFunc::funcPtr() const {
	return (FARPROC)m_funcPtr;
}

CMacDynLib::CMacDynLib(const char *libname, const char *searchpath, long int version)
	: m_libHandle((void *)0), CDynLib(libname, searchpath, version)
{
}

CMacDynLib::~CMacDynLib() {
	doUnload();
}
bool CMacDynLib::isLoaded() const {
	return m_libHandle != 0;
}

bool CMacDynLib::doLoad() {
	if ( !isLoaded() ) {
		const char *libpath = findLib();
		if ( libpath && libpath[0] ) {
			m_libHandle = dlopen(libpath, RTLD_LOCAL|RTLD_NOW);
		}
	}
	return isLoaded();
}

bool CMacDynLib::doUnload() {
	if ( !valid() )
		return false;
	if ( dlclose(m_libHandle) != 0 ) {
		m_libHandle = 0; // maybe not useable any more
		return false;
	}
	m_libHandle = 0;
	return true;
}

bool CMacDynLib::valid() const {
	return CDynLib::valid() && m_libHandle != 0;
}

const char *CMacDynLib::findLib() {

	// std::cerr << "findLib()" << std::endl;

	if ( !m_libpath.empty() ) {
		// std::cerr << "findLib(exit) path:" << m_libpath.c_str() << std::endl;
		return m_libpath.c_str();
	}

	std::string dllname(CFilepathConverter::nativeDynlibPrefix());
	dllname += libname();
	if ( m_version >= 0 ) {
		char buf[64] = { 0 };
		sprintf(buf, ".%ld", m_version);
		dllname += buf;
	}
	dllname += CFilepathConverter::nativeDynlibSuffix();
	
	// std::cerr << "findLib() dllname:" << dllname.c_str() << std::endl;

	m_libpath = "";

	FILE *f = 0;
	std::string strlibpath = "";
	if ( !m_searchpath.empty() ) {
		CStringList::const_iterator i = m_searchpath.begin();
		for ( ; i != m_searchpath.end(); i++ ) {
			strlibpath = *i;
			// std::cerr << "XXXXXXXX findLib(*1*) strlibpath:" << strlibpath.c_str() << std::endl;
			if ( strlibpath.size() > 0 )
				if ( strlibpath[strlibpath.size()-1] != '/' )
					strlibpath += "/";
			strlibpath += dllname;
			// std::cerr << "findLib() strlibpath:" << strlibpath.c_str() << std::endl;
			CFilepath p(strlibpath.c_str());
			// std::cerr << "findLib() p.fullpath():" << p.fullpath() << std::endl;
			if ( 0 != (f=fopen(p.fullpath(), "r")) ) {
				fclose(f);
				m_libpath = p.fullpath();
				// std::cerr << "findLib() m_libpath:" << m_libpath.c_str() << std::endl;
			}
		}
	} else {
		// Search the standard path
		m_libpath = "";
		m_libpath += dllname;
		// std::cerr << "findLib() m_libpath:" << m_libpath.c_str() << " dllname " << dllname.c_str() << std::endl;
	}

	// std::cerr << "findLib(exit2) m_libpath:" << m_libpath.c_str() << std::endl;

	return m_libpath.c_str();
}

ILibFunc *CMacDynLib::getFunc(const char *name) const {
	if ( !valid() )
		return 0;
	void *sym = dlsym(m_libHandle, name);
	return new CMacLibFunc(sym);
}

/** @brief The Factory method that returns a Win32 library
 */
CDynLib *CDynLibFactory::newDynLib(const char *libname, const char *searchpath, long int version) {
	return new CMacDynLib(libname, searchpath, version);
}
#endif // !(defined __WIN32__)
