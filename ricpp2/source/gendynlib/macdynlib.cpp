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
#include <dlfcn.h>

using namespace RiCPP;

CMacLibFunc::CMacLibFunc(void *aFuncPtr) {
	m_funcPtr = aFuncPtr;
}

CMacLibFunc::~CMacLibFunc() {
}

bool CMacLibFunc::valid() {
	return m_funcPtr != (void *)0;
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

bool CMacDynLib::isLoaded() {
	return m_libHandle != (void *)0;
}

bool CMacDynLib::doLoad() {
	if ( !isLoaded() ) {
		std::string strlibname = "lib";
		strlibname += libname();
		strlibname += ".dylib"; 
		m_libHandle = dlopen(strlibname.c_str(), RTLD_LOCAL|RTLD_LAZY);
	}
	return isLoaded();
}

bool CMacDynLib::doUnload() {
	if ( !valid() )
		return false;
	if ( dlclose(m_libHandle) != 0 ) {
		m_libHandle = (void *)0; // maybe not useable any more
		return false;
	}
	m_libHandle = (void *)0;
	return true;
}

bool CMacDynLib::valid() const {
	return CDynLib::valid() && m_libHandle != (void *)0;
}

const char *CMacDynLib::findLib() {
	if ( !m_libPath.empty() )
		return m_libPath.c_str();

	// ToDo search Lib
	m_libPath = libname();
	// return NULL if not found
	return m_libPath.c_str();
}

CLibFunc *CMacDynLib::getFunc(const char *name) const {
	if ( !valid() )
		return NULL;
	void *sym = dlsym(m_libHandle, name);
	return new CMacLibFunc(sym);
}

CDynLib *CDynLibFactory::newDynLib(const char *libname, const char *searchpath) {
	return new CMacDynLib(libname, searchpath);
}
