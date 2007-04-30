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

#include "tools/filepath.h"
#include <windows.h>

using namespace RiCPP;

void CFilepath::convertToNative() {
	m_nativepath = m_filepath;
	for ( std::string::size_type i = 0; i < m_nativepath.size(); ++i ) {
		if ( m_nativepath[i] == '/' )
			m_nativepath[i] = '\\';
	}
	m_fullpath = "";
	char pathbuf[MAX_PATH];
	char *ptr;
	if ( GetFullPathNameA(m_nativepath.c_str(), sizeof(pathbuf), pathbuf, &ptr) != 0 ) {
		pathbuf[sizeof(pathbuf)]=0;
		m_fullpath = pathbuf;
	}
}

bool CFilepath::isAbsolute() const {
	return (m_nativepath.size() > 0 && m_nativepath[0] == '\\') || (m_nativepath.size() > 1 && m_nativepath[1] == ':');
}
