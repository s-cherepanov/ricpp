#include "ricpp/renderstate/motionstate.h"

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
