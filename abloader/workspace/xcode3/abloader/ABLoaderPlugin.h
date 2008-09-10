#ifndef _ABLOADER_ABLOADERPLUGIN_ABLOADERPLUGIN_H
#define _ABLOADER_ABLOADERPLUGIN_ABLOADERPLUGIN_H

/*
 *  ABLoaderPlugin.h
 *  ABLoaderPlugin
 *
 *  Created by Andreas Pidde on 05.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma GCC visibility push(hidden)

#ifndef _RICPP_RICPPBRIDGE_RICPPBRIDGE_H
#include <ricpp/ricppbridge/ricppbridge.h>
#endif // _RICPP_RICPPBRIDGE_RICPPBRIDGE_H

#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#include <ricpp/pluginhandler/pluginhandler.h>
#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

#ifndef _ABLOADER_ABLOADER_ABLOADER_H
#include "abloader/abloader/abloader.h"
#endif // _ABLOADER_ABLOADER_ABLOADER_H

class RiCPP::CRiCPPBridge;
class RiCPP::CABLoader;

#pragma GCC visibility pop

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <PluginManager/LoaderPlugin.h>
class ABLoaderPlugin : public AB::LoaderPlugin
{
private:
	static ABLoaderPlugin *plugin_instance;
	static RiCPP::CRiCPPBridge *ri;
	static RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator> *abLoaderFactory;

	public:
		ABLoaderPlugin();
		virtual ~ABLoaderPlugin();
		static ABLoaderPlugin* MyInstance();
	    static void deleteInstance(ABLoaderPlugin *plugin);
		virtual AB::Loader* Instance(AB::ParamSet* paramSet);
		virtual void load(char const* filename);
		virtual std::string	loadsFileExtension();
};

extern "C" AB::Plugin* Instance();

#pragma GCC visibility pop

#endif // _ABLOADER_ABLOADERPLUGIN_ABLOADERPLUGIN_H
