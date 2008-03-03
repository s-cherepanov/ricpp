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

/** @file dynlib.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Contains the implementation of the system independent parts
 *         to load a dynamic library.
 */

#include "ricpp/gendynlib/dynlib.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;

CDynLib::CDynLib(const char *libname, const char *searchpath, long int version)
	: m_useCount(0), m_version(version), m_libname(libname), m_searchpath(':', searchpath, true), m_libpath("")
{
}

unsigned long CDynLib::useCount() const
{
	return m_useCount;
}

bool CDynLib::valid() const
{
	return m_useCount > 0;
}

const char *CDynLib::libname() const
{
	return m_libname.c_str();
}

const char *CDynLib::libpath()
{
	return findLib();
}

void CDynLib::load()
// throw ExceptRiCPPError
{
	if ( isLoaded() ) {
		++m_useCount;
		return;
	}
	m_useCount = 0;
	if ( doLoad() ) {
		++m_useCount;
		return;
	}
	throw ExceptRiCPPError(RIE_NOFILE, RIE_SEVERE, __LINE__, __FILE__, "Lib: '%s' Path: '%s'", libname(), libpath());
}

bool CDynLib::unload()
{
	if ( m_useCount == 1 ) {
		m_useCount = 0;
		return doUnload();
	}
	--m_useCount;
	return false;
}

void CDynLib::deleteFunc(ILibFunc *f) const
{
	if ( f )
		delete f;
}


/**********/


void CDynLibFactory::deleteDynLib(CDynLib *lib)
{
	if ( lib )
		delete lib;
}
