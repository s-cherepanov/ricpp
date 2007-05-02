// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright � of RiCPP 2007, Andreas Pidde
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

#include "gendynlib/macdynlib.h"
#include "ricpp/renderererror.h"
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

void *CMacLibFunc::funcPtr() const {
	return m_funcPtr;
}


CMacDynLib::CMacDynLib(const char *libname, const char *searchpath) : m_libHandle((void *)0), CDynLib(libname, searchpath) {
}

CMacDynLib::~CMacDynLib() {
	doUnload();
}

const char *CMacDynLib::staticClassName() {
	return "CMacDynLib";
}

const char *CMacDynLib::className() const {
	return CMacDynLib::staticClassName();
}

bool CMacDynLib::isLoaded() const {
	return m_libHandle != 0;
}

bool CMacDynLib::doLoad() {
	if ( !isLoaded() ) {
		const char *libpath = findLib();
		if ( libpath && libpath[0] ) {
			m_libHandle = dlopen(libpath, RTLD_LOCAL|RTLD_LAZY);
			/*
			if ( !m_libHandle ) {
				const char *err = dlerror();
				std::string errstr(libpath);
				if ( err ) {
					errstr += " ";
					errstr += err;
				} else {
					errstr += " could not be loaded";
				}
				throw ERendererError(RIE_SYSTEM, RIE_SEVERE, errstr.c_str());
			}
			*/
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
	if ( !m_libpath.empty() )
		return m_libpath.c_str();

	std::string dllname = "lib";
	dllname += libname();
	dllname += ".dylib";
	
	m_libpath = "";

	FILE *f = 0;
	std::string strlibpath = "";
	if ( !m_searchpath.empty() ) {
		CStringList::const_iterator i = m_searchpath.begin();
		for ( ; i != m_searchpath.end(); i++ ) {
			strlibpath = *i;
			if ( strlibpath.size() > 0 )
				if ( strlibpath[strlibpath.size()-1] != '/' )
					strlibpath += "/";
			strlibpath += dllname;
			if ( 0 != (f=fopen(strlibpath.c_str(), "r")) ) {
				fclose(f);
				m_libpath = strlibpath;
				break;
			}
		}
	} else {
		// Search the standard path
		m_libpath = dllname;
	}

	// return NULL if not found
	return m_libpath.c_str();
}

CLibFunc *CMacDynLib::getFunc(const char *name) const {
	if ( !valid() )
		return 0;
	void *sym = dlsym(m_libHandle, name);
	return new CMacLibFunc(sym);
}

CDynLib *CDynLibFactory::newDynLib(const char *libname, const char *searchpath) {
	return new CMacDynLib(libname, searchpath);
}
