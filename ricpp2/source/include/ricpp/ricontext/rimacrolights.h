#ifndef _RICPP_RICONTEXT_RIMACROLIGHTS_H
#define _RICPP_RICONTEXT_RIMACROLIGHTS_H

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

/** @file rimacrolights.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for light sources, @see rimacro.h
 *  @todo Needs some work. 
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Light sources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handleIdx;
	RtLightHandle m_tmpHandle;
public:
	inline static const char *myClassName(void) { return "CRiLightSource"; }
	inline virtual const char *className() const { return CRiLightSource::myClassName(); }

	inline CRiLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
		m_tmpHandle = illLightHandle;
	}

	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo(), RI_LIGHT_SOURCE, aName, state.dict(), CParameterClasses(), state.options().colorDescr(), n, tokens, params), m_name(aName), m_tmpHandle(illLightHandle)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}

	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		m_tmpHandle = illLightHandle;
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

	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }

	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }

	inline virtual RtLightHandle handle() const { return m_tmpHandle; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		m_tmpHandle = ri.preLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, m_tmpHandle);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.doLightSource(h, m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.postLightSource(h, m_name, parameters());
		m_tmpHandle = illLightHandle;
	}

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
	RtLightHandle m_tmpHandle;
public:
	inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
	inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

	inline CRiAreaLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
		m_tmpHandle = illLightHandle;
	}

	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo(), RI_AREA_LIGHT_SOURCE, aName, state.dict(), CParameterClasses(), state.options().colorDescr(), n, tokens, params), m_name(aName), m_tmpHandle(illLightHandle)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}

	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		m_tmpHandle = illLightHandle;
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

	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }
	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }

	inline virtual RtLightHandle handle() const { return m_tmpHandle; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		m_tmpHandle = ri.preAreaLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, m_tmpHandle);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.doAreaLightSource(h, m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.postAreaLightSource(h, m_name, parameters());
		m_tmpHandle = illLightHandle;
	}

	inline CRiAreaLightSource &operator=(const CRiAreaLightSource &c)
	{
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

	inline virtual void handleIdx(RtLightHandle handleIdx) { m_handleIdx = handleIdx; }

	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }

	inline virtual void onOff(RtBoolean onoff) { m_onoff = onoff; }

	inline virtual RtBoolean onOff() const { return m_onoff; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.preIlluminate(h, m_onoff);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.doIlluminate(h, m_onoff);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.renderState()->lights().getHandle(m_handleIdx);
		ri.postIlluminate(h, m_onoff);
	}

	inline CRiIlluminate &operator=(const CRiIlluminate &c)
	{
		if ( this == &c )
			return *this;

		handleIdx(c.handleIdx());
		onOff(c.onOff());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiIlluminate

}

#endif // _RICPP_RICONTEXT_RIMACROLIGHTS_H
