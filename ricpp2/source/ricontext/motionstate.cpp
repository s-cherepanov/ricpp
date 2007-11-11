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

/** @file motionstate.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the handling of the motion blocks (used by render state).
 */

#include "ricpp/ricontext/motionstate.h"

using namespace RiCPP;

CMotionState::CMotionState()
{
	m_curSample = -1;
	m_firstRequest = m_curRequest = REQ_UNKNOWN;
	m_curState = MOT_OUTSIDE;
}

void CMotionState::incCurSampleIdx()
{
	m_curSample++;
	if ( (m_curState & MOT_INSIDE) && m_curSample >= static_cast<RtInt>(m_times.size()) )
		m_curState |= MOT_TOO_MANY_REQ;
}

CMotionState &CMotionState::operator=(const CMotionState &aMotionState)
{
	if ( this == &aMotionState )
		return *this;

	m_times = aMotionState.m_times;
	m_curSample = aMotionState.m_curSample;
	m_firstRequest = aMotionState.m_firstRequest;
	m_curRequest = aMotionState.m_curRequest;
	m_curState = aMotionState.m_curState;

	return *this;
}

void CMotionState::open(RtInt N, RtFloat times[])
{
	if ( !times )
		N=0;
	m_times.clear();
	m_times.resize(N);
	for ( RtInt i = 0; i < N; ++i ) {
		m_times[i] = times[i];
	}
	m_curSample = -1;
	m_curState = MOT_INSIDE;
	m_firstRequest = REQ_UNKNOWN;
}

void CMotionState::close()
{
	m_curState = MOT_OUTSIDE;
}

RtFloat CMotionState::curSample()
{
	if ( m_times.empty() )
		return 0;

	if (m_curSample <= 0 )
		return m_times[0];

	if ( m_curSample >= static_cast<RtInt>(m_times.size()) )
		return m_times[m_times.size()-1];

	return m_times[m_curSample];
}

void CMotionState::request(EnumRequests req)
{
	m_curRequest = req;
	if ( m_curState & MOT_INSIDE ) {
		if ( m_curSample = -1 ) {
			m_firstRequest = req;
		}
		if ( req != REQ_MOTION_END ) {
			incCurSampleIdx();
			if ( m_curRequest != m_firstRequest ) {
				m_curState |= MOT_NO_REQ_MATCH;
			}
		}
	}
}
