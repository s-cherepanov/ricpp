#ifndef _RICPP_RENDERCONTEXT_PARAMETERMAP_H
#define _RICPP_RENDERCONTEXT_PARAMETERMAP_H

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

/*! \file parametermap.h
 *  \brief Declaration of a container (TParameterMap) for a list
 *         of parameters (TParameter) of a function call.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parameter.h"
#include "declarationmap.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
/*! The map to store a parameter values for a parameter name.
 */
typedef std::map<std::string, TParameter> STRING2PARAM;

////////////////////////////////////////////////////////////////////////////////
/*! Holds the parameters (TParameter) of an interface call in a map.
 */
class TParameterMap
{
	std::string m_mapName;					//!< Optional parameter name (used by TOptionMap, TLightSource).
	STRING2PARAM m_parameters;				//!< Map of parameters.

	std::vector<RtToken> m_tokensVect;      //!< Original sequence of tokens.
	std::vector<RtPointer> m_paramsVect;    //!< Original sequence of values.

	RtVoid fillTokensParams(IndexType n);       //!< Fills m_tokens and m_parameters.

public:
	inline TParameterMap() {}               //!< Standard constructor, just to have one.
	TParameterMap(const TParameterMap &);   //!< Copy constructor, copies a parameter map.
	~TParameterMap();                       //!< Destructor clean up.

	TParameterMap &duplicate() const;                   //!< Duplicates an instance of the TParameterMap (deep copy).
	TParameterMap &operator=(const TParameterMap &);    //!< Assignment of an instance (deep copy).

	void releaseAll();                      //!< frees all memory.

	bool set(TDeclarationMap &decls, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]);	//!< Refills an instance (old data is discarded).
	bool add(TDeclarationMap &decls, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]);	//!< Adds the parameters to the instance.

	const TParameter *getParameterOf(const char *name) const;   //!< Returns a parameter for a specific name.
	const TParameter *getParameterOf(IndexType position) const; //!< Returns a parameter for a specific name, NULL if position >= size.
	
	const RtPointer getValueOf(const char *name) const;         //!< Returns a value for a specific name.

	void setName(const char *name);         //!< Sets the name of the parameter map.
	const char *getName() const;            //!< Returns the name of the parameter map.

	//! Gets size of token value list.
	/*! \return The size of token value list (m_tokensVect.size(), because m_tokensVect.size()== m_paramsVect.size())
	 */
	inline IndexType getSize() const { return (RtInt)m_tokensVect.size(); }

	//! Gets list of tokens in original order.
	/*! \return The tokens of the token value list.
	 */
	inline const RtToken *getTokens() const { return m_tokensVect.size() ? &(m_tokensVect[0]) : NULL; }

	//! Gets list of values in original order.
	/*! \return The values of the token value list.
	 */
	inline const RtPointer *getParams() const { return m_paramsVect.size() ? &(m_paramsVect[0]) : NULL; }
}; // TParameterMap

#endif // _RICPP_RENDERCONTEXT_PARAMETERMAP_H
