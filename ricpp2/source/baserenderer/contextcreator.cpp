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
	throw ERendererError(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::context(), context handle not generated from context creator");
}

RtVoid CContextCreator::begin(RtString name) {
	m_curContext = getNewContext();
	if ( !m_curContext ) {
		throw ERendererError(RIE_BADHANDLE, RIE_ERROR, "CContextCreator::begin(), could not get a new context handle");
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

unsigned int CContextCreator::majorVersion() {
	return IRiContext::majorVersion;
}

unsigned int CContextCreator::minorVersion() {
	return IRiContext::minorVersion;
}
