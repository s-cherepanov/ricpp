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

#ifndef _RICPP_RICONTEXT_TRIANGULATION_H
#include "ricpp/ricontext/triangulation.h"
#endif // _RICPP_RICONTEXT_TRIANGULATION_H

#include <iostream>

namespace RiCPP {
	
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
		long m_startVertexIndex; //!< Start index of incident vertices in the mesh, ccw, -1 if not initialised
		long m_endVertexIndex;   //!< End of incident vertices in the mesh (-1 if not initialised), m_endVertexIndex - m_startVertexIndex = number of vertices incident to the face.
		long m_startEdgeIndex;   //!< Start index of incident edges, (start edge is the edge that joins start vertex and the ccw next index) -1 if not initialised.
		long m_endEdgeIndex;     //!< End of incident vertices and edges, m_endVertexEdgeIndex - m_startVertexEdgeIndex = number of vertices and edges of the face. -1 if not initialised.
		/** @}
		 */
		
		long m_faceIndex;            //!< Index of this face used in the hierarchical path.
		long m_parentFaceIndex;      //!< Index of the parent in the hierarchical path.
		long m_origFaceIndex;        //!< Index of the face in the original polygon mesh (prior to subdivision), -1 if not initialised.
		
	public:		
		//! Standard constructor, initializes all elements.
		inline CSubdivFace() :
		m_type(FACE_FILLED),
		m_startVertexIndex(-1L),
		m_endVertexIndex(-1L),
		m_startEdgeIndex(-1L),
		m_endEdgeIndex(-1L),
		m_parentFaceIndex(-1L), m_faceIndex(-1L), m_origFaceIndex(-1L)
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
		
		//! Sets the start index of the incident vertices of the face.
		/*! \param sVertexIndex The new start index for a ccw vertex (index) loop,
		 *         \sa vertexIndex(), -1 if it was not set.
		 */
		inline void startVertexIndex(long aVertexIndex)
		{
			m_startVertexIndex = aVertexIndex;
		}
		
		//! Gets the start index of the incident vertices of the face.
		/*! \return The start index of the vertices (ccw vertex indices) of
		 *          the face, -1 if it was not set.
		 */
		inline long startVertexIndex() const
		{
			return m_startVertexIndex;
		}
		
		//! Sets the start index of the incident edges of the face.
		/*! \param anEdgeIndex The new start index for a ccw edge (index) loop,
		 *         \sa vertexIndex(), -1 if it was not set.
		 */
		inline void startEdgeIndex(long anEdgeIndex)
		{
			m_startEdgeIndex = anEdgeIndex;
		}
		
		//! Gets the start index of the incident edges of the face.
		/*! \return The start index of the edge (ccw edge indices) of
		 *          the face, -1 if it was not set.
		 */
		inline long startEdgeIndex() const
		{
			return m_startEdgeIndex;
		}
		
		//! Sets the end index (+1) of the vertices of the face.
		/*! \param aVertexAndEdgeIndex The new end index (+1) for a ccw vertex (index) loop,
		 *         \sa startVertexIndex()
		 */
		inline void endVertexIndex(long aVertexIndex)
		{
			m_endVertexIndex = aVertexIndex;
		}
		
		//! Gets the end index (+1) of the vertices of the face
		/*! \return The end index (+1) of the vertices (ccw vertex indices) of
		 *          the face, -1 if it was not set.,
		 *          \sa startVertexIndex()
		 */
		inline long endVertexIndex() const
		{
			return m_endVertexIndex;
		}
		
		//! Sets the end index (+1) of the incident edge of the face.
		/*! \param aVertexAndEdgeIndex The new end index (+1) for a ccw edge (index) loop,
		 *         \sa startEdgeIndex()
		 */
		inline void endEdgeIndex(long anEdgeIndex)
		{
			m_endEdgeIndex = anEdgeIndex;
		}
		
		//! Gets the end index (+1) of the incident of the face
		/*! \return The end index (+1) of the incident edges (ccw vertex indices) of
		 *          the face, -1 if it was not set.,
		 *          \sa startEdgeIndex()
		 */
		inline long endEdgeIndex() const
		{
			return m_endEdgeIndex;
		}
		
		/*! \return The number of vertices grouped around the face. After the initial
		 *          subdivision, this should be always 4 for catmull-clark subdivision
		 *          meshes.
		 */
		inline long vertices() const
		{
			return m_endVertexIndex - m_startVertexIndex;
		}
		
		/*! \return The number of edges grouped around the face. After the initial
		 *          subdivision, this should be always 4 for catmull-clark subdivision
		 *          meshes.
		 */
		inline long edges() const
		{
			return m_endEdgeIndex - m_startEdgeIndex;
		}
		
		//! Sets the parent face index for the hierarchical path.
		/*! \param aParentFaceIndex parent face index for the hierarchical path.
		 */
		inline void parentFaceIndex(long aParentFaceIndex)
		{
			m_parentFaceIndex = aParentFaceIndex;
		}
		
		//! Gets the parent face index for the hierarchical path.
		/*! \return Parent face index for the hierarchical path.
		 */
		inline long parentFaceIndex() const
		{
			return m_parentFaceIndex;
		}
		
		//! Sets the face index for the hierarchical path.
		/*! \param aFaceIndex face index for the hierarchical path.
		 */
		inline void faceIndex(long anIndex)
		{
			m_faceIndex = anIndex;
		}
		
		//! Gets the face index for the hierarchical path.
		/*! \return Face index for the hierarchical path.
		 */
		inline long faceIndex() const
		{
			return m_faceIndex;
		}
		
		//! Sets the original face index.
		/*! \param anOrigFaceIndex The index of the face at the original subdivision mesh (before subdivision).
		 */
		inline void origFaceIndex(long anOrigFaceIndex)
		{
			m_origFaceIndex = anOrigFaceIndex;
		}
		
		//! Gets the original face index.
		/*! \return The index of the face at the original subdivision mesh (before subdivision). -1 if not set.
		 */
		inline long origFaceIndex() const
		{
			return m_origFaceIndex;
		}
		
		inline long edgeIndex(long pos) {
			return pos < 0 || pos > edges() ? -1 : m_startEdgeIndex + pos;
		}

		inline long vertexIndex(long pos) {
			return pos < 0 || pos > vertices() ? -1 : m_startVertexIndex + pos;
		}
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
		//! The type of an edge (rounded or crease).
		enum EnumSubdivEdgeType {
			EDGE_ROUNDED, //!< Edge is rounded.
			EDGE_CREASE   //!< Edge is a crease.
		};
		
		RtFloat	m_value;           //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, gives the sharpness of a crease. The value blends from 0: smooth to 1: sharp.
		EnumSubdivEdgeType m_type; //!< Edge type: EDGE_ROUNDED or EDGE_CREASE.
		
		long m_oneVertex,   //! One vertex of the edge, index of an incident vertex, m_oneVertex < m_otherVertex.
		m_otherVertex;      //! The other vertex of the edge, index of an incident vertex, m_oneVertex < m_otherVertex.
		long m_oneFace,     //! One incident face of the edge, index of an incident face. The value is <0 if not initalized.
		m_otherFace;        //! The other incident face of the edge, index of an incident face, m_otherFace == -1 and m_oneFace >= 0 if there is only one face incident to the edge.
	public:
		//! Standard constructor, initializes all elements.
		inline CSubdivEdge() :
		m_value(0.0), m_type(EDGE_ROUNDED),
		m_oneVertex(-1L), m_otherVertex(-1L),
		m_oneFace(-1L), m_otherFace(-1L)
		{}
		
		//! Copy constructor, initializes with the values of edge.
		/*! \param edge Edge used for initialization.
		 */
		inline CSubdivEdge(const CSubdivEdge &edge)
		{
			*this = edge;
		}
		
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
		
		/** @{
		 *  @name comparsion
		 */
		//! Two edges are considered as equal, if they have the same vertex index numbers.
		/*! Because the vertex index numbers are ordered the comparation is done only for the
		 *  vertices with the same name.
		 *  \param e Edge to compare with.
		 *  \return true if the vertices of the edges are the same, false if not equal.
		 */
		inline bool operator==(const CSubdivEdge &e) const
		{
			return (m_oneVertex == e.m_oneVertex) && (m_otherVertex == e.m_otherVertex);
		}
		
		//! Comparation (*this grater as e) using the vertex index numbers (\sa operator==).
		/*!  \param e Edge to compare with.
		 *   \return true if the vertex index numbers of e are greater than the ones of *this, false if they are less or equal.
		 */
		inline bool operator>(const CSubdivEdge &e) const
		{
			return (m_oneVertex > e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex > e.m_otherVertex));
		}
		
		//! Comparation (*this grater or equal as e) using the vertex index numbers (\sa operator==).
		/*!  \param e Edge to compare with.
		 *   \return true if the vertex index numbers of e are greater or equal than the ones of *this, false if they are less.
		 */
		inline bool operator>=(const CSubdivEdge &e) const
		{
			return (m_oneVertex >= e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex >= e.m_otherVertex));
		}
		
		//! Comparation (*this less as e) using the vertex index numbers (\sa operator==).
		/*!  \param e Edge to compare with.
		 *   \return true if the vertex index numbers of e are less than the ones of *this, false if they are greater or equal.
		 */
		inline bool operator<(const CSubdivEdge &e) const
		{
			return (m_oneVertex < e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex < e.m_otherVertex));
		}
		
		//! Comparation (*this less or equal as e) using the vertex index numbers (\sa operator==).
		/*!  \param e Edge to compare with
		 *   \return true if the vertex index numbers of e are less or equal than the ones of *this, false if they are greater.
		 */
		inline bool operator<=(const CSubdivEdge &e) const
		{
			return (m_oneVertex <= e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex <= e.m_otherVertex));
		}
		/** @}
		 */
		
		//! Inserts the two vertices of an edge.
		/*! The vertex index numbers are sorted,
		 *  so that m_oneVertex < m_otherVertex after assignment.
		 *
		 *  \param v1 A vertex index number of the edge.
		 *  \param v2 Another index number of the edge.
		 */
		inline void newEdge(long v1, long v2)
		{
			if ( v1 < v2 ) {
				m_oneVertex   = v1;
				m_otherVertex = v2;
			} else {
				m_oneVertex   = v2;
				m_otherVertex = v1;
			}
		}
		
		//! Inserts the index number of an incident face.
		/*! There are maximal two incident faces. m_oneFace is filled first.
		 *  \param f An index number of a face
		 *  \return true face was inserted, false if there where already two incident faces.
		 */
		inline bool insertFace(long f)
		{
			if ( m_oneFace == -1 ) {
				m_oneFace = f;
				return true;
			}
			if ( m_otherFace == -1 ) {
				m_otherFace = f;
				return true;
			}
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
		
		//! Gets the edge value, the sharpness of a crease.
		/*! \return The edge value.
		 */
		inline RtFloat value() const
		{
			return m_value;
		}
		
		//! Gets the neighbour vertex of an edge and aVertex.
		/*! \param aVertex A vertex of the edge.
		 *  \retval neighbour The Index of the neighbour of aVertex of the edge.
		 *  \return true A neighbour vertex is returned in neighbour, false otherwise neighbour left untouched.
		 */
		inline bool getNeighbourVertex(long &neighbour, long aVertex) const
		{
			if ( aVertex == m_oneVertex )
				neighbour = m_otherVertex;
			else if ( aVertex == m_otherVertex )
				neighbour = m_oneVertex;
			else
				return false;
			
			return true;
		}
		
		//! Gets the neighbour face of an edge and aFace.
		/*! \param aFace A face of the edge.
		 *  \retval neighbour The Index of the neighbour of aFace of the edge.
		 *  \return true: A neighbour face index is returned in neighbour.
		 *          false: there is no neighbour, the parameter neighbour is left untouched.
		 */
		inline bool getNeighbourFace(long &neighbour, long aFace) const
		{
			if ( aFace == m_oneFace )
				neighbour = m_otherFace;
			else if ( aFace == m_otherFace )
				neighbour = m_oneFace;
			else
				return false;
			
			return true;
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
			return m_otherFace < 0;
		}
		
		//! Is the edge a sharp crease?
		/*! An edge is a sharp crease if it is tagged as EDGE_CREASE.
		 *  \return true: If the edge is sharp, false: edge is not sharp.
		 */
		inline bool isSharp() const
		{
			return m_type == EDGE_CREASE;
		}
		
		//! Is the edge a boundary and not a sharp crease?
		/*! \return true unsharp boundary edge, false no boundary or sharp crease
		 */
		inline bool isUnsharpBoundary() const
		{
			return (isBoundary() && !isSharp());
		}
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
		//! The type of a vertex (rounded or corner).
		enum CSubdivVertexType {
			VERTEX_ROUNDED, //!< Rounded vertex.
			VERTEX_CORNER   //!< Vertex is a corner.
		};
		
		RtFloat	m_value;          //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, belends between round vertex (m_value==0) and corner (m_value==1).
		CSubdivVertexType m_type; //!< Vertex type: VERTEX_ROUNDED or VERTEX_CORNER.
		
		// Incident edges
		long m_idxStartEdge;    //!< Index incident edge start, there must be two or more edges, points to TSubdivMesh::m_idxIncidentEdges.
		long m_idxEndEdge;      //!< End of incident edges, number of incident eges is m_idxEndEdge-m_idxStartEdge.
		
		// Incident surfaces
		long m_idxStartFace;    //!< Index list of incident faces (one or more) of the mesh.
		long m_idxEndFace;      //!< End of incident faces, number of incident faces is m_idxEndFace-m_idxStartFace.
		
		// Values, same ordering as the surfaces, or only one if the same for all faces (duplicate values, e.g. corners need one normal per face)
		IndexType m_idxStartValue; //< Index of the value for m_idxStartFace
		IndexType m_idxEndValue;   //< End of values, number of values is m_idxEndValue-m_idxStartValue (either ==1 or number of faces)

	public:
		//! Standard constructor, clears all members.
		inline CSubdivVertex() :
		m_value(0.0), m_type(VERTEX_ROUNDED),
		m_idxStartEdge(0L), m_idxEndEdge(0L),
		m_idxStartFace(0L), m_idxEndFace(0L),
		m_idxStartValue(0L), m_idxEndValue(0L)
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
		inline void type(CSubdivVertexType aType)
		{
			m_type = aType;
		}
		
		//! Gets the vertex type.
		/*! \return The vertex type, a normal vertex or a corner.
		 */
		inline CSubdivVertexType type() const
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
		
		//! Gets the vertex value.
		/*! \return The vertex value, the sharpness of the corner.
		 */
		inline RtFloat value() const
		{
			return m_value;
		}
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
	
	class CSubdivisionIndices;
	
	class CSubdivisionStrategy {
	public:
		inline virtual ~CSubdivisionStrategy() {}
		virtual void subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) = 0;
	};
	
	class CCatmullClarkSubdivision : public CSubdivisionStrategy {
		virtual void subdivide (CSubdivisionIndices &parent, CSubdivisionIndices &child);
	};

	class CSubdivisionStrategyFactory {
	private:
		std::map<RtToken, CSubdivisionStrategy *> m_strategies;
	public:
		CSubdivisionStrategy *registerStrategy(RtToken strategyName, CSubdivisionStrategy *aStrategy);
		CSubdivisionStrategy *unregisterStrategy(RtToken strategyName);
		CSubdivisionStrategy *getStrategy(RtToken strategyName);
	};
	
	class CSubdivisionIndices {
	private:
		std::vector<CSubdivFace>   m_faces;
		std::vector<CSubdivEdge>   m_edges;
		std::vector<CSubdivVertex> m_vertices;
	public:
		//! Initialization of the connectivity of the subdivision mesh.
		/*! The mesh must be valid for Catmull-Clark subdivision: An edge has
		 *  one or two adjacent faces (2-manifold), each vertex has at least two incidend edges.
		 *  The parameters are those of the TRi::subdivisionMesh() call. This member
		 *  function is only called once per TRi::subdivisionMesh() to initialize
		 *  the structure. TSubdivMesh::subdivideCatmullClark() is used to get
		 *  the indices for further subdivisions.
		 *
		 *  \param nverts  Number of vertices per face, size is the number of faces
		 *  \param verts   Vertex indices.
		 *  \param ntags   Number of tags.
		 *  \param tags    Names of the tags.
		 *  \param nargs   Number of arguments (three per tag: intargs, floargs, stringargs).
		 *  \param intargs Integer arguments of the tags.
		 *  \param floargs Float arguments of the tags.
		 *  \param stringargs String parameters of the tags.
		 */
		void initialize(const std::vector<RtInt> &nverts, const std::vector<RtInt> &verts,
						RtInt ntags, const std::vector<RtToken> &tags,
						const std::vector<RtInt> &nargs,
						const std::vector<RtInt> &intargs,
						const std::vector<RtFloat> &floargs,
						const std::vector<RtToken> &stringargs
						);
		CSubdivFace *face(long faceIndex);
		CSubdivEdge *edge(long faceIndex, long edgeIndex);
		CSubdivVertex *vertex(long faceIndex, long vertexIndex);
		void subdivide (CSubdivisionIndices &parent, const CSubdivisionStrategy &strategy);
	};
	
	class CSubdivisionHierarchie  : public CTesselator {
	private:
		std::list<CSubdivisionIndices> m_indices;
		CRiHierarchicalSubdivisionMesh m_obj;
		const CSubdivisionStrategy &m_strategy;		

		void subdivide(long minDepth, const CSubdivisionStrategy &strategy);
		CSubdivFace *face(long size, long *faceIndices);
		CSubdivEdge *edge(long size, long *faceIndices, long edgeIndex);
		CSubdivVertex *vertex(long size, long *faceIndices, long vertexIndex);
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			return m_obj;
		}
		
	public:
		inline CSubdivisionHierarchie(CRiHierarchicalSubdivisionMesh &anObj, const CSubdivisionStrategy &aStrategy)
			: m_obj(anObj), m_strategy(aStrategy)
		{
		}
		inline CSubdivisionHierarchie(CRiSubdivisionMesh &anObj, const CSubdivisionStrategy &aStrategy)
			: m_obj(anObj), m_strategy(aStrategy)
		{
		}

		inline virtual const CSubdivisionStrategy &strategy() const { return m_strategy; }
		inline virtual const std::list<CSubdivisionIndices> &indices() const { return m_indices; }

		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
};
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_SUBDIVISION_H
