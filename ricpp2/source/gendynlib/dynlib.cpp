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

#include "dynlib.h"

using namespace RiCPP;

CDynLib::CDynLib(const char *libname, const char *searchpath, long int version)
	: m_useCount(0), m_version(version), m_libname(libname)
{
	m_libpath.clear();
	m_searchpath.explode(';', searchpath, true);
}

CDynLib::~CDynLib() {
}

unsigned long CDynLib::useCount() const {
	return m_useCount;
}

bool CDynLib::valid() const {
	return m_useCount > 0;
}

const char *CDynLib::libname() const {
	return m_libname.c_str();
}

const char *CDynLib::libpath() const {
	return isLoaded() ? m_libpath.c_str() : "";
}

bool CDynLib::load() {
	if ( isLoaded() ) {
		++m_useCount;
		return true;
	}
	m_useCount = 0;
	if ( doLoad() ) {
		++m_useCount;
		return true;
	}
	return false;
}

bool CDynLib::unload() {
	if ( m_useCount == 1 ) {
		m_useCount = 0;
		return doUnload();
	}
	--m_useCount;
	return false;
}

void CDynLib::deleteFunc(CLibFunc *f) const {
	if ( f )
		delete f;
}


/**********/


void CDynLibFactory::deleteDynLib(CDynLib *lib) {
	if ( lib ) {
		delete lib;
	}
}
