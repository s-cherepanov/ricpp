#ifndef _RICPP_RENDERSTATE_RIMACROLIGHTS_H
#define _RICPP_RENDERSTATE_RIMACROLIGHTS_H

#ifndef _RICPP_RENDERSTATE_RIMACROBASE_H
#include "ricpp/renderstate/rimacrobase.h"
#endif // _RICPP_RENDERSTATE_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Light sources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handleIdx;
public:
	inline static const char *myClassName(void) { return "CRiLightSource"; }
	inline virtual const char *className() const { return CRiLightSource::myClassName(); }

	inline CRiLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
	}
	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo()), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		CParameterClasses p;
		setParams(state.dict(), p, state.options().colorDescr(), n, tokens, params);
	}
	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_LIGHT_SOURCE; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.preLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, h);
		ri.doLightSource(h, m_name, parameters());
	}
	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }
	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }
	inline CRiLightSource &operator=(const CRiLightSource &c) {
		if ( this == &c )
			return *this;

		name(c.name());
		handleIdx(c.handleIdx());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiAreaLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handleIdx;
public:
	inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
	inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

	inline CRiAreaLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
	}
	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo()), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		CParameterClasses p;
		setParams(state.dict(), p, state.options().colorDescr(), n, tokens, params);
	}
	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_AREA_LIGHT_SOURCE; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		RtLightHandle h = ri.preAreaLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, h);
		ri.doAreaLightSource(h, m_name, parameters());
	}
	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }
	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }
	inline CRiAreaLightSource &operator=(const CRiAreaLightSource &c) {
		if ( this == &c )
			return *this;

		name(c.name());
		handleIdx(c.handleIdx());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAreaLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiIlluminate : public CRManInterfaceCall {
protected:
	RtLightHandle m_handleIdx;
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiIlluminate"; }
	inline virtual const char *className() const { return CRiIlluminate::myClassName(); }

	inline CRiIlluminate(long aLineNo, RtLightHandle handleIdx, RtBoolean onoff) :
		CRManInterfaceCall(aLineNo), m_handleIdx(handleIdx), m_onoff(onoff)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ILLUMINATE; }

	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIlluminate(m_handleIdx, m_onoff);
		ri.doIlluminate(m_handleIdx, m_onoff);
	}
	inline virtual RtLightHandle handleIdx() { return m_handleIdx; }
	inline CRiIlluminate &operator=(const CRiIlluminate &)
	{
		return *this;
	}
}; // CRiIlluminate

}

#endif // _RICPP_RENDERSTATE_RIMACROLIGHTS_H
