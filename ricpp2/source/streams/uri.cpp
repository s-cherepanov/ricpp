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

#ifndef _RICPP_TOOLS_PLATFORM_H
#include "ricpp/tools/platform.h"
#endif // _RICPP_TOOLS_PLATFORM_H

#include <iostream>

using namespace RiCPP;


void CUri::clear()
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


bool CUri::dec_octet(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	unsigned char d;
	m_dec_octet = "";
	unsigned char c1 = digit(str, m_dec_octet, d);

	if ( !c1 ) {
		return false;
	}

	if ( c1 != '0' ) {
		unsigned char c2 = digit(str, m_dec_octet, d);
		if ( c1 == '1' && c2  ) {
			digit(str, m_dec_octet, d);
		} else if ( c1 == '2' && c2 && c2 < '5' ) {
			digit(str, m_dec_octet, d);
		} else if ( c1 == '2' && c2 == '5' ) {
			unsigned char c3 = digit(str, m_dec_octet, d);
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


void CUri::fragment(
	const unsigned char **str,
	std::string &result)
{
	m_fragment = "";

	while (
		pchar(str, m_fragment) ||
		matchOneOf("/?", str, m_fragment) );

	result += m_fragment;
}


void CUri::query(
	const unsigned char **str,
	std::string &result)
{
	m_query = "";

	while (
		pchar(str, m_query) ||
		matchOneOf("/?", str, m_query) );

	result += m_query;
}


bool CUri::path_abempty(
	const unsigned char **str,
	std::string &result)
{
	m_path = "";

	if ( la(str) == '/' ) {
		return path_absolute(str, result);
	}

	return path_empty(str, result);
}


bool CUri::path_absolute(
	const unsigned char **str,
	std::string &result)
{
	m_path = "";
	m_pathType = pathTypeEmpty;
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


bool CUri::path_noscheme(
	const unsigned char **str,
	std::string &result)
{
	m_path = "";
	m_pathType = pathTypeEmpty;
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


bool CUri::path_rootless(
	const unsigned char **str,
	std::string &result)
{
	m_path = "";
	m_pathType = pathTypeEmpty;
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


bool CUri::path_empty(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	m_path = "";
	m_pathType = pathTypeEmpty;
	m_segments.clear();

	if ( pchar(str, m_path) ) {
		m_path = "";
		*str = sav;
		return false;
	}

	return true;
}


void CUri::segment(
	const unsigned char **str,
	std::string &result)
{
	m_segment = "";

	while ( pchar(str, m_segment) );

	result += m_segment;
}


bool CUri::segment_nz(
	const unsigned char **str,
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


bool CUri::segment_nz_nc(
	const unsigned char **str,
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


void CUri::port(
	const unsigned char **str,
	std::string &result)
{
	unsigned char d;
	m_port = "";

	while ( digit(str, m_port, d) );

	result += m_port;
}


// Not used
/*
bool CUri::path(
	const unsigned char **str,
	std::string &result)
{
	return
		path_abempty(str, result)  ||
		path_absolute(str, result) ||
		path_noscheme(str, result) ||
		path_rootless(str, result) ||
		path_empty(str, result);
}
*/


bool CUri::ipVFuture(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	unsigned char d;
	m_ipVFuture = "";
	m_ipAddrType = ipAddrTypeEmpty;

	if ( !match("v", str, m_ipVFuture) ) {
		return false;
	}

	if ( !hexdig(str, m_ipVFuture, d) ) {
		*str = sav;
		m_ipVFuture = "";
		return false;
	}

	while ( hexdig(str, m_ipVFuture, d) );

	if ( !(match(".", str, m_ipVFuture) &&
		(match(":", str, m_ipVFuture) ||
		unreserved(str, m_ipVFuture) ||
		sub_delims(str, m_ipVFuture))) )
	{
		*str = sav;
		m_ipVFuture = "";
		return false;
	}

	while (
		match(":", str, m_ipVFuture) ||
		unreserved(str, m_ipVFuture) ||
		sub_delims(str, m_ipVFuture) );

	result += m_ipVFuture;
	m_ipAddrType = ipAddrTypeVFuture;
	return true;
}


bool CUri::ipV6address(
	const unsigned char **str,
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
				if ( la(str) == '.' ) {
					*str = sav2;
					m_ipV6Address = "";
					for ( sav2 = sav; sav2 < *str; ++sav2 ) {
						m_ipV6Address += *sav2;
					}
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
				if ( la(str) == ']' ) {
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

	if (
		(!elisionFound && count != 8) ||
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


bool CUri::ip_literal(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;

	m_ip_literal = "";
	if (
		match("[", str, m_ip_literal) &&
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


bool CUri::ipV4address(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	m_ipV4Address = "";
	m_ipAddrType = ipAddrTypeEmpty;

	for ( int i=0; i<4; ++i ) {

		if ( i > 0 ) {
			if ( !match(".", str, m_ipV4Address) ) {
				m_ipV4Address = "";
				*str = sav;
				return false;
			}
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
bool CUri::ls32(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	m_ls32 = "";
	m_ipV4Address = "";

	if ( h16(str, m_ls32) ) {
		if ( match(":", str, m_ls32) ) {
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


void CUri::host(
	const unsigned char **str,
	std::string &result)
{
	m_host = "";
	if (
		ip_literal(str, m_host) ||
		ipV4address(str, m_host) )
	{
		result += m_host;
		return;
	}
	reg_name(str, m_host);
	result += m_host;
	return;
}


void CUri::userinfo(
	const unsigned char **str,
	std::string &result)
{
	m_userinfo = "";

	while (
		unreserved(str, m_userinfo)  ||
		pct_encoded(str, m_userinfo) ||
		sub_delims(str, m_userinfo)  ||
		match(":", str, m_userinfo) );

	result += m_userinfo;
}


void CUri::reg_name(
	const unsigned char **str,
	std::string &result)
{
	m_reg_name = "";

	while (
		unreserved(str, m_reg_name) != 0 ||
		pct_encoded(str, m_reg_name) ||
		sub_delims(str, m_reg_name) != 0 );

	result += m_reg_name;
	m_ipAddrType = ipRegName;
}


void CUri::authority(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;

	m_authority = "";
	m_hasPort = false;
	m_hasUserinfo = false;

	userinfo(str, m_authority);

	// '@' seperates userinfo from hostport
	if ( match("@", str, m_authority) ) {
		m_hasUserinfo = true;
	} else {
		*str = sav;
		m_authority = "";
	}

	host(str, m_authority);

	if ( match(":", str, m_authority) ) {
		m_hasPort = true;
		port(str, m_authority);
	}

	result += m_authority;
}


bool CUri::scheme(
	const unsigned char **str,
	std::string &result)
{
	m_scheme = "";
	if ( alpha(str, m_scheme) ) {
		for ( ;; ) {
			if ( alphanum(str, m_scheme) ) {
				continue;
			}
			unsigned char c = la(str);
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


bool CUri::hier_part(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	m_hier_part = "";

	if ( match("//", str, m_hier_part) ) {
		m_hasAuthority = true;
		authority(str, m_hier_part);
		if ( path_abempty(str, m_hier_part) ) {
			result += m_hier_part;
			return true;
		}
		*str = sav;
		m_hier_part = "";
		return false;
	}

	if (
		path_absolute(str, m_hier_part) ||
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


bool CUri::relative_uri(
	const unsigned char **str,
	std::string &result)
{
	m_relative_uri = "";
	m_query = "";
	m_hasQuery = false;

	if ( hier_part(str, m_relative_uri) )
	{
		if (  match("?", str, m_relative_uri) ) {
			m_hasQuery = true;
			query(str, m_relative_uri);
		}
		result += m_relative_uri;
		return true;
	}

	return false;
}


bool CUri::absolute_uri(
	const unsigned char **str,
	std::string &result)
{
	const unsigned char *sav = *str;
	m_absolute_uri = "";
	m_hasScheme = false;

	if ( !scheme(str, m_absolute_uri) ) {
		return false;
	}

	if ( !match(":", str, m_absolute_uri) ) {
		m_absolute_uri = "";
		*str = sav;
		return false;
	}
	m_hasScheme = true;

	// The base URI can consist of the <scheme> only:
	// written as <scheme>: - so <hier_part> is true
	if ( hier_part(str, m_absolute_uri) ) {
		if ( match("?", str, m_absolute_uri) ) {
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

	if ( !absolute_uri(str, m_uri_reference) ) {
		relative_uri(str, m_uri_reference);
	}

	if ( match("#", str, m_uri_reference) ) {
		m_hasFragment = true;
		fragment(str, m_uri_reference);
	}

	// Make shure that there are no more trailing characters
	return la(str) == 0;
}


bool CUri::parse(const char *anUri)
{
	clear();
	if ( !anUri ) {
		return m_valid;
	}

	std::string uri(anUri);
	const char *uriptr = uri.c_str();

	m_valid = uri_reference((const unsigned char **)&uriptr);
	return m_valid;
}


void CUri::addSegment(
	const std::string &seg,
	std::list<std::string> &segList)
{
	if ( seg == "." ) {
		// Ignore complete path segments "."
		return;
	}

	if ( seg == ".." ) {
		// One level up
		if ( !segList.empty() ) {
			segList.pop_back();
			return;
		}
		return;		
	}

	segList.push_back(seg);
}


void CUri::removeDotSegments(
	const std::list<std::string> &fromSegList,
	std::list<std::string> &segList,
	bool &lastWasDot )
{
	lastWasDot = false;
	for (
		std::list<std::string>::const_iterator si =
			fromSegList.begin();
		si != fromSegList.end();
		si++ )
	{
		// "." and ".." have special meaning
		addSegment(*si, segList);
		lastWasDot = (*si) == "." ||
			(*si) == "..";
	}
}


void CUri::segmentsToPath(
	bool isAbsolute,
	const std::list<std::string> &fromSegList,
	std::string &resultStr,
	bool lastWasDot )
{
	resultStr = "";

	for (
		std::list<std::string>::const_iterator si =
			fromSegList.begin();
		si != fromSegList.end();
		si++ )
	{
		resultStr += "/";
		resultStr += (*si);
	}

	if ( (resultStr.empty() && isAbsolute) ||
		(lastWasDot && resultStr[resultStr.size()-1] != '/') )
	{
		resultStr += "/";
	}
}


bool CUri::makeAbsolute(
	CUri &resultUri,
	const CUri &baseUri,
	const CUri &relativeUri,
	bool isStrict)
{
	if ( &relativeUri == &resultUri || &baseUri == &resultUri ) {
		CUri temp;
		bool ret = makeAbsolute(temp, baseUri, relativeUri, isStrict);
		resultUri = temp;
		return ret;
	}

	resultUri.clear();

	bool refHasScheme = relativeUri.hasScheme();

	if ( !isStrict &&
		strcasecmp(relativeUri.getScheme().c_str(), baseUri.getScheme().c_str()) == 0 )
	{
		refHasScheme = false;
	}

	// Path
	std::list<std::string> pathList;

	resultUri.m_hasFragment = relativeUri.hasFragment();		
	resultUri.m_fragment = relativeUri.getFragment();		

	bool lastWasDot;

	if ( refHasScheme ) {

		resultUri.m_hasScheme = true;
		resultUri.m_scheme = relativeUri.getScheme();
		resultUri.m_hasAuthority = relativeUri.hasAuthority();		
		removeDotSegments(relativeUri.getSegments(), pathList, lastWasDot);
		segmentsToPath(relativeUri.isAbsolutePath(), pathList, resultUri.m_path, lastWasDot);
		resultUri.m_authority = relativeUri.getAuthority();		
		resultUri.m_hasQuery = relativeUri.hasQuery();		
		resultUri.m_query = relativeUri.getQuery();


	} else {

		resultUri.m_hasScheme = baseUri.hasScheme();		
		resultUri.m_scheme = baseUri.getScheme();

		if ( relativeUri.hasAuthority() ) {

			resultUri.m_hasAuthority = relativeUri.hasAuthority();		
			resultUri.m_authority = relativeUri.getAuthority();		
			removeDotSegments(relativeUri.getSegments(), pathList, lastWasDot);
			segmentsToPath(relativeUri.isAbsolutePath(),
				pathList, resultUri.m_path, lastWasDot);
			resultUri.m_hasQuery = relativeUri.hasQuery();		
			resultUri.m_query = relativeUri.getQuery();

		} else {

			resultUri.m_hasAuthority = baseUri.hasAuthority();		
			resultUri.m_authority = baseUri.getAuthority();		

			if ( relativeUri.getPath().empty() ) {

				if ( relativeUri.hasQuery() ) {
					resultUri.m_hasQuery =  relativeUri.hasQuery();
					resultUri.m_query = relativeUri.getQuery();
				} else {
					resultUri.m_hasQuery =  baseUri.hasQuery();
					resultUri.m_query = baseUri.getQuery();
				}
				// m_path = baseUri.path() (??? was in RFC, but that's different to the other cases)
				removeDotSegments(baseUri.getSegments(), pathList, lastWasDot);
				segmentsToPath(baseUri.isAbsolutePath(),
					pathList, resultUri.m_path, lastWasDot);

			} else {

				resultUri.m_hasQuery =  relativeUri.hasQuery();
				resultUri.m_query = relativeUri.getQuery();

				if ( relativeUri.isAbsolutePath() ) {
					removeDotSegments(relativeUri.getSegments(), pathList, lastWasDot);
					segmentsToPath(relativeUri.isAbsolutePath(),
						pathList, resultUri.m_path, lastWasDot);
				} else {
					std::list<std::string>baseSegments = baseUri.getSegments();
					if ( !baseSegments.empty() )
						baseSegments.pop_back();
					removeDotSegments(baseSegments, pathList, lastWasDot);
					removeDotSegments(relativeUri.getSegments(), pathList, lastWasDot);
					segmentsToPath(baseUri.isAbsolutePath(),
						pathList, resultUri.m_path, lastWasDot);
				}
			}						
		}
	}

	return resultUri.reparse();
}


bool CUri::reparse()
{
	std::string uriStr;

	if ( hasScheme() ) {
		uriStr += getScheme();
		uriStr += ":";
	} 

	if ( hasAuthority() ) {
		uriStr += "//";
		uriStr += getAuthority();
		if ( !getPath().empty() && getPath()[0] != '/' ) {
			uriStr += "/";
		} 
	} 

	uriStr += getPath();

	if ( hasQuery() ) {
		uriStr += "?";
		uriStr += getQuery();
	}

	if ( hasFragment() ) {
		uriStr += "#";
		uriStr += getFragment();
	}

	return parse(uriStr);
}


bool CUri::reparseScheme(const char *aScheme, std::string &result)
{
	const unsigned char *str =
		(unsigned char *)aScheme;

	if ( (m_hasScheme = (aScheme != 0)) ) {
		if ( !scheme(&str, result) ) {
			return false;
		}
		if ( *str != 0 ) {
			return false;
		}
		result += ":";
	} else {
		m_scheme = "";
	}

	return true;
}
	

bool CUri::setScheme(const char *aScheme)
{
	std::string result;
	return reparseScheme(aScheme, result) ? reparse() : false;
}


bool CUri::reparseAuthority(const char *anAuthority, std::string &result)
{
	const unsigned char *str =
		(unsigned char *)anAuthority;

	if ( (m_hasAuthority = (anAuthority != 0)) ) {
		result += "//";
		authority(&str, result);
		return *str == 0;
	}
	
	m_authority = "";
	return true;
}


bool CUri::setAuthority(const char *anAuthority)
{
	std::string result;
	return reparseAuthority(anAuthority, result) ? reparse() : false;
}


bool CUri::reparsePath(const char *aPath, std::string &result)
{
	const unsigned char *str =
		(unsigned char *)aPath;

	if ( aPath && *aPath ) {
		if ( (aPath[0] != '/') && hasAuthority() ) {
			std::string temp("/");
			temp += aPath;
			str = (unsigned char *)temp.c_str();
			path_absolute(&str, result);
			return *str == 0;
		} else if ( aPath[0] == '/' ) {
			path_absolute(&str, result);
		} else {
			path_rootless(&str, result);
		}
		return *str == 0;
	}

	m_path = "";
	m_pathType = pathTypeEmpty;
	return true;
}


bool CUri::setPath(const char *aPath)
{
	std::string result;
	return reparsePath(aPath, result) ? reparse() : false;
}

bool CUri::reparseHierPart(const char *aHierPart, std::string &result)
{
	if ( aHierPart && *aHierPart ) {
		if ( !hier_part((const unsigned char **)&aHierPart, result) ) {
			return false;
		}
		return *aHierPart == 0;
	}
	
	m_authority = "";
	m_hasAuthority = false;
	m_path = "";
	m_pathType = pathTypeEmpty;
	return true;
}

bool CUri::setHierPart(const char *aHierPart)
{
	std::string result;
	return reparseHierPart(aHierPart, result) ? reparse() : false;
}


bool CUri::reparseQuery(const char *aQuery, std::string &result)
{
	const unsigned char *str =
		(unsigned char *)aQuery;

	if ( (m_hasQuery = (aQuery != 0)) ) {
		result += "?";
		query(&str, result);
		return *str == 0;
	}

	m_query = "";
	return true;
}


bool CUri::setQuery(const char *aQuery)
{
	std::string result;

	return reparseQuery(aQuery, result) ? reparse() : false;
}


bool CUri::reparseFragment(const char *aFragment, std::string &result)
{
	const unsigned char *str =
		(unsigned char *)aFragment;

	if ( (m_hasFragment = (str != 0)) ) {
		result += "#";
		fragment(&str, result);
		return *str == 0;
	}
	
	m_fragment = "";
	return true;
}


bool CUri::setFragment(const char *aFragment)
{
	std::string result;
	return reparseFragment(aFragment, result) ? reparse() : false;
}


bool CUri::set(
	const char *aScheme,
	const char *anAuthority,
	const char *aPath,
	const char *aQuery,
	const char *aFragment)
{
	std::string result;

	bool retVal =
		reparseScheme(aScheme, result) &&
		reparseAuthority(anAuthority, result) &&
		reparsePath(aPath, result) &&
		reparseQuery(aQuery, result) &&
		reparseFragment(aFragment, result);

	return retVal ? parse(result) : false;
}


bool CUri::set(
	const char *aScheme,
	const char *aHierPart,
	const char *aQuery,
	const char *aFragment)
{
	std::string result;

	bool retVal =
		reparseScheme(aScheme, result) &&
		reparseHierPart(aHierPart, result) &&
		reparseQuery(aQuery, result) &&
		reparseFragment(aFragment, result);

	return retVal ? parse(result) : false;
}

unsigned int CUri::escape(
	const unsigned char **str,
	unsigned int n,
	std::string &result) const
{
	static const char *hexes = "0123456789abcdef";
	if ( !*str )
		return 0;
	
	unsigned int i = 0;
	for ( ; i < n && **str; ++i, ++(*str) ) {
		const unsigned char uc = **str;
		const unsigned char h = uc/16;
		const unsigned char m = uc%16;
		result += "%";
		result += hexes[h];
		result += hexes[m];
	}
	
	return i;
}

bool CUri::encodeFilepath(const char *aPath, const char *aScheme)
{
	clear();
	
	if ( !aPath)
		return false;
	
	std::string encoded = "";


#ifdef _WIN32
	{
		size_t len = strlen(aPath);
		if ( len > 1 ) {
			if ( (aPath[1] == ':' || aPath[1] == '|' )  &&
			     ((aPath[0] >= 'A' && aPath[0] <= 'Z') ||
				  (aPath[0] >= 'a' && aPath[0] <= 'z')) )
			{
				encoded = '/';
				encoded += aPath[0];
				encoded += ':';
				aPath += 2;
			}
		}
	}
#endif

	for ( const unsigned char *c = (const unsigned char *)aPath; *c; ) {
#ifdef _WIN32
		if ( *c == '\\' ) {
			encoded += '/';
			++c;
			continue;
		}
#endif
		if ( *c == '/' ) {
			encoded += *c;
			++c;
			continue;
		}
		if ( *c != '%' && pchar(&c, encoded)  )
			continue;
		escape(&c, 1, encoded);
	}
	
	clear();

	// std::cerr << "# ENCODED: '" << encoded << "'" << std::endl;

	// set
	return set(aScheme, encoded.c_str(), 0, 0);
}


const char *CUri::decodeFilepath(std::string &path) const
{
	path = "";
	
	const char *hierPart = getHierPart().c_str();
	if ( !hierPart )
		return path.c_str();

	std::string decoded = "";
	
	for ( const unsigned char *c = (const unsigned char *)hierPart; *c; ++c ) {
		if ( *c == '%' ) {
			++c;
			const unsigned char h = *c;
			++c;
			const unsigned char m = *c;
			
			assert( (h >= 'A' && h <='Z') || (h >= 'a' && h <='z') || (h >= '0' && h <='9') );
			assert( (m >= 'A' && m <='Z') || (m >= 'a' && m <='z') || (m >= '0' && m <='9') );
					
			const unsigned char uch = (h >= 'A' && h <= 'F') ? h - 'A' + 10 : ((h >= 'a' && h <= 'f') ? h - 'a' + 10 : h - '0');
			const unsigned char ucl = (m >= 'A' && m <= 'F') ? m - 'A' + 10 : ((m >= 'a' && m <= 'f') ? m - 'a' + 10 : m - '0');
			const unsigned char uc = uch * 16 + ucl;

			decoded += uc;
			// std::cerr << "# DECODE: %" << h << m << " '" << uc << "' - " << (int)uc << ", " << (int)uch << ", " << (int)ucl << std::endl;
			continue;
		}
		decoded += *c;
	}
	
	const char *filepath = decoded.c_str();
	
	if ( decoded.length() > 3 ) {
		// empty authority ?
		if ( filepath[0] == '/' && filepath[1] == '/' && filepath[2] == '/' )
			filepath += 2;
	}


#ifdef _WIN32
	{
		size_t len = strlen(filepath);
		if ( len > 2 ) {
			if ( filepath[0] == '/' &&
			     (filepath[2] == ':') &&
			     ((filepath[1] >= 'A' && filepath[1] <= 'Z') ||
			      (filepath[1] >= 'a' && filepath[1] <= 'z')) )
			{
				++filepath;
				--len;
			}
		}
	}
#endif

	path = filepath;

#ifdef _WIN32
	{
		size_t len = path.length();
		while ( len > 0 ) {
			--len;
			if ( path[len] == '/' )
				path[len] = '\\';
		}
	}
#endif

	// std::cerr << "# DECODED: '" << path << "'" << std::endl;

	return path.c_str();
}
