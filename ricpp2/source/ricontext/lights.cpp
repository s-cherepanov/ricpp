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
	m_handle = illLightHandle;
	*this = light;
}

CLightSource::~CLightSource()
{
}

CLightSource &CLightSource::duplicate() const
{
	CLightSource *light = new CLightSource(*this);
	if ( !light )
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::duplicate()");
	return *light;
}

CLightSource &CLightSource::operator=(const CLightSource &light) {
	if ( this == &light )
		return *this;

	m_isIlluminated = light.m_isIlluminated;
	m_isGlobalLight = light.m_isGlobalLight;
	m_isAreaLight = light.m_isAreaLight;

	m_lightParameters = light.m_lightParameters;

	return *this;
}

void CLightSource::lightSource(
	CDeclarationDictionary &dict,
	const CColorDescr &colorDescr,
    bool isIlluminated, bool isGlobal, bool isArea,
    const char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	m_isIlluminated = isIlluminated;
	m_isGlobalLight = isGlobal;
	m_isAreaLight   = isArea;

	m_lightParameters.set(isArea ? RI_AREA_LIGHT_SOURCE : RI_LIGHT_SOURCE, name, CValueCounts(), dict, colorDescr, name, n, tokens, params);
}

void CLightSource::lightSource(
    bool isIlluminated, bool isGlobal, bool isArea,
    const char *name, const CParameterList &params)
{
	m_isIlluminated = isIlluminated;
	m_isGlobalLight = isGlobal;
	m_isAreaLight   = isArea;

	m_lightParameters.set(name, params);
}

CLightSource *CLightSourceFactory::newLightSource(const char *name) const
{
	CLightSource *light = new CLightSource(name);
	if ( !light ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::newLightSource()");
	}

	return light;
}

CLightSource *CLightSourceFactory::newLightSource(
	CDeclarationDictionary &dict, const CColorDescr &colorDescr,
	bool isIlluminated, bool isGlobal, bool isArea,
	const char *name,
	RtInt n, RtToken tokens[], RtPointer params[]) const
{
	CLightSource *light = newLightSource();
	if ( !light ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::newLightSource()");
	}

	light->lightSource(
		dict, colorDescr,
		isIlluminated, isGlobal, isArea,
		name,
		n, tokens, params);

	return light;
}


CLightSource *CLightSourceFactory::newLightSource(
	bool isIlluminated, bool isGlobal, bool isArea,
	const char *name,
	const CParameterList &params) const
{
	CLightSource *light = newLightSource();
	if ( !light ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::newLightSource()");
	}

	light->lightSource(
		isIlluminated, isGlobal, isArea,
		name, params);

	return light;
}


CLights::TypeLightContainer::size_type CLights::handleToLightIndex(RtLightHandle handle) const
{
	if ( handle < 0 ) {
		handle = -(handle+1);
		if ( handle >= static_cast<RtLightHandle>(m_handles.size()) )
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::getWriteableLight(), Handle undefined");
		handle = m_handles[handle];
	}

	if ( handle == 0 || handle > static_cast<RtLightHandle>(m_lights.size()) )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::getWriteableLight(), Handle undefined");

	return handle-1;
}


const CLightSource *CLights::getLight(RtLightHandle handle) const
{
	return m_lights[handleToLightIndex(handle)];
}


CLightSource *CLights::getLight(RtLightHandle handle)
{
	return m_lights[handleToLightIndex(handle)];
}

RtLightHandle CLights::newLightHandleIdx()
{
	try {
		m_handles.push_back(0);
	} catch(std::exception e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s: %s", "CLightSource::newLightHandle()", noNullStr(e.what()));
	}

	RtLightHandle idx = static_cast<RtLightHandle>(m_handles.size()-1);
	idx = -(idx+1);
	// idx < 0

	return idx;
}

void CLights::setHandle(RtLightHandle idx, RtLightHandle handle)
{
	if ( idx >= 0 )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::setHandle(), Handle idx undefined");

	// idx < 0
	idx = -(idx+1);

	if ( idx >= static_cast<RtLightHandle>(m_handles.size()) )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::setHandle(), Handle idx undefined");
	
	if ( handle <= 0 || handle > static_cast<RtLightHandle>(m_lights.size()) )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::setHandle(), Light handle undefined");

	m_handles[static_cast<TypeLightContainer::size_type>(idx)] = handle;
}

RtLightHandle CLights::getHandle(RtLightHandle idx)
{
	if ( idx >= 0 )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::getHandle(), Handle idx undefined");

	// idx < 0
	idx = -(idx+1);

	if ( idx >= static_cast<RtLightHandle>(m_handles.size()) )
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::getHandle(), Handle idx undefined");

	return m_handles[static_cast<TypeLightContainer::size_type>(idx)];
}


RtLightHandle CLights::lightSource(
	bool isIlluminated, bool isGlobal, bool isArea,
	const char *name,
	const CParameterList &params)
{
	assert(m_lightsFactory != 0);

	try {
		m_lights.push_back(0);
	} catch(std::exception e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s: %s", "CLightSource::lightSource()", noNullStr(e.what()));
	}

	CLightSource *s = m_lightsFactory->newLightSource(
		isIlluminated, isGlobal, isArea,
		name,
		params);

	m_lights[m_lights.size()-1] = s;

	RtLightHandle handle = static_cast<RtLightHandle>(m_lights.size());
	// handle > 0
	s->handle(handle);
	
	return handle;
}

RtLightHandle CLights::lightSource(
	CDeclarationDictionary &dict, const CColorDescr &colorDescr,
	bool isIlluminated, bool isGlobal, bool isArea,
	const char *name,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	assert(m_lightsFactory != 0);

	try {
		m_lights.push_back(0);
	} catch(std::exception e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s: %s", "CLightSource::lightSource()", noNullStr(e.what()));
	}

	CLightSource *s = m_lightsFactory->newLightSource(
		dict, colorDescr,
		isIlluminated, isGlobal, isArea,
		name,
		n, tokens, params);

	m_lights[m_lights.size()-1] = s;

	RtLightHandle handle = static_cast<RtLightHandle>(m_lights.size());
	// handle > 0
	s->handle(handle);
	
	return handle;
}

CLights::~CLights()
{
	assert(m_lightsFactory != 0);

	for ( TypeLightContainer::iterator i = begin();
		i != end();
		i++)
	{
		if ( (*i) != 0 ) {
			m_lightsFactory->deleteLightSource((*i));
		}
	}
}

void CLights::mark()
{
	m_lightMarks.push_back(static_cast<unsigned long>(m_handles.size()));
	m_lightMarks.push_back(static_cast<unsigned long>(m_lights.size()));
}


void CLights::clearToMark()
{
	if ( !m_lights.empty() )
	{
		assert (!m_lightMarks.empty());

		TypeLightContainer::size_type lightsz =
			static_cast<TypeLightContainer::size_type>(m_lightMarks.back());

		assert(lightsz <= m_lights.size());
		
		for (
			TypeLightContainer::size_type i = lightsz;
			i < m_lights.size();
			++i )
		{
			m_lightsFactory->deleteLightSource(m_lights[i]);
		}
		m_lights.resize(lightsz);

		TypeLightHandleContainer::size_type lighthandlesz =
			static_cast<TypeLightHandleContainer::size_type>(m_lightMarks.back());

		assert(lighthandlesz <= m_handles.size());

		m_handles.resize(lighthandlesz);
		m_lightMarks.pop_back();
	}
}
