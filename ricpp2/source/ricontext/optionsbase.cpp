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

/** @file optionsbase.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the base class for options and attributes used for the render state.
 */
#include "ricpp/ricontext/optionsbase.h"

using namespace RiCPP;

COptionsBase &COptionsBase::operator=(const COptionsBase &ga)
{
	if ( this == &ga )
		return *this;
	
	clearMembers();
	
	m_curColorDesc = ga.m_curColorDesc;
	
	for (
		const_iterator i = ga.begin();
		i != ga.end();
		++i )
	{
		const CNamedParameterList *c = i->second;
		m_paramList.push_back(*c);
		m_paramMap[i->first] = &m_paramList.back();
	}

	return *this;
}


void COptionsBase::set(
	CDeclarationDictionary &dict,
	RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	CNamedParameterList *pl = get(name);
	if ( !pl ) {
		m_paramList.push_back(CNamedParameterList(name));
		pl = &m_paramList.back();
		m_paramMap[name] = pl;
	}

	CValueCounts counts;
	pl->add(counts, dict, m_curColorDesc, n, tokens, params);
}


void COptionsBase::set(
	RtToken name, const CParameterList &params)
{
	CNamedParameterList *pl = get(name);
	if ( !pl ) {
		m_paramList.push_back(CNamedParameterList(name));
		pl = &m_paramList.back();
		m_paramMap[name] = pl;
	}

	pl->add(params);
}


CNamedParameterList *COptionsBase::get(RtToken name)
{
	Map_type::iterator i = m_paramMap.find(name);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}


const CNamedParameterList *COptionsBase::get(RtToken name) const
{
	Map_type::const_iterator i = m_paramMap.find(name);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}


const CParameter *COptionsBase::get(RtToken name, RtToken token) const
{
	const CNamedParameterList *pl = get(name);
	if ( !pl )
		return 0;

	return pl->get(token);
}


bool COptionsBase::erase(RtToken name)
{
	if ( !name )
		return false;

	const CNamedParameterList *paramList = get(name);
	if ( !paramList )
		return false;

	std::list<CNamedParameterList>::iterator i;
	for ( i = m_paramList.begin(); i != m_paramList.end(); ++i ) {
		if ( paramList == &(*i) ) {
			m_paramMap.erase(name);
			m_paramList.erase(i);
			return true;
		}
	}

	return false;
}


bool COptionsBase::erase(CNamedParameterList *paramList)
{
	if ( !paramList )
		return false;

	return erase(paramList->name());
}

