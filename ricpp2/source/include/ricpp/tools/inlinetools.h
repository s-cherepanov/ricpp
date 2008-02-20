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
#include <cstdlib>

#include <string>

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
 * @return @a buf or "" is returned, the filename is cut away.
 *         @a buf can be empty (point to NUL) after calling this
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

/** @brief Removes quotes at start and end of a string
 *
 * Removes the quotes, e.g of filenames from command line
 *
 * @param str C-String, gets the quotes removed (at end)
 * @return Pointer to the beginning of the string without quotes
 */
inline char *removeQuotes(char *str)
{
	if ( emptyStr(str) )
		return str;

	size_t len = strlen(str);
	if ( len < 2 )
		return str;

	if ( str[0] == '\"' && str[len-1]== '\"' ) {
		str[len-1] = 0;
		return str+1;
	}

	return str;
}

/* @brief Extracts a handle number from a handle string.
 *  @param handle Handle in String form (a '_' sperates a prefix from a number)
 *  @return The extracted number
 *//*
inline unsigned long extractHandleNo(const char *handle)
{
	const char *ptr = strrchr(handle, '_');
	if ( ptr ) ++ptr;
	unsigned long n = 0;
	if ( ptr[0] ) {
		n = atol(ptr);
	}
	return n;
}*/

/** @brief Converts a number (unsigned long) to a c-string
 *  @param buffer String buffer.
 *  @param bufsize Size of the string buffer.
 *  @param aLong Number to convert
 *  @return @a buffer with the copied number.
 */
inline const char *valToStr(char *buffer, size_t bufsize, unsigned long aLong)
{
	if ( !buffer || bufsize < 1 )
		return "";
	buffer[0] = 0;
	#ifdef WIN32
		sprintf_s(buffer, bufsize, "%lu", aLong);
	#else
		snprintf(buffer, bufsize-1, "%lu", aLong);
	#endif
	buffer[sizeof(buffer)-1] = 0;
	return buffer;
}

/** @brief Converts a number (long) to a c-string
 *  @param buffer String buffer.
 *  @param bufsize Size of the string buffer.
 *  @param aLong Number to convert
 *  @return @a buffer with the copied number.
 */
inline const char *valToStr(char *buffer, size_t bufsize, long aLong)
{
	if ( !buffer || bufsize < 1 )
		return "";
	buffer[0] = 0;
	#ifdef WIN32
		sprintf_s(buffer, bufsize, "%ld", aLong);
	#else
		snprintf(buffer, bufsize-1, "%ld", aLong);
	#endif
	buffer[sizeof(buffer)-1] = 0;
	return buffer;
}

/** @brief Converts a number (unsigned int) to a c-string
 *  @param buffer String buffer.
 *  @param bufsize Size of the string buffer.
 *  @param anInt Number to convert
 *  @return @a buffer with the copied number.
 */
inline const char *valToStr(char *buffer, size_t bufsize, unsigned int anInt)
{
	if ( !buffer || bufsize < 1 )
		return "";
	buffer[0] = 0;
	#ifdef WIN32
		sprintf_s(buffer, bufsize, "%u", anInt);
	#else
		snprintf(buffer, bufsize-1, "%u", anInt);
	#endif
	buffer[sizeof(buffer)-1] = 0;
	return buffer;
}


/** @brief Converts a number (int) to a c-string
 *  @param buffer String buffer.
 *  @param bufsize Size of the string buffer.
 *  @param anInt Number to convert
 *  @return @a buffer with the copied number.
 */
inline const char *valToStr(char *buffer, size_t bufsize, int anInt)
{
	if ( !buffer || bufsize < 1 )
		return "";
	buffer[0] = 0;
	#ifdef WIN32
		sprintf_s(buffer, bufsize, "%d", anInt);
	#else
		snprintf(buffer, bufsize-1, "%d", anInt);
	#endif
	buffer[sizeof(buffer)-1] = 0;
	return buffer;
}


/** @brief Converts a number (double or float) to a c-string
 *  @param buffer String buffer.
 *  @param bufsize Size of the string buffer.
 *  @param aDouble Number to convert
 *  @return @a buffer with the copied number.
 */
inline const char *valToStr(char *buffer, size_t bufsize, double aDouble)
{
	if ( !buffer || bufsize == 0 )
		return "";
	buffer[0] = 0;
	#ifdef WIN32
		sprintf_s(buffer, bufsize, "%f", aDouble);
	#else
		snprintf(buffer, bufsize-1, "%f", aDouble);
	#endif
	buffer[sizeof(buffer)-1] = 0;
	return buffer;
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

/** @brief Get the inverse of a number
 *  @param val Number to invert
 *  @return 1 / @a val
 */
template<typename type> type inversetempl(type val)
{
	return static_cast<type>(1.0)/val;
}


/** @brief Rounds a number
 *  @param val Number to round
 *  @return Rounded number
 */
template<typename type> type roundtempl(type val)
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
template<typename type> type randftempl()
{
   return static_cast<type>(rand()) / static_cast<type>(RAND_MAX);
}

/** @brief Gets a random number between -1 and +1
 *  @return Random number
 */
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

/** @brief Gets pi times 2
 *  @return pi*2
 */ 
template <typename type> type piTimes2() { return static_cast<type>(6.283185307179586476925286766559); }

/** @brief Gets pi
 *  @return pi
 */ 
template <typename type> type pi()       { return static_cast<type>(3.1415926535897932384626433832795); }

/** @brief Gets pi divided by 2
 *  @return pi/2
 */ 
template <typename type> type pi_2()     { return static_cast<type>(1.5707963267948966192313216916398); }

/** @brief Gets pi divided by 4
 *  @return pi/4
 */ 
template <typename type> type pi_4()     { return static_cast<type>(0.78539816339744830961566084581988); }

/** @brief Convert degrees to radians
 *  @param degree The degree value to convert
 *  @return The radian value ((degree * pi) / 180.0)
 */
template <typename type> type deg2rad(type degree) {return static_cast<type>((degree * pi<type>()) / 180.0);}

/** @brief Convert radians to degrees
 *  @param radian The radian value to convert
 *  @return The degree value (radian * 180.0) / pi)
 */
template <typename type> type rad2deg(type radian) {return static_cast<type>((radian * 180.0) / pi<type>());}

/** @brief Get the sign of a value
 *  @param f The value to test
 *  @return -1 if f is negative, 1 otherwise
 */
template <typename type> int sign(type f) { return f < 0 ? static_cast<type>(-1) : static_cast<type>(1); }

/** @brief Base calss of a simple (backtracking) recursive descend parser
 *
 *  Used for URI parsing and RIB if-expression
 */
class CRecursiveDescentParser {
public:
	/** @brief Advances the input pointer and copies to result.
	*
	* Advances the input pointer @a *str @a n times and
	* stores the characters in @a result.
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @param n Number of advances.
	*/
	inline void advance(
		const unsigned char **str,
		std::string &result,
		int n=1) const
	{
		while ( n-- > 0 ) {
			result += ((*str)++)[0];
		}
	}

	/** @brief Lookahead
	*
	* Gets the first character of @a *str.
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @return First character of @a *str
	*/
	inline unsigned char la(const unsigned char **str) const
	{
		return (*str)[0];
	}

	/** @brief Matches a sequence, advances the input pointer.
	*
	* Matches the sequence @a matchStr and advances the input
	* pointer. If the whole string could not be matched,
	* the input pointer is restored. If all characters
	* of @a mathString could be matched, they are stored
	* in @a result.
	*
	* @param matchStr String to match
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return true, the @a matchStr matches.
	*/
	inline bool match(
		const char *matchStr,
		const unsigned char **str,
		std::string &result) const
	{
		// An empty string matches
		if ( !matchStr || !*matchStr ) {
			return true;
		}

		const unsigned char *sav = *str;
		const unsigned char *ptr = (const unsigned char *)matchStr;
		while ( *ptr && la(str) == *ptr ) {
			++ptr;
			++(*str);
		}
		if ( *ptr == 0 ) {
			result += (char *)matchStr;
			return true;
		}
		*str = sav;
		return false;
	}

	/** @brief Matches one character of a string, advances the input pointer.
	*
	* Matches one character of @a matchStr and advances the pointer if
	* one character matches. If matchStr is empty no
	* character matches. If one character
	* of @a mathString could be matched, the matched character is stored
	* in @a result and returned.
	*
	*
	* @param matchStr One character of the string to match.
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return The matched character or 0 if no character matches.
	*/
	inline unsigned char matchOneOf(
		const char *matchStr,
		const unsigned char **str,
		std::string &result) const
	{
		if ( !matchStr || !*matchStr ) {
			return 0;
		}

		for ( const unsigned char *ptr =
		          (const unsigned char *)matchStr;
		      *ptr;
			  ++ptr )
		{
			if ( la(str) == *ptr ) {
				advance(str, result);
				return *ptr;
			}
		}
		return 0;
	}

	/** @brief Digit.
	*
	* Tests if current input character is a digit.
	* If the current input character is a digit, it is stored
	* in @a result and returned.
	*
	@verbatim
	digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
	        "8" | "9"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @retval d The digit found as number.
	* @return 0, no digit that matches.
	*/
	inline unsigned char digit(
		const unsigned char **str,
		std::string &result,
		unsigned char &d) const
	{
		unsigned char c = la(str);
		if ( c>='0' && c<='9' ) {
			advance(str, result);
			d = c - '0';
			return c;
		}
		return 0;
	}

	/** @brief Digit not null.
	*
	* Tests if current input character is a digit, not a '0'.
	* If the current input character is a digit, it is stored
	* in @a result and returned.
	*
	@verbatim
	digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" |
	        "8" | "9"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @retval d The digit found as number.
	* @return 0, no digit that matches.
	*/
	inline unsigned char digit_not_null(
		const unsigned char **str,
		std::string &result,
		unsigned char &d) const
	{
		unsigned char c = la(str);
		if ( c>='1' && c<='9' ) {
			advance(str, result);
			d = c - '0';
			return c;
		}
		return 0;
	}

	/** @brief Octal digit.
	*
	* Tests if current input character is an octal digit.
	* If the current input character is an octal digit, it is stored
	* in @a result and returned.
	*
	@verbatim
	oct_digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @retval d The digit found as number.
	* @return 0, no octal digit that matches.
	*/
	inline unsigned char octdig(
		const unsigned char **str,
		std::string &result,
		unsigned char &d) const
	{
		unsigned char c = la(str);
		if ( c>='0' && c<='7' ) {
			advance(str, result);
			d = c - '7';
			return c;
		}
		return 0;
	}

	/** @brief Uppercase letter.
	*
	* Test if current input character is an uppercase letter.
	* If the current input character is an uppercase letter ('A'-'Z'),
	* it is appended to @a result and is returned.
	*
	@verbatim
	upalpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
	          "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
	          "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return 0, no letter or letter that matches.
	*/
	inline unsigned char upalpha(
		const unsigned char **str,
		std::string &result) const
	{
		unsigned char c = la(str);
		if ( c>='A' && c<='Z' ) {
			advance(str, result);
			return c;
		}
		return 0;
	}

	/** @brief Lowercase letter.
	*
	* Test if current input character is a lowercase letter.
	* If the current input character is an lowercase letter ('a'-'z'),
	* it is appended to @a result and is returned.
	*
	@verbatim
	lowalpha = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
	           "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
	           "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return 0, no letter or letter that matches.
	*/
	inline unsigned char lowalpha(
		const unsigned char **str,
		std::string &result) const
	{
		unsigned char c = la(str);
		if ( c>='a' && c<='z' ) {
			advance(str, result);
			return c;
		}
		return 0;
	}

	/** @brief Lowercase or uppercase letter.
	*
	* Test if current input character is a lowercase or uppercase letter.
	* If the current input character is a letter ('A'-'Z' or 'a'-'z'),
	* it is appended to @a result and is returned.
	*
	@verbatim
	alpha = lowalpha | upalpha
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return 0, no letter or the letter that matches.
	* @see lowalpha() upalpha()
	*/
	inline unsigned char alpha(
		const unsigned char **str,
		std::string &result) const
	{
		return lowalpha(str, result) || upalpha(str, result);
	}

	/** @brief Alphanumerical character.
	*
	* Test if current input character is a letter or a digit.
	* If the current input character is alphanumeric ('A'-'Z' or 'a'-'z', '0'-'9'),
	* it is appended to @a result and is returned.
	*
	@verbatim
	alphanum = alpha | digit
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @return 0, if no alphanumeric has been found
	* or the alphanumeric that matches.
	* @see alpha() digit()
	*/
	inline unsigned char alphanum(
		const unsigned char **str,
		std::string &result) const
	{
		unsigned char d;
		return alpha(str, result) || digit(str, result, d);
	}

	/** @brief Hexadecimal digit.
	*
	* Test if current input character is hexadecimal digit.
	* If the current input character is a hexadecimal digit
	* ('0'-'9', 'A'-'f' or 'a'-'f'),
	* it is appended to @a result and is returned.
	*
	@verbatim
	hexdig = digit |
	         "A" | "B" | "C" | "D" | "E" | "F" |
	         "a" | "b" | "c" | "d" | "e" | "f"
	@endverbatim
	*
	* @param str Address of a character pointer to the
	* input string (address of input pointer).
	* @retval result String to store the characters matched.
	* @retval d The digit found as number.
	* @return 0, no hexdigit or the hexdigit that matches.
	* @see digit()
	*/
	inline unsigned char hexdig(
		const unsigned char **str,
		std::string &result,
		unsigned char &d) const
	{
		unsigned char c = la(str);
		if ( digit(str, result, d) ) {
			return c;
		}
		if ( (c>='A' && c<='F') ) {
			d = 10 + c - 'A';
			advance(str, result);
			return c;
		}
		if ( (c>='a' && c<='f') ) {
			d = 10 + c - 'a';
			advance(str, result);
			return c;
		}
		return 0;
	}
};


} // namespace RiCPP
#endif // _RICPP_TOOLS_INLINETOOLS_H
