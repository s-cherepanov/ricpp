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

/** @file baserenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the backend base class of a renderer context.
 *  @todo Orientation, basis for motion blur and DOF
 */

#include "ricpp/baserenderer/baserenderer.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

using namespace RiCPP;

#define RICPP_PREAMBLE(AREQ) \
	EnumRequests req = AREQ; \
	try { \
		if ( !preCheck(req) ) \
			return;

#define RICPP_PREAMBLE_RET(AREQ, ERR_RETVAL) \
	EnumRequests req = AREQ; \
	try { \
		if ( !preCheck(req) ) \
			return ERR_RETVAL;
			
#define RICPP_PROCESS(AREQ) \
{ \
	CRManInterfaceCall *r = macroFactory().AREQ; \
	if ( !r ) \
		throw ExceptRiCPPError( \
			RIE_NOMEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"'%s': Unable to create request object.", CRequestInfo::requestName(req)); \
	processRequest(r); \
}

#define RICPP_PROCESS_IMMEDIATE(AREQ) \
{ \
	CRManInterfaceCall *r = macroFactory().AREQ; \
	if ( !r ) \
		throw ExceptRiCPPError( \
			RIE_NOMEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"'%s': Unable to create request object.", CRequestInfo::requestName(req)); \
	processRequest(r, true); \
}


#define RICPP_POSTAMBLE \
	} catch ( ExceptRiCPPError &e2 ) { \
		ricppErrHandler().handleError(e2); \
		return; \
	} catch ( std::exception &e1 ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what()); \
			return; \
	} catch ( ... ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s'",  CRequestInfo::requestName(req)); \
			return; \
	}
		
#define RICPP_POSTAMBLE_RET(ERR_RETVAL) \
	} catch ( ExceptRiCPPError &e2 ) { \
		ricppErrHandler().handleError(e2); \
		return ERR_RETVAL; \
	} catch ( std::exception &e1 ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what()); \
			return ERR_RETVAL; \
	} catch ( ... ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s'",  CRequestInfo::requestName(req)); \
			return ERR_RETVAL; \
	}

#define RICPP_UNREC_TOKENS { \
\
	if ( n != renderState()->numTokens() ) { \
		ricppErrHandler().handleError( \
			RIE_BADTOKEN, RIE_WARNING, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unrecognized tokens in '%s'",  CRequestInfo::requestName(req)); \
	} \
}

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_parserCallback(0)
{
	m_renderState = 0;
	m_modeStack = 0;
	m_optionsFactory = 0;
	m_attributesFactory = 0;
	m_filterFuncFactory = 0;
	m_macroFactory = 0;
	m_attributesResourceFactory = 0;
}

CBaseRenderer::~CBaseRenderer()
{
	if ( m_renderState ) {
		delete m_renderState;
	}
	m_renderState = 0;

	if ( m_optionsFactory ) {
		deleteOptionsFactory(m_optionsFactory);
	}
	m_optionsFactory = 0;

	if ( m_attributesFactory ) {
		deleteAttributesFactory(m_attributesFactory);
	}
	m_attributesFactory = 0;

	if ( m_filterFuncFactory ) {
		deleteFilterFuncFactory(m_filterFuncFactory);
	}
	m_filterFuncFactory = 0;

	if ( m_modeStack ) {
		deleteModeStack(m_modeStack);
	}
	m_modeStack = 0;

	if ( m_macroFactory ) {
		deleteMacroFactory(m_macroFactory);
	}
	m_macroFactory = 0;

	if ( m_attributesResourceFactory )
	{
		deleteAttributesResourceFactory(m_attributesResourceFactory);
	}
	m_attributesResourceFactory = 0;
}

CRManInterfaceFactory *CBaseRenderer::getNewMacroFactory()
{
	return new CRManInterfaceFactory;
}
void CBaseRenderer::deleteMacroFactory(CRManInterfaceFactory *ptr)
{
	if ( ptr )
		delete ptr;
}
CRManInterfaceFactory &CBaseRenderer::macroFactory()
{
	return *m_macroFactory;
}


RtVoid CBaseRenderer::registerRibParserCallback(IRibParserCallback &cb)
{
	m_parserCallback = &cb;
}


void CBaseRenderer::registerResources()
{
	if ( !m_attributesResourceFactory ) {
		m_attributesResourceFactory = getNewAttributesResourceFactory();
		if ( !m_attributesResourceFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a resource factory for attribue resources.");
		}
	}

	renderState()->registerResourceFactory(m_attributesResourceFactory);
}


CAttributesResourceFactory *CBaseRenderer::getNewAttributesResourceFactory()
{
	return new CAttributesResourceFactory;
}


void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	if ( m_renderState != 0 ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "Render state already initialized in initRenderState()");
	}

	if ( !m_modeStack ) {
		m_modeStack = getNewModeStack();

		if ( !m_modeStack ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		}
	}

	if ( !m_optionsFactory ) {
			m_optionsFactory = getNewOptionsFactory();

		if ( !m_optionsFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an options factory");
		}
	}

	if ( !m_attributesFactory ) {
			m_attributesFactory = getNewAttributesFactory();

		if ( !m_attributesFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
		}
	}

	if ( !m_filterFuncFactory ) {
		m_filterFuncFactory = getNewFilterFuncFactory();

		if ( !m_filterFuncFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an pixel filter factory");
		}
	}

	if ( !m_macroFactory ) {
		m_macroFactory = getNewMacroFactory();

		if ( !m_macroFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an macro factory");
		}
	}

	m_renderState = new CRenderState(*m_modeStack, *m_optionsFactory, *m_attributesFactory, *m_filterFuncFactory); // , *m_macroFactory);

	if ( !m_renderState ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
	}

	registerResources();
}


bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !renderState() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "State not initialized in request %s", CRequestInfo::requestName(req));
	}

	if ( !renderState()->validRequest(req) ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "request %s not in valid block", CRequestInfo::requestName(req));
	}

	renderState()->motionState().request(req);
	if ( (renderState()->motionState().curState() & ~CMotionState::MOT_INSIDE) != 0 ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "inside a motion block: %s", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasOptions() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - options not available.", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasAttributes() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - attributes not available.", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasTransform() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - transformations not available.", CRequestInfo::requestName(req));
	}

	if ( !hasMacroFactory() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - macro factory not available.", CRequestInfo::requestName(req));
	}

	return !renderState()->reject();
}


void CBaseRenderer::defaultDeclarations()
{
	renderState()->defaultDeclarations();
	
	// Default declarations (Tokens are already defined!)
	renderState()->declare(RI_FLATNESS, "float", true);
	renderState()->declare(RI_FOV, "float", true);

	renderState()->declare(RI_INTENSITY, "float", true);
	renderState()->declare(RI_LIGHTCOLOR, "color", true);
	renderState()->declare(RI_FROM, "point", true);
	renderState()->declare(RI_TO, "point", true);
	renderState()->declare(RI_CONEANGLE, "float", true);
	renderState()->declare(RI_CONEDELTAANGLE, "float", true);
	renderState()->declare(RI_BEAMDISTRIBUTION, "float", true);

	renderState()->declare(RI_KA, "float", true);
	renderState()->declare(RI_KD, "float", true);
	renderState()->declare(RI_KS, "float", true);
	renderState()->declare(RI_ROUGHNESS, "float", true);
	renderState()->declare(RI_KR, "float", true);
	renderState()->declare(RI_TEXTURENAME, "string", true);
	renderState()->declare(RI_SPECULARCOLOR, "color", true);
	renderState()->declare(RI_MINDISTANCE, "float", true);
	renderState()->declare(RI_MAXDISTANCE, "float", true);
	renderState()->declare(RI_BACKGROUND, "color", true);
	renderState()->declare(RI_DISTANCE, "float", true);
	renderState()->declare(RI_AMPLITUDE, "float", true);

	renderState()->declare(RI_P, "vertex point", true);
	renderState()->declare(RI_PZ, "vertex float", true);
	renderState()->declare(RI_PW, "vertex hpoint", true);
	renderState()->declare(RI_N,  "varying point", true);  // Normal
	renderState()->declare(RI_NP, "uniform point", true);
	renderState()->declare(RI_CS, "varying color", true);  // Color
	renderState()->declare(RI_OS, "varying color", true);  // Opacity
	renderState()->declare(RI_S,  "varying float", true);  // Texture coordinates
	renderState()->declare(RI_T,  "varying float", true);
	renderState()->declare(RI_ST, "varying float[2]", true);

	renderState()->declare(RI_ORIGIN, "constant integer[2]", true);   // Origin of the display

	renderState()->declare(RI_NAME, "constant string", true);
	renderState()->declare(RI_WIDTH, "varying float", true);
	renderState()->declare(RI_CONSTANTWIDTH, "float", true);

	renderState()->declare(RI_FILE, "string", true);

	renderState()->declare(RI_HANDLEID, "string", true);
}


void CBaseRenderer::recordRequest(CRManInterfaceCall &aRequest)
{
	renderState()->curMacro()->add(aRequest.duplicate());
}


void CBaseRenderer::recordRequest(CRManInterfaceCall *aRequest)
{
	renderState()->curMacro()->add(aRequest);
}


void CBaseRenderer::processRequest(CRManInterfaceCall &aRequest, bool immediately)
{
	aRequest.preProcess(*this);

	if ( !immediately && renderState()->curMacro() ) {
		recordRequest(aRequest);
	}
	
	if ( immediately || (!renderState()->recordMode() && renderState()->executeConditionial()) ) {
		aRequest.doProcess(*this);
	}

	aRequest.postProcess(*this);
}


void CBaseRenderer::processRequest(CRManInterfaceCall *aRequest, bool immediately)
{
	if ( !aRequest )
		return;
	
	bool recorded = false;
	aRequest->preProcess(*this);

	if ( !immediately && renderState()->curMacro() ) {
		recordRequest(aRequest);
		recorded = true;
	}
	
	if ( immediately || (!renderState()->recordMode() && renderState()->executeConditionial()) ) {
		aRequest->doProcess(*this);
	}

	aRequest->postProcess(*this);
	
	if ( !recorded )
		macroFactory().deleteRequest(aRequest);
}


void CBaseRenderer::replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb)
{
	try {
		if ( !preCheck(aRequest.interfaceIdx()) ) {
			return;
		}
		aRequest.preProcess(*this, cb);
		if ( !renderState()->recordMode() && renderState()->executeConditionial() ) {
			aRequest.doProcess(*this, cb);
		}
		aRequest.postProcess(*this, cb);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
	}
}

RtVoid CBaseRenderer::processArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
	CRiArchiveMacro *m = renderState()->findArchiveInstance(handle);
	if ( m ) {
		if ( m->isClosed() ) {
			CRiMacro *msav = renderState()->curReplay();
			renderState()->curReplay(m);
			try {
				m->replay(*this, callback);
				renderState()->curReplay(msav);
			} catch(...) {
				renderState()->curReplay(msav);
				throw;
			}
		} else {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Archive instance %s used before it's ArchiveEnd() (self inclusion, recursion doesn't work).", handle);
		}
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Archive instance %s not found.", handle);
	}
}

RtVoid CBaseRenderer::readArchiveFromStream(RtString name, IRibParserCallback &parserCallback, const IArchiveCallback *callback, const CParameterList &params)
{
	CParameterList p = params;
	CUri sav(renderState()->baseUri());
	std::string oldArchiveName = renderState()->archiveName();
	long oldLineNo = renderState()->lineNo();

	CRibParser parser(parserCallback, *renderState(), renderState()->baseUri());
	try {
		if ( parser.canParse(name) ) {
			renderState()->baseUri() = parser.absUri();
			bool savCache = renderState()->cacheFileArchives();
			if ( savCache ) {
				renderState()->archiveFileBegin(name, macroFactory());
			}
			renderState()->archiveName(name);
			renderState()->lineNo(1);
			parser.parse(callback, params);
			if ( savCache ) {
				renderState()->archiveFileEnd();
			}
			renderState()->archiveName(oldArchiveName.c_str());
			renderState()->lineNo(oldLineNo);
			renderState()->baseUri() = sav;
			parser.close();
			renderState()->curParamList() = p;
		} else {
			throw ExceptRiCPPError(RIE_SYSTEM, RIE_ERROR,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Cannot open archive: %s", name);
		}
	} catch (ExceptRiCPPError &e1) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		throw e1;
	} catch (std::exception &e2) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		throw ExceptRiCPPError(RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"While parsing name: %s", name, e2.what());
	} catch(...) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		throw ExceptRiCPPError(RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error while parsing: %s", name);
	}
}

RtVoid CBaseRenderer::processReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	if ( !emptyStr(name) ) {
		// 1. Look for archive in stored archives
		RtArchiveHandle handle = renderState()->storedArchiveName(name);
		if ( handle ) {
			processArchiveInstance(handle, callback, params);
			return;
		}
	}

	if ( !m_parserCallback ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Parser callbacks not defined for archive: %s", name);
	}

 	// 2. Read archive from stream (name == RI_NULL for stdin)
	readArchiveFromStream(name, *m_parserCallback, callback, params);
}

// ----------------------------------------------------------------------------

RtVoid CBaseRenderer::preBegin(CRiBegin &obj, RtString name, const CParameterList &params)
{
}

RtContextHandle CBaseRenderer::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_BEGIN;

	// Render state is initialized here, there is no mode so it must be not valid.
	// This beginV() is only called through the framework by the frontend after creation of this backend.
	if ( renderState() ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return illContextHandle;
	}

	try {
		// Init

		// Create a new state object
		initRenderState();

		// Indicates that begin has been called
		renderState()->contextBegin();

		// Set the default declarations
		defaultDeclarations();

		// Handle the parameters
		renderState()->parseParameters(RI_BEGIN, name, CParameterClasses(), n, tokens, params);

		RICPP_PROCESS_IMMEDIATE(newRiBegin(renderState()->lineNo(), name, renderState()->curParamList()));

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return illContextHandle;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			__LINE__, __FILE__,
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return illContextHandle;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			__LINE__, __FILE__,
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return illContextHandle;
	}

	RICPP_UNREC_TOKENS

	// There is no handle here, the frontend creates the backend
	return 1; // indicates success
}


RtVoid CBaseRenderer::preEnd(CRiEnd &obj)
{
	renderState()->contextEnd();
}


RtVoid CBaseRenderer::end(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_END;

	if ( !renderState() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, __LINE__, __FILE__, "'%s': State not initialized, break.", CRequestInfo::requestName(req));
		return;
	}

	ExceptRiCPPError err;
	if ( renderState()->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, __LINE__, __FILE__, "'%s': Ended context not at begin-state", CRequestInfo::requestName(req));
	}

	try {
		RICPP_PROCESS_IMMEDIATE(newRiEnd(renderState()->lineNo()));
	} catch ( ExceptRiCPPError &e2 ) {
		err = e2;
	} catch ( std::exception &e1 ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
	} catch ( ... ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s'", CRequestInfo::requestName(req));
	}

	// Delete the state, even if there are errors
	if ( m_renderState ) {
		delete m_renderState;
	}
	m_renderState = 0;

	if ( err.isError() ) {
		ricppErrHandler().handleError(err);
		return;
	}
}

RtVoid CBaseRenderer::preErrorHandler(CRiErrorHandler &obj, const IErrorHandler &handler)
{
}

RtVoid CBaseRenderer::errorHandler(const IErrorHandler &handler)
{
	RICPP_PREAMBLE(REQ_ERROR_HANDLER)
		RICPP_PROCESS(newRiErrorHandler(renderState()->lineNo(), handler));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preSynchronize(CRiSynchronize &obj, RtToken name)
{
}

RtVoid CBaseRenderer::synchronize(RtString name)
{
	RICPP_PREAMBLE(REQ_SYSTEM)
		name = renderState()->tokFindCreate(name);
		RICPP_PROCESS(newRiSynchronize(renderState()->lineNo(), name));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preSystem(CRiSystem &obj, RtString cmd)
{
}

RtVoid CBaseRenderer::system(RtString cmd)
{
	RICPP_PREAMBLE(REQ_SYSTEM)
		RICPP_PROCESS(newRiSystem(renderState()->lineNo(), cmd));
	RICPP_POSTAMBLE
}


void CBaseRenderer::preDeclare(CRiDeclare &obj, RtToken name, RtString declaration)
{
}

RtToken CBaseRenderer::declare(RtToken name, RtString declaration)
{
	RICPP_PREAMBLE_RET(REQ_DECLARE, RI_NULL)
		if ( emptyStr(name) ) {
			throw ExceptRiCPPError(
				RIE_MISSINGDATA,
				RIE_ERROR,
				renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
				"name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration)
			);
		}
		name = renderState()->tokFindCreate(name);
		// Always process the declaration only once where it occurs
		renderState()->declare(name, declaration, false);
		RICPP_PROCESS(newRiDeclare(renderState()->lineNo(), name, declaration));
	RICPP_POSTAMBLE_RET(RI_NULL)

	return name;
}


RtVoid CBaseRenderer::preFrameBegin(CRiFrameBegin &obj, RtInt number)
{
	renderState()->frameBegin(number);
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_FRAME_BEGIN)
		RICPP_PROCESS(newRiFrameBegin(renderState()->lineNo(), number));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preFrameEnd(CRiFrameEnd &obj)
{
	renderState()->frameEnd();
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_FRAME_END)
		RICPP_PROCESS(newRiFrameEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preWorldBegin(CRiWorldBegin &obj)
{
	renderState()->worldBegin();
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_WORLD_BEGIN)
		RICPP_PROCESS(newRiWorldBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preWorldEnd(CRiWorldEnd &obj)
{
	renderState()->worldEnd();
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_WORLD_END)
		RICPP_PROCESS(newRiWorldEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preAttributeBegin(CRiAttributeBegin &obj)
{
	renderState()->attributeBegin();
}


RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_ATTRIBUTE_BEGIN)
		RICPP_PROCESS(newRiAttributeBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preAttributeEnd(CRiAttributeEnd &obj)
{
	renderState()->attributeEnd();
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_ATTRIBUTE_END)
		RICPP_PROCESS(newRiAttributeEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTransformBegin(CRiTransformBegin &obj)
{
	renderState()->transformBegin();
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_TRANSFORM_BEGIN)
		RICPP_PROCESS(newRiTransformBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTransformEnd(CRiTransformEnd &obj)
{
	renderState()->transformEnd();
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_TRANSFORM_END)
		RICPP_PROCESS(newRiTransformEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preSolidBegin(CRiSolidBegin &obj, RtToken type)
{
	renderState()->solidBegin(type);
}

RtVoid CBaseRenderer::solidBegin(RtToken type)
{
	RICPP_PREAMBLE(REQ_SOLID_BEGIN)

		RtToken typeTok = RI_PRIMITIVE;
		type = renderState()->tokFindCreate(type);
		if ( type == RI_INTERSECTION || type == RI_UNION || type == RI_DIFFERENCE ) {
			typeTok = type;
		}
		
		RICPP_PROCESS(newRiSolidBegin(renderState()->lineNo(), typeTok));
		
		if ( type != typeTok ) {
			throw ExceptRiCPPError(
				RIE_BADSOLID, RIE_ERROR,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Unknown solid operation '%s' at '%s' (took RI_PRIMITIVE instead)", noNullStr(type), CRequestInfo::requestName(req));
		}

	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preSolidEnd(CRiSolidEnd &obj)
{
	renderState()->solidEnd();
}

RtVoid CBaseRenderer::solidEnd(void)
{
	RICPP_PREAMBLE(REQ_SOLID_END)
		RICPP_PROCESS(newRiSolidEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preObjectBegin(CRiObjectBegin &obj, RtString name)
{
}

RtObjectHandle CBaseRenderer::objectBegin(RtString name)
{
	RtObjectHandle handle = illObjectHandle;
	RICPP_PREAMBLE_RET(REQ_OBJECT_BEGIN, illObjectHandle)
		name = renderState()->tokFindCreate(name);
		handle = renderState()->objectBegin(name, macroFactory());
		RICPP_PROCESS_IMMEDIATE(newRiObjectBegin(renderState()->lineNo(), handle, name));
	RICPP_POSTAMBLE_RET(illObjectHandle)
	return handle;
}

RtVoid CBaseRenderer::preObjectEnd(CRiObjectEnd &obj)
{
}

RtVoid CBaseRenderer::objectEnd(void)
{
	RICPP_PREAMBLE(REQ_OBJECT_END)
		RICPP_PROCESS_IMMEDIATE(newRiObjectEnd(renderState()->lineNo()));
		renderState()->objectEnd();
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle)
{
}

RtVoid CBaseRenderer::doObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle)
{
	CRiObjectMacro *m = renderState()->objectInstance(handle);
	if ( m ) {
		if ( m->isClosed() ) {
			CRiMacro *msav = renderState()->curReplay();
			renderState()->curReplay(m);
			try {
				m->replay(*this, 0);
				renderState()->curReplay(msav);
			} catch (...) {
				renderState()->curReplay(msav);
				throw;
			}
		} else {
			throw ExceptRiCPPError(
				RIE_BADHANDLE, RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Object instance used before it's doObjectInstance().");
		}
	} else {
		throw ExceptRiCPPError(
			RIE_BADHANDLE, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Object instance not found.");
	}
}

RtVoid CBaseRenderer::objectInstance(RtObjectHandle handle)
{
	RICPP_PREAMBLE(REQ_OBJECT_INSTANCE)
		RICPP_PROCESS(newRiObjectInstance(renderState()->lineNo(), handle));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[])
{
	renderState()->motionBegin(N, times);
}

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[])
{
	RICPP_PREAMBLE(REQ_MOTION_BEGIN)
		RICPP_PROCESS(newRiMotionBegin(renderState()->lineNo(), N, times));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preMotionEnd(CRiMotionEnd &obj)
{
	renderState()->motionEnd();
}

RtVoid CBaseRenderer::motionEnd(void)
{
	RICPP_PREAMBLE(REQ_MOTION_END)
		RICPP_PROCESS(newRiMotionEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preResourceBegin(CRiResourceBegin &obj)
{
	renderState()->resourceBegin();
}

RtVoid CBaseRenderer::resourceBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_RESOURCE_BEGIN)
		RICPP_PROCESS(newRiResourceBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preResourceEnd(CRiResourceEnd &obj)
{
	renderState()->resourceEnd();
}

RtVoid CBaseRenderer::resourceEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_RESOURCE_END)
		RICPP_PROCESS(newRiResourceEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params)
{
	renderState()->resource(*this, handle, type, params);
}

RtVoid CBaseRenderer::resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_RESOURCE)
		handle = renderState()->tokFindCreate(handle);
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_RESOURCE, type, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiResource(renderState()->lineNo(), handle, type, renderState()->curParamList()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params)
{
}

RtArchiveHandle CBaseRenderer::archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) 
{
	RtArchiveHandle handle = illArchiveHandle;
	RICPP_PREAMBLE_RET(REQ_ARCHIVE_BEGIN, illArchiveHandle)
		name = renderState()->tokFindCreate(name);
		handle = renderState()->archiveBegin(name, macroFactory());
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		RICPP_PROCESS_IMMEDIATE(newRiArchiveBegin(renderState()->lineNo(), handle, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illArchiveHandle)
	return handle;
}

RtVoid CBaseRenderer::preArchiveEnd(CRiArchiveEnd &obj)
{
}

RtVoid CBaseRenderer::archiveEnd(void)
{
	RICPP_PREAMBLE(REQ_ARCHIVE_END)
		RICPP_PROCESS_IMMEDIATE(newRiArchiveEnd(renderState()->lineNo()));
		renderState()->archiveEnd();
	RICPP_POSTAMBLE
}



RtVoid CBaseRenderer::preFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect)
{
	renderState()->options().format(xres, yres, aspect);
}

RtVoid CBaseRenderer::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	RICPP_PREAMBLE(REQ_FORMAT)
		RICPP_PROCESS(newRiFormat(renderState()->lineNo(), xres, yres, aspect));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect)
{
	renderState()->options().frameAspectRatio(aspect);
}

RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	RICPP_PREAMBLE(REQ_FRAME_ASPECT_RATIO)
		RICPP_PROCESS(newRiFrameAspectRatio(renderState()->lineNo(), aspect));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	renderState()->options().screenWindow(left, right, bot, top);
}

RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	RICPP_PREAMBLE(REQ_SCREEN_WINDOW)
		RICPP_PROCESS(newRiScreenWindow(renderState()->lineNo(), left, right, bot, top));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	renderState()->options().cropWindow(xmin, xmax, ymin, ymax);
}

RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	RICPP_PREAMBLE(REQ_CROP_WINDOW)
		RICPP_PROCESS(newRiCropWindow(renderState()->lineNo(), xmin, xmax, ymin, ymax));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProjection(CRiProjection &obj, RtToken name, const CParameterList &params)
{
	// Sets the state (can throw)
	renderState()->options().projection(name, params);
}

RtVoid CBaseRenderer::projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_PROJECTION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_PROJECTION, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiProjection(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preClipping(CRiClipping &obj, RtFloat hither, RtFloat yon)
{
	renderState()->options().clipping(hither, yon);
}

RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	RICPP_PREAMBLE(REQ_CLIPPING)
		RICPP_PROCESS(newRiClipping(renderState()->lineNo(), hither, yon));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	renderState()->options().clippingPlane(x, y, z, nx, ny, nz);
}

RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	RICPP_PREAMBLE(REQ_CLIPPING_PLANE)
		RICPP_PROCESS(newRiClippingPlane(renderState()->lineNo(), x, y, z, nx, ny, nz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	renderState()->options().depthOfField(fstop, focallength, focaldistance);
}

RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	RICPP_PREAMBLE(REQ_DEPTH_OF_FIELD)
		RICPP_PROCESS(newRiDepthOfField(renderState()->lineNo(), fstop, focallength, focaldistance));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preShutter(CRiShutter &obj, RtFloat smin, RtFloat smax)
{
	renderState()->options().shutter(smin, smax);
}

RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	RICPP_PREAMBLE(REQ_SHUTTER)
		RICPP_PROCESS(newRiShutter(renderState()->lineNo(), smin, smax));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::prePixelVariance(CRiPixelVariance &obj, RtFloat variation)
{
	renderState()->options().pixelVariance(variation);
}

RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	RICPP_PREAMBLE(REQ_PIXEL_VARIANCE)
		RICPP_PROCESS(newRiPixelVariance(renderState()->lineNo(), variation));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::prePixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples)
{
	renderState()->options().pixelSamples(xsamples, ysamples);
}

RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	RICPP_PREAMBLE(REQ_PIXEL_SAMPLES)
		RICPP_PROCESS(newRiPixelSamples(renderState()->lineNo(), xsamples, ysamples));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::prePixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	renderState()->options().pixelFilter(function, xwidth, ywidth);
}

RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	RICPP_PREAMBLE(REQ_PIXEL_FILTER)
		RICPP_PROCESS(newRiPixelFilter(renderState()->lineNo(), function, xwidth, ywidth));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma)
{
	renderState()->options().exposure(gain, gamma);
}

RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	RICPP_PREAMBLE(REQ_EXPOSURE)
		RICPP_PROCESS(newRiExposure(renderState()->lineNo(), gain, gamma));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preImager(CRiImager &obj, RtString name, const CParameterList &params)
{
	renderState()->options().imager(name, params);
}

RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_IMAGER)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_IMAGER, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiImager(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	renderState()->options().quantize(type, one, qmin, qmax, ampl);
}

RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	RICPP_PREAMBLE(REQ_QUANTIZE)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiQuantize(renderState()->lineNo(), type, one, qmin, qmax, ampl));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params)
{
	renderState()->options().displayChannel(renderState()->dict(), renderState()->options().colorDescr(), channel, params);
}

RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_DISPLAY_CHANNEL)
		channel = renderState()->tokFindCreate(channel);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiDisplayChannel(renderState()->lineNo(), channel, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::preDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params)
{
	renderState()->options().display(name, type, mode, params);
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_DISPLAY)
		type = renderState()->tokFindCreate(type);
		mode = renderState()->tokFindCreate(mode);
		renderState()->parseParameters(RI_DISPLAY, type, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiDisplay(renderState()->lineNo(), name, type, mode, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preHider(CRiHider &obj, RtToken type, const CParameterList &params)
{
	renderState()->options().hider(type, params);
}

RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_HIDER)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_HIDER, type, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiHider(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	renderState()->options().colorSamples(N, nRGB, RGBn);
}

RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	RICPP_PREAMBLE(REQ_COLOR_SAMPLES)
		RICPP_PROCESS(newRiColorSamples(renderState()->lineNo(), N, nRGB, RGBn));
	RICPP_POSTAMBLE

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(
			RIE_RANGE, RIE_WARNING,
			"Illegal values for the parameters of colorSamples(), using identity mapping."
		);
	}
}


RtVoid CBaseRenderer::preRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail)
{
	renderState()->options().relativeDetail(relativedetail);
}

RtVoid CBaseRenderer::relativeDetail(RtFloat relativedetail)
{
	RICPP_PREAMBLE(REQ_RELATIVE_DETAIL)
		RICPP_PROCESS(newRiRelativeDetail(renderState()->lineNo(), relativedetail));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preOption(CRiOption &obj, RtToken name, const CParameterList &params)
{
	renderState()->option(name, params);
}

RtVoid CBaseRenderer::optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_OPTION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_OPTION, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiOption(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preControl(CRiControl &obj, RtToken name, const CParameterList &params)
{
	renderState()->control(name, params);
}

RtVoid CBaseRenderer::controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_CONTROL)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_CONTROL, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS_IMMEDIATE(newRiControl(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preLightSource(CRiLightSource &obj, RtString name, const CParameterList &params)
{
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		throw ExceptRiCPPError(
			RIE_BADHANDLE,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Handle not created for LightSource \"%s\"",
			noNullStr(name));
	}
}

	
RtVoid CBaseRenderer::doLightSource(CRiLightSource &obj, RtString name, const CParameterList &params)
{
	if ( !renderState()->newLightSource(obj.handle(), false, name, params) ) {
		throw ExceptRiCPPError(
			RIE_NOMEM,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"LightSource of \"%s\"",
			noNullStr(name));
	}
}

RtLightHandle CBaseRenderer::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;

	RICPP_PREAMBLE_RET(REQ_LIGHT_SOURCE, illLightHandle)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);
		h = renderState()->newLightHandle(name, renderState()->curParamList());
		RICPP_PROCESS(newRiLightSource(renderState()->lineNo(), h, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illLightHandle)
	RICPP_UNREC_TOKENS

	return h;
}


RtVoid CBaseRenderer::preAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params)
{
	if ( name != RI_NULL ) {
		// Test the handle
		CHandle *handle = renderState()->lightSourceHandle(obj.handle());
		if ( !handle ) {
			throw ExceptRiCPPError(
				RIE_BADHANDLE,
				RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Handle not created for AreaLightSource \"%s\"",
				noNullStr(name));
		}
	} else {
		// AreaLightSource ended
		renderState()->endAreaLightSource();
	}
}

RtVoid CBaseRenderer::doAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params)
{
	// name == RI_NULL : Area light source was closed
	if ( name != RI_NULL ) {
		// Create an area light source
		if ( !renderState()->newLightSource(obj.handle(), true, name, params) ) {
			throw ExceptRiCPPError(
				RIE_NOMEM,
				RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"AreaLightSource of \"%s\"",
				noNullStr(name));
		}
		renderState()->startAreaLightSource(obj.handle());
	}
}
	
RtLightHandle CBaseRenderer::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;

	RICPP_PREAMBLE_RET(REQ_AREA_LIGHT_SOURCE, illLightHandle)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_AREA_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);
		h = renderState()->newLightHandle(name, renderState()->curParamList());
		RICPP_PROCESS(newRiAreaLightSource(renderState()->lineNo(), h, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illLightHandle)
	RICPP_UNREC_TOKENS
	return h;
}

RtVoid CBaseRenderer::preIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff)
{
	// Only a test if LightSource was declared
	CHandle *handle = renderState()->lightSourceHandle(light);
	if ( !handle ) {
		throw ExceptRiCPPError(
			RIE_BADHANDLE,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Illuminate a light source (Handle: %s)",
			noNullStr(light));
	}
}


RtVoid CBaseRenderer::doIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff)
{
	CLightSource *l = renderState()->lightSourceInstance(light);
	renderState()->attributes().illuminate(l, onoff);
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	RICPP_PREAMBLE(REQ_ILLUMINATE)
		RICPP_PROCESS(newRiIlluminate(renderState()->lineNo(), light, onoff));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params)
{
	renderState()->attribute(name, params);
}


RtVoid CBaseRenderer::attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_ATTRIBUTE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATTRIBUTE, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiAttribute(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preColor(CRiColor &obj, RtColor Cs)
{
	renderState()->attributes().color(Cs);
}

RtVoid CBaseRenderer::color(RtColor Cs)
{
	RICPP_PREAMBLE(REQ_COLOR)
		RICPP_PROCESS(newRiColor(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Cs));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preOpacity(CRiOpacity &obj, RtColor Os)
{
	renderState()->attributes().opacity(Os);
}

RtVoid CBaseRenderer::opacity(RtColor Os)
{
	RICPP_PREAMBLE(REQ_OPACITY)
		RICPP_PROCESS(newRiOpacity(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Os));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preSurface(CRiSurface &obj, RtString name, const CParameterList &params)
{
	renderState()->attributes().surface(name, params);
}

RtVoid CBaseRenderer::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_SURFACE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_SURFACE, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiSurface(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params)
{
	renderState()->attributes().atmosphere(name, params);
}

RtVoid CBaseRenderer::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_ATMOSPHERE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATMOSPHERE, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiAtmosphere(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preInterior(CRiInterior &obj, RtString name, const CParameterList &params)
{
	renderState()->attributes().interior(name, params);
}

RtVoid CBaseRenderer::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_INTERIOR)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_INTERIOR, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiInterior(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preExterior(CRiExterior &obj, RtString name, const CParameterList &params)
{
	renderState()->attributes().exterior(name, params);
}

RtVoid CBaseRenderer::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_EXTERIOR)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_EXTERIOR, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiExterior(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params)
{
	renderState()->attributes().displacement(name, params);
}

RtVoid CBaseRenderer::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_DISPLACEMENT)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DISPLACEMENT, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiDisplacement(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	renderState()->attributes().textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
}

RtVoid CBaseRenderer::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	RICPP_PREAMBLE(REQ_TEXTURE_COORDINATES)
		RICPP_PROCESS(newRiTextureCoordinates(renderState()->lineNo(), s1, t1, s2, t2, s3, t3, s4, t4));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preShadingRate(CRiShadingRate &obj, RtFloat size)
{
	renderState()->attributes().shadingRate(size);
}

RtVoid CBaseRenderer::shadingRate(RtFloat size)
{
	RICPP_PREAMBLE(REQ_SHADING_RATE)
		RICPP_PROCESS(newRiShadingRate(renderState()->lineNo(), size));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preShadingInterpolation(CRiShadingInterpolation &obj, RtToken type)
{
	renderState()->attributes().shadingInterpolation(type);
}

RtVoid CBaseRenderer::shadingInterpolation(RtToken type)
{
	RICPP_PREAMBLE(REQ_SHADING_INTERPOLATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiShadingInterpolation(renderState()->lineNo(), type));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preMatte(CRiMatte &obj, RtBoolean onoff)
{
	renderState()->attributes().matte(onoff);
}

RtVoid CBaseRenderer::matte(RtBoolean onoff)
{
	RICPP_PREAMBLE(REQ_MATTE)
		RICPP_PROCESS(newRiMatte(renderState()->lineNo(), onoff == RI_FALSE ? RI_FALSE : RI_TRUE));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preBound(CRiBound &obj, RtBound aBound)
{
	renderState()->attributes().bound(aBound);
}

RtVoid CBaseRenderer::bound(RtBound aBound)
{
	RICPP_PREAMBLE(REQ_BOUND)
		RICPP_PROCESS(newRiBound(renderState()->lineNo(), aBound));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preDetail(CRiDetail &obj, RtBound aBound)
{
	renderState()->attributes().detail(aBound);
}

RtVoid CBaseRenderer::detail(RtBound aBound)
{
	RICPP_PREAMBLE(REQ_DETAIL)
		RICPP_PROCESS(newRiDetail(renderState()->lineNo(), aBound));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( !renderState()->attributes().detailRangeCalled() ) {
		renderState()->attributes().detailRange(minvis, lowtran, uptran, maxvis);
		renderState()->storeAttributes();
	} else {
		renderState()->restoreAttributes();
		renderState()->attributes().detailRange(minvis, lowtran, uptran, maxvis);
	}
}

RtVoid CBaseRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	bool changed = false;
	if ( minvis > lowtran ) {
		changed = true;
		std::swap(minvis, lowtran);
	}
	if ( lowtran > uptran ) {
		changed = true;
		std::swap(lowtran, uptran);
		if ( minvis > lowtran ) {
			std::swap(minvis, lowtran);
		}
	}
	if ( uptran > maxvis ) {
		changed = true;
		std::swap(uptran, maxvis);
		if ( lowtran > uptran ) {
			std::swap(lowtran, uptran);
			if ( minvis > lowtran ) {
				std::swap(minvis, lowtran);
			}
		}
	}

	RICPP_PREAMBLE(REQ_DETAIL_RANGE)
		RICPP_PROCESS(newRiDetailRange(renderState()->lineNo(), minvis, lowtran, uptran, maxvis));
	RICPP_POSTAMBLE

	if ( changed ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"'%s': Detail range malformed, changed to (%f, %f, %f, %f)", 
			CRequestInfo::requestName(req), minvis, lowtran, uptran, maxvis);
	}
}


RtVoid CBaseRenderer::preGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value)
{
	renderState()->attributes().geometricApproximation(type, value);
}

RtVoid CBaseRenderer::geometricApproximation(RtToken type, RtFloat value)
{
	RICPP_PREAMBLE(REQ_GEOMETRIC_APPROXIMATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiGeometricApproximation(renderState()->lineNo(), type, value));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type)
{
	renderState()->attributes().geometricRepresentation(type);
}

RtVoid CBaseRenderer::geometricRepresentation(RtToken type)
{
	RICPP_PREAMBLE(REQ_GEOMETRIC_REPRESENTATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiGeometricRepresentation(renderState()->lineNo(), type));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preOrientation(CRiOrientation &obj, RtToken anOrientation)
{
	renderState()->attributes().orientation(anOrientation);
}

RtVoid CBaseRenderer::orientation(RtToken anOrientation)
{
	RtToken tokOrientation = RI_INSIDE;
	RICPP_PREAMBLE(REQ_ORIENTATION)
		anOrientation = renderState()->tokFindCreate(anOrientation);
		if ( anOrientation == RI_LH ||
		     anOrientation == RI_RH ||
		     anOrientation == RI_OUTSIDE )
		{
			tokOrientation = anOrientation;
		}
		RICPP_PROCESS(newRiOrientation(renderState()->lineNo(), tokOrientation));
	RICPP_POSTAMBLE

	if ( tokOrientation != anOrientation ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in '%s': %s, using %s instead.", 
			CRequestInfo::requestName(req), noNullStr(anOrientation), noNullStr(tokOrientation));
	}
}


RtVoid CBaseRenderer::preReverseOrientation(CRiReverseOrientation &obj)
{
	renderState()->attributes().reverseOrientation();
}

RtVoid CBaseRenderer::reverseOrientation(void)
{
	RICPP_PREAMBLE(REQ_REVERSE_ORIENTATION)
		RICPP_PROCESS(newRiReverseOrientation(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preSides(CRiSides &obj, RtInt nsides)
{
	renderState()->attributes().sides(nsides);
}

RtVoid CBaseRenderer::sides(RtInt nsides)
{
	RICPP_PREAMBLE(REQ_SIDES)
		RICPP_PROCESS(newRiSides(renderState()->lineNo(), nsides));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	renderState()->attributes().basis(ubasis, ustep, vbasis, vstep);
}

RtVoid CBaseRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	RICPP_PREAMBLE(REQ_BASIS)
		RICPP_PROCESS(newRiBasis(renderState()->lineNo(), ubasis, ustep, vbasis, vstep));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	renderState()->attributes().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
}

RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	RICPP_PREAMBLE(REQ_TRIM_CURVE)
		RICPP_PROCESS(newRiTrimCurve(renderState()->lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preIdentity(CRiIdentity &obj)
{
	renderState()->curTransform().identity();
}


RtVoid CBaseRenderer::identity(void)
{
	RICPP_PREAMBLE(REQ_IDENTITY)
		RICPP_PROCESS(newRiIdentity(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTransform(CRiTransform &obj, RtMatrix aTransform)
{
	renderState()->curTransform().transform(aTransform);
}


RtVoid CBaseRenderer::transform(RtMatrix aTransform)
{
	RICPP_PREAMBLE(REQ_TRANSFORM)
		RICPP_PROCESS(newRiTransform(renderState()->lineNo(), aTransform));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform)
{
	renderState()->curTransform().concatTransform(aTransform);
}


RtVoid CBaseRenderer::concatTransform(RtMatrix aTransform)
{
	RICPP_PREAMBLE(REQ_CONCAT_TRANSFORM)
		RICPP_PROCESS(newRiConcatTransform(renderState()->lineNo(), aTransform));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::prePerspective(CRiPerspective &obj, RtFloat fov)
{
	renderState()->curTransform().perspective(fov);
}


RtVoid CBaseRenderer::perspective(RtFloat fov)
{
	RICPP_PREAMBLE(REQ_PERSPECTIVE)
		RICPP_PROCESS(newRiPerspective(renderState()->lineNo(), fov));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().translate(dx, dy, dz);
}


RtVoid CBaseRenderer::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_TRANSLATE)
		RICPP_PROCESS(newRiTranslate(renderState()->lineNo(), dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().rotate(angle, dx, dy, dz);
}


RtVoid CBaseRenderer::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_ROTATE)
		RICPP_PROCESS(newRiRotate(renderState()->lineNo(), angle, dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().scale(dx, dy, dz);
}


RtVoid CBaseRenderer::scale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_SCALE)
		RICPP_PROCESS(newRiScale(renderState()->lineNo(), dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	renderState()->curTransform().skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
}


RtVoid CBaseRenderer::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	RICPP_PREAMBLE(REQ_SKEW)
		RICPP_PROCESS(newRiSkew(renderState()->lineNo(), angle, dx1, dy1, dz1, dx2, dy2, dz2));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preDeformation(CRiDeformation &obj, RtString name, const CParameterList &params)
{
}

RtVoid CBaseRenderer::doDeformation(CRiDeformation &obj, RtString name, const CParameterList &params)
{
	ricppErrHandler().handleError(
		RIE_UNIMPLEMENT, RIE_WARNING,
		renderState()->printLineNo(__LINE__),
		renderState()->printName(__FILE__),
		"RiDeformation is not supported, name = '%s'",  noNullStr(name));
}

RtVoid CBaseRenderer::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_DEFORMATION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DEFORMATION, name, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiDeformation(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space)
{
	renderState()->scopedCoordinateSystem(space);
}

RtVoid CBaseRenderer::scopedCoordinateSystem(RtToken space)
{
	RICPP_PREAMBLE(REQ_SCOPED_COORDINATE_SYSTEM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiScopedCoordinateSystem(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preCoordinateSystem(CRiCoordinateSystem &obj, RtToken space)
{
	renderState()->coordinateSystem(space);
}

RtVoid CBaseRenderer::coordinateSystem(RtToken space)
{
	RICPP_PREAMBLE(REQ_COORDINATE_SYSTEM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiCoordinateSystem(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preCoordSysTransform(CRiCoordSysTransform &obj, RtToken space)
{
	const CTransformation *t = renderState()->findTransform(space);
	if ( t ) {
		RtToken curSpaceType = renderState()->curTransform().spaceType();

		if ( curSpaceType != t->spaceType() ) {
			// To do handle different space types.
		} else {
			renderState()->curTransform() = *t;
			renderState()->curTransform().spaceType(curSpaceType);
		}

	}
}

RtVoid CBaseRenderer::coordSysTransform(RtToken space)
{
	RICPP_PREAMBLE(REQ_COORD_SYS_TRANSFORM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiCoordSysTransform(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
{
}

RtVoid CBaseRenderer::doTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
{
	if ( npoints <= 0 || points == 0 ) {
		return;
	}
	const CTransformation *from = renderState()->findTransform(fromspace);
	const CTransformation *to   = renderState()->findTransform(tospace);
	if ( from == 0 || to == 0 ) {
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "transformPoints(%s (%s found), %s (%s found)), spaces not found", noNullStr(fromspace), from ? "was" : "not", noNullStr(tospace), to ? "was" : "not");
		return;
	}

	/// @todo Implementation of doTransformPoints()
}

RtPoint *CBaseRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	RICPP_PREAMBLE_RET(REQ_TRANSFORM_POINTS, 0)
		fromspace = renderState()->tokFindCreate(fromspace);
		tospace = renderState()->tokFindCreate(tospace);
		RICPP_PROCESS(newRiTransformPoints(renderState()->lineNo(), fromspace, tospace, npoints, points));
	RICPP_POSTAMBLE_RET(0);
	return points ? &points[0] : 0;
}


RtVoid CBaseRenderer::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_POLYGON)
		renderState()->parseParameters(CPolygonClasses(nvertices), n, tokens, params);
		RICPP_PROCESS(newRiPolygon(renderState()->lineNo(), nvertices, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_GENERAL_POLYGON)
		renderState()->parseParameters(CGeneralPolygonClasses(nloops, nverts), n, tokens, params);
		RICPP_PROCESS(newRiGeneralPolygon(renderState()->lineNo(), nloops, nverts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_POINTS_POLYGONS)
		renderState()->parseParameters(CPointsPolygonsClasses(npolys, nverts, verts), n, tokens, params);
		RICPP_PROCESS(newRiPointsPolygons(renderState()->lineNo(), npolys, nverts, verts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_POINTS_GENERAL_POLYGONS)
		renderState()->parseParameters(CPointsGeneralPolygonsClasses(npolys, nloops, nverts, verts), n, tokens, params);
		RICPP_PROCESS(newRiPointsGeneralPolygons(renderState()->lineNo(), npolys, nloops, nverts, verts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_PATCH)
		type = renderState()->tokFindCreate(type);
		if ( type != RI_BILINEAR && type != RI_BICUBIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "type neither RI_BILINEAR nor RI_BICUBIC at %s(), type == '%s'", CRequestInfo::requestName(req), type);
		}
		renderState()->parseParameters(CPatchClasses(type), n, tokens, params);
		RICPP_PROCESS(newRiPatch(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_PATCH_MESH)
		type = renderState()->tokFindCreate(type);
		if ( type != RI_BILINEAR && type != RI_BICUBIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "type neither RI_BILINEAR nor RI_BICUBIC at %s(), type == '%s'", CRequestInfo::requestName(req), type);
		}

		uwrap = renderState()->tokFindCreate(uwrap);
		if ( uwrap != RI_PERIODIC && uwrap != RI_NONPERIODIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "uwrap neither RI_PERIODIC nor RI_NONPERIODIC at %s(), uwrap == '%s'", CRequestInfo::requestName(req), uwrap);
		}

		vwrap = renderState()->tokFindCreate(vwrap);
		if ( vwrap != RI_PERIODIC && vwrap != RI_NONPERIODIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "vwrap neither RI_PERIODIC nor RI_NONPERIODIC at %s(), vwrap == '%s'", CRequestInfo::requestName(req), vwrap);
		}

		RtInt ustep, vstep;
		if ( type == RI_BILINEAR ) {
			ustep = vstep = 1;
		} else {
			RtBasis ubasis, vbasis;
			renderState()->attributes().getBasis(ubasis, ustep, vbasis, vstep);
		}

		renderState()->parseParameters(CPatchMeshClasses(type, nu, ustep, uwrap, nv, vstep, vwrap), n, tokens, params);
		RICPP_PROCESS(newRiPatchMesh(renderState()->lineNo(), ustep, vstep, type, nu, uwrap, nv, vwrap, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_NU_PATCH)
		renderState()->parseParameters(CNuPatchClasses(nu, uorder, nv, vorder), n, tokens, params);
		RICPP_PROCESS(newRiNuPatch(renderState()->lineNo(), nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_SUBDIVISION_MESH)
		scheme = renderState()->tokFindCreate(scheme);
		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);
		RICPP_PROCESS(newRiSubdivisionMesh(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_HIERARCHICAL_SUBDIVISION_MESH)
		scheme = renderState()->tokFindCreate(scheme);
		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);
		RICPP_PROCESS(newRiHierarchicalSubdivisionMesh(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_SPHERE)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiSphere(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_CONE)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiCone(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_CYLINDER)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiCylinder(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_HYPERBOLOID)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiHyperboloid(renderState()->lineNo(), point1, point2, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_PARABOLOID)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiParaboloid(renderState()->lineNo(), rmax, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_DISK)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiDisk(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_TORUS)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		RICPP_PROCESS(newRiTorus(renderState()->lineNo(), majorrad, minorrad, phimin, phimax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_POINTS)
		renderState()->parseParameters(CPointsClasses(npts), n, tokens, params);
		RICPP_PROCESS(newRiPoints(renderState()->lineNo(), npts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_CURVES)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CCurvesClasses(type, ncurves, nverts, wrap, renderState()->attributes().vStep()), n, tokens, params);
		RICPP_PROCESS(newRiCurves(renderState()->lineNo(), renderState()->attributes().vStep(), type, ncurves, nverts, wrap, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_BLOBBY)
		renderState()->parseParameters(CBlobbyClasses(nleaf), n, tokens, params);
		RICPP_PROCESS(newRiBlobby(renderState()->lineNo(), nleaf, ncode, code, nflt, flt, nstr, str, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
{
	if ( !m_parserCallback ) {
		throw ExceptRiCPPError(
			RIE_BUG,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"The frontend is unknown, while executing a procedural %s.", subdivfunc.name());

	}
}

RtVoid CBaseRenderer::doProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
{
	subdivfunc(m_parserCallback->frontend(), data, RI_INFINITY);
	if ( freefunc )
		(*freefunc)(data);
}

RtVoid CBaseRenderer::procedural(RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
{
	RICPP_PREAMBLE(REQ_PROCEDURAL)
		RICPP_PROCESS(newRiProcedural(renderState()->lineNo(), data, bound, subdivfunc, freefunc));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_GEOMETRY)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiGeometry(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_TEXTURE)
		swrap = renderState()->tokFindCreate(swrap);
		twrap = renderState()->tokFindCreate(twrap);
		renderState()->parseParameters(RI_TEXTURE, RI_TEXTURE, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeTexture(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_BUMP)
		swrap = renderState()->tokFindCreate(swrap);
		twrap = renderState()->tokFindCreate(twrap);
		renderState()->parseParameters(RI_TEXTURE, RI_BUMP, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeBump(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_LAT_LONG_ENVIRONMENT)
		renderState()->parseParameters(RI_TEXTURE, RI_LAT_LONG_ENVIRONMENT, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeLatLongEnvironment(renderState()->lineNo(), pic, tex, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_CUBE_FACE_ENVIRONMENT)
		renderState()->parseParameters(RI_TEXTURE, RI_CUBE_FACE_ENVIRONMENT, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeCubeFaceEnvironment(renderState()->lineNo(), px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_SHADOW)
		renderState()->parseParameters(RI_TEXTURE, RI_SHADOW, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeShadow(renderState()->lineNo(), pic, tex, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_MAKE_BRICK_MAP)
		renderState()->parseParameters(RI_TEXTURE, RI_BRICK_MAP, CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiMakeBrickMap(renderState()->lineNo(), nNames, ptcnames, bkmname, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}

RtVoid CBaseRenderer::archiveRecordV(RtToken type, RtString line)
{
	RtToken typeTok = RI_COMMENT;
	RICPP_PREAMBLE(REQ_ARCHIVE_RECORD)
		type = renderState()->tokFindCreate(type);
		if ( type == RI_VERBATIM || type == RI_STRUCTURE )
			typeTok = type;
		RICPP_PROCESS(newRiArchiveRecord(renderState()->lineNo(), typeTok, line));
	RICPP_POSTAMBLE

	if ( typeTok != type ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"'%s': unknown comment type %s, taking %s", 
			CRequestInfo::requestName(req), noNullStr(type), noNullStr(typeTok));
	}
}

RtVoid CBaseRenderer::doReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	processReadArchive(name, callback, params);
}

RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	RICPP_PREAMBLE(REQ_READ_ARCHIVE)
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		RICPP_PROCESS(newRiReadArchive(renderState()->lineNo(), name, callback, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS
}


RtVoid CBaseRenderer::preIfBegin(CRiIfBegin &obj, RtString expr)
{
	renderState()->ifBegin(expr);
}


RtVoid CBaseRenderer::ifBegin(RtString expr)
{
	RICPP_PREAMBLE(REQ_IF_BEGIN)
		RICPP_PROCESS(newRiIfBegin(renderState()->lineNo(), expr));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preElseIfBegin(CRiElseIfBegin &obj, RtString expr)
{
	renderState()->elseIfBegin(expr);
}


RtVoid CBaseRenderer::elseIfBegin(RtString expr)
{
	RICPP_PREAMBLE(REQ_ELSE_IF)
		RICPP_PROCESS(newRiElseIfBegin(renderState()->lineNo(), expr));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preElseBegin(CRiElseBegin &obj)
{
	renderState()->elseBegin();
}


RtVoid CBaseRenderer::elseBegin(void)
{
	RICPP_PREAMBLE(REQ_ELSE)
		RICPP_PROCESS(newRiElseBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preIfEnd(CRiIfEnd &obj)
{
	renderState()->ifEnd();
}

RtVoid CBaseRenderer::ifEnd(void)
{
	RICPP_PREAMBLE(REQ_IF_END)
		RICPP_PROCESS(newRiIfEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}
