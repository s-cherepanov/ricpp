#include "ricpp/renderstate/lights.h"

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

	m_lightParameters.set(CValueCounts(), dict, colorDescr, name, n, tokens, params);
}

CLightSource *CLightSourceFactory::newLightSource(const char *name)
{
	CLightSource *l = new CLightSource(name);
	if ( !l ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::newLightSource()");
	}

	return l;
}

CLightSource *CLightSourceFactory::newLightSource(
	CDeclarationDictionary &dict, const CColorDescr &colorDescr,
	bool isIlluminated, bool isGlobal, bool isArea,
	const char *name,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	CLightSource *l = newLightSource();
	if ( !l ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::newLightSource()");
	}

	l->lightSource(
		dict, colorDescr,
		isIlluminated, isGlobal, isArea,
		name,
		n, tokens, params);
	return l;
}

const CLightSource *CLights::getLight(RtLightHandle l) const
{
	if ( l >= m_lights.size() )
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::insertLight(), Handle undefined");
	return m_lights[static_cast<LightContainer::size_type>(l)];
}

CLightSource *CLights::getWriteableLight(RtLightHandle l)
{
	if ( l >= m_lights.size() )
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::insertLight(), Handle undefined");
	return m_lights[static_cast<LightContainer::size_type>(l)];
}


RtLightHandle CLights::newLightHandleIdx()
{
	try {
		m_handles.push_back(0);
	} catch(std::exception e) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s: %s", "CLightSource::newLightHandle()", noNullStr(e.what()));
	}

	RtLightHandle l = static_cast<RtLightHandle>(m_handles.size()-1);
	return l;
}

void CLights::setHandle(RtLightHandle idx, RtLightHandle handle)
{
	if ( idx >= m_handles.size() )
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::setHandle(), Handle idx undefined");
	if ( handle >= m_lights.size() )
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::setHandle(), Light handle undefined");
	m_handles[static_cast<LightContainer::size_type>(idx)] = handle;
}

RtLightHandle CLights::getHandle(RtLightHandle idx)
{
	if ( idx >= m_handles.size() )
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "CLightSource::getHandle(), Handle idx undefined");
	return m_handles[static_cast<LightContainer::size_type>(idx)];
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
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s: %s", "CLightSource::lightSource()", noNullStr(e.what()));
	}

	CLightSource *s = m_lightsFactory->newLightSource(
		dict, colorDescr,
		isIlluminated, isGlobal, isArea,
		name,
		n, tokens, params);

	m_lights[m_lights.size()-1] = s;

	RtLightHandle l = static_cast<RtLightHandle>(m_lights.size()-1);
	return l;
}

CLights::~CLights()
{
	assert(m_lightsFactory != 0);

	for ( LightContainer::iterator i = begin();
		i != end();
		i++)
	{
		if ( (*i) != 0 ) {
			m_lightsFactory->deleteLightSource((*i));
		}
	}
}
