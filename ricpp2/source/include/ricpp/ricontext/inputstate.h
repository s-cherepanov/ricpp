#ifndef _RICPP_RICONTEXT_INPUTSTATE_H
#define _RICPP_RICONTEXT_INPUTSTATE_H

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

/** @file inputstate.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief State of the input of the ricpp context (read file, replay file etc.)
 */

#include <vector>
#include <string>
#include <cassert>

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

namespace RiCPP {

	typedef const char * InputHandleType;
	const InputHandleType illInputHandleType=0;
	enum EnumInputState {
		INPUT_UNKNOWN = 0,
		INPUT_OUTSIDE,
		INPUT_DEF_OBJECT,
		INPUT_DEF_ARCHIVE,
		INPUT_DEF_CACHEFILE,
		INPUT_DEF_READFILE,
		INPUT_REPLAY_OBJECT,
		INPUT_REPLAY_ARCHIVE,
		INPUT_REPLAY_FILE
	}; // EnumInputState

	class CInputState {
		struct SInputStackElem {
			EnumInputState m_state;
			std::string m_name;
			InputHandleType m_handle;
			unsigned long m_lineNo;
			inline SInputStackElem(
				EnumInputState aState = INPUT_UNKNOWN,
				const char *aName = 0,
				InputHandleType aHandle = illInputHandleType,
				unsigned long aLineNo = 0)
				:m_state(aState), m_name(noNullStr(aName)), m_handle(aHandle), m_lineNo(aLineNo)
			{
			}
		}; // SInputStackElem

		static const char * const ms_stateNames[INPUT_REPLAY_FILE+1];
		std::vector<SInputStackElem> m_stateStack;
		std::vector<std::string> m_path;

		void popState();

	public:
		CInputState();

		void objectBegin(const char *name, InputHandleType handle);
		void objectEnd();
		void archiveBegin(const char *name, InputHandleType handle);
		void archiveEnd();
		void cacheFileBegin(const char *name, InputHandleType handle);
		void cacheFileEnd();
		void interpretFileBegin(const char *name, InputHandleType handle);
		void interpretFileEnd();

		void replayObjectBegin(const char *name, InputHandleType handle);
		void replayObjectEnd();
		void replayArchiveBegin(const char *name, InputHandleType handle);
		void replayArchiveEnd();
		void replayFileBegin(const char *name, InputHandleType handle);
		void replayFileEnd();

		inline void reset() { m_stateStack.resize(1); }
		inline size_t size() const { return m_stateStack.size(); }

		inline const std::vector<std::string> &path() const
		{
			return m_path;
		}
		
		inline const std::string &curName() const
		{
			assert(!m_stateStack.empty());
			return m_stateStack.back().m_name;
		}

		inline InputHandleType curHandle() const
		{
			assert(!m_stateStack.empty());
			return m_stateStack.back().m_handle;
		}

		inline EnumInputState curState() const
		{
			assert(!m_stateStack.empty());
			return m_stateStack.back().m_state;
		}

		inline bool records() const
		{
			return curState() >= INPUT_DEF_OBJECT && curState() <= INPUT_DEF_READFILE;
		}

		inline bool replays() const
		{
			return curState() >= INPUT_REPLAY_OBJECT;
		}

		inline bool outside() const
		{
			return curState() == INPUT_OUTSIDE;
		}

		inline const char *curStateName() const
		{
			assert(!m_stateStack.empty());
			return ms_stateNames[curState()];
		}

		inline unsigned long lineNo() const
		{
			assert(!m_stateStack.empty());
			return m_stateStack.back().m_lineNo;
		}

		inline void lineNo(unsigned long aLineNo)
		{
			assert(!m_stateStack.empty());
			m_stateStack.back().m_lineNo = aLineNo;
		}
	}; // CInputState

} // namespace RiCPP

#endif // _RICPP_RICONTEXT_INPUTSTATE_H
