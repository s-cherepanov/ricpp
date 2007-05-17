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
	try {
		initRenderState();
	} catch ( ERendererError &e ) {
		ricppErrHandler().handleError(e);
		return;
	}

	m_renderState.modeStack()->begin();
}

RtVoid CBaseRenderer::end(void)
// throw ERendererError
{
	ERendererError e;
	if ( m_renderState.modeStack()->curMode() != MODE_BEGIN ) {
		e.set(RIE_ILLSTATE, RIE_ERROR, "Ended context not at begin-state");
	}

	m_renderState.modeStack()->end();

	if ( e.isError() ) {
		ricppErrHandler().handleError(e);
	}
}
