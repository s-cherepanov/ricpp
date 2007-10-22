#ifndef _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H
#define _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROBASE_H
#include "ricpp/renderstate/rimacrobase.h"
#endif // _RICPP_RENDERSTATE_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Transformations and coordinate systems
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiIdentity : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiIdentity"; }
	inline virtual const char *className() const { return CRiIdentity::myClassName(); }

	inline CRiIdentity(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIdentity();
		ri.doIdentity();
	}
	inline CRiIdentity &operator=(const CRiIdentity &) {
		return *this;
	}
}; // CRiIdentity

///////////////////////////////////////////////////////////////////////////////
class CRiTransform : public CRManInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "CRiTransform"; }
	inline virtual const char *className() const { return CRiTransform::myClassName(); }

	inline CRiTransform(long aLineNo, RtMatrix transform) : CRManInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransform(m_transform);
		ri.doTransform(m_transform);
	}
	inline CRiTransform &operator=(const CRiTransform &) {
		return *this;
	}
}; // CRiTransform

///////////////////////////////////////////////////////////////////////////////
class CRiConcatTransform : public CRManInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "CRiConcatTransform"; }
	inline virtual const char *className() const { return CRiConcatTransform::myClassName(); }

	inline CRiConcatTransform(long aLineNo, RtMatrix transform) : CRManInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CONCAT_TRANSFORM; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preConcatTransform(m_transform);
		ri.doConcatTransform(m_transform);
	}
	inline CRiConcatTransform &operator=(const CRiConcatTransform &) {
		return *this;
	}
}; // CRiConcatTransform

///////////////////////////////////////////////////////////////////////////////
class CRiPerspective : public CRManInterfaceCall {
protected:
	RtFloat m_fov;
public:
	inline static const char *myClassName(void) { return "CRiPerspective"; }
	inline virtual const char *className() const { return CRiPerspective::myClassName(); }

	inline CRiPerspective(long aLineNo, RtFloat fov) : CRManInterfaceCall(aLineNo), m_fov(fov) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PERSPECTIVE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePerspective(m_fov);
		ri.doPerspective(m_fov);
	}
	inline CRiPerspective &operator=(const CRiPerspective &) {
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

	inline CRiTranslate(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
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
class CRiDeformation : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiDeformation"; }
	inline virtual const char *className() const { return CRiDeformation::myClassName(); }

	inline CRiDeformation(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DEFORMATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDeformation(m_name.c_str(), parameters());
		ri.doDeformation(m_name.c_str(), parameters());
	}
	inline CRiDeformation &operator=(const CRiDeformation &) {
		return *this;
	}
}; // CRiDeformation

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
// ????
class CRiTransformPoints : public CRManInterfaceCall {
protected:
	std::string m_fromSpace, m_toSpace;
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
		RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, m_points);
		p = p;
	}
	inline CRiTransformPoints &operator=(const CRiTransformPoints &) {
		return *this;
	}
}; // CRiTransformPoints

}
#endif //_RICPP_RENDERSTATE_RIMACROTRANSFORMS_H
