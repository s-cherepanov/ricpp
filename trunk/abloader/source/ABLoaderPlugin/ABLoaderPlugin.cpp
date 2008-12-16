/*
 *  ABLoaderPlugin.cpp
 *  ABLoaderPlugin
 *
 *  Created by Andreas Pidde on 05.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "abloader/ABLoaderPlugin/ABLoaderPlugin.h"
#include "abloader/abloader/abloader.h"

#pragma GCC visibility push(hidden)

#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#include "ricpp/ricppbridge/ricppbridge.h"
#endif // _RICPP_RICPP_RICPPBRIDGE_H
#include <iostream>

#pragma GCC visibility pop

static RiCPP::CRiCPPBridge *ri = 0; ///< The bridge to the rendering context
static RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator> *abLoaderFactory = 0;

using namespace AB;


static void SetRoot()
{
	if ( ri == 0 ) {
		ri = new RiCPP::CRiCPPBridge;
		if ( ri ) {
			// Register abloader 'renderer' -  can also be omitted and to load libabloader.1.dylib dynamically
			// for static use link module with libabloader.a for dynamic use link with libricppbridge.a
			abLoaderFactory = new RiCPP::TemplPluginFactory<RiCPP::CABLoaderCreator>;
			if ( abLoaderFactory ) {
				ri->registerRendererFactory("abribloader", reinterpret_cast<RiCPP::TemplPluginFactory<RiCPP::CContextCreator> *>(abLoaderFactory));
			}
			
			// const char *searchPath = "/usr/local/lib/Augenblick:&";
			// ri->control("searchpath", "renderer", &searchPath, RI_NULL);
			
			// const int cache[2] = {0, 1};
			// ri->control("rib", "cache-file-archives", &(cache[0]), RI_NULL);
		}
	}
}

static void UnsetRoot()
{
	if ( ri ) {
		delete ri;
	}
	ri = 0;
	abLoaderFactory = 0;
}

ABLoaderPlugin *ABLoaderPlugin::plugin_instance = 0;

ABLoaderPlugin::ABLoaderPlugin()
{
}

ABLoaderPlugin::~ABLoaderPlugin()
{
}

ABLoaderPlugin* ABLoaderPlugin::MyInstance()
{
	if ( plugin_instance == 0 ) {
		SetRoot();
		plugin_instance = new ABLoaderPlugin;
	}
	return plugin_instance;
}

void ABLoaderPlugin::deleteInstance(ABLoaderPlugin *plugin)
{
	if ( plugin && plugin == plugin_instance ) {
		UnsetRoot();
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
	
	ri->begin("abribloader"); {
		ri->readArchive(filename.c_str(), 0, RI_NULL);
	} ri->end();
	AB::Augenblick::Instance()->requestNewRendering();
}

extern "C" AB::Plugin* Instance()
{
	return ABLoaderPlugin::MyInstance();
}
