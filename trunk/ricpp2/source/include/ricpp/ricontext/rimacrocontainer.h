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
#include <deque>
namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Macro Container
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Macro container.
	 */
	class CRiMacro : public CHandle {
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
		typedef std::deque<CRManInterfaceCall *> MacroContainerType; ///< Type for the container
		MacroContainerType *m_calls;      ///< Container of all interface calls for this macro - strange things happens while push_back() if not dynamic.
		
		CRManInterfaceFactory *m_factory; ///< Container used to create the contents of the macro
		EnumMacroTypes m_macroType;       ///< Type of macro, either object or (inline, file) archive.
		bool m_isClosed;                  ///< true, if macro definition is completed.
		bool m_postpone;                  ///< true, if macro should be postponed (e.g. in RIB oputput), default is false

		std::vector<std::string> m_path;  ///< Macro path, Macros can be nested, name is the last entry (back())

	public:

		/** @brief Constructor initializes the macro.
		 *
		 *  @param anId Name of the macro, file name or handle name. (@see CHandle)
		 *  @param aHandleNo Corresponding running number of the handle (@see CHandle)
		 *  @param isFromHandleId Flag should be true if anId was created from __handleid parameter or RIB string handle (@see CHandle)
		 *  @param aFactory The factory that will be used to create and destroy the contents of the macro
		 *  @param macroType Type of the macro.
		 */
		CRiMacro(RtString anId = 0,
				 unsigned long aHandleNo = 0, bool isFromHandleId = false,
				 CRManInterfaceFactory *aFactory = 0,
				 EnumMacroTypes macroType = MACROTYPE_UNKNOWN);

		
		CRiMacro(const CRiMacro &aMacro);

		/** @brief Destructor, frees resources.
		 */
		virtual ~CRiMacro();
		
		/** @brief Duplicates the handle
		 *  @return Clone of *this
		 */
		inline virtual CHandle *duplicate() const
		{
			return new CRiMacro(*this);
		}
		
		void debug(const char *prefix=0) const;
		virtual void clear();
		CRiMacro &operator=(const CRiMacro &aMacro);

		inline const std::vector<std::string> &path() const { return m_path; }
		inline std::vector<std::string> &path() { return m_path; }

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
		inline bool isClosed() const
		{
			return m_isClosed;
		}
		
		/** @brief Opens a macro for writing (adding requests).
		 *
		 *  A macro is opened after created. @see isClosed()
		 */
		inline void open()
		{
			m_isClosed = false;
		}

		/** @brief Closes a macro.
		 *
		 *  A macro is closed by the framework after called an End-Request. @see isClosed()
		 */
		inline void close()
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
		inline bool postpone() const
		{
			return m_postpone;
		}
		
		/** @brief Sets the postpone state.
		 *
		 *  @see postpone()
		 *
		 *  @param doPostpone true, if a macro instanciation should be postponed (e.g. by a RIB writer).
		 */
		inline void postpone(bool doPostpone)
		{
			m_postpone = doPostpone;
		}

		inline const CRManInterfaceFactory *factory() const { return m_factory; }
		inline CRManInterfaceFactory *factory() { return m_factory; }
		inline void factory(CRManInterfaceFactory *aFactory) { m_factory = aFactory; }
		
		/** @brief Gets the type of the macro.
		 *
		 *  Gets the type of the macro (EnumMacroTypes) for some checking.
		 *
		 *  @return Type of the macro (EnumMacroTypes object, inline archive or archive file).
		 */
		inline EnumMacroTypes macroType() const
		{
			return m_macroType;
		}

		/** @brief Sets the type of the macro.
		 *
		 *  @param aMacroType Type of the macro (EnumMacroTypes object, inline archive or archive file).
		 */
		inline void macroType(EnumMacroTypes aMacroType)
		{
			m_macroType = aMacroType;
		}
	}; // CRiMacro

} // namespace RiCPP

#endif // _RICPP_RICONTEXT_RIMACROCONTAINER_H
