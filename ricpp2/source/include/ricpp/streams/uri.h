#ifndef _RICPP_STREAMS_URI_H
#define _RICPP_STREAMS_URI_H

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

/** @file uri.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief URI parser.
 */

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <string>
#include <list>
#include <cassert>

namespace RiCPP {
	/** @brief Class to represent an absolute or relative URI.
	 *
	 * URIs that do not match the generic format as
	 * described in rfc2396 are not suported. A string
	 * representing a URI can be splitted to it's
	 * components by CUri::parse(). Please read
	 * "rfc2396 Unified Identifiers (URI): Generic Syntax"
	 * for more information (also for full BNF).
	 *
	 * @see http://www.ietf.org/rfc/rfc2396.txt
	 */
	class CUri {
	public:
		/** @brief Path segment.
		 *
		 * Container for a path segment with optional parameters.
		 *
		 */
		class CSegment {
			friend class CUri;
			/** @brief Segment string.
			 *
			 * Stored without the separating '/', can be empty
			 */
			std::string m_name; 

			/** @brief List of optional parameters
			 *
			 * Stored without the separating ';'.
			 * The parameter strings can be empty - than only the ';'
			 * is used at the place of the parameter.
			 */
			std::list<std::string> m_parameters;

			/** @brief Clears the components m_name and m_parameters.
			 */
			void clear() {
				m_name.clear();
				m_parameters.clear();
			}

		public:
			/** @brief Constant iterator for the parameter list.
			 */
			typedef std::list<std::string>::const_iterator const_iterator;

			/** @brief Type for the size of the parameter list.
			 */
			typedef std::list<std::string>::size_type size_type;

			/** @brief Gets the name of the segment.
			 * @return name of the segment, without '/'.
			 */
			inline const std::string &getName() const { return m_name; }

			/** @brief Gets The constant iterator, begin of the parameter list.
			 * @return The begin of the parameter list.
			 */
			const_iterator begin() const
			{
				return m_parameters.begin();
			}

			/** @brief Gets the constant iterator, end of the parameter list.
			 * @return The end of the parameter list.
			 */
			const_iterator end() const
			{
				return m_parameters.end();
			}

			/** @brief Gets the number of parameters
			 * @return The number of parameters.
			 */
			size_type size() const
			{
				return m_parameters.size();
			}
		};

		/** @brief Constant iterator for the segment list.
		 */
		typedef std::list<CSegment>::const_iterator const_iterator;

		/** @brief Type for the size of the segment list.
		 */
		typedef std::list<CSegment>::size_type size_type;

		/** @brief Constant iterator for the domainlabel list.
		 */
		typedef std::list<std::string>::const_iterator domainlabels_const_iterator;

		/** @brief Type for the size of the domainlabel list.
		 */
		typedef std::list<std::string>::size_type domainlabels_size_type;

	private:
		/** @brief true, if a valid URI has been parsed.
		 */
		bool m_valid;

		/** @brief The path component.
		 *
		 * The absolute or relative path or the opaque part of the URI
		 */
		std::string m_path;

		/** @brief Temporary string of an escaped character.
		 *
		 * If an escaped chracter (%nm) is parsed, it is stored temporarily
		 * in this member.
		 */
		std::string m_escaped;

		/** @brief The URI reference.
		 *
		 * This member is used to store the whole string parsed.
		 */
		std::string m_uri_reference;

		/** @brief The absolute URI.
		 *
		 * Contains the absolute URI, empty URI was relative.
		 */
		std::string m_absoluteURI;

		/** @brief The relative URI.
		 *
		 * Contains the relative URI, empty URI was absolute.
		 */
		std::string m_relativeURI;

		/** @brief true, if a scheme was found.
		 *
		 * If there is a scheme found, m_scheme will contain it and
		 * will not be empty.
		 */
		bool m_hasScheme;

		/** @brief The scheme of an absolute URI
		 *
		 * The scheme is stored without the trailing ':'
		 *
		 */
		std::string m_scheme;

		/** @brief The opaque part of URI
		 *
		 * If a URI contains an opaque part, there is no
		 * hierarchical path.
		 */
		std::string m_opaque_part;

		/** @brief The hierarchical part of URI
		 *
		 * If a URI contains an hierarchical part, there is no
		 * opaque path.
		 */
		std::string m_hier_part;

		/** @brief The net path of a hierarchical URI
		 *
		 * Contains authority and absolute path.
		 */
		std::string m_net_path;
		/** @brief The absolute path of a hierarchical URI
		 *
		 * Part of a path that begins with the lrfmost '/'
		 */
		std::string m_abs_path;
		/** @brief The relative path of a hierarchical URI
		 *
		 * Contains an absolute path.
		 */
		std::string m_rel_path;
		
		/** @brief URI contains a query component
		 *
		 * true, if an '?' has been found in an approriate place
		 */
		bool m_hasQuery;

		/** @brief The query.
		 *
		 * The query is stored without the leading '?' and can be an empty string.
		 */
		std::string m_query;

		/** @brief URI contains a fragment component
		 *
		 * true, if an '#' has been found in an approriate place
		 */
		bool m_hasFragment;

		/** @brief The fragment.
		 *
		 * The fragment is stored without the leading '#' and can be an empty string.
		 */
		std::string m_fragment;

		/** @brief The authority.
		 *
		 * The authority can be an empty string ( "file:///c/" contains an
		 * empty authority after the leading "//").
		 */
		bool m_hasAuthority;

		/** @brief The authority.
		 *
		 * The authority is stored without the leading "//" and can be an empty string.
		 */
		std::string m_authority;

		/** @brief Authority contains a user info.
		 */
		bool m_hasUserinfo;

		/** @brief The user info.
		 *
		 * The authority contains a user info, can be an empty string.
		 */
		std::string m_userinfo;

		/** @brief Authority contains a server name.
		 */
		bool m_hasServer;

		/** @brief The server name.
		 *
		 * The authority contains a server name, can be an empty string.
		 */
		std::string m_server;

		/** @brief The registry based name of the URI.
		 *
		 * The authority contains a registry based name, can be an empty string.
		 */
		std::string m_reg_name;

		/** @brief the hostname.
		 */
		std::string m_host;

		/** @brief the host and port.
		 */
		std::string m_hostport;

		/** @brief The host contains an IP4 address.
		 */
		std::string m_ipV4address;

		/** @brief The rightmost character in the hostname is a dot.
		 */
		bool m_hasTrailingDot;

		/** @brief The readable hostname.
		 */
		std::string m_hostname;

		/** @brief, true if there is a portaddress in the authority.
		 */
		bool m_hasPort;

		/** @brief The port address without leading ':'.
		 */
		std::string m_port;

		/** @brief The domainlabels (incl. top label) of the host address.
		 */
		std::list<std::string> m_domainlabels;

		/** @brief Temporary stored domain label.
		 */
		std::string m_domainlabel;

		/** @brief The top label.
		 */
		std::string m_toplabel;
		
		/** @brief The relative segment of a path.
		 */
		std::string m_rel_segment;

		/** @brief The pathsegments concatenated with '/'.
		 */
		std::string m_path_segments;

		/** @brief Tempoprary store for a path segment.
		 */
		std::string m_segment;

		/** @brief Temporary parameters.
		 *
		 * Temporary store for the parameters (separated by ';' of a path segment.
		 */
		std::string m_param;

		/** @brief Temporary store of a segment with parameters.
		 */
		CSegment m_segmentContainer;

		/** @brief List of the segments of a path component, exculding the relative segment.
		 */
		std::list<CSegment> m_segments;

		/** @brief Clears all components.
		 *
		 * A cleared URI is a valid URI.
		 */
		void clearAll();

		/** @brief Advances the input pointer.
		 *
		 * Advances the input pointer \a *str and stores the characters in \a result.
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @param steps Number of advances.
		 */
		inline void advance(const unsigned char **str, std::string &result, int steps=1)
		{
			while ( steps-- > 0 ) {
				result += ((*str)++)[0];
			}
		}

		/** @brief Digit.
		 *
		 * Test if current input character is a digit.
		 * If the current input character is a digit, it is stored
		 * in \a result and returned.
		 *
		 @verbatim
		 digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
		         "8" | "9"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no digit or digit, the digit found.
		 */
		inline unsigned char digit(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( c>='0' && c<='9' ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		/** @brief Uppercase letter.
		 *
		 * Test if current input character is an uppercase letter.
		 * If the current input character is an uppercase letter ('A'-'Z'),
		 * it is stored in \a result and returned.
		 *
		 @verbatim
		 upalpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
		           "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
		           "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no letter or letter, the letter found.
		 */
		inline unsigned char upalpha(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
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
		 * it is stored in \a result and returned.
		 *
		 @verbatim
		 lowalpha = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
		            "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
		            "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no letter or letter, the letter found.
		 */
		inline unsigned char lowalpha(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( c>='a' && c<='z' ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		/** @brief Letter.
		 *
		 * Test if current input character is a lowercase or uppercase letter.
		 * If the current input character is a letter ('A'-'Z' or 'a'-'z'),
		 * it is stored in \a result and returned.
		 *
		 @verbatim
		 alpha = lowalpha | upalpha
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no letter or letter, the letter found.
		 * @see lowalpha() upalpha()
		 */
		inline unsigned char alpha(const unsigned char **str, std::string &result)
		{
			return lowalpha(str, result) || upalpha(str, result);
		}

		/** @brief Alphanumerical character.
		 *
		 * Test if current input character is a letter or a digit.
		 * If the current input character is alphanumeric ('A'-'Z' or 'a'-'z', '0'-'9'),
		 * it is stored in \a result and returned.
		 *
		 @verbatim
		 alphanum = alpha | digit
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no alphanumeric or alphanumeric, the alphanumeric found.
		 * @see alpha() digit()
		 */
		inline unsigned char alphanum(const unsigned char **str, std::string &result)
		{
			return alpha(str, result) || digit(str, result);
		}

		/** @brief Hexadecimal digit.
		 *
		 * Test if current input character is hexadecimal digit.
		 * If the current input character is a hexadecimal digit ('0'-'9', 'A'-'f' or 'a'-'f'),
		 * it is stored in \a result and returned.
		 *
		 @verbatim
		 hex = digit | "A" | "B" | "C" | "D" | "E" | "F" |
		               "a" | "b" | "c" | "d" | "e" | "f"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0, no hexdigit or hexdigit, the hexdigit found.
		 * @see digit()
		 */
		inline unsigned char hex(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( digit(str, result) )
				return c;
			if ( (c>='A' && c<='F') || (c>='a' && c<='f') ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		/** @brief Punctation marks and symbols.
		 *
		 * Test if current input is a punctation mark or symbol allowed in a URI
		 * If found, the mark stored in \a result and is returned.
		 *
		 @verbatim
		 mark = "-" | "_" | "." | "!" | "~" | "*" | "'" |
		        "(" | ")"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0 or the mark found.
		 */
		inline unsigned char mark(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			switch (c) {
				case '-':
				case '_':
				case '.':
				case '!':
				case '~':
				case '*':
				case '\'':
				case '(':
				case ')':
					advance(str, result);
					return c;
				default:
					return 0;
			}
		}

		/** @brief Unreserved characters.
		 *
		 * Characters that are allowed in a URI but have no special purpose.
		 * If found, the character stored in \a result and is returned. Unreserved
		 * characters can be escaped without changing the semantics of a URI,
		 * but there is no need to do that in general.
		 *
		 @verbatim
		 unreserved = "alphanum" | "mark"
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0 or the unreserved character found.
		 * @see alphanum() mark()
		 */
		inline unsigned char unreserved(const unsigned char **str, std::string &result)
		{
			return alphanum(str, result) || mark(str, result);
		}

		/** @brief Reserved characters.
		 *
		 * Characters that are allowed in a URI and have the special purpose of
		 * delimeters. If found, the character stored in \a result and is returned.
		 * Reserved characters have to be escaped, if they occur in the data part (have
		 * no delimiting purpose)
		 *
		 @verbatim
		 reserved = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
		            "$" | ","
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return 0 or the reserved character found.
		 */
		inline unsigned char reserved(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			switch (c) {
				case ';':
				case '/':
				case '?':
				case ':':
				case '@':
				case '&':
				case '=':
				case '+':
				case '$':
				case ',':
					advance(str, result);
					return c;
				default:
					return 0;
			}
		}

		/** @brief Escape sequence.
		 *
		 * Test if current input is an escape character (e.g. "%20" for blank)
		 * If found, the current escape sequence it is stored in \a result and
		 * m_escaped and true is returned.
		 *
		 @verbatim
		 escaped = "%" hex hex
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, escape sequence found.
		 * @see m_escaped hex()
		 */
		inline bool escaped(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_escaped = "";
			if ( (*str)[0] != '%' ) {
				return false;
			}
			advance(str, m_escaped);

			if ( !hex(str, m_escaped) ) {
				*str = sav;
				return false;
			}
			advance(str, m_escaped);

			if ( !hex(str, m_escaped) ) {
				*str = sav;
				return false;
			}
			advance(str, m_escaped);
			
			result += m_escaped;
			return true;
		}

		/** @brief Valid URI characters.
		 *
		 * Characters that are allowed in a URI.
		 * If found, the character (or escaped character) is stored in \a result and
		 * true is returned.
		 *
		 @verbatim
		 uric = reserved | unreserved | escaped
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, URI character was found.
		 * @see reserved() unreserved() escaped()
		 */
		inline bool uric(const unsigned char **str, std::string &result)
		{
			return reserved(str, result) != 0 ||
				unreserved(str, result) != 0 ||
				escaped(str, result);
		}

		/** @brief Valid URI characters without the slash '/'.
		 *
		 * Character that introduces the opaque part.
		 * If found, the character (or escaped character) is stored in \a result and
		 * true is returned.
		 *
		 @verbatim
		 uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |
		                 "&" | "=" | "+" | "$" | ","
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, URI character (without '/') was found.
		 * @see unreserved() escaped()
		 */
		inline bool uric_no_slash(const unsigned char **str, std::string &result)
		{
			if ( unreserved(str, result) != 0 ||
				escaped(str, result) )
			{
				return true;
			}

			unsigned char c = (*str)[0];
			switch (c) {
				case ';':
				case '?':
				case ':':
				case '@':
				case '&':
				case '=':
				case '+':
				case '$':
				case ',':
					advance(str, result);
					return true;
				default:
					return false;
			}
		}

		/** @brief Character of a path component.
		 *
		 * Character that is part of a path conmponent.
		 * If found, the character (or escaped character) is stored in \a result and
		 * true is returned.
		 *
		 @verbatim
		 pchar = unreserved | escaped |
		         ":" | "@" | "&" | "=" | "+" | "$" | ","
		            
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, path character was found.
		 * @see unreserved() escaped()
		 */
		inline bool pchar(const unsigned char **str, std::string &result)
		{
			if ( unreserved(str, result) != 0 ||
				escaped(str, result) )
			{
				return true;
			}

			unsigned char c = (*str)[0];
			switch (c) {
				case ':':
				case '@':
				case '&':
				case '=':
				case '+':
				case '$':
				case ',':
					advance(str, result);
					return true;
				default:
					return false;
			}
		}

		/** @brief Fragment identifier.
		 *
		 * Is not a regular part of an URI because it is not used to identify the URI.
		 * However, handled with the URI because it is directly written after the
		 * regular URI, separated by a crosshatch '#'. It is interpreted by the
		 * user agent to identify a part of the retrieved ressource. The fragment
		 * is copied to \a result and m_fragment. The fragment can be an empty string.
		 * m_hasFragment is set true if '#' was found.
		 *
		 @verbatim
		 fragment = *uric
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_fragment m_hasFragment uric()
		 */
		void fragment(const unsigned char **str, std::string &result);

		/** @brief Query component.
		 *
		 * The query component, interpreted by the ressource,
		 * separated by a question mark '?'. The query component
		 * is copied to \a result and m_query. The query can be an empty string.
		 * m_hasQuery is set true if '#' was found.
		 *
		 @verbatim
		 query = *uric
		 @endverbatim
		 *
		 * Within query component, the characters:
		 @verbatim
		 ";", "/", "?", ":", "@", "&", "=", "+", ",", "$"
		 @endverbatim
		 * are reserved (delimiters).
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_query m_hasQuery uric()
		 */
		void query(const unsigned char **str, std::string &result);

		/** @brief Path parameter component.
		 *
		 * Every path segment can be followed by a sequence of parameters
		 * separated by a semicolon ';'. The parameter
		 * is copied to \a result and m_param. The parameter can be an empty string.
		 *
		 @verbatim
		 param = *pchar
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_param pchar()
		 */
		void param(const unsigned char **str, std::string &result);

		/** @brief Single path segment.
		 *
		 * The segment is copied to \a result, m_segment and m_segmentContainer.
		 * The segment can be an empty string.
		 *
		 @verbatim
		 param = *pchar *( ";" param )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_segment m_segmentContainer pchar() param()
		 */
		void segment(const unsigned char **str, std::string &result);

		/** @brief Path segments.
		 *
		 * Path segments are separated by '/' and can be followed by parameters.
		 * The segments are copied to \a result, m_path_segments and to
		 * the m_segments list.
		 * The path segments can be an empty (since segment can be empty).
		 *
		 @verbatim
		 path_segments = segment *( "/" segment )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_segment m_segmentContainer segment()
		 */
		void path_segments(const unsigned char **str, std::string &result);

		/** @brief Port number.
		 *
		 * The port network number for the server. Is separated by a colon ':'
		 * from the host. The port number can be omitted (default port assumed).
		 * If found, the number is stored in m_port.
		 *
		 @verbatim
		 port = *digit
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_port digit()
		 */
		void port(const unsigned char **str, std::string &result);

		/** @brief Absolute path.
		 *
		 * The absolute path is the part of a path to the left of the first slash '/',
		 * including the slash (cannot be empty therefore, an empty path is recognized
		 * as relative path). If a path has been found, it is stored in m_abs_path and m_path,
		 * true is returned. Because of the used BNF for URI an absolute part can be part of
		 * a relative path.
		 *
		 @verbatim
		 abs_path = "/" path_segments
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, absolute path has been found
		 * @see m_abs_path m_path rel_path() path_segments()
		 */
		bool abs_path(const unsigned char **str, std::string &result);

		/** @brief Opaque part.
		 *
		 * If an absolute URI (one with a scheme) do not begin with a slash it is
		 * considered as an opaque part and is not further interpreted.
		 * If an opaque part has been found, it is stored in m_opaque_part and m_path,
		 * true is returned.
		 *
		 @verbatim
		 opaque_part = uric_no_slash *uric
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, opaque part has been found
		 * @see m_opaque_part uric() uric_no_slash()
		 */
		bool opaque_part(const unsigned char **str, std::string &result);

		/*
		// Absolute or opaque path, not used
		void path(const unsigned char **str, std::string &result);
		*/

		/** @brief IPv4 address.
		 *
		 * The IPv4 address are for numbers separated by a dot each.
		 * If an IPv4 address has been found, it is stored in m_ipV4address,
		 * true is returned.
		 *
		 @verbatim
		 ipV4address = 1*digit "." 1*digit "." 1*digit "." 1*digit "."
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, IPv4 address has been found
		 * @see m_ipV4address digit()
		 */
		bool ipV4address(const unsigned char **str, std::string &result);

		/*
		// Top label of a hostname, not used because every toplabel is a domainlabel
		bool toplabel(const unsigned char **str, std::string &result);
		*/

		/** @brief Domainlabel.
		 *
		 * Part of a hostname. The toplables (rightmost domainlabel of a hostname,
		 * that never starts with a digit) are also recogniced as domainlabels.
		 * Recognition of toplabels is handled by hostname().
		 * If a label has been found, it is stored in m_domainlabel,
		 * true is returned.
		 *
		 @verbatim
		 domainlabel = alphanum | alphanum *( alphanum | "-" ) alphanum
		 toplabel = alpha | alpha *( alphanum | "-" ) alphanum
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, domainlabel or toplabel has been found
		 * @see m_port m_domainlabel hostname()
		 */
		bool domainlabel(const unsigned char **str, std::string &result);

		/** @brief Hostname.
		 *
		 * If a Hostname has been found, it is stored in m_hostname, the domainlabels
		 * and the toplabel are stored in m_domainlabels, the toplabel also in
		 * m_toplabel and true is returned.
		 *
		 * The implementation searches aslon as possible for the domainlabels. If
		 * the last domainlabel fount is not a toplabel, the whole domainlabels are rejected.
		 * reg_name will match these parts.
		 *
		 * The last dot can be used to distinguish between absolute and local hostnames.
		 * m_hasTrailingDot is set true if the rightmost character in Hostname is a dot.
		 *
		 @verbatim
		 hostname = *( domainlabel "." ) toplabel [ "." ]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, hostname has been found
		 * @see m_domainlabels m_hostname m_toplabel m_hasTrailingDot domainlabel() 
		 */
		bool hostname(const unsigned char **str, std::string &result);

		/** @brief Host.
		 *
		 * A host is represented by a hostname or an IPv4 address.
		 * If a host has been found, it is stored in m_host and true is returned.
		 *
		 @verbatim
		 host = hostname | IPv4address
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, host has been found
		 * @see m_host hostname() ipV4Address()
		 */
		bool host(const unsigned char **str, std::string &result);

		/** @brief Host and port.
		 *
		 * A host followed by an optional port, separated by ':'.
		 * If found, it is stored in m_hostport and true is returned.
		 * m_hasPort is set true, if the port part was recognized, false
		 * if not.
		 *
		 @verbatim
		 hostport = host [ ":" port ]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, hostport has been found
		 * @see m_hostport m_hasPort host() port()
		 */
		bool hostport(const unsigned char **str, std::string &result);

		/** @brief Userinfo for an authority.
		 *
		 * The userinfo is followed by an at-sign "@".
		 * If found, it is stored in m_userinfo.
		 * The at-sign is not stored in m_userinfo. The string can be empty.
		 *
		 @verbatim
		 userinfo = *( unreserved | escaped |
		               ";" | ":" | "&" | "=" | "+" | "$" | "," )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_userinfo unreserved() escaped()
		 */
		void userinfo(const unsigned char **str, std::string &result);

		/** @brief Server-nased naming authority.
		 *
		 * Server component of the URI. 
		 * If found, it is stored in m_server. m_hasUserinfo is set
		 * if the at-sign "@" has been found.
		 * The string can be empty.
		 *
		 @verbatim
		 server = [ [ userinfo "@" ] hostport ]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_server m_hasUserinfo userinfo() hostport()
		 */
		void server(const unsigned char **str, std::string &result);

		/** @brief Registry-based naming authority.
		 *
		 * The structure is specific to the URI scheme, e.g. mailto.
		 * If found, it is stored in m_reg_name and
		 * true is returned.
		 *
		 @verbatim
		 reg_name = 1*( unreserved | escaped | "$" | ","
		                ";" | ":" | "@" | "&" | "=" | "+" )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, reg_name has been found
		 * @see m_reg_name unreserved() escaped()
		 */
		bool reg_name(const unsigned char **str, std::string &result);

		/** @brief Authority component.
		 *
		 * Top hierarchical element for naming authority. 
		 * If found, it is stored in m_authority. The string
		 * can be empty (because server can be empty). m_hasServer
		 * is set if a server (maybe empty) found.
		 *
		 @verbatim
		 authority = server | reg_name
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @see m_authority m_hasServer server() reg_name()
		 */
		void authority(const unsigned char **str, std::string &result);

		/** @brief Scheme component.
		 *
		 * Scheme to identify ressources, part of an absolute URI. 
		 * If found, it is stored in m_scheme and true is returned.
		 *
		 @verbatim
		 scheme = alpha *( alpha | digit | "+" | "-" | "." )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a scheme has been found
		 * @see m_scheme alpha() digit()
		 */
		bool scheme(const unsigned char **str, std::string &result);

		/** @brief First segment of a relative-path reference.
		 *
		 * If found, it is stored in m_rel_segment and true is returned.
		 *
		 @verbatim
		 rel_segment = 1*( unreserved | escaped |
		                   ";" | "@" | "&" | "=" | "+" | "$" | "," )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a rel_segment has been found
		 * @see m_rel_segment unreserved() escaped()
		 */
		bool rel_segment(const unsigned char **str, std::string &result);

		/** @brief Relative-path reference.
		 *
		 * If found, it is stored in m_rel_path and m_path and true is returned.
		 * Only within relative-path segments the character sequences "."
		 * and ".." have a special meaning: The current hierarchy level" and
		 * "the level above this hierarchy level".
		 *
		 @verbatim
		 rel_path = rel_segment [ abs_path]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a rel_path has been found
		 * @see m_rel_path m_path rel_segment() abs_path()
		 */
		bool rel_path(const unsigned char **str, std::string &result);

		/** @brief Network based absolute path.
		 *
		 * If found, it is stored in m_net_path (m_path by abspath())
		 * and true is returned. m_hasAuthority is set
		 * if an authority component has been found.
		 *
		 @verbatim
		 net_path = "//" authority [ abs_path]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a net_path has been found
		 * @see m_net_path authority() abs_path()
		 */
		bool net_path(const unsigned char **str, std::string &result);

		/** @brief Hierarchical part.
		 *
		 * If found, it is stored in m_hier_part
		 * and true is returned. m_hasQuery is set
		 * if an query component has been found.
		 *
		 @verbatim
		 hier_part = ( net_path | abs_path ) [ "?" query ]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a hier_part has been found
		 * @see m_hier_part m_hasQuery net_path() abs_path() query()
		 */
		bool hier_part(const unsigned char **str, std::string &result);

		/** @brief Relative URI reference.
		 *
		 * A URI reference relative to an absolute base URI.
		 *
		 * If found, it is stored in m_relativeURI
		 * and true is returned. m_hasQuery is set
		 * if an query component has been found.
		 *
		 @verbatim
		 relativeURI = ( net_path | abs_path | rel_path ) [ "?" query ]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if a relativeURI has been found
		 * @see m_relativeURI m_hasQuery net_path() abs_path() rel_path() query()
		 */
		bool relativeURI(const unsigned char **str, std::string &result);

		/** @brief Absolute URI.
		 *
		 * An absoulte URI with a scheme.
		 *
		 * If found, it is stored in m_absoluteURI and m_hasScheme is set,
		 * true is returned.
		 *
		 @verbatim
		 absoluteURI = scheme ":" ( hier_part | opaque_part )
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true if an absoluteURI has been found
		 * @see m_absoluteURI m_hasScheme scheme() hier_part() opaque_part()
		 */
		bool absoluteURI(const unsigned char **str, std::string &result);

		/** @brief URI reference.
		 *
		 * Tests and parses the generic syntax of a URI reference.
		 *
		 * If found, it is stored in m_uri_reference.
		 * If a (possibly empty) fragment has been fount, it
		 * is stored in m_fragment and m_hasFrament is set. An empty URI reference
		 * (absolute or relative URI) references the "current document".
		 * Returns true if there are no more trailing characters after parsing.
		 *
		 @verbatim
		 URI-reference = [ absoluteURI | relativeURI ] ["#" fragment]
		 @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @return true if there are no more trailing characters after parsing .
		 * @see m_uri_reference m_fragment m_hasfragment absoluteURI() relativeURI() fragment()
		 */
		bool uri_reference(const unsigned char **str);

		/** @brief Adds path segment to the segment list.
		 *
		 * Adds a pathsegment to the segment list handles the special meaning
		 * of "." (same hierarchy level) and ".." (hierarchy level above) segments.
		 *
		 * @param seg Segment to add
		 * @retval segList List of segments reflecting the hierarchy levels
		 */
		void addSegment(const CSegment &seg, std::list<CSegment> &segList) const;

	public:
		/** @brief Constructor, parses URI.
		 *
		 * @param anUri Char pointer to a URI
		 * @see parse() isValid()
		 */
		inline CUri(const char *anUri = 0) { parse(anUri); }

		/** @brief Constructor, parses URI.
		 *
		 * @param anUri String containing a URI
		 * @see parse() isValid()
		 */
		inline CUri(const std::string &anUri) { parse(anUri.c_str()); }

		/** @brief Copy constructor.
		 *
		 * @param uri Another URI
		 * @see CUri::operator=()
		 */
		inline CUri(const CUri &uri) { *this = uri; }

		/** @brief Parses URI.
		 *
		 * Parses an URI, the components are filled. Sets
		 * m_valid if the URI could be parsed.
		 *
		 * @param anUri Char pointer to a URI
		 * @return true if anURI was a vaild URI
		 * @see m_vaild uri_reference() isValid()
		 */
		bool parse(const char *anUri);

		/** @brief Parses URI.
		 *
		 * Parses an URI, the components are filled. Sets
		 * m_valid if the URI could be parsed.
		 *
		 * @param anUri String containing a URI
		 * @return true if anURI was a vaild URI
		 * @see m_valid uri_reference() isValid()
		 */
		inline bool parse(const std::string &anUri)
		{
			return parse(anUri.c_str());
		}

		/** @brief Assigns URI.
		 *
		 * Assignes a URI by parsing the string representation.
		 *
		 * @param anUri Char pointer containing a URI.
		 * @return This URI.
		 * @see parse()
		 */
		inline CUri &operator=(const char *anUri)
		{
			parse(anUri);
			return *this;
		}

		/** @brief Assigns URI.
		 *
		 * Assignes a URI by parsing the string representation.
		 *
		 * @param anUri String containing a URI.
		 * @return This URI.
		 * @see parse()
		 */
		inline CUri &operator=(const std::string &anUri)
		{
			parse(anUri.c_str());
			return *this;
		}

		/** @brief Assigns URI.
		 *
		 * Assignes a URI by parsing the string representation.
		 * Assumes that the \a uri is valid.
		 *
		 * @param uri CUri instance containing a vaild URI.
		 * @return This URI.
		 * @see parse() isValid()
		 */
		inline CUri &operator=(const CUri &uri)
		{
			if ( this == &uri ) {
				return *this;
			}
			parse(uri.toString());
			if ( !uri.isValid() )
				m_valid = false;
			return *this;
		}

		/** @brief Resolving relative references to absolute form.
		 *
		 * Resolving a relative URI reference (this) to the absolute
		 * form using the absolute base URI \a baseURI.
		 *
		 * @param baseUri CUri instance containig the absolute base URI
		 * @retval resultUriStr The absolute form is returned as string representation in this variable
		 * @return true, absolute form could be resolved.
		 * @see parse() isValid()
		 */
		bool makeAbsolute(const CUri &baseUri, std::string &resultUriStr) const;

		/** @brief Resolving relative references to absolute form.
		 *
		 * Converting a relative URI to the absolute
		 * form using the absolute base URI \a baseURI.
		 *
		 * @param baseUri CUri instance containig the absolute base URI
		 * @return true, absolute form could be resolved.
		 * @see parse() isValid()
		 */
		inline bool makeAbsolute(const CUri &baseUri) {
			std::string resultUriStr;
			if ( makeAbsolute(baseUri, resultUriStr) ) {
				*this = resultUriStr;
				return isValid();
			}
			return false;
		}

		/** @brief Resolving relative references to absolute form.
		 *
		 * Resolving a relative URI reference (this) to the absolute
		 * form using the absolute base URI \a baseURI.
		 *
		 * @param baseUri CUri instance containig the absolute base URI
		 * @retval resultUri The absolute form is returned in this variable
		 * @return true, absolute form could be resolved.
		 * @see parse() isValid()
		 */
		inline bool makeAbsolute(const CUri &baseUri, CUri &resultUri) {
			if ( &resultUri == this )
				return makeAbsolute(baseUri);

			std::string resultUriStr;
			if ( makeAbsolute(baseUri, resultUriStr) ) {
				resultUri = resultUriStr;
				return resultUri.isValid();
			}
			return false;
		}

		/** @brief Tests if valid contents.
		 *
		 * The parser (parse()) sets m_valid, if a valid URI was parsed.
		 * All query methods works only on valid CUri instances.
		 *
		 * @return true, the components of this URI are valid
		 */
		inline bool isValid() const { return m_valid; }

		/** @brief Converts a CUri to a string.
		 *
		 * @return The URI reference
		 */
		inline const std::string &toString() const { return m_uri_reference; }

		/** @brief Tests if URI reference is absolute.
		 * @return true, URI is absolute
		 */
		inline bool isAbsolute() const { return !m_absoluteURI.empty(); }

		/** @brief Gets the absolute URI.
		 * @return Absolute URI.
		 */
		inline const std::string &getAbsoluteUri() const { return m_absoluteURI; }

		/** @brief Tests if URI reference is relative.
		 * @return true, URI is relative.
		 */
		inline bool isRelative() const { return !m_relativeURI.empty(); }

		/** @brief Gets the relative URI.
		 * @return Relative URI.
		 */
		inline const std::string &getRelativeUri() const { return m_relativeURI; }

		/** @brief Tests, if URI represents the "current document".
		 *
		 * A URI represents the current document if absolute and relative parts are empty.
		 *
		 * @return true, if URI represents the "current document".
		 */
		inline bool isCurrentDocument() const
		{
			return
				m_absoluteURI.empty() &&
				m_relativeURI.empty();
		}

		/** @brief Tests, if URI has defined a scheme component (is an absolute URI).
		 * @return true, URI has a scheme component.
		 */
		inline bool hasScheme() const { return m_hasScheme; }

		/** @brief Gets the scheme component.
		 * @return The scheme component.
		 */
		inline const std::string &getScheme() const { return m_scheme; }

		/** @brief Tests, if URI has defined an opaque part.
		 * @return true, if URI has an opaque part.
		 */
		inline bool hasOpaquePart() const { return !m_opaque_part.empty(); }

		/** @brief Gets the opaque part.
		 * @return The opaque part.
		 */
		inline const std::string &getOpaquePart() const { return m_opaque_part; }

		/** @brief Tests, if URI has defined an hierarchy part.
		 * @return true, if URI has an hierarchy part.
		 */
		inline bool hasHierPart() const { return !m_hier_part.empty(); }

		/** @brief Gets the hierarchy part.
		 * @return The hierarchy part.
		 */
		inline const std::string &getHierPart() const { return m_hier_part; }

		/** @brief Gets the path component.
		 * 
		 * The path component is either an absolute path, relative path or
		 * the opaque part.
		 *
		 * @return The path component.
		 */
		inline const std::string &getPath() const { return m_path; }

		/** @brief Gets the network path component.
		 *
		 * Contains a (possibly empty) authority.
		 *
		 * @return The network path component.
		 */
		inline const std::string &getNetPath() const { return m_net_path; }

		/** @brief Tests, if path component is absolute.
		 *
		 * Either network path or absolute path.
		 *
		 * @return true, if path component was absolute.
		 */
		inline const bool hasAbsPath() const { return !m_path.empty() && m_path[0] == '/'; }

		/** @brief Gets the path component to the right of the leftmost "/".
		 *
		 * Is part of network path and relative path. Can also return a
		 * path if hasAbsPath() is false, because it is the component to the
		 * right of the leftmost "/" even by relative pathes.
		 *
		 * @return The absolute path component.
		 */
		inline const std::string &getAbsPath() const { return m_abs_path; }

		/** @brief Tests, if path component is relative.
		 * @return true, if path component was relative.
		 */
		inline const bool hasRelPath() const { return m_path.empty() || m_path[0] != '/'; }

		/** @brief Gets the relative path component
		 *
		 * Contains the relative segments and the following segments
		 * Relative path components also have a relative segment (getRelSegment())
		 *
		 * @return Relative path component.
		 */
		inline const std::string &getRelPath() const { return m_rel_path; }

		/** @brief Tests, if URI has defined a query component.
		 * @return true, if path component was relative.
		 */
		inline bool hasQuery() const { return m_hasQuery; }

		/** @brief Gets the query component.
		 * @return Query component.
		 */
		inline const std::string &getQuery() const { return m_query; }

		/** @brief Tests, if URI has defined a fragment component.
		 * @return true, if fragment component was relative.
		 */
		inline bool hasFragment() const { return m_hasFragment; }

		/** @brief Gets the fragment component.
		 * @return Fragment component.
		 */
		inline const std::string &getFragment() const { return m_fragment; }

		/** @brief Tests, if URI has defined a naming authority of a network path.
		 * @return true, if authority is defined.
		 */
		inline bool hasAuthority() const { return m_hasAuthority; }

		/** @brief Gets the naming authority of a network path.
		 * @return Naming authority part of a network path.
		 */
		inline const std::string &getAuthority() const { return m_authority; }

		/** @brief Tests, if URI has defined the userinfo of an authority.
		 * @return true, if userinfo of an authority is defined.
		 */
		inline bool hasUserinfo() const { return m_hasUserinfo; }

		/** @brief Gets the userinfo of an authority.
		 * @return userinfo.
		 */
		inline const std::string &getUserinfo() const { return m_userinfo; }

		/** @brief Tests, if the server part of a naming authority.
		 * @return true, if the server part of a naming authority is defined
		 */
		inline bool hasServer() const { return m_hasServer; }

		/** @brief Gets the server part of a naming authority.
		 * @return Server part of a naming authority.
		 */
		inline const std::string &getServer() const { return m_server; }

		/** @brief Tests, if the naming authority is registry based.
		 * @return true, if the server part of a network path is defined
		 */
		inline bool hasRegName() const { return !m_reg_name.empty(); }

		/** @brief Gets the registry based naming authority.
		 * @return Registry based naming authority.
		 */
		inline const std::string &getRegName() const { return m_reg_name; }

		/** @brief Gets the host part of the naming authority.
		 *
		 * Contains hostname or IPv4 address
		 *
		 * @return Host part of the naming authority.
		 */
		inline const std::string &getHost() const { return m_host; }

		/** @brief Gets the host part with portnumber of the naming authority.
		 *
		 * Contains host and optional a port, separated by a colon ":"
		 *
		 * @return Hostport part of the naming authority.
		 */
		inline const std::string &getHostport() const { return m_hostport; }

		/** @brief Gets the IPv4 address if present.
		 * @return IPv4 address.
		 */
		inline const std::string &getIPv4address() const { return m_ipV4address; }

		/** @brief Gets the hostname if present.
		 * @return Hostname.
		 */
		inline const std::string &getHostname() const { return m_hostname; }

		/** @brief Tests, if hostname has a trailing dot.
		 * @return true, if hostname has a trailing dot.
		 */
		inline bool hasTrailingDot() const { return m_hasTrailingDot; }

		/** @brief Tests, if host has a port number.
		 *
		 * The portnumber can be empty, then the default port is used.
		 *
		 * @return true, if hostname has a port number.
		 */
		inline bool hasPort() const { return m_hasPort; }

		/** @brief Gets the (possibly empty) port number.
		 * @return Port number.
		 */
		inline const std::string &getPort() const { return m_port; }

		/** @brief Gets the first domainlabel.
		 *
		 * Constant iterator for domainlabels, including the toplabel.
		 *
		 * @return Constant iterator for domainlabels.
		 * @see domainlabelsEnd()
		 */
		inline domainlabels_const_iterator domainlabelsBegin() const
		{
			return m_domainlabels.begin();
		}

		/** @brief Stop mark for domainlabels iterator.
		 *
		 * Constant iterator end for domainlabels, including the toplabel.
		 *
		 * @return Constant iterator end for domainlabels.
		 * @see domainlabelsBegin()
		 */
		inline domainlabels_const_iterator domainlabelsEnd() const
		{
			return m_domainlabels.end();
		}

		/** @brief Gets the number of domainlabels.
		 *
		 * Number of domainlabels, including the toplabel.
		 *
		 * @return Number of domainlabels.
		 */
		inline domainlabels_size_type domainlabelsSize() const
		{
			return m_domainlabels.size();
		}

		/** @brief Gets the toplabel
		 *
		 * Gets the toplabel of the domainlabels.
		 *
		 * @return The top label.
		 */
		inline const std::string &getToplabel() const { return m_toplabel; }

		/** @brief Gets the (first) relative segment of a relative path.
		 * @return The relative segment of a relative path.
		 */
		inline const std::string &getRelSegment() const { return m_rel_segment; }

		/** @brief Gets the path segments.
		 *
		 * Gets the path segments of an absolute path or the segements that follow
		 * the relative segment. The segments are separated by '/'. The first '/'
		 * is not part of the segments. The segments can be iterated ny
		 * segmentsBegin() - segmentsEnd().
		 *
		 * @return The path segments
		 */
		inline const std::string &getPathSegments() const { return m_path_segments; }

		/** @brief Gets the first of the path segments.
		 *
		 * Gets the iterator to the first of the path segments (not including the
		 * relative segment). If the path endend with a '/', the last segment
		 * has an empty name.
		 *
		 * @return The constant iterator for the path segmnents
		 * @see getRelSegments() getPathSegments() segmentsEnd()
		 */
		inline const_iterator segmentsBegin() const
		{
			return m_segments.begin();
		}

		/** @brief Gets the end of the path segments.
		 *
		 * Constant iterator end for path segments.
		 *
		 * @return Constant iterator end for path segments.
		 * @see segmentsBegin()
		 */
		inline const_iterator segmentsEnd() const
		{
			return m_segments.end();
		}

		/** @brief Gets the numer of the path segments
		 *
		 * The relative segment of a relative path is not counted.
		 *
		 * @return Number of path segments.
		 */
		inline size_type segmentsSize() const
		{
			return m_segments.size();
		}
	};
}

#endif // // _RICPP_STREAMS_URI_H
