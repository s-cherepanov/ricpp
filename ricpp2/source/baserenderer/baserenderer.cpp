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
	m_ribFilter(0)
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
}

void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	m_renderState = 0;
	CModeStack *modeStack = 0;
	COptionsFactory *optionsFactory = 0;
	CAttributesFactory *attributesFactory = 0;

	try {
		modeStack = getNewModeStack();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
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
	} catch (...) {
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
	} catch (...) {
	}

	if ( !attributesFactory ) {
		delete modeStack;
		delete optionsFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
		return;
	}

	try {
		m_renderState = new CRenderState(*modeStack, *optionsFactory, *attributesFactory);
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !m_renderState ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}
}


/** @brief Create new entry in dectaration list
 */
RtToken CBaseRenderer::handleDeclaration(RtString name, RtString declaration, bool isDefault)
{
	RtToken token;
	CDeclaration *d = 0;
	try {
		token = m_renderState->tokFindCreate(name);
		// if no declaration only tokenize the name
		if ( !emptyStr(declaration) ) {
			d = new CDeclaration(token, declaration, 3, isDefault); // <--- curColorSize if attributes are implemented !!!!
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
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration));
		return RI_NULL;
	}

	if ( !m_renderState->validRequest(REQ_DECLARE) ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_ERROR, "declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration));
		return RI_NULL;
	}

	if ( !name || !*name ) {
		ricppErrHandler().handleError(RIE_MISSINGDATA, RIE_ERROR, "name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration));
		return RI_NULL;
	}

	RtToken token = handleDeclaration(name, declaration, false);

	try {
		doDeclare(token, declaration);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return RI_NULL;
	}

	return token;
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

	initRenderState(); // Can throw

	try {
		m_renderState->contextBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'begin'");
		return 0;
	}

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

	doBeginV(name, n, tokens, params); // Can throw

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
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in frameBegin(%d)", (int)number);
		return;
	}

	if ( !m_renderState->validRequest(REQ_FRAME_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameBegin(%d)", (int)number);
		return;
	}

	try {
		m_renderState->frameBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'frameBegin', Frame no.: %d", (int)number);
		return;
	}
	m_renderState->frameNumber(number);

	doFrameBegin(number); // Can throw
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in frameEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_FRAME_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameEnd()");
		return;
	}
	
	m_renderState->frameEnd();
	m_renderState->frameNumber(0);

	doFrameEnd();
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in worldBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_WORLD_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldBegin()");
		return;
	}
	
	try {
		m_renderState->worldBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'worldBegin'");
		return;
	}

	doWorldBegin();
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in worldEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_WORLD_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldEnd()");
		return;
	}
	
	m_renderState->worldEnd();

	doWorldEnd();
}

RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in attributeBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_ATTRIBUTE_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeBegin()");
		return;
	}
	
	try {
		m_renderState->attributeBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	doAttributeBegin();
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in attributeEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_ATTRIBUTE_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeEnd()");
		return;
	}
	
	m_renderState->attributeEnd();

	doAttributeEnd();
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in transformBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_TRANSFORM_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformBegin()");
		return;
	}
	
	try {
		m_renderState->transformBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	doTransformBegin();
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in transformEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_TRANSFORM_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformEnd()");
		return;
	}
	
	m_renderState->transformEnd();

	doTransformEnd();
}


RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in readArchive().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_READ_ARCHIVE) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "readArchive()");
		return;
	}

	doReadArchiveV(name, callback, n, tokens, params);
}

RtVoid CBaseRenderer::doReadArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	CUri sav(m_baseUri);
	CRibParser parser(*m_ri, *m_errorHandler, *m_protocolHandler, *m_ribFilter, *m_renderState, m_baseUri);
	try {
		if ( parser.canParse(name) ) {
			m_baseUri = parser.absUri();
			parser.parse(callback, n, tokens, params);
		}
	} catch(...) {
	}
	parser.close();
	m_baseUri = sav;
}

RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	//! @todo record interface calls (object, archive)

	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in colorSamples().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_COLOR_SAMPLES) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "colorSamples()");
		return;
	}

	try {

		m_renderState->options().colorSamples(N, nRGB, RGBn);
		doColorSamples(N, nRGB, RGBn);

	} catch(ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch(...) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "Unknown error in colorSamples().");
		return;
	}

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(RIE_RANGE, RIE_ERROR, "Illegal values for the parameters of colorSamples().");
	}
}
