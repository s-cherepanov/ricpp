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

#include "gendynlib/win32dynlib.h"
#include "tools/filepath.h"
#include <stdio.h>

using namespace RiCPP;

CWin32LibFunc::CWin32LibFunc(FARPROC ptr) : m_funcPtr(ptr) {
}

CWin32LibFunc::~CWin32LibFunc() {
}

bool CWin32LibFunc::valid() {
	return m_funcPtr != (FARPROC)NULL;
}

FARPROC CWin32LibFunc::funcPtr() const {
	return m_funcPtr;
}

const HMODULE CWin32DynLib::invalidLibHandle = (HMODULE)NULL;

CWin32DynLib::CWin32DynLib(const char *libname, const char *searchpath) : m_libHandle(invalidLibHandle), CDynLib(libname, searchpath) {
	// lower case of libname
	for ( std::string::size_type i = 0; i < m_libname.size(); ++i ) {
		m_libname[i] = tolower(m_libname[i]);
	}
}

CWin32DynLib::~CWin32DynLib() {
	doUnload();
}

const char *CWin32DynLib::staticClassName() {
	return "CWin32DynLib";
}

const char *CWin32DynLib::className() const {
	return CWin32DynLib::staticClassName();
}

bool CWin32DynLib::isLoaded() const {
	return m_libHandle != invalidLibHandle;
}

bool CWin32DynLib::doLoad() {
	if ( !isLoaded() ) {
		m_libHandle = LoadLibraryA(findLib());
		if ( m_libHandle != invalidLibHandle ) {
			char modulepath[MAX_PATH];
			modulepath[0] = (char)0;
			DWORD fsize = GetModuleFileNameA(m_libHandle, modulepath, sizeof(modulepath));
			modulepath[sizeof(modulepath)-1] = (char)0;
			m_libpath = modulepath;
		}
	}
	return isLoaded();
}

bool CWin32DynLib::doUnload() {
	BOOL result = TRUE;
	if ( m_libHandle != invalidLibHandle ) {
		result = FreeLibrary(m_libHandle);
		m_libHandle = invalidLibHandle;
	}
	return (bool)(result != FALSE);
}

bool CWin32DynLib::valid() const {
	return CDynLib::valid() && m_libHandle != invalidLibHandle;
}

const char *CWin32DynLib::findLib() {
	if ( !m_libpath.empty() )
		return m_libpath.c_str();

	// ToDo search Lib
	std::string dllname = libname();
	dllname += ".dll";

	std::string strlibpath("");
	if ( !m_searchpath.empty() ) {
		CStringList::const_iterator i = m_searchpath.begin();
		for ( ; i != m_searchpath.end(); i++ ) {
			strlibpath = *i;
			if ( strlibpath.size() > 0 )
				if ( strlibpath[strlibpath.size()-1] != '/' )
					strlibpath += "/";
			strlibpath += dllname;
			FILE *f = NULL;
			CFilepath p(strlibpath.c_str());
			if ( fopen_s(&f, p.nativepath(), "r") == 0 ) {
				fclose(f);
				m_libpath = p.fullpath();
				break;
			}
		}
	} else {
		// Search the standard path
		m_libpath = dllname;
	}

	return m_libpath.c_str();
}

CLibFunc *CWin32DynLib::getFunc(const char *name) const {
	if ( m_libHandle != invalidLibHandle ) {
		return new CWin32LibFunc(GetProcAddress(m_libHandle, name));
	}
	return NULL;
}

CDynLib *CDynLibFactory::newDynLib(const char *libname, const char *searchpath) {
	return new CWin32DynLib(libname, searchpath);
}
