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

/** @file parameter.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the RI parameters and parameter lists.
 */
#include "ricpp/ricontext/parameter.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifdef _DEBUG
#include <iostream>
// #define _TRACE_EXTRACT
#endif


using namespace RiCPP;

// -----------------------------------------------------------------------------

void CParameter::copyStringPtr()
{
	m_stringPtrs.reserve(m_strings.size());
	for ( std::vector<std::string>::iterator i = m_strings.begin();
		  i != m_strings.end();
		  i++ )
	{
		m_stringPtrs.push_back( (*i).c_str() );
	}
}


void CParameter::clear()
{
	if ( m_declaration && m_declaration->isInline() )
		delete m_declaration;

	m_declaration = 0;
	m_position = 0;

	m_ints.clear();
	m_floats.clear();
	m_stringPtrs.clear();
	m_strings.clear();
	
	m_parameterName.clear();
}


void CParameter::assign(const CParameter &param)
{
	assert(m_declaration == 0);
	CDeclaration *decl = 0;
	if ( param.m_declaration && param.m_declaration->isInline() ) {
		decl = new CDeclaration(*param.m_declaration);
		if ( !decl )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,
								   __LINE__, __FILE__,
								   "While assigning parameter %s",
								   param.parameterName());
		m_declaration = decl;
	} else {
		m_declaration = param.m_declaration;
	}

	m_position = param.m_position;
	m_parameterName = param.m_parameterName;

	m_ints = param.m_ints;
	m_floats = param.m_floats;
	m_strings = param.m_strings;
	
	copyStringPtr();
}


CParameter &CParameter::operator=(const CParameter &p)
{
	if ( this == &p )
		return *this;
	
	clear();
	assign(p);
	return *this;
}


CParameter &CParameter::assignRemap(const CParameter &param, CDeclarationDictionary &newDict)
{
	if ( this == &param )
		return *this;

	clear();
	
	CDeclaration *decl = 0;
	if ( param.m_declaration && param.m_declaration->isInline() ) {
		decl = new CDeclaration(*param.m_declaration, newDict.tokenMap());
		if ( !decl )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,
								   __LINE__, __FILE__,
								   "While assigning parameter %s",
								   param.parameterName());
		m_declaration = decl;
	} else {
		decl = newDict.remapDecl(param.m_declaration);
		if ( !decl )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,
								   __LINE__, __FILE__,
								   "While remapping parameter %s",
								   param.parameterName());
		m_declaration = decl;
	}

	m_position = param.m_position;
	m_parameterName = param.m_parameterName;
	
	m_ints = param.m_ints;
	m_floats = param.m_floats;
	m_strings = param.m_strings;
	
	copyStringPtr();
	return *this;
}


bool CParameter::setDeclaration(
	RtToken aQualifier, RtToken aTable, 
	RtString theName,
	IndexType thePosition,
	const CParameterClasses &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &curColorDescr)
{
	clear();

	if ( emptyStr(theName) )
		return false;

	m_position = thePosition;
	m_parameterName = theName;
	
	const CDeclaration *existingDecl =
		dict.findAndUpdate(aQualifier, aTable, theName, curColorDescr);
	const CDeclaration *decl = 0;
	
	if ( existingDecl ) {
		decl = existingDecl;
	} else {
		// inline
		decl = new CDeclaration(theName, curColorDescr, dict.tokenMap());
		if ( !decl ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,
								   __LINE__, __FILE__,
								   "Parameter of %s", theName);
		} else if ( !decl->isInline() ) {
			delete decl;
			decl = 0;
			throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR,
								   __LINE__, __FILE__,
								   "Parameter of %s, no declaration or illegal inline declaration",
								   theName);
		}
	}

	m_declaration = decl;
	
	return true;
}


void CParameter::set(
	RtToken aQualifier, RtToken aTable, 
	RtString theName,
	RtPointer theData,
	IndexType thePosition,
	const CParameterClasses &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &curColorDescr)
{
	if ( !setDeclaration(aQualifier, aTable, theName, thePosition, counts, dict, curColorDescr) )
		return;

	unsigned long elems = m_declaration->selectNumberOf(counts) * m_declaration->elemSize();

	if ( elems > 0 && !theData ) {
		clear();
		throw ExceptRiCPPError(RIE_MISSINGDATA, RIE_ERROR,
							   __LINE__, __FILE__,
							   "Parameter of %s", theName);
	}

	if ( theData ) {
		switch ( m_declaration->basicType() ) {
			case BASICTYPE_INTEGER:
				m_ints.clear();
				m_ints.reserve(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_ints.push_back(((RtInt *)theData)[cnt]);
				}
				break;
			case BASICTYPE_FLOAT:
				m_floats.clear();
				m_floats.reserve(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_floats.push_back(((RtFloat *)theData)[cnt]);
				}
				break;
			case BASICTYPE_STRING:
				m_strings.clear();
				m_strings.reserve(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_strings.push_back(((const char **)theData)[cnt] ? ((const char **)theData)[cnt] : "");
				}
				copyStringPtr();
				break;
			default:
				throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR,
									   __LINE__, __FILE__,
									   "Parameter of %s, no type info found",
									   theName);
				break;
		}
	}
}


const CDeclaration &CParameter::declaration() const
{
	if ( !m_declaration ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE,
							   __LINE__, __FILE__,
							   "Declaration of parameter %s not defined.",
							   parameterName());
	}
	return *m_declaration;
}


RtPointer CParameter::valptr()
{
	switch ( basicType() ) {
		case BASICTYPE_INTEGER:
			if ( m_ints.empty() )
				return 0;
			return static_cast<RtPointer>(&m_ints[0]);
		case BASICTYPE_FLOAT:
			if ( m_floats.empty() )
				return 0;
			return static_cast<RtPointer>(&m_floats[0]);
		case BASICTYPE_STRING:
			if ( m_stringPtrs.empty() )
				return 0;
			return static_cast<RtPointer>(&m_stringPtrs[0]);
		default:
			return 0;
	}
}

IndexType CParameter::vectorSize() const
{
	switch ( basicType() ) {
		case BASICTYPE_INTEGER:
			return static_cast<unsigned long >(m_ints.size());
		case BASICTYPE_FLOAT:
			return static_cast<unsigned long >(m_floats.size());
		case BASICTYPE_STRING:
			return static_cast<unsigned long >(m_stringPtrs.size());
		default:
			return 0;
	}
}

bool CParameter::get(unsigned long pos, CValue &result) const
{
	switch ( basicType() ) {
		case BASICTYPE_INTEGER:
			if ( pos >= m_ints.size() ) {
				result.clear();
				return false;
			}
			result.set(m_ints[pos]);
			break;
		case BASICTYPE_FLOAT:
			if ( pos >= m_floats.size() ) {
				result.clear();
				return false;
			}
			result.set(m_floats[pos]);
			break;
		case BASICTYPE_STRING:
			if ( pos >= m_stringPtrs.size() ) {
				result.clear();
				return false;
			}
			result.set(m_stringPtrs[pos]);
			break;
		default:
			result.clear();
			return false;
	}
	return true;
}

bool CParameter::get(unsigned long pos, RtInt &result) const
{
	if ( basicType() != BASICTYPE_INTEGER || pos >= m_ints.size() )
		return false;
	result = m_ints[pos];
	return true;
}

bool CParameter::get(unsigned long pos, RtFloat &result) const
{
	if ( basicType() != BASICTYPE_FLOAT || pos >= m_floats.size() )
		return false;
	result = m_floats[pos];
	return true;
}

bool CParameter::get(unsigned long pos, std::string &result) const
{
	if ( basicType() != BASICTYPE_STRING || pos >= m_strings.size() )
		return false;
	result = m_strings[pos];
	return true;
}

bool CParameter::get(unsigned long pos, RtString &result) const
{
	if ( basicType() != BASICTYPE_STRING || pos >= m_stringPtrs.size() )
		return false;
	result = m_stringPtrs[pos];
	return true;
}

void CParameter::extract(IndexType pos, std::vector<RtInt>::iterator &result) const
{
	unsigned long es  = declaration().elemSize();
	pos = es * pos;
	unsigned long endPos  = pos+es;
	for ( ;  pos < endPos; ++pos ) {
		*result = m_ints[pos];
		result++;
	}
}

void CParameter::extract(IndexType from, IndexType to, std::vector<RtInt>::iterator &result) const
{
	long inc = 1;
	if ( to < from )
		inc = -1;
	for ( unsigned long pos = from; pos < to; pos += inc ) {
		extract(pos, result);
	}
}

void CParameter::extract(IndexType pos, std::vector<RtFloat>::iterator &result) const
{
#ifdef _TRACE_EXTRACT
	std::cout << "-> extract" << std::endl;
#endif _TRACE_EXTRACT

	unsigned long es  = declaration().elemSize();
	pos = es * pos;
	unsigned long endPos  = pos+es;
	for ( ;  pos < endPos; ++pos ) {
#ifdef _TRACE_EXTRACT
		std::cout << "-- extract pos = " << pos << std::endl;
#endif _TRACE_EXTRACT
		*result = m_floats[pos];
		result++;
	}

#ifdef _TRACE_EXTRACT
	std::cout << "<- extract" << std::endl;
#endif _TRACE_EXTRACT
}

void CParameter::extract(IndexType from, IndexType to, std::vector<RtFloat>::iterator &result) const
{
	long inc = 1;
	if ( to < from )
		inc = -1;
	for ( unsigned long pos = from; pos < to; pos += inc ) {
		extract(pos, result);
	}
}

void CParameter::extract(IndexType pos, std::vector<std::string>::iterator &result) const
{
	unsigned long es  = declaration().elemSize();
	pos = es * pos;
	unsigned long endPos  = pos+es;
	for ( ;  pos < endPos; ++pos ) {
		*result = m_strings[pos];
		result++;
	}
}

void CParameter::extract(IndexType from, IndexType to, std::vector<std::string>::iterator &result) const
{
	long inc = 1;
	if ( to < from )
		inc = -1;
	for ( unsigned long pos = from; pos < to; pos += inc ) {
		extract(pos, result);
	}
}

void CParameter::extract(IndexType pos, std::vector<RtString>::iterator &result) const
{
	unsigned long es  = declaration().elemSize();
	pos = es * pos;
	unsigned long endPos  = pos+es;
	for ( ;  pos < endPos; ++pos ) {
		*result = m_stringPtrs[pos];
		result++;
	}
}

void CParameter::extract(IndexType from, IndexType to, std::vector<RtString>::iterator &result) const
{
	long inc = 1;
	if ( to < from )
		inc = -1;
	for ( unsigned long pos = from; pos < to; pos += inc ) {
		extract(pos, result);
	}
}

void CParameter::bilinearBlend(const IndexType (& cornerIdx)[4],
							   IndexType tessU,
							   IndexType tessV,
							   std::vector<RtFloat> &retvals) const
{
	retvals.clear();

	assert ( basicType() == BASICTYPE_FLOAT );
	if ( basicType() != BASICTYPE_FLOAT )
		return;
	
	assert ( tessU > 0 && tessV > 0 );
	if ( tessU <= 0 || tessV <= 0 )
		return;
	
	IndexType elemSize = declaration().elemSize();
	assert( elemSize > 0 );
	if ( elemSize <= 0 )
		return;
	
	const std::vector<RtFloat> &vals = floats();
	IndexType sz = static_cast<IndexType>(vals.size());
	assert ( sz > cornerIdx[0]*elemSize && sz > cornerIdx[1]*elemSize && sz > cornerIdx[2]*elemSize && sz > cornerIdx[3]*elemSize );
	if ( !(sz > cornerIdx[0]*elemSize && sz > cornerIdx[1]*elemSize && sz > cornerIdx[2]*elemSize && sz > cornerIdx[3]*elemSize) )
		return;
	
	retvals.resize((tessU+1)*(tessV+1)*elemSize);
	
	RtFloat deltau = (RtFloat)(1.0/(RtFloat)(tessU));
	RtFloat deltav = (RtFloat)(1.0/(RtFloat)(tessV));
	
	RtFloat u, v;
	IndexType ui, vi, ei, idx;
	IndexType startPos, endPos;
	
	for ( v = (RtFloat)0.0, vi = 0; vi < tessV+1; ++vi, v += deltav ) {
		if ( v > 1.0 || vi == tessV ) {
			v = 1.0;
		}
		startPos = vi * (tessU + 1) * elemSize;
		endPos   = startPos + tessU * elemSize;
		assert(startPos != endPos);
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
}

// -----------------------------------------------------------------------------


CParameterList::CParameterList(
	const char *aQualifier, const char *aTable, 
	const CParameterClasses &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &curColorDescr,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	set(aQualifier, aTable, counts, dict, curColorDescr, n, tokens, params);
}


void CParameterList::rebuild()
{
	m_tokenPtr.resize(0);
	m_paramPtr.resize(0);

	for (
		std::list<CParameter>::iterator i = m_params.begin();
		i != m_params.end();
		++i )
	{
		CParameter &p = (*i);
		const CDeclaration *d = p.declarationPtr();
		if ( d ) {
			if ( d->isInline() ) {
				m_tokenPtr.push_back(d->qualifiedName());
			} else {
				m_tokenPtr.push_back(d->token());
			}
			m_paramPtr.push_back(p.valptr());
		}
	}
}


void CParameterList::reset(const CParameterClasses &counts)
{
	m_valueCounts = counts;
	m_params.resize(0);
	m_paramMap.clear();
	m_tokenPtr.resize(0);
	m_paramPtr.resize(0);
}


void CParameterList::assign(const CParameterList &params)
{
	reset(params.m_valueCounts);
	add(params);
}


CParameterList &CParameterList::operator=(const CParameterList &params)
{
	if ( this == &params )
		return *this;
	assign(params);
	return *this;
}


void CParameterList::set(
	const char *aQualifier, const char *aTable, 
	const CParameterClasses &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &curColorDescr,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	reset(counts);
	add(aQualifier, aTable, dict, curColorDescr, n, tokens, params);
}


void CParameterList::add(
	const char *aQualifier, const char *aTable, 
	CDeclarationDictionary &dict,
	const CColorDescr &curColorDescr,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	for ( RtInt i = 0; i < n; ++i ) {
		CParameter *param = get(tokens[i]);
		if ( param ) {
			erase(param);
		}
		try {
			m_params.push_back(CParameter(aQualifier, aTable, tokens[i], params[i], i, m_valueCounts, dict, curColorDescr));
			RtToken var = m_params.back().var(); // The unqualified variable name as key
			assert(var);
			if ( var )
				m_paramMap[var] = &m_params.back();
			RtToken token = m_params.back().token(); // The qualified variable name as key
			if ( token != var )
				m_paramMap[token] = &m_params.back();
		} catch(ExceptRiCPPError &) {
			// Consume Error, ignore illegal parameters
		}
	}
	rebuild();
}


void CParameterList::add(const CParameterList &params)
{
	if ( this == &params )
		return;
	
	assert(m_valueCounts == params.m_valueCounts);

	for (
		const_iterator i = params.begin();
		i != params.end();
		++i )
	{
		if ( i->token() != RI_NULL ) {
			m_params.push_back(*i);
			RtToken var = m_params.back().var(); // The unqualified variable name as key
			assert(var);
			if ( var )
				m_paramMap[var] = &m_params.back();
			RtToken token = m_params.back().token(); // The qualified variable name as key
			if ( token != var )
				m_paramMap[token] = &m_params.back();
		}
	}

	rebuild();
}


CParameter *CParameterList::get(RtToken var)
{
	Map_type::iterator i = m_paramMap.find(var);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}


const CParameter *CParameterList::get(RtToken var) const
{
	Map_type::const_iterator i = m_paramMap.find(var);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}


bool CParameterList::erase(RtToken var)
{
	const CParameter *param = get(var);
	if ( !param )
		return false;

	std::list<CParameter>::iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( param == &(*i) ) {
			m_paramMap.erase(param->token());
			m_params.erase(i);
			rebuild();
			return true;
		}
	}

	return false;
}


bool CParameterList::erase(CParameter *param)
{
	if (!param )
		return false;

	std::list<CParameter>::iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( param == &(*i) ) {
			m_paramMap.erase(param->token());
			m_params.erase(i);
			rebuild();
			return true;
		}
	}

	return false;
}


bool CParameterList::hasColor() const
{
	std::list<CParameter>::const_iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( (*i).type() == TYPE_COLOR ) {
			return true;
		}
	}
	return false;
}

CParameterList &CParameterList::assignRemap(const CParameterList &params, CDeclarationDictionary &newDict)
{
	if ( this == &params )
		return *this;
	
	reset(params.valueCounts());
	
	for (
		 const_iterator i = params.begin();
		 i != params.end();
		 ++i )
	{
		if ( i->token() != RI_NULL ) {
			m_params.push_back(CParameter(*i, newDict));
			RtToken var = m_params.back().var(); // The unqualified variable name as key
			assert(var);
			if ( var )
				m_paramMap[var] = &m_params.back();
			RtToken token = m_params.back().token(); // The qualified variable name as key
			if ( token != var )
				m_paramMap[token] = &m_params.back();
		}
	}
	
	rebuild();
	
	return *this;	
}




// ----------------------------------------------------------------------------


CNamedParameterList::CNamedParameterList(const CNamedParameterList &params, CDeclarationDictionary &newDict)
: CParameterList(params, newDict)
{
	name(newDict.tokenMap().findCreate(params.name()));
}

void CNamedParameterList::assign(const CNamedParameterList &params)
{
	name(params.name());
}

CNamedParameterList &CNamedParameterList::operator=(const CNamedParameterList &params)
{
	if ( this == &params )
		return *this;

	assign(params);

	CParameterList::operator=(params);
	return *this;
}


CNamedParameterList &CNamedParameterList::assignRemap(const CNamedParameterList &params, CDeclarationDictionary &newDict)
{
	if ( this == &params )
		return *this;
	
	name(newDict.tokenMap().findCreate(params.name()));
	
	CParameterList::assignRemap(params, newDict);
	return *this;	
}
