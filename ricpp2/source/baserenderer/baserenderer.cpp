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
#include <cassert>

using namespace RiCPP;

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_ri(0),
	m_errorHandler(0),
	m_protocolHandler(0),
	m_ribFilter(0),
	m_macroFactory(0),
	m_curMacro(0)
{
	CFilepath fp;
	std::string s(fp.filepath());
	s+= "/";
	m_baseUri.set("file", "", s.c_str(), 0, 0);
}

CBaseRenderer::~CBaseRenderer()
{
	if ( m_renderState )
		delete m_renderState;
	if ( m_macroFactory )
		delete m_macroFactory;
}

void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	m_renderState = 0;
	CModeStack *modeStack = 0;
	COptionsFactory *optionsFactory = 0;
	CAttributesFactory *attributesFactory = 0;
	CLightSourceFactory *lightSourceFactory = 0;

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
		delete attributesFactory;
		delete modeStack;
		delete optionsFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an light source factory");
		return;
	}

	try {
		m_renderState = new CRenderState(*modeStack, *optionsFactory, *attributesFactory, *lightSourceFactory);
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	}

	if ( !m_renderState ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}
}

bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "State not initialized in %s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->validRequest(req) ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_ERROR, "%s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->hasOptionsReader() ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "%s() - options not available.", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->hasAttributesReader() ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "%s() - attributes not available.", CRequestInfo::requestName(req));
		return false;
	}

	return true;
}

/** @brief Create new entry in dectaration list
 */
RtToken CBaseRenderer::handleDeclaration(RtString name, RtString declaration, bool isDefault)
{
	RtToken token = RI_NULL;
	CDeclaration *d = 0;

	try {
		token = m_renderState->tokFindCreate(name);
		// if no declaration only tokenize the name
		if ( !emptyStr(declaration) ) {
		
			try {
				d = new CDeclaration(token, declaration, m_renderState->options().colorDescr(), m_renderState->tokenMap(), isDefault);
			} catch (ExceptRiCPPError &e) {
				ricppErrHandler().handleError(e);
				return RI_NULL;
			}
			
			if ( !d )
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Declaration of \"%s\": \"%s\"", name, declaration);
			m_renderState->declAdd(d);
		}
	} catch (ExceptRiCPPError &e) {
		ricppErrHandler().handleError(e);
		return RI_NULL;
	}

	return token;
}

RtToken CBaseRenderer::declare(RtString name, RtString declaration)
{
	if ( !preCheck(REQ_DECLARE) )
		return RI_NULL;

	if ( !name || !*name ) {
		ricppErrHandler().handleError(RIE_MISSINGDATA, RIE_ERROR, "name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration));
		return RI_NULL;
	}


	RtToken token = RI_NULL;

	try {
		token = handleDeclaration(name, declaration, false);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return RI_NULL;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDeclare *m = m_macroFactory->newRiDeclare(m_renderState->lineNo(), name, declaration);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDeclare", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
			return RI_NULL;
		}

	} else {

		try {
			doDeclare(token, declaration);
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
			return RI_NULL;
		}

	}

	return token;
}


void CBaseRenderer::defaultDeclarations()
{
	// Default declarations
	handleDeclaration(RI_FLATNESS, "float", true);
	handleDeclaration(RI_FOV, "float", true);

	handleDeclaration(RI_INTENSITY, "float", true);
	handleDeclaration(RI_LIGHTCOLOR, "color", true);
	handleDeclaration(RI_FROM, "point", true);
	handleDeclaration(RI_TO, "point", true);
	handleDeclaration(RI_CONEANGLE, "float", true);
	handleDeclaration(RI_CONEDELTAANGLE, "float", true);
	handleDeclaration(RI_BEAMDISTRIBUTION, "float", true);

	handleDeclaration(RI_KA, "float", true);
	handleDeclaration(RI_KD, "float", true);
	handleDeclaration(RI_KS, "float", true);
	handleDeclaration(RI_ROUGHNESS, "float", true);
	handleDeclaration(RI_KR, "float", true);
	handleDeclaration(RI_TEXTURENAME, "string", true);
	handleDeclaration(RI_SPECULARCOLOR, "color", true);
	handleDeclaration(RI_MINDISTANCE, "float", true);
	handleDeclaration(RI_MAXDISTANCE, "float", true);
	handleDeclaration(RI_BACKGROUND, "color", true);
	handleDeclaration(RI_DISTANCE, "float", true);
	handleDeclaration(RI_AMPLITUDE, "float", true);

	handleDeclaration(RI_P, "vertex point", true);
	handleDeclaration(RI_PZ, "vertex float", true);
	handleDeclaration(RI_PW, "vertex hpoint", true);
	handleDeclaration(RI_N,  "varying point", true);  // Normal
	handleDeclaration(RI_NP, "uniform point", true);
	handleDeclaration(RI_CS, "varying color", true);  // Color
	handleDeclaration(RI_OS, "varying color", true);  // Opacity
	handleDeclaration(RI_S,  "varying float", true);  // Texture coordinates
	handleDeclaration(RI_T,  "varying float", true);
	handleDeclaration(RI_ST, "varying float[2]", true);

	handleDeclaration(RI_ORIGIN, "constant integer[2]", true);   // Origin of the display

	handleDeclaration(RI_NAME, "constant string", true);
	handleDeclaration(RI_WIDTH, "varying float", true);
	handleDeclaration(RI_CONSTANTWIDTH, "float", true);

	handleDeclaration(RI_FILE, "string", true);
}


RtContextHandle CBaseRenderer::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( m_renderState ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return 0;
	}

	if ( !m_macroFactory )
		m_macroFactory = getNewMacroFactory();

	initRenderState(); // Can throw

	try {
		m_renderState->contextBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'begin'");
		return 0;
	}

	defaultDeclarations();

	m_renderState->parseParameters(CValueCounts(), n, tokens, params);
	doBeginV(name, n, tokens, params); // Can throw

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'begin'");
	}

	return 0;
}

RtVoid CBaseRenderer::end(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in end(), break.");
		return;
	}

	ExceptRiCPPError err;
	if ( m_renderState->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, "Ended context not at begin-state");
	}

	if ( m_renderState->areaLightSourceHandle() != illLightHandle &&
	     m_renderState->areaLightSourceDepth() == m_renderState->modesSize() )
	{
		doAreaLightSourceV(m_renderState->areaLightSourceHandle(), RI_NULL, 0, 0, 0);
		m_renderState->endAreaLightSource();
	}

	m_renderState->contextEnd();

	try {
		doEnd(); // Can throw, err is lost then
	} catch ( ExceptRiCPPError &err2 )  {
		err = err2;
	}

	// Delete the state anyway
	delete m_renderState;
	m_renderState = 0;

	if ( err.isError() ) {
		ricppErrHandler().handleError(err);
		return;
	}
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_FRAME_BEGIN) )
		return;

	try {
		m_renderState->frameBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'frameBegin', Frame no.: %d", (int)number);
		return;
	}

	m_renderState->frameNumber(number);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameBegin *m = m_macroFactory->newRiFrameBegin(m_renderState->lineNo(), number);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameBegin(number); // Can throw
	}
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_FRAME_END) )
		return;
	
	if ( m_renderState->areaLightSourceHandle() != illLightHandle &&
	     m_renderState->areaLightSourceDepth() == m_renderState->modesSize() )
	{
		doAreaLightSourceV(m_renderState->areaLightSourceHandle(), RI_NULL, 0, 0, 0);
		m_renderState->endAreaLightSource();
	}

	m_renderState->frameEnd();
	m_renderState->frameNumber(0);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameEnd *m = m_macroFactory->newRiFrameEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameEnd();
	}
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_WORLD_BEGIN) )
		return;

	try {
		m_renderState->worldBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'worldBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiWorldBegin *m = m_macroFactory->newRiWorldBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiWorldBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doWorldBegin();
	}
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_WORLD_END) )
		return;
	
	if ( m_renderState->areaLightSourceHandle() != illLightHandle &&
	     m_renderState->areaLightSourceDepth() == m_renderState->modesSize() )
	{
		doAreaLightSourceV(m_renderState->areaLightSourceHandle(), RI_NULL, 0, 0, 0);
		m_renderState->endAreaLightSource();
	}

	m_renderState->worldEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiWorldEnd *m = m_macroFactory->newRiWorldEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiWorldEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doWorldEnd();
	}
}

RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_ATTRIBUTE_BEGIN) )
		return;
	
	try {
		m_renderState->attributeBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAttributeBegin *m = m_macroFactory->newRiAttributeBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttributeBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAttributeBegin();
	}
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_ATTRIBUTE_END) )
		return;
	
	if ( m_renderState->areaLightSourceHandle() != illLightHandle &&
	     m_renderState->areaLightSourceDepth() == m_renderState->modesSize() )
	{
		doAreaLightSourceV(m_renderState->areaLightSourceHandle(), RI_NULL, 0, 0, 0);
		m_renderState->endAreaLightSource();
	}

	m_renderState->attributeEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAttributeEnd *m = m_macroFactory->newRiAttributeEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttributeEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAttributeEnd();
	}
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_TRANSFORM_BEGIN) )
		return;
	
	try {
		m_renderState->transformBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTransformBegin *m = m_macroFactory->newRiTransformBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTransformBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doTransformBegin();
	}
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_TRANSFORM_END) )
		return;

	m_renderState->transformEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTransformEnd *m = m_macroFactory->newRiTransformEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTransformEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doTransformEnd();
	}
}



RtVoid CBaseRenderer::solidBegin(RtToken type){}
RtVoid CBaseRenderer::solidEnd(void) {}

RtObjectHandle CBaseRenderer::objectBegin(void) { return illObjectHandle; }
RtVoid CBaseRenderer::objectEnd(void) {}
RtVoid CBaseRenderer::objectInstance(RtObjectHandle handle) {}

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[]) {}
RtVoid CBaseRenderer::motionEnd(void) {}

RtVoid CBaseRenderer::resourceBegin(void) {}
RtVoid CBaseRenderer::resourceEnd(void) {}

RtArchiveHandle CBaseRenderer::archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { return illArchiveHandle; }
RtVoid CBaseRenderer::archiveEnd(void) {}


RtVoid CBaseRenderer::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	if ( !preCheck(REQ_FORMAT) )
		return;

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFormat *m = m_macroFactory->newRiFormat(m_renderState->lineNo(), xres, yres, aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFormat", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFormat(xres, yres, aspect);
	}
}


RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	if ( !preCheck(REQ_FRAME_ASPECT_RATIO) )
		return;

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameAspectRatio *m = m_macroFactory->newRiFrameAspectRatio(m_renderState->lineNo(), aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameAspectRatio", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameAspectRatio(aspect);
	}
}


RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	if ( !preCheck(REQ_SCREEN_WINDOW) )
		return;

	m_renderState->options().screenWindow(left, right, bot, top);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiScreenWindow *m = m_macroFactory->newRiScreenWindow(m_renderState->lineNo(), left, right, bot, top);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiScreenWindow", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doScreenWindow(left, right, bot, top);
	}
}


RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	if ( !preCheck(REQ_CROP_WINDOW) )
		return;

	m_renderState->options().cropWindow(xmin, xmax, ymin, ymax);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiCropWindow *m = m_macroFactory->newRiCropWindow(m_renderState->lineNo(), xmin, xmax, ymin, ymax);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiCropWindow", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doCropWindow(xmin, xmax, ymin, ymax);
	}
}


RtVoid CBaseRenderer::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_PROJECTION) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->options().projection(name, m_renderState->curParamList());

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiProjection *m = m_macroFactory->newRiProjection(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, m_renderState->numTokens(), m_renderState->tokens(), m_renderState->params());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiProjection", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doProjectionV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'projectionV'");
	}
}


RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	if ( !preCheck(REQ_CLIPPING) )
		return;

	m_renderState->options().clipping(hither, yon);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiClipping *m = m_macroFactory->newRiClipping(m_renderState->lineNo(), hither, yon);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClipping", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doClipping(hither, yon);
	}
}


RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	if ( !preCheck(REQ_CLIPPING_PLANE) )
		return;

	m_renderState->options().clippingPlane(x, y, z, nx, ny, nz);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiClippingPlane *m = m_macroFactory->newRiClippingPlane(m_renderState->lineNo(), x, y, z, nx, ny, nz);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClippingPlane", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doClippingPlane(x, y, z, nx, ny, nz);
	}
}


RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	if ( !preCheck(REQ_DEPTH_OF_FIELD) )
		return;

	m_renderState->options().depthOfField(fstop, focallength, focaldistance);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDepthOfField *m = m_macroFactory->newRiDepthOfField(m_renderState->lineNo(), fstop, focallength, focaldistance);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDepthOfField", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDepthOfField(fstop, focallength, focaldistance);
	}
}


RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	if ( !preCheck(REQ_SHUTTER) )
		return;

	m_renderState->options().shutter(smin, smax);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiShutter *m = m_macroFactory->newRiShutter(m_renderState->lineNo(), smin, smax);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShutter", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doShutter(smin, smax);
	}
}


RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	if ( !preCheck(REQ_PIXEL_VARIANCE) )
		return;

	m_renderState->options().pixelVariance(variation);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelVariance *m = m_macroFactory->newRiPixelVariance(m_renderState->lineNo(), variation);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelVariance", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelVariance(variation);
	}
}


RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( !preCheck(REQ_PIXEL_SAMPLES) )
		return;

	m_renderState->options().pixelSamples(xsamples, ysamples);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelSamples *m = m_macroFactory->newRiPixelSamples(m_renderState->lineNo(), xsamples, ysamples);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelSamples", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelSamples(xsamples, ysamples);
	}
}


RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( !preCheck(REQ_PIXEL_FILTER) )
		return;

	m_renderState->options().pixelFilter(function, xwidth, ywidth);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelFilter *m = m_macroFactory->newRiPixelFilter(m_renderState->lineNo(), function, xwidth, ywidth);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelFilter", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelFilter(function, xwidth, ywidth);
	}
}


RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	if ( !preCheck(REQ_EXPOSURE) )
		return;

	m_renderState->options().exposure(gain, gamma);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiExposure *m = m_macroFactory->newRiExposure(m_renderState->lineNo(), gain, gamma);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiExposure", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doExposure(gain, gamma);
	}
}


RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_IMAGER) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);
	
	m_renderState->options().imagerV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiImager *m = m_macroFactory->newRiImager(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiImager", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doImagerV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'imagerV'");
	}
}


RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( !preCheck(REQ_QUANTIZE) )
		return;

	m_renderState->options().quantize(type, one, qmin, qmax, ampl);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiQuantize *m = m_macroFactory->newRiQuantize(m_renderState->lineNo(), type, one, qmin, qmax, ampl);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiQuantize", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doQuantize(type, one, qmin, qmax, ampl);
	}
}


RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY_CHANNEL) )
		return;

	channel = m_renderState->tokFindCreate(channel);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);
	
	m_renderState->options().displayChannelV(m_renderState->dict(), channel, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {
		try {
			if ( m_curMacro->valid() ) {
				CRiDisplayChannel *m = m_macroFactory->newRiDisplayChannel(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), channel, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplayChannel", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}
	} else {
		doDisplayChannelV(channel, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displayChannelV'");
	}
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY) )
		return;

	name = m_renderState->tokFindCreate(name);
	mode = m_renderState->tokFindCreate(mode);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->options().displayV(m_renderState->dict(), name, type, mode, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDisplay *m = m_macroFactory->newRiDisplay(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, type, mode, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplay", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDisplayV(name, type, mode, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displayV'");
	}
}


RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_HIDER) )
		return;

	type = m_renderState->tokFindCreate(type);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->options().hiderV(m_renderState->dict(), type, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiHider *m = m_macroFactory->newRiHider(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), type, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiHider", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doHiderV(type, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'hiderV'");
	}
}


RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	if ( !preCheck(REQ_COLOR_SAMPLES) )
		return;

	m_renderState->options().colorSamples(N, nRGB, RGBn);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiColorSamples *m = m_macroFactory->newRiColorSamples(m_renderState->lineNo(), N, nRGB, RGBn);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiColorSamples", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
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

	m_renderState->options().relativeDetail(relativedetail);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiRelativeDetail *m = m_macroFactory->newRiRelativeDetail(m_renderState->lineNo(), relativedetail);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiRelativeDetail", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doRelativeDetail(relativedetail);
	}
}


RtVoid CBaseRenderer::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_OPTION) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->options().set(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiOption *m = m_macroFactory->newRiOption(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOption", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doOptionV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'optionV'");
	}
}


RtLightHandle CBaseRenderer::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;

	if ( !preCheck(REQ_LIGHT_SOURCE) )
		return h;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiLightSource *m = m_macroFactory->newRiLightSource(*m_renderState, name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiLightSource", __LINE__, __FILE__));
				m_curMacro->add(m);
				return m->handleIdx();
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}
	} else {
		try {
			h = m_renderState->lights().lightSource(m_renderState->dict(), m_renderState->options().colorDescr(),
				true, !m_renderState->inWorldBlock(), false, name, n, tokens, params);
			doLightSourceV(h, name, n, tokens, params);
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'lightSourceV'");
	}

	return h;
}


RtLightHandle CBaseRenderer::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_AREA_LIGHT_SOURCE) )
		return illLightHandle;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	RtLightHandle h = m_renderState->areaLightSourceHandle();

	if ( h != illLightHandle ) {
		m_renderState->endAreaLightSource();
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAreaLightSource *m = m_macroFactory->newRiAreaLightSource(*m_renderState, name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAreaLightSource", __LINE__, __FILE__));
				m_curMacro->add(m);
				h = m->handleIdx();
				m_renderState->startAreaLightSource(h);

				if ( n != m_renderState->numTokens() ) {
					ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
				}
				return h;
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

		if ( n != m_renderState->numTokens() ) {
			ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
		}
		return illLightHandle;
	}

	try {
		if ( !emptyStr(name) ) {
			h = m_renderState->lights().lightSource(m_renderState->dict(), m_renderState->options().colorDescr(),
				true, !m_renderState->inWorldBlock(), true, name, n, tokens, params);
		}
		m_renderState->startAreaLightSource(h);
		doAreaLightSourceV(h, name, n, tokens, params);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'areaLightSourceV'");
	}

	return h;
}


RtVoid CBaseRenderer::attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_ATTRIBUTE) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().set(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAttribute *m = m_macroFactory->newRiAttribute(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttribute", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAttributeV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'attributeV'");
	}
}


RtVoid CBaseRenderer::color(RtColor Cs)
{
	if ( !preCheck(REQ_COLOR) )
		return;

	m_renderState->attributes().color(Cs);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiColor *m = m_macroFactory->newRiColor(m_renderState->lineNo(), m_renderState->options().colorDescr(), Cs);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiColor", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doColor(Cs);
	}
}


RtVoid CBaseRenderer::opacity(RtColor Os)
{
	if ( !preCheck(REQ_OPACITY) )
		return;

	m_renderState->attributes().opacity(Os);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiOpacity *m = m_macroFactory->newRiOpacity(m_renderState->lineNo(), m_renderState->options().colorDescr(), Os);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOpacity", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doOpacity(Os);
	}
}


RtVoid CBaseRenderer::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_SURFACE) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().surfaceV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiSurface *m = m_macroFactory->newRiSurface(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiSurface", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doSurfaceV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'surfaceV'");
	}
}


RtVoid CBaseRenderer::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_ATMOSPHERE) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().atmosphereV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAtmosphere *m = m_macroFactory->newRiAtmosphere(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAtmosphere", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAtmosphereV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'atmosphereV'");
	}
}


RtVoid CBaseRenderer::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_INTERIOR) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().interiorV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiInterior *m = m_macroFactory->newRiInterior(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiInterior", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doInteriorV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'interiorV'");
	}
}


RtVoid CBaseRenderer::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_EXTERIOR) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().exteriorV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiExterior *m = m_macroFactory->newRiExterior(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiExterior", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doExteriorV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'exteriorV'");
	}
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	if ( !preCheck(REQ_ILLUMINATE) )
		return;

	m_renderState->attributes().illuminate(light, onoff);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiIlluminate *m = m_macroFactory->newRiIlluminate(m_renderState->lineNo(), light, onoff);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiIlluminate", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doIlluminate(light, onoff);
	}
}


RtVoid CBaseRenderer::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLACEMENT) )
		return;

	name = m_renderState->tokFindCreate(name);
	m_renderState->parseParameters(CValueCounts(), n, tokens, params);

	m_renderState->attributes().displacementV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDisplacement *m = m_macroFactory->newRiDisplacement(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorDescr(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplacement", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDisplacementV(name, n, tokens, params);
	}

	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'displacementV'");
	}
}


RtVoid CBaseRenderer::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	if ( !preCheck(REQ_TEXTURE_COORDINATES) )
		return;

	m_renderState->attributes().textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTextureCoordinates *m = m_macroFactory->newRiTextureCoordinates(m_renderState->lineNo(), s1, t1, s2, t2, s3, t3, s4, t4);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTextureCoordinates", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doTextureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	}
}


RtVoid CBaseRenderer::shadingRate(RtFloat size)
{
	if ( !preCheck(REQ_SHADING_RATE) )
		return;

	m_renderState->attributes().shadingRate(size);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiShadingRate *m = m_macroFactory->newRiShadingRate(m_renderState->lineNo(), size);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShadingRate", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doShadingRate(size);
	}
}


RtVoid CBaseRenderer::shadingInterpolation(RtToken type)
{
	if ( !preCheck(REQ_SHADING_INTERPOLATION) )
		return;

	m_renderState->attributes().shadingInterpolation(type);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiShadingInterpolation *m = m_macroFactory->newRiShadingInterpolation(m_renderState->lineNo(), type);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShadingInterpolation", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doShadingInterpolation(type);
	}
}


RtVoid CBaseRenderer::matte(RtBoolean onoff)
{
	if ( !preCheck(REQ_MATTE) )
		return;

	m_renderState->attributes().matte(onoff);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiMatte *m = m_macroFactory->newRiMatte(m_renderState->lineNo(), onoff);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiMatte", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doMatte(onoff);
	}
}


RtVoid CBaseRenderer::bound(RtBound aBound)
{
	if ( !preCheck(REQ_BOUND) )
		return;

	m_renderState->attributes().bound(aBound);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiBound *m = m_macroFactory->newRiBound(m_renderState->lineNo(), aBound);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiBound", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doBound(aBound);
	}
}


RtVoid CBaseRenderer::detail(RtBound aBound)
{
	if ( !preCheck(REQ_DETAIL) )
		return;

	m_renderState->attributes().detail(aBound);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDetail *m = m_macroFactory->newRiDetail(m_renderState->lineNo(), aBound);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDetail", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDetail(aBound);
	}
}


RtVoid CBaseRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( !preCheck(REQ_DETAIL_RANGE) )
		return;

	m_renderState->attributes().detailRange(minvis, lowtran, uptran, maxvis);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDetailRange *m = m_macroFactory->newRiDetailRange(m_renderState->lineNo(), minvis, lowtran, uptran, maxvis);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDetailRange", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDetailRange(minvis, lowtran, uptran, maxvis);
	}
}


RtVoid CBaseRenderer::geometricApproximation(RtToken type, RtFloat value)
{
	if ( !preCheck(REQ_GEOMETRIC_APPROXIMATION) )
		return;

	type = m_renderState->tokFindCreate(type);
	m_renderState->attributes().geometricApproximation(type, value);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiGeometricApproximation *m = m_macroFactory->newRiGeometricApproximation(m_renderState->lineNo(), type, value);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiGeometricApproximation", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doGeometricApproximation(type, value);
	}
}


RtVoid CBaseRenderer::geometricRepresentation(RtToken type)
{
	if ( !preCheck(REQ_GEOMETRIC_REPRESENTATION) )
		return;

	type = m_renderState->tokFindCreate(type);

	m_renderState->attributes().geometricRepresentation(type);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiGeometricRepresentation *m = m_macroFactory->newRiGeometricRepresentation(m_renderState->lineNo(), type);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiGeometricRepresentation", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doGeometricRepresentation(type);
	}
}


RtVoid CBaseRenderer::orientation(RtToken anOrientation)
{
	if ( !preCheck(REQ_ORIENTATION) )
		return;

	anOrientation = m_renderState->tokFindCreate(anOrientation);

	m_renderState->attributes().orientation(anOrientation);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiOrientation *m = m_macroFactory->newRiOrientation(m_renderState->lineNo(), anOrientation);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOrientation", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doOrientation(anOrientation);
	}
}


RtVoid CBaseRenderer::reverseOrientation(void)
{
	if ( !preCheck(REQ_REVERSE_ORIENTATION) )
		return;

	m_renderState->attributes().reverseOrientation();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiReverseOrientation *m = m_macroFactory->newRiReverseOrientation(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiReverseOrientation", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doReverseOrientation();
	}
}


RtVoid CBaseRenderer::sides(RtInt nsides)
{
	if ( !preCheck(REQ_SIDES) )
		return;

	m_renderState->attributes().sides(nsides);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiSides *m = m_macroFactory->newRiSides(m_renderState->lineNo(), nsides);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiSides", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doSides(nsides);
	}
}


RtVoid CBaseRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( !preCheck(REQ_BASIS) )
		return;

	m_renderState->attributes().basis(ubasis, ustep, vbasis, vstep);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiBasis *m = m_macroFactory->newRiBasis(m_renderState->lineNo(), ubasis, ustep, vbasis, vstep);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiBasis", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doBasis(ubasis, ustep, vbasis, vstep);
	}
}


RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	if ( !preCheck(REQ_BASIS) )
		return;

	m_renderState->attributes().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTrimCurve *m = m_macroFactory->newRiTrimCurve(m_renderState->lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTrimCurve", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
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

RtVoid CBaseRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
RtVoid CBaseRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}

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

	m_renderState->parseParameters(CValueCounts(), n, tokens, params);
	doReadArchiveV(name, callback, n, tokens, params);
	if ( n != m_renderState->numTokens() ) {
		ricppErrHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Unrecognized tokens in 'readArchiveV'");
	}
}

RtVoid CBaseRenderer::doReadArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	CUri sav(m_baseUri);
	const char *oldArchiveName = m_renderState->archiveName();
	long oldLineNo = m_renderState->lineNo();

	n = m_renderState->numTokens();
	tokens = m_renderState->tokens();
	params = m_renderState->params();

	CRibParser parser(*m_ri, *m_errorHandler, *m_protocolHandler, *m_ribFilter, *m_renderState, m_baseUri);
	try {
		if ( parser.canParse(name) ) {
			m_baseUri = parser.absUri();
			m_renderState->archiveName(name);
			m_renderState->lineNo(0);
			parser.parse(callback, n, tokens, params);
			m_renderState->archiveName(oldArchiveName);
			m_renderState->lineNo(oldLineNo);
			m_baseUri = sav;
			parser.close();
		}
	} catch(ExceptRiCPPError &err) {
		m_baseUri = sav;
		m_renderState->archiveName(oldArchiveName);
		m_renderState->lineNo(oldLineNo);
		parser.close();
		ricppErrHandler().handleError(err);
		return;
	} catch(...) {
		m_baseUri = sav;
		m_renderState->archiveName(oldArchiveName);
		m_renderState->lineNo(oldLineNo);
		parser.close();
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "unknown error");
		return;
	}
}


RtVoid CBaseRenderer::ifBegin(RtString expr) {}
RtVoid CBaseRenderer::elseIfBegin(RtString expr) {}
RtVoid CBaseRenderer::elseBegin(void) {}
RtVoid CBaseRenderer::ifEnd(void) {}
