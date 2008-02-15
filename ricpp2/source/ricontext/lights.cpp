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

/** @file lights.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of CLightSource light descriptors.
 *
 *  Stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls).
 */

#include "ricpp/ricontext/lights.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

using namespace RiCPP;

CLightSource::CLightSource(const CLightSource &light)
{
	*this = light;
}

CLightSource::~CLightSource()
{
}

CLightSource *CLightSource::duplicate() const
{
	return new CLightSource(*this);
}

CLightSource &CLightSource::operator=(const CLightSource &light) {
	if ( this == &light )
		return *this;

	m_isIlluminated = light.m_isIlluminated;
	m_isGlobalLight = light.m_isGlobalLight;
	m_isAreaLight = light.m_isAreaLight;
	m_handle = light.m_handle;
	m_lightParameters = light.m_lightParameters;

	return *this;
}

CLightSource::CLightSource(
	CDeclarationDictionary &dict,
	const CColorDescr &colorDescr,
	RtLightHandle aHandle,
    bool isIlluminated, bool isGlobal, bool isArea,
    const char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	m_handle = aHandle,
	m_isIlluminated = isIlluminated;
	m_isGlobalLight = isGlobal;
	m_isAreaLight   = isArea;

	m_lightParameters.set(isArea ? RI_AREA_LIGHT_SOURCE : RI_LIGHT_SOURCE, name, CValueCounts(), dict, colorDescr, name, n, tokens, params);
}

CLightSource::CLightSource(
	RtLightHandle aHandle,
    bool isIlluminated, bool isGlobal, bool isArea,
    const char *name, const CParameterList &params)
{
	m_handle = aHandle;

	m_isIlluminated = isIlluminated;
	m_isGlobalLight = isGlobal;
	m_isAreaLight   = isArea;

	m_lightParameters.set(name, params);
}
