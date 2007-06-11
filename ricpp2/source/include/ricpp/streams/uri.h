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

#include <string>
#include <cassert>

namespace RiCPP {
	/** @brief Class to represent a generic URI
	 *
	 * Filepathes are converted to a relative URI, that
	 * can be considered as file: URI in the context of
	 * RiCPP.
	 *
	 * Schemes that do not match the generic format as
	 * described in rfc2396 are not suported.
	 *
	 * @see http://www.ietf.org/rfc/rfc2396.txt
	 */
	class CUri {
		bool m_valid;

		std::string m_URI_reference;
		std::string m_absoluteURI;
		std::string m_relativeURI;
		std::string m_domainlabel;
		std::string m_toplabel;
		std::string m_IPv4address;
		std::string m_port;
		std::string m_opaque_part;
		std::string m_hier_part;
		std::string m_path;
		std::string m_net_path;
		std::string m_abs_path;
		std::string m_rel_path;
		std::string m_path_segments;
		std::string m_segment;
		std::string m_param;
		std::string m_query;
		std::string m_fragment;
		std::string m_host;
		std::string m_hostname;
		std::string m_hostport;
		std::string m_userinfo;
		std::string m_server;
		std::string m_reg_name;
		std::string m_authority;
		std::string m_scheme;
		std::string m_rel_segment;
	
		inline void clearAll() {
			m_valid = true;
			m_URI_reference.clear();
			m_absoluteURI.clear();
			m_relativeURI.clear();
			m_domainlabel.clear();
			m_toplabel.clear();
			m_IPv4address.clear();
			m_port.clear();
			m_opaque_part.clear();
			m_hier_part.clear();
			m_path.clear();
			m_net_path.clear();
			m_abs_path.clear();
			m_rel_path.clear();
			m_path_segments.clear();
			m_segment.clear();
			m_param.clear();
			m_query.clear();
			m_fragment.clear();
			m_host.clear();
			m_hostname.clear();
			m_hostport.clear();
			m_userinfo.clear();
			m_server.clear();
			m_reg_name.clear();
			m_authority.clear();
			m_scheme.clear();
			m_rel_segment.clear();
		}

		inline void advance(const unsigned char **str, std::string &result, int steps=1)
		{
			while ( steps-- ) {
				result += ((*str)++)[0];
			}
		}

		inline unsigned char digit(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( c>='0' && c<='9' ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		inline unsigned char upalpha(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( c>='A' && c<='Z' ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		inline unsigned char lowalpha(const unsigned char **str, std::string &result)
		{
			unsigned char c = (*str)[0];
			if ( c>='a' && c<='z' ) {
				advance(str, result);
				return c;
			}
			return 0;
		}

		inline unsigned char alpha(const unsigned char **str, std::string &result)
		{
			return lowalpha(str, result) || upalpha(str, result);
		}

		inline unsigned char alphanum(const unsigned char **str, std::string &result)
		{
			return alpha(str, result) || digit(str, result);
		}

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

		inline bool escaped(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			if ( (*str)[0] != '%' ) {
				return false;
			}
			advance(str, result);

			if ( !hex(str, result) ) {
				*str = sav;
				return false;
			}
			advance(str, result);

			if ( !hex(str, result) ) {
				*str = sav;
				return false;
			}
			advance(str, result);

			return true;
		}

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

		inline unsigned char unreserved(const unsigned char **str, std::string &result)
		{
			return alphanum(str, result) || mark(str, result);
		}

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

		inline bool uric(const unsigned char **str, std::string &result)
		{
			return reserved(str, result) != 0 ||
				unreserved(str, result) != 0 ||
				escaped(str, result);
		}

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

		inline bool fragment(const unsigned char **str, std::string &result)
		{
			m_fragment = "";
			while ( uric(str, m_fragment) );
			result += m_fragment;
			return true;
		}

		inline bool query(const unsigned char **str, std::string &result)
		{
			m_query = "";
			while ( uric(str, m_query) );
			result += m_query;
			return true;
		}

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

		inline bool param(const unsigned char **str, std::string &result)
		{
			m_param = "";
			while ( pchar(str, m_param) );
			result += m_param;
			return true;
		}

		inline bool segment(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_segment = "";

			while ( pchar(str, m_segment) );

			while ( *str[0] == ';' ) {
				advance(str, m_segment);
				if ( !param(str, m_segment) ) {
					m_segment = "";
					*str = sav;
					return false;
				}
			}
			result += m_segment;
			return true;
		}

		inline bool path_segments(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_path_segments = "";

			if ( !segment(str, m_path_segments) ) {
				return false;
			}

			while ( *str[0] == '/' ) {
				advance(str, m_path_segments);
				if ( !segment(str, m_path_segments) ) {
					m_path_segments = "";
					*str = sav;
					return false;
				}
			}
			result += m_path_segments;
			return true;
		}

		inline bool abs_path(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_abs_path = "";
			if ( (*str)[0] != '/' )
				return false;

			advance(str, m_abs_path);
			if ( !path_segments(str, m_abs_path) ) {
				m_abs_path = "";
				*str = sav;
				return false;
			}
			result += m_abs_path;
			return true;
		}

		inline bool opaque_part(const unsigned char **str, std::string &result)
		{
			m_opaque_part = "";
			if ( uric_no_slash(str, m_opaque_part) ) {
				while ( uric(str, m_opaque_part) );
				result += m_opaque_part;
				return true;
			}
			return false;
		}

		inline bool path(const unsigned char **str, std::string &result)
		{
			m_path = "";
			if ( opaque_part(str, m_path) || abs_path(str, m_path) ) {
				result += m_path;
			}
			return true;
		}

		inline bool port(const unsigned char **str, std::string &result)
		{
			m_port = "";
			while ( digit(str, m_port) );
			result += m_port;
			return true;
		}

		inline bool IPv4address(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_IPv4address = "";
			int i;
			for ( i=0; i<4; ++i ) {
				if ( i > 0 ) {
					if ( *str[0] != '.' ) {
						m_IPv4address = "";
						*str = sav;
						return false;
					}
					advance(str, m_IPv4address);
				}
				if ( digit(str, m_IPv4address) ) {
					while ( digit(str, m_IPv4address) );
				} else {
					m_IPv4address = "";
					*str = sav;
					return false;
				}
			}
			result += m_IPv4address;
			return true;
		}

		inline bool toplabel(const unsigned char **str, std::string &result)
		{
			m_toplabel = "";
			unsigned char c1;

			if ( !(c1 = alpha(str, m_toplabel)) )
				return false;

			unsigned char c;
			do {
				c = alphanum(str, m_toplabel);
				if ( !c ) {
					c = (*str)[0];
					if ( c != '-' )
						break;
					else
						advance(str, m_toplabel);
				} 
			} while ( true );

			assert(m_toplabel[0] != '-');
			while ( m_toplabel[m_toplabel.size()-1] == '-' ) {
				(*str)--;
				m_toplabel.resize(m_toplabel.size()-1);
			}
			result += m_toplabel;
			return true;
		}

		inline bool domainlabel(const unsigned char **str, std::string &result)
		{
			m_domainlabel = "";
			unsigned char c1;

			if ( !(c1 = alphanum(str, m_domainlabel)) )
				return false;

			unsigned char c;
			do {
				c = alphanum(str, m_domainlabel);
				if ( !c ) {
					c = (*str)[0];
					if ( c != '-' )
						break;
					else
						advance(str, m_domainlabel);
				} 
			} while ( true );

			assert(m_domainlabel[0] != '-');
			while ( m_domainlabel[m_domainlabel.size()-1] == '-' ) {
				(*str)--;
				m_domainlabel.resize(m_domainlabel.size()-1);
			}
			result += m_domainlabel;
			return true;
		}

		inline bool hostname(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;

			m_hostname = "";
			m_toplabel = "";
			m_domainlabel = "";

			while ( domainlabel(str, m_hostname) ) {
				m_toplabel = m_domainlabel;
				if ( (*str)[0] != '.' ) {
					if ( isdigit(m_toplabel[0]) ) {
						m_hostname = "";
						m_toplabel = "";
						m_domainlabel = "";
						*str = sav;
						return false;
					}
					result += m_hostname;
					return true;
				}
				advance(str, m_hostname);
			}
			if ( m_toplabel.empty() || isdigit(m_toplabel[0]) ) {
				m_hostname = "";
				m_toplabel = "";
				m_domainlabel = "";
				*str = sav;
				return false;
			}
			result += m_hostname;
			return true;
		}

		inline bool host(const unsigned char **str, std::string &result)
		{
			m_host = "";
			if ( hostname(str, m_host) ||
				IPv4address(str, m_host) )
			{
				result += m_host;
				return true;
			}
			return false;
		}

		inline bool hostport(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_hostport = "";
			if ( !host(str, m_hostport) ) {
				return false;
			}
			if ( (*str)[0] == ':' ) {
				advance(str, m_hostport);
				if ( !port(str, m_hostport) ) {
					m_hostport = "";
					*str = sav;
					return false;
				}
			}
			result += m_hostport;
			return true;
		}

		inline bool userinfo(const unsigned char **str, std::string &result)
		{
			m_userinfo = "";
			for ( ;; ) {
				if ( unreserved(str, m_userinfo) != 0 ||
					escaped(str, m_userinfo) )
				{
					continue;
				}
				unsigned char c = (*str)[0];
				switch (c) {
					case ';':
					case ':':
					case '&':
					case '=':
					case '+':
					case '$':
					case ',':
						advance(str, m_userinfo);
						continue;
					default:
						break;
				}
				break;
			}
			result += m_userinfo;
			return true;
		}

		inline bool server(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_server = "";
			if ( userinfo(str, m_server) ) {
				if ( (*str)[0] != '@' ) {
					m_userinfo = "";
					m_server = "";
					*str = sav;
					// continue with hostport without userinfo
				} else {
					advance(str, m_server);
				}
				if ( !hostport(str, m_server) ) {
					m_server = "";
					*str = sav;
					return false;
				}
			} else {
				hostport(str, m_server);
			}
			result += m_server;
			return true;
		}

		inline bool reg_name(const unsigned char **str, std::string &result)
		{
			m_reg_name = "";
			for ( ;; ) {
				if ( unreserved(str, m_reg_name) != 0 ||
					escaped(str, m_reg_name) )
				{
					continue;
				}
				unsigned char c = (*str)[0];
				switch (c) {
					case '$':
					case ',':
					case ';':
					case ':':
					case '@':
					case '&':
					case '=':
					case '+':
						advance(str, m_reg_name);
						continue;
					default:
						break;
				}
				break;
			}
			result += m_reg_name;
			return !m_reg_name.empty();
		}

		inline bool authority(const unsigned char **str, std::string &result)
		{
			m_authority = "";
			if ( server(str, m_authority) ||
				reg_name(str, m_authority) )
			{
				result += m_authority;
				return true;
			}
			return false;
		}

		inline bool scheme(const unsigned char **str, std::string &result)
		{
			m_scheme = "";
			if ( alpha(str, m_scheme) ) {
				for ( ;; ) {
					if ( alphanum(str, m_scheme) )
						continue;
					unsigned char c = (*str)[0];
					switch (c) {
						case '+':
						case '-':
						case '.':
							advance(str, m_scheme);
							continue;
						default:
							break;
					}
					break;
				}
				result += m_scheme;
				return true;
			}
			return false;
		}

		inline bool rel_segment(const unsigned char **str, std::string &result)
		{
			m_rel_segment = "";
			for ( ;; ) {
				if ( unreserved(str, m_rel_segment) != 0 ||
					escaped(str, m_rel_segment) )
				{
					continue;
				}
				unsigned char c = (*str)[0];
				switch (c) {
					case ';':
					case '@':
					case '&':
					case '=':
					case '+':
					case '$':
					case ',':
						advance(str, m_rel_segment);
						continue;
					default:
						break;
				}
				break;
			}
			result += m_rel_segment;
			return !m_rel_segment.empty();
		}

		inline bool rel_path(const unsigned char **str, std::string &result)
		{
			m_rel_path = "";
			if ( rel_segment(str, m_rel_path) ) {
				abs_path(str, m_rel_path);
				result += m_rel_path;
				return true;
			}
			return false;
		}

		inline bool net_path(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_net_path = "";

			if ( (*str)[0] != '/' ) {
				return false;
			}
			advance(str, m_net_path);

			if ( (*str)[0] != '/' ) {
				m_net_path = "";
				*str = sav;
				return false;
			}
			advance(str, m_net_path);

			if ( !authority(str, m_net_path) ) {
				m_net_path = "";
				*str = sav;
				return false;
			}

			abs_path(str, m_net_path);

			result += m_net_path;
			return true;
		}

		inline bool hier_part(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_hier_part = "";
			if ( net_path(str, m_hier_part) ||
				abs_path(str, m_hier_part) )
			{
				if ( (*str)[0] == '?' ) {
					advance(str, m_hier_part);
					if ( !query(str, m_hier_part) ) {
						*str = sav;
						return false;
					}
				}
				result += m_hier_part;
				return true;
			}
			return false;
		}

		inline bool relativeURI(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_relativeURI = "";
			if ( net_path(str, m_relativeURI) ||
				abs_path(str, m_relativeURI) ||
				rel_path(str, m_relativeURI) )
			{
				if ( (*str)[0] == '?' ) {
					advance(str, m_relativeURI);
					if ( !query(str, m_relativeURI) ) {
						m_relativeURI = "";
						*str = sav;
						return false;
					}
				}
				result += m_relativeURI;
				return true;
			}
			return false;
		}

		inline bool absoluteURI(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_absoluteURI = "";
			if ( !scheme(str, m_absoluteURI) ) {
				return false;
			}

			if ( (*str)[0] != ':' ) {
				m_absoluteURI = "";
				*str = sav;
				return false;
			}
			advance(str, m_absoluteURI);

			if ( hier_part(str, m_absoluteURI) ||
				opaque_part(str, m_absoluteURI) )
			{
				result += m_absoluteURI;
				return true;
			}

			m_absoluteURI = "";
			*str = sav;
			return false;
		}

		inline bool URI_reference(const unsigned char **str)
		{
			const unsigned char *sav = *str;
			m_URI_reference = "";
			if ( !absoluteURI(str, m_URI_reference) )
				relativeURI(str, m_URI_reference);
			if ( (*str)[0] == '#' ) {
				advance(str, m_URI_reference);
				if ( !fragment(str, m_URI_reference) ) {
					m_URI_reference = "";
					*str = sav;
					return false;
				}
			}
			return *str[0] == 0;
		}

	public:
		inline CUri(const char *anUri) { parse(anUri); }
		inline bool parse(const char *anUri)
		{
			clearAll();
			if ( !anUri )
				return m_valid;
			m_valid = URI_reference((const unsigned char **)&anUri);
			return m_valid;
		}
		inline bool isValid() const { return m_valid; }
	};
}

#endif // // _RICPP_STREAMS_URI_H
