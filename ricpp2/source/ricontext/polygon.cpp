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

/** @file polygon.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of polygon triangulation. 
 *  @see Triangulation by Ear Clipping,
 *       David Eberly 2008 http://www.geometrictools.com/
 */

#include "ricpp/tools/templatefuncs.h"
#include "ricpp/ribase/ricpptypes.h"

#include <cassert>
#include <vector>
#include <algorithm>

using namespace RiCPP;

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
	RtFloat m_dotp90;        ///< Dot product of the join (prev-this (join) this-next 90 degrees ccw) for bing left or right of and reflex calculation.
	bool m_reflex;           ///< join is a reflex vertex, depends on m_dotp90 and orientatin of the polygon outlin, @see CPolygonNode::recalc().

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
/** @brief Used to compare polygon nodes while sorting an array of nodes.
 *
 *  @param pid1 Indicates first node to compare
 *  @param pid2 Indicates second node to compare
 *
 *  @brief retuns (x of pid1) > (x of pid 2)
 */
inline static bool greaterXPos(const CPolygonNodeId &pid1, const CPolygonNodeId &pid2) 
{
	// To sort from max x to min x
	return ((*(pid1.m_nodes))[pid1.m_idx])[0] > ((*(pid2.m_nodes))[pid2.m_idx])[0];
}


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
	std::vector<unsigned long> m_triangles; ///< Vector of vertex indices, allways 3 in a group.
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


// =============================================================================
// -----------------------------------------------------------------------------
void CPolygonContainer::circularLink(
	unsigned long nodeoffs,
	unsigned long vertoffs,
	unsigned long nVertices)
{
	assert (nVertices >= 3);
	
	unsigned long prev = nVertices - 1;
	unsigned long next = 1;

	unsigned long curr;
	
	for ( curr = 0; curr < nVertices-1; ++curr ) {
		m_nodes[curr + nodeoffs].m_prev  = prev + nodeoffs;
		m_nodes[curr + nodeoffs].m_next  = next + nodeoffs;
		m_nodes[curr + nodeoffs].m_index = curr + vertoffs;
		prev = curr;
		++next;
	}

	m_nodes[curr + nodeoffs].m_prev = prev + nodeoffs;
	m_nodes[curr + nodeoffs].m_next = nodeoffs;
	m_nodes[curr + nodeoffs].m_index = curr + vertoffs;
}

// -----------------------------------------------------------------------------
unsigned long CPolygonContainer::rightmostVertex(unsigned long offset) const
{
	unsigned long idx = offset;
	unsigned long rightmost = idx;
	RtFloat x = m_nodes[rightmost][0];
	
	for ( idx = m_nodes[idx].m_next;
		  idx != offset;
		  idx = m_nodes[idx].m_next )
	{
		if ( m_nodes[idx][0] > x ) {
			rightmost = idx;
			x = m_nodes[rightmost][0];
		}
	}
	
	return rightmost;
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::isCCW(
	unsigned long offset,
	unsigned long rightmost) const
{
	unsigned long startpoint, endpoint;
	RtFloat vstart[2], vend[2], dotprod;

	for ( startpoint = m_nodes[rightmost].m_prev;
		  startpoint != rightmost;
		  startpoint = m_nodes[startpoint].m_prev )
	{
		if ( fabs(vlen2(m_nodes[startpoint].m_p, m_nodes[rightmost].m_p))
			 > eps<RtFloat>() )
		{
			break;
		}
	}
	
	// There are no degenerate polygons here (already tested)
	assert ( startpoint != rightmost );

	vectFromPos2(vstart, m_nodes[startpoint].m_p, m_nodes[rightmost].m_p);
	
	for ( endpoint = m_nodes[rightmost].m_next;
		  endpoint != rightmost;
		  endpoint = m_nodes[endpoint].m_next )
	{
		vectFromPos2(vend, m_nodes[rightmost].m_p, m_nodes[endpoint].m_p);
		dotprod = dot2_90(vstart, vend);
		if ( fabs(dotprod) > eps<RtFloat>() ) {
			return sign(dotprod) < 0; // < 0 ccw, > 0 cw
		}
	}
	
	// There are no degenerate polygons here (already tested)
	assert ( false );
	return false;
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::isCCW(unsigned long offset) const
{
	unsigned long rightmost = rightmostVertex(offset);
	return isCCW(offset, rightmost);
}

// -----------------------------------------------------------------------------
void CPolygonContainer::swapOrientation(
	unsigned long offset, unsigned long nvertices)
{
	unsigned long end = offset + nvertices;
	while ( offset != end ) {
		std::swap(m_nodes[offset].m_next, m_nodes[offset].m_prev);
		++offset;
	}
}

// -----------------------------------------------------------------------------
void CPolygonContainer::joinOutline(
	unsigned long borderVertex,
	unsigned long holeVertex,
	unsigned long bridgeIdx)
{
	unsigned long savBorderNext = m_nodes[borderVertex].m_next;
	m_nodes[borderVertex].m_next = holeVertex;

	m_nodes[borderVertex].recalc(m_nodes, outlineCCW());

	unsigned long savHolePrev = m_nodes[holeVertex].m_prev;
	m_nodes[holeVertex].m_prev = borderVertex;

	m_nodes[holeVertex].recalc(m_nodes, outlineCCW());
	
	m_nodes[bridgeIdx].m_index = m_nodes[holeVertex].m_index;
	m_nodes[bridgeIdx].m_prev = savHolePrev;
	m_nodes[bridgeIdx].m_next = bridgeIdx+1;
	
	m_nodes[bridgeIdx].recalc(m_nodes, outlineCCW());

	m_nodes[savHolePrev].m_next = bridgeIdx; // Geometry is not changed
	
	m_nodes[bridgeIdx+1].m_index = m_nodes[borderVertex].m_index;
	m_nodes[bridgeIdx+1].m_prev = bridgeIdx;
	m_nodes[bridgeIdx+1].m_next = savBorderNext;
	
	m_nodes[bridgeIdx+1].recalc(m_nodes, outlineCCW());

	m_nodes[savBorderNext].m_prev = bridgeIdx+1; // Geometry is not changed
}

// -----------------------------------------------------------------------------
RtFloat CPolygonContainer::visiblePointX(
	unsigned long offset,
	unsigned long holeVertex,
	unsigned long &idxFound) const
{
	RtFloat x = m_nodes[holeVertex][0];
	RtFloat y = m_nodes[holeVertex][1];
	unsigned int sv = offset, retidx = 0;
	RtFloat retx = x;

	for ( unsigned long ev = m_nodes[sv].m_next;
		  ev != offset;
		  ev = m_nodes[sv].m_next )
	{
		if ( (m_nodes[sv][0] >= x || m_nodes[ev][0] >= x) ) {
			RtFloat tempx;
			if ( fabs(m_nodes[ev][1] - m_nodes[sv][1]) > eps<RtFloat>() ) {
				RtFloat v = (y - m_nodes[sv][1]) /
							(m_nodes[ev][1] - m_nodes[sv][1]);
				if ( fabs(v) <= 1.0 ) {
					tempx = m_nodes[sv][0] + v *
							(m_nodes[ev][0] - m_nodes[sv][0]);
					if ( tempx >= x && tempx <= retx) {
						retx = tempx;
						retidx = sv;
					}
				}
			} else {
				RtFloat v = y - m_nodes[sv][1];
				if ( fabs(v) < eps<RtFloat>() ) {
					tempx = tmin(m_nodes[sv][0], m_nodes[ev][0]);
					if ( tempx < x )
						tempx = x;
					if ( tempx <= retx ) {
						retx = tempx;
						retidx = sv;
					}
				}
			}
		}
		sv = ev;
	}
	idxFound = retidx;
	return retx;
}

// -----------------------------------------------------------------------------
unsigned long CPolygonContainer::getVertexInTriangle(
	unsigned long offset, RtFloat *p1, RtFloat *p2, RtFloat *p3) const
{
	unsigned long idx = offset;
	unsigned long idxFound = 0;
	RtFloat acosangle;
	do {
		if ( m_nodes[idx].reflex() &&
			 point2InTriangle(m_nodes[idx].m_p, p1, p2, p3) )
		{
			if ( idxFound != 0 ) {
				RtFloat acosangleTemp = dot2_pos_norm(m_nodes[idx].m_p, p1, p2);
				if ( fabs(acosangle - acosangleTemp) < eps<RtFloat>() ) {
					if ( m_nodes[idx][0] < m_nodes[idxFound][0] ) {
						idxFound = idx;
						acosangle = acosangleTemp;
					}
				} else {
					if ( acosangleTemp > acosangle ) {
						idxFound = idx;
						acosangle = acosangleTemp;
					}
				}
			} else {
				idxFound = idx;
				acosangle = dot2_pos_norm(m_nodes[idx].m_p, p1, p2);
			}
		}
		idx = m_nodes[idx].m_next;	
	} while (idx != offset);

	return idxFound;
}

// -----------------------------------------------------------------------------
void CPolygonContainer::integrateHole(
	unsigned long offset,
	unsigned long holeVertex, unsigned long bridgeIdx)
{
	// Find matching vertex in border to integrate hole
	unsigned long borderVertex = 0;
	
	RtFloat x = visiblePointX(offset, holeVertex, borderVertex);
	if ( borderVertex == 0 )
		return;
	unsigned long nextBorderVertex = m_nodes[borderVertex].m_next;

	if ( !( fabs(m_nodes[borderVertex][0] - x) <= eps<RtFloat>() &&
		    fabs(m_nodes[borderVertex][1] - m_nodes[holeVertex][1])
			    <= eps<RtFloat>() ) )
	{
		if ( fabs(m_nodes[nextBorderVertex][0] - x) <= eps<RtFloat>() &&
			 fabs(m_nodes[nextBorderVertex][1] - m_nodes[holeVertex][1])
			     <= eps<RtFloat>() )
		{
			borderVertex = nextBorderVertex;
		} else {
			if ( m_nodes[borderVertex][0] < m_nodes[nextBorderVertex][0] ) {
				borderVertex = nextBorderVertex;
			}
			// Triangle (holevertex, point in border, bordervertex)
			// If points in triangle get reflex vertex with smalest angle
			// holevertex to reflex vertex and horizontal line.
			// bordervertex otherwise
			RtFloat triPoint[2] = {x, m_nodes[holeVertex][1]};
			unsigned int vertex =
				getVertexInTriangle(offset,
									m_nodes[holeVertex].m_p,
									triPoint,
									m_nodes[borderVertex].m_p);
			if ( vertex != 0 )
				borderVertex = vertex;
		}
	}
	
	// Integrate hole at holeVertex
	joinOutline(borderVertex, holeVertex, bridgeIdx);
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::polygonNormal(
	unsigned long offs,
	const RtInt verts[],
	const RtFloat *p,
	RtPoint pnorm) const
{
	// Find normal of the polygon
	unsigned long idx;
	RtInt pidx;
	RtPoint p0, p1, p2;
	bool p1set = false, p2set = false;
	
	idx = offs;
	pidx = 3*verts[m_nodes[idx].m_index];
	p0[0] = p[pidx++];
	p0[1] = p[pidx++];
	p0[2] = p[pidx  ];
	
	for ( idx = m_nodes[idx].m_next;
		  idx != offs;
		  idx = m_nodes[idx].m_next )
	{
		pidx = 3 * verts[m_nodes[idx].m_index];
		p1[0] = p[pidx++] - p0[0];
		p1[1] = p[pidx++] - p0[1];
		p1[2] = p[pidx  ] - p0[2];
		if ( fabs(p1[0]) > eps<RtFloat>() || fabs(p1[1]) > eps<RtFloat>() ||
			 fabs(p1[2]) > eps<RtFloat>() )
		{
			p1set = true;
			break;
		}
	};
	if ( !p1set ) {
		// All positions have more or less the same values
		return false;
	}
	
	for ( idx = m_nodes[offs].m_prev;
		  idx != offs;
		  idx = m_nodes[idx].m_prev )
	{
		pidx = 3 * verts[m_nodes[idx].m_index];
		p2[0] = p[pidx++] - p0[0];
		p2[1] = p[pidx++] - p0[1];
		p2[2] = p[pidx  ] - p0[2];
		if ( planeNorm(p1, p2, pnorm) ) {
			p2set = true;
			break;
		}
	};

	// !p2set: All positions are more or less colinear
	return p2set;
}

// -----------------------------------------------------------------------------
void CPolygonContainer::insertPolygon(
	RtInt nloops,
	const RtInt loops[],
	const RtInt verts[],
	const RtFloat *p)
{
	m_nodes.clear();
	m_outlines.clear();
	
	if ( nloops < 1 || p == 0 || loops[0] < 3 )
		return;
	
	// One node for each vertex + 2 additional for each hole,
	// because 2 vertices have do be added for each hole to
	// join the polygons.
	// element 0 is not used (used like NULL for pointers)

	RtInt sumLoops = sum(nloops, loops);
	m_nodes.resize(sumLoops + 2*(nloops-1) + 1);
		
	// Insert border and hole indirect indexes for pn,
	// link nodes 0 : 1 : 2  .... : n-1 : 0 circularily

	m_outlines.resize(nloops);
	m_outlines[0] = 1; // start at 1, first index of m_nodes is not used
	
	circularLink(m_outlines[0], loops[0], 0);
	
	// Link the holes
	unsigned long indexcount = 1 + loops[0];
	unsigned long vertexcount = loops[0];
	unsigned long i, idx, pidx;
	
	for ( i = 1; i < (unsigned long)nloops; ++i ) {
		indexcount += 2; // bridge edges from border to hole and back to border
		m_outlines[i] = indexcount;
		circularLink(m_outlines[i], loops[i], vertexcount);
		indexcount  += loops[i];
		vertexcount += loops[i];
	}
	
	// Find the normal of the polygon
	RtPoint pnorm;
	if ( !polygonNormal(m_outlines[0], verts, p, pnorm) ) {
		m_nodes.clear();
		m_outlines.clear();
		return;
	}
	
	// Find the major and minor axes for 2D polygon coords,
	// used pixie code here
	int majorAxis, minorAxis;
	
	if ( fabs(pnorm[0]) >= tmax(fabs(pnorm[1]), fabs(pnorm[2])) ) {
		majorAxis = 1;
		minorAxis = 2;
	} else if ( fabs(pnorm[1]) >= tmax(fabs(pnorm[0]), fabs(pnorm[2])) ) {
		majorAxis = 0;
		minorAxis = 2;
	} else {
		majorAxis = 0;
		minorAxis = 1;
	}
	
	// Extract the coordinates for both axes
	for ( i = 0; i < (unsigned long)nloops; ++i ) {
		for ( idx = m_nodes[m_outlines[i]].m_next;
			 idx != m_outlines[i];
			 idx = m_nodes[idx].m_next )
		{
			pidx = 3 * verts[m_nodes[idx].m_index];
			m_nodes[idx][0] = p[pidx + majorAxis];
			m_nodes[idx][1] = p[pidx + minorAxis];
		};
	}
	
	// Join holes (inner polygons) with the border (outer polygon) and
	// make outer polygon's and hole's orientation being opposit
	// (by swapping m_next and m_prev)
	// Order from right to left
	
	// Orientation of the outer polygon
	m_outlineIsCCW = isCCW(m_outlines[0]);
	
	for ( idx = m_nodes[m_outlines[0]].m_next;
		 idx != m_outlines[0];
		 idx = m_nodes[idx].m_next )
	{
		m_nodes[idx].recalc(m_nodes, m_outlineIsCCW);
	};
	
	if ( nloops > 1 ) {
		std::vector<CPolygonNodeId> temp_outlines;
		
		temp_outlines.resize(nloops);
		
		// Inserted just to get the indices of loops and m_outlines equal
		temp_outlines[0].m_offset = m_outlines[0];
		temp_outlines[0].m_idx    = 0;
		temp_outlines[0].m_nodes  = &m_nodes;
		
		for ( i = 1; i < (unsigned long)nloops; ++i ) {
			// the rightmost vertex of the hole i
			unsigned long rm = rightmostVertex(m_outlines[i]);
			bool holeCCW = isCCW(m_outlines[i], rm);
			if ( holeCCW == m_outlineIsCCW )
				swapOrientation(m_outlines[i], loops[i]);
			temp_outlines[i].m_offset = i;
			temp_outlines[i].m_idx    = rm;
			temp_outlines[i].m_nodes  = &m_nodes;
		}
		std::sort(temp_outlines.begin()++, temp_outlines.end(), greaterXPos);
		
		// Integrate holes into border by using a bridge edge
		// (2 additional vertices)
		// from the rightmost hole vertex to an appropriate border vertex
		std::vector<CPolygonNodeId>::const_iterator holesIter = temp_outlines.begin();
		for ( holesIter++;
			 holesIter != temp_outlines.end();
			 holesIter++ )
		{
			integrateHole(m_outlines[0],
						  (*holesIter).m_idx, (*holesIter).m_offset-2);
		}		
	}
}
