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
	private:
		RtToken m_name;
		RtLightHandle m_handle;
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		inline static const char *myClassName(void) { return "CRiLightSource"; }
		inline virtual const char *className() const { return CRiLightSource::myClassName(); }

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

		inline CRiLightSource(
			long aLineNo = -1,
			RtLightHandle aHandle = illLightHandle,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_handle(aHandle), m_name(aName)
		{
		}

		inline CRiLightSource(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtLightHandle aHandle,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_OPTION, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_handle(aHandle), m_name(aName)
		{
		}

		inline CRiLightSource(
			long aLineNo,
			RtLightHandle aHandle,
			RtToken aName, 
			const CParameterList &theParameters)
			: TypeParent(aLineNo, theParameters), m_handle(aHandle), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiLightSource(const CRiLightSource &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiLightSource(*this);
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

		inline virtual void process(IRiRoot &ri)
		{
			handle(ri.lightSourceV(name(), paramSize(), tokenPtr(), valuePtr()));
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
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putStringToken(handle());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, const char *aName, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putStringToken(aName);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, unsigned long handleNo, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putValue(handleNo);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		inline CRiLightSource &operator=(const CRiLightSource &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());
			handle(c.handle());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiLightSource

	///////////////////////////////////////////////////////////////////////////////
	class CRiAreaLightSource : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name;
		RtLightHandle m_handle;
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
		inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

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

		inline CRiAreaLightSource(
			long aLineNo = -1,
			RtLightHandle aHandle = illLightHandle,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_handle(aHandle), m_name(aName)
		{
		}

		inline CRiAreaLightSource(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtLightHandle aHandle,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_OPTION, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_handle(aHandle), m_name(aName)
		{
		}

		inline CRiAreaLightSource(
			long aLineNo,
			RtLightHandle aHandle,
			RtToken aName, 
			const CParameterList &theParameters)
			: TypeParent(aLineNo, theParameters), m_handle(aHandle), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiAreaLightSource(const CRiAreaLightSource &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiAreaLightSource(*this);
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

		inline virtual void process(IRiRoot &ri)
		{
			handle(ri.areaLightSourceV(name(), paramSize(), tokenPtr(), valuePtr()));
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
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putStringToken(handle());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, const char *aName, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putStringToken(aName);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, unsigned long handleNo, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putValue(handleNo);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
			
		inline CRiAreaLightSource &operator=(const CRiAreaLightSource &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());
			handle(c.handle());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiAreaLightSource

}

#endif // _RICPP_RICONTEXT_RIMACROLIGHTS_H
