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
	o << pre << "   m_startChildIndex: " << m_startChildIndex << std::endl;
	o << pre << "   m_vertex[0]: " << m_vertex[0] << ", " << "m_vertex[1]: " << m_vertex[1] << std::endl;
	o << pre << "   m_face[0]: " <<  m_face[0] << ", " << " m_face[1]: " <<  m_face[1];
}

CSubdivEdge &CSubdivEdge::operator=(const CSubdivEdge &e)
{
	if ( &e == this ) {
		return *this;
	}
	
	m_value = e.m_value;
	m_type = e.m_type;
	m_startChildIndex = e.m_startChildIndex;
	
	m_vertex[0] = e.m_vertex[0];
	m_vertex[1] = e.m_vertex[1];
	m_face[0] = e.m_face[0];
	m_face[1] = e.m_face[1];
	
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
	m_incidentEdges = v.m_incidentEdges;
	m_incidentFaces = v.m_incidentFaces;
	return *this;
}

// ----------------------------------------------------------------------------

void CCatmullClarkSubdivision::subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) const
{
	// Insert the number of child faces == number of parents vertices
	// After the first subdivision step each face has 4 vertices, therefore 4 childs
	long nChilds = 0;
	for ( std::vector<CSubdivFace>::iterator faceIter = parent.faces().begin();
		  faceIter != parent.faces().end();
		  faceIter++ )
	{
		(*faceIter).startChildIndex(nChilds);
		nChilds += (*faceIter).nVertices(); // Number of childs (nChilds not set) = number of vertices
		(*faceIter).endChildIndex(nChilds);
	}
	
	// childFace(0)(0), childFace(0)(1), ... childFace(0)(n), ... childFace(faces-1)(m)
	child.faces().resize(nChilds);

	long faceIdx = 0;
	for ( std::vector<CSubdivFace>::iterator faceIter = parent.faces().begin();
		 faceIter != parent.faces().end();
		 faceIter++ )
	{
		for ( long i = 0; i < (*faceIter).nVertices(); ++i )  {
			child.faces()[faceIdx+i].startVertexIndex((faceIdx+i)*4);
			child.faces()[faceIdx+i].endVertexIndex((faceIdx+i)*4 + 4);
		}
		faceIdx = (*faceIter).endChildIndex();		
	}
	
	// Each child face has 4 vertices (and 4 edges)
	
	// vertexIdxFace(0)(0), vertexIdxFace(0)(1), ... vertexIdxFace(0)(3), ... vertexIdxFace(nChilds-1)(4)
	child.vertexIndices().resize(nChilds*4);
	child.edgeIndices().resize(nChilds*4);
	
	// inherited edge start - new mid edge vertex, new mid edge vertex - inherited edge end,
	// new mid edge vertex - new face center vertex (min face index), opt. new mid edge vertex - new face center vertex (max face index)
	// childEdge(0)(0), ... childEdge(0)(3 or 4), ..., childEdge(nOldEdges-1)(3 or 4)
	long edgesNChilds = 0;
	for ( std::vector<CSubdivEdge>::iterator edgeIter = parent.edges().begin();
		  edgeIter != parent.edges().end();
		  edgeIter++ ) 
	{
		(*edgeIter).startChildIndex(edgesNChilds);
		edgesNChilds += (*edgeIter).nChilds(); // 3 for boundary, 4 otherwise
	}
	child.edges().resize(edgesNChilds);
	
	// inherited vertices (1 per parent vertex), new edge vertices (1 per parents edge), new center vertices (1 per parents faces)
	child.vertices().resize(parent.vertices().size()+parent.edges().size()+parent.faces().size());
	
	// Copy the inherited vertices
	std::copy(parent.vertices().begin(), parent.vertices().end(), child.vertices().begin());

	// Fill in connectivity for the child edges
	long localEdgeIndex = 0, // Local edge Index of a face
	     edgeIndex = 0, // Index of the current parent edge in container
	     childEdgeIndex = 0, // first child of the current parent edge
	     localVertexIndex = 0; // 0 or 1, start and end vertec of the current parent edge
	for ( std::vector<CSubdivEdge>::iterator edgeIter = parent.edges().begin();
		 edgeIter != parent.edges().end();
		 edgeIter++, edgeIndex++ ) 
	{
		// The index of the first child edge, valid edges childEdgeIndex, childEdgeIndex+1, ..., childEdgeIndex+nChilds-1
		childEdgeIndex = (*edgeIter).startChildIndex();
		assert((long)child.edges().size() >= childEdgeIndex + (*edgeIter).nChilds());

		// edge start -> edge mid
		child.edges()[childEdgeIndex].setVertices((*edgeIter).vertex(0), static_cast<long>(parent.vertices().size()+edgeIndex));
		child.edges()[childEdgeIndex].type((*edgeIter).type());
		child.edges()[childEdgeIndex].value((*edgeIter).value());
		
		// edge mid -> edge start
		child.edges()[childEdgeIndex+1].setVertices(static_cast<long>(parent.vertices().size())+edgeIndex, (*edgeIter).vertex(1));
		child.edges()[childEdgeIndex+1].type((*edgeIter).type());
		child.edges()[childEdgeIndex+1].value((*edgeIter).value());

		// face having the edge: edge start -> edge end
		if ( (*edgeIter).face(0) >= 0 ) {
			localEdgeIndex = parent.faces()[(*edgeIter).face(0)].localIndex(parent.edgeIndices(), edgeIndex);
			assert(localEdgeIndex >= 0 && localEdgeIndex < parent.faces()[(*edgeIter).face(0)].nVertices());
			// edge start -> edge mid
			child.edges()[childEdgeIndex].insertFace((*edgeIter).vertex(0),
													 static_cast<long>(parent.vertices().size() + edgeIndex),
													 parent.faces()[(*edgeIter).face(1)].startChildIndex() + localEdgeIndex);
			// edge mid -> edge end
			child.edges()[childEdgeIndex+1].insertFace(static_cast<long>(parent.vertices().size() + edgeIndex),
													   (*edgeIter).vertex(1),
													   parent.faces()[(*edgeIter).face(0)].startChildIndex() + (localEdgeIndex+1) % parent.faces()[(*edgeIter).face(0)].nVertices());
		}
		
		// face having the edge: edge start <- edge end
		if ( (*edgeIter).face(1) >= 0 ) {
			localEdgeIndex = parent.faces()[(*edgeIter).face(1)].localIndex(parent.edgeIndices(), edgeIndex);
			assert(localEdgeIndex >= 0 && localEdgeIndex < parent.faces()[(*edgeIter).face(1)].nVertices());
			// edge start <- edge mid
			child.edges()[childEdgeIndex].insertFace(static_cast<long>(parent.vertices().size() + edgeIndex),
													 (*edgeIter).vertex(0),
													 parent.faces()[(*edgeIter).face(1)].startChildIndex()+localEdgeIndex);
			// edge mid <- edge end
			child.edges()[childEdgeIndex+1].insertFace((*edgeIter).vertex(1),
													   static_cast<long>(parent.vertices().size()+edgeIndex),
													   parent.faces()[(*edgeIter).face(1)].startChildIndex() + (localEdgeIndex+1) % parent.faces()[(*edgeIter).face(1)].nVertices());
		}

		// one or two edges: edge mid -> face center
		localVertexIndex = 0; // will be 0 and 1
		for ( int localFaceIdx = 0; localFaceIdx < 2; ++localFaceIdx ) {
			if ( (*edgeIter).face(localFaceIdx) >= 0 ) {
				localEdgeIndex = parent.faces()[(*edgeIter).face(localFaceIdx)].localIndex(parent.edgeIndices(), edgeIndex);
				assert(localEdgeIndex >= 0 && localEdgeIndex < parent.faces()[(*edgeIter).face(localFaceIdx)].nVertices());
				
				// mid -> center of face i
				child.edges()[childEdgeIndex + 2 + localVertexIndex].setVertices(
				    static_cast<long>(parent.vertices().size() + edgeIndex),
					static_cast<long>(parent.vertices().size() + parent.edges().size()) + (*edgeIter).face(localFaceIdx));
				// edge mid -> face center of face i's localEdgeIndex child
				child.edges()[childEdgeIndex + 2 + localVertexIndex].insertFace(
				    static_cast<long>(parent.vertices().size()+edgeIndex),
					static_cast<long>(parent.vertices().size()+parent.edges().size())+(*edgeIter).face(localFaceIdx),
					parent.faces()[(*edgeIter).face(localFaceIdx)].startChildIndex()+localEdgeIndex
				);
				// face center -> edge mid of face i's (localEdgeIndex+1)%nVertices child
				child.edges()[childEdgeIndex+2+localVertexIndex].insertFace(
				    static_cast<long>(parent.vertices().size()+parent.edges().size())+(*edgeIter).face(localFaceIdx),
				    static_cast<long>(parent.vertices().size()+edgeIndex),
					parent.faces()[(*edgeIter).face(localFaceIdx)].startChildIndex()+((localEdgeIndex+1) % parent.faces()[(*edgeIter).face(localFaceIdx)].nVertices())
				);
				localVertexIndex++;
			}
		}
	}

	// Insert child face data
	faceIdx = 0;
	for ( std::vector<CSubdivFace>::iterator faceIter = parent.faces().begin();
		 faceIter != parent.faces().end();
		 faceIter++, faceIdx++ )
	{
		assert((*faceIter).nVertices() > 1);
		if ( (*faceIter).nVertices() < 2 ) {
			continue;
		}
		
		long centerIdx = static_cast<long>(parent.vertices().size()+parent.edges().size())+faceIdx;
		if ( (*faceIter).nVertices() != 4) {
			// extraordinary, vertex numbers:
			/*  c0 e0 Cr e(n-1)
			 *  c1 e1 Cr e0
			 *  c2 e2 Cr e1   - Cr for centerIdx
			 *  cn en Cr e(n-2)
			 */
			long prev = (*faceIter).nVertices()-1;
			for ( long i = 0; i < (*faceIter).nVertices(); ++i ) {
				CSubdivEdge &ed0 = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+i]];
				CSubdivEdge &edp = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+prev]];
				long c0 = parent.vertexIndices()[(*faceIter).startVertexIndex()+i];
				long e0 = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+i];
				long ep = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+prev];
				long idx0 = child.faces()[(*faceIter).startChildIndex()+i].startVertexIndex();
				
				child.vertexIndices()[idx0+0] = c0;
				child.vertexIndices()[idx0+1] = e0;
				child.vertexIndices()[idx0+2] = centerIdx;
				child.vertexIndices()[idx0+3] = ep;

				child.edgeIndices()[idx0+0] = childMainEdge(ed0, c0);        // c0 - e0
				child.edgeIndices()[idx0+1] = childCenterEdge(ed0, faceIdx); // e0 - Cr
				child.edgeIndices()[idx0+2] = childCenterEdge(edp, faceIdx); // Cr - ep
				child.edgeIndices()[idx0+3] = childMainEdge(edp, c0);        // ep - c0

				prev = (prev+1)%(*faceIter).nVertices();
			}
		} else {
			// ordinary, vertex numbers:
			/*  c0  e0  Cr  e3
			 *  e0  c1  e1  Cr
			 *  Cr  e1  c2  e2
			 *  e3  Cr  e2  c3
			 */
			CSubdivEdge &ed0 = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+0]];
			CSubdivEdge &ed1 = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+1]];
			CSubdivEdge &ed2 = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+2]];
			CSubdivEdge &ed3 = parent.edges()[parent.edgeIndices()[(*faceIter).startVertexIndex()+3]];
			
			long c0 = parent.vertexIndices()[(*faceIter).startVertexIndex()+0];
			long c1 = parent.vertexIndices()[(*faceIter).startVertexIndex()+1];
			long c2 = parent.vertexIndices()[(*faceIter).startVertexIndex()+2];
			long c3 = parent.vertexIndices()[(*faceIter).startVertexIndex()+3];
			
			long e0 = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+0];
			long e1 = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+1];
			long e2 = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+2];
			long e3 = static_cast<long>(parent.vertices().size())+parent.edgeIndices()[(*faceIter).startVertexIndex()+3];

			long idx0 = child.faces()[(*faceIter).startChildIndex()+0].startVertexIndex();
			long idx1 = child.faces()[(*faceIter).startChildIndex()+1].startVertexIndex();
			long idx2 = child.faces()[(*faceIter).startChildIndex()+2].startVertexIndex();
			long idx3 = child.faces()[(*faceIter).startChildIndex()+3].startVertexIndex();
			
			
			// c0  e0  Cr  e3
			child.vertexIndices()[idx0+0] = c0;
			child.vertexIndices()[idx0+1] = e0;
			child.vertexIndices()[idx0+2] = centerIdx;
			child.vertexIndices()[idx0+3] = e3;

			child.edgeIndices()[idx0+0] = childMainEdge(ed0, c0);        // c0 - e0
			child.edgeIndices()[idx0+1] = childCenterEdge(ed0, faceIdx); // e0 - Cr
			child.edgeIndices()[idx0+2] = childCenterEdge(ed3, faceIdx); // Cr - e3
			child.edgeIndices()[idx0+3] = childMainEdge(ed3, c0);        // e3 - c0
						

			// e0  c1  e1  Cr
			child.vertexIndices()[idx1+0] = e0;
			child.vertexIndices()[idx1+1] = c1;
			child.vertexIndices()[idx1+2] = e1;
			child.vertexIndices()[idx1+3] = centerIdx;
			
			child.edgeIndices()[idx1+0] = childMainEdge(ed0, c1);        // e0 - c1
			child.edgeIndices()[idx1+1] = childMainEdge(ed1, c1);        // c1 - e1
			child.edgeIndices()[idx1+2] = childCenterEdge(ed1, faceIdx); // e1 - Cr
			child.edgeIndices()[idx1+3] = childCenterEdge(ed0, faceIdx); // Cr - e0


			// Cr  e1  c2  e2
			child.vertexIndices()[idx2+0] = centerIdx;
			child.vertexIndices()[idx2+1] = e1;
			child.vertexIndices()[idx2+2] = c2;
			child.vertexIndices()[idx2+3] = e2;
			
			child.edgeIndices()[idx2+0] = childCenterEdge(ed1, faceIdx); // Cr - e1
			child.edgeIndices()[idx2+1] = childMainEdge(ed1, c2);        // e1 - c2
			child.edgeIndices()[idx2+2] = childMainEdge(ed2, c2);        // c2 - e2
			child.edgeIndices()[idx2+3] = childCenterEdge(ed2, faceIdx); // e2 - Cr
			
			
			// e3  Cr  e2  c3
			child.vertexIndices()[idx3+0] = e3;
			child.vertexIndices()[idx3+1] = centerIdx;
			child.vertexIndices()[idx3+2] = e2;
			child.vertexIndices()[idx3+3] = c3;
			
			child.edgeIndices()[idx3+0] = childCenterEdge(ed3, faceIdx); // e3 - Cr
			child.edgeIndices()[idx3+1] = childCenterEdge(ed2, faceIdx); // Cr - e2
			child.edgeIndices()[idx3+2] = childMainEdge(ed2, c3);        // e2 - c3
			child.edgeIndices()[idx3+3] = childMainEdge(ed3, c3);        // c3 - e3
		}
	}

}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void CSubdivFace::insertHoleVal(long idx, const CRiHierarchicalSubdivisionMesh &obj)
{
	assert(obj.nArgs().size() == obj.tags().size()*3);
	RtInt intOffs=0;
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
	RtInt intOffs=0;
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

void CSubdivisionIndices::updateVertexData()
{
	// Insert vertices
	long edgeCnt = 0, faceCnt = 0;
	for ( std::vector<CSubdivVertex>::iterator vertIter = m_vertices.begin();
		 vertIter != m_vertices.end();
		 vertIter++)
	{
		(*vertIter).startFace(faceCnt);
		faceCnt += (*vertIter).incidentFaces();
		
		(*vertIter).startEdge(edgeCnt);
		edgeCnt += (*vertIter).incidentEdges();
	}
	m_incidentFaces.resize(faceCnt);
	m_incidentEdges.resize(edgeCnt);
	
	std::vector<long> cnt(m_vertices.size());
	cnt.assign(cnt.size(), 0);
	
	long faceIdx = 0;
	for ( std::vector<CSubdivFace>::const_iterator faceIter = m_faces.begin();
		 faceIter != m_faces.end();
		 faceIter++, faceIdx++)
	{
		for ( long vertIdx = (*faceIter).startVertexIndex(); vertIdx != (*faceIter).endVertexIndex(); vertIdx++ ) {
			CSubdivVertex &v = m_vertices[m_vertexIndices[vertIdx]];
			m_incidentFaces[v.startFace()+cnt[m_vertexIndices[vertIdx]]] = faceIdx;
			cnt[m_vertexIndices[vertIdx]]++;
		}
	}
	
	cnt.assign(cnt.size(), 0);

	long edgeIdx = 0;
	for ( std::vector<CSubdivEdge>::const_iterator edgeIter = m_edges.begin();
		 edgeIter != m_edges.end();
		 edgeIter++, edgeIdx++)
	{
		m_incidentEdges[(*edgeIter).vertex(0)+cnt[(*edgeIter).vertex(0)]] = edgeIdx;
		cnt[(*edgeIter).vertex(0)]++;
		m_incidentEdges[(*edgeIter).vertex(1)+cnt[(*edgeIter).vertex(1)]] = edgeIdx;
		cnt[(*edgeIter).vertex(1)]++;
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
			m_vertices[m_vertexIndices[vertIdx]].incIncidentFaces();
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
				m_vertices[m_vertexIndices[vertIdx]].incIncidentEdges();
				m_vertices[m_vertexIndices[m_faces[faceIdx].nextVertexIndex(vertIdx)]].incIncidentEdges();
				// Is edge a crease?
				m_edges[edgeIdx].insertCreaseVal(obj);
				edgeIdx++;
			}
		}
	}
	m_edgeIndices.resize(edgeIdx);
	assert(nVertsIdx == (long)obj.verts().size());
	
	updateVertexData();
}

void CSubdivisionIndices::subdivide(CSubdivisionIndices &aParent, const CSubdivisionStrategy &aStrategy,
						const CRiHierarchicalSubdivisionMesh &anObj)
{
	m_parent = &aParent;	
	aStrategy.subdivide(aParent, *this);
	updateVertexData();
}

// ----------------------------------------------------------------------------

void CSubdivisionHierarchieTesselator::subdivide(IndexType minDepth)
{
	m_indices.push_back(CSubdivisionIndices());
	CSubdivisionIndices &root = m_indices.back();
	root.initialize(m_obj);
	
	if ( m_obj.scheme() != RI_NULL ) {
		const CSubdivisionStrategy *strategy = strategies().findObj(m_obj.scheme());
		if ( strategy ) {
			CSubdivisionIndices &cur = root;
			IndexType depth = tmax(tessU(), tessV());
			if ( depth > 0 )
				depth = static_cast<long>(log2(static_cast<double>(depth)));
			depth = tmax(depth, minDepth);
			while ( depth ) {
				m_indices.push_back(CSubdivisionIndices());
				CSubdivisionIndices &child = m_indices.back();
				child.subdivide(cur, *strategy, m_obj);
				cur = child;
				depth--;
			}
			// Calculate parameters
			// ...
		} else {
			// unknown strategy
		}
	}
	
	// Extract faces
	// Calculate normals
}

CSurface *CSubdivisionHierarchieTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	// Not tested
	// subdivide(0);
	return 0;
}
