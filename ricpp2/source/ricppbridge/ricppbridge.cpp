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

/** @file ricppbridge.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bridges the interface functions to a current renderer
 */

#include "ricpp/ricppbridge/ricppbridge.h"

using namespace RiCPP;

CRiCPPBridge::CRiCPPBridge() :
	m_lastError(RIE_NOERROR)
{
	// Default options
	m_curErrorHandler = &m_printErrorHandler;
}


CRiCPPBridge::~CRiCPPBridge() {
}


RtInt CRiCPPBridge::getTokens(RtToken token, va_list marker)
{
	RtInt n;
	RtPointer param;

	m_tokens.clear();
	m_params.clear();

	for ( n = 0; token != RI_NULL; ++n, token = va_arg(marker, RtToken) ) {
		param = va_arg(marker, RtPointer);
		m_tokens.push_back(token);
		m_params.push_back(param);
	}
	va_end(marker);

	if ( n == 0 ) {
		m_tokens.resize(1);
		m_params.resize(1);
		m_tokens[0] = NULL;
		m_params[0] = NULL;
	}

	return n;
}


RtVoid CRiCPPBridge::handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList) {
	m_lastError = code;

	// Do no more error handling if the context is already aborted
	if ( m_ctxMgmt.curCtx().aborted() )
		return;

	// If a severe error occured, end all rendering and reset the renderer
	if ( severity == RIE_SEVERE ) {
		m_ctxMgmt.abort();
	}

	static const int ERROR_STR_SIZE = 256;
	char str[ERROR_STR_SIZE];
	str[0] = (char)0;
	if ( message && *message && argList ) {
#ifdef WIN32
		_vsnprintf_s(str, ERROR_STR_SIZE-1, ERROR_STR_SIZE-2, message, argList);
#else
		vsnprintf(str, ERROR_STR_SIZE-1, message, argList);
#endif
		str[ERROR_STR_SIZE-1] = (char)0;
		message = str;
	}

	std::string errorstring;
	ERendererError err(code, severity, message, line, file);
	err.formatErrorMessage(errorstring);

	if ( m_curErrorHandler )
		(*m_curErrorHandler)(code, severity, errorstring.c_str());
}


RtToken CRiCPPBridge::declare(RtString name, RtString declaration) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			return m_ctxMgmt.curCtx().renderer()->declare(name, declaration);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::declare(name:\"%s\", declaration:\"%s\")", name ? name : "", declaration ? declaration : "");
	}
	return RI_NULL;
}


RtVoid CRiCPPBridge::begin(RtString name) {
	// Since this can activate a different context creator, deactivate the old context
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().contextCreator()->context(0);
		} catch (ERendererError &e) {
			m_ctxMgmt.context(illContextHandle);
			handleError(e);
		}
	}

	m_ctxMgmt.context(illContextHandle);

	CContextCreator *contextCreator = NULL;
	try {
		contextCreator = rendererCreator().getContextCreator(name);
		if ( !contextCreator ) {
			handleError(RIE_SYSTEM, RIE_SEVERE,
				"Renderer creator missing in CRiCPPBridge::begin(name:\"%s\")",
				name ? name : "");
			return;
		}
	} catch (ERendererError &e) {
		handleError(e);
		return;
	}

	try {
		contextCreator->begin(name);
	} catch (ERendererError &e) {
		handleError(e);
		return;
	}

	CContext ctx(contextCreator, contextCreator->getContext());
	m_ctxMgmt.context(m_ctxMgmt.add(ctx));
}

RtVoid CRiCPPBridge::end(void) {
	if ( m_ctxMgmt.getContext() != illContextHandle ) {
		m_ctxMgmt.end();
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::end()");
	}
}

RtContextHandle CRiCPPBridge::getContext(void) {
	return m_ctxMgmt.getContext();
}

RtVoid CRiCPPBridge::context(RtContextHandle handle) {
	// Since handle can activate a different context creator, deactivate the old context
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().contextCreator()->context(0);
		} catch (ERendererError &e) {
			m_ctxMgmt.context(illContextHandle);
			handleError(e);
		}
	}
	
	if ( handle == illContextHandle ) {
		// Only deactivated the current context, outside a context now.
		return;
	}

	if ( !m_ctxMgmt.context(handle) ) {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::context(handle:%lu)", (unsigned long)handle);
		return;
	}

	// Activate the stored handle
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().contextCreator()->context(m_ctxMgmt.curCtx().renderer());
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::context(handle:%lu), renderer context not set internally", (unsigned long)handle);
	}
}

RtVoid CRiCPPBridge::frameBegin(RtInt number) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->frameBegin(number);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::frameBegin(number:%d)", (int)number);
	}
}

RtVoid CRiCPPBridge::frameEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->frameEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::frameEnd()");
	}
}

RtVoid CRiCPPBridge::worldBegin(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->worldBegin();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::worldBegin()");
	}
}

RtVoid CRiCPPBridge::worldEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->worldEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::worldEnd()");
	}
}

RtVoid CRiCPPBridge::attributeBegin(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->attributeBegin();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::attributeBegin()");
	}
}

RtVoid CRiCPPBridge::attributeEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->attributeEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::attributeEnd()");
	}
}

RtVoid CRiCPPBridge::transformBegin(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->transformBegin();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::transformBegin()");
	}
}

RtVoid CRiCPPBridge::transformEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->transformEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::transformEnd()");
	}
}

RtVoid CRiCPPBridge::solidBegin(RtToken type) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->solidBegin(type);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::solidBegin(type:\"%s\")", type ? type : "");
	}
}

RtVoid CRiCPPBridge::solidEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->solidEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::solidEnd()");
	}
}

RtObjectHandle CRiCPPBridge::objectBegin(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			return m_ctxMgmt.curCtx().renderer()->objectBegin();
		} catch (ERendererError &e) {
			handleError(e);
			return (RtObjectHandle)RI_NULL;
		}
	}

	ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::objectBegin()");
	return (RtObjectHandle)RI_NULL;
}

RtVoid CRiCPPBridge::objectEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->objectEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::objectEnd()");
	}
}

RtVoid CRiCPPBridge::objectInstance(RtObjectHandle handle) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->objectInstance(handle);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::objectInstance(handle)");
	}
}

RtVoid CRiCPPBridge::motionBegin(RtInt N, RtFloat sample, ...) {
	va_list marker;
	std::vector<RtFloat> samples;
	RtInt n;

	samples.reserve(N);

	va_start(marker, sample);
	for ( n = 0; n < N; ++n ) {
		samples.push_back(sample);
		sample = (RtFloat)va_arg(marker, double); // 'default argument promotions' float -> double
	}
	va_end(marker);

	motionBeginV(N, samples.empty() ? NULL : &samples[0]);
	// No try and catch, simple call to ...V() function, exception handling is done there
}

RtVoid CRiCPPBridge::motionBeginV(RtInt N, RtFloat times[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->motionBeginV(N, times);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::motionBeginV(N:%d, ...)", (int) N);
	}
}

RtVoid CRiCPPBridge::motionEnd(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->motionEnd();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::motionEnd()");
	}
}

RtVoid CRiCPPBridge::synchronize(RtToken name) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->synchronize(name);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::synchronize(name:\"%s\")", name ? name : "");
	}
}

RtVoid CRiCPPBridge::format(RtInt xres, RtInt yres, RtFloat aspect) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->format(xres, yres, aspect);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "format(xres:%d, yres:%d, aspect:%f)", (int)xres, (int)yres, (float)aspect);
	}
}

RtVoid CRiCPPBridge::frameAspectRatio(RtFloat aspect) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->frameAspectRatio(aspect);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "frameAspectRatio(aspect:%f)", (float)aspect);
	}
}

RtVoid CRiCPPBridge::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->screenWindow(left, right, bot, top);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "screenWindow(left:%f, right:%f, bot:%f, top:%f)", (float)left, (float)right, (float)bot, (float)top);
	}
}

RtVoid CRiCPPBridge::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->cropWindow(xmin, xmax, ymin, ymax);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "cropWindow(xmin:%f, xmax:%f, ymin:%f, ymax:%f)", (float)xmin, (float)xmax, (float)ymin, (float)ymax);
	}
}

RtVoid CRiCPPBridge::projection(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	projectionV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->projectionV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::projectionV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::clipping(RtFloat hither, RtFloat yon) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->clipping(hither, yon);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::clipping(hither:%f, yon:%f)", (float)hither, (float)yon);
	}
}

RtVoid CRiCPPBridge::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->clippingPlane(x, y, z, nx, ny, nz);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::clippingPlane(x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f)",x, y, z, nx, ny, nz);
	}
}

RtVoid CRiCPPBridge::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->depthOfField(fstop, focallength, focaldistance);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::depthOfField(fstop:%f, focallength:%f, focaldistance:%f)", (float)fstop, (float)focallength, (float)focaldistance);
	}
}

RtVoid CRiCPPBridge::shutter(RtFloat smin, RtFloat smax) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->shutter(smin, smax);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::shutter(smin:%f, smax:%f)", (float)smin, (float)smax);
	}
}

RtVoid CRiCPPBridge::pixelVariance(RtFloat variation) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pixelVariance(variation);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::pixelVariance(variation:%f)", (float)variation);
	}
}

RtVoid CRiCPPBridge::pixelSamples(RtFloat xsamples, RtFloat ysamples) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pixelSamples(xsamples, ysamples);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::pixelSamples(xsamples:%f, ysamples:%f)", (float)xsamples, (float)ysamples);
	}
}

RtVoid CRiCPPBridge::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pixelFilter(function, xwidth, ywidth);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::pixelFilter(function:%s, xwidth:%f, ywidth:%f)", function.name(), (float)xwidth, (float)ywidth);
	}
}

RtVoid CRiCPPBridge::exposure(RtFloat gain, RtFloat gamma) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->exposure(gain, gamma);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::exposure(gain:%f, gamma:%f)", (float)gain, (float)gamma);
	}
}

RtVoid CRiCPPBridge::imager(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	imagerV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->imagerV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::imagerV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->quantize(type, one, qmin, qmax, ampl);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::quantize(type:%s, one:%d, qmin:%d, qmax:%d, ampl:%f)", type ? type : "", (int)one, (int)qmin, (int)qmax, (float)ampl);
	}
}

RtVoid CRiCPPBridge::display(RtString name, RtToken type, RtToken mode, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	displayV(name, type, mode, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::displayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->displayV(name, type, mode, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::displayV(name:%s, type:%s, mode:%s, n:%d, ...)", name ? name : "", type ? type : "", mode ? mode : "", (int)n);
	}
}

RtVoid CRiCPPBridge::hider(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	imagerV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->hiderV(type, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::hiderV(type:%s, n:%d, ...)", type ? type : "", (int)n);
	}
}

RtVoid CRiCPPBridge::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->colorSamples(N, nRGB, RGBn);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::colorSamples(N:%d, ...)", (int)N);
	}
}

RtVoid CRiCPPBridge::relativeDetail(RtFloat relativedetail) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->relativeDetail(relativedetail);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::relativeDetail(relativedetail:%f)", (float)relativedetail);
	}
}

RtVoid CRiCPPBridge::option(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	optionV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->optionV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		// options for the Renderer creator and its children
		try {
			doOptionV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	}
}

RtVoid CRiCPPBridge::doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !name )
		return;

	// The searchpath for the dynamic renderer libraries
	if ( !strcmp(name, "searchpath") ) {
		if ( n < 1 )
			return;
		if ( !strcmp(tokens[0], "renderer") ) {
			rendererCreator().searchpath((RtString)params[0]);
		}
	}
}

RtLightHandle CRiCPPBridge::lightSource(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return lightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle CRiCPPBridge::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			return m_ctxMgmt.curCtx().renderer()->lightSourceV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::lightSourceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
	return (RtLightHandle) RI_NULL;
}

RtLightHandle CRiCPPBridge::areaLightSource(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return areaLightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle CRiCPPBridge::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			return m_ctxMgmt.curCtx().renderer()->areaLightSourceV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::areaLightSourceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
	return (RtLightHandle) RI_NULL;
}

RtVoid CRiCPPBridge::attribute(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return attributeV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->attributeV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::attributeV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::color(RtColor Cs) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->color(Cs);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::color(Cs)");
	}
}

RtVoid CRiCPPBridge::opacity(RtColor Cs) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->opacity(Cs);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::opacity(Cs)");
	}
}

RtVoid CRiCPPBridge::surface(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return surfaceV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->surfaceV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::surfaceV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::atmosphere(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return atmosphereV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->atmosphereV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::atmosphereV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::interior(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return interiorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->interiorV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::interiorV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::exterior(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return exteriorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->exteriorV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::exteriorV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}


RtVoid CRiCPPBridge::illuminate(RtLightHandle light, RtBoolean onoff) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->illuminate(light, onoff);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::illuminate(light, onoff:%d)", (int)onoff);
	}
}


RtVoid CRiCPPBridge::displacement(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return displacementV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->displacementV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::displacementV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::textureCoordinates(s1:%f, t1:%f, s2:%f, t2:%f, s3:%f, t3:%f, s4:%f, t4:%f)", (float)s1, (float)t1, (float)s2, (float)t2, (float)s3, (float)t3, (float)s4, (float)t4);
	}
}

RtVoid CRiCPPBridge::shadingRate(RtFloat size) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->shadingRate(size);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::shadingRate(size:%f)", (float)size);
	}
}

RtVoid CRiCPPBridge::shadingInterpolation(RtToken type) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->shadingInterpolation(type);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::shadingInterpolation(type:%s)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::matte(RtBoolean onoff) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->matte(onoff);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::matte(onoff:%d)", (int)onoff);
	}
}

RtVoid CRiCPPBridge::bound(RtBound aBound) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->bound(aBound);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::bound(aBound)");
	}
}

RtVoid CRiCPPBridge::detail(RtBound aBound) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->detail(aBound);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::detail(aBound)");
	}
}

RtVoid CRiCPPBridge::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->detailRange(minvis, lowtran, uptran, maxvis);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::detailRange(minvis:%f, lowtran:%f, uptran:%f, maxvis:%f)", (float)minvis, (float)lowtran, (float)uptran, (float)maxvis);
	}
}

RtVoid CRiCPPBridge::geometricApproximation(RtToken type, RtFloat value) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->geometricApproximation(type, value);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::geometricApproximation(type:%s, value%%f)", type ? type : "", (float)value);
	}
}

RtVoid CRiCPPBridge::geometricRepresentation(RtToken type) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->geometricRepresentation(type);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::geometricRepresentation(type:%s)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::orientation(RtToken anOrientation) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->orientation(anOrientation);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::orientation(anOrientation:%s)", anOrientation ? anOrientation : "");
	}
}

RtVoid CRiCPPBridge::reverseOrientation(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->reverseOrientation();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::reverseOrientation()");
	}
}

RtVoid CRiCPPBridge::sides(RtInt nsides) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->sides(nsides);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::sides(nsides:%d)", (int)nsides);
	}
}

RtVoid CRiCPPBridge::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->basis(ubasis, ustep, vbasis, vstep);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::basis(ubasis, ustep, vbasis, vstep)");
	}
}

RtVoid CRiCPPBridge::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::trimCurve(nloops:%d, ncurves, order, knot, amin, amax, n, u, v, w)", (int)nloops);
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::identity(void) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->identity();
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::identity()");
	}
}

RtVoid CRiCPPBridge::transform(RtMatrix aTransform) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->transform(aTransform);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::transform(aTransform)");
	}
}

RtVoid CRiCPPBridge::concatTransform(RtMatrix aTransform) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->concatTransform(aTransform);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::concatTransform(aTransform)");
	}
}

RtVoid CRiCPPBridge::perspective(RtFloat fov) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->perspective(fov);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::perspective(fov:%f)",(float)fov);
	}
}

RtVoid CRiCPPBridge::translate(RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->translate(dx, dy, dz);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::translate(dx:%f, dy:%f, dz:%f)", (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->rotate(angle, dx, dy, dz);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::rotate(angle:%f, dx:%f, dy:%f, dz:%f)", (float)angle, (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::scale(RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->scale(dx, dy, dz);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::scale(dx:%f, dy:%f, dz:%f)", (float)dx, (float)dy, (float)dz);
	}
}

RtVoid CRiCPPBridge::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::skew(angle:%f, dx1:%f, dy1:%f, dz1:%f, dx2:%f, dy2:%f, dz2:%f)", (float)angle, (float)dx1, (float)dy1, (float)dz1, (float)dx2, (float)dy2, (float)dz2);
	}
}

RtVoid CRiCPPBridge::deformation(RtString name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return deformationV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->deformationV(name, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::deformationV(name:%s, n:%d, ...)", name ? name : "", (int)n);
	}
}

RtVoid CRiCPPBridge::coordinateSystem(RtToken space) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->coordinateSystem(space);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::coordinateSystem(space:%s)", space ? space : "");
	}
}

RtVoid CRiCPPBridge::coordSysTransform(RtToken space) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->coordSysTransform(space);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::coordSysTransform(space:%s)", space ? space : "");
	}
}

RtPoint *CRiCPPBridge::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			return m_ctxMgmt.curCtx().renderer()->transformPoints(fromspace, tospace, npoints, points);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::transformPoints(fromspace:%s, tospace:%s, npoints:%d, ...)", fromspace ? fromspace : "", tospace ? tospace : "", (int)npoints);
	}
	return NULL;
}

/******************************************************************************/

RtVoid CRiCPPBridge::polygon(RtInt nvertices, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return polygonV(nvertices, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->polygonV(nvertices, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::polygonV(nvertices:%d, ...)", (int)nvertices);
	}
}

RtVoid CRiCPPBridge::generalPolygon(RtInt nloops, RtInt *nverts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return generalPolygonV(nloops, nverts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->generalPolygonV(nloops, nverts, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::generalPolygonV(nloops:%d, ...)", (int)nloops);
	}
}

RtVoid CRiCPPBridge::pointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsPolygonsV(npolys, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::generalPolygonV(npolys:%d, ...)", (int)npolys);
	}
}

RtVoid CRiCPPBridge::pointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::pointsGeneralPolygonsV(npolys:%d, ...)", (int)npolys);
	}
}

RtVoid CRiCPPBridge::patch(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return patchV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->patchV(type, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::patchV(type:%s, ...)", type ? type : "");
	}
}

RtVoid CRiCPPBridge::patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return patchMeshV(type, nu, uwrap, nv, vwrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::patchMeshV(type:%s, nu:%d, uwrap:%s, nv:%d, vwrap:%s, ...)", type ? type : "", (int)nu, uwrap ? uwrap : "", (int)nv, vwrap ? vwrap : "");
	}
}

RtVoid CRiCPPBridge::nuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::nuPatchV(nu:%d, uorder:%d, ...)", (int)nu, (int)uorder);
	}
}


RtVoid CRiCPPBridge::subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::subdivisionMeshV(scheme:%s, nfaces:%d, ...)", scheme ? scheme : "", (int)nfaces);
	}
}

RtVoid CRiCPPBridge::sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return sphereV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::sphereV(radius:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)radius, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return coneV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->coneV(height, radius, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::sphereV(height:%f, radius:%f, thetamax:%f, ...)", (float)height, (float)radius, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return cylinderV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::cylinderV(radius:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)radius, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return hyperboloidV(point1, point2, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		ricppErrHandler().handleErrorV(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::hyperboloidV(...)");
	}
}

RtVoid CRiCPPBridge::paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return paraboloidV(rmax, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::paraboloidV(rmax:%f, zmin:%f, zmax:%f, thetamax:%f, ...)", (float)rmax, (float)zmin, (float)zmax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return diskV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->diskV(height, radius, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::diskV(height:%f, radius:%f, thetamax:%f, ...)", (float)height, (float)radius, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return torusV(majorrad, minorrad, phimin, phimax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::torusV(majorrad:%f, minorrad:%f, phimin:%f, phimax:%f, thetamax:%f, ...)", (float)majorrad, (float)minorrad, (float)phimin, (float)phimax, (float)thetamax);
	}
}

RtVoid CRiCPPBridge::points(RtInt npts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return pointsV(npts, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->pointsV(npts, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::pointsV(npts:%f, ...)", (int)npts);
	}
}

RtVoid CRiCPPBridge::curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return curvesV(type, ncurves, nverts, wrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->curvesV(type, ncurves, nverts, wrap, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::curvesV(type:%s, ncurves:%d, ...)", type ? type : "", (int)ncurves);
	}
}

RtVoid CRiCPPBridge::blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::blobbyV(nleaf:%d, ...)", (int)nleaf);
	}
}

RtVoid CRiCPPBridge::procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->procedural(data, bound, subdivfunc, freefunc);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::procedural(..., subdivfunc:%s, freefunc:%s)", subdivfunc.name(), freefunc.name());
	}
}

RtVoid CRiCPPBridge::geometry(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return geometryV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->geometryV(type, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::geometryV(type:%s, ...)", type ? type : "" );
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::makeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::makeTextureV(pic:%s, tex:%s, swrap:%s, twrap:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", swrap ? swrap : swrap, twrap ? twrap : twrap, filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::makeBumpV(pic:%s, tex:%s, swrap:%s, twrap:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", swrap ? swrap : swrap, twrap ? twrap : twrap, filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::makeLatLongEnvironmentV(pic:%s, tex:%s, filterfunc:%s, swidth:%f, twidth:%f, ...)", pic ? pic : pic, tex ? tex : "", filterfunc.name(), (float)swidth, (float)twidth );
	}
}

RtVoid CRiCPPBridge::makeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::makeCubeFaceEnvironmentV(px:%s, nx:%s, py:%s, ny:%s, pz:%s, nz:%s, tex:%s, fov:%f, filterfunc:%s, swidth:%f, twidth:%f, ...)", px ? px : px, nx ? nx : nx, py ? py : py, ny ? ny : ny, pz ? pz : pz, nz ? nz : nz, tex ? tex : "", (float)fov, filterfunc.name(), (float)swidth, (float)twidth);
	}
}

RtVoid CRiCPPBridge::makeShadow(RtString pic, RtString tex, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return makeShadowV(pic, tex, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->makeShadowV(pic, tex, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::makeShadowV(pic:%s, tex:%s, ...)", pic ? pic : pic, tex ? tex : "");
	}
}

/******************************************************************************/

RtVoid CRiCPPBridge::archiveRecord(RtToken type, RtString format, ...) {
	static const int ARCHIVE_BUF_SIZE = 512;
	char archiveBuf[ARCHIVE_BUF_SIZE];
	va_list argList;
	va_start(argList, format);
	archiveBuf[0] = (char)0;
#ifdef WIN32
	_vsnprintf_s(archiveBuf, ARCHIVE_BUF_SIZE-1, ARCHIVE_BUF_SIZE-2, format ? format : "", argList);
#else
	vsnprintf(archiveBuf, ARCHIVE_BUF_SIZE-1, format ? format : "", argList);
#endif
	archiveBuf[ARCHIVE_BUF_SIZE-1] = (char)0;
	va_end(argList);
	archiveRecordV(type, archiveBuf);
}

RtVoid CRiCPPBridge::archiveRecordV(RtToken type, RtString line) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->archiveRecordV(type, line);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::archiveRecordV(type:%s, line:%s, ...)", type ? type : type, line ? line : "");
	}
}

RtVoid CRiCPPBridge::readArchive(RtString name, const IArchiveCallback *callback, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
	RtInt n = getTokens(token, marker);

	return readArchiveV(name, callback, n, &m_tokens[0], &m_params[0]);
}

RtVoid CRiCPPBridge::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_ctxMgmt.curCtx().valid() ) {
		try {
			m_ctxMgmt.curCtx().renderer()->readArchiveV(name, callback, n, tokens, params);
		} catch (ERendererError &e) {
			handleError(e);
		}
	} else {
		handleError(RIE_BADHANDLE, RIE_SEVERE, "CRiCPPBridge::readArchiveV(name:%s, callback:%s, ...)", name ? name : name, callback ? callback->name() : "");
	}
}
