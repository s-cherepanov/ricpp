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

/** @file decldict.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the dictionary for declarations
 */

#include "ricpp/declaration/decldict.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#include <cassert>

using namespace RiCPP;

CDeclarationDictionary::CDeclarationDictionary()
: m_active(false)
{
	m_tokenMap.defaultTokens();
}


void CDeclarationDictionary::clear()
{
	m_declOldNewRemap.clear();
	m_active.clear();
	std::list<const CDeclaration *>::const_iterator i;
	for ( i = m_all.begin(); i != m_all.end(); i++ ) {
		delete (*i);
	}
}

CDeclarationDictionary::~CDeclarationDictionary()
{
	clear();
}

const CDeclaration *CDeclarationDictionary::find(
	const char *aQualifier,
	const char *table,
	const char *var,
	const CColorDescr &curColorDescr,
	bool update
)
{
	// A least there has to be a variable name
	if ( emptyStr(var) )
		return 0;

	// Test inline declaration
	try {
		CDeclaration d(var, curColorDescr, m_tokenMap);
		if ( d.isInline() ) {
			return 0;
		}
	} catch ( ExceptRiCPPError & ) {
	    // ok, not an inline declaration
	}
		
	const CDeclaration *d = 0;
	RtToken token = RI_NULL;
	std::string s;

	if ( !strrchr(var, ':') ) {
		// Variable contains no ':' - test scope
		
		if ( notEmptyStr(aQualifier) && notEmptyStr(table) ) {
			// Qualifier and tablename (local var)
			s = aQualifier;
			s += ':';
			s += table;
			s += ':';
			s += var;

			token = m_tokenMap.find(s.c_str());
			if ( token != RI_NULL ) {
				d = update ? findAndUpdate(token, curColorDescr) : find(token);
				if ( d ) return d;
			}
		}
		
		if ( notEmptyStr(table) ) {
			// Search for Tablename only
			s = table;
			s += ':';
			s += var;

			token = m_tokenMap.find(s.c_str());
			if ( token != RI_NULL ) {
				d = update ? findAndUpdate(token, curColorDescr) : find(token);
				if ( d ) return d;
			}
		}
		
		if ( notEmptyStr(aQualifier) ) {
			// Search for qualifier only
			s = aQualifier;
			s += ':';
			s += var;

			token = m_tokenMap.find(s.c_str());
			if ( token != RI_NULL ) {
				d = update ? findAndUpdate(token, curColorDescr) : find(token);
				if ( d ) return d;
			}
		}
	}

	// Variable contains a ':' (parameter passed to the interface with attached qualifier prefix)
	// or qualifier, table are not set,
	// or declaration not found previously 
	// Search for name only (globally declared (old style) variable)
	token = m_tokenMap.find(var);
	if ( token != RI_NULL ) {
		d = update ? findAndUpdate(token, curColorDescr) : find(token);
		if ( d ) return d;
	}

	return 0;
}


const CDeclaration *CDeclarationDictionary::find(const char *aQualifier, const char *table, const char *var)
{
	CColorDescr descr;
	return find(aQualifier, table, var, descr, false);
}


const CDeclaration *CDeclarationDictionary::findAndUpdate(
	RtToken token, const CColorDescr &curColorDescr)
{
	const CDeclaration *decl = find(token);
	if ( !decl )
		return decl;

	if ( decl->type() != TYPE_COLOR )
		return decl;

	if ( decl->colorDescr() == curColorDescr )
		return decl;

	CDeclaration *newDecl = 0;
	try {
		newDecl = new CDeclaration(*decl, curColorDescr);
	} catch ( ... ) {
		newDecl = 0;
	}
	
	if ( !newDecl )
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Declaration of %s", token);

	m_all.push_back(newDecl);
	m_active.unregisterObj(decl->token());
	m_active.registerObj(newDecl->token(), newDecl);
	return newDecl;
}

const CDeclaration *CDeclarationDictionary::findAndUpdate(
	const char *aQualifier,
	const char *table,
	const char *var,
	const CColorDescr &curColorDescr
)
{
	return find(aQualifier, table, var, curColorDescr, true);
}

void CDeclarationDictionary::add(CDeclaration *decl)
{
	if ( !decl )
		return;

	m_all.push_back(decl);
	const CDeclaration *found = m_active.findObj(decl->token());
	if ( found ) {
		m_active.unregisterObj(found->token());
	}
	
	bool flag = m_active.registerObj(decl->token(), decl);
	flag = flag;
	assert(flag);
}


RtToken CDeclarationDictionary::declare(RtToken name, RtString declaration, bool isDefault, const CColorDescr &curColorDescr)
{
	if ( !emptyStr(name) && !emptyStr(declaration) ) {
		name = m_tokenMap.findCreate(name);
		
		CDeclaration *d = new CDeclaration(name, declaration, curColorDescr, m_tokenMap, isDefault);		
		if ( !d )
			throw ExceptRiCPPError(
								   RIE_NOMEM,
								   RIE_SEVERE,
								   __LINE__,
								   __FILE__,
								   "Declaration of \"%s\": \"%s\"",
								   noNullStr(name),
								   noNullStr(declaration));
		
		add(d);
		return d->token();
	}
	
	return RI_NULL;
}

CDeclarationDictionary &CDeclarationDictionary::assignRemap(const CDeclarationDictionary &declDict)
{
	if ( this == &declDict )
		return *this;

	clear();
	std::list<const CDeclaration *>::const_iterator i;
	
	for ( i = declDict.m_all.begin(); i != declDict.m_all.end(); i++ ) {
		if ( *i == 0 )
			continue;
		CDeclaration *d = new CDeclaration(**i, m_tokenMap);
		m_declOldNewRemap[*i] = d;
		m_all.push_back(*i);
	}
	
	const_iterator activei = declDict.m_active.begin();
	for ( ; activei != declDict.m_active.end(); ++activei ) {
		CDeclaration *decl = remapDecl((*activei).second);
		if ( decl ) {
			m_active.registerObj(decl->token(), decl);
		}
	}
	
	return *this;
}


CDeclaration *CDeclarationDictionary::remapDecl(const CDeclaration *oldDecl)
{
	if ( !oldDecl )
		return 0;
	
	std::map<const CDeclaration *, CDeclaration *>::iterator i;

	if ( (i = m_declOldNewRemap.find(oldDecl)) != m_declOldNewRemap.end() ) {
		return (*i).second;
	}
	
	return 0;
}
