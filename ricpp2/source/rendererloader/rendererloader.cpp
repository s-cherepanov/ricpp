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

/** @file rendererloader.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the creation of a renderer
 */

#include "ricpp/rendererloader/rendererloader.h"
#include "ricpp/ribwriter/ribwriter.h"

#include <cassert>

using namespace RiCPP;

CRendererLoader::CRendererLoader()
{
	m_standardRendererName = CRibWriter::myName();
}

const char *CRendererLoader::rendererName(RtString name) const
{
	assert (!m_standardRendererName.empty());

	if ( emptyStr(name) )
		return m_standardRendererName.empty() ? CRibWriter::myName() : m_standardRendererName.c_str();

	return name;
}

CContextCreator *CRendererLoader::getContextCreator(RtString name) {
	CContextCreator *cc = lastPlugin(rendererName(name));
	if ( cc ) {
		if ( cc->majorContextVersion() != IRiContext::myMajorVersion() ) {
			throw ERiCPPError(RIE_VERSION, RIE_SEVERE, __LINE__, __FILE__, "Context version mismatch, renderer name: '%s'", rendererName(name));
		}
		if ( strcmp(nonullstr(cc->contextType()), nonullstr(IRiContext::myType())) != 0 ) {
			throw ERiCPPError(RIE_BADFILE, RIE_SEVERE, __LINE__, __FILE__, "Context type mismatch, renderer name: '%s'", rendererName(name));
		}
	}
	return cc;
}

void CRendererLoader::standardRendererName(const char *name)
{
	if ( emptyStr(name) )
		name = CRibWriter::myName();
	m_standardRendererName = name;
}
