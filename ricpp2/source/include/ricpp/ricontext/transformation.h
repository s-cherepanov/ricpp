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
	class CTransformationFactory;

	/** @brief Class with the transformation stack and composit transformation matrix
	 */
	class CTransformation {
		friend class CTransformationFactory;
		
		//! Space type of the coordinate system (current, world, camera, screen, raster, etc.)
		RtToken m_spaceType;
		
		//! A simple counter for additional transform blocks stored for this at a stack (used for detailrange)
		unsigned long m_storeCounter;

		CTransformationFactory *m_factory;

	public:
		const CTransformationFactory *factory() const { return m_factory; }

	protected:
		void factory(CTransformationFactory *aFactory) { m_factory = aFactory; }
		
	protected:

		class IMovedTransform {
		public:
			inline IMovedTransform() {}
			inline virtual ~IMovedTransform() {}
			virtual EnumRequests reqType() const = 0;
			
			virtual IMovedTransform *duplicate() const = 0;

			virtual void clear() = 0;
			virtual void fill(RtInt n) = 0;
			
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse) = 0;
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse) = 0;
		}; // IMovedTransform
		
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
			inline virtual EnumRequests reqType() const { return REQ_ROTATE; }

			virtual IMovedTransform *duplicate() const { return new CMovedRotate(*this); }
			
			CMovedRotate &operator=(const CMovedRotate &o);

			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedRotate
			
		class CMovedMatrix : public IMovedTransform {
		public:
			std::vector<RtFloat> m_transform;
			bool m_concat;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedMatrix()
			{
				m_concat = false;
				m_motionBegin = m_motionEnd = 0;
			}
			
			inline CMovedMatrix(const CMovedMatrix &o)
			{
				*this = o;
			}
			
			inline virtual ~CMovedMatrix() {}
			inline virtual EnumRequests reqType() const { return REQ_TRANSFORM; }
			
			virtual IMovedTransform *duplicate() const { return new CMovedMatrix(*this); }
			
			CMovedMatrix &operator=(const CMovedMatrix &o);		
			
			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(const RtMatrix transform, bool concat, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedMatrix

		class CMovedScale : public IMovedTransform {
		public:
			struct SScale {
				RtFloat m_dx;
				RtFloat m_dy;
				RtFloat m_dz;
			};
			
			std::vector<SScale> m_scale;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedScale()
			{
				m_motionBegin = m_motionEnd = 0;
			}
			
			inline CMovedScale(const CMovedScale &o)
			{
				*this = o;
			}
			
			inline virtual ~CMovedScale() {}
			inline virtual EnumRequests reqType() const { return REQ_SCALE; }
			
			virtual IMovedTransform *duplicate() const { return new CMovedScale(*this); }
			
			CMovedScale &operator=(const CMovedScale &o);
			
			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedScale

		class CMovedTranslate : public IMovedTransform {
		public:
			struct STranslate {
				RtFloat m_dx;
				RtFloat m_dy;
				RtFloat m_dz;
			};
			
			std::vector<STranslate> m_translate;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedTranslate()
			{
				m_motionBegin = m_motionEnd = 0;
			}
			
			inline CMovedTranslate(const CMovedTranslate &o)
			{
				*this = o;
			}
			
			inline virtual ~CMovedTranslate() {}
			inline virtual EnumRequests reqType() const { return REQ_TRANSLATE; }
			
			virtual IMovedTransform *duplicate() const { return new CMovedTranslate(*this); }
			
			CMovedTranslate &operator=(const CMovedTranslate &o);
			
			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedTranslate
		
		class CMovedPerspective : public IMovedTransform {
		public:
			std::vector<RtFloat> m_fov;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedPerspective()
			{
				m_motionBegin = m_motionEnd = 0;
			}
			
			inline CMovedPerspective(const CMovedPerspective &o)
			{
				*this = o;
			}
			
			inline virtual ~CMovedPerspective() {}
			inline virtual EnumRequests reqType() const { return REQ_PERSPECTIVE; }
			
			virtual IMovedTransform *duplicate() const { return new CMovedPerspective(*this); }
			
			CMovedPerspective &operator=(const CMovedPerspective &o);
			
			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat fov, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedPerspective
		
		class CMovedSkew : public IMovedTransform {
		public:
			struct SSkew {
				RtFloat m_angle;
				RtFloat m_dx1;
				RtFloat m_dy1;
				RtFloat m_dz1;
				RtFloat m_dx2;
				RtFloat m_dy2;
				RtFloat m_dz2;
			};

			std::vector<SSkew> m_skew;
			unsigned long m_motionBegin, m_motionEnd;
			
			inline CMovedSkew()
			{
				m_motionBegin = m_motionEnd = 0;
			}
			
			inline CMovedSkew(const CMovedSkew &o)
			{
				*this = o;
			}
			
			inline virtual ~CMovedSkew() {}
			inline virtual EnumRequests reqType() const { return REQ_SKEW; }
			
			virtual IMovedTransform *duplicate() const { return new CMovedSkew(*this); }
			
			CMovedSkew &operator=(const CMovedSkew &o);
			
			virtual void clear();
			virtual void fill(RtInt n);
			
			void set(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse);
			virtual void sampleReset(CMatrix3D &ctm, CMatrix3D &inverse);
		}; // CMovedSkew
		
		//! false, did an operation that could not calculate the inverse.
		bool m_isValid;
		bool m_isValid_onMotionStart;
		
		//! Composit transformation matrix
		CMatrix3D m_CTM;
		CMatrix3D m_CTM_onMotionStart;
		
		//! Inverse of the composit transformation matrix
		CMatrix3D m_inverseCTM;
		CMatrix3D m_inverseCTM_onMotionStart;

		
		//! Defered transformations (motion block)
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

		RtToken spaceType() const { return m_spaceType; }
		void spaceType(RtToken aSpaceType) { m_spaceType = aSpaceType; }

		inline CMatrix3D &getCTM()
		{
			return m_CTM;
		}

		inline const CMatrix3D &getCTM() const
		{
			return m_CTM;
		}

		inline CMatrix3D &getInverseCTM()
		{
			return m_inverseCTM;
		}

		inline const CMatrix3D &getInverseCTM() const
		{
			return m_inverseCTM;
		}

		inline bool isValid() const { return m_isValid; }

		virtual RtVoid identity(void);
		virtual RtVoid transform(RtMatrix aTransform);
		virtual RtVoid concatTransform(RtMatrix aTransform);
		virtual RtVoid perspective(RtFloat fov);
		virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
		virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
		virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
		virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

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
			CTransformation *t = new CTransformation();
			if ( t )
				t->factory(this);
			return t;
		}

		/** @brief Factory method to get a copy of an transformation set.
		 *
		 *  Must be overwritten to supply an own transformation set.
		 *
		 *  @param trans Transformation set to copy.
		 *  @return New transformation set as copy of @a trans.
		 */
		inline virtual CTransformation *newTransformationInstance(const CTransformation &trans) const
		{
			return new CTransformation(trans);
		}

		/** @brief Deletes a transformation set delivered by this factory.
		 *
		 *  Because the destructor is virtual, this method don_t need to be overwritten.
		 *
		 *	@param trans Transformation set to delete, had to be constructed previously by this factory.
		 */
		inline virtual void deleteTransformationInstance(CTransformation *trans) const
		{
			if ( trans )
				delete trans;
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
		CTransformation *newTransformation(const CTransformation &trans) const;
		static void deleteTransformation(CTransformation *trans);

	}; // CTransformationFactory

}

#endif // _RICPP_RICONTEXT_TRANSFORMATON_H
