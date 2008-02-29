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
 *  @brief Helpers for C-binding, for internal use only
 */

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/ricpp/errorhandlers.h"
#endif

#include <map>
#include <cstdarg>

namespace RiCPP { /* */

IRiRoot *RiCPPRoot();
void RiCPPRoot(IRiRoot *aRoot);
RtInt RiCPPGetArgs(va_list marker, RtToken **tokens, RtPointer **params);

RtContextHandle RiCPPInternalBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]);
RtVoid RiCPPInternalEnd(void);

RtVoid RiCPPInternalErrorHandler(RtErrorHandler handler);
RtVoid RiCPPInternalControlV(RtToken name, int n, RtToken tokens[], RtPointer params[]);

// ----------------------------------------------------------------------------

/** @brief An arbitrary pixel filter for C binding
 */
class CFilterSlot : public IFilterFunc {
	friend std::map<RtFilterFunc, CFilterSlot>;

	static std::map<RtFilterFunc, CFilterSlot> s_filters; ///< Maps filterfunctions to their objects

	RtFilterFunc m_filter;

	inline virtual IFilterFunc *duplicate() const { return new CFilterSlot(*this); }

	inline CFilterSlot(RtFilterFunc aFilter=0) { m_filter = aFilter; }

	inline RtFilterFunc filterPtr() const { return m_filter; }
	inline void filterPtr(RtFilterFunc aFilter) { m_filter = aFilter; }
	inline CFilterSlot &operator=(const CFilterSlot &o)
	{
		if ( this == &o )
			return *this;
		filterPtr(o.filterPtr());
		return *this;
	}
public:
	inline CFilterSlot(const CFilterSlot &o) { *this = o; }
	inline ~CFilterSlot() { }

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
};

// ----------------------------------------------------------------------------

/** @brief An arbitrary error handler for C binding
 */
class CErrorHandlerSlot : public IErrorHandler {
	friend std::map<RtErrorHandler, CErrorHandlerSlot>;

	static std::map<RtErrorHandler, CErrorHandlerSlot> s_errorHandlers; ///< Maps filterfunctions to their objects

	RtErrorHandler m_errorHandler;

	inline virtual IErrorHandler *duplicate() const { return new CErrorHandlerSlot(*this); }

	inline CErrorHandlerSlot(RtErrorHandler aHandler=0) { m_errorHandler = aHandler; }

	inline RtErrorHandler errorHandlerPtr() const { return m_errorHandler; }
	inline void errorHandlerPtr(RtErrorHandler aHandler) { m_errorHandler = aHandler; }
	inline CErrorHandlerSlot &operator=(const CErrorHandlerSlot &o)
	{
		if ( this == &o )
			return *this;
		errorHandlerPtr(o.errorHandlerPtr());
		return *this;
	}
public:
	inline CErrorHandlerSlot(const CErrorHandlerSlot &o) { *this = o; }
	inline ~CErrorHandlerSlot() { }

	inline virtual const IErrorHandler &singleton() const { return *this; }
	inline static const IErrorHandler &getSingleton(RtErrorHandler function)
	{
		if ( function == RiErrorAbort )
			return CAbortErrorHandler::func;
		else if ( function == RiErrorPrint )
			return CPrintErrorHandler::func;
		else if ( function == RiErrorIgnore )
			return CIgnoreErrorHandler::func;

		s_errorHandlers[function] = CErrorHandlerSlot(function);
		return s_errorHandlers[function];
	}

	inline static RtToken myName() { return "CErrorHandlerSlot"; }
	inline virtual RtToken name() const { return myName(); }

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
	friend std::map<RtArchiveCallback, CArchiveCallbackSlot>;

	static std::map<RtArchiveCallback, CArchiveCallbackSlot> s_callbacks; ///< Maps filterfunctions to their objects

	RtArchiveCallback m_callback;

	inline CArchiveCallbackSlot(RtArchiveCallback aCallback=0) : m_callback(aCallback) { }
	inline virtual IArchiveCallback *duplicate() const { return new CArchiveCallbackSlot(*this); }

	inline RtArchiveCallback callbackPtr() const { return m_callback; }
	inline void callbackPtr(RtArchiveCallback aCallback) { m_callback = aCallback; }

	inline CArchiveCallbackSlot &operator=(const CArchiveCallbackSlot &o)
	{
		if ( this == &o )
			return *this;
		callbackPtr(o.callbackPtr());
		return *this;
	}
public:
	inline CArchiveCallbackSlot(const CArchiveCallbackSlot &o) { *this = o; }
	inline virtual ~CArchiveCallbackSlot() { }

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
		} else {
			// Error
		}
	}
}; // CArchiveCallbackSlot

// ----------------------------------------------------------------------------

#define PREAMBLE { \
	if ( RiCPP::RiCPPRoot() == 0 ) { \
		return; \
	} \
}

#define POSTAMBLE \
	RiLastError = RiCPP::RiCPPRoot()->lastError();

#define PREAMBLE_RET(RETVAL) { \
	if ( RiCPP::RiCPPRoot() == 0 ) { \
		return (RETVAL); \
	} \
}

#define POSTAMBLE_RET(RETVAL) \
	RiLastError = RiCPP::RiCPPRoot()->lastError(); \
	return (RETVAL);

#define GETARGS(VAR) \
	RtToken *tokens; \
	RtPointer *params; \
	va_list	args; \
	va_start(args, VAR); \
	RtInt n = RiCPP::RiCPPGetArgs(args, &tokens, &params);

} /* namespace RiCPP */

#endif // _RICPP_RI_RIINTERNAL_H
