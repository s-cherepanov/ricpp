#ifndef _RICPP_RENDERSTATE_LIGHTS_H
#define _RICPP_RENDERSTATE_LIGHTS_H

/*! \file light.h
 *  \brief Declaration of CLightSource light descriptors, stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RENDERSTATE_PARAMETER_H
#include "ricpp/renderstate/parameter.h"
#endif // _RICPP_RENDERSTATE_PARAMETER_H

#include <deque>

namespace RiCPP {

//! Stores a description of a light source either TRi::LightSource() or TRi::AreaLightSource()
/*! TIntermediateRenderer stores all lights in a vector, the position of an instance in this vector
 *  is used as light handle.
 */
class CLightSource {
	bool m_isIlluminated;            //!< true, if the light source is on
	bool m_isGlobalLight;            //!< true, if the light source is created outside a world block
	bool m_isAreaLight;              //!< true, if the light source is created via TRi::AreaLightSource()

	CNamedParameterList m_lightParameters; //!< Parameter list of the light source, contains the name of the light source
public:
	//! Default constructor initializes with empty values
	inline CLightSource(const char *name=0)
		: m_isIlluminated(false),
		  m_isGlobalLight(false), m_isAreaLight(false),
		  m_lightParameters(name)
	{
	}

	//! Copy constructor (deep copy)
	/*! \param light The light to copy from
	 */
	CLightSource(const CLightSource &light);

	//! Destructor frees all members
	~CLightSource();

	//! Returns a copy of 'this'
	/*! \return Deep copy of the instance the member function is called for.
	 *  \exception TSystemException If there is not enough memory
	 */
	CLightSource &duplicate() const;

	//! Assignment (deep copy)
	/*! \param light The light to copy from
	 */
	CLightSource &operator=(const CLightSource &light);

	//! Fills the parameters of a TRi::LightSource() or TRi::AreaLightSource()
	/*! \param dict Current declaration dictionary
	 *  \param colorComps Components of a color value
	 *  \param isIluminated true, if light source is on (illuminated), \sa m_isIlluminated
	 *  \param isGlobal true, if a global light source, \sa m_isGlobalLight
	 *  \param isArea true, if an area light source, \sa m_isAreaLight
	 *  \param name Name of the light source, e.g. TRi::RI_POINCLightSource
	 *  \param n Number of token-value pairs (size of tokens and params)
	 *  \param tokens The tokens of the token-value pairs
	 *  \param params The values of the token-value pairs
     */
	void lightSource(
		CDeclarationDictionary &dict, const CColorDescr &colorDescr,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]);

	//! Access the parameter list
	/* \return A reference to the parameter list of the light source
	 */
	inline const CNamedParameterList &lightParameters() const { return m_lightParameters; }

	//! Access the parameter list
	/* \return A reference to the parameter list of the light source (writeable)
	 */
	inline CNamedParameterList &lightParametersWriteable() { return m_lightParameters; }

	//! Is the light source on?
	inline bool illuminated() const { return m_isIlluminated; }

	//! Switch the light source on/off
	inline void illuminated(bool isIlluminated) { m_isIlluminated = isIlluminated; }

	//! A global light source?
	/* \return true if a global source is stored by this instance
	 */
	inline bool globalLight() const { return m_isGlobalLight; }

	//! Change the global light source flag.
	/* \param isGlobal The new global light source flag
	 */
	inline void globalLight(bool isGlobal) { m_isGlobalLight = isGlobal; }

	//! An area light source?
	/* \return true if the light source is created via TRi::AreaLightSource()
	 */
	inline bool areaLight() const { return m_isAreaLight; }

	//! Change the area light source flag.
	/* \param isArea The new area light source flag
	 */
	inline void areaLight(bool isArea) { m_isAreaLight = isArea; }

}; // CLightSource


class CLightSourceFactory {
public:
	inline CLightSourceFactory() {}
	inline virtual ~CLightSourceFactory() {}

	virtual CLightSource *newLightSource(const char *name=0);

	virtual CLightSource *newLightSource(
		CDeclarationDictionary &dict, const CColorDescr &colorDescr,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]);

	inline virtual void deleteLightSource(CLightSource *l)
	{
		delete l;
	}
}; // CLightSourceFactory

class ILightsReader {
public:
	typedef std::deque<CLightSource *> LightContainer;
	typedef std::deque<RtLightHandle> LightHandleContainer;

	inline virtual ~ILightsReader() {}
	virtual const CLightSource *getLight(RtLightHandle l) const = 0;
	virtual LightContainer::const_iterator begin() const = 0;
	virtual LightContainer::const_iterator end() const = 0;
	virtual LightContainer::size_type size() const = 0;
};

class CLights : public ILightsReader {
private:
	LightHandleContainer m_handles;
	LightContainer m_lights; ///! Ptr to List, Index is the RtLightHandle
	CLightSourceFactory *m_lightsFactory;     ///< Create new lights

public:
	inline CLights(CLightSourceFactory &lightsFactory)
	{
		m_lightsFactory = &lightsFactory;
	}
	
	virtual ~CLights();

	RtLightHandle newLightHandleIdx();
	void setHandle(RtLightHandle idx, RtLightHandle handle);
	RtLightHandle getHandle(RtLightHandle idx);

	RtLightHandle lightSource(
		CDeclarationDictionary &dict, const CColorDescr &colorDescr,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]);

	virtual const CLightSource *getLight(RtLightHandle l) const;
	CLightSource *getWriteableLight(RtLightHandle l);

	virtual inline LightContainer::iterator begin() { return m_lights.begin(); }
	virtual inline LightContainer::iterator end() { return m_lights.end(); }
	virtual inline LightContainer::const_iterator begin() const { return m_lights.begin(); }
	virtual inline LightContainer::const_iterator end() const { return m_lights.end(); }
	virtual inline LightContainer::size_type size() const { return m_lights.size(); }
}; // CLights

}

#endif // _RICPP_RENDERSTATE_LIGHTS_H
