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

#include <cmath>
#include <cstdlib>

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
	template<class _T>
	inline _T tmin(
		_T a,
		_T b)
	{
		return a < b ? a : b;
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

	
	/** @brief Clambs a value the values boundmin, boundmax.
	 *
	 *  The bounds are exchanged if @a boundmin > @a boundmax.
	 *
	 *  @param val value to clamb.
	 *  @param boundmin Size of the string buffer.
	 *  @param boundmax Number to convert
	 *  @return The clambed value (>= @a boundmin and <= @a boundmax)
	 */
	template<typename type> inline type clamptempl(type val, type boundmin, type boundmax)
	{
		if ( boundmin > boundmax ) {
			type t = boundmin;
			boundmin = boundmax;
			boundmax = t;
		}
		
		if ( val < boundmin )
			return boundmin;
		if ( val > boundmax )
			return boundmax;
		
		return val;
	}
	
	/** @brief Get the inverse of a number
	 *  @param val Number to invert
	 *  @return 1 / @a val
	 */
	template<typename type> inline type inversetempl(type val)
	{
		return static_cast<type>(1.0)/val;
	}
	
	/** @brief Rounds a number
	 *  @param val Number to round
	 *  @return Rounded number
	 */
	template<typename type> inline type roundtempl(type val)
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
	template<typename type> inline type randftempl()
	{
		return static_cast<type>(rand()) / static_cast<type>(RAND_MAX);
	}
	
	/** @brief Gets a random number between -1 and +1
	 *  @return Random number
	 */
	template<typename type> inline type randf2templ()
	{
		// -1.0 ... 1.0
		return static_cast<type>(
								 (
								  (
								   static_cast<type>(rand()) /
								   static_cast<type>(RAND_MAX)
								   ) -
								  static_cast<type>(0.5)
								  ) *
								 static_cast<type>(2.0)
								 );
	}
	
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
	
	/** @brief Convert degrees to radians
	 *  @param degree The degree value to convert
	 *  @return The radian value ((degree * pi) / 180.0)
	 */
	template <typename type> inline type deg2rad(type degree) {return static_cast<type>((degree * pi<type>()) / 180.0);}
	
	/** @brief Convert radians to degrees
	 *  @param radian The radian value to convert
	 *  @return The degree value (radian * 180.0) / pi)
	 */
	template <typename type> inline type rad2deg(type radian) {return static_cast<type>((radian * 180.0) / pi<type>());}
	
	/** @brief Get the sign of a value
	 *  @param f The value to test
	 *  @return -1 if f is negative, 1 otherwise
	 */
	template <typename type> int inline sign(type f) { return f < 0 ? static_cast<type>(-1) : static_cast<type>(1); }
	
	/** @brief Vector (cross) product of two 3d vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @retval vp Vector product
	 */
	template <typename _T> inline void vprod(const _T *v1, const _T *v2, _T *vp)
	{
		vp[0] = v1[1]*v2[2] - v1[2]*v2[1];
		vp[1] = v1[2]*v2[0] - v1[0]*v2[2];
		vp[2] = v1[0]*v2[1] - v1[1]*v2[0];
	}

	/** @brief Scalar (dot) product of two 3d vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot3(const _T *v1, const _T *v2)
	{
		return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	}

	/** @brief Scalar (dot) product of two 2d vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot2(const _T *v1, const _T *v2)
	{
		return v1[0]*v2[0] + v1[1]*v2[1];
	}

	/** @brief Scalar product of two 2d vectors (1 rotated 90deg ccw for left right test)
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2 (will be rotated 90 deg ccw)
	 *  @return Scalar product
	 */
	template <typename _T> inline _T dot2_90(const _T *v1, const _T *v2)
	{
		return v1[1]*v2[0] - v1[0]*v2[1];
	}

	/** @brief Normal vector (normalized) for a plane given by two vectors
	 *  @param v1 Vector 1
	 *  @param v2 Vector 2
	 *  @retval norm Normal
	 *  @return true if normal is calculated, false otherwise
	 */
	template <typename _T> inline bool planeNorm(const _T *v1, const _T *v2, _T *norm)
	{
		norm[0] = v1[1]*v2[2] - v1[2]*v2[1];
		norm[1] = v1[2]*v2[0] - v1[0]*v2[2];
		norm[2] = v1[0]*v2[1] - v1[1]*v2[0];
		
		_T length = static_cast<_T>(sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]));
		if ( length == 0 ) {
			norm[0] = 0; norm[1] = 0; norm[2] = static_cast<_T>(1);
			return false;
		}
		
		norm[0] /= length;
		norm[1] /= length;
		norm[2] /= length;
		
		return true;
	}

	/** @brief A normal vector (normalized) for a plane given by three points
	 *  @param p1 Point 1
	 *  @param p2 Point 2
	 *  @param p3 Point 3
	 *  @retval norm The normal vector as normalized cross product vect(P2, P1) x vect(P2, P3)
	 *  @return true if normal is calculated, false otherwise
	 */
	template <typename _T> inline bool planeNorm(const _T *p1, const _T *p2, const _T *p3, _T *norm)
	{
		_T v1[3] = {p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]};
		_T v2[3] = {p3[0]-p2[0], p3[1]-p2[1], p3[2]-p2[2]};
		
		return planeNorm(v1, v2, norm);
	}

	/** @brief Normalize a 3D vector to size 1
	 *  @param x x component of a vector (can be a part of @a norm)
	 *  @param y y component of a vector (can be a part of @a norm)
	 *  @param z z component of a vector (can be a part of @a norm)
	 *  @retval norm The normalized vector (size = 3)
	 */
	template <typename _T> inline void normalize(_T x, _T y, _T z, _T *norm)
	{
		_T length = static_cast<_T>(sqrt(x*x+y*y+z*z));
		if ( length == 0 ) {
			norm[0] = 0; norm[1] = 0; norm[2] = 0;
			return false;
		}
		
		norm[0] = x/length;
		norm[1] = y/length;
		norm[2] = z/length;
	}

	/** @brief Normalize a 2D vector to size 1
	 *  @param x x component of a vector (can be a part of @a norm)
	 *  @param y y component of a vector (can be a part of @a norm)
	 *  @retval norm The normalized vector (size = 2)
	 */
	template <typename _T> inline void normalize(_T x, _T y, _T *norm)
	{
		_T length = static_cast<_T>(sqrt(x*x+y*y));
		if ( length == 0 ) {
			norm[0] = 0; norm[1] = 0;
			return false;
		}
		
		norm[0] = x/length;
		norm[1] = y/length;
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
