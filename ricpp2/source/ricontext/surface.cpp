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

/** @file triangulation.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements container class for surfaces.
 */

#include "ricpp/ricontext/surface.h"

#ifdef _DEBUG
#include <iostream>
#define _TRACE
#endif

using namespace RiCPP;

void CFace::insertConst(const CParameter &p)
{
	const CDeclaration *decl = p.declarationPtr();
	if ( !decl )
		return;
	
	switch ( decl->basicType() ) {
		case BASICTYPE_INTEGER: {
			TemplPrimVar<RtInt> &var = reserveInts(*decl);
			var.value() = p.ints();
			break;
		}
		case BASICTYPE_FLOAT: {
			TemplPrimVar<RtFloat> &var = reserveFloats(*decl);
			var.value() = p.floats();
			break;
		}
		case BASICTYPE_STRING: {
			TemplPrimVar<std::string> &var = reserveStrings(*decl);
			var.value() = p.strings();
			break;
		}
		default:
			break;
	}
}

void CFace::insertUniform(const CParameter &p, RtInt face)
{
	const CDeclaration *decl = p.declarationPtr();
	if ( !decl )
		return;
	
	switch ( decl->basicType() ) {
		case BASICTYPE_INTEGER: {
			TemplPrimVar<RtInt> &var = reserveInts(*decl);
			var.value().resize(decl->elemSize());
			std::vector<RtInt>::iterator iter = var.value().begin();
			p.extract(face, iter);
			break;
		}
		case BASICTYPE_FLOAT: {
			TemplPrimVar<RtFloat> &var = reserveFloats(*decl);
			var.value().resize(decl->elemSize());
			std::vector<RtFloat>::iterator iter = var.value().begin();
			p.extract(face, iter);
			break;
		}
		case BASICTYPE_STRING: {
			TemplPrimVar<std::string> &var = reserveStrings(*decl);
			var.value().resize(decl->elemSize());
			std::vector<std::string>::iterator iter = var.value().begin();
			p.extract(face, iter);
			break;
		}
		default:
			break;
	}
}

void CFace::insertVarying(const CParameter &p, RtInt nverts, const RtInt *verts)
{
	const CDeclaration *decl = p.declarationPtr();
	if ( !decl )
		return;
	
	switch ( decl->basicType() ) {
		case BASICTYPE_INTEGER: {
			TemplPrimVar<RtInt> &var = reserveInts(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<RtInt>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(verts[i], iter);
			}
			break;
		}
		case BASICTYPE_FLOAT: {
			TemplPrimVar<RtFloat> &var = reserveFloats(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<RtFloat>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(verts[i], iter);
			}
			break;
		}
		case BASICTYPE_STRING: {
			TemplPrimVar<std::string> &var = reserveStrings(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<std::string>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(verts[i], iter);
			}
			break;
		}
		default:
			break;
	}
}

void CFace::insertFaceVarying(const CParameter &p, RtInt nverts, RtInt offs)
{
	const CDeclaration *decl = p.declarationPtr();
	if ( !decl )
		return;
	
	switch ( decl->basicType() ) {
		case BASICTYPE_INTEGER: {
			TemplPrimVar<RtInt> &var = reserveInts(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<RtInt>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(offs+i, iter);
			}
			break;
		}
		case BASICTYPE_FLOAT: {
			TemplPrimVar<RtFloat> &var = reserveFloats(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<RtFloat>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(offs+i, iter);
			}
			break;
		}
		case BASICTYPE_STRING: {
			TemplPrimVar<std::string> &var = reserveStrings(*decl);
			var.value().resize(decl->elemSize()*nverts);
			std::vector<std::string>::iterator iter = var.value().begin();
			for ( RtInt i = 0; i < nverts; ++i ) {
				p.extract(offs+i, iter);
			}
			break;
		}
		default:
			break;
	}
}

TemplPrimVar<RtFloat> &CFace::insertFloatVar(const CDeclaration &decl, IndexType nVar)
{
	TemplPrimVar<RtFloat> &f = m_floats[decl.token()];
	f.decl(&decl);
	f.value().resize(nVar * decl.elemSize());
	return f;
}

void CFace::buildTriangleIndices(IndexType tessU, IndexType tessV, bool isLH)
{
	clearIndices();
	assert(tessU != 0 && tessV != 0);
	if ( tessU == 0 || tessV == 0 )
		return;
	
	m_faceType = FACETYPE_TRIANGLES;
	
	m_indices.resize(tessV*tessU*2*3);
	std::vector<IndexType>::iterator idxIter = m_indices.begin();
	
	for ( IndexType vIdx = 0; vIdx < tessV; ++vIdx ) {
		IndexType offs = vIdx * (tessU+1);
		
		for ( IndexType uIdx = 0; uIdx < tessU; ++uIdx ) {
			assert(idxIter != m_indices.end());
			*idxIter = offs + uIdx;
			idxIter++;
			if ( isLH ) {
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1;
				idxIter++;
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1 + tessU;
				idxIter++;
			} else {
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1 + tessU;
				idxIter++;
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1;
				idxIter++;
			}
		}
		
		offs += tessU+1;
		
		for ( IndexType uIdx = 0; uIdx < tessU; ++uIdx ) {
			assert(idxIter != m_indices.end());
			*idxIter = offs + uIdx;
			idxIter++;
			if ( isLH ) {
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx - tessU;
				idxIter++;
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1;
				idxIter++;
			} else {
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx+1;
				idxIter++;
				assert(idxIter != m_indices.end());
				*idxIter = offs + uIdx - tessU;
				idxIter++;
			}
		}
	}
	m_sizes.resize(1);
	m_sizes[0] = m_indices.size();
}

void CFace::buildStripIndices(IndexType tessU, IndexType tessV, bool isLH)
{
	clearIndices();
	assert(tessU != 0 && tessV != 0);
	if ( tessU == 0 || tessV == 0 )
		return;
	
	const IndexType uIndices = (tessU+1)*2;
	const IndexType nStrips = tessV;
	const IndexType lastRowIdx = (tessU+1)*tessV;
	
	m_faceType = FACETYPE_TRIANGLESTRIPS;
	
	m_indices.resize(nStrips*uIndices);
	std::vector<IndexType>::iterator idxIter = m_indices.begin();
	for ( IndexType startIdx = 0; startIdx < lastRowIdx; ) {
		const IndexType nextRowIdx = startIdx + tessU+1;
		for ( IndexType idx = startIdx; idx < nextRowIdx; ++idx) {
			assert(idxIter != m_indices.end());
			*idxIter = isLH ? idx+tessU+1 : idx;
			idxIter++;
			assert(idxIter != m_indices.end());
			*idxIter = isLH ? idx : idx+tessU+1;
			idxIter++;
		}
		startIdx = nextRowIdx;
	}
	
	m_sizes.resize(nStrips);
	std::vector<IndexType>::iterator sizeIter = m_sizes.begin();
	for ( ; sizeIter != m_sizes.end(); sizeIter++ ) {
		*sizeIter = uIndices;
	}
}


bool CFace::bilinearBlend(
	const CParameter &source,
	const IndexType (& cornerIdx)[4],
	IndexType tessU,
	IndexType tessV)
{
	assert ( source.basicType() == BASICTYPE_FLOAT );
	
	if ( tessU < 1 || tessV < 1 ) {
		return false;
	}

	IndexType elemSize = source.declaration().elemSize();
	const std::vector<RtFloat> &vals = source.floats();
	
	// For bilinear blending one needs 4 elems.
	IndexType sz = vals.size()/elemSize;
	if ( sz <= cornerIdx[0] || sz <= cornerIdx[1] || sz <= cornerIdx[2] || sz <= cornerIdx[3] )
		return false;
	
	std::vector<RtFloat> &retvals = reserveFloats(source.declaration()).value();
	retvals.resize((tessU+1)*(tessV+1)*elemSize);
	
	RtFloat deltau = (RtFloat)1.0/(RtFloat)(tessU);
	RtFloat deltav = (RtFloat)1.0/(RtFloat)(tessV);
	
	RtFloat u, v;
	IndexType ui, vi, ei, idx;
	IndexType startPos, endPos;

	for ( v = (RtFloat)0.0, vi = 0; vi < tessV+1; ++vi, v += deltav ) {
		if ( v > 1.0 || vi == tessV ) {
			v = 1.0;
		}
		startPos = vi * (tessU + 1) * elemSize;
		endPos   = startPos + tessU * elemSize;
		for ( ei = 0; ei < elemSize; ++ei ) {
			retvals[startPos+ei] = lerp(v, vals[cornerIdx[0]*elemSize+ei], vals[cornerIdx[2]*elemSize+ei]);
		}
		for ( ei = 0; ei < elemSize; ++ei ) {
			retvals[endPos+ei]   = lerp(v, vals[cornerIdx[1]*elemSize+ei], vals[cornerIdx[3]*elemSize+ei]);
		}
		idx = startPos+elemSize;
		for ( u = deltau, ui = 1; ui < tessU; ++ui, u += deltau ) {
			if ( u > 1.0 ) {
				u = 1.0;
			}
			for ( ei = 0; ei < elemSize; ++ei, ++idx ) {
				retvals[idx] = lerp(u, retvals[startPos+ei], retvals[endPos+ei]);
			}
		}
	}

	return true;
}
