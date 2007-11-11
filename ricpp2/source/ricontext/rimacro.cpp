/*! \file rimacro.cpp
 *  \brief Implementation of the macro classes, records interface calls
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ricpp/ricontext/rimacro.h"

using namespace RiCPP;

///////////////////////////////////////////////////////////////////////////////
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
				(*i)->replay(ri, callback);
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
void CRiGeneralPolygon::enterValues(RtInt theNLoops, RtInt *theNVerts)
{
	m_nVerts.resize(theNLoops);
	m_nVerts.assign(theNVerts, theNVerts+theNLoops);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt theNLoops, RtInt *theNVerts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo,
		decl, CGeneralPolygonClasses(theNLoops, theNVerts), curColorDescr,
		n, tokens, params)
{
	enterValues(theNLoops, theNVerts);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo,
	RtInt theNLoops, RtInt *theNVerts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo, theParameters)
{
	enterValues(theNLoops, theNVerts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsPolygons::enterValues(RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts)
{
	m_nVerts.resize(theNPolys);
	m_nVerts.assign(theNVerts, theNVerts+theNPolys);

	RtInt i, nIdx = 0; // Sum of nverts
	for ( i = 0; i < theNPolys; ++i )
		nIdx += theNVerts[i];
	m_verts.resize(nIdx);
	m_verts.assign(theVerts, theVerts+nIdx);
}

CRiPointsPolygons::CRiPointsPolygons(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo,
		decl, CPointsPolygonsClasses(theNPolys, theNVerts, theVerts), curColorDescr,
		n, tokens, params)
{
	enterValues(theNPolys, theNVerts, theVerts);
}

CRiPointsPolygons::CRiPointsPolygons(
	long aLineNo,
	RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo, theParameters)
{
	enterValues(theNPolys, theNVerts, theVerts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsGeneralPolygons::enterValues(RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts)
{
	m_nLoops.resize(theNPolys);
	m_nLoops.assign(theNLoops, theNLoops+theNPolys);

	int i;
	RtInt sumLoop = 0;
	for ( i = 0; i < theNPolys; ++i )
		sumLoop += theNLoops[i];
	m_nVerts.resize(sumLoop);
	m_nVerts.assign(theNVerts, theNVerts+sumLoop);

	RtInt nIdx = 0; // Sum of theNVerts
	for ( i = 0; i < sumLoop; ++i )
		nIdx += theNVerts[i];
	m_verts.resize(nIdx);
	m_verts.assign(theVerts, theVerts+nIdx);

}

CRiPointsGeneralPolygons::CRiPointsGeneralPolygons(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CPolygonRManInterfaceCall(aLineNo,
		decl, CPointsGeneralPolygonsClasses(theNPolys, theNLoops, theNVerts, theVerts), curColorDescr,
		n, tokens, params)
{
	enterValues(theNPolys, theNLoops, theNVerts, theVerts);
}

CRiPointsGeneralPolygons::CRiPointsGeneralPolygons(
	long aLineNo,
	RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts,
	const CParameterList &theParameters)
	: CPolygonRManInterfaceCall(aLineNo, theParameters)
{
	enterValues(theNPolys, theNLoops, theNVerts, theVerts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPatchMesh::set(
		RtInt aUStep, RtInt aVStep,
		RtToken aType,
		RtInt aNu, RtToken aUWrap,
		RtInt aNv, RtToken aVWrap)
{
	m_ustep = aUStep;
	m_vstep = aVStep;
	m_type = aType;
	m_nu = aNu;
	m_uwrap = aUWrap;
	m_nv = aNv;
	m_vwrap = aVWrap;
}

CRiPatchMesh::CRiPatchMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt aUStep, RtInt aVStep,
	RtToken aType,
	RtInt aNu, RtToken aUWrap,
	RtInt aNv, RtToken aVWrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CUVRManInterfaceCall(aLineNo,
		decl, CPatchMeshClasses(aType, aNu, aUStep, aUWrap, aNv, aVStep, aVWrap), curColorDescr,
		n, tokens, params)
{
	set(aUStep, aVStep, aType, aNu, aUWrap, aNv, aVWrap);
}

CRiPatchMesh::CRiPatchMesh(
	long aLineNo,
	RtInt aUStep, RtInt aVStep,
	RtToken aType,
	RtInt aNu, RtToken aUWrap,
	RtInt aNv, RtToken aVWrap,
	const CParameterList &theParameters)
	: CUVRManInterfaceCall(aLineNo, theParameters)
{
	set(aUStep, aVStep, aType, aNu, aUWrap, aNv, aVWrap);
}

///////////////////////////////////////////////////////////////////////////////
void CRiNuPatch::set(
		RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax)
{
	m_nu     = aNU;
	m_uorder = aUOrder;
	m_nv     = aNV;
	m_vorder = aVOrder;
	m_umin   = aUMin;
	m_umax   = aUMax;
	m_vmin   = aVMin;
	m_vmax   = aVMax;

	m_uknot.resize(aNU+aUOrder);
	m_uknot.assign(aUKnot, aUKnot+aNU+aUOrder);
	m_vknot.resize(aNV+aVOrder);
	m_vknot.assign(aVKnot, aVKnot+aNV+aVOrder);
}

void CRiNuPatch::set(
		RtInt aNU, RtInt aUOrder, const std::vector<RtFloat> &aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const std::vector<RtFloat> &aVKnot, RtFloat aVMin, RtFloat aVMax)
{
	const RtFloat *aUKnotPtr = aUKnot.size() > 0 ? &(aUKnot[0]) : 0;
	const RtFloat *aVKnotPtr = aVKnot.size() > 0 ? &(aVKnot[0]) : 0;
	set(aNU, aUOrder, aUKnotPtr, aUMin, aUMax, aNV, aVOrder, aVKnotPtr, aVMin, aVMax);
}

CRiNuPatch::CRiNuPatch(
	long aLineNo,
	CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
	RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo, decl, CNuPatchClasses(aNU, aUOrder, aNV, aVOrder), curColorDescr, n, tokens, params)
{
	set(aNU, aUOrder, aUKnot, aUMin, aUMax, aNV, aVOrder, aVKnot, aVMin, aVMax);
}

CRiNuPatch::CRiNuPatch(
	long aLineNo,
	RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
	RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax,
	const CParameterList &theParameters)
	: CGeometryRManInterfaceCall(aLineNo, theParameters)
{
	set(aNU, aUOrder, aUKnot, aUMin, aUMax, aNV, aVOrder, aVKnot, aVMin, aVMax);
}

///////////////////////////////////////////////////////////////////////////////
void CRiSubdivisionMesh::set(
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[])
{
	m_scheme = aScheme;

	m_nverts.resize(aNFaces);
	m_nverts.assign(aNVerts, aNVerts+aNFaces);

	RtInt i;

	m_nedges = 0; // number of edges equals the number of vertices per face
	for ( i = 0; i < aNFaces; ++i ) {
		m_nedges += aNVerts[i];
	}

	m_verts.resize(m_nedges);
	m_verts.assign(aVerts, aVerts+m_nedges);
	if ( m_nedges > 0 )
		m_nvertices = 1+(RtInt)tmax(m_nedges, &(aVerts[0]));

	m_tags.reserve(aNTags);
	for ( i = 0; i < aNTags; ++i ) {
		m_tags.push_back(aTags[i]);
	}

	m_nargs.resize(aNTags*2);
	m_nargs.assign(aNArgs, aNArgs+aNTags*2);

	RtInt sumargs = 0;
	for ( i = 0; i < aNTags*2; i += 2 ) {
		sumargs += aNArgs[i];
	}
	m_intargs.resize(sumargs);
	m_intargs.assign(aIntArgs, aIntArgs+sumargs);

	sumargs = 0;
	for ( i = 1; i < aNTags*2; i += 2 ) {
		sumargs += aNArgs[i];
	}
	m_floargs.resize(sumargs);
	m_floargs.assign(aFloArgs, aFloArgs+sumargs);
}

void CRiSubdivisionMesh::set(
	RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
	const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
	const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs)
{
	set(
		m_scheme, (RtInt)m_nverts.size(),
		m_nverts.empty() ? 0 : &(m_nverts[0]),
		m_verts.empty() ? 0 : &(m_verts[0]),
		(RtInt)m_tags.size(),
		m_tags.empty() ? 0 : &(m_tags[0]),
		m_nargs.empty() ? 0 : &(m_nargs[0]),
		m_intargs.empty() ? 0 : &(m_intargs[0]),
		m_floargs.empty() ? 0 : &(m_floargs[0]));
}

CRiSubdivisionMesh::CRiSubdivisionMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo,
		decl, CSubdivisionMeshClasses(aNFaces, aNVerts, aVerts), curColorDescr,
		n, tokens, params)
{
	set(aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs);
}

CRiSubdivisionMesh::CRiSubdivisionMesh(
	long aLineNo,
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
	const CParameterList &theParameters)
	: CGeometryRManInterfaceCall(aLineNo, theParameters)
{
	set(aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs);
}


///////////////////////////////////////////////////////////////////////////////
void CRiHierarchicalSubdivisionMesh::set(
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[],
	RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[], RtToken aStrArgs[])
{
	m_scheme = aScheme;

	m_nverts.resize(aNFaces);
	m_nverts.assign(aNVerts, aNVerts+aNFaces);

	RtInt i;

	m_nedges = 0; // number of edges equals the number of vertices per face
	for ( i = 0; i < aNFaces; ++i ) {
		m_nedges += aNVerts[i];
	}

	m_verts.resize(m_nedges);
	m_verts.assign(aVerts, aVerts+m_nedges);
	if ( m_nedges > 0 )
		m_nvertices = 1+(RtInt)tmax(m_nedges, &(aVerts[0]));

	m_tags.reserve(aNTags);
	for ( i = 0; i < aNTags; ++i ) {
		m_tags.push_back(aTags[i]);
	}

	m_nargs.resize(aNTags*3);
	m_nargs.assign(aNArgs, aNArgs+aNTags*3);

	RtInt sumargs = 0;
	for ( i = 0; i < aNTags*2; i += 3 ) {
		sumargs += aNArgs[i];
	}
	m_intargs.resize(sumargs);
	m_intargs.assign(aIntArgs, aIntArgs+sumargs);

	sumargs = 0;
	for ( i = 1; i < aNTags*2; i += 3 ) {
		sumargs += aNArgs[i];
	}
	m_floargs.resize(sumargs);
	m_floargs.assign(aFloArgs, aFloArgs+sumargs);

	sumargs = 0;
	for ( i = 1; i < aNTags*2; i += 3 ) {
		sumargs += aNArgs[i];
	}
	m_strargs.resize(sumargs);
	m_strargs.assign(aStrArgs, aStrArgs+sumargs);
}

void CRiHierarchicalSubdivisionMesh::set(
	RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
	const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
	const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs,
	const std::vector<RtToken> &aStrArgs)
{
	set(
		m_scheme, (RtInt)m_nverts.size(),
		m_nverts.empty() ? 0 : &(m_nverts[0]),
		m_verts.empty() ? 0 : &(m_verts[0]),
		(RtInt)m_tags.size(),
		m_tags.empty() ? 0 : &(m_tags[0]),
		m_nargs.empty() ? 0 : &(m_nargs[0]),
		m_intargs.empty() ? 0 : &(m_intargs[0]),
		m_floargs.empty() ? 0 : &(m_floargs[0]),
		m_strargs.empty() ? 0 : &(m_strargs[0]));
}

CRiHierarchicalSubdivisionMesh::CRiHierarchicalSubdivisionMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
	RtToken aStrArgs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo,
		decl, CSubdivisionMeshClasses(aNFaces, aNVerts, aVerts), curColorDescr,
		n, tokens, params)
{
	set(aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs, aStrArgs);
}

CRiHierarchicalSubdivisionMesh::CRiHierarchicalSubdivisionMesh(
	long aLineNo,
	RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
	RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
	RtToken aStrArgs[],
	const CParameterList &theParameters)
	: CGeometryRManInterfaceCall(aLineNo, theParameters)
{
	set(aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs, aStrArgs);
}

///////////////////////////////////////////////////////////////////////////////
CRiCurves::CRiCurves(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt ustep, RtInt vstep, RtToken type,
	RtInt ncurves, RtInt nverts[], RtToken wrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo, decl,
	    CCurvesClasses(type, ncurves, nverts, wrap, vstep), curColorDescr,
		n, tokens, params),
	  m_type(type), m_wrap(wrap), m_ncurves(ncurves), m_ustep(ustep), m_vstep(vstep)
{
	m_nverts.resize(ncurves);
	m_nverts.assign(nverts, nverts+ncurves);
}

///////////////////////////////////////////////////////////////////////////////
CRiBlobby::CRiBlobby(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt nleaf, RtInt ncode, RtInt code[],
	RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: CGeometryRManInterfaceCall(aLineNo,
		decl, CBlobbyClasses(nleaf), curColorDescr,
		n, tokens, params),
	  m_nleaf(nleaf), m_ncode(ncode), m_nflt(nflt), m_nstr(nstr)
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
