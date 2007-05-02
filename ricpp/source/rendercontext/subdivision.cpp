// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2003, Andreas Pidde
//
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

/*! \file subdivison.h
 *  \brief Implementation of the classes for subdivision surfaces meshes.
 *
 *         s.a. Subdivision Surfaces in Character Animation
 *              by Tony DeRose, Michael Kass and Tien Truong
 *              and the RenderMan specification.
 *
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "subdivision.h"

#include <math.h>
#include <algorithm>


// ----------------------------------------------------------------------------
void TSubdivVertex::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "TSubdivVertex:" << std::endl;
	o << pre << "   m_value: " << m_value << ", " << "m_type: " << (m_type == VERTEX_ROUNDED ? "VERTEX_ROUNDED" : "VERTEX_CORNER") << std::endl;
	o << pre << "   m_idxStartEdge: " << m_idxStartEdge << ", " << "m_idxEndEdge: " << m_idxEndEdge << ", " << "m_idxEdgeCnt: " << m_idxEdgeCnt << std::endl;
	o << pre << "   m_idxStartFace: " << m_idxStartFace << ", " << "m_idxEndFace: " << m_idxEndFace << ", " << "m_idxFaceCnt: " << m_idxFaceCnt << std::endl;
}


// ----------------------------------------------------------------------------
TSubdivVertex &TSubdivVertex::operator=(const TSubdivVertex &v) {
	if ( &v == this )
		return *this;

	m_value = v.m_value;
	m_type  = v.m_type;

	m_idxStartEdge = v.m_idxStartEdge;
	m_idxEndEdge   = v.m_idxEndEdge;
	m_idxEdgeCnt   = v.m_idxEdgeCnt;

	m_idxStartFace = v.m_idxStartFace;
	m_idxEndFace   = v.m_idxEndFace;
	m_idxFaceCnt   = v.m_idxFaceCnt;

	return *this;
}


// ----------------------------------------------------------------------------
void TSubdivEdge::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "TSubdivEdge:" << std::endl;
	o << pre << "   m_value: " << m_value << ", " << "m_type: " << (m_type == EDGE_ROUNDED ? "EDGE_ROUNDED" : "EDGE_CREASE") << std::endl;
	o << pre << "   m_oneVertex: " << m_oneVertex << ", " << "m_otherVertex: " << m_otherVertex << std::endl;
	o << pre << "   m_oneFace: " << m_oneFace << ", " << "m_otherFace: " << m_otherFace;
}


// ----------------------------------------------------------------------------
TSubdivEdge &TSubdivEdge::operator=(const TSubdivEdge &e) {
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
void TSubdivFacet::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "TSubdivFacet:" << std::endl;
	o << pre;
	// o << "   m_value: " << m_value << ", ";
    o << "m_type: " << (m_type == FACE_FILLED ? "FACE_FILLED" : "FACE_HOLE") << std::endl;
	o << pre << "   m_startVertexEdgeIndex: " << m_startVertexEdgeIndex << ", " << "m_endVertexEdgeIndex: " << m_endVertexEdgeIndex << std::endl;
	o << pre << "   m_normal: [" << m_normal[0] << ", " << m_normal[1] << ", " << m_normal[2] << "]" << std::endl;
}


// ----------------------------------------------------------------------------
TSubdivFacet &TSubdivFacet::operator=(const TSubdivFacet &f) {
	if ( &f == this ) {
		return *this;
	}

	// m_value             = f.m_value;
	m_type                 = f.m_type;
	m_startVertexEdgeIndex = f.m_startVertexEdgeIndex;
	m_endVertexEdgeIndex   = f.m_endVertexEdgeIndex;
	m_faceIndex            = f.m_faceIndex;
	m_boundary             = f.m_boundary;

	m_normal[0]            = f.m_normal[0];
	m_normal[1]            = f.m_normal[1];
	m_normal[2]            = f.m_normal[2];

	return *this;
}


// ----------------------------------------------------------------------------
long TSubdivMesh::incidentBoundaryEdges(long vertexIdx) const {
	long i, j;
	long ret = 0;
	long start = m_vertices[vertexIdx].m_idxStartEdge;
	long end = m_vertices[vertexIdx].m_idxEndEdge;
	for ( i = start; i < end; ++i ) {
		j = m_idxIncidentEdges[i];
		if ( j >= 0 && m_edges[j].isBoundary() ) {
			++ret;
		}
	}
	return ret;
}


// ----------------------------------------------------------------------------
long TSubdivMesh::incidentSharpEdges(long vertexIdx) const {
	long i, j;
	long ret = 0;
	long start = m_vertices[vertexIdx].m_idxStartEdge;
	long end = m_vertices[vertexIdx].m_idxEndEdge;
	for ( i = start; i < end; ++i ) {
		j = m_idxIncidentEdges[i];
		if ( j >= 0 && m_edges[j].isSharp() ) {
			++ret;
		}
	}
	return ret;
}

// ----------------------------------------------------------------------------
void TSubdivMesh::dump(std::ostream &o, const std::string &pre) const {
	o << pre << "TSubdivMesh:" << std::endl;
	std::string pre2 = pre;
	pre2 += "   ";

	o << pre2 << "m_sumVerticesPerFace: " << m_sumVerticesPerFace << std::endl;
	o << pre2 << "m_vertices.size(): " << (long)m_vertices.size() << ", " << "m_edges.size(): " << (long)m_edges.size() << ", " << "m_facets.size(): " << (long)m_facets.size() << std::endl;
	o << pre2 << "m_idxFaceVertices.size(): " << (long)m_idxFaceVertices.size() << ", " << "m_idxFaceEdges.size(): " << (long)m_idxFaceEdges.size() << ", " << "m_idxAdjacentFacets.size(): " << (long)m_idxAdjacentFacets.size() << ", " << "m_idxIncidentEdges.size(): " << (long)m_idxIncidentEdges.size() << std::endl;
	o << std::endl;

	o << pre << "m_vertices:" << std::endl;
	::dumpLn(m_vertices, o, pre2);
	o << pre << "m_edges:" << std::endl;
	::dumpLn(m_edges, o, pre2);
	o << pre << "m_facets:" << std::endl;
	::dumpLn(m_facets, o, pre2);

	o << pre << "m_idxFaceVertices:" << std::endl;
	::dumpV(m_idxFaceVertices, o, pre2);
	o << pre << "m_idxFaceEdges:" << std::endl;
	::dumpV(m_idxFaceEdges, o, pre2);
	o << pre << "m_idxAdjacentFacets:" << std::endl;
	::dumpV(m_idxAdjacentFacets, o, pre2);
	o << pre << "m_idxIncidentEdges:" << std::endl;
	::dumpV(m_idxIncidentEdges, o, pre2);

	o << pre << "m_interVertexData:" << std::endl;
	::dumpLnV(m_interVertexData, o, pre2);

	o << pre << "m_varyingData:" << std::endl;
	::dumpLnV(m_varyingData, o, pre2);
}


// ----------------------------------------------------------------------------
TSubdivMesh &TSubdivMesh::operator=(const TSubdivMesh &m) {
	if ( &m == this ) {
		return *this;
	}

	m_vertices = m.m_vertices;
	m_edges = m.m_edges;
	m_facets = m.m_facets;
	m_sumVerticesPerFace = m.m_sumVerticesPerFace;

	m_idxFaceVertices = m.m_idxFaceVertices;
	m_idxFaceEdges = m.m_idxFaceEdges;
	m_idxAdjacentFacets = m.m_idxAdjacentFacets;
	m_idxIncidentEdges = m.m_idxIncidentEdges;

	m_interVertexData = m.m_interVertexData;
	m_varyingData = m.m_varyingData;

	m_primFaces = m.m_primFaces;
	// TPrimFace::operator=(*this);
	return *this;
}

// ----------------------------------------------------------------------------
long TSubdivMesh::unsharpBoundaryEdges(long faceIdx) {
	long start = m_facets[faceIdx].startVertexEdgeIndex();
	long end   = m_facets[faceIdx].endVertexEdgeIndex();
	long count = 0;

	while ( start < end ) {
		if ( m_edges[m_idxFaceEdges[start]].isUnsharpBoundary() )
			++count;
		++start;
	}

	return count;
}

// ----------------------------------------------------------------------------
void TSubdivMesh::initialize(
	RtInt nfaces, RtInt nverts[], RtInt verts[],
	RtInt ntags, RtToken tags[],
	RtInt nargs[], RtInt intargs[], RtFloat floargs[])
{
    // First part store the indices to get adjacent/incident information

	// The number of faces is given in nfaces
	m_facets.clear();
	m_facets.resize(nfaces);

	long i, j;
	long num_verts = 0L;     // Sum of the number of vertices in all faces (eq. number of adjacent faces of all vertices).
	RtInt max_vert_idx = -1; // Maximum index of the vertices, (max_vert_idx+1) number of different vertices needed.

	for ( i = 0; i < nfaces; ++i ) {
		for ( j = num_verts + nverts[i]; j > num_verts;  ) {
			--j;
			// Find the maximal index value
			if ( verts[j] > max_vert_idx )
				max_vert_idx = verts[j];
		}

		// The number of vertices and edges of a face already known,
		// start at current num_verts, end at num_verts += nverts[i]
		m_facets[i].startVertexEdgeIndex(num_verts);
		num_verts += nverts[i];
		m_facets[i].endVertexEdgeIndex(num_verts);
		m_facets[i].faceIndex(i);
	}

	m_sumVerticesPerFace = num_verts;

	// max_vert_idx, num_verts and
	// the m_facets m_startVertexEdgeIndex and
	// m_facets[i].m_endVertexEdgeIndex calculated

	// fill m_idxFaceVertices, a copy of verts, initialy the
	// vertices can be simply copied from the parameters
	// given for the subdivision mesh
	m_idxFaceVertices.clear();
	m_idxFaceVertices.resize(num_verts);
	for ( i=0; i<num_verts; ++i )
		m_idxFaceVertices[i] = verts[i];

	// Reserve space m_idxAdjacentFacets, adjacent faces of the vertices
	m_idxAdjacentFacets.clear();
	m_idxAdjacentFacets.resize(num_verts);

	// Reserve the maximal number of vertices for the edges
	m_edges.clear();
	m_edges.resize(num_verts);

	// reserve space for the vertices
	m_vertices.clear();
	m_vertices.resize(max_vert_idx+1);

	// Insert all possible edges (maybe twice) and
	// count the number of adjacent faces of the vertices
	RtInt v1, v2;
	RtInt counter = 0;
	RtInt prevj;
	for ( i = 0; i < nfaces; ++i ) {
		prevj = counter; // The loop is closed, the last and the first vertex are connected by an edge
		for ( j = nverts[i] + counter; j > counter; ) {
			--j;
			v1 = verts[j];
			v2 = verts[prevj];
			m_edges[j].newEdge(v1, v2);          // The vertices are ordered by their index
			prevj = j;
			++m_vertices[verts[j]].m_idxEndFace; // Count the number of adjacent faces, using m_idxEndFace
		}
		counter += nverts[i];
	}

	// Sort the edges by the vertices
	std::sort(m_edges.begin(), m_edges.end());

	// Remove double edges
	std::vector<TSubdivEdge>::iterator edgeIter = std::unique(m_edges.begin(), m_edges.end());
	m_edges.resize(edgeIter - m_edges.begin());

	// Count sum of the incident edges of all vertices, use
	// m_idxEndEdge temporarily to count
	long sumIncidentEdges = 0;
	// For all edges
	for ( edgeIter = m_edges.begin(); edgeIter != m_edges.end(); edgeIter++ ) {
		if ( edgeIter->m_oneVertex >= 0 ) {
			m_vertices[edgeIter->m_oneVertex].m_idxEndEdge++;
			++sumIncidentEdges;
		} else {
			// Error
			// An edge should be always between two edges
			// Since each edge is build using newEdge this should
			// never happen, providing all elements of vertes are >= 0
		}
		if ( edgeIter->m_otherVertex >= 0 ) {
			m_vertices[edgeIter->m_otherVertex].m_idxEndEdge++;
			++sumIncidentEdges;
		} else {
			// Error
			// An edge should be always between two edges
			// Since each edge is build using newEdge this should
			// never happen, providing all elements of vertes are >= 0
		}
	}

	// Reserve memory for all incident edge indices
	m_idxIncidentEdges.clear();
	m_idxIncidentEdges.resize(sumIncidentEdges);

	// Set the start index and end index of incident edges in vertices vector,
	// Initialize the m_idxEdgeCnt.
	long lastIncident = 0;
	std::vector<TSubdivVertex>::iterator vertexIter;
	for ( vertexIter = m_vertices.begin(); vertexIter != m_vertices.end(); vertexIter++ ) {
		vertexIter->m_idxEdgeCnt = vertexIter->m_idxStartEdge = lastIncident;
		lastIncident += vertexIter->m_idxEndEdge;
		vertexIter->m_idxEndEdge += vertexIter->m_idxStartEdge;
	}

	// Insert the edge numbers
	for ( i = 0; i < (long)m_edges.size(); ++i ) {
		if ( m_edges[i].m_oneVertex >= 0 ) {
			m_idxIncidentEdges[m_vertices[m_edges[i].m_oneVertex].m_idxEdgeCnt++] = i;
		}
		if ( m_edges[i].m_otherVertex >= 0 ) {
			m_idxIncidentEdges[m_vertices[m_edges[i].m_otherVertex].m_idxEdgeCnt++] = i;
		}
	}

	// Insert start-end for the adjacent faces of the vertices
	j = 0L;
	for ( i = 0; i < (long)m_vertices.size(); ++i ) {
		m_vertices[i].m_idxFaceCnt = m_vertices[i].m_idxStartFace = j;
		j += m_vertices[i].m_idxEndFace;
		m_vertices[i].m_idxEndFace += m_vertices[i].m_idxStartFace;
	}

	// Reserve space for the edges of all faces
	m_idxFaceEdges.clear();
	m_idxFaceEdges.resize(num_verts);

	// Insert the edges of the faces and the adjacent faces of the vertices.
	long e;
	counter = 0;
	for ( i = 0; i < nfaces; ++i ) {
		prevj = counter;
		for ( j = nverts[i] + counter; j > counter; ) {
			// Edges of the face
			--j;
			v1 = verts[j];
			v2 = verts[prevj];
			if ( getEdgeIndex(e, v1, v2) ) {
				if ( m_edges[e].insertFace(i) ) {
					m_idxFaceEdges[j] = e;
				}
			}
			prevj = j;

			// Adjacent faces of the vertex verts[j]
			m_idxAdjacentFacets[m_vertices[verts[j]].m_idxFaceCnt++] = i;
		}
		counter += nverts[i];
	}


	// Second part insert tag data

	int int_offset = 0, float_offset = 0;
	long k;
	
	for ( i = 0; i < ntags; ++i ) {

		if ( !strcmp(tags[i], TRi::RI_HOLE) ) {
			// Mark holes

			RtInt nHoles = nargs[i*2];
			if ( nHoles < 0 ) {
				// Error
				// Don't care
			} else {
				while ( nHoles-- > 0 ) {
					RtInt theHole = intargs[int_offset+nHoles];
					if ( theHole >= 0 && theHole < (int)m_facets.size() ) {
						m_facets[theHole].type(TSubdivFacet::FACE_HOLE);
					} else {
						// Error
						// Don't care
					}
				}
			}
			if ( nargs[i*2+1] != 0 ) {
				// Error
				// Don't care
			}

		} else if ( !strcmp(tags[i], TRi::RI_CREASE) ) {

			// Mark creases
			RtInt nCreases = nargs[i*2];
			if ( nCreases < 0 ) {
				// Error
				// Don't care
			} else {
				int nFloats = nargs[i*2+1];
				long jj;
				if ( nFloats < 0 ) {
					// Error
					// Don't care
				} else {
					RtFloat value = (RtFloat)1.0; // i.e. floargs[float_offset] == RI_INFINITY
					if ( nFloats >= 1 ) {
						value = floargs[float_offset];
						value += 1.0;
						if ( value == 0.0 ) value = 1.0;
						else value = (RtFloat)1.0/value;
						if ( value < 0.0 ) value = 0.0;
						if ( value > 1.0 ) value = 1.0;
						value = (RtFloat)1.0 - value;
						// if ( nFloats > 1 ) {
							// Error
							// Don't care
						// }
					}

					// Chain of edges that form a crease
					for ( j = 0; j < (nCreases - 1); ++j ) {
						v1 = intargs[int_offset+j];   // Endpoints of edges
						v2 = intargs[int_offset+j+1];
						long vertex;
						if ( v1 < 0  ) {
							// Error
							// Don't care
						} else {
							// Find the edge v1-v2
							for ( k = m_vertices[v1].m_idxStartEdge;
								  k < m_vertices[v1].m_idxEndEdge;
								  ++k )
							{
								jj = m_idxIncidentEdges[k];
								if ( m_edges[jj].getNeighbourVertex(vertex, v1) && v2 == vertex )
								{
									m_edges[jj].type(TSubdivEdge::EDGE_CREASE);
									m_edges[jj].value(value);
									// There can be only one edge v1-v2
									break;
								}
							}
							if ( k == m_vertices[v1].m_idxEndEdge ) {
								// Error edge not found
								// Don't care
							}
						}
					}
				}
			}

		} else if ( !strcmp(tags[i], TRi::RI_CORNER) ) {

			// Mark corners
			int nCorners = nargs[i*2];
			if ( nCorners < 0  ) {
				// Error
				// Don't care
			} else {
				int nFloats  = nargs[i*2+1];
				if ( nFloats < 0  ) {
					// Error
					// Don't care
				} else {
					bool floargPerCorner = nFloats >= nCorners;
					RtFloat value = (RtFloat)1.0; // i.e. floargs[float_offset] == RI_INFINITY
					if ( nFloats >= 1 ) {
						value = floargs[float_offset];
						value += 1.0;
						if ( value == 0.0 ) value = 1.0;
						else value = (RtFloat)1.0/value;
						if ( value < 0.0 ) value = 0.0;
						if ( value > 1.0 ) value = 1.0;
						value = (RtFloat)1.0 - value;
						// if ( nFloats > 1 && nFloats != nCorners ) {
							// Error
							// don't care
						// }
					}
					if ( nCorners >= 0 ) {
						while ( nCorners-- > 0 ) {
							int theCorner = intargs[int_offset+nCorners];
							if ( theCorner >= 0 ) {
								m_vertices[theCorner].type(TSubdivVertex::VERTEX_CORNER);
								if ( floargPerCorner ) {
									value = floargs[float_offset+nCorners];
									value += 1.0;
									if ( value == 0.0 ) value = 1.0;
									else value = (RtFloat)1.0/value;
									value = (RtFloat)1.0 - value;
									if ( value < 0.0 ) value = 0.0;
									if ( value > 1.0 ) value = 1.0;
								}
								m_vertices[theCorner].value(value);
							} else {
								// Error
								// Don't care
							}
						}
					} else {
						// Error
						// Don't care
					}
				}
			}

		} else if ( !strcmp(tags[i], TRi::RI_INTERPOLATEBOUNDARY) ) {

			if ( nargs[i*2] != 0 ) {
				// Error
				// Don't care
			}
			if ( nargs[i*2+1] != 0 ) {
				// Error
				// Don't care
			}
			for ( j=0; j<(RtInt)m_edges.size(); ++j ) {
				if ( m_edges[j].isBoundary() ) {
					m_edges[j].type(TSubdivEdge::EDGE_CREASE);
					m_edges[j].value(1.0);
				}
			}
			for ( j=0; j<(RtInt)m_vertices.size(); ++j ) {
				if ( incidentBoundaryEdges(j) > 1 ) {
					m_vertices[j].type(TSubdivVertex::VERTEX_CORNER);
					m_vertices[j].value(1.0);
				}
			}
		} else {
			// unknown
		}
		int_offset   += nargs[i*2];
		float_offset += nargs[i*2+1];
	}

	// Mark boundary faces for removal
	// markBoundaryFaces();
}


// ----------------------------------------------------------------------------
void TSubdivMesh::subdivideCatmullClarkConnectivity(const TSubdivMesh &aMesh)
{
	if ( &aMesh == this )
		return;

	// The sizes of the old mesh
	long sizeVertices = aMesh.m_vertices.size();
	long sizeEdges    = aMesh.m_edges.size();
	long sizeFaces    = aMesh.m_facets.size();

	// m_vertices [ number of old vertices + number of midpoints of old edges + number of centers of old faces ]
	m_vertices.clear();
	m_vertices.resize(sizeVertices + sizeEdges + sizeFaces);

	// m_edges    [ number of old edges * 2 + sum of number of vertices/edges of old faces ]
	m_edges.clear();
	m_edges.resize(sizeEdges * 2 + aMesh.m_sumVerticesPerFace);

	// m_facets    [ sum of number of vertices/edges of the old faces ]
	m_facets.clear();
	m_facets.resize(aMesh.m_sumVerticesPerFace);

	// There are always four vertices and four edges for each face after the first subdivision step is performed
	m_sumVerticesPerFace = m_facets.size() * 4;

	// Reserve space for the vertex indices per face
	m_idxFaceVertices.clear();
	m_idxFaceVertices.resize(m_sumVerticesPerFace);

	// Reserve space for the edge indices per face
	m_idxFaceEdges.clear();
	m_idxFaceEdges.resize(m_sumVerticesPerFace);

	// Fill the new faces - the new faces build of the old ones will be next to each other in m_facets,
	// therefore the original faces are queued up serially in m_facets
	long i, j, prevj, sum, sumVerts, vertIdx;

	for ( i = 0, sum = 0, sumVerts=0; i < (long)aMesh.m_facets.size(); ++i ) {
		// for each face i of the old subdivision mesh

		prevj = 0;
		for ( j = aMesh.m_facets[i].vertices();
			  j > 0;
			  ++sum
		    )
		{
			// for each vertex (i,j) of the old subdivision mesh that is no boundary face

			// 0 <= sum < m_facets.size()

			--j; // 0 <= j < Number of vertices of aMesh.m_facets[i]

			// Each new face will be given by a loop of four vertices and four edges after the first subdivision step
			m_facets[sum].startVertexEdgeIndex(sum*4);
			m_facets[sum].endVertexEdgeIndex(sum*4 + 4);

			// Four vertices and four edges: face center -> edge midpoint (i,prevj) -> vertex (i,j) -> edge midpoint (i,j) -> (face center)
			m_idxFaceVertices[sum*4  ] = sizeVertices + sizeEdges + i; // New center vertex
			m_idxFaceVertices[sum*4+1] = sizeVertices + aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex() + j]; // new (mid edge 1) vertex
			m_idxFaceVertices[sum*4+2] = aMesh.m_idxFaceVertices[aMesh.m_facets[i].startVertexEdgeIndex() + prevj]; // vertex j
			m_idxFaceVertices[sum*4+3] = sizeVertices + aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex() + prevj]; // new (mid edge 2) vertex
			
			// One more adjacent face at each vertex
			m_vertices[m_idxFaceVertices[sum*4  ]].m_idxEndFace++;
			m_vertices[m_idxFaceVertices[sum*4+1]].m_idxEndFace++;
			m_vertices[m_idxFaceVertices[sum*4+2]].m_idxEndFace++;
			m_vertices[m_idxFaceVertices[sum*4+3]].m_idxEndFace++;

			long idx0, idx1, add1, add2;

			vertIdx = m_idxFaceVertices[sum*4+2];
			idx0 = aMesh.m_edges[aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+j]].m_oneVertex;
			idx1 = aMesh.m_edges[aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+j]].m_otherVertex;
			if ( vertIdx == idx0 ) {
				if ( idx0 < idx1 ) {
					add1=0;
				} else {
					add1=1;
				}
			} else {
				if ( idx0 < idx1 ) {
					add1=1;
				} else {
					add1=0;
				}
			}

			idx0 = aMesh.m_edges[aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+prevj]].m_oneVertex;
			idx1 = aMesh.m_edges[aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+prevj]].m_otherVertex;
			if ( vertIdx == idx0 ) {
				if ( idx0 < idx1 ) {
					add2=0;
				} else {
					add2=1;
				}
			} else {
				if ( idx0 < idx1 ) {
					add2=1;
				} else {
					add2=0;
				}
			}

			// Edges
			m_idxFaceEdges[sum*4  ] = sizeEdges*2 + sumVerts + j; // center -> mid1
			m_idxFaceEdges[sum*4+1] = aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+j]*2+add1;   // mid1 -> vertex
			m_idxFaceEdges[sum*4+2] = aMesh.m_idxFaceEdges[aMesh.m_facets[i].startVertexEdgeIndex()+prevj]*2+add2; // vertex -> mid2
			m_idxFaceEdges[sum*4+3] = sizeEdges*2 + sumVerts + prevj;    // mid 2 -> center

			m_edges[m_idxFaceEdges[sum*4  ]].insertFace(sum);
			m_edges[m_idxFaceEdges[sum*4+1]].insertFace(sum);
			m_edges[m_idxFaceEdges[sum*4+2]].insertFace(sum);
			m_edges[m_idxFaceEdges[sum*4+3]].insertFace(sum);

			m_edges[m_idxFaceEdges[sum*4  ]].newEdge(m_idxFaceVertices[sum*4  ], m_idxFaceVertices[sum*4+1]);
			m_edges[m_idxFaceEdges[sum*4+1]].newEdge(m_idxFaceVertices[sum*4+1], m_idxFaceVertices[sum*4+2]);
			m_edges[m_idxFaceEdges[sum*4+2]].newEdge(m_idxFaceVertices[sum*4+2], m_idxFaceVertices[sum*4+3]);
			m_edges[m_idxFaceEdges[sum*4+3]].newEdge(m_idxFaceVertices[sum*4+3], m_idxFaceVertices[sum*4  ]);

			// type and value inherited from old face
			m_facets[sum].type(aMesh.m_facets[i].type());
			// m_facets[sum].value(aMesh.m_facets[i].value());
			m_facets[sum].faceIndex(aMesh.m_facets[i].faceIndex());
			m_facets[sum].boundary(aMesh.m_facets[i].boundary());


			prevj = j;
		}
		sumVerts += aMesh.m_facets[i].vertices();
	}

	// Copy type and value of old edges to new edges, the editional new
	// edges (from mid of old edge to center of old face) gets normal attributes
	for ( i = 0; i < (long)aMesh.m_edges.size(); ++i ) {
		m_edges[i*2].type(aMesh.m_edges[i].type());
		m_edges[i*2].value(aMesh.m_edges[i].value());
		m_edges[i*2+1].type(aMesh.m_edges[i].type());
		m_edges[i*2+1].value(aMesh.m_edges[i].value());
	}

	// Number of incidend edges per vertex
	for ( i = 0; i < (long)m_edges.size(); ++i ) {
		if ( m_edges[i].m_oneVertex >= 0 )
			m_vertices[m_edges[i].m_oneVertex].m_idxEndEdge++;
		if ( m_edges[i].m_otherVertex >= 0 )
			m_vertices[m_edges[i].m_otherVertex].m_idxEndEdge++;
	}
	
	long sumE = 0;
	long sumF = 0;
	for ( i = 0; i < (long)m_vertices.size(); ++i ) {
		m_vertices[i].m_idxEdgeCnt = m_vertices[i].m_idxStartEdge = sumE;
		sumE += m_vertices[i].m_idxEndEdge;
		m_vertices[i].m_idxEndEdge = sumE;

		m_vertices[i].m_idxFaceCnt = m_vertices[i].m_idxStartFace = sumF;
		sumF += m_vertices[i].m_idxEndFace;
		m_vertices[i].m_idxEndFace = sumF;
	}

	m_idxIncidentEdges.clear();
	m_idxIncidentEdges.resize(sumE);

	m_idxAdjacentFacets.clear();
	m_idxAdjacentFacets.resize(sumF);

	// Insert the incident edges of the vertices
	for ( i = 0; i < (long)m_edges.size(); ++i ) {
		if ( m_edges[i].m_oneVertex >= 0 )
			m_idxIncidentEdges[m_vertices[m_edges[i].m_oneVertex].m_idxEdgeCnt++] = i;
		if ( m_edges[i].m_otherVertex >= 0 )
			m_idxIncidentEdges[m_vertices[m_edges[i].m_otherVertex].m_idxEdgeCnt++] = i;
	}

	// Insert the adjacent faces of the vertices
	for ( i = 0; i < (long)m_facets.size(); ++i ) {
		for ( j = m_facets[i].startVertexEdgeIndex(); j < m_facets[i].endVertexEdgeIndex(); ++j ) {
			m_idxAdjacentFacets[m_vertices[m_idxFaceVertices[j]].m_idxFaceCnt++] = i;
		}
	}

	// Insert vertices values
	for ( i = 0; i < (long)aMesh.m_vertices.size(); ++i ) {
		m_vertices[i].value(aMesh.m_vertices[i].value());
		m_vertices[i].type(aMesh.m_vertices[i].type());
	}
}


// ----------------------------------------------------------------------------
void TSubdivMesh::subdivideCatmullClark(
	const TSubdivMesh &aMesh,
	const TNamedVector<RtFloat> &values)
{
	long components = values.components();
	long cardinality = values.cardinality();

	long elemSize = components*cardinality;

	// The sizes of the old mesh
	long sizeVertices = aMesh.m_vertices.size(); // number of vertices
	long sizeEdges    = aMesh.m_edges.size();    // number of edges
	long sizeFaces    = aMesh.m_facets.size();    // number of faces

	long offsVertices = 0;                                 // offset vertex points
	long offsEdges    = sizeVertices*elemSize;             // offset edge points
	long offsFaces    = (sizeVertices+sizeEdges)*elemSize; // offset face points

	// Vector of new vertices
	m_interVertexData.resize(m_interVertexData.size()+1);
	TNamedVector<RtFloat> &retvals = m_interVertexData.back();
	retvals.initialize(values.name(), components, cardinality, sizeVertices + sizeEdges + sizeFaces);

	// Facepoints, always the centeroid of the face
	RtFloat f;
	RtFloat *pvalues = values.begin();
	long i, j, k;
	long offs = offsFaces;
	for ( i = 0; i < sizeFaces; ++i, offs += elemSize ) {
		long nverts = aMesh.m_facets[i].vertices();
		for ( k = 0; k < elemSize; ++k ) {
			f = 0.0;
			for (
				j = aMesh.m_facets[i].startVertexEdgeIndex();
				j < aMesh.m_facets[i].endVertexEdgeIndex();
				++j )
			{
				f += pvalues[aMesh.m_idxFaceVertices[j]*elemSize+k];
			}
			retvals[offs+k] = f/nverts;
		}
	}

	// Edgepoints, smooth, sharp or blended
	offs = offsEdges;
	for ( i = 0; i < sizeEdges; ++i, offs += elemSize ) {
		if ( aMesh.m_edges[i].isSharp() ) {
			for ( k = 0; k < elemSize; ++k ) {
				// Sharp edge ( v >= TRi::RI_INFINITY )
				retvals[offs+k] =
					(values[aMesh.m_edges[i].m_oneVertex  *elemSize+k] +
					 values[aMesh.m_edges[i].m_otherVertex*elemSize+k]) /
					(RtFloat)2.0;

				RtFloat v = aMesh.m_edges[i].value(); // The sharpness

				// Blend infinitely sharp to smooth (stored in f)

				RtInt f1 = aMesh.m_edges[i].m_oneFace,
					  f2 = aMesh.m_edges[i].m_otherFace;

				if ( f2 >= 0 ) {
					// There are two adjacent faces
					f = ( values[aMesh.m_edges[i].m_oneVertex   * elemSize+k] +
						values[aMesh.m_edges[i].m_otherVertex * elemSize+k] +
						retvals[offsFaces + f1 * elemSize+k] +
						retvals[offsFaces + f2 * elemSize+k]
						) / (RtFloat)4.0;
				} else {
					// Boundary, there's only one adjacent face
					f = ( values[aMesh.m_edges[i].m_oneVertex   * elemSize+k] +
						values[aMesh.m_edges[i].m_otherVertex * elemSize+k] +
						retvals[offsFaces + f1 * elemSize+k]
						) / (RtFloat)3.0;
				}

				retvals[offs+k] = ((RtFloat)1.0-v)*f + v*retvals[offs+k];
			}

		} else if ( aMesh.m_edges[i].isBoundary() ) {

			// Boundary, there's only one adjacent face
			// RtInt f1 = aMesh.m_edges[i].m_oneFace;
			for ( k = 0; k < elemSize; ++k ) {
				retvals[offs + k] =
				(
					values[aMesh.m_edges[i].m_oneVertex   * elemSize+k] +
					values[aMesh.m_edges[i].m_otherVertex * elemSize+k]
				) / (RtFloat)2.0;
			}


		} else {

			// Smooth edge rule
			for ( k = 0; k < elemSize; ++k ) {
				retvals[offs + k] = (
					values[aMesh.m_edges[i].m_oneVertex   * elemSize+k] +
					values[aMesh.m_edges[i].m_otherVertex * elemSize+k] +
					retvals[offsFaces + aMesh.m_edges[i].m_oneFace   * elemSize+k] +
					retvals[offsFaces + aMesh.m_edges[i].m_otherFace * elemSize+k]
				) / (RtFloat)4.0;
			}

		}
	}

	// Vertexpoints
	offs = offsVertices;
	for ( i = 0; i < sizeVertices; ++i, offs += elemSize ) {
		long valence = aMesh.m_vertices[i].m_idxEndEdge - aMesh.m_vertices[i].m_idxStartEdge;
		RtFloat n = (RtFloat)valence;
		RtFloat n_2 = n*n;
		RtFloat n_2_n = (n-(RtFloat)2.0)/n;
		long nAdjacent = aMesh.m_vertices[i].m_idxEndFace - aMesh.m_vertices[i].m_idxStartFace;

		RtFloat v;
		RtInt count;
		long e, edgeIdx, faceIdx;
		RtFloat f, cf;

		for ( k = 0; k < elemSize; ++k ) {

			f = (RtFloat)0.0;
			for ( j=0; j<valence; ++j ) {
				edgeIdx = aMesh.m_vertices[i].m_idxStartEdge+j;
				if ( aMesh.m_edges[aMesh.m_idxIncidentEdges[edgeIdx]].getNeighbourVertex(e, i) ) {
					f += values[e*elemSize+k];
				}
			}
			for ( j=0; j<nAdjacent; ++j ) {
				faceIdx = aMesh.m_vertices[i].m_idxStartFace+j;
				f += retvals[offsFaces+aMesh.m_idxAdjacentFacets[faceIdx]*elemSize+k];
			}

			f /= n_2;
			f += values[i*elemSize+k]*n_2_n;

			if ( aMesh.incidentSharpEdges(i) == 2
				 &&
				 aMesh.m_vertices[i].type() != TSubdivVertex::VERTEX_CORNER )
			{
				count = 0;
				v = 0.0;
				cf = (RtFloat)6.0 * values[i*elemSize+k];
				for ( j = 0; j < valence; ++j ) {
					if ( aMesh.m_edges[aMesh.m_idxIncidentEdges[aMesh.m_vertices[i].m_idxStartEdge+j]].isSharp() ) {
						long idx = i;
						// bool b =
						aMesh.m_edges[aMesh.m_idxIncidentEdges[aMesh.m_vertices[i].m_idxStartEdge+j]].getNeighbourVertex(idx, i);
						// assert(b);
						cf += values[idx*elemSize+k];
						++count;
						v += aMesh.m_edges[aMesh.m_idxIncidentEdges[aMesh.m_vertices[i].m_idxStartEdge+j]].value();
					}
				}
				cf /= (RtFloat)8.0;

				// assert(count==2);
				if ( count > 0 ) {
					v /= (RtFloat)count;
				}

				retvals[offs+k] = ((RtFloat)1.0-v)*f + v*cf;
			} else if ( aMesh.incidentSharpEdges(i) > 2 || 
				        aMesh.m_vertices[i].type() == TSubdivVertex::VERTEX_CORNER )
			{
				if ( aMesh.m_vertices[i].type() != TSubdivVertex::VERTEX_CORNER )
				{
					// Sharp corner
					retvals[offs+k] = values[i*elemSize+k];
				} else {
					// Blend sharp corner to smooth corner
					RtFloat v = aMesh.m_vertices[i].value();
					// Blend sharp vertex and smooth corner
					retvals[offs+k] = ((RtFloat)1.0-v)*f + v*values[i*elemSize+k];
				}
			} else if ( aMesh.incidentBoundaryEdges(i) >= 1 ) {
				// Boundary
				count = 0;
				cf = (RtFloat)6.0 * values[i*elemSize+k];
				for ( j = 0; j < valence; ++j ) {
					if ( aMesh.m_edges[aMesh.m_idxIncidentEdges[aMesh.m_vertices[i].m_idxStartEdge+j]].isBoundary() ) {
						long idx = i;
						// bool b =
						aMesh.m_edges[aMesh.m_idxIncidentEdges[aMesh.m_vertices[i].m_idxStartEdge+j]].getNeighbourVertex(idx, i);
						// assert(b);
						cf += values[idx*elemSize+k];
						++count;
					}
				}
				cf /= (RtFloat)6.0+count;

				retvals[offs+k] = cf;
			} else {
				// Smooth corner
				retvals[offs+k] = f;
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TSubdivMesh::subdivideCatmullClarkLinear(
	const TSubdivMesh &aMesh,
	const TNamedVector<RtFloat> &values)
{
	long components = values.components();
	long cardinality = values.cardinality();

	long elemSize = components*cardinality;

	// The sizes of the old mesh
	long sizeVertices = aMesh.m_vertices.size();
	long sizeEdges    = aMesh.m_edges.size();
	long sizeFaces    = aMesh.m_facets.size();

	long offsVertices = 0;
	long offsEdges = sizeVertices*elemSize;
	long offsFaces = (sizeVertices+sizeEdges)*elemSize;

	// Vector of new vertices
	m_varyingData.resize(m_varyingData.size()+1);
	TNamedVector<RtFloat> &retvals = m_varyingData.back();
	retvals.initialize(values.name(), components, cardinality, sizeVertices + sizeEdges + sizeFaces);

	// Copy the vertices
	long i, k, offs;
	RtFloat f;

	offs = offsVertices;
	for ( i = 0; i < sizeVertices*elemSize; ++i ) {
		retvals[offs+i] = values[i];
	}

	// Get the edge values
	offs = offsEdges;
	for ( i = 0; i < sizeEdges; ++i ) {
		for ( k = 0; k < elemSize; ++k, ++offs ) {
			retvals[offs] =
				(values[aMesh.m_edges[i].m_oneVertex*elemSize+k] +
				values[aMesh.m_edges[i].m_otherVertex*elemSize+k]) /
				(RtFloat)2.0;
		}
	}

	// Get the face values
	offs = offsFaces;
	long j, nverts;
	for ( i = 0; i < sizeFaces; ++i ) {
		for ( k = 0; k < elemSize; ++k, ++offs ) {
			f = 0.0;
			nverts = aMesh.m_facets[i].vertices();
			for ( j = aMesh.m_facets[i].startVertexEdgeIndex(); j < aMesh.m_facets[i].endVertexEdgeIndex(); ++j ) {
				f += values[aMesh.m_idxFaceVertices[j]*elemSize+k];
			}
			retvals[offs] = f/(RtFloat)nverts;
		}
	}
}


// ----------------------------------------------------------------------------
bool TSubdivMesh::getEdgeIndex(long &e, long v1, long v2) const {
	long i, j, n;
	if ( v1 >= 0 ) {
		// Get the incident edges of a vertex
		long start = m_vertices[v1].m_idxStartEdge;
		long end = m_vertices[v1].m_idxEndEdge;
		// Search for the neighbour vertices
		for ( i = start; i < end; ++i ) {
			j = m_idxIncidentEdges[i];
			if ( j >= 0 && m_edges[j].getNeighbourVertex(n, v1) && n == v2 ) {
				e = m_idxIncidentEdges[i];
				// Edge is found
				return true;
			}
		}
	}
	// No edge found
	return false;
}


// ----------------------------------------------------------------------------
void TSubdivMesh::calcFaceNormals() {
	TNamedVector<RtFloat> *p = findElementVector(TRi::RI_P, m_interVertexData);
	if ( !p )
		return;
	RtPoint zero = {0.0, 0.0, 0.0}, n;
	RtFloat *v1, *v2, *v3;
	std::vector<TSubdivFacet>::iterator i = m_facets.begin();
	for ( ; i != m_facets.end(); i++ ) {
		if ( (*i).vertices() < 3 ) {
			(*i).normal(zero);
			continue;
		}
		long start=(*i).startVertexEdgeIndex();
		v1 = &(*p)[m_idxFaceVertices[start  ]*3];
		v2 = &(*p)[m_idxFaceVertices[start+1]*3];
		v3 = &(*p)[m_idxFaceVertices[start+2]*3];
		planeNorm(v3, v1, v2, &n[0]);
		(*i).normal(n);
	}
}


// ----------------------------------------------------------------------------
void TSubdivMesh::calcVertexNormals() {
	calcFaceNormals();

	if ( m_vertices.size() <= 0 ) {
		return;
	}

	m_vertexNormalData.initialize(TRi::RI_N, 3, 1, m_vertices.size());

	long j, k=0;
	std::vector<TSubdivVertex>::iterator i = m_vertices.begin();
	for ( ; i != m_vertices.end(); i++ ) {
		RtPoint n = {0.0, 0.0, 0.0};
		for ( j = (*i).m_idxStartFace; j < (*i).m_idxEndFace; ++j ) {
			const RtPoint &norm = m_facets[m_idxAdjacentFacets[j]].normal();
			n[0] += norm[0];
			n[1] += norm[1];
			n[2] += norm[2];
		}
		j -= (*i).m_idxStartFace;
		if ( j > 0 ) {
			n[0] /= j;
			n[1] /= j;
			n[2] /= j;
		}
		m_vertexNormalData[k++] = n[0];
		m_vertexNormalData[k++] = n[1];
		m_vertexNormalData[k++] = n[2];
	}
}


// ----------------------------------------------------------------------------
bool TSubdivMesh::initPrimFaces(RtInt nfaces, TSubdivisionMesh &prim) {
	if ( nfaces <= 0 ) {
		// No control hull
		return false;
	}

	// Already filled
	if ( m_primFaces.size() > 0 ) {
		return false;
	}

	// Interpolate the normals at the vertices using the adjacent face normals
	calcVertexNormals();

	// Iterator for the facets
	std::vector<TSubdivFacet>::iterator i;

	// Number of faces of the control hull
	m_primFaces.resize(nfaces);

	// Count the number of triangles used per face
	RtInt face;
	for ( i = m_facets.begin(); i != m_facets.end(); i++ ) {
		face = (*i).faceIndex();
		if ( face < nfaces ) {
			return false;
		}
		assert(face < nfaces);
		if (
			(*i).type() != TSubdivFacet::FACE_HOLE &&
			!(*i).boundary() &&
			(*i).vertices() > 2 
		) {
			// One more facet at the face
			m_primFaces[face].m_nFacets++;
			// The number of triangles is the number of vertices of the facet, subtracted by 2
			m_primFaces[face].m_nTriangles += ((*i).vertices()-2);
		}
	}

	// Iterate over the faces and fill in the common data
	for ( face = 0; face < nfaces; ++face ) {
		m_primFaces[face].m_geometry = &prim;
		m_primFaces[face].m_vertexIndices.resize(1);
		m_primFaces[face].m_vertexIndices[0].type(TGeometricIndices::triangles);
		m_primFaces[face].m_vertexIndices[0].m_indices.resize(m_primFaces[face].m_nTriangles * 3);
	}

	return true;
}


// ----------------------------------------------------------------------------
void TSubdivMesh::fillPrimFaces(std::vector< TSubdivPrimFace > &primFaces, RtInt nfaces, bool calcMissing) {
	if ( nfaces <= 0 ) {
		// No control hull
		return;
	}

	if ( interpolatedArrays() == 0 ) {
		// No values
		return;
	}


	// Vector to store vertex Indices of a face temporarily
	std::vector<long> vertexIndices;

	// The number of facets per face (like end() in iterators)
	std::vector<long> facetCnt;
	facetCnt.resize(nfaces);

	// The offsets to the facet index array facetIndices
	std::vector<long> facetOffsets;
	facetOffsets.resize(nfaces);

	// Iterator for the faces, store the offsets for the facet indices per face
	std::vector< TSubdivPrimFace >::iterator faceIter;
	long i = 0;
	long nfacets = 0;
	for ( faceIter = primFaces.begin(); faceIter != primFaces.end(); faceIter++, ++i ) {
		facetCnt[i] = nfacets;
		facetOffsets[i] = nfacets;
		nfacets += (*faceIter).m_nFacets;
	}

	// facet indices for the faces, ordered by face number
	std::vector<long> facetIndices;
	facetIndices.resize(nfacets);

	// Iterator for the facets, insert the facet indices
	std::vector<TSubdivFacet>::iterator facetIter;
	for ( i = 0, facetIter = m_facets.begin(); facetIter != m_facets.end(); facetIter++, ++i ) {
		facetIndices[facetCnt[(*facetIter).faceIndex()]] = i;
		++facetCnt[(*facetIter).faceIndex()];
	}

	// facetOffsets is now the start in facetIndices, facetCnt the end

	long j, face, facet2Idx, vertexCnt, vertexIndex;

	// Copy the values on a per face basis
	for ( face = 0; face < nfaces; ++face ) {

		// No facets
		if ( facetOffsets[face] == facetCnt[face] )
			continue;

		{
			// Facets are invisible
			TSubdivFacet &facet = m_facets[facetIndices[facetOffsets[face]]];
			if (
				facet.type() == TSubdivFacet::FACE_HOLE ||
				facet.boundary() ||
				facet.vertices() < 3
			) {
				continue;
			}
		}

		// Now get a map of the vertex indices to vertex indices stored per face,
		// will be stored in m_perFaceIdx

		// Mark all vertices (per face indices) of the face with -1
		for ( facet2Idx = facetOffsets[face]; facet2Idx != facetCnt[face]; ++facet2Idx ) {
			TSubdivFacet &facet = m_facets[facetIndices[facet2Idx]];
			for ( j = facet.startVertexEdgeIndex(); j != facet.endVertexEdgeIndex(); ++j ) {
				m_vertices[m_idxFaceVertices[j]].m_perFaceIdx = -1;
			}
		}

		// Give a new index mapping
		vertexCnt = 0;
		for ( facet2Idx = facetOffsets[face]; facet2Idx != facetCnt[face]; ++facet2Idx ) {
			TSubdivFacet &facet = m_facets[facetIndices[facet2Idx]];
			for ( j = facet.startVertexEdgeIndex(); j != facet.endVertexEdgeIndex(); ++j ) {
				if ( m_vertices[m_idxFaceVertices[j]].m_perFaceIdx == -1 ) {
					m_vertices[m_idxFaceVertices[j]].m_perFaceIdx = vertexCnt;
					++vertexCnt;
				}
			}
		}

		// vertexCnt is the number of indices needed for per face values

		// reserve memory for the new indicees
		vertexIndices.resize(0);
		vertexIndices.resize(vertexCnt);

		for ( facet2Idx = facetOffsets[face]; facet2Idx != facetCnt[face]; ++facet2Idx ) {
			TSubdivFacet &facet=m_facets[facetIndices[facet2Idx]];
			for ( j = facet.startVertexEdgeIndex(); j != facet.endVertexEdgeIndex(); ++j ) {
				// Map [per face index] to [mesh index]
				vertexIndices[m_vertices[m_idxFaceVertices[j]].m_perFaceIdx] = m_idxFaceVertices[j];
			}
		}

		TSubdivPrimFace &primFace = primFaces[face];

		// Copy all values to the primFace
		std::deque< TNamedVector<RtFloat> >::iterator valueIter;
		long valueStart = primFace.m_vertexData.size();
		primFace.m_vertexData.resize(valueStart+m_interVertexData.size()+m_varyingData.size());
		long dataIdx = valueStart;

		for ( valueIter = m_interVertexData.begin();
			  valueIter != m_interVertexData.end(); ++valueIter, ++dataIdx
			)
		{
			primFace.m_vertexData[dataIdx].initialize((*valueIter).name(), (*valueIter).components(), (*valueIter).cardinality(), (IndexType)vertexCnt);
			IndexType elements = (*valueIter).components() * (*valueIter).cardinality(), elemCnt;
			for ( vertexIndex=0; vertexIndex<vertexCnt; ++vertexIndex ) {
				for ( elemCnt = 0; elemCnt < elements; ++elemCnt ) {
					(primFace.m_vertexData[dataIdx])[vertexIndex*elements+elemCnt] = (*valueIter)[vertexIndices[vertexIndex]*elements+elemCnt];
				}
			}
		}

		for ( valueIter = m_varyingData.begin();
			  valueIter != m_varyingData.end(); ++valueIter, ++dataIdx
			)
		{
			primFace.m_vertexData[dataIdx].initialize((*valueIter).name(), (*valueIter).components(), (*valueIter).cardinality(), (IndexType)vertexCnt);
			IndexType elements = (*valueIter).components() * (*valueIter).cardinality(), elemCnt;
			for ( vertexIndex=0; vertexIndex<vertexCnt; ++vertexIndex ) {
				for ( elemCnt = 0; elemCnt < elements; ++elemCnt ) {
					(primFace.m_vertexData[dataIdx])[vertexIndex*elements+elemCnt] = (*valueIter)[vertexIndices[vertexIndex]*elements+elemCnt];
				}
			}
		}

		if ( calcMissing ) {
			dataIdx = primFace.m_vertexData.size();
			primFace.m_vertexData.resize(primFace.m_vertexData.size()+1);
			primFace.m_vertexData[dataIdx].initialize(TRi::RI_N, 3, 1, (IndexType)vertexCnt);
			IndexType elements = 3, elemCnt;
			for ( vertexIndex=0; vertexIndex<vertexCnt; ++vertexIndex ) {
				for ( elemCnt = 0; elemCnt < elements; ++elemCnt ) {
					(primFace.m_vertexData[dataIdx])[vertexIndex*elements+elemCnt] = m_vertexNormalData[vertexIndices[vertexIndex]*elements+elemCnt];
				}
			}

			primFace.m_freeNormalStrip = false;
			primFace.m_normalStrip = &primFace.m_vertexData[dataIdx];
			++dataIdx;

			primFace.m_freeSTStrip = false;
			primFace.m_stStrip = findElementVector(TRi::RI_ST, primFace.m_vertexData);

			if ( !primFace.m_stStrip ) {
				TNamedVector<RtFloat> *sStrip = findElementVector(TRi::RI_S, primFace.m_vertexData);
				TNamedVector<RtFloat> *tStrip = findElementVector(TRi::RI_T, primFace.m_vertexData);
				if ( sStrip && tStrip && sStrip->size() == tStrip->size() && sStrip->size() <= (unsigned long)vertexCnt ) {
					primFace.m_freeSTStrip = true;
					primFace.m_stStrip = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, vertexCnt);
					for ( vertexIndex=0; vertexIndex<vertexCnt; ++vertexIndex ) {
						(*primFace.m_stStrip)[2*vertexIndex] = (*sStrip)[vertexIndex];
						(*primFace.m_stStrip)[2*vertexIndex+1] = (*tStrip)[vertexIndex];
					}
				}
			}

			primFace.m_freeVertexStrip = false;
			primFace.m_vertexStrip = findElementVector(TRi::RI_P, primFace.m_vertexData);
		}

		long n = 0;
		for ( facet2Idx = facetOffsets[face]; facet2Idx != facetCnt[face]; ++facet2Idx ) {
			TSubdivFacet &facet = m_facets[facetIndices[facet2Idx]];
			// visible facet
			TVertexIndexType start = (TVertexIndexType)(facet.startVertexEdgeIndex());
			TVertexIndexType end   = (TVertexIndexType)(facet.endVertexEdgeIndex()-1);
			TVertexIndexType v3;
			for ( v3 = start+1; v3 != end; ++v3) {
				primFaces[face].m_vertexIndices[0].m_indices[n*3+2] = m_vertices[m_idxFaceVertices[start]].m_perFaceIdx;
				primFaces[face].m_vertexIndices[0].m_indices[n*3+1] = m_vertices[m_idxFaceVertices[v3]].m_perFaceIdx;
				primFaces[face].m_vertexIndices[0].m_indices[n*3+0] = m_vertices[m_idxFaceVertices[end]].m_perFaceIdx;
				++n;
				start = v3;
			}
		}
	}
}


// ----------------------------------------------------------------------------
bool TSubdivMesh::isSpecial(const char *valName) const {
	return
		!strcmp(valName, TRi::RI_P)  ||
		!strcmp(valName, TRi::RI_PW) ||
		!strcmp(valName, TRi::RI_ST) ||
		!strcmp(valName, TRi::RI_S)  ||
		!strcmp(valName, TRi::RI_T)  ||
		!strcmp(valName, TRi::RI_N);
}

// ----------------------------------------------------------------------------
void TSubdivMesh::fillPrimFaces(RtInt nfaces, TSubdivisionMesh &prim) {
	if ( nfaces <= 0 ) {
		return;
	}

	if ( m_primFaces.size() > 0 ) {
		return;
	}

	calcVertexNormals();

	std::vector<TSubdivFacet>::iterator i;
	/*
	for ( i = m_facets.begin(); i != m_facets.end(); i++ ) {
		if ( nfaces <= (*i).faceIndex() ) {
			nfaces = (*i).faceIndex() + 1;
		}
	}
	*/

	m_primFaces.resize(nfaces);

	RtInt face;
	for ( i = m_facets.begin(); i != m_facets.end(); i++ ) {
		face = (*i).faceIndex();
		if (
			(*i).type() != TSubdivFacet::FACE_HOLE &&
			!(*i).boundary() &&
			(*i).vertices() > 2 
		) {
			m_primFaces[face].m_nFacets++;
			m_primFaces[face].m_nTriangles += ((*i).vertices()-2);
		}
	}

	for ( face = 0; face < nfaces; ++face ) {
		m_primFaces[face].m_geometry = &prim;
		m_primFaces[face].m_freeNormalStrip = false;
		m_primFaces[face].m_normalStrip = &m_vertexNormalData;
		m_primFaces[face].m_freeVertexStrip = false;
		m_primFaces[face].m_vertexStrip = findElementVector(TRi::RI_P, m_interVertexData);
		if ( !m_primFaces[face].m_vertexStrip ) {
			TNamedVector<RtFloat> *pwStrip = findElementVector(TRi::RI_PW, m_interVertexData);
			if ( pwStrip ) {
				IndexType idx;
				IndexType sz=pwStrip->numElems();
				m_primFaces[face].m_freeVertexStrip = true;
				m_primFaces[face].m_vertexStrip = new TNamedVector<RtFloat>(TRi::RI_P, 3, 1, sz);
				for ( idx=0; idx<sz; ++idx ) {
					if ( (*pwStrip)[4*idx+3] != (RtFloat)0.0 ) {
						(*m_primFaces[face].m_vertexStrip)[3*idx+0] = (*pwStrip)[4*idx]/(*pwStrip)[4*idx+3];
						(*m_primFaces[face].m_vertexStrip)[3*idx+1] = (*pwStrip)[4*idx+1]/(*pwStrip)[4*idx+3];
						(*m_primFaces[face].m_vertexStrip)[3*idx+2] = (*pwStrip)[4*idx+2]/(*pwStrip)[4*idx+3];
					} else {
						(*m_primFaces[face].m_vertexStrip)[3*idx+0]   = (*pwStrip)[4*idx];
						(*m_primFaces[face].m_vertexStrip)[3*idx+1] = (*pwStrip)[4*idx+1];
						(*m_primFaces[face].m_vertexStrip)[3*idx+2] = (*pwStrip)[4*idx+2];
					}
				}
			}
		}
		m_primFaces[face].m_vertexIndices.resize(1);
		m_primFaces[face].m_vertexIndices[0].type(TGeometricIndices::triangles);
		m_primFaces[face].m_vertexIndices[0].m_indices.resize(m_primFaces[face].m_nTriangles * 3);
		m_primFaces[face].m_nTriangles = 0;

		m_primFaces[face].m_freeSTStrip = false;
		m_primFaces[face].m_stStrip = findElementVector(TRi::RI_ST, m_varyingData);

		if ( !m_primFaces[face].m_stStrip ) {
			TNamedVector<RtFloat> *sStrip = findElementVector(TRi::RI_S, m_varyingData);
			TNamedVector<RtFloat> *tStrip = findElementVector(TRi::RI_T, m_varyingData);

			if ( sStrip ) {
				IndexType idx;
				IndexType sz=sStrip->numElems();
				m_primFaces[face].m_freeSTStrip = true;
				m_primFaces[face].m_stStrip = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, sz);
				if ( m_primFaces[face].m_stStrip ) {
					for ( idx=0; idx<sz; ++idx ) {
						(*m_primFaces[face].m_stStrip)[2*idx] = (*sStrip)[idx];
					}
				}
			} else {
				if ( m_primFaces[face].m_vertexStrip ) {
					IndexType idx;
					IndexType sz=m_primFaces[face].m_vertexStrip->numElems();
					m_primFaces[face].m_freeSTStrip = true;
					m_primFaces[face].m_stStrip = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, sz);
					if ( m_primFaces[face].m_stStrip && sz == m_primFaces[face].m_stStrip->numElems() ) {
						for ( idx=0; idx<sz; ++idx ) {
							(*m_primFaces[face].m_stStrip)[2*idx] = (*m_primFaces[face].m_vertexStrip)[3*idx];
						}
					}
				}
			}

			if ( tStrip && m_primFaces[face].m_stStrip && m_primFaces[face].m_stStrip->numElems() == tStrip->numElems() ) {
				IndexType idx;
				IndexType sz=tStrip->numElems();
				for ( idx=0; idx<sz; ++idx ) {
					(*m_primFaces[face].m_stStrip)[2*idx+1] = (*tStrip)[idx];
				}
			} else {
				if ( m_primFaces[face].m_vertexStrip ) {
					IndexType idx;
					IndexType sz=m_primFaces[face].m_vertexStrip->numElems();
					if ( m_primFaces[face].m_stStrip && sz == m_primFaces[face].m_stStrip->numElems() ) {
						for ( idx=0; idx<sz; ++idx ) {
							(*m_primFaces[face].m_stStrip)[2*idx+1] = (*m_primFaces[face].m_vertexStrip)[3*idx+1];
						}
					}
				}
			}
		}

		std::deque< TNamedVector<RtFloat> >::iterator valueIter;
		long dataSize = m_primFaces[face].m_vertexData.size();
		long vertexSize = m_interVertexData.size();
		long varyingSize = m_varyingData.size();
		m_primFaces[face].m_vertexData.resize(dataSize+vertexSize+varyingSize);
		for ( valueIter = m_interVertexData.begin(); valueIter != m_interVertexData.end(); valueIter++, ++dataSize ) {
			if ( isSpecial((*valueIter).name()) )
				continue;
			m_primFaces[face].m_vertexData[dataSize].initialize((*valueIter).name(), (*valueIter).components(), (*valueIter).cardinality(), (*valueIter).size(), (*valueIter).begin());
		}
		for ( valueIter = m_varyingData.begin(); valueIter != m_varyingData.end(); valueIter++, ++dataSize ) {
			if ( isSpecial((*valueIter).name()) )
				continue;
			m_primFaces[face].m_vertexData[dataSize].initialize((*valueIter).name(), (*valueIter).components(), (*valueIter).cardinality(), (*valueIter).size(), (*valueIter).begin());
		}
	}

	for ( i = m_facets.begin(); i != m_facets.end(); i++ ) {
		face = (*i).faceIndex();
		if (
			(*i).type() != TSubdivFacet::FACE_HOLE &&
			!(*i).boundary() &&
			(*i).vertices() > 2
		) {
			TVertexIndexType start = (TVertexIndexType)((*i).startVertexEdgeIndex());
			TVertexIndexType end   = (TVertexIndexType)((*i).endVertexEdgeIndex()-1);
			TVertexIndexType v3;
			long n = m_primFaces[face].m_nTriangles;
			for ( v3 = start+1; v3 != end; ++v3) {
				m_primFaces[face].m_vertexIndices[0].m_indices[n*3+2] = m_idxFaceVertices[start];
				m_primFaces[face].m_vertexIndices[0].m_indices[n*3+1] = m_idxFaceVertices[v3];
				m_primFaces[face].m_vertexIndices[0].m_indices[n*3+0] = m_idxFaceVertices[end];
				++n;
				start = v3;
			}
			m_primFaces[face].m_nTriangles = n;
		}
	}
}


// ----------------------------------------------------------------------------
bool TSubdivisionMesh::hasFaceData() {
	RtInt size = m_parameters.getSize();
	RtInt i;
	m_hasFaceData = false;
	for ( i = 0; i < size; ++i ) {
		const TParameter *p = m_parameters.getParameterOf(i);
		if ( p ) {
			const TParameterDeclaration *pd = p->getDeclaration();
			
			if ( pd && pd->getBasicType() == TYPE_FLOAT ) {
				if ( pd->getClass() == CLASS_FACEVARYING || pd->getClass() == CLASS_FACEVERTEX ) {
					m_hasFaceData = true;
					return m_hasFaceData;
				}
			}
		}
	}
	return m_hasFaceData;
}

// ----------------------------------------------------------------------------
void TSubdivisionMesh::insertVertexData(long meshIdx, bool isPerFace) {
	if ( meshIdx >= (long)m_meshes.size() ) {
		// error
		return;
	}

	RtInt size = m_parameters.getSize();
	RtInt i;
	for ( i = 0; i < size; ++i ) {
		const TParameter *p = m_parameters.getParameterOf(i);
		if ( p ) {
			const TParameterDeclaration *pd = p->getDeclaration();
			
			if ( pd && pd->getBasicType() == TYPE_FLOAT ) {
				TNamedVector<RtFloat> nv(
					p->getParameterName(),
					pd->getComponents(p->getColorComps()), 
					pd->getCardinality(), p->getSize(),
					(RtFloat *)p->getValue());

				if ( pd->getClass() == CLASS_VARYING && !isPerFace ) {
					m_meshes[meshIdx].m_varyingData.push_back(nv);
				} else if ( pd->getClass() == CLASS_VERTEX && !isPerFace ) {
					m_meshes[meshIdx].m_interVertexData.push_back(nv);
				} else if ( pd->getClass() == CLASS_FACEVARYING && isPerFace ) {
					m_meshes[meshIdx].m_varyingData.push_back(nv);
				} else if ( pd->getClass() == CLASS_FACEVERTEX && isPerFace ) {
					m_meshes[meshIdx].m_interVertexData.push_back(nv);
				} else {
					// ignore
				}
			}
		}
	}
}


// ----------------------------------------------------------------------------
unsigned int TSubdivisionMesh::subdivDepth(RtFloat x, RtFloat y) const {
	RtFloat maxs = x > y ? x : y;
	if ( maxs <= 0 )
		return 0;
	return (unsigned int)(log(maxs)/log(2.0)+0.5);
}


// ----------------------------------------------------------------------------
TSubdivMesh &TSubdivisionMesh::subdivide(RtFloat x, RtFloat y) {
	unsigned int maxi = subdivDepth(x, y);
	unsigned int osize = m_meshes.size();

	if ( osize < 1 )
		osize = 0;

	if ( m_hasFaceData ) {
		assert(osize>1);

		if ( maxi*2+1 < osize ) {
			if ( m_meshes[maxi*2+1].m_primFaces.size() == 0 ) {
				if ( m_meshes[maxi*2+1].initPrimFaces(m_nfaces, *this) ) {
					m_meshes[maxi*2].fillPrimFaces(m_meshes[maxi*2+1].m_primFaces, m_nfaces, false);
					m_meshes[maxi*2+1].fillPrimFaces(m_meshes[maxi*2+1].m_primFaces, m_nfaces, true);
				}
			}
			return m_meshes[maxi*2+1];
		}

		for ( ; osize < (maxi+1)*2; osize++ ) {
			TSubdivMesh &prev = m_meshes[osize-2];
			m_meshes.resize(m_meshes.size()+1);
			TSubdivMesh &cur = m_meshes.back();

			if ( prev.interpolatedArrays() == 0 ) {
				continue;
			}

			if ( !strcmp(m_scheme.c_str(), TRi::RI_CATMULLCLARK) ) {
				if ( osize == 3 ) {
					prev.markBoundaryFaces();
				}
				cur.subdivideCatmullClarkConnectivity(prev);

				std::deque< TNamedVector<RtFloat> >::iterator iter = prev.m_interVertexData.begin();
				for ( ; iter != prev.m_interVertexData.end(); iter++ ) {
					cur.subdivideCatmullClark(prev, *iter);
				}

				iter = prev.m_varyingData.begin();
				for ( ; iter != prev.m_varyingData.end(); iter++ ) {
					cur.subdivideCatmullClarkLinear(prev, *iter);
				}
			}
		}

		if ( m_meshes[maxi*2+1].initPrimFaces(m_nfaces, *this) ) {
			m_meshes[maxi*2].fillPrimFaces(m_meshes[maxi*2+1].m_primFaces, m_nfaces, false);
			m_meshes[maxi*2+1].fillPrimFaces(m_meshes[maxi*2+1].m_primFaces, m_nfaces, true);
		}
		return m_meshes[maxi*2+1];

	} else {

		assert(osize>0);

		if ( maxi < osize ) {
			m_meshes[maxi].fillPrimFaces(m_nfaces, *this);
			return m_meshes[maxi];
		}

		for ( ; osize < maxi; ++osize ) {
			TSubdivMesh &prev = m_meshes.back();
			m_meshes.resize(m_meshes.size()+1);
			TSubdivMesh &cur = m_meshes.back();

			if ( !strcmp(m_scheme.c_str(), TRi::RI_CATMULLCLARK) ) {
				if ( osize == 1 ) {
					prev.markBoundaryFaces();
				}
				cur.subdivideCatmullClarkConnectivity(prev);
				std::deque< TNamedVector<RtFloat> >::iterator iter = prev.m_interVertexData.begin();
				for ( ; iter != prev.m_interVertexData.end(); iter++ ) {
					cur.subdivideCatmullClark(prev, *iter);
				}

				std::deque< TNamedVector<RtFloat> >::iterator iter2 = prev.m_varyingData.begin();
				for ( ; iter2 != prev.m_varyingData.end(); iter2++ ) {
					cur.subdivideCatmullClarkLinear(prev, *iter2);
				}
			}
		}

		m_meshes.back().fillPrimFaces(m_nfaces, *this);
		return m_meshes.back();
	}
}


// ----------------------------------------------------------------------------
void TSubdivisionMesh::dump(std::ostream &o, const std::string &pre) const {
	std::deque<TSubdivMesh>::const_iterator i = m_meshes.begin();
	std::string pre2 = pre;
	pre2 += "   ";
	o << pre << "TSubdivisionMesh:" << std::endl;
	for ( ; i < m_meshes.end(); i++ ) {
		i->dump(o, pre2);
		o << std::endl;
	}
}
