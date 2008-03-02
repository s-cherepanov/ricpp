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




CMotionState::CMotionStateElem &CMotionState::CMotionStateElem::operator=(const CMotionState::CMotionStateElem &elem)
{
	if ( this == &elem )
		return *this;

	m_times = elem.m_times;
	m_curSample = elem.m_curSample;
	m_firstRequest = elem.m_firstRequest;
	m_curRequest = elem.m_curRequest;
	m_curState = elem.m_curState;

	return *this;
}

CMotionState::CMotionStateElem::CMotionStateElem()
{
	m_curSample = -1;
	m_firstRequest = m_curRequest = REQ_UNKNOWN;
	m_curState = MOT_OUTSIDE;
}

CMotionState::CMotionStateElem::~CMotionStateElem()
{
}


CMotionState::CMotionState()
{
}

void CMotionState::incCurSampleIdx()
{
	assert(!m_elems.empty());

	if ( !m_elems.empty() ) {
		m_elems.back().m_curSample++;
		if ( m_elems.back().m_curSample >= static_cast<RtInt>(m_elems.back().m_times.size()) )
			m_elems.back().m_curState |= MOT_TOO_MANY_REQ;
	}
}

CMotionState &CMotionState::operator=(const CMotionState &aMotionState)
{
	if ( this == &aMotionState )
		return *this;

	m_elems = aMotionState.m_elems;

	return *this;
}

void CMotionState::open(RtInt N, RtFloat times[])
{
	if ( !times )
		N=0;

	m_elems.push_back(CMotionStateElem());

	assert(!m_elems.empty());

	if ( !m_elems.empty() ) {
		m_elems.back().m_times.clear();
		m_elems.back().m_times.resize(N);
		for ( RtInt i = 0; i < N; ++i ) {
			m_elems.back().m_times[i] = times[i];
		}
		m_elems.back().m_curSample = -1;
		m_elems.back().m_curState = MOT_INSIDE;
		m_elems.back().m_firstRequest = REQ_UNKNOWN;
	}
}

void CMotionState::close()
{
	// m_curState = MOT_OUTSIDE;
	assert(!m_elems.empty());

	if ( !m_elems.empty() ) {
		m_elems.pop_back();
	}
}

RtFloat CMotionState::curSample()
{
	assert(!m_elems.empty());

	if ( m_elems.empty() ) {
		return 0;
	}

	if ( m_elems.back().m_times.empty() )
		return 0;

	if ( m_elems.back().m_curSample <= 0 )
		return m_elems.back().m_times[0];

	if ( m_elems.back().m_curSample >= static_cast<RtInt>(m_elems.back().m_times.size()) )
		return m_elems.back().m_times[m_elems.back().m_times.size()-1];

	return m_elems.back().m_times[m_elems.back().m_curSample];
}

void CMotionState::request(EnumRequests req)
{
	assert(!m_elems.empty());

	if ( m_elems.empty() ) {
		return;
	}

	m_elems.back().m_curRequest = req;
	if ( req != REQ_MOTION_END ) {
		if ( -1 == m_elems.back().m_curSample ) {
			m_elems.back().m_firstRequest = req;
		}
		incCurSampleIdx();
		if ( m_elems.back().m_curRequest != m_elems.back().m_firstRequest ) {
			m_elems.back().m_curState |= MOT_NO_REQ_MATCH;
		}
	} else {
		m_elems.back().m_curState = MOT_OUTSIDE;
	}
}
