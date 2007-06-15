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
			typedef std::list<std::string>::const_iterator const_iterator;
			typedef std::list<std::string>::size_type size_type;

			inline const std::string &getName() const { return m_name; }
			const_iterator begin() const
			{
				return m_parameters.begin();
			}
			const_iterator end() const
			{
				return m_parameters.end();
			}
			size_type size() const
			{
				return m_parameters.size();
			}
		};
		typedef std::list<CSegment>::const_iterator const_iterator;
		typedef std::list<CSegment>::size_type size_type;

	private:
		bool m_valid;

		std::string m_path;
		std::string m_escaped;

		std::string m_uri_reference;

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
		std::string m_ipV4address;

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

		void clearAll();

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

		void fragment(const unsigned char **str, std::string &result);
		void query(const unsigned char **str, std::string &result);
		void param(const unsigned char **str, std::string &result);
		void segment(const unsigned char **str, std::string &result);
		void path_segments(const unsigned char **str, std::string &result);
		void port(const unsigned char **str, std::string &result);
		bool abs_path(const unsigned char **str, std::string &result);
		bool opaque_part(const unsigned char **str, std::string &result);

		/*
		void path(const unsigned char **str, std::string &result);
		*/

		bool ipV4address(const unsigned char **str, std::string &result);

		/*
		bool toplabel(const unsigned char **str, std::string &result);
		*/

		bool domainlabel(const unsigned char **str, std::string &result);
		bool hostname(const unsigned char **str, std::string &result);
		bool host(const unsigned char **str, std::string &result);
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
			parse(uri.getUriReference());
			return *this;
		}

		bool makeAbsolute(const CUri &baseUri, std::string &resultUriStr) const;

		inline bool isValid() const { return m_valid; }

		inline const char *c_str() const { return m_uri_reference.c_str(); }
		inline const std::string &toString() const { return m_uri_reference; }

		inline const std::string &getUriReference() const { return m_uri_reference; }

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
		inline const std::string &getAbsPath() const { return m_abs_path; }
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
