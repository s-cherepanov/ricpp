#ifndef _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H
#define _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H

#ifndef _RICPP_RENDERSTATE_RIMACROBASE_H
#include "ricpp/renderstate/rimacrobase.h"
#endif // _RICPP_RENDERSTATE_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Attributes
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiAttribute : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiAttribute"; }
	inline virtual const char *className() const { return CRiAttribute::myClassName(); }

	inline CRiAttribute(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiAttribute(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAttribute(m_name.c_str(), parameters());
		ri.doAttribute(m_name.c_str(), parameters());
	}
	inline CRiAttribute &operator=(const CRiAttribute &) {
		return *this;
	}
}; // CRiAttribute

///////////////////////////////////////////////////////////////////////////////
class CRiColor : public CRManInterfaceCall {
protected:
	std::vector<RtFloat> m_color;
public:
	inline static const char *myClassName(void) { return "CRiColor"; }
	inline virtual const char *className() const { return CRiColor::myClassName(); }

	inline CRiColor(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		RtInt i;
		for ( i=0; i<curColorDescr.colorSamples(); ++i )
			m_color.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preColor(m_color.empty() ? 0 : &m_color[0]);
		ri.doColor(m_color.empty() ? 0 : &m_color[0]);
	}
	inline CRiColor &operator=(const CRiColor &) {
		return *this;
	}
}; // CRiColor

///////////////////////////////////////////////////////////////////////////////
class CRiOpacity : public CRManInterfaceCall {
protected:
	std::vector<RtFloat> m_opacity;
public:
	inline static const char *myClassName(void) { return "CRiOpacity"; }
	inline virtual const char *className() const { return CRiOpacity::myClassName(); }

	inline CRiOpacity(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		RtInt i;
		for ( i=0; i<curColorDescr.colorSamples(); ++i )
			m_opacity.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OPACITY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
		ri.doOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
	}
	inline CRiOpacity &operator=(const CRiOpacity &) {
		return *this;
	}
}; // CRiOpacity

///////////////////////////////////////////////////////////////////////////////
class CRiSurface : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiSurface"; }
	inline virtual const char *className() const { return CRiSurface::myClassName(); }

	inline CRiSurface(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiSurface(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SURFACE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSurface(m_name.c_str(), parameters());
		ri.doSurface(m_name.c_str(), parameters());
	}
	inline CRiSurface &operator=(const CRiSurface &) {
		return *this;
	}
}; // CRiSurface

///////////////////////////////////////////////////////////////////////////////
class CRiAtmosphere : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiAtmosphere"; }
	inline virtual const char *className() const { return CRiAtmosphere::myClassName(); }

	inline CRiAtmosphere(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiAtmosphere(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATMOSPHERE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAtmosphere(m_name.c_str(), parameters());
		ri.doAtmosphere(m_name.c_str(), parameters());
	}
	inline CRiAtmosphere &operator=(const CRiAtmosphere &) {
		return *this;
	}
}; // CRiAtmosphere

///////////////////////////////////////////////////////////////////////////////
class CRiInterior : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiInterior"; }
	inline virtual const char *className() const { return CRiInterior::myClassName(); }

	inline CRiInterior(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiInterior(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_INTERIOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preInterior(m_name.c_str(), parameters());
		ri.doInterior(m_name.c_str(), parameters());
	}
	inline CRiInterior &operator=(const CRiInterior &) {
		return *this;
	}
}; // CRiInterior

///////////////////////////////////////////////////////////////////////////////
class CRiExterior : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiExterior"; }
	inline virtual const char *className() const { return CRiExterior::myClassName(); }

	inline CRiExterior(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiExterior(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_EXTERIOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preExterior(m_name.c_str(), parameters());
		ri.doExterior(m_name.c_str(), parameters());
	}
	inline CRiExterior &operator=(const CRiExterior &) {
		return *this;
	}
}; // CRiExterior

///////////////////////////////////////////////////////////////////////////////
class CRiDisplacement : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiDisplacement"; }
	inline virtual const char *className() const { return CRiDisplacement::myClassName(); }

	inline CRiDisplacement(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiDisplacement(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLACEMENT; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplacement(m_name.c_str(), parameters());
		ri.doDisplacement(m_name.c_str(), parameters());
	}
	inline CRiDisplacement &operator=(const CRiDisplacement &) {
		return *this;
	}
}; // CRiDisplacement

///////////////////////////////////////////////////////////////////////////////
class CRiTextureCoordinates : public CRManInterfaceCall {
protected:
	RtFloat m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4;
public:
	inline static const char *myClassName(void) { return "CRiTextureCoordinates"; }
	inline virtual const char *className() const { return CRiTextureCoordinates::myClassName(); }

	inline CRiTextureCoordinates(long aLineNo, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) : CRManInterfaceCall(aLineNo), m_s1(s1), m_t1(t1), m_s2(s2), m_t2(t2), m_s3(s3), m_t3(t3), m_s4(s4), m_t4(t4) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TEXTURE_COORDINATES; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
		ri.doTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
	}
	inline CRiTextureCoordinates &operator=(const CRiTextureCoordinates &) {
		return *this;
	}
}; // CRiTextureCoordinates

///////////////////////////////////////////////////////////////////////////////
class CRiShadingRate : public CRManInterfaceCall {
protected:
	RtFloat m_size;
public:
	inline static const char *myClassName(void) { return "CRiShadingRate"; }
	inline virtual const char *className() const { return CRiShadingRate::myClassName(); }

	inline CRiShadingRate(long aLineNo, RtFloat size) : CRManInterfaceCall(aLineNo), m_size(size) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_RATE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShadingRate(m_size);
		ri.doShadingRate(m_size);
	}
	inline CRiShadingRate &operator=(const CRiShadingRate &) {
		return *this;
	}
}; // CRiShadingRate

///////////////////////////////////////////////////////////////////////////////
class CRiShadingInterpolation : public CRManInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "CRiShadingInterpolation"; }
	inline virtual const char *className() const { return CRiShadingInterpolation::myClassName(); }

	inline CRiShadingInterpolation(long aLineNo, RtToken type) : CRManInterfaceCall(aLineNo), m_type(type) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_INTERPOLATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShadingInterpolation(m_type.c_str());
		ri.doShadingInterpolation(m_type.c_str());
	}
	inline CRiShadingInterpolation &operator=(const CRiShadingInterpolation &) {
		return *this;
	}
}; // CRiShadingInterpolation

///////////////////////////////////////////////////////////////////////////////
class CRiMatte : public CRManInterfaceCall {
protected:
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiMatte"; }
	inline virtual const char *className() const { return CRiMatte::myClassName(); }

	inline CRiMatte(long aLineNo, RtBoolean onoff) : CRManInterfaceCall(aLineNo), m_onoff(onoff) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MATTE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMatte(m_onoff);
		ri.doMatte(m_onoff);
	}
	inline CRiMatte &operator=(const CRiMatte &) {
		return *this;
	}
}; // CRiMatte

///////////////////////////////////////////////////////////////////////////////
class CRiBound : public CRManInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "CRiBound"; }
	inline virtual const char *className() const { return CRiBound::myClassName(); }

	inline CRiBound(long aLineNo, RtBound bound) : CRManInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_BOUND; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBound(m_bound);
		ri.doBound(m_bound);
	}
	inline CRiBound &operator=(const CRiBound &) {
		return *this;
	}
}; // CRiBound

///////////////////////////////////////////////////////////////////////////////
class CRiDetail : public CRManInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "CRiDetail"; }
	inline virtual const char *className() const { return CRiDetail::myClassName(); }

	inline CRiDetail(long aLineNo, RtBound bound) : CRManInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDetail(m_bound);
		ri.doDetail(m_bound);
	}
	inline CRiDetail &operator=(const CRiDetail &) {
		return *this;
	}
}; // CRiDetail

///////////////////////////////////////////////////////////////////////////////
class CRiDetailRange : public CRManInterfaceCall {
protected:
	RtFloat m_minvis, m_lowtran, m_uptran, m_maxvis;
public:
	inline static const char *myClassName(void) { return "CRiDetailRange"; }
	inline virtual const char *className() const { return CRiDetailRange::myClassName(); }

	inline CRiDetailRange(long aLineNo, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) : CRManInterfaceCall(aLineNo), m_minvis(minvis), m_lowtran(lowtran), m_uptran(uptran), m_maxvis(maxvis) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL_RANGE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
		ri.doDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
	}
	inline CRiDetailRange &operator=(const CRiDetailRange &) {
		return *this;
	}
}; // CRiDetailRange

///////////////////////////////////////////////////////////////////////////////
class CRiGeometricApproximation : public CRManInterfaceCall {
protected:
	std::string m_type;
	RtFloat m_value;
public:
	inline static const char *myClassName(void) { return "CRiGeometricApproximation"; }
	inline virtual const char *className() const { return CRiGeometricApproximation::myClassName(); }

	inline CRiGeometricApproximation(long aLineNo, RtToken type, RtFloat value) : CRManInterfaceCall(aLineNo), m_type(type), m_value(value) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_APPROXIMATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometricApproximation(m_type.c_str(), m_value);
		ri.doGeometricApproximation(m_type.c_str(), m_value);
	}
	inline CRiGeometricApproximation &operator=(const CRiGeometricApproximation &) {
		return *this;
	}
}; // CRiGeometricApproximation

///////////////////////////////////////////////////////////////////////////////
class CRiGeometricRepresentation : public CRManInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "CRiGeometricRepresentation"; }
	inline virtual const char *className() const { return CRiGeometricRepresentation::myClassName(); }

	inline CRiGeometricRepresentation(long aLineNo, RtToken type) : CRManInterfaceCall(aLineNo), m_type(type) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_REPRESENTATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometricRepresentation(m_type.c_str());
		ri.doGeometricRepresentation(m_type.c_str());
	}
	inline CRiGeometricRepresentation &operator=(const CRiGeometricRepresentation &) {
		return *this;
	}
}; // CRiGeometricRepresentation

///////////////////////////////////////////////////////////////////////////////
class CRiOrientation : public CRManInterfaceCall {
protected:
	std::string m_orientation;
public:
	inline static const char *myClassName(void) { return "CRiOrientation"; }
	inline virtual const char *className() const { return CRiOrientation::myClassName(); }

	inline CRiOrientation(long aLineNo, RtToken orientation) : CRManInterfaceCall(aLineNo), m_orientation(orientation) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ORIENTATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preOrientation(m_orientation.c_str());
		ri.doOrientation(m_orientation.c_str());
	}
	inline CRiOrientation &operator=(const CRiOrientation &) {
		return *this;
	}
}; // CRiOrientation

///////////////////////////////////////////////////////////////////////////////
class CRiReverseOrientation : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiReverseOrientation"; }
	inline virtual const char *className() const { return CRiReverseOrientation::myClassName(); }

	inline CRiReverseOrientation(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_REVERSE_ORIENTATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preReverseOrientation();
		ri.doReverseOrientation();
	}
	inline CRiReverseOrientation &operator=(const CRiReverseOrientation &) {
		return *this;
	}
}; // CRiReverseOrientation

///////////////////////////////////////////////////////////////////////////////
class CRiSides : public CRManInterfaceCall {
protected:
	RtInt m_nsides;
public:
	inline static const char *myClassName(void) { return "CRiSides"; }
	inline virtual const char *className() const { return CRiSides::myClassName(); }

	inline CRiSides(long aLineNo, RtInt nsides) : CRManInterfaceCall(aLineNo), m_nsides(nsides) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SIDES; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSides(m_nsides);
		ri.doSides(m_nsides);
	}
	inline CRiSides &operator=(const CRiSides &) {
		return *this;
	}
}; // CRiSides

///////////////////////////////////////////////////////////////////////////////
class CRiBasis : public CRManInterfaceCall {
protected:
	RtInt m_ustep, m_vstep;
	RtBasis m_ubasis, m_vbasis;
public:
	inline static const char *myClassName(void) { return "CRiBasis"; }
	inline virtual const char *className() const { return CRiBasis::myClassName(); }

	inline CRiBasis(long aLineNo, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) : CRManInterfaceCall(aLineNo), m_ustep(ustep), m_vstep(vstep) {
		memcpy(m_ubasis, ubasis, sizeof(RtBasis));
		memcpy(m_vbasis, vbasis, sizeof(RtBasis));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_BASIS; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
		ri.doBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
	}
	inline CRiBasis &operator=(const CRiBasis &) {
		return *this;
	}
}; // CRiBasis

///////////////////////////////////////////////////////////////////////////////
class CRiTrimCurve : public CRManInterfaceCall {
protected:
	CTrimCurveData m_trimCurve;

public:
	inline static const char *myClassName(void) { return "CRiTrimCurve"; }
	inline virtual const char *className() const { return CRiTrimCurve::myClassName(); }

	inline CRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) : CRManInterfaceCall(aLineNo), m_trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w) {}
	inline CRiTrimCurve(long aLineNo, const CTrimCurveData &CRimCurve)  : CRManInterfaceCall(aLineNo), m_trimCurve(CRimCurve) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRIM_CURVE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preTrimCurve(m_trimCurve.m_nLoops,
			m_trimCurve.m_nCurves.empty() ? 0 : &m_trimCurve.m_nCurves[0],
			m_trimCurve.m_order.empty() ? 0 : &m_trimCurve.m_order[0],
			m_trimCurve.m_knots.empty() ? 0 : &m_trimCurve.m_knots[0],
			m_trimCurve.m_min.empty() ? 0 : &m_trimCurve.m_min[0],
			m_trimCurve.m_max.empty() ? 0 : &m_trimCurve.m_max[0],
			m_trimCurve.m_n.empty() ? 0 : &m_trimCurve.m_n[0],
			m_trimCurve.m_u.empty() ? 0 : &m_trimCurve.m_u[0],
			m_trimCurve.m_v.empty() ? 0 : &m_trimCurve.m_v[0],
			m_trimCurve.m_w.empty() ? 0 : &m_trimCurve.m_w[0]);
		ri.doTrimCurve(m_trimCurve.m_nLoops,
			m_trimCurve.m_nCurves.empty() ? 0 : &m_trimCurve.m_nCurves[0],
			m_trimCurve.m_order.empty() ? 0 : &m_trimCurve.m_order[0],
			m_trimCurve.m_knots.empty() ? 0 : &m_trimCurve.m_knots[0],
			m_trimCurve.m_min.empty() ? 0 : &m_trimCurve.m_min[0],
			m_trimCurve.m_max.empty() ? 0 : &m_trimCurve.m_max[0],
			m_trimCurve.m_n.empty() ? 0 : &m_trimCurve.m_n[0],
			m_trimCurve.m_u.empty() ? 0 : &m_trimCurve.m_u[0],
			m_trimCurve.m_v.empty() ? 0 : &m_trimCurve.m_v[0],
			m_trimCurve.m_w.empty() ? 0 : &m_trimCurve.m_w[0]);
	}
	inline CRiTrimCurve &operator=(const CRiTrimCurve &) {
		return *this;
	}
	inline const CTrimCurveData &CRimCurve() { return m_trimCurve; }
}; // CRiTrimCurve

}

#endif // _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H
