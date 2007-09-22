#ifndef _RICPP_TOOLS_INLINETOOLS_H
#define _RICPP_TOOLS_INLINETOOLS_H

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

/** @file inlinetools.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Frequent used inline tools
 */
#include <cstring>
#include <cmath>

namespace RiCPP {

/** @brief Gets "<empty>" for a 0 pointer or an empty "" string
 * @param str String to test
 * @return str itself or "<empty>" if the string is empty
 */
inline const char *markEmptyStr(const char *str)
{
	return  str && *str ? str : "<empty>";
}

/** @brief Gets "" for a 0 pointer
 * @param str String to test
 * @return str itself or "" if the string points to 0
 */
inline const char *noNullStr(const char *str)
{
	return  str ? str : "";
}

/** @brief Tests if string is empty, either pointer is 0 or ""
 * @param str String to test
 * @return true, if str is 0 or points to ""
 */
inline bool emptyStr(const char *str)
{
	return !str || !*str;
}

/** @brief Tests if string is not empty
 * @param str String to test
 * @return true, if str is not 0 or nor ""
 */
inline bool notEmptyStr(const char *str)
{
	return str && *str;
}

/** @brief toupper conversion for for_each()
 * @retval c Char to convert
 */
inline void asciiToUpper(char &c)
{
	if ( c >= 'a' && c <= 'z' )
		c = (c-'a') + 'A';
}

/** @brief Converts a ':' to a '|', used for pathlists
 * @retval c Char to convert
 */
inline void maskColon(char &c)
{
	if ( c == ':' )
		c = '|';
}

/** @brief Converts a '|' back to a ':', used for pathlists
 * @retval c Char to convert
 */
inline void unmaskColon(char &c)
{
	if ( c == '|' )
		c = ':';
}

/**@brief Helper function, cuts away the filename of a path.
 * @param buf Pointer to the characrer buffer with the filepath.
 *            The buffer will be modified.
 * @return \a buf or "" is returned, the filename is cut away.
 *         \a buf can be empty (point to NUL) after calling this
 *         function.
 */
inline char *cutfilename(char *buf)
{
	if ( !buf )
		return "";
		
	size_t len = strlen(buf);
	
	// empty string - no filename no changes
	if ( !len )
		return buf;

	// --> strlen(buf) > 0, buf[0] and buf[1] are within buffer

	while ( len != 0 && buf[len-1] != '/' )
		--len;
	if ( len ) {
		// A '/' found at buf[len-1]
		buf[len-1] = 0; // can lead to buf[0] == NUL, if buf contained a file with root path
	} else {
		// No '/' found.
		// Was a path containing a filename only, it was a relative path,
		// therefore '.' is the directory
		buf[0] = '.'; 
		buf[1] = 0; 
	}
		
	return buf;
}

template<typename type> type clamptempl(type val, type boundmin, type boundmax)
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

template<typename type> type inversetempl(type val)
{
	return static_cast<type>(1.0)/val;
}

template<typename type> type roundtempl(type val)
{
	if ( val < 0 ) {
		type t = ceil(val);
		return (val-t > -0.5) ? t : floor(val);
	}

	type t = floor(val);
	return (val-t < 0.5) ? t : ceil(val);
}

template<typename type> type randftempl()
{
   return static_cast<type>(rand()) / static_cast<type>(RAND_MAX);
}

template<typename type> type randf2templ()
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

template <typename type> type piTimes2() { return static_cast<type>(6.283185307179586476925286766559); }
template <typename type> type pi()       { return static_cast<type>(3.1415926535897932384626433832795); }
template <typename type> type pi_2()     { return static_cast<type>(1.5707963267948966192313216916398); }
template <typename type> type pi_4()     { return static_cast<type>(0.78539816339744830961566084581988); }

//! Convert degrees to radians
/*! \param Degree The degree value to convert
 *  \return The radian value ((degree * pi) / 180.0)
 */
template <typename type> type deg2rad(type degree) {return static_cast<type>((degree * pi<type>()) / 180.0);}

//! Convert radians to degrees
/*! \param Degree The radian value to convert
 *  \return The degree value (radian * 180.0) / pi)
 */
template <typename type> type rad2deg(type radian) {return static_cast<type>((radian * 180.0) / pi<type>());}

//! Get the sign of a value
/*! \param f The value to test
 *  \return -1 if f is negative, 1 otherwise
 */
template <typename type> int sign(type f) { return f < 0 ? static_cast<type>(-1) : static_cast<type>(1); }



} // namespace RiCPP
#endif // _RICPP_TOOLS_INLINETOOLS_H
