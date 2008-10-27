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

/** @file ribfilterlist.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the class to handle RIB filter lists, part of CRiCPPBridge
 */

#include "ricpp/ribfilter/ribfilterlist.h"

using namespace RiCPP;

bool CRibFilterList::addFront(CRibFilter *aFilter)
{
	if ( !aFilter )
		return false;
	// Test if already in a list
	if ( aFilter->m_next != 0 )
		return false;

	aFilter->m_next = firstHandler();
	m_filters.push_front(aFilter);
	return true;
}

bool CRibFilterList::addFront(const char *name)
{
	return addFront(newFilterPlugin(name));
}

bool CRibFilterList::removeFront()
{
	std::list<CRibFilter *>::iterator current = m_filters.begin();

	if ( current != m_filters.end() ) {
		CRibFilter *filter = (*current);
		filter->m_next = 0;
		m_filters.remove(*current);
		deleteFilterPlugin(filter); // deletes only if self loaded
		return true;
	}

	return false;
}


CRibFilter *CRibFilterList::newFilterPlugin(const char *name)
{
	return m_pluginHandler.newPlugin(name);
}

bool CRibFilterList::deleteFilterPlugin(CRibFilter *aFilter)
{
	if ( !aFilter )
		return false;

	// member of a list?
	if ( aFilter->m_next != 0 )
		return false;

	return m_pluginHandler.deletePlugin(aFilter);
}

