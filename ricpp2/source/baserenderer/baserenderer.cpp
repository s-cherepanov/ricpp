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
#include <assert.h>

using namespace RiCPP;

void CBaseRenderer::initRenderState()
// throw ERendererError
{
	m_renderState = 0;
	CModeStack *modeStack = 0;

	try {
		modeStack = getNewModeStack();
	} catch (ERendererError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !modeStack ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		return;
	}

	try {
		m_renderState = new CRenderState(*modeStack);
	} catch (ERendererError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}
}


RtToken CBaseRenderer::declare(RtString name, RtString declaration)
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized declare(\"%s\", \"%s\")", markemptystr(name), markemptystr(declaration));
		return RI_NULL;
	}

	if ( !m_renderState->validRequest(REQ_DECLARE) ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_ERROR, "declare(\"%s\", \"%s\")", markemptystr(name), markemptystr(declaration));
		return RI_NULL;
	}

	if ( !name || !*name ) {
		ricppErrHandler().handleError(RIE_MISSINGDATA, RIE_ERROR, "name is missing in declare(\"%s\", \"%s\")", markemptystr(name), markemptystr(declaration));
		return RI_NULL;
	}

	RtToken token;
	CDeclaration *d = 0;
	try {
		token = m_renderState->tokFindCreate(name);
		// if no declaration only tokenize the name
		if ( !emptyStr(declaration) ) {
			d = new CDeclaration(token, declaration, 3, false); // <--- curColorSize if attributes are implemented !!!!
			if ( !d )
				throw ERendererError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Declaration of \"%s\": \"%s\"", name, declaration);
			m_renderState->declAdd(d);
		}
	} catch (ERendererError &e) {
		ricppErrHandler().handleError(e);
		return RI_NULL;
	}

	try {
		doDeclare(token, declaration);
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return RI_NULL;
	}

	return token;
}

RtVoid CBaseRenderer::begin(RtString name)
// throw ERendererError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( m_renderState ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return;
	}

	try {
		initRenderState();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}

	m_renderState->begin();

	try {
		doBegin(name);
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::end(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in end(), break.");
		return;
	}

	ERendererError err;
	if ( m_renderState->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, "Ended context not at begin-state");
	}

	m_renderState->end();

	try {
		doEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}

	if ( err.isError() ) {
		ricppErrHandler().handleError(err);
		return;
	}
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized frameBegin(%d)", (int)number);
		return;
	}

	if ( !m_renderState->validRequest(REQ_FRAME_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameBegin(%d)", (int)number);
		return;
	}

	m_renderState->frameBegin();
	m_renderState->frameNumber(number);

	try {
		doFrameBegin(number);
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized frameEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_FRAME_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameEnd()");
		return;
	}
	
	m_renderState->frameEnd();
	m_renderState->frameNumber(0);

	try {
		doFrameEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized worldBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_WORLD_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldBegin()");
		return;
	}
	
	m_renderState->worldBegin();

	try {
		doWorldBegin();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized worldEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_WORLD_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldEnd()");
		return;
	}
	
	m_renderState->worldEnd();

	try {
		doWorldEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::attributeBegin(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized attributeBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_ATTRIBUTE_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeBegin()");
		return;
	}
	
	m_renderState->attributeBegin();

	try {
		doAttributeBegin();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized attributeEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_ATTRIBUTE_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeEnd()");
		return;
	}
	
	m_renderState->attributeEnd();

	try {
		doAttributeEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized transformBegin().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_TRANSFORM_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformBegin()");
		return;
	}
	
	m_renderState->transformBegin();

	try {
		doTransformBegin();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ERendererError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized transformEnd().");
		return;
	}

	if ( !m_renderState->validRequest(REQ_TRANSFORM_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformEnd()");
		return;
	}
	
	m_renderState->transformEnd();

	try {
		doTransformEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}
