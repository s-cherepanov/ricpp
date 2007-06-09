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

/** @file pluginhandler.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Test for the plugin management templates, will be removed
 */

#include "ricpp/pluginhandler/pluginhandler.h"

using namespace RiCPP;

/** @brief Test class for the plugin templates
 */
class CPlugin : public IPlugin {
public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline CPlugin::CPlugin() {}
	inline virtual ~CPlugin() {}

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}
};

const char *CPlugin::myName() { return "demo"; }
const char *CPlugin::myType() { return "plugin"; }
unsigned long CPlugin::myMajorVersion() { return 1; }
unsigned long CPlugin::myMinorVersion() { return 1; }
unsigned long CPlugin::myRevision() { return 1; }

void testPluginFactory()
{
	try {
		CPlugin plugin;
		TPluginFactory<CPlugin> pluginFactory;
		TPluginLoaderFactory<CPlugin> pluginLoaderFactory("demo");
		TPluginHandler<CPlugin> pluginHandler;
		TPluginHandlerSingleton<CPlugin> pluginHandlerSingleton;
	} catch ( ERiCPPError &e ) {
		e = e;
	}
}
