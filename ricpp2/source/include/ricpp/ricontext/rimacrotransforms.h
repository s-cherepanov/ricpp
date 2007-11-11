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

/** @file rimacrotransform.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for transformations, \see rimacro.h
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiIdentity(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiIdentity(const CRiIdentity &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiIdentity()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiIdentity(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIdentity();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doIdentity();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postIdentity();
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiIdentity

///////////////////////////////////////////////////////////////////////////////
/** @brief Set a transformation matirx.
 */
class CRiTransform : public CRManInterfaceCall {
private:
	RtMatrix m_transform; //!< Transformation matrix.
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  Assigns the identity matrix to the transformation matrix.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiTransform(long aLineNo = -1) : CRManInterfaceCall(aLineNo)
	{
		memcpy(m_transform, RiIdentityMatrix, sizeof(RtMatrix));
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aTransform The transformation matrix
	 */
	inline CRiTransform(long aLineNo, const RtMatrix aTransform) : CRManInterfaceCall(aLineNo)
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

	/** @brief Destructor.
	 */
	inline virtual ~CRiTransform()
	{
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

	/** @brief Sets the transformation matrix.
	 *
	 *  @param aTransform The transformation matrix.
	 */
	inline void transform(const RtMatrix aTransform)
	{
		memcpy(m_transform, aTransform, sizeof(RtMatrix));
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransform(m_transform);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTransform(m_transform);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTransform(m_transform);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTransform

///////////////////////////////////////////////////////////////////////////////
/** @brief Concats a transformation matrix.
 */
class CRiConcatTransform : public CRManInterfaceCall {
private:
	RtMatrix m_transform; //!< Transformation matrix.

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  Assigns the identity matrix to the transformation matrix.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiConcatTransform(long aLineNo = -1) : CRManInterfaceCall(aLineNo)
	{
		memcpy(m_transform, RiIdentityMatrix, sizeof(RtMatrix));
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aTransform The transformation matrix
	 */
	inline CRiConcatTransform(long aLineNo, const RtMatrix aTransform) : CRManInterfaceCall(aLineNo)
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

	/** @brief Destructor.
	 */
	inline virtual ~CRiConcatTransform()
	{
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

	/** @brief Sets the transformation matrix.
	 *
	 *  @param aTransform The transformation matrix.
	 */
	inline void transform(const RtMatrix aTransform)
	{
		memcpy(m_transform, aTransform, sizeof(RtMatrix));
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransform(m_transform);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTransform(m_transform);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTransform(m_transform);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiConcatTransform

///////////////////////////////////////////////////////////////////////////////
/** @brief Perspective transformation.
 */
class CRiPerspective : public CRManInterfaceCall {
private:
	RtFloat m_fov; //!< Field of view
public:
	inline static const char *myClassName(void) { return "CRiPerspective"; }
	inline virtual const char *className() const { return CRiPerspective::myClassName(); }

	inline CRiPerspective(long aLineNo=-1, RtFloat aFov=90) : CRManInterfaceCall(aLineNo), m_fov(aFov) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PERSPECTIVE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePerspective(m_fov);
		ri.doPerspective(m_fov);
	}
	inline CRiPerspective &operator=(const CRiPerspective &c)
	{
		return *this;
	}
}; // CRiPerspective

///////////////////////////////////////////////////////////////////////////////
class CRiTranslate : public CRManInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiTranslate"; }
	inline virtual const char *className() const { return CRiTranslate::myClassName(); }

	inline CRiTranslate(long aLineNo=-1, RtFloat aDx=0, RtFloat aDy=0, RtFloat aDz=0) : CRManInterfaceCall(aLineNo), m_dx(aDx), m_dy(aDy), m_dz(aDz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSLATE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTranslate(m_dx, m_dy, m_dz);
		ri.doTranslate(m_dx, m_dy, m_dz);
	}
	inline CRiTranslate &operator=(const CRiTranslate &) {
		return *this;
	}
}; // CRiTranslate

///////////////////////////////////////////////////////////////////////////////
class CRiRotate : public CRManInterfaceCall {
protected:
	RtFloat m_angle, m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiRotate"; }
	inline virtual const char *className() const { return CRiRotate::myClassName(); }

	inline CRiRotate(long aLineNo, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_angle(angle), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ROTATE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preRotate(m_angle, m_dx, m_dy, m_dz);
		ri.doRotate(m_angle, m_dx, m_dy, m_dz);
	}
	inline CRiRotate &operator=(const CRiRotate &) {
		return *this;
	}
}; // CRiRotate

///////////////////////////////////////////////////////////////////////////////
class CRiScale : public CRManInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiScale"; }
	inline virtual const char *className() const { return CRiScale::myClassName(); }

	inline CRiScale(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SCALE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preScale(m_dx, m_dy, m_dz);
		ri.doScale(m_dx, m_dy, m_dz);
	}
	inline CRiScale &operator=(const CRiScale &) {
		return *this;
	}
}; // CRiScale

///////////////////////////////////////////////////////////////////////////////
class CRiSkew : public CRManInterfaceCall {
protected:
	RtFloat m_angle;
	RtFloat m_dx1, m_dy1, m_dz1;
	RtFloat m_dx2, m_dy2, m_dz2;
public:
	inline static const char *myClassName(void) { return "CRiSkew"; }
	inline virtual const char *className() const { return CRiSkew::myClassName(); }

	inline CRiSkew(long aLineNo, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) : CRManInterfaceCall(aLineNo), m_angle(angle), m_dx1(dx1), m_dy1(dy1), m_dz1(dz1), m_dx2(dx2), m_dy2(dy2), m_dz2(dz2) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SKEW; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSkew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2);
		ri.doSkew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2);
	}
	inline CRiSkew &operator=(const CRiSkew &) {
		return *this;
	}
}; // CRiSkew

///////////////////////////////////////////////////////////////////////////////
/** @brief Deformation
 */
class CRiDeformation : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Name of the deformation as token.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDeformation"; }
	inline virtual const char *className() const { return CRiDeformation::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the deformation as atomized string.
	 */
	inline CRiDeformation(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the defformation as atomized string.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiDeformation(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the deformation as atomized string.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiDeformation(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDeformation(const CRiDeformation &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDeformation()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDeformation(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DEFORMATION; }

	/** @brief Gets the name of the deformation as atomized string.
	 *
	 *  @return The name of the deformation as atomized string.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the deformation as atomized string.
	 *
	 *  @param aName The name of the deformation as atomized string.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDeformation(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDeformation(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDeformation(m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDeformation &operator=(const CRiDeformation &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDeformation

///////////////////////////////////////////////////////////////////////////////
class CRiScopedCoordinateSystem : public CRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiScopedCoordinateSystem"; }
	inline virtual const char *className() const { return CRiScopedCoordinateSystem::myClassName(); }

	inline CRiScopedCoordinateSystem(long aLineNo, const char *name) : CRManInterfaceCall(aLineNo), m_name(name) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SCOPED_COORDINATE_SYSTEM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preScopedCoordinateSystem(m_name.c_str());
		ri.doScopedCoordinateSystem(m_name.c_str());
	}
	inline CRiScopedCoordinateSystem &operator=(const CRiScopedCoordinateSystem &) {
		return *this;
	}
}; // CRiScopedCoordinateSystem

///////////////////////////////////////////////////////////////////////////////
class CRiCoordinateSystem : public CRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiCoordinateSystem"; }
	inline virtual const char *className() const { return CRiCoordinateSystem::myClassName(); }

	inline CRiCoordinateSystem(long aLineNo, const char *name) : CRManInterfaceCall(aLineNo), m_name(name) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCoordinateSystem(m_name.c_str());
		ri.doCoordinateSystem(m_name.c_str());
	}
	inline CRiCoordinateSystem &operator=(const CRiCoordinateSystem &) {
		return *this;
	}
}; // CRiCoordinateSystem

///////////////////////////////////////////////////////////////////////////////
class CRiCoordSysTransform : public CRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiCoordSysTransform"; }
	inline virtual const char *className() const { return CRiCoordSysTransform::myClassName(); }

	inline CRiCoordSysTransform(long aLineNo, const char *name) : CRManInterfaceCall(aLineNo), m_name(name) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCoordSysTransform(m_name.c_str());
		ri.doCoordSysTransform(m_name.c_str());
	}
	inline CRiCoordSysTransform &operator=(const CRiCoordSysTransform &) {
		return *this;
	}
}; // CRiCoordSysTransform

///////////////////////////////////////////////////////////////////////////////
/** @brief transfoermation of points
 *  @todo May not work as macro?
 */
class CRiTransformPoints : public CRManInterfaceCall {
protected:
	RtToken m_fromSpace, m_toSpace;
	RtInt m_nPoints;
	RtPoint *m_points;
	// std::vector<RtFloat> m_fromPoints;
	// std::vector<RtFloat> m_toPoints;
public:
	inline static const char *myClassName(void) { return "CRiTransformPoints"; }
	inline virtual const char *className() const { return CRiTransformPoints::myClassName(); }

	inline CRiTransformPoints(long aLineNo, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points) : CRManInterfaceCall(aLineNo), m_fromSpace(fromspace), m_toSpace(tospace), m_nPoints(npoints), m_points(points) {
	/*
	m_fromPoints.resize(m_npoints*3);
	m_toPoints.resize(m_npoints*3);
	if ( points )
	m_fromPoints.assign(&m_points[0][0], (&m_points[0][0])+m_npoints*3);
		*/
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
	/*
	m_toPoints = m_fromPoints;
	RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, (RtPoint *)&m_toPoints[0]);
		*/
		// RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, m_points);
		// p = p;
	}
	inline CRiTransformPoints &operator=(const CRiTransformPoints &) {
		return *this;
	}
}; // CRiTransformPoints

}
#endif //_RICPP_RICONTEXT_RIMACROTRANSFORMS_H
