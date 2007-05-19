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

using namespace RiCPP;

void CBaseRenderer::initRenderState()
// throw ERendererError
{
	CModeStack *modeStack = getNewModeStack();
	if ( !modeStack )
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
	m_renderState.modeStack(modeStack, true);
}

RtVoid CBaseRenderer::begin(RtString name)
// throw ERendererError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( m_renderState.valid() ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice.");
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
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in end.");
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
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized frameEnd.");
		return;
	}

	if ( !m_renderState.modeStack()->validRequest(REQ_FRAME_END) ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_ERROR, "frameEnd()");
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
