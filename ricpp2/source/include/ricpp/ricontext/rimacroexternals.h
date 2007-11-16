#ifndef _RICPP_RICONTEXT_RIMACROEXTERNALS_H
#define _RICPP_RICONTEXT_RIMACROEXTERNALS_H

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

/** @file rimacroexternals.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for externals (like textures), \see rimacro.h
 */
#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// External resources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief Create a texture map from a pixel file.
 */
class CRiMakeTexture : public CVarParamRManInterfaceCall {
private:
	std::string m_pic,         //!< File with the picture (input).
	            m_tex;         //!< File with the tecture (output, internal representation).
	RtToken     m_swrap,       //!< Wrap type of the texture in direction s.
	            m_twrap;       //!< Wrap type of the texture in direction t.
	IFilterFunc *m_filterfunc; //!< Filter function.
	RtFloat m_swidth,          //!< Width of influence of the pixel filer, direction s.
	        m_twidth;          //!< Width of influence of the pixel filer, direction t.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMakeTexture"; }
	inline virtual const char *className() const { return CRiMakeTexture::myClassName(); }

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

	/** @brief Default Constructor.
	 *
	 *  Empty object
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMakeTexture(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_swrap = 0;
		m_twrap = 0;
		m_filterfunc = 0;
		m_swidth = 0;
		m_twidth = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo     The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict        Dictonary with the current declarations.
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aSWrap      Wrap type of the texture in direction s.
	 *  @param aTWrap      Wrap type of the texture in direction t.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 *  @param n           Number of parameters (size of @a tokens, @a params).
	 *  @param tokens      Tokens of the request.
	 *  @param params      Parameter values of the request.
	 */
	inline CRiMakeTexture(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_filterfunc = 0;
		set(aPic, aTex, aSWrap, aTWrap, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo       The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPic          File with the picture (input).
	 *  @param aTex          File with the tecture (output, internal representation).
	 *  @param aSWrap        Wrap type of the texture in direction s.
	 *  @param aTWrap        Wrap type of the texture in direction t.
	 *  @param aFilterFunc   Filter function.
	 *  @param aSWidth       Width of influence of the pixel filer, direction s.
	 *  @param aTWidth       Width of influence of the pixel filer, direction t.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiMakeTexture(
		long aLineNo,
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_filterfunc = 0;
		set(aPic, aTex, aSWrap, aTWrap, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMakeTexture(const CRiMakeTexture &c)
	{
		m_filterfunc = 0;
		*this = c;
	}

	/** @brief Destructor.
	 *
	 *  Frees resources.
	 */
	inline virtual ~CRiMakeTexture()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMakeTexture(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_TEXTURE; }

	/** @brief Sets the element variables
	 *
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aSWrap      Wrap type of the texture in direction s.
	 *  @param aTWrap      Wrap type of the texture in direction t.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 */
	inline void set(
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc *aFilterFunc, RtFloat aSWidth, RtFloat aTWidth)
	{
		m_pic = noNullStr(aPic);
		m_tex = noNullStr(aTex);
		m_swrap = aSWrap;
		m_twrap = aTWrap;
		m_swidth = aSWidth;
		m_twidth = aTWidth;

		if ( m_filterfunc )
			delete m_filterfunc;
		m_filterfunc = 0;
		if ( aFilterFunc ) {
			m_filterfunc = aFilterFunc->duplicate();
			if ( !m_filterfunc ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeTexture, m_filterfunc", __LINE__, __FILE__);
			}
		}
	}

	/** @brief Gets the file name of the picture (input).
	 *
	 *  @return The file name of the picture (input).
	 */
	const RtString pic() const
	{
		return m_pic.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtString tex() const
	{
		return m_tex.c_str();
	}

	/** @brief Gets the wrap type of the texture in direction s.
	 *
	 *  @return The wrap type of the texture in direction s.
	 */
	const RtToken swrap() const
	{
		return m_swrap;
	}

	/** @brief Gets the wrap type of the texture in direction t.
	 *
	 *  @return The wrap type of the texture in direction t.
	 */
	const RtToken twrap() const
	{
		return m_twrap;
	}

	/** @brief Gets the filter function.
	 *
	 *  @return The filter function. 
	 */
	const IFilterFunc *filterfunc() const
	{
		return m_filterfunc;
	}

	/** @brief Gets the width of influence of the pixel filer, direction s.
	 *
	 *  @return The width of influence of the pixel filer, direction s.
	 */
	const RtFloat swidth() const
	{
		return m_swidth;
	}

	/** @brief Gets the width of influence of the pixel filer, direction t.
	 *
	 *  @return The width of influence of the pixel filer, direction t.
	 */
	const RtFloat twidth() const
	{
		return m_twidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.doMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.postMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiMakeTexture &operator=(const CRiMakeTexture &c)
	{
		if ( this == &c )
			return *this;

		set(c.pic(), c.tex(), c.swrap(), c.twrap(), c.filterfunc(), c.swidth(), c.twidth());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMakeTexture

///////////////////////////////////////////////////////////////////////////////
/** @brief Create a bump map from a pixel file.
 */
class CRiMakeBump : public CVarParamRManInterfaceCall {
private:
	std::string m_pic,         //!< File with the picture (input).
	            m_tex;         //!< File with the tecture (output, internal representation).
	RtToken     m_swrap,       //!< Wrap type of the texture in direction s.
	            m_twrap;       //!< Wrap type of the texture in direction t.
	IFilterFunc *m_filterfunc; //!< Filter function.
	RtFloat m_swidth,          //!< Width of influence of the pixel filer, direction s.
	        m_twidth;          //!< Width of influence of the pixel filer, direction t.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMakeBump"; }
	inline virtual const char *className() const { return CRiMakeBump::myClassName(); }


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

	/** @brief Default Constructor.
	 *
	 *  Empty object
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMakeBump(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_swrap = 0;
		m_twrap = 0;
		m_filterfunc = 0;
		m_swidth = 0;
		m_twidth = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo     The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict        Dictonary with the current declarations.
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aSWrap      Wrap type of the texture in direction s.
	 *  @param aTWrap      Wrap type of the texture in direction t.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 *  @param n           Number of parameters (size of @a tokens, @a params).
	 *  @param tokens      Tokens of the request.
	 *  @param params      Parameter values of the request.
	 */
	inline CRiMakeBump(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_filterfunc = 0;
		set(aPic, aTex, aSWrap, aTWrap, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo       The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPic          File with the picture (input).
	 *  @param aTex          File with the tecture (output, internal representation).
	 *  @param aSWrap        Wrap type of the texture in direction s.
	 *  @param aTWrap        Wrap type of the texture in direction t.
	 *  @param aFilterFunc   Filter function.
	 *  @param aSWidth       Width of influence of the pixel filer, direction s.
	 *  @param aTWidth       Width of influence of the pixel filer, direction t.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiMakeBump(
		long aLineNo,
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_filterfunc = 0;
		set(aPic, aTex, aSWrap, aTWrap, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMakeBump(const CRiMakeBump &c)
	{
		m_filterfunc = 0;
		*this = c;
	}

	/** @brief Destructor.
	 *
	 *  Frees resources.
	 */
	inline virtual ~CRiMakeBump()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMakeBump(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_BUMP; }


	/** @brief Sets the element variables
	 *
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aSWrap      Wrap type of the texture in direction s.
	 *  @param aTWrap      Wrap type of the texture in direction t.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 */
	inline void set(
		RtString aPic, RtString aTex, RtToken aSWrap, RtToken aTWrap,
		const IFilterFunc *aFilterFunc, RtFloat aSWidth, RtFloat aTWidth)
	{
		m_pic = noNullStr(aPic);
		m_tex = noNullStr(aTex);
		m_swrap = aSWrap;
		m_twrap = aTWrap;
		m_swidth = aSWidth;
		m_twidth = aTWidth;

		if ( m_filterfunc )
			delete m_filterfunc;
		m_filterfunc = 0;
		if ( aFilterFunc ) {
			m_filterfunc = aFilterFunc->duplicate();
			if ( !m_filterfunc ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeBump, m_filterfunc", __LINE__, __FILE__);
			}
		}
	}

	/** @brief Gets the file name of the picture (input).
	 *
	 *  @return The file name of the picture (input).
	 */
	const RtString pic() const
	{
		return m_pic.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtString tex() const
	{
		return m_tex.c_str();
	}

	/** @brief Gets the wrap type of the texture in direction s.
	 *
	 *  @return The wrap type of the texture in direction s.
	 */
	const RtToken sWrap() const
	{
		return m_swrap;
	}

	/** @brief Gets the wrap type of the texture in direction t.
	 *
	 *  @return The wrap type of the texture in direction t.
	 */
	const RtToken tWrap() const
	{
		return m_twrap;
	}

	/** @brief Gets the filter function.
	 *
	 *  @return The filter function. 
	 */
	const IFilterFunc *filterFunc() const
	{
		return m_filterfunc;
	}

	/** @brief Gets the width of influence of the pixel filer, direction s.
	 *
	 *  @return The width of influence of the pixel filer, direction s.
	 */
	const RtFloat sWidth() const
	{
		return m_swidth;
	}

	/** @brief Gets the width of influence of the pixel filer, direction t.
	 *
	 *  @return The width of influence of the pixel filer, direction t.
	 */
	const RtFloat tWidth() const
	{
		return m_twidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.doMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.postMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap, m_twrap, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiMakeBump &operator=(const CRiMakeBump &c)
	{
		if ( this == &c )
			return *this;

		set(c.pic(), c.tex(), c.sWrap(), c.tWrap(), c.filterFunc(), c.sWidth(), c.tWidth());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMakeBump


///////////////////////////////////////////////////////////////////////////////
/** @brief Create an environment map from a pixel file.
 */
class CRiMakeLatLongEnvironment : public CVarParamRManInterfaceCall {
private:
	std::string m_pic,         //!< File with the picture (input).
	            m_tex;         //!< File with the tecture (output, internal representation).
	IFilterFunc *m_filterfunc; //!< Filter function.
	RtFloat m_swidth,          //!< Width of influence of the pixel filer, direction s.
	        m_twidth;          //!< Width of influence of the pixel filer, direction t.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMakeLatLongEnvironment"; }
	inline virtual const char *className() const { return CRiMakeLatLongEnvironment::myClassName(); }


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

	/** @brief Default Constructor.
	 *
	 *  Empty object
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMakeLatLongEnvironment(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_filterfunc = 0;
		m_swidth = 0;
		m_twidth = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo     The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict        Dictonary with the current declarations.
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 *  @param n           Number of parameters (size of @a tokens, @a params).
	 *  @param tokens      Tokens of the request.
	 *  @param params      Parameter values of the request.
	 */
	inline CRiMakeLatLongEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aPic, RtString aTex,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_filterfunc = 0;
		set(aPic, aTex, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo       The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPic          File with the picture (input).
	 *  @param aTex          File with the tecture (output, internal representation).
	 *  @param aFilterFunc   Filter function.
	 *  @param aSWidth       Width of influence of the pixel filer, direction s.
	 *  @param aTWidth       Width of influence of the pixel filer, direction t.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiMakeLatLongEnvironment(
		long aLineNo,
		RtString aPic, RtString aTex,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_filterfunc = 0;
		set(aPic, aTex, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMakeLatLongEnvironment(const CRiMakeLatLongEnvironment &c)
	{
		m_filterfunc = 0;
		*this = c;
	}

	/** @brief Destructor.
	 *
	 *  Frees resources.
	 */
	inline virtual ~CRiMakeLatLongEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_LAT_LONG_ENVIRONMENT; }

	/** @brief Sets the element variables
	 *
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aSWrap      Wrap type of the texture in direction s.
	 *  @param aTWrap      Wrap type of the texture in direction t.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 */
	inline void set(
		RtString aPic, RtString aTex,
		const IFilterFunc *aFilterFunc, RtFloat aSWidth, RtFloat aTWidth)
	{
		m_pic = noNullStr(aPic);
		m_tex = noNullStr(aTex);
		m_swidth = aSWidth;
		m_twidth = aTWidth;

		if ( m_filterfunc )
			delete m_filterfunc;
		m_filterfunc = 0;
		if ( aFilterFunc ) {
			m_filterfunc = aFilterFunc->duplicate();
			if ( !m_filterfunc ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeLatLongEnvironment, m_filterfunc", __LINE__, __FILE__);
			}
		}
	}

	/** @brief Gets the file name of the picture (input).
	 *
	 *  @return The file name of the picture (input).
	 */
	const RtString pic() const
	{
		return m_pic.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtString tex() const
	{
		return m_tex.c_str();
	}

	/** @brief Gets the filter function.
	 *
	 *  @return The filter function. 
	 */
	const IFilterFunc *filterFunc() const
	{
		return m_filterfunc;
	}

	/** @brief Gets the width of influence of the pixel filer, direction s.
	 *
	 *  @return The width of influence of the pixel filer, direction s.
	 */
	const RtFloat sWidth() const
	{
		return m_swidth;
	}

	/** @brief Gets the width of influence of the pixel filer, direction t.
	 *
	 *  @return The width of influence of the pixel filer, direction t.
	 */
	const RtFloat tWidth() const
	{
		return m_twidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.doMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.postMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiMakeLatLongEnvironment &operator=(const CRiMakeLatLongEnvironment &c)
	{
		if ( this == &c )
			return *this;

		set(c.pic(), c.tex(), c.filterFunc(), c.sWidth(), c.tWidth());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMakeLatLongEnvironment


///////////////////////////////////////////////////////////////////////////////
/** @brief Create a cube face environment map from a pixel file.
 */
class CRiMakeCubeFaceEnvironment : public CVarParamRManInterfaceCall {
private:
	std::string m_px,          //!< Texture of the cube face to positive x axis.
	            m_nx,          //!< Texture of the cube face to negative x axis.
				m_py,          //!< Texture of the cube face to positive y axis.
				m_ny,          //!< Texture of the cube face to negative y axis.
				m_pz,          //!< Texture of the cube face to positive z axis.
				m_nz,          //!< Texture of the cube face to negative z axis.
				m_tex;         //!< Wrap type of the texture in direction t.
	RtFloat     m_fov;         //!< Field of view of the texture projection.
	IFilterFunc *m_filterfunc; //!< Filter function.
	RtFloat     m_swidth,      //!< Width of influence of the pixel filer, direction s.
	            m_twidth;      //!< Width of influence of the pixel filer, direction t.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMakeCubeFaceEnvironment"; }
	inline virtual const char *className() const { return CRiMakeCubeFaceEnvironment::myClassName(); }

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

	/** @brief Default Constructor.
	 *
	 *  Empty object.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMakeCubeFaceEnvironment(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_fov = 0;
		m_filterfunc = 0;
		m_swidth = 0;
		m_twidth = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo     The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict        Dictonary with the current declarations.
	 *  @param aPx         Texture of the cube face to positive x axis.
	 *  @param aNx         Texture of the cube face to negative x axis.
	 *  @param aPy         Texture of the cube face to positive y axis.
	 *  @param aNy         Texture of the cube face to negative y axis.
	 *  @param aPz         Texture of the cube face to positive z axis.
	 *  @param aNz         Texture of the cube face to negative z axis.
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aFov        Field of view of the texture projection.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 *  @param n           Number of parameters (size of @a tokens, @a params).
	 *  @param tokens      Tokens of the request.
	 *  @param params      Parameter values of the request.
	 */
	inline CRiMakeCubeFaceEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aPx, RtString aNx, RtString aPy, RtString aNy, RtString aPz, RtString aNz,
		RtString aTex, RtFloat aFov,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_filterfunc = 0;
		set(aPx, aNx, aPy, aNy, aPz, aNz, aTex, aFov, &aFilterFunc, aSWidth, aTWidth);
	}


	/** @brief Constructor.
	 *
	 *  @param aLineNo       The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPx           Texture of the cube face to positive x axis.
	 *  @param aNx           Texture of the cube face to negative x axis.
	 *  @param aPy           Texture of the cube face to positive y axis.
	 *  @param aNy           Texture of the cube face to negative y axis.
	 *  @param aPz           Texture of the cube face to positive z axis.
	 *  @param aNz           Texture of the cube face to negative z axis.
	 *  @param aTex          File with the tecture (output, internal representation).
	 *  @param aFov          Field of view of the texture projection.
	 *  @param aFilterFunc   Filter function.
	 *  @param aSWidth       Width of influence of the pixel filer, direction s.
	 *  @param aTWidth       Width of influence of the pixel filer, direction t.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiMakeCubeFaceEnvironment(
		long aLineNo,
		RtString aPx, RtString aNx, RtString aPy, RtString aNy, RtString aPz, RtString aNz,
		RtString aTex, RtFloat aFov,
		const IFilterFunc &aFilterFunc, RtFloat aSWidth, RtFloat aTWidth,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_filterfunc = 0;
		set(aPx, aNx, aPy, aNy, aPz, aNz, aTex, aFov, &aFilterFunc, aSWidth, aTWidth);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMakeCubeFaceEnvironment(const CRiMakeCubeFaceEnvironment &c)
	{
		m_filterfunc = 0;
		*this = c;
	}

	/** @brief Destructor.
	 *
	 *  Frees resources.
	 */
	inline virtual ~CRiMakeCubeFaceEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMakeCubeFaceEnvironment(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_CUBE_FACE_ENVIRONMENT; }

	/** @brief Sets the element variables
	 *
	 *  @param aPx         Texture of the cube face to positive x axis.
	 *  @param aNx         Texture of the cube face to negative x axis.
	 *  @param aPy         Texture of the cube face to positive y axis.
	 *  @param aNy         Texture of the cube face to negative y axis.
	 *  @param aPz         Texture of the cube face to positive z axis.
	 *  @param aNz         Texture of the cube face to negative z axis.
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param aFov        Field of view of the texture projection.
	 *  @param aFilterFunc Filter function.
	 *  @param aSWidth     Width of influence of the pixel filer, direction s.
	 *  @param aTWidth     Width of influence of the pixel filer, direction t.
	 */
	inline void set(
		RtString aPx, RtString aNx, RtString aPy, RtString aNy, RtString aPz, RtString aNz,
		RtString aTex, RtFloat aFov,
		const IFilterFunc *aFilterFunc, RtFloat aSWidth, RtFloat aTWidth)
	{
		m_px = aPx;
		m_nx = aNx;
		m_py = aPy;
		m_ny = aNy;
		m_pz = aPz;
		m_nz = aNz;
		m_tex = noNullStr(aTex);
		m_fov = aFov;
		m_swidth = aSWidth;
		m_twidth = aTWidth;

		if ( m_filterfunc )
			delete m_filterfunc;
		m_filterfunc = 0;
		if ( aFilterFunc ) {
			m_filterfunc = aFilterFunc->duplicate();
			if ( !m_filterfunc ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeCubeFaceEnvironment, m_filterfunc", __LINE__, __FILE__);
			}
		}
	}

	/** @brief Gets the texture of the cube face to positive x axis.
	 *
	 *  @return The texture of the cube face to positive x axis.
	 */
	const RtString px() const
	{
		return m_px.c_str();
	}

	/** @brief Gets the texture of the cube face to negative x axis.
	 *
	 *  @return The texture of the cube face to negative x axis.
	 */
	const RtString nx() const
	{
		return m_nx.c_str();
	}

	/** @brief Gets the texture of the cube face to positive y axis.
	 *
	 *  @return The texture of the cube face to positive y axis.
	 */
	const RtString py() const
	{
		return m_py.c_str();
	}

	/** @brief Gets the texture of the cube face to negative y axis.
	 *
	 *  @return The texture of the cube face to negative y axis.
	 */
	const RtString ny() const
	{
		return m_ny.c_str();
	}

	/** @brief Gets the texture of the cube face to positive z axis.
	 *
	 *  @return The texture of the cube face to positive z axis.
	 */
	const RtString pz() const
	{
		return m_pz.c_str();
	}

	/** @brief Gets the texture of the cube face to negative z axis.
	 *
	 *  @return The texture of the cube face to negative z axis.
	 */
	const RtString nz() const
	{
		return m_nz.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtString tex() const
	{
		return m_tex.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtFloat fov() const
	{
		return m_fov;
	}

	/** @brief Gets the filter function.
	 *
	 *  @return The filter function. 
	 */
	const IFilterFunc *filterFunc() const
	{
		return m_filterfunc;
	}

	/** @brief Gets the width of influence of the pixel filer, direction s.
	 *
	 *  @return The width of influence of the pixel filer, direction s.
	 */
	const RtFloat sWidth() const
	{
		return m_swidth;
	}

	/** @brief Gets the width of influence of the pixel filer, direction t.
	 *
	 *  @return The width of influence of the pixel filer, direction t.
	 */
	const RtFloat tWidth() const
	{
		return m_twidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.doMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.postMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiMakeCubeFaceEnvironment &operator=(const CRiMakeCubeFaceEnvironment &c)
	{
		if ( this == &c )
			return *this;

		set(c.px(), c.nx(), c.py(), c.ny(), c.pz(), c.nz(), c.tex(), c.fov(), c.filterFunc(), c.sWidth(), c.tWidth());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMakeCubeFaceEnvironment

///////////////////////////////////////////////////////////////////////////////
/** @brief Create a shadowmap from pixel file.
 */
class CRiMakeShadow : public CVarParamRManInterfaceCall {
private:
	std::string m_pic,         //!< File with the picture (input).
	            m_tex;         //!< File with the tecture (output, internal representation).
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMakeShadow"; }
	inline virtual const char *className() const { return CRiMakeShadow::myClassName(); }

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

	/** @brief Default Constructor.
	 *
	 *  Empty object
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMakeShadow(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo     The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict        Dictonary with the current declarations.
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 *  @param n           Number of parameters (size of @a tokens, @a params).
	 *  @param tokens      Tokens of the request.
	 *  @param params      Parameter values of the request.
	 */
	inline CRiMakeShadow(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *aPic, const char *aTex,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		set(aPic, aTex);
	}


	/** @brief Constructor.
	 *
	 *  @param aLineNo       The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPic          File with the picture (input).
	 *  @param aTex          File with the tecture (output, internal representation).
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiMakeShadow(
		long aLineNo,
		RtString aPic, RtString aTex,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		set(aPic, aTex);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMakeShadow(const CRiMakeShadow &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiMakeShadow()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMakeShadow(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_SHADOW; }


	/** @brief Sets the element variables
	 *
	 *  @param aPic        File with the picture (input).
	 *  @param aTex        File with the tecture (output, internal representation).
	 */
	inline void set(
		RtString aPic, RtString aTex)
	{
		m_pic = noNullStr(aPic);
		m_tex = noNullStr(aTex);
	}

	/** @brief Gets the file name of the picture (input).
	 *
	 *  @return The file name of the picture (input).
	 */
	const RtString pic() const
	{
		return m_pic.c_str();
	}

	/** @brief Gets the file name/resource name of the texture (output).
	 *
	 *  @return The file name/resource name of the texture (output).
	 */
	const RtString tex() const
	{
		return m_tex.c_str();
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiMakeShadow &operator=(const CRiMakeShadow &c)
	{
		if ( this == &c )
			return *this;

		set(c.pic(), c.tex());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiMakeShadow


///////////////////////////////////////////////////////////////////////////////
/* @todo Creates a 3D texture map.
 *
 *  Can be used by the texture3D function of the Shading Language.
 *
 *  @param nNames Number of poinbt clouds
 *  @param ptcnames Pointer to filenames of the point clouds
 *  @param bkmname Filename for the 3d texture map (output).
 *  @param n Number of tokens
 *  @param tokens Tokens for additional parameter list
 *  @param params Value pointer for additional parameter list
 */
// virtual RtVoid makeBrickMapV(RtInt nNames, RtString *ptcnames, RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

///////////////////////////////////////////////////////////////////////////////
/** @brief Archive record (comment)
 */
class CRiArchiveRecord : public CRManInterfaceCall {
private:
	RtToken     m_type; //!< Type of the archive record.
	std::string m_line; //!< Text content of the archive record.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiArchiveRecord"; }
	inline virtual const char *className() const { return CRiArchiveRecord::myClassName(); }

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

	/** @brief Default Constructor.
	 *
	 *  Empty object
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiArchiveRecord(long aLineNo=-1)
		: CRManInterfaceCall(aLineNo)
	{
		m_type = 0;
	}

	inline CRiArchiveRecord(long aLineNo, RtToken aType, RtString aLine)
		: CRManInterfaceCall(aLineNo), m_type(aType), m_line(noNullStr(aLine))
	{
	}
	
	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiArchiveRecord(const CRiArchiveRecord &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiArchiveRecord()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiArchiveRecord(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_RECORD; }

	/** @brief sets the member variables.
	 *
	 *  @param aType The type of the archive record.
	 *  @param aLine  The text content of the archive record.
	 */
	void set(RtToken aType, RtString aLine)
	{
		m_type = aType;
		m_line = noNullStr(aLine);
	}

	/** @brief Gets the type of the archive record.
	 *
	 *  @return The type of the archive record.
	 */
	RtToken type() const
	{
		return m_type;
	}

	/** @brief Gets the text content of the archive record.
	 *
	 *  @retrun The text content of the archive record.
	 */
	RtString line() const
	{
		return m_line.c_str(); 
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preArchiveRecord(m_type, m_line.c_str());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doArchiveRecord(m_type, m_line.c_str());
		if ( cb ) 
			(*cb)(ri, m_type, m_line.c_str());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postArchiveRecord(m_type, m_line.c_str());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiArchiveRecord &operator=(const CRiArchiveRecord &c)
	{
		if ( this == &c )
			return *this;

		set(c.type(), c.line());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiArchiveRecord

///////////////////////////////////////////////////////////////////////////////
/** @brief Read a RIB archive
 */
class CRiReadArchive : public CVarParamRManInterfaceCall {
private:
	std::string       m_filename; //!< Filename of the RIB archive.
	IArchiveCallback *m_callback; //!< Optional callbback for archive records.
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiReadArchive"; }
	inline virtual const char *className() const { return CRiReadArchive::myClassName(); }

	inline CRiReadArchive(long aLineNo=-1)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_callback =0;
	}

	inline CRiReadArchive(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aFilename, const IArchiveCallback *cb,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_filename(noNullStr(aFilename))
	{
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline CRiReadArchive(
		long aLineNo,
		RtString aFilename, const IArchiveCallback *cb,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_filename(aFilename)
	{
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline virtual ~CRiReadArchive()
	{
		if ( m_callback )
			delete m_callback;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
	
	inline const IArchiveCallback *callback() const
	{
		return m_callback;
	}
	inline void callback(const IArchiveCallback *cb)
	{
		if ( m_callback )
			delete m_callback;
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline RtString filename() const
	{
		return m_filename.c_str();
	}
	inline void filename(RtString aFilename)
	{
		m_filename = noNullStr(aFilename);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, m_callback);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, m_callback);
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, m_callback);
	}

	inline CRiReadArchive &operator=(const CRiReadArchive &c)
	{
		if ( this == &c )
			return *this;

		filename(c.filename());
		callback(c.callback());
		
		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiReadArchive

}

#endif // _RICPP_RICONTEXT_RIMACROEXTERNALS_H