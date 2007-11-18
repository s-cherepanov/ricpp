#ifndef _RICPP_RICPP_RICPPTYPES_H
#define _RICPP_RICPP_RICPPTYPES_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:	/** @brief Defines a new display channel for display()
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

/** @file ricpptypes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Rt type definitions
 */
 namespace RiCPP {

/** @defgroup ricpp_type Ri types
 *  @brief Some basic types are copied from ri.h (s.a. RISPEC3.2, Appendix C)
 *  @{
 */
typedef short   RtBoolean; ///< Booleans can be RI_TRUE or RI_FALSE
typedef int     RtInt;     ///< Integers
typedef float   RtFloat;   ///< Floats

typedef const char *RtToken;     ///< Tokens, changed from 'char *' to 'const char *', Tokens are either pre-declared or declared with IRi::declare()

typedef RtFloat *RtColor;        ///< Colors, changed from 'RtFloat[3]' to 'RtFloat *' to manage color using other than 3 components
typedef RtFloat  RtPoint[3];     ///< Points in 3D
typedef RtFloat  RtVector[3];    ///< Vectors in 3D
typedef RtFloat  RtNormal[3];    ///< Normals in 3D
typedef RtFloat  RtHpoint[4];    ///< Points in homgenous 3D
typedef RtFloat  RtMpoint[4][4]; ///< 'Map Points' introduced in App Note #31, used by CRiCPP::blobbyV() (added)
typedef RtFloat  RtMatrix[4][4]; ///< Homogenous matrix for 3D transformations
typedef RtFloat  RtBasis[4][4];  ///< Base matrix for spline interpolations
typedef RtFloat  RtBound[6];     ///< Bound of 3D object: Left, right, bottom, top, front, back
typedef const char *RtString;    ///< Character string, which is not a predefined token, changed from 'char *' to 'const char *'

typedef void   *RtPointer;       ///< Pointer to arbitrary data
#define RtVoid  void             ///< Simple 'void' datatype, used as 'return type' for functions returning nothing (C++: typedef is not working for void)

/** @defgroup ricpp_handles Ri handle types
 *  @ingroup ricpp_types
 *  @brief handles are only valid for the rendering context, where they are defined
 *  @see ricpp_illhandle
 *  @{
 */
typedef long RtLightHandle;            ///< Handle for a light handle (was RtPointer) can be positive or negative, see CLights.
typedef unsigned long RtContextHandle; ///< Handle for a render context handle (was RtPointer).
typedef RtToken RtObjectHandle;        ///< Handle for an object instance (was RtPointer) can be positive or negative.
typedef RtToken RtArchiveHandle;       ///< Handle for a rib archive (was RtPointer), is the tokenized name.
//@}
//@}

} // namespace RiCPP

#endif // _RICPP_RICPP_RICPPTYPES_H
