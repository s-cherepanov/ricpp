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
#include "ricpp/ricontext/rimacro.h"

#include <cassert>

using namespace RiCPP;

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_protocolHandler(0)
{
	m_renderState = 0;
	m_modeStack = 0;
	m_optionsFactory = 0;
	m_attributesFactory = 0;
	m_lightSourceFactory = 0;
	// m_macroFactory = 0;
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

	if ( m_lightSourceFactory ) {
		deleteLightSourceFactory(m_lightSourceFactory);
	}
	m_lightSourceFactory = 0;

	if ( m_modeStack ) {
		deleteModeStack(m_modeStack);
	}
	m_modeStack = 0;

	/*
	if ( m_macroFactory ) {
		deleteMacroFactory(m_macroFactory);
	}
	m_macroFactory = 0;
	*/

	if ( m_attributesResourceFactory )
	{
		deleteAttributesResourceFactory(m_attributesResourceFactory);
	}
	m_attributesResourceFactory = 0;
}

/*
CRManInterfaceFactory *CBaseRenderer::getNewMacroFactory()
{
	return new CRManInterfaceFactory;
}
void CBaseRenderer::deleteMacroFactory(CRManInterfaceFactory *ptr)
{
	if ( ptr )
		delete ptr;
}
*/

void CBaseRenderer::registerResources()
{
	if ( !m_attributesResourceFactory ) {
		m_attributesResourceFactory = getNewAttributesResourceFactory();
		if ( !m_attributesResourceFactory ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a resource factory for attribue resources.");
			return;
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
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "Render state already initialized in %s()", "initRenderState");
	}

	if ( !m_modeStack ) {
		try {
			m_modeStack = getNewModeStack();
		} catch (ExceptRiCPPError &err) {
			ricppErrHandler().handleError(err);
			return;
		}

		if ( !m_modeStack ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
			return;
		}
	}

	if ( !m_optionsFactory ) {
		try {
			m_optionsFactory = getNewOptionsFactory();
		} catch (ExceptRiCPPError &err) {
			ricppErrHandler().handleError(err);
			return;
		}

		if ( !m_optionsFactory ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an options factory");
			return;
		}
	}

	if ( !m_attributesFactory ) {
		try {
			m_attributesFactory = getNewAttributesFactory();
		} catch (ExceptRiCPPError &err) {
			ricppErrHandler().handleError(err);
			return;
		}

		if ( !m_attributesFactory ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
			return;
		}
	}

	if ( !m_lightSourceFactory ) {
		try {
			m_lightSourceFactory = getNewLightSourceFactory();
		} catch (ExceptRiCPPError &err) {
			ricppErrHandler().handleError(err);
			return;
		}

		if ( !m_lightSourceFactory ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an light source factory");
			return;
		}
	}

	/*
	if ( !m_macroFactory ) {
		try {
			m_macroFactory = getNewMacroFactory();
		} catch (ExceptRiCPPError &err) {
			ricppErrHandler().handleError(err);
			return;
		}

		if ( !m_macroFactory ) {
			ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an macro factory");
			return;
		}
	}
	*/

	try {
		m_renderState = new CRenderState(*m_modeStack, *m_optionsFactory, *m_attributesFactory, *m_lightSourceFactory); // , *m_macroFactory);
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}

	registerResources();
}

bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !renderState() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "State not initialized in %s()", CRequestInfo::requestName(req));
	}

	if ( !renderState()->validRequest(req) ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s", CRequestInfo::requestName(req));
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

	/*
	if ( !renderState()->hasMacroFactory() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - macro factory not available.", CRequestInfo::requestName(req));
	}
	*/

	return !renderState()->reject();
}

void CBaseRenderer::processRequest(CRManInterfaceCall &aRequest)
{
	aRequest.preProcess(*this);

	if ( renderState()->curMacro() && !aRequest.isMacroDefinition() ) {
		if ( !renderState()->curMacro()->stopInsertion() ) {
			renderState()->curMacro()->add(aRequest.duplicate());
		}
	} else {
		if ( renderState()->executeConditionial() ) {
			aRequest.doProcess(*this);
		}
	}

	aRequest.postProcess(*this);
}

void CBaseRenderer::replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb)
{
	renderState()->lineNo(aRequest.lineNo());
	aRequest.preProcess(*this, cb);
	if ( renderState()->executeConditionial() ) {
		aRequest.doProcess(*this, cb);
	}
	aRequest.postProcess(*this, cb);
}

/** @brief Create new entry in dectaration list
 */
void CBaseRenderer::preDeclare(RtToken name, RtString declaration, bool isDefault)
{
	// if no declaration only tokenize the name
	if ( !emptyStr(declaration) ) {
	
		CDeclaration *d = new CDeclaration(name, declaration, renderState()->options().colorDescr(), renderState()->tokenMap(), isDefault);		
		if ( !d )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Declaration of \"%s\": \"%s\"", name, declaration);

		renderState()->declAdd(d);
	}
}

RtToken CBaseRenderer::declare(RtString name, RtString declaration)
{
	try {

		if ( !preCheck(REQ_DECLARE) )
			return RI_NULL;

		if ( emptyStr(name) ) {
			throw ExceptRiCPPError(
				RIE_MISSINGDATA,
				RIE_ERROR,
				renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
				"name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration)
			);
		}
		name = renderState()->tokFindCreate(name);

		CRiDeclare r(renderState()->lineNo(), name, declaration);
		processRequest(r);

		return name;

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return 0;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'declare': %s", e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'declare'");
		return 0;
	}
}


void CBaseRenderer::defaultDeclarations()
{
	// Default declarations
	preDeclare(RI_FLATNESS, "float", true);
	preDeclare(RI_FOV, "float", true);

	preDeclare(RI_INTENSITY, "float", true);
	preDeclare(RI_LIGHTCOLOR, "color", true);
	preDeclare(RI_FROM, "point", true);
	preDeclare(RI_TO, "point", true);
	preDeclare(RI_CONEANGLE, "float", true);
	preDeclare(RI_CONEDELTAANGLE, "float", true);
	preDeclare(RI_BEAMDISTRIBUTION, "float", true);

	preDeclare(RI_KA, "float", true);
	preDeclare(RI_KD, "float", true);
	preDeclare(RI_KS, "float", true);
	preDeclare(RI_ROUGHNESS, "float", true);
	preDeclare(RI_KR, "float", true);
	preDeclare(RI_TEXTURENAME, "string", true);
	preDeclare(RI_SPECULARCOLOR, "color", true);
	preDeclare(RI_MINDISTANCE, "float", true);
	preDeclare(RI_MAXDISTANCE, "float", true);
	preDeclare(RI_BACKGROUND, "color", true);
	preDeclare(RI_DISTANCE, "float", true);
	preDeclare(RI_AMPLITUDE, "float", true);

	preDeclare(RI_P, "vertex point", true);
	preDeclare(RI_PZ, "vertex float", true);
	preDeclare(RI_PW, "vertex hpoint", true);
	preDeclare(RI_N,  "varying point", true);  // Normal
	preDeclare(RI_NP, "uniform point", true);
	preDeclare(RI_CS, "varying color", true);  // Color
	preDeclare(RI_OS, "varying color", true);  // Opacity
	preDeclare(RI_S,  "varying float", true);  // Texture coordinates
	preDeclare(RI_T,  "varying float", true);
	preDeclare(RI_ST, "varying float[2]", true);

	preDeclare(RI_ORIGIN, "constant integer[2]", true);   // Origin of the display

	preDeclare(RI_NAME, "constant string", true);
	preDeclare(RI_WIDTH, "varying float", true);
	preDeclare(RI_CONSTANTWIDTH, "float", true);

	preDeclare(RI_FILE, "string", true);
}

RtVoid CBaseRenderer::preBegin(RtString name, const CParameterList &params)
{
}

RtContextHandle CBaseRenderer::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( renderState() ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return 0;
	}

	try {
		// Init

		initRenderState();

		renderState()->contextBegin();

		defaultDeclarations();

		// Handle the parameters
		renderState()->parseParameters(CValueCounts(), n, tokens, params);
		preBegin(name, renderState()->curParamList());
		doBegin(name, renderState()->curParamList());
		postBegin(name, renderState()->curParamList());

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return 0;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at 'begin': %s", e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "Unknown error at 'begin'");
		return 0;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'begin'");
	}

	return 0;
}

RtVoid CBaseRenderer::preEnd(void)
{
	if ( renderState()->areaLightSourceHandle() != illLightHandle &&
	     renderState()->areaLightSourceDepth() == renderState()->modesSize() )
	{
		doAreaLightSource(renderState()->areaLightSourceHandle(), RI_NULL, CParameterList());
		renderState()->endAreaLightSource();
	}
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
		preEnd();
		doEnd();
		postEnd();
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
	try {

		if ( !preCheck(REQ_FRAME_BEGIN) )
			return;

		CRiFrameBegin r(renderState()->lineNo(), number);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'frameBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'frameBegin'");
		return;
	}
}


RtVoid CBaseRenderer::preFrameEnd(void)
{
	if ( renderState()->areaLightSourceHandle() != illLightHandle &&
	     renderState()->areaLightSourceDepth() == renderState()->modesSize() )
	{
		doAreaLightSource(renderState()->areaLightSourceHandle(), RI_NULL, CParameterList());
		renderState()->endAreaLightSource();
	}

	renderState()->frameEnd();
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	try {

		if ( !preCheck(REQ_FRAME_END) )
			return;
		CRiFrameEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'frameEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'frameEnd'");
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
	try {
		if ( !preCheck(REQ_WORLD_BEGIN) )
			return;

		CRiWorldBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'worldBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'worldBegin'");
		return;
	}
}

RtVoid CBaseRenderer::preWorldEnd(void)
{
	if ( renderState()->areaLightSourceHandle() != illLightHandle &&
	     renderState()->areaLightSourceDepth() == renderState()->modesSize() )
	{
		doAreaLightSource(renderState()->areaLightSourceHandle(), RI_NULL, CParameterList());
		renderState()->endAreaLightSource();
	}

	renderState()->worldEnd();
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	try {
		if ( !preCheck(REQ_WORLD_END) )
			return;

		CRiWorldEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'worldEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'worldEnd'");
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
	try {
		if ( !preCheck(REQ_ATTRIBUTE_BEGIN) )
			return;

		CRiAttributeBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'attributeBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'attributeBegin'");
		return;
	}
}


RtVoid CBaseRenderer::preAttributeEnd(void)
{
	if ( renderState()->areaLightSourceHandle() != illLightHandle &&
	     renderState()->areaLightSourceDepth() == renderState()->modesSize() )
	{
		doAreaLightSource(renderState()->areaLightSourceHandle(), RI_NULL, CParameterList());
		renderState()->endAreaLightSource();
	}

	renderState()->attributeEnd();
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	try {
		if ( !preCheck(REQ_ATTRIBUTE_END) )
			return;

		CRiAttributeEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'attributeEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'attributeEnd'");
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
	try {
		if ( !preCheck(REQ_TRANSFORM_BEGIN) )
			return;

		CRiTransformBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'transformBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'transformBegin'");
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
	try {
		if ( !preCheck(REQ_TRANSFORM_END) )
			return;

		CRiTransformEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'transformEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'transformEnd'");
		return;
	}
}


RtVoid CBaseRenderer::preSolidBegin(RtToken type)
{
	renderState()->solidBegin(type);
}

RtVoid CBaseRenderer::solidBegin(RtToken type)
{
	try {
		if ( !preCheck(REQ_SOLID_BEGIN) )
			return;
		
		RtToken typeTok;

		if ( emptyStr(type) ) {
			typeTok = RI_PRIMITIVE;
		}
		type = renderState()->tokFind(type);
		if ( type != RI_PRIMITIVE && type != RI_INTERSECTION && type != RI_UNION && type != RI_DIFFERENCE ) {
			typeTok = RI_PRIMITIVE;
		} else {
			typeTok = type;
		}

		CRiSolidBegin r(renderState()->lineNo(), typeTok);
		processRequest(r);

		if ( type != typeTok ) {
			throw ExceptRiCPPError(RIE_BADSOLID, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown solid operation '%s' at 'solidBegin'", noNullStr(type));
		}

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'solidBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'solidBegin'");
		return;
	}
}


RtVoid CBaseRenderer::preSolidEnd(void)
{
	renderState()->solidEnd();
}

RtVoid CBaseRenderer::solidEnd(void)
{
	try {
		if ( !preCheck(REQ_SOLID_END) )
			return;

		CRiSolidEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'solidEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'solidEnd'");
		return;
	}
}


RtObjectHandle CBaseRenderer::preObjectBegin(void)
{
	return renderState()->objectBegin();
}

RtObjectHandle CBaseRenderer::objectBegin(void) {
	try {
		if ( !preCheck(REQ_OBJECT_BEGIN) )
			return illObjectHandle;

		CRiObjectBegin r(renderState()->lineNo());
		processRequest(r);
		return r.handle();
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return illObjectHandle;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectBegin': %s", e1.what());
		return illObjectHandle;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectBegin'");
		return illObjectHandle;
	}
	
	return illObjectHandle;
}

RtVoid CBaseRenderer::preObjectEnd(void)
{
	renderState()->objectEnd();
}

RtVoid CBaseRenderer::objectEnd(void)
{
	try {
		if ( !preCheck(REQ_OBJECT_END) )
			return;

		CRiObjectEnd r(renderState()->lineNo());
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectEnd'");
		return;
	}
}

RtVoid CBaseRenderer::preObjectInstance(RtObjectHandle handle)
{
}

RtVoid CBaseRenderer::doObjectInstance(RtObjectHandle handle)
{
	CRiObjectMacro *m = renderState()->objectInstance(handle);
	renderState()->curReplay(m);
	if ( m ) {
		if ( m->isClosed() ) {
			m->replay(*this, 0);
		} else {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Object instance used before it's ObjectEnd().");
		}
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Object instance not found.");
	}
}

RtVoid CBaseRenderer::postObjectInstance(RtObjectHandle handle)
{
}

RtVoid CBaseRenderer::objectInstance(RtObjectHandle handle)
{
	CRiMacro *m = renderState()->curReplay();

	try {
		if ( !preCheck(REQ_OBJECT_INSTANCE) )
			return;

		CRiObjectInstance r(renderState()->lineNo(), handle);
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectInstance': %s", e1.what());
		return;
	} catch ( ... ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'objectInstance'");
		return;
	}

	renderState()->curReplay(m);
}

RtVoid CBaseRenderer::preMotionBegin(RtInt N, RtFloat times[])
{
	renderState()->motionBegin(N, times);
}

RtVoid CBaseRenderer::doMotionBegin(RtInt N, RtFloat times[]) {}
RtVoid CBaseRenderer::postMotionBegin(RtInt N, RtFloat times[]) {}

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[])
{
	try {
		if ( !preCheck(REQ_MOTION_BEGIN) )
			return;

		CRiMotionBegin r(renderState()->lineNo(), N, times);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'motionBeginV': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'motionBeginV'");
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
	try {
		if ( !preCheck(REQ_MOTION_END) )
			return;

		CRiMotionEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'motionEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'motionEnd'");
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
	try {
		if ( !preCheck(REQ_RESOURCE_BEGIN) )
			return;

		CRiResourceBegin r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceBegin': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceBegin'");
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
	try {
		if ( !preCheck(REQ_RESOURCE_END) )
			return;

		CRiResourceEnd r(renderState()->lineNo());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceEnd'");
		return;
	}
}

RtVoid CBaseRenderer::preResource(RtString handle, RtString type, const CParameterList &params)
{
}

RtVoid CBaseRenderer::doResource(RtString handle, RtString type, const CParameterList &params)
{
	renderState()->resource(*this, handle, type, params);
}

RtVoid CBaseRenderer::postResource(RtString handle, RtString type, const CParameterList &params)
{
}

RtVoid CBaseRenderer::resourceV(RtString handle, RtString type, RtInt n, RtToken tokens[], RtPointer params[])
{
	try {
		if ( !preCheck(REQ_RESOURCE) )
			return;

		renderState()->parseParameters(CValueCounts(), n, tokens, params);

		CRiResource r(renderState()->lineNo(), handle, type, renderState()->curParamList());
		processRequest(r);
		return;
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceV': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'resourceV'");
		return;
	}
}

RtArchiveHandle CBaseRenderer::preArchiveBegin(RtToken name, const CParameterList &params)
{
	return renderState()->archiveBegin(name);
}

RtArchiveHandle CBaseRenderer::archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	try {
		if ( !preCheck(REQ_ARCHIVE_BEGIN) )
			return illArchiveHandle;

		renderState()->parseParameters(CValueCounts(), n, tokens, params);

		CRiArchiveBegin r(renderState()->lineNo(), name, renderState()->curParamList());
		processRequest(r);
		return r.handle();
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return illArchiveHandle;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveBegin': %s", e1.what());
		return illArchiveHandle;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveBegin'");
		return illArchiveHandle;
	}
	
	return illArchiveHandle;
}

RtVoid CBaseRenderer::preArchiveEnd(void)
{
	renderState()->archiveEnd();
}

RtVoid CBaseRenderer::archiveEnd(void)
{
	try {
		if ( !preCheck(REQ_ARCHIVE_END) )
			return;

		CRiArchiveEnd r(renderState()->lineNo());
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveEnd': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveEnd'");
		return;
	}
}

RtVoid CBaseRenderer::preArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
}

RtVoid CBaseRenderer::doArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
	CRiArchiveMacro *m = renderState()->archiveInstance(handle);
	renderState()->curReplay(m);
	if ( m ) {
		if ( m->isClosed() ) {
			m->replay(*this, callback);
		} else {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Object instance used before it's ArchiveEnd().");
		}
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Archive instance not found.");
	}
}

RtVoid CBaseRenderer::postArchiveInstance(RtArchiveHandle handl, const IArchiveCallback *callback, const CParameterList &paramse)
{
}

RtVoid CBaseRenderer::archiveInstanceV(RtArchiveHandle handle, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	CRiMacro *m = renderState()->curReplay();

	try {
		if ( !preCheck(REQ_ARCHIVE_INSTANCE) )
			return;

		CRiArchiveInstance r(renderState()->lineNo(), handle, callback);
		processRequest(r);
		
	} catch ( ExceptRiCPPError &e2 ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveInstance': %s", e1.what());
		return;
	} catch ( ... ) {
		renderState()->curReplay(m);
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'archiveInstance'");
		return;
	}

	renderState()->curReplay(m);
}



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

RtVoid CBaseRenderer::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_PROJECTION;
	try {
		// Check validity and accessibility (if-then-else)
		if ( !preCheck(req) )
			return;

		// Parse parameters, ignore unrecognized ones
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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

		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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


RtVoid CBaseRenderer::preColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	renderState()->options().colorSamples(N, nRGB, RGBn);
}

RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
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


RtVoid CBaseRenderer::preOption(RtString name, const CParameterList &params)
{
	renderState()->options().set(name, params);
}

RtVoid CBaseRenderer::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_OPTION;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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


RtLightHandle CBaseRenderer::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;

	try {
		if ( !preCheck(REQ_LIGHT_SOURCE) )
			return h;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

		if ( renderState()->curMacro() ) {
				if ( !renderState()->curMacro()->stopInsertion() ) {
					CRiLightSource *m = new CRiLightSource(*renderState(), name, n, tokens, params);
					if ( !m )
						throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiLightSource", __LINE__, __FILE__));
					renderState()->curMacro()->add(m);
					return m->handleIdx();
				}
		} else {
			if ( renderState()->executeConditionial() ) {
				h = renderState()->lights().lightSource(renderState()->dict(), renderState()->options().colorDescr(),
					true, !renderState()->inWorldBlock(), false, name, n, tokens, params);

				if ( renderState()->updateStateOnly() )
					return h;

				doLightSource(h, name, renderState()->curParamList());
			}
		}

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'declare': %s", e1.what());
		return 0;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'declare'");
		return 0;
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'lightSourceV'");
	}

	return h;
}


RtLightHandle CBaseRenderer::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_AREA_LIGHT_SOURCE) )
		return illLightHandle;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	RtLightHandle h = renderState()->areaLightSourceHandle();

	if ( h != illLightHandle ) {
		renderState()->endAreaLightSource();
	}

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiAreaLightSource *m = new CRiAreaLightSource(*renderState(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAreaLightSource", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
				h = m->handleIdx();
				renderState()->startAreaLightSource(h);

				if ( n != renderState()->numTokens() ) {
					ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
				}
				return h;
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

		if ( n != renderState()->numTokens() ) {
			ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
		}
		return illLightHandle;
	}

	try {
		if ( !emptyStr(name) ) {
			h = renderState()->lights().lightSource(renderState()->dict(), renderState()->options().colorDescr(),
				true, !renderState()->inWorldBlock(), true, name, n, tokens, params);
		}
		renderState()->startAreaLightSource(h);

		if ( renderState()->updateStateOnly() )
			return h;

		doAreaLightSource(h, name, renderState()->curParamList());
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
	}

	return h;
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	if ( !preCheck(REQ_ILLUMINATE) )
		return;

	renderState()->attributes().illuminate(light, onoff);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiIlluminate *m = new CRiIlluminate(renderState()->lineNo(), light, onoff);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiIlluminate", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doIlluminate(light, onoff);
	}
}


RtVoid CBaseRenderer::preAttribute(RtString name, const CParameterList &params)
{
	renderState()->attributes().set(name, params);
}

RtVoid CBaseRenderer::attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_ATTRIBUTE;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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


RtVoid CBaseRenderer::preTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	renderState()->attributes().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
}

RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
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


RtVoid CBaseRenderer::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	EnumRequests req = REQ_DEFORMATION;
	try {
		if ( !preCheck(req) )
			return;

		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

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


RtPoint *CBaseRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	EnumRequests req = REQ_TRANSFORM_POINTS;
	try {
		if ( !preCheck(req) )
			return &points[0];

		fromspace = renderState()->tokFindCreate(fromspace);
		tospace = renderState()->tokFindCreate(tospace);
		CRiTransformPoints r(renderState()->lineNo(), fromspace, tospace, npoints, points);
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return &points[0];
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
		return &points[0];
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error at '%s'",  CRequestInfo::requestName(req));
		return &points[0];
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


RtVoid CBaseRenderer::generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[])
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


RtVoid CBaseRenderer::pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
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


RtVoid CBaseRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[])
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


RtVoid CBaseRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
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


RtVoid CBaseRenderer::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	try {

		if ( !preCheck(REQ_SPHERE) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		CRiSphere r(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'sphere': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'sphere'");
		return;
	}
}

RtVoid CBaseRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	try {

		if ( !preCheck(REQ_TORUS) )
			return;

		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		CRiTorus r(renderState()->lineNo(), majorrad, minorrad, phimin, phimax, thetamax, renderState()->curParamList());
		processRequest(r);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'torus': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'torus'");
		return;
	}
}

RtVoid CBaseRenderer::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

RtVoid CBaseRenderer::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::makeBrickMapV(RtInt nNames, RtString *ptcnames, RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::archiveRecordV(RtToken type, RtString line) {}

RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_READ_ARCHIVE) )
		return;

	RtToken handle = renderState()->storedArchiveName(name);
	if ( handle != RI_NULL ) {
		archiveInstanceV(handle, callback, n, tokens, params);
		return;
	}

	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	preReadArchive(name, callback, renderState()->curParamList());
	doReadArchive(name, callback, renderState()->curParamList());
	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'readArchiveV'");
	}
}

RtVoid CBaseRenderer::doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
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
