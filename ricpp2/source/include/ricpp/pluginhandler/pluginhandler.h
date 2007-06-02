#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#define _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

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

/** @file pluginhandler.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief General handler for plugins
 */

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "ricpp/tools/objptrregistry.h"
#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/RICPP/renderererror.h"
#endif // _RICPP_RICPP_RENDERERERROR_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

namespace RiCPP {

/** @brief Minimal interface a plugin must implement.
 *
 *  This is the minimal interface, a plugin must implement to be
 *  used by TPluginFactory, TPluginLoaderFactory and TPluginHandler.
 *  Classes need not to inherit from this class, it's only for convenience.
 *  Also the static functions:
 *
 *  - static unsigned long myMajorVersion();
 *  - static unsigned long myMinorVersion();
 *  - static unsigned long myRevision();
 *  - static const char *myType();
 *  - static const char *myName();
 *
 *  are needed (same meaning as the virtuals without the prefix 'my').
 */
class IPlugin {
public:
	/** @brief virtual destructor
	 */
	inline virtual ~IPlugin() {}

	/** @brief Major version number of the plugin interface.
	 *
	 * A plugin must implement the major version to work correctly. The
	 * major version of the plugin in the used interface must be equal.
	 *
	 * @return Major version number of the plugin.
	 */
	virtual unsigned long majorVersion() = 0;

	/** @brief Minor version number of the plugin interface.
	 *
	 * Minor version changes reflect changes in the functionality of
	 * the plugin, but no changes did not affect the interface.
	 *
	 * @return Minor version number of the plugin.
	 */
	virtual unsigned long minorVersion() = 0;

	/** @brief Revision number of the plugin interface.
	 *
	 * Simple revisions that do not change the functionality.
	 *
	 * @return Revision number of the plugin
	 */
	virtual unsigned long revision() = 0;

	/** @brief Type of the plugin for grouping the plugins somehow.
	 * @return Type of the plugin
	 */
	virtual const char *type() = 0;

	/** @brief Name of the plugin and factory library.
	 *
	 * @return Name of the plugin and plugin library.
	 */
	virtual const char *name() = 0;

	/** @brief Called after the plugin is created.
	 */
	virtual void startup() = 0;

	/** @brief Called before the plugin is deleted.
	 */
	virtual void shutdown() = 0;
}; // class IPlugin


/** @brief Plugin factory to create plugins in memory.
 *
 *  Can be used to statically create plugins, no dynamic libraries are interfered.
 */
template
<class Plugin>
class TPluginFactory
{
protected:
	/** @brief Registry of created plugins.
	 *
	 * Plugins are also deleted if the factory is deleted if the
	 * destructMembers parameter in TPluginFactory() was true.
	 */
	TObjPtrRegistry<Plugin *, Plugin *> m_pluginRegistry;

	/** @brief Pointer to the last Plugin created.
	 *
	 *  Can be used to use plugins as singletons.
	 *  @see lastPlugin()
	 */
	Plugin *m_lastPlugin;
public:

	/** @brief Constructor of the factory.
	 *
	 *  @param destructMembers true, to handle deletion of the plugins.
	 *  Should be normally true if the object is used, exception TPluginLoaderFactory.
	 */
	inline TPluginFactory(bool destructMembers = true)
		: m_pluginRegistry(destructMembers)
	{
		m_lastPlugin = 0;
	}

	/** @brief Virtual destructor.
	 */
	inline virtual ~TPluginFactory()
	{
	}

	/** @brief Major version number of the plugin.
	 *  @return Major version.
	 *  @see IPlugin::majorVersion()
	 */
	inline virtual unsigned long majorVersion() {return Plugin::myMajorVersion(); }

	/** @brief Minor version number of the plugin.
	 *  @return Minor version number.
	 *  @see IPlugin::minorVersion()
	 */
	inline virtual unsigned long minorVersion() {return Plugin::myMinorVersion(); }

	/** @brief Revision number of the plugin.
	 *  @return Revision number.
	 *  @see IPlugin::revision()
	 */
	inline virtual unsigned long revision() {return Plugin::myRevision(); }

	/** @brief Type name of the plugin.
	 *  @return Type name of the plugin.
	 *  @see IPlugin::type()
	 */
	inline virtual const char *type() { return Plugin::myType(); }

	/** @brief Name of the plugin.
	 *  @return Name of the plugin.
	 *  @see IPlugin::name()
	 */
	inline virtual const char *name() { return Plugin::myName(); }

	/** @brief Create a new plugin in memory.
	 *
	 *  The startup() of the plugon is called after creation
	 *
	 *  @return The new plugin.
	 *  @exception ERiCPPError
	 */
	inline virtual Plugin *newPlugin()
	{ 
		Plugin *p = 0;
		try {
			p = new Plugin;
		} catch ( ... ) {
			// p could not be created
		}
		if ( !p ) {
			throw ERiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create a plugin \"%s\"", markemptystr(name()));
		}

		if ( p ) {
			m_lastPlugin = p;
			m_pluginRegistry.registerObj(p, p);
			p->startup();
		}
		return p;
	}

	/** @brief Delete a plugin.
	 *
	 *  The plugin is deleted if it is registerd (created by newPlugin()).
	 *  The shutdown() of the plugin is called before deletion.
	 *  @return true, plugin is deleted.
	 */
	inline virtual bool deletePlugin(Plugin *p)
	{
		if ( p && m_pluginRegistry.findObj(p) ) {
			p->shutdown();
			if ( m_lastPlugin == p )
				m_lastPlugin = 0;
			m_pluginRegistry.unRegisterObj(p);
			if ( !m_pluginRegistry.membersAreDestructed() )
				delete p;
		}
		return false;
	}

	/** @brief Plugin registered by this factory?
	 *
	 *  @param p Pointer of plugin to look for
	 *  @return true, plugin is registerd.
	 */
	inline virtual bool isRegistered(Plugin *p)
	{
		return m_pluginRegistry.findObj(p) != 0;
	}

	/** @brief Last plugin created
	 *
	 * Creates a new one if no plugin was created or the last created was deleted.
	 *
	 *  @return The last plugin created.
	 */
	inline virtual Plugin *lastPlugin()
	{
		if ( !m_lastPlugin )
			return newPlugin();
		return m_lastPlugin;
	}

	/** @brief Is the object valid?
	 *
	 *  This is always true, needed for classes that inherit from TPluginFactory,
	 *  i.e. TPluginLoaderFactory::valid()
	 *
	 *  @return true, the object ist valid.
	 */
	inline virtual bool valid() {
		return true;
	}
}; // template class TPluginFactory


/** @brief Plugin factory to create plugins out of libraries.
 *
 *  Can be used to create plugins that are stored in danamic libraries.
 */
template
<class Plugin>
class TPluginLoaderFactory : public TPluginFactory<Plugin>
{
private:
	std::string m_libName;        ///< Name of the plugin (library).
	std::string m_searchPath;     ///< Searchpath to find the dynamic library.

	CDynLib  *m_lib;              ///< Library of the plugin.
	ILibFunc *m_funcNewPlugin;    ///< Gets a new plugin.
	ILibFunc *m_funcDeletePlugin; ///< Deletes a plugin (created by m_newPlugin() of the same library).
	ILibFunc *m_funcMajorVersion; ///< Gets the major plugin version.
	ILibFunc *m_funcMinorVersion; ///< Gets the minor version of a plugin.
	ILibFunc *m_funcRevision;     ///< Gets the revision of a plugin.
	ILibFunc *m_funcName;         ///< Gets the name of a plugin (should be == libname).
	ILibFunc *m_funcType;         ///< Gets the type of a plugin.

	typedef Plugin *(CDECL *TypeNewPlugin)(unsigned long); ///< Gets a new plugin (Type of library function for m_funcNewPlugin).
	typedef void (CDECL *TypeDeletePlugin)(Plugin *);      ///< Deletes a plugin (Type of library function m_funcDeletePlugin).
	typedef unsigned long (CDECL *TypeMajorVersion)(void); ///< Gets the major plugin version (Type of library function m_funcMajorVersion).
	typedef unsigned long (CDECL *TypeMinorVersion)(void); ///< Gets the minor version of a plugin (Type of library function m_funcMinorVersion).
	typedef unsigned long (CDECL *TypeRevision)(void);     ///< Gets the revision of a plugin (Type of library function m_funcRevision).
	typedef const char *(CDECL *TypeType)(void);           ///< Gets the name of a plugin (Type of library function m_funcName).
	typedef const char *(CDECL *TypeName)(void);           ///< Gets the type of a plugin (Type of library function m_funcType).
public:

	/** @brief Constructor to load a dynamic library.
	 *
	 * The destruction of the plugins is handled by a library function,
	 * therefore \a destructMembers of TPluginFactory is set to false.
	 *
	 * @param libname Basename of the library
	 * @param pathlist Pathlist (with variables) to search for the library
	 * @see CDynLibFactory, CDynLib
	 * @exception ERiCPPError
	 */
	inline TPluginLoaderFactory(const char *libname, const char *pathlist = 0)
		: TPluginFactory(false)
	{
		m_funcNewPlugin = 0;
		m_funcDeletePlugin = 0;
		m_funcMajorVersion = 0;
		m_funcMinorVersion = 0;
		m_funcRevision = 0;
		m_funcType = 0;
		m_funcName = 0;
		m_lib = 0;

		m_libName = nonullstr(libname);
		m_searchPath = nonullstr(pathlist);

		// Create the library handler
		try {
			m_lib = CDynLibFactory::newDynLib(m_libName.c_str(), m_searchPath.c_str(), Plugin::myMajorVersion());
		} catch ( ... ) {
			// m_lib not created
		}
		if ( !m_lib ) {
			throw ERiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create a new CDynLib for plugin factory \"%s\"", markemptystr(libname));
		}

		// Load the library
		try {
			m_lib->load();
		} catch ( ERiCPPError &err ) {
			CDynLibFactory::deleteDynLib(m_lib);
			throw err;
		}

		// Get the function pointer
		try {
			m_funcNewPlugin = m_lib->getFunc("newPlugin");
			m_funcDeletePlugin = m_lib->getFunc("deletePlugin");
			m_funcMajorVersion = m_lib->getFunc("majorVersion");
			m_funcMinorVersion = m_lib->getFunc("minorVersion");
			m_funcRevision = m_lib->getFunc("revision");
			m_funcType = m_lib->getFunc("type");
			m_funcName = m_lib->getFunc("name");
		} catch ( ... ) {
			// One of the funcs is not created
		}
		if ( !valid() ) {
			throw ERiCPPError(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create the library functions for plugin factory \"%s\"", markemptystr(libname));
		}
	}

	/** @brief Destructor removes the library.
	 **/
	inline virtual ~TPluginLoaderFactory()
	{
		TObjPtrRegistry<Plugin *, Plugin *>::const_iterator it;
		for ( it = m_pluginRegistry.begin(); it != m_pluginRegistry.end(); ++it ) {
			delete it->second;
		}
		if ( m_lib ) {
			m_lib->deleteFunc(m_funcNewPlugin);
			m_lib->deleteFunc(m_funcDeletePlugin);
			m_lib->deleteFunc(m_funcMajorVersion);
			m_lib->deleteFunc(m_funcMinorVersion);
			m_lib->deleteFunc(m_funcRevision);
			m_lib->deleteFunc(m_funcType);
			m_lib->deleteFunc(m_funcName);
			CDynLibFactory::deleteDynLib(m_lib);
		}
	}

	inline virtual unsigned long majorVersion()
	{
		return ((TypeMajorVersion)(m_funcMajorVersion->funcPtr()))();
	}
	inline virtual unsigned long minorVersion()
	{
		return ((TypeMinorVersion)(m_funcMinorVersion->funcPtr()))();
	}
	inline virtual unsigned long revision()
	{
		return ((TypeRevision)(m_funcRevision->funcPtr()))();
	}
	inline virtual const char *type()
	{
		return ((TypeType)(m_funcType->funcPtr()))();
	}
	inline virtual const char *name()
	{
		return ((TypeName)(m_funcName->funcPtr()))();
	}
	inline virtual const char *searchPath()
	{
		return m_searchPath.c_str();
	}

	inline virtual Plugin *newPlugin()
	{ 
		Plugin *p = 0;

		p = ((TypeNewPlugin)(m_funcNewPlugin->funcPtr()))(Plugin::myMajorVersion());
		if ( p ) {
			m_lastPlugin = p;
			m_pluginRegistry.registerObj(p, p);
			p->startup();
		}
		return p;
	}

	inline virtual bool deletePlugin(Plugin *p)
	{
		if ( p && m_pluginRegistry.findObj(p) ) {
			p->shutdown();
			m_pluginRegistry.unRegisterObj(p);
			((TypeDeletePlugin)(m_funcDeletePlugin->funcPtr()))(p);
			if ( m_lastPlugin == p )
				m_lastPlugin = 0;
			return true;
		}
		return false;
	}

	inline virtual Plugin *lastPlugin()
	{
		return TPluginFactory<Plugin>::lastPlugin();
	}

	inline virtual bool isRegistered(Plugin *p)
	{
		return TPluginFactory<Plugin>::isRegistered(p);
	}

	inline virtual bool valid() {
		if ( !m_lib || !m_lib->valid() )
			return false;

		if ( !m_funcNewPlugin || !m_funcNewPlugin->valid() )
			return false;
		if ( !m_funcDeletePlugin || !m_funcDeletePlugin->valid() )
			return false;
		if ( !m_funcMajorVersion || !m_funcMajorVersion->valid() )
			return false;
		if ( !m_funcMinorVersion || !m_funcMinorVersion->valid() )
			return false;
		if ( !m_funcRevision || !m_funcRevision->valid() )
			return false;
		if ( !m_funcType || !m_funcType->valid() )
			return false;
		if ( !m_funcName || !m_funcName->valid() )
			return false;

		if ( Plugin::myMajorVersion() != majorVersion() )
			return false;

		return true;
	}
}; // template class TPluginLoaderFactory

/** @brief Handles the plugins for a specific type.
 *
 *  Plugins can be loaded from a dynamic library or created by registered factories.
 */
template
<class Plugin>
class TPluginHandler
{
protected:
	/** @brief Registry for the plugin factories.
	 */
	TObjPtrRegistry<std::string, TPluginFactory<Plugin> *> m_factoryRegistry;
	/** @brief Searchpath to find the dynamic libraries for the plugins.
	 */
	std::string m_searchPath;

	/** @brief Get and register a plugin library
	 *  @param name Basename of the plugin library
	 *  @return Factory object for the plugin
	 */
	TPluginFactory<Plugin> *getFactory(const char *name)
	{
		std::string key(nonullstr(name));
		TPluginFactory<Plugin> *f = m_factoryRegistry.findObj(key);
		if ( !f ) {
			try {
				f = new TPluginLoaderFactory<Plugin>(name, m_searchPath.c_str());
			} catch ( ERiCPPError &e ) {
				throw e;
			} catch ( ... ) {
				// could not create TPluginLoaderFactory
			}
			if ( !f )
				throw ERiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__ "Cannot create a new plugin factory for \"%s\"", markemptystr(name));
		}
		m_factoryRegistry.registerObj(key, f);
		return f;
	}

public:

	/** @brief Constructor
	 *  @param pathlist Pathlist to find the libraries
	 */
	inline TPluginHandler(const char *pathlist = 0)
	: m_factoryRegistry(true),
	  m_searchPath(nonullstr(pathlist))
	{
	}

	/** @brief Virtual destructor
	 */
	inline virtual ~TPluginHandler()
	{
	}

	/** @brief Gets a new plugin
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 */
	inline virtual Plugin *newPlugin(const char *name)
	{
		return getFactory(name)->newPlugin();
	}

	/** @brief Gets the last plugin created
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 * @see TPluginFactory::lastPlugin()
	 */
	inline virtual Plugin *lastPlugin(const char *name)
	{
		return getFactory(name)->lastPlugin();
	}

	/** @brief Deletes a plugin.
	 *
	 * @param p Plugin to delete
	 * @return true, if the plugin could be deleted
	 */
	inline virtual bool deletePlugin(Plugin *p)
	{
		if ( !p )
			return false;
		std::string key(nonullstr(p->myName()));
		TPluginFactory<Plugin> *f = m_factoryRegistry.findObj(key);
		if ( !f ) {
			return false;
		}
		return f->deletePlugin(p);
	}

	/** @brief Registers a plugin factory
	 *
	 *  Registers a plugin factory for a specific name. Normally
	 *  TPluginFactory are registered to create specific plugins with
	 *  \c new instead of loading them from a dynamic library.
	 *
	 * @param name Name of the plugins
	 * @param f Factory to create the plugins
	 * @return true, if the plugin factory could be registerd
	 */
	inline virtual bool registerFactory(const char *name, TPluginFactory<Plugin> *f)
	{
		std::string key(nonullstr(name));
		if ( key.empty() || !f ) {
			return false;
		}
		return m_factoryRegistry.registerObj(key, f);
	}
}; // template class TPluginHandler

} // namespace RiCPP

#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
