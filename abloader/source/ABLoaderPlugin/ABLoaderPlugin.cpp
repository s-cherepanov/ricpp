/*
 *  ABLoaderPlugin.cpp
 *  ABLoaderPlugin
 *
 *  Created by Andreas Pidde on 05.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "abloader/ABLoaderPlugin/ABLoaderPlugin.h"

#include "ricpp/ri/ri.h"

#include <iostream>

using namespace AB;

ABLoaderPlugin *ABLoaderPlugin::plugin_instance = 0;
// RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator> *ABLoaderPlugin::abLoaderFactory;

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
	}
	return plugin_instance;
}

void ABLoaderPlugin::deleteInstance(ABLoaderPlugin *plugin)
{
	if ( plugin && plugin != plugin_instance ) {
		delete plugin;
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
	RiBegin("abloader");
	RiReadArchive(filename.c_str(), 0, RI_NULL);
	RiEnd();
}

extern "C" AB::Plugin* Instance()
{
	return ABLoaderPlugin::MyInstance();
}
