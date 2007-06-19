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

/** @file uri.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a class for generic URIs
 */

#include "ricpp/streams/uri.h"

using namespace RiCPP;

void CUri::clearAll()
{
	m_valid = true;
	m_pathType = pathTypeEmpty;
	m_ipAddrType = ipAddrTypeEmpty;

	m_path.clear();
	m_ls32.clear();
	m_h16.clear();
	m_dec_octet.clear();
	m_pct_encoded.clear();

	m_uri_reference.clear();

	m_absolute_uri.clear();
	m_relative_uri.clear();

	m_hasScheme = false;
	m_scheme.clear();

	m_host.clear();

	m_hier_part.clear();

	m_hasQuery = false;
	m_query.clear();

	m_hasFragment = false;
	m_fragment.clear();

	m_hasAuthority = false;
	m_authority.clear();

	m_hasUserinfo = false;
	m_userinfo.clear();

	m_reg_name.clear();

	m_ip_literal.clear();
	m_ipV6Address.clear();
	m_ipVFuture.clear();
	m_ipV4Address.clear();

	m_hasPort = false;
	m_port.clear();

	m_segment.clear();
	m_segments.clear();
}

bool CUri::dec_octet(const unsigned char **str,
               std::string &result)
{
	const unsigned char *sav = *str;
	m_dec_octet = "";
	unsigned char c1 = digit(str, m_dec_octet);

	if ( !c1 ) {
		return false;
	}

	if ( c1 != '0' ) {
		unsigned char c2 = digit(str, m_dec_octet);
		if ( c1 == '1' && c2  ) {
			digit(str, m_dec_octet);
		} else if ( c1 == '2' && c2 && c2 < '5' ) {
			digit(str, m_dec_octet);
		} else if ( c1 == '2' && c2 == '5' ) {
			unsigned char c3 = digit(str, m_dec_octet);
			if ( c3 > '5' ) {
				*str = sav;
				m_dec_octet = "";
				return false;
			}
		}
	}
	result += m_dec_octet;
	return true;
}

void CUri::fragment(const unsigned char **str,
                    std::string &result)
{
	m_fragment = "";
	while ( pchar(str, m_fragment) ||
		    matchOneOf("/?", str, m_fragment) )
	{
	}
	result += m_fragment;
}

void CUri::query(const unsigned char **str,
                 std::string &result)
{
	m_query = "";
	while ( pchar(str, m_query) ||
		    matchOneOf("/?", str, m_query) )
	{
	}
	result += m_query;
}

bool CUri::path_abempty(const unsigned char **str,
                        std::string &result)
{
	m_path = "";
	if ( (*str)[0] == '/' )
		return path_absolute(str, result);
	return true;
}

bool CUri::path_absolute(const unsigned char **str,
                        std::string &result)
{
	m_path = "";
	m_segments.clear();

	if ( !match("/", str, m_path) ) {
		return false;
	}

	if ( segment_nz(str, m_path) ) {
		m_segments.push_back(m_segment);
		while ( match("/", str, m_path) ) {
			segment(str, m_path);
			m_segments.push_back(m_segment);
		}
	}

	result += m_path;
	m_pathType = pathTypeAbsolute;
	return true;
}

bool CUri::path_noscheme(const unsigned char **str,
                        std::string &result)
{
	m_path = "";
	m_segments.clear();

	if ( !segment_nz_nc(str, m_path) ) {
		return false;
	}
	m_segments.push_back(m_segment);

	while ( match("/", str, m_path) ) {
		segment(str, m_path);
		m_segments.push_back(m_segment);
	}

	result += m_path;
	m_pathType = pathTypeNoScheme;
	return true;
}

bool CUri::path_rootless(const unsigned char **str,
                        std::string &result)
{
	m_path = "";
	m_segments.clear();

	if ( !segment_nz(str, m_path) ) {
		return false;
	}
	m_segments.push_back(m_segment);

	while ( match("/", str, m_path) ) {
		segment(str, m_path);
		m_segments.push_back(m_segment);
	}

	result += m_path;
	m_pathType = pathTypeRootless;
	return true;
}

bool CUri::path_empty(const unsigned char **str,
                        std::string &result)
{
	const unsigned char *sav = *str;
	m_path = "";
	m_segments.clear();

	if ( pchar(str, m_path) ) {
		m_path = "";
		*str = sav;
		return false;
	}

	m_pathType = pathTypeEmpty;
	return true;
}

void CUri::segment(const unsigned char **str,
                   std::string &result)
{
	m_segment = "";

	while ( pchar(str, m_segment) );

	result += m_segment;
}

bool CUri::segment_nz(const unsigned char **str,
                      std::string &result)
{
	m_segment = "";

	if ( !pchar(str, m_segment) ) {
		return false;
	}

	while ( pchar(str, m_segment) );

	result += m_segment;
	return true;
}

bool CUri::segment_nz_nc(const unsigned char **str,
                         std::string &result)
{
	m_segment = "";

	if ( !(unreserved(str, m_segment) ||
		   pct_encoded(str, m_segment) ||
		   sub_delims(str, m_segment) ||
		   match("@", str, m_segment)) )
	{
		return false;
	}

	while ( unreserved(str, m_segment) ||
		    pct_encoded(str, m_segment) ||
		    sub_delims(str, m_segment) ||
		    match("@", str, m_segment) )
	{
	}

	result += m_segment;
	return true;
}

void CUri::port(const unsigned char **str,
                std::string &result)
{
	m_port = "";
	while ( digit(str, m_port) );
	result += m_port;
}

// Not used
/*
bool CUri::path(const unsigned char **str,
                std::string &result)
{
	return path_abempty(str, result)  ||
		   path_absolute(str, result) ||
		   path_noscheme(str, result) ||
		   path_rootless(str, result) ||
		   path_empty(str, result);
}
*/

bool CUri::ipVFuture(const unsigned char **str,
                     std::string &result)
{
	const unsigned char *sav = *str;
	m_ipVFuture = "";
	m_ipAddrType = ipAddrTypeEmpty;

	if ( *str[0] != 'v' ) {
		return false;
	}
	advance(str, m_ipVFuture);

	if ( !hexdig(str, m_ipVFuture) ) {
		*str = sav;
		m_ipVFuture = "";
		return false;
	}
	while ( hexdig(str, m_ipVFuture) );

	if ( *str[0] != '.' ) {
		*str = sav;
		m_ipVFuture = "";
		return false;
	}
	advance(str, m_ipVFuture);

	bool has_char = false;
	for ( ; ; has_char = true ) {
		if ( *str[0] == ':' ) {
			advance(str, m_ipVFuture);
		} else if ( !(unreserved(str, m_ipVFuture) ||
		              sub_delims(str, m_ipVFuture)) )
		{
			break;
		}
	}

	if ( !has_char ) {
		*str = sav;
		m_ipVFuture = "";
		return false;
	}

	result += m_ipVFuture;
	m_ipAddrType = ipAddrTypeVFuture;
	return true;
}

bool CUri::ipV6address(const unsigned char **str,
                       std::string &result)
{
	const unsigned char *sav = *str;

	m_ipV6Address = "";
	m_ipAddrType = ipAddrTypeEmpty;

	int countLeft = 0;
	int countRight = 0;

	bool elisionFound = match("::", str, m_ipV6Address);
	const unsigned char *sav2 = *str;
	
	for ( ;; ) {
		if ( h16(str, m_ipV6Address) ) {
			++countRight;
			if ( !match(":", str, m_ipV6Address) ) {
				if ( (*str)[0] == '.' ) {
					*str = sav2;
					m_ipV6Address = "";
					for ( sav2 = sav; sav2 < *str; ++sav2 )				
						m_ipV6Address += *sav2;
					if ( !ipV4address(str, m_ipV6Address) ) {
						*str = sav2;
						m_ipV6Address = "";
						return false;
					}
					// IPv4: 2x 16 Bit
					++countRight;
				}
				break;
			} 
			if ( !elisionFound && match(":", str, m_ipV6Address) ) {
				elisionFound = true;
				countLeft = countRight;
				countRight = 0;
				if ( (*str)[0] == ']' ) {
					break;
				}
			}
			sav2 = *str;
		} else {
			*str = sav2;
			m_ipV6Address = "";
			return false;
		}
	}
	
	int count = countLeft + countRight;

	if ( (!elisionFound && count != 8) ||
	     (elisionFound && count > 7) )
	{
		*str = sav2;
		m_ipV6Address = "";
		return false;
	}
	
	result += m_ipV6Address;
	m_ipAddrType = ipAddrTypeV6Address;
	return true;
}

bool CUri::ip_literal(const unsigned char **str,
                std::string &result)
{
	const unsigned char *sav = *str;
	m_ip_literal = "";
	if ( match("[", str, m_ip_literal) &&
	     (ipV6address(str, m_ip_literal) || ipVFuture(str, m_ip_literal)) &&
	     match("]", str, m_ip_literal) )
	{
		result += m_ip_literal;
		return true;
	}

	m_ip_literal = "";
	*str = sav;
	return false;
}


bool CUri::ipV4address(const unsigned char **str,
                       std::string &result)
{
	const unsigned char *sav = *str;
	m_ipV4Address = "";
	m_ipAddrType = ipAddrTypeEmpty;

	for ( int i=0; i<4; ++i ) {
		if ( i > 0 ) {
			if ( *str[0] != '.' ) {
				m_ipV4Address = "";
				*str = sav;
				return false;
			}
			advance(str, m_ipV4Address);
		}
		if ( !dec_octet(str, m_ipV4Address) ) {
			m_ipV4Address = "";
			*str = sav;
			return false;
		}
	}
	result += m_ipV4Address;
	m_ipAddrType = ipAddrTypeV4Address;
	return true;
}

// Not used
/*
bool CUri::ls32(const unsigned char **str,
                std::string &result)
{
	const unsigned char *sav = *str;
	m_ls32 = "";
	m_ipV4Address = "";

	if ( h16(str, m_ls32) ) {
		if ( (*str)[0] == ':' ) {
			advance(str, m_ls32);
			if ( h16(str, m_ls32) ) {
				result += m_ls32;
				return true;
			}
		}
	}

	*str = sav;
	m_ls32 = "";

	if ( !ipV4address(str, m_ls32) ) {
		*str = sav;
		m_ls32 = "";
		return false;
	}

	result += m_ls32;
	return true;
}
*/

void CUri::host(const unsigned char **str,
                std::string &result)
{
	m_host = "";
	if ( ip_literal(str, m_host) ||
		ipV4address(str, m_host) )
	{
		result += m_host;
		return;
	}
	reg_name(str, m_host);
	result += m_host;
	return;
}

void CUri::userinfo(const unsigned char **str,
                    std::string &result)
{
	m_userinfo = "";
	while ( unreserved(str, m_userinfo)  ||
		    pct_encoded(str, m_userinfo) ||
		    sub_delims(str, m_userinfo)  ||
			match(":", str, m_userinfo) );

	result += m_userinfo;
}

void CUri::reg_name(const unsigned char **str,
                    std::string &result)
{
	m_reg_name = "";
	while ( unreserved(str, m_reg_name) != 0 ||
			pct_encoded(str, m_reg_name) ||
			sub_delims(str, m_reg_name) != 0 );

	result += m_reg_name;
	m_ipAddrType = ipRegName;
}

void CUri::authority(const unsigned char **str,
                     std::string &result)
{
	const unsigned char *sav = *str;

	m_authority = "";
	m_hasPort = false;
	m_hasUserinfo = false;

	userinfo(str, m_authority);

	// '@' seperates userinfo from hostport
	if ( (*str)[0] == '@' ) {
		advance(str, m_authority);
		m_hasUserinfo = true;
	} else {
		*str = sav;
		m_authority = "";
	}

	host(str, m_authority);

	if ( (*str)[0] == ':' ) {
		advance(str, m_authority);
		m_hasPort = true;
		port(str, m_authority);
	}

	m_hasAuthority = true;
	result += m_authority;
}

bool CUri::scheme(const unsigned char **str,
                  std::string &result)
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

bool CUri::hier_part(const unsigned char **str,
                     std::string &result)
{
	const unsigned char *sav = *str;
	m_hier_part = "";

	if ( match("//", str, m_hier_part) ) {
		authority(str, m_hier_part);
		if ( path_abempty(str, m_hier_part) ) {
			result += m_hier_part;
			return true;
		}
	}
	*str = sav;
	m_hier_part = "";

	if ( path_absolute(str, m_hier_part) ||
		 path_rootless(str, m_hier_part) ||
		 path_empty(str, m_hier_part) )
	{
		result += m_hier_part;
		return true;
	}

	*str = sav;
	m_hier_part = "";
	return false;
}

bool CUri::relative_uri(const unsigned char **str,
                       std::string &result)
{
	m_relative_uri = "";
	m_query = "";
	m_hasQuery = false;

	if ( hier_part(str, m_relative_uri) )
	{
		if ( (*str)[0] == '?' ) {
			advance(str, m_relative_uri);
			m_hasQuery = true;
			query(str, m_relative_uri);
		}
		result += m_relative_uri;
		return true;
	}

	return false;
}

bool CUri::absolute_uri(const unsigned char **str,
                       std::string &result)
{
	const unsigned char *sav = *str;
	m_absolute_uri = "";
	m_hasScheme = false;

	if ( !scheme(str, m_absolute_uri) ) {
		return false;
	}

	if ( (*str)[0] != ':' ) {
		m_absolute_uri = "";
		*str = sav;
		return false;
	}
	m_hasScheme = true;

	advance(str, m_absolute_uri);

	// The base URI can consist of the <scheme> only:
	// written as <scheme>: - so <hier_part> is true
	if ( hier_part(str, m_absolute_uri) )
	{
		if ( (*str)[0] == '?' ) {
			advance(str, m_absolute_uri);
			m_hasQuery = true;
			query(str, m_absolute_uri);
		}
		result += m_absolute_uri;
		return true;
	}

	// Grammar must have an hier_part or an opaque part
	sav = *str;
	return false;
}

bool CUri::uri_reference(const unsigned char **str)
{
	m_uri_reference = "";
	m_fragment = "";
	m_hasFragment = false;

	if ( !absolute_uri(str, m_uri_reference) )
		relative_uri(str, m_uri_reference);

	if ( (*str)[0] == '#' ) {
		advance(str, m_uri_reference);
		m_hasFragment = true;
		fragment(str, m_uri_reference);
	}
	// Make shure that there are no more trailing characters
	return *str[0] == 0;
}

bool CUri::parse(const char *anUri)
{
	clearAll();
	if ( !anUri ) {
		return m_valid;
	}
	std::string uri(anUri);
	const char *uriptr = uri.c_str();
	m_valid = uri_reference((const unsigned char **)&uriptr);
	return m_valid;
}

void CUri::addSegment(const std::string &seg,
					  std::list<std::string> &segList) const
{
	if ( seg == "." )
	{
		// Ignore complete path segments "."
		return;
	}

	if ( seg == ".." &&
		!segList.empty() )
	{
		const std::string &segBack = segList.back();
		if ( segBack != ".." ) {
			segList.pop_back();
			return;
		}
	}

	segList.push_back(seg);
}

bool CUri::makeAbsolute(const CUri &baseUri,
                        std::string &resultUriStr) const
{
	resultUriStr = "";

	if ( getPath().empty() &&
		!hasScheme() &&
		!hasQuery() &&
		!hasAuthority() )
	{
		if ( hasFragment() )
		{
			resultUriStr += "#";
			resultUriStr += getFragment();
		}
		return true;
	}

	if ( isAbsoluteUri() ) {
		// The URI is already absolute or is opaque
		resultUriStr = toString();
		return true;
	}

	// Check the base URI, must be absolute
	if ( !(isValid() && baseUri.isValid() && baseUri.isAbsoluteUri()) ) {
		return false;
	}

	// Query is not inherited

	// Fragment is not inherited

	// Inherit the scheme
	bool refHasScheme = hasScheme();
	const char *refScheme = getScheme().c_str();
	if ( !refHasScheme ) {
		refScheme = baseUri.getScheme().c_str();
		refHasScheme = baseUri.hasScheme();
	}

	// Authority
	bool refHasAuthority = hasAuthority();
	const char *refAuthority = getAuthority().c_str();
	if ( !refHasAuthority ) {
		refAuthority = baseUri.getAuthority().c_str();
		refHasAuthority = baseUri.hasAuthority();
	}

	// Possibly inherited parts
	if ( refHasScheme )
	{
		resultUriStr += noNullStr(refScheme);
		resultUriStr += ":";
	}

	if ( refHasAuthority )
	{
		resultUriStr += "//";
		resultUriStr += noNullStr(refAuthority);
	}

	// Path
	std::string refPath;
	std::list<std::string> pathList;

	refPath = "";
	pathList.clear();

	if ( hasAuthority() || pathType() == pathTypeAbsolute ) {
		refPath = m_path;
	} else {
		segments_const_iterator si = baseUri.segmentsBegin();
		segments_const_iterator siend = baseUri.segmentsEnd();
		bool lastWasDot = false;
		for ( int pass = 0; pass < 2; ++pass ) {
			for ( ; si != siend; si++ ) {
				if ( pass > 0 ) {
					// Only in relative pathes "." and ".." have special meaning
					addSegment(*si, pathList);
					lastWasDot = (*si) == "." ||
					             (*si) == "..";
				} else {
					pathList.push_back(*si);
				}
			}
			if ( pass == 0 ) {
				// skip the last segment, that is
				// all to the right of the rightmost '/' -
				// possibly was an empty segment
				if ( !pathList.empty() )
					pathList.pop_back();

				std::string relSeg;
				si = segmentsBegin();
				siend = segmentsEnd();
			}
		}

		refPath = "";
		if ( pathList.empty() ) {
			refPath += "/";
		}
		for ( si = pathList.begin(); si != pathList.end(); si++ ) {
			refPath += "/";
			refPath += (*si);
		}
		if ( lastWasDot && strcmp(refPath.c_str(), "/") !=0 ) {
			refPath += "/";
		}
	}

	// Mixed parts (only the path)
	resultUriStr += refPath;

	// Not inherited parts
	if ( hasQuery() )
	{
		resultUriStr += "?";
		resultUriStr += getQuery();
	}

	if ( hasFragment() )
	{
		resultUriStr += "#";
		resultUriStr += getFragment();
	}

	return true;
}
