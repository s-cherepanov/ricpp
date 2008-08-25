#ifndef _RICPP_RENDERCONTEXT_SUBDIVISION_H
#define _RICPP_RENDERCONTEXT_SUBDIVISION_H

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
 *  \brief Declaration of the classes for catmull-clark subdivision surfaces meshes.
 *
 *         s.a. Subdivision Surfaces in Character Animation
 *              by Tony DeRose, Michael Kass and Tien Truong
 *              and the RenderMan specification.
 *
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "surfaces.h"
#include "rimacro.h"

//! Face for subdivision.
/*! The face can be filled or be a hole (i.e. it is calculated as normal, but
 *  the area is not filled at output time). A face is given by a sequence of vertices
 *  grouped around the face. The vertices are connected by edges by a counter clockwise ordering.
 */
class TSubdivFacet {
public:
	//! The type of a surface (filled or hole).
	enum TSubdivFacetType {
		FACE_FILLED,  //!< Filled face.
		FACE_HOLE     //!< Face is a hole.
	};

private:
	// RtFloat m_value;     // Tag value copied from appropriate TRi::subdivisionMesh parameter.
	TSubdivFacetType m_type; //!< Face type: FACE_FILLED or FACE_HOLE, FACE_FILLED is the default value.

	// Adjacent vertices and incident edges (for each vertex one edge).
	long m_startVertexEdgeIndex; //!< Start index of adjacent vertices and edges, index for TSubdivMesh::m_idxFaceVertices and TSubdivMesh::m_idxFaceEdges. It's a double index, -1 if not initialised.
	long m_endVertexEdgeIndex;   //!< End of adjacent vertices and incident edges, m_endVertexEdgeIndex - m_startVertexEdgeIndex = number of vertices and edges of the face. It's a double index, -1 if not initialised.

	long m_faceIndex;            //!< Index of the face in the original polygon mesh, -1 if not initialised.
	bool m_boundary;             //!< Boundary faces will not be drawn, they are marked initially.

	RtPoint m_normal;            //!< The normal vector of the face.

public:

	//! Standard constructor, initializes all elements.
	inline TSubdivFacet() :
		// m_value(0.0),
		m_type(FACE_FILLED),
		m_startVertexEdgeIndex(-1L), m_endVertexEdgeIndex(-1L),
		m_faceIndex(-1L),
		m_boundary(false)
	{
		m_normal[2] = m_normal[1] = m_normal[0] = 0.0;
	}

	//! Copy constructor, initializes with the values of face.
	/*! \param face Face used for initialization.
	 */
	inline TSubdivFacet(const TSubdivFacet &face) {
		*this = face;
	}

	// Destructor, nothing to do - standard is used.
	// inline ~TSubdivFacet() {}

	//! Prints the contents of the instance to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	void dump(std::ostream &o, const std::string &pre) const;

	//! Gets a new instance as clone of *this.
	/*! \return A clone of *this.
	 *  \throw TSystemException if there is not enough memory for a new TSubdivFacet instance.
	 */
	inline TSubdivFacet &duplicate() {
		TSubdivFacet *f = new TSubdivFacet(*this);
		if ( !f ) {
			throw new TSystemException(__FILE__, __LINE__);
		}
		return *f;
	}

	//! Assignment operator, assigns all values of f to *this.
	/*! \param f Values of this face are assigned to *this.
	 *  \return A reference of the instance (*this).
	 */
	TSubdivFacet &operator=(const TSubdivFacet &e);

	//! Sets the face type, a hole or filled face.
	/*! \param aType New face type.
	 *  \return The old face type.
	 */
	inline TSubdivFacetType type(TSubdivFacetType aType) {
		TSubdivFacetType old = m_type;
		m_type = aType;
		return old;
	}

	//! Gets the face type.
	/*! \return The face type, a filled face or a hole.
	 */
	inline TSubdivFacetType type() const {
		return m_type;
	}

	// Sets the face value. Face values have no meaning and are ignored at the moment.
	/* \param aValue New face value.
	 * \return The old face value.
	 */
	/*
	inline RtFloat value(RtFloat aValue) {
		RtFloat old = m_value;
		m_value = aValue;
		return old;
	}
	*/

	// Gets the face value. Face values have no meaning and are ignored at the moment.
	/* \return The face value.
	 */
	/*
	inline RtFloat value() const {
		return m_value;
	}
	*/


	// Sets the boundary mark.
	/* \param isBoundary the new boundary mark.
	 * \return The old boundary mark.
	 */
	inline bool boundary(bool isBoundary) {
		bool old = m_boundary;
		m_boundary = isBoundary;
		return old;
	}

	// Gets the boundary mark.
	/* \return Whether face is at boundary or not.
	 */
	inline bool boundary() const {
		return m_boundary;
	}

	//! Sets the start index of the vertices of the face.
	/*! \param vertexAndEdgeIndex The new start index for a vertex (index) loop,
	 *         \sa startVertexEdge(), -1 if it was not set.
	 *  \return The former start index of the vertices (vertex indices) of
	 *          TSubdivMesh::m_idxFaceVertices.
	 */
	inline long startVertexEdgeIndex(long vertexAndEdgeIndex) {
		long old = m_startVertexEdgeIndex;
		m_startVertexEdgeIndex = vertexAndEdgeIndex;
		return old;
	}

	//! Gets the start index of the vertices of the face.
	/*! \return The start index of the vertices (vertex indices) of
	 *          TSubdivMesh::m_idxFaceVertices, -1 if it was not set.
	 *
	 *          That's also the index of edge (index)
	 *          of TSubdivMesh::m_idxFaceEdges, because there is an edge for
	 *          each vertex i, leading to the next vertex (i+1)%vertices, forming
	 *          a loop enclosing the face. The loop is interpreted counter clockwise.
	 */
	inline long startVertexEdgeIndex() const {
		return m_startVertexEdgeIndex;
	}

	//! Sets the end index (+1) of the vertices of the face.
	/*! \param vertexAndEdgeIndex The new end index (+1) for a vertex (index) loop,
	 *         \sa startVertexEdge()
	 *  \return The former end index of the vertices (vertex indices) of
	 *          TSubdivMesh::m_idxFaceVertices, -1 if it was not set.
	 */
	inline long endVertexEdgeIndex(long vertexAndEdgeIndex) {
		long old = m_endVertexEdgeIndex;
		m_endVertexEdgeIndex = vertexAndEdgeIndex;
		return old;
	}

	//! Gets the end index (+1) of the vertices of the face
	/*! \return The end index (+1) of the vertices (vertex indices) of
	 *          TSubdivMesh::m_idxFaceVertices, -1 if it was not set.,
	 *          \sa startVertexEdge()
	 */
	inline long endVertexEdgeIndex() const {
		return m_endVertexEdgeIndex;
	}

	/*! \return The number of vertices grouped around the face. After the initial
	 *          subdivision, this should be always 4 for catmull-clark subdivision
	 *          meshes.
	 */
	inline long vertices() const {
		return m_endVertexEdgeIndex - m_startVertexEdgeIndex;
	}

	//! Sets the face index.
	/*! \param aFaceIndex The index of the face at the original subdivision mesh.
	 *  \return The former index of the face at the original subdivision mesh.
	 */
	inline long faceIndex(long aFaceIndex) {
		long old = m_faceIndex;
		m_faceIndex = aFaceIndex;
		return old;
	}

	//! Gets the face index.
	/*! \return The index of the face at the original subdivision mesh. -1 if not set.
	 */
	inline long faceIndex() const {
		return m_faceIndex;
	}

	//! Sets the face normal.
	/*! \param normal A reference to the normal vector of the face.
	 */
	inline void normal(const RtPoint &normal) {
		m_normal[0] = normal[0];
		m_normal[1] = normal[1];
		m_normal[2] = normal[2];
	}

	//! Gets the face normal.
	/*! \return A reference to the normal vector of the face.
	 */
	inline const RtPoint &normal() const {
		return m_normal;
	}
}; // TSubdivFacet


//! Prints the contents of the TSubdivFacet f to a stream by calling its dump member function.
/*! \param f TSubdivFacet to print to a stream.
 *  \param o Stream to print on.
 *  \param pre String used at the begining for each line, usually blanks as indent.
 */
inline void dump(const TSubdivFacet &f, std::ostream &o, const std::string &pre) {
	f.dump(o, pre);
}


//! Edge for subdivision.
/*! Each edge has two vertices and one or two adjacent faces (2-manifold). Edges can
 *  be ordered by the index numbers of their vertices. The orientation is
 *  given by the ordering of the vertices at the adjacent face - the edge itself has no
 *  orientation. The ordering of the vertex index numbers are stored in an TSubdivEdge instance,
 *  the condition m_oneVertex < m_otherVertex is always true and used by searching.
 *  The ordering of m_oneFace and m_otherFace is arbitrary and of no use. If there is only
 *  one adjacent face at a boundary, m_oneFace contains the face index and m_otherFace equals -1.
 *  The index numbers are those of the appropriate containers used in TSubdivMesh
 *  (TSubdivMesh::m_vertices for the vertices and TSubdivMesh::m_facets for the facets).
 */
class TSubdivEdge {
	friend class TSubdivMesh; //!< The TSubdivMesh is used to fill the instance members.

	//! The type of an edge (rounded or crease).
	enum TSubdivEdgeType {
		EDGE_ROUNDED, //!< Edge is rounded.
		EDGE_CREASE   //!< Edge is a crease.
	};

	RtFloat	m_value;        //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, gives the sharpness of a crease. The value blends from 0: smooth to 1: sharp.
	TSubdivEdgeType m_type; //!< Edge type: EDGE_ROUNDED or EDGE_CREASE.

	long m_oneVertex,   //! One vertex of the edge, index of TSubdivMesh::m_vertices, m_oneVertex < m_otherVertex.
              m_otherVertex; //! The other vertex of the edge, index of TSubdivMesh::m_vertices, m_oneVertex < m_otherVertex.
	long m_oneFace,     //! One adjacent face of the edge, index of TSubdivMesh::m_facets. The value is <0 if not initalized.
              m_otherFace;   //! The other adjacent face of the edge, index of TSubdivMesh::m_facets, m_otherFace == -1 and m_oneFace >= 0 if there is only one face adjacent to the edge.
public:
	//! Standard constructor, initializes all elements.
	inline TSubdivEdge() :
		m_value(0.0), m_type(EDGE_ROUNDED),
		m_oneVertex(-1L), m_otherVertex(-1L),
		m_oneFace(-1L), m_otherFace(-1L)
	{}

	//! Copy constructor, initializes with the values of edge.
	/*! \param edge Edge used for initialization.
	 */
	inline TSubdivEdge(const TSubdivEdge &edge) {
		*this = edge;
	}

	// Destructor, nothing to do - standard is used.
	// inline ~TSubdivEdge() {}

	//! Prints the contents of the instance to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	void dump(std::ostream &o, const std::string &pre) const;

	//! Gets a new instance as clone of *this.
	/*! \return A clone of *this.
	 *  \throw TSystemException if there is not enough memory.
	 */
	inline TSubdivEdge &duplicate() {
		TSubdivEdge *e = new TSubdivEdge(*this);
		if ( !e ) {
			throw new TSystemException(__FILE__, __LINE__);
		}
		return *e;
	}

	//! Assignment operator, assigns all values of e to *this.
	/*! \param e Values of this edge are assigned to *this.
	 *  \return A reference of the instance (*this).
	 */
	TSubdivEdge &operator=(const TSubdivEdge &e);

	//! Two edges are considered as equal, if they have the same vertex index numbers.
	/*! Because the vertex index numbers are ordered the comparation is done only for the
	 *  vertices with the same name.
	 *  \param e Edge to compare with.
	 *  \return true if the vertices of the edges are the same, false if not equal.
	 */
	inline bool operator==(const TSubdivEdge &e) const {
		return (m_oneVertex == e.m_oneVertex) && (m_otherVertex == e.m_otherVertex);
	}

	//! Comparation (*this grater as e) using the vertex index numbers (\sa operator==).
	/*!  \param e Edge to compare with.
	 *   \return true if the vertex index numbers of e are greater than the ones of *this, false if they are less or equal.
	 */
	inline bool operator>(const TSubdivEdge &e) const {
		return (m_oneVertex > e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex > e.m_otherVertex));
	}

	//! Comparation (*this grater or equal as e) using the vertex index numbers (\sa operator==).
	/*!  \param e Edge to compare with.
	 *   \return true if the vertex index numbers of e are greater or equal than the ones of *this, false if they are less.
	 */
	inline bool operator>=(const TSubdivEdge &e) const {
		return (m_oneVertex >= e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex >= e.m_otherVertex));
	}

	//! Comparation (*this less as e) using the vertex index numbers (\sa operator==).
	/*!  \param e Edge to compare with.
	 *   \return true if the vertex index numbers of e are less than the ones of *this, false if they are greater or equal.
	 */
	inline bool operator<(const TSubdivEdge &e) const {
		return (m_oneVertex < e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex < e.m_otherVertex));
	}

	//! Comparation (*this less or equal as e) using the vertex index numbers (\sa operator==).
	/*!  \param e Edge to compare with
	 *   \return true if the vertex index numbers of e are less or equal than the ones of *this, false if they are greater.
	 */
	inline bool operator<=(const TSubdivEdge &e) const {
		return (m_oneVertex <= e.m_oneVertex) || ((m_oneVertex == e.m_oneVertex) && (m_otherVertex <= e.m_otherVertex));
	}

	//! Inserts the two vertices of an edge.
	/*! The vertex index numbers are sorted,
	 *  so that m_oneVertex < m_otherVertex after assignment.
	 *
	 *  \param v1 A vertex index number of the edge.
	 *  \param v2 Another index number of the edge.
	 */
	inline void newEdge(long v1, long v2) {
		if ( v1 < v2 ) {
			m_oneVertex   = v1;
			m_otherVertex = v2;
		} else {
			m_oneVertex   = v2;
			m_otherVertex = v1;
		}
	}

	//! Inserts the index number of an adjacent face.
	/*! There are maximal two adjacent faces. m_oneFace is filled first.
	 *  \param f An index number of a face
	 *  \return true face was inserted, false if there where already two adjacent faces.
	 */
	inline bool insertFace(long f) {
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
	 *  \return The old edge type.
	 */
	inline TSubdivEdgeType type(TSubdivEdgeType aType) {
		TSubdivEdgeType old = m_type;
		m_type = aType;
		return old;
	}

	//! Gets the edge type.
	/*! \return The edge type, a round edge or a sharp crease.
	 */
	inline TSubdivEdgeType type() const {return m_type;}

	//! Sets the edge value, the sharpness of a crease.
	/*! \param aValue New edge value.
	 *  \return The old edge value.
	 */
	inline RtFloat value(RtFloat aValue) {
		RtFloat old = m_value;
		m_value = aValue;
		return old;
	}

	//! Gets the edge value, the sharpness of a crease.
	/*! \return The edge value.
	 */
	inline RtFloat value() const {return m_value;}

	//! Gets the neighbour vertex of an edge and aVertex.
	/*! \param aVertex A vertex of the edge.
	 *  \retval neighbour The Index of the neighbour of aVertex of the edge.
	 *  \return true A neighbour vertex is returned in neighbour, false otherwise neighbour left untouched.
	 */
	inline bool getNeighbourVertex(long &neighbour, long aVertex) const {
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
	inline bool getNeighbourFace(long &neighbour, long aFace) const {
		if ( aFace == m_oneFace )
			neighbour = m_otherFace;
		else if ( aFace == m_otherFace )
			neighbour = m_oneFace;
		else
			return false;

		return true;
	}

	//! Is the edge a boundary edge?
	/*! If there is only one adjacent face, the edge is a boundary. Since m_oneFace is
	 *  filled at first and there must be at least one adjacent face, the edge is
	 *  considered as a boundary if m_otherFace is negative.
	 *  Edges adjacent to a 'hole' face are not considered as boundary.
	 *  \return true: If the edge is a boundary. false: edge is inside the mesh (has two adjacent faces)
	 */
	inline bool isBoundary() const { return m_otherFace < 0; }

	//! Is the edge a sharp crease?
	/*! An edge is also a sharp crease if it is tagged as EDGE_CREASE.
	 *  \return true: If the edge is sharp, false: edge is not sharp.
	 */
	inline bool isSharp() const { return m_type == EDGE_CREASE; }

	//! Is the edge a boundary and not a sharp crease?
	/*! \return true unsharp boundary edge, false no boundary or sharp crease
	 */
	inline bool isUnsharpBoundary() const {
		return ((m_otherFace < 0) && (m_type != EDGE_CREASE));
	}
}; // TSubdivEdge


//! Prints the contents of the TSubdivEdge e to a stream by calling its dump member function.
/*! \param e TSubdivEdge to print to a stream.
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dump(const TSubdivEdge &e, std::ostream &o, const std::string &pre) {
	e.dump(o, pre);
}


//! Vertex for subdivision.
/*! Used for conectivity information only, point values etc. are not stored here.
 *  The vertices are stored in TSubdivMesh::m_vertices. The index of a vertex
 *  in this container is used as identification.
 */
class TSubdivVertex {
	friend class TSubdivMesh; //!< The TSubdivMesh is used to fill the TSubdivVertex instances.

	//! The type of a vertex (rounded or corner).
	enum TSubdivVertexType {
		VERTEX_ROUNDED, //!< Rounded vertex.
		VERTEX_CORNER   //!< Vertex is a corner.
	};

	RtFloat	m_value;          //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, belends between round vertex (m_value==0) and corner (m_value==1).
	TSubdivVertexType m_type; //!< Vertex type: VERTEX_ROUNDED or VERTEX_CORNER.

	// Incident edges
	long m_idxStartEdge;    //!< Index incident edge start, there must be two or more edges, points to TSubdivMesh::m_idxIncidentEdges.
	long m_idxEndEdge;      //!< End of incident edges, number of incident eges is m_idxEndEdge-m_idxStartEdge.
	long m_idxEdgeCnt;      //!< Auxillary variable used as counter while filling TSubdivMesh::m_idxIncidentEdges.

	// Adjacent surfaces
	long m_idxStartFace;    //!< Index list of adjacent faces (one or more), points to TSubdivMesh::m_idxAdjacentFacets.
	long m_idxEndFace;      //!< End of adjacent faces, number of adjacent faces is m_idxEndFace-m_idxStartFace.
	long m_idxFaceCnt;      //!< Auxillary variable used as counter while filling TSubdivMesh::m_idxAdjacentFacets.

	long m_perFaceIdx;      //!< Index used in per face arrays (temporary variable)
public:
	//! Standard constructor, clears all members.
	inline TSubdivVertex() :
		m_value(0.0), m_type(VERTEX_ROUNDED),
		m_idxStartEdge(0L), m_idxEndEdge(0L), m_idxEdgeCnt(0L),
		m_idxStartFace(0L), m_idxEndFace(0L), m_idxFaceCnt(0L)
	{
	}

	//! Copy constructor, initializes with the values of ver.
	/*! \param ver Vertex used for initialization.
	 */
	inline TSubdivVertex(const TSubdivVertex &ver) {
		*this = ver;
	}
	// Destructor, nothing to do - standard is used.
	// inline ~TSubdivVertex() {}

	//! Prints the contents of the instance to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	void dump(std::ostream &o, const std::string &pre) const;

	//! Gets a new instance as clone of *this.
	/*! \return A clone of *this.
	 *  \throw TSystemException if there is not enough memory.
	 */
	inline TSubdivVertex &duplicate() {
		TSubdivVertex *v = new TSubdivVertex(*this);
		if ( !v ) {
			throw new TSystemException(__FILE__, __LINE__);
		}
		return *v;
	}

	//! Assignment operator, assigns all values of v to *this.
	/*! \param v Values of this vertex are assigned to *this.
	 *  \return A reference of the instance (*this).
	 */
	TSubdivVertex &operator=(const TSubdivVertex &v);

	//! Sets the vertex type.
	/*! \param aType New vertex type.
	 *  \return The old vertex type
	 */
	inline TSubdivVertexType type(TSubdivVertexType aType) {
		TSubdivVertexType old = m_type;
		m_type = aType;
		return old;
	}

	//! Gets the vertex type.
	/*! \return The vertex type, a normal vertex or a corner.
	 */
	inline TSubdivVertexType type() const {return m_type;}

	//! Sets the vertex value, the sharpness of the corner.
	/*! \param aValue New vertex value.
	 *  \return The old vertex value.
	 */
	inline RtFloat value(RtFloat aValue) {
		RtFloat old = m_value;
		m_value = aValue;
		return old;
	}

	//! Gets the vertex value.
	/*! \return The vertex value, the sharpness of the corner.
	 */
	inline RtFloat value() const {return m_value;}
}; // TSubdivVertex


//! Prints contents of the TSubdivVertex v to a stream by calling its dump member function.
/*! \param v TSubdivVertex to print to a stream.
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dump(const TSubdivVertex &v, std::ostream &o, const std::string &pre) {
	v.dump(o, pre);
}


//! Surfaces of a face of a subdivision mesh
class TSubdivPrimFace : public TPrimFace {
public:
	long m_nFacets;    //!< Number of the facets of the face
	long m_nTriangles; //!< Number of the triangles of the face

	//! Standard constructor, initializes a new TSubdivPrimFace
	inline TSubdivPrimFace() : m_nFacets(0), m_nTriangles(0) {}

	//! Copy constructor, sets all to the values of spf.
	/*! \param spf Values to copy from.
	 */
	inline TSubdivPrimFace(const TSubdivPrimFace &spf) : m_nFacets(0), m_nTriangles(0) {
		*this = spf;
	}

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TSubdivPrimFace *retVal = new TSubdivPrimFace(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}

	//! Assignment operator
	/*! \param spf values to copy from.
	 *  \return *this
	 */
	TSubdivPrimFace &operator=(const TSubdivPrimFace &spf) {
		if ( this == &spf ) {
			return *this;
		}
		m_nFacets = spf.m_nFacets;
		m_nTriangles = spf.m_nTriangles;
		TPrimFace::operator=(spf);
		return *this;
	}
}; // TSubdivPrimFace


//! Subdivision mesh for one tesselation.
/*! An instance of this class is used to store the vertex, edge and face
 *  connectivity information of a single subdivision mesh. It can be
 *  initialized by the parameters used for a TRi::subdivisionMesh() call
 *  (via TSubdivMesh::initialize()) or by a
 *  subdivision of an instance with TSubdivMesh::subdivideCatmullClark().
 */
class TSubdivMesh { // : public TPrimFace {
	friend class TSubdivisionMesh; //!< Used to fill initial m_intervertexData and m_varyingData.

public:
	std::vector<TSubdivVertex> m_vertices; //!< All vertices of the subdivision mesh, the index is the one of the values in the parameter lists.
	std::vector<TSubdivEdge>   m_edges;    //!< All edges of the subdivision mesh.
	std::vector<TSubdivFacet>  m_facets;  //!< All facets of the subdivision mesh.

	std::vector<long> m_idxFaceVertices;   //!< Indices of m_vertices, vertices of all faces.
	std::vector<long> m_idxFaceEdges;      //!< Indices of m_edges, edges of all faces.
	std::vector<long> m_idxAdjacentFacets; //!< Indices of m_facets, adjacent faces of all vertices.
	std::vector<long> m_idxIncidentEdges;  //!< Indices of m_edges for all incident edges of all vertices of the mesh.

	std::deque< TNamedVector<RtFloat> > m_interVertexData; //!< Float data for each vertex, interpolated
	std::deque< TNamedVector<RtFloat> > m_varyingData;     //!< Float data interpolated linearily.

	TNamedVector<RtFloat> m_vertexNormalData;   //!< Normal data for each vertex
	std::vector< TSubdivPrimFace > m_primFaces; //!< Data to render for each face

private:
	long m_sumVerticesPerFace;             //!< Sum of the number of vertices in all faces, it is also the number of edges per face.

	//! Gets the number of incident boundary edges of a vertex.
	/*! Boundary edge is an edge with only one adjacent face. This member function is
	 *  used to find corners.
	 *  \param vertexIdx The vertex index (vertexIdx >= 0)
	 *  \return Number of incident boundary edges.
	 */
	long incidentBoundaryEdges(long vertexIdx) const;

	//! Gets the number of incident sharp creases of a vertex.
	/*! This member function is used to find corners. Sharp edges are those tagged as TSubdivEdge::EDGE_CREASE.
	 *  \param vertexIdx The vertex index (vertexIdx >= 0).
	 *  \return Number of incident sharp edges.
	 */
	long incidentSharpEdges(long vertexIdx) const;

	//
	long unsharpBoundaryEdges(long faceIdx);

	//! \return true if name is N, P, Pw, st, s or t - values need not to be copied
	bool isSpecial(const char *name) const;

public:
	//! Standard constructor, initializes all elements (nothing to do).
	inline TSubdivMesh() :
		m_sumVerticesPerFace(0)
	{}

	//! Copy constructor, initializes with the values of face.
	/*! \param face Face used for initialization.
	 */
	inline TSubdivMesh(const TSubdivMesh &mesh) {
		*this = mesh;
	}

	// Destructor, nothing to do - standard is used.
	inline virtual ~TSubdivMesh() {}

	//! Prints the contents of the instance to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	void dump(std::ostream &o, const std::string &pre) const;

	//! Gets a new instance as clone of *this.
	/*! \return A clone of *this.
	 *  \throw TSystemException if there is not enough memory.
	 */
	inline virtual TSubdivMesh &duplicate() {
		TSubdivMesh *m = new TSubdivMesh(*this);
		if ( !m ) {
			throw new TSystemException(__FILE__, __LINE__);
		}
		return *m;
	}

	//! Assignment operator, assigns all values of m to *this.
	/*! \param m Values of this mesh are assigned to *this.
	 *  \return A reference of the instance (*this).
	 */
	TSubdivMesh &operator=(const TSubdivMesh &m);

	//! Gets the edge index of the edge with the end verticess v1 and v2.
    /*! \retval e Edge index found.
	 *  \param v1 A vertex of the edge (v1 >= 0).
	 *  \param v2 A vertex of the edge (v2 >= 0).
	 *  \return true if an edge is found, false if there is no edge (v1, v2).
	 */
	inline bool getEdgeIndex(long &e, long v1, long v2) const;

	//! Initialization of the connectivity of the subdivision mesh.
	/*! The mesh must be valid for Catmull-Clark subdivision: An edge has
	 *  one or two adjacent faces (2-manifold), each vertex has at least two incidend edges.
	 *  The parameters are those of the TRi::subdivisionMesh() call. This member
	 *  function is only called once per TRi::subdivisionMesh() to initialize
	 *  the structure. TSubdivMesh::subdivideCatmullClark() is used to get
	 *  the indices for further subdivisions.
	 *
	 *  \param nfaces  Number of faces.
	 *  \param nverts  Number of vertices per face.
	 *  \param verts   Vertex indices.
	 *  \param ntags   Number of tags.
	 *  \param tags    Names of the tags.
	 *  \param nargs   Number of arguments (two per tag: intargs, floargs).
	 *  \param intargs Integer arguments of the tags.
	 *  \param floargs Float arguments of the tags.
	 */
	void initialize(
		RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[],
		RtInt nargs[], RtInt intargs[], RtFloat floargs[]
	);

	//! Calculates the connectivity (face, edge, vertex) of a subdivision step
	//! using the catmull-clark subdivision scheme.
	/*! \param aMesh Mesh with the prior subdivision step.
	 */
	void subdivideCatmullClarkConnectivity(const TSubdivMesh &aMesh);

	//! Performs a subdivision step for "vertex" values.
	/*! \param aMesh Subdivision mesh of the previous subdivision step.
	 *  \param values Values used for subdivison.
	 */
	void subdivideCatmullClark(
		const TSubdivMesh &aMesh,
		const TNamedVector<RtFloat> &values);

	//! Performs a linear interpolation step for "varying" values.
	/*! \param aMesh Subdivision mesh of the previous subdivision step.
	 *  \param values Values used for subdivison
	 */
	void subdivideCatmullClarkLinear(
		const TSubdivMesh &aMesh,
		const TNamedVector<RtFloat> &values);

	//! Marks all boundary faces.
	/*! Non interpolated boundaries wiill not be drawn.
	 */
	inline void markBoundaryFaces() {
		for ( long i = 0; i < (long)m_facets.size(); ++i )
			if ( unsharpBoundaryEdges(i) > 0 )
				m_facets[i].boundary(true);
	}

	//! Calculates the face normals, using the first three vertices.
	void calcFaceNormals();

	//! Calculates the vertex normals, using the faceNormals.
	void calcVertexNormals();

	//
	bool initPrimFaces(RtInt nfaces, TSubdivisionMesh &prim);

	//
	void fillPrimFaces(std::vector< TSubdivPrimFace > &primFaces, RtInt nfaces, bool calcMissing);

	//! Fills the TPrimFace faces for the output (e.g. OpenGL).
	/*! \param nfaces Number of the faces of the control hull
	 *  \param prim   The subdivision mesh
	 */
	void fillPrimFaces(RtInt nfaces, TSubdivisionMesh &prim);

	//! The number of value arrays to interpolate.
	/*! If the result is 0, no subdivision has to be done.
	 * \\return Number of value arrays to interpolate.
	 */
	long interpolatedArrays() {
		return (long)(m_interVertexData.size()+m_varyingData.size());
	}
}; // TSubdivMesh


//! Prints contents of the TSubdivMesh m to a stream by calling its dump member function.
/*! \param m TSubdivMesh to print to a stream.
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dump(const TSubdivMesh &m, std::ostream &o, const std::string &pre) {
	m.dump(o, pre);
}


//! Container for all subdivision steps of a subdivision surface.
class TSubdivisionMesh : public TRiSubdivisionMesh {
	bool m_hasFaceData;

	//! \return true if has face vertex or face varying values, sets m_hasFaceData
	bool hasFaceData();

	//! Deque of subdivison meshes of subdivision meshes, index == number of subdivisions.
	std::deque<TSubdivMesh> m_meshes;

	//! Indices of the vertices of the face vertex or face varying class.
	std::vector<RtInt> m_faceVerts;

	//! Initializes members for the initial mesh
	void insertVertexData(long meshIdx, bool isPerFace);

	//! Depth for a subdivision.
	/*! \param x Number in tesselations in u directions.
	 *  \param y Number in tesselations in v directions.
	 *  \return Number of subdivisions (log2(max(x,y)).
	 */
	unsigned int subdivDepth(RtFloat x, RtFloat y) const;
public:
	//! Constructor, initializes the first mesh.
	/*! \param aLineNo Line number of a RIB file.
	 *  \param declmap Map with all valid declarations.
     *  \param colorComps Componets of a color value.
     *  \param scheme Subdivision schem, "catmull-clark" only.
     *  \param nfaces Number of the faces of the primary polygon mesh.
     *  \param nverts Number of vertices for each face.
     *  \param verts Vertex indices for each face.
     *  \param ntags Number of tags.
     *  \param tags (hole, crease, corner).
     *  \param nargs 2 records arguments for each tag (number of integer and float arguments).
     *  \param n Number of tokens and params.
     *  \param tokens Array of tokens.
     *  \param params Array of parameters.
     */
	inline TSubdivisionMesh(long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[]
	) : TRiSubdivisionMesh (aLineNo, declmap, colorComps,
		scheme,	nfaces, nverts, verts,
		ntags, tags, nargs, intargs, floargs,
		n, tokens, params)
	{
		m_hasFaceData = false;
		m_faceVerts.resize(m_nedges);
		RtInt i;
		for ( i = 0; i < m_nedges; ++i )
			m_faceVerts[i] = i;

		i = 0;

		if ( hasFaceData() ) {
			m_meshes.resize(2);
			m_meshes[i].initialize(nfaces, nverts, &(m_faceVerts[0]), 0, tags, nargs, intargs, floargs);
			insertVertexData(i, true);  // Per face data
			++i;
		} else {
			m_meshes.resize(1);
		}
		m_meshes[i].initialize(nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs);
		insertVertexData(i, false); // Meshed data
	}

	// Destructor, no special purposes.
	inline virtual ~TSubdivisionMesh() {
	}

	//! Subdivide.
	/*! \param x Number in tesselations in u directions.
	 *  \param y Number in tesselations in v directions.
	 *  \return Number of subdivisions (log2(max(x,y)).
	 */
	TSubdivMesh &subdivide(RtFloat x, RtFloat y);

	//! Prints the contents of the instance to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	void dump(std::ostream &o, const std::string &pre) const;

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			if ( this->m_nfaces > 0 ) {
				TSubdivMesh &sm = subdivide(ricb.curUTesselation(), ricb.curVTesselation());
				std::vector< TSubdivPrimFace >::iterator i;
				for ( i = sm.m_primFaces.begin(); i != sm.m_primFaces.end(); i++ ) {
					dp->drawGeometricPrim(*i);
				}
			}
		} else {
			TRiSubdivisionMesh::replay(ri);
		}
	}
}; // TSubdivisionMesh


//! Prints contents of the TSubdivisionMesh s subdivision collection
//! to a stream by calling its dump member function.
/*! \param m TSubdivMesh to print to a stream.
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dump(const TSubdivisionMesh &s, std::ostream &o, const std::string &pre) {
	s.dump(o, pre);
}

#endif // _RICPP_RENDERCONTEXT_SUBDIVISION_H
