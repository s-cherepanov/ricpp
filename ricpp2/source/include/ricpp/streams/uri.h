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
	 * described in rfc3986 are not suported. A string
	 * representing a URI can be splitted to it's
	 * components by CUri::parse(). Please read
	 * "rfc3986 Unified Identifiers (URI): Generic Syntax"
	 * for more information (also for full BNF). The grammar
	 * used for this class is changed so that the fragment
	 * is not included in absolute-uri and relative-uri.
	 * Recognition but should be functional
	 * aquivalent (used rfc 2396 style for uri-reference rule,
	 * also the syntax of the BNF is not ABNF).
	 *
	 * @see http://www.ietf.org/rfc/rfc3986.txt
	 */
	class CUri {
	public:
		/** @brief Constant iterator for the segment list.
		 */
		typedef std::list<std::string>::const_iterator segments_const_iterator;

		/** @brief Type for the size of the segment list.
		 */
		typedef std::list<std::string>::size_type segments_size_type;

		/** @brief Types of the pathes.
		 */
		enum EnumPathType {
			pathTypeEmpty = 0, ///!< @brief Path is empty.
			pathTypeAbsolute,  ///!< @brief Path is absolute with leading slash '/'.
			pathTypeNoScheme,  ///!< @brief @brief Path begins with non-colon segment (not used).
			pathTypeRootless   ///!< Relative path.
		};

		/** @brief Types of the IP addresses
		 */
		enum EnumIPAddrType {
			ipAddrTypeEmpty = 0, ///!< @brief No authority set.
			ipAddrTypeV4Address, ///!< @brief IPv4 address.
			ipAddrTypeV6Address, ///!< @brief IPv6 address.
			ipAddrTypeVFuture,   ///!< @brief Generic IP address.
			ipRegName            ///!< @brief Registry based name or host.
		};

	private:
		/** @brief true, if a valid URI has been parsed correctly.
		 */
		bool m_valid;

		/** @brief Type of the path component of the URI.
		 */
		EnumPathType m_pathType;

		/** @brief Type of the ip adress, registry based name of the authority of the URI.
		 */
		EnumIPAddrType m_ipAddrType;

		/** @brief The path component.
		 *
		 * The absolute or relative path or the opaque part of the URI.
		 */
		std::string m_path;

		/** @brief Temporary string of an pct_encoded character.
		 *
		 * If an percent encoded character (%nm) is parsed, it is stored temporarily
		 * in this member.
		 */
		std::string m_pct_encoded;

		/** @brief Temporary string of an 16bit hexadecimal (4 hexdigits).
		 */
		std::string m_h16;

		/** @brief Last significant bits of a IPv6 address.
		 */
		std::string m_ls32;

		/** @brief 8 bit decimal encoded number (octet).
		 */
		std::string m_dec_octet;

		/** @brief The URI reference.
		 *
		 * This member is used to store the whole string parsed.
		 */
		std::string m_uri_reference;

		/** @brief The absolute URI.
		 *
		 * Contains the absolute URI (without fragment), empty URI is relative.
		 */
		std::string m_absolute_uri;

		/** @brief The relative URI.
		 *
		 * Contains the relative URI (without fragment), empty URI is absolute.
		 */
		std::string m_relative_uri;

		/** @brief true, if a scheme was found.
		 *
		 * If there has been a scheme found (trailing  ':'), m_scheme will
		 * contain it and will not be empty.
		 */
		bool m_hasScheme;

		/** @brief The scheme of an absolute URI.
		 *
		 * The scheme is stored without the trailing ':', is not empty if found.
		 *
		 */
		std::string m_scheme;

		/** @brief The hierarchical part of URI.
		 */
		std::string m_hier_part;
	
		/** @brief The host of a network path
		 */
		std::string m_host;

		/** @brief URI contains a query component
		 *
		 * True, if an '?' has been found introducing a query.
		 */
		bool m_hasQuery;

		/** @brief The query.
		 *
		 * The query has been stored without the leading '?' and can be an empty string.
		 */
		std::string m_query;

		/** @brief URI contains a fragment component.
		 *
		 * true, if an '#' has been found introducing a fragment.
		 */
		bool m_hasFragment;

		/** @brief The fragment.
		 *
		 * The fragment is stored without the leading '#' and can be an empty string.
		 */
		std::string m_fragment;

		/** @brief The authority.
		 *
		 * The authority can be an empty string ( e.g. "file:///c:/" contains an
		 * empty authority after the leading "//").
		 */
		bool m_hasAuthority;

		/** @brief The authority.
		 *
		 * The authority is stored without the leading "//" and can be an empty string.
		 */
		std::string m_authority;

		/** @brief Authority contains a user information.
		 *
		 * The at-sign '@' has been found trailing a user information component.
		 */
		bool m_hasUserinfo;

		/** @brief The user information.
		 *
		 * The authority contains a user information, can be an empty string.
		 */
		std::string m_userinfo;

		/** @brief The registry based name of the URI.
		 *
		 * The authority contains a registry based name, can be an empty string.
		 * Alternatively it can contain an IP literal or an IPv4 address.
		 */
		std::string m_reg_name;

		/** @brief The host contains an IP literal (IPv6 or future address).
		*
		 * Alternatively it can contain a registry based name or an
		 * IPv4adress.
		 */
		std::string m_ip_literal;

		/** @brief The host contains an IPv6 address.
		 *
		 * Alternatively it can contain a registry based name,
		 * or an IPv4 address. An IPv6address can
		 * also contain an IPv4 part stored in m_ipV4Address.
		 */
		std::string m_ipV6Address;

		/** @brief The host contains a generic IP address.
		 */
		std::string m_ipVFuture;

		/** @brief The host contains an IP4 address.
		 *
		 * Can also be a part of the IPv6adress.
		 * Alternatively it can contain a registry based name,
		 * an IP literal or (IPv6) address.
		 *
		 */
		std::string m_ipV4Address;

		/** @brief, true if there is a port number in the authority.
		 *
		 * Found the ':' of a port number.
		 */
		bool m_hasPort;

		/** @brief The port address without leading ':'.
		 *
		 * The port number can be an empty string.
		 *
		 */
		std::string m_port;

		/** @brief Temporary storage for a path segment.
		 *
		 * The segments are stored in m_segments.
		 *
		 */
		std::string m_segment;

		/** @brief List of the segments of a path component.
		 */
		std::list<std::string> m_segments;

		/** @brief Clears all components.
		 *
		 * A cleared URI is a valid URI.
		 */
		void clearAll();

		/** @brief Advances the input pointer.
		 *
		 * Advances the input pointer \a *str \a n times and
		 * stores the characters in \a result.
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @param steps Number of advances.
		 */
		inline void advance(const unsigned char **str,
		                    std::string &result,
							int n=1)
		{
			while ( n-- > 0 ) {
				result += ((*str)++)[0];
			}
		}

		/** @brief Matches a sequence, advances the input pointer.
		 *
		 * Matches the sequence \a matchStr and advances the input
		 * pointer. If the whole string could not be matched,
		 * the input pointer is restored.
		 *
		 * @param matchStr String to match
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, the \a matchStr matches.
		 */
		inline bool match( const char *matchStr,
						   const unsigned char **str,
		                   std::string &result)
		{
			// An empty string matches
			if ( !matchStr || !*matchStr )
				return true;

			const unsigned char *sav = *str;
			const unsigned char *ptr = (const unsigned char *)matchStr;
			while ( *ptr && *str[0] == *ptr ) {
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
		 * Matches one character of \a matchStr and advances the pointer if
		 * one character matches. If matchStr is empty no
		 * character matches,
		 *
		 * @param matchStr One character of the string to match.
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, one character that matches or 0, if no character matches.
		 */
		inline unsigned char matchOneOf( const char *matchStr,
						        const unsigned char **str,
		                        std::string &result)
		{
			if ( !matchStr || !*matchStr )
				return 0;

			const unsigned char *ptr = (const unsigned char *)matchStr;
			for ( ; *ptr; ++ptr ) {
				if ( *str[0] == *ptr ) {
					result += ((*str)++)[0];
					return *ptr;
				}
			}
			return 0;
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
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return 0, no digit or digit that matches.
		 */
		inline unsigned char digit(const unsigned char **str,
		                           std::string &result)
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
		 * it is appended to \a result and is returned.
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
		inline unsigned char upalpha(const unsigned char **str,
		                             std::string &result)
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
		 * it is appended to \a result and is returned.
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
		inline unsigned char lowalpha(const unsigned char **str,
		                              std::string &result)
		{
			unsigned char c = (*str)[0];
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
		 * it is appended to \a result and is returned.
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
		inline unsigned char alpha(const unsigned char **str,
		                           std::string &result)
		{
			return lowalpha(str, result) || upalpha(str, result);
		}

		/** @brief Alphanumerical character.
		 *
		 * Test if current input character is a letter or a digit.
		 * If the current input character is alphanumeric ('A'-'Z' or 'a'-'z', '0'-'9'),
		 * it is appended to \a result and is returned.
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
		inline unsigned char alphanum(const unsigned char **str,
		                              std::string &result)
		{
			return alpha(str, result) || digit(str, result);
		}

		/** @brief Hexadecimal digit.
		 *
		 * Test if current input character is hexadecimal digit.
		 * If the current input character is a hexadecimal digit
		 * ('0'-'9', 'A'-'f' or 'a'-'f'),
		 * it is appended to \a result and is returned.
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
		 * @return 0, no hexdigit or the hexdigit that matches.
		 * @see digit()
		 */
		inline unsigned char hexdig(const unsigned char **str,
		                         std::string &result)
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

		/** @brief Unreserved characters.
		 *
		 * Characters that are allowed in a URI but have no special purpose.
		 * If found, the character is appended to \a result and is returned. Unreserved
		 * characters can be pct_encoded without changing the semantics of a URI,
		 * but there is no need to do that in general.
		 *
		 @verbatim
		 unreserved = "alpha" | "digit" | "-" | "." | "_" | "~"
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return 0 or the character that matches.
		 * @see alpha() digit()
		 */
		inline unsigned char unreserved(const unsigned char **str,
		                                std::string &result)
		{
			unsigned char c = (*str)[0];

			switch (c) {
				case '-':
				case '.':
				case '_':
				case '~':
					advance(str, result);
					return c;
				default:
					return alphanum(str, result);
			}
		}

		/** @brief Generic delimiters.
		 *
		 @verbatim
		 gen_delims = ":" | "/" | "?" | "#" | "[" | "]" | "@"
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return 0 or the character that matches.
		 */
		inline unsigned char gen_delims(const unsigned char **str,
		                              std::string &result)
		{
			unsigned char c = (*str)[0];
			switch (c) {
				case ':':
				case '/':
				case '?':
				case '#':
				case '[':
				case ']':
				case '@':
					advance(str, result);
					return c;
				default:
					return 0;
			}
		}

		/** @brief Subcomponents delimiters.
		 *
		 @verbatim
		 sub_delims = "!" | "$" | "&" | "'" | "(" | ")" |
		              "*" | "+" | "," | ";" | "="
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return 0 or the reserved character that matches.
		 */
		inline unsigned char sub_delims(const unsigned char **str,
		                              std::string &result)
		{
			unsigned char c = (*str)[0];
			switch (c) {
				case '!':
				case '$':
				case '&':
				case '\'':
				case '(':
				case ')':
				case '*':
				case '+':
				case ',':
				case ';':
				case '=':
					advance(str, result);
					return c;
				default:
					return 0;
			}
		}

		/** @brief Reserved characters.
		 *
		 * Characters that are allowed in a URI and have the special purpose of
		 * delimeters. If found, the character is appended to \a result and
		 * is returned.
		 * Reserved characters have to be pct_encoded, if they occur in the data part
		 * (than no delimiting purposes any more).
		 *
		 @verbatim
		 reserved = gen_delims | sub_delims
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return 0 or the reserved character found.
		 */
		inline unsigned char reserved(const unsigned char **str,
		                              std::string &result)
		{
			return gen_delims(str, result) || sub_delims(str, result);
		}

		/** @brief Escape sequence.
		 *
		 * Test if current input is an escape character (e.g. "%20" for blank)
		 * If found, the current escape sequence it is appended to \a result and stored in
		 * m_pct_encoded and true is returned.
		 *
		 @verbatim
		 pct_encoded = "%" hex hex
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, escape sequence found.
		 * @see m_pct_encoded hexdig()
		 */
		inline bool pct_encoded(const unsigned char **str,
		                    std::string &result)
		{
			const unsigned char *sav = *str;
			m_pct_encoded = "";
			if ( (*str)[0] != '%' ) {
				return false;
			}
			advance(str, m_pct_encoded);

			if ( !hexdig(str, m_pct_encoded) ) {
				*str = sav;
				return false;
			}
			if ( !hexdig(str, m_pct_encoded) ) {
				*str = sav;
				return false;
			}
			
			result += m_pct_encoded;
			return true;
		}

		/** @brief 16bit hexadecimal sequence (4 hexdigits).
		 *
		 * If found, the current h16 it is appended to \a result and stored in
		 * m_h16 and true is returned.
		 *
		 @verbatim
		 h16 = 1*4hexdig
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, a h16 matches.
		 * @see m_h16 hexdig()
		 */
		inline bool h16(const unsigned char **str,
		                std::string &result)
		{
			const unsigned char *sav = *str;
			m_h16 = "";
			int i;

			for ( i = 0; i < 4 && hexdig(str, m_h16); ++i );
			if ( i == 0 ) {
				*str = sav;
				m_h16 = "";
				return false;
			}

			result += m_h16;
			return true;
		}

		/** @brief 8 Bit decimal 0-255.
		 *
		 * If found, the current octet it is appended to \a result and stored in
		 * m_dec_octet and true is returned.
		 *
		 @verbatim
		 dec_octet = digit |
		             0x31-39 digit |
					 "1" 2digit |
					 "2" %x30-34 digit |
					 "25" %x30-35
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, an octet matches.
		 * @see m_dec_octet digit()
		 */
		bool dec_octet(const unsigned char **str,
		               std::string &result);

		/** @brief Character of a path component.
		 *
		 * Character that is part of a path component.
		 * If found, the character (or percent encoded character sequence)
		 * is appended to \a result and true is returned.
		 *
		 @verbatim
		 pchar = unreserved | pct_encoded | sub_delims |
		         ":" | "@"
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, path character was found.
		 * @see unreserved() pct_encoded()
		 */
		inline bool pchar(const unsigned char **str,
		                  std::string &result)
		{
			return
				unreserved(str, result) != 0 ||
			    pct_encoded(str, result) ||
				sub_delims(str, result) != 0 ||
				matchOneOf(":@", str, result);
		}

		/** @brief Fragment identifier.
		 *
		 * Is not a regular part of an URI because it is not used to identify the URI.
		 * However, handled with the URI because it is directly written after the
		 * regular URI, separated by a crosshatch '#'. It is interpreted by the
		 * user agent to identify a part of the retrieved ressource. The fragment
		 * is appanded on \a result and stored in m_fragment.
		 * The fragment can be an empty string.
		 * m_hasFragment is set, if '#' was found.
		 *
		 @verbatim
		 fragment = *( pchar | "/" | "?" )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_fragment m_hasFragment uric()
		 */
		void fragment(const unsigned char **str,
		              std::string &result);

		/** @brief Query component.
		 *
		 * The query component, interpreted by the ressource,
		 * separated by a question mark '?'. The query component
		 * is appanded on \a result and stored in m_query.
		 * The query can be an empty string.
		 * m_hasQuery is set, if '#' was found.
		 *
		 @verbatim
		 query =  *( pchar | "/" | "?" )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_query m_hasQuery uric()
		 */
		void query(const unsigned char **str,
		           std::string &result);

		/** @brief Single path segment.
		 *
		 * The segment is appanded on \a result and stored
		 * temporarily in m_segment. The segment can be an empty string.
		 *
		 @verbatim
		 segment = *pchar
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_segment pchar()
		 */
		void segment(const unsigned char **str,
		             std::string &result);

		/** @brief Single path segment not empty.
		 *
		 * The segment is appanded on \a result and stored
		 * temporarily in m_segment. The segment can be an empty string.
		 *
		 @verbatim
		 segment_nz = 1*pchar
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, if a segment has been found.
		 * @see m_segment pchar()
		 */
		bool segment_nz(const unsigned char **str,
		                std::string &result);

		/** @brief Single path segment not empty, without any colon ':'.
		 *
		 * The segment is appanded on \a result and stored
		 * temporarily in m_segment. The segment can be an empty string.
		 *
		 @verbatim
		 segment_nz_nc = 1*( unreserved | pct_encoded | sub_delims | "@" )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, if a segment has been found.
		 * @see m_segment pchar()
		 */
		bool segment_nz_nc(const unsigned char **str,
		                   std::string &result);

		/* @brief Path component (not used)
		 *
		 * If a path has been found, it is stored in m_path, the type of the
		 * path is stored in m_pathType, true is returned.
		 *
		 @verbatim
		 path = path_abempty |
		        path_absolute |
				path_noscheme |
		        path_rootless |
				path_empty
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, path has been found
		 * @see m_path m_pathType path_abempty() path_absolute() path_noscheme() path_rootless path_empty()
		 */
		// bool path(const unsigned char **str,
		//           std::string &result);

		/** @brief Absolute path or empty.
		 *
		 * The absolute path begins with a '/'.
		 * If a path has been found, it is appended to \a result and is stored in m_path,
		 * the type of the path pathtypeEmpty or pathTypeAbsolute is stored in m_pathType,
		 * true is returned.
		 *
		 @verbatim
		 path_abempty = *( "/" segment )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, absolute or empty path has been found.
		 * @see m_path m_pathType path_absolute() path_empty()
		 */
		bool CUri::path_abempty(const unsigned char **str,
								std::string &result);

		/** @brief Absolute path.
		 *
		 * The absolute path begins with a '/'.
		 * If a path has been found, it is appended to \a result and is stored in m_path,
		 * the type of the path pathTypeAbsolute is stored in m_pathType, true is returned.
		 *
		 @verbatim
		 path_absolute = "/" [ segment_nz *( "/" segment ) ]
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, absolute path has been found.
		 * @see m_path m_pathType
		 */
		bool CUri::path_absolute(const unsigned char **str,
								 std::string &result);

		/** @brief Path begins with non-colon ':' segment.
		 *
		 * If a path has been found, it is appended to \a result and is stored in m_path,
		 * the type of the path pathTypeNoScheme is stored in m_pathType, true is returned.
		 *
		 @verbatim
		 path_noscheme = segment_nz_nc *( "/" segment )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, absolute path has been found.
		 * @see m_path m_pathType
		 */
		bool CUri::path_noscheme(const unsigned char **str,
								 std::string &result);

		/** @brief Relative path.
		 *
		 * If a path has been found, it is appended to \a result and is stored in m_path,
		 * the type of the path pathTypeRootless is stored in m_pathType, true is returned.
		 *
		 @verbatim
		 path_rootless = segment_nz *( "/" segment )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, absolute path has been found
		 * @see m_path m_pathType
		 */
		bool CUri::path_rootless(const unsigned char **str,
								 std::string &result);

		/** @brief Empty path.
		 *
		 * If a path has been found, m_path is cleared, the type of the
		 * path pathTypeEmpty is stored in m_pathType, true is returned.
		 *
		 @verbatim
		 path_empty = 0\<pchar\>
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, absolute path has been found
		 * @see m_path m_pathType
		 */
		bool CUri::path_empty(const unsigned char **str,
							  std::string &result);

		/** @brief Port number.
		 *
		 * The port number for the server. Is separated by a colon ':'
		 * from the host. The port number can be omitted (default port assumed).
		 * If found, the number is appended to \a result stored in m_port.
		 *
		 @verbatim
		 port = *digit
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_port digit()
		 */
		void port(const unsigned char **str,
		          std::string &result);

		/** @brief Generic IP address.
		 *
		 * If a generic IP address has been found, it is appended to \a result
		 * and stored in m_ipVFuture, true is returned. Sets m_ipAddrType
		 * to ipAddrTypeVFuture.
		 *
		 @verbatim
		 IPvFuture = "v" 1*hexdig "." 1*( unreserved | sub_delims | ":" )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, generic IP address has been found
		 * @see m_ipVFuture hexdig() unreserved() sub_delims()
		 */
		bool ipVFuture(const unsigned char **str,
		               std::string &result);

		/** @brief IPv6 address.
		 *
		 * If an IPv6 address has been found, it is appended to \a result
		 * and stored in m_ipV6Address, true is returned. Sets m_ipAddrType
		 * to ipAddrTypeV6Address.
		 *
           @verbatim
           IPv6address  =
                                        6( h16 ":" ) ls32 |
                                   "::" 5( h16 ":" ) ls32 |
             [               h16 ] "::" 4( h16 ":" ) ls32 |
             [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 |
             [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 |
             [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32 |
             [ *4( h16 ":" ) h16 ] "::"              ls32 |
             [ *5( h16 ":" ) h16 ] "::"              h16  |
             [ *6( h16 ":" ) h16 ] "::"
		   @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, IPv6 address has been found
		 * @see m_ipV6Address h16()
		 */
		bool ipV6address(const unsigned char **str,
		                  std::string &result);

		/** @brief IP literal.
		 *
		 * The IP literal is a IPv6 or a generic IP address.
		 * If an IP literal has been found, it is appended to \a result
		 * and stored in m_ip_literal, true is returned.
		 *
		 @verbatim
		 IP_literal = "[" ( IPv6address | IPvFuture ) "]"
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, IP literal has been found.
		 * @see m_ip_literal ipV6Address() ipVFuture()
		 */
		bool ip_literal(const unsigned char **str,
		                std::string &result);

		/** @brief IPv4 address.
		 *
		 * The IPv4 address are for numbers separated by a dot each.
		 * If an IPv4 address has been found, it is appended to \a result
		 * and stored in m_ipV4Address, true is returned. Sets m_ipAddrType
		 * to ipAddrTypeV4Address.
		 *
		 @verbatim
		 ipV4address = 1*digit "." 1*digit "." 1*digit "." 1*digit
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, IPv4 address has been found
		 * @see m_ipV4Address digit()
		 */
		bool ipV4address(const unsigned char **str,
		                 std::string &result);

		// Not used
		/* @brief Least significant 32 bits (or IP 4 address) of an IPv6 address.
		 *
		 * If found, it it is appended to \a result
		 * and stored in m_ls32, true is returned.
		 *
		 @verbatim
		 ls32 = ( h16 ":" h16 ) | IPv4address
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, least significant part of a IPv6 address has been found.
		 * @see m_ls32 h16() ipV4Address()
		 */
		// bool ls32(const unsigned char **str,
		//           std::string &result);

		/** @brief Host.
		 *
		 * A host is represented by a hostname or an IP address.
		 * If a host has been found, it is appended to \a result and stored
		 * in m_host, true is returned.
		 *
		 @verbatim
		 host = IP_literal | IPv4address |  reg_name
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, host has been found.
		 * @see m_host hostname() ipV4Address()
		 */
		void host(const unsigned char **str,
		          std::string &result);

		/** @brief Userinfo for an authority.
		 *
		 * The userinfo is followed by an at-sign "@".
		 * If found, it is appended to \a result and stored in m_userinfo.
		 * The at-sign is not stored in m_userinfo. The string can be empty.
		 *
		 @verbatim
		 userinfo = *( unreserved | pct_encoded | sub_delims | ":" )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_userinfo unreserved() pct_encoded()
		 */
		void userinfo(const unsigned char **str,
		              std::string &result);

		/** @brief Registry-based naming authority.
		 *
		 * The structure is specific to the URI scheme, e.g. mailto.
		 * It is appended to \a result and stored in m_reg_name.
		 *
		 @verbatim
		 reg_name = *( unreserved | pct_encoded | sub_delims )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true, reg_name has been found
		 * @see m_reg_name unreserved() pct_encoded()
		 */
		void reg_name(const unsigned char **str,
		              std::string &result);

		/** @brief Authority component.
		 *
		 * Top hierarchical element for naming authority. 
		 * If found, it is appended to \a result and stored in m_authority.
		 * The string can be empty (because host can be empty). m_hasUserinfo
		 * is set if a m_hasUserinfo (maybe empty) has been found. Also
		 * m_hasPort is set if a port has been found (port is not empty then).
		 *
		 @verbatim
		 authority = [ userinfo "@" ] host [ ':' port ]
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @see m_authority m_hasServer server() reg_name()
		 */
		void authority(const unsigned char **str,
		               std::string &result);

		/** @brief Scheme component.
		 *
		 * Scheme to identify ressources, part of an absolute URI. 
		 * If found, it is appended to \a result and stored in m_scheme,
		 * true is returned.
		 *
		 @verbatim
		 scheme = alpha *( alpha | digit | "+" | "-" | "." )
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true if a scheme has been found
		 * @see m_scheme alpha() digit()
		 */
		bool scheme(const unsigned char **str,
		            std::string &result);

		/** @brief Hierarchical part.
		 *
		 * If found, it is appended to \a result, stored in 
		 * m_hier_part and true is returned. m_hasQuery is set
		 * if an query component has been found.
		 *
		 @verbatim
		 hier_part = "//" authority path_abempty |
		             path_absolute |
					 path_rootless |
					 path_empty
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true if a hier_part has been found
		 * @see m_hier_part m_hasQuery net_path() abs_path() query()
		 */
		bool hier_part(const unsigned char **str,
		               std::string &result);

		/** @brief Relative URI reference.
		 *
		 * A URI reference relative to an absolute base URI.
		 *
		 * If found, it is appended to \a result, stored in
		 * m_relative_uri and true is returned. m_hasQuery is set
		 * if an query component has been found.
		 *
		 @verbatim
		 relative_uri = hier_part [ "?" query ] 
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true if a relative_uri has been found
		 * @see m_relative_uri m_hasQuery net_path() abs_path() rel_path() query()
		 */
		bool relative_uri(const unsigned char **str,
		                 std::string &result);

		/** @brief Absolute URI.
		 *
		 * An absoulte URI with a scheme.
		 *
		 * If found, it is appended to \a result, stored in m_absolute_uri.
		 * m_hasScheme is set and true is returned.
		 *
		 @verbatim
		 absolute_uri = scheme ":" hier_part [ "?" query ]
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @retval result String to store the characters matched.
		 * @return true if an absolute_uri has been found
		 * @see m_absolute_uri m_hasScheme scheme() hier_part() opaque_part()
		 */
		bool absolute_uri(const unsigned char **str,
		                 std::string &result);

		/** @brief URI reference.
		 *
		 * Tests and parses the generic syntax of a URI reference.
		 *
		 * If found, it is appended to \a result and stored in m_uri_reference.
		 * If a (possibly empty) fragment has been fount, it
		 * is stored in m_fragment and m_hasFrament is set. An empty URI reference
		 * (absolute or relative URI) references the "current document".
		 * Returns true if there are no more trailing characters after parsing.
		 *
		 @verbatim
		 URI-reference = [ absolute_uri | relative_uri ] ["#" fragment]
		 @endverbatim
		 *
		 * @param str Address of a character pointer to the
		 * input string (address of input pointer).
		 * @return true if there are no more trailing characters after parsing .
		 * @see m_uri_reference m_fragment m_hasfragment absolute_uri() relative_uri() fragment()
		 */
		bool uri_reference(const unsigned char **str);

		/** @brief Adds path segment to the segment list.
		 *
		 * Adds a pathsegment to the segment list, handles the special meaning
		 * of "." (same hierarchy level) and ".." (hierarchy level above) segments.
		 *
		 * @param seg Segment to add
		 * @retval segList List of segments reflecting the hierarchy levels
		 */
		void addSegment(const std::string &seg,
			std::list<std::string> &segList) const;

	public:
		/** @brief Constructor, parses URI.
		 *
		 * @param anUri Char pointer to a URI
		 * @see parse() isValid()
		 */
		inline CUri(const char *anUri = 0)
		{
			parse(anUri);
		}

		/** @brief Constructor, parses URI.
		 *
		 * @param anUri String containing a URI
		 * @see parse() isValid()
		 */
		inline CUri(const std::string &anUri)
		{
			parse(anUri.c_str());
		}

		/** @brief Constructor, creates Uri from base and relative URI.
		 *
		 * @param baseUri Absolute base URI
		 * @param relative_uri Relative URI
		 * @see CUri::operator=()
		 */
		inline CUri(const CUri &baseUri, const CUri &relative_uri)
		{
			*this = relative_uri;
			makeAbsolute(baseUri);
		}

		/** @brief Copy constructor.
		 *
		 * @param uri URI to assign to this instance.
		 * @see CUri::operator=()
		 */
		inline CUri(const CUri &uri)
		{
			*this = uri;
		}

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
		 * Converting a relative URI to the absolute
		 * form using the absolute base URI \a baseURI.
		 *
		 * @param baseUri CUri instance containig the absolute base URI
		 * @return true, absolute form could be resolved.
		 * @see parse() isValid()
		 */
		inline bool makeAbsolute(const CUri &baseUri)
		{
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
		 * @retval resultUriStr The absolute form is returned as string representation in this variable
		 * @return true, absolute form could be resolved.
		 * @see parse() isValid()
		 */
		bool makeAbsolute(const CUri &baseUri,
		                  std::string &resultUriStr) const;

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
		inline bool makeAbsolute(const CUri &baseUri,
		                         CUri &resultUri)
		{
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
		inline bool isValid() const
		{
			return m_valid;
		}

		/** @brief Converts a CUri to a string.
		 *
		 * @return The URI reference
		 */
		inline const std::string &toString() const
		{
			return m_uri_reference;
		}

		/** @brief Tests if URI reference is absolute.
		 * @return true, URI is absolute
		 */
		inline bool isAbsoluteUri() const
		{
			return !m_absolute_uri.empty();
		}

		/** @brief Gets the absolute URI.
		 * @return Absolute URI.
		 */
		inline const std::string &getAbsoluteUri() const
		{
			return m_absolute_uri;
		}

		/** @brief Tests if URI reference is relative.
		 * @return true, URI is relative.
		 */
		inline bool isRelativeUri() const
		{
			return !m_relative_uri.empty();
		}

		/** @brief Gets the relative URI.
		 * @return Relative URI.
		 */
		inline const std::string &getRelativeUri() const
		{
			return m_relative_uri;
		}

		/** @brief Tests, if URI represents the "current document".
		 *
		 * A URI represents the current document if absolute and relative parts are empty.
		 *
		 * @return true, if URI represents the "current document".
		 */
		inline bool isCurrentDocument() const
		{
			return m_path.empty() && !m_hasScheme && !m_hasAuthority;
		}

		/** @brief Tests, if URI has defined a scheme component (is an absolute URI).
		 * @return true, URI has a scheme component.
		 */
		inline bool hasScheme() const
		{
			return m_hasScheme;
		}

		/** @brief Gets the scheme component.
		 * @return The scheme component.
		 */
		inline const std::string &getScheme() const
		{
			return m_scheme;
		}

		/** @brief Gets the hierarchy part.
		 * @return The hierarchy part.
		 */
		inline const std::string &getHierPart() const
		{
			return m_hier_part;
		}

		/** @brief Gets the path component.
		 * 
		 * The path component is either an absolute path, relative path or
		 * the opaque part.
		 *
		 * @return The path component.
		 */
		inline const std::string &getPath() const
		{
			return m_path;
		}

		/** @brief Tests, if URI has defined a query component.
		 * @return true, if path component was relative.
		 */
		inline bool hasQuery() const
		{
			return m_hasQuery;
		}

		/** @brief Gets the query component.
		 * @return Query component.
		 */
		inline const std::string &getQuery() const
		{
			return m_query;
		}

		/** @brief Tests, if URI has defined a fragment component.
		 * @return true, if fragment component was relative.
		 */
		inline bool hasFragment() const
		{
			return m_hasFragment;
		}

		/** @brief Gets the fragment component.
		 * @return Fragment component.
		 */
		inline const std::string &getFragment() const
		{
			return m_fragment;
		}

		/** @brief Tests, if URI has defined a naming authority of a network path.
		 * @return true, if authority is defined.
		 */
		inline bool hasAuthority() const
		{
			return m_hasAuthority;
		}

		/** @brief Gets the naming authority of a network path.
		 * @return Naming authority part of a network path.
		 */
		inline const std::string &getAuthority() const
		{
			return m_authority;
		}

		/** @brief Tests, if URI has defined the userinfo of an authority.
		 * @return true, if userinfo of an authority is defined.
		 */
		inline bool hasUserinfo() const
		{
			return m_hasUserinfo;
		}

		/** @brief Gets the userinfo of an authority.
		 * @return userinfo.
		 */
		inline const std::string &getUserinfo() const
		{
			return m_userinfo;
		}

		/** @brief Gets the registry based naming authority.
		 * @return Registry based naming authority.
		 */
		inline const std::string &getRegName() const
		{
			return m_reg_name;
		}

		/** @brief Gets the IPv4 address if present.
		 * @return IPv4 address.
		 */
		inline const std::string &getIPv4Address() const
		{
			return m_ipV4Address;
		}

		/** @brief Gets the IP literal if present.
		 * @return IP literal (IPv6 or IPvFuture).
		 */
		inline const std::string &getIPLiteral() const
		{
			return m_ip_literal;
		}

		/** @brief Gets the IPv6 address if present.
		 * @return IPv6 address.
		 */
		inline const std::string &getIPv6Address() const
		{
			return m_ipV6Address;
		}

		/** @brief Gets the generic IP address if present.
		 * @return IPv6 address.
		 */
		inline const std::string &getIPvFuture() const
		{
			return m_ipVFuture;
		}

		/** @brief Tests, if a host is present.
		 * @return true, if a host is present.
		 */
		inline bool hasHost() const
		{
			return !m_host.empty();
		}

		/** @brief Gets the host if present of a network address
		 * @return host.
		 */
		inline const std::string &getHost() const
		{
			return m_host;
		}

		/** @brief Tests, if host has a port number.
		 *
		 * The portnumber can be empty, then the default port is used.
		 *
		 * @return true, if hostname has a port number.
		 */
		inline bool hasPort() const
		{
			return m_hasPort;
		}

		/** @brief Gets the (possibly empty) port number.
		 * @return Port number.
		 */
		inline const std::string &getPort() const
		{
			return m_port;
		}

		/** @brief Gets the first of the path segments.
		 *
		 * Gets the iterator to the first of the path segments (not including the
		 * relative segment). If the path endend with a '/', the last segment
		 * has an empty name.
		 *
		 * @return The constant iterator for the path segmnents
		 * @see getRelSegments() getPathSegments() segmentsEnd()
		 */
		inline segments_const_iterator segmentsBegin() const
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
		inline segments_const_iterator segmentsEnd() const
		{
			return m_segments.end();
		}

		/** @brief Gets the numer of the path segments
		 *
		 * The relative segment of a relative path is not counted.
		 *
		 * @return Number of path segments.
		 */
		inline segments_size_type segmentsSize() const
		{
			return m_segments.size();
		}

		/** @brief Gets the type of the path component of the URI.
		 * @return Type of the path component of the URI.
		 * @see path()
		 */
		inline CUri::EnumPathType pathType() const
		{
			return m_pathType;
		}

		/** @brief Gets the type of the host component of the URI.
		 * @return Type of the host component of the URI.
		 * @see host()
		 */
		inline CUri::EnumIPAddrType ipAddrType() const
		{
			return m_ipAddrType;
		}
	};
}

#endif // // _RICPP_STREAMS_URI_H
