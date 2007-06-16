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

	m_path.clear();
	m_escaped.clear();

	m_uri_reference.clear();

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
	m_ipV4address.clear();

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

void CUri::fragment(const unsigned char **str, std::string &result)
{
	m_fragment = "";
	while ( uric(str, m_fragment) );
	result += m_fragment;
}

void CUri::query(const unsigned char **str, std::string &result)
{
	m_query = "";
	while ( uric(str, m_query) );
	result += m_query;
}

void CUri::param(const unsigned char **str, std::string &result)
{
	m_param = "";
	while ( pchar(str, m_param) );
	result += m_param;
}

void CUri::segment(const unsigned char **str, std::string &result)
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

void CUri::path_segments(const unsigned char **str, std::string &result)
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

void CUri::port(const unsigned char **str, std::string &result)
{
	m_port = "";
	while ( digit(str, m_port) );
	result += m_port;
}

bool CUri::abs_path(const unsigned char **str, std::string &result)
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

bool CUri::opaque_part(const unsigned char **str, std::string &result)
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
// m_path is set in opaque_part() or abs_path() (also set in rel_path() here)
// The path component m_path can also be a net_path() or rel_path()
void CUri::path(const unsigned char **str, std::string &result)
{
	m_path = "";
	if ( opaque_part(str, m_path) || abs_path(str, m_path) ) {
		result += m_path;
	}
}
*/

bool CUri::ipV4address(const unsigned char **str, std::string &result)
{
	const unsigned char *sav = *str;
	m_ipV4address = "";

	for ( int i=0; i<4; ++i ) {
		if ( i > 0 ) {
			if ( *str[0] != '.' ) {
				m_ipV4address = "";
				*str = sav;
				return false;
			}
			advance(str, m_ipV4address);
		}
		if ( digit(str, m_ipV4address) ) {
			while ( digit(str, m_ipV4address) );
		} else {
			// At least one digit
			m_ipV4address = "";
			*str = sav;
			return false;
		}
	}
	result += m_ipV4address;
	return true;
}

/*
bool CUri::toplabel(const unsigned char **str, std::string &result)
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

bool CUri::domainlabel(const unsigned char **str, std::string &result)
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

bool CUri::hostname(const unsigned char **str, std::string &result)
{
	// Special handling: Last domainlabel has to be a toplabel
	const unsigned char *sav = *str;

	std::list<const unsigned char *>savList;

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
				while ( isdigit(m_toplabel[0]) ) {
					if ( m_domainlabels.empty() ) {
						m_hostname = "";
						m_toplabel = "";
						m_domainlabel = "";
						*str = sav;
						return false;
					}
					*str = savList.back();
					savList.pop_back();
					m_toplabel = m_domainlabels.back();
					m_domainlabels.pop_back();
				}
				m_hostname = "";
				while ( sav != *str ) {
					m_hostname += *sav;
					++sav;
				}
			}

			result += m_hostname;
			return true;
		}
		m_domainlabels.push_back(m_domainlabel);
		advance(str, m_hostname);
		savList.push_back(*str);
	}

	// At least one domain label
	if ( m_domainlabels.empty() ) {
		m_hostname = "";
		m_toplabel = "";
		m_domainlabel = "";
		m_domainlabels.clear();
		*str = sav;
		return false;
	}

	// Last domainlabel has to be a toplabel
	if ( isdigit(m_toplabel[0]) ) {
		while ( isdigit(m_toplabel[0]) ) {
			if ( m_domainlabels.empty() ) {
				m_hostname = "";
				m_toplabel = "";
				m_domainlabel = "";
				*str = sav;
				return false;
			}
			*str = savList.back();
			savList.pop_back();
			m_toplabel = m_domainlabels.back();
			m_domainlabels.pop_back();
		}
		m_hostname = "";
		while ( sav != *str ) {
			m_hostname += *sav;
			++sav;
		}
	}

	m_hasTrailingDot = !m_hostname.empty() && m_hostname[m_hostname.size()-1] == '.';
	result += m_hostname;
	return true;
}

/*
bool CUri::hostname(const unsigned char **str, std::string &result)
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
*/

bool CUri::host(const unsigned char **str, std::string &result)
{
	m_host = "";
	if ( hostname(str, m_host) ||
		ipV4address(str, m_host) )
	{
		result += m_host;
		return true;
	}
	return false;
}

bool CUri::hostport(const unsigned char **str, std::string &result)
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

void CUri::userinfo(const unsigned char **str, std::string &result)
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

void CUri::server(const unsigned char **str, std::string &result)
{
	const unsigned char *sav = *str;
	m_server = "";
	m_hasUserinfo = false;
	userinfo(str, m_server);

	// '@' seperates userinfo from hostport
	if ( (*str)[0] == '@' ) {
		advance(str, m_server);
		m_hasUserinfo = true;

		// There must be a hostport after an userinfo if a server
		if ( !hostport(str, m_server) ) {
			// empty server (also no userinfo)
			*str = sav;
			m_hasUserinfo = false;
			m_userinfo = "";
			return;
		}
		result += m_server;
		return;
	}

	m_userinfo = "";
	m_server = "";

	// no '@' -> no userinfo
	*str = sav;

	hostport(str, m_server);
	result += m_server;
}

bool CUri::reg_name(const unsigned char **str, std::string &result)
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

void CUri::authority(const unsigned char **str, std::string &result)
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

bool CUri::scheme(const unsigned char **str, std::string &result)
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

bool CUri::rel_segment(const unsigned char **str, std::string &result)
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

bool CUri::rel_path(const unsigned char **str, std::string &result)
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

bool CUri::net_path(const unsigned char **str, std::string &result)
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

	result += m_net_path;
	return true;
}

bool CUri::hier_part(const unsigned char **str, std::string &result)
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

bool CUri::relativeURI(const unsigned char **str, std::string &result)
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

	// can consist of query only
	if ( (*str)[0] == '?' ) {
		advance(str, m_relativeURI);
		m_hasQuery = true;
		query(str, m_relativeURI);
		result += m_relativeURI;
		return true;
	}

	return false;
}

bool CUri::absoluteURI(const unsigned char **str, std::string &result)
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

	advance(str, m_absoluteURI);

	// The base URI can consist of the <scheme> only:
	// written as <scheme>:/ - so <hier_part> is true
	if ( hier_part(str, m_absoluteURI) ||
		opaque_part(str, m_absoluteURI) )
	{
		result += m_absoluteURI;
		return true;
	}

	// Grammar must have an hier_part or an opaque part
	sav = *str;
	return false;
}

bool CUri::uri_reference(const unsigned char **str)
{
	const unsigned char *sav = *str;
	m_uri_reference = "";
	m_fragment = "";
	m_hasFragment = false;

	if ( !absoluteURI(str, m_uri_reference) )
		relativeURI(str, m_uri_reference);

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

void CUri::addSegment(const CSegment &seg, std::list<CSegment> &segList) const
{
	if ( seg.getName() == "." &&
		seg.size() == 0 )
	{
		// Ignore complete path segments "."
		return;
	}

	if ( seg.getName() == ".." &&
		seg.size() == 0 &&
		!segList.empty() )
	{
		const CSegment &segBack = segList.back();
		if ( segBack.getName() != ".." ) {
			segList.pop_back();
			return;
		}
	}

	segList.push_back(seg);
}

bool CUri::makeAbsolute(const CUri &baseUri, std::string &resultUriStr) const
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

	if ( isAbsolute() || hasOpaquePart() ) {
		// The URI is already absolute or is opaque
		resultUriStr = toString();
		return true;
	}

	// Check the base URI, must be absolute
	if ( !(isValid() && baseUri.isValid() && baseUri.isAbsolute()) ) {
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
	std::list<CSegment> pathList;

	refPath = "";
	pathList.clear();

	if ( hasAuthority() || hasAbsPath() ) {
		refPath = m_path;
	} else {
		const_iterator si = baseUri.segmentsBegin();
		const_iterator siend = baseUri.segmentsEnd();
		bool lastWasDot = false;
		for ( int pass = 0; pass < 2; ++pass ) {
			for ( ; si != siend; si++ ) {
				addSegment(*si, pathList);
				lastWasDot = (*si).getName() == "." || (*si).getName() == "..";
			}
			if ( pass == 0 ) {
				// skip the last segment, that is
				// all to the right of the rightmost '/' -
				// possibly was an empty segment
				if ( !pathList.empty() )
					pathList.pop_back();

				CSegment relSeg;
				relSeg.m_name = getRelSegment();
				addSegment(relSeg, pathList);
				lastWasDot = relSeg.getName() == "." || relSeg.getName() == "..";

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
			refPath += (*si).getName();
			CSegment::const_iterator pi = (*si).begin();
			for ( ; pi != (*si).end(); pi++ ) {
				refPath += ";";
				refPath += (*pi);
			}
		}
		if ( lastWasDot && strcmp(refPath.c_str(), "/")!=0 ) {
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
