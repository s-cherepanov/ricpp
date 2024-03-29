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

#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif

#include <cmath>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace RiCPP {

	/** @brief Gets the maximum of two elements.
	 *  @param a First element
	 *  @param b Second element
	 *  @return Maximum of a and b
	 */
	template<typename _T>
	inline _T tmax(
		_T a,
		_T b)
	{
		return a > b ? a : b;
	}


	/** @brief Gets the maximal value out of a vector.
	 *  @param n The size of the vector
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


	/** @brief Gets the minimum of two elements.
	 *  @param a First element
	 *  @param b Second element
	 *  @return Minimum of a and b
	 */
	template<typename _T>
	inline _T tmin(
		_T a,
		_T b)
	{
		return a > b ? b : a;
	}


	/** @brief Gets the minimal value out of a vector.
	 *  @param n The size of the vector
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

	/** @brief Gets the sum of the elements in a vector
	 *  @param n The size of the vector
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

	/** @brief Validates min <= max, swaps parameters if min > max
	 *  @param min Minimum
	 *  @param max Maximum
	 *  @return false, numbers are swapped
	 */
	template<typename type> inline bool validateMinMax(type &min, type &max)
	{
		if ( min > max ) {
			using std::swap;
			swap(min,max);
			return false;
		}
		return true;
	}
	
	/** @brief Clamps a value the values boundmin, boundmax.
	 *
	 *  The bounds are exchanged if @a boundmin > @a boundmax.
	 *
	 *  @param val value to clamp.
	 *  @param boundmin Size of the string buffer.
	 *  @param boundmax Number to convert
	 *  @return The clambed value (>= @a boundmin and <= @a boundmax)
	 */
	template<typename type> inline type clamp(type val, type boundmin, type boundmax)
	{
		validateMinMax(boundmin, boundmax);
		if ( val < boundmin )
			return boundmin;
		if ( val > boundmax )
			return boundmax;
		
		return val;
	}
	
	/** @brief Gets the inverse of a number
	 *  @param val Number to invert
	 *  @return 1 / @a val
	 */
	template<typename type> inline type invert(type val)
	{
		return static_cast<type>(1.0)/val;
	}
	
	/** @brief Gets the negative of a number
	 *  @param val Number negate
	 *  @return -@a val
	 */
	template<typename type> inline type negate(type val)
	{
		return static_cast<type>(-1.0)*val;
	}

	/** @brief Rounds a number (in special is tround(4.5) = 5.0, tround(-4.5) = -5.0)
	 *  @param val Number to round
	 *  @return Rounded number
	 */
	template<typename type> inline type tround(type val)
	{
		if ( val < 0 ) {
			type t = ceil(val);
			return (val-t > -0.5) ? t : floor(val);
		}
		
		type t = floor(val);
		return (val-t < 0.5) ? t : ceil(val);
	}
	
	
	/** @brief Gets a random number between 0 and 1
	 *  @return Random number
	 */
	template<typename type> inline type trand()
	{
		return static_cast<type>(rand()) / static_cast<type>(RAND_MAX);
	}
	
	/** @brief Gets a random number between -1 and +1
	 *  @return Random number
	 */
	template<typename type> inline type trand2()
	{
		// -1.0 ... 1.0
		return (trand<type>() - static_cast<type>(0.5)) * static_cast<type>(2.0);
	}
	
	/** @brief Gets the logarithm of a numerus x to the base b
	 *  @param b base
	 *  @param x numerus	 
	 *  @return log<b>(x)
	 */
	template<typename type> inline type logx(type b, type x)
	{
		return log10(x)/log10(b);
	}

	/** @brief Gets the binary logarithm of a numerus r
	 *  @param r numerus
	 *  @return log2(r)
	 */
	template<typename type> inline type log2(type r)
	{
		return logx(static_cast<type>(2), r);
	}
	
	/** @brief Number of segments of a NURBS curve
	 *
	 *  The number of the segments of a NURBS curve is the number of the
	 *  control point less the order of the curve plus 1.
	 *
	 *  @param ncpts Number of control points
	 *  @param order The order of the curve
	 *  @return Number of segmments
	 */
	template <typename type> inline type nuNumSegs(type ncpts, type order)
	{
		return 1 + ncpts - order;
	}
	
	
	
	/** @brief Gets the epsilon limit value
	 *
	 * eps is the smallest positive value eps+a != a
	 *
	 *  @return eps
	 */ 
	template <typename type> inline type eps() { return std::numeric_limits<type>::epsilon(); }

	/** @brief Gets pi times 2
	 *  @return pi*2
	 */ 
	template <typename type> inline type piTimes2() { return static_cast<type>(6.283185307179586476925286766559); }
	
	/** @brief Gets pi
	 *  @return pi
	 */ 
	template <typename type> inline type pi()       { return static_cast<type>(3.1415926535897932384626433832795); }
	
	/** @brief Gets pi divided by 2
	 *  @return pi/2
	 */ 
	template <typename type> inline type pi_2()     { return static_cast<type>(1.5707963267948966192313216916398); }
	
	/** @brief Gets pi divided by 4
	 *  @return pi/4
	 */ 
	template <typename type> inline type pi_4()     { return static_cast<type>(0.78539816339744830961566084581988); }
	
	/** @brief Gets pi divided by 180.0
	 *  @return pi/180.0
	 */ 
	template <typename type> inline type pi_180()   { return static_cast<type>(3.1415926535897932384626433832795/180.0); }

	/** @brief Gets 180.0 divided by pi
	 *  @return 180.0 / pi
	 */ 
	template <typename type> inline type _180_pi()  { return static_cast<type>(180.0/3.1415926535897932384626433832795); }

	/** @brief Convert degrees to radians
	 *  @param degree The degree value to convert
	 *  @return The radian value (degree * pi / 180.0)
	 */
	template <typename type> inline type deg2rad(type degree) {return degree * pi_180<type>();}
	
	/** @brief Convert radians to degrees
	 *  @param radian The radian value to convert
	 *  @return The degree value (radian * 180.0 / pi)
	 */
	template <typename type> inline type rad2deg(type radian) {return radian * _180_pi<type>();}
	
	template <typename type> inline bool inOpenInterval(type f, type min, type max) { validateMinMax(min, max); return f > min && f < max; }
	template <typename type> inline bool inClosedInterval(type f, type min, type max) { validateMinMax(min, max); return f >= min && f <= max; }
	template <typename type> inline bool inLeftClosedInterval(type f, type min, type max) { validateMinMax(min, max); return f >= min && f < max; }
	template <typename type> inline bool inRightClosedInterval(type f, type min, type max) { validateMinMax(min, max); return f > min && f <= max; }

	template <typename type> inline bool positive(type f) { return f >= 0; }
	template <typename type> inline bool negative(type f) { return f < 0; }
	template <typename type> inline bool nearlyZero(type f) { return inOpenInterval(f, -eps<type>(), eps<type>()); }
	template <typename type> inline bool nearlyEqual(type f1, type f2) { return nearlyZero(f1-f2); }
	
	/** @brief Get the sign of a value
	 *  @param f The value to test
	 *  @return -1 if f is negative, 1 if positive, 0 if zero
	 */
	template <typename type> inline type sign(type f)
	{
		return f < 0 ?
			static_cast<type>(-1) :
			(nearlyZero(f) ? static_cast<type>(0) : static_cast<type>(1));
	}

	template <typename _T> inline bool zeroVect(_T x, _T y)
	{
		return nearlyZero(x) && nearlyZero(y);
	}
	
	template <typename _T> inline bool zeroVect(_T x, _T y, _T z)
	{
		return nearlyZero(x) && nearlyZero(y) && nearlyZero(z);
	}
	
	template <typename _T> inline bool zeroVect2(const _T *v2)
	{
		return zeroVect(v2[0], v2[1]);
	}
	
	template <typename _T> inline bool zeroVect3(const _T *v3)
	{
		return zeroVect(v3[0], v3[1], v3[2]);
	}
	

	template <typename _T> inline bool eqVect2(const _T *v1, const _T *v2)
	{
		return nearlyZero(v2[0]-v1[0]) && nearlyZero(v2[1]-v1[1]);
	}

	template <typename _T> inline bool eqVect3(const _T *v1, const _T *v2)
	{
		return nearlyZero(v2[0]-v1[0]) && nearlyZero(v2[1]-v1[1]) && nearlyZero(v2[2]-v1[2]);
	}

	template <typename _T> inline _T *vectFromPos2(_T *v2, const _T *from2, const _T *to2)
	{
		v2[0] = to2[0] - from2[0];
		v2[1] = to2[1] - from2[1];
		return v2;
	}
	
	template <typename _T> inline _T *vectFromPos3(_T *v3, const _T *from3, const _T *to3)
	{
		v3[0] = to3[0] - from3[0];
		v3[1] = to3[1] - from3[1];
		v3[2] = to3[2] - from3[2];
		return v3;
	}
	
	template <typename _T> inline void vvAdd2(_T *res, const _T *v1, const _T *v2)
	{
		res[0] = v1[0] + v2[0];
		res[1] = v1[1] + v2[1];
	}

	template <typename _T> inline void vvAdd3(_T *res, const _T *v1, const _T *v2)
	{
		res[0] = v1[0] + v2[0];
		res[1] = v1[1] + v2[1];
		res[2] = v1[2] + v2[2];
	}

	template <typename _T> inline void vsAdd2(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] + s;
		res[1] = v[1] + s;
	}
	
	template <typename _T> inline void vsAdd3(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] + s;
		res[1] = v[1] + s;
		res[2] = v[2] + s;
	}
	
	template <typename _T> inline void vvSub2(_T *res, const _T *v1, const _T *v2)
	{
		res[0] = v1[0] - v2[0];
		res[1] = v1[1] - v2[1];
	}
	
	template <typename _T> inline void vvSub3(_T *res, const _T *v1, const _T *v2)
	{
		res[0] = v1[0] - v2[0];
		res[1] = v1[1] - v2[1];
		res[2] = v1[2] - v2[2];
	}
	
	template <typename _T> inline void vsSub2(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] - s;
		res[1] = v[1] - s;
	}
	
	template <typename _T> inline void vsSub3(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] - s;
		res[1] = v[1] - s;
		res[2] = v[2] - s;
	}
	
	template <typename _T> inline void vsMul2(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] * s;
		res[1] = v[1] * s;
	}
	
	template <typename _T> inline void vsMul3(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] * s;
		res[1] = v[1] * s;
		res[2] = v[2] * s;
	}

	template <typename _T> inline void vsDiv2(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] / s;
		res[1] = v[1] / s;
	}
	
	template <typename _T> inline void vsDiv3(_T *res, const _T *v, _T s)
	{
		res[0] = v[0] / s;
		res[1] = v[1] / s;
		res[2] = v[2] / s;
	}
	
	template <typename _T> inline _T vlen(_T x, _T y)
	{
		return static_cast<_T>(sqrt(x*x+y*y));
	}
	
	template <typename _T> inline _T vlen(_T x, _T y, _T z)
	{
		return static_cast<_T>(sqrt(x*x+y*y+z*z));
	}
	
	template <typename _T> inline _T vlen2(const _T *p2)
	{
		return static_cast<_T>(sqrt(p2[0]*p2[0] + p2[1]*p2[1]));
	}
	
	template <typename _T> inline _T vlen2(const _T *from2, const _T *to2)
	{
		_T v2[2];
		return vlen2(vectFromPos2(v2, from2, to2));
	}
	
	template <typename _T> inline _T vlen3(const _T *p3)
	{
		return static_cast<_T>(sqrt(p3[0]*p3[0]+p3[1]*p3[1]+p3[2]*p3[2]));
	}
	
	template <typename _T> inline _T vlen3(const _T *from3, const _T *to3)
	{
		_T v3[3];
		return vlen2(vectFromPos3(v3, from3, to3));
	}
	
	/** @brief Normalizes a 2D vector to size 1
	 *  @retval norm The normalized vector (size = 2)
	 *  @param x x component of a vector (can be a part of @a norm)
	 *  @param y y component of a vector (can be a part of @a norm)
	 */
	template <typename _T> inline void normalize(_T &x, _T &y)
	{
		const _T length = vlen(x, y);
		
		/*
		 if ( length == 0 ) {
		 x = y = 0;
		 return;
		 }
		 */
		
		x /= length;
		y /= length;
	}

	template <typename _T> inline void normalize(_T *norm, _T x, _T y)
	{
		norm[0] = x;
		norm[1] = y;
		normalize(norm[0], norm[1]);
	}
	
	
	template <typename _T> inline void normalize2(_T *v)
	{
		normalize(v[0], v[1]);
	}

	/** @brief Normalizes a 3D vector to size 1
	 *  @retval norm The normalized vector (size = 3)
	 *  @param x x component of a vector (can be a part of @a norm)
	 *  @param y y component of a vector (can be a part of @a norm)
	 *  @param z z component of a vector (can be a part of @a norm)
	 */
	template <typename _T> inline void normalize(_T &x, _T &y, _T &z)
	{
		const _T length = vlen(x, y, z);
		
		/*
		if ( length == 0 ) {
			x = y = z = 0;
			return;
		}
		*/
		
		x /= length;
		y /= length;
		z /= length;
	}
	
	template <typename _T> inline void normalize(_T *norm, _T x, _T y, _T z)
	{
		norm[0] = x;
		norm[1] = y;
		norm[2] = z;
		normalize(norm[0], norm[1], norm[2]);
	}

	template <typename _T> inline void normalize3(_T *v)
	{
		normalize(v[0], v[1], v[2]);
	}

	/** @brief Flips a 2D vector
	 */
	template <typename _T> inline void flip(_T &x, _T &y)
	{
		x *= (_T)-1;
		y *= (_T)-1;
	}

	template <typename _T> inline void flip(_T *v, _T x, _T y)
	{
		v[0] = x;
		v[1] = y;
		flip(v[0], v[1]);
	}
		
	template <typename _T> inline void flip2(_T *v)
	{
		flip(v[0], v[1]);
	}

	/** @brief Flips a 3D vector
	 */
	template <typename _T> inline void flip(_T &x, _T &y, _T &z)
	{
		x *= (_T)-1;
		y *= (_T)-1;
		z *= (_T)-1;
	}

	template <typename _T> inline void flip(_T *v, _T x, _T y, _T z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		flip(v[0], v[1], v[2]);
	}

	template <typename _T> inline void flip3(_T *v)
	{
		flip(v[0], v[1], v[2]);
	}

	/** @brief Determinant of a 2x2 matrix:
	 *
     @verbatim
     |v1[0] v2[0]|
     |v1[1] v2[1]|
     @endverbatim
	 *
	 *  @param v1 (Column) vector 1
	 *  @param v2 (Column) vector 2
	 *  @return Determinant
	 */
	template <typename _T> inline _T det2(const _T *v1, const _T *v2)
	{
		return v1[0]*v2[1] - v2[0]*v1[1];
	}
	
	/** @brief Determinant of a 3x3 matrix:
	 *
     @verbatim
     |v1[0] v2[0] v3[0]|
     |v1[1] v2[1] v3[1]|
     |v1[2] v2[2] v3[2]|
     @endverbatim
	 *
	 *  @param v1 (Column) vector 1
	 *  @param v2 (Column) vector 2
	 *  @param v3 (Column) vector 3
	 *  @return Determinant
	 */
	template <typename _T> inline _T det3(const _T *v1, const _T *v2, const _T *v3)
	{
		return v1[0] * (v2[1]*v3[2] - v3[1]*v2[2]) +
		       v2[0] * (v3[1]*v1[2] - v1[1]*v3[2]) +
		       v3[0] * (v1[1]*v2[2] - v2[1]*v1[2]);
	}
	
	/** @brief Scalar (inner, dot) product of two 2d vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot2(const _T *v1, const _T *v2)
	{
		return v1[0]*v2[0] + v1[1]*v2[1];
	}

	template <typename _T> inline _T dot2_norm(const _T *v1, const _T *v2)
	{
		_T nv1[2], nv2[2];
		
		normalize(nv1, v1[0], v1[1]);
		normalize(nv2, v2[0], v2[1]);
		
		return dot2(nv1, nv2);
	}
	
	template <typename _T> inline _T dot2_pos(const _T *p1, const _T *p2, const _T *p3)
	{
		_T v1[2], v2[2];
		vectFromPos2(v1, p2, p1);
		vectFromPos2(v2, p2, p3);
		return dot2(v1, v2);
	}

	template <typename _T> inline _T dot2_pos_norm(const _T *p1, const _T *p2, const _T *p3)
	{
		_T v1[2], v2[2];
		vectFromPos2(v1, p2, p1);
		normalize2(v1);
		vectFromPos2(v2, p2, p3);
		normalize2(v2);
		return dot2(v1, v2);
	}

	/** @brief Scalar product of two 2d vectors (v2 is rotated 90deg ccw for left right test)
	 *
	 *  Can be interpreted as the dot product of v1 and v2 rotated 90 degrees ccw.
	 *  It is also the determinant of the 2x2 matrix:
	 *
     @verbatim
     |v1[0] v2[0]|
     |v1[1] v2[1]|
     @endverbatim
	 *
	 * being the signed area of the parallelogram spanned by v1 and v2.
	 *
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2 (will be rotated 90 deg ccw)
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot2_90(const _T *v1, const _T *v2)
	{
		return det2(v1, v2);
	}

	template <typename _T> inline _T dot2_90_norm(const _T *v1, const _T *v2)
	{
		_T nv1[2], nv2[2];
		
		normalize(nv1, v1[0], v1[1]);
		normalize(nv2, v2[0], v2[1]);
		
		return dot2_90(nv1, nv2);
	}
	
	template <typename _T> inline _T dot2_90_pos(const _T *p_prev, const _T *p_mid, const _T *p_next)
	{
		_T v1[2], v2[2];
		vectFromPos2(v1, p_mid, p_prev);
		vectFromPos2(v2, p_mid, p_next);
		return dot2_90(v1, v2);
	}

	template <typename _T> inline _T dot2_90_pos_norm(const _T *p_prev, const _T *p_mid, const _T *p_next)
	{
		_T v1[2], v2[2];
		vectFromPos2(v1, p_mid, p_prev);
		normalize2(v1);
		vectFromPos2(v2, p_mid, p_next);
		normalize2(v2);
		return dot2_90(v1, v2);
	}

	/** @brief Scalar (inner, dot) product of two 3d vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot3(const _T *v1, const _T *v2)
	{
		return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	}
	
	template <typename _T> inline _T dot3_norm(const _T *v1, const _T *v2)
	{
		_T nv1[3], nv2[3];
		
		normalize(nv1, v1[0], v1[1], v1[2]);
		normalize(nv2, v2[0], v2[1], v2[2]);
		
		return dot3(nv1, nv2);
	}
	
	/** @brief Vector (outer, cross) product of two 3d vectors for a left handed system
	 *  @retval vp Vector product
	 *  @param vx Vector 1 (x-axis)
	 *  @param vy Vector 2 (y-axis)
	 */
	template <typename _T> inline void vprodLH(_T *vp, const _T *v1, const _T *v2)
	{
		_T result[3] = {0,0,0};
		if ( !(zeroVect3(v1) || zeroVect3(v1)) ) {
			result[0] = v1[1]*v2[2] - v1[2]*v2[1];
			result[1] = v1[2]*v2[0] - v1[0]*v2[2];
			result[2] = v1[0]*v2[1] - v1[1]*v2[0];
		}
		vp[0] = result[0];
		vp[1] = result[1];
		vp[2] = result[2];
	}

	template <typename _T> inline void vprodRH(_T *vp, const _T *v1, const _T *v2)
	{
		vprodLH(vp, v2, v1);
	}
	
	/*  @brief Scalar triple product of three 3d vectors
	 *  @retval vp Triple product
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @param v2 Vector 3
	 *//*
	template <typename _T> inline _T triprodLH(const _T *v1, const _T *v2, const _T *v3)
	{
		_T vp[3];
		vprodLH(vp, v2, v3);
		return dot3(v1, vp);
	}
	template <typename _T> inline _T triprodRH(const _T *v1, const _T *v2, const _T *v3)
	{
		_T vp[3];
		vprodLH(vp, v3, v2);
		return dot3(v1, vp);
	}
	*/
	

	/** @brief Flips a (normal) vector pointing in direction of the positive z axis
	 */
	template <typename _T> inline bool faceforward(_T &x, _T &y, _T &z)
	{
		_T vz[3] = { 0, 0, 1 }, v[3] = {x, y, z};
		if ( dot3(v, vz) > 0 ) {
			flip3(v);
			return true;
		}
		return false;
	}

	template <typename _T> inline bool faceforward(_T *v, _T x, _T y, _T z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		return faceforward(v[0], v[1], v[2]);
	}

	template <typename _T> inline _T faceforward(_T *v)
	{
		return faceforward(v[0], v[1], v[2]);
	}

	/** @brief Flips a (normal) vector pointing in direction of the negative z axis
	 */
	template <typename _T> inline bool facebackward(_T &x, _T &y, _T &z)
	{
		_T vz[3] = { 0, 0, 1 }, v[3] = {x, y, z};
		if ( dot3(v, vz) < 0 ) {
			flip3(v);
			return true;
		}
		return false;
	}
	
	template <typename _T> inline bool facebackward(_T *v, _T x, _T y, _T z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		return faceforward(v[0], v[1], v[2]);
	}
	
	template <typename _T> inline _T facebackward(_T *v)
	{
		return faceforward(v[0], v[1], v[2]);
	}
	
	template <typename _T> inline bool point2InTriangle(const _T *p, const _T *t1, const _T *t2, const _T *t3)
	{
		_T s1, s2, s3;
		_T v[2], vp[2];
		vectFromPos2<_T>(v, t1, t2);
		vectFromPos2<_T>(vp, t1, p);
		s1 = dot2_90<_T>(v, vp);
		vectFromPos2<_T>(v, t2, t3);
		vectFromPos2<_T>(vp, t2, p);
		s2 = dot2_90<_T>(v, vp);
		vectFromPos2<_T>(v, t3, t1);
		vectFromPos2<_T>(vp, t3, p);
		s3 = dot2_90<_T>(v, vp);
		return (sign(s1) >= 0 && sign(s2) >= 0 && sign(s3) >= 0) || (sign(s1) <= 0 && sign(s2) <= 0 && sign(s3) <= 0);
	}

	template <typename _T> inline bool point2InTriangle(const _T *p, const _T *t1, const _T *t2, const _T *t3, int &onEdge)
	{
		_T s1, s2, s3;
		_T v[2], vp[2];
		onEdge = 0;
		vectFromPos2<_T>(v, t1, t2);
		vectFromPos2<_T>(vp, t1, p);
		s1 = dot2_90<_T>(v, vp);
		if ( nearlyZero(s1) )
			onEdge |= 1;
		vectFromPos2<_T>(v, t2, t3);
		vectFromPos2<_T>(vp, t2, p);
		s2 = dot2_90<_T>(v, vp);
		if ( nearlyZero(s2) )
			onEdge |= 2;
		vectFromPos2<_T>(v, t3, t1);
		vectFromPos2<_T>(vp, t3, p);
		s3 = dot2_90<_T>(v, vp);
		if ( nearlyZero(s3) )
			onEdge |= 4;
		return (sign(s1) >= 0 && sign(s2) >= 0 && sign(s3) >= 0) || (sign(s1) <= 0 && sign(s2) <= 0 && sign(s3) <= 0);
	}
	/*
	template <typename _T> inline bool singularTriangle2(const _T *t1, const _T *t2, const _T *t3)
	{
		return eqVect2(t1,t2) && eqVect2(t2,t3);
		
	}

	template <typename _T> inline bool degenSideTriangle2(const _T *t1, const _T *t2, const _T *t3)
	{
		return eqVect2(t1,t2) || eqVect2(t2,t3) || eqVect2(t1,t3);
		
	}
	*/
	template <typename _T> inline bool degenTriangle2(const _T *t1, const _T *t2, const _T *t3)
	{
		return nearlyZero(dot2_90_pos(t1, t2, t3));
		
	}

	template <typename _T> inline int intersects2(const _T *a1, const _T *a2, const _T *b1, const _T *b2) {
		_T v[2], vi[2];
		_T s1, s2;
		vectFromPos2(v, a1, a2);
		vectFromPos2(vi, a1, b1);
		s1 = sign(det2(v, vi));
		vectFromPos2(vi, a1, b2);
		s2 = sign(det2(v, vi));
		
		if ( s1 == s2 && s1 != 0 )
			return 0;

		_T s3, s4;
		vectFromPos2(v, b1, b2);
		vectFromPos2(vi, b1, a1);
		s3 = sign(det2(v, vi));
		vectFromPos2(vi, b1, a2);
		s4 = sign(det2(v, vi));
		
		if ( s3 == s4 && s3 != 0 )
			return 0;

		if ( s1 == s2 && s3 == s4 ) {
			// colinear (all 0)
			if ( fabs(a1[0]-b1[0]) > fabs(a1[1]-b1[1]) ) {
				return (inClosedInterval(a1[1], b1[1], b2[1]) || inClosedInterval(a2[1], b1[1], b2[1])) ? 1 : 0;
			}
			return (inClosedInterval(a1[0], b1[0], b2[0]) || inClosedInterval(a2[0], b1[0], b2[0])) ? 1 : 0;
		}
			
		return (s1*s2*s3*s4) == 0 ? 1 : 2;
	}
	
	/** @brief Normal vector (not normalized) for a plane given by two 3D vectors, left handed
	 *  @retval norm Normal
	 *  @param v1 3 dim vector 1 (x axis)
	 *  @param v2 3 dim vector 2 (y axis)
	 *  @return true if normal is calculated, false otherwise
	 */
	template <typename _T> inline bool planeLH(_T *norm, const _T *v1, const _T *v2)
	{
		vprodLH(norm, v1, v2);
		if ( zeroVect3(norm) )
			return false;
		
		return true;
	}

	template <typename _T> inline bool planeRH(_T *norm, const _T *v1, const _T *v2)
	{
		return planeLH(norm, v2, v1);
	}

	
	/** @brief A normal vector (normalized) for a plane given by three points
	 *  @param p1 Point 1 (p2-p1 x axis)
	 *  @param p2 Point 2 (center)
	 *  @param p3 Point 3 (ps-p1 y axis)
	 *  @retval norm The normal vector as normalized cross product vect(P2, P1) x vect(P2, P3)
	 *  @return true if normal is calculated, false otherwise
	 */
	template <typename _T> inline bool planeLH(_T *norm, const _T *p1, const _T *p2, const _T *p3)
	{
		_T v1[3], v2[3];
		vectFromPos3(v1, p2, p1);
		vectFromPos3(v2, p2, p3);
		return planeLH(norm, v1, v2);
	}

	template <typename _T> inline bool planeRH(_T *norm, const _T *p1, const _T *p2, const _T *p3)
	{
		return planeLH(norm, p3, p2, p1);
	}

	/** @brief Linear interpolation between two values.
	 *  @param u Position (if 0 <= @a u <= 1 between @a minu and @a maxu)
	 *  @param minu Minimal value for interpolation (if u == 0)
	 *  @param maxu Maximal value for interpolation (if u == 1)
	 *  @return Interpolated value.
	 */
	template<typename _T> inline _T lerp(
		_T u,
		_T minu,
		_T maxu)
	{
		return minu + u * (maxu - minu);
	}

	/** @brief Bilinear interpolation between four values by three linear interpolations.
	 *  @param u Position (if 0 <= @a u <= 1 between @a x1 and @a x2 as well as between @a x4 and @a x4)
	 *  @param v Position (if 0 <= @a v <= 1 between the interpolated positions between x1...x2 and x3...x4)
	 *  @param x1 first value for linear interpolation in direction u.
	 *  @param x2 second value for linear interpolation in direction u.
	 *  @param x3 first value for linear interpolation in direction v.
	 *  @param x4 second value for linear interpolation in direction v.
	 *  @return The interpolated value.
	 */
	template<typename _T>
	inline _T bilerp(
		_T u,
		_T v,
		_T x1,
		_T x2,
		_T x3,
		_T x4)
	{
		return lerp(v, lerp(u, x1, x2), lerp(u, x3, x4));
	}

	template<typename _T> inline void printVector4(_T *v) {
		std::cout << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
	}

	template<typename _T> inline void printVector3(_T *v) {
		std::cout << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
	}

	template<typename _T> inline void printVector2(_T *v) {
		std::cout << v[0] << ", " << v[1] << std::endl;
	}
	
	template<typename _T> inline void printMatrix(_T m[4][4]) {
		printVector4(&m[0][0]);
		printVector4(&m[1][0]);
		printVector4(&m[2][0]);
		printVector4(&m[3][0]);
	}
	
	template<typename _T> inline void printMatrix(_T m[3][3]) {
		printVector3(&m[0][0]);
		printVector3(&m[1][0]);
		printVector3(&m[2][0]);
	}

	template<typename _T> inline void printMatrix(_T m[2][2]) {
		printVector2(&m[0][0]);
		printVector2(&m[1][0]);
	}
	
	template<typename _T> inline _T delta(_T minVal, _T maxVal, _T tess)
	{
		assert(tess > 0);
		return (maxVal-minVal) / tess;
		// Can be less eps, even zero... (e.g. cone height 0)
	}	
	
	template<typename _T> inline _T deltaNotZero(_T minVal, _T maxVal, _T tess)
	{
		assert(tess > 0);
		RtFloat result = (maxVal-minVal) / tess;
		if ( (static_cast<_T>(1) + result) == static_cast<_T>(1) )
			result = eps<_T>();
		return result;
	}	

	/*  @brief Deletes the contents of a container with pointers.
	 *
	 *  Is not working - why? -> Can be used in transformation.cpp
	 *
	 *  @param container Container with pointers.
	 */ /*
	template<typename _T>
	void delete_contents(
			_T &container)
	{
		typename _T::iterator i;
		for ( i = container.begin(); i != container.end(); i++ ) {
			if ( (*i) != 0 )
				delete *i;
		}
		container.clear();
	}
	*/
		
	/*  @brief Duplicates the contents of a container with pointers.
	 *
	 *  Is not working - why? -> Can be used in transformation.cpp
	 *
	 *  @param container Container with pointers that gets the duplicates.
	 *  @param source Container with pointers that delivers the duplicates.
	 */ /*
	template<typename _T>
	void push_back_duplicate(
					 _T &destination,
					 const _T &source)
	{
	    if ( &source == &destination )
	        return;
		typename _T::const_iterator i;
		for ( i = source.begin(); i != source.end(); i++ ) {
			if ( (*i) != 0 )
				destination.push_back((*i)->duplicate());
			else
				destination.push_back(0);
		}
	}
	*/
}

#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H
