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
 */

#include "ricpp/baserenderer/baserenderer.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

using namespace RiCPP;

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


void CBaseRenderer::recordRequest(CRManInterfaceCall &aRequest)
{
	renderState()->curMacro()->add(aRequest.duplicate());
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
		if ( m_parserCallback ) {
			m_parserCallback->ricppErrHandler().handleError(e2);
		} else {
			ricppErrHandler().handleError(e2);
		}
	} catch ( std::exception &e1 ) {
		if ( m_parserCallback ) {
			m_parserCallback->ricppErrHandler().handleError(
				RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
		} else {
			ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
		}
	} catch ( ... ) {
		if ( m_parserCallback ) {
			m_parserCallback->ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
		} else {
			ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
		}
	}
}


// ----------------------------------------------------------------------------


RtVoid CBaseRenderer::system(RtString cmd)
{
	EnumRequests req = REQ_SYSTEM;
	try {

		if ( !preCheck(req) )
			return;

		CRiSystem r(renderState()->lineNo(), cmd);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


void CBaseRenderer::preDeclare(RtToken name, RtString declaration)
{
}

void CBaseRenderer::postDeclare(RtToken name, RtString declaration)
{
}

RtToken CBaseRenderer::declare(RtToken name, RtString declaration)
{
	EnumRequests req = REQ_DECLARE;
	try {

		// Test for correct state
		if ( !preCheck(req) )
			return RI_NULL;

		// Name has to exist
		if ( emptyStr(name) ) {
			throw ExceptRiCPPError(
				RIE_MISSINGDATA,
				RIE_ERROR,
				renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
				"name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration)
			);
		}
		
		// Create or get a token for the name
		name = renderState()->tokFindCreate(name);

		// Always process the declaration only once where it occurs
		name = renderState()->declare(name, declaration, false);

		// Process
		CRiDeclare r(renderState()->lineNo(), name, declaration);
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return 0;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return 0;
	}

	// Return the token
	return name;
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

RtVoid CBaseRenderer::preBegin(RtString name, const CParameterList &params)
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
		return 0;
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

		CRiBegin r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return 0;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return 0;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'begin'");
	}

	// There is no handle here, the frontend creates the backend
	return 1;
}


RtVoid CBaseRenderer::preEnd(void)
{
	renderState()->contextEnd();
}


RtVoid CBaseRenderer::end(void)
// throw ExceptRiCPPError
{
	if ( !renderState() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, __LINE__, __FILE__, "%s", "State not initialized in end(), break.");
		return;
	}

	ExceptRiCPPError err;
	if ( renderState()->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, "Ended context not at begin-state");
	}

	try {
		CRiEnd r(renderState()->lineNo());
		processRequest(r);
	} catch ( ExceptRiCPPError &e2 ) {
		err = e2;
	} catch ( std::exception &e1 ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at 'end': %s", e1.what());
	} catch ( ... ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at 'end'");
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

RtVoid CBaseRenderer::preFrameBegin(RtInt number)
{
	renderState()->frameBegin(number);
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_FRAME_BEGIN;
	try {

		if ( !preCheck(req) )
			return;

		CRiFrameBegin r(renderState()->lineNo(), number);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preFrameEnd(void)
{
	renderState()->frameEnd();
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_FRAME_END;
	try {

		if ( !preCheck(REQ_FRAME_END) )
			return;
			
		CRiFrameEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preWorldBegin(void)
{
	renderState()->worldBegin();
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_WORLD_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiWorldBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preWorldEnd(void)
{
	renderState()->worldEnd();
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_WORLD_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiWorldEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preAttributeBegin(void)
{
	renderState()->attributeBegin();
}

RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_ATTRIBUTE_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiAttributeBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preAttributeEnd(void)
{
	renderState()->attributeEnd();
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_ATTRIBUTE_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiAttributeEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preTransformBegin(void)
{
	renderState()->transformBegin();
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_TRANSFORM_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiTransformBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preTransformEnd(void)
{
	renderState()->transformEnd();
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_TRANSFORM_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiTransformEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preSolidBegin(RtToken type)
{
	renderState()->solidBegin(type);
}

RtVoid CBaseRenderer::solidBegin(RtToken type)
{
	EnumRequests req = REQ_SOLID_BEGIN;
	try {
		if ( !preCheck(req) )
			return;
		
		RtToken typeTok;

		if ( emptyStr(type) ) {
			typeTok = RI_PRIMITIVE;
		}
		type = renderState()->tokFindCreate(type);
		if ( type != RI_PRIMITIVE && type != RI_INTERSECTION && type != RI_UNION && type != RI_DIFFERENCE ) {
			typeTok = RI_PRIMITIVE;
		} else {
			typeTok = type;
		}

		CRiSolidBegin r(renderState()->lineNo(), typeTok);
		processRequest(r);

		if ( type != typeTok ) {
			throw ExceptRiCPPError(
				RIE_BADSOLID, RIE_ERROR,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Unknown solid operation '%s' at '%s'", noNullStr(type), CRequestInfo::requestName(req));
		}

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preSolidEnd(void)
{
	renderState()->solidEnd();
}

RtVoid CBaseRenderer::solidEnd(void)
{
	EnumRequests req = REQ_SOLID_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiSolidEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preObjectBegin(RtObjectHandle h, RtString name)
{
}

RtObjectHandle CBaseRenderer::objectBegin(RtString name)
{
	EnumRequests req = REQ_OBJECT_BEGIN;
	RtObjectHandle handle = illObjectHandle;
	try {
		if ( !preCheck(req) )
			return illObjectHandle;

		name = renderState()->tokFindCreate(name);
		handle = renderState()->objectBegin(name);

		CRiObjectBegin r(renderState()->lineNo(), handle, name);
		processRequest(r, true);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return handle;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return handle;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return handle;
	}
	
	return handle;
}

RtVoid CBaseRenderer::preObjectEnd(void)
{
}

RtVoid CBaseRenderer::objectEnd(void)
{
	EnumRequests req = REQ_OBJECT_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiObjectEnd r(renderState()->lineNo());
		processRequest(r, true);

		renderState()->objectEnd();
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preObjectInstance(RtObjectHandle handle)
{
}

RtVoid CBaseRenderer::doObjectInstance(RtObjectHandle handle)
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

RtVoid CBaseRenderer::postObjectInstance(RtObjectHandle handle)
{
}

RtVoid CBaseRenderer::objectInstance(RtObjectHandle handle)
{
	EnumRequests req = REQ_OBJECT_INSTANCE;
	try {
		if ( !preCheck(req) )
			return;

		CRiObjectInstance r(renderState()->lineNo(), handle);
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preMotionBegin(RtInt N, RtFloat times[])
{
	renderState()->motionBegin(N, times);
}

RtVoid CBaseRenderer::doMotionBegin(RtInt N, RtFloat times[]) {}
RtVoid CBaseRenderer::postMotionBegin(RtInt N, RtFloat times[]) {}

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[])
{
	EnumRequests req = REQ_MOTION_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiMotionBegin r(renderState()->lineNo(), N, times);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preMotionEnd(void)
{
	renderState()->motionEnd();
}

RtVoid CBaseRenderer::doMotionEnd(void) {}
RtVoid CBaseRenderer::postMotionEnd(void) {}

RtVoid CBaseRenderer::motionEnd(void)
{
	EnumRequests req = REQ_MOTION_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiMotionEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preResourceBegin(void)
{
	renderState()->resourceBegin();
}

RtVoid CBaseRenderer::doResourceBegin(void)
{
}

RtVoid CBaseRenderer::postResourceBegin(void)
{
}

RtVoid CBaseRenderer::resourceBegin(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_RESOURCE_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiResourceBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preResourceEnd(void)
{
	renderState()->resourceEnd();
}

RtVoid CBaseRenderer::doResourceEnd(void)
{
}

RtVoid CBaseRenderer::postResourceEnd(void)
{
}

RtVoid CBaseRenderer::resourceEnd(void)
// throw ExceptRiCPPError
{
	EnumRequests req = REQ_RESOURCE_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiResourceEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preResource(RtToken handle, RtToken type, const CParameterList &params)
{
	renderState()->resource(*this, handle, type, params);
}

RtVoid CBaseRenderer::doResource(RtToken handle, RtToken type, const CParameterList &params)
{
}

RtVoid CBaseRenderer::postResource(RtToken handle, RtToken type, const CParameterList &params)
{
}

RtVoid CBaseRenderer::resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_RESOURCE;
	try {
		if ( !preCheck(req) )
			return;

		handle = renderState()->tokFindCreate(handle);
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_RESOURCE, type, CParameterClasses(), n, tokens, params);

		CRiResource r(renderState()->lineNo(), handle, type, renderState()->curParamList());
		processRequest(r);
		return;
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtArchiveHandle CBaseRenderer::preArchiveBegin(RtToken name, const CParameterList &params)
{
	return renderState()->archiveBegin(name);
}

RtArchiveHandle CBaseRenderer::archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {
	EnumRequests req = REQ_ARCHIVE_BEGIN;
	RtArchiveHandle h = illArchiveHandle;
	try {
		if ( !preCheck(req) )
			return illArchiveHandle;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);

		CRiArchiveBegin r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r, true);
		h = r.handle();
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return h;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return h;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return h;
	}
	
	return h;
}

RtVoid CBaseRenderer::preArchiveEnd(void)
{
	renderState()->archiveEnd();
}

RtVoid CBaseRenderer::archiveEnd(void)
{
	EnumRequests req = REQ_ARCHIVE_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiArchiveEnd r(renderState()->lineNo());
		processRequest(r, true);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


/*
RtVoid CBaseRenderer::preArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
}

RtVoid CBaseRenderer::doArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
	renderState()->archiveInstance(handle, *this, callback, params);
}

RtVoid CBaseRenderer::postArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &paramse)
{
}

RtVoid CBaseRenderer::archiveInstanceV(RtArchiveHandle handle, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	try {
		if ( !preCheck(REQ_ARCHIVE_INSTANCE) )
			return;

		renderState()->parseParameters(CParameterClasses(), n, tokens, params);

		CRiArchiveInstance r(renderState()->lineNo(), handle, callback, renderState()->curParamList());
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveInstance': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveInstance'");
		return;
	}
}
*/


RtVoid CBaseRenderer::preFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	renderState()->options().format(xres, yres, aspect);
}

RtVoid CBaseRenderer::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	EnumRequests req = REQ_FORMAT;
	try {
		if ( !preCheck(req) )
			return;

		CRiFormat r(renderState()->lineNo(), xres, yres, aspect);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preFrameAspectRatio(RtFloat aspect)
{
	renderState()->options().frameAspectRatio(aspect);
}

RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	EnumRequests req = REQ_FRAME_ASPECT_RATIO;
	try {
		if ( !preCheck(req) )
			return;

		CRiFrameAspectRatio r(renderState()->lineNo(), aspect);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	renderState()->options().screenWindow(left, right, bot, top);
}

RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	EnumRequests req = REQ_SCREEN_WINDOW;
	try {
		if ( !preCheck(req) )
			return;

		CRiScreenWindow r(renderState()->lineNo(), left, right, bot, top);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	renderState()->options().cropWindow(xmin, xmax, ymin, ymax);
}

RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	EnumRequests req = REQ_CROP_WINDOW;
	try {
		if ( !preCheck(req) )
			return;

		CRiCropWindow r(renderState()->lineNo(), xmin, xmax, ymin, ymax);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preProjection(RtToken name, const CParameterList &params)
{
	// Sets the state (can throw)
	renderState()->options().projection(name, params);
}

RtVoid CBaseRenderer::projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_PROJECTION;
	try {
		// Check validity and accessibility (if-then-else)
		if ( !preCheck(req) )
			return;

		// Parse parameters, ignore unrecognized ones
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_PROJECTION, name, CParameterClasses(), n, tokens, params);

		// Process command
		CRiProjection r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	// If there were any unrecognized tokens, give an error message
	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preClipping(RtFloat hither, RtFloat yon)
{
	renderState()->options().clipping(hither, yon);
}

RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	EnumRequests req = REQ_CLIPPING;
	try {
		if ( !preCheck(req) )
			return;

		CRiClipping r(renderState()->lineNo(), hither, yon);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	renderState()->options().clippingPlane(x, y, z, nx, ny, nz);
}

RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	EnumRequests req = REQ_CLIPPING_PLANE;
	try {
		if ( !preCheck(req) )
			return;

		CRiClippingPlane r(renderState()->lineNo(), x, y, z, nx, ny, nz);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	renderState()->options().depthOfField(fstop, focallength, focaldistance);
}

RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	EnumRequests req = REQ_DEPTH_OF_FIELD;
	try {
		if ( !preCheck(req) )
			return;

		CRiDepthOfField r(renderState()->lineNo(), fstop, focallength, focaldistance);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preShutter(RtFloat smin, RtFloat smax)
{
	renderState()->options().shutter(smin, smax);
}

RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	EnumRequests req = REQ_SHUTTER;
	try {
		if ( !preCheck(req) )
			return;

		CRiShutter r(renderState()->lineNo(), smin, smax);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::prePixelVariance(RtFloat variation)
{
	renderState()->options().pixelVariance(variation);
}

RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	EnumRequests req = REQ_PIXEL_VARIANCE;
	try {
		if ( !preCheck(req) )
			return;

		CRiPixelVariance r(renderState()->lineNo(), variation);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::prePixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	renderState()->options().pixelSamples(xsamples, ysamples);
}

RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	EnumRequests req = REQ_PIXEL_SAMPLES;
	try {
		if ( !preCheck(req) )
			return;

		CRiPixelSamples r(renderState()->lineNo(), xsamples, ysamples);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::prePixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	renderState()->options().pixelFilter(function, xwidth, ywidth);
}

RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	EnumRequests req = REQ_PIXEL_FILTER;
	try {
		if ( !preCheck(req) )
			return;

		CRiPixelFilter r(renderState()->lineNo(), function, xwidth, ywidth);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preExposure(RtFloat gain, RtFloat gamma)
{
	renderState()->options().exposure(gain, gamma);
}

RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	EnumRequests req = REQ_EXPOSURE;
	try {
		if ( !preCheck(req) )
			return;

		CRiExposure r(renderState()->lineNo(), gain, gamma);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preImager(RtString name, const CParameterList &params)
{
	renderState()->options().imager(name, params);
}

RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_IMAGER;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_IMAGER, name, CParameterClasses(), n, tokens, params);

		CRiImager r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	renderState()->options().quantize(type, one, qmin, qmax, ampl);
}

RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	EnumRequests req = REQ_QUANTIZE;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		CRiQuantize r(renderState()->lineNo(), type, one, qmin, qmax, ampl);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preDisplayChannel(RtString channel, const CParameterList &params)
{
	renderState()->options().displayChannel(renderState()->dict(), renderState()->options().colorDescr(), channel, params);
}

RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DISPLAY_CHANNEL;
	try {
		if ( !preCheck(req) )
			return;

		channel = renderState()->tokFindCreate(channel);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);

		CRiDisplayChannel r(renderState()->lineNo(), channel, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::preDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params)
{
	renderState()->options().display(name, type, mode, params);
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DISPLAY;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		mode = renderState()->tokFindCreate(mode);
		renderState()->parseParameters(RI_DISPLAY, type, CParameterClasses(), n, tokens, params);

		CRiDisplay r(renderState()->lineNo(), name, type, mode, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preHider(RtToken type, const CParameterList &params)
{
	renderState()->options().hider(type, params);
}

RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_HIDER;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_HIDER, type, CParameterClasses(), n, tokens, params);

		CRiHider r(renderState()->lineNo(), type, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	renderState()->options().colorSamples(N, nRGB, RGBn);
}

RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	EnumRequests req = REQ_COLOR_SAMPLES;
	try {
		if ( !preCheck(req) )
			return;

		CRiColorSamples r(renderState()->lineNo(), N, nRGB, RGBn);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(RIE_RANGE, RIE_WARNING, "Illegal values for the parameters of colorSamples(), using identity mapping.");
	}
}


RtVoid CBaseRenderer::preRelativeDetail(RtFloat relativedetail)
{
	renderState()->options().relativeDetail(relativedetail);
}

RtVoid CBaseRenderer::relativeDetail(RtFloat relativedetail)
{
	EnumRequests req = REQ_RELATIVE_DETAIL;
	try {
		if ( !preCheck(req) )
			return;

		CRiRelativeDetail r(renderState()->lineNo(), relativedetail);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preOption(RtToken name, const CParameterList &params)
{
	renderState()->option(name, params);
}

RtVoid CBaseRenderer::optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_OPTION;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_OPTION, name, CParameterClasses(), n, tokens, params);

		CRiOption r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preControl(RtToken name, const CParameterList &params)
{
	renderState()->control(name, params);
}

RtVoid CBaseRenderer::doControl(RtToken name, const CParameterList &params)
{
}

RtVoid CBaseRenderer::controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_CONTROL;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_CONTROL, name, CParameterClasses(), n, tokens, params);

		CRiControl r(renderState()->lineNo(), name, renderState()->curParamList());
		// Do not store controls as macros execute immediatly
		processRequest(r, true);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	CHandle *handle = renderState()->lightSourceHandle(h);
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

	
RtVoid CBaseRenderer::doLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( !renderState()->newLightSource(h, false, name, params) ) {
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
	EnumRequests req = REQ_LIGHT_SOURCE;
	RtLightHandle h = illLightHandle;

	try {
		if ( !preCheck(REQ_LIGHT_SOURCE) )
			return h;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);

		h = renderState()->newLightHandle(name, renderState()->curParamList());

		CRiLightSource r(renderState()->lineNo(), h, name, renderState()->curParamList());
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return h;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return h;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return h;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'lightSourceV'");
	}

	return h;
}


RtVoid CBaseRenderer::preAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( name != RI_NULL ) {
		// Test the handle
		CHandle *handle = renderState()->lightSourceHandle(h);
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

RtVoid CBaseRenderer::doAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	// name == RI_NULL : Area light source was closed
	if ( name != RI_NULL ) {
		// Create an area light source
		if ( !renderState()->newLightSource(h, true, name, params) ) {
			throw ExceptRiCPPError(
				RIE_NOMEM,
				RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"AreaLightSource of \"%s\"",
				noNullStr(name));
		}
		renderState()->startAreaLightSource(h);
	}
}
	
RtLightHandle CBaseRenderer::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_AREA_LIGHT_SOURCE;
	RtLightHandle h = illLightHandle;

	try {
		if ( !preCheck(req) )
			return h;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_AREA_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);

		h = renderState()->newLightHandle(name, renderState()->curParamList());
		
		CRiAreaLightSource r(renderState()->lineNo(), h, name, renderState()->curParamList());
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return h;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return h;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return h;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'lightSourceV'");
	}

	return h;
}

RtVoid CBaseRenderer::preIlluminate(RtLightHandle light, RtBoolean onoff)
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


RtVoid CBaseRenderer::doIlluminate(RtLightHandle light, RtBoolean onoff)
{
	CLightSource *l = renderState()->lightSourceInstance(light);
	renderState()->attributes().illuminate(l, onoff);
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	EnumRequests req = REQ_ILLUMINATE;
	try {
		if ( !preCheck(req) )
			return;

		CRiIlluminate r(renderState()->lineNo(), light, onoff);
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preAttribute(RtToken name, const CParameterList &params)
{
	renderState()->attribute(name, params);
}


RtVoid CBaseRenderer::attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_ATTRIBUTE;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATTRIBUTE, name, CParameterClasses(), n, tokens, params);

		CRiAttribute r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preColor(RtColor Cs)
{
	renderState()->attributes().color(Cs);
}

RtVoid CBaseRenderer::color(RtColor Cs)
{
	EnumRequests req = REQ_COLOR;
	try {
		if ( !preCheck(req) )
			return;

		CRiColor r(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Cs);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preOpacity(RtColor Os)
{
	renderState()->attributes().opacity(Os);
}

RtVoid CBaseRenderer::opacity(RtColor Os)
{
	EnumRequests req = REQ_OPACITY;
	try {
		if ( !preCheck(req) )
			return;

		CRiOpacity r(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Os);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preSurface(RtString name, const CParameterList &params)
{
	renderState()->attributes().surface(name, params);
}

RtVoid CBaseRenderer::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_SURFACE;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_SURFACE, name, CParameterClasses(), n, tokens, params);

		CRiSurface r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preAtmosphere(RtString name, const CParameterList &params)
{
	renderState()->attributes().atmosphere(name, params);
}

RtVoid CBaseRenderer::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_ATMOSPHERE;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATMOSPHERE, name, CParameterClasses(), n, tokens, params);

		CRiAtmosphere r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preInterior(RtString name, const CParameterList &params)
{
	renderState()->attributes().interior(name, params);
}

RtVoid CBaseRenderer::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_INTERIOR;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_INTERIOR, name, CParameterClasses(), n, tokens, params);

		CRiInterior r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preExterior(RtString name, const CParameterList &params)
{
	renderState()->attributes().exterior(name, params);
}

RtVoid CBaseRenderer::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_EXTERIOR;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_EXTERIOR, name, CParameterClasses(), n, tokens, params);

		CRiExterior r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preDisplacement(RtString name, const CParameterList &params)
{
	renderState()->attributes().displacement(name, params);
}

RtVoid CBaseRenderer::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DISPLACEMENT;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DISPLACEMENT, name, CParameterClasses(), n, tokens, params);

		CRiDisplacement r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	renderState()->attributes().textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
}

RtVoid CBaseRenderer::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	EnumRequests req = REQ_TEXTURE_COORDINATES;
	try {
		if ( !preCheck(req) )
			return;

		CRiTextureCoordinates r(renderState()->lineNo(), s1, t1, s2, t2, s3, t3, s4, t4);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preShadingRate(RtFloat size)
{
	renderState()->attributes().shadingRate(size);
}

RtVoid CBaseRenderer::shadingRate(RtFloat size)
{
	EnumRequests req = REQ_SHADING_RATE;
	try {
		if ( !preCheck(req) )
			return;

		CRiShadingRate r(renderState()->lineNo(), size);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preShadingInterpolation(RtToken type)
{
	renderState()->attributes().shadingInterpolation(type);
}

RtVoid CBaseRenderer::shadingInterpolation(RtToken type)
{
	EnumRequests req = REQ_SHADING_INTERPOLATION;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		CRiShadingInterpolation r(renderState()->lineNo(), type);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preMatte(RtBoolean onoff)
{
	renderState()->attributes().matte(onoff);
}

RtVoid CBaseRenderer::matte(RtBoolean onoff)
{
	EnumRequests req = REQ_MATTE;
	try {
		if ( !preCheck(req) )
			return;

		CRiMatte r(renderState()->lineNo(), onoff);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preBound(RtBound aBound)
{
	renderState()->attributes().bound(aBound);
}

RtVoid CBaseRenderer::bound(RtBound aBound)
{
	EnumRequests req = REQ_BOUND;
	try {
		if ( !preCheck(req) )
			return;

		CRiBound r(renderState()->lineNo(), aBound);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preDetail(RtBound aBound)
{
	renderState()->attributes().detail(aBound);
}

RtVoid CBaseRenderer::detail(RtBound aBound)
{
	EnumRequests req = REQ_DETAIL;
	try {
		if ( !preCheck(req) )
			return;

		CRiDetail r(renderState()->lineNo(), aBound);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	renderState()->attributes().detailRange(minvis, lowtran, uptran, maxvis);
}

RtVoid CBaseRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	EnumRequests req = REQ_DETAIL_RANGE;
	try {
		if ( !preCheck(req) )
			return;

		CRiDetailRange r(renderState()->lineNo(), minvis, lowtran, uptran, maxvis);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preGeometricApproximation(RtToken type, RtFloat value)
{
	renderState()->attributes().geometricApproximation(type, value);
}

RtVoid CBaseRenderer::geometricApproximation(RtToken type, RtFloat value)
{
	EnumRequests req = REQ_GEOMETRIC_APPROXIMATION;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);

		CRiGeometricApproximation r(renderState()->lineNo(), type, value);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preGeometricRepresentation(RtToken type)
{
	renderState()->attributes().geometricRepresentation(type);
}

RtVoid CBaseRenderer::geometricRepresentation(RtToken type)
{
	EnumRequests req = REQ_GEOMETRIC_REPRESENTATION;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);

		CRiGeometricRepresentation r(renderState()->lineNo(), type);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preOrientation(RtToken anOrientation)
{
	renderState()->attributes().orientation(anOrientation);
}

RtVoid CBaseRenderer::orientation(RtToken anOrientation)
{
	EnumRequests req = REQ_ORIENTATION;
	try {
		if ( !preCheck(req) )
			return;

		anOrientation = renderState()->tokFindCreate(anOrientation);

		CRiOrientation r(renderState()->lineNo(), anOrientation);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preReverseOrientation(void)
{
	renderState()->attributes().reverseOrientation();
}

RtVoid CBaseRenderer::reverseOrientation(void)
{
	EnumRequests req = REQ_REVERSE_ORIENTATION;
	try {
		if ( !preCheck(req) )
			return;

		CRiReverseOrientation r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preSides(RtInt nsides)
{
	renderState()->attributes().sides(nsides);
}

RtVoid CBaseRenderer::sides(RtInt nsides)
{
	EnumRequests req = REQ_SIDES;
	try {
		if ( !preCheck(req) )
			return;

		CRiSides r(renderState()->lineNo(), nsides);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	renderState()->attributes().basis(ubasis, ustep, vbasis, vstep);
}

RtVoid CBaseRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	EnumRequests req = REQ_BASIS;
	try {
		if ( !preCheck(req) )
			return;

		CRiBasis r(renderState()->lineNo(), ubasis, ustep, vbasis, vstep);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	renderState()->attributes().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
}

RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	EnumRequests req = REQ_TRIM_CURVE;
	try {
		if ( !preCheck(req) )
			return;

		CRiTrimCurve r(renderState()->lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preIdentity(void)
{
	renderState()->curTransform().identity();
}


RtVoid CBaseRenderer::identity(void)
{
	EnumRequests req = REQ_IDENTITY;
	try {
		if ( !preCheck(req) )
			return;

		CRiIdentity r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preTransform(RtMatrix aTransform)
{
	renderState()->curTransform().transform(aTransform);
}


RtVoid CBaseRenderer::transform(RtMatrix aTransform)
{
	EnumRequests req = REQ_TRANSFORM;
	try {
		if ( !preCheck(req) )
			return;

		CRiTransform r(renderState()->lineNo(), aTransform);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preConcatTransform(RtMatrix aTransform)
{
	renderState()->curTransform().concatTransform(aTransform);
}


RtVoid CBaseRenderer::concatTransform(RtMatrix aTransform)
{
	EnumRequests req = REQ_CONCAT_TRANSFORM;
	try {
		if ( !preCheck(req) )
			return;

		CRiConcatTransform r(renderState()->lineNo(), aTransform);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::prePerspective(RtFloat fov)
{
	renderState()->curTransform().perspective(fov);
}


RtVoid CBaseRenderer::perspective(RtFloat fov)
{
	EnumRequests req = REQ_PERSPECTIVE;
	try {
		if ( !preCheck(req) )
			return;

		CRiPerspective r(renderState()->lineNo(), fov);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().translate(dx, dy, dz);
}


RtVoid CBaseRenderer::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	EnumRequests req = REQ_TRANSLATE;
	try {
		if ( !preCheck(req) )
			return;

		CRiTranslate r(renderState()->lineNo(), dx, dy, dz);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().rotate(angle, dx, dy, dz);
}


RtVoid CBaseRenderer::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	EnumRequests req = REQ_ROTATE;
	try {
		if ( !preCheck(req) )
			return;

		CRiRotate r(renderState()->lineNo(), angle, dx, dy, dz);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preScale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	renderState()->curTransform().scale(dx, dy, dz);
}


RtVoid CBaseRenderer::scale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	EnumRequests req = REQ_SCALE;
	try {
		if ( !preCheck(req) )
			return;

		CRiScale r(renderState()->lineNo(), dx, dy, dz);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	renderState()->curTransform().skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
}


RtVoid CBaseRenderer::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	EnumRequests req = REQ_SKEW;
	try {
		if ( !preCheck(req) )
			return;

		CRiSkew r(renderState()->lineNo(), angle, dx1, dy1, dz1, dx2, dy2, dz2);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::doDeformation(RtString name, const CParameterList &params)
{
	ricppErrHandler().handleError(
		RIE_UNIMPLEMENT, RIE_WARNING,
		renderState()->printLineNo(__LINE__),
		renderState()->printName(__FILE__),
		"RiDeformation is not supported, name = '%s'",  noNullStr(name));
}

RtVoid CBaseRenderer::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DEFORMATION;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DEFORMATION, name, CParameterClasses(), n, tokens, params);

		CRiDeformation r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preScopedCoordinateSystem(RtToken space)
{
	renderState()->scopedCoordinateSystem(space);
}

RtVoid CBaseRenderer::scopedCoordinateSystem(RtToken space)
{
	EnumRequests req = REQ_SCOPED_COORDINATE_SYSTEM;
	try {
		if ( !preCheck(req) )
			return;

		space = renderState()->tokFindCreate(space);
		CRiScopedCoordinateSystem r(renderState()->lineNo(), space);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preCoordinateSystem(RtToken space)
{
	renderState()->coordinateSystem(space);
}

RtVoid CBaseRenderer::coordinateSystem(RtToken space)
{
	EnumRequests req = REQ_COORDINATE_SYSTEM;
	try {
		if ( !preCheck(req) )
			return;

		space = renderState()->tokFindCreate(space);
		CRiCoordinateSystem r(renderState()->lineNo(), space);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preCoordSysTransform(RtToken space)
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
	EnumRequests req = REQ_COORD_SYS_TRANSFORM;
	try {
		if ( !preCheck(req) )
			return;

		space = renderState()->tokFindCreate(space);
		CRiCoordSysTransform r(renderState()->lineNo(), space);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
{
	if ( npoints <= 0 ) {
		return;
	}
	const CTransformation *from = renderState()->findTransform(fromspace);
	const CTransformation *to   = renderState()->findTransform(tospace);
	if ( from == 0 || to == 0 ) {
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "transformPoints(%s (%s found), %s (%s found)), spaces not found", noNullStr(fromspace), from ? "was" : "not", noNullStr(tospace), to ? "was" : "not");
		return;
	}

	// 2Do
}

RtPoint *CBaseRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	EnumRequests req = REQ_TRANSFORM_POINTS;
	try {
		if ( !preCheck(req) )
			return 0;

		fromspace = renderState()->tokFindCreate(fromspace);
		tospace = renderState()->tokFindCreate(tospace);
		CRiTransformPoints r(renderState()->lineNo(), fromspace, tospace, npoints, points);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return 0;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return 0;
	}

	return &points[0];
}

RtVoid CBaseRenderer::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_POLYGON;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CPolygonClasses(nvertices), n, tokens, params);

		CRiPolygon r(renderState()->lineNo(), nvertices, renderState()->curParamList());
		processRequest(r);

		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_GENERAL_POLYGON;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CGeneralPolygonClasses(nloops, nverts), n, tokens, params);

		CRiGeneralPolygon r(renderState()->lineNo(), nloops, nverts, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_POINTS_POLYGONS;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CPointsPolygonsClasses(npolys, nverts, verts), n, tokens, params);

		CRiPointsPolygons r(renderState()->lineNo(), npolys, nverts, verts, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_POINTS_GENERAL_POLYGONS;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CPointsGeneralPolygonsClasses(npolys, nloops, nverts, verts), n, tokens, params);

		CRiPointsGeneralPolygons r(renderState()->lineNo(), npolys, nloops, nverts, verts, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_PATCH;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CPatchClasses(type), n, tokens, params);

		CRiPatch r(renderState()->lineNo(), type, renderState()->curParamList());
		processRequest(r);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_PATCH_MESH;
	try {
		if ( !preCheck(req) )
			return;

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

		CRiPatchMesh r(renderState()->lineNo(), ustep, vstep, type, nu, uwrap, nv, vwrap, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_NU_PATCH;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CNuPatchClasses(nu, uorder, nv, vorder), n, tokens, params);

		CRiNuPatch r(renderState()->lineNo(), nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_SUBDIVISION_MESH;
	try {
		if ( !preCheck(req) )
			return;

		scheme = renderState()->tokFindCreate(scheme);

		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);

		CRiSubdivisionMesh r(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_HIERARCHICAL_SUBDIVISION_MESH;
	try {
		if ( !preCheck(req) )
			return;

		scheme = renderState()->tokFindCreate(scheme);

		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);

		CRiHierarchicalSubdivisionMesh r(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_SPHERE;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiSphere r(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_CONE;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiCone r(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_CYLINDER;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiCylinder r(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_HYPERBOLOID;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiHyperboloid r(renderState()->lineNo(), point1, point2, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_PARABOLOID;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiParaboloid r(renderState()->lineNo(), rmax, zmin, zmax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DISK;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiDisk r(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_TORUS;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);

		CRiTorus r(renderState()->lineNo(), majorrad, minorrad, phimin, phimax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_POINTS;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CPointsClasses(npts), n, tokens, params);

		CRiPoints r(renderState()->lineNo(), npts, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_CURVES;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);

		renderState()->parseParameters(CCurvesClasses(type, ncurves, nverts, wrap, renderState()->attributes().vStep()), n, tokens, params);

		CRiCurves r(renderState()->lineNo(), renderState()->attributes().vStep(), type, ncurves, nverts, wrap, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_BLOBBY;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CBlobbyClasses(nleaf), n, tokens, params);

		CRiBlobby r(renderState()->lineNo(), nleaf, ncode, code, nflt, flt, nstr, str, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
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

RtVoid CBaseRenderer::doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
{
	subdivfunc(m_parserCallback->frontend(), data, RI_INFINITY);
	if ( freefunc )
		(*freefunc)(data);
}

RtVoid CBaseRenderer::procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
{
	EnumRequests req = REQ_PROCEDURAL;
	try {
		if ( !preCheck(req) )
			return;

		CRiProcedural r(renderState()->lineNo(), data, bound, subdivfunc, freefunc);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}

RtVoid CBaseRenderer::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_GEOMETRY;
	try {
		if ( !preCheck(req) )
			return;

		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);

		CRiGeometry r(renderState()->lineNo(), type, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_TEXTURE;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_TEXTURE, CParameterClasses(), n, tokens, params);

		CRiMakeTexture r(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_BUMP;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_BUMP, CParameterClasses(), n, tokens, params);

		CRiMakeBump r(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_LAT_LONG_ENVIRONMENT;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_LAT_LONG_ENVIRONMENT, CParameterClasses(), n, tokens, params);

		CRiMakeLatLongEnvironment r(renderState()->lineNo(), pic, tex, filterfunc, swidth, twidth, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_CUBE_FACE_ENVIRONMENT;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_CUBE_FACE_ENVIRONMENT, CParameterClasses(), n, tokens, params);

		CRiMakeCubeFaceEnvironment r(renderState()->lineNo(), px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_SHADOW;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_SHADOW, CParameterClasses(), n, tokens, params);

		CRiMakeShadow r(renderState()->lineNo(), pic, tex, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_MAKE_BRICK_MAP;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(RI_TEXTURE, RI_BRICK_MAP, CParameterClasses(), n, tokens, params);

		CRiMakeBrickMap r(renderState()->lineNo(), nNames, ptcnames, bkmname, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::archiveRecordV(RtToken type, RtString line)
{
	EnumRequests req = REQ_ARCHIVE_RECORD;
	try {
		if ( !preCheck(req) )
			return;

		RtToken typeTok;

		if ( emptyStr(type) ) {
			typeTok = RI_COMMENT;
		}
		type = renderState()->tokFindCreate(type);

		CRiArchiveRecord r(renderState()->lineNo(), type, line);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::processReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	if ( !emptyStr(name) ) {
		// 1. Look for archive in stored archives
		RtArchiveHandle handle = renderState()->storedArchiveName(name);
		if ( handle ) {
			renderState()->archiveInstance(handle, *this, callback, params);
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
	renderState()->processReadArchive(name, *m_parserCallback, callback, params);
}

RtVoid CBaseRenderer::doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	processReadArchive(name, callback, params);
}

RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_READ_ARCHIVE;
	try {
		if ( !preCheck(req) )
			return;

		renderState()->parseParameters(CParameterClasses(), n, tokens, params);

		CRiReadArchive r(renderState()->lineNo(), name, callback, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in %s'",  CRequestInfo::requestName(req));
	}
}


RtVoid CBaseRenderer::preIfBegin(RtString expr)
{
	renderState()->ifBegin(expr);
}


RtVoid CBaseRenderer::ifBegin(RtString expr)
{
	EnumRequests req = REQ_IF_BEGIN;
	try {
		if ( !preCheck(req) )
			return;

		CRiIfBegin r(renderState()->lineNo(), expr);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preElseIfBegin(RtString expr)
{
	renderState()->elseIfBegin(expr);
}


RtVoid CBaseRenderer::elseIfBegin(RtString expr)
{
	EnumRequests req = REQ_ELSE_IF;
	try {
		if ( !preCheck(req) )
			return;

		CRiElseIfBegin r(renderState()->lineNo(), expr);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preElseBegin(void)
{
	renderState()->elseBegin();
}


RtVoid CBaseRenderer::elseBegin(void)
{
	EnumRequests req = REQ_ELSE;
	try {
		if ( !preCheck(req) )
			return;

		CRiElseBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}


RtVoid CBaseRenderer::preIfEnd(void)
{
	renderState()->ifEnd();
}

RtVoid CBaseRenderer::ifEnd(void)
{
	EnumRequests req = REQ_IF_END;
	try {
		if ( !preCheck(req) )
			return;

		CRiIfEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return;
	}
}
