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
	public:
		class CSegment {
			friend class CUri;
			std::string m_name;
			std::list<std::string> m_parameters;
			void clear() {
				m_name.clear();
				m_parameters.clear();
			}
		public:
			inline const char *getName() const { return m_name.c_str(); }
			inline std::list<std::string>::const_iterator parametersBegin() const
			{
				return m_parameters.begin();
			}
			inline std::list<std::string>::const_iterator parametersEnd() const
			{
				return m_parameters.end();
			}
			inline std::list<std::string>::size_type parametersSize() const
			{
				return m_parameters.size();
			}
		};
	private:
		bool m_valid;

		std::string m_path;
		std::string m_escaped;

		std::string m_URI_reference;

		std::string m_absoluteURI;
		std::string m_relativeURI;

		bool m_hasScheme;
		std::string m_scheme;

		std::string m_opaque_part;
		std::string m_hier_part;

		std::string m_net_path;
		std::string m_abs_path;
		std::string m_rel_path;

		bool m_hasQuery;
		std::string m_query;

		bool m_hasFragment;
		std::string m_fragment;

		bool m_hasAuthority;
		std::string m_authority;

		bool m_hasUserinfo;
		std::string m_userinfo;

		bool m_hasServer;
		std::string m_server;
		std::string m_reg_name;

		std::string m_host;
		std::string m_hostport;
		std::string m_IPv4address;

		bool m_hasTrailingDot;
		std::string m_hostname;

		bool m_hasPort;
		std::string m_port;
	
		std::list<std::string> m_domainlabels;
		std::string m_domainlabel;
		std::string m_toplabel;
		
		std::string m_rel_segment;
		std::string m_path_segments;
		std::string m_segment;
		std::string m_param;

		CSegment m_segmentContainer;
		std::list<CSegment> m_segments;

		inline void clearAll() {
			m_valid = true;

			m_path.clear();
			m_escaped.clear();

			m_URI_reference.clear();

			m_absoluteURI.clear();
			m_relativeURI.clear();

			m_hasScheme = false;
			m_scheme.clear();

			m_opaque_part.clear();
			m_hier_part.clear();

			m_net_path.clear();
			m_abs_path.clear();
			m_rel_path.clear();

			m_hasQuery = false;
			m_query.clear();

			m_hasFragment = false;
			m_fragment.clear();

			m_hasAuthority = false;
			m_authority.clear();

			m_hasUserinfo = false;
			m_userinfo.clear();

			m_hasServer = false;
			m_server.clear();
			m_reg_name.clear();

			m_host.clear();
			m_hostport.clear();
			m_IPv4address.clear();

			m_hasTrailingDot = false;
			m_hostname.clear();

			m_hasPort = false;
			m_port.clear();

			m_domainlabels.clear();
			m_domainlabel.clear();
			m_toplabel.clear();
			
			m_rel_segment.clear();
			m_path_segments.clear();
			m_segment.clear();
			m_param.clear();

			m_segmentContainer.clear();
			m_segments.clear();
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

		inline void fragment(const unsigned char **str, std::string &result)
		{
			m_fragment = "";
			while ( uric(str, m_fragment) );
			result += m_fragment;
		}

		inline void query(const unsigned char **str, std::string &result)
		{
			m_query = "";
			while ( uric(str, m_query) );
			result += m_query;
		}

		inline void param(const unsigned char **str, std::string &result)
		{
			m_param = "";
			while ( pchar(str, m_param) );
			result += m_param;
		}

		inline void segment(const unsigned char **str, std::string &result)
		{
			m_segmentContainer.clear();
			m_segment = "";

			while ( pchar(str, m_segment) );

			m_segmentContainer.m_name = m_segment;

			while ( *str[0] == ';' ) {
				advance(str, m_segment);
				param(str, m_segment);
				// m_param can be empty
				m_segmentContainer.m_parameters.push_back(m_param);
			}
			result += m_segment;
		}

		inline void path_segments(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;

			m_path_segments = "";
			m_segments.clear();

			segment(str, m_path_segments);
			// m_segmentContainer can be empty
			m_segments.push_back(m_segmentContainer);

			while ( *str[0] == '/' ) {
				advance(str, m_path_segments);
				segment(str, m_path_segments);
				// m_segmentContainer can be empty
				m_segments.push_back(m_segmentContainer);
			}
			result += m_path_segments;
		}

		inline bool abs_path(const unsigned char **str, std::string &result)
		{
			m_abs_path = "";
			m_path = "";
			
			if ( (*str)[0] != '/' )
				return false;

			advance(str, m_abs_path);
			path_segments(str, m_abs_path);
			// m_abs_path can be "/"
			m_path = m_abs_path;
			result += m_abs_path;
			return true;
		}

		inline bool opaque_part(const unsigned char **str, std::string &result)
		{
			m_opaque_part = "";
			m_path = "";
			if ( uric_no_slash(str, m_opaque_part) ) {
				while ( uric(str, m_opaque_part) );
				m_path = m_opaque_part;
				result += m_opaque_part;
				return true;
			}
			return false;
		}

		/*
		// Never called path is either the opaque part or the abs path,
		// m_path is set in opaque_part() or abs_path()
		// The path component m_path can also be a net_path() or rel_path()
		inline bool path(const unsigned char **str, std::string &result)
		{
			m_path = "";
			if ( opaque_part(str, m_path) || abs_path(str, m_path) ) {
				result += m_path;
			}
			return true;
		}
		*/

		inline void port(const unsigned char **str, std::string &result)
		{
			m_port = "";
			while ( digit(str, m_port) );
			result += m_port;
		}

		inline bool IPv4address(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_IPv4address = "";

			for ( int i=0; i<4; ++i ) {
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
					// At least one digit
					m_IPv4address = "";
					*str = sav;
					return false;
				}
			}
			result += m_IPv4address;
			return true;
		}

		/*
		inline bool toplabel(const unsigned char **str, std::string &result)
		{
			// This is never called, because a toplabel is recognized
			// as a domainlabel in hostname()
			unsigned char c1;
			m_toplabel = "";

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
		*/

		inline bool domainlabel(const unsigned char **str, std::string &result)
		{
			unsigned char c1;
			m_domainlabel = "";

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
			// Special handling: Last domainlabel has to be a toplabel
			const unsigned char *sav = *str;

			m_hasTrailingDot = false;
			m_hostname = "";
			m_toplabel = "";
			m_domainlabels.clear();

			while ( domainlabel(str, m_hostname) ) {
				// rember to found domainlabel in toplabel
				// is not neccessairily a valid toplabel here
				m_toplabel = m_domainlabel;
				
				// look ahead (*str)[0]
				if ( (*str)[0] != '.' ) {
					// Last domainlabel has to be a toplabel
					if ( isdigit(m_toplabel[0]) ) {
						m_hostname = "";
						m_toplabel = "";
						m_domainlabel = "";
						m_domainlabels.clear();
						*str = sav;
						return false;
					}
					result += m_hostname;
					return true;
				}
				m_domainlabels.push_back(m_domainlabel);
				advance(str, m_hostname);
			}
			if ( !m_domainlabels.empty() )
				m_domainlabels.pop_back();

			// Last domainlabel has to be a toplabel
			if ( m_toplabel.empty() || isdigit(m_toplabel[0]) ) {
				m_hostname = "";
				m_toplabel = "";
				m_domainlabel = "";
				m_domainlabels.clear();
				*str = sav;
				return false;
			}
			m_hasTrailingDot = !m_hostname.empty() && m_hostname[m_hostname.size()-1] == '.';
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
			m_hasPort = false;

			if ( !host(str, m_hostport) ) {
				return false;
			}
			if ( (*str)[0] == ':' ) {
				advance(str, m_hostport);
				m_hasPort = true;
				port(str, m_hostport);
			}
			result += m_hostport;
			return true;
		}

		inline void userinfo(const unsigned char **str, std::string &result)
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
		}

		inline void server(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_server = "";
			m_hasUserinfo = false;
			userinfo(str, m_server);
			if ( (*str)[0] != '@' ) {
				m_userinfo = "";
				m_server = "";
				*str = sav;
				// continue with hostport without userinfo
			} else {
				advance(str, m_server);
				m_hasUserinfo = true;
			}
			hostport(str, m_server);
			result += m_server;
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
			// At least one legal character
			return !m_reg_name.empty();
		}

		inline void authority(const unsigned char **str, std::string &result)
		{
			m_authority = "";
			m_hasServer = true;
			
			server(str, m_authority);
			if ( m_server.empty() )
			{
				if ( reg_name(str, m_authority) ) {
					m_hasServer = false;
				}
			}
			result += m_authority;
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
			// At least one legal character
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
			// At least one legal character
			return !m_rel_segment.empty();
		}

		inline bool rel_path(const unsigned char **str, std::string &result)
		{
			m_rel_path = "";
			m_path = "";

			if ( rel_segment(str, m_rel_path) ) {
				abs_path(str, m_rel_path);
				m_path = m_rel_path;
				result += m_rel_path;
				return true;
			}
			return false;
		}

		inline bool net_path(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_net_path = "";
			m_path = "";
			m_hasAuthority = false;

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
			m_hasAuthority = true;

			authority(str, m_net_path);

			abs_path(str, m_net_path);

			m_path = m_net_path;
			result += m_net_path;
			return true;
		}

		inline bool hier_part(const unsigned char **str, std::string &result)
		{
			const unsigned char *sav = *str;
			m_hier_part = "";
			m_query = "";
			m_hasQuery = false;

			if ( net_path(str, m_hier_part) ||
				abs_path(str, m_hier_part) )
			{
				if ( (*str)[0] == '?' ) {
					advance(str, m_hier_part);
					m_hasQuery = true;
					query(str, m_hier_part);
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
			m_query = "";
			m_hasQuery = false;

			if ( net_path(str, m_relativeURI) ||
				abs_path(str, m_relativeURI) ||
				rel_path(str, m_relativeURI) )
			{
				if ( (*str)[0] == '?' ) {
					advance(str, m_relativeURI);
					m_hasQuery = true;
					query(str, m_relativeURI);
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
			m_hasScheme = false;

			if ( !scheme(str, m_absoluteURI) ) {
				return false;
			}

			if ( (*str)[0] != ':' ) {
				m_absoluteURI = "";
				*str = sav;
				return false;
			}
			m_hasScheme = true;
			sav = *str;

			advance(str, m_absoluteURI);

			if ( hier_part(str, m_absoluteURI) ||
				opaque_part(str, m_absoluteURI) )
			{
				result += m_absoluteURI;
				return true;
			}

			// Grammar must have absolute or relative URI
			// however base URI can consist of the scheme only
			// so return true
			return true;
		}

		inline bool URI_reference(const unsigned char **str)
		{
			const unsigned char *sav = *str;
			m_URI_reference = "";
			m_fragment = "";
			m_hasFragment = false;

			if ( !absoluteURI(str, m_URI_reference) )
				relativeURI(str, m_URI_reference);

			if ( (*str)[0] == '#' ) {
				advance(str, m_URI_reference);
				m_hasFragment = true;
				fragment(str, m_URI_reference);
			}
			// Make shure that there are no more trailing characters
			return *str[0] == 0;
		}

	public:
		inline CUri(const char *anUri = 0) { parse(anUri); }
		inline CUri(const CUri &uri) { *this = uri; }

		inline bool parse(const char *anUri)
		{
			if ( !anUri ) {
				clearAll();
				return m_valid;
			}
			std::string uri(anUri);
			const char *uriptr = uri.c_str();
			m_valid = URI_reference((const unsigned char **)&uriptr);
			return m_valid;
		}

		inline CUri &operator=(const char *anUri)
		{
			parse(anUri);
			return *this;
		}

		inline CUri &operator=(const CUri &uri)
		{
			if ( this == &uri ) {
				return *this;
			}
			parse(uri.getUriReference());
			return *this;
		}

		inline bool makeAbsolute(const CUri &baseUri, std::string &resultUriStr) const
		{
			resultUriStr = "";

			if ( emptyStr(getPath()) &&
				!hasScheme() &&
				!hasAuthority() &&
				!hasQuery() )
			{
				// The URI references the document itself
				return true;
			}

			if ( isAbsolute() ) {
				// The URI is already absolute
				resultUriStr = c_str();
				return true;
			}

			// Check the base URI
			if ( !baseUri.isValid() ) {
				return false;
			}

			// Query is not inherited

			// Fragment is not inherited

			// Inherit the scheme
			bool refHasScheme = true;
			const char *refScheme = noNullStr(getScheme());
			if ( !hasScheme() ) {
				refScheme = noNullStr(baseUri.getScheme());
				refHasScheme = baseUri.hasScheme();
			}

			// Authority
			bool refHasAuthority = true;
			const char *refAuthority = noNullStr(getAuthority());
			if ( !hasAuthority() ) {
				refAuthority = noNullStr(baseUri.getAuthority());
				refHasAuthority = baseUri.hasAuthority();
			}

			// Possibly inherited parts
			if ( refHasScheme )
			{
				resultUriStr += refScheme;
				resultUriStr += ":";
			}

			if ( refHasAuthority )
			{
				resultUriStr += "//";
				resultUriStr += refAuthority;
			}


			// Path
			std::string refPath;
			std::list<CSegment> pathList;

			std::list<CSegment>::const_iterator si = baseUri.segmentsBegin();
			std::list<CSegment>::const_iterator siend = baseUri.segmentsEnd();
			int startpass = 0;

			if ( hasAuthority() || (!emptyStr(getPath()) && m_path[0]=='/') ) {
				startpass = 1;
			}

			for ( int pass = startpass; pass < 2; ++pass ) {
				if ( pass == 1 ) {
					refPath += getRelSegment();
					si = segmentsBegin();
					siend = segmentsEnd();
				}
				for ( ; si != siend; si++ ) {
					if ( !strcmp((*si).getName(), ".") &&
						(*si).parametersSize() == 0 )
					{
						// Ignore "."
						continue;
					}
					if ( !strcmp((*si).getName(), "..") &&
						(*si).parametersSize() == 0 )
					{
						if ( pathList.empty() ) {
							// first segment == ".." considered to be an error
							return false;
						}
						// Remove "<segment>/..", segment cannot be ".."
						// because the ".." are removed from left to right
						pathList.pop_back();
						continue;
					}
					pathList.push_back(*si);
				}
				if ( pass == 0 ) {
					// skip the last segment (all right of the '/' - possibly was an empty segment)
					if ( !pathList.empty() )
						pathList.pop_back();
					bool rootOnly = false;
					if ( pathList.empty() && (notEmptyStr(baseUri.getAbsPath()) || baseUri.hasAuthority() ) ) {
						// The root '/'
						pathList.push_back(CSegment());
						rootOnly = true;
					}
					for ( si = pathList.begin(); si != pathList.end(); si++ ) {
						refPath += "/";
						refPath += (*si).getName();
						std::list<std::string>::const_iterator pi = (*si).parametersBegin();
						for ( ; pi != (*si).parametersEnd(); pi++ ) {
							refPath += ";";
							refPath += (*pi);
						}
					}
					if ( !pathList.empty() && !rootOnly && notEmptyStr(getRelSegment()) ) {
						refPath += "/";
					}
					pathList.clear();
				}
			}

			for ( si = pathList.begin(); si != pathList.end(); si++ ) {
				refPath += "/";
				refPath += (*si).getName();
				std::list<std::string>::const_iterator pi = (*si).parametersBegin();
				for ( ; pi != (*si).parametersEnd(); pi++ ) {
					refPath += ";";
					refPath += (*pi);
				}
			}

			if ( refPath.empty() &&
				notEmptyStr(getAbsPath()) )
			{
				refPath += "/";
			}

			// Mixed parts (only the path)
			resultUriStr += refPath;

			// Not inherited parts
			if ( hasQuery() )
			{
				resultUriStr += "?";
				resultUriStr += noNullStr(getQuery());
			}

			if ( hasFragment() )
			{
				resultUriStr += "#";
				resultUriStr += noNullStr(getFragment());
			}

			return true;
		}

		inline bool isValid() const { return m_valid; }

		inline const char *c_str() const { return m_URI_reference.c_str();}

		inline const char *getUriReference() const { return m_URI_reference.c_str();}

		inline bool isRelative() const { return !m_relativeURI.empty(); }
		inline bool isAbsolute() const { return !m_absoluteURI.empty(); }
		inline const char *getAbsoluteUri() const { return m_absoluteURI.c_str(); }
		inline const char *getRelativeUri() const { return m_relativeURI.c_str(); }

		inline bool hasScheme() const { return m_hasScheme; }
		inline const char *getScheme() const { return m_scheme.c_str(); }

		inline const char *getOpaquePart() const { return m_opaque_part.c_str(); }
		inline const char *getHierPart() const { return m_hier_part.c_str(); }

		inline const char *getPath() const { return m_path.c_str(); }
		inline const char *getNetPath() const { return m_net_path.c_str(); }
		inline const char *getAbsPath() const { return m_abs_path.c_str(); }
		inline const char *getRelPath() const { return m_rel_path.c_str(); }

		inline bool hasQuery() const { return m_hasQuery; }
		inline const char *getQuery() const { return m_query.c_str(); }

		inline bool hasFragment() const { return m_hasFragment; }
		inline const char *getFragment() const { return m_fragment.c_str(); }

		inline bool hasAuthority() const { return m_hasAuthority; }
		inline const char *getAuthority() const { return m_authority.c_str(); }

		inline bool hasUserinfo() const { return m_hasUserinfo; }
		inline const char *getUserinfo() const { return m_userinfo.c_str(); }

		inline bool hasServer() const { return m_hasServer; }
		inline const char *getServer() const { return m_server.c_str(); }
		inline const char *getRegName() const { return m_reg_name.c_str(); }

		inline const char *getHost() const { return m_host.c_str(); }
		inline const char *getHostport() const { return m_hostport.c_str(); }
		inline const char *getIPv4Address() const { return m_IPv4address.c_str(); }

		inline bool hasTrailingDot() const { return m_hasTrailingDot; }
		inline const char *getHostname() const { return m_hostname.c_str(); }

		inline bool hasPort() const { return m_hasPort; }
		inline const char *getPort() const { return m_port.c_str(); }

		inline std::list<std::string>::const_iterator domainlablesBegin() const
		{
			return m_domainlabels.begin();
		}
		inline std::list<std::string>::const_iterator domainlablesEnd() const
		{
			return m_domainlabels.end();
		}
		inline std::list<std::string>::size_type domainlablesSize() const
		{
			return m_domainlabels.size();
		}
		inline const char *getTopLabel() const { return m_toplabel.c_str(); }

		inline const char *getRelSegment() const { return m_rel_segment.c_str(); }
		inline const char *getPathSegments() const { return m_path_segments.c_str(); }

		inline std::list<CSegment>::const_iterator segmentsBegin() const
		{
			return m_segments.begin();
		}
		inline std::list<CSegment>::const_iterator segmentsEnd() const
		{
			return m_segments.end();
		}
		inline std::list<CSegment>::size_type segmentsSize() const
		{
			return m_segments.size();
		}
	};
}

#endif // // _RICPP_STREAMS_URI_H
