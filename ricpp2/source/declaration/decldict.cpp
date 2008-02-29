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

#ifndef _RICPP_RIBASE_RICPPERROR_H
#include "ricpp/ribase/ricpperror.h"
#endif // _RICPP_RIBASE_RICPPERROR_H

#include <cassert>

using namespace RiCPP;

CDeclarationDictionary::~CDeclarationDictionary()
{
	std::list<const CDeclaration *>::const_iterator i;
	for ( i = m_all.begin(); i != m_all.end(); i++ ) {
		delete (*i);
	}
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
