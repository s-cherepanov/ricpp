#ifndef _RICPP_RENDERCONTEXT_LIGHT_H
#define _RICPP_RENDERCONTEXT_LIGHT_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file light.h
 *  \brief Declaration of TLight light descriptors, stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parametermap.h"
#include "matrix3d.h"

//! Stores a description of a light source either TRi::LightSource() or TRi::AreaLightSource()
/*! TIntermediateRenderer stores all lights in a vector, the position of an instance in this vector
 *  is used as light handle.
 */
class TLight {
	bool m_isGlobalLight;            //!< true if the light source is created outside a world block
	bool m_isAreaLight;              //!< true if the light source is created via TRi::AreaLightSource()
	TMatrix3D m_CTM;                 //!< the composite transformation matrix used for the light source
	TParameterMap m_lightParameters; //!< Parameter list of the light source, contains the name of the light source
public:
	//! Default constructor initializes with empty values
	inline TLight() : m_isGlobalLight(false), m_isAreaLight(false) {}

	//! A constructor to initialize the instance with the parameters of a light source
	/*! \param decls Current declaration map
	 *  \param colorComps Components of a color value
	 *  \param isArea true, if an area light source, \sa m_isAreaLight
	 *  \param isArea true, if a global light source, \sa m_isGlobalLight
	 *  \param m composite Transformation matrix of the light, \sa m_CTM
	 *  \param name Name of the light source, e.g. TRi::RI_POINTLIGHT
	 *  \param n Number of token-value pairs (size of tokens and params)
	 *  \param tokens The tokens of the token-value pairs
	 *  \param params The values of the token-value pairs
     */
	TLight(
		TDeclarationMap &decls, RtInt colorComps,
		bool isArea, bool isGlobal,
		const TMatrix3D &m,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[]
	); 

	//! Copy constructor (deep copy)
	/*! \param light The light to copy from
	 */
	TLight(const TLight &light);

	//! Destructor frees all members
	~TLight();

	//! Returns a copy of 'this'
	/*! \return Deep copy of the instance the member function is called for.
	 *  \exception TSystemException If there is not enough memory
	 */
	TLight &duplicate() const;

	//! Assignment (deep copy)
	/*! \param light The light to copy from
	 */
	TLight &operator=(const TLight &light);

	//! Frees all members
	void releaseAll();

	//! Fills the parameters of a TRi::LightSource() or TRi::AreaLightSource()
	/*! \param decls Current declaration map
	 *  \param colorComps Components of a color value
	 *  \param isArea true, if an area light source, \sa m_isAreaLight
	 *  \param isArea true, if a global light source, \sa m_isGlobalLight
	 *  \param m composite Transformation matrix of the light, \sa m_CTM
	 *  \param name Name of the light source, e.g. TRi::RI_POINTLIGHT
	 *  \param n Number of token-value pairs (size of tokens and params)
	 *  \param tokens The tokens of the token-value pairs
	 *  \param params The values of the token-value pairs
     */
	void lightSource(TDeclarationMap &decls, RtInt colorComps, bool isArea, bool isGlobal, const TMatrix3D &m, const char *name, RtInt n, RtToken tokens[], RtPointer params[]);

	//! Access the parameter list
	/* \return A reference to the parameter list of the light source
	 */
	inline TParameterMap &lightParameters() { return m_lightParameters; }

	//! Access the CTM
	/* \return A reference to the CTM of the light source
	 */
	inline TMatrix3D &transformation() { return m_CTM; }

	//! An area light source?
	/* \return true if the light source is created via TRi::AreaLightSource()
	 */
	inline bool areaLight() const { return m_isAreaLight; }

	//! Change the area light source flag.
	/* \param isArea The new area light source flag
	 * \return The previous area light source flag
	 */
	inline bool areaLight(bool isArea) { bool b = m_isAreaLight; m_isAreaLight = isArea; return b; }

	//! A global light source?
	/* \return true if a global source is stored by this instance
	 */
	inline bool globalLight() const { return m_isGlobalLight; }

	//! Change the global light source flag.
	/* \param isGlobal The new global light source flag
	 * \return The previous global light source flag
	 */
	inline bool globalLight(bool isGlobal) { bool b = m_isGlobalLight; m_isGlobalLight = isGlobal; return b;  }
}; // TLight

#endif // _RICPP_RENDERCONTEXT_LIGHT_H
