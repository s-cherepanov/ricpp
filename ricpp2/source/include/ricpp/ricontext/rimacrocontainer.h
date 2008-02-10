#ifndef _RICPP_RICONTEXT_RIMACROCONTAINER_H
#define _RICPP_RICONTEXT_RIMACROCONTAINER_H

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

/** @file rimacrocontainer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The container for ri macros, @see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Macro Container
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container.
 */
class CRiMacro {
public:
	/** @brief Macro types
	 */
	enum EnumMacroTypes {
		MACROTYPE_UNKNOWN = 0, ///< Macro type is yet unknown
		MACROTYPE_OBJECT,      ///< Macro contains interface calls for an object definition
		MACROTYPE_ARCHIVE,     ///< Macro contains interface calls for an rib inline archive definition
		MACROTYPE_FILE         ///< Macro contains interface calls for an rib file archive definition
	};

private:
	std::list<CRManInterfaceCall *> m_callList;  ///< List of all interface calls for this macro.
	std::string m_name;         ///< Name of the macro (eg. file name, handle name).
	EnumMacroTypes m_macroType; ///< Type of macro, either object or (inline, file) archive.
	bool m_isClosed;            ///< true, if macro definition is completed.
	bool m_postpone;            ///< true, if macro should be postponed (e.g. in RIB oputput), default is false

public:

	/** @brief Constructor initializes the macro.
	 *
	 *  @param aName Name of the macro, file name or handle name.
	 *  @param macroType Type of the macro.
	 */
	inline CRiMacro(
		RtString aName = 0,
		EnumMacroTypes macroType = MACROTYPE_UNKNOWN
	) :
		m_name(noNullStr(aName)), m_macroType(macroType),
		m_isClosed(false), m_postpone(false)
	{
	}

	/** @brief Destructor, frees resources.
	 */
	virtual ~CRiMacro();

	/** @brief Gets the name of the macro.
	 *
	 *  @return The name of the macro.
	 */
	inline RtString name() const
	{
		return m_name.c_str();
	}

	/** @brief Sets the name of the macro.
	 *
	 *  @param aName The new name for the macro.
	 */
	inline void name(RtString aName)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Replays archive with callback for RIB archives at a back end renderer.
	 *
	 *  @param ri The back end renderer used to replay.
	 *  @param callback Used for archive callbacks (esp. for IRi::readArchive()).
	 */
	void replay(IDoRender &ri, const IArchiveCallback *callback);

	/** @brief Adds an interface call to the macro.
	 *
	 *  @param c Instance of an interface call.
	 *  @return true, if added and false, if not (because c == 0).
	 */
	bool add(CRManInterfaceCall *c);

	/** @brief Gets the open/closed state of a macro.
	 *
	 *  A macro is closed, if all request are read and an End (Like IRi::objectEnd() ) was called.
	 *  A closed macro can be instanciated.
	 *
	 *  @return true, if the macro is closed
	 */
	bool isClosed() const
	{
		return m_isClosed;
	}
	
	/** @brief Opens a macro for writing (adding requests).
	 *
	 *  A macro is opened after created. @see isClosed()
	 */
	void open()
	{
		m_isClosed = false;
	}

	/** @brief Closes a macro.
	 *
	 *  A macro is closed by the framework after called an End-Request. @see isClosed()
	 */
	void close()
	{
		m_isClosed = true;
	}

	/** @brief Postpone a macro instanciation (hint only).
	 *
	 *  postpone() gives a hint, that the instanciation should be postponed. E.g.
	 *  indicates, that a RIB writer should print a ReadArchive request instead
	 *  of reading the macro and print the content. Also possible for ObjectInstance.
	 *
	 *  @return true, if a macro instanciation is postponed (e.g. by a RIB writer).
	 */
	bool postpone() const
	{
		return m_postpone;
	}
	
	/** @brief Sets the postpone state.
	 *
	 *  @see postpone()
	 *
	 *  @param doPostpone true, if a macro instanciation should be postponed (e.g. by a RIB writer).
	 */
	void postpone(bool doPostpone)
	{
		m_postpone = doPostpone;
	}

	/** @brief Gets the type of the macro.
	 *
	 *  Gets the type of the macro (EnumMacroTypes) for some checking.
	 *
	 *  @return Type of the macro (EnumMacroTypes object, inline archive or archive file).
	 */
	EnumMacroTypes macroType() const
	{
		return m_macroType;
	}

	/** @brief Sets the type of the macro.
	 *
	 *  @param aMacroType Type of the macro (EnumMacroTypes object, inline archive or archive file).
	 */
	void macroType(EnumMacroTypes aMacroType)
	{
		m_macroType = aMacroType;
	}
}; // CRiMacro


///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container for objects.
 */
class CRiObjectMacro : public CRiMacro {
private:
	RtObjectHandle m_handle; ///< Associated handle to identify the macro.
public:
	/** @brief Creates a macro container for an object (retained geometry).
	 *
	 *  The type for the object macro is always MACROTYPE_OBJECT
	 */
	inline CRiObjectMacro()
		: CRiMacro(0, MACROTYPE_OBJECT)
	{
		m_handle = illObjectHandle;
	}

	/** @brief Gets the associated handle of the object.
	 *
	 *  @return The associated handle of the object.
	 */
	inline RtObjectHandle handle() const { return m_handle; }

	/** @brief Sets the associated handle of the object.
	 *
	 *  @param h The associated handle of the object.
	 */
	inline void handle(RtObjectHandle h) { m_handle = h; }
};


///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container for archives (file or inline).
 */
class CRiArchiveMacro : public CRiMacro {
private:
	RtArchiveHandle m_handle; ///< Associated handle to identify the macro.
public:
	/** @brief Creates a macro container for a RIB archive.
	 *
	 * @param aName Filename or archive name
	 * @param aMacroType Type of the macro, will be either MACROTYPE_ARCHIVE or MACROTYPE_FILE, but is not tested
	 */
	inline CRiArchiveMacro(
		const char *aName = 0,
		EnumMacroTypes aMacroType = MACROTYPE_ARCHIVE
	) : CRiMacro(aName, aMacroType)
	{
		m_handle = illArchiveHandle; 
 	}

	/** @brief Gets the associated handle of the archive.
	 *
	 * @return The associated handle of the archive.
	 */
	inline RtArchiveHandle handle() const { return m_handle; }

	/** @brief Sets the associated handle of the archive.
	 *
	 * @param h The associated handle of the archive.
	 */
	inline void handle(RtArchiveHandle h) { m_handle = h; }
};

} // namespace RiCPP

#endif // _RICPP_RICONTEXT_RIMACROCONTAINER_H
