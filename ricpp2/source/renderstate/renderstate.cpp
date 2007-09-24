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

/** @file renderstate.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the facade for the render state objects (like modes, options, attributes)
 */

#include "ricpp/renderstate/renderstate.h"

using namespace RiCPP;

CRenderState::~CRenderState()
{
	while ( popOptions() );
	if ( m_optionsFactory ) {
		delete m_optionsFactory;
	}

	while ( popAttributes() );
	if ( m_attributesFactory ) {
		delete m_attributesFactory;
	}

	m_transformStack.clear();

	if ( m_modeStack ) {
		delete m_modeStack;
	}
}

void CRenderState::pushOptions()
{
	if ( m_optionsStack.empty() ) {
		m_optionsStack.push_back(m_optionsFactory->newOptions());
	} else {
		m_optionsStack.push_back(m_optionsFactory->newOptions(*m_optionsStack.back()));
	}
	if ( m_optionsStack.back() == 0 ) {
		m_optionsStack.pop_back();
		// Throw
	}
}

bool CRenderState::popOptions()
{
	if ( !m_optionsStack.empty() ) {
		m_optionsFactory->deleteOptions(m_optionsStack.back());
		m_optionsStack.pop_back();
	}
	return !m_optionsStack.empty();
}

void CRenderState::pushAttributes()
{
	if ( m_attributesStack.empty() ) {
		m_attributesStack.push_back(m_attributesFactory->newAttributes(m_optionsStack.back()->colorDesc()));
	} else {
		m_attributesStack.push_back(m_attributesFactory->newAttributes(*m_attributesStack.back()));
	}
	if ( m_attributesStack.back() == 0 ) {
		m_attributesStack.pop_back();
		// Throw
	}
}

bool CRenderState::popAttributes()
{
	if ( !m_attributesStack.empty() ) {
		m_attributesFactory->deleteAttributes(m_attributesStack.back());
		m_attributesStack.pop_back();
	}
	return !m_attributesStack.empty();
}

void CRenderState::pushTransform()
{
	m_transformStack.push_back(CTransformation());
}

bool CRenderState::popTransform()
{
	if ( !m_transformStack.empty() ) {
		m_transformStack.pop_back();
	}
	return !m_transformStack.empty();
}
