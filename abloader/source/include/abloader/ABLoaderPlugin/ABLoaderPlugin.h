/*
 *  ABLoaderPlugin.h
 *  ABLoaderPlugin
 *
 *  Created by Andreas Pidde on 05.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _INCLUDE_ABLOADERPLUGIN
#define _INCLUDE_ABLOADERPLUGIN

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <PluginManager/LoaderPlugin.h>

class EXPORT ABLoaderPlugin : public AB::LoaderPlugin
{
private:
	static ABLoaderPlugin *plugin_instance;

	public:
		ABLoaderPlugin();
		virtual ~ABLoaderPlugin();
	
		static ABLoaderPlugin* MyInstance();
	    static void deleteInstance(ABLoaderPlugin *plugin);
	
		virtual AB::Loader* Instance(AB::ParamSet* paramSet);
	    virtual void load(std::string const &filename);
		virtual std::string	loadsFileExtension();
};

extern "C" AB::Plugin* Instance();

#pragma GCC visibility pop

#endif // _INCLUDE_ABLOADERPLUGIN
