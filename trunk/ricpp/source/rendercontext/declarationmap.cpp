// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file declarationmap.cpp
 *  \brief Implementation of TDeclarationMap to store contents of RiDeclare calls
 *         (Maps the declaration name to the declaration) and the auxillary
 *         class TRenderException, a exception thrown by TDeclarationMap::declare() if
 *         a declaration could not get parsed
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "declarationmap.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TDeclarationMap - holds a map of all called parameter declarations
//                   (TParameterDeclaration) indexed by name


////////////////////////////////////////////////////////////////////////////////
// releaseAll - frees all memory used by the map
//
void TDeclarationMap::releaseAll() {
	m_declarations.clear();
	m_stringContainer.clear();
	m_stringMap.clear();
}


////////////////////////////////////////////////////////////////////////////////
// releaseDeclarations - clear all declarations
//
void TDeclarationMap::releaseDeclarations() {
	m_declarations.clear();
}


////////////////////////////////////////////////////////////////////////////////
// releaseNonDefaultDeclarations - clear all declarations
//
void TDeclarationMap::releaseNonDefaultDeclarations() {
	STRING2DECL::iterator i = m_declarations.begin();
	while ( i != m_declarations.end() ) {
			if ( !(*i).second.isDefault() ) {
				m_declarations.erase(i);
				i = m_declarations.begin();
			} else {
				++i;
			}
	}
}


////////////////////////////////////////////////////////////////////////////////
// declare - adds a declarition of a render context to the map, returns the
//           name of the declaration
//
// Throws TRenderException
//
RtToken TDeclarationMap::declare(const char *name, const char *declaration, bool isDef) {
	TParameterDeclaration var;
	if ( !var.parse(name, declaration, isDef) ) {
		throw TRenderException(RIE_SYNTAX, isDef ? RIE_SEVERE : RIE_ERROR, "RiDeclare - couldn´t parse declare \"%s\" \"%s\"", name ? name : "", declaration ? declaration : "");
		return TRi::RI_NULL;
	}

	// Find declaration or insert into list
	RtToken currName = var.getName();
	STRING2TOKEN::iterator iter = m_stringMap.find(currName);
	if ( iter == m_stringMap.end() ) {
		// New token, create a fixed pointer to a c_str
		if ( isDef ) {
			// use pointer as fix address
			currName = name;
		} else {
			// new string as fix address
			m_stringContainer.push_back(currName);
			const std::string &strName = m_stringContainer.back();
			currName = strName.c_str();
		}
		m_stringMap[currName] = currName;
	} else {
		currName = (*iter).second;
	}

	STRING2DECL::iterator i = m_declarations.find(currName);
	if ( i != m_declarations.end() ) {
		// If one should not redeclare standard declarations, insert the following lines
		// if ( i->second.isDefault() ) {
		//	throw TRenderException(RIE_SYNTAX, isDef ? RIE_SEVERE : RIE_ERROR, "RiDeclare - declared default twice %s %s", name ? name : "", declaration ? declaration : "");
		//	return TRi::RI_NULL;
		// }

		// delete first declaration
		m_declarations.erase(i);
	}

	// insert declaration
	m_declarations[currName] = var;
	return currName;
}


////////////////////////////////////////////////////////////////////////////////
// findDeclaration - finds the parameterdeclaration for a given name
//                   (case sensitive)
//
const TParameterDeclaration *TDeclarationMap::findDeclaration(const char *name) {
	if ( !name ) return NULL;

	STRING2TOKEN::iterator iter = m_stringMap.find(name);
	if ( iter != m_stringMap.end() ) {
		name = (*iter).second;
		STRING2DECL::iterator i = m_declarations.find(name);
		return i !=  m_declarations.end() ?
			&(i->second) :
			NULL;
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
// getToken - returns the token for a declaration
//
RtToken TDeclarationMap::getToken(const char *name) {
	const TParameterDeclaration *pd = findDeclaration(name);
	return pd ? pd->getName() : TRi::RI_NULL;
}


////////////////////////////////////////////////////////////////////////////////
// push - pushes all declarations
//
void TDeclarationMap::push() {
	m_declarationStack.push(m_declarations);
}


////////////////////////////////////////////////////////////////////////////////
// pop - pops declarations
//
void TDeclarationMap::pop() {
	m_declarations.clear();
	m_declarations = m_declarationStack.top();
	m_declarationStack.pop();
}

////////////////////////////////////////////////////////////////////////////////
// copyDecls - copies all declarations (not the declatration stack)
//                The tokens are not changed therefore declmap don't have to be
//                destroyed while the instance the declarations are copied to
//                stays alive. Thats no problem here, since the member function
//                is only used to copy the default declaration to a new context.
//
void TDeclarationMap::copyDecls(const TDeclarationMap &declmap) {
	if ( &declmap == this )
		return;

	// Copy the declarations
	m_declarations = declmap.m_declarations;

	// Copy the string to (const char *) map
	STRING2TOKEN::const_iterator iter = declmap.m_stringMap.begin();
	STRING2TOKEN::iterator finditer;
	for ( ; iter != declmap.m_stringMap.end(); ++iter ) {
		finditer = m_stringMap.find((*iter).first);
		if ( finditer == m_stringMap.end() ) {
			m_stringMap[(*iter).first] = (*iter).second;
		}
	}

	m_stringMap = declmap.m_stringMap;
}
