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
#include "ricpp/declaration/declaration.h"
#include <assert.h>

using namespace RiCPP;

void CBaseRenderer::initRenderState()
// throw ERendererError
{
	CModeStack *modeStack = getNewModeStack();
	if ( !modeStack ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		return;
	}
	m_renderState.modeStack(modeStack, true);
}


RtToken CBaseRenderer::declare(RtString name, RtString declaration)
{
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized declare(\"%s\", \"%s\")", name ? name : "<null>", declaration ? declaration : "<null>");
		return RI_NULL;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_DECLARE) ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_ERROR, "declare(\"%s\", \"%s\")", name ? name : "<null>", declaration ? declaration : "<null>");
		return RI_NULL;
	}

	if ( !name || !*name ) {
		ricppErrHandler().handleError(RIE_MISSINGDATA, RIE_ERROR, "name is missing in declare(\"%s\", \"%s\")", name ? name : "<null>", declaration ? declaration : "<null>");
		return RI_NULL;
	}

	RtToken t = RI_NULL;
	try {
		t = doDeclare(name, declaration);
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return RI_NULL;
	}

	return t;
}

RtToken doDeclare(RtString name, RtString declaration)
{
	assert ( name != RI_NULL && *name != 0 );


	return RI_NULL;
}

RtVoid CBaseRenderer::begin(RtString name)
// throw ERendererError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return;
	}

	try {
		initRenderState();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}

	m_renderState.modeStack()->begin();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in end(), break.");
		return;
	}

	ERendererError err;
	if ( m_renderState.modeStack()->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, "Ended context not at begin-state");
	}

	m_renderState.modeStack()->end();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized frameBegin(%d)", (int)number);
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_FRAME_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameBegin(%d)", (int)number);
		return;
	}

	m_renderState.modeStack()->frameBegin();
	m_renderState.frameNumber(number);

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized frameEnd().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_FRAME_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameEnd()");
		return;
	}
	
	m_renderState.modeStack()->frameEnd();
	m_renderState.frameNumber(0);

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized worldBegin().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_WORLD_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldBegin()");
		return;
	}
	
	m_renderState.modeStack()->worldBegin();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized worldEnd().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_WORLD_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "worldEnd()");
		return;
	}
	
	m_renderState.modeStack()->worldEnd();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized attributeBegin().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_ATTRIBUTE_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeBegin()");
		return;
	}
	
	m_renderState.modeStack()->attributeBegin();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized attributeEnd().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_ATTRIBUTE_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "attributeEnd()");
		return;
	}
	
	m_renderState.modeStack()->attributeEnd();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized transformBegin().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_TRANSFORM_BEGIN) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformBegin()");
		return;
	}
	
	m_renderState.modeStack()->transformBegin();

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
	if ( !m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized transformEnd().");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_TRANSFORM_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "transformEnd()");
		return;
	}
	
	m_renderState.modeStack()->transformEnd();

	try {
		doTransformEnd();
	} catch ( ERendererError &e2 ) {
		ricppErrHandler().handleError(e2);
		return;
	}
}
