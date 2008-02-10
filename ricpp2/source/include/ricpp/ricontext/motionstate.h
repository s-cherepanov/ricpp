#ifndef _RICPP_RICONTEXT_MOTIONSTATE_H
#define _RICPP_RICONTEXT_MOTIONSTATE_H

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

/** @file motionstate.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling of the motion blocks (used by render state).
 */

#ifndef _RICPP_RICPP_RICPPTYPES_H
#include "ricpp/ricpp/ricpptypes.h"
#endif // _RICPP_RICPP_RICPPTYPES_H

#ifndef _RICPP_RICPP_REQUESTS_H
#include "ricpp/ricpp/requests.h"
#endif // _RICPP_RICPP_REQUESTS_H

#include <vector>

namespace RiCPP {

/**! @brief Motion block state
 */
class CMotionState
{
public:
	/**! @brief State of the motion block.
	 */
	enum EnumMotionState {
		MOT_OUTSIDE = 0,      ///< Outside a motion block, however, m_curRequest bears the current request id.
		MOT_INSIDE = 1,       ///< Inside a motion block, no error.
		MOT_TOO_MANY_REQ = 2, ///< Inside a motion block, too many requests (more than the number of the samples).
		MOT_NO_REQ_MATCH = 4  ///< Inside a motion block, requests don't match or are not valid for motion blocks.
	};

private:
	std::vector<RtFloat> m_times; ///< Samples of the last (current) motion block request.
	RtInt m_curSample;            ///< Current sample (if inside the mootion block, can be hugher than the size of m_times, if there are to many requests within a motion block).
	EnumRequests m_firstRequest;  ///< Id of the first request inside the last (current) motion block.
	EnumRequests m_curRequest;    ///< Id of the current request inside or outside the last (current) motion block..
	unsigned int m_curState;      ///< The state of the motion block.

protected:
	void incCurSampleIdx();

public:

	CMotionState();

	inline CMotionState(const CMotionState &aMotionState)
	{
		*this = aMotionState;
	}

	inline virtual ~CMotionState()
	{
	}

	inline virtual CMotionState *duplicate()
	{
		return new CMotionState(*this);
	}

	CMotionState &operator=(const CMotionState &aMotionState);

	inline std::vector<RtFloat> &times()
	{
		return m_times;
	}

	inline const std::vector<RtFloat> &times() const
	{
		return m_times;
	}

	void open(RtInt N, RtFloat times[]);

	void close();

	inline RtInt curSampleIdx() const
	{
		return m_curSample;
	}

	RtFloat curSample();

	void request(EnumRequests req);

	inline EnumRequests firstRequest() const
	{
		return m_firstRequest;
	}

	inline EnumRequests curRequest() const
	{
		return m_curRequest;
	}

	inline unsigned int curState() const
	{
		return m_curState;
	}
}; // CMotionState

}

#endif // _RICPP_RICONTEXT_MOTIONSTATE_H
