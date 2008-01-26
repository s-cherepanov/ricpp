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

/** @file ricppbridge.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bridges the interface functions to a current renderer
 */

#include "ricpp/ricppbridge/ricppbridge.h"
// #include "ricpp/ribwriter/ribwriter.h"
#include "ricpp/tools/filepath.h"

using namespace RiCPP;

inline CRiCPPBridge::CContextManagement::CContextManagement()
{
	m_nextCtxHandle = 1;
	// illContextHandle is always the first context (outside begin-end)
	// m_curCtx initially has nor context creator or rendering context.
	m_ctxHandle = illContextHandle;
	m_ctxMap[m_ctxHandle] = m_curCtx;
	m_outer = 0;
}

RtContextHandle CRiCPPBridge::CContextManagement::addContext(const CContext &ctx)
{
	m_ctxMap[m_nextCtxHandle] = ctx;
	return m_nextCtxHandle++;
}

void CRiCPPBridge::CContextManagement::removeContext(RtContextHandle handle)
{
	// illContextHandle illContextHandle is not removed
	if ( m_ctxHandle == illContextHandle )
		return;

	// Remove if the handle is an element of the map
	assert(isContext(handle));
	if ( isContext(handle) ) {
		// m_ctxMap[handle].renderingContext() is not 0 but already destroyed
		m_ctxMap.erase(handle);
	}

	if ( handle == m_ctxHandle ) {
		// Was the current context handle - no active context any more
		m_ctxHandle = illContextHandle;
		m_curCtx = m_ctxMap[m_ctxHandle];
	}
}

bool CRiCPPBridge::CContextManagement::context(RtContextHandle handle)
{
	if ( isContext(handle) ) {
		m_curCtx.deactivate();
		m_ctxHandle = handle;
		m_curCtx = m_ctxMap[m_ctxHandle];
		m_curCtx.activate();
		return true;
	}
	m_ctxHandle = illContextHandle;
	m_curCtx = m_ctxMap[m_ctxHandle];
	return false;
}

RtContextHandle CRiCPPBridge::CContextManagement::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	// throws ExceptRiCPPError
{
	try {
		m_curCtx.deactivate();
	} catch (ExceptRiCPPError &e) {
		m_ctxHandle = illContextHandle;
		m_curCtx = m_ctxMap[m_ctxHandle];
		throw e;
	}

	m_ctxHandle = illContextHandle; // No context!
	m_curCtx = m_ctxMap[m_ctxHandle];

	// Try to create a new context creator
	CContextCreator *contextCreator = 0;
	try {
		contextCreator = m_rendererLoader.getContextCreator(name);
	} catch (ExceptRiCPPError &e) {
		// Could not create a context
		throw e;
	}

	// A context creator exists
	assert(contextCreator != 0);
	if ( !contextCreator )
		return illContextHandle;

	if ( m_outer )
		contextCreator->registerRibParserCallback(*m_outer);

	IRiContext *backendContext;
	try {
		backendContext = contextCreator->beginV(name, n, tokens, params);
	} catch (ExceptRiCPPError &e) {
		// Context may be invalid, nevertheless it is stored, so interface
		// requests can be called until end()
		CContext ctx(contextCreator, contextCreator->getContext());
		m_ctxHandle = addContext(ctx);
		m_curCtx = m_ctxMap[m_ctxHandle];
		throw e;
	}

	// A backend exists
	assert(backendContext != 0);
	// Inserts the context creator / context pair into the m_ctxMap
	if ( backendContext != 0 ) {
		assert(m_outer != 0);
		CContext ctx(contextCreator, backendContext);
		m_ctxHandle = addContext(ctx);
		m_curCtx = m_ctxMap[m_ctxHandle];
	}
	return m_ctxHandle;
}


void CRiCPPBridge::CContextManagement::end()
	// throws ExceptRiCPPError
{
	ExceptRiCPPError e;

	if ( m_ctxHandle != illContextHandle ) {
		// CContextCreator *cc = m_curCtx.contextCreator();
		try {
			// Also destroys the rendering context (done by the CContextCreator)
			m_curCtx.end();
		} catch (ExceptRiCPPError &e2) {
			e = e2;
		}
		// The context creator is cached and not removed
		// m_rendererLoader.removeContextCreator(cc);
		
		// Just remove the CContext pair
		removeContext(m_ctxHandle);
		
		// Done by removeContext()
		// m_ctxHandle = illContextHandle;
		// m_curCtx = m_ctxMap[m_ctxHandle];
	}

	if ( e.isError() )
		throw e;
}

CRiCPPBridge::CRiCPPBridge() :
	m_lastError(RIE_NOERROR),
	m_ribFilterList(&m_ribFilter)
{
	m_ricppErrorHandler.setOuter(const_cast<CRiCPPBridge &>(*this));
	m_ctxMgmt.setOuter(*this);
	m_ribFilter.m_next = this;
	// Default options
	RtToken tsearchpath[] = {"renderer", "ribfilter"};
	RtPointer psearchpath[] = {(RtPointer)"$PROGDIR", (RtPointer)".:$PROGDIR"};
	doOptionV("searchpath", sizeof(tsearchpath)/sizeof(char *), tsearchpath, psearchpath);
	doOptionV("standardpath", sizeof(tsearchpath)/sizeof(char *), tsearchpath, psearchpath);

	m_curErrorHandler = &CPrintErrorHandler::func;

	/* Loaded from DLL
	TemplPluginFactory<CRibWriterCreator> *f = new TemplPluginFactory<CRibWriterCreator>;
	if ( f )
		m_ctxMgmt.registerFactory("ribwriter", (TemplPluginFactory<CContextCreator> *)f);
	*/
}

CRiCPPBridge::~CRiCPPBridge()
{
	/* see constructor - no ribwriter creation
	TemplPluginFactory<CContextCreator> *f =
		m_ctxMgmt.unregisterFactory("ribwriter");
	if ( f )
		delete f;
	*/
}

RtInt CRiCPPBridge::getTokens(RtToken token, va_list marker)
{
	RtInt n;
	RtPointer param;

	m_tokens.clear();
	m_params.clear();

	for ( n = 0; token != RI_NULL; ++n, token = va_arg(marker, RtToken) ) {
		param = va_arg(marker, RtPointer);
		m_tokens.push_back(token);
		m_params.push_back(param);
	}
	va_end(marker);

	if ( n == 0 ) {
		m_tokens.resize(1);
		m_params.resize(1);
		m_tokens[0] = NULL;
		m_params[0] = NULL;
	}

	return n;
}


RtVoid CRiCPPBridge::CRiCPPBridgeErrorHandler::handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList)
{
	assert(m_outer != NULL);
	m_outer->m_lastError = code;

	// Do no more error handling if the context is already aborted
	if ( m_outer->m_ctxMgmt.curBackend().aborted() )
		return;

	// To do an option to enable abort on error - done by the abort handler synchronize(RI_ABORT)
	// if ( severity == RIE_SEVERE ) {
	//	m_outer->m_ctxMgmt.abort();
	// }

	static const int ERROR_STR_SIZE = 256;
	char str[ERROR_STR_SIZE];
	str[0] = (char)0;
	if ( message && *message && argList ) {
#ifdef WIN32
		_vsnprintf_s(str, ERROR_STR_SIZE-1, ERROR_STR_SIZE-2, message, argList);
#else
		vsnprintf(str, ERROR_STR_SIZE-1, message, argList);
#endif
		str[ERROR_STR_SIZE-1] = (char)0;
		message = str;
	}

	std::string errorstring;
	ExceptRiCPPError err(code, severity, message, line, file);
	err.formatErrorMessage(errorstring);

	if ( m_outer->m_curErrorHandler )
		(*m_outer->m_curErrorHandler)(*m_outer, code, severity, errorstring.c_str());
}


RtToken CRiCPPBridge::declare(RtString name, RtString declaration)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->declare(name, declaration);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return RI_NULL;
		}
	} else {
		ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::declare(name:\"%s\", declaration:\"%s\")", name ? name : "", declaration ? declaration : "");
	}
	return RI_NULL;
}

RtContextHandle CRiCPPBridge::begin(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	if ( name && *name ) {
		CStringList stringList;
		stringList.expand(name, true); // Only one string, replace environment variables

		CStringList::const_iterator first = stringList.begin();
		if ( first != stringList.end() ) {
			const char *ptr = (*first).c_str();
			if ( ptr && ptr[0] == '|' ) {
				// name was the name of a piped ribfile
				if ( n == 0 ) {
					// remove the 0 entries
					m_tokens.clear();
					m_params.clear();
				}
				m_tokens.push_back(RI_FILE);
				m_params.push_back((RtPointer)(&name)); // Pipe is identified because of leading '|'
				// new name == 0 to load the rib writer
				return beginV(0, ++n, &m_tokens[0], &m_params[0]);
			}
			ptr = strrchr((*first).c_str(), '.');
			if ( ptr && !(strcmp(ptr, ".rib") && strcmp(ptr, ".ribz")) ) {
				// name was the name of a rib file
				if ( n == 0 ) {
					// remove the 0 entries
					m_tokens.clear();
					m_params.clear();
				}
				m_tokens.push_back(RI_FILE);
				m_params.push_back((RtPointer)(&name));
				// new name == 0 to load the rib writer
				return beginV(0, ++n, &m_tokens[0], &m_params[0]);
			}
		}
	}

	return beginV(name, n, &m_tokens[0], &m_params[0]);
}

RtContextHandle CRiCPPBridge::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	// Start the new context
	try {
		RtContextHandle h = m_ctxMgmt.beginV(name, n, tokens, params);
		return h;
	} catch (ExceptRiCPPError &e) {
		// And handle the error, the context was set by m_ctxMgmt appropriately
		ricppErrHandler().handleError(e);
		return illContextHandle;
	}

	return illContextHandle;
}

RtVoid CRiCPPBridge::end(void)
{
	// End the context to clean up, end even aborted contexts.
	if ( m_ctxMgmt.getContext() != illContextHandle ) {
		ExceptRiCPPError e2;
		try {
			m_ctxMgmt.end();
		} catch (ExceptRiCPPError &e) {
			e2 = e;
		}

		assert(m_ctxMgmt.getContext() == illContextHandle);

		if ( e2.isError() ) {
			ricppErrHandler().handleError(e2);
			return;
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::end()");
	}
}

RtVoid CRiCPPBridge::context(RtContextHandle handle)
{
	// Since handle can activate a different context creator, deactivate the old context
	try {
		m_ctxMgmt.context(illContextHandle);
	} catch (ExceptRiCPPError &e) {
		ricppErrHandler().handleError(e);
		return;
	}
	
	if ( !m_ctxMgmt.isContext(handle) ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, "Context handle not found in CRiCPPBridge::context(handle:%lu)", handle);
		return;
	}
	
	bool setContext = false;
	try { 
		setContext = m_ctxMgmt.context(handle);
	} catch (ExceptRiCPPError &e) {
		ricppErrHandler().handleError(e);
		return;
	}

	if ( handle == illContextHandle )
		return;
		
	assert(setContext);
}

RtVoid CRiCPPBridge::frameBegin(RtInt number)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->frameBegin(number);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::frameBegin(number:%d)", (int)number);
	}
}

RtVoid CRiCPPBridge::frameEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->frameEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::frameEnd()");
	}
}

RtVoid CRiCPPBridge::worldBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->worldBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::worldBegin()");
	}
}

RtVoid CRiCPPBridge::worldEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->worldEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::worldEnd()");
	}
}

RtVoid CRiCPPBridge::attributeBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->attributeBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::attributeBegin()");
	}
}

RtVoid CRiCPPBridge::attributeEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->attributeEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::attributeEnd()");
	}
}

RtVoid CRiCPPBridge::transformBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->transformBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::transformBegin()");
	}
}

RtVoid CRiCPPBridge::transformEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->transformEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::transformEnd()");
	}
}

RtVoid CRiCPPBridge::solidBegin(RtToken type)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->solidBegin(type);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::solidBegin(type:\"%s\")", type ? type : "");
	}
}

RtVoid CRiCPPBridge::solidEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->solidEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::solidEnd()");
	}
}

RtObjectHandle CRiCPPBridge::objectBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->objectBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
			return illObjectHandle;
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::objectBegin()");
	}
	return illObjectHandle;
}

RtVoid CRiCPPBridge::objectEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->objectEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::objectEnd()");
	}
}

RtVoid CRiCPPBridge::objectInstance(RtObjectHandle handle)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->objectInstance(handle);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::objectInstance(handle)");
	}
}

RtVoid CRiCPPBridge::motionBegin(RtInt N, RtFloat sample, ...)
{
	va_list marker;
	std::vector<RtFloat> samples;
	RtInt n;

	samples.reserve(N);

	va_start(marker, sample);
	for ( n = 0; n < N; ++n ) {
		samples.push_back(sample);
		sample = (RtFloat)va_arg(marker, double); // 'default argument promotions' float -> double
	}
	va_end(marker);

	motionBeginV(N, samples.empty() ? NULL : &samples[0]);
	// No try and catch, simple call to ...V() function, exception handling is done there
}

RtVoid CRiCPPBridge::motionBeginV(RtInt N, RtFloat times[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->motionBeginV(N, times);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::motionBeginV(N:%d, ...)", (int) N);
	}
}

RtVoid CRiCPPBridge::motionEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->motionEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::motionEnd()");
	}
}


RtVoid CRiCPPBridge::resourceBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->resourceBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::resourceBegin()");
	}
}

RtVoid CRiCPPBridge::resourceEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->resourceEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::resourceEnd()");
	}
}

RtArchiveHandle CRiCPPBridge::archiveBegin(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return archiveBeginV(name, n, &m_tokens[0], &m_params[0]);
}

RtArchiveHandle CRiCPPBridge::archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->archiveBeginV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::archiveBeginV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
	return illArchiveHandle;
}

RtVoid CRiCPPBridge::archiveEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->archiveEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::archiveEnd()");
	}
}


RtVoid CRiCPPBridge::synchronize(RtToken name)
{
	if ( !strcmp(name, RI_ABORT) ) {
		if ( !m_ctxMgmt.curBackend().aborted() )
			m_ctxMgmt.abort();
		return;
	}

	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->synchronize(name);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::synchronize(name:\"%s\")", name ? name : "");
	}
}

RtVoid CRiCPPBridge::system(RtString cmd)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->system(cmd);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "system(system:%s)", cmd ? cmd : "");
	}
}

RtVoid CRiCPPBridge::resource(RtString handle, RtString type, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	resourceV(handle, type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::resourceV(RtString handle, RtString type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->resourceV(handle, type, n, tokens, params);
			return;
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::resourceV(handle:%s, type:%s, n:%d, ...)", handle ? handle : "", type ? type : "", (int)n);
	}

	return;
}

RtVoid CRiCPPBridge::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->format(xres, yres, aspect);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "format(xres:%d, yres:%d, aspect:%f)", (int)xres, (int)yres, (float)aspect);
	}
}

RtVoid CRiCPPBridge::frameAspectRatio(RtFloat aspect)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->frameAspectRatio(aspect);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "frameAspectRatio(aspect:%f)", (float)aspect);
	}
}

RtVoid CRiCPPBridge::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->screenWindow(left, right, bot, top);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "screenWindow(left:%f, right:%f, bot:%f, top:%f)", (float)left, (float)right, (float)bot, (float)top);
	}
}

RtVoid CRiCPPBridge::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->cropWindow(xmin, xmax, ymin, ymax);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "cropWindow(xmin:%f, xmax:%f, ymin:%f, ymax:%f)", (float)xmin, (float)xmax, (float)ymin, (float)ymax);
	}
}

RtVoid CRiCPPBridge::projection(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	projectionV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->projectionV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::projectionV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::clipping(RtFloat hither, RtFloat yon)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->clipping(hither, yon);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::clipping(hither:%f, yon:%f)", (float)hither, (float)yon);
	}
}

RtVoid CRiCPPBridge::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->clippingPlane(x, y, z, nx, ny, nz);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::clippingPlane(x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f)",x, y, z, nx, ny, nz);
	}
}

RtVoid CRiCPPBridge::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->depthOfField(fstop, focallength, focaldistance);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::depthOfField(fstop:%f, focallength:%f, focaldistance:%f)", (float)fstop, (float)focallength, (float)focaldistance);
	}
}

RtVoid CRiCPPBridge::shutter(RtFloat smin, RtFloat smax)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->shutter(smin, smax);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::shutter(smin:%f, smax:%f)", (float)smin, (float)smax);
	}
}

RtVoid CRiCPPBridge::pixelVariance(RtFloat variation)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pixelVariance(variation);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::pixelVariance(variation:%f)", (float)variation);
	}
}

RtVoid CRiCPPBridge::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pixelSamples(xsamples, ysamples);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::pixelSamples(xsamples:%f, ysamples:%f)", (float)xsamples, (float)ysamples);
	}
}

RtVoid CRiCPPBridge::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pixelFilter(function, xwidth, ywidth);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::pixelFilter(function:%s, xwidth:%f, ywidth:%f)", function.name(), (float)xwidth, (float)ywidth);
	}
}

RtVoid CRiCPPBridge::exposure(RtFloat gain, RtFloat gamma)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->exposure(gain, gamma);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::exposure(gain:%f, gamma:%f)", (float)gain, (float)gamma);
	}
}

RtVoid CRiCPPBridge::imager(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	imagerV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->imagerV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::imagerV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->quantize(type, one, qmin, qmax, ampl);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::quantize(type:%s, one:%d, qmin:%d, qmax:%d, ampl:%f)", type ? type : "", (int)one, (int)qmin, (int)qmax, (float)ampl);
	}
}

RtVoid CRiCPPBridge::displayChannel(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	displayChannelV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::displayChannelV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->displayChannelV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::displayChannelV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::display(RtString name, RtToken type, RtString mode, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	displayV(name, type, mode, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->displayV(name, type, mode, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::displayV(name:%s, type:%s, mode:%s, n:%d, ...)", name ? name : "", type ? type : "", mode ? mode : "", (int)n);
	}
}

RtVoid CRiCPPBridge::hider(RtToken type, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	imagerV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->hiderV(type, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::hiderV(type:%s, n:%d, ...)", type ? type : "", (int)n);
	}
}

RtVoid CRiCPPBridge::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->colorSamples(N, nRGB, RGBn);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::colorSamples(N:%d, ...)", (int)N);
	}
}

RtVoid CRiCPPBridge::relativeDetail(RtFloat relativedetail)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->relativeDetail(relativedetail);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::relativeDetail(relativedetail:%f)", (float)relativedetail);
	}
}

RtVoid CRiCPPBridge::control(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	controlV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::controlV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->controlV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		// Options for the Renderer creator and its children.
		// Because the bridge is state less control request are handled
		// in the same way as options if there is no active renderer.
		try {
			doOptionV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	}
}

RtVoid CRiCPPBridge::ribVersion()
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->ribVersion();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::version()");
	}
}

RtVoid CRiCPPBridge::option(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	optionV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->optionV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		// options for the Renderer creator and its children
		try {
			doOptionV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	}
}

RtVoid CRiCPPBridge::doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !name )
		return;

	// The searchpath for the dynamic renderer libraries
	if ( !strcmp(name, "searchpath") ) {
		if ( n < 1 )
			return;

		CStringList sl(&m_pathReplace);
		std::string s;
		int i;
		for ( i = 0; i<n; ++i) {
			if ( !strcmp(tokens[i], "renderer") ) {
				m_pathReplace.path(m_ctxMgmt.searchpath());
				m_pathReplace.standardpath(m_standardPathRenderer.c_str());
				sl.explode(CFilepathConverter::internalPathlistSeperator(), (const char *)params[i], true, true, true);
				sl.implode(CFilepathConverter::internalPathlistSeperator(), s, true);
				m_ctxMgmt.searchpath(s.c_str());
			} else if ( !strcmp(tokens[i], "ribfilter") ) {
				m_pathReplace.path(m_ribFilterList.searchpath());
				m_pathReplace.standardpath(m_standardPathRibFilter.c_str());
				sl.explode(CFilepathConverter::internalPathlistSeperator(), (const char *)params[i], true, true, true);
				sl.implode(CFilepathConverter::internalPathlistSeperator(), s, true);
				m_ribFilterList.searchpath(s.c_str());
			}
			sl.clear();
			m_pathReplace.path("");
			m_pathReplace.standardpath("");
		}
	}

	// The standardpath for the dynamic renderer libraries
	if ( !strcmp(name, "standardpath") ) {
		if ( n < 1 )
			return;

		CStringList sl(&m_pathReplace);
		std::string s;
		int i;
		for ( i = 0; i<n; ++i) {
			if ( !strcmp(tokens[i], "renderer") ) {
				m_pathReplace.path(m_ctxMgmt.searchpath());
				m_pathReplace.standardpath(m_standardPathRenderer.c_str());
				sl.explode(CFilepathConverter::internalPathlistSeperator(), (const char *)params[i], true, true, true);
				sl.implode(CFilepathConverter::internalPathlistSeperator(), s, true);
				m_standardPathRenderer = noNullStr(s.c_str());
			} else if ( !strcmp(tokens[i], "ribfilter") ) {
				m_pathReplace.path(m_ribFilterList.searchpath());
				m_pathReplace.standardpath(m_standardPathRibFilter.c_str());
				m_pathReplace.path(m_ribFilterList.searchpath());
				sl.explode(CFilepathConverter::internalPathlistSeperator(), (const char *)params[i], true, true, true);
				sl.implode(CFilepathConverter::internalPathlistSeperator(), s, true);
				m_standardPathRibFilter = noNullStr(s.c_str());
			}
			sl.clear();
			m_pathReplace.path("");
			m_pathReplace.standardpath("");
		}
	}
}

RtLightHandle CRiCPPBridge::lightSource(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return lightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle CRiCPPBridge::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->lightSourceV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::lightSourceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
	return illLightHandle;
}

RtLightHandle CRiCPPBridge::areaLightSource(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return areaLightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle CRiCPPBridge::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->areaLightSourceV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::areaLightSourceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
	return illLightHandle;
}

RtVoid CRiCPPBridge::attribute(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return attributeV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->attributeV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::attributeV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::color(RtColor Cs)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->color(Cs);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::color(Cs)");
	}
}

RtVoid CRiCPPBridge::opacity(RtColor Os)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->opacity(Os);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::opacity(Os)");
	}
}

RtVoid CRiCPPBridge::surface(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return surfaceV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->surfaceV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::surfaceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::atmosphere(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return atmosphereV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->atmosphereV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::atmosphereV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::interior(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return interiorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->interiorV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::interiorV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::exterior(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return exteriorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->exteriorV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::exteriorV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}


RtVoid CRiCPPBridge::illuminate(RtLightHandle light, RtBoolean onoff)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->illuminate(light, onoff);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::illuminate(light, onoff:%d)", (int)onoff);
	}
}


RtVoid CRiCPPBridge::displacement(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return displacementV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->displacementV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::displacementV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::textureCoordinates(s1:%f, t1:%f, s2:%f, t2:%f, s3:%f, t3:%f, s4:%f, t4:%f)", (float)s1, (float)t1, (float)s2, (float)t2, (float)s3, (float)t3, (float)s4, (float)t4);
	}
}

RtVoid CRiCPPBridge::shadingRate(RtFloat size)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->shadingRate(size);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::shadingRate(size:%f)", (float)size);
	}
}

RtVoid CRiCPPBridge::shadingInterpolation(RtToken type)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->shadingInterpolation(type);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::shadingInterpolation(type:%s)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::matte(RtBoolean onoff)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->matte(onoff);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::matte(onoff:%d)", (int)onoff);
	}
}

RtVoid CRiCPPBridge::bound(RtBound aBound)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->bound(aBound);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::bound(aBound)");
	}
}

RtVoid CRiCPPBridge::detail(RtBound aBound)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->detail(aBound);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::detail(aBound)");
	}
}

RtVoid CRiCPPBridge::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->detailRange(minvis, lowtran, uptran, maxvis);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::detailRange(minvis:%f, lowtran:%f, uptran:%f, maxvis:%f)", (float)minvis, (float)lowtran, (float)uptran, (float)maxvis);
	}
}


RtVoid CRiCPPBridge::geometricApproximation(RtToken type, RtFloat value)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->geometricApproximation(type, value);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::geometricApproximationV(type:%s, value:%f)", type ? type : "", value);
	}
}

RtVoid CRiCPPBridge::geometricRepresentation(RtToken type)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->geometricRepresentation(type);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::geometricRepresentation(type:%s)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::orientation(RtToken anOrientation)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->orientation(anOrientation);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::orientation(anOrientation:%s)", anOrientation ? anOrientation : "");
	}
}

RtVoid CRiCPPBridge::reverseOrientation(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->reverseOrientation();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::reverseOrientation()");
	}
}

RtVoid CRiCPPBridge::sides(RtInt nsides)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->sides(nsides);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::sides(nsides:%d)", (int)nsides);
	}
}

RtVoid CRiCPPBridge::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->basis(ubasis, ustep, vbasis, vstep);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::basis(ubasis, ustep, vbasis, vstep)");
	}
}

RtVoid CRiCPPBridge::trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::trimCurve(nloops:%d, ncurves, order, knot, amin, amax, n, u, v, w)", (int)nloops);
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::identity(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->identity();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::identity()");
	}
}

RtVoid CRiCPPBridge::transform(RtMatrix aTransform)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->transform(aTransform);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::transform(aTransform)");
	}
}

RtVoid CRiCPPBridge::concatTransform(RtMatrix aTransform)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->concatTransform(aTransform);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::concatTransform(aTransform)");
	}
}

RtVoid CRiCPPBridge::perspective(RtFloat fov)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->perspective(fov);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::perspective(fov:%f)",(float)fov);
	}
}

RtVoid CRiCPPBridge::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->translate(dx, dy, dz);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::translate(dx:%f, dy:%f, dz:%f)", (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->rotate(angle, dx, dy, dz);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::rotate(angle:%f, dx:%f, dy:%f, dz:%f)", (float)angle, (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::scale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->scale(dx, dy, dz);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::scale(dx:%f, dy:%f, dz:%f)", (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::skew(angle:%f, dx1:%f, dy1:%f, dz1:%f, dx2:%f, dy2:%f, dz2:%f)", (float)angle, (float)dx1, (float)dy1, (float)dz1, (float)dx2, (float)dy2, (float)dz2);
	}
}

RtVoid CRiCPPBridge::deformation(RtString name, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return deformationV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->deformationV(name, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::deformationV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::scopedCoordinateSystem(RtToken space)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->scopedCoordinateSystem(space);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::scopedCoordinateSystem(space:%s)", space ? space : "");
	}
}

RtVoid CRiCPPBridge::coordinateSystem(RtToken space)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->coordinateSystem(space);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::coordinateSystem(space:%s)", space ? space : "");
	}
}

RtVoid CRiCPPBridge::coordSysTransform(RtToken space)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->coordSysTransform(space);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::coordSysTransform(space:%s)", space ? space : "");
	}
}

RtPoint *CRiCPPBridge::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			return m_ctxMgmt.curBackend().renderingContext()->transformPoints(fromspace, tospace, npoints, points);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::transformPoints(fromspace:%s, tospace:%s, npoints:%d, ...)", fromspace ? fromspace : "", tospace ? tospace : "", (int)npoints);
	}
	return NULL;
}

/******************************************************************************/

RtVoid CRiCPPBridge::polygon(RtInt nvertices, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return polygonV(nvertices, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->polygonV(nvertices, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::polygonV(nvertices:%d, ...)", (int)nvertices);
	}
}

RtVoid CRiCPPBridge::generalPolygon(RtInt nloops, RtInt nverts[], RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return generalPolygonV(nloops, nverts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->generalPolygonV(nloops, nverts, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::generalPolygonV(nloops:%d, ...)", (int)nloops);
	}
}

RtVoid CRiCPPBridge::pointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsPolygonsV(npolys, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::generalPolygonV(npolys:%d, ...)", (int)npolys);
	}
}

RtVoid CRiCPPBridge::pointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::pointsGeneralPolygonsV(npolys:%d, ...)", (int)npolys);
	}
}

RtVoid CRiCPPBridge::patch(RtToken type, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return patchV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->patchV(type, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::patchV(type:%s, ...)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return patchMeshV(type, nu, uwrap, nv, vwrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::patchMeshV(type:%s, nu:%d, uwrap:%s, nv:%d, vwrap:%s, ...)", type ? type : "", (int)nu, uwrap ? uwrap : "", (int)nv, vwrap ? vwrap : "");
	}
}

RtVoid CRiCPPBridge::nuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::nuPatchV(nu:%d, uorder:%d, ...)", (int)nu, (int)uorder);
	}
}


RtVoid CRiCPPBridge::subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::subdivisionMeshV(scheme:%s, nfaces:%d, ...)", scheme ? scheme : "", (int)nfaces);
	}
}


RtVoid CRiCPPBridge::hierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::hierarchicalSubdivisionMeshV(scheme:%s, nfaces:%d, ...)", scheme ? scheme : "", (int)nfaces);
	}
}

RtVoid CRiCPPBridge::sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return sphereV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::sphereV(radius:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)radius, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return coneV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->coneV(height, radius, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::sphereV(height:%f, radius:%f, thetamax:%f, ...)", (float)height, (float)radius, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return cylinderV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::cylinderV(radius:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)radius, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return hyperboloidV(point1, point2, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::hyperboloidV(...)");
	}
}

RtVoid CRiCPPBridge::paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return paraboloidV(rmax, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::paraboloidV(rmax:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)rmax, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return diskV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->diskV(height, radius, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::diskV(height:%f, radius:%f, thetamax:%f, ...)", (float)height, (float)radius, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return torusV(majorrad, minorrad, phimin, phimax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::torusV(majorrad:%f, minorrad:%f, phimin:%f, phimax:%f, thetamax:%f, ...)", (float)majorrad, (float)minorrad, (float)phimin, (float)phimax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::points(RtInt npts, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsV(npts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->pointsV(npts, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::pointsV(npts:%f, ...)", (int)npts);
	}
}

RtVoid CRiCPPBridge::curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return curvesV(type, ncurves, nverts, wrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->curvesV(type, ncurves, nverts, wrap, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::curvesV(type:%s, ncurves:%d, ...)", type ? type : "", (int)ncurves);
	}
}

RtVoid CRiCPPBridge::blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::blobbyV(nleaf:%d, ...)", (int)nleaf);
	}
}

RtVoid CRiCPPBridge::procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			// @todo Also by frontend (as in readArchive(), if not postponed.
			m_ctxMgmt.curBackend().renderingContext()->procedural(data, bound, subdivfunc, freefunc);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::procedural(..., subdivfunc:%s, freefunc:%s)", subdivfunc.name(), freefunc ? freefunc->name() : "no freefunc");
	}
}

RtVoid CRiCPPBridge::geometry(RtToken type, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return geometryV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->geometryV(type, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::geometryV(type:%s, ...)", type ? type : "" );
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::makeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeTextureV(pic:%s, tex:%s, swrap:%s, twrap:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", swrap ? swrap : swrap, twrap ? twrap : twrap, filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeBumpV(pic:%s, tex:%s, swrap:%s, twrap:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", swrap ? swrap : swrap, twrap ? twrap : twrap, filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeLatLongEnvironmentV(pic:%s, tex:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeCubeFaceEnvironmentV(px:%s, nx:%s, py:%s, ny:%s, pz:%s, nz:%s, tex:%s, fov:%f, filterfunc:%s, swidth:%f, twidth:%f, ...)", px ? px : px, nx ? nx : nx, py ? py : py, ny ? ny : ny, pz ? pz : pz, nz ? nz : nz, tex ? tex : "", (float)fov, filterfunc.name(), (float)swidth, (float)twidth);
	}
}

RtVoid CRiCPPBridge::makeShadow(RtString pic, RtString tex, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeShadowV(pic, tex, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeShadowV(pic, tex, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeShadowV(pic:%s, tex:%s, ...)", pic ? pic : pic, tex ? tex : "");
	}
}

RtVoid CRiCPPBridge::makeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeBrickMapV(nNames, ptcnames, bkmname, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->makeBrickMapV(nNames, ptcnames, bkmname, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::makeBrickMapV(nNames:%d, bkmname:%s, ...)", (int)nNames, bkmname ? bkmname : "");
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::archiveRecord(RtToken type, RtString format, ...)
{
	static const int ARCHIVE_BUF_SIZE = 512;
	char archiveBuf[ARCHIVE_BUF_SIZE];
	va_list argList;
	va_start(argList, format);
	archiveBuf[0] = (char)0;
#ifdef WIN32
	_vsnprintf_s(archiveBuf, ARCHIVE_BUF_SIZE-1, ARCHIVE_BUF_SIZE-2, format ? format : "", argList);
#else
	vsnprintf(archiveBuf, ARCHIVE_BUF_SIZE-1, format ? format : "", argList);
#endif
	archiveBuf[ARCHIVE_BUF_SIZE-1] = (char)0;
	va_end(argList);
	archiveRecordV(type, archiveBuf);
}

RtVoid CRiCPPBridge::archiveRecordV(RtToken type, RtString line)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->archiveRecordV(type, line);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::archiveRecordV(type:%s, line:%s, ...)", type ? type : type, line ? line : "");
	}
}

RtVoid CRiCPPBridge::readArchive(RtString name, const IArchiveCallback *callback, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return readArchiveV(name, callback, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->readArchiveV(name, callback, n, tokens, params);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "CRiCPPBridge::readArchiveV(name:%s, callback:%s, ...)", name ? name : name, callback ? callback->name() : "");
	}
}


RtVoid CRiCPPBridge::ifBegin(RtString expr)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->ifBegin(expr);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "ifBegin(expr:%s)", expr ? expr : "");
	}
}

RtVoid CRiCPPBridge::elseIfBegin(RtString expr)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->elseIfBegin(expr);
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "elseIfBegin(expr:%s)", expr ? expr : "");
	}
}

RtVoid CRiCPPBridge::elseBegin(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->elseBegin();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "elseBegin()");
	}
}

RtVoid CRiCPPBridge::ifEnd(void)
{
	if ( m_ctxMgmt.curBackend().valid() ) {
		try {
			m_ctxMgmt.curBackend().renderingContext()->ifEnd();
		} catch (ExceptRiCPPError &e) {
			ricppErrHandler().handleError(e);
		}
	} else {
		if ( !m_ctxMgmt.curBackend().aborted() )
			ricppErrHandler().handleError(RIE_NOTSTARTED, RIE_SEVERE, "ifEnd()");
	}
}
