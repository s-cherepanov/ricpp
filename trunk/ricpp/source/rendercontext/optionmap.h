#ifndef _RICPP_RENDERCONTEXT_OPTIONMAP_H
#define _RICPP_RENDERCONTEXT_OPTIONMAP_H

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

/*! \file optionmap.h
 *  \brief Declates TOptionMap, maps the option name to an option value (RiOption calls).
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parametermap.h"
#include "tools/helpers.h"

////////////////////////////////////////////////////////////////////////////////
/*! Used by TOptionMap to connect a name to the values of an TRi::option() or
 *  TRi::attribute() call.
 */
typedef std::map<std::string, TParameterMap *, std::less<std::string> > STRING2PARAMMAP;

////////////////////////////////////////////////////////////////////////////////
/*! Used to store implementation specific options (attributes) of
 *  TRi::option() or TRi::attribute().
 */
class TOptionMap {
	STRING2PARAMMAP m_parammaps; //!< The options, mapped by name.
public:

	//! The empty standard constructor.
	inline TOptionMap() {}
	//! The copy constructor (deep copy).
	/*! \exception TSystemException Thrown if not enough memory available (via duplicate()).
	 */
	TOptionMap(const TOptionMap &); 
	//! The destructor frees the option map.
	~TOptionMap();

	//! Returns a Deep copy of this.
	/*! \return A new instance with a copy of this.
	 *  \exception TSystemException Thrown if not enough memory available.
	 */
	TOptionMap &duplicate() const;

	//! Assigns another option map to this.
	/*! \param opts The optionmap to assign.
	 *  \return A reference to *this.
	 *  \exception TSystemException Thrown if not enough memory available, via TParameterMap::duplicate().
	 */
	TOptionMap &operator=(const TOptionMap &opts);

	//! Frees the resources used by m_parammap.
	void releaseAll();

	//! Gets the options for name.
	/*! \param name Name of the options to find.
	 *  \return The TParameterMap with the options stored orn NULL if options are not found.
	 */
	TParameterMap *getOption(const char *name) const;

	//! Gets an option value for option name.
	/*! \param name Name of the options to find.
	 *  \param param The name of the parameter to find in option name.
	 *  \return The option value for option name and parameter param.
	 */
	const RtPointer getValueOf(const char *name, const char *param) const;

	//! Sets options, used by TRi::option() or attributes by TRi::attribute()
	/*! \param decls Current declarations valid in renderer context.
	 *  \param colorComps Number of color components in renderer context.
	 *  \param name The option name.
	 *  \param n The number of token-value pairs.
	 *  \param tokens The tokens of the token-value pairs.
	 *  \param params The values of the token-value pairs.
	 *  \return true If values could get copied, false: otherwise.
	 *  \exception TSystemException Thrown if there is not enough memory.
	 */
	bool setOption(TDeclarationMap &decls, RtInt colorComps, const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
}; // TOptionMap

#endif // _RICPP_RENDERCONTEXT_OPTIONMAP_H
