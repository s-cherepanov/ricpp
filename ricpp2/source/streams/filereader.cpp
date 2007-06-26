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

/** @file filereader.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a class to read files
 */

#include "ricpp/streams/filereader.h"

using namespace RiCPP;

// file: scheme, see
// ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WEBDEV.v10.en/ASYNCPLUGPROTO/workshop/networking/predefined/file.htm

const char *CFileReader::myName() { return "filereader"; }
const char *CFileReader::myType() { return CReader::myType(); }
unsigned long CFileReader::myMajorVersion() { return CReader::myMajorVersion(); }
unsigned long CFileReader::myMinorVersion() { return 1; }
unsigned long CFileReader::myRevision() { return 1; }

bool CFileReader::open(const char *an_uri)
{
	m_uri = an_uri;
	return false;
}

TemplBuffer<>::size_type CFileReader::fillBuffer(TemplBuffer<> &buffer)
{
	return 0;
}

bool CFileReader::close()
{
	return false;
}
