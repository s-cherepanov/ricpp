#ifndef _RICPP_RIBFILTER_RIBFILTERLIST_H
#define _RICPP_RIBFILTER_RIBFILTERLIST_H

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

/** @file ribfilterlist.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Class to handle RIB filter lists, part of CRiCPPBridge
 */

#ifndef _RICPP_RIBFILTER_RIBFILTER_H
#include "ricpp/ribfilter/ribfilter.h"
#endif // _RICPP_RIBFILTER_RIBFILTER_H

#include <list>

namespace RiCPP {

class CRibFilterList {
	friend class CRiCPPBridge;

	IRiRoot *m_ri;
	std::list<CRibFilter *>m_filters;
	TPluginHandler<CRibFilter> m_pluginHandler;

	inline CRibFilterList(IRiRoot *ri) {
		m_ri = ri;
	}

	CRibFilter *newFilterPlugin(const char *name);
	bool deleteFilterPlugin(CRibFilter *aFilter);

public:
	inline IRiRoot *firstHandler() {
		return m_filters.empty() ? m_ri : m_filters.front();
	}
	inline IRiRoot *lastHandler() {
		return m_ri;
	}

	bool addFront(CRibFilter *aFilter);
	bool addFront(const char *name);
	bool removeFront();
	inline void searchpath(const char *path) { m_pluginHandler.searchpath(path); }
	inline virtual bool registerFactory(const char *name, TPluginFactory<CRibFilter> *f)
	{
		return m_pluginHandler.registerFactory(name, f);
	}
}; // class CRibFilterList

} // namespace RiCPP

#endif // _RICPP_RIBFILTER_RIBFILTERLIST_H
