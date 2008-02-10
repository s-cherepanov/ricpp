#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#define _RICPP_RICONTEXT_RIMACROBASE_H

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

/** @file rimacrobase.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The base classes for macro processing, @see rimacro.h
 */
#ifndef _RICPP_RICONTEXT_DORENDER_H
#include "ricpp/ricontext/dorender.h"
#endif // _RICPP_RICONTEXT_DORENDER_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Macro base classes
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls.
 */
class CRManInterfaceCall {
private:
	long m_lineNo; ///< Place to store the line number of a call in a RIB file, -1 if there is no line number.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Constructor, to set a line number (defaults to -1).
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 */
	inline CRManInterfaceCall(long aLineNo = -1): m_lineNo(aLineNo) {}

	/** @brief Copy constructor
	 *
	 *  @param c Object to copy
	 */
	inline CRManInterfaceCall(const CRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRManInterfaceCall() {}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_UNKNOWN;}

	/** @brief Gets the name of the RI request as token.
	 *
	 *  @return The name of the RI request as token.
	 */
	inline virtual RtToken requestName() const { return CRequestInfo::requestName(interfaceIdx()); }

	/** @brief Replays the interface call.
	 *
	 *  @param ri The renderer backend used for replay.
	 *  @param cb Archive callback.
	 */
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.replayRequest(*this, cb);
	}

	/** @brief Asks for the line number.
	 *
	 *  @return The line number, -1 if there is none.
	 */
	inline long lineNo() const { return m_lineNo; }

	/** @brief Sets the line number.
	 *
	 *  @param aLineNo The new line number, -1 if there is none.
	 */
	inline void lineNo(long aLineNo) { m_lineNo=aLineNo; }

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRManInterfaceCall &operator=(const CRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		lineNo(c.lineNo());
		return *this;
	}

	/** @brief Pre-Processes an interface call
	 *
	 *  Pre-Processing normally involves state update of the renderer.
	 *  Is also performed before macro insertion.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Processes an interface call
	 *
	 *  Processing the interface call, e.g. do the rendering. This is
	 *  not called at a macro (object) definition.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for processing.
	 */
	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Post-Processes an interface call
	 *
	 *  Post-Processing the interface call, is called after
	 *  Macro insertion or processing.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Pre-Processes an interface call
	 *
	 *  Pre-Processing normally involves state update of the renderer.
	 *  Is also performed before macro insertion.
	 *
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, 0);
	}

	/** @brief Processes an interface call
	 *
	 *  Processing the interface call, e.g. do the rendering. This is
	 *  not called at a macro (object) definition.
	 *
	 *  @param ri The renderer backend used for processing.
	 */
	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, 0);
	}

	/** @brief Post-Processes an interface call
	 *
	 *  Post-Processing the interface call, is called after
	 *  Macro insertion or processing.
	 *
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, 0);
	}
}; // CRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls with parameter lists.
 */
class CVarParamRManInterfaceCall : public CRManInterfaceCall {
private:
	CParameterList m_parameters;             ///< Parameters of an interface call.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CVarParamRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CVarParamRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to a kind of class atomizedClassName
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Constructor.
	 *
	 *  Constructs an empty parameter list, with line number, defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CVarParamRManInterfaceCall(long aLineNo=-1) :
		CRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param decl Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CVarParamRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CRManInterfaceCall(aLineNo)
	{
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param theParameters Parsed parameter list.
	 */
	inline CVarParamRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CRManInterfaceCall(aLineNo)
	{
		parameters(theParameters);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CVarParamRManInterfaceCall(const CVarParamRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CVarParamRManInterfaceCall() {}
	
	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CVarParamRManInterfaceCall(*this);
	}

	/** @brief Gets the size of tokens and parameters of the request.
	 *
	 * @return The size of tokens and parameters of the request.
	 */
	inline virtual CParameterList::size_type size() const
	{
		return m_parameters.size();
	}

	/** @brief Gets the tokens of the request.
	 *
	 * For the size of the array @see size(), for the parameter pointers @see getParams().
	 *
	 * @return The tokens of the request.
	 */
	inline RtToken *getTokens()
	{
		return m_parameters.tokenPtr();
	}

	/** @brief Gets the parameter pointers of the request.
	 *
	 * For the size of the array @see size(), for the tokenss @see getTokens().
	 *
	 * @return The parameters of the request.
	 */
	inline RtPointer *getParams()
	{
		return m_parameters.valuePtr();
	}

	/** @brief Gets a writeable reference of the parameter list.
	 *
	 *  @return Writeable reference of the parameter list interface.
	 */
	inline CParameterList &parameters()
	{
		return m_parameters;
	}

	/** @brief Gets a read only reference of the parameter list.
	 *
	 *  @return Read only reference of the parameter list interface.
	 */
	inline const CParameterList &parameters() const
	{
		return m_parameters;
	}

	/** @brief Fills the parameter list.
	 *
	 *  @param decl Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline void setParams(
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		m_parameters.set(p, decl, curColorDescr, n, tokens, params);
	}
	
	/** @brief Fills the parameter list.
	 *
	 *  @param theParameters Parsed parameter list.
	 */
	inline void parameters(const CParameterList &theParameters)
	{
		m_parameters = theParameters;
	}

	/** @brief Assignment.
	 *
	 *  @param c CVarParamRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CVarParamRManInterfaceCall &operator=(const CVarParamRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		parameters(c.parameters());
		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // TVarParamInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls for geometry.
 */
class CGeometryRManInterfaceCall : public CVarParamRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CGeometryRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CGeometryRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to a kind of class atomizedClassName
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CGeometryRManInterfaceCall(long aLineNo=-1) : CVarParamRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param decl Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CGeometryRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CVarParamRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param theParameters Parsed parameter list.
	 */
	inline CGeometryRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CGeometryRManInterfaceCall(const CGeometryRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CGeometryRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CGeometryRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CGeometryRManInterfaceCall &CGeometryRManInterfaceCall::operator=(const CGeometryRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CGeometryRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls based on UV Meshes (Splines, Quadrics).
 */
class CUVRManInterfaceCall : public CGeometryRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CUVRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CUVRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CUVRManInterfaceCall(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param decl Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CUVRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CGeometryRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param theParameters Parsed parameter list.
	 */
	inline CUVRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CGeometryRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CUVRManInterfaceCall(const CUVRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CUVRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CUVRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CUVRManInterfaceCall &CUVRManInterfaceCall::operator=(const CUVRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CUVRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls based on Polygon meshes.
 */
class CPolygonRManInterfaceCall : public CGeometryRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CPolygonRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CPolygonRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store..
	 */
	inline CPolygonRManInterfaceCall(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param decl Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CPolygonRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CGeometryRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param theParameters Parsed parameter list.
	 */
	inline CPolygonRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CGeometryRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CPolygonRManInterfaceCall(const CPolygonRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CPolygonRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CPolygonRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CPolygonRManInterfaceCall &CPolygonRManInterfaceCall::operator=(const CPolygonRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CPolygonRManInterfaceCall

}

#endif // _RICPP_RICONTEXT_RIMACROBASE_H
