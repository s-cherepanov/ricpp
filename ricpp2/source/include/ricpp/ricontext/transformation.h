#ifndef _RICPP_RICONTEXT_TRANSFORMATON_H
#define _RICPP_RICONTEXT_TRANSFORMATON_H

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

/** @file transformation.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling the transformations for the render state.
 */

#ifndef _RICPP_DECLAREATION_TYPES_H
#include "ricpp/ricpp/types.h"
#endif //  _RICPP_DECLAREATION_TYPES_H

#ifndef _RICPP_RICONTEXT_MOTIONSTATE_H
#include "ricpp/ricontext/motionstate.h"
#endif // _RICPP_RICONTEXT_MOTIONSTATE_H

namespace RiCPP {

	/** @brief Class with the transformation stack and composit transformation matrix
	 */
	class CTransformation {
		
		//! false, did an operation that could not calculate the inverse.
		bool m_isValid;
		
		//! Space type of the coordinate system (current, world, camera, screen, raster, etc.)
		RtToken m_spaceType;
		
		//! A simple counter for additional transform blocks stored for this at a stack (used for detailrange)
		unsigned long m_storeCounter;
		
	protected:

		class IMovedTransform {
		public:
			inline IMovedTransform() {}
			inline virtual ~IMovedTransform() {}
			
			virtual IMovedTransform *duplicate() const = 0;
			virtual EnumRequests reqType() const = 0;

			virtual void clear() = 0;
			virtual void fill(RtInt n) = 0;
			
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse) = 0;
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse) = 0;
		};
		
		class CMovedRotate : public IMovedTransform {
		public:
			struct SRotate {
				RtFloat m_angle;
				RtFloat m_dx;
				RtFloat m_dy;
				RtFloat m_dz;
			};
			
			std::vector<SRotate> m_rotate;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedRotate()
			{
				m_motionBegin = m_motionEnd = 0;
			}

			inline CMovedRotate(const CMovedRotate &o)
			{
				*this = o;
			}

			inline virtual ~CMovedRotate() {}

			virtual IMovedTransform *duplicate() const { return new CMovedRotate(*this); }
			
			CMovedRotate &operator=(const CMovedRotate &o);

			inline virtual EnumRequests reqType() const { return REQ_ROTATE; }

			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		};
		
		//! Composit transformation matrix
		CMatrix3D m_CTM;

		//! Inverse of the composit transformation matrix
		CMatrix3D m_inverseCTM;

		//! Defered transformations (motion block, deformation)
		std::vector<IMovedTransform *> m_deferedTrans;
		
		const CMotionState *m_motionState;

	public:
		/** @brief Constructor, matrices are set to identity.
		 */
		CTransformation();

		/** @brief Copy constructor.
		 */
		inline CTransformation(const CTransformation &rt)
		{
			m_storeCounter = 0;
			*this = rt;
		}

		/** @brief Destructor.
		 */
		virtual ~CTransformation();

		/** @brief Returns a (deep) copy of this.
		 *  @return a copy of this.
		 *  @exception ExceptRiCPPError Thrown if there is not enough memory.
		 */
		virtual CTransformation *duplicate() const;

		/** @brief Assigns instance o to this instance.
		 *  @param o Instance to copy from.
		 *  @return A reference to *this.
		 */
		CTransformation &operator=(const CTransformation &o);

		/** @brief Store counter is incremented by one to indicate that another transform block is stored
		 */
		inline void incStoreCounter() { m_storeCounter++; }

		/** @brief Store counter is decremented by one to indicate that another transform block is removed
		 */
		inline void decStoreCounter() { m_storeCounter--; }
		
		/** @brief Number of additional transform blocks stored
		 */
		inline unsigned long storeCounter() const { return m_storeCounter; }

		/** @brief Sets the number of additional attribute blocks stored
		 */
		inline void storeCounter(unsigned long cnt) { m_storeCounter = cnt; }

		inline CMatrix3D &getCTM()
		{
			return m_CTM;
		}

		inline virtual const CMatrix3D &getCTM() const
		{
			return m_CTM;
		}

		inline CMatrix3D &getInverseCTM()
		{
			return m_inverseCTM;
		}

		inline virtual const CMatrix3D &getInverseCTM() const
		{
			return m_inverseCTM;
		}

		inline virtual bool isValid() const { return m_isValid; }

		RtVoid identity(void);
		RtVoid transform(RtMatrix aTransform);
		RtVoid concatTransform(RtMatrix aTransform);
		RtVoid perspective(RtFloat fov);
		RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
		RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
		RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
		RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

		RtToken spaceType() const { return m_spaceType; }
		void spaceType(RtToken aSpaceType) { m_spaceType = aSpaceType; }

		virtual RtVoid motionBegin(const CMotionState &state);
		virtual RtVoid motionEnd();
		virtual RtVoid motionSuspend();
		
		virtual RtVoid sample(RtFloat shutterTime, const TypeMotionTimes &times);
		virtual RtVoid sampleReset();
	}; // CTransformation

	/** @brief Create new attributes containers.
	 *
	 *  An instance of the CAttributesFactory is used to create new instances
	 *  of CAttributes containers. Renderers can supply an own factory to
	 *  create an expanded set of attributes.
	 */
	class CTransformationFactory
	{
	protected:
		/** @brief Factory method to get a new transformation set.
		 *
		 *  Must be overwritten to supply an own transformation set.
		 *
		 *  @return New transformation set.
		 */
		inline virtual CTransformation *newTransformationInstance()
		{
			return new CTransformation();
		}

		/** @brief Factory method to get a copy of an transformation set.
		 *
		 *  Must be overwritten to supply an own transformation set.
		 *
		 *  @param trans Transformation set to copy.
		 *  @return New transformation set as copy of @a trans.
		 */
		inline virtual CTransformation *newTransformationInstance(const CTransformation &trans)
		{
			return new CTransformation(trans);
		}

	public:
		/** @brief Virtual destructor.
		 */
		inline virtual ~CTransformationFactory() {}

		/** @brief Gets a new transformation set.
		 *  @return A new transformation set
		 */
		virtual CTransformation *newTransformation();

		/** @brief Gets a copy of an attribute set.
		 *  @param trans Transformation set to copy.
		 *  @return A new transformation set as copy of @a trans
		 */
		virtual CTransformation *newTransformation(const CTransformation &trans);

		/** @brief Deletes a transformation set delivered by this factory.
		 *
		 *  Because the destructor is virtual, this method don_t need to be overwritten.
		 *
		 *	@param trans Transformation set to delete, had to be constructed previously by this factory.
		 */
		inline virtual void deleteTransformation(CTransformation *trans)
		{
			if ( trans )
				delete trans;
		}
	}; // CTransformationFactory

}

#endif // _RICPP_RICONTEXT_TRANSFORMATON_H
