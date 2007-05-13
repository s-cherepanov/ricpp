#ifndef RICPP_RICPP_MODES_H
#define RICPP_RICPP_MODES_H

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

/** @file modes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Constants (enumerations) for the various modes
 */

namespace RiCPP {

/** @brief Possible modes
 *
 * Blocks of the renderer, e.g MODE_BEGIN for RiBegin()/RiEnd()
 * (outside: no block is open)
 */
enum EnumModes {
	MODE_OUTSIDE= 0,    //!< Index outside Begin/End

	MODE_BEGIN,		    //!< Index directly inside Begin/End
	MODE_FRAME,		    //!< Index directly inside frame block
	MODE_WORLD,		    //!< Index directly inside world block

	MODE_ATTRIBUTE,	    //!< Index directly inside attribute block
	MODE_TRANSFORM,	    //!< Index directly inside transformation block
	MODE_SOLID,		    //!< Index directly inside solid block
	MODE_MOTION,		    //!< Index directly inside motion block

	MODE_OBJECT		    //!< Index directly inside object block
};

/** Number of modes
 */
const int N_MODES = (int)MODE_OBJECT+1;

/** Bits for sets of modes
 */
enum EnumModeBits {
	MODE_BIT_OUTSIDE   =   1, //!< Bit for outside Begin/End

	MODE_BIT_BEGIN     =   2, //!< Bit for directly inside Begin/End
	MODE_BIT_FRAME     =   4, //!< Bit for directly inside frame block
	MODE_BIT_WORLD     =   8, //!< Bit for directly inside world block

	MODE_BIT_ATTRIBUTE =  16, //!< Bit for directly inside attribute block
	MODE_BIT_TRANSFORM =  32, //!< Bit for directly inside transform block
	MODE_BIT_SOLID     =  64, //!< Bit for directly inside solid block
	MODE_BIT_MOTION    = 128, //!< Bit for directly inside motion block

	MODE_BIT_OBJECT    = 256  //!< Bit for directly inside object block
};

} // namespace RiCPP

#endif // RICPP_RICPP_MODES_H