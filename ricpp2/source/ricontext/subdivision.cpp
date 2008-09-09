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
    o << "m_type: " << (m_type == FACE_FILLED ? "FACE_FILLED" : "FACE_HOLE") << std::endl;
	o << pre << "   m_startVertexIndex: " << m_startVertexIndex << ", " << "m_endVertexIndex: " << m_endVertexIndex << std::endl;
	o << "   m_parentFaceIndex: " << m_parentFaceIndex << ", ";
	o << pre << "   m_startChildIndex: " << m_startChildIndex << ", " << "m_endChildIndex: " << m_endChildIndex << std::endl;
}


CSubdivFace &CSubdivFace::operator=(const CSubdivFace &f) {
	if ( &f == this ) {
		return *this;
	}
	
	m_type             = f.m_type;
	m_startVertexIndex = f.m_startVertexIndex;
	m_endVertexIndex   = f.m_endVertexIndex;
	m_parentFaceIndex  = f.m_parentFaceIndex;
	m_startChildIndex   = f.m_startChildIndex;
	m_endChildIndex     = f.m_endChildIndex;
	
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
}


CSubdivVertex &CSubdivVertex::operator=(const CSubdivVertex &v)
{
	if ( &v == this )
		return *this;
	
	m_value = v.m_value;
	m_type  = v.m_type;
	
	return *this;
}


// ----------------------------------------------------------------------------

//	std::map<RtToken, CSubdivisionStrategy *> m_strategies;


// ----------------------------------------------------------------------------
void CCatmullClarkSubdivision::subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) const
{
	// The number of child faces == number of parents vertices
	long nChilds = 0;
	if ( parent.parent() == 0 ) {
		for ( std::vector<CSubdivFace>::const_iterator faceIter = parent.faces().begin();
			  faceIter != parent.faces().end();
			 faceIter++ )
		{
			nChilds += (*faceIter).nVertices();
		}
	} else {
		// After the first subdivision step each face has 4 vertices, therefore 4 childs
		nChilds = parent.faces().size() * 4;
	}
	
	child.faces().resize(nChilds); // childFace(0)(0), childFace(0)(1), ... childFace(0)(n), ... childFace(faces-1)(m)
	
	child.vertexIndices().resize(nChilds*4); // vertexIdxFace(0)(0), vertexIdxFace(0)(1), ... vertexIdxFace(0)(3), ... vertexIdxFace(nChilds-1)(4)
	child.edgeIndices().resize(nChilds*4); // edgeIdxFace(0)(0), edgeIdxFace(0)(1), ... edgeIdxFace(0)(3), ... edgeIdxFace(nChilds-1)(4)

	child.edges().resize(parent.edges().size() * 3); // inh. start - new edge vertex, new edge vertex - inh. end, new edge vertex - new center: childEdge(0)(0), childEdge(0)(1), childEdge(0)(3), ..., childEdge(nOldEdges-1)(3)
	child.vertices().resize(parent.vertices().size()+parent.edges().size()+parent.faces().size()); // inherited vertices, new edge vertices, new center vertices

	std::copy(parent.vertices().begin(), parent.vertices().end(), child.vertices().begin());
	
	long faceIdx = 0;
	for ( std::vector<CSubdivFace>::iterator faceIter = parent.faces().begin();
		 faceIter != parent.faces().end();
		 faceIter++ )
	{
		(*faceIter).startChildIndex(faceIdx);
		(*faceIter).endChildIndex(faceIdx+(*faceIter).nVertices());
		for ( long i = 0; i < (*faceIter).nVertices(); ++i )  {
			child.faces()[faceIdx+i].startVertexIndex((faceIdx+i)*4);
			child.faces()[faceIdx+i].endVertexIndex((faceIdx+i)*4 + 4);
		}
		if ( (*faceIter).nVertices() != 4 ) {
			// extraordinary, vertex numbers:
			/*  0 e0 c en
			 *  1 e1 c e0
			 *  2 e2 c e1
			 * ...
			 */
		} else {
			// ordinary, vertex numbers:
			/*  0   e0  c   e3
			 *  e0  1   e1  c
			 *  c   e1  2   e2
			 *  e3  c   e2  3
			 */
		}
											
		faceIdx = (*faceIter).endChildIndex();
	}
	
}

// ----------------------------------------------------------------------------
/*
 std::vector<CSubdivFace>   m_faces;
 std::vector<CSubdivEdge>   m_edges;
 std::vector<CSubdivVertex> m_vertices;
*/

struct SEdge {
	long m_start,
		 m_end;
	inline SEdge(long start=0, long end=0)
	{
		validateMinMax(start, end);
		m_start = start;
		m_end = end;
	}
};

inline static bool operator==(const SEdge &s1, const SEdge &s2)
{
	return s1.m_start == s2.m_start && s1.m_end == s2.m_end;
}

inline static bool operator<(const SEdge &s1, const SEdge &s2)
{
	return s1.m_start < s2.m_start || (s1.m_start == s2.m_start && s1.m_end < s2.m_end);
}

void CSubdivFace::insertHoleVal(long idx, const CRiHierarchicalSubdivisionMesh &obj)
{
	assert(obj.nArgs().size() == obj.tags().size()*3);
	RtInt intOffs=0,;
	for ( size_t i = 0; i < obj.tags().size(); ++i ) {
		if ( obj.tags()[i] == RI_HOLE ) {
			RtInt intVals = obj.nArgs()[i*3];
			if ( intVals > 0 ) {
				for ( RtInt intIdx = 0; intIdx < intVals; ++intIdx ) {
					if ( obj.intArgs()[intIdx] == idx ) {
						m_type = FACE_HOLE;
						return;
					}
				}
			}
		}
		intOffs += obj.nArgs()[i*3];
	}
}

void CSubdivEdge::insertCreaseVal(const CRiHierarchicalSubdivisionMesh &obj)
{
	assert(obj.nArgs().size() == obj.tags().size()*3);
	RtInt intOffs=0, floatOffs=0;
	for ( size_t i = 0; i < obj.tags().size(); ++i ) {
		if ( obj.tags()[i] == RI_CREASE ) {
			RtInt intVals = obj.nArgs()[i*3];
			RtInt floatVals = obj.nArgs()[i*3+1];
			if ( intVals > 0 ) {
				for ( RtInt intIdx = 0; intIdx < intVals-1; ++intIdx ) {
					if ( isEdge(obj.intArgs()[intIdx], obj.intArgs()[intIdx+1]) ) {
						m_value = obj.floatArgs()[floatVals];
						return;
					}
				}
			}
		}
		intOffs += obj.nArgs()[i*3];
		floatOffs += obj.nArgs()[i*3+1];
	}
}

void CSubdivVertex::insertCornerVal(long idx, const CRiHierarchicalSubdivisionMesh &obj)
{
	assert(obj.nArgs().size() == obj.tags().size()*3);
	RtInt intOffs=0, floatOffs=0;
	for ( size_t i = 0; i < obj.tags().size(); ++i ) {
		if ( obj.tags()[i] == RI_CORNER ) {
			RtInt intVals = obj.nArgs()[i*3];
			RtInt floatVals = obj.nArgs()[i*3+1];
			if ( intVals > 0 ) {
				for ( RtInt intIdx = 0; intIdx < intVals; ++intIdx ) {
					if ( obj.intArgs()[intIdx] == idx ) {
						m_value = obj.floatArgs()[floatVals];
						return;
					}
				}
			}
		}
		intOffs += obj.nArgs()[i*3];
		floatOffs += obj.nArgs()[i*3+1];
	}
}

void CSubdivisionIndices::insertBoundaryVal(const CRiHierarchicalSubdivisionMesh &obj)
{
	assert(obj.nArgs().size() == obj.tags().size()*3);
	RtInt intOffs=0,;
	for ( size_t i = 0; i < obj.tags().size(); ++i ) {
		if ( obj.tags()[i] == RI_INTERPOLATEBOUNDARY ) {
			RtInt intVals = obj.nArgs()[i*3];
			RtInt interpolate = 1;
			if ( intVals > 0 ) {
				interpolate = obj.intArgs()[0];
			}
			m_interpolateBoundary = interpolate;
			return;
		}
		intOffs += obj.nArgs()[i*3];
	}
}

void CSubdivisionIndices::initialize(const CRiHierarchicalSubdivisionMesh &obj)
{
	m_illTopology = false;

	if ( obj.nVerts().size() <= 0 )
		return;
	
	// Resize the containers
	m_vertexIndices.resize(obj.verts().size());
	std::copy(obj.verts().begin(), obj.verts().end(), m_vertexIndices.begin());
	m_edgeIndices.resize(obj.verts().size());
	m_edges.resize(obj.verts().size()); // upper bound
	m_vertices.resize(tmax(obj.verts().size(), &obj.verts()[0])+1);
	m_faces.resize(obj.nVerts().size());

	// Boundary interpolation
	insertBoundaryVal(obj);

	// Insert face type (hole)
	long faceIdx = 0;
	for ( faceIdx = 0; faceIdx < (long)m_faces.size(); ++faceIdx) {
		m_faces[faceIdx].insertHoleVal(faceIdx, obj);
	}

	// Insert vertex values (corner)
	long vertIdx = 0;
	for ( vertIdx = 0; vertIdx < (long)m_vertices.size(); ++vertIdx) {
		m_vertices[vertIdx].insertCornerVal(vertIdx, obj);
	}
	
	std::map<SEdge, long> edgeMap;
	std::map<SEdge, long>::iterator edgeMapIter;
	long edgeIdx = 0, nVertsIdx = 0;
	faceIdx = 0;
	for ( std::vector<RtInt>::const_iterator nVertsIter = obj.nVerts().begin();
		  nVertsIter != obj.nVerts().end();
		  nVertsIter++,  faceIdx++)
	{
		// Set vertex/edge indices
		m_faces[faceIdx].startVertexIndex(nVertsIdx);
		nVertsIdx += (*nVertsIter);
		m_faces[faceIdx].endVertexIndex(nVertsIdx);
		
		for( long vertIdx = m_faces[faceIdx].startVertexIndex();
			 vertIdx != m_faces[faceIdx].endVertexIndex();
			 ++vertIdx )
		{
			// Insert edge
			SEdge edge(m_vertexIndices[vertIdx],
					   m_vertexIndices[m_faces[faceIdx].nextVertexIndex(vertIdx)]);
			edgeMapIter = edgeMap.find(edge);
			if ( edgeMapIter != edgeMap.end() ) {
				long existingEdgeIdx = (*edgeMapIter).second;
				if ( !m_edges[existingEdgeIdx].insertFace(m_vertexIndices[vertIdx],
														  m_vertexIndices[m_faces[faceIdx].nextVertexIndex(vertIdx)],
														  faceIdx) )
				{
					// Illegal topology
					m_illTopology = true;
				}
			} else {
				edgeMap[edge] = edgeIdx;
				m_edges[edgeIdx] = CSubdivEdge(m_vertexIndices[vertIdx],
						           m_vertexIndices[m_faces[faceIdx].nextVertexIndex(vertIdx)],
				                   faceIdx);
				// Is edge a crease?
				m_edges[edgeIdx].insertCreaseVal(obj);
				edgeIdx++;
			}
		}
	}
	m_edgeIndices.resize(edgeIdx);
	assert(nVertsIdx == (long)obj.verts().size());
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

void CSubdivisionIndices::subdivide(CSubdivisionIndices &aParent, const CSubdivisionStrategy &aStrategy,
						const CRiHierarchicalSubdivisionMesh &anObj)
{
	m_parent = &aParent;	
	aStrategy.subdivide(aParent, *this);
}


void CSubdivisionHierarchieTesselator::subdivide(IndexType minDepth)
{
	m_indices.push_back(CSubdivisionIndices());
	CSubdivisionIndices &root = m_indices.back();
	root.initialize(m_obj);
	
	const CSubdivisionStrategy *strategy = strategies().findObj(m_obj.scheme());
	
	if ( strategy ) {
		CSubdivisionIndices &cur = root;
		IndexType depth = std::max(tessU(), tessV());
		depth = std::max(depth, minDepth);
		if ( depth > 0 )
			depth = static_cast<long>(log2(static_cast<double>(depth)));
		while ( depth ) {
			m_indices.push_back(CSubdivisionIndices());
			CSubdivisionIndices &child = m_indices.back();
			child.subdivide(cur, *strategy, m_obj);
			cur = child;
			depth--;
		}
	}
	
	// Extract faces
}

CSurface *CSubdivisionHierarchieTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	subdivide(0);
	return 0;
}
