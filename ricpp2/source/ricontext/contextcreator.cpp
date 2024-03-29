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

/** @file contextcreator.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a class for creation and basic handling of render contexts.
 */

#include "ricpp/ricontext/contextcreator.h"

using namespace RiCPP;

const char *CContextCreator::myName() { return "contextcreator"; }
const char *CContextCreator::myType() { return "contextcreator"; }
unsigned long CContextCreator::myMajorVersion() { return 1; }
unsigned long CContextCreator::myMinorVersion() { return 1; }
unsigned long CContextCreator::myRevision() { return 1; }

const char *CContextCreator::name() const { return myName(); }
const char *CContextCreator::type() const { return myType(); }
unsigned long CContextCreator::majorVersion() const {return myMajorVersion(); }
unsigned long CContextCreator::minorVersion() const {return myMinorVersion(); }
unsigned long CContextCreator::revision() const {return myRevision(); }

IRiCPPErrorHandler &CContextCreator::ricppErrHandler() { return m_errorHandler; }
IRiContext *CContextCreator::getNewContext() { return 0; }
void CContextCreator::startup() {}
void CContextCreator::shutdown() {}

CContextCreator::CContextCreator() : m_curContext(0), m_ribParserCallback(0) { }

CContextCreator::~CContextCreator()
{
	m_curContext = 0;

	// Delete all contexts in m_contextList
	std::list<IRiContext *>::iterator iter;
	for ( iter = m_contextList.begin(); iter != m_contextList.end(); iter ++ ) {
		delete *iter;
	}

	m_contextList.clear();
}


IRiContext *CContextCreator::getContext() { return m_curContext; }

void CContextCreator::deleteContext()
{
	// removes m_curContext from list
	if ( m_curContext ) {
		// m_curContext is in list everytime
		m_contextList.remove(m_curContext);
		delete m_curContext;
	}
	m_curContext = 0;
}

RtVoid CContextCreator::context(IRiContext *context)
// throw ExceptRiCPPError
{
	if ( m_curContext == context )
		return;

	// Deactivate the current context
	if ( m_curContext )
		m_curContext->deactivate();

	// There is no new current context
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
	ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::context(), context handle not generated from context creator");
}

IRiContext *CContextCreator::beginV(const CDeclarationDictionary &theDeclDict, const COptions &theOptions, const COptionsBase &theControls, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	// Deactivate the current context
	if ( m_curContext )
		m_curContext->deactivate();

	// Get a new context
	m_curContext = getNewContext();

	if ( !m_curContext ) {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::begin(), could not get a new context handle");
		return 0;
	}

	m_contextList.push_back(m_curContext);

	// Activate the context by calling its begin
	try {
		if ( m_ribParserCallback )
			m_curContext->registerRibParserCallback(*m_ribParserCallback);

		if ( !m_curContext->init(theDeclDict, theOptions, theControls)
			||
			m_curContext->beginV(name, n, tokens, params) == illContextHandle
		) {
			deleteContext();
			return 0;
		}
	} catch ( ExceptRiCPPError &e ) {
		deleteContext();
		ricppErrHandler().handleError(e);
		return 0;
	}

	return m_curContext;
}

RtVoid CContextCreator::abort(void)
{
	if ( m_curContext )
		m_curContext->abort();
}

RtVoid CContextCreator::end(void)
// throw ExceptRiCPPError
{
	ExceptRiCPPError e;

	try {
		if ( m_curContext )
			m_curContext->end();
	} catch (ExceptRiCPPError &e2) {
		e = e2;
	}

	// delete anyway
	deleteContext();

	if ( e.isError() ) {
		ricppErrHandler().handleError(e);
	}
}

unsigned long CContextCreator::majorContextVersion(void) const { return IRiContext::myMajorVersion(); }
unsigned long CContextCreator::minorContextVersion() const { return 0; }
unsigned long CContextCreator::contextRevision() const { return 0; }
RtToken CContextCreator::contextName() const { return RI_NULL; }
RtToken CContextCreator::contextType() const { return IRiContext::myType(); }
RtToken CContextCreator::rendererType() const { return RI_NULL; }

RtVoid CContextCreator::registerRibParserCallback(IRibParserCallback &cb)
{
	m_ribParserCallback = &cb;
}
