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
	RtLightHandle m_handle;
public:
	inline static const char *myClassName(void) { return "CRiLightSource"; }
	inline virtual const char *className() const { return CRiLightSource::myClassName(); }

	inline CRiLightSource(
		long aLineNo = -1,
		RtLightHandle aHandle = illLightHandle,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_handle(aHandle), m_name(aName)
	{
	}

	inline CRiLightSource(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtLightHandle aHandle,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, RI_OPTION, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_handle(aHandle), m_name(aName)
	{
	}

	inline CRiLightSource(
		long aLineNo,
		RtLightHandle aHandle,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_handle(aHandle), m_name(aName)
	{
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

	inline virtual RtLightHandle handle() const { return m_handle; }

	inline virtual void handle(RtLightHandle aHandle) { m_handle = aHandle; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preLightSource(m_handle, m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doLightSource(m_handle, m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postLightSource(m_handle, m_name, parameters());
	}

	inline CRiLightSource &operator=(const CRiLightSource &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());
		handle(c.handle());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiAreaLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handle;
public:
	inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
	inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

	inline CRiAreaLightSource(
		long aLineNo = -1,
		RtLightHandle aHandle = illLightHandle,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_handle(aHandle), m_name(aName)
	{
	}

	inline CRiAreaLightSource(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtLightHandle aHandle,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, RI_OPTION, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_handle(aHandle), m_name(aName)
	{
	}

	inline CRiAreaLightSource(
		long aLineNo,
		RtLightHandle aHandle,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_handle(aHandle), m_name(aName)
	{
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

	inline virtual RtLightHandle handle() const { return m_handle; }
	inline virtual void handle(RtLightHandle aHandle) { m_handle = aHandle; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAreaLightSource(m_handle, m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doAreaLightSource(m_handle, m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postAreaLightSource(m_handle, m_name, parameters());
	}

	inline CRiAreaLightSource &operator=(const CRiAreaLightSource &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());
		handle(c.handle());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAreaLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiIlluminate : public CRManInterfaceCall {
protected:
	RtLightHandle m_handle;
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiIlluminate"; }
	inline virtual const char *className() const { return CRiIlluminate::myClassName(); }

	inline CRiIlluminate(long aLineNo, RtLightHandle aHandle, RtBoolean onoff) :
		CRManInterfaceCall(aLineNo), m_handle(aHandle), m_onoff(onoff)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ILLUMINATE; }

	inline virtual void handle(RtLightHandle aHandle) { m_handle = aHandle; }

	inline virtual RtLightHandle handle() const { return m_handle; }

	inline virtual void onOff(RtBoolean onoff) { m_onoff = onoff; }

	inline virtual RtBoolean onOff() const { return m_onoff; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIlluminate(m_handle, m_onoff);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doIlluminate(m_handle, m_onoff);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postIlluminate(m_handle, m_onoff);
	}

	inline CRiIlluminate &operator=(const CRiIlluminate &c)
	{
		if ( this == &c )
			return *this;

		handle(c.handle());
		onOff(c.onOff());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiIlluminate

}

#endif // _RICPP_RICONTEXT_RIMACROLIGHTS_H
