#ifndef _RICPP_RENDERCONTEXT_DISPLAYDESC_H
#define _RICPP_RENDERCONTEXT_DISPLAYDESC_H

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

/*! \file displaydesc.h
 *  \brief Declaration of the TDisplayDesc display descriptor
 *         (stores contents of a RiDisplay call)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parametermap.h"

////////////////////////////////////////////////////////////////////////////////
//! A display descriptor to store and access standard TRi::display() parameters.
/*! The parameters are stored, because there can be multiple displays (e.g.
 *  a file with depth coordinates only and a framebuffer showing RGBA values.
 */
class TDisplayDesc {
public:
	std::string  m_name; //!< Literal name of the display, default is empty
	RtToken m_type;      //!< Type of the display, e.g. TRi::RI_FILE, TRi::RI_FRAMEBUFFER, default is TRi::RI_NULL
	RtToken m_mode;      //!< Mode of the output, e.g. TRi::RI_RGBA etc., default si TRi::RI_NULL

	RtInt m_origin[2];   //!< Origin (offset from upper left), default is (0, 0)

	TParameterMap m_parameterMap; //!< All paramters of the interface call, default is empty

	TDisplayDesc(); //!< Standard Constructor, fills in default values
	//! constructor for simple parameters
	/*! \param name Name of the display, \sa m_name
	 *  \param type The display type, \sa m_type
	 *  \param mode The display mode, \sa m_mode
	 *  \param x0 x origin (x offset from upper left)
	 *  \param y0 y origin (y offset from upper left)
	 */
	TDisplayDesc(RtToken name, RtToken type, RtToken mode, RtInt x0, RtInt y0);
	//! Copy constructor for deep copy
	/*! \param tdd Display description to copy
	 */
	TDisplayDesc(const TDisplayDesc &tdd);
	inline ~TDisplayDesc() {} //!< Destructor is empty at the moment

	TDisplayDesc &duplicate() const; //!< Duplication of an instances
	 //! Assignment, deep copy.
	/*! \param tdd Display description to copy
	 */
	TDisplayDesc &operator=(const TDisplayDesc &tdd);

	//! Take parameters of TRi::displayV() to fill the member variables.
	/*! \param name Name of the display, \sa m_name
	 *  \param type The display type, \sa m_type
	 *  \param mode The display mode, \sa m_mode
	 *  \param x0 x origin (x offset from upper left)
	 *  \param y0 y origin (y offset from upper left)
	 */
	bool displayV(TDeclarationMap &decls, RtInt colorComps, RtToken name, RtToken type, RtToken mode, RtToken origin, RtInt n, RtToken tokens[], RtPointer params[]);
}; // TDisplayDesc

#endif // _RICPP_RENDERCONTEXT_DISPLAYDESC_H
