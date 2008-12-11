/*
 *  ABLoaderPlugin.cpp
 *  ABLoaderPlugin
 *
 *  Created by Andreas Pidde on 05.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "abloader/ABLoaderPlugin/ABLoaderPlugin.h"

#include <iostream>

using namespace AB;

ABLoaderPlugin *ABLoaderPlugin::plugin_instance = 0;
RiCPP::CRiCPPBridge *ABLoaderPlugin::ri = 0;
RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator> *ABLoaderPlugin::abLoaderFactory;

ABLoaderPlugin::ABLoaderPlugin()
{
}

ABLoaderPlugin::~ABLoaderPlugin()
{
}

ABLoaderPlugin* ABLoaderPlugin::MyInstance()
{
	if ( plugin_instance == 0 ) {
		plugin_instance = new ABLoaderPlugin;
		ri = new RiCPP::CRiCPPBridge;
		if ( ri ) {
			// Register abloader 'renderer' -  can also be omitted and to load libabloader.1.dylib dynamically
			// for static use link module with libabloader.a for dynamic use link with libricppbridge.a
			abLoaderFactory = new RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator>;
			if ( abLoaderFactory ) {
				ri->registerRendererFactory("abloader", reinterpret_cast<RiCPP::TemplPluginFactory<RiCPP::CContextCreator> *>(abLoaderFactory));
			}
		}
	}
	return plugin_instance;
}

void ABLoaderPlugin::deleteInstance(ABLoaderPlugin *plugin)
{
	if ( plugin && plugin != plugin_instance ) {
		delete plugin;
	}
	
	if ( ri ) {
		delete ri;
		ri = 0;
	}
	if ( plugin_instance ) {
		delete plugin_instance;
		plugin_instance = 0;
	}
}

std::string	ABLoaderPlugin::loadsFileExtension()
{
	return "rib";
}

Loader* ABLoaderPlugin::Instance(ParamSet* paramSet)
{
	return MyInstance();
}

void ABLoaderPlugin::load(std::string const &filename)
{
	if ( !ri )
		return;
	ri->begin("abloader", RI_NULL);
	ri->readArchive(filename.c_str(), 0, RI_NULL);
	ri->end();
}

extern "C" AB::Plugin* Instance()
{
	return ABLoaderPlugin::MyInstance();
}
