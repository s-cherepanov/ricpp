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

/*! \file optionmap.cpp
 *  \brief Implements TOptionMap, maps the option name to an option value (RiOption calls).
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "optionmap.h"

////////////////////////////////////////////////////////////////////////////////
TOptionMap::TOptionMap(const TOptionMap &opts) {
	*this = opts;
}

////////////////////////////////////////////////////////////////////////////////
TOptionMap::~TOptionMap() {
	releaseAll();
}

////////////////////////////////////////////////////////////////////////////////
TOptionMap &TOptionMap::duplicate() const {
	TOptionMap *o = new TOptionMap(*this);
	if ( !o )
		throw TSystemException(__FILE__, __LINE__);
	return *o;
}

////////////////////////////////////////////////////////////////////////////////
void TOptionMap::releaseAll() {
	::release_second(m_parammaps);
}

////////////////////////////////////////////////////////////////////////////////
// operator= - assigns opts to *this 
//
// Throws TSystemException (via TParameterMap::duplicate())
//
TOptionMap &TOptionMap::operator=(const TOptionMap &opts) {
	if ( this == &opts )
		return *this;

	releaseAll();

	STRING2PARAMMAP::const_iterator i;
	for ( i = opts.m_parammaps.begin(); i != opts.m_parammaps.end(); i++ ) {
		TParameterMap *p = &((*i).second->duplicate()); // can throw TSystemException;
		m_parammaps[p->getName()] = p;
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
TParameterMap *TOptionMap::getOption(const char *name) const {
	STRING2PARAMMAP::const_iterator i = m_parammaps.find(name);
	if ( i != m_parammaps.end() ) {
		TParameterMap *p = (*i).second;
		return p;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
const RtPointer TOptionMap::getValueOf(const char *name, const char *param) const {
	TParameterMap *p = getOption(name);
	return p
		? p->getValueOf(param)
		: (const RtPointer)TRi::RI_NULL;
}

////////////////////////////////////////////////////////////////////////////////
bool TOptionMap::setOption(TDeclarationMap &decls, RtInt colorComps, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	bool rVal = true;
	TParameterMap *p = getOption(name);
	if ( p ) {
		rVal = p->add(decls, 0, 0, 0, 0, 0, colorComps, n, tokens, params);
	} else {
		p = new TParameterMap();
		if ( !p )
			throw TSystemException(__FILE__, __LINE__);
		p->setName(name);
		rVal = p->add(decls, 0, 0, 0, 0, 0, colorComps, n, tokens, params);		
		m_parammaps[p->getName()] = p;
	}
	return rVal;
}
