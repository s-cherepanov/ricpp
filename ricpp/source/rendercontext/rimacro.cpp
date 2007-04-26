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

/*! \file rimacro.cpp
 *  \brief Implementation of the macro classes, records interface calls
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "rimacro.h"

TRiMacro::~TRiMacro() {
	std::list<TRInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		delete *i;
	}
	m_callList.clear();
	m_frameList.clear();
}

bool TRiMacro::add(TRInterfaceCall *c) {
	if ( !c )
		return false;
	m_callList.push_back(c);
	return true;
}

// bool TRiMacro::finish() {
//	return true;
// }

void TRiMacro::replay(TRi &ri, TRiCallbackInterface &ricb, RtArchiveCallback callback, TDrawPrim *dp) {
	ricb.archiveName(m_name.c_str());
	std::list<TRInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		ricb.lineNo((*i)->lineNo());
		if ( (*i)->interfaceIdx() == IDX_ARCHIVE_RECORD ) {
			((TRiArchiveRecord *)(*i))->replay(ri, callback);
		} else if ( (*i)->interfaceIdx() == IDX_READ_ARCHIVE ) {
			((TRiReadArchive *)(*i))->replay(ri, callback);
		} else {
			(*i)->replay(ri, ricb, dp);
		}
	}
	ricb.lineNo(-1);
	ricb.archiveName(NULL);
}

void TRiMacro::replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
	ricb.archiveName(m_name.c_str());
	std::list<TRInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		ricb.lineNo((*i)->lineNo());
		(*i)->replay(ri, ricb, dp);
	}
	ricb.lineNo(-1);
	ricb.archiveName(NULL);
}

/*
void TRiMacro::replay(TRi &ri, RtInt lastFrameNo, RtInt frameNo) {
	int frameCnt = -1;
	std::list<TRInterfaceCall *>::iterator i;
}
*/

// ----------------------------------------------------------------------------
void TVarParamRInterfaceCall::setParams(
	TDeclarationMap &declmap,
	RtInt vertices, RtInt corners, RtInt facets,
	RtInt faceVertices, RtInt faceCorners,
	RtInt colorComps,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	m_parameters.set(declmap, vertices, corners, facets, faceVertices, faceCorners, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiGeneralPolygon::TRiGeneralPolygon(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtInt nloops, RtInt *nverts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TPolygonRInterfaceCall(aLineNo), m_nLoops(nloops)
{
	m_nVerts.resize(nloops);
	m_nVerts.assign(nverts, nverts+nloops);
	TGeneralPolygonClasses p(nloops, nverts);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiPointsPolygons::TRiPointsPolygons(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtInt npolys, RtInt *nverts, RtInt *verts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TPolygonRInterfaceCall(aLineNo), m_nPolys(npolys)
{
	m_nVerts.resize(npolys);
	m_nVerts.assign(nverts, nverts+npolys);

	int i;
	RtInt nIdx = 0; // Sum of nverts
	for ( i = 0; i < npolys; ++i )
		nIdx += nverts[i];
	m_verts.resize(nIdx);
	m_verts.assign(verts, verts+nIdx);

	TPointsPolygonsClasses p(npolys, nverts, verts);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiPointsGeneralPolygons::TRiPointsGeneralPolygons(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TPolygonRInterfaceCall(aLineNo), m_nPolys(npolys)
{
	m_nLoops.resize(npolys);
	m_nLoops.assign(nloops, nloops+npolys);

	int i;
	RtInt sumLoop = 0;
	for ( i = 0; i < npolys; ++i )
		sumLoop += nloops[i];
	m_nVerts.resize(sumLoop);
	m_nVerts.assign(nverts, nverts+sumLoop);

	RtInt nIdx = 0; // Sum of nverts
	for ( i = 0; i < sumLoop; ++i )
		nIdx += nverts[i];
	m_verts.resize(nIdx);
	m_verts.assign(verts, verts+nIdx);

	TPointsGeneralPolygonsClasses p(npolys, nloops, nverts, verts);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiPatchMesh::TRiPatchMesh(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps, RtInt ustep, RtInt vstep,
	RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TUVRInterfaceCall(aLineNo), m_type(type), m_uwrap(uwrap), m_vwrap(vwrap), m_nu(nu), m_nv(nv), m_ustep(ustep), m_vstep(vstep)
{
	TPatchMeshClasses p(type, nu, ustep, uwrap, nv, vstep, vwrap);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiNuPatch::TRiNuPatch(long aLineNo, TDeclarationMap &declmap, RtInt colorComps, RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[])
	: TGeometryRInterfaceCall(aLineNo), m_nu(nu), m_uorder(uorder), m_nv(nv), m_vorder(vorder), m_umin(umin), m_umax(umax), m_vmin(vmin), m_vmax(vmax)
{
	m_uknot.resize(nu+uorder);
	m_uknot.assign(uknot, uknot+nu+uorder);
	m_vknot.resize(nv+vorder);
	m_vknot.assign(vknot, vknot+nv+vorder);

	TNuPatchClasses p(nu, uorder, nv, vorder);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiSubdivisionMesh::TRiSubdivisionMesh(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
	RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TGeometryRInterfaceCall(aLineNo)
{
//	TRiSubdivisionMeshData data;
//	m_data.push_back(data);

	m_scheme = scheme;
	m_nfaces = nfaces;
	m_ntags = ntags;

	m_nverts.resize(nfaces);
	m_nverts.assign(nverts, nverts+nfaces);

	RtInt i;

	m_nedges = 0; // number of edges equals the number of vertices per face
	for ( i = 0; i < nfaces; ++i ) {
		m_nedges += nverts[i];
	}

	m_verts.resize(m_nedges);
	m_verts.assign(verts, verts+m_nedges);

	m_nvertices = -1;
	for ( i=0; i < m_nedges; ++i ) {
		if ( m_nvertices < verts[i] )
			m_nvertices = verts[i];
	}
	++m_nvertices;

	// m_strtags.reserve(ntags);
	for ( i = 0; i < ntags; ++i ) {
		m_strtags.push_back(tags[i]);
	}

	m_tags.reserve(ntags);
	for ( i = 0; i < ntags; ++i ) {
		m_tags.push_back(m_strtags[i].c_str());
	}

	m_nargs.resize(ntags*2);
	m_nargs.assign(nargs, nargs+ntags*2);

	RtInt sumargs = 0;
	for ( i = 0; i < ntags*2; i += 2 ) {
		sumargs += nargs[i];
	}
	m_intargs.resize(sumargs);
	m_intargs.assign(intargs, intargs+sumargs);

	sumargs = 0;
	for ( i = 1; i < ntags*2; i += 2 ) {
		sumargs += nargs[i];
	}
	m_floargs.resize(sumargs);
	m_floargs.assign(floargs, floargs+sumargs);

	TSubdivisionMeshClasses p(nfaces, nverts, verts);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiCurves::TRiCurves(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtInt ustep, RtInt vstep, RtToken type,
	RtInt ncurves, RtInt nverts[], RtToken wrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TGeometryRInterfaceCall(aLineNo), m_type(type), m_wrap(wrap), m_ncurves(ncurves), m_ustep(ustep), m_vstep(vstep)
{
	m_nverts.resize(ncurves);
	m_nverts.assign(nverts, nverts+ncurves);

	TCurvesClasses p(type, ncurves, nverts, wrap, vstep);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
TRiBlobby::TRiBlobby(
	long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
	RtInt nleaf, RtInt ncode, RtInt code[],
	RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TGeometryRInterfaceCall(aLineNo), m_nleaf(nleaf), m_ncode(ncode), m_nflt(nflt), m_nstr(nstr)
{
	int i;

	m_code.resize(ncode);
	m_code.assign(code, code+ncode);
	m_flt.resize(nflt);
	m_flt.assign(flt, flt+nflt);

	// m_strcontainer.reserve(nstr);
	m_str.reserve(nstr);
	for ( i = 0; i < nstr; ++i ) {
		m_strcontainer.push_back(str[i]);
	}
	for ( i = 0; i < nstr; ++i ) {
		m_str.push_back(m_strcontainer[i].c_str());
	}

	TBlobbyClasses p(nleaf);
	setParams(declmap, p, colorComps, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
void TRiProcedural::insertData(RtPointer data, TRi &ri) {
	// std::deque<std::string> m_strcontainer;
	// std::vector<RtString> m_str;
	const char **cp = (const char **)data;
	m_strcontainer.clear();
	m_str.clear();

	if ( m_subdivfunc == (RtProcSubdivFunc)ri.procDelayedReadArchive ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
	} else if ( m_subdivfunc == (RtProcSubdivFunc)ri.procDynamicLoad ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
	} else if ( m_subdivfunc == (RtProcSubdivFunc)ri.procRunProgram ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
		m_strcontainer.push_back(cp && cp[1] ? cp[1] : "");
		m_str.push_back(m_strcontainer[1].c_str());
	}
}

/*
GtsSurface *TRiSphere::tesselate(RtFloat tessx, RtFloat tessy) {

	GtsSurface *surf = m_surfs.getSurface(tessx, tessy);
	if ( !surf )
		return NULL;
	if ( gts_surface_face_number(surf) > 0 )
		return surf;

	RtInt uverts = (RtInt)tessx+1;
	RtInt vverts = (RtInt)tessy+1;

	RtFloat deltau = (RtFloat)(1.0 / (uverts - 1));
	RtFloat deltav = (RtFloat)(1.0 / (vverts - 1));

	RtFloat u, v, cosphi;
	double phi, theta;
	RtFloat p[4], n[4];

	RtFloat phimin;
	if ( m_zmin > -m_radius )
		phimin = (RtFloat)asin(m_zmin/m_radius);
	else
		phimin = (RtFloat)asin(-1.0);
	
	RtFloat phimax;
	if ( m_zmax < m_radius )
		phimax = (RtFloat)asin(m_zmax/m_radius);
	else
		phimax = (RtFloat)asin(1.0);

	RtFloat phispan = phimax-phimin;
	RtFloat deltaphi = phispan/(vverts-1);

	RtFloat changeNormal = (RtFloat)(m_thetamax < 0 ? 1.0 : -1.0);
	// if ( !orientation )
	//	changeNormal *= -1.0;

	std::vector<GtsVertex *> vertices;
	vertices.resize(uverts * vverts);
	std::vector<GtsVertex *> normals;
	normals.resize(uverts * vverts);

	for (	phi = phimin, v = 0.0;
			vverts > 0;
			v += deltav, --vverts, phi += deltaphi )
	{
		n[2]   = (RtFloat)sin(phi);
		n[2]  *= changeNormal;
		p[2]   = (RtFloat)(m_radius * n[2]);
		cosphi = (RtFloat)cos(phi);
		uverts = (RtInt)tessx;
		for (	u = 0.0;
				uverts > 0;
				u += deltau, --uverts )
		{
			theta = u * m_thetamax;
			n[0]  = (RtFloat)cos(theta) * cosphi;
			n[1]  = (RtFloat)sin(theta) * cosphi;
			p[0]  = m_radius * n[0];
			p[1]  = m_radius * n[1];

			GtsVertex * vert = gts_vertex_new (surf->vertex_class,
				p[0], p[1], p[2]);
			vertices.push_back(vert);

			n[0] *= changeNormal;
			n[1] *= changeNormal;

			GtsVertex * norm = gts_vertex_new (surf->vertex_class,
				n[0], n[1], n[2]);
			normals.push_back(norm);
		}
	}

	return surf;
}
*/

