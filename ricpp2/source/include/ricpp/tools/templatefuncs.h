#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#define _RICPP_TOOLS_TEMPLATEFUNCS_H

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

/** @file templatefuncs.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Small utility templates.
 */

namespace RiCPP {

/** @brief Returns the maximum of two elements.
 *  @param a First element
 *  @param b Second element
 *  @return Maximum of a and b
 */
template<typename _T>
_T tmax(
	_T a,
	_T b)
{
	return a > b ? a : b;
}


/** @brief Returns the maximum of a vector.
 *  @param n The index type of the vector
 *  @param v The vector
 *  @return Maximum found in v
 */
template<typename _IT, typename _T>
_T tmax(
	_IT n,
	const _T *v)
{
	_T result = 0;
	if (n > 0) {
		result = v[--n];
		while ( n-- ) {
			if ( v[n] > result )
				result = v[n];
		}
	}
	return result;
}


//! Returns the minimum of two elements
/*! @param a First element
 *  @param b Second element
 *  @return Minimum of a and b
 */
template<class _T>
_T tmin(
	_T a,
	_T b)
{
	return a < b ? a : b;
}


//! Returns the minimum of a vector
/*! @param n The index type of the vector
 *  @param v The vector
 *  @return Minimum found in v
 */
template<typename _IT, typename _T>
_T tmin(
	_IT n,
	const _T *v)
{
	_T result = 0;
	if (n > 0) {
		result = v[--n];
		while ( n-- ) {
			if ( v[n] < result )
				result = v[n];
		}
	}
	return result;
}

/** @brief Returns the sum of the elements in a vector
 *  @param n The index type of the vector
 *  @param v The vector
 *  @return Sum of elements of v
 */
template<typename _IT, typename _T>
_T sum(
	_IT n,
	const _T *v)
{
	_T result = 0;
	while ( n-- > 0 ) {
		result += v[n];
	}
	return result;
}

//! Linear interpolation
template<typename _C> _C lerp(
	_C u,
	_C minu,
	_C maxu)
{
	return minu + u * (maxu - minu);
}

//! Bilinear interpolation
template<typename _C>
_C bilerp(
	_C u,
	_C v,
	_C x1,
	_C x2,
	_C x3,
	_C x4)
{
	return lerp(v, lerp(u, x1, x2), lerp(u, x3, x4));
}

}

#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H
