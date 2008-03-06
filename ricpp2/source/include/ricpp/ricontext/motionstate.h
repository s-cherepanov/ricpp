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

#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif // _RICPP_RIBASE_RICPPTYPES_H

#ifndef _RICPP_RICPP_REQUESTS_H
#include "ricpp/ricpp/requests.h"
#endif // _RICPP_RICPP_REQUESTS_H

#include <vector>
#include <deque>
#include <cassert>

namespace RiCPP {

	typedef std::deque<RtFloat>                 TypeMotionTimes;

	/**! @brief Final motion block state, being part of the TRenderState.
	 */
	class CMotionState
	{
	public:
		/** @brief State of the motion block.
		 */
		enum EnumMotionState {
			MOT_OUTSIDE = 0,      ///< Outside a motion block, however, m_curRequest bears the current request id.
			MOT_INSIDE = 1,       ///< Inside a motion block, no error.
			MOT_TOO_MANY_REQ = 2, ///< Inside a motion block, too many requests (more than the number of the samples).
			MOT_NO_REQ_MATCH = 4  ///< Inside a motion block, requests don't match or are not valid for motion blocks.
		};

	private:
		/** @brief Descriptor of an open motion block
		 */
		class CMotionStateElem {
		public:
			std::vector<RtFloat> m_times; ///< Samples of the last (current) motion block request, copies part of CMotionState::m_motionTimes
			RtInt m_curSample;            ///< Current sample index (if inside the motion block, can be hugher than the size of m_times, if there are to many requests within a motion block).
			unsigned long m_first;        ///< Start index of samples in @c m_motionTimes.
			unsigned long m_last;         ///< End index of samples in @c m_motionTimes.
			EnumRequests m_firstRequest;  ///< Id of the first request inside the last (current) motion block.
			EnumRequests m_curRequest;    ///< Id of the current request inside or outside the last (current) motion block.
			unsigned int m_curState;      ///< The state of the motion block.
			bool m_attributesStored;      ///< Additional attribute block because of moved model instances (ReadArchive, ObjectInstance, Procedural).

			/** @brief Default constructor
			 *
			 *  Sets some standard values for an empty motion element.
			 *
			 */
			CMotionStateElem();

			/** @brief Destructor
			 */
			inline ~CMotionStateElem() {}

			/** @brief Assignment
			 *  @param elem Object to assign to *this.
			 *  @return *this
			 */
			CMotionStateElem &operator=(const CMotionStateElem &elem);
		};

		std::deque<CMotionStateElem> m_elems; ///< Nested motion blocks (motion blocks can be nested if a model instance is moved) @see @c m_attributesStored
		
		TypeMotionTimes m_motionTimes;        ///< Times of all motion blocks occured in this attribute block
		std::vector<unsigned long> m_marks;   ///< Marks for m_motionTimes, marked on AttributeBegin, a chunk of elements can be removed if an attribute block ends

		void incCurSampleIdx();               ///< Increments the sample index in a motion block (CMotionStateElem::m_curSample)

	public:

		inline CMotionState() {}

		inline CMotionState(const CMotionState &aMotionState)
		{
			*this = aMotionState;
		}

		inline ~CMotionState() {}

		inline CMotionState *duplicate() const
		{
			return new CMotionState(*this);
		}

		CMotionState &operator=(const CMotionState &aMotionState);

		inline std::vector<RtFloat> &curTimes()
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_times;
		}

		inline const std::vector<RtFloat> &curTimes() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_times;
		}

		void motionBegin(RtInt N, RtFloat times[]);

		void motionEnd();

		void mark();

		void clearToMark();

		const TypeMotionTimes &allTimes() const {
			return m_motionTimes;
		}

		inline RtInt curSampleCnt() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_curSample;
		}

		inline unsigned long firstSampleIdx() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_first;
		}

		inline unsigned long lastSampleIdx() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_last;
		}

		RtFloat curSample() const;

		void request(EnumRequests req);

		inline EnumRequests firstRequest() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_firstRequest;
		}

		inline EnumRequests curRequest() const
		{
			assert( !m_elems.empty() );
			return m_elems.back().m_curRequest;
		}

		inline unsigned int curState() const
		{
			if ( m_elems.empty() ) {
				return MOT_OUTSIDE;
			}
			return m_elems.back().m_curState;
		}

		inline bool attributesStored() const
		{
			assert( !m_elems.empty() );
			if ( m_elems.empty() ) {
				return false;
			}
			return m_elems.back().m_attributesStored;
		}

		inline void attributesStored(bool flag)
		{
			assert( !m_elems.empty() );
			if ( m_elems.empty() ) {
				return;
			}
			m_elems.back().m_attributesStored = flag;
		}
	}; // CMotionState


	class IMovedValueClass {
	public:
		inline IMovedValueClass() {}
		inline virtual ~IMovedValueClass() {}
		virtual IMovedValueClass *duplicate() const = 0;
			
		virtual void clear() = 0;
		virtual void fill(RtInt n) = 0;
		virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times) = 0;
		virtual void sampleReset() = 0;
	}; // IMovedValueClass
		
}

#endif // _RICPP_RICONTEXT_MOTIONSTATE_H
