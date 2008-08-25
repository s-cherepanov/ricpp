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

/** @file subdivision.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the classes for catmull-clark subdivision surfaces meshes.
 *
 *  See also "Subdivision Surfaces in Character Animation"
 *  by Tony DeRose, Michael Kass and Tien Truong
 *  and the RenderMan specification.
 */

#include "ricpp/ricontext/subdivision.h"

#ifdef _DEBUG
#include <iostream>
#define _TRACE
#endif

using namespace RiCPP;

// ----------------------------------------------------------------------------
void CSubdivFace::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "CSubdivFace:" << std::endl;
	o << pre;
	o << "   m_origFaceIndex: " << m_origFaceIndex << ", ";
	o << "   m_index: " << m_index << ", ";
    o << "m_type: " << (m_type == FACE_FILLED ? "FACE_FILLED" : "FACE_HOLE") << std::endl;
	o << pre << "   m_startVertexEdgeIndex: " << m_startVertexEdgeIndex << ", " << "m_endVertexEdgeIndex: " << m_endVertexEdgeIndex << std::endl;
	o << pre << "   m_normal: [" << m_normal[0] << ", " << m_normal[1] << ", " << m_normal[2] << "]" << std::endl;
}


// ----------------------------------------------------------------------------
CSubdivFace &CSubdivFace::operator=(const CSubdivFace &f) {
	if ( &f == this ) {
		return *this;
	}
	
	m_type                 = f.m_type;
	m_startVertexEdgeIndex = f.m_startVertexEdgeIndex;
	m_endVertexEdgeIndex   = f.m_endVertexEdgeIndex;
	m_origFaceIndex        = f.m_origFaceIndex;
	m_index                = f.m_index;
	
	m_normal[0]            = f.m_normal[0];
	m_normal[1]            = f.m_normal[1];
	m_normal[2]            = f.m_normal[2];
	
	return *this;
}
