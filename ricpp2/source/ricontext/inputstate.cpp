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

/** @file inputstate.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief State of the input of the ricpp context (read file, replay file etc.)
 */

#include "ricpp/ricontext/inputstate.h"

#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

using namespace RiCPP;

const char * const CInputState::ms_stateNames[INPUT_REPLAY_FILE+1] =
{
	RI_UNKNOWN,
	"INPUT_OUTSIDE",
	"INPUT_DEF_OBJECT",
	"INPUT_DEF_ARCHIVE",
	"INPUT_DEF_CACHEFILE",
	"INPUT_DEF_READFILE",
	"INPUT_REPLAY_OBJECT",
	"INPUT_REPLAY_ARCHIVE",
	"INPUT_REPLAY_FILE"
};

CInputState::CInputState()
{
	m_stateStack.push_back(SInputStackElem(INPUT_OUTSIDE, "", illInputHandleType, 0));
}

void CInputState::popState()
{
	assert(m_stateStack.size()>1);
	assert(m_stateStack.size()-1==m_path.size());

	m_stateStack.pop_back();
	m_path.pop_back();

	assert(m_stateStack.size()>=1);
	assert(m_stateStack.size()-1==m_path.size());
}

void CInputState::objectBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_DEF_OBJECT, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::objectEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_DEF_OBJECT ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in objectEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::archiveBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_DEF_ARCHIVE, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::archiveEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_DEF_ARCHIVE ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in archiveEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::cacheFileBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_DEF_CACHEFILE, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::cacheFileEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_DEF_CACHEFILE ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in cacheFileEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::interpretFileBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_DEF_READFILE, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::interpretFileEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_DEF_READFILE ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in interpretFileEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::replayObjectBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_REPLAY_OBJECT, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::replayObjectEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_REPLAY_OBJECT ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in replayObjectEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::replayArchiveBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_REPLAY_ARCHIVE, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::replayArchiveEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_REPLAY_ARCHIVE ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in replayArchiveEnd(): illegal stack state %s", curStateName());
	}
	popState();
}

void CInputState::replayFileBegin(const char *name, InputHandleType handle)
{
	m_stateStack.push_back(SInputStackElem(INPUT_REPLAY_FILE, name, handle, 0));
	m_path.push_back(name);
}

void CInputState::replayFileEnd()
{
	assert(m_stateStack.size()>1);
	if ( curState() != INPUT_REPLAY_FILE ) {
		throw ExceptRiCPPError(
			RIE_BUG, RIE_SEVERE,
			__LINE__, __FILE__,
			"in replayFileEnd(): illegal stack state %s", curStateName());
	}
	popState();
}
