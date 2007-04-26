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

/*! \file displaydesc.cpp
 *  \brief Implementation of the TDisplayDesc display descriptor
 *         (stores contents of a RiDisplay call)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "displaydesc.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TDisplayDesc

TDisplayDesc::TDisplayDesc() {
	m_name = "";
	m_type = TRi::RI_NULL;
	m_mode = TRi::RI_NULL;
	m_origin[0] = m_origin[1] = 0;
}


////////////////////////////////////////////////////////////////////////////////
/*! This constructor takes some common used parameters
 *  \param name       Token with name of the
 *  \param type       Token with type of the display
 *  \param mode       Token with the display mode
 *  \param x0         X coordinate of the origin
 *  \param y0         Y coordinate of the origin
 */
TDisplayDesc::TDisplayDesc(RtToken name, RtToken type, RtToken mode, RtInt x0, RtInt y0) {
	m_name = name;
	m_type = type;
	m_mode = mode;
	m_origin[0] = x0;
	m_origin[1] = y0;
}


////////////////////////////////////////////////////////////////////////////////
/*! \param tdd instance to copy from
 */
TDisplayDesc::TDisplayDesc(const TDisplayDesc &tdd) {
	m_name = "";
	m_type = TRi::RI_NULL;
	m_mode = TRi::RI_NULL;
	m_origin[0] = m_origin[1] = 0;
	*this = tdd;
}


////////////////////////////////////////////////////////////////////////////////
/*! \return A new instance with a deep copy of 'this'.
 */
TDisplayDesc &TDisplayDesc::duplicate() const {
	TDisplayDesc *disp = new TDisplayDesc(*this);
	if ( !disp )
		throw TSystemException(__FILE__, __LINE__);
	return *disp;
}


////////////////////////////////////////////////////////////////////////////////
/*! \param tdd instance to assign from
 *  \return A reference of '*this"
 */
TDisplayDesc &TDisplayDesc::operator=(const TDisplayDesc &tdd) {
	if ( &tdd == this )
		return *this;
	m_name = tdd.m_name;
	m_type = tdd.m_type;
	m_mode = tdd.m_mode;
	m_origin[0] = tdd.m_origin[0];
	m_origin[1] = tdd.m_origin[1];
	m_parameterMap = tdd.m_parameterMap;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*! Fills a TDisplayDesc instance with a display description given in
 *  a TRi::displayV() call
 *
 *  \param decls      Map of declarations of the current render context
 *  \param colorComps Number of components per color
 *  \param name       Token with name of the
 *  \param type       Token with type of the display
 *  \param mode       Token with the display mode
 *  \param n          Number of additional parameters
 *  \param origin     The token RI_ORIGIN
 *  \param tokens     Tokens of the additional parameters
 *  \param params     Values of the parameters
 *  \return false if parameters could not be parsed, true if ok
 */
bool TDisplayDesc::displayV(TDeclarationMap &decls, RtInt colorComps, RtToken name, RtToken type, RtToken mode, RtToken origin, RtInt n, RtToken tokens[], RtPointer params[]) {
	m_name = name;
	m_type = decls.getToken(type);
	m_mode = decls.getToken(mode);
	m_origin[0] = 0;
	m_origin[1] = 0;
	if ( !m_parameterMap.set(decls, 0, 0, 0, 0, 0, colorComps, n, tokens, params) )
		return false;
	RtInt *p = (RtInt *)m_parameterMap.getValueOf(origin);
	if ( p ) {
		m_origin[0] = p[0];
		m_origin[1] = p[1];
	}
	return true;
}
