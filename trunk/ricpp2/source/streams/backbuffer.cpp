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

/** @file backbuffer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation for zlib-enabled dtreams and base classes for plugable streambuffer facets.
 */

#include "ricpp/streams/backbuffer.h"

using namespace RiCPP;

// ----------------------------------------------------------------------------

bool CFileBackBuffer::open(
	const CUri &absUri,
	TypeOpenMode mode)
{
	close();

	CBackBufferRoot::open(absUri, mode);
	
	m_mode = mode;
	std::string filename;
	absUri.decodeFilepath(filename);

	// std::cerr << "# FILENAME: '" << filename << "'" << std::endl;

	m_filebuf.open(filename.c_str(), mode);
	return m_filebuf.is_open();
}

std::streamsize CFileBackBuffer::sgetn(char *b, std::streamsize size) 
{
	if ( !isOpen() || !b || !size ) {
		// File not open or no buffer
		return 0;
	}
	if ( !(m_mode & std::ios_base::in) ) {
		// File not opened for reading
		return 0;
	}

	// Read from buffer
#ifdef _MSC_VER
	return m_filebuf._Sgetn_s(b, size, size);
#else
	return m_filebuf.sgetn(b, size);
#endif
}

std::streamsize CFileBackBuffer::sputn(const char *b, std::streamsize size) 
{
	if ( !isOpen() || !b || !size ) {
		// File not open or no buffer
		return 0;
	}
	if ( !(m_mode & std::ios_base::out) ) {
		// File not opened for writing
		return 0;
	}
	return m_filebuf.sputn(b, size);
}

// ----------------------------------------------------------------------------

const char *CBackBufferFactory::myType() { return "backbufferfactory"; }
const char *CBackBufferFactory::myName() { return "backbufferfactory"; }
unsigned long CBackBufferFactory::myMajorVersion() { return 1; }
unsigned long CBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CBackBufferFactory::myRevision() { return 1; }

void CBackBufferFactory::addScheme(const char *scheme)
{
	std::string str_scheme(noNullStr(scheme));
	if ( str_scheme.empty() )
		return;
	std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(toupper));
	m_schemes.push(str_scheme.c_str());
}

bool CBackBufferFactory::acceptsScheme(const char *scheme) const
{
	std::string str_scheme(noNullStr(scheme));
	std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(asciiToUpper));
	return m_schemes.isMember(str_scheme.c_str());
}

bool CBackBufferFactory::close(CBackBufferRoot *bbr)
{
	bool rval = false;
	if ( bbr ) {
		bbr->close();
		return unRegisterObj(bbr);
	}
	return rval;
}

// ----------------------------------------------------------------------------

const char *CFileBackBufferFactory::myType() { return CBackBufferFactory::myType(); }
const char *CFileBackBufferFactory::myName() { return "file_backbuffer"; }
unsigned long CFileBackBufferFactory::myMajorVersion() { return CBackBufferFactory::myMajorVersion(); }
unsigned long CFileBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CFileBackBufferFactory::myRevision() { return 1; }

CBackBufferRoot *CFileBackBufferFactory::open(
	const CUri &absUri,
	TypeOpenMode mode)
{
	CFileBackBuffer *buf = new CFileBackBuffer;
	if ( buf ) {
		if ( !buf->open(absUri, mode) ) {
			delete buf;
			return 0;
		}
	}
	registerObj(buf);
	return buf;
}

// ----------------------------------------------------------------------------

void CBackBufferProtocolHandlers::init(const char *direct)
{
	m_direct = direct ? direct : "";
	m_hasDirect = direct != 0;

	if ( !m_backBufferPluginHandler.isRegistered("filebuffer") ) {

		// The FILE handler is registered directly and is not loaded
		m_backBufferPluginHandler.registerFactory(
			"file.buffer",
			(TemplPluginFactory<CBackBufferFactory> *)&m_fileBuffer );

		// Load more handlers (there are none at the moment)
		try {
			m_backBufferPluginHandler.registerFromDirectory(direct, ".buffer");
		} catch ( ... ) {
			throw;
		}
	}
}

CBackBufferFactory *CBackBufferProtocolHandlers::getBufferFactory(const char *scheme)
{
	TemplPluginHandler<CBackBufferFactory>::const_iterator i;
	for ( i = m_backBufferPluginHandler.begin(); i != m_backBufferPluginHandler.end(); i++ ) {
		CBackBufferFactory *f = (*i).second->lastPlugin();
		if ( f && f->acceptsScheme(scheme) )
			return f;
	}
	return 0;
}
