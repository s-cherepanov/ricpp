#ifndef _RICPP_RICONTEXT_RIMACROTRANSFORMS_H
#define _RICPP_RICONTEXT_RIMACROTRANSFORMS_H

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

/** @file rimacrotransforms.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for transformations, @see rimacro.h
 */
#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Transformations and coordinate systems
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Reset transformation to identity.
	 */
	class CRiIdentity : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiIdentity"; }
		inline virtual const char *className() const { return CRiIdentity::myClassName(); }


		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiIdentity(long aLineNo=-1) : TypeParent(aLineNo) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiIdentity(const CRiIdentity &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiIdentity(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }

		inline virtual void process(IRiRoot &ri)
		{
			ri.identity();
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiIdentity &operator=(const CRiIdentity &c)
		{
			if ( this == &c )
				return *this;

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiIdentity


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Set a transformation matirx.
	 */
	class CRiTransform : public CRManInterfaceCall {
	private:
		RtMatrix m_transform; ///< Transformation matrix.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiTransform"; }
		inline virtual const char *className() const { return CRiTransform::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  Assigns the identity matrix to the transformation matrix.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiTransform(long aLineNo = -1) : TypeParent(aLineNo)
		{
			memcpy(m_transform, RiIdentityMatrix, sizeof(RtMatrix));
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aTransform The transformation matrix
		 */
		inline CRiTransform(long aLineNo, const RtMatrix aTransform) : TypeParent(aLineNo)
		{
			memcpy(m_transform, aTransform, sizeof(RtMatrix));
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiTransform(const CRiTransform &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiTransform(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM; }

		/** @brief Gets the transformation matrix.
		 *
		 *  @retval aTransform The transformation matrix.
		 */
		inline void getTransform(RtMatrix aTransform) const
		{
			memcpy(aTransform, m_transform, sizeof(RtMatrix));
		}

		/** @brief Gets the transformation matrix.
		 *
		 *  @return The transformation matrix.
		 */
		const RtMatrix &transform() const
		{
			return m_transform;
		}
		RtMatrix &transform()
		{
			return m_transform;
		}
		
		/** @brief Sets the transformation matrix.
		 *
		 *  @param aTransform The transformation matrix.
		 */
		inline void transform(const RtMatrix aTransform)
		{
			memcpy(m_transform, aTransform, sizeof(RtMatrix));
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.transform(transform());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(transform());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiTransform &operator=(const CRiTransform &c)
		{
			if ( this == &c )
				return *this;

			transform(transform());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTransform


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Concats a transformation matrix.
	 */
	class CRiConcatTransform : public CRManInterfaceCall {
	private:
		RtMatrix m_transform; ///< Transformation matrix.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiConcatTransform"; }
		inline virtual const char *className() const { return CRiConcatTransform::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  Assigns the identity matrix to the transformation matrix.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiConcatTransform(long aLineNo = -1) : TypeParent(aLineNo)
		{
			memcpy(m_transform, RiIdentityMatrix, sizeof(RtMatrix));
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aTransform The transformation matrix
		 */
		inline CRiConcatTransform(long aLineNo, const RtMatrix aTransform) : TypeParent(aLineNo)
		{
			memcpy(m_transform, aTransform, sizeof(RtMatrix));
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiConcatTransform(const CRiConcatTransform &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiConcatTransform(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_CONCAT_TRANSFORM; }

		/** @brief Gets the transformation matrix.
		 *
		 *  @retval aTransform The transformation matrix.
		 */
		inline void getTransform(RtMatrix aTransform) const
		{
			memcpy(aTransform, m_transform, sizeof(RtMatrix));
		}

		/** @brief Gets the transformation matrix.
		 *
		 *  @return The transformation matrix.
		 */
		const RtMatrix &transform() const
		{
			return m_transform;
		}
		RtMatrix &transform()
		{
			return m_transform;
		}
		
		/** @brief Sets the transformation matrix.
		 *
		 *  @param aTransform The transformation matrix.
		 */
		inline void transform(const RtMatrix aTransform)
		{
			memcpy(m_transform, aTransform, sizeof(RtMatrix));
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.concatTransform(transform());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(transform());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiConcatTransform &operator=(const CRiConcatTransform &c)
		{
			if ( this == &c )
				return *this;

			transform(transform());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiConcatTransform


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Perspective transformation.
	 */
	class CRiPerspective : public CRManInterfaceCall {
	private:
		RtFloat m_fov; ///< Field of view
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiPerspective"; }
		inline virtual const char *className() const { return CRiPerspective::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aFov Field of view
		 */
		inline CRiPerspective(long aLineNo=-1, RtFloat aFov=90) : TypeParent(aLineNo), m_fov(aFov) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiPerspective(const CRiPerspective &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiPerspective(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_PERSPECTIVE; }

		/** @brief Gets the field of view.
		 *
		 *  @return The field of view.
		 */
		RtFloat fov() const
		{
			return m_fov;
		}

		/** @brief Sets the field of view.
		 *
		 *  @param aFov Field of view.
		 */
		void fov(RtFloat aFov)
		{
			m_fov = m_fov + 0;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.perspective(fov());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(fov());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiPerspective &operator=(const CRiPerspective &c)
		{
			if ( this == &c )
				return *this;

			fov(c.fov());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPerspective


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Translation
	 */
	class CRiTranslate : public CRManInterfaceCall {
	private:
		RtFloat m_dx, ///< Translation in direction x.
				m_dy, ///< Translation in direction y.
				m_dz; ///< Translation in direction z.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiTranslate"; }
		inline virtual const char *className() const { return CRiTranslate::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aDx Translation in direction x.
		 *  @param aDy Translation in direction y.
		 *  @param aDz Translation in direction z.
		 */
		inline CRiTranslate(long aLineNo=-1, RtFloat aDx=0, RtFloat aDy=0, RtFloat aDz=0) : TypeParent(aLineNo), m_dx(aDx), m_dy(aDy), m_dz(aDz) {}

		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiTranslate(const CRiTranslate &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiTranslate(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSLATE; }

		/** @brief Gets the translation in direction x.
		 *
		 *  @return The translation in direction x.
		 */
		inline RtFloat dx() const
		{
			return m_dx;
		}

		/** @brief Sets the translation in direction x.
		 *
		 *  @param aDx The translation in direction x.
		 */
		inline void dx(RtFloat aDx)
		{
			m_dx = aDx;
		}

		/** @brief Gets the translation in direction y.
		 *
		 *  @return The translation in direction y.
		 */
		inline RtFloat dy() const
		{
			return m_dy;
		}

		/** @brief Sets the translation in direction y.
		 *
		 *  @param aDy The translation in direction y.
		 */
		inline void dy(RtFloat aDy)
		{
			m_dy = aDy;
		}

		/** @brief Gets the translation in direction z.
		 *
		 *  @return The translation in direction z.
		 */
		inline RtFloat dz() const
		{
			return m_dz;
		}

		/** @brief Sets the translation in direction z.
		 *
		 *  @param aDz The translation in direction z.
		 */
		inline void dz(RtFloat aDz)
		{
			m_dz = aDz;
		}

		/** @brief Gets the translation data.
		 *
		 *  @retval aDx The translation in direction x.
		 *  @retval aDy The translation in direction y.
		 *  @retval aDz The translation in direction z.
		 */
		inline void get(RtFloat &aDx, RtFloat &aDy, RtFloat &aDz) const
		{
			aDx = m_dx;
			aDy = m_dy;
			aDz = m_dz;
		}

		/** @brief Sets the translation data.
		 *
		 *  @param aDx The translation in direction x.
		 *  @param aDy The translation in direction y.
		 *  @param aDz The translation in direction z.
		 */
		inline void set(RtFloat aDx, RtFloat aDy, RtFloat aDz)
		{
			m_dx = aDx;
			m_dy = aDy;
			m_dz = aDz;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.translate(dx(), dy(), dz());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(dx());
			ribWriter.putBlank();
			ribWriter.putValue(dy());
			ribWriter.putBlank();
			ribWriter.putValue(dz());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiTranslate &operator=(const CRiTranslate &c)
		{
			if ( this == &c )
				return *this;

			set(c.dx(), c.dy(), c.dz());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTranslate

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Rotation
	 */
	class CRiRotate : public CRManInterfaceCall {
	private:
		RtFloat m_angle, ///< Angle of rotation.
				m_dx,    ///< Rotation axis x coordiante.
				m_dy,    ///< Rotation axis z coordiante.
				m_dz;    ///< Rotation axis y coordiante.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiRotate"; }
		inline virtual const char *className() const { return CRiRotate::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param anAngle The rotation angle.
		 *  @param aDx The x coordiante of the rotation axis.
		 *  @param aDy The y coordiante of the rotation axis.
		 *  @param aDz The z coordiante of the rotation axis.
		 */
		inline CRiRotate(long aLineNo, RtFloat anAngle = 0, RtFloat aDx = 0, RtFloat aDy = 0, RtFloat aDz = 1) : TypeParent(aLineNo), m_angle(anAngle), m_dx(aDx), m_dy(aDy), m_dz(aDz) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiRotate(const CRiRotate &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiRotate(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ROTATE; }

		/** @brief Gets the rotation angle.
		 *
		 *  @return The rotation angle.
		 */
		inline RtFloat angle() const
		{
			return m_angle;
		}

		/** @brief Sets the rotation angle.
		 *
		 *  @param anAngle The rotation angle.
		 */
		inline void angle(RtFloat anAngle)
		{
			m_angle = anAngle;
		}

		/** @brief Gets the x coordiante of the rotation axis.
		 *
		 *  @return The x coordiante of the rotation axis.
		 */
		inline RtFloat dx() const
		{
			return m_dx;
		}

		/** @brief Sets the x coordiante of the rotation axis.
		 *
		 *  @param aDx The x coordiante of the rotation axis.
		 */
		inline void dx(RtFloat aDx)
		{
			m_dx = aDx;
		}

		/** @brief Gets the y coordiante of the rotation axis.
		 *
		 *  @return The y coordiante of the rotation axis.
		 */
		inline RtFloat dy() const
		{
			return m_dy;
		}

		/** @brief Sets the y coordiante of the rotation axis.
		 *
		 *  @param aDy The y coordiante of the rotation axis.
		 */
		inline void dy(RtFloat aDy)
		{
			m_dy = aDy;
		}

		/** @brief Gets the z coordiante of the rotation axis.
		 *
		 *  @return The z coordiante of the rotation axis.
		 */
		inline RtFloat dz() const
		{
			return m_dz;
		}

		/** @brief Sets the z coordiante of the rotation axis.
		 *
		 *  @param aDz The z coordiante of the rotation axis.
		 */
		inline void dz(RtFloat aDz)
		{
			m_dz = aDz;
		}

		/** @brief Gets the rotation data.
		 *
		 *  @retval anAngle The rotation angle.
		 *  @retval aDx The x coordiante of the rotation axis.
		 *  @retval aDy The y coordiante of the rotation axis.
		 *  @retval aDz The z coordiante of the rotation axis.
		 */
		inline void get(RtFloat &anAngle, RtFloat &aDx, RtFloat &aDy, RtFloat &aDz) const
		{
			anAngle = m_angle;
			aDx = m_dx;
			aDy = m_dy;
			aDz = m_dz;
		}

		/** @brief Sets the translation.
		 *
		 *  @param anAngle The rotation angle.
		 *  @param aDx The x coordiante of the rotation axis.
		 *  @param aDy The y coordiante of the rotation axis.
		 *  @param aDz The z coordiante of the rotation axis.
		 */
		inline void set(RtFloat anAngle, RtFloat aDx, RtFloat aDy, RtFloat aDz)
		{
			m_angle = anAngle;
			m_dx = aDx;
			m_dy = aDy;
			m_dz = aDz;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.rotate(angle(), dx(), dy(), dz());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(angle());
			ribWriter.putBlank();
			ribWriter.putValue(dx());
			ribWriter.putBlank();
			ribWriter.putValue(dy());
			ribWriter.putBlank();
			ribWriter.putValue(dz());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiRotate &operator=(const CRiRotate &c)
		{
			if ( this == &c )
				return *this;

			set(c.angle(), c.dx(), c.dy(), c.dz());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiRotate


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Scaling
	 */
	class CRiScale : public CRManInterfaceCall {
	private:
		RtFloat m_dx, ///< Scaling in direction x.
				m_dy, ///< Scaling in direction y.
				m_dz; ///< Scaling in direction z.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiScale"; }
		inline virtual const char *className() const { return CRiScale::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aDx Scaling in direction x.
		 *  @param aDy Scaling in direction y.
		 *  @param aDz Scaling in direction z.
		 */
		inline CRiScale(long aLineNo=-1, RtFloat aDx=1, RtFloat aDy=1, RtFloat aDz=1) : TypeParent(aLineNo), m_dx(aDx), m_dy(aDy), m_dz(aDz) {}

		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiScale(const CRiTranslate &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiScale(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SCALE; }

		/** @brief Gets the scaling in direction x.
		 *
		 *  @return The scaling in direction x.
		 */
		inline RtFloat dx() const
		{
			return m_dx;
		}

		/** @brief Sets the scaling in direction x.
		 *
		 *  @param aDx The scaling in direction x.
		 */
		inline void dx(RtFloat aDx)
		{
			m_dx = aDx;
		}

		/** @brief Gets the scaling in direction y.
		 *
		 *  @return The scaling in direction y.
		 */
		inline RtFloat dy() const
		{
			return m_dy;
		}

		/** @brief Sets the scaling in direction y.
		 *
		 *  @param aDy The scaling in direction y.
		 */
		inline void dy(RtFloat aDy)
		{
			m_dy = aDy;
		}

		/** @brief Gets the scaling in direction z.
		 *
		 *  @return The scaling in direction z.
		 */
		inline RtFloat dz() const
		{
			return m_dz;
		}

		/** @brief Sets the scaling in direction z.
		 *
		 *  @param aDz The scaling in direction z.
		 */
		inline void dz(RtFloat aDz)
		{
			m_dz = aDz;
		}

		/** @brief Gets the scaling data.
		 *
		 *  @retval aDx The scaling in direction x.
		 *  @retval aDy The scaling in direction y.
		 *  @retval aDz The scaling in direction z.
		 */
		inline void get(RtFloat &aDx, RtFloat &aDy, RtFloat &aDz) const
		{
			aDx = m_dx;
			aDy = m_dy;
			aDz = m_dz;
		}

		/** @brief Sets the scaling data.
		 *
		 *  @param aDx The scaling in direction x.
		 *  @param aDy The scaling in direction y.
		 *  @param aDz The scaling in direction z.
		 */
		inline void set(RtFloat aDx, RtFloat aDy, RtFloat aDz)
		{
			m_dx = aDx;
			m_dy = aDy;
			m_dz = aDz;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.scale(dx(), dy(), dz());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(dx());
			ribWriter.putBlank();
			ribWriter.putValue(dy());
			ribWriter.putBlank();
			ribWriter.putValue(dz());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiScale &operator=(const CRiScale &c)
		{
			if ( this == &c )
				return *this;

			set(c.dx(), c.dy(), c.dz());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiScale

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Skew transformation.
	 */
	class CRiSkew : public CRManInterfaceCall {
	private:
		RtFloat m_angle; ///< Skew angle
		RtFloat m_dx1,  ///< X coordiante axis 1
				m_dy1,  ///< Y coordiante axis 1
				m_dz1;  ///< Z coordiante axis 1
		RtFloat m_dx2,  ///< X coordiante axis 2
				m_dy2,  ///< Y coordiante axis 2
				m_dz2;  ///< Z coordiante axis 2
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiSkew"; }
		inline virtual const char *className() const { return CRiSkew::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param anAngle The skew angle.
		 *  @param aDx1 The x coordiante of the first skew axis.
		 *  @param aDy1 The y coordiante of the first skew axis.
		 *  @param aDz1 The z coordiante of the first skew axis.
		 *  @param aDx2 The x coordiante of the second skew axis.
		 *  @param aDy2 The y coordiante of the second skew axis.
		 *  @param aDz2 The z coordiante of the second skew axis.
		 */
		inline CRiSkew(long aLineNo=-1, RtFloat anAngle=0, RtFloat aDx1=0, RtFloat aDy1=0, RtFloat aDz1=1, RtFloat aDx2=0, RtFloat aDy2=1, RtFloat aDz2=0) : TypeParent(aLineNo), m_angle(anAngle), m_dx1(aDx1), m_dy1(aDy1), m_dz1(aDz1), m_dx2(aDx2), m_dy2(aDy2), m_dz2(aDz2) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiSkew(const CRiSkew &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiSkew(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SKEW; }

		inline virtual void process(IRiRoot &ri)
		{
			ri.skew(angle(), dx1(), dy1(), dz1(), dx2(), dy2(), dz2());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		/** @brief Gets the skew angle.
		 *
		 *  @return The skew angle.
		 */
		inline RtFloat angle() const
		{
			return m_angle;
		}

		/** @brief Sets the skew angle.
		 *
		 *  @param anAngle The skew angle.
		 */
		inline void angle(RtFloat anAngle)
		{
			m_angle = anAngle;
		}

		/** @brief Gets the x coordiante of the first skew axis.
		 *
		 *  @return The x coordiante of the first skew axis.
		 */
		inline RtFloat dx1() const
		{
			return m_dx1;
		}

		/** @brief Sets the x coordiante of the first skew axis.
		 *
		 *  @param aDx1 The x coordiante of the first skew axis.
		 */
		inline void dx1(RtFloat aDx1)
		{
			m_dx1 = aDx1;
		}

		/** @brief Gets the y coordiante of the first skew axis.
		 *
		 *  @return The y coordiante of the first skew axis.
		 */
		inline RtFloat dy1() const
		{
			return m_dy1;
		}

		/** @brief Sets the y coordiante of the first skew axis.
		 *
		 *  @param aDy1 The y coordiante of the first skew axis.
		 */
		inline void dy1(RtFloat aDy1)
		{
			m_dy1 = aDy1;
		}

		/** @brief Gets the z coordiante of the first skew axis.
		 *
		 *  @return The z coordiante of the first skew axis.
		 */
		inline RtFloat dz1() const
		{
			return m_dz1;
		}

		/** @brief Sets the z coordiante of the first skew axis.
		 *
		 *  @param aDz1 The z coordiante of the first skew axis.
		 */
		inline void dz1(RtFloat aDz1)
		{
			m_dz1 = aDz1;
		}

		/** @brief Gets the x coordiante of the second skew axis.
		 *
		 *  @return The x coordiante of the second skew axis.
		 */
		inline RtFloat dx2() const
		{
			return m_dx2;
		}

		/** @brief Sets the x coordiante of the second skew axis.
		 *
		 *  @param aDx2 The x coordiante of the second skew axis.
		 */
		inline void dx2(RtFloat aDx2)
		{
			m_dx2 = aDx2;
		}

		/** @brief Gets the y coordiante of the second skew axis.
		 *
		 *  @return The y coordiante of the second skew axis.
		 */
		inline RtFloat dy2() const
		{
			return m_dy2;
		}

		/** @brief Sets the y coordiante of the second skew axis.
		 *
		 *  @param aDy2 The y coordiante of the second skew axis.
		 */
		inline void dy2(RtFloat aDy2)
		{
			m_dy2 = aDy2;
		}

		/** @brief Gets the z coordiante of the second skew axis.
		 *
		 *  @return The z coordiante of the second skew axis.
		 */
		inline RtFloat dz2() const
		{
			return m_dz2;
		}

		/** @brief Sets the z coordiante of the second skew axis.
		 *
		 *  @param aDz2 The z coordiante of the second skew axis.
		 */
		inline void dz2(RtFloat aDz2)
		{
			m_dz2 = aDz2;
		}

		/** @brief Gets the rotation data.
		 *
		 *  @retval anAngle The rotation angle.
		 *  @retval aDx1 The x coordiante of the first skew axis.
		 *  @retval aDy1 The y coordiante of the first skew axis.
		 *  @retval aDz1 The z coordiante of the first skew axis.
		 *  @retval aDx2 The x coordiante of the second skew axis.
		 *  @retval aDy2 The y coordiante of the second skew axis.
		 *  @retval aDz2 The z coordiante of the second skew axis.
		 */
		inline void get(RtFloat &anAngle, RtFloat &aDx1, RtFloat &aDy1, RtFloat &aDz1, RtFloat &aDx2, RtFloat &aDy2, RtFloat &aDz2) const
		{
			anAngle = m_angle;
			aDx1 = m_dx1;
			aDy1 = m_dy1;
			aDz1 = m_dz1;
			aDx2 = m_dx2;
			aDy2 = m_dy2;
			aDz2 = m_dz2;
		}

		/** @brief Sets the translation.
		 *
		 *  @param anAngle The rotation angle.
		 *  @param aDx1 The x coordiante of the first skew axis.
		 *  @param aDy1 The y coordiante of the first skew axis.
		 *  @param aDz1 The z coordiante of the first skew axis.
		 *  @param aDx2 The x coordiante of the second skew axis.
		 *  @param aDy2 The y coordiante of the second skew axis.
		 *  @param aDz2 The z coordiante of the second skew axis.
		 */
		inline void set(RtFloat anAngle, RtFloat aDx1, RtFloat aDy1, RtFloat aDz1, RtFloat aDx2, RtFloat aDy2, RtFloat aDz2)
		{
			m_angle = anAngle;
			m_dx1 = aDx1;
			m_dy1 = aDy1;
			m_dz1 = aDz1;
			m_dx2 = aDx2;
			m_dy2 = aDy2;
			m_dz2 = aDz2;
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(angle());
			ribWriter.putBlank();
			ribWriter.putValue(dx1());
			ribWriter.putBlank();
			ribWriter.putValue(dy1());
			ribWriter.putBlank();
			ribWriter.putValue(dz1());
			ribWriter.putBlank();
			ribWriter.putValue(dx2());
			ribWriter.putBlank();
			ribWriter.putValue(dy2());
			ribWriter.putBlank();
			ribWriter.putValue(dz2());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiSkew &operator=(const CRiSkew &c)
		{
			if ( this == &c )
				return *this;

			set(c.angle(), c.dx1(), c.dy1(), c.dz1(), c.dx2(), c.dy2(), c.dz2());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSkew

	///////////////////////////////////////////////////////////////////////////////
	class CRiScopedCoordinateSystem : public CRManInterfaceCall {
	private:
		RtToken m_space; ///< Name of the coordinate system.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiScopedCoordinateSystem"; }
		inline virtual const char *className() const { return CRiScopedCoordinateSystem::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aSpace Name of the coordinate system.
		 */
		inline CRiScopedCoordinateSystem(
			long aLineNo = -1,
			RtToken aSpace = RI_NULL)
			: TypeParent(aLineNo), m_space(aSpace)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiScopedCoordinateSystem(const CRiScopedCoordinateSystem &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiScopedCoordinateSystem(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SCOPED_COORDINATE_SYSTEM; }

		/** @brief Gets the name of the coordinate system.
		 *
		 *  @return The name of the coordinate system.
		 */
		inline RtToken space() const
		{
			return m_space;
		}

		/** @brief Sets the name of the coordinate system.
		 *
		 *  @param aSpace The name of the coordinate system.
		 */
		inline void space(RtToken aSpace)
		{
			m_space = aSpace;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.scopedCoordinateSystem(space());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(space());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiScopedCoordinateSystem &operator=(const CRiScopedCoordinateSystem &c)
		{
			if ( this == &c )
				return *this;

			space(c.space());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiScopedCoordinateSystem

	///////////////////////////////////////////////////////////////////////////////
	class CRiCoordinateSystem : public CRManInterfaceCall {
	private:
		RtToken m_space; ///< Name of the coordinate system.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiCoordinateSystem"; }
		inline virtual const char *className() const { return CRiCoordinateSystem::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aSpace Name of the coordinate system.
		 */
		inline CRiCoordinateSystem(
			long aLineNo = -1,
			RtToken aSpace = RI_NULL)
			: TypeParent(aLineNo), m_space(aSpace)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiCoordinateSystem(const CRiCoordinateSystem &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiCoordinateSystem(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }

		/** @brief Gets the name of the coordinate system.
		 *
		 *  @return The name of the coordinate system.
		 */
		inline RtToken space() const
		{
			return m_space;
		}

		/** @brief Sets the name of the coordinate system.
		 *
		 *  @param aName The name of the coordinate system.
		 */
		inline void space(RtToken aSpace)
		{
			m_space = aSpace;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.coordinateSystem(space());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(space());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiCoordinateSystem &operator=(const CRiCoordinateSystem &c)
		{
			if ( this == &c )
				return *this;

			space(c.space());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiCoordinateSystem

	///////////////////////////////////////////////////////////////////////////////
	class CRiCoordSysTransform : public CRManInterfaceCall {
	private:
		RtToken m_space; ///< Name of the coordinate system.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiCoordSysTransform"; }
		inline virtual const char *className() const { return CRiCoordSysTransform::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aSpace Name of the coordinate system.
		 */
		inline CRiCoordSysTransform(
			long aLineNo = -1,
			RtToken aSpace = RI_NULL)
			: TypeParent(aLineNo), m_space(aSpace)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiCoordSysTransform(const CRiCoordSysTransform &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiCoordSysTransform(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_COORD_SYS_TRANSFORM; }

		/** @brief Gets the name of the coordinate system.
		 *
		 *  @return The name of the coordinate system.
		 */
		inline RtToken space() const
		{
			return m_space;
		}

		/** @brief Sets the name of the coordinate system.
		 *
		 *  @param aSpace The name of the coordinate system.
		 */
		inline void space(RtToken aSpace)
		{
			m_space = aSpace;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.coordSysTransform(space());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(space());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiCoordSysTransform &operator=(const CRiCoordSysTransform &c)
		{
			if ( this == &c )
				return *this;

			space(c.space());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiCoordSysTransform

	///////////////////////////////////////////////////////////////////////////////
	/** @brief transformation of points
	 */
	class CRiTransformPoints : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	private:
		RtToken m_fromspace, ///< Space from which the coordiantes are transformed.
				m_tospace;   ///< Space to which the coordiantes are transformed.
		RtInt m_npoints;     ///< Number of Points
		RtPoint *m_points;   ///< Address of the result

		std::vector<RtFloat> m_pointsvector; ///< The source points (copy)

		/** @brief Sets the member variables (used by assignment.
		 *
		 *  @param aFromSpace The space from which the coordiantes are transformed.
		 *  @param aToSpace   The space to which the coordiantes are transformed.
		 *  @param aNPoints   The number of points.
		 *  @param aPoints    The pointer to the result.
		 *  @param aPointsVector Copy of the original points.
		 */
		inline void set(
			RtToken aFromSpace, RtToken aToSpace,
			RtInt aNPoints, RtPoint *aPoints,
			const std::vector<RtFloat> &aPointsVector)
		{
			m_fromspace = aFromSpace;
			m_tospace = aToSpace;
			m_npoints = aNPoints;
			m_points = aPoints;
			if ( aPoints == 0 )
				aNPoints = 0;
			m_pointsvector = aPointsVector;
		}

		/** @brief Resets the points to their original values.
		 */
		inline void reset()
		{
			if ( m_points == 0 || m_npoints <= 0 )
				return;
			memcpy(m_points, &(m_pointsvector[0]), m_npoints*sizeof(RtPoint));
		}
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiTransformPoints"; }
		inline virtual const char *className() const { return CRiTransformPoints::myClassName(); }


		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  The default constructor sets no points.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiTransformPoints(long aLineNo=-1) : TypeParent(aLineNo)
		{
			m_fromspace = RI_NULL;
			m_tospace = RI_NULL;
			m_npoints = 0;
			m_points = 0;
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo    The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aFromSpace The space from which the coordiantes are transformed.
		 *  @param aToSpace   The space to which the coordiantes are transformed.
		 *  @param aNPoints   The number of points.
		 *  @param aPoints    The pointer to the result.
		 */
		inline CRiTransformPoints(
			long aLineNo,
			RtToken aFromSpace, RtToken aToSpace,
			RtInt aNPoints, RtPoint *aPoints)
			: TypeParent(aLineNo)
		{
			set(aFromSpace, aToSpace, aNPoints, aPoints);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiTransformPoints(const CRiTransformPoints &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiTransformPoints(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_POINTS; }

		/** @brief Gets the space from which the coordiantes are transformed.
		 *
		 *  @return The space from which the coordiantes are transformed.
		 */
		RtToken fromSpace() const
		{
			return m_fromspace;
		}

		/** @brief Gets the space to which the coordiantes are transformed.
		 *
		 *  @return The space to which the coordiantes are transformed.
		 */
		RtToken toSpace() const
		{
			return m_tospace;
		}

		/** @brief Sets the member variables.
		 *
		 *  @param aFromSpace The space from which the coordiantes are transformed.
		 *  @param aToSpace   The space to which the coordiantes are transformed.
		 *  @param aNPoints   The number of points.
		 *  @param aPoints    The pointer to the result.
		 */
		inline void set(
			RtToken aFromSpace, RtToken aToSpace,
			RtInt aNPoints, RtPoint *aPoints)
		{
			m_fromspace = aFromSpace;
			m_tospace = aToSpace;
			m_npoints = aNPoints;
			m_points = aPoints;
			if ( aPoints == 0 )
				aNPoints = 0;
			m_pointsvector.resize(aNPoints*3);
			if ( aNPoints > 0 ) {
				m_pointsvector.assign(&(aPoints[0][0]), &(aPoints[aNPoints][0]));
			}
		}

		/** @brief Gets the number of points.
		 *
		 *  @return The number of points.
		 */
		RtInt nPoints() const
		{
			return m_npoints;
		}

		/** @brief Gets the pointer to the result.
		 *
		 *  @return The pointer to the result.
		 */
		RtPoint *points() const
		{
			return m_points;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.transformPoints(fromSpace(), toSpace(), nPoints(), points());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Has no Rib binding
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiTransformPoints &operator=(const CRiTransformPoints &c)
		{
			if ( this == &c )
				return *this;

			set(c.fromSpace(), c.toSpace(), c.nPoints(), c.points(), c.m_pointsvector);

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTransformPoints

}
#endif //_RICPP_RICONTEXT_RIMACROTRANSFORMS_H
