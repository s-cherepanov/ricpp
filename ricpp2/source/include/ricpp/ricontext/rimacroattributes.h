#ifndef _RICPP_RICONTEXT_RIMACROATTRIBUTES_H
#define _RICPP_RICONTEXT_RIMACROATTRIBUTES_H

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

/** @file rimacroattributes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for attributes, @see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Attributes
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	/** @brief User defined attribute.
	 */
	class CRiAttribute : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the attribute as atomized string.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiAttribute"; }

		inline virtual const char *className() const { return CRiAttribute::myClassName(); }

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
		 *  @param aName Name of the attribute as atomized string.
		 */
		inline CRiAttribute(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the attribute as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiAttribute(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_ATTRIBUTE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the attribute as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiAttribute(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiAttribute(const CRiAttribute &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiAttribute()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiAttribute(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preAttribute(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doAttribute(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postAttribute(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiAttribute &operator=(const CRiAttribute &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiAttribute


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Surface color.
	 */
	class CRiColor : public CRManInterfaceCall {
	private:
		std::vector<RtFloat> m_color; ///< Color
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiColor"; }
		inline virtual const char *className() const { return CRiColor::myClassName(); }

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
		 *  @param colorSamples Number of samples per color value
		 *  @param Cs Surface color value
		 */
		inline CRiColor(long aLineNo=-1, RtInt colorSamples=0, const RtFloat *Cs=0)
			: TypeParent(aLineNo)
		{
			color(colorSamples, Cs);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiColor(const CRiColor &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiColor()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiColor(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR; }

		const RtFloat *color() const
		{
			return &(m_color[0]);
		}

		RtFloat *color()
		{
			return &(m_color[0]);
		}

		void color(RtInt colorSamples, const RtFloat *Cs)
		{
			if ( colorSamples < 0 )
				colorSamples = 0;
			m_color.clear();
			m_color.reserve(colorSamples);
			for ( RtInt i = 0; i < colorSamples; ++i )
				m_color.push_back(Cs[i]);
		}

		RtInt size() const
		{
			return (RtInt)m_color.size();
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preColor(*this, m_color.empty() ? 0 : &m_color[0]);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doColor(*this, m_color.empty() ? 0 : &m_color[0]);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postColor(*this, m_color.empty() ? 0 : &m_color[0]);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray((unsigned long)m_color.size(), color());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}	

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiColor &operator=(const CRiColor &c)
		{
			if ( this == &c )
				return *this;

			color(c.size(), c.color());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiColor


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Surface opacity.
	 */
	class CRiOpacity : public CRManInterfaceCall {
	private:
		std::vector<RtFloat> m_opacity;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiOpacity"; }
		inline virtual const char *className() const { return CRiOpacity::myClassName(); }

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
		 *  @param colorSamples Number of samples per color value
		 *  @param Os Surface opacity value
		 */
		inline CRiOpacity(long aLineNo=-1, RtInt colorSamples=0, const RtFloat *Os=0)
			: TypeParent(aLineNo)
		{
			opacity(colorSamples, Os);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiOpacity(const CRiOpacity &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiOpacity()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiOpacity(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_OPACITY; }

		const RtFloat *opacity() const
		{
			return &(m_opacity[0]);
		}

		RtFloat *opacity()
		{
			return &(m_opacity[0]);
		}

		void opacity(RtInt colorSamples, const RtFloat *Os)
		{
			if ( colorSamples < 0 )
				colorSamples = 0;
			m_opacity.clear();
			m_opacity.reserve(colorSamples);
			for ( RtInt i = 0; i < colorSamples; ++i )
				m_opacity.push_back(Os[i]);
		}

		RtInt size() const
		{
			return (RtInt)m_opacity.size();
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preOpacity(*this, m_opacity.empty() ? 0 : &m_opacity[0]);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doOpacity(*this, m_opacity.empty() ? 0 : &m_opacity[0]);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postOpacity(*this, m_opacity.empty() ? 0 : &m_opacity[0]);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray((unsigned long)m_opacity.size(), opacity());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiOpacity &operator=(const CRiOpacity &c)
		{
			if ( this == &c )
				return *this;

			opacity(c.size(), c.opacity());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiOpacity


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Surface shader.
	 */
	class CRiSurface : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the shader as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiSurface"; }
		inline virtual const char *className() const { return CRiSurface::myClassName(); }

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
		 *  @param aName Name of the shader as atomized string.
		 */
		inline CRiSurface(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the shader as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiSurface(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_SURFACE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the shader as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiSurface(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiSurface(const CRiSurface &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiSurface()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiSurface(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SURFACE; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preSurface(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doSurface(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postSurface(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiSurface &operator=(const CRiSurface &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSurface


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Atmosphere shader.
	 */
	class CRiAtmosphere : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the shader as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiAtmosphere"; }
		inline virtual const char *className() const { return CRiAtmosphere::myClassName(); }

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
		 *  @param aName Name of the shader as atomized string.
		 */
		inline CRiAtmosphere(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the shader as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiAtmosphere(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_ATMOSPHERE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the shader as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiAtmosphere(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiAtmosphere(const CRiAtmosphere &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiAtmosphere()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiAtmosphere(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ATMOSPHERE; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preAtmosphere(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doAtmosphere(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postAtmosphere(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiAtmosphere &operator=(const CRiAtmosphere &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiAtmosphere


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Interior shader.
	 */
	class CRiInterior : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the shader as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiInterior"; }
		inline virtual const char *className() const { return CRiInterior::myClassName(); }

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
		 *  @param aName Name of the shader as atomized string.
		 */
		inline CRiInterior(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the shader as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiInterior(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_INTERIOR, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the shader as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiInterior(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiInterior(const CRiInterior &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiInterior()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiInterior(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_INTERIOR; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preInterior(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doInterior(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postInterior(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiInterior &operator=(const CRiInterior &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiInterior


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Exterior shader.
	 */
	class CRiExterior : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the shader as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiInterior"; }
		inline virtual const char *className() const { return CRiInterior::myClassName(); }

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
		 *  @param aName Name of the shader as atomized string.
		 */
		inline CRiExterior(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the shader as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiExterior(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_EXTERIOR, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the shader as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiExterior(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiExterior(const CRiExterior &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiExterior()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiExterior(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_EXTERIOR; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preExterior(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doExterior(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postExterior(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiExterior &operator=(const CRiExterior &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiExterior


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Displacemet shader.
	 */
	class CRiDisplacement : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the shader as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiDisplacement"; }
		inline virtual const char *className() const { return CRiDisplacement::myClassName(); }

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
		 *  @param aName Name of the shader as atomized string.
		 */
		inline CRiDisplacement(
			long aLineNo = -1,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the shader as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiDisplacement(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_DISPLACEMENT, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the shader as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiDisplacement(
			long aLineNo,
			RtToken aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiDisplacement(const CRiDisplacement &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiDisplacement()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiDisplacement(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLACEMENT; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preDisplacement(*this, m_name, parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doDisplacement(*this, m_name, parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postDisplacement(*this, m_name, parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiDisplacement &operator=(const CRiDisplacement &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDisplacement


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Deformation
	 */
	class CRiDeformation : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the deformation as token.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}
		
		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the deformation as atomized string.
		 */
		inline CRiDeformation(
							  long aLineNo = -1,
							  RtToken aName = RI_NULL)
		: TypeParent(aLineNo), m_name(aName)
		{
		}
		
		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the deformation as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiDeformation(
							  long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
							  const char *aName,
							  RtInt n, RtToken tokens[], RtPointer params[])
		: TypeParent(aLineNo, RI_DEFORMATION, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}
		
		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the deformation as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiDeformation(
							  long aLineNo,
							  RtToken aName,
							  const CParameterList &theParameters
							  )
		: TypeParent(aLineNo, theParameters), m_name(aName)
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
			ri.preDeformation(*this, m_name, parameters());
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doDeformation(*this, m_name, parameters());
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postDeformation(*this, m_name, parameters());
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			
			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDeformation
		
		
	///////////////////////////////////////////////////////////////////////////////
	/** @brief The texture (s, t) coordinates at the 4 corners of a parametric patch.
	 */
	class CRiTextureCoordinates : public CRManInterfaceCall {
	private:
		RtFloat m_s1, ///< s coordinate top left
				m_t1, ///< t coordinate top left
				m_s2, ///< s coordinate top right
				m_t2, ///< t coordinate top right
				m_s3, ///< s coordinate bottom left
				m_t3, ///< t coordinate bottom left
				m_s4, ///< s coordinate bottom left
				m_t4; ///< t coordinate bottom left
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiTextureCoordinates"; }
		inline virtual const char *className() const { return CRiTextureCoordinates::myClassName(); }

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
		 *  @param as1 s coordinate top left
		 *  @param at1 t coordinate top left
		 *  @param as2 s coordinate top right
		 *  @param at2 t coordinate top right
		 *  @param as3 s coordinate bottom left
		 *  @param at3 t coordinate bottom left
		 *  @param as4 s coordinate bottom left
		 *  @param at4 t coordinate bottom left
		 */
		inline CRiTextureCoordinates(
			long aLineNo = -1,
			RtFloat as1 = CAttributes::defTextureS1, RtFloat at1 = CAttributes::defTextureT1,
			RtFloat as2 = CAttributes::defTextureS2, RtFloat at2 = CAttributes::defTextureT2,
			RtFloat as3 = CAttributes::defTextureS3, RtFloat at3 = CAttributes::defTextureT3,
			RtFloat as4 = CAttributes::defTextureS4, RtFloat at4 = CAttributes::defTextureT4)
			: TypeParent(aLineNo),
			  m_s1(as1), m_t1(at1),
			  m_s2(as2), m_t2(at2),
			  m_s3(as3), m_t3(at3),
			  m_s4(as4), m_t4(at4)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiTextureCoordinates(const CRiTextureCoordinates &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiTextureCoordinates()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiTextureCoordinates(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_TEXTURE_COORDINATES; }

		/** @brief Gets the s coordinate of the top left
		 *
		 *  @return s coordinate of the top left
		 */
		RtFloat s1() const
		{
			return m_s1;
		}

		/** @brief Sets the s coordinate of the top left
		 *
		 *  @param as1 s coordinate of the top left
		 */
		void s1(RtFloat as1)
		{
			m_s1 = as1;
		}

		/** @brief Gets the t coordinate of the top left
		 *
		 *  @return t coordinate of the top left
		 */
		RtFloat t1() const
		{
			return m_t1;
		}

		/** @brief Sets the t coordinate of the top left
		 *
		 *  @param at1 t coordinate of the top left
		 */
		void t1(RtFloat at1)
		{
			m_t1 = at1;
		}

		/** @brief Gets the s coordinate of the top right
		 *
		 *  @return s coordinate of the top right
		 */
		RtFloat s2() const
		{
			return m_s2;
		}

		/** @brief Sets the s coordinate of the top right
		 *
		 *  @param as2 s coordinate of the top right
		 */
		void s2(RtFloat as2)
		{
			m_s2 = as2;
		}

		/** @brief Gets the t coordinate of the top right
		 *
		 *  @return t coordinate of the top right
		 */
		RtFloat t2() const
		{
			return m_t2;
		}

		/** @brief Sets the t coordinate of the top right
		 *
		 *  @param at2 t coordinate of the top right
		 */
		void t2(RtFloat at2)
		{
			m_t2 = at2;
		}

		/** @brief Gets the s coordinate of the bottom left
		 *
		 *  @return s coordinate of the bottom left
		 */
		RtFloat s3() const
		{
			return m_s3;
		}

		/** @brief Sets the s coordinate of the bottom left
		 *
		 *  @param as3 s coordinate of the bottom left
		 */
		void s3(RtFloat as3)
		{
			m_s3 = as3;
		}

		/** @brief Gets the t coordinate of the bottom left
		 *
		 *  @return t coordinate of the bottom left
		 */
		RtFloat t3() const
		{
			return m_t3;
		}

		/** @brief Sets the t coordinate of the bottom left
		 *
		 *  @param at3 t coordinate of the bottom left
		 */
		void t3(RtFloat at3)
		{
			m_t3 = at3;
		}

		/** @brief Gets the s coordinate of the bottom right
		 *
		 *  @return s coordinate of the bottom right
		 */
		RtFloat s4() const
		{
			return m_s4;
		}

		/** @brief Sets the s coordinate of the bottom right
		 *
		 *  @param as4 s coordinate of the bottom right
		 */
		void s4(RtFloat as4)
		{
			m_s4 = as4;
		}

		/** @brief Gets the t coordinate of the bottom right
		 *
		 *  @return t coordinate of the bottom right
		 */
		RtFloat t4() const
		{
			return m_t4;
		}

		/** @brief Sets the t coordinate of the bottom right
		 *
		 *  @param at4 t coordinate of the bottom right
		 */
		void t4(RtFloat at4)
		{
			m_t4 = at4;
		}

		/** @brief Gets the values of the coordinates.
		 *
		 *  @param as1 s coordinate top left
		 *  @param at1 t coordinate top left
		 *  @param as2 s coordinate top right
		 *  @param at2 t coordinate top right
		 *  @param as3 s coordinate bottom left
		 *  @param at3 t coordinate bottom left
		 *  @param as4 s coordinate bottom left
		 *  @param at4 t coordinate bottom left
		 */
		inline void set(
			RtFloat &as1, RtFloat &at1,
			RtFloat &as2, RtFloat &at2,
			RtFloat &as3, RtFloat &at3,
			RtFloat &as4, RtFloat &at4) const
		{
			as1 = m_s1; at1 = m_t1;
			as2 = m_s2; at2 = m_t2;
			as3 = m_s3; at3 = m_t3;
			as4 = m_s4; at4 = m_t4;
		}

		/** @brief Sets the values of the coordinates.
		 *
		 *  @param as1 s coordinate top left
		 *  @param at1 t coordinate top left
		 *  @param as2 s coordinate top right
		 *  @param at2 t coordinate top right
		 *  @param as3 s coordinate bottom left
		 *  @param at3 t coordinate bottom left
		 *  @param as4 s coordinate bottom left
		 *  @param at4 t coordinate bottom left
		 */
		inline void set(
			RtFloat as1, RtFloat at1,
			RtFloat as2, RtFloat at2,
			RtFloat as3, RtFloat at3,
			RtFloat as4, RtFloat at4)
		{
			m_s1 = as1; m_t1 = at1;
			m_s2 = as2; m_t2 = at2;
			m_s3 = as3; m_t3 = at3;
			m_s4 = as4; m_t4 = at4;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preTextureCoordinates(*this, m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doTextureCoordinates(*this, m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postTextureCoordinates(*this, m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(s1());
			ribWriter.putBlank();
			ribWriter.putValue(t1());
			ribWriter.putBlank();
			ribWriter.putValue(s2());
			ribWriter.putBlank();
			ribWriter.putValue(t2());
			ribWriter.putBlank();
			ribWriter.putValue(s3());
			ribWriter.putBlank();
			ribWriter.putValue(t3());
			ribWriter.putBlank();
			ribWriter.putValue(s4());
			ribWriter.putBlank();
			ribWriter.putValue(t4());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign.
		 *  @return A reference to this object.
		 */
		inline CRiTextureCoordinates &operator=(const CRiTextureCoordinates &c)
		{
			if ( this == &c )
				return *this;

			set(c.s1(), c.t1(), c.s2(), c.t2(), c.s3(), c.t3(), c.s4(), c.t4());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTextureCoordinates


	///////////////////////////////////////////////////////////////////////////////
	/** @brief The minimum rate of surface shading
	 */
	class CRiShadingRate : public CRManInterfaceCall {
	private:
		RtFloat m_size; ///< Minimum rate of surface shading.
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiShadingRate"; }
		inline virtual const char *className() const { return CRiShadingRate::myClassName(); }

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
		 *  @param aSize Minimum rate of surface shading.
		 */
		inline CRiShadingRate(
			long aLineNo=-1,
			RtFloat aSize = CAttributes::defShadingRate)
			: TypeParent(aLineNo), m_size(aSize)
		{
		}
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiShadingRate(const CRiShadingRate &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiShadingRate()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiShadingRate(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_RATE; }

		/** @brief Gets the minimum rate of surface shading.
		 *
		 *  @return Minimum rate of surface shading.
		 */
		RtFloat size() const
		{
			return m_size;
		}

		/** @brief Sets minimum rate of surface shading.
		 *
		 *  @param aSize Minimum rate of surface shading.
		 */
		void size(RtFloat aSize)
		{
			m_size = aSize;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preShadingRate(*this, m_size);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doShadingRate(*this, m_size);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postShadingRate(*this, m_size);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(size());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiShadingRate &operator=(const CRiShadingRate &c)
		{
			if ( this == &c )
				return *this;

			size(c.size());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiShadingRate

	///////////////////////////////////////////////////////////////////////////////
	/** @brief The type of the shading interpolation
	 */
	class CRiShadingInterpolation : public CRManInterfaceCall {
	private:
		RtToken m_type; ///< Type of the shading interpolation (RI_CONSTANT, RI_SMOOTH).
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiShadingInterpolation"; }
		inline virtual const char *className() const { return CRiShadingInterpolation::myClassName(); }

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
		 *  @param aType The shading type.
		 */
		inline CRiShadingInterpolation(
			long aLineNo=-1,
			RtToken aType = RI_CONSTANT)
			: TypeParent(aLineNo), m_type(aType)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiShadingInterpolation(const CRiShadingInterpolation &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiShadingInterpolation()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiShadingInterpolation(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_INTERPOLATION; }

		/** @brief Gets the shading type.
		 *
		 *  @return The shading type.
		 */
		RtToken type() const
		{
			return m_type;
		}

		/** @brief Sets the shading type.
		 *
		 *  @param aType The shading type.
		 */
		void type(RtToken aType)
		{
			m_type = aType;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preShadingInterpolation(*this, m_type);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doShadingInterpolation(*this, m_type);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postShadingInterpolation(*this, m_type);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiShadingInterpolation &operator=(const CRiShadingInterpolation &c)
		{
			if ( this == &c )
				return *this;

			type(c.type());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiShadingInterpolation


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Sets the matte mode
	 */
	class CRiMatte : public CRManInterfaceCall {
	private:
		RtBoolean m_onoff; ///< Switches the matte mode on (RI_TRUE) or off (RI_FALSE)
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiMatte"; }
		inline virtual const char *className() const { return CRiMatte::myClassName(); }

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
		 *  @param anOnOff RI_TRUE, matte is on and RI_FALSE, matte is off
		 */
		inline CRiMatte(long aLineNo = -1, RtBoolean anOnOff = CAttributes::defMatte)
			: TypeParent(aLineNo), m_onoff(anOnOff)
		{
		}
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiMatte(const CRiMatte &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiMatte()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiMatte(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_MATTE; }

		/** @brief Gets the matte mode.
		 *
		 *  @return The shading type.
		 */
		RtBoolean onOff() const
		{
			return m_onoff;
		}

		/** @brief Sets the matte mode.
		 *
		 *  @param anOnOff RI_TRUE, matte is on and RI_FALSE, matte is off
		 */
		void onOff(RtBoolean anOnOff)
		{
			m_onoff = anOnOff;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preMatte(*this, m_onoff);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doMatte(*this, m_onoff);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postMatte(*this, m_onoff);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue((RtInt)(onOff() ? 1 : 0));
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiMatte &operator=(const CRiMatte &c)
		{
			if ( this == &c )
				return *this;

			onOff(c.onOff());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiMatte


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Sets the bounding box for subsequent primitives.
	 */
	class CRiBound : public CRManInterfaceCall {
	private:
		RtBound m_bound; ///< The bounding box
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiBound"; }
		inline virtual const char *className() const { return CRiBound::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiBound(
			long aLineNo = -1)
			: TypeParent(aLineNo)
		{
			m_bound[0] = m_bound[1] = m_bound[2] = -RI_INFINITY;
			m_bound[3] = m_bound[4] = m_bound[5] =  RI_INFINITY;
		}

		/** @brief Constructor.
		 *
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aBound The bounding box of the subsequent primitives
		 */
		inline CRiBound(
			long aLineNo,
			const RtBound aBound)
			: TypeParent(aLineNo)
		{
			bound(aBound);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiBound(const CRiBound &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiBound()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiBound(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_BOUND; }

		/** @brief Gets the (read-only) bounding box of subsequent primitives.
		 *
		 * @return The (read-only) bounding box of subsequent primitives.
		 */
		const RtBound &bound() const
		{
			return m_bound;
		}

		/** @brief Gets the (writeable) bounding box of subsequent primitives.
		 *
		 * @retval aBound The (writeable) bounding box of subsequent primitives.
		 */
		void getBound(RtBound aBound) const
		{
			memcpy(aBound, m_bound, sizeof(RtBound));
		}

		/** @brief Sets the bounding box of subsequent primitives.
		 *
		 * @retval aBound The bounding box of subsequent primitives.
		 */
		void bound(const RtBound aBound)
		{
			memcpy(m_bound, aBound, sizeof(RtBound));
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preBound(*this, m_bound);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doBound(*this, m_bound);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postBound(*this, m_bound);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(bound());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiBound &operator=(const CRiBound &c)
		{
			if ( this == &c )
				return *this;

			bound(c.bound());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiBound


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Area of the projected bounding box is the current detail in
	 *         raster space (times the relative detail)
	 */
	class CRiDetail : public CRManInterfaceCall {
	private:
		RtBound m_bound; ///< The bounding box
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiDetail"; }
		inline virtual const char *className() const { return CRiDetail::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiDetail(
			long aLineNo = -1)
			: TypeParent(aLineNo)
		{
			m_bound[0] = m_bound[1] = m_bound[2] = -RI_INFINITY;
			m_bound[3] = m_bound[4] = m_bound[5] =  RI_INFINITY;
		}

		/** @brief Constructor.
		 *
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aBound The bounding box of the subsequent primitives
		 */
		inline CRiDetail(
			long aLineNo,
			const RtBound aBound)
			: TypeParent(aLineNo)
		{
			bound(aBound);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiDetail(const CRiDetail &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiDetail()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiDetail(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL; }

		/** @brief Gets the (read-only) bounding box of subsequent primitives.
		 *
		 * @return The (read-only) bounding box of subsequent primitives.
		 */
		const RtBound &bound() const
		{
			return m_bound;
		}

		/** @brief Gets the (writeable) bounding box of subsequent primitives.
		 *
		 * @retval aBound The (writeable) bounding box of subsequent primitives.
		 */
		void getBound(RtBound aBound) const
		{
			memcpy(aBound, m_bound, sizeof(RtBound));
		}

		/** @brief Sets the bounding box of subsequent primitives.
		 *
		 * @retval aBound The bounding box of subsequent primitives.
		 */
		void bound(const RtBound aBound)
		{
			memcpy(m_bound, aBound, sizeof(RtBound));
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preDetail(*this, m_bound);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doDetail(*this, m_bound);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postDetail(*this, m_bound);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(bound());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiDetail &operator=(const CRiDetail &c)
		{
			if ( this == &c )
				return *this;

			bound(c.bound());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDetail


	///////////////////////////////////////////////////////////////////////////////
	/** @brief The detail range of graphical primitives.
	 */
	class CRiDetailRange : public CRManInterfaceCall {
	private:
		RtFloat m_minvis,  ///< Minimal detail value at which the primitives are visible (rendered)
				m_lowtran, ///< Detail value at which the primitives becomes visible (rendered)
				m_uptran,  ///< Detail value at which the primitives becomes invisible (not rendered)
				m_maxvis;  ///< Maximal detail value at which the primitives are visible (rendered)
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiDetailRange"; }
		inline virtual const char *className() const { return CRiDetailRange::myClassName(); }

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
		 *  @param aMinVis,  ///< Minimal detail value at which the primitives are visible (rendered)
		 *  @param aLowTran, ///< Detail value at which the primitives becomes visible (rendered)
		 *  @param anUpTran, ///< Detail value at which the primitives becomes invisible (not rendered)
		 *  @param aMaxVis;  ///< Maximal detail value at which the primitives are visible (rendered)
		 */
		inline CRiDetailRange(
			long aLineNo = -1,
			RtFloat aMinVis = CAttributes::defMinVis,
			RtFloat aLowTran = CAttributes::defLowTran,
			RtFloat anUpTran = CAttributes::defUpTran,
			RtFloat aMaxVis = CAttributes::defMaxVis)
			: TypeParent(aLineNo),
			  m_minvis(aMinVis), m_lowtran(aLowTran), m_uptran(anUpTran), m_maxvis(aMaxVis)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiDetailRange(const CRiDetailRange &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiDetailRange()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiDetailRange(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL_RANGE; }

		RtFloat minVis() const
		{
			return m_minvis;
		}

		void minVis(RtFloat aMinVis)
		{
			m_minvis = aMinVis;
		}

		RtFloat lowTran() const
		{
			return m_minvis;
		}

		void lowTran(RtFloat aLowTran)
		{
			m_lowtran = aLowTran;
		}

		RtFloat upTran() const
		{
			return m_uptran;
		}

		void upTran(RtFloat anUpTran)
		{
			m_uptran = anUpTran;
		}

		RtFloat maxVis() const
		{
			return m_maxvis;
		}

		void maxVis(RtFloat aMaxVis)
		{
			m_maxvis = aMaxVis;
		}

		/** @brief Gets the detail range.
		 *
		 *  @retval aMinVis,  ///< Minimal detail value at which the primitives are visible (rendered)
		 *  @retval aLowTran, ///< Detail value at which the primitives becomes visible (rendered)
		 *  @retval anUpTran, ///< Detail value at which the primitives becomes invisible (not rendered)
		 *  @retval aMaxVis;  ///< Maximal detail value at which the primitives are visible (rendered)
		 */
		inline void get(
			RtFloat &aMinVis,
			RtFloat &aLowTran,
			RtFloat &anUpTran,
			RtFloat &aMaxVis) const
		{
			aMinVis = m_minvis;
			aLowTran = m_lowtran;
			anUpTran = m_uptran;
			aMaxVis = m_maxvis;
		}

		/** @brief Sets the detail range.
		 *
		 *  @param aMinVis,  ///< Minimal detail value at which the primitives are visible (rendered)
		 *  @param aLowTran, ///< Detail value at which the primitives becomes visible (rendered)
		 *  @param anUpTran, ///< Detail value at which the primitives becomes invisible (not rendered)
		 *  @param aMaxVis;  ///< Maximal detail value at which the primitives are visible (rendered)
		 */
		inline void set(
			RtFloat aMinVis,
			RtFloat aLowTran,
			RtFloat anUpTran,
			RtFloat aMaxVis)
		{
			m_minvis = aMinVis;
			m_lowtran = aLowTran;
			m_uptran = anUpTran;
			m_maxvis = aMaxVis;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preDetailRange(*this, m_minvis, m_lowtran, m_uptran, m_maxvis);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doDetailRange(*this, m_minvis, m_lowtran, m_uptran, m_maxvis);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postDetailRange(*this, m_minvis, m_lowtran, m_uptran, m_maxvis);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(minVis());
			ribWriter.putBlank();
			ribWriter.putValue(lowTran());
			ribWriter.putBlank();
			ribWriter.putValue(upTran());
			ribWriter.putBlank();
			ribWriter.putValue(maxVis());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiDetailRange &operator=(const CRiDetailRange &c)
		{
			if ( this == &c )
				return *this;

			set(c.minVis(), c.lowTran(), c.upTran(), c.maxVis());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDetailRange


	///////////////////////////////////////////////////////////////////////////////
	/** @brief The type of he geometrical approximation.
	 */
	class CRiGeometricApproximation : public CRManInterfaceCall {
	private:
		RtToken m_type;
		RtFloat m_value;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiGeometricApproximation"; }
		inline virtual const char *className() const { return CRiGeometricApproximation::myClassName(); }

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
		 *  The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aType The type of the geometric approximation (RI_TESSELATION, RI_DEVIATION)
		 *  @param aValue Measure for the approximation type (e.g. number of tesselations)
		 */
		inline CRiGeometricApproximation(
			long aLineNo = -1,
			RtToken aType = RI_NULL,
			RtFloat aValue = CAttributes::defGeometricApproximationValue)
		: TypeParent(aLineNo), m_type(aType), m_value(aValue)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiGeometricApproximation(const CRiGeometricApproximation &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiGeometricApproximation()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiGeometricApproximation(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_APPROXIMATION; }
		
		inline RtToken type() const
		{
			return m_type;
		}

		inline void type(RtToken aType)
		{
			m_type = aType;
		}

		inline RtFloat value() const
		{
			return m_value;
		}

		inline void value(RtFloat aValue)
		{
			m_value = aValue;
		}
		
		void set(RtToken aType, RtFloat aValue)
		{
			m_type = aType;
			m_value = aValue;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preGeometricApproximation(*this, m_type, m_value);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doGeometricApproximation(*this, m_type, m_value);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postGeometricApproximation(*this, m_type, m_value);
		}

		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			ribWriter.putBlank();
			ribWriter.putValue(value());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiGeometricApproximation &operator=(const CRiGeometricApproximation &c)
		{
			if ( this == &c )
				return *this;

			set(c.type(), c.value());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiGeometricApproximation


	///////////////////////////////////////////////////////////////////////////////
	class CRiGeometricRepresentation : public CRManInterfaceCall {
	private:
		RtToken m_type;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiGeometricRepresentation"; }
		inline virtual const char *className() const { return CRiGeometricRepresentation::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aType The type of the geometric approximation (RI_POINTS, RI_LINES, RI_PRIMITIVE)
		 */
		inline CRiGeometricRepresentation(
			long aLineNo = -1,
			RtToken aType = RI_PRIMITIVE)
		: TypeParent(aLineNo), m_type(aType)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiGeometricRepresentation(const CRiGeometricApproximation &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiGeometricRepresentation()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiGeometricRepresentation(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_REPRESENTATION; }

		inline RtToken type() const
		{
			return m_type;
		}

		inline void type(RtToken aType)
		{
			m_type = aType;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preGeometricRepresentation(*this, m_type);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doGeometricRepresentation(*this, m_type);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postGeometricRepresentation(*this, m_type);
		}
				
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiGeometricRepresentation &operator=(const CRiGeometricRepresentation &c)
		{
			if ( this == &c )
				return *this;

			type(c.type());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiGeometricRepresentation


	///////////////////////////////////////////////////////////////////////////////
	class CRiOrientation : public CRManInterfaceCall {
	private:
		RtToken m_orientation;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiOrientation"; }
		inline virtual const char *className() const { return CRiOrientation::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param anOrientation Orientation of the Surface (RI_RH, RI_LH, RI_OUTSIDE, RI_INSIDE)
		 */
		inline CRiOrientation(
			long aLineNo=-1,
			RtToken anOrientation = RI_OUTSIDE)
		: TypeParent(aLineNo), m_orientation(anOrientation)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiOrientation(const CRiOrientation &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiOrientation()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiOrientation(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ORIENTATION; }

		inline RtToken orientation() const
		{
			return m_orientation;
		}

		inline void orientation(RtToken anOrientation)
		{
			m_orientation = anOrientation;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preOrientation(*this, m_orientation);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doOrientation(*this, m_orientation);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postOrientation(*this, m_orientation);
		}
				
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(orientation());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiOrientation &operator=(const CRiOrientation &c)
		{
			if ( this == &c )
				return *this;

			orientation(c.orientation());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiOrientation


	///////////////////////////////////////////////////////////////////////////////
	class CRiReverseOrientation : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiReverseOrientation"; }
		inline virtual const char *className() const { return CRiReverseOrientation::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiReverseOrientation(long aLineNo=-1) : TypeParent(aLineNo) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiReverseOrientation(const CRiReverseOrientation &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiReverseOrientation()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiReverseOrientation(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_REVERSE_ORIENTATION; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preReverseOrientation(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doReverseOrientation(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postReverseOrientation(*this);
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
		inline CRiReverseOrientation &operator=(const CRiReverseOrientation &c)
		{
			if ( this == &c )
				return *this;

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiReverseOrientation


	///////////////////////////////////////////////////////////////////////////////
	class CRiSides : public CRManInterfaceCall {
	private:
		RtInt m_nsides;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiSides"; }
		inline virtual const char *className() const { return CRiSides::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param theNSides Number of sides displayed (1 (outside only) or 2 (inside and outside) )
		 */
		inline CRiSides(
			long aLineNo = -1,
			RtInt theNSides = CAttributes::defNSides)
		: TypeParent(aLineNo), m_nsides(theNSides)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiSides(const CRiSides &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiSides()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiSides(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SIDES; }

		inline RtInt nSides() const
		{
			return m_nsides;
		}

		inline void nSides(RtInt theNSides)
		{
			m_nsides = theNSides;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preSides(*this, m_nsides);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doSides(*this, m_nsides);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postSides(*this, m_nsides);
		}
				
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(nSides());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiSides &operator=(const CRiSides &c)
		{
			if ( this == &c )
				return *this;
				
			nSides(c.nSides());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSides


	///////////////////////////////////////////////////////////////////////////////
	class CRiBasis : public CRManInterfaceCall {
	private:
		RtInt m_ustep, m_vstep;
		RtBasis m_ubasis, m_vbasis;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiBasis"; }
		inline virtual const char *className() const { return CRiBasis::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiBasis(long aLineNo = -1)
		: TypeParent(aLineNo)
		{
			set(RiBezierBasis, RI_BEZIERSTEP, RiBezierBasis, RI_BEZIERSTEP);
		}

		inline CRiBasis(long aLineNo, const RtBasis anUBasis, RtInt anUStep, const RtBasis aVBasis, RtInt aVStep)
		: TypeParent(aLineNo), m_ustep(anUStep), m_vstep(aVStep)
		{
			set(anUBasis, anUStep, aVBasis, aVStep);
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiBasis(const CRiBasis &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiBasis()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiBasis(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_BASIS; }
		
		const RtBasis &uBasis() const
		{
			return m_ubasis;
		}

		void uBasis(const RtBasis &anUBasis)
		{
			memcpy(m_ubasis, anUBasis, sizeof(RtBasis));
		}

		void uBasis(const RtBasis &anUBasis, RtInt anUStep)
		{
			memcpy(m_ubasis, anUBasis, sizeof(RtBasis));
			m_ustep = anUStep;
		}

		const RtBasis &vBasis() const
		{
			return m_vbasis;
		}

		void vBasis(const RtBasis &aVBasis)
		{
			memcpy(m_vbasis, aVBasis, sizeof(RtBasis));
		}

		void vBasis(const RtBasis &aVBasis, RtInt aVStep)
		{
			memcpy(m_vbasis, aVBasis, sizeof(RtBasis));
			m_vstep = aVStep;
		}

		RtInt uStep() const
		{
			return m_ustep;
		}
		
		void uStep(RtInt anUStep)
		{
			m_ustep = anUStep;
		}

		RtInt vStep() const
		{
			return m_vstep;
		}
		
		void vStep(RtInt aVStep)
		{
			m_vstep = aVStep;
		}

		inline void set(const RtBasis anUBasis, RtInt anUStep, const RtBasis aVBasis, RtInt aVStep)
		{
			memcpy(m_ubasis, anUBasis, sizeof(RtBasis));
			m_ustep = anUStep;
			memcpy(m_vbasis, aVBasis, sizeof(RtBasis));
			m_vstep = aVStep;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preBasis(*this, m_ubasis, m_ustep, m_vbasis, m_vstep);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doBasis(*this, m_ubasis, m_ustep, m_vbasis, m_vstep);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postBasis(*this, m_ubasis, m_ustep, m_vbasis, m_vstep);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putBasis(uBasis());
			ribWriter.putBlank();
			ribWriter.putValue(uStep());
			ribWriter.putBlank();
			ribWriter.putBasis(vBasis());
			ribWriter.putBlank();
			ribWriter.putValue(vStep());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiBasis &operator=(const CRiBasis &c)
		{
			if ( this == &c )
				return *this;
				
			set(c.uBasis(), c.uStep(), c.vBasis(), c.vStep());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiBasis


	///////////////////////////////////////////////////////////////////////////////
	class CRiTrimCurve : public CRManInterfaceCall {
	private:
		CTrimCurveData m_trimCurve;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiTrimCurve"; }
		inline virtual const char *className() const { return CRiTrimCurve::myClassName(); }

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
		 * The default constructor sets the size of a bounding box to infinity.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiTrimCurve(long aLineNo=-1) : TypeParent(aLineNo), m_trimCurve(0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}

		inline CRiTrimCurve(long aLineNo, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) : TypeParent(aLineNo), m_trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w) {}

		inline CRiTrimCurve(long aLineNo, const CTrimCurveData &CRimCurve)  : TypeParent(aLineNo), m_trimCurve(CRimCurve) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiTrimCurve(const CRiTrimCurve &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiTrimCurve()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiTrimCurve(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_TRIM_CURVE; }

		inline const CTrimCurveData &trimCurve() const { return m_trimCurve; }
		inline void trimCurve(const CTrimCurveData &t) { m_trimCurve = t; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preTrimCurve(*this, 
				m_trimCurve.m_data.m_nloops,
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

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doTrimCurve(*this, 
				m_trimCurve.m_data.m_nloops,
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

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postTrimCurve(*this, 
				m_trimCurve.m_data.m_nloops,
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

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_nCurves);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_order);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_knots);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_min);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_max);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_n);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_u);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_v);
			ribWriter.putBlank();
			ribWriter.putArray(m_trimCurve.m_w);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline CRiTrimCurve &operator=(const CRiTrimCurve &c)
		{
			if ( this == &c )
				return *this;
				
			trimCurve(c.trimCurve());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTrimCurve


	///////////////////////////////////////////////////////////////////////////////
	class CRiIlluminate : public CRManInterfaceCall {
	private:
		RtLightHandle m_handle;
		RtBoolean m_onoff;
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		inline static const char *myClassName(void) { return "CRiIlluminate"; }
		inline virtual const char *className() const { return CRiIlluminate::myClassName(); }

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

		inline CRiIlluminate(long aLineNo, RtLightHandle aHandle, RtBoolean onoff) :
			TypeParent(aLineNo), m_handle(aHandle), m_onoff(onoff)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiIlluminate(const CRiIlluminate &c)
		{
			*this = c;
		}

		inline virtual ~CRiIlluminate()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiIlluminate(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ILLUMINATE; }

		inline virtual void handle(RtLightHandle aHandle) { m_handle = aHandle; }

		inline virtual RtLightHandle handle() const { return m_handle; }

		inline virtual void onOff(RtBoolean onoff) { m_onoff = onoff; }

		inline virtual RtBoolean onOff() const { return m_onoff; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preIlluminate(*this, m_handle, m_onoff);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doIlluminate(*this, m_handle, m_onoff);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postIlluminate(*this, m_handle, m_onoff);
		}
	
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(handle());
			ribWriter.putBlank();
			ribWriter.putValue((RtInt)(onOff() ? 1 : 0));
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, const char *aName, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(aName);
			ribWriter.putBlank();
			ribWriter.putValue((RtInt)(onOff() ? 1 : 0));
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, unsigned long handleNo, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(handleNo);
			ribWriter.putBlank();
			ribWriter.putValue((RtInt)(onOff() ? 1 : 0));
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		inline CRiIlluminate &operator=(const CRiIlluminate &c)
		{
			if ( this == &c )
				return *this;

			handle(c.handle());
			onOff(c.onOff());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiIlluminate

}

#endif // _RICPP_RICONTEXT_RIMACROATTRIBUTES_H
