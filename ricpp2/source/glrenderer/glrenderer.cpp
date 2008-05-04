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

/** @file glrenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a context to do some draft OpenGL rendering
 */

#include "ricpp/glrenderer/glrenderer.h"

using namespace RiCPP;

// -----------------------------------------------------------------------------

const char *CGLRendererCreator::myName() { return GLRENDERERCREATOR_NAME; }
const char *CGLRendererCreator::myType() { return CContextCreator::myType(); }
unsigned long CGLRendererCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }
unsigned long CGLRendererCreator::myMinorVersion() { return 1; }
unsigned long CGLRendererCreator::myRevision() { return 1; }

const char *CGLRenderer::myName() { return GLRENDERER_NAME; }
const char *CGLRenderer::myType() { return IRiContext::myType(); }
unsigned long CGLRenderer::myMajorVersion() { return IRiContext::myMajorVersion(); }
unsigned long CGLRenderer::myMinorVersion() { return 1; }
unsigned long CGLRenderer::myRevision() { return 1; }
RtToken CGLRenderer::myRendererType() { return RI_DRAFT; }


CGLRenderer::CGLRenderer()
{
}

CGLRenderer::~CGLRenderer()
{
}


void CGLRenderer::defaultDeclarations()
{
	CBaseRenderer::defaultDeclarations();
}
