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
#include "ricpp/ricpp/renderererror.h"

#include <assert.h>

using namespace RiCPP;

CDeclarationDictionary::~CDeclarationDictionary()
{
	std::list<const CDeclaration *>::const_iterator i;
	for ( i = m_all.begin(); i != m_all.end(); i++ ) {
		delete (*i);
	}
}

const CDeclaration *CDeclarationDictionary::find(RtToken tableNamespace, const char *table, const char *var, const CTokenizer &tokenizer) const
{
	if ( emptyStr(var) )
		return 0;

	RtToken token;
	std::string s;

	if ( notEmptyStr(tableNamespace) ) {
		s = tableNamespace;
		s += ':';
		if ( notEmptyStr(tableNamespace) ) {
			s += table;
			s += ':';
		}
		s += var;

		token = tokenizer.find(s.c_str());
		if ( token != RI_NULL ) {
			return find(token);
		}
	}

	if ( notEmptyStr(table) ) {
		s = table;
		s += ':';
		s += nonullstr(var);
		token = tokenizer.find(s.c_str());
		if ( token != RI_NULL ) {
			return find(token);
		}
	}

	s = var;
	token = tokenizer.find(s.c_str());
	if ( token != RI_NULL ) {
		return find(token);
	}

	return 0;
}

const CDeclaration *CDeclarationDictionary::findAndUpdate(RtToken token, unsigned int curColorSize)
{
	const CDeclaration *decl = find(token);
	if ( !decl )
		return decl;

	if ( decl->type() != TYPE_COLOR )
		return decl;

	if ( decl->typeSize() == curColorSize )
		return decl;

	CDeclaration *newDecl = 0;
	try {
		newDecl = new CDeclaration(*decl, curColorSize);
	} catch ( ... ) {
		newDecl = 0;
	}
	
	if ( !newDecl )
		throw ERendererError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Declaration of %s", token);

	m_all.push_back(newDecl);
	m_active.unRegisterObj(decl->token());
	m_active.registerObj(newDecl->token(), newDecl);
	return newDecl;
}

const CDeclaration *CDeclarationDictionary::findAndUpdate(
	RtToken tableNamespace,
	const char *table,
	const char *var,
	const CTokenizer &tokenizer,
	unsigned int curColorSize
)
{
	if ( emptyStr(var) )
		return 0;

	RtToken token;
	std::string s;

	if ( notEmptyStr(tableNamespace) ) {
		s = tableNamespace;
		s += ':';
		if ( notEmptyStr(tableNamespace) ) {
			s += table;
			s += ':';
		}
		s += var;

		token = tokenizer.find(s.c_str());
		if ( token != RI_NULL ) {
			return findAndUpdate(token, curColorSize);
		}
	}

	if ( notEmptyStr(table) ) {
		s = table;
		s += ':';
		s += nonullstr(var);
		token = tokenizer.find(s.c_str());
		if ( token != RI_NULL ) {
			return findAndUpdate(token, curColorSize);
		}
	}

	s = var;
	token = tokenizer.find(s.c_str());
	if ( token != RI_NULL ) {
		return findAndUpdate(token, curColorSize);
	}

	return 0;
}

void CDeclarationDictionary::add(CDeclaration *decl)
{
	if ( !decl )
		return;

	m_all.push_back(decl);
	const CDeclaration *found = m_active.findObj(decl->token());
	if ( found ) {
		m_active.unRegisterObj(found->token());
	}
	
	bool flag = m_active.registerObj(decl->token(), decl);
	flag = flag;
	assert(flag);
}
