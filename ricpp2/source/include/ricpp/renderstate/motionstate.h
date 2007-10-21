#ifndef _RICPP_RENDERSTATE_MOTIONSTATE_H
#define _RICPP_RENDERSTATE_MOTIONSTATE_H

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
		MOT_OUTSIDE = 0,      //!< Outside a motion block, however, m_curRequest bears the current request id.
		MOT_INSIDE = 1,       //!< Inside a motion block, no error.
		MOT_TOO_MANY_REQ = 2, //!< Inside a motion block, too many requests (more than the number of the samples).
		MOT_NO_REQ_MATCH = 4  //!< Inside a motion block, requests don't match or are not valid for motion blocks.
	};

private:
	std::vector<RtFloat> m_times; //!< Samples of the last (current) motion block request.
	RtInt m_curSample;            //!< Current sample (if inside the mootion block, can be hugher than the size of m_times, if there are to many requests within a motion block).
	EnumRequests m_firstRequest;  //!< Id of the first request inside the last (current) motion block.
	EnumRequests m_curRequest;    //!< Id of the current request inside or outside the last (current) motion block..
	unsigned int m_curState;      //!< The state of the motion block.

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

#endif // _RICPP_RENDERSTATE_MOTIONSTATE_H
