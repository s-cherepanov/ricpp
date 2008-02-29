#ifndef _RICPP_RI_RIINTERNAL_H
#define _RICPP_RI_RIINTERNAL_H

/* RICPP - RenderMan(R) Interface CPP Language Binding
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
*/

/** @file riinternal.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Helpers for the C-binding, for internal use only
 */

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/ricpp/errorhandlers.h"
#endif

#include <map>
#include <cstdarg>

namespace RiCPP {

 /** @brief Gets pointer to the current renderer frontend or backend
  *
  * Eventually will be IRi frontend later. The RiCPPInternal...() function
  * can then be removed. Will then be named IRi::RiRoot(). However,
  * I'm not shure whether the backend can be of use here or not.
  *
  * @return Current renderer interface. Returns 0 if no renderer is set.
  */
IRiRoot *RiCPPRoot();

 /** @brief Sets the current renderer frontend or backend
  *
  * @param aRoot Current renderer interface.
  */
void RiCPPRoot(IRiRoot *aRoot);

 /** @brief Sets the current renderer frontend or backend
  *
  * @param marker Marker of the RI_NULL terminated variable argument list.
  * @retval tokens C-Array of tokens
  * @retval params C-Array of parameter pointers
  * @return The number of token - parameter pairs found.
  */
RtInt RiCPPGetArgs(va_list marker, RtToken **tokens, RtPointer **params);

/** @brief Calls the IRi::beginV() routine
 *
 *  riprog and ridynload implement the normal RiBegin() and additional
 *  RiCPPBegin(), RiCPPBeginV() and call RiCPPInternalBeginV().
 *  ridynload ignores begin/end calls.
 *
 *  @param name Name of the renderer or RIB file
 *  @param n number of token-parameter pairs
 *  @param tokens Tokens
 *  @param params Values
 */
RtContextHandle RiCPPInternalBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]);

/** @brief Calls the IRi::end() routine
 *
 *  riprog and ridynload implement the normal RiEnd() and additional RiCPPEnd()
 *  and call RiCPPInternalBeginV(). ridynload ignores begin/end calls.
 */
RtVoid RiCPPInternalEnd(void);

/** @brief Calls the IRi::errorHandler() routine
 *
 *  riprog and ridynload implement the normal RiErrorHandler() and call RiCPPInternalErrorHandler()
 *  @param handler the C-binding ErrorHandler (dummy pointer)
 */
RtVoid RiCPPInternalErrorHandler(RtErrorHandler handler);

/** @brief Calls the IRi::errorHandler() routine
 *
 *  riprog and ridynload implement the normal RiControl(), RiControlV() and call RiCPPInternalControlV()
 *  @param handler the C-binding ErrorHandler (dummy pointer)
 */
RtVoid RiCPPInternalControlV(RtToken name, int n, RtToken tokens[], RtPointer params[]);

// ----------------------------------------------------------------------------

/** @brief An arbitrary pixel filter for C binding
 */
class CFilterSlot : public IFilterFunc {
	static std::map<RtFilterFunc, CFilterSlot> s_filters; ///< Maps filterfunctions to their objects
	RtFilterFunc m_filter; ///< Pointer to the filter function
public:
	inline CFilterSlot(RtFilterFunc aFilter=0) { m_filter = aFilter; }
	inline CFilterSlot(const CFilterSlot &o) { *this = o; }
	inline ~CFilterSlot() { }

	inline virtual IFilterFunc *duplicate() const { return new CFilterSlot(*this); }

	inline virtual const IFilterFunc &singleton() const { return *this; }
	inline static const IFilterFunc &getSingleton(RtFilterFunc function)
	{
		if ( function == RiGaussianFilter )
			return CGaussianFilter::func;
		else if ( function == RiBoxFilter )
			return CBoxFilter::func;
		else if ( function == RiTriangleFilter )
			return CCatmullRomFilter::func;
		else if ( function == RiSincFilter )
			return CSincFilter::func;

		s_filters[function] = CFilterSlot(function);
		return s_filters[function];
	}

	inline static RtToken myName() { return "CFilterSlot"; }
	inline virtual RtToken name() const { return myName(); }

	inline virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
	{
		if ( m_filter )
			return (*m_filter)(x, y, xwidth, ywidth);
		// Error
		return 0;
	}

	inline RtFilterFunc filterPtr() const { return m_filter; }
	inline void filterPtr(RtFilterFunc aFilter) { m_filter = aFilter; }
	inline CFilterSlot &operator=(const CFilterSlot &o)
	{
		if ( this == &o )
			return *this;
		filterPtr(o.filterPtr());
		return *this;
	}
};

// ----------------------------------------------------------------------------

/** @brief An arbitrary error handler for C binding
 */
class CErrorHandlerSlot : public IErrorHandler {
	static std::map<RtErrorHandler, CErrorHandlerSlot> s_errorHandlers; ///< Maps filterfunctions to their objects
	RtErrorHandler m_errorHandler; ///< Pointer to the filter function (dummy pointer for the standard handlers)
public:
	inline CErrorHandlerSlot(RtErrorHandler aHandler=0) { m_errorHandler = aHandler; }
	inline CErrorHandlerSlot(const CErrorHandlerSlot &o) { *this = o; }
	inline ~CErrorHandlerSlot() { }

	inline virtual IErrorHandler *duplicate() const { return new CErrorHandlerSlot(*this); }

	inline virtual const IErrorHandler &singleton() const { return *this; }
	inline static const IErrorHandler &getSingleton(RtErrorHandler function)
	{
		if ( function == RiErrorAbort )
			return CAbortErrorHandler::func;
		else if ( function == RiErrorPrint )
			return CPrintErrorHandler::func;
		else if ( function == RiErrorIgnore )
			return CIgnoreErrorHandler::func;

		// Use the C function if not a standard handler
		s_errorHandlers[function] = CErrorHandlerSlot(function);
		return s_errorHandlers[function];
	}

	inline static RtToken myName() { return "CErrorHandlerSlot"; }
	inline virtual RtToken name() const { return myName(); }

	inline RtErrorHandler errorHandlerPtr() const { return m_errorHandler; }
	inline void errorHandlerPtr(RtErrorHandler aHandler) { m_errorHandler = aHandler; }
	inline CErrorHandlerSlot &operator=(const CErrorHandlerSlot &o)
	{
		if ( this == &o )
			return *this;
		errorHandlerPtr(o.errorHandlerPtr());
		return *this;
	}

	inline virtual RtVoid operator()(IRi &ri, RtInt code, RtInt severity, RtString msg) const
	{
		if ( m_errorHandler )
			return (*m_errorHandler)(code, severity, msg);
		// Error
	}
};

// ----------------------------------------------------------------------------

/** @brief An arbitrary archive callback for C binding
 */
class CArchiveCallbackSlot : public IArchiveCallback {
	static std::map<RtArchiveCallback, CArchiveCallbackSlot> s_callbacks; ///< Maps filterfunctions to their objects
	RtArchiveCallback m_callback; ///< Pointer to the archive callback
public:
	inline CArchiveCallbackSlot(RtArchiveCallback aCallback=0) : m_callback(aCallback) { }
	inline CArchiveCallbackSlot(const CArchiveCallbackSlot &o) { *this = o; }
	inline virtual ~CArchiveCallbackSlot() { }

	inline virtual IArchiveCallback *duplicate() const { return new CArchiveCallbackSlot(*this); }

	inline virtual const IArchiveCallback &singleton() const { return *this; }
	inline static const IArchiveCallback &getSingleton(RtArchiveCallback function)
	{
		s_callbacks[function] = CArchiveCallbackSlot(function);
		return s_callbacks[function];
	}

	inline static RtToken myName() {return "CArchiveCallbackSlot"; }
	inline virtual RtToken name() const { return myName(); }

	inline virtual RtVoid operator()(IRiRoot &ri, RtToken type, RtString line) const
	{
		if ( m_callback ) {
			(*m_callback)(type, "%s", line);
			return;
		}
		// Error
	}

	inline RtArchiveCallback callbackPtr() const { return m_callback; }
	inline void callbackPtr(RtArchiveCallback aCallback) { m_callback = aCallback; }

	inline CArchiveCallbackSlot &operator=(const CArchiveCallbackSlot &o)
	{
		if ( this == &o )
			return *this;
		callbackPtr(o.callbackPtr());
		return *this;
	}
}; // CArchiveCallbackSlot

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

/** @brief Preamble of the Ri interface routines
 *  
 *  Tests whether a renderer is defined.
 */
#define PREAMBLE { \
	if ( RiCPP::RiCPPRoot() == 0 ) { \
		return; \
	} \
}

/** @brief Postamble of the Ri interface routines
 *  
 *  Sets the error number.
 */
#define POSTAMBLE \
	RiLastError = RiCPP::RiCPPRoot()->lastError();

/** @brief Preamble with return outlet of the Ri interface routines
 *  
 *  Like PREAMBLE, tests whether a renderer is defined.
 *  Can return a value @a RETVAL if test fails.
 */
#define PREAMBLE_RET(RETVAL) { \
	if ( RiCPP::RiCPPRoot() == 0 ) { \
		return (RETVAL); \
	} \
}

/** @brief Postamble with return outlet  of the Ri interface routines
 *  
 *  Like POSTAMBLE, sets the error number.
 *  Returns a value @a RETVAL
 */
#define POSTAMBLE_RET(RETVAL) \
	RiLastError = RiCPP::RiCPPRoot()->lastError(); \
	return (RETVAL);

/** @brief Shortcut macro to call RiCPP::RiCPPGetArgs()
 *  
 *  Defines n, tokens, params, calls RiCPP::RiCPPGetArg() to
 *  copy the variable, RI_NULL terminated arguments. n, tokens, params can then
 *  be used to call the appropriate Ri...V() routine.
 */
#define GETARGS(VAR) \
	RtToken *tokens = 0; \
	RtPointer *params = 0; \
	RtInt n = 0; \
	{ \
		va_list args; \
		va_start(args, VAR); \
		n = RiCPP::RiCPPGetArgs(args, &tokens, &params); \
	}

} /* namespace RiCPP */

#endif // _RICPP_RI_RIINTERNAL_H
