#ifndef _RICPP_RICONTEXT_LIGHTS_H
#define _RICPP_RICONTEXT_LIGHTS_H

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

/** @file lights.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of CLightSource light descriptors.
 *
 *  Stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls).
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

#ifndef _RICPP_RICPP_RICPPCONST_H
#include "ricpp/ricpp/ricppconst.h"
#endif // _RICPP_RICPP_RICPPCONST_H

#include <deque>

namespace RiCPP {

/** @brief Stores a description of a light source either IRi::LightSource() or IRi::AreaLightSource().
 *
 *  A rendering context stores all lights in a CLights instance.
 */
class CLightSource {
	bool m_isIlluminated;            ///< true, if the light source is on
	bool m_isGlobalLight;            ///< true, if the light source is created outside a world block
	bool m_isAreaLight;              ///< true, if the light source is created via TRi::AreaLightSource()

	RtLightHandle m_handle;          ///< Handle as identifier
	CNamedParameterList m_lightParameters; ///< Parameter list of the light source, contains the name of the light source
	
public:
	//! Default constructor initializes with empty values
	inline CLightSource()
		: m_isIlluminated(false),
		  m_isGlobalLight(false), m_isAreaLight(false),
		  m_handle(illLightHandle), m_lightParameters(0)
	{
	}

	//! Constructor, fills the parameters
	/*! @param dict Current declaration dictionary.
	 *  @param colorDescr Current color descriptor.
	 *  @param isIlluminated true, if light source is on (illuminated), @see m_isIlluminated
	 *  @param isGlobal true, if a global light source, @see m_isGlobalLight
	 *  @param isArea true, if an area light source, @see m_isAreaLight
	 *  @param name Name of the light source, e.g. TRi::RI_POINCLightSource
	 *  @param n Number of token-value pairs (size of tokens and params)
	 *  @param tokens The tokens of the token-value pairs
	 *  @param params The values of the token-value pairs
     */
	CLightSource(
		CDeclarationDictionary &dict, const CColorDescr &colorDescr,
		RtLightHandle aHandle,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]);

	CLightSource(
		RtLightHandle aHandle,
		bool isIlluminated, bool isGlobal, bool isArea,
		const char *name,
		const CParameterList &params);

	//! Copy constructor (deep copy)
	/*! @param light The light to copy from
	 */
	CLightSource(const CLightSource &light);

	//! Destructor frees all members
	virtual ~CLightSource();

	//! Returns a copy of 'this'
	/*! @return Clone of *this.
	 */
	CLightSource *CLightSource::duplicate() const;

	//! Assignment (deep copy)
	/*! @param light The light to copy from
	 */
	CLightSource &operator=(const CLightSource &light);

	//! Access the parameter list
	/* @return A reference to the parameter list of the light source
	 */
	inline const CNamedParameterList &lightParameters() const { return m_lightParameters; }

	//! Access the parameter list
	/* @return A reference to the parameter list of the light source (writeable)
	 */
	inline CNamedParameterList &lightParametersWriteable() { return m_lightParameters; }

	//! Is the light source on?
	inline bool illuminated() const { return m_isIlluminated; }

	//! Switch the light source on/off
	inline void illuminated(bool isIlluminated) { m_isIlluminated = isIlluminated; }

	//! A global light source?
	/* @return true if a global source is stored by this instance
	 */
	inline bool globalLight() const { return m_isGlobalLight; }

	//! Change the global light source flag.
	/* @param isGlobal The new global light source flag
	 */
	inline void globalLight(bool isGlobal) { m_isGlobalLight = isGlobal; }

	//! An area light source?
	/* @return true if the light source is created via TRi::AreaLightSource()
	 */
	inline bool areaLight() const { return m_isAreaLight; }

	//! Change the area light source flag.
	/* @param isArea The new area light source flag
	 */
	inline void areaLight(bool isArea) { m_isAreaLight = isArea; }

	inline const char *name() const
	{
		return m_handle;
	}

	inline RtLightHandle handle() const
	{
		return m_handle;
	}

	inline void handle(RtLightHandle h)
	{
		m_handle = h;
	}

}; // CLightSource

}

#endif // _RICPP_RICONTEXT_LIGHTS_H
