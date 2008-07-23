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

#ifndef _RICPP_DECLARATION_DECLARATION_H
#include "ricpp/declaration/declaration.h"
#endif // _RICPP_DECLARATION_DECLARATION_H

#include <list>

namespace RiCPP {

//
// TemplPrimVar
//
template<typename ValueType> class TemplPrimVar {
private:
	const CDeclaration *m_decl;
	std::vector<ValueType> m_value;

public:
	inline TemplPrimVar() { m_decl = 0; }
	inline explicit TemplPrimVar(const CDeclaration &theDecl) { m_decl = &theDecl; }

	inline std::vector<ValueType> &value() { return m_value; }
	inline const std::vector<ValueType> &value() const { return m_value; }

	inline void decl(CDeclaration *theDecl) { m_decl = theDecl; }
	inline const CDeclaration *decl() const { return m_decl; }
}; // TemplPrimVar


//
// CFace
//
class CFace {
public:
	typedef unsigned long TypeIndex;
	
	typedef std::map<RtToken, TemplPrimVar<RtFloat> >::const_iterator TypeConstFloatIterator;
	typedef std::map<RtToken, TemplPrimVar<RtInt> >::const_iterator TypeConstIntIterator;
	typedef std::map<RtToken, TemplPrimVar<RtToken> >::const_iterator TypeConstTokenIterator;
	
private:
	std::vector<TypeIndex> m_indices;

	std::map<RtToken, TemplPrimVar<RtFloat> > m_floats;
	std::map<RtToken, TemplPrimVar<RtInt> > m_ints;
	std::map<RtToken, TemplPrimVar<RtToken> > m_tokens;
	
public:	
	inline TemplPrimVar<RtFloat> &reserveFloats(const CDeclaration &decl) { return m_floats[decl.token()]; }
	inline TemplPrimVar<RtInt> &reserveInts(const CDeclaration &decl) { return m_ints[decl.token()]; }
	inline TemplPrimVar<RtToken> &reserveTokens(const CDeclaration &decl)  { return m_tokens[decl.token()]; }

	inline const TemplPrimVar<RtFloat> &floats(const RtToken token) const
	{
		TypeConstFloatIterator iter = m_floats.find(token);
		if ( iter == m_floats.end() )
			throw;
		return (*iter).second;
	}
	
	inline const TemplPrimVar<RtInt> &ints(const RtToken token) const
	{
		TypeConstIntIterator iter = m_ints.find(token);
		if ( iter == m_ints.end() )
			throw;
		return (*iter).second;
	}
	
	inline const TemplPrimVar<RtToken> &tokens(const RtToken token) const
	{
		TypeConstTokenIterator iter = m_tokens.find(token);
		if ( iter == m_tokens.end() )
			throw;
		return (*iter).second;
	}

	inline TemplPrimVar<RtFloat> &floats(const RtToken token) { return const_cast<TemplPrimVar<RtFloat> &>(((const CFace *)this)->floats(token)); }
	inline TemplPrimVar<RtInt> &ints(const RtToken token) { return const_cast<TemplPrimVar<RtInt> &>(((const CFace *)this)->ints(token)); }
	inline TemplPrimVar<RtToken> &tokens(const RtToken token) { return const_cast<TemplPrimVar<RtToken> &>(((const CFace *)this)->tokens(token)); }
	
	inline TypeConstFloatIterator floatBegin() const { return m_floats.begin(); }
	inline TypeConstFloatIterator floatEnd() const { return m_floats.end(); }
	inline TypeConstIntIterator intBegin() const { return m_ints.begin(); }
	inline TypeConstIntIterator intEnd() const { return m_ints.end(); }
	inline TypeConstTokenIterator tokenBegin() const { return m_tokens.begin(); }
	inline TypeConstTokenIterator TokenEnd() const { return m_tokens.end(); }
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

public:
	inline CFace &newFace() { CFace aFace; m_faces.push_back(aFace); return m_faces.back(); }
	
	inline const_iterator begin() const { return m_faces.begin(); }
	inline const_iterator end() const { return m_faces.end(); }

	inline iterator begin() { return m_faces.begin(); }
	inline iterator end() { return m_faces.end(); }
}; // CSurface

}

#endif // _RICPP_RICONTEXT_SURFACE_H
