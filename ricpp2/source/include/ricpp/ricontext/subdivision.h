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
		long m_startVertexIndex; //!< Start index of incident vertices (and edges) in the mesh, normally cw, -1 if not initialised
		long m_endVertexIndex;   //!< Endindex+1 of incident vertices (and edges) in the mesh (-1 if not initialised), m_endVertexIndex - m_startVertexIndex = number of vertices incident to the face.
		/** @}
		 */
		
		/** @{
		 * @name Hierarchy
		 */
		long m_parentFaceIndex; //!< Index of the parent, -1 if not initialized (root)
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
		m_parentFaceIndex(-1L),
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
			return startVertexIndex() + (anIndex - startVertexIndex() + 1) % nVertices();
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

		void insertHoleVal(long idx, const CRiHierarchicalSubdivisionMesh &obj);
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
		
		long m_oneVertex,   //! One incident vertex of the edge, only on initialization: m_oneVertex < m_otherVertex.
		m_otherVertex;      //! The other incident vertex of the edge, only on initialization: m_oneVertex < m_otherVertex.
		long m_oneFace,     //! Index of an incident face (having the edge: m_oneVertex->m_otherVertex). The value is <0 if not initalized.
		m_otherFace;        //! The other incident face of the edge (having the edge: m_otherVertex->m_oneVertex), m_otherFace == -1 and m_oneFace >= 0 if there is only one face incident to the edge.
	public:
		//! Standard constructor, initializes all elements.
		inline CSubdivEdge() :
		m_value(0.0), m_type(EDGE_ROUNDED),
		m_oneVertex(-1L), m_otherVertex(-1L),
		m_oneFace(-1L), m_otherFace(-1L)
		{}
		
		inline CSubdivEdge(long aStartIdx, long anEndIdx, long incidentFace) :
		m_value(0.0), m_type(EDGE_ROUNDED)
		{
			if ( aStartIdx < anEndIdx ) {
				m_oneFace = incidentFace;
				m_otherFace = -1;
				m_oneVertex = aStartIdx;
				m_otherVertex = anEndIdx;
			} else {
				m_oneFace = -1;
				m_otherFace = incidentFace;
				m_oneVertex = anEndIdx;
				m_otherVertex = aStartIdx;
			}
		}

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
		
		//! Two edges are considered as equal, if they have the same vertex index numbers.
		/*! Because the vertex index numbers are ordered, the comparation is done only for the
		 *  vertices with the same name.
		 *  \param e Edge to compare with.
		 *  \return true if the vertices of the edges are the same, false if not equal.
		 */
		inline bool operator==(const CSubdivEdge &e) const
		{
			return (m_oneVertex == e.m_oneVertex) && (m_otherVertex == e.m_otherVertex);
		}
		
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
		/*! There are maximal two incident faces.
		 *  \param startVertex
		 *  \param endVertex
		 *  \param f An index number of a face
		 *  \return true face was inserted, false if there where already two incident faces.
		 */
		inline bool insertFace(long startVertex, long endVertex, long f)
		{
			if ( m_oneVertex == startVertex ) {
				if ( m_otherVertex != endVertex || m_oneFace != -1 ) {
					// illegal topology
					return false;
				}
				m_oneFace = f;
				return true;
			}
			if ( m_oneVertex == endVertex ) {
				if ( m_otherVertex != startVertex || m_otherFace != -1 ) {
					// illegal topology
					return false;
				}
				m_otherFace = f;
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
		
		//! Gets the adjacent vertex of a vertex and an edge.
		/*! \param aVertex A vertex of the edge.
		 *  \retval adjacent The Index of the adjacent (neighbour) of aVertex and the edge.
		 *  \return True, A neighbour vertex is returned in adjacent (-1 if not existant).
		 *          False, if aVertex is not incident to the edge, adjacent left untouched.
		 */
		inline bool getAdjacentVertex(long &adjacent, long aVertex) const
		{
			if ( aVertex == m_oneVertex )
				adjacent = m_otherVertex;
			else if ( aVertex == m_otherVertex )
				adjacent = m_oneVertex;
			else
				return false;
			
			return true;
		}
		
		//! Gets the adjacent face of another face and the edge.
		/*! \param aFace A face of the edge.
		 *  \retval adjacent The Index of the neighbour (adjacent) of aFace of the edge.
		 *  \return true: A neighbour face index is returned in adjacent (-1 if not existant).
		 *          false: aFace is not incident to the edge, the parameter adjacent is left untouched.
		 */
		inline bool getAdjacentFace(long &adjacent, long aFace) const
		{
			if ( aFace == m_oneFace )
				adjacent = m_otherFace;
			else if ( aFace == m_otherFace )
				adjacent = m_oneFace;
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
			return m_oneFace < 0 || m_otherFace < 0;
		}
		
		//! Is the edge a crease?
		/*! An edge is a sharp crease if it is tagged as EDGE_CREASE.
		 *  \return true: If the edge is a crease, false: edge is not a crease.
		 */
		inline bool isCrease() const
		{
			return m_type == EDGE_CREASE;
		}
		
		//! Is the edge a sharp crease?
		/*! An edge is a sharp crease if it is tagged as EDGE_CREASE having a m_value of RI_INFINITY
		 *  \return true: If the edge is sharp, false: edge is not sharp.
		 */
		inline bool isSharp() const
		{
			return m_type == EDGE_CREASE && m_value == RI_INFINITY;
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
			return (v1 == m_oneVertex && v2 == m_otherVertex) || (v2 == m_oneVertex && v1 == m_otherVertex);
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
		//! The type of a vertex (rounded or corner).
		enum CSubdivVertexType {
			VERTEX_ROUNDED, //!< Rounded vertex.
			VERTEX_CORNER   //!< Vertex is a corner.
		};
		
		RtFloat	m_value;          //!< Tag value copied from appropriate TRi::subdivisionMesh parameter, belends between round vertex (m_value==0) and corner (m_value==1).
		CSubdivVertexType m_type; //!< Vertex type: VERTEX_ROUNDED or VERTEX_CORNER.
		
		// Parameters have the same index as the vertex, face vertices, corners and vertices incident to sharp creases have additional parameters (of the same value)

	public:
		//! Standard constructor, clears all members.
		inline CSubdivVertex() :
		m_value(0.0), m_type(VERTEX_ROUNDED)
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

		inline void corner(RtFloat aValue)
		{
			m_value = aValue;
			m_type = VERTEX_CORNER;
		}
		
		//! Gets the vertex value.
		/*! \return The vertex value, the sharpness of the corner.
		 */
		inline RtFloat value() const
		{
			return m_value;
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
	
	class CSubdivisionIndices;
	
	class CSubdivisionStrategy {
	public:
		inline virtual ~CSubdivisionStrategy() {}
		virtual void subdivide(CSubdivisionIndices &parent, CSubdivisionIndices &child) const = 0;
	};
	
	class CCatmullClarkSubdivision : public CSubdivisionStrategy {
		virtual void subdivide (CSubdivisionIndices &parent, CSubdivisionIndices &child) const;
	};

	class CSubdivisionStrategies : public TemplObjPtrRegistry<RtToken, CSubdivisionStrategy *> {
	public:
		inline CSubdivisionStrategies() : TemplObjPtrRegistry<RtToken, CSubdivisionStrategy *>(true) {};
	};
	
	class CSubdivisionIndices {
	private:
		std::vector<CSubdivFace>   m_faces;
		std::vector<long>          m_edgeIndices;
		std::vector<CSubdivEdge>   m_edges;
		std::vector<long>          m_vertexIndices;
		std::vector<CSubdivVertex> m_vertices;
		RtInt m_interpolateBoundary;
		bool m_illTopology;
		CSubdivisionIndices *m_parent;
		void insertBoundaryVal(const CRiHierarchicalSubdivisionMesh &obj);
	public:
		inline CSubdivisionIndices() : m_illTopology(false), m_interpolateBoundary(0), m_parent(0) {}
		//! Initialization of the connectivity of the subdivision mesh.
		/*! The mesh must be valid for Catmull-Clark subdivision: An edge has
		 *  one or two adjacent faces (2-manifold), each vertex has at least two incidend edges.
		 *  The parameters are those of the TRi::subdivisionMesh() call. This member
		 *  function is only called once per TRi::subdivisionMesh() to initialize
		 *  the structure. TSubdivMesh::subdivideCatmullClark() is used to get
		 *  the indices for further subdivisions.
		 *
		 *  nverts  Number of vertices per face, size is the number of faces
		 *  verts   Vertex indices.
		 *  ntags   Number of tags.
		 *  tags    Names of the tags.
		 *  nargs   Number of arguments (three per tag: intargs, floargs, stringargs).
		 *  intargs Integer arguments of the tags.
		 *  floargs Float arguments of the tags.
		 *  stringargs String parameters of the tags.
		 */
		void initialize(const CRiHierarchicalSubdivisionMesh &obj);
		
		CSubdivFace *face(long faceIndex);
		inline std::vector<CSubdivFace> &faces() {return m_faces;}
		inline const std::vector<CSubdivFace> &faces() const {return m_faces;}

		CSubdivEdge *edge(long faceIndex, long edgeIndex);
		inline std::vector<CSubdivEdge> &edges() {return m_edges;}
		inline const std::vector<CSubdivEdge> &edges() const {return m_edges;}
		inline std::vector<long> &edgeIndices() {return m_edgeIndices;}
		inline const std::vector<long> &edgeIndices() const {return m_edgeIndices;}

		CSubdivVertex *vertex(long faceIndex, long vertexIndex);
		inline std::vector<CSubdivVertex> &vertices() {return m_vertices;}
		inline const std::vector<CSubdivVertex> &vertices() const {return m_vertices;}
		inline std::vector<long> &vertexIndices() {return m_vertexIndices;}
		inline const std::vector<long> &vertexIndices() const {return m_vertexIndices;}
		
		void subdivide(CSubdivisionIndices &aParent, const CSubdivisionStrategy &aStrategy,
						const CRiHierarchicalSubdivisionMesh &anObj);
		inline bool illTopology() const { return m_illTopology; }
		inline RtInt interpolateBoundary() const { return m_interpolateBoundary; }
		inline const CSubdivisionIndices *parent() const { return m_parent; }
	};
	
	class CSubdivisionHierarchieTesselator  : public CTesselator {
	private:
		std::list<CSubdivisionIndices> m_indices;
		CRiHierarchicalSubdivisionMesh m_obj;
		const CSubdivisionStrategies &m_strategies;		

		void subdivide(IndexType minDepth);
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			return m_obj;
		}
		
	public:
		inline CSubdivisionHierarchieTesselator(CRiHierarchicalSubdivisionMesh &anObj, const CSubdivisionStrategies &theStrategies)
			: m_obj(anObj), m_strategies(theStrategies)
		{
		}
		inline CSubdivisionHierarchieTesselator(CRiSubdivisionMesh &anObj, const CSubdivisionStrategies &theStrategies)
			: m_obj(anObj), m_strategies(theStrategies)
		{
		}

		inline virtual const CSubdivisionStrategies &strategies() { return m_strategies; }
		inline virtual const std::list<CSubdivisionIndices> &indices() const { return m_indices; }

		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
};
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_SUBDIVISION_H
