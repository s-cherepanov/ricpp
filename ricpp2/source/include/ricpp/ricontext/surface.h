#ifndef _RICPP_RICONTEXT_SURFACE_H
#define _RICPP_RICONTEXT_SURFACE_H

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

/** @file surface.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Container class for surfaces
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H
#include <list>

namespace RiCPP {

//
// TemplPrimVar
//
template<typename ValueType> class TemplPrimVar {
private:
	const CDeclaration *m_decl;
	std::vector<ValueType> m_values;
public:
	inline TemplPrimVar() { m_decl = 0; }
	inline explicit TemplPrimVar(const CDeclaration &theDecl) { m_decl = &theDecl; }

	inline std::vector<ValueType> &values() { return m_values; }
	inline const std::vector<ValueType> &values() const { return m_values; }

	inline void declarationPtr(const CDeclaration *theDecl) { m_decl = theDecl; }
	inline const CDeclaration *declarationPtr() const { return m_decl; }
	inline const CDeclaration &declaration() const
	{
		if ( !declarationPtr() ) {
			throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE,
								   "Declaration of a parameter in TemplPrimVar::declaration() not defined.",
								   __LINE__, __FILE__);
		}
		return *declarationPtr();
	}
	
	inline bool isConstant() const
	{
		if ( declaration.storageClass() == CLASS_UNKNOWN ) {
			throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE,
								   "Storageclass type of a parameter in TemplPrimVar::isConstant() not defined.",
								   __LINE__, __FILE__);
		}
		return declaration().storageClass() == CLASS_CONSTANT || declaration().storageClass() == CLASS_UNIFORM;
	}
	
	inline IndexType vectorSize() const
	{
		return m_values().size();
	}

	inline IndexType size() const
	{
		return m_values().size() / declaration().elemSize();
	}
}; // TemplPrimVar


//
// EnumFaceTypes
//
	
/** @brief Possible face types for @c CFace
 */
enum EnumFaceTypes {
	FACETYPE_UNKNOWN = 0,    ///< Unknown face type
	FACETYPE_TRIANGLES,      ///< Triangles
	FACETYPE_TRIANGLESTRIPS ///< Triangle strips
};

/** @brief Number of face types.
 */
const int N_FACETYPES = (int)FACETYPE_TRIANGLESTRIPS+1;
	
//
// CFace
//
class CFace {
public:
	typedef std::map<RtToken, TemplPrimVar<RtFloat> >::const_iterator TypeConstFloatIterator;
	typedef std::map<RtToken, TemplPrimVar<RtInt> >::const_iterator TypeConstIntIterator;
	typedef std::map<RtToken, TemplPrimVar<std::string> >::const_iterator TypeConstStringIterator;
	
	typedef std::map<RtToken, TemplPrimVar<RtFloat> >::iterator TypeFloatIterator;
	typedef std::map<RtToken, TemplPrimVar<RtInt> >::iterator TypeIntIterator;
	typedef std::map<RtToken, TemplPrimVar<std::string> >::iterator TypeStringIterator;

private:
	EnumFaceTypes m_faceType;
	std::vector<IndexType> m_sizes;
	std::vector<IndexType> m_indices;

	std::map<RtToken, TemplPrimVar<RtFloat> > m_floats;
	std::map<RtToken, TemplPrimVar<RtInt> > m_ints;
	std::map<RtToken, TemplPrimVar<std::string> > m_strings;
		
public:	
	inline CFace() : m_faceType(FACETYPE_UNKNOWN) {}
	inline TemplPrimVar<RtFloat> &reserveFloats(const CDeclaration &decl)
	{
		TemplPrimVar<RtFloat> &r = m_floats[decl.token()];
		r.declarationPtr(&decl);
		return r;
	}
	
	inline TemplPrimVar<RtInt> &reserveInts(const CDeclaration &decl) {
		TemplPrimVar<RtInt> &r = m_ints[decl.token()];
		r.declarationPtr(&decl);
		return r;
	}
	
	inline TemplPrimVar<std::string> &reserveStrings(const CDeclaration &decl)  {
		TemplPrimVar<std::string> &r = m_strings[decl.token()];
		r.declarationPtr(&decl);
		return r;
	}
	
	inline void faceType(EnumFaceTypes ft)
	{
		m_faceType = ft;
	}
	
	inline EnumFaceTypes faceType() const
	{
		return m_faceType;
	}

	inline std::vector<IndexType> &sizes()
	{
		return m_sizes;
	}
	
	inline const std::vector<IndexType> &sizes() const
	{
		return m_sizes;
	}
	
	inline std::vector<IndexType> &indices()
	{
		return m_indices;
	}
	
	inline const std::vector<IndexType> &indices() const
	{
		return m_indices;
	}
	
	inline const TemplPrimVar<RtFloat> *floats(const RtToken token) const
	{
		TypeConstFloatIterator iter = m_floats.find(token);
		if ( iter == m_floats.end() )
			return 0;
		return &(*iter).second;
	}
	
	inline const TemplPrimVar<RtInt> *ints(const RtToken token) const
	{
		TypeConstIntIterator iter = m_ints.find(token);
		if ( iter == m_ints.end() )
			return 0;
		return &(*iter).second;
	}
	
	inline const TemplPrimVar<std::string> *strings(const RtToken token) const
	{
		TypeConstStringIterator iter = m_strings.find(token);
		if ( iter == m_strings.end() )
			return 0;
		return &(*iter).second;
	}

	inline TemplPrimVar<RtFloat> *floats(const RtToken token) { return const_cast<TemplPrimVar<RtFloat> *>(((const CFace *)this)->floats(token)); }
	inline TemplPrimVar<RtInt> *ints(const RtToken token) { return const_cast<TemplPrimVar<RtInt> *>(((const CFace *)this)->ints(token)); }
	inline TemplPrimVar<std::string> *strings(const RtToken token) { return const_cast<TemplPrimVar<std::string> *>(((const CFace *)this)->strings(token)); }
	
	inline TypeConstFloatIterator floatsBegin() const { return m_floats.begin(); }
	inline TypeConstFloatIterator floatsEnd() const { return m_floats.end(); }
	inline TypeFloatIterator floatsBegin() { return m_floats.begin(); }
	inline TypeFloatIterator floatsEnd() { return m_floats.end(); }

	inline TypeConstIntIterator intsBegin() const { return m_ints.begin(); }
	inline TypeConstIntIterator intsEnd() const { return m_ints.end(); }
	inline TypeIntIterator intsBegin() { return m_ints.begin(); }
	inline TypeIntIterator intsEnd() { return m_ints.end(); }

	inline TypeConstStringIterator stringsBegin() const { return m_strings.begin(); }
	inline TypeConstStringIterator stringsEnd() const { return m_strings.end(); }
	inline TypeStringIterator stringsBegin() { return m_strings.begin(); }
	inline TypeStringIterator stringsEnd() { return m_strings.end(); }
	
	void insertConst(const CParameter &p);
	void insertUniform(const CParameter &p, RtInt face);
	void insertVarying(const CParameter &p, RtInt nverts, const RtInt *verts);
	void insertFaceVarying(const CParameter &p, RtInt nverts, RtInt offs);

	TemplPrimVar<RtFloat> &insertFloatVar(const CDeclaration &decl, IndexType nVar);

	void buildStripIndices(IndexType tessU, IndexType tessV, bool isLH);
	void buildTriangleIndices(IndexType tessU, IndexType tessV, bool isLH);
	
	/** @brief Bilinear blending of a 2x2 patch
     *  cornerIdx
	 *  0    1    u->
	 * 
	 *  2    3    
	 */
	bool bilinearBlend(const CParameter &source,
					   const IndexType (& cornerIdx)[4],
					   IndexType tessU,
					   IndexType tessV);
	
	bool bilinearBlend(const CParameter &source,
					   const std::vector<IndexType> &cornerIdx,
					   IndexType tessU,
					   IndexType tessV);
	
	bool bicubicBlend(const CParameter &source,
					  const IndexType (& controlIdx)[16],
					  IndexType tessU,
					  IndexType tessV,
					  const CBicubicVectors &basisVectors);

	bool nuBlend(const CParameter &source,
							const std::vector<IndexType> &vertexIdx,
							RtInt useg, RtInt vseg,
							const CUVBSplineBasis &basis);

	inline void clearIndices()
	{
		m_faceType = FACETYPE_UNKNOWN;
		m_indices.clear();
		m_sizes.clear();
	}
	
	inline void clear()
	{
		clearIndices();
		m_floats.clear();
		m_ints.clear();
		m_strings.clear();
	}
}; // CFace


//
// CSurface
//
class CSurface
{
public:
	typedef std::list<CFace>::const_iterator const_iterator;
	typedef std::list<CFace>::iterator iterator;

private:
	std::list<CFace> m_faces;
	IndexType m_tessU, m_tessV;

public:
	inline CSurface(IndexType aTessU, IndexType aTessV) : m_tessU(aTessU), m_tessV(aTessV) {}

	inline IndexType tessU() const { return m_tessU; }
	inline void tessU(IndexType aTessU) { m_tessU = aTessU; }
	inline IndexType tessV() const { return m_tessV; }
	inline void tessV(IndexType aTessV) { m_tessV = aTessV; }
	
	inline void tesselation(IndexType aTessU, IndexType aTessV)
	{
		tessU(aTessU);
		tessV(aTessV);
	}
	
	inline void tesselation(IndexType &aTessU, IndexType &aTessV) const
	{
		aTessU = tessU();
		aTessV = tessV();
	}

	inline CFace &newFace() { CFace aFace; m_faces.push_back(aFace); return m_faces.back(); }
	
	inline const_iterator begin() const { return m_faces.begin(); }
	inline const_iterator end() const { return m_faces.end(); }

	inline iterator begin() { return m_faces.begin(); }
	inline iterator end() { return m_faces.end(); }

	inline bool empty() const { return m_faces.empty(); }
}; // CSurface

}

#endif // _RICPP_RICONTEXT_SURFACE_H
