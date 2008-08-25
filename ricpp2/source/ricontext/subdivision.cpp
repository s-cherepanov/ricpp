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
	o << "   m_faceIndex: " << m_faceIndex << ", ";
	o << "   m_parentFaceIndex: " << m_parentFaceIndex << ", ";
	o << "   m_origFaceIndex: " << m_origFaceIndex << ", ";
    o << "m_type: " << (m_type == FACE_FILLED ? "FACE_FILLED" : "FACE_HOLE") << std::endl;
	o << pre << "   m_startVertexIndex: " << m_startVertexIndex << ", " << "m_endVertexIndex: " << m_endVertexIndex << std::endl;
	o << pre << "   m_startEdgeIndex: " << m_startEdgeIndex << ", " << "m_endEdgeIndex: " << m_endEdgeIndex << std::endl;
}


CSubdivFace &CSubdivFace::operator=(const CSubdivFace &f) {
	if ( &f == this ) {
		return *this;
	}
	
	m_type             = f.m_type;
	m_startVertexIndex = f.m_startVertexIndex;
	m_endVertexIndex   = f.m_endVertexIndex;
	m_startEdgeIndex   = f.m_startEdgeIndex;
	m_endEdgeIndex     = f.m_endEdgeIndex;
	m_faceIndex        = f.m_faceIndex;
	m_parentFaceIndex  = f.m_parentFaceIndex;
	m_origFaceIndex    = f.m_origFaceIndex;
	
	return *this;
}

// ----------------------------------------------------------------------------
void CSubdivEdge::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "CSubdivEdge:" << std::endl;
	o << pre << "   m_value: " << m_value << ", " << "m_type: " << (m_type == EDGE_ROUNDED ? "EDGE_ROUNDED" : "EDGE_CREASE") << std::endl;
	o << pre << "   m_oneVertex: " << m_oneVertex << ", " << "m_otherVertex: " << m_otherVertex << std::endl;
	o << pre << "   m_oneFace: " << m_oneFace << ", " << "m_otherFace: " << m_otherFace;
}


CSubdivEdge &CSubdivEdge::operator=(const CSubdivEdge &e)
{
	if ( &e == this ) {
		return *this;
	}
	
	m_value = e.m_value;
	m_type = e.m_type;
	
	m_oneVertex = e.m_oneVertex;
	m_otherVertex = e.m_otherVertex;
	m_oneFace = e.m_oneFace;
	m_otherFace = e.m_otherFace;
	
	return *this;
}


// ----------------------------------------------------------------------------
void CSubdivVertex::dump(std::ostream &o, const std::string &pre) const
{
	o << pre << "CSubdivVertex:" << std::endl;
	o << pre << "   m_value: " << m_value << ", " << "m_type: " << (m_type == VERTEX_ROUNDED ? "VERTEX_ROUNDED" : "VERTEX_CORNER") << std::endl;
	o << pre << "   m_idxStartEdge: " << m_idxStartEdge << ", " << "m_idxEndEdge: " << m_idxEndEdge << std::endl;
	o << pre << "   m_idxStartFace: " << m_idxStartFace << ", " << "m_idxEndFace: " << m_idxEndFace << std::endl;
}


CSubdivVertex &CSubdivVertex::operator=(const CSubdivVertex &v)
{
	if ( &v == this )
		return *this;
	
	m_value = v.m_value;
	m_type  = v.m_type;
	
	m_idxStartEdge = v.m_idxStartEdge;
	m_idxEndEdge   = v.m_idxEndEdge;
	
	m_idxStartFace = v.m_idxStartFace;
	m_idxEndFace   = v.m_idxEndFace;
	
	return *this;
}


// ----------------------------------------------------------------------------

//	std::map<RtToken, CSubdivisionStrategy *> m_strategies;

CSubdivisionStrategy *CSubdivisionStrategyFactory::registerStrategy(RtToken strategyName, CSubdivisionStrategy *aStrategy)
{
	return 0;
}

CSubdivisionStrategy *CSubdivisionStrategyFactory::unregisterStrategy(RtToken strategyName)
{
	return 0;
}

CSubdivisionStrategy *CSubdivisionStrategyFactory::getStrategy(RtToken strategyName)
{
	return 0;
}


// ----------------------------------------------------------------------------
void CCatmullClarkSubdivision::subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child)
{
}

// ----------------------------------------------------------------------------
/*
 std::vector<CSubdivFace>   m_faces;
 std::vector<CSubdivEdge>   m_edges;
 std::vector<CSubdivVertex> m_vertices;
*/

void CSubdivisionIndices::initialize(const std::vector<RtInt> &nverts, const std::vector<RtInt> &verts,
									 RtInt ntags, const std::vector<RtToken> &tags,
									 const std::vector<RtInt> &nargs,
									 const std::vector<RtInt> &intargs,
									 const std::vector<RtFloat> &floargs,
									 const std::vector<RtToken> &stringargs)
{
}

CSubdivFace *CSubdivisionIndices::face(long faceIndex)
{
	return 0;
}

CSubdivEdge *CSubdivisionIndices::edge(long faceIndex, long edgeIndex)
{
	return 0;
}

CSubdivVertex *CSubdivisionIndices::vertex(long faceIndex, long vertexIndex)
{
	return 0;
}

void CSubdivisionIndices::subdivide(CSubdivisionIndices &parent, const CSubdivisionStrategy &strategy)
{
}


void CSubdivisionHierarchie::subdivide(long minDepth, const CSubdivisionStrategy &strategy)
{
}

CSubdivFace *CSubdivisionHierarchie::face(long size, long *faceIndices)
{
	return 0;
}

CSubdivEdge *CSubdivisionHierarchie::edge(long size, long *faceIndices, long edgeIndex)
{
	return 0;
}

CSubdivVertex *CSubdivisionHierarchie::vertex(long size, long *faceIndices, long vertexIndex)
{
	return 0;
}


CSurface *CSubdivisionHierarchie::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	return 0;
}
