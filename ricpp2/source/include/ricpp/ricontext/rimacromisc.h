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
 *  @brief The macro classes for error handlers, system and declarations, \see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

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
	IErrorHandler *m_handler; //!< Pointer to the error handler function

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor, default handler is a CPrintErrorHandler.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
	{
		m_handler = new CPrintErrorHandler();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiErrorHandler", __LINE__, __FILE__);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts,
	 *                 -1 if there is no file.
	 *  @param handler Pointer to the error handler function that should be used.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler(
		long aLineNo,
		const IErrorHandler &handler)
		: CRManInterfaceCall(aLineNo)
	{
		m_handler = handler.duplicate();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiErrorHandler", __LINE__, __FILE__);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler(const CRiErrorHandler &c)
	{
		*this = c;
	}

	/** @brief Destructor, frees the resources.
	 */
	inline virtual ~CRiErrorHandler()
	{
		if ( m_handler )
			delete m_handler;
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
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline void handler(const IErrorHandler &handler)
	{
		if ( m_handler )
			delete m_handler;
		m_handler = handler.duplicate();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"CRiErrorHandler::handler()", __LINE__, __FILE__);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cbi)
	{
		ri.preErrorHandler(*m_handler);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doErrorHandler(*m_handler);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postErrorHandler(*m_handler);
	}

	/** @brief Assignment.
	 *
	 *  @param c Object to assign.
	 *  @return A reference to this object.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler &operator=(const CRiErrorHandler &c)
	{
		if ( this == &c )
			return *this;

		handler(c.handler());

		CRManInterfaceCall::operator=(c);
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
	RtToken m_name;            //!< Atomized name of the declarated variable
	std::string m_declaration; //!< The declaration, like: varying float[3]
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @param name The atomized name of the declared variable.
	 *  @param declaration The declaration string.
	 */
	inline CRiDeclare(
		long aLineNo = -1,
		RtToken aName = 0, 
		RtString aDeclaration = 0)
		: CRManInterfaceCall(aLineNo),
		  m_name(aName),
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

	/** @brief Destructor.
	 */
	inline virtual ~CRiDeclare() {}

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
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the token of the name iof the declaration.
	 *
	 *  @param name Token of the name of the declaration.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
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

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cbi)
	{
		ri.preDeclare(name(), declaration());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDeclare(name(), declaration());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDeclare(name(), declaration());
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDeclare


///////////////////////////////////////////////////////////////////////////////
/** @brief System command.
 *
 * A system command issued on the system of the renderer.
 */
class CRiSystem : public CRManInterfaceCall {
private:
	std::string m_cmd; //!< The command
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @param name The atomized name of the declared variable.
	 *  @param declaration The declaration string.
	 */
	inline CRiSystem(
		long aLineNo = -1,
		RtString aCommand = 0)
		: CRManInterfaceCall(aLineNo),
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

	/** @brief Destructor.
	 */
	inline virtual ~CRiSystem() {}

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

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cbi)
	{
		ri.preSystem(command());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSystem(command());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSystem(command());
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

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSystem
}

#endif // _RICPP_RICONTEXT_RIMACROMISC_H
