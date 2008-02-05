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

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

#include <iostream>
#include <sstream>

namespace RiCPP {

/** @brief Minimal interface a plugin must implement.
 *
 *  This is the minimal interface, a plugin must implement to be
 *  used by TemplPluginFactory, TemplPluginLoaderFactory and TemplPluginHandler.
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
 *
 */
class IPlugin {
public:
	/** @brief Standard Constructor
	 */
	IPlugin();

	/** @brief Virtual destructor
	 */
	virtual ~IPlugin();

	/** @brief Major version number of the plugin interface.
	 *
	 * A plugin must implement the major version to work correctly. The
	 * major version of the plugin and the used interface must be equal.
	 * The major version is compared while loding the plugin. If the
	 * version do not match to the version of the linkted interface
	 * the plugin is not created.
	 *
	 * @return Major version number of the plugin.
	 */
	virtual unsigned long majorVersion() const = 0;

	/** @brief Minor version number of the plugin interface.
	 *
	 * Minor version changes reflect changes in the functionality of
	 * the plugin, but does not affect the interface.
	 *
	 * @return Minor version number of the plugin.
	 */
	virtual unsigned long minorVersion() const = 0;

	/** @brief Revision number of the plugin interface.
	 *
	 * Simple revisions (e.g. bug fixes) that do not change the functionality.
	 *
	 * @return Revision number of the plugin
	 */
	virtual unsigned long revision() const = 0;

	/** @brief Type of the plugin for grouping the plugins.
	 *
	 * The type of the loaded plugin and the linked interface must match (case sensitive),
	 * otherwise the plugin is not loaded. E.g. you cannot load a renderer as ribfilter.
	 *
	 * @return Type of the plugin
	 */
	virtual const char *type() const = 0;

	/** @brief Name of the plugin and factory library.
	 *
	 * The name of the plugin for reference.
	 *
	 * @return Name of the plugin and plugin library.
	 */
	virtual const char *name() const = 0;

	/** @brief Called after the plugin is created.
	 *
	 * You can do some initialization if you implement the startup()
	 * function of a plugin.
	 */
	virtual void startup() = 0;

	/** @brief Called before the plugin is deleted.
	 *
	 * Cleanup can be made in an implemented shutdown() function.
	 */
	virtual void shutdown() = 0;
}; // class IPlugin


/** @brief Plugin factory to create plugins in memory.
 *
 * Can be used to create plugins on the heap, no dynamic libraries are involved.
 * E.g. the ribwriter will be created on the heap, so it can be assumed that
 * it works without having to access a library.
 */
template
<class Plugin>
class TemplPluginFactory
{
protected:
	/** @brief Registry of created plugins.
	 *
	 * If the destructMembers parameter in TemplPluginFactory() was true,
	 * plugins registered are deleted automatically, if the factory is deleted.
	 * \a key and \a value is the same pointer, m_pluginRegistry is used
	 * to find registerd pointers quickly.
	 */
	TemplObjPtrRegistry<Plugin *, Plugin *> m_pluginRegistry;

	/** @brief Pointer to the last plugin created.
	 *
	 * Can be used to use a plugin as a singleton, only if only lastPlugin() is used to
	 * create plugins.
	 *
	 * @see lastPlugin()
	 */
	Plugin *m_lastPlugin;
public:

	/** @brief Constructor of the factory.
	 *
	 * \a destructMembers should be set true if the object shall be
	 * deleted automatically, while the TPluginFactor is deleted.
	 * This parameter does not affect deletePlugin().
	 *
	 * @param destructMembers true, to handle deletion of the plugins.
	 */
	inline TemplPluginFactory(bool destructMembers = true)
		: m_pluginRegistry(destructMembers)
	{
		m_lastPlugin = 0;
	}

	/** @brief Virtual destructor.
	 */
	inline virtual ~TemplPluginFactory()
	{
	}

	/** @brief Major version number of the plugin.
	 *
	 * The major version number of the interface and the created plugin have to be equal.
	 * This is the case for all heap objects, but must be tested if the object is loaded
	 * from a dynamic library.
	 *
	 * @return Major version number of the linked interface.
	 * @see IPlugin::majorVersion()
	 */
	inline virtual unsigned long majorVersion() const
	{
		return Plugin::myMajorVersion();
	}

	/** @brief Minor version number of the plugin.
	 *
	 * In the case of a plugin created on the heap, the minor version of the plugin and
	 * the linked interface are naturally the same. That need not to be the case if the
	 * plugin is loaded from a dynamic library.
	 *
	 * @return Minor version number of the plugin.
	 * @see IPlugin::minorVersion()
	 */
	inline virtual unsigned long minorVersion() const
	{
		return Plugin::myMinorVersion();
	}

	/** @brief Revision number of the plugin.
	 *
	 * In the case of a plugin created on the heap, the revision number of the plugin and
	 * the linked interface are naturally the same. That need not to be the case if the
	 * plugin is loaded from a dynamic library.
	 *
	 *  @return Revision number of the plugin.
	 *  @see IPlugin::revision()
	 */
	inline virtual unsigned long revision() const
	{
		return Plugin::myRevision();
	}

	/** @brief Type name of the plugin.
	 *
	 * The type name of the interface and the created plugin have to be equal (case sensitive).
	 * This is the case for all heap objects , but must be tested if the object is loaded from
	 * a dynamic library.
	 *
	 * @return Type name of the plugin.
	 * @see IPlugin::type()
	 */
	inline virtual const char *type() const
	{
		return Plugin::myType();
	}

	/** @brief Name of the plugin.
	 *
	 * In the case of a plugin created on the heap, the name of the plugin and
	 * the linked interface are naturally the same. That need not to be the case if the
	 * plugin is loaded from a dynamic library.
	 *
	 * @return Name of the plugin.
	 * @see IPlugin::name()
	 */
	inline virtual const char *name() const
	{
		return Plugin::myName();
	}

	/** @brief Create a new plugin in memory.
	 *
	 * Use \c new to create a new instance of the plugin. The startup() of this
	 * plugin is called after creation. The plugin must be deleted by deletePlugin(),
	 * or automatically by ~TemplPluginFactory(), if the \a destructMembers parameter of
	 * the constructor was set to true.
	 *
	 *  @return The new plugin.
	 *  @exception ExceptRiCPPError
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
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__,
				"Cannot create a plugin '%s'",
				markEmptyStr(name()));
		}

		m_lastPlugin = p;
		m_pluginRegistry.registerObj(p, p);
		p->startup();

		return p;
	}

	/** @brief Delete a plugin.
	 *
	 *  The plugin is deleted if it is registerd (created by newPlugin()).
	 *  The shutdown() of the plugin is called before deletion.
	 *
	 *  @return true, plugin is deleted.
	 */
	inline virtual bool deletePlugin(Plugin *p)
	{
		if ( p && m_pluginRegistry.findObj(p) ) {
			p->shutdown();
			if ( m_lastPlugin == p )
				m_lastPlugin = 0;
			m_pluginRegistry.unregisterObj(p);
			if ( !m_pluginRegistry.membersAreDestructable() )
				delete p;
		}
		return false;
	}

	/** @brief Plugin registered by this factory?
	 *
	 *  @param p Pointer of plugin to look for
	 *  @return true, plugin is registered.
	 */
	inline virtual bool isRegistered(Plugin *p) const
	{
		return m_pluginRegistry.findObj(p) != 0;
	}

	/** @brief Last plugin created
	 *
	 * Creates a new plugin if none was created before or the last created was deleted.
	 * If only lastPlugin() (no call of newPlugin() and deletePlugin() directly) is used,
	 * only one plugin will be created.
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
	 * This is always true for TemplPluginFactory, needed for classes that
	 * inherit from TemplPluginFactory, i.e. TemplPluginLoaderFactory::valid()
	 * tests if the library could be loaded correctly and the major
	 * version number and type string matches the ones of the
	 * linked interface.
	 *
	 * @return true, if the object is valid.
	 */
	inline virtual bool valid() const
	{
		return true;
	}
}; // template class TemplPluginFactory


/** @brief Plugin factory to create plugins loaded from dynamic libraries.
 *
 * Can be used to create plugins that are stored in danamic libraries.
 */
template
<class Plugin>
class TemplPluginLoaderFactory : public TemplPluginFactory<Plugin>
{
private:
	std::string m_libName;        ///< Name of the plugin (library).
	std::string m_searchpath;     ///< Searchpath to find the dynamic library.

	CDynLib  *m_lib;              ///< Library of the plugin.
	ILibFunc *m_funcNewPlugin;    ///< Gets a new plugin.
	ILibFunc *m_funcDeletePlugin; ///< Deletes a plugin (created by m_newPlugin() of the same library).
	ILibFunc *m_funcMajorVersion; ///< Gets the major plugin version.
	ILibFunc *m_funcMinorVersion; ///< Gets the minor version of a plugin.
	ILibFunc *m_funcRevision;     ///< Gets the revision of a plugin.
	ILibFunc *m_funcName;         ///< Gets the name of a plugin (should be == libname).
	ILibFunc *m_funcType;         ///< Gets the type of a plugin.

	/** @brief Gets a new plugin (Type of library function for m_funcNewPlugin).
	 */
	typedef Plugin *(CDECL *TypeNewPlugin)(unsigned long, const char *);

	/** @brief Deletes a plugin (Type of library function m_funcDeletePlugin).
	 */
	typedef void (CDECL *TypeDeletePlugin)(Plugin *);

	/** @brief Gets the major version of the plugin (Type of library function m_funcMajorVersion).
	 */
	typedef unsigned long (CDECL *TypeMajorVersion)(void);

	/** @brief Gets the minor version of a plugin (Type of library function m_funcMinorVersion).
	 */
	typedef unsigned long (CDECL *TypeMinorVersion)(void);

	/** @brief Gets the revision of a plugin (Type of library function m_funcRevision).
	 */
	typedef unsigned long (CDECL *TypeRevision)(void);

	/** @brief Gets the name of a plugin (Type of library function m_funcName).
	 */
	typedef const char *(CDECL *TypeType)(void);

	/** @brief Gets the type of a plugin (Type of library function m_funcType).
	 */
	typedef const char *(CDECL *TypeName)(void);
public:

	/** @brief Constructor to load a dynamic library.
	 *
	 * The destruction of the plugins is handled by a library function,
	 * therefore \a destructMembers of TemplPluginFactory is set to false.
	 *
	 * @param libname Basename of the library
	 * @param pathlist Pathlist (optionally with $-variables) to search for the library
	 * @see CDynLibFactory, CDynLib
	 * @exception ExceptRiCPPError
	 */
	inline TemplPluginLoaderFactory(
		const char *libname,
		const char *pathlist = 0
		) : TemplPluginFactory<Plugin>(false)
	{
		m_funcNewPlugin = 0;
		m_funcDeletePlugin = 0;
		m_funcMajorVersion = 0;
		m_funcMinorVersion = 0;
		m_funcRevision = 0;
		m_funcType = 0;
		m_funcName = 0;
		m_lib = 0;

		m_libName = noNullStr(libname);
		m_searchpath = noNullStr(pathlist);

		// Create the library handler
		try {
			m_lib = CDynLibFactory::newDynLib(m_libName.c_str(),
				m_searchpath.c_str(), Plugin::myMajorVersion());
		} catch ( ... ) {
			// m_lib not created
		}
		if ( !m_lib ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__,
				"Cannot create a new CDynLib for plugin factory '%s'",
				markEmptyStr(libname));
		}

		// Load the library
		try {
			m_lib->load();
		} catch ( ExceptRiCPPError &err ) {
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
			m_lib->unload();
			throw ExceptRiCPPError(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__,
				"Cannot create the library functions or version mismatch for plugin factory '%s'",
				markEmptyStr(libname));
		}
	}

	/** @brief Destructor removes the library.
	 *
	 * Deletes loaded plugins, functions and library (gets closed).
	 *
	 */
	inline virtual ~TemplPluginLoaderFactory()
	{
		typename TemplObjPtrRegistry<Plugin *, Plugin *>::const_iterator it;
		for ( it = TemplPluginFactory<Plugin>::m_pluginRegistry.begin();
		      it != TemplPluginFactory<Plugin>::m_pluginRegistry.end();
			  ++it )
		{
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

	inline virtual unsigned long majorVersion() const
	{
		return ((TypeMajorVersion)m_funcMajorVersion->funcPtr())();
	}

	inline virtual unsigned long minorVersion() const
	{
		return ((TypeMinorVersion)m_funcMinorVersion->funcPtr())();
	}

	inline virtual unsigned long revision() const
	{
		return ((TypeRevision)m_funcRevision->funcPtr())();
	}
	
	inline virtual const char *type() const
	{
		return ((TypeType)m_funcType->funcPtr())();
	}
	
	inline virtual const char *name() const
	{
		return ((TypeName)m_funcName->funcPtr())();
	}
	
	inline virtual const char *searchpath() const
	{
		return m_searchpath.c_str();
	}

	inline virtual Plugin *newPlugin()
	{ 
		Plugin *p = 0;

		p = ((TypeNewPlugin)m_funcNewPlugin->funcPtr())(
			Plugin::myMajorVersion(), Plugin::myType());
		if ( p ) {
			TemplPluginFactory<Plugin>::m_lastPlugin = p;
			TemplPluginFactory<Plugin>::m_pluginRegistry.registerObj(p, p);
			p->startup();
		} else {
			throw ExceptRiCPPError(RIE_VERSION, RIE_SEVERE, __LINE__, __FILE__,
				"Plugin of wrong version or type '%s'",
				markEmptyStr(m_libName.c_str()));
		}
		return p;
	}

	inline virtual bool deletePlugin(Plugin *p)
	{
		if ( p && TemplPluginFactory<Plugin>::m_pluginRegistry.findObj(p) ) {
			p->shutdown();
			TemplPluginFactory<Plugin>::m_pluginRegistry.unregisterObj(p);
			((TypeDeletePlugin)m_funcDeletePlugin->funcPtr())(p);
			if ( TemplPluginFactory<Plugin>::m_lastPlugin == p )
				TemplPluginFactory<Plugin>::m_lastPlugin = 0;
			return true;
		}
		return false;
	}

	inline virtual Plugin *lastPlugin()
	{
		return TemplPluginFactory<Plugin>::lastPlugin();
	}

	inline virtual bool isRegistered(Plugin *p) const
	{
		return TemplPluginFactory<Plugin>::isRegistered(p);
	}

	inline virtual bool valid() const
	{
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
			
		if ( strcmp(noNullStr(Plugin::myType()), noNullStr(type())) != 0 )
			return false;

		return true;
	}
}; // template class TemplPluginLoaderFactory

/** @brief Handles the plugins for a specific type.
 *
 * Plugins can be loaded from a dynamic library or created by registered factories.
 */
template
<class Plugin>
class TemplPluginHandler
{
protected:
	/** @brief Registry for the plugin factories.
	 */
	TemplObjPtrRegistry<std::string, TemplPluginFactory<Plugin> *> m_factoryRegistry;
	
	/** @brief Memory managed plugin factories.
	 */
	TemplObjPtrRegistry<std::string, TemplPluginFactory<Plugin> *> m_internalFactoryRegistry;

	/** @brief Memory all plugin (pointers only) factories to iterate
	 */
	TemplObjPtrRegistry<std::string, TemplPluginFactory<Plugin> *> m_allFactoryRegistry;

	/** @brief Searchpath to find the dynamic libraries for the plugins.
	 */
	std::string m_searchpath;

	/** @brief Block the loading of factories from dynamic libraries.
	 */
	bool m_blockLoad;

	/** @brief Get and register a plugin library
	 *  @param name Basename of the plugin library
	 *  @return Factory object for the plugin
	 */
	inline virtual TemplPluginFactory<Plugin> *getFactory(const char *name)
	{
		std::string key(noNullStr(name));

		TemplPluginFactory<Plugin> *f = m_factoryRegistry.findObj(key);
		if ( !f ) {
			f = m_internalFactoryRegistry.findObj(key);
		}
		if ( !f ) {
			if ( m_blockLoad )
				return 0;

			try {
				f = new TemplPluginLoaderFactory<Plugin>(key.c_str(), m_searchpath.c_str());
			} catch ( ExceptRiCPPError &e ) {
				throw e;
			} catch ( ... ) {
				// could not create TemplPluginLoaderFactory
			}
			if ( !f )
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__,
					"Cannot create a new plugin factory for '%s'",
					markEmptyStr(key.c_str()));

			if ( Plugin::myMajorVersion() != f->majorVersion() ) {
				// Should not happen, since already tested using valid()
				// in the constructor of TemplPluginLoaderFactory
				delete f;
				throw ExceptRiCPPError(RIE_VERSION, RIE_SEVERE, __LINE__, __FILE__,
					"Plugin Factory is of wrong version '%ld' vs. '%ld' - not loaded",
					Plugin::myMajorVersion(), f->majorVersion());
			}

			if ( strcmp(noNullStr(Plugin::myType()), noNullStr(f->type())) != 0 ) {
				// Should not happen, since already tested using valid()
				// in the constructor of TemplPluginLoaderFactory
				delete f;
				throw ExceptRiCPPError(RIE_BADFILE, RIE_SEVERE, __LINE__, __FILE__,
					"Plugin Factory is of wrong type '%s' vs. '%s' - not loaded",
					noNullStr(Plugin::myType()), noNullStr(f->type()));
			}

			m_internalFactoryRegistry.registerObj(key, f);
			m_allFactoryRegistry.registerObj(key, f);
		} else {
			if ( Plugin::myMajorVersion() != f->majorVersion() ) {
				// Should not happen, since already tested
				throw ExceptRiCPPError(RIE_VERSION, RIE_SEVERE, __LINE__, __FILE__,
					"Plugin Factory is of wrong version '%ld' vs. '%ld' - already loaded",
					Plugin::myMajorVersion(), f->majorVersion());
			}
			if ( strcmp(noNullStr(Plugin::myType()), noNullStr(f->type())) != 0 ) {
				// Should not happen, since already tested
				throw ExceptRiCPPError(RIE_BADFILE, RIE_SEVERE, __LINE__, __FILE__,
					"Plugin Factory is of wrong type '%s' vs. '%s' - already loaded",
					noNullStr(Plugin::myType()), noNullStr(f->type()));
			}
		}
		return f;
	}

public:
	/** @brief Const iterator for the elements.
	 */
	typedef
		typename
		TemplObjPtrRegistry<std::string, TemplPluginFactory<Plugin> *>::const_iterator
		const_iterator;

	/** @brief Size type for the number of stored elements
	 */
	typedef
		typename
		TemplObjPtrRegistry<std::string, TemplPluginFactory<Plugin> *>::size_type
		size_type;

	/** @brief Constructor
	 *  @param pathlist Pathlist to find the libraries
	 */
	inline TemplPluginHandler(const char *pathlist = 0)
	: m_factoryRegistry(false),
	  m_internalFactoryRegistry(true),
	  m_allFactoryRegistry(false),
	  m_searchpath(noNullStr(pathlist)),
	  m_blockLoad(false)
	{
	}

	/** @brief Virtual destructor
	 */
	inline virtual ~TemplPluginHandler()
	{
	}

	/** @brief Gets a new plugin
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 * @return The loaded plugin
	 * @exception ERiCPPException Could not load plugin or types do not match.
	 */
	inline virtual Plugin *newPlugin(const char *name)
	{
		return getFactory(name)->newPlugin();
	}

	/** @brief Gets the last plugin created
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 * @return The loaded plugin loaded lately
	 * @see TemplPluginFactory::lastPlugin()
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
		std::string key(noNullStr(p->myName()));
		TemplPluginFactory<Plugin> *f = m_factoryRegistry.findObj(key);
		if ( !f ) {
			return false;
		}
		return f->deletePlugin(p);
	}

	/** @brief Registers a plugin factory.
	 *
	 * Registers a plugin factory for a specific name. Normally
	 * TemplPluginFactory are registered to create specific plugins with
	 * \c new instead of loading them from a dynamic library.
	 *
	 * @param name Name of the plugins factory.
	 * @param f Factory to create the plugins (or 0 to load)
	 * @return true, if the plugin factory could be registered
	 * (no \a name of factory \a f) or is already registered.
	 * @exception ExceptRiCPPError No memory or wrong version
	 */
	inline virtual bool registerFactory(const char *name, TemplPluginFactory<Plugin> *f=0)
	{
		std::string key(noNullStr(name));
		if ( key.empty() ) {
			return false;
		}
		if ( f ) {
			m_allFactoryRegistry.registerObj(key, f);
			return m_factoryRegistry.registerObj(key, f);
		}

		bool rval = getFactory(name) != 0;
		return rval;
	}

	/** @brief Unregisters a plugin factory.
	 *
	 * @param name Name of the plugin factory.
	 * @return true, if factory was unregistered
	 */
	inline bool unregisterFactory(const char *name)
	{
		std::string key(noNullStr(name));
		if ( key.empty() ) {
			return false;
		}
		TemplPluginFactory<Plugin> *f = m_factoryRegistry.findObj(key);
		if ( f ) {
			m_factoryRegistry.unregisterObj(key);
			m_allFactoryRegistry.unregisterObj(key);
			return true;
		} else {
			if ( m_internalFactoryRegistry.unregisterObj(key) ) {
				m_allFactoryRegistry.unregisterObj(key);
				return true;
			}
		}
		return false;
	}

	/** @brief Query, if plugin factory is registered.
	 *
	 * @param name Name of the plugin factory.
	 * @return true, if the factory is registered.
	 */
	inline virtual bool isRegistered(const char *name)
	{
		std::string key(noNullStr(name));
		if ( key.empty() ) {
			return false;
		}
		TemplPluginFactory<Plugin> *f = m_allFactoryRegistry.findObj(key);
		return f != 0;
	}

	/** @brief Sets a new searchpath.
	 *
	 * @param path New searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline void searchpath(const char *path)
	{
		m_searchpath = noNullStr(path);
	}

	/** @brief Gets the current searchpath.
	 *
	 * @return Searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline const char *searchpath() const
	{
		return m_searchpath.c_str();
	}

	/** @brief Block or unblock loadable plugin factories.
	 * @param isBlocked True to block the loading of dynamic libreries for plugin factories.
	 */
	inline void blockLoad(bool isBlocked) 
	{
		m_blockLoad = isBlocked;
	}

	/** @brief Queries, if plugin factories are loadable.
	 * @return True, loading of dynamic libreries for plugin factories has been blocked.
	 */
	inline bool blockLoad() const
	{
		return m_blockLoad;
	}

	/** @brief Load all plugins from a directory.
	 *
	 * Load all plugins of appropriate version and name suffix \a nameSuffix
	 * from a directory \a direct.
	 * @return Number of registered factories;
	 * @exception ExceptRiCPPError No memory
	 */
	inline int registerFromDirectory(const char *direct, const char *nameSuffix)
	{
		// load "<dynlib_prefix>*<nameSuffix>.<version><dynlib_suffix>"
		std::string pattern(CFilepathConverter::nativeDynlibPrefix());		
		pattern += "*";
		pattern += nameSuffix ? nameSuffix : "";
		std::ostringstream o;
		o << Plugin::myMajorVersion();

		std::string suffix = ".";
		suffix += o.str();
		suffix += CFilepathConverter::nativeDynlibSuffix();

		pattern += suffix;

		std::string base;
		CDirectory directory(pattern.c_str(), direct);
		CDirectory::const_iterator i = directory.begin();
		int n = 0;
		for ( ; i!= directory.end(); ++i ) {
			(*i).basename(suffix.c_str(), base);
			if ( !base.empty() ) {
				const char *basename = base.c_str();
				if ( base.length() >= strlen(CFilepathConverter::nativeDynlibPrefix()) ) {
					basename += strlen(CFilepathConverter::nativeDynlibPrefix());
				}
				try {
					if ( getFactory(basename) ) {
						++n;
					}
				} catch (ExceptRiCPPError &e) {
					if ( e.code() == RIE_NOMEM ) {
						throw e;
					}
					// ignore the other errors (file not appropriate)
				}
			}
		}
		return n;
	}

	/** @brief Constant iterator to access the elements (beginning).
	 *  @return Iterator with the first elements as current element.
	 */
	inline const_iterator begin() const
	{
		return m_allFactoryRegistry.begin();
	}

	/** @brief Constant iterator to access the elements (behind the last element).
	 *  @return Iterator to query the end of the iteration
	 *          (like sthe std iterators does not refer a valid element).
	 */
	inline const_iterator end() const
	{
		return m_allFactoryRegistry.end();
	}

	/** @brief Gets the size of the element map
	 * @return The number of stored elements.
	 */
	inline size_type size() const
	{
		return m_allFactoryRegistry.size();
	}
}; // template class TemplPluginHandler

/** @brief Singleton that handles the plugins for a specific type.
 *
 * Plugins can be loaded from a dynamic library or created by registered factories.
 * Objects of TemplPluginHandlerSingleton are singletons of TemplPluginHandler.
 */
template
<class Plugin>
class TemplPluginHandlerSingleton
{
protected:
	/** @brief Plugin handler as singleton.
	 */
	static TemplPluginHandler<Plugin> ms_pluginHandler;
public:
	/** @brief Const iterator for the elements.
	 */
	typedef
		typename
		TemplPluginHandler<Plugin>::const_iterator
		const_iterator;

	/** @brief Size type for the number of stored elements
	 */
	typedef
		typename
		TemplPluginHandler<Plugin>::size_type
		size_type;

	/** @brief Constructor
	 */
	inline TemplPluginHandlerSingleton()
	{
	}

	/** @brief Virtual destructor
	 */
	inline virtual ~TemplPluginHandlerSingleton()
	{
	}

	/** @brief Gets a new plugin
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 * @return The loaded plugin
	 * @exception ERiCPPException Could not load plugin or types do not match.
	 */
	inline virtual Plugin *newPlugin(const char *name)
	{
		return ms_pluginHandler.newPlugin(name);
	}

	/** @brief Gets the last plugin created
	 *
	 * @param name Name of the plugin and basename of the plugin library
	 * @return The loaded plugin loaded lately
	 * @see TemplPluginFactory::lastPlugin()
	 */
	inline virtual Plugin *lastPlugin(const char *name)
	{
		return ms_pluginHandler.lastPlugin(name);
	}

	/** @brief Deletes a plugin.
	 *
	 * @param p Plugin to delete
	 * @return true, if the plugin could be deleted
	 */
	inline virtual bool deletePlugin(Plugin *p)
	{
		return ms_pluginHandler.deletePlugin(p);
	}

	/** @brief Registers a plugin factory
	 *
	 * Registers a plugin factory for a specific name. Normally
	 * TemplPluginFactory are registered to create specific plugins with
	 * \c new instead of loading them from a dynamic library.
	 *
	 * @param name Name of the plugins
	 * @param f Factory to create the plugins
	 * @return true, if the plugin factory could be registerd
	 */
	inline virtual bool registerFactory(const char *name, TemplPluginFactory<Plugin> *f)
	{
		return ms_pluginHandler.registerFactory(name, f);
	}

	/** @brief unregisters a plugin factory.
	 *
	 * @param name Name of the plugin factory.
	 * @return true, if factory was unregistered
	 */
	inline virtual bool unregisterFactory(const char *name)
	{
		return ms_pluginHandler.unregisterFactory(name);
	}

	/** @brief Query, if plugin factory is registered.
	 *
	 * @param name Name of the plugin factory.
	 * @return true, if the factory is registered.
	 */
	inline virtual bool isRegistered(const char *name)
	{
		return ms_pluginHandler.isRegistered(name);
	}

	/** @brief Sets a new searchpath.
	 *
	 * @param path New searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline void searchpath(const char *path)
	{
		ms_pluginHandler.searchpath(path);
	}

	/** @brief Gets the current searchpath.
	 * @return Searchpath, directory seperator '/', pathes separated by ';'.
	 */
	inline const char *searchpath() const
	{
		return ms_pluginHandler.searchpath();
	}

	/** @brief Block or unblock loadable plugin factories
	 * @param isBlocked True to block the loading of dynamic libreries for plugin factories.
	 */
	inline void blockLoad(bool isBlocked) 
	{
		ms_pluginHandler.blockLoad(isBlocked);
	}

	/** @brief Queries, if plugin factories are loadable.
	 * @return True, loading of dynamic libreries for plugin factories has been blocked.
	 */
	inline bool blockLoad() const
	{
		return ms_pluginHandler.blockLoad();
	}

	/** @brief Load all plugins from a directory.
	 *
	 * Load all plugins of appropriate version and suffix \a suffix from a directory \a direct
	 *
	 * @return Number of loaded plugins
	 */
	inline int registerFromDirectory(const char *direct, const char *suffix)
	{
		return ms_pluginHandler.registerFromDirectory(direct, suffix);
	}

	/** @brief Constant iterator to access the elements (beginning).
	 *  @return Iterator with the first elements as current element.
	 */
	inline const_iterator begin() const
	{
		return ms_pluginHandler.begin();
	}

	/** @brief Constant iterator to access the elements (behind the last element).
	 *  @return Iterator to query the end of the iteration
	 *          (like sthe std iterators does not refer a valid element).
	 */
	inline const_iterator end() const
	{
		return ms_pluginHandler.end();
	}

	/** @brief Gets the size of the element map
	 * @return The number of stored elements.
	 */
	inline size_type size() const
	{
		return ms_pluginHandler.size();
	}
}; // template class TemplPluginHandler

template <class Plugin> TemplPluginHandler<Plugin> TemplPluginHandlerSingleton<Plugin>::ms_pluginHandler;

} // namespace RiCPP

#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
