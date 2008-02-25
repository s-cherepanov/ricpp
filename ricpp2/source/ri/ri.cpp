// Adapter ri - ricpp Common for programs and dynamic load procedurals, external IRi

#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif // _RICPP_RICPP_FILTERS_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <cstdarg>
#include <cassert>
#include <cmath>
#include <vector>
#include <map>

using namespace RiCPP;

/** @brief An arbitrary pixel filter for C binding
 */
class CFilterSlot : public IFilterFunc {
	RtFilterFunc m_filter;
public:
	inline CFilterSlot(RtFilterFunc aFilter=0) { m_filter = aFilter; }
	inline CFilterSlot(const CFilterSlot &o) { *this = o; }

	static RtToken myName();

	inline virtual IFilterFunc *duplicate() const { return new CFilterSlot(*this); }
	inline virtual RtToken name() const { return myName(); }

	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;

	inline virtual const IFilterFunc &singleton() const { return *this; }

	inline RtFilterFunc filterPtr() const { return m_filter; }
	inline void filterPtr(RtFilterFunc aFilter) { m_filter = aFilter; }
	inline CFilterSlot &operator=(const CFilterSlot &o) {
		if ( this == &o )
			return *this;
		filterPtr(o.filterPtr());
		return *this;
	}
};

//
// CFilterSlot
//
RtToken CFilterSlot::myName() {return RI_EMPTY; }
RtFloat	CFilterSlot::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	if ( m_filter )
		return (*m_filter)(x, y, xwidth, ywidth);
	// Error
	return 0;
}

static std::map<RtFilterFunc, CFilterSlot> m_filters;

static IRiRoot *_riRoot = 0;

IRiRoot *_ricppRoot() {
	return _riRoot;
}

void _ricppRoot(IRiRoot *aRoot) {
	_riRoot = aRoot;
}

#ifdef RICPP_EXTERN
#undef RICPP_EXTERN
#endif
#define RICPP_EXTERN(atype) atype

#define PREAMBLE { \
	if ( _ricppRoot() == 0 ) { \
		return; \
	} \
}

#define POSTAMBLE

#define PREAMBLE_RET(RETVAL) { \
	if ( _ricppRoot() == 0 ) { \
		return RETVAL; \
	} \
}

#define POSTAMBLE_RET(RETVAL) \
	return RETVAL;

#define GETARGS(VAR) \
	RtToken *tokens; \
	RtPointer *params; \
	va_list	args; \
	va_start(args,VAR); \
	RtInt n = getArgs(args,VAR, &tokens, &params);


static RtInt getArgs(va_list marker, RtToken token, RtToken **tokens, RtPointer **params) {
	static std::vector<RtToken> vec_tokens;   // The tokens of the parameter list of an interface call
	static std::vector<RtPointer> vec_params; // The values of the parameter list of an interface call

	RtInt n;
	RtPointer param;

	assert(tokens != 0);
	assert(params != 0);

	vec_tokens.clear(); *tokens = 0;
	vec_params.clear(); *params = 0;

	for ( n = 0; token != RI_NULL; ++n, token = va_arg(marker, RtToken) ) {
		param = va_arg(marker, RtPointer);
		vec_tokens.push_back(token);
		vec_params.push_back(param);
	}
	va_end(marker);

	if ( n == 0 ) {
		vec_tokens.resize(1);
		vec_params.resize(1);
		vec_tokens[0] = 0;
		vec_params[0] = 0;
	}

	*tokens = &(vec_tokens[0]);
	*params = &(vec_params[0]);

	return n;
}

extern "C" {

RICPP_EXTERN(RtInt) RiLastError = RIE_NOERROR;

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiCPPBegin(RtToken name, ...)
{
	GETARGS(name)
	RiCPPBeginV(name, n, tokens, params);
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiBegin(RtToken name)
{
	// RiCPPBeginV() is defined in riprog.cpp for programs (sets _ricppRoot())
	// RiCPPBeginV() is defined as an error in ridynload.cpp
	RtContextHandle r = RiCPPBeginV(name, 0, 0, 0);
}

RICPP_EXTERN(RtVoid) RiEnd(void)
{
	// RiCPPEnd() is defined in riprog.cpp for programs (clears _ricppRoot())
	// RiCPPEnd() is defined as an error in ridynload.cpp
	RiCPPEnd();
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtObjectHandle) RiObjectBegin()
{
	return RiCPPObjectBegin(0);
}

RICPP_EXTERN(RtVoid) RiObjectEnd(void)
{
	RiCPPObjectEnd();
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtObjectHandle) RiCPPObjectBegin(RtToken name)
{
	RtObjectHandle h = illObjectHandle;
	PREAMBLE_RET(h)
		h = _ricppRoot()->objectBegin(name);
	POSTAMBLE_RET(h)
}

RICPP_EXTERN(RtVoid) RiCPPObjectEnd(void)
{
	PREAMBLE
		_ricppRoot()->objectEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiCPPControl(RtToken name, ...)
{
	PREAMBLE
	GETARGS(name)
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiCPPControlV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtToken) RiDeclare (const char *name,const char *declaration)
{
	RtToken h = RI_NULL;
	PREAMBLE_RET(h)
		h = _ricppRoot()->declare(name, declaration);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiSynchronize(RtToken type)
{
	PREAMBLE
		_ricppRoot()->synchronize(type);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiSystem(RtString cmd)
{
	PREAMBLE
		_ricppRoot()->system(cmd);
	POSTAMBLE
}


// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiFrameBegin(RtInt number)
{
	PREAMBLE
		_ricppRoot()->frameBegin(number);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiFrameEnd(void)
{
	PREAMBLE
	_ricppRoot()->frameEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiWorldBegin(void)
{
	PREAMBLE
		_ricppRoot()->worldBegin();
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiWorldEnd(void)
{
	PREAMBLE
		_ricppRoot()->worldEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->format(xres, yres, aspect);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiFrameAspectRatio(RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->frameAspectRatio(aspect);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	PREAMBLE
		_ricppRoot()->screenWindow(left, right, bot, top);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	PREAMBLE
		_ricppRoot()->cropWindow(xmin, xmax, ymin, ymax);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiProjection(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiProjectionV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiClipping(RtFloat hither, RtFloat yon)
{
	PREAMBLE
		_ricppRoot()->clipping(hither, yon);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	PREAMBLE
		_ricppRoot()->clippingPlane(x, y, z, nx, ny, nz);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	PREAMBLE
		_ricppRoot()->depthOfField(fstop, focallength, focaldistance);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiShutter(RtFloat smin, RtFloat smax)
{
	PREAMBLE
		_ricppRoot()->shutter(smin, smax);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiPixelVariance(RtFloat variation)
{
	PREAMBLE
		_ricppRoot()->pixelVariance(variation);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	PREAMBLE
		_ricppRoot()->pixelSamples(xsamples, ysamples);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth)
{
	PREAMBLE
	if ( function == RiGaussianFilter )
		_ricppRoot()->pixelFilter(CGaussianFilter::func, xwidth, ywidth);
	else if ( function == RiBoxFilter )
		_ricppRoot()->pixelFilter(CBoxFilter::func, xwidth, ywidth);
	else if ( function == RiTriangleFilter )
		_ricppRoot()->pixelFilter(CCatmullRomFilter::func, xwidth, ywidth);
	else if ( function == RiSincFilter )
		_ricppRoot()->pixelFilter(CSincFilter::func, xwidth, ywidth);
	else {
		m_filters[function] = CFilterSlot(function);
		_ricppRoot()->pixelFilter(m_filters[function], xwidth, ywidth);
	}
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiExposure (RtFloat gain, RtFloat gamma)
{
	PREAMBLE
		_ricppRoot()->exposure(gain, gamma);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiImager(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiImagerV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	PREAMBLE
		_ricppRoot()->quantize(type, one, qmin, qmax, ampl);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplay (char *name, RtToken type, RtToken mode, ...)
{
	PREAMBLE
		GETARGS(mode)
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayV (char *name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayChannel (RtToken channel, ...)
{
	PREAMBLE
		GETARGS(channel)
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}


} // extern "C"
