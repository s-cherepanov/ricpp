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
 *  @brief Implementation of the zlib-enabled streambuffer facets.
 */

#include "ricpp/streams/backbuffer.h"

using namespace RiCPP;

const char *CBackBufferFactory::myType() { return "backbufferfactory"; }
const char *CBackBufferFactory::myName() { return "backbufferfactory"; }
unsigned long CBackBufferFactory::myMajorVersion() { return 1; }
unsigned long CBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CBackBufferFactory::myRevision() { return 1; }

const char *CFileBackBufferFactory::myType() { return CBackBufferFactory::myType(); }
const char *CFileBackBufferFactory::myName() { return "file_backbuffer"; }
unsigned long CFileBackBufferFactory::myMajorVersion() { return CBackBufferFactory::myMajorVersion(); }
unsigned long CFileBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CFileBackBufferFactory::myRevision() { return 1; }
