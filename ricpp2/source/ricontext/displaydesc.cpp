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

/** @file displaydesc.cpp
 *  @brief Implementation of the CDisplayDescr display descriptor
 *         (stores contents of a RiDisplay call).
 *  @author Andreas Pidde (andreas@pidde.de)
 */

#include "ricpp/ricontext/displaydesc.h"

using namespace RiCPP;

CDisplayChannelDescr::CDisplayChannelDescr()
{
	m_channel = 0;
}

CDisplayChannelDescr::CDisplayChannelDescr(const CDisplayChannelDescr &dcd)
{
	m_channel = 0;
	*this = dcd;
}

CDisplayChannelDescr::CDisplayChannelDescr(const CDisplayChannelDescr &dd, CDeclarationDictionary &newDict)
{
	m_channel = 0;
	assignRemap(dd, newDict);
}

CDisplayChannelDescr::~CDisplayChannelDescr()
{
	if ( m_channel )
		delete m_channel;
}

CDisplayChannelDescr &CDisplayChannelDescr::operator=(const CDisplayChannelDescr &dcd)
{
	if ( this == &dcd )
		return *this;

	if ( m_channel )
		delete m_channel;

	m_channel = 0;
	if ( dcd.m_channel )
		m_channel = new CDeclaration(*dcd.m_channel);

	CParameterList::operator=(dcd);
	return *this;
}


CDisplayChannelDescr &CDisplayChannelDescr::assignRemap(const CDisplayChannelDescr &dcd, CDeclarationDictionary &newDict)
{
	if ( this == &dcd )
		return *this;

	if ( m_channel )
		delete m_channel;

	m_channel = 0;
	if ( dcd.m_channel )
		m_channel = new CDeclaration(*dcd.m_channel, newDict.tokenMap());

	CParameterList::assignRemap(dcd, newDict);
	return *this;
}


bool CDisplayChannelDescr::operator==(const CDisplayChannelDescr &dcd) const
{
	if ( m_channel == dcd.channel() )
		return true;
	if ( !m_channel )
		return false;
	if ( !dcd.channel() )
		return false;

	return !strcmp(noNullStr(m_channel->qualifiedName()), noNullStr(dcd.channel()->qualifiedName()));
}

bool CDisplayChannelDescr::operator==(const std::string name) const
{
	return *this == name.c_str();
}

bool CDisplayChannelDescr::operator==(const char *qualifiedName) const
{
	if ( !m_channel )
		return false;
	if ( qualifiedName == m_channel->qualifiedName() )
		return true;

	return !strcmp(noNullStr(m_channel->qualifiedName()), noNullStr(qualifiedName));
}

RtVoid CDisplayChannelDescr::displayChannelV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString aChannel, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_channel )
		delete m_channel;

	m_channel = new CDeclaration(aChannel, colorDescr, dict.tokenMap());
	set(0, 0, CValueCounts(), dict, colorDescr, aChannel, n, tokens, params);
}

RtVoid CDisplayChannelDescr::displayChannel(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString aChannel, const CParameterList &params)
{
	if ( m_channel )
		delete m_channel;

	m_channel = new CDeclaration(aChannel, colorDescr, dict.tokenMap());
	set(aChannel, params);
}

// ----------------------------------------------------------------------------
// CDisplayDescr

CDisplayDescr::CDisplayDescr()
{
	m_type = RI_NULL;
	m_mode = RI_NULL;
	m_origin[0] = m_origin[1] = 0;
}


CDisplayDescr::CDisplayDescr(const CDisplayDescr &dd) {
	m_type = RI_NULL;
	m_mode = RI_NULL;
	m_origin[0] = m_origin[1] = 0;
	*this = dd;
}

CDisplayDescr::CDisplayDescr(const CDisplayDescr &dd, CDeclarationDictionary &newDict)
{
	m_type = RI_NULL;
	m_mode = RI_NULL;
	m_origin[0] = m_origin[1] = 0;
	assignRemap(dd, newDict);
}

CDisplayDescr &CDisplayDescr::operator=(const CDisplayDescr &dd) {
	if ( &dd == this )
		return *this;

	m_type = dd.m_type;
	m_mode = dd.m_mode;
	m_origin[0] = dd.m_origin[0];
	m_origin[1] = dd.m_origin[1];

	m_channels = dd.m_channels;
	m_channelNames = dd.m_channelNames;

	CNamedParameterList::operator=(dd);
	return *this;
}

CDisplayDescr &CDisplayDescr::assignRemap(const CDisplayDescr &dd, CDeclarationDictionary &newDict)
{
	if ( &dd == this )
		return *this;

	m_type = newDict.tokenMap().findCreate(dd.m_type);
	m_mode = newDict.tokenMap().findCreate(dd.m_mode);
	m_origin[0] = dd.m_origin[0];
	m_origin[1] = dd.m_origin[1];

	m_channels.clear();
	
	std::list<CDisplayChannelDescr>::const_iterator i = dd.m_channels.begin();

	for ( ; i != dd.m_channels.end(); ++i ) {
		m_channels.push_back(CDisplayChannelDescr((*i), newDict)); 
	}
	m_channelNames = dd.m_channelNames;

	CParameterList::assignRemap(dd, newDict);
	return *this;
}

void CDisplayDescr::display(const CDisplayDescr::TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode) {
	m_type = aType;
	m_mode = aMode;

	m_origin[0] = 0;
	m_origin[1] = 0;
	

	const CParameter *p = get(RI_ORIGIN);
	if ( p ) {
		const std::vector<RtInt> &ints = p->ints();
		if ( ints.size() >= 2 )
		{
			m_origin[0] = ints[0];
			m_origin[1] = ints[1];
		}
	}

	m_channels.clear();

	m_channelNames.clear();
	m_channelNames.explode(',', aMode, false);
	for ( CStringList::const_iterator iter = m_channelNames.begin();
	      iter != m_channelNames.end();
	      ++iter )
	{
		for ( CDisplayDescr::TypeDisplayChannels::const_iterator dciter = channels.begin();
			  dciter != channels.end();
			  ++dciter )
		{
			if ( *dciter == *iter ) {
				m_channels.push_back(*dciter);
			}
		}
	}
}

void CDisplayDescr::displayV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, const CDisplayDescr::TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, RtInt n, RtToken tokens[], RtPointer params[]) {
	set(RI_DISPLAY, aType, CValueCounts(), dict, colorDescr, aName, n, tokens, params);
	display(channels, aName, aType, aMode);
}

void CDisplayDescr::display(const CDisplayDescr::TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, const CParameterList &params) {
	set(aName, params);
	display(channels, aName, aType, aMode);
}
