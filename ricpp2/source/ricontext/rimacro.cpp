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

/** @file rimacro.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the macro classes, records interface calls
 */

#include "ricpp/ricontext/rimacro.h"

#ifndef _RICPP_RICONTEXT_TRIANGULATION_H
#include "ricpp/ricontext/triangulation.h"
#endif // _RICPP_RICONTEXT_TRIANGULATION_H

using namespace RiCPP;

///////////////////////////////////////////////////////////////////////////////


CRiMacro::CRiMacro(RtString anId,
				   unsigned long aHandleNo, bool isFromHandleId,
				   CRManInterfaceFactory *aFactory,
				   EnumMacroTypes macroType)
: CHandle(anId, aHandleNo, isFromHandleId), m_factory(aFactory),
m_macroType(macroType), m_isClosed(false), m_postpone(true)
{
}

CRiMacro::CRiMacro(const CRiMacro &aMacro)
{
	m_factory = 0;
	*this = aMacro;
}

CRiMacro::~CRiMacro()
{
	clear();
	m_factory = 0;
}


void CRiMacro::clear()
{
	MacroContainerType::iterator i;
	if ( m_factory ) {
		for ( i = m_calls.begin(); i != m_calls.end(); ++i ) {
			m_factory->deleteRequest(*i);
		}
	} else {
		for ( i = m_calls.begin(); i != m_calls.end(); ++i ) {
			delete *i;
		}
	}
	m_calls.clear();
}

CRiMacro &CRiMacro::operator=(const CRiMacro &aMacro)
{
	if ( &aMacro == this )
		return *this;

	clear();
	m_path = aMacro.m_path;
	m_factory = aMacro.m_factory;
	m_macroType = aMacro.m_macroType;
	m_isClosed = aMacro.m_isClosed;
	m_postpone = aMacro.m_postpone;
	
	MacroContainerType::const_iterator i;

	if ( m_factory ) {
		for ( i = aMacro.m_calls.begin(); i != aMacro.m_calls.end(); ++i ) {
			if ( *i )
				add(m_factory->duplicateRequest(*i));
		}
	} else {
		for ( i = aMacro.m_calls.begin(); i != aMacro.m_calls.end(); ++i ) {
			if ( *i )
				add((*i)->duplicate());
		}
	}
	return *this;
}

bool CRiMacro::add(CRManInterfaceCall *c)
{
	if ( !c )
		return false;
	m_calls.push_back(c);
	return true;
}

void CRiMacro::replay(IDoRender &ri, const IArchiveCallback *callback)
{
	CRenderState *state = ri.renderState();
	if ( !state )
		return;

	RtToken archiveNameStored = state->archiveName();
	long linNoStored = state->lineNo();
	
	state->archiveName(handle());

	for ( MacroContainerType::iterator i = m_calls.begin(); i != m_calls.end(); ++i ) {
		try {
			state->lineNo((*i)->lineNo());
			(*i)->replay(ri, callback);
		} catch ( ExceptRiCPPError &e2 ) {
			state->lineNo(linNoStored);
			state->archiveName(archiveNameStored);
			throw e2;
		}
	}
	state->lineNo(linNoStored);
	state->archiveName(archiveNameStored);
}

///////////////////////////////////////////////////////////////////////////////
void CRManInterfaceCall::replay(IDoRender &ri, const IArchiveCallback *cb)
{
	ri.replayRequest(*this, cb);
}

///////////////////////////////////////////////////////////////////////////////
void CVarParamRManInterfaceCall::detach()
{
	CTesselator *aTesselator = m_tesselator;
	m_tesselator = 0;
	if ( aTesselator ) {
		aTesselator->detach();
	}
}

void CVarParamRManInterfaceCall::attach(CTesselator *aTesselator)
{
	if ( m_tesselator == aTesselator )
		return;

	detach();
	m_tesselator = aTesselator;
	if ( aTesselator )
		aTesselator->attach(this);
}

void CVarParamRManInterfaceCall::writeRIB(CRibElementsWriter &ribWriter, RtInt n, const RtToken ignoreTokens[]) const
{
	bool paramError = false;
	for ( CParameterList::const_iterator i = parameters().begin();
		  i != parameters().end();
		  i++ )
	{
		const CParameter &p = (*i);

		RtInt cnt = 0;
		for ( ; cnt < n; ++cnt ) {
			if ( p.token() == ignoreTokens[cnt] )
				break;
		}
		if ( cnt < n ) continue;
		
		ribWriter.putBlank();

		if ( p.isInline() )
			ribWriter.putString(p.parameterName());
		else
			ribWriter.putStringToken(p.parameterName());

		ribWriter.putBlank();

		switch ( p.basicType() ) {
			case BASICTYPE_INTEGER:
				ribWriter.putArray(p.ints());
				break;
			case BASICTYPE_FLOAT:
				ribWriter.putArray(p.floats());
				break;
			case BASICTYPE_STRING:
				ribWriter.putArray(p.stringPtrs());
				break;
			default:
				// Error unknown type
				paramError = true;
				ribWriter.putChars("[ ]");
				break;
		}
	}

	if ( paramError ) {
		// throw error
		throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, __LINE__, __FILE__, "Unknown basic type of parameters indicated by [ ].");
	}

	TypeParent::writeRIB(ribWriter, n, ignoreTokens);
}

/*
void CVarParamRManInterfaceCall::getBounds(RtBound bounds) const
{
	// @todo Bounds, first consider RI_P then (if not found) RI_PW
}
 */

///////////////////////////////////////////////////////////////////////////////
void CRiGeneralPolygon::enterValues(RtInt theNLoops, const RtInt theNVerts[])
{
	m_nVerts.resize(theNLoops);
	m_nVerts.assign(theNVerts, theNVerts+theNLoops);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt theNLoops, const RtInt theNVerts[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CGeneralPolygonClasses(theNLoops, theNVerts), curColorDescr,
		n, tokens, params), m_triangulated(0)
{
	enterValues(theNLoops, theNVerts);
}

CRiGeneralPolygon::CRiGeneralPolygon(
	long aLineNo,
	RtInt theNLoops, const RtInt theNVerts[],
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters), m_triangulated(0)
{
	enterValues(theNLoops, theNVerts);
}


const CTriangulatedPolygon *CRiGeneralPolygon::triangulate(const IPolygonTriangulationStrategy &strategy, bool frontCW)
{
	if ( m_triangulated )
		return m_triangulated;

	if ( m_nVerts.size() == 0 )
		return 0;

	const CParameter *par = parameters().get(RI_P);
	if ( !par )
		return 0;

	const std::vector<RtFloat> &f = par->floats();
	assert(f.size() > 0);
	if ( f.size() == 0 )
		return m_triangulated;

	// Triangulate polygon
	m_triangulated = new CTriangulatedPolygon();
	if ( m_triangulated )
		m_triangulated->triangulate(strategy, (RtInt)m_nVerts.size(), &m_nVerts[0], &f[0], frontCW);
	
	return m_triangulated;
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsPolygons::enterValues(RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[])
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
	RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CPointsPolygonsClasses(theNPolys, theNVerts, theVerts), curColorDescr,
		n, tokens, params)
{
	enterValues(theNPolys, theNVerts, theVerts);
}

CRiPointsPolygons::CRiPointsPolygons(
	long aLineNo,
	RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[],
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters)
{
	enterValues(theNPolys, theNVerts, theVerts);
}

///////////////////////////////////////////////////////////////////////////////
void CRiPointsGeneralPolygons::enterValues(RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[])
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
	RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CPointsGeneralPolygonsClasses(theNPolys, theNLoops, theNVerts, theVerts), curColorDescr,
		n, tokens, params)
{
	enterValues(theNPolys, theNLoops, theNVerts, theVerts);
}

CRiPointsGeneralPolygons::CRiPointsGeneralPolygons(
	long aLineNo,
	RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[],
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters)
{
	enterValues(theNPolys, theNLoops, theNVerts, theVerts);
}

const std::vector<CTriangulatedPolygon> &CRiPointsGeneralPolygons::triangulate(const IPolygonTriangulationStrategy &strategy, bool frontCW)
{
	if ( m_triangulated.size() > 0 )
		return m_triangulated;

	if ( m_nLoops.size() == 0 )
		return m_triangulated;
	
	if ( m_nVerts.size() == 0 )
		return m_triangulated;

	if ( m_verts.size() == 0 )
		return m_triangulated;
	
	const CParameter *par = parameters().get(RI_P);
	if ( !par )
		return m_triangulated;
	
	const std::vector<RtFloat> &f = par->floats();
	assert(f.size() > 0);
	if ( f.size() == 0 )
		return m_triangulated;
	
	// Triangulate polygons
	m_triangulated.resize(m_nLoops.size());
	RtInt nvertscnt = 0, vertscnt = 0;
	RtInt j;
	for ( unsigned int i = 0; i < m_nLoops.size(); ++i ) {
		m_triangulated[i].triangulate(strategy, m_nLoops[i], &m_nVerts[nvertscnt], &m_verts[vertscnt], &f[0], frontCW);
		for ( j = 0; j < m_nLoops[i]; ++j ) {
			vertscnt += m_nVerts[nvertscnt++];
		}
	}
	
	return m_triangulated;
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
	: TypeParent(aLineNo,
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
	: TypeParent(aLineNo, theParameters)
{
	set(aUStep, aVStep, aType, aNu, aUWrap, aNv, aVWrap);
}

///////////////////////////////////////////////////////////////////////////////
void CRiNuPatch::set(
		RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax)
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
	RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
	RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo, decl, CNuPatchClasses(aNU, aUOrder, aNV, aVOrder), curColorDescr, n, tokens, params)
{
	set(aNU, aUOrder, aUKnot, aUMin, aUMax, aNV, aVOrder, aVKnot, aVMin, aVMax);
}

CRiNuPatch::CRiNuPatch(
	long aLineNo,
	RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
	RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax,
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters)
{
	set(aNU, aUOrder, aUKnot, aUMin, aUMax, aNV, aVOrder, aVKnot, aVMin, aVMax);
}

///////////////////////////////////////////////////////////////////////////////
void CRiSubdivisionMesh::set(
	CTokenMap *tokenMap, 
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[])
{
	m_scheme = tokenMap ? tokenMap->findCreate(aScheme) : aScheme;

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

	m_tags.resize(0);
	m_tags.reserve(aNTags);
	for ( i = 0; i < aNTags; ++i ) {
		m_tags.push_back(tokenMap ? tokenMap->findCreate(aTags[i]) : aTags[i]);
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
	CTokenMap *tokenMap, 
	RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
	const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
	const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs)
{
	set(
		tokenMap,
		aScheme, (RtInt)aNVerts.size(),
		aNVerts.empty() ? 0 : &(aNVerts[0]),
		aVerts.empty() ? 0 : &(aVerts[0]),
		(RtInt)aTags.size(),
		aTags.empty() ? 0 : &(aTags[0]),
		aNArgs.empty() ? 0 : &(aNArgs[0]),
		aIntArgs.empty() ? 0 : &(aIntArgs[0]),
		aFloArgs.empty() ? 0 : &(aFloArgs[0]));
}

CRiSubdivisionMesh::CRiSubdivisionMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CSubdivisionMeshClasses(aNFaces, aNVerts, aVerts), curColorDescr,
		n, tokens, params)
{
	set(&decl.tokenMap(), aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs);
}

CRiSubdivisionMesh::CRiSubdivisionMesh(
	long aLineNo, CTokenMap *tokenMap, 
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters)
{
	set(tokenMap, aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs);
}


///////////////////////////////////////////////////////////////////////////////
void CRiHierarchicalSubdivisionMesh::set(
	CTokenMap *tokenMap, 
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[],
	const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[], const RtToken aStrArgs[])
{
	m_scheme = tokenMap ? tokenMap->findCreate(aScheme) : aScheme;

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

	m_tags.resize(0);
	m_tags.reserve(aNTags);
	for ( i = 0; i < aNTags; ++i ) {
		m_tags.push_back(tokenMap ? tokenMap->findCreate(aTags[i]) : aTags[i]);
	}

	m_nargs.resize(aNTags*3);
	m_nargs.assign(aNArgs, aNArgs+aNTags*3);

	RtInt sumargs = 0;
	for ( i = 0; i < aNTags*3; i += 3 ) {
		sumargs += aNArgs[i];
	}
	m_intargs.resize(sumargs);
	m_intargs.assign(aIntArgs, aIntArgs+sumargs);

	sumargs = 0;
	for ( i = 1; i < aNTags*3; i += 3 ) {
		sumargs += aNArgs[i];
	}
	m_floargs.resize(sumargs);
	m_floargs.assign(aFloArgs, aFloArgs+sumargs);

	sumargs = 0;
	for ( i = 2; i < aNTags*3; i += 3 ) {
		sumargs += aNArgs[i];
	}

	m_strargs.resize(sumargs);
	for ( i = 0; i < sumargs; i += 0 ) {
		m_strargs[i] = noNullStr(aStrArgs[i]);
	}

	m_strptrargs.resize(sumargs);
	for ( i = 0; i < sumargs; i += 0 ) {
		m_strptrargs[i] = m_strargs[i].c_str();
	}
}

void CRiHierarchicalSubdivisionMesh::set(
	CTokenMap *tokenMap, 
	RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
	const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
	const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs,
	const std::vector<RtToken> &aStrArgs)
{
	set(
		tokenMap,
		aScheme, (RtInt)aNVerts.size(),
		aNVerts.empty() ? 0 : &(aNVerts[0]),
		aVerts.empty() ? 0 : &(aVerts[0]),
		(RtInt)aTags.size(),
		aTags.empty() ? 0 : &(aTags[0]),
		aNArgs.empty() ? 0 : &(aNArgs[0]),
		aIntArgs.empty() ? 0 : &(aIntArgs[0]),
		aFloArgs.empty() ? 0 : &(aFloArgs[0]),
		aStrArgs.empty() ? 0 : &(aStrArgs[0]));
}

CRiHierarchicalSubdivisionMesh::CRiHierarchicalSubdivisionMesh(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
	const RtToken aStrArgs[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CSubdivisionMeshClasses(aNFaces, aNVerts, aVerts), curColorDescr,
		n, tokens, params)
{
	set(&decl.tokenMap(), aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs, aStrArgs);
}

CRiHierarchicalSubdivisionMesh::CRiHierarchicalSubdivisionMesh(
	long aLineNo, CTokenMap *tokenMap, 
	RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
	RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
	const RtToken aStrArgs[],
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters)
{
	set(tokenMap, aScheme, aNFaces, aNVerts, aVerts, aNTags, aTags, aNArgs, aIntArgs, aFloArgs, aStrArgs);
}


CRiHierarchicalSubdivisionMesh::CRiHierarchicalSubdivisionMesh(const CRiSubdivisionMesh &c) : TypeParent(c)
{
	
	std::vector<RtInt> nArgs3((c.nArgs().size()/2)*3);
	std::vector<RtToken> strArgsEmpty;
	
	if ( !c.nArgs().empty() ) {
		for ( size_t i = 0, j = 0; i<c.nArgs().size()-1; ) {
			nArgs3[j++] = c.nArgs()[i++];
			nArgs3[j++] = c.nArgs()[i++];
			nArgs3[j++] = 0;
		}
	}
	set(0, c.scheme(), c.nVerts(), c.verts(), c.tags(), nArgs3, c.intArgs(), c.floatArgs(), strArgsEmpty); 
}

///////////////////////////////////////////////////////////////////////////////
CRiCurves::CRiCurves(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt aStep, RtToken aType,
	RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap,
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo, decl,
	    CCurvesClasses(aType, aNCurves, aNVerts, aWrap, aStep), curColorDescr,
		n, tokens, params),
	  m_type(aType), m_wrap(aWrap), m_step(aStep)
{
	m_nverts.resize(aNCurves);
	m_nverts.assign(aNVerts, aNVerts+aNCurves);
}

CRiCurves::CRiCurves(
	long aLineNo,
	RtInt aStep, RtToken aType,
	RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap,
	const CParameterList &theParameters)
	: TypeParent(aLineNo, theParameters),
	  m_type(aType), m_wrap(aWrap), m_step(aStep)
{
	m_nverts.resize(aNCurves);
	m_nverts.assign(aNVerts, aNVerts+aNCurves);
}

void CRiCurves::set(
	RtInt aStep, RtToken aType,
	RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap)
{
	m_type = aType;
	m_wrap = aWrap;
	m_step = aStep;
	m_nverts.resize(aNCurves);
	m_nverts.assign(aNVerts, aNVerts+aNCurves);
}

void CRiCurves::set(
	RtInt aStep, RtToken aType,
	const std::vector<RtInt> &aNVerts, RtToken aWrap)
{
	m_type = aType;
	m_wrap = aWrap;
	m_step = aStep;
	m_nverts = aNVerts;
}

void CRiCurves::get(
	RtInt &aStep, RtToken &aType,
	std::vector<RtInt> &aNVerts, RtToken &aWrap)
{
	aType = m_type;
	aWrap = m_wrap;
	aStep = m_step;
	aNVerts = m_nverts;
}

///////////////////////////////////////////////////////////////////////////////
CRiBlobby::CRiBlobby(
	long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
	RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
	RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[],
	RtInt n, RtToken tokens[], RtPointer params[])
	: TypeParent(aLineNo,
		decl, CBlobbyClasses(aNLeaf), curColorDescr,
		n, tokens, params)
{
	set(aNLeaf, aNCode, aCode, aNFlt, aFlt, aNStr, aStr);
}

CRiBlobby::CRiBlobby(long aLineNo,
	RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
	RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[],
	const CParameterList &theParameters
	)
	: TypeParent(aLineNo, theParameters)
{
	set(aNLeaf, aNCode, aCode, aNFlt, aFlt, aNStr, aStr);
}

void CRiBlobby::set(
	RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
	RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[])
{
	m_nleaf = aNLeaf;

	m_code.resize(aNCode);
	m_code.assign(aCode, aCode+aNCode);
	m_flt.resize(aNFlt);
	m_flt.assign(aFlt, aFlt+aNFlt);

	int i;
	m_strcontainer.resize(0);
	m_str.resize(0);
	// m_strcontainer.reserve(aNStr);
	m_str.reserve(aNStr);
	for ( i = 0; i < aNStr; ++i ) {
		m_strcontainer.push_back(aStr[i]);
	}
	for ( i = 0; i < aNStr; ++i ) {
		m_str.push_back(m_strcontainer[i].c_str());
	}
}

void CRiBlobby::set(
	RtInt aNLeaf, const std::vector<RtInt> &aCode,
	const std::vector<RtFloat> &aFlt, const std::vector<RtString> &aStr)
{
	m_nleaf = aNLeaf;

	m_code = aCode;
	m_flt = aFlt;

	int i;
	m_strcontainer.resize(0);
	m_str.resize(0);
	// m_strcontainer.reserve(aNStr);
	RtInt aNStr = (RtInt)aStr.size();
	m_str.reserve(aNStr);
	for ( i = 0; i < aNStr; ++i ) {
		m_strcontainer.push_back(aStr[i]);
	}
	for ( i = 0; i < aNStr; ++i ) {
		m_str.push_back(m_strcontainer[i].c_str());
	}
}


void CRiBlobby::get(
	RtInt &aNLeaf, std::vector<RtInt> &aCode,
	std::vector<RtFloat> &aFlt, std::vector<RtString> &aStr) const
{
	aNLeaf = m_nleaf;
	aCode = m_code;
	aFlt = m_flt;
	aStr = m_str;
}

///////////////////////////////////////////////////////////////////////////////
void CRiProcedural::insertData(RtPointer data)
{
	if ( m_data )
		delete m_data;
	m_data = 0;
	
	if ( !m_subdivfunc )
		return;

	m_data = m_subdivfunc->createSubdivData(data);
}

void CRiProcedural::insertData(const ISubdivData *data)
{
	if ( m_data )
		delete m_data;
	m_data = 0;

	if ( data )
		m_data = data->duplicate();
}

CRiProcedural::CRiProcedural(
	long aLineNo, RtPointer data, const RtBound bound,
	ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
	: CRManInterfaceCall(aLineNo)
{
	m_data = 0;
	m_subdivfunc = &subdivfunc.singleton();
	m_freefunc = freefunc ? &freefunc->singleton() : 0;
	memcpy(m_bound, bound, sizeof(RtBound));
	insertData(data);
}

inline CRiProcedural &CRiProcedural::operator=(const CRiProcedural &c)
{
	if ( this == &c )
		return *this;

	m_subdivfunc = 0;
	m_freefunc = 0;

	if ( c.m_subdivfunc ) {
		m_subdivfunc = &c.m_subdivfunc->singleton();
	}

	if ( c.m_freefunc ) {
		m_freefunc = &c.m_freefunc->singleton();
	}
	memcpy(m_bound, c.m_bound, sizeof(RtBound));
	insertData(c.m_data);

	CRManInterfaceCall::operator=(c);
	return *this;
}
