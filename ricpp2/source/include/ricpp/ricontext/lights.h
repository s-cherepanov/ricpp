#ifndef _RICPP_RICONTEXT_LIGHTS_H
#define _RICPP_RICONTEXT_LIGHTS_H

/*! \file light.h
 *  \brief Declaration of CLightSource light descriptors, stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

#include <deque>

namespace RiCPP {

/** @brief Stores a description of a light source either IRi::LightSource() or IRi::AreaLightSource().
 *
 *  A rendering context stores all lights in a CLights instance.
 */
class CLightSource {
	bool m_isIlluminated;            //!< true, if the light source is on
	bool m_isGlobalLight;            //!< true, if the light source is created outside a world block
	bool m_isAreaLight;              //!< true, if the light source is created via TRi::AreaLightSource()

	CNamedParameterList m_lightParameters; //!< Parameter list of the light source, contains the name of the light source
	RtLightHandle m_handle;
	
public:
	//! Default constructor initializes with empty values
	inline CLightSource(const char *name=0)
		: m_isIlluminated(false),
		  m_isGlobalLight(false), m_isAreaLight(false),
		  m_lightParameters(name), m_handle(0)
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

	RtLightHandle handle() const
	{
		return m_handle;
	}

	void handle(RtLightHandle h)
	{
		m_handle = h;
	}

}; // CLightSource


/** @brief Factory allows to create customized light sources.
 */
class CLightSourceFactory {
public:
	inline CLightSourceFactory() {}
	inline virtual ~CLightSourceFactory() {}

	virtual CLightSource *newLightSource(const char *name=0) const;

	virtual CLightSource *newLightSource(
		CDeclarationDictionary &dict, const CColorDescr &colorDescr,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]) const;

	inline virtual void deleteLightSource(CLightSource *l)
	{
		delete l;
	}

	virtual CLightSource *newObject() const
	{
		return newLightSource(0);
	}
}; // CLightSourceFactory


/** @brief Container for the light sources of a rendering context
 */
class CLights {
public:
	typedef std::deque<CLightSource *> TypeLightContainer;
	typedef std::deque<RtLightHandle> TypeLightHandleContainer;

private:
	/** @brief Indirection for light handles
	 *
	 * m_handles contains indizees for m_lights.
	 * A RtLightHandle used as index for m_handles is negative to
	 * distinguish it from RtLightHandle for m_lights. index = -(RtLightHandle+1) for
	 * negative RtLightHandle.
	 * The indirection is needed because of archives (and objects). Inbetween archives
	 * no light instances exist, but may be referenced to get illuminated.
	 * Mind also, there can be many instances of one archive.
	 */
	TypeLightHandleContainer m_handles;
	TypeLightContainer m_lights; ///< Ptr to light sources (CLightSource), Index is a positive RtLightHandle (-1)
	CLightSourceFactory *m_lightsFactory; ///< Create new lights

	std::vector<unsigned long> m_lightMarks;
	/** @brief Transforms a handle to an index for m_lights, throws if out of range.
	 *
	 * @param handle \a handle can be negative (indirection) or postive (Light handle)
	 * @return Index for m_lights
	 * @except ExceptRiCPPError Out of range
	 */
	TypeLightContainer::size_type handleToLightIndex(RtLightHandle handle) const;

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

	virtual const CLightSource *getLight(RtLightHandle handle) const;
	virtual CLightSource *getLight(RtLightHandle handle);

	void mark();
	void clearToMark();

	virtual inline TypeLightContainer::iterator begin() { return m_lights.begin(); }
	virtual inline TypeLightContainer::iterator end() { return m_lights.end(); }
	virtual inline TypeLightContainer::const_iterator begin() const { return m_lights.begin(); }
	virtual inline TypeLightContainer::const_iterator end() const { return m_lights.end(); }
	virtual inline TypeLightContainer::size_type size() const { return m_lights.size(); }
}; // CLights

}

#endif // _RICPP_RICONTEXT_LIGHTS_H
