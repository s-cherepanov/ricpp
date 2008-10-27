// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
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

/*! \file trimcurve.cpp
 *  \brief Implementation of the container for a trim curve for NURBS, part of the attribute stack
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "trimcurve.h"

void TTrimCurveData::trimCurve(RtInt nloops, const RtInt *ncurves, const RtInt *order, const RtFloat *knot, const RtFloat *amin, const RtFloat *amax, const RtInt *n, const RtFloat *u, const RtFloat *v, const RtFloat *w) {
	releaseAll();

	if ( nloops == 0 || ncurves==NULL || order==NULL || knot==NULL || amin==NULL || amax==NULL || n==NULL || u==NULL || v==NULL || w==NULL )
		return;

	m_nLoops = nloops;
	if ( nloops > 0 ) {
		int i, j, sum;	// Counter

		m_total  = 0;	// Total number of curves
		m_npoints = 0;	// Total number of control points
		m_nknots  = 0;	// Total number of knots

		for ( i = 0; i < nloops; ++i ) {
			m_total += ncurves[i];
		}

		for ( i = 0; i < m_total; ++i ) {
			m_npoints += n[i];
			m_nknots += n[i] + order[i];
		}

		if ( m_total==0 || m_npoints==0 || m_nknots==0 ) {
			// Not a valid trim curve
			releaseAll();
			return;
		}

		m_nCurves.resize(nloops);
		m_nCurves.assign(ncurves, ncurves+nloops);
		m_order.resize(m_total);
		m_order.assign(order, order+m_total);
		m_knots.resize(m_nknots);
		m_knots.assign(knot, knot+m_nknots);
		m_min.resize(m_total);
		m_min.assign(amin, amin+m_total);
		m_max.resize(m_total);
		m_max.assign(amax, amax+m_total);
		m_n.resize(m_total);
		m_n.assign(n, n+m_total);
		m_u.resize(m_npoints);
		m_u.assign(u, u+m_npoints);
		m_v.resize(m_npoints);
		m_v.assign(v, v+m_npoints);
		m_w.resize(m_npoints);
		m_w.assign(w, w+m_npoints);

		m_points.resize(m_npoints*3);
		sum = 0;
		for ( i = 0; i < m_total; ++i ) {
			for ( j = 0; j < n[i]; ++j, ++sum ) {
				m_points[sum*3+0] = m_u[sum];
				m_points[sum*3+1] = m_v[sum];
				m_points[sum*3+2] = m_w[sum];
			}
		}
	}
}
