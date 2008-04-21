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
#include <algorithm>

using namespace RiCPP;

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


	unsigned long savHolePrev = m_nodes[holeVertex].m_prev;
	m_nodes[holeVertex].m_prev = borderVertex;

	
	m_nodes[bridgeIdx].m_index = m_nodes[holeVertex].m_index;
	m_nodes[bridgeIdx][0] = m_nodes[holeVertex][0];
	m_nodes[bridgeIdx][1] = m_nodes[holeVertex][1];
	m_nodes[bridgeIdx].m_prev = savHolePrev;
	m_nodes[bridgeIdx].m_next = bridgeIdx+1;
	
	m_nodes[savHolePrev].m_next = bridgeIdx; // Geometry is not changed
	
	m_nodes[bridgeIdx+1].m_index = m_nodes[borderVertex].m_index;
	m_nodes[bridgeIdx+1][0] = m_nodes[borderVertex][0];
	m_nodes[bridgeIdx+1][1] = m_nodes[borderVertex][1];
	m_nodes[bridgeIdx+1].m_prev = bridgeIdx;
	m_nodes[bridgeIdx+1].m_next = savBorderNext;
	
	m_nodes[savBorderNext].m_prev = bridgeIdx+1; // Geometry is not changed

	m_nodes[borderVertex].recalc(m_nodes, outlineCCW());
	m_nodes[holeVertex].recalc(m_nodes, outlineCCW());
	m_nodes[bridgeIdx].recalc(m_nodes, outlineCCW());
	m_nodes[bridgeIdx+1].recalc(m_nodes, outlineCCW());
}

// -----------------------------------------------------------------------------
RtFloat CPolygonContainer::visiblePointX(
	unsigned long offset,
	unsigned long holeVertex,
	unsigned long &idxFound) const
{
	RtFloat x = m_nodes[holeVertex][0];
	RtFloat y = m_nodes[holeVertex][1];
	unsigned long retidx = 0;
	RtFloat retx = m_nodes[offset][0];

	unsigned long sv = offset;
	unsigned long ev = m_nodes[sv].m_next;
	do {
		if ( (m_nodes[sv][0] >= x || m_nodes[ev][0] >= x) ) {
			// End point or start point out right from rightmost hole vertex.
			RtFloat tempx;
			if ( fabs(m_nodes[ev][1] - m_nodes[sv][1]) < eps<RtFloat>() ) {
				// Both points have almost the same y coordinate
				RtFloat v = y - m_nodes[sv][1];
				if ( fabs(v) < eps<RtFloat>() ) {
					// Has almost on the same y coordinate as the rightmost hole vertex.
					tempx = tmin(m_nodes[sv][0], m_nodes[ev][0]);
					if ( tempx < x ) {
						// One of the endpoints is ont the left of the hole.
						tempx = x;
					}
					if ( tempx <= retx ) {
						retx = tempx;
						retidx = sv;
					}
				}
			} else {
				RtFloat v = (y - m_nodes[sv][1]) /
							(m_nodes[ev][1] - m_nodes[sv][1]);
				if ( v >= 0 && v <= 1.0 ) {
					// y of hole is between the y coordinates of the edge
					tempx = m_nodes[sv][0] +
							v * (m_nodes[ev][0] - m_nodes[sv][0]);
					if ( tempx >= x && (retidx == 0 || tempx <= retx) ) {
						// x coordinate of the intersection of the horizontal with the edge is on the right
						// of the hole and is nearer to this hole vertex than (or as near as) the one previously found.
						retx = tempx;
						retidx = sv;
					}
				}
			}
		}
		sv = ev;
		ev = m_nodes[sv].m_next;
	} while ( sv != offset );

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
			if ( m_nodes[nextBorderVertex][0] > m_nodes[borderVertex][0] ) {
				borderVertex = nextBorderVertex;
			}
			// Triangle (holevertex, point in border, bordervertex)
			// If points in triangle get reflex vertex with smalest angle
			// holevertex to reflex vertex and horizontal line.
			// bordervertex otherwise
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
		if ( planeNorm(pnorm, p1, p2) ) {
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
	const RtInt nNodes = sumLoops + 2*(nloops-1) + 1;
	m_nodes.resize(nNodes);
		
	// Insert border and hole indirect indexes for pn,
	// link nodes 0 : 1 : 2  .... : n-1 : 0 circularily

	m_outlines.resize(nloops);
	m_outlines[0] = 1; // start at 1, first index of m_nodes is not used
	
	circularLink(m_outlines[0], 0, loops[0]);
	
	// Link the holes
	unsigned long indexcount = 1 + loops[0];
	unsigned long vertexcount = loops[0];
	unsigned long i, idx, pidx;
	
	for ( i = 1; i < (unsigned long)nloops; ++i ) {
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
	
	idx = m_outlines[0];
	do {
		m_nodes[idx].recalc(m_nodes, m_outlineIsCCW);
		idx = m_nodes[idx].m_next;
	} while(idx != m_outlines[0]);

	if ( nloops > 1 ) {
		std::vector<CPolygonNodeId> temp_outlines;
		
		for ( i = 1; i < (unsigned long)nloops; ++i ) {
			if ( m_outlines[i] != 0 ) {
				// the rightmost vertex of the hole i
				unsigned long rm = rightmostVertex(m_outlines[i]);
				bool holeCCW = isCCW(m_outlines[i], rm);
				if ( holeCCW == m_outlineIsCCW ) {
					swapOrientation(m_outlines[i], loops[i]);
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
