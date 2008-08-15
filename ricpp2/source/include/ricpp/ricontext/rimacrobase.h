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
#ifndef _RICPP_RICONTEXT_RIBELEMENTS_H
#include "ricpp/ricontext/ribelements.h"
#endif // _RICPP_RICONTEXT_RIBELEMENTS_H

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
		bool m_delayedDeletion; ///< Marker to delay deletion of an interface call until the very end of CBaseRenderer::worldEnd(), CBaseRenderer::processRequest() stores the request
		bool m_inMacro; ///< Marker request is stored in a macro or object, setted by CBaseRanderer::recordRequest()
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
		inline CRManInterfaceCall(long aLineNo = -1): m_lineNo(aLineNo), m_delayedDeletion(false), m_inMacro(false) {}

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
		// inline
		virtual CRManInterfaceCall *duplicate() const = 0;
		// {
		//	return new CRManInterfaceCall(*this);
		// }

		/** @brief Destructor.
		 */
		inline virtual ~CRManInterfaceCall() {}

		/** @brief Gets the interface number of the corresponding RI request.
		 *
		 *  @return Interface number of the corresponding RI request.
		 */
		// inline 
		virtual EnumRequests CRManInterfaceCall::interfaceIdx() const = 0;
		// {
		//	return REQ_UNKNOWN;
		// }

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
		virtual void replay(IDoRender &ri, const IArchiveCallback *cb);

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
			delayedDeletion(c.delayedDeletion());
			inMacro(c.inMacro());
			
			return *this;
		}

		/** @brief Processes an interface call
		 *
		 *  Can be used to process an interface call of a renderer, like
		 *  calling the interface function directly. Currently not
		 *  used.
		 *
		 *  @param ri The renderer backend or frontend used for processing.
		 */
		inline virtual void process(IRiRoot &ri)
		{
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

		/** @brief Write the RIB Code of the request.
		 *
		 *  @param ribWriter Elementary stream object for RIB writing.
		 *  @param n Number of tokens to ignore ( sizeof(ignoreTokens) )
		 *  @param ignoreTokens Tokens to ignore if a parameterlist is printed (namely RI_HANDLEID)
		 */
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putNewLine();
		}

		/*  @brief Query if bounds are available.
		 *  @return true, if bounds are available
		 *  @see getBounds()
		 */
		// inline virtual bool boundable() const { return false; }

		/*  @brief Gets the bounds, if available.
		 *  @todo Implement bounds.
		 *  @retval bounds The bounderies of the object representated.
		 *  @see boundable()
		 */
		// inline virtual void getBounds(RtBound bounds) const {}
		// inline virtual void setBounds(const RtBound bounds) {}
		
		inline virtual bool delayedDeletion() const { return m_delayedDeletion; }
		inline virtual void delayedDeletion(bool flag) { m_delayedDeletion = flag; }

		inline virtual bool inMacro() const { return m_inMacro; }
		inline virtual void inMacro(bool flag) { m_inMacro = flag; }
	}; // CRManInterfaceCall


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Base class of all interface calls with parameter lists.
	 */
	class CVarParamRManInterfaceCall : public CRManInterfaceCall, IParameterClasses {
	private:
		CParameterList m_parameters;             ///< Parameters of an interface call.
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Constructor.
		 *
		 *  Constructs an empty parameter list, with line number, defaults to -1.
		 *
		 *  @param aLineNo The line number to store.
		 */
		inline CVarParamRManInterfaceCall(long aLineNo=-1) :
			TypeParent(aLineNo)
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
			TypeParent(aLineNo)
		{
			setParams(0, 0, decl, p, curColorDescr, n, tokens, params);
		}

		/** @brief Constructor.
		 *
		 *  @param aQualifier The qualifier
		 *  @param table The table
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
			const char *aQualifier, const char *table,
			CDeclarationDictionary &decl,
			const CParameterClasses &p,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]) :
			TypeParent(aLineNo)
		{
			setParams(aQualifier, table, decl, p, curColorDescr, n, tokens, params);
		}

		/** @brief Constructor.
		 *
		 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
		 * @param theParameters Parsed parameter list.
		 */
		inline CVarParamRManInterfaceCall(
			long aLineNo,
			const CParameterList &theParameters) :
			TypeParent(aLineNo)
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

		/*  @brief Duplication.
		 * 
		 *  @return New instance as clone of this instance.
		 *//*
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CVarParamRManInterfaceCall(*this);
		} */

		/** @brief Gets the size of tokens and parameters of the request.
		 *
		 * @return The size of tokens and parameters of the request.
		 */
		inline RtInt paramSize() const
		{
			return (RtInt)m_parameters.size();
		}

		/** @brief Gets the tokens of the request.
		 *
		 * For the size of the array @see size(), for the parameter pointers @see getParams().
		 *
		 * @return The tokens of the request.
		 */
		inline RtToken *tokenPtr()
		{
			return m_parameters.tokenPtr();
		}

		/** @brief Gets the parameter pointers of the request.
		 *
		 * For the size of the array @see size(), for the tokenss @see getTokens().
		 *
		 * @return The parameters of the request.
		 */
		inline RtPointer *valuePtr()
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
		 *  @param aQualifier The qualifier
		 *  @param table The table
		 *  @param decl Dictonary with the current declarations.
		 *  @param p Counters (vertices, corners etc.) of the request.
		 *  @param curColorDescr Current color descriptor.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline void setParams(
			const char *aQualifier, const char *table,
			CDeclarationDictionary &decl,
			const CParameterClasses &p,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			m_parameters.set(aQualifier, table, p, decl, curColorDescr, n, tokens, params);
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
			TypeParent::operator=(c);
			return *this;
		}

		/** @brief Write the RIB Code of the request.
		 *
		 *  @param ribWriter Elementary stream object for RIB writing.
		 *  @param n Number of tokens to ignore ( sizeof(ignoreTokens) )
		 *  @param ignoreTokens Tokens to ignore if a parameterlist is printed (namely RI_HANDLEID)
		 */
		virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const;

		/** @brief Gets the size for constants (normally 1).
		 * @return The size for constants.
		 */
		inline virtual RtInt constants() const { return parameters().parameterClasses().constants(); }
		
		/** @brief Gets the number of facets.
		 * @return The number of facets.
		 */
		inline virtual RtInt facets() const { return parameters().parameterClasses().facets(); }
		
		/** @brief Gets the number of corners.
		 * @return The number of corners.
		 */
		inline virtual RtInt corners() const { return parameters().parameterClasses().corners(); }
		
		/** @brief Gets the number of vertices.
		 * @return The number of vertices.
		 */
		inline virtual RtInt vertices() const { return parameters().parameterClasses().vertices(); }
		
		/** @brief Gets the number of face corners.
		 * @return The number of face corners.
		 */
		inline virtual RtInt faceCorners() const { return parameters().parameterClasses().faceCorners(); }
		
		/** @brief Gets the number of face vertices.
		 * @return The number of face vertices.
		 */
		inline virtual RtInt faceVertices() const  { return parameters().parameterClasses().faceVertices(); }
		
		/*  @brief Gets the bounds, by bounding parameter RI_P, RI_PW
		 *
		 *  The boundable() must be implemented by the appropriate class.
		 *
		 *  @retval bounds The bounderies of the object representated.
		 *  @see boundable()
		 */
		// virtual void getBounds(RtBound bounds) const;
	}; // CVarParamRManInterfaceCall
}

#endif // _RICPP_RICONTEXT_RIMACROBASE_H
