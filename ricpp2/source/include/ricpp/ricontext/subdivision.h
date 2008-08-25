#ifndef _RICPP_RICONTEXT_SURFACE_H
#define _RICPP_RICONTEXT_SURFACE_H

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

#ifndef _RICPP_RICPPBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif _RICPP_RICPPBASE_RICPPTYPES_H

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
		 * @brief Incident vertices and edges (for each vertex one edge).
		 * @todo Special handling for corner vertices and those incident to sharp edges (normals per face). May need separated vertex and edge indices.
		 */
		long m_startVertexEdgeIndex; //!< Start index of Incident vertices and edges, index for TSubdivMesh::m_idxFaceVertices and TSubdivMesh::m_idxFaceEdges. -1 if not initialised.
		long m_endVertexEdgeIndex;   //!< End of incident vertices and edges, m_endVertexEdgeIndex - m_startVertexEdgeIndex = number of vertices and edges of the face. -1 if not initialised.
		/** @}
		 */
		
		long m_index;                //!< Index used for hierarchical path.
		long m_origFaceIndex;        //!< Index of the face in the original polygon mesh, -1 if not initialised.
		
		RtPoint m_normal;            //!< The normal vector of the face.
		
	public:		
		//! Standard constructor, initializes all elements.
		inline CSubdivFace() :
		m_type(FACE_FILLED),
		m_startVertexEdgeIndex(-1L), m_endVertexEdgeIndex(-1L),
		m_origFaceIndex(-1L), m_index(-1L)
		{
			m_normal[2] = m_normal[1] = m_normal[0] = 0.0;
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
		inline CSubdivFace *duplicate()
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
		
		//! Sets the start index of the vertices of the face.
		/*! \param vertexAndEdgeIndex The new start index for a vertex (index) loop,
		 *         \sa aVertexAndEdgeIndex(), -1 if it was not set.
		 */
		inline void startVertexEdgeIndex(long aVertexAndEdgeIndex)
		{
			m_startVertexEdgeIndex = aVertexAndEdgeIndex;
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
		inline long startVertexEdgeIndex() const
		{
			return m_startVertexEdgeIndex;
		}
		
		//! Sets the end index (+1) of the vertices of the face.
		/*! \param aVertexAndEdgeIndex The new end index (+1) for a vertex (index) loop,
		 *         \sa startVertexEdge()
		 */
		inline void endVertexEdgeIndex(long aVertexAndEdgeIndex)
		{
			m_endVertexEdgeIndex = aVertexAndEdgeIndex;
		}
		
		//! Gets the end index (+1) of the vertices of the face
		/*! \return The end index (+1) of the vertices (vertex indices) of
		 *          TSubdivMesh::m_idxFaceVertices, -1 if it was not set.,
		 *          \sa startVertexEdge()
		 */
		inline long endVertexEdgeIndex() const
		{
			return m_endVertexEdgeIndex;
		}
		
		/*! \return The number of vertices grouped around the face. After the initial
		 *          subdivision, this should be always 4 for catmull-clark subdivision
		 *          meshes.
		 */
		inline long vertices() const
		{
			return m_endVertexEdgeIndex - m_startVertexEdgeIndex;
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
		
		//! Sets the face index for the hierarchical path.
		/*! \param aFaceIndex face index for the hierarchical path.
		 */
		inline void index(long anIndex)
		{
			m_index = anIndex;
		}
		
		//! Gets the face index for the hierarchical path.
		/*! \return Face index for the hierarchical path.
		 */
		inline long index() const
		{
			return m_index;
		}
		
		//! Sets the face normal.
		/*! \param aNormal A reference to the normal vector of the face.
		 */
		inline void normal(const RtPoint &aNormal)
		{
			m_normal[0] = aNormal[0];
			m_normal[1] = aNormal[1];
			m_normal[2] = aNormal[2];
		}
		
		//! Gets the face normal.
		/*! \return A reference to the normal vector of the face.
		 */
		inline const RtPoint &normal() const
		{
			return m_normal;
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
	
} // namespace RiCPP

#endif
