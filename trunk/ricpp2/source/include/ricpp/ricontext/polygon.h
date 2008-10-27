#ifndef _RICPP_RICONTEXT_POLYGON_H
#define _RICPP_RICONTEXT_POLYGON_H

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

/** @file polygon.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Header for polygon triangulation. 
 *  @see Triangulation by Ear Clipping,
 *       David Eberly 2008 http://www.geometrictools.com/
 */

#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif // _RICPP_RIBASE_RICPPTYPES_H

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif // _RICPP_RICPP_TYPES_H

#include <cassert>
#include <vector>

namespace RiCPP {

// =============================================================================
/** @brief Node of a list being a single vertex of a polygon if connected,
 *         helper class used by CPolygonContainer.
 *  @todo Generalize for doubly linked lists stored in arrays,
 *        if needed somewhere.
 */
class CPolygonNode {
private:
	friend class CPolygonContainer;
	
	// Links
	IndexType m_next,  ///< Next node in circular list of polygon outline.
	m_prev;            ///< Previous node in circular list of polygon outline.
	
	// Payload
	IndexType m_index; ///< Index of an array with vertex indices (double indirection for varying) and single indirection for face varying.
	IndexType m_dup;   ///< Duplicated index, used if a hole is inserted into a boundary loop via new a bridge edge (two new vertices, copied from existing ones) as endpoints are needed).
	RtFloat m_p[2];    ///< 2D coordinates of the vertex for a major and a minor axis.
	RtFloat m_dotp90;  ///< Determinant, dot product of the join (prev-this-next) for being left of or being right of, and reflex calculation.
	bool m_reflex;     ///< Join is a reflex vertex, depends on m_dotp90 and orientation of the polygon outline, @see CPolygonNode::recalc().
	
public:
	/** @brief Standard constructor, clear @c m_next and @c m_prev to indicate
	 *         unlinked state.
	 */
	inline CPolygonNode()
	{
		m_next = m_prev = 0;
		m_dup = 0;
	}
	
	/** @brief Gets Index of the own node.
	 *  @return Index of the own node.
	 */
	inline IndexType index() const { return m_index; }

	/** @brief Gets Index of the next node.
	 *  @return Index of the next node.
	 */
	inline IndexType next() const { return m_next; }
	
	/** @brief Gets Index of the previous node.
	 *  @return Index of the previous node.
	 */
	inline IndexType prev() const { return m_prev; }
	
	/** @brief Gets Index of a duplicated node (inserted by joining holes).
	 *  @return Index of the duplicated node.
	 */
	inline IndexType dup() const { return m_dup; }

	/** @brief Recalculate dot product and reflex state of the vertex.
	 *
	 *  Vertex has to be part of a polygon outline (being linked).
	 *  @param nodes All nodes (part of CPolygonContainer).
	 *  @param isCCW outline's orientation is counterclockwise.
	 */
	inline void recalc(std::vector<CPolygonNode> &nodes, bool isCCW)
	{
		RtFloat dotp = dot2_90_pos(nodes[m_prev].m_p, m_p, nodes[m_next].m_p);
		m_dotp90 = dotp;
		if ( isCCW ) {
			m_reflex = sign(dotp) >= 0.0;
		} else {
			m_reflex = sign(dotp) <= 0.0;
		}
	}
	
	/** @brief Inserts this node after another of a polygon outline,
	 *         and recalc().
	 *
	 *  @param nodes Array of all nodes (part of CPolygonContainer).
	 *  @param myNode position of this node in @a pn.
	 *  @param idx After node at @idx, this node will be inserted.
	 *  @param isCCW outline's orientatin is counterclockwise.
	 */
	inline void insertAfter(
							std::vector<CPolygonNode> &nodes,
							IndexType myIdx,
							IndexType idx,
							bool isCCW)
	{
		m_next = nodes[idx].m_next;
		m_prev = idx;
		
		nodes[m_prev].m_next = myIdx;
		nodes[m_next].m_prev = myIdx;
		
		nodes[m_prev].recalc(nodes, isCCW);
		recalc(nodes, isCCW);
		nodes[m_next].recalc(nodes, isCCW);
	}
	
	/** @brief Requests whether node is removed.
	 *
	 *  @return true, if node is removed from any outline.
	 */
	inline bool removed() const
	{
		return m_next == 0 && m_prev == 0;
	}
	
	/** @brief Removes node from it's outline and recalc().
	 *
	 *  @param nodes All nodes (part of CPolygonContainer).
	 *  @param isCCW outline's orientatin is counterclockwise.
	 */
	inline void remove(std::vector<CPolygonNode> &nodes, bool isCCW)
	{
		nodes[m_prev].m_next = m_next;
		nodes[m_next].m_prev = m_prev;
		
		nodes[m_prev].recalc(nodes, isCCW);
		nodes[m_next].recalc(nodes, isCCW);
		m_next = 0;
		m_prev = 0;
	}
	
	/** @brief Tests whether vertex is reflex.
	 *
	 *  @return true, vertex is reflex.
	 */
	inline bool reflex() const
	{
		return m_reflex;
	}
	
	/** @brief Accesses the 2D coordinates of the vertex.
	 *
	 *  @param idx Index of the 2D coordinate (0 or 1).
	 *  @return Coordinate for @a idx.
	 */
	inline RtFloat &operator[](int idx)
	{
		assert( idx == 0 || idx == 1 );
		return m_p[idx];
	}
	
	/** @brief Accesses the 2D coordinates of the vertex (read only).
	 *
	 *  @param idx Index of the 2D coordinate (0 or 1).
	 *  @return Constant coordinate for @a idx.
	 */
	inline const RtFloat &operator[](int idx) const
	{
		assert( idx == 0 || idx == 1 );
		return m_p[idx];
	}
	
	/** @brief Gets read acces to the two position coordinates.
	 *
	 *  @return Pointer (read only) to the first coordinate.
	 */
	inline const RtFloat *pos() const
	{
		return m_p;
	}
}; // CPolygonNode


// =============================================================================
/** @brief Contains an index of a special node.
 *
 *  Helper class, used to sort nodes of holes in descending x order.
 */
class CPolygonNodeId {
public:
	IndexType m_offset;   ///< Start index of a circular linked hole outline
	IndexType m_idx;      ///< Index of a special node within the outline
	std::vector<CPolygonNode> *m_nodes; ///< Container of all nodes
}; // CPolygonNodeId


// =============================================================================
/** @brief Container for a general (but simple, planar) polygon. As used by RMan.
 *
 *  Holes (inner polygons) will be joined with the outer polygon.
 */
class CPolygonContainer {
private:
	/** @brief Node for each vertex (index) of the polygons (1 outer and 0-n inner)
	 *
	 *  Two nodes are added for each inner loop (bridge vertices from holes to the outer polygon),
	 *  just in front of each hole. The indices of each node aare in consecutive order and
	 *  will be linked circularily. The holes will be integrated (using the two additional
	 *  nodes) with the outer polygon by calls to integrateHole().
	 *
	 *  The first element (index == 0) is empty, index 0 act as NULL.
	 */
	std::vector<CPolygonNode> m_nodes;
	std::vector<IndexType> m_outlines; ///< Outlines of the outer and inner polygons, will be joined to one polygon
	bool m_outlineIsCCW;                   ///< Indicates the sense of the polygon, true for counter clockwise

	/** @brief Links the nodes of of a polygon circularily
	 *
	 *  Links the nodes of the vertices of a polygon in the @c m_nodes vertex.
	 *  The nodes of one loop are from m_nodes[nodeoffs] and m_nodes[nodeoffs+nVertices].
	 *  The original inderect vertex index is stored in each node for later reference.
	 *
	 *  @param nodeoffs Start of the nodes in @c m_nodes
	 *  @param vertoffs Start of the vertices in the original vertex array of the polygon
	 *                  used to rember the original indirect vertex indices.
	 *  @param nVertices Number of vertices, has to be >= 3
	 *
	 */
	void circularLink(IndexType nodeoffs,
					  IndexType vertoffs,
					  IndexType nVertices);

	/** Gets the node index for the rightmost (lower) vertex of a loop.
	 *
	 *  @param offset Start node (index for @c m_nodes).
	 *  @return Node index (@c m_nodes) of the rightmost vertex.
	 */
	IndexType rightmostVertex(IndexType offset) const;

	/** @brief Finds the winding sense of a loop for a given rightmost vertex.
	 *
	 *  @param offset Start node.
	 *  @param rightmost The rightmost vertex of a node.
	 *
	 */
	bool isCCW(IndexType offset,
			   IndexType rightmost) const;

	/** @brief Finds the winding sense of a loop.
	 *
	 *  @param offset Start Node (index for @c m_nodes).
	 */
	bool isCCW(IndexType offset) const;

	/** @brief Swaps the orientation (sense) of a loop.
	 *
	 *  This is called to swap the sense of an inner loop, if
	 *  the sense is the same as the one of the outer polygon.
	 *  The member function must be called before the hole
	 *  are inserted. Because the determinant and reflex state
	 *  will be calculated later (values are not set if called), these
	 *  values are let alone.
	 *
	 *  @param offset Start Node (index for @c m_nodes).
	 *  @param Number of vertices/nodes.
	 */
	void swapOrientation(IndexType offset,
						 IndexType nvertices);

	/** @brief Joins a hole with the outer polygon
	 *
	 *  @brief borderVertex Vertex of the outer polygon used to make the join.
	 *  @brief holeVertex Vertex of the inner polygon (hole) used to make the join.
	 */
	void joinOutline(IndexType borderVertex,
					 IndexType holeVertex,
					 IndexType bridgeIdx);

	/** @brief Finds the nearest visible point to the right of a hole, lying at the outer polygon.
	 *
	 *  @param offset Start of the outer polygon (index for @c m_nodes).
	 *  @param holeVertex Index of the  node of the rightmost vertex of a hole.
	 *  @retval idxFound The index of the found node (start vertex of a segment)
	 *                   of the outer polygon. The visible point lies either on the vertex
	 *                   of the node or on the segment between the vertex and the next.
	 *  @return X coordinate of the nearest visible point at the outer polygon to the right of @a holeVertex
	 */
	RtFloat visiblePointX(IndexType offset,
						  IndexType holeVertex,
						  IndexType &idxFound) const;

	/** @brief Finds the node of a vertex inside a triangle having the smallest absolute angle between p1-vertex and p1-p2.
	 *  
	 *  @return Index of a node of the found vertex or 0 (if no vertex found inside the triangle).
	 */
	IndexType getVertexInTriangle(IndexType offset,
								  RtFloat *p1,
								  RtFloat *p2,
								  RtFloat *p3) const;

	void integrateHole(IndexType offset,
					   IndexType holeVertex,
					   IndexType bridgeIdx);

	/** @brief Finds the normal of a planar polygon.
	 *
	 *  @param offs   Start node of the polygon (index for @c m_nodes).
	 *  @param verts  Indices of the vertices for the positions in @a p.
	 *  @param p      Positions of the vertices.
	 *  @return false, if the polygon has no normal (is dffegenerated), true otherwise
	 */
	bool polygonNormal(IndexType offs,
					   const RtInt verts[], const RtFloat *p,
					   RtPoint pnorm) const;
	

	/** @ The normal of the polygon, calculated by polygonNormal(), in insertPolygon().
	 */
	RtPoint m_pnorm;
public:
	/** @brief Constructor
	 */
	inline CPolygonContainer() : m_outlineIsCCW(false) { m_pnorm[0] = 0; m_pnorm[1] = 0; m_pnorm[2] = 0; }
	
	/** @brief Inserts a polygon and integrates the holes to the outline.
	 *
	 *  Parameters are alike the ones in the RMan RiGeneralPointsPolygons
	 *  for npoly=1, thats one general polygon (1 outline loop, none or many hole loops).
	 *  If CPolygonContainer is used for RiGeneralPolygon, a @a verts array can
	 *  be gerated by successive numbers.
	 *
	 *  @param nloops Number of loops, at least 1
	 *  @param loops Number of vertices for each loop (at least 3 per loop)
	 *  @param verts Indices (for @a p) of the vertices of the loop, size = sum(loops[...])
	 *  @param p Positions of the vertices.
	 */
	void insertPolygon(RtInt nloops, const RtInt loops[],
					   const RtInt verts[], const RtFloat *p);
	
	/** @brief Assigns a polygon container to this one.
	 *
	 *  @param pc The polygon container to assign.
	 *  @return *this
	 */
	CPolygonContainer &operator=(const CPolygonContainer &pc);
	
	/** @brief Gets the writeable vector of the connected nodes.
	 *
	 *  Use outline() to get a valid index into the vector.
	 *
	 *  @return Writeable vector of the connected nodes.
	 */
	inline std::vector<CPolygonNode> &nodes()
	{
		return m_nodes;
	}
	
	/** @brief Gets the read only vector of the connected nodes.
	 *
	 *  Use outline() to get a valid index into the vector.
	 *
	 *  @return Read only vector of the connected nodes.
	 */
	inline const std::vector<CPolygonNode> &nodes() const
	{
		return m_nodes;
	}

	/** @brief Gets the index of a node of the outline (outer polygon with integrated holes).
	 *
	 *  @return Index of a node of the outline returned by node().
	 */
	inline IndexType outline() const
	{
		return m_outlines[0];
	}
	
	inline bool empty() const
	{
		return m_outlines.empty();
	}

	/** @brief Requests whether the sense of the outline of the polygon is counter clockwise.
	 *
	 *  @return  true, sense of outline is counterclockwise, false if otherwise
	 */
	inline bool outlineCCW() const
	{
		return m_outlineIsCCW;
	}

	/** @brief Gets the normal of the polygon.
	 *
	 *  @return The normal of the polygon (valid after insertPolygon() is called)
	 */
	inline const RtPoint &normal() const { return m_pnorm; }

}; // CPolygonContainer


// =============================================================================
/** @brief Abstract base class of the triagulation strategy.
 *
 *  Ear clipping is a triangulation strategy.
 */
class IPolygonTriangulationStrategy {
public:
	/** @brief Triangulate a polygon.
	 *
	 *  The triangles have to have the same sense as the polygnal outline.
	 *
	 *  @param pn Node container, contents can be changed
	 *  @param offs Offset of a circular linked list with a polygon ouline
	 *              in @a pn
	 *  @param isCCW Outline has ccw orientation.
	 *  @param frontCW Render as left handed primitive, normally true in RiCPP (insert previous this next node as triangle),
	 *         if it is false the orientation is swapped (insert next this previous node as triangle),
	 *  @retval triangles, container will be filled with vertex indices
	 *                     (CPolygonNode::m_index)
	 */
	virtual ~IPolygonTriangulationStrategy() {}
	virtual void triangulate(
		std::vector<CPolygonNode> &nodes,
		IndexType offs,
		bool isCCW,
		bool frontCW,
		std::vector<IndexType> &triangles) const = 0;
}; // IPolygonTriangulationStrategy


// =============================================================================
/** @brief Implements the ear clipping triangulation for simple polygons.
 */
class CEarClipper : public IPolygonTriangulationStrategy {
public:
	virtual void triangulate(
		std::vector<CPolygonNode> &nodes,
		IndexType offs,
		bool isCCW,
		bool frontCW,
		std::vector<IndexType> &triangles) const;
};


// =============================================================================
/** @brief Container for the indirect indices of a triangulated polygon.
 */
class CTriangulatedPolygon {
private:
	std::vector<IndexType> m_triangles; ///< Vector of vertex indices, always 3 in a group.

	/** @ The normal of the polygon, set by triangulate.
	 */
	RtPoint m_pnorm;
public:
	/** @brief Constructor
	 */
	inline CTriangulatedPolygon() { m_pnorm[0] = 0; m_pnorm[1] = 0; m_pnorm[2] = 0; }
	
	/** @brief Triangulates a polygon (part of a polyhedra)
	 *
	 *  @param strategy Triangulierungs-Strategie
	 *  @param nloops Number of loops, at least 1
	 *  @param loops Number of vertices for each loop (at least 3 per loop)
	 *  @param nverts Indices (for @a p) of the vertices of the loop, size = sum(loops[...])
	 *  @param p Positions of the vertices.
	 *  @param frontCW Render as left handed primitive, normally true in RiCPP, if it is false the
	 *         orientation is swapped @see IPolygonTriangulationStrategy::triangulate()
	 *
	 *  @see CPolygonContainer::insertPolygon()
	 */
	void triangulate(const IPolygonTriangulationStrategy &strategy,
					 RtInt nloops, const RtInt nverts[],
					 const RtInt verts[], const RtFloat *p, bool frontCW);

	/** @brief Triangulates a polygon.
	 *
	 *  @param strategy Triangulierungs-Strategie
	 *  @param nloops Number of loops, at least 1
	 *  @param nverts Number of vertices for each loop (at least 3 per loop)
	 *  @param p Positions of the vertices.
	 *  @param frontCW Render as left handed primitive, normally true in RiCPP, if it is false the
	 *         orientation is swapped @see IPolygonTriangulationStrategy::triangulate()
	 *
	 *  @see CPolygonContainer::insertPolygon()
	 */
	void triangulate(const IPolygonTriangulationStrategy &strategy, RtInt nloops, const RtInt nverts[], const RtFloat *p, bool frontCW);

	/** @brief Gets the read only vector with the indirect indices.
	 *
	 *  The indices are indirect indices (i.e. indices of the verts array) to handle facevarying
	 *  as well as varying parameters of a RMan Ri...Polygons (polyhedra) call. For
	 *  single polygon indirect indices and indices have the same value.
	 *
	 *  @return Vector with the indirect indices (polyhedra - indices of verts, not of positions) of the triangles.
	 */
	inline const std::vector<IndexType> &triangles() const
	{
		return m_triangles;
	}

	/** @brief Get the dereferenced triangles
	 *  @param verts Vertex Indices
	 *  @retval tri Vertex indices of the triangulated polygon.
	 */
	void drefTriangles(const RtInt verts[], std::vector<IndexType> &tri) const;

	/** @brief Gets the normal of the polygon.
	 *
	 *  @return The normal of the polygon (valid after insertPolygon() is called)
	 */
	inline const RtPoint &normal() const { return m_pnorm; }
}; // CTriangulatedPolygon

}

#endif // _RICPP_RICONTEXT_POLYGON_H

