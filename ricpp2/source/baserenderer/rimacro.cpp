/*! \file rimacro.cpp
 *  \brief Implementation of the macro classes, records interface calls
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ricpp/baserenderer/rimacro.h"

using namespace RiCPP;

CRiMacro::~CRiMacro()
{
	std::list<CRManInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		delete *i;
	}
	m_callList.clear();
}

bool CRiMacro::add(CRManInterfaceCall *c)
{
	if ( !c )
		return false;
	m_callList.push_back(c);
	return true;
}


void CRiMacro::replay(IDoRender &ri, const IArchiveCallback *callback)
{
	CRenderState *state = ri.renderState();
	if ( !state )
		return;

	state->archiveName(m_name.c_str());
	std::list<CRManInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		try {
			state->lineNo((*i)->lineNo());
			if ( ri.preCheck((*i)->interfaceIdx()) ) {
				if ( (*i)->interfaceIdx() == REQ_ARCHIVE_RECORD ) {
					(*i)->replay(ri, callback);
				} else if ( (*i)->interfaceIdx() == REQ_READ_ARCHIVE ) {
					(*i)->replay(ri, callback);
				} else {
					(*i)->replay(ri);
				}
			}
		} catch ( ExceptRiCPPError &e2 ) {
			state->lineNo(-1);
			state->archiveName(0);
			throw e2;
		}
	}
	state->lineNo(-1);
	state->archiveName(0);
}

void CRiMacro::replay(IDoRender &ri)
{
	CRenderState *state = ri.renderState();
	if ( !state )
		return;

	state->archiveName(m_name.c_str());
	std::list<CRManInterfaceCall *>::iterator i;
	for ( i = m_callList.begin(); i != m_callList.end(); ++i ) {
		try {
			state->lineNo((*i)->lineNo());
			if ( ri.preCheck((*i)->interfaceIdx()) ) {
				(*i)->replay(ri);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			state->lineNo(-1);
			state->archiveName(0);
			throw e2;
		}
	}
	state->lineNo(-1);
	state->archiveName(0);
}

///////////////////////////////////////////////////////////////////////////////
void CRiGeneralPolygon::enterValues(RtInt nloops, RtInt *nverts)
{
	m_nLoops = nloops;
	m_nVerts.resize(nloops);
	m_nVerts.assign(nverts, nverts+nloops);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt nloops, RtInt *nverts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(nloops, nverts);
	CGeneralPolygonClasses p(nloops, nverts);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo,
	RtInt nloops, RtInt *nverts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(nloops, nverts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsPolygons::enterValues(RtInt npolys, RtInt *nverts, RtInt *verts)
{
	m_nPolys = npolys;
	m_nVerts.resize(npolys);
	m_nVerts.assign(nverts, nverts+npolys);

	int i;
	RtInt nIdx = 0; // Sum of nverts
	for ( i = 0; i < npolys; ++i )
		nIdx += nverts[i];
	m_verts.resize(nIdx);
	m_verts.assign(verts, verts+nIdx);
}

CRiPointsPolygons::CRiPointsPolygons(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt npolys, RtInt *nverts, RtInt *verts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(npolys, nverts, verts);
	CPointsPolygonsClasses p(npolys, nverts, verts);
	setParams(decl, p, curColorDescr, n, tokens, params);
}
inline CRiPointsPolygons::CRiPointsPolygons(
	long aLineNo,
	RtInt npolys, RtInt *nverts, RtInt *verts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(npolys, nverts, verts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsGeneralPolygons::enterValues(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts)
{
	m_nPolys = npolys;
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

}

CRiPointsGeneralPolygons::CRiPointsGeneralPolygons(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(npolys, nloops, nverts, verts);
	CPointsGeneralPolygonsClasses p(npolys, nloops, nverts, verts);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

CRiPointsGeneralPolygons::CRiPointsGeneralPolygons(
	long aLineNo,
	RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo)
{
	enterValues(npolys, nloops, nverts, verts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPatchMesh::enterValues(RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap)
{
	m_type = type;
	m_uwrap = uwrap;
	m_vwrap = vwrap;
	m_nu = nu;
	m_nv = nv;
	m_ustep = ustep;
	m_vstep = vstep;
}

CRiPatchMesh::CRiPatchMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr, RtInt ustep, RtInt vstep,
	RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CUVRManInterfaceCall(aLineNo)
{
	enterValues(ustep, vstep, type, nu, uwrap, nv, vwrap);
	CPatchMeshClasses p(type, nu, ustep, uwrap, nv, vstep, vwrap);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

CRiPatchMesh::CRiPatchMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
	const CParameterList &theParameters)
	: CUVRManInterfaceCall(aLineNo)
{
	enterValues(ustep, vstep, type, nu, uwrap, nv, vwrap);
}

///////////////////////////////////////////////////////////////////////////////
CRiNuPatch::CRiNuPatch(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr, RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo), m_nu(nu), m_uorder(uorder), m_nv(nv), m_vorder(vorder), m_umin(umin), m_umax(umax), m_vmin(vmin), m_vmax(vmax)
{
	m_uknot.resize(nu+uorder);
	m_uknot.assign(uknot, uknot+nu+uorder);
	m_vknot.resize(nv+vorder);
	m_vknot.assign(vknot, vknot+nv+vorder);

	CNuPatchClasses p(nu, uorder, nv, vorder);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
CRiSubdivisionMesh::CRiSubdivisionMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
	RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo)
{
//	CRiSubdivisionMeshData data;
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

	CSubdivisionMeshClasses p(nfaces, nverts, verts);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
CRiCurves::CRiCurves(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt ustep, RtInt vstep, RtToken type,
	RtInt ncurves, RtInt nverts[], RtToken wrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo), m_type(type), m_wrap(wrap), m_ncurves(ncurves), m_ustep(ustep), m_vstep(vstep)
{
	m_nverts.resize(ncurves);
	m_nverts.assign(nverts, nverts+ncurves);

	CCurvesClasses p(type, ncurves, nverts, wrap, vstep);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
CRiBlobby::CRiBlobby(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt nleaf, RtInt ncode, RtInt code[],
	RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo), m_nleaf(nleaf), m_ncode(ncode), m_nflt(nflt), m_nstr(nstr)
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

	CBlobbyClasses p(nleaf);
	setParams(decl, p, curColorDescr, n, tokens, params);
}

///////////////////////////////////////////////////////////////////////////////
void CRiProcedural::insertData(RtPointer data, IRi &ri) {
	// std::deque<std::string> m_strcontainer;
	// std::vector<RtString> m_str;
	const char **cp = (const char **)data;
	m_strcontainer.clear();
	m_str.clear();

	if ( !m_subdivfunc )
		return;

	if ( m_subdivfunc->name() == RI_READARCHIVE ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
	} else if ( m_subdivfunc->name() == RI_DYNAMIC_LOAD ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
	} else if ( m_subdivfunc->name() == RI_RUN_PROGRAM ) {
		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
		m_strcontainer.push_back(cp && cp[1] ? cp[1] : "");
		m_str.push_back(m_strcontainer[1].c_str());
	}
}
