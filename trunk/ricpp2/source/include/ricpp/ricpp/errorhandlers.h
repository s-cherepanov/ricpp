#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#define _RICPP_RICPP_ERRORHANDLERS_H

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

/** @file errorhandlers.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard error handlers of the interface.
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** @brief The abort-error handler, prints an error at standard output and exit() if the error is severe
 */
class CAbortErrorHandler  : public IErrorHandler  {
public:
	static const CAbortErrorHandler &func();
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual IErrorHandler *duplicate() const { return new CAbortErrorHandler(*this); }

	/**@brief The name of the handler.
	 * @return The name of the renderer as used in RIB
	 */
	inline virtual const char *name() const {return myName();}

	/** @brief Handles the error (print, abort if severe)
	 * @param ri Front end that detected the error
	 * @param code Error code (RIE_...)
	 * @param severity Error severity level (RIE_INFO ... RIE_SEVERE)
	 * @param message Error message, describing the error
	 */
	virtual RtVoid operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const;

	inline virtual const IErrorHandler &singleton()  const { return func(); }
};

/** @brief The print-error handler, prints an error at standard output
 */
class CPrintErrorHandler : public IErrorHandler  {
public:
	static const CPrintErrorHandler &func();
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual IErrorHandler *duplicate() const { return new CPrintErrorHandler(*this); }

	/**@brief The name of the handler.
	 * @return The name of the renderer as used in RIB
	 */
	inline virtual const char *name() const {return myName();}

	/** @brief Handles the error (print)
	 * @param ri Front end that detected the error
	 * @param code Error code (RIE_...)
	 * @param severity Error severity level (RIE_INFO ... RIE_SEVERE)
	 * @param message Error message, describing the error
	 */
	virtual RtVoid operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const;

	inline virtual const IErrorHandler &singleton()  const { return func(); }
};

/** @brief The ignore-error handler, does nothing, ignores the error
 */
class CIgnoreErrorHandler : public IErrorHandler  {
public:
	static const CIgnoreErrorHandler &func();
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual IErrorHandler *duplicate() const { return new CIgnoreErrorHandler(*this); }

	/**@brief The name of the handler.
	 * @return The name of the renderer as used in RIB
	 */
	inline virtual const char *name() const {return myName();}

	/** @brief Handles the error (ignore)
	 * @param ri Front end that detected the error
	 * @param code Error code (RIE_...)
	 * @param severity Error severity level (RIE_INFO ... RIE_SEVERE)
	 * @param message Error message, describing the error
	 */
	virtual RtVoid operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const;

	inline virtual const IErrorHandler &singleton()  const { return func(); }
};

/** @brief Factory for error handler functions.
 */
class CErrorHandlerFactory {
public:
	inline virtual ~CErrorHandlerFactory() {}

	virtual const IErrorHandler *newFunc(RtToken name);
	virtual void deleteFunc(IErrorHandler *f);

	virtual const IErrorHandler *singleton(RtToken name) const;
};

} // namespace RiCPP

#endif // _RICPP_RICPP_ERRORHANDLERS_H
