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

/*! \file paramclasses.cpp
 *  \brief Implementation of a bunch of objects that can be used to calculate the
 *         sizes of parameters for the (sl)classes of geometric primitives:
 *         constant, uniform, varying, vertex, facevarying, facevertex
 *  \author Andreas Pidde (andreas@pidde.de)
 *  \sa pramclasses.h
 */
#include "ricpp/ricpp/paramclasses.h"

using namespace RiCPP;

/*!	\param type The type of the patch either TRi:RI_BILINEAR or TRi:RI_BICUBIC
	\param nu Number of control points in u direction
	\param nustep The steps over the control points in u direction to the next patch,
	              taken from the last TRi::basis() call
	\param uwrap Periodic TRi::RI_PERIODIC or nonperiodic TRi::RI_NOPERIODIC in u direction
	\param nv Number of control points in v direction
	\param nvstep The steps over the control points in v direction to the next patch,
	              taken from the last TRi::basis() call
	\param vwrap Periodic TRi::RI_PERIODIC or nonperiodic TRi::RI_NOPERIODIC in v direction
 */
void CPatchMeshClasses::reset(
	RtToken type,
	RtInt nu, RtInt nustep, RtToken uwrap,
	RtInt nv, RtInt nvstep, RtToken vwrap)
{
	bool isbicubic   = !strcmp(type,  RI_BICUBIC);  // bicubic or bilinear patch, defaults to bilinear
	bool isuperiodic = !strcmp(uwrap, RI_PERIODIC); // wraps periodically or nonperiodically in u direction, defaults to nonperiodic
	bool isvperiodic = !strcmp(vwrap, RI_PERIODIC); // wraps periodically or nonperiodically in v direction, defaults to nonperiodic

	int nupatches=0, nvpatches=0;
	if ( isbicubic ) {
		nupatches = isuperiodic ? nu/nustep : (nu-4)/nustep + 1;
		nvpatches = isvperiodic ? nv/nvstep : (nv-4)/nvstep + 1;
	} else {
		nupatches = isuperiodic ? nu : nu-1;
		nvpatches = isvperiodic ? nv : nv-1;
	}

	m_nPatches = nupatches * nvpatches;

	if ( !isuperiodic && isvperiodic  )
		m_nCorners = (nupatches + 1) * nvpatches;
	else if ( !isuperiodic && !isvperiodic )
		m_nCorners = (nupatches + 1) * (nvpatches + 1);
	else if ( isuperiodic  && !isvperiodic )
		m_nCorners = nupatches * (nvpatches + 1);
	else // if ( isuperiodic  && isvperiodic  )
		m_nCorners = nupatches * nvpatches;

	m_nVertices = nu * nv;

	m_nFaceCorners = 4 * m_nPatches;

	RtInt verticesInFace = isbicubic ? 16 : 4;
	m_nFaceVertices = verticesInFace * m_nPatches;
}


/*! reset() calculates the values for the member variables.
 */
void CNuPatchClasses::reset(
	RtInt nu, RtInt uorder,
	RtInt nv, RtInt vorder)
{
	RtInt nusegments = 1 + nu - uorder;
	RtInt nvsegments = 1 + nv - vorder;

	m_nvertices      = nu * nv;
	m_nsegments      = nusegments * nvsegments;
	m_ncorners       = (nusegments+1)*(nvsegments+1);
	m_nFaceCorners   = 4 * m_nsegments;
	m_nFaceVertices  = uorder * vorder * m_nsegments;
}


/*! reset() calculates the values for the member variables.
 */
void CSubdivisionMeshClasses::reset(RtInt nfaces, RtInt nvertices[], RtInt vertices[]) {
	m_nVerts = 0;
	m_nFaceVerts = 0;
	m_nFaces = nfaces;
	m_nFaceCorners = m_nFaces * 4;
	if ( nfaces > 0 && nvertices && vertices ) {
		int i, j;
		m_nVerts = -1;
		RtInt *pverts = vertices;
		for ( i = 0, m_nFaceVerts = 0; i < nfaces; ++i ) {
			m_nFaceVerts += nvertices[i];
			for ( j=0; j<nvertices[i]; ++j, pverts++ ) {
				if ( m_nVerts < *pverts )
					m_nVerts = *pverts;
			}
		}
		++m_nVerts;
	}
}


/*! reset() calculates the values for the member variables.
 */
void CCurvesClasses::reset(
	RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt vstep)
{
	bool isbicubic  = !strcmp(type, RI_BICUBIC);  // defaults to bilinear
	bool isperiodic = !strcmp(wrap, RI_PERIODIC); // defaults to non periodic

	int i;
	m_ncurves = ncurves;
	m_sumvarying = 0;
	m_sumverts = 0;

	for ( i = 0; i < ncurves; ++i ) {
		m_sumverts += nverts[i];
		if ( isbicubic ) {
			if ( isperiodic ) {
				m_sumvarying += (nverts[i] / vstep);
			} else {
				m_sumvarying += ((nverts[i] - 4) / vstep) + 4;
			}
		} else {
			if ( isperiodic ) {
				m_sumvarying += nverts[i];
			} else {
				m_sumvarying += nverts[i] - 1;
			}
		}
	}

	m_sumfacevarying = m_ncurves * 2;
	m_sumfaceverts = m_ncurves * (isbicubic ? 4 : 2);
}
