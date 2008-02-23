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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the attribute as atomized string.
	 */
	inline CRiAttribute(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_ATTRIBUTE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preAttribute(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doAttribute(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postAttribute(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAttribute


///////////////////////////////////////////////////////////////////////////////
/** @brief Surface color.
 */
class CRiColor : public CRManInterfaceCall {
private:
	std::vector<RtFloat> m_color; ///< Color

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param colorSamples Number of samples per color value
	 *  @param Cs Surface color value
	 */
	inline CRiColor(long aLineNo=-1, RtInt colorSamples=0, const RtFloat *Cs=0)
		: CRManInterfaceCall(aLineNo)
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
		ri.preColor(m_color.empty() ? 0 : &m_color[0]);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doColor(m_color.empty() ? 0 : &m_color[0]);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postColor(m_color.empty() ? 0 : &m_color[0]);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiColor


///////////////////////////////////////////////////////////////////////////////
/** @brief Surface opacity.
 */
class CRiOpacity : public CRManInterfaceCall {
private:
	std::vector<RtFloat> m_opacity;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param colorSamples Number of samples per color value
	 *  @param Os Surface opacity value
	 */
	inline CRiOpacity(long aLineNo=-1, RtInt colorSamples=0, const RtFloat *Os=0)
		: CRManInterfaceCall(aLineNo)
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
		ri.preOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiOpacity


///////////////////////////////////////////////////////////////////////////////
/** @brief Surface shader.
 */
class CRiSurface : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the shader as token.

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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the shader as atomized string.
	 */
	inline CRiSurface(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_SURFACE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preSurface(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSurface(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSurface(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSurface


///////////////////////////////////////////////////////////////////////////////
/** @brief Atmosphere shader.
 */
class CRiAtmosphere : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the shader as token.

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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the shader as atomized string.
	 */
	inline CRiAtmosphere(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_ATMOSPHERE, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preAtmosphere(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doAtmosphere(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postAtmosphere(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAtmosphere


///////////////////////////////////////////////////////////////////////////////
/** @brief Interior shader.
 */
class CRiInterior : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the shader as token.

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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the shader as atomized string.
	 */
	inline CRiInterior(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_INTERIOR, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preInterior(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doInterior(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postInterior(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiInterior


///////////////////////////////////////////////////////////////////////////////
/** @brief Exterior shader.
 */
class CRiExterior : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the shader as token.

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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the shader as atomized string.
	 */
	inline CRiExterior(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_EXTERIOR, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preExterior(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doExterior(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postExterior(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiExterior


///////////////////////////////////////////////////////////////////////////////
/** @brief Displacemet shader.
 */
class CRiDisplacement : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the shader as token.

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
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the shader as atomized string.
	 */
	inline CRiDisplacement(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, RI_DISPLACEMENT, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
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
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
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
		ri.preDisplacement(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisplacement(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisplacement(m_name, parameters());
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

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplacement


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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		: CRManInterfaceCall(aLineNo),
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
		ri.preTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTextureCoordinates


///////////////////////////////////////////////////////////////////////////////
/** @brief The minimum rate of surface shading
 */
class CRiShadingRate : public CRManInterfaceCall {
private:
	RtFloat m_size; ///< Minimum rate of surface shading.

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aSize Minimum rate of surface shading.
	 */
	inline CRiShadingRate(
		long aLineNo=-1,
		RtFloat aSize = CAttributes::defShadingRate)
		: CRManInterfaceCall(aLineNo), m_size(aSize)
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
		ri.preShadingRate(m_size);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doShadingRate(m_size);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postShadingRate(m_size);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiShadingRate

///////////////////////////////////////////////////////////////////////////////
/** @brief The type of the shading interpolation
 */
class CRiShadingInterpolation : public CRManInterfaceCall {
private:
	RtToken m_type; ///< Type of the shading interpolation (RI_CONSTANT, RI_SMOOTH).

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType The shading type.
	 */
	inline CRiShadingInterpolation(
		long aLineNo=-1,
		RtToken aType = CAttributes::defShadingInterpolation)
		: CRManInterfaceCall(aLineNo), m_type(aType)
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
		ri.preShadingInterpolation(m_type);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doShadingInterpolation(m_type);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postShadingInterpolation(m_type);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiShadingInterpolation


///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the matte mode
 */
class CRiMatte : public CRManInterfaceCall {
private:
	RtBoolean m_onoff; ///< Switches the matte mode on (RI_TRUE) or off (RI_FALSE)

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param anOnOff RI_TRUE, matte is on and RI_FALSE, matte is off
	 */
	inline CRiMatte(long aLineNo = -1, RtBoolean anOnOff = CAttributes::defMatte)
		: CRManInterfaceCall(aLineNo), m_onoff(anOnOff)
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
	RtBoolean onoff() const
	{
		return m_onoff;
	}

	/** @brief Sets the matte mode.
	 *
	 *  @param anOnOff RI_TRUE, matte is on and RI_FALSE, matte is off
	 */
	void onoff(RtBoolean anOnOff)
	{
		m_onoff = anOnOff;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMatte(m_onoff);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doMatte(m_onoff);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postMatte(m_onoff);
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

		onoff(c.onoff());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMatte


///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the bounding box for subsequent primitives.
 */
class CRiBound : public CRManInterfaceCall {
private:
	RtBound m_bound; ///< The bounding box

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the size of a bounding box to infinity.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiBound(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
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
		: CRManInterfaceCall(aLineNo)
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
		ri.preBound(m_bound);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doBound(m_bound);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postBound(m_bound);
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

		CRManInterfaceCall::operator=(c);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the size of a bounding box to infinity.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiDetail(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
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
		: CRManInterfaceCall(aLineNo)
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
		ri.preDetail(m_bound);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDetail(m_bound);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDetail(m_bound);
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

		CRManInterfaceCall::operator=(c);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		: CRManInterfaceCall(aLineNo),
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
		ri.preDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
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

		CRManInterfaceCall::operator=(c);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtToken aType = CAttributes::defGeometricApproximationType,
		RtFloat aValue = CAttributes::defGeometricApproximationValue)
	: CRManInterfaceCall(aLineNo), m_type(aType), m_value(aValue)
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
		ri.preGeometricApproximation(m_type, m_value);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doGeometricApproximation(m_type, m_value);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postGeometricApproximation(m_type, m_value);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiGeometricApproximation


///////////////////////////////////////////////////////////////////////////////
class CRiGeometricRepresentation : public CRManInterfaceCall {
private:
	RtToken m_type;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtToken aType = CAttributes::defGeometricRepresentation)
	: CRManInterfaceCall(aLineNo), m_type(aType)
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
		ri.preGeometricRepresentation(m_type);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doGeometricRepresentation(m_type);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postGeometricRepresentation(m_type);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiGeometricRepresentation


///////////////////////////////////////////////////////////////////////////////
class CRiOrientation : public CRManInterfaceCall {
private:
	RtToken m_orientation;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtToken anOrientation = CAttributes::defOrientation)
	: CRManInterfaceCall(aLineNo), m_orientation(anOrientation)
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
		ri.preOrientation(m_orientation);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doOrientation(m_orientation);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postOrientation(m_orientation);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiOrientation


///////////////////////////////////////////////////////////////////////////////
class CRiReverseOrientation : public CRManInterfaceCall {
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the size of a bounding box to infinity.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiReverseOrientation(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

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
		ri.preReverseOrientation();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doReverseOrientation();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postReverseOrientation();
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiReverseOrientation


///////////////////////////////////////////////////////////////////////////////
class CRiSides : public CRManInterfaceCall {
private:
	RtInt m_nsides;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
	: CRManInterfaceCall(aLineNo), m_nsides(theNSides)
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
		ri.preSides(m_nsides);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSides(m_nsides);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSides(m_nsides);
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSides


///////////////////////////////////////////////////////////////////////////////
class CRiBasis : public CRManInterfaceCall {
private:
	RtInt m_ustep, m_vstep;
	RtBasis m_ubasis, m_vbasis;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the size of a bounding box to infinity.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiBasis(long aLineNo = -1)
	: CRManInterfaceCall(aLineNo)
	{
		set(RiBezierBasis, RI_BEZIERSTEP, RiBezierBasis, RI_BEZIERSTEP);
	}

	inline CRiBasis(long aLineNo, const RtBasis anUBasis, RtInt anUStep, const RtBasis aVBasis, RtInt aVStep)
	: CRManInterfaceCall(aLineNo), m_ustep(anUStep), m_vstep(aVStep)
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
	
	const RtBasis &ubasis() const
	{
		return m_ubasis;
	}

	void ubasis(const RtBasis &anUBasis)
	{
		memcpy(m_ubasis, anUBasis, sizeof(RtBasis));
	}

	void ubasis(const RtBasis &anUBasis, RtInt anUStep)
	{
		memcpy(m_ubasis, anUBasis, sizeof(RtBasis));
		m_ustep = anUStep;
	}

	const RtBasis &vbasis() const
	{
		return m_vbasis;
	}

	void vbasis(const RtBasis &aVBasis)
	{
		memcpy(m_vbasis, aVBasis, sizeof(RtBasis));
	}

	void vbasis(const RtBasis &aVBasis, RtInt aVStep)
	{
		memcpy(m_vbasis, aVBasis, sizeof(RtBasis));
		m_vstep = aVStep;
	}

	RtInt ustep() const
	{
		return m_ustep;
	}
	
	void ustep(RtInt anUStep)
	{
		m_ustep = anUStep;
	}

	RtInt vstep() const
	{
		return m_vstep;
	}
	
	void vstep(RtInt aVStep)
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
		ri.preBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
	}

	inline CRiBasis &operator=(const CRiBasis &c)
	{
		if ( this == &c )
			return *this;
			
		set(c.ubasis(), c.ustep(), c.vbasis(), c.vstep());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiBasis


///////////////////////////////////////////////////////////////////////////////
class CRiTrimCurve : public CRManInterfaceCall {
private:
	CTrimCurveData m_trimCurve;

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the size of a bounding box to infinity.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiTrimCurve(long aLineNo=-1) : CRManInterfaceCall(aLineNo), m_trimCurve(0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}

	inline CRiTrimCurve(long aLineNo, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) : CRManInterfaceCall(aLineNo), m_trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w) {}

	inline CRiTrimCurve(long aLineNo, const CTrimCurveData &CRimCurve)  : CRManInterfaceCall(aLineNo), m_trimCurve(CRimCurve) {}

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
		ri.preTrimCurve(m_trimCurve.m_data.m_nloops,
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
		ri.doTrimCurve(m_trimCurve.m_data.m_nloops,
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
		ri.postTrimCurve(m_trimCurve.m_data.m_nloops,
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

	inline CRiTrimCurve &operator=(const CRiTrimCurve &c)
	{
		if ( this == &c )
			return *this;
			
		trimCurve(c.trimCurve());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTrimCurve


///////////////////////////////////////////////////////////////////////////////
class CRiIlluminate : public CRManInterfaceCall {
protected:
	RtLightHandle m_handle;
	RtBoolean m_onoff;
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	inline CRiIlluminate(long aLineNo, RtLightHandle aHandle, RtBoolean onoff) :
		CRManInterfaceCall(aLineNo), m_handle(aHandle), m_onoff(onoff)
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

#endif // _RICPP_RICONTEXT_RIMACROATTRIBUTES_H
