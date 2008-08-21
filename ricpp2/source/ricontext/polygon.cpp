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

#include "ricpp/ricontext/polygon.h"

#ifndef _RICPP_TOOLS_BINTREE_H
#include "ricpp/tools/bintree.h"
#endif // _RICPP_TOOLS_BINTREE_H

#include <algorithm>
#include <iostream>

using namespace RiCPP;

#ifdef _DEBUG
// #define _TRACE_POLY_JOIN
// #define _TRACE_POLY_INTEGRATE
// #define _TRACE_POLY_TRIANGULATE
// #define _TRACE_POLY_EAR
#endif

// =============================================================================
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
void CPolygonContainer::circularLink(
	IndexType nodeoffs,
	IndexType vertoffs,
	IndexType nVertices)
{
	assert (nVertices >= 3);
	
	IndexType prev = nVertices - 1;
	IndexType next = 1;

	IndexType curr;
	
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
IndexType CPolygonContainer::rightmostVertex(IndexType offset) const
{
	IndexType idx = offset;
	IndexType rightmost = idx;
	RtFloat x = m_nodes[rightmost][0];
	RtFloat y = m_nodes[rightmost][1];
	
	for ( idx = m_nodes[idx].m_next;
		  idx != offset;
		  idx = m_nodes[idx].m_next )
	{
		if ( m_nodes[idx][0] > x ||
			 (m_nodes[idx][0] == x &&
			  m_nodes[idx][1] > y) )
		{
			rightmost = idx;
			x = m_nodes[rightmost][0];
			y = m_nodes[rightmost][1];
		}
	}
	
	return rightmost;
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::isCCW(
	IndexType offset,
	IndexType rightmost) const
{
	IndexType startpoint, endpoint;
	RtFloat vstart[2], vend[2], det;

	// Look a previous segment with a length
	for ( startpoint = m_nodes[rightmost].m_prev;
		  startpoint != rightmost;
		  startpoint = m_nodes[startpoint].m_prev )
	{
		if ( !nearlyZero(vlen2(m_nodes[startpoint].m_p, m_nodes[rightmost].m_p)) ) {
			break;
		}
	}
	
	// There are no degenerate polygons here (already tested)
	assert ( startpoint != rightmost );

	// If the next vertex is to the left of the segment, the polygons orientation is ccw
	
	vectFromPos2(vstart, m_nodes[startpoint].m_p, m_nodes[rightmost].m_p);
	
	for ( endpoint = m_nodes[rightmost].m_next;
		  endpoint != rightmost;
		  endpoint = m_nodes[endpoint].m_next )
	{
		vectFromPos2(vend, m_nodes[rightmost].m_p, m_nodes[endpoint].m_p);
		det = det2(vstart, vend);
		RtFloat s = sign(det);
		if ( s != 0 ) {
			return s > 0; // > 0 ccw, < 0 cw
		}
	}
	
	// There are no degenerate polygons here (already tested)
	assert ( false );
	return false;
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::isCCW(IndexType offset) const
{
	IndexType rightmost = rightmostVertex(offset);
	return isCCW(offset, rightmost);
}

// -----------------------------------------------------------------------------
void CPolygonContainer::swapOrientation(
	IndexType offset, IndexType nvertices)
{
	// Swap the links only, determinant and reflex state are not set by now - they will be calculated later
	IndexType end = offset + nvertices;
	while ( offset != end ) {
		std::swap(m_nodes[offset].m_next, m_nodes[offset].m_prev);
		++offset;
	}
}

// -----------------------------------------------------------------------------
void CPolygonContainer::joinOutline(
	IndexType borderVertex,
	IndexType holeVertex,
	IndexType bridgeIdx)
{
#ifdef _TRACE_POLY_JOIN
	std::cout << "% Join Border " << borderVertex << " Hole " << holeVertex << " Bridge " << bridgeIdx << " " << bridgeIdx+1 << std::endl;
#endif

	// Link the boundary vertex with the hole vertex
	IndexType savBorderNext = m_nodes[borderVertex].m_next;
	m_nodes[borderVertex].m_next = holeVertex;

	IndexType savHolePrev = m_nodes[holeVertex].m_prev;
	m_nodes[holeVertex].m_prev = borderVertex;
	
	// Link the first bridge vertex on the hole with the second bridge vertex on the boundary
	m_nodes[bridgeIdx].m_index = m_nodes[holeVertex].m_index;
	m_nodes[bridgeIdx].m_dup = holeVertex;
	m_nodes[bridgeIdx][0] = m_nodes[holeVertex][0];
	m_nodes[bridgeIdx][1] = m_nodes[holeVertex][1];
	m_nodes[bridgeIdx].m_prev = savHolePrev;
	m_nodes[bridgeIdx].m_next = bridgeIdx+1;
	
	m_nodes[savHolePrev].m_next = bridgeIdx; // Geometry is not changed
	
	// Link the second bridge vertex on the boundary with the first bridge edge on the hole
	m_nodes[bridgeIdx+1].m_index = m_nodes[borderVertex].m_index;
	m_nodes[bridgeIdx+1].m_dup = borderVertex;
	m_nodes[bridgeIdx+1][0] = m_nodes[borderVertex][0];
	m_nodes[bridgeIdx+1][1] = m_nodes[borderVertex][1];
	m_nodes[bridgeIdx+1].m_prev = bridgeIdx;
	m_nodes[bridgeIdx+1].m_next = savBorderNext;
	
	m_nodes[savBorderNext].m_prev = bridgeIdx+1; // Geometry is not changed

	// Recalcuate the values (determinant, reflex state) of the adjacent vertices of the bridge edges
	m_nodes[borderVertex].recalc(m_nodes, outlineCCW());
	m_nodes[holeVertex].recalc(m_nodes, outlineCCW());
	m_nodes[bridgeIdx].recalc(m_nodes, outlineCCW());
	m_nodes[bridgeIdx+1].recalc(m_nodes, outlineCCW());
}

// -----------------------------------------------------------------------------
RtFloat CPolygonContainer::visiblePointX(
	IndexType offset,
	IndexType holeVertex,
	IndexType &idxFound) const
{
	RtFloat x = m_nodes[holeVertex][0];    // x coordinate of the rightmost (and uppermost) vertex of the hole
	RtFloat y = m_nodes[holeVertex][1];    // y coordinate

	IndexType retidx = 0;              // Start vertex of the segment having the nearest visible point to the right of the hole (0: undefined)
	RtFloat retx = m_nodes[offset][0];     // x coordinate of the visible point

	IndexType sv = offset;             // Start vertex of the current segment to iterate
	IndexType ev = m_nodes[sv].m_next; // End Vertex

	// Iterate the segments of the boundary
	do {
		
		if ( (m_nodes[sv][0] >= x || m_nodes[ev][0] >= x) ) {
			// At least one of the end point or start point of the segment is on the right of the rightmost hole vertex.
			if ( nearlyZero(m_nodes[ev][1] - m_nodes[sv][1]) ) {
				// Both points have almost the same y coordinate (horizontal segment)
				RtFloat v = y - m_nodes[sv][1]; // y distance of hole vertex
				if ( nearlyZero(v) ) {
					// Segment has almost on the same y coordinate as the rightmost hole vertex.
					RtFloat tempx = tmin(m_nodes[sv][0], m_nodes[ev][0]);
					
					if ( tempx < x ) {
						// One of the endpoints is on the left of the hole, use the x coordinate of the hole vertex as nearest visible point
						tempx = x;
					} // else both segment vertices on the right side of the hole
					
					if ( retidx == 0 || tempx <= retx ) {
						// Take x coordinate and start vertex index as result, if it is nearer as the one found before
						retx = tempx;
						retidx = sv;
					}
				}
			} else {
				// Not a horizontal segment
				RtFloat v = (y - m_nodes[sv][1]) /
							(m_nodes[ev][1] - m_nodes[sv][1]); // Parameter of the y-intersection of the horizontal beam to the right and the boundary segment
				if ( inClosedInterval<RtFloat>(v, 0.0, 1.0) ) {
					// y of hole is between the y coordinates of the edge
					RtFloat tempx = m_nodes[sv][0] +
							v * (m_nodes[ev][0] - m_nodes[sv][0]); // The x coordinate on the segment for the y
					if ( tempx >= x && (retidx == 0 || tempx <= retx) ) {
						// Take x coordinate and start vertex index as result, if it is nearer as the one found before
						retx = tempx;
						retidx = sv;
					}
				}
			}
		}
		
		// Take the next segment
		sv = ev;
		ev = m_nodes[sv].m_next;
	} while ( sv != offset );

	idxFound = retidx;
	return retx;
}

// -----------------------------------------------------------------------------
IndexType CPolygonContainer::getVertexInTriangle(
	IndexType offset, RtFloat *p1, RtFloat *p2, RtFloat *p3) const
{
	IndexType idx = offset;
	IndexType idxFound = 0;
	RtFloat acosangle;
	
	do {
		if ( m_nodes[idx].reflex() &&
			 point2InTriangle(m_nodes[idx].m_p, p1, p2, p3) )
		{
			if ( idxFound != 0 ) {
				RtFloat acosangleTemp = dot2_pos_norm(m_nodes[idx].m_p, p1, p2);
				if ( nearlyZero(acosangle - acosangleTemp) ) {
					// on the line p1-p2
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
	IndexType offset,
	IndexType holeVertex, IndexType bridgeIdx)
{
	// Find matching vertex in border to integrate hole
	IndexType borderVertex = 0;
	
#ifdef _TRACE_POLY_INTEGRATE
	std::cout << "% Integrate Hole: " << holeVertex << " Bridge " << bridgeIdx << " " << bridgeIdx+1 << std::endl;
#endif
	
	// x coordinate and border vertex of the segment containing the visible point
	RtFloat x = visiblePointX(offset, holeVertex, borderVertex);

	if ( borderVertex == 0 ) // not inside the boundary
		return;

	// Next vertex of the border vertex of the segment containing the visible point
	IndexType nextBorderVertex = m_nodes[borderVertex].m_next;

#ifdef _TRACE_POLY_INTEGRATE
	std::cout << "%           Border " << borderVertex << " " << nextBorderVertex << std::endl;
#endif
	
	if ( !( nearlyZero(m_nodes[borderVertex][0] - x) &&
		    nearlyZero(m_nodes[borderVertex][1] - m_nodes[holeVertex][1]) ) )
	{
		// Visible point is *not* the start vertex
		if ( nearlyZero(m_nodes[nextBorderVertex][0] - x)  &&
			 nearlyZero(m_nodes[nextBorderVertex][1] - m_nodes[holeVertex][1]) )
		{
			// Visible point is the end vertex
			borderVertex = nextBorderVertex;
		} else {
			// Set borderVertext to the nearer vertex of the outer polygon which should also be to the right of the hole.
			if ( m_nodes[nextBorderVertex][0] > m_nodes[holeVertex][0] &&
				 m_nodes[borderVertex][0] > m_nodes[holeVertex][0] )
			{
				// If both lie to right of the hole, take the nearer vertex
				RtFloat range1 = vlen2(m_nodes[holeVertex].m_p, m_nodes[borderVertex].m_p);
				RtFloat range2 = vlen2(m_nodes[holeVertex].m_p, m_nodes[nextBorderVertex].m_p);
				if ( range2 < range1 ) {
					borderVertex = nextBorderVertex;
				}
			} else {
				// If one is to the left of the most right hole vertex, take the other one.
				// At least one vertex is to the right (borderVertex or nextBorderVertex)
				if ( m_nodes[nextBorderVertex][0] > m_nodes[holeVertex][0] ) {
					borderVertex = nextBorderVertex;
				}
			}
			// If points in triangle  (holevertex, visble point in border, bordervertex)
			// get the reflex vertex with smallest angle as bridge vertex.
			RtFloat triPoint[2] = {x, m_nodes[holeVertex][1]};
			// Triangle point can be on (if degenerated triangle), above, or below the horizontal line.
			unsigned int vertex =
				getVertexInTriangle(offset,
									m_nodes[holeVertex].m_p,
									triPoint,
									m_nodes[borderVertex].m_p);
			if ( vertex != 0 )
				borderVertex = vertex;
		}
	}
	
	// Integrate hole at holeVertex with the boundary at bordervertex using bridgeIdx and bridgeIdx+1
	joinOutline(borderVertex, holeVertex, bridgeIdx);
}

// -----------------------------------------------------------------------------
bool CPolygonContainer::polygonNormal(
	IndexType offs,
	const RtInt verts[],
	const RtFloat *p,
	RtPoint pnorm) const
{
	// Find normal of the polygon
	IndexType idx;
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
		if ( !(nearlyZero(p1[0]) && nearlyZero(p1[1]) && nearlyZero(p1[2])) ) {
			p1set = true;
			break;
		}
	};
			
	if ( !p1set ) {
		// All positions have the same values
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
		if ( planeRH(pnorm, p1, p2) ) {
			p2set = true;
			break;
		}
	};

	// !p2set: All positions are linear dependend
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
	const RtInt nNodes = sumLoops + 2*(nloops-1) + 1;
	m_nodes.resize(nNodes);
		
	// Insert border and hole indirect indexes for pn,
	// link nodes 0 : 1 : 2  .... : n-1 : 0 circularily

	m_outlines.resize(nloops);
	m_outlines[0] = 1; // start at 1, first index of m_nodes is not used
	
	circularLink(m_outlines[0], 0, loops[0]);
	
	// Link the holes
	IndexType indexcount = 1 + loops[0];
	IndexType vertexcount = loops[0];
	IndexType i, idx, pidx;
	
	for ( i = 1; i < (IndexType)nloops; ++i ) {
		indexcount += 2; // bridge edges from border to hole and back to border
		m_outlines[i] = 0;
		if ( loops[i] >= 3 ) {
			m_outlines[i] = indexcount;
			circularLink(m_outlines[i], vertexcount, loops[i]);
		}
		vertexcount += loops[i];
		indexcount  += loops[i];
	}
	
	// Find the normal of the polygon
	if ( !polygonNormal(m_outlines[0], verts, p, m_pnorm) ) {
		m_nodes.clear();
		m_outlines.clear();
		return;
	}
	
	// Find the major and minor axes for 2D polygon coords,
	// used pixie code here
	int majorAxis, minorAxis;
	
	if ( fabs(m_pnorm[0]) >= tmax(fabs(m_pnorm[1]), fabs(m_pnorm[2])) ) {
		majorAxis = 1;
		minorAxis = 2;
	} else if ( fabs(m_pnorm[1]) >= tmax(fabs(m_pnorm[0]), fabs(m_pnorm[2])) ) {
		majorAxis = 0;
		minorAxis = 2;
	} else {
		majorAxis = 0;
		minorAxis = 1;
	}
	
	// Extract the coordinates for both axes
	for ( i = 0; i < (IndexType)nloops; ++i ) {
		idx = m_outlines[i];
		if ( idx != 0 ) {
			do {
				pidx = 3 * verts[m_nodes[idx].m_index];
				m_nodes[idx][0] = p[pidx + majorAxis];
				m_nodes[idx][1] = p[pidx + minorAxis];
				idx = m_nodes[idx].m_next;
			} while (idx != m_outlines[i]);
		}
	}
	
	// Join holes (inner polygons) with the border (outer polygon) and
	// make outer polygon's and hole's orientation being opposit
	// (by swapping m_next and m_prev)
	// Order from right to left
	
	// Orientation of the outer polygon
	m_outlineIsCCW = isCCW(m_outlines[0]);
	
	// std::cout << "% Outer is " << (m_outlineIsCCW ? "CCW" : "CW") << std::endl;
	
	idx = m_outlines[0];
	do {
		m_nodes[idx].recalc(m_nodes, m_outlineIsCCW);
		idx = m_nodes[idx].m_next;
	} while(idx != m_outlines[0]);

	if ( nloops > 1 ) {
		std::vector<CPolygonNodeId> temp_outlines;
		
		for ( i = 1; i < (IndexType)nloops; ++i ) {
			if ( m_outlines[i] != 0 ) {
				// the rightmost vertex of the hole i
				IndexType rm = rightmostVertex(m_outlines[i]);
				bool holeCCW = isCCW(m_outlines[i], rm);
				// std::cout << "% Hole " << i << " is " << (holeCCW ? "CCW" : "CW") << std::endl;
				if ( holeCCW == m_outlineIsCCW ) {
					// std::cout << "% Swap orientation for Hole " << i << std::endl;
					swapOrientation(m_outlines[i], loops[i]);
					holeCCW = !holeCCW;
				}
				temp_outlines.push_back(CPolygonNodeId());
				temp_outlines.back().m_offset = m_outlines[i];
				temp_outlines.back().m_idx    = rm;
				temp_outlines.back().m_nodes  = &m_nodes;
				idx = m_outlines[i];
				do {
					m_nodes[idx].recalc(m_nodes, m_outlineIsCCW);
					idx = m_nodes[idx].m_next;
				} while(idx != m_outlines[i]);
			}
		}
		std::sort(temp_outlines.begin(), temp_outlines.end(), greaterXPos);
		
		// Integrate holes into border by using a bridge edge
		// (2 additional vertices)
		// from the rightmost hole vertex to an appropriate border vertex
		
		for ( std::vector<CPolygonNodeId>::const_iterator holesIter = temp_outlines.begin();
			  holesIter != temp_outlines.end();
			  holesIter++ )
		{
			integrateHole(m_outlines[0],
						  (*holesIter).m_idx, (*holesIter).m_offset-2);
		}		
	}
}

// -----------------------------------------------------------------------------
CPolygonContainer &CPolygonContainer::operator=(const CPolygonContainer &pc)
{
	if ( this == &pc )
		return *this;
	
	m_nodes = pc.m_nodes;
	m_outlines = pc.m_outlines;
	m_outlineIsCCW = pc.m_outlineIsCCW;
	m_pnorm[0] = pc.m_pnorm[0];
	m_pnorm[1] = pc.m_pnorm[1];
	m_pnorm[2] = pc.m_pnorm[2];
	return *this;
}


// =============================================================================
// -----------------------------------------------------------------------------
static bool isEar(
	std::vector<CPolygonNode> &nodes,
	IndexType tip,
	RtFloat &weight)
{
	IndexType prev = nodes[tip].prev();
	IndexType next = nodes[tip].next();
	
#   ifdef _TRACE_POLY_EAR
	    std::cout << "% >isEar() tip " << tip << " prev " << prev << " next " << next << std::endl;
#   endif

	assert(prev!= next);
	if ( next == prev ) {
#       ifdef _TRACE_POLY_EAR
		std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear, prev == next" << std::endl;
#       endif
		return true;
	}

	RtFloat vprev[2], vnext[2], vinner[2];
	
	vectFromPos2(vprev, nodes[prev].pos(), nodes[tip].pos());
	vectFromPos2(vnext, nodes[tip].pos(), nodes[next].pos());
	vectFromPos2(vinner, nodes[next].pos(), nodes[prev].pos());

	// weight = tmax(dot2_norm(vprev, vinner), dot2_norm(vinner, vnext));
	weight = dot2_pos_norm(nodes[prev].pos(), nodes[tip].pos(), nodes[next].pos());
	
	if ( nearlyZero(det2(vprev, vnext)) ) {
		// Degenerated triangle, either a needle tip (cutted away) or a line (not cutted) 
		RtFloat lenPrev = vlen2(vprev);
		RtFloat lenNext = vlen2(vnext);
		RtFloat lenInner = vlen2(vnext);
		bool rval = lenPrev <= lenInner || lenNext <= lenInner;
#       ifdef _TRACE_POLY_EAR
		    std::cout << "% <isEar() degenerated triangle tip " << tip << " prev " << prev << " next " << next << " weight " << weight << (rval ? " is an ear" : " not an ear") << std::endl;
#       endif
		// weight = -2.0;
		return rval;
	}

	if ( nodes[tip].reflex() ) {
#       ifdef _TRACE_POLY_EAR
		std::cout << "% <isEar() tip " << tip << " not an ear (reflex)" << std::endl;
#       endif
		return false;
	}
	
	int onEdge, startEdge;
	for ( IndexType j = nodes[next].next(); j != prev; j = nodes[j].next() ) {
		if ( nodes[j].reflex() ) {
			// Is vertex j in the triangle
			if ( point2InTriangle(nodes[j].pos(), nodes[prev].pos(), nodes[tip].pos(), nodes[next].pos(), onEdge) )
			{
				if ( !onEdge ) {
#                   ifdef _TRACE_POLY_EAR
					    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - contains vertex" << std::endl;
#                   endif
					return false;
				}
				startEdge = onEdge;
				
#               ifdef _TRACE_POLY_EAR
				    std::cout << "% -isEar() tip " << tip << " prev " << prev << " next " << next << " vertex on edge" << std::endl;
#               endif

				IndexType tempPrev = j;
				IndexType temp = nodes[j].next();
				
				while ( point2InTriangle(nodes[temp].pos(), nodes[prev].pos(), nodes[tip].pos(), nodes[next].pos(), onEdge) ) {
					if ( !(onEdge & startEdge) ) {
#                       ifdef _TRACE_POLY_EAR
						    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - contains previous vertex" << std::endl;
#                       endif
						return false;
					}
					tempPrev = temp;
					temp = nodes[temp].next();
					if ( temp == prev ) {
						// All on the triangle
#                       ifdef _TRACE_POLY_EAR
						    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - all previous vertices on an edge" << std::endl;
#                       endif
						return false;
					}
				}
				
				if ( intersects2(nodes[tempPrev].pos(), nodes[temp].pos(), nodes[next].pos(), nodes[prev].pos()) == 2 ) {
					// Line intersects the base line -> no ear
#                   ifdef _TRACE_POLY_EAR
					    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - a next edge " << tempPrev << ", " << temp << " intersects base" << std::endl;
#                   endif
					return false;
				}

				tempPrev = j;
				temp = nodes[j].prev();

				while ( point2InTriangle(nodes[temp].pos(), nodes[prev].pos(), nodes[tip].pos(), nodes[next].pos(), onEdge) ) {
					if ( !(onEdge & startEdge) ) {
#                       ifdef _TRACE_POLY_EAR
						    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - contains next vertex" << std::endl;
#                       endif
						return false;
					}
					tempPrev = temp;
					temp = nodes[temp].prev();
					if ( temp == next ) {
						// All on the triangle (already tested)
#                       ifdef _TRACE_POLY_EAR
						    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - all previous next on an edge" << std::endl;
#                       endif
						return false;
					}
				}
				
				if ( intersects2(nodes[tempPrev].pos(), nodes[temp].pos(), nodes[next].pos(), nodes[prev].pos()) == 2 ) {
					// Line intersects the base line -> no ear
#                   ifdef _TRACE_POLY_EAR
					    std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " not an ear - a previous edge " << tempPrev << ", " << temp << " intersects base" << std::endl;
#                   endif
					return false;
				}
			}
		}
	}
	
#   ifdef _TRACE_POLY_EAR
		std::cout << "% <isEar() tip " << tip << " prev " << prev << " next " << next << " weight " << weight << " is an ear" << std::endl;
#   endif
	return true;
}


// =============================================================================
// -----------------------------------------------------------------------------
void CEarClipper::triangulate(
	std::vector<CPolygonNode> &nodes,
	IndexType offs,
	bool isCCW,
	bool frontCW,
	std::vector<IndexType> &triangles) const
{
#ifdef _TRACE_POLY_TRIANGULATE
	std::cout << "% >triangulate() offs " << offs << " isCCW " << (isCCW ? "true" : "false") << std::endl;
#endif

	triangles.clear();
	triangles.resize((nodes.size()-3) * 3); // n of triangles == n of vertices-2 (-3 because elem 0 was not used in nodes)

	std::vector<TemplTreeNode<RtFloat> > tn(nodes.size());
	TemplBinTree<RtFloat> tr;

	IndexType prev;
	IndexType next;
	IndexType i = offs;
	RtFloat v = 0;
	
	// Fill tree
	do {
		prev = nodes[i].prev();
		next = nodes[i].next();
#       ifdef _TRACE_POLY_TRIANGULATE
		    std::cout << "% -triangulate() examine i " << i << " prev " << prev << " next " << next << std::endl;
#       endif
		v = dot2_pos_norm(nodes[prev].pos(), nodes[i].pos(), nodes[next].pos());
		if ( isEar(nodes, i, v) ) {
#           ifdef _TRACE_POLY_TRIANGULATE
			    std::cout << "% -triangulate() Try i " << i << " prev " << prev << " next " << next << " dot " << v << std::endl;
#           endif
			tn[i].content() = v;
			tr.insert(i, tn);
		}
		i = next;
	} while ( i != offs );

	IndexType tri = 0, m;

	// Iterate tree
	while ( !tr.empty() ) {
		assert (tri <= triangles.size()-3);
		if ( tri > triangles.size()-3 )
			break;

		i = tr.maxNode(tr.root(), tn);
		prev = nodes[i].prev();
		next = nodes[i].next();

#       ifdef _TRACE_POLY_TRIANGULATE
		std::cout << "% -triangulate() examine i " << i << " prev " << prev << " next " << next << std::endl;
#       endif
		
		if ( !isEar(nodes, i, v) ) {
#           ifdef _TRACE_POLY_TRIANGULATE
			    std::cout << "% -triangulate() *** Drop i " << i << " prev " << prev << " next " << next << " weight " << v << std::endl;
#           endif
			tr.remove(i, tn);
			continue;
		}
		
		if ( !degenTriangle2(nodes[prev].pos(), nodes[i].pos(), nodes[next].pos()) ) {
#           ifdef _TRACE_POLY_TRIANGULATE
			    std::cout << "% -triangulate() *** Cut i " << i << " prev " << prev << " next " << next << " weight " << v << std::endl;
#           endif
			if ( frontCW ) {
				triangles[tri++] = nodes[prev].index();
				triangles[tri++] = nodes[i].index();
				triangles[tri++] = nodes[next].index();
			} else {
				triangles[tri++] = nodes[next].index();
				triangles[tri++] = nodes[i].index();
				triangles[tri++] = nodes[prev].index();
			}
			nodes[i].remove(nodes, isCCW);
		} else {
			if ( !nearlyZero(v+(RtFloat)1.0) ) {
				// inner node is not part of another ear
				nodes[i].remove(nodes, isCCW);
			}
#       ifdef _TRACE_POLY_TRIANGULATE
			    std::cout << "% -triangulate() *** Cull (det==0) i " << i << " prev " << prev << " next " << next << " weight " << v << std::endl;
#       endif
		}
		
		tr.remove(i, tn);
		
		if ( i == offs )
			offs = next;
		i = next;
		next = nodes[i].next();
		// prev not changed

		if ( next == prev || next == i || prev == i )
			break;

		for ( m = 0; m < 2; ++m ) {
			v = dot2_pos_norm(nodes[prev].pos(), nodes[i].pos(), nodes[next].pos());
			if ( isEar(nodes, i, v) ) {
#               ifdef _TRACE_POLY_TRIANGULATE
				    std::cout << "% -triangulate() Try i " << i << " prev " << prev << " next " << next << " weight " << v << std::endl;
#               endif
				tn[i].content() = v;
				tr.insert(i, tn);
			} else {
#               ifdef _TRACE_POLY_TRIANGULATE
				    std::cout << "% -triangulate() Drop i " << i << " prev " << prev << " next " << next << " weight " << v << std::endl;
#               endif
				tr.remove(i, tn);
			}
			next = i;
			i = prev;
			prev = nodes[i].prev();
		}
	}

	assert( (tri/3)*3 == tri);
#   ifdef _TRACE_POLY_TRIANGULATE
	    std::cout << "% <triangulate() #triangles: " << tri/3 << " #Points " << tri << std::endl;
#   endif
	triangles.resize(tri);
}


// =============================================================================
// -----------------------------------------------------------------------------
void CTriangulatedPolygon::triangulate(const IPolygonTriangulationStrategy &strategy, RtInt nloops, const RtInt nverts[], const RtFloat *p, bool frontCW)
{
	RtInt sumPoints = sum(nloops, nverts);
	std::vector<RtInt> verts;
	verts.resize(sumPoints);
	for ( RtInt i = 0; i< sumPoints; ++i ) {
		verts[i] = i;
	}
	triangulate(strategy, nloops, nverts, &verts[0], p, frontCW);
}

void CTriangulatedPolygon::drefTriangles(const RtInt verts[], std::vector<IndexType> &tri) const
{
	tri.clear();
	tri.resize(m_triangles.size());
	for ( IndexType i=0; i < m_triangles.size(); ++i ) {
		tri[i] = verts[m_triangles[i]];
	}
}

