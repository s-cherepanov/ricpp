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
	/** @brief Class to represent a absolute or relative URI.
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

			/** @brief Size type for the size of the parameter list.
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

		/** @brief Constant iterator for the size of the segment list.
		 */
		typedef std::list<CSegment>::size_type size_type;

		/** @brief Constant iterator for the domainlabel list.
		 */
		typedef std::list<std::string>::const_iterator domainlabels_const_iterator;

		/** @brief Size type for the size of the domainlabel list.
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
		 * true, if an '?' is found in an approriate place
		 */
		bool m_hasQuery;

		/** @brief The query.
		 *
		 * The query is stored without the leading '?' and can be an empty string.
		 */
		std::string m_query;

		/** @brief URI contains a fragment component
		 *
		 * true, if an '#' is found in an approriate place
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

		/** @brief temporary store of a segment with parameters.
		 */
		CSegment m_segmentContainer;
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
		 * @verbatim
		 * digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
		 *         "8" | "9"
		 * @endverbatim
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
		 * @verbatim
		 * upalpha = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
		 *           "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
		 *           "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
		 * @endverbatim
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
		 * @verbatim
		 * lowalpha = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
		 *            "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
		 *            "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		 * @endverbatim
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
		 * @verbatim
		 * alpha = lowalpha | upalpha
		 * @endverbatim
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
		 * @verbatim
		 * alphanum = alpha | digit
		 * @endverbatim
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
		 * @verbatim
		 * hex = digit | "A" | "B" | "C" | "D" | "E" | "F" |
		 *               "a" | "b" | "c" | "d" | "e" | "f"
		 * @endverbatim
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
		 * @verbatim
		 * mark = "-" | "_" | "." | "!" | "~" | "*" | "'" |
		 *        "(" | ")"
		 * @endverbatim
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
		 * @verbatim
		 * unreserved = "alphanum" | "mark"
		 * @endverbatim
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
		 * @verbatim
		 * reserved = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
		 *            "$" | ","
		 * @endverbatim
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
		 * @verbatim
		 * escaped = "%" hex hex
		 * @endverbatim
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
		 * @verbatim
		 * uric = reserved | unreserved | escaped
		 * @endverbatim
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
		 * @verbatim
		 * uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |
		 *                 "&" | "=" | "+" | "$" | ","
		 *            
		 * @endverbatim
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
		 * @verbatim
		 * pchar = unreserved | escaped |
		 *         ":" | "@" | "&" | "=" | "+" | "$" | ","
		 *            
		 * @endverbatim
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
		 * @verbatim
		 * fragment = *uric
		 * @endverbatim
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
		 * @verbatim
		 * query = *uric
		 * @endverbatim
		 *
		 * Within query component, the characters:
		 * @verbatim
		 * ";", "/", "?", ":", "@", "&", "=", "+", ",", "$"
		 * @endverbatim
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
		 * @verbatim
		 * param = *pchar
		 * @endverbatim
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
		 * @verbatim
		 * param = *pchar *( ";" param )
		 * @endverbatim
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
		 * @verbatim
		 * path_segments = segment *( "/" segment )
		 * @endverbatim
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
		 * @verbatim
		 * port = *digit
		 * @endverbatim
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
		 * as relative path). If a path is found, it is stored in m_abs_path and m_path,
		 * true is returned. Because of the used BNF for URI an absolute part can be part of
		 * a relative path.
		 *
		 * @verbatim
		 * abs_path = "/" path_segments
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, absolute path is found
		 * @see m_abs_path m_path rel_path() path_segments()
		 */
		bool abs_path(const unsigned char **str, std::string &result);

		/** @brief Opaque part.
		 *
		 * If an absolute URI (one with a scheme) do not begin with a slash it is
		 * considered as an opaque part and is not further interpreted.
		 * If an opaque part is found, it is stored in m_opaque_part and m_path,
		 * true is returned.
		 *
		 * @verbatim
		 * opaque_part = uric_no_slash *uric
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, opaque part is found
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
		 * If an IPv4 address is found, it is stored in m_ipV4address,
		 * true is returned.
		 *
		 * @verbatim
		 * ipV4address = 1*digit "." 1*digit "." 1*digit "." 1*digit "."
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, IPv4 address is found
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
		 * If a label is found, it is stored in m_domainlabel,
		 * true is returned.
		 *
		 * @verbatim
		 * domainlabel = alphanum | alphanum *( alphanum | "-" ) alphanum
		 * toplabel = alpha | alpha *( alphanum | "-" ) alphanum
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, domainlabel or toplabel is found
		 * @see m_port m_domainlabel hostname()
		 */
		bool domainlabel(const unsigned char **str, std::string &result);

		/** @brief Hostname.
		 *
		 * If a Hostname is found, it is stored in m_hostname, the domainlabels
		 * and the toplabel are stored in m_domainlabels, the toplabel also in
		 * m_toplabel and true is returned.
		 *
		 * The last dot can be used to distinguish between absolute and local hostnames.
		 * m_hasTrailingDot is set true if the rightmost character in Hostname is a dot.
		 *
		 * @verbatim
		 * hostname = *( domainlabel "." ) toplabel [ "." ]
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, domainlabel or toplabel is found
		 * @see m_domainlabels m_hostname m_toplabel m_hasTrailingDot domainlabel() 
		 */
		bool hostname(const unsigned char **str, std::string &result);

		/** @brief Host.
		 *
		 * A host is represented by a hostname or an IPv4 address.
		 * If a host is found, it is stored in m_host and true is returned.
		 *
		 * @verbatim
		 * host = hostname | IPv4address
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, domainlabel or toplabel is found
		 * @see m_host hostname() ipV4Address()
		 */
		bool host(const unsigned char **str, std::string &result);

		/** @brief Host and port
		 *
		 * A host followed by an optional port, separated by ':'.
		 * If found, it is stored in m_hostport and true is returned.
		 * m_hasPort is set true, if the port part was recognized, false
		 * if not.
		 *
		 * @verbatim
		 * hostport = host [ ":" port ]
		 * @endverbatim
		 *
		 * @param str Address of a string pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters read.
		 * @return true, domainlabel or toplabel is found
		 * @see m_hostport m_hasPort host() port()
		 */
		bool hostport(const unsigned char **str, std::string &result);

		void userinfo(const unsigned char **str, std::string &result);
		void server(const unsigned char **str, std::string &result);
		bool reg_name(const unsigned char **str, std::string &result);
		void authority(const unsigned char **str, std::string &result);
		bool scheme(const unsigned char **str, std::string &result);
		bool rel_segment(const unsigned char **str, std::string &result);
		bool rel_path(const unsigned char **str, std::string &result);
		bool net_path(const unsigned char **str, std::string &result);
		bool hier_part(const unsigned char **str, std::string &result);
		bool relativeURI(const unsigned char **str, std::string &result);
		bool absoluteURI(const unsigned char **str, std::string &result);
		bool uri_reference(const unsigned char **str);

		void addSegment(const CSegment &seg, std::list<CSegment> &segList) const;

	public:
		inline CUri(const char *anUri = 0) { parse(anUri); }
		inline CUri(const CUri &uri) { *this = uri; }

		bool parse(const char *anUri);
		inline bool parse(const std::string &anUri)
		{
			return parse(anUri.c_str());
		}

		inline CUri &operator=(const char *anUri)
		{
			parse(anUri);
			return *this;
		}

		inline CUri &operator=(const std::string &anUri)
		{
			parse(anUri.c_str());
			return *this;
		}

		inline CUri &operator=(const CUri &uri)
		{
			if ( this == &uri ) {
				return *this;
			}
			parse(uri.toString());
			return *this;
		}

		bool makeAbsolute(const CUri &baseUri, std::string &resultUriStr) const;

		inline bool isValid() const { return m_valid; }
		inline const std::string &toString() const { return m_uri_reference; }

		inline bool isAbsolute() const { return !m_absoluteURI.empty(); }
		inline const std::string &getAbsoluteUri() const { return m_absoluteURI; }

		inline bool isRelative() const { return !m_relativeURI.empty(); }
		inline const std::string &getRelativeUri() const { return m_relativeURI; }

		inline bool isCurrentDocument() const
		{
			return
				m_path.empty() &&
				m_opaque_part.empty() &&
				m_scheme.empty() &&
				m_authority.empty();
		}

		inline bool hasScheme() const { return m_hasScheme; }
		inline const std::string &getScheme() const { return m_scheme; }

		inline bool hasOpaquePart() const { return !m_opaque_part.empty(); }
		inline const std::string &getOpaquePart() const { return m_opaque_part; }

		inline bool hasHierPart() const { return !m_hier_part.empty(); }
		inline const std::string &getHierPart() const { return m_hier_part; }

		inline const std::string &getPath() const { return m_path; }
		inline const std::string &getNetPath() const { return m_net_path; }
		inline const bool hasAbsPath() const { return !m_path.empty() && m_path[0] == '/'; }
		inline const std::string &getAbsPath() const { return m_abs_path; }
		inline const bool hasRelPath() const { return m_path.empty() || m_path[0] != '/'; }
		inline const std::string &getRelPath() const { return m_rel_path; }

		inline bool hasQuery() const { return m_hasQuery; }
		inline const std::string &getQuery() const { return m_query; }

		inline bool hasFragment() const { return m_hasFragment; }
		inline const std::string &getFragment() const { return m_fragment; }

		inline bool hasAuthority() const { return m_hasAuthority; }
		inline const std::string &getAuthority() const { return m_authority; }

		inline bool hasUserinfo() const { return m_hasUserinfo; }
		inline const std::string &getUserinfo() const { return m_userinfo; }

		inline bool hasServer() const { return m_hasServer; }
		inline const std::string &getServer() const { return m_server; }

		inline bool emptyRegName() const { return m_reg_name.empty(); }
		inline const std::string &getRegName() const { return m_reg_name; }

		inline const std::string &getHost() const { return m_host; }
		inline const std::string &getHostport() const { return m_hostport; }
		inline const std::string &getIPv4address() const { return m_ipV4address; }

		inline bool hasTrailingDot() const { return m_hasTrailingDot; }
		inline const std::string &getHostname() const { return m_hostname; }

		inline bool hasPort() const { return m_hasPort; }
		inline const std::string &getPort() const { return m_port; }

		inline domainlabels_const_iterator domainlabelsBegin() const
		{
			return m_domainlabels.begin();
		}
		inline domainlabels_const_iterator domainlabelsEnd() const
		{
			return m_domainlabels.end();
		}
		inline domainlabels_size_type domainlabelsSize() const
		{
			return m_domainlabels.size();
		}

		inline const std::string &getTopLabel() const { return m_toplabel; }

		inline const std::string &getRelSegment() const { return m_rel_segment; }
		inline const std::string &getPathSegments() const { return m_path_segments; }

		inline const_iterator segmentsBegin() const
		{
			return m_segments.begin();
		}
		inline const_iterator segmentsEnd() const
		{
			return m_segments.end();
		}
		inline size_type segmentsSize() const
		{
			return m_segments.size();
		}
	};
}

#endif // // _RICPP_STREAMS_URI_H
