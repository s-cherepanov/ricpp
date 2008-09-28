#ifndef _RICPP_RICONTEXT_SUBDIVISION_H
#define _RICPP_RICONTEXT_SUBDIVISION_H

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

/** @file subdivision.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the classes for catmull-clark subdivision surfaces meshes.
 *
 *  See also "Subdivision Surfaces in Character Animation"
 *  by Tony DeRose, Michael Kass and Tien Truong
 *  and the RenderMan specification.
 */


#ifndef _RICPP_RIBASE_RICPPCONST_H
#include "ricpp/ribase/ricppconst.h"
#endif /* _RICPP_RIBASE_RICPPCONST_H */

#ifndef _RICPP_DECLARATION_DECLARATION_H
#include "ricpp/declaration/declaration.h"
#endif /* _RICPP_DECLARATION_DECLARATION_H */

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "ricpp/tools/objptrregistry.h"
#endif /* _RICPP_TOOLS_OBJPTRREGISTRY_H */

#include <iostream>
#include <vector>
#include <list>

namespace RiCPP {
	class CSubdivEdge;
	class CRiHierarchicalSubdivisionMesh;

	//! @brief Face for subdivision.
	/*! The face can be filled or be a hole (i.e. it is calculated as normal, but
	 *  the area is not filled at output time). A face is given by a sequence of vertices
	 *  grouped around the face. The vertices are connected by edges by a counter clockwise ordering.
	 */
	class CSubdivFace {
	public:
		//! The type of a surface (filled or hole).
		enum EnumSubdivFaceType {
			FACE_FILLED,  //!< Filled face.
			FACE_HOLE     //!< Face is a hole.
		};
		
	private:
		EnumSubdivFaceType m_type;   //!< Face type: FACE_FILLED or FACE_HOLE, FACE_FILLED is the default value.
		
		/** @{
		 * @name Conectivity
		 * @brief Incident vertices and edges (for each vertex one edge).
		 */
		long m_startVertexIndex; //!< Start index of incident vertices (and edges) in the mesh, normally cw, -1 if not initialised
		long m_endVertexIndex;   //!< Endindex+1 of incident vertices (and edges) in the mesh (-1 if not initialised), m_endVertexIndex - m_startVertexIndex = number of vertices incident to the face.
		/** @}
		 */
		
		/** @{
		 * @name Hierarchy
		 */
		long m_startChildIndex; //!< Start index of child faces, -1 if not initialized (leaf)
		long m_endChildIndex;   //!< Endindex+1 of of child faces, -1 if not initialized (leaf)
		/** @}
		 */
	public:		
		//! Standard constructor, initializes all elements.
		inline CSubdivFace() :
		m_type(FACE_FILLED),
		m_startVertexIndex(-1L),
		m_endVertexIndex(-1L),
		m_startChildIndex(-1L),
		m_endChildIndex(-1L)
		{
		}
		
		//! Copy constructor, initializes with the values of face.
		/*! \param aFace Face used for initialization.
		 */
		inline CSubdivFace(const CSubdivFace &aFace)
		{
			*this = aFace;
		}
		
		// Destructor, nothing to do - standard is used.
		// inline ~CSubdivFace() {}
		
		//! Prints the contents of the instance to a stream.
		/*! \param o Stream to print on.
		 *  \param pre String used as first output for each line.
		 */
		void dump(std::ostream &o, const std::string &pre) const;
		
		//! Gets a new instance as clone of *this.
		/*! \return A clone of *this.
		 */
		inline CSubdivFace *duplicate() const
		{
			return new CSubdivFace(*this);
		}
		
		//! Assignment operator, assigns all values of f to *this.
		/*! \param f Values of this face are assigned to *this.
		 *  \return A reference of the instance (*this).
		 */
		CSubdivFace &operator=(const CSubdivFace &e);
		
		//! Sets the face type, a hole or filled face.
		/*! \param aType New face type.
		 */
		inline void type(EnumSubdivFaceType aType)
		{
			m_type = aType;
		}
		
		//! Gets the face type.
		/*! \return The face type, a filled face or a hole.
		 */
		inline EnumSubdivFaceType type() const
		{
			return m_type;
		}
		
		//! Sets the start index of the incident vertices (and edges) of the face.
		/*! \param sVertexIndex The new start index for a ccw vertex (index) loop,
		 *         \sa vertexIndex(), -1 if it was not set.
		 */
		inline void startVertexIndex(long aVertexIndex)
		{
			m_startVertexIndex = aVertexIndex;
		}
		
		//! Gets the start index of the incident vertices (and edges) of the face.
		/*! \return The start index of the vertices (ccw vertex indices) of
		 *          the face, -1 if it was not set.
		 */
		inline long startVertexIndex() const
		{
			return m_startVertexIndex;
		}
		
		
		//! Sets the end index (+1) of the vertices (and edges) of the face.
		/*! \param aVertexAndEdgeIndex The new end index (+1) for a ccw vertex (index) loop,
		 *         \sa startVertexIndex()
		 */
		inline void endVertexIndex(long aVertexIndex)
		{
			m_endVertexIndex = aVertexIndex;
		}
		
		//! Gets the end index (+1) of the vertices (and edges) of the face
		/*! \return The end index (+1) of the vertices (ccw vertex indices) of
		 *          the face, -1 if it was not set.,
		 *          \sa startVertexIndex()
		 */
		inline long endVertexIndex() const
		{
			return m_endVertexIndex;
		}
		
		inline long nextVertexIndex(long anIndex) const
		{
			assert(startVertexIndex() <= anIndex && endVertexIndex() > anIndex);
			return anIndex >= (endVertexIndex() - 1) ? startVertexIndex() : anIndex + 1;
		}

		inline long prevVertexIndex(long anIndex) const
		{
			assert(startVertexIndex() <= anIndex && endVertexIndex() > anIndex);
			return anIndex <= startVertexIndex() ? endVertexIndex() - 1 : anIndex - 1;
		}

		/*! \return The number of vertices and edges grouped around the face. After the initial
		 *          subdivision, this should be always 4 for catmull-clark subdivision
		 *          meshes.
		 */
		inline long nVertices() const
		{
			return m_endVertexIndex - m_startVertexIndex;
		}
		
		inline void startChildIndex(long aStartChildIndex)
		{
			m_startChildIndex = aStartChildIndex;
		}

		inline long startChildIndex() const
		{
			return m_startChildIndex;
		}
		
		inline void endChildIndex(long anEndChildIndex)
		{
			m_endChildIndex = anEndChildIndex;
		}

		inline long endChildIndex() const
		{
			return m_endChildIndex;
		}

		inline long nChilds() const
		{
			return m_endChildIndex - m_startChildIndex;
		}

		inline long localIndex(const std::vector<long> &edgeOrVertexIndices, long anIncidentEdgeOrVertex) const
		{
			for ( long i = startVertexIndex(); i != endVertexIndex(); i++ ) {
				if ( edgeOrVertexIndices[i] == anIncidentEdgeOrVertex )
					return i-startVertexIndex();
			}
			return -1;
		}
		
		void insertHoleVal(long idx, const CRiHierarchicalSubdivisionMesh &obj);
		long triangulate(long *triangleIndices, long size, long offs) const;
	}; // CSubdivFace
	
	
	//! Prints the contents of the CSubdivFace f to a stream by calling its dump member function.
	/*! \param f CSubdivFace to print to a stream.
	 *  \param o Stream to print on.
	 *  \param pre String used at the begining for each line, usually blanks as indent.
	 */
	inline void dump(const CSubdivFace &f, std::ostream &o, const std::string &pre)
	{
		f.dump(o, pre);
	}
	
	//! Edge for subdivision.
	/*! Each edge has two incident vertices and one or two incident faces (2-manifold). Edges can
	 *  be ordered by the index numbers of their vertices. The orientation is
	 *  given by the ordering of the vertices per incident face - the edge itself has no
	 *  orientation. The ordering of the vertex index numbers are stored in an CSubdivEdge instance,
	 *  the condition m_oneVertex < m_otherVertex is always true and is used by searching.
	 *  The ordering of m_oneFace and m_otherFace is arbitrary and of no use. If there is only
	 *  one incident face at a boundary, m_oneFace contains the face index and m_otherFace equals -1.
	 *  The index numbers are those of the appropriate containers used in the mesh.
	 */
	class CSubdivEdge {
	public:
		//! The type of an edge (rounded or crease).
		enum EnumSubdivEdgeType {
			EDGE_ROUNDED, //!< Edge is rounded.
			EDGE_CREASE   //!< Edge is a crease.
		};
		
	private:
		static const RtFloat ms_sharpVal;
		RtFloat	m_value;           //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, gives the sharpness of a crease. The value blends from 0: smooth to 1: sharp.
		EnumSubdivEdgeType m_type; //!< Edge type: EDGE_ROUNDED or EDGE_CREASE.
		
		long m_startChildIndex;  //!< Index of the child edge (boundary edge has 3 childs, all other 4)
		long m_vertex[2];   /**< Incident vertices of the edge, only on initialization: m_vertex[0] < m_vertex[1],
							 */
		long m_face[2];     /**< Index of an incident faces (m_face[0] has the edge: m_vertex[0]->m_vertex[1], the other has m_vertex[1]->m_vertex[0]).
		                     * Boundaries have one index -1
		                     */
	public:
		//! Standard constructor, initializes all elements.
		inline CSubdivEdge() :
		m_value(0.0), m_type(EDGE_ROUNDED),
		m_startChildIndex(-1)
		{
			m_vertex[0] = m_vertex[1] = -1L;
			m_face[0]   = m_face[1] = -1L;
		}
		
		inline CSubdivEdge(long aStartIdx, long anEndIdx, long incidentFace) :
		m_value(0.0), m_type(EDGE_ROUNDED)
		{
			m_face[0] = incidentFace;
			m_face[1] = -1;
			m_vertex[0] = aStartIdx;
			m_vertex[1] = anEndIdx;
		}

		//! Copy constructor, initializes with the values of edge.
		/*! \param edge Edge used for initialization.
		 */
		inline CSubdivEdge(const CSubdivEdge &edge)
		{
			*this = edge;
		}
		
		inline static RtFloat sharpVal() { return ms_sharpVal; }
		
		// Destructor, nothing to do - standard is used.
		// inline ~CSubdivEdge() {}
		
		//! Prints the contents of the instance to a stream.
		/*! \param o Stream to print on.
		 *  \param pre String used as first output for each line.
		 */
		void dump(std::ostream &o, const std::string &pre) const;
		
		//! Gets a new instance as clone of *this.
		/*! \return A clone of *this.
		 */
		inline CSubdivEdge *duplicate() const
		{
			return new CSubdivEdge(*this);
		}
		
		//! Assignment operator, assigns all values of e to *this.
		/*! \param e Values of this edge are assigned to *this.
		 *  \return A reference of the instance (*this).
		 */
		CSubdivEdge &operator=(const CSubdivEdge &e);

		inline bool hasVertices(long v1, long v2) const
		{
			return ((m_vertex[0] == v1) && (m_vertex[1] == v2)) ||
			       ((m_vertex[0] == v2) && (m_vertex[1] == v1));
		}
		
		//! Two edges are considered as equal, if they have the same vertex index numbers.
		/*! \param e Edge to compare with.
		 *  \return true if the vertices of the edges are the same, false if not equal.
		 */
		inline bool operator==(const CSubdivEdge &e) const
		{
			return hasVertices(e.m_vertex[0], e.m_vertex[1]);
		}
		
		//! Inserts the two vertices of an edge.
		/*! \param v1 A vertex index number of the edge.
		 *  \param v2 Another index number of the edge.
		 */
		inline void setVertices(long v1, long v2)
		{
			m_vertex[0] = v1;
			m_vertex[1] = v2;
		}
		
		//! Inserts the index number of an incident face.
		/*! There are maximal two incident faces.
		 *  \param startVertex
		 *  \param endVertex
		 *  \param f An index number of a face
		 *  \return true face was inserted, false if there where already two incident faces.
		 */
		inline bool insertFace(long startVertex, long endVertex, long f)
		{
			if ( m_vertex[0] == startVertex ) {
				if ( m_vertex[1] != endVertex || m_face[0] != -1 ) {
					// illegal topology
					return false;
				}
				m_face[0] = f;
				return true;
			}
			if ( m_vertex[0] == endVertex ) {
				if ( m_vertex[1] != startVertex || m_face[1] != -1 ) {
					// illegal topology
					return false;
				}
				m_face[1] = f;
				return true;
			}
			// illegal topology
			return false;
		}
		
		//! Sets the edge type. The edge is a round edge or a sharp crease.
		/*! \param aType New edge type.
		 */
		inline void type(EnumSubdivEdgeType aType)
		{
			m_type = aType;
		}
		
		//! Gets the edge type.
		/*! \return The edge type, a round edge or a sharp crease.
		 */
		inline EnumSubdivEdgeType type() const
		{
			return m_type;
		}
		
		//! Sets the edge value, the sharpness of a crease.
		/*! \param aValue New edge value.
		 */
		inline void value(RtFloat aValue)
		{
			m_value = aValue;
		}
		
		inline void crease(RtFloat aValue)
		{
			m_value = aValue;
			m_type = EDGE_CREASE;
		}

		//! Gets the edge value, the sharpness of a crease.
		/*! \return The edge value.
		 */
		inline RtFloat value() const
		{
			return m_value;
		}
		
		/*
		inline RtFloat valueFactor() const
		{
			RtFloat val = clamp(m_value, (RtFloat)0, ms_sharpVal);
			return val / ms_sharpVal;
		}
		*/

		inline void startChildIndex(long aStartChildIndex)
		{
			m_startChildIndex = aStartChildIndex;
		}
		
		inline long startChildIndex() const
		{
			return m_startChildIndex;
		}
		
		inline long endChildIndex() const
		{
			return m_startChildIndex + nChilds();
		}
		
		inline long nChilds() const
		{
			return isBoundary() ? 3 : 4;
		}
		
		inline long face(int idx) const
		{
			assert(idx == 0 || idx == 1);
			if ( idx != 0 && idx != 1 )
				return -1;
			return m_face[idx];
		}

		inline long vertex(int idx) const
		{
			assert(idx == 0 || idx == 1);
			if ( idx != 0 && idx != 1 )
				return -1;
			return m_vertex[idx];
		}

		inline long localFaceIdx(int idx) const
		{
			if ( m_face[0] == idx )
				return 0;
			if ( m_face[1] == idx )
				return 1;
			return -1;
		}
		
		inline long localVertexIdx(int idx) const
		{
			if ( m_vertex[0] == idx )
				return 0;
			if ( m_vertex[1] == idx )
				return 1;
			return -1;
		}

		//! Gets the adjacent vertex of a vertex and an edge.
		/*! \param aVertex A vertex of the edge.
		 *  \retval adjacent The Index of the adjacent (neighbour) of aVertex and the edge.
		 *  \return True, A neighbour vertex is returned in adjacent (-1 if not existant).
		 *          False, if aVertex is not incident to the edge, adjacent left untouched.
		 */
		inline bool getAdjacentVertex(long aVertex, long &adjacent) const
		{
			if ( aVertex == m_vertex[0] )
				adjacent = m_vertex[1];
			else if ( aVertex == m_vertex[1] )
				adjacent = m_vertex[0];
			else
				return false;
			
			return true;
		}
		
		inline long adjacentVertex(long aVertex) const
		{
			if ( aVertex == m_vertex[0] )
				return m_vertex[1];
			else if ( aVertex == m_vertex[1] )
				return m_vertex[0];
			
			return -1;
		}

		//! Gets the adjacent face of another face and the edge.
		/*! \param aFace A face of the edge.
		 *  \retval adjacent The Index of the neighbour (adjacent) of aFace of the edge.
		 *  \return true: A neighbour face index is returned in adjacent (-1 if not existant).
		 *          false: aFace is not incident to the edge, the parameter adjacent is left untouched.
		 */
		inline bool getAdjacentFace(long aFace, long &adjacent) const
		{
			if ( aFace == m_face[0] )
				adjacent = m_face[1];
			else if ( aFace == m_face[1] )
				adjacent = m_face[0];
			else
				return false;
			
			return true;
		}

		inline long adjacentFace(long aFace) const
		{
			if ( aFace == m_face[0] )
				return m_face[1];
			else if ( aFace == m_face[1] )
				return m_face[0];
			
			return -1;
		}
		
		//! Is the edge a boundary edge?
		/*! If there is only one incident face, the edge is a boundary. Since m_oneFace is
		 *  filled at first and there must be at least one incident face, the edge is
		 *  considered as a boundary if m_otherFace is negative.
		 *  Edges incident to a 'hole' face are not considered as boundary.
		 *  \return true: If the edge is a boundary. false: edge is inside the mesh (has two incident faces)
		 */
		inline bool isBoundary() const
		{
			return m_face[0] < 0 || m_face[1] < 0;
		}
		
		//! Is the edge a crease?
		/*! An edge is a sharp crease if it is tagged as EDGE_CREASE.
		 *  \return true: If the edge is a crease, false: edge is not a crease.
		 */
		inline bool isCrease() const
		{
			return (m_type == EDGE_CREASE) && (m_value > 0);
		}
		
		//! Is the edge a sharp crease?
		/*! An edge is a sharp crease if it is tagged as EDGE_CREASE having a m_value of RI_INFINITY
		 *  \return true: If the edge is sharp, false: edge is not sharp.
		 */
		inline bool isSharp() const
		{
			return m_type == EDGE_CREASE && m_value >= ms_sharpVal;
		}

		//! Is the edge a boundary and a crease?
		/*! \return true Creased boundary edge, false no boundary or not a crease
		 */
		inline bool isCreasedBoundary() const
		{
			return isCrease() && isBoundary();
		}
		
		inline bool isEdge(long v1, long v2) const
		{
			return (v1 == m_vertex[0] && v2 == m_vertex[1]) || (v2 == m_vertex[0] && v1 == m_vertex[1]);
		}

		void insertCreaseVal(const CRiHierarchicalSubdivisionMesh &obj);
	}; // CSubdivEdge
	
	
	//! Prints the contents of the CSubdivEdge e to a stream by calling its dump member function.
	/*! \param e CSubdivEdge to print to a stream.
	 *  \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	inline void dump(const CSubdivEdge &e, std::ostream &o, const std::string &pre)
	{
		e.dump(o, pre);
	}


	//! Vertex for subdivision.
	/*! Used for conectivity information only, point values etc. are not stored here.
	 *  The vertices are stored in TSubdivMesh::m_vertices. The index of a vertex
	 *  in this container is used as identification.
	 */
	class CSubdivVertex {
	public:
		//! The type of a vertex (rounded or corner).
		enum EnumSubdivVertexType {
			VERTEX_ROUNDED, //!< Rounded vertex.
			VERTEX_CORNER   //!< Vertex is a corner.
		};
		
	private:
		static const RtFloat ms_sharpVal;
		RtFloat	m_value;          //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, belends between round vertex (m_value==0) and corner (m_value==1).
		EnumSubdivVertexType m_type; //!< Vertex type: VERTEX_ROUNDED or VERTEX_CORNER.
		
		long m_startFace; //!< Index for CSubdivisionIndices::m_incidentFaces
		long m_startEdge; //!< Index for CSubdivisionIndices::m_incidentEdges

		long m_incidentFaces; //!< Number of incident faces
		long m_incidentEdges; //!< Number of incident edges
		
		long m_faceIndex;     //!< Temporary index of the (first) vertex in a single face - vertex can have multiple values if vertex is a corner, because of the normals and hierarchical edits.

		bool m_faceVertex;     //!< Is a vertex of a face boundary
		
		// Parameters have the same index as the vertex, face vertices, corners and vertices incident to sharp creases have additional parameters (of the same value)

	public:
		//! Standard constructor, clears all members.
		inline CSubdivVertex() :
		m_value(0.0), m_type(VERTEX_ROUNDED), m_startFace(0), m_startEdge(0), m_incidentFaces(0), m_incidentEdges(0), m_faceIndex(-1), m_faceVertex(false)
		{
		}
		
		//! Copy constructor, initializes with the values of ver.
		/*! \param ver Vertex used for initialization.
		 */
		inline CSubdivVertex(const CSubdivVertex &ver)
		{
			*this = ver;
		}
		
		// Destructor, nothing to do - standard is used.
		// inline ~CSubdivVertex() {}
		
		inline static RtFloat sharpVal() { return ms_sharpVal; }

		//! Prints the contents of the instance to a stream.
		/*! \param o Stream to print on.
		 *  \param pre String used as first output for each line.
		 */
		void dump(std::ostream &o, const std::string &pre) const;
		
		//! Gets a new instance as clone of *this.
		/*! \return A clone of *this.
		 *  \throw TSystemException if there is not enough memory.
		 */
		inline CSubdivVertex *duplicate() const
		{
			return new CSubdivVertex(*this);
		}
		
		//! Assignment operator, assigns all values of v to *this.
		/*! \param v Values of this vertex are assigned to *this.
		 *  \return A reference of the instance (*this).
		 */
		CSubdivVertex &operator=(const CSubdivVertex &v);
		
		//! Sets the vertex type.
		/*! \param aType New vertex type.
		 */
		inline void type(EnumSubdivVertexType aType)
		{
			m_type = aType;
		}
		
		//! Gets the vertex type.
		/*! \return The vertex type, a normal vertex or a corner.
		 */
		inline EnumSubdivVertexType type() const
		{
			return m_type;
		}
		
		//! Sets the vertex value, the sharpness of the corner.
		/*! \param aValue New vertex value.
		 */
		inline void value(RtFloat aValue)
		{
			m_value = aValue;
		}

		inline void corner(RtFloat aValue)
		{
			m_value = aValue;
			m_type = VERTEX_CORNER;
		}
		
		inline bool isSharp() const
		{
			return m_type == VERTEX_CORNER && m_value >= ms_sharpVal;
		}

		inline bool isCorner() const
		{
			return m_type == VERTEX_CORNER && m_value > 0;
		}

		//! Gets the vertex value.
		/*! \return The vertex value, the sharpness of the corner.
		 */
		inline RtFloat value() const
		{
			return m_value;
		}

		/*
		inline RtFloat valueFactor() const
		{
			RtFloat val = clamp(m_value, (RtFloat)0, ms_sharpVal);
			return val / ms_sharpVal;
		}
		*/

		inline long startEdge() const
		{
			return m_startEdge;
		}

		inline long nextEdge(long anEdge) const
		{
			long e = anEdge +1;
			return e >= endEdge() ? startEdge() : e;
		}

		inline void startEdge(long anEdgeIndex)
		{
			m_startEdge = anEdgeIndex;
		}

		inline long endEdge() const
		{
			return m_startEdge+m_incidentEdges;
		}

		inline long incidentEdges() const
		{
			return m_incidentEdges;
		}
		
		inline void incidentEdges(long val)
		{
			m_incidentEdges = val;
		}

		inline void incIncidentEdges()
		{
			++m_incidentEdges;
		}

		inline long startFace() const
		{
			return m_startFace;
		}
		
		inline void startFace(long aFaceIndex)
		{
			m_startFace = aFaceIndex;
		}
		
		inline long endFace() const
		{
			return m_startFace+m_incidentFaces;
		}
		
		inline long nextFace(long aFace) const
		{
			long f = aFace + 1;
			return f >= endFace() ? startFace() : f;
		}
		
		inline long incidentFaces() const
		{
			return m_incidentFaces;
		}

		inline void incidentFaces(long val)
		{
			m_incidentFaces = val;
		}

		inline void incIncidentFaces()
		{
			++m_incidentFaces;
		}
		
		inline bool faceVertex() const
		{
			return m_faceVertex;
		}

		inline void faceVertex(bool isFaceVertex)
		{
			m_faceVertex = isFaceVertex;
		}

		inline void faceIndex(long aFaceIndex)
		{
			m_faceIndex = aFaceIndex;
		}

		inline long faceIndex() const
		{
			return m_faceIndex;
		}
		
		void insertCornerVal(long idx, const CRiHierarchicalSubdivisionMesh &obj);
	}; // CSubdivVertex
	
	
	//! Prints contents of the CSubdivVertex v to a stream by calling its dump member function.
	/*! \param v CSubdivVertex to print to a stream.
	 *  \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	inline void dump(const CSubdivVertex &v, std::ostream &o, const std::string &pre)
	{
		v.dump(o, pre);
	}
		
	class CSubdivisionStrategy;
	
	class CSubdivisionIndices {
	private:
		std::vector<CSubdivFace>   m_faces;          //!< All faces of a subdivision mesh.
		std::vector<long>          m_edgeIndices;    //!< Indices (to m_edges) for edges incident to faces.
		std::vector<CSubdivEdge>   m_edges;          //!< All edges of a a subdivision mesh.
		std::vector<long>          m_vertexIndices;  //!< Indices (to m_vertices) for vertices incident to faces.
		std::vector<CSubdivVertex> m_vertices;       //!< All vertices of a a subdivision mesh.
		std::vector<long>          m_incidentEdges;  //!< Indices (to m_edges) for edges incident to vertices.
		std::vector<long>          m_incidentFaces;  //!< Indices (to m_faces) for faces incident to vertices.
		RtInt m_interpolateBoundary;                 //!< Value of the boundary tag, 0: no interpolation, 1: creased edge chains and sharp corners, 2: creased edge chains, but corners not effected
		bool m_illTopology;                          //!< Error while subdividing.
		
		std::vector<long>          m_creasedEdges;   //!< Indices (to m_edges) for creased edge chains, sizes in m_creaseSizes.
		std::vector<long>          m_creaseSizes;    //!< Sizes of creased edge chains m_creasedEdges

		void insertBoundaryVal(const CRiHierarchicalSubdivisionMesh &obj); //!< Sets m_interpolateBoundary using tags of @a obj.
		bool insertEdgeIndex(const CSubdivFace &aFace, const CSubdivEdge &anEdge, long anEdgeIndex); //!< Inserts the edges of aFace into m_edgeIndices
		void updateVertexData(); //!< Update the incident data m_incidentEdges, m_incidentFaces after a subdivision step
		
		long clearFaceVertexIndices(const std::list<CSubdivisionIndices>::iterator &aParent, const std::list<CSubdivisionIndices>::iterator &cur, long faceIdx);
		void fillFaceVertexIndices(const std::list<CSubdivisionIndices>::iterator &aParent, const std::list<CSubdivisionIndices>::iterator &cur, long faceIdx, std::vector<IndexType> &indices, long &triangleCnt, long &sharedCnt, long &unsharedCnt);
		void fillOrigIndices(const std::list<CSubdivisionIndices>::iterator &aParent, const std::list<CSubdivisionIndices>::iterator &cur, long faceIdx, std::vector<IndexType> &origIndices, std::vector<bool> &faceIndices, long &sharedStart, long &unsharedStart);
		long countTriangleIndices(const std::list<CSubdivisionIndices>::const_iterator &aParent, const std::list<CSubdivisionIndices>::const_iterator &cur, long faceIdx) const;
		void correctTriangleIndices(std::vector<IndexType> &indices, long unsharedStart, long prevSharedStart) const;
	public:
		/** @brief Constructor
		 */
		inline CSubdivisionIndices() : m_illTopology(false), m_interpolateBoundary(0) {}
		

		/** @brief Initialization of the connectivity of the subdivision mesh.
		 *
		 *  The mesh must be valid for Catmull-Clark subdivision: An edge has
		 *  one or two adjacent faces (2-manifold), each vertex has at least two incident edges.
		 *  The parameters are those of the TRi::subdivisionMesh() call. This member
		 *  function is only called once per mesh to initialize
		 *  the structure.
		 *
		 *  @param obj Definitison of a subdivision mesh.
		 */
		void initialize(const CRiHierarchicalSubdivisionMesh &obj);
		
		inline std::vector<CSubdivFace> &faces() {return m_faces;}
		inline const std::vector<CSubdivFace> &faces() const {return m_faces;}

		inline std::vector<CSubdivEdge> &edges() {return m_edges;}
		inline const std::vector<CSubdivEdge> &edges() const {return m_edges;}
		inline std::vector<long> &edgeIndices() {return m_edgeIndices;}
		inline const std::vector<long> &edgeIndices() const {return m_edgeIndices;}

		inline std::vector<CSubdivVertex> &vertices() {return m_vertices;}
		inline const std::vector<CSubdivVertex> &vertices() const {return m_vertices;}
		inline std::vector<long> &vertexIndices() {return m_vertexIndices;}
		inline const std::vector<long> &vertexIndices() const {return m_vertexIndices;}
		
		inline const std::vector<long> &incidentEdges() const { return m_incidentEdges; }
		inline const std::vector<long> &incidentFaces() const { return m_incidentFaces; }
				
		long sharpCreasedVertex(const CSubdivVertex &aVertex, RtInt interpolateBoundary, long &crease0, long &crease1) const;
		long creasedVertex(const CSubdivVertex &aVertex, RtInt interpolateBoundary, long &crease0, long &crease1, RtFloat &factor0, RtFloat &factor1) const;

		RtFloat sumCrease(const CSubdivVertex &aVertex, long forEdgeIdx, long &edgeCnt) const;
		long vertexBoundary(const CSubdivVertex &aVertex, long &bound0, long &bound1) const;

		inline bool isBoundary(const CSubdivVertex &aVertex) const
		{
			long b0, b1;
			long vb = vertexBoundary(aVertex, b0, b1);
			return vb >= 1;
		}

		inline bool isBoundary(const CSubdivFace &aFace) const
		{
			for ( long i = aFace.startVertexIndex(); i < aFace.endVertexIndex(); ++i ) {
				if ( isBoundary(m_vertices[m_vertexIndices[i]]) )
					return true;
			}
			return false;
		}

		inline bool isSharpCorner(const CSubdivVertex &aVertex) const
		{
			long c0, c1;
			long vb = sharpCreasedVertex(aVertex, interpolateBoundary(), c0, c1);
			return vb > 2 || vb == aVertex.incidentEdges() || aVertex.isSharp();
		}

		inline bool isCorner(const CSubdivVertex &aVertex) const
		{
			long c0, c1;
			RtFloat val0, val1;
			long vb = creasedVertex(aVertex, interpolateBoundary(), c0, c1, val0, val1);
			return vb > 2 || vb == aVertex.incidentEdges() || aVertex.isCorner();
		}

		inline bool faceEdge(const CSubdivEdge &anEdge) const
		{
			return m_vertices[anEdge.vertex(0)].faceVertex() &&  m_vertices[anEdge.vertex(1)].faceVertex();
		}
	
		inline bool illTopology() const { return m_illTopology; }

		inline RtInt interpolateBoundary() const { return m_interpolateBoundary; }
		inline void interpolateBoundary(RtInt anInterpolateBoundary)
		{
			m_interpolateBoundary = anInterpolateBoundary;
		}

		void subdivide(CSubdivisionIndices &aParent, const CSubdivisionStrategy &aStrategy,
					   const CRiHierarchicalSubdivisionMesh &anObj);
		
		void prepareFace(const std::list<CSubdivisionIndices>::iterator &root, const std::list<CSubdivisionIndices>::iterator &cur, long faceIdx, std::vector<IndexType> &indices, IndexType &sharedIndices, std::vector<IndexType> &origIndices, std::vector<bool> &faceIndices);
		bool calcNormalForVertexInFace(long faceIdx, long vertexIdx, const std::vector<RtFloat> &pos, bool flipNormals, RtFloat *normal) const;
		void calcNormal(long faceIdx, long vertexIdx, bool isFaceVertex, const std::vector<RtFloat> &pos, bool flipNormals, RtFloat *resultsF3) const;
		void calcNormals(long faceIdx, IndexType &sharedIndices, std::vector<IndexType> &origIndices, std::vector<bool> &faceIndices, const std::vector<RtFloat> &pos, bool flipNormals, std::vector<RtFloat> &floats) const;
	};
	
	class CSubdivisionStrategy {
	public:
		inline virtual ~CSubdivisionStrategy() {}
		virtual void subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) const = 0;
		virtual void insertVaryingValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const = 0;
		virtual void insertVertexValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const = 0;
		virtual void insertFaceVaryingValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, IndexType &sharedIndices, std::vector<IndexType> &origIndices, std::vector<bool> &faceIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const = 0;
		virtual bool discardableBoundaryFace(CSubdivisionIndices &root, long faceIdx) const = 0;
	};
	
	class CCatmullClarkSubdivision : public CSubdivisionStrategy {
		inline long childCenterEdge(const CSubdivEdge &anEdge, int faceIdx) const
		{
			if ( anEdge.face(0) == faceIdx )
				return anEdge.startChildIndex()+2;
			if ( anEdge.face(1) == faceIdx &&  anEdge.face(0) < 0 )
				return anEdge.startChildIndex()+2;
			if ( anEdge.face(1) == faceIdx &&  anEdge.face(0) >= 0 )
				return anEdge.startChildIndex()+3;
			return -1;
		}
		inline long childMainEdge(const CSubdivEdge &anEdge, int vertexIdx) const
		{
			if ( anEdge.vertex(0) == vertexIdx )
				return anEdge.startChildIndex();
			if ( anEdge.vertex(1) == vertexIdx )
				return anEdge.startChildIndex()+1;
			return -1;
		}
	public:
		virtual void subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) const;
		virtual void insertVaryingValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const;
		virtual void insertVertexValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const;
		virtual void insertFaceVaryingValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, IndexType &sharedIndices, std::vector<IndexType> &origIndices, std::vector<bool> &faceIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const;
		virtual bool discardableBoundaryFace(CSubdivisionIndices &root, long faceIdx) const;
	};
	
	class CNoneSubdivision : public CCatmullClarkSubdivision {
	public:
		inline virtual void insertVertexValues(const std::list<CSubdivisionIndices>::const_iterator &theIndices, const std::list<CSubdivisionIndices>::const_iterator &curIndices, const CDeclaration &decl, std::vector<RtFloat> &floats) const
		{
			return insertVaryingValues(theIndices, curIndices, decl, floats);
		}
		inline virtual bool discardableBoundaryFace(CSubdivisionIndices &root, long faceIdx) const
		{
			return false;
		}
	};

	class CSubdivisionStrategies : public TemplObjPtrRegistry<RtToken, CSubdivisionStrategy *> {
	public:
		inline CSubdivisionStrategies() : TemplObjPtrRegistry<RtToken, CSubdivisionStrategy *>(true) {};
	};
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_SUBDIVISION_H
