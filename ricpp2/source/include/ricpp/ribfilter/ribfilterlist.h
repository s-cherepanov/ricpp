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

namespace RiCPP {

/** @brief List to manage RIB filters.
 *  @see CRibFilter
 */
class CRibFilterList {
	friend class CRiCPPBridge;

	IRiRoot *m_ri; ///< Interface routines of the frontend that are called at last.
	std::list<CRibFilter *>m_filters; ///< List of filters.
	TemplPluginHandler<CRibFilter> m_pluginHandler; ///< Plugin handler to register filters in memory.

	/** @brief Sets the RenderMan interface.
	 *  @param ri Pointer to the interface, that will be called.
	 */
	inline CRibFilterList(IRiRoot *ri) {
		m_ri = ri;
	}

	/** @brief Gets a new filter.
	 *  @param name Name of the filter (registered or dynamic library).
	 */
	CRibFilter *newFilterPlugin(const char *name);

	/** @brief Deletes a filter.
	 *
	 *  Deletes a filter, that was created by the factory that is part
	 *  of this class.
	 *  @param aFilter Filter to destroy.
	 *  @return true, filter was deleted.
	 */
	bool deleteFilterPlugin(CRibFilter *aFilter);

public:
	/** @brief First interface that is called.
	 *
	 *  Is called from parser.
	 *  @return first interface, called from parser.
	 */
	inline IRiRoot *firstHandler() {
		return m_filters.empty() ? m_ri : m_filters.front();
	}

	/** @brief Last interface that is called.
	 *  Is called from parser.
	 *  @return last interface (part of the frontend), called from parser.
	 */
	inline IRiRoot *lastHandler() {
		return m_ri;
	}

	/** @brief Adds a new filter instance in front of the others.
	 *
	 *  @param aFilter Filter to add.
	 */
	bool addFront(CRibFilter *aFilter);

	/** @brief Adds a new filter plugin instance in front of the others.
	 *
	 *  Can load or instanciate a filter.
	 *
	 *  @param name Name of the filter to add.
	 */
	bool addFront(const char *name);

	/** @brief Removes the first filter.
	 *  Only the filters created by this object are also deleted.
	 *  @return true, filter was deleted.
	 */
	bool removeFront();

	/** @brief Gets the searchpath to the libraries
	 *  @return String with searchpath "path1:path2:..."
	 */
	inline const char *searchpath() const { return m_pluginHandler.searchpath(); }

	/** @brief Sets a searchpath to the libraries
	 *  @param path String with searchpath "path1:path2:..."
	 */
	inline void searchpath(const char *path) { m_pluginHandler.searchpath(path); }

	/** @brief Registers a filter factory
	 *  @param name Name of the plugin group
	 *  @param f filter factory
	 */
	inline virtual bool registerFactory(const char *name, TemplPluginFactory<CRibFilter> *f)
	{
		return m_pluginHandler.registerFactory(name, f);
	}
}; // class CRibFilterList

} // namespace RiCPP

#endif // _RICPP_RIBFILTER_RIBFILTERLIST_H
