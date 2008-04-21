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
	friend class CPolygonContainer;
	// Links
	unsigned long m_next,    ///< Next node in circular list of polygon outline.
	m_prev;                  ///< Previous node in circular list of polygon outline.
public:
	
	// Payload
	unsigned long m_index;   ///< Index of an array with vertex indices (double indirection for varying) and single indirection for face varying.
	RtFloat m_p[2];          ///< 2D coordinates of the vertex for some major and minor axes.
	RtFloat m_dotp90;        ///< Dot product of the join (prev-this (join) this-next 90 degrees ccw) for being left of or being right of, and reflex calculation.
	bool m_reflex;           ///< Join is a reflex vertex, depends on m_dotp90 and orientation of the polygon outline, @see CPolygonNode::recalc().
	
	// -------------------------------------------------------------------------
	/** @brief Standard constructor, clear @c m_next and @c m_prev to indicate
	 *         unlinked state.
	 */
	inline CPolygonNode()
	{
		m_next = m_prev = 0;
	}
	
	// -------------------------------------------------------------------------
	/** @brief Gets Index of the next node.
	 *  @return Index of the next node.
	 */
	inline unsigned long next() const { return m_next; }
	
	// -------------------------------------------------------------------------
	/** @brief Gets Index of the previous node.
	 *  @return Index of the privious node.
	 */
	inline unsigned long prev() const { return m_prev; }
	
	// -------------------------------------------------------------------------
	/** @brief Recalculate dot product and reflex state of the vertex.
	 *
	 *  Vertex has to be part of a polygon outline (being linked).
	 *  @param nodes All nodes (part of CPolygonContainer).
	 *  @param isCCW outline's orientatin is counterclockwise.
	 */
	inline void recalc(std::vector<CPolygonNode> &nodes, bool isCCW)
	{
		RtFloat dotp = dot2_90_pos(nodes[m_prev].m_p, m_p, nodes[m_next].m_p);
		m_dotp90 = dotp;
		if ( isCCW ) {
			m_reflex = sign(dotp) <= 0.0;
		} else {
			m_reflex = sign(dotp) >= 0.0;
		}
	}
	
	// -------------------------------------------------------------------------
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
							unsigned long myIdx,
							unsigned long idx,
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
	
	// -------------------------------------------------------------------------
	/** @brief Requests whether node is removed.
	 *
	 *  @return true, if node is removed from any outline.
	 */
	inline bool removed() const
	{
		return m_next == 0 && m_prev == 0;
	}
	
	// -------------------------------------------------------------------------
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
	
	// -------------------------------------------------------------------------
	/** @brief Tests whether vertex is reflex.
	 *
	 *  @return true, vertex is reflex.
	 */
	inline bool reflex() const
	{
		return m_reflex;
	}
	
	// -------------------------------------------------------------------------
	/** @brief Accesses the 2D coordinates of the vertex.
	 *
	 *  @param idx Index of the 2D coordinate (0 or 1)
	 *  @return Coordinate for @a idx.
	 */
	inline RtFloat &operator[](int idx)
	{
		assert( idx == 0 || idx == 1 );
		return m_p[idx];
	}
	
	inline const RtFloat &operator[](int idx) const
	{
		assert( idx == 0 || idx == 1 );
		return m_p[idx];
	}
	
	// -------------------------------------------------------------------------
	/** @brief Assignment operator;
	 *
	 *  @param pn Node to assign to this node.
	 *  @return Reference to this node.
	 */
	inline CPolygonNode &operator=(const CPolygonNode &pn)
	{
		if ( &pn == this )
			return *this;
		
		m_next = pn.m_next;
		m_prev = pn.m_prev;
		m_index = pn.m_index;
		m_p[0] = pn.m_p[0];
		m_p[1] = pn.m_p[1];
		m_dotp90 = pn.m_dotp90;
		m_reflex = pn.m_reflex;
		
		return *this;
	}
}; // CPolygonNode


// =============================================================================
/** @brief Contains an index of a special node.
 *
 *  Helper class, used to sort nodes of holes in descending x order.
 */
class CPolygonNodeId {
public:
	unsigned long m_offset; ///< Start index of a circular linked hole outline
	unsigned long m_idx;  ///< Index of a special node within the outline
	std::vector<CPolygonNode> *m_nodes;   ///< Container of all nodes
}; // CPolygonNodeId


// =============================================================================
/** @brief Container for a general (but simple) polygon.
 *
 *  Holes (inner polygons) will be joined with the outer polygon.
 */
class CPolygonContainer {
	std::vector<CPolygonNode> m_nodes;
	std::vector<unsigned long> m_outlines;
	bool m_outlineIsCCW;
	
	void circularLink(unsigned long nodeoffs,
					  unsigned long vertoffs,
					  unsigned long nVertices);
	unsigned long rightmostVertex(unsigned long offset) const;
	bool isCCW(unsigned long offset, unsigned long rightmost) const;
	bool isCCW(unsigned long offset) const;
	void swapOrientation(unsigned long offset, unsigned long nvertices);
	void joinOutline(unsigned long borderVertex,
					 unsigned long holeVertex,
					 unsigned long bridgeIdx);
	RtFloat visiblePointX(unsigned long offset,
						  unsigned long holeVertex,
						  unsigned long &idxFound) const;
	unsigned long getVertexInTriangle(unsigned long offset,
									  RtFloat *p1,
									  RtFloat *p2,
									  RtFloat *p3) const;
	void integrateHole(unsigned long offset,
					   unsigned long holeVertex,
					   unsigned long bridgeIdx);
	bool polygonNormal(unsigned long offs,
					   const RtInt verts[], const RtFloat *p,
					   RtPoint pnorm) const;
	
	
public:
	inline CPolygonContainer() {}
	
	void insertPolygon(RtInt nloops, const RtInt loops[],
					   const RtInt verts[], const RtFloat *p);
	
	inline CPolygonContainer &operator=(const CPolygonContainer &pc)
	{
		m_nodes = pc.m_nodes;
		m_outlines = pc.m_outlines;
		m_outlineIsCCW = pc.m_outlineIsCCW;
	}
	
	inline std::vector<CPolygonNode> &nodes()
	{
		return m_nodes;
	}
	
	inline unsigned long outline() const
	{
		return m_outlines[0]; // Index of the outline (border, outer polygon)
	}
	
	inline bool outlineCCW() const
	{
		return m_outlineIsCCW;
	}
}; // CPolygonContainer


// =============================================================================
/** @brief Abstract base class of the triagulation strategy.
 *
 *  Ear clipping is a triangulation strategy.
 */
class CPolygonTriangulationStrategy {
public:
	inline CPolygonTriangulationStrategy() {}
	/** @brief Triangulate a polygon.
	 *
	 *  @param pn Node container, contents can be changed
	 *  @param offs Offset of a circular linked list with a polygon ouline
	 *              in @a pn
	 *  @param isCCW Outline has ccw orientation.
	 *  @retval triangles, container will be filled with vertex indices
	 *                     (CPolygonNode::m_index)
	 */
	virtual void triangulate(std::vector<CPolygonNode> &nodes, unsigned long offs, bool isCCW, std::vector<unsigned long> &triangles) const = 0;
}; // CPolygonTriangulationStrategy


// =============================================================================
/** @brief Implements the ear clipping triangulation for simple polygons.
 */
class CEarClipper : public CPolygonTriangulationStrategy {
public:
	inline virtual void triangulate(std::vector<CPolygonNode> &nodes, unsigned long offs, bool isCCW, std::vector<unsigned long> &triangles) const {}
};


// =============================================================================
/** @brief Container for a triangulated polygon.
 */
class CTriangulatedPolygon {
	std::vector<unsigned long> m_triangles; ///< Vector of vertex indices, always 3 in a group.
	const CPolygonTriangulationStrategy *m_strategy;
public:
	inline CTriangulatedPolygon(const CPolygonTriangulationStrategy &triangulation)
	{
		m_strategy = &triangulation;
	}
	inline void triangulate(RtInt nloops, const RtInt loops[],
							const RtInt verts[], const RtFloat *p)
	{
		CPolygonContainer c;
		c.insertPolygon(nloops, loops, verts, p);
		m_strategy->triangulate(c.nodes(), c.outline(), c.outlineCCW(), m_triangles);
	}
	inline const std::vector<unsigned long> &triangles() const
	{
		return m_triangles;
	}
}; // CTriangulatedPolygon

}

#endif // _RICPP_RICONTEXT_POLYGON_H

