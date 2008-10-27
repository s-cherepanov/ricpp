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
#include <iostream>
using namespace RiCPP;


CMotionState::CMotionStateElem::CMotionStateElem()
{
	m_curSample = -1;
	m_first = 0;
	m_last = 0;
	m_firstRequest = m_curRequest = REQ_UNKNOWN;
	m_curState = MOT_OUTSIDE;
	m_attributesStored = false;
}

CMotionState::CMotionStateElem &
CMotionState::CMotionStateElem::operator=(const CMotionState::CMotionStateElem &elem)
{
	if ( this == &elem )
		return *this;

	m_times = elem.m_times;

	m_curSample = elem.m_curSample;
	m_first = elem.m_first;
	m_last = elem.m_last;

	m_firstRequest = elem.m_firstRequest;
	m_curRequest = elem.m_curRequest;
	m_curState = elem.m_curState;
	m_attributesStored = elem.m_attributesStored;

	return *this;
}


// -----------------------------------------------------------------------------

void CMotionState::incCurSampleIdx()
{
	assert(!m_elems.empty());

	if ( !m_elems.empty() ) {
		m_elems.back().m_curSample++;
		if ( m_elems.back().m_curSample >= static_cast<RtInt>(m_elems.back().m_times.size()) ) {
			m_elems.back().m_curState |= MOT_TOO_MANY_REQ;
		}
	}
}

CMotionState &CMotionState::operator=(const CMotionState &aMotionState)
{
	if ( this == &aMotionState )
		return *this;

	m_elems = aMotionState.m_elems;
	m_motionTimes = aMotionState.m_motionTimes;
	m_marks = aMotionState.m_marks;

	return *this;
}

void CMotionState::motionBegin(RtInt N, RtFloat times[])
{
	if ( !times )
		N=0;

	m_elems.push_back(CMotionStateElem());
	assert(!m_elems.empty());
	
	unsigned long sz = (unsigned long)m_motionTimes.size();
	if ( N ) {
		m_motionTimes.resize(sz+N);
	}
	assert(sz+N == m_motionTimes.size());
	for ( unsigned long i = sz; i < m_motionTimes.size(); ++i ) {
		m_motionTimes[i] = times[i-sz];
	}
	
	if ( !m_elems.empty() ) {
		m_elems.back().m_times.clear();
		m_elems.back().m_times.resize(N);
		for ( RtInt i = 0; i < N; ++i ) {
			m_elems.back().m_times[i] = times[i];
		}
		m_elems.back().m_curSample = -1;
		m_elems.back().m_first = sz;
		m_elems.back().m_last = (unsigned long)m_motionTimes.size();		
		m_elems.back().m_curState = MOT_INSIDE;
		m_elems.back().m_firstRequest = REQ_UNKNOWN;
	}
}

void CMotionState::motionEnd()
{
	assert(!m_elems.empty());

	if ( !m_elems.empty() ) {
		m_elems.pop_back();
	}
}

void CMotionState::mark()
{
	m_marks.push_back((unsigned long)m_motionTimes.size());
}

void CMotionState::clearToMark()
{
	if (!m_marks.empty()) {
		unsigned int sz = m_marks.back();
		assert (sz <= m_motionTimes.size());
		m_marks.pop_back();
		if ( sz < m_motionTimes.size() ) {
			m_motionTimes.erase(m_motionTimes.begin()+sz, m_motionTimes.end());
		}
	}
}


RtFloat CMotionState::curSample() const
{
	assert(!m_elems.empty());

	if ( m_elems.empty() )
		return 0;

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

	if ( m_elems.empty() )
		return;

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
