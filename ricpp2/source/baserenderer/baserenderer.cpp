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
#include "ricpp/renderstate/rimacro.h"

#include <cassert>

using namespace RiCPP;

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_protocolHandler(0)
{
}

CBaseRenderer::~CBaseRenderer()
{
	if ( m_renderState )
		delete m_renderState;
}

CRManInterfaceFactory *CBaseRenderer::getNewMacroFactory()
{
	return new CRManInterfaceFactory;
}

void CBaseRenderer::registerResources()
{
	renderState()->registerResourceFactory(getNewAttributesResourceFactory());
}

CAttributesResourceFactory *CBaseRenderer::getNewAttributesResourceFactory()
{
	return new CAttributesResourceFactory;
}

void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	m_renderState = 0;
	CModeStack *modeStack = 0;
	COptionsFactory *optionsFactory = 0;
	CAttributesFactory *attributesFactory = 0;
	CLightSourceFactory *lightSourceFactory = 0;
	CRManInterfaceFactory *macroFactory = 0;

	try {
		modeStack = getNewModeStack();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !modeStack ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		return;
	}

	try {
		optionsFactory = getNewOptionsFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !optionsFactory ) {
		delete modeStack;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an options factory");
		return;
	}

	try {
		attributesFactory = getNewAttributesFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !attributesFactory ) {
		delete modeStack;
		delete optionsFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
		return;
	}

	try {
		lightSourceFactory = getNewLightSourceFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !lightSourceFactory ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an light source factory");
		return;
	}

	try {
		macroFactory = getNewMacroFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !macroFactory ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		delete lightSourceFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an macro factory");
		return;
	}

	try {
		m_renderState = new CRenderState(*modeStack, *optionsFactory, *attributesFactory, *lightSourceFactory, *macroFactory);
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !m_renderState ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		delete lightSourceFactory;
		delete macroFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}

	registerResources();
}

bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !renderState() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "State not initialized in %s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !renderState()->validRequest(req) ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !renderState()->hasOptions() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - options not available.", CRequestInfo::requestName(req));
		return false;
	}

	if ( !renderState()->hasAttributes() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - attributes not available.", CRequestInfo::requestName(req));
		return false;
	}

	if ( !renderState()->hasTransform() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - transformations not available.", CRequestInfo::requestName(req));
		return false;
	}

	if ( !renderState()->hasMacroFactory() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - macro factory not available.", CRequestInfo::requestName(req));
		return false;
	}

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
		if ( renderState()->curCondition() ) {
			aRequest.doProcess(*this);
		}
	}

	aRequest.postProcess(*this);
}

void CBaseRenderer::replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb)
{
	renderState()->lineNo(aRequest.lineNo());
	aRequest.preProcess(*this, cb);
	if ( renderState()->curCondition() ) {
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

	// Delete the state, also if there are errors
	delete m_renderState;
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

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[]) {}
RtVoid CBaseRenderer::motionEnd(void) {}


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
	try {
		if ( !preCheck(REQ_FORMAT) )
			return;

		preFormat(xres, yres, aspect);

		if ( renderState()->curMacro() ) {

			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiFormat *m = renderState()->macroFactory().newRiFormat(renderState()->lineNo(), xres, yres, aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFormat", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
			return;
		}

		if ( renderState()->updateStateOnly() )
			return;

		doFormat(xres, yres, aspect);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}
}


RtVoid CBaseRenderer::preFrameAspectRatio(RtFloat aspect)
{
	renderState()->options().frameAspectRatio(aspect);
}

RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	try {
		if ( !preCheck(REQ_FRAME_ASPECT_RATIO) )
			return;

		preFrameAspectRatio(aspect);

		if ( renderState()->curMacro() ) {

			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiFrameAspectRatio *m = renderState()->macroFactory().newRiFrameAspectRatio(renderState()->lineNo(), aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameAspectRatio", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
			return;

		}

		if ( renderState()->updateStateOnly() )
			return;

		doFrameAspectRatio(aspect);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}
}


RtVoid CBaseRenderer::preScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	renderState()->options().screenWindow(left, right, bot, top);
}

RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	try {
		if ( !preCheck(REQ_SCREEN_WINDOW) )
			return;

		preScreenWindow(left, right, bot, top);

		if ( renderState()->curMacro() ) {

			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiScreenWindow *m = renderState()->macroFactory().newRiScreenWindow(renderState()->lineNo(), left, right, bot, top);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiScreenWindow", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
			return;

		}

		if ( renderState()->updateStateOnly() )
			return;

		doScreenWindow(left, right, bot, top);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}
}


RtVoid CBaseRenderer::preCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	renderState()->options().cropWindow(xmin, xmax, ymin, ymax);
}

RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	try {
		if ( !preCheck(REQ_CROP_WINDOW) )
			return;

		preCropWindow(xmin, xmax, ymin, ymax);

		if ( renderState()->curMacro() ) {

				if ( !renderState()->curMacro()->stopInsertion() ) {
					CRiCropWindow *m = renderState()->macroFactory().newRiCropWindow(renderState()->lineNo(), xmin, xmax, ymin, ymax);
					if ( !m )
						throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiCropWindow", __LINE__, __FILE__));
					renderState()->curMacro()->add(m);
				}
				return;

		}

		if ( renderState()->updateStateOnly() )
			return;

		doCropWindow(xmin, xmax, ymin, ymax);

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}
}

RtVoid CBaseRenderer::preProjection(RtToken name, const CParameterList &params)
{
	// Sets the state (can throw)
	renderState()->options().projection(name, renderState()->curParamList());
}

RtVoid CBaseRenderer::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	try {

		// Check validity and accessibility (if-then-else)
		if ( !preCheck(REQ_PROJECTION) )
			return;

		// Parse parameters, ignore unrecognized ones
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(CValueCounts(), n, tokens, params);

		// Preprocess the projection (can throw)
		preProjection(name, renderState()->curParamList());

		// Record as a macro
		if ( renderState()->curMacro() ) {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiProjection *m = renderState()->macroFactory().newRiProjection(renderState()->lineNo(), name, renderState()->curParamList());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiProjection", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
			return;
		}

		// Test if only the state has to be updated
		if ( renderState()->updateStateOnly() )
			return;

		// Do whatever needs to be done for the projection, can throw an exception
		doProjection(name, renderState()->curParamList());

	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, "Unknown error at 'projectionV': %s", e1.what());
		return;
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, "Unknown error at 'projectionV'");
		return;
	}

	// If there were any unrecognized tokens, give an error message
	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'projectionV'");
	}
}


RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	if ( !preCheck(REQ_CLIPPING) )
		return;

	renderState()->options().clipping(hither, yon);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiClipping *m = renderState()->macroFactory().newRiClipping(renderState()->lineNo(), hither, yon);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClipping", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doClipping(hither, yon);
	}
}


RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	if ( !preCheck(REQ_CLIPPING_PLANE) )
		return;

	renderState()->options().clippingPlane(x, y, z, nx, ny, nz);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiClippingPlane *m = renderState()->macroFactory().newRiClippingPlane(renderState()->lineNo(), x, y, z, nx, ny, nz);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClippingPlane", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doClippingPlane(x, y, z, nx, ny, nz);
	}
}


RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	if ( !preCheck(REQ_DEPTH_OF_FIELD) )
		return;

	renderState()->options().depthOfField(fstop, focallength, focaldistance);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDepthOfField *m = renderState()->macroFactory().newRiDepthOfField(renderState()->lineNo(), fstop, focallength, focaldistance);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDepthOfField", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDepthOfField(fstop, focallength, focaldistance);
	}
}


RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	if ( !preCheck(REQ_SHUTTER) )
		return;

	renderState()->options().shutter(smin, smax);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiShutter *m = renderState()->macroFactory().newRiShutter(renderState()->lineNo(), smin, smax);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShutter", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doShutter(smin, smax);
	}
}


RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	if ( !preCheck(REQ_PIXEL_VARIANCE) )
		return;

	renderState()->options().pixelVariance(variation);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiPixelVariance *m = renderState()->macroFactory().newRiPixelVariance(renderState()->lineNo(), variation);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelVariance", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doPixelVariance(variation);
	}
}


RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( !preCheck(REQ_PIXEL_SAMPLES) )
		return;

	renderState()->options().pixelSamples(xsamples, ysamples);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiPixelSamples *m = renderState()->macroFactory().newRiPixelSamples(renderState()->lineNo(), xsamples, ysamples);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelSamples", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doPixelSamples(xsamples, ysamples);
	}
}


RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( !preCheck(REQ_PIXEL_FILTER) )
		return;

	renderState()->options().pixelFilter(function, xwidth, ywidth);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiPixelFilter *m = renderState()->macroFactory().newRiPixelFilter(renderState()->lineNo(), function, xwidth, ywidth);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelFilter", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doPixelFilter(function, xwidth, ywidth);
	}
}


RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	if ( !preCheck(REQ_EXPOSURE) )
		return;

	renderState()->options().exposure(gain, gamma);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiExposure *m = renderState()->macroFactory().newRiExposure(renderState()->lineNo(), gain, gamma);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiExposure", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doExposure(gain, gamma);
	}
}


RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_IMAGER) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);
	
	renderState()->options().imager(name, renderState()->curParamList());

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiImager *m = renderState()->macroFactory().newRiImager(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiImager", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doImager(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'imagerV'");
	}
}


RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( !preCheck(REQ_QUANTIZE) )
		return;

	renderState()->options().quantize(type, one, qmin, qmax, ampl);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiQuantize *m = renderState()->macroFactory().newRiQuantize(renderState()->lineNo(), type, one, qmin, qmax, ampl);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiQuantize", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doQuantize(type, one, qmin, qmax, ampl);
	}
}


RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY_CHANNEL) )
		return;

	channel = renderState()->tokFindCreate(channel);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);
	
	renderState()->options().displayChannelV(renderState()->dict(), channel, n, tokens, params);

	if ( renderState()->curMacro() ) {
		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDisplayChannel *m = renderState()->macroFactory().newRiDisplayChannel(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), channel, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplayChannel", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}
	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDisplayChannel(channel, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displayChannelV'");
	}
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY) )
		return;

	name = renderState()->tokFindCreate(name);
	mode = renderState()->tokFindCreate(mode);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->options().displayV(renderState()->dict(), name, type, mode, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDisplay *m = renderState()->macroFactory().newRiDisplay(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, type, mode, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplay", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDisplay(name, type, mode, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displayV'");
	}
}


RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_HIDER) )
		return;

	type = renderState()->tokFindCreate(type);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->options().hider(type, renderState()->curParamList());

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiHider *m = renderState()->macroFactory().newRiHider(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), type, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiHider", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doHider(type, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'hiderV'");
	}
}


RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	if ( !preCheck(REQ_COLOR_SAMPLES) )
		return;

	renderState()->options().colorSamples(N, nRGB, RGBn);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiColorSamples *m = renderState()->macroFactory().newRiColorSamples(renderState()->lineNo(), N, nRGB, RGBn);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiColorSamples", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doColorSamples(N, nRGB, RGBn);
	}

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(RIE_RANGE, RIE_WARNING, "Illegal values for the parameters of colorSamples(), using identity mapping.");
	}
}


RtVoid CBaseRenderer::relativeDetail(RtFloat relativedetail)
{
	if ( !preCheck(REQ_RELATIVE_DETAIL) )
		return;

	renderState()->options().relativeDetail(relativedetail);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiRelativeDetail *m = renderState()->macroFactory().newRiRelativeDetail(renderState()->lineNo(), relativedetail);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiRelativeDetail", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doRelativeDetail(relativedetail);
	}
}


RtVoid CBaseRenderer::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_OPTION) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->options().set(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiOption *m = renderState()->macroFactory().newRiOption(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOption", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doOption(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'optionV'");
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
					CRiLightSource *m = renderState()->macroFactory().newRiLightSource(*renderState(), name, n, tokens, params);
					if ( !m )
						throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiLightSource", __LINE__, __FILE__));
					renderState()->curMacro()->add(m);
					return m->handleIdx();
				}
		} else {
			if ( renderState()->curCondition() ) {
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
				CRiAreaLightSource *m = renderState()->macroFactory().newRiAreaLightSource(*renderState(), name, n, tokens, params);
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


RtVoid CBaseRenderer::attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_ATTRIBUTE) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().set(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiAttribute *m = renderState()->macroFactory().newRiAttribute(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttribute", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doAttribute(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'attributeV'");
	}
}


RtVoid CBaseRenderer::color(RtColor Cs)
{
	if ( !preCheck(REQ_COLOR) )
		return;

	renderState()->attributes().color(Cs);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiColor *m = renderState()->macroFactory().newRiColor(renderState()->lineNo(), renderState()->options().colorDescr(), Cs);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiColor", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doColor(Cs);
	}
}


RtVoid CBaseRenderer::opacity(RtColor Os)
{
	if ( !preCheck(REQ_OPACITY) )
		return;

	renderState()->attributes().opacity(Os);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiOpacity *m = renderState()->macroFactory().newRiOpacity(renderState()->lineNo(), renderState()->options().colorDescr(), Os);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOpacity", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doOpacity(Os);
	}
}


RtVoid CBaseRenderer::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_SURFACE) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().surfaceV(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiSurface *m = renderState()->macroFactory().newRiSurface(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiSurface", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doSurface(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'surfaceV'");
	}
}


RtVoid CBaseRenderer::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_ATMOSPHERE) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().atmosphereV(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiAtmosphere *m = renderState()->macroFactory().newRiAtmosphere(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAtmosphere", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doAtmosphere(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'atmosphereV'");
	}
}


RtVoid CBaseRenderer::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_INTERIOR) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().interiorV(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiInterior *m = renderState()->macroFactory().newRiInterior(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiInterior", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doInterior(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'interiorV'");
	}
}


RtVoid CBaseRenderer::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_EXTERIOR) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().exteriorV(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiExterior *m = renderState()->macroFactory().newRiExterior(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiExterior", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doExterior(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'exteriorV'");
	}
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	if ( !preCheck(REQ_ILLUMINATE) )
		return;

	renderState()->attributes().illuminate(light, onoff);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiIlluminate *m = renderState()->macroFactory().newRiIlluminate(renderState()->lineNo(), light, onoff);
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


RtVoid CBaseRenderer::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLACEMENT) )
		return;

	name = renderState()->tokFindCreate(name);
	renderState()->parseParameters(CValueCounts(), n, tokens, params);

	renderState()->attributes().displacementV(renderState()->dict(), name, n, tokens, params);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDisplacement *m = renderState()->macroFactory().newRiDisplacement(renderState()->lineNo(), renderState()->dict(), renderState()->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplacement", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDisplacement(name, renderState()->curParamList());
	}

	if ( n != renderState()->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displacementV'");
	}
}


RtVoid CBaseRenderer::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	if ( !preCheck(REQ_TEXTURE_COORDINATES) )
		return;

	renderState()->attributes().textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiTextureCoordinates *m = renderState()->macroFactory().newRiTextureCoordinates(renderState()->lineNo(), s1, t1, s2, t2, s3, t3, s4, t4);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTextureCoordinates", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doTextureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	}
}


RtVoid CBaseRenderer::shadingRate(RtFloat size)
{
	if ( !preCheck(REQ_SHADING_RATE) )
		return;

	renderState()->attributes().shadingRate(size);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiShadingRate *m = renderState()->macroFactory().newRiShadingRate(renderState()->lineNo(), size);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShadingRate", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doShadingRate(size);
	}
}


RtVoid CBaseRenderer::shadingInterpolation(RtToken type)
{
	if ( !preCheck(REQ_SHADING_INTERPOLATION) )
		return;

	renderState()->attributes().shadingInterpolation(type);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiShadingInterpolation *m = renderState()->macroFactory().newRiShadingInterpolation(renderState()->lineNo(), type);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShadingInterpolation", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doShadingInterpolation(type);
	}
}


RtVoid CBaseRenderer::matte(RtBoolean onoff)
{
	if ( !preCheck(REQ_MATTE) )
		return;

	renderState()->attributes().matte(onoff);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiMatte *m = renderState()->macroFactory().newRiMatte(renderState()->lineNo(), onoff);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiMatte", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doMatte(onoff);
	}
}


RtVoid CBaseRenderer::bound(RtBound aBound)
{
	if ( !preCheck(REQ_BOUND) )
		return;

	renderState()->attributes().bound(aBound);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiBound *m = renderState()->macroFactory().newRiBound(renderState()->lineNo(), aBound);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiBound", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doBound(aBound);
	}
}


RtVoid CBaseRenderer::detail(RtBound aBound)
{
	if ( !preCheck(REQ_DETAIL) )
		return;

	renderState()->attributes().detail(aBound);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDetail *m = renderState()->macroFactory().newRiDetail(renderState()->lineNo(), aBound);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDetail", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDetail(aBound);
	}
}


RtVoid CBaseRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( !preCheck(REQ_DETAIL_RANGE) )
		return;

	renderState()->attributes().detailRange(minvis, lowtran, uptran, maxvis);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiDetailRange *m = renderState()->macroFactory().newRiDetailRange(renderState()->lineNo(), minvis, lowtran, uptran, maxvis);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDetailRange", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doDetailRange(minvis, lowtran, uptran, maxvis);
	}
}


RtVoid CBaseRenderer::geometricApproximation(RtToken type, RtFloat value)
{
	if ( !preCheck(REQ_GEOMETRIC_APPROXIMATION) )
		return;

	type = renderState()->tokFindCreate(type);
	renderState()->attributes().geometricApproximation(type, value);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiGeometricApproximation *m = renderState()->macroFactory().newRiGeometricApproximation(renderState()->lineNo(), type, value);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiGeometricApproximation", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doGeometricApproximation(type, value);
	}
}


RtVoid CBaseRenderer::geometricRepresentation(RtToken type)
{
	if ( !preCheck(REQ_GEOMETRIC_REPRESENTATION) )
		return;

	type = renderState()->tokFindCreate(type);

	renderState()->attributes().geometricRepresentation(type);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiGeometricRepresentation *m = renderState()->macroFactory().newRiGeometricRepresentation(renderState()->lineNo(), type);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiGeometricRepresentation", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doGeometricRepresentation(type);
	}
}


RtVoid CBaseRenderer::orientation(RtToken anOrientation)
{
	if ( !preCheck(REQ_ORIENTATION) )
		return;

	anOrientation = renderState()->tokFindCreate(anOrientation);

	renderState()->attributes().orientation(anOrientation);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiOrientation *m = renderState()->macroFactory().newRiOrientation(renderState()->lineNo(), anOrientation);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOrientation", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doOrientation(anOrientation);
	}
}


RtVoid CBaseRenderer::reverseOrientation(void)
{
	if ( !preCheck(REQ_REVERSE_ORIENTATION) )
		return;

	renderState()->attributes().reverseOrientation();

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiReverseOrientation *m = renderState()->macroFactory().newRiReverseOrientation(renderState()->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiReverseOrientation", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doReverseOrientation();
	}
}


RtVoid CBaseRenderer::sides(RtInt nsides)
{
	if ( !preCheck(REQ_SIDES) )
		return;

	renderState()->attributes().sides(nsides);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiSides *m = renderState()->macroFactory().newRiSides(renderState()->lineNo(), nsides);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiSides", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doSides(nsides);
	}
}


RtVoid CBaseRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( !preCheck(REQ_BASIS) )
		return;

	renderState()->attributes().basis(ubasis, ustep, vbasis, vstep);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiBasis *m = renderState()->macroFactory().newRiBasis(renderState()->lineNo(), ubasis, ustep, vbasis, vstep);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiBasis", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doBasis(ubasis, ustep, vbasis, vstep);
	}
}


RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	if ( !preCheck(REQ_BASIS) )
		return;

	renderState()->attributes().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

	if ( renderState()->curMacro() ) {

		try {
			if ( !renderState()->curMacro()->stopInsertion() ) {
				CRiTrimCurve *m = renderState()->macroFactory().newRiTrimCurve(renderState()->lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTrimCurve", __LINE__, __FILE__));
				renderState()->curMacro()->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		if ( renderState()->updateStateOnly() )
			return;
		doTrimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}
}

RtVoid CBaseRenderer::identity(void) {}
RtVoid CBaseRenderer::transform(RtMatrix aTransform) {}
RtVoid CBaseRenderer::concatTransform(RtMatrix aTransform) {}
RtVoid CBaseRenderer::perspective(RtFloat fov) {}
RtVoid CBaseRenderer::translate(RtFloat dx, RtFloat dy, RtFloat dz) {}
RtVoid CBaseRenderer::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
RtVoid CBaseRenderer::scale(RtFloat dx, RtFloat dy, RtFloat dz) {}
RtVoid CBaseRenderer::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}

RtVoid CBaseRenderer::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::scopedCoordinateSystem(RtToken space) {}
RtVoid CBaseRenderer::coordinateSystem(RtToken space) {}
RtVoid CBaseRenderer::coordSysTransform(RtToken space) {}
RtPoint *CBaseRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return &points[0]; }

RtVoid CBaseRenderer::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {}

RtVoid CBaseRenderer::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {}

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


RtVoid CBaseRenderer::ifBegin(RtString expr) {}
RtVoid CBaseRenderer::elseIfBegin(RtString expr) {}
RtVoid CBaseRenderer::elseBegin(void) {}
RtVoid CBaseRenderer::ifEnd(void) {}
