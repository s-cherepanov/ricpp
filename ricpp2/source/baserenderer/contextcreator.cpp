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

#include "baserenderer/contextcreator.h"
#include <algorithm>

using namespace RiCPP;

CContextCreator::CContextCreator() {
	m_curContext = 0;
}

CContextCreator::~CContextCreator() {
	m_curContext = 0;
	// Delete all contexts in m_contextList
}

IRiContext *CContextCreator::getNewContext() {
	// Has to be overloaded for a concrete context creator
	return 0;
}

void CContextCreator::deleteContext() {
	// remove m_curContext from list
	if ( m_curContext ) {
		// m_curContext is in list everytime
		m_contextList.remove(m_curContext);
		delete m_curContext;
	}
	m_curContext = 0;
}

IRiContext *CContextCreator::getContext() {
	return m_curContext;
}

RtVoid CContextCreator::context(IRiContext *context) {
	if ( m_curContext )
		m_curContext->deactivate();

	if ( !context ) {
		m_curContext = 0;
		return;
	}

	// find Context, set as current, activate() and return
	if ( find(m_contextList.begin(), m_contextList.end(), context) != m_contextList.end() ) {
		m_curContext = context;
		m_curContext->activate();
		return;
	}
	// if not found clear current, throw error
	m_curContext = 0;
	errHandler().handleErrorV(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::context(), context handle not generated from context creator");
}

RtVoid CContextCreator::begin(RtString name) {
	m_curContext = getNewContext();
	if ( !m_curContext ) {
		errHandler().handleErrorV(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::begin(), could not get a new context handle");
	}

	m_curContext->begin(name);
}

RtVoid CContextCreator::abort(void) {
	if ( m_curContext )
		m_curContext->abort();

	deleteContext();
}

RtVoid CContextCreator::end(void) {
	if ( m_curContext )
		m_curContext->end();

	deleteContext();
}

unsigned int CContextCreator::majorVersion(void) {
	return IRiContext::majorVersion;
}
