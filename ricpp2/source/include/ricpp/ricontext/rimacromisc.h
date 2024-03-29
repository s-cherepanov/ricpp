#ifndef _RICPP_RICONTEXT_RIMACROMISC_H
#define _RICPP_RICONTEXT_RIMACROMISC_H

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

/** @file rimacromisc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for error handlers, system and declarations, @see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

#include <cassert>

namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Misc
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	// Not suported: Pure frontend functions
	// REQ_BEGIN
	// REQ_END
	// REQ_CONTEXT
	// REQ_GET_CONTEXT

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Stores the content of a IRi::errorHandler() call.
	 *
	 *  REQ_ERROR_HANDLER - is a frontend funktion but has a class CRiErrorHandler
	 *  because Errorhandler is also a RIB request. Stores the setting of an error handler.
	 */
	class CRiErrorHandler : public CRManInterfaceCall {
	private:
		const IErrorHandler *m_handler; ///< Pointer to the error handler function
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string).
		 */
		inline static const char *myClassName(void) { return "CRiErrorHandler"; }

		inline virtual const char *className() const { return CRiErrorHandler::myClassName(); }

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

		/** @brief Default Constructor, default handler is a CPrintErrorHandler.
		 *
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 *  @exception ExceptRiCPPError Could not construct a handler.
		 */
		inline CRiErrorHandler(
			long aLineNo = -1)
			: TypeParent(aLineNo)
		{
			m_handler = &CPrintErrorHandler::func();
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number at a rib file where the declare statement starts,
		 *                 -1 if there is no file.
		 *  @param handler Pointer to the error handler function that should be used.
		 *  @exception ExceptRiCPPError Could not construct a handler.
		 */
		inline CRiErrorHandler(
			long aLineNo,
			const IErrorHandler &handler)
			: TypeParent(aLineNo)
		{
			m_handler = &(handler.singleton());
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 *  @exception ExceptRiCPPError Could not construct a handler.
		 */
		inline CRiErrorHandler(const CRiErrorHandler &c)
		{
			*this = c;
		}

		/** @brief Cloning.
		 *
		 *  @return a clone of this object.
		 */
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiErrorHandler(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ERROR_HANDLER; }

		/** @brief Gets the handler.
		 *
		 *  @return The currently assigned handler.
		 */
		inline const IErrorHandler &handler() const
		{
			assert(m_handler != 0);
			return *m_handler;
		}

		/** @brief Assigns a new handler.
		 *
		 *  Old handler is destroyed. If the new handler cannot be constructed, an
		 *  ExceptRiCPPError is thrown.
		 *
		 *  @param handler A clone of the handler will be created and assigned.
		 *  @exception ExceptRiCPPError Could not construct a handler.
		 */
		inline void handler(const IErrorHandler &handler)
		{
			m_handler = &handler.singleton();
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.errorHandler(handler());
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
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(handler().name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}	

		/** @brief Assignment.
		 *
		 *  @param c Object to assign.
		 *  @return A reference to this object.
		 *  @exception ExceptRiCPPError Could not construct a handler.
		 */
		inline CRiErrorHandler &operator=(const CRiErrorHandler &c)
		{
			if ( this == &c )
				return *this;

			handler(c.handler());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiErrorHandler


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Stores the content of a IRi::declare() call.
	 *
	 * A declaration of an interface variable is stored in an instance of this class.
	 * The declaration is only stored here, not checked.
	 */
	class CRiDeclare : public CRManInterfaceCall {
	private:
		std::string m_name;        ///< Name of the declarated variable (not the token)
		std::string m_declaration; ///< The declaration, like: varying float[3]
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string).
		 */
		inline static const char *myClassName(void) { return "CRiDeclare"; }

		inline virtual const char *className() const { return CRiDeclare::myClassName(); }

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
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 *  @param aName The atomized name of the declared variable.
		 *  @param aDeclaration The declaration string.
		 */
		inline CRiDeclare(
			long aLineNo = -1,
			RtString aName = 0, 
			RtString aDeclaration = 0)
			: TypeParent(aLineNo),
			  m_name(noNullStr(aName)),
			  m_declaration(noNullStr(aDeclaration))
		{}
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiDeclare(const CRiDeclare &c)
		{
			*this = c;
		}

		/** @brief Cloning.
		 *
		 *  @return a clone of this object
		 */
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiDeclare(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_DECLARE; }

		/** @brief Gets the token of the name iof the declaration.
		 *
		 *  @return Token of the name of the declaration.
		 */
		inline RtString name() const
		{
			return m_name.c_str();
		}

		/** @brief Sets the token of the name iof the declaration.
		 *
		 *  @param aName The name of the declaration (not the token).
		 */
		inline void name(RtString aName)
		{
			m_name = noNullStr(aName);
		}

		/** @brief Gets the declaration string.
		 *
		 *  @return The declaration string.
		 */
		inline RtString declaration() const
		{
			return m_declaration.c_str();
		}

		/** @brief Sets (does not check) the declaration string.
		 *
		 *  @param aDeclaration The declaration string.
		 */
		inline void declaration(RtString aDeclaration)
		{
			m_declaration = noNullStr(aDeclaration);
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.declare(name(), declaration());
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
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			ribWriter.putBlank();
			ribWriter.putString(declaration());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 * @param c Object to assign.
		 * @return A reference to this object.
		 */
		inline CRiDeclare &operator=(const CRiDeclare &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());
			declaration(c.declaration());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDeclare


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Synchronize command.
	 */
	class CRiSynchronize : public CRManInterfaceCall {
	private:
		RtToken m_name; ///< The synchronization command
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string).
		 */
		inline static const char *myClassName(void) { return "CRiSynchronize"; }

		inline virtual const char *className() const { return CRiSynchronize::myClassName(); }

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
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 *  @param aName Synchronization
		 */
		inline CRiSynchronize(
			long aLineNo = -1,
			RtToken aName = 0)
			: TypeParent(aLineNo),
			  m_name(aName)
		{}
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiSynchronize(const CRiSynchronize &c)
		{
			*this = c;
		}

		/** @brief Cloning.
		 *
		 *  @return a clone of this object
		 */
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiSynchronize(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SYNCHRONIZE; }

		/** @brief Gets the synchronization token.
		 *
		 *  @return The synchronization token.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the synchronization token.
		 *
		 *  @param aCommand The synchronization token.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.synchronize(name());
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
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 * @param c Object to assign.
		 * @return A reference to this object.
		 */
		inline CRiSynchronize &operator=(const CRiSynchronize &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSynchronize


	///////////////////////////////////////////////////////////////////////////////
	/** @brief System command.
	 *
	 * A system command issued on the system of the renderer.
	 */
	class CRiSystem : public CRManInterfaceCall {
	private:
		std::string m_cmd; ///< The command
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string).
		 */
		inline static const char *myClassName(void) { return "CRiSystem"; }

		inline virtual const char *className() const { return CRiSystem::myClassName(); }

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
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 *  @param aCommand System command.
		 */
		inline CRiSystem(
			long aLineNo = -1,
			RtString aCommand = 0)
			: TypeParent(aLineNo),
			  m_cmd(noNullStr(aCommand))
		{}
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiSystem(const CRiSystem &c)
		{
			*this = c;
		}

		/** @brief Cloning.
		 *
		 *  @return a clone of this object
		 */
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiSystem(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_SYSTEM; }

		/** @brief Gets the command string.
		 *
		 *  @return The command string.
		 */
		inline RtString command() const
		{
			return m_cmd.c_str();
		}

		/** @brief Sets the command string..
		 *
		 *  @param aCommand The command string.
		 */
		inline void command(RtString aCommand)
		{
			m_cmd = noNullStr(aCommand);
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.system(command());
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
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(command());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 * @param c Object to assign.
		 * @return A reference to this object.
		 */
		inline CRiSystem &operator=(const CRiSystem &c)
		{
			if ( this == &c )
				return *this;

			command(c.command());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSystem


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Backend/Frontend control.
	 *
	 *  Not a Rib command and not used in macros.
	 */
	class CRiControl : public CVarParamRManInterfaceCall {
	private:
		RtToken m_name; ///< Name of the control request as atomized string.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiControl"; }

		inline virtual const char *className() const { return CRiControl::myClassName(); }

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
		 *  @param aName Name of the control request as atomized string.
		 */
		inline CRiControl(
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
		 *  @param aName Name of the control request as atomized string.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiControl(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_CONTROL, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(aName)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the control request as atomized string.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiControl(
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
		inline CRiControl(const CRiControl &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiControl(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_CONTROL; }

		/** @brief Gets the name of the control request as atomized string.
		 *
		 *  @return The name of the control request as atomized string.
		 */
		inline RtToken name() const
		{
			return m_name;
		}

		/** @brief Sets the name of the control request as atomized string.
		 *
		 *  @param aName The name of the control request as atomized string.
		 */
		inline void name(RtToken aName)
		{
			m_name = aName;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.controlV(name(), paramSize(), tokenPtr(), valuePtr());
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
			// Normally not called because Control is not a RIB control
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
		inline CRiControl &operator=(const CRiControl &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiControl


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Resource handling
	 */
	class CRiResource : public CVarParamRManInterfaceCall {
	private:
		RtToken m_handle; ///< Handle id of a resource
		RtToken m_type;   ///< Type of a resource
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiResource"; }

		inline virtual const char *className() const { return CRiResource::myClassName(); }


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

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aHandle Atomized handle (id) of the resource
		 *  @param aType Atomized type of the resource
		 */
		inline CRiResource(
			long aLineNo = -1,
			RtToken aHandle = 0,
			RtToken aType = 0
			)
			: TypeParent(aLineNo), m_handle(aHandle), m_type(aType)
		{
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store.
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aHandle Atomized handle (id) of the resource
		 *  @param aType Atomized type of the resource
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiResource(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtToken aHandle,
			RtToken aType,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_RESOURCE, aType, decl, CParameterClasses(), curColorDescr, n, tokens, params),
			  m_handle(aHandle), m_type(aType)
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aHandle Atomized handle (id) of the resource
		 *  @param aType Atomized type of the resource
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiResource(
			long aLineNo,
			RtToken aHandle,
			RtToken aType,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_handle(aHandle), m_type(aType)
		{
		}


		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiResource(const CRiResource &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiResource(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE; }

		/** @brief Gets the handle of the resource.
		 *
		 *  @return The atomized handle of the resource.
		 */
		inline RtString handle() const
		{
			return m_handle;
		}

		/** @brief Sets the handle of the resource.
		 *
		 *  @param aHandle An atomized handle of a resource.
		 */
		inline void handle(RtToken aHandle)
		{
			m_handle = aHandle;
		}

		/** @brief Gets the type of the resource.
		 *
		 *  @return The atomized type of the resource.
		 */
		inline RtString type() const
		{
			return m_type;
		}

		/** @brief Sets the type of the resource.
		 *
		 *  @param aType An atomized type of a resource.
		 */
		inline void type(RtToken aType)
		{
			m_type = aType;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.resourceV(handle(), type(), paramSize(), tokenPtr(), valuePtr());
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
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(handle());
			ribWriter.putBlank();
			ribWriter.putString(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiResource &operator=(const CRiResource &c)
		{
			if ( this == &c )
				return *this;

			handle(c.handle());
			type(c.type());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiResource

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Archive record (comment)
	 */
	class CRiArchiveRecord : public CRManInterfaceCall {
	private:
		RtToken     m_type; ///< Type of the archive record.
		std::string m_line; ///< Text content of the archive record.
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  Empty object
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiArchiveRecord(long aLineNo=-1)
			: TypeParent(aLineNo)
		{
			m_type = 0;
		}

		inline CRiArchiveRecord(long aLineNo, RtToken aType, RtString aLine)
			: TypeParent(aLineNo), m_type(aType), m_line(noNullStr(aLine))
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
		 *  @return The text content of the archive record.
		 */
		RtString line() const
		{
			return m_line.c_str(); 
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.archiveRecordV(type(), line());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			/** @todo IArchiveCallback cb is used only for CRiArchiveRecord and
			 *        has to do with CRIReadArchive, try to get rid of this
			 *        parameter in pre... do... post... functions.
			 */
			ri.doProcess(*this);
			if ( cb && ri.frontend() ) 
				(*cb)(*ri.frontend(), m_type, m_line.c_str());
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putComment(type(), line());
			// No Line End (already done by putComment).
			// Therefore, no of call of TypeParent::writeRIB()
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiArchiveRecord

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Read a RIB archive
	 */
	class CRiReadArchive : public CVarParamRManInterfaceCall {
	private:
		std::string       m_filename; ///< Filename of the RIB archive.
		const IArchiveCallback *m_callback; ///< Optional callbback for archive records.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
	public:
		/** @brief Gets the name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiReadArchive"; }
		inline virtual const char *className() const { return CRiReadArchive::myClassName(); }

		/** @brief Default Constructor.
		 *
		 *  Empty object
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiReadArchive(long aLineNo=-1)
			: TypeParent(aLineNo)
		{
			m_callback = 0;
		}

		inline CRiReadArchive(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtString aFilename, const IArchiveCallback *cb,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_filename(noNullStr(aFilename))
		{
			m_callback = 0;
			if ( cb )
				m_callback = &(cb->singleton());
		}

		inline CRiReadArchive(
			long aLineNo,
			RtString aFilename, const IArchiveCallback *cb,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_filename(noNullStr(aFilename))
		{
			m_callback = 0;
			if ( cb )
				m_callback = &(cb->singleton());
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiReadArchive(const CRiReadArchive &c)
		{
			m_callback = 0;
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiReadArchive(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
		
		inline const IArchiveCallback *callback() const
		{
			return m_callback;
		}

		inline void callback(const IArchiveCallback *cb)
		{
			m_callback = 0;
			if ( cb )
				m_callback = &(cb->singleton());
		}

		inline RtString name() const
		{
			return m_filename.c_str();
		}

		inline void name(RtString aFilename)
		{
			m_filename = noNullStr(aFilename);
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.readArchiveV(name(), callback(), paramSize(), tokenPtr(), valuePtr());
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
		
		/*
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
		*/

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		inline CRiReadArchive &operator=(const CRiReadArchive &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());
			callback(c.callback());
			
			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiReadArchive


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Instanciate an object.
	 */
	class CRiObjectInstance : public CRManInterfaceCall {
	private:
		RtObjectHandle m_handle; ///< Object handle
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiObjectInstance"; }

		inline virtual const char *className() const { return CRiObjectInstance::myClassName(); }

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

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aHandle Handle to identify the object
		 */
		inline CRiObjectInstance(
			long aLineNo=-1,
			RtObjectHandle aHandle=illObjectHandle)
			: TypeParent(aLineNo), m_handle(aHandle)
		{
		}


		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiObjectInstance(const CRiObjectInstance &c)
		{
			*this = c;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiObjectInstance(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_INSTANCE; }

		/** @brief Gets the object handle.
		 */
		inline virtual RtObjectHandle handle() const { return m_handle; }

		/** @brief Sets the object handle.
		 *
		 *  @param h An object handle to identify the object.
		 */
		inline virtual void handle(RtObjectHandle h) { m_handle = h; }

		inline virtual void process(IRiRoot &ri)
		{
			ri.objectInstance(handle());
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
			ribWriter.putStringToken(handle());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, const char *aName, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(aName);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, unsigned long handleNo, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(handleNo);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiObjectInstance &operator=(const CRiObjectInstance &c)
		{
			if ( this == &c )
				return *this;

			handle(c.handle());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiObjectInstance

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Procedurals.
	 *  @todo Needs some work.
	 */
	class CRiProcedural : public CRManInterfaceCall {
	private:
		RtBound m_bound;
		ISubdivFunc *m_subdivfunc;
		IFreeFunc *m_freefunc;
		ISubdivData *m_data;

		void insertData(RtPointer data);
		void insertData(const ISubdivData *data);
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiProcedural"; }
		inline virtual const char *className() const { return CRiProcedural::myClassName(); }

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
		 *  Empty object.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiProcedural(long aLineNo = -1)
			: TypeParent(aLineNo)
		{
			memset(m_bound, 0, sizeof(RtBound));
			m_subdivfunc = 0;
			m_freefunc = 0;
			m_data = 0;
		}

		CRiProcedural(long aLineNo,
			RtPointer aData, const RtBound aBound,
			ISubdivFunc &sSubdivFunc, IFreeFunc *aFreeFunc);

		const RtBound &bound() const { return m_bound; };
		RtBound &bound() { return m_bound; };
		
		const ISubdivFunc *subdivFunc() const { return m_subdivfunc; }
		ISubdivFunc *subdivFunc() { return m_subdivfunc; }

		const IFreeFunc *freeFunc() const { return m_freefunc; };
		IFreeFunc *freeFunc() { return m_freefunc; };

		const ISubdivData *subdivData() const { return m_data; }
		ISubdivData *subdivData() { return m_data; }

		const RtPointer data() const { return m_data ? m_data->data() : 0; }
		RtPointer data() { return m_data ? m_data->data() : 0; }
		
		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiProcedural(const CRiProcedural &c)
		{
			m_subdivfunc = 0;
			m_freefunc = 0;
			m_data = 0;
			*this = c;
		}

		/** @brief Destructor, frees resources.
		 */
		inline virtual ~CRiProcedural()
		{
			if ( m_data )
				delete m_data;
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiProcedural(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_PROCEDURAL; }

		inline virtual void process(IRiRoot &ri)
		{
			assert(subdivFunc());
			if ( !subdivFunc() ) {
				return;
			}
			ri.procedural(data(), bound(), *subdivFunc(), freeFunc());
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
			if ( !m_subdivfunc )
				return;
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(m_subdivfunc->name());
			ribWriter.putBlank();
			if ( m_data )
				ribWriter.putArray(m_subdivfunc->numArgs(), (const RtString *)m_data->data());
			else
				ribWriter.putArray();
			ribWriter.putBlank();
			ribWriter.putArray(m_bound);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		CRiProcedural &operator=(const CRiProcedural &c);
	}; // CRiProcedural

}
#endif // _RICPP_RICONTEXT_RIMACROMISC_H
