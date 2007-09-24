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

/** @file baserenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the backend base class of a renderer context.
 */

#include "ricpp/baserenderer/baserenderer.h"
#include <cassert>

using namespace RiCPP;

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_ri(0),
	m_errorHandler(0),
	m_protocolHandler(0),
	m_ribFilter(0),
	m_macroFactory(0),
	m_curMacro(0)
{
	CFilepath fp;
	std::string s(fp.filepath());
	s+= "/";
	m_baseUri.set("file", "", s.c_str(), 0, 0);
}

CBaseRenderer::~CBaseRenderer()
{
	if ( m_renderState )
		delete m_renderState;
	if ( m_macroFactory )
		delete m_macroFactory;
}

void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	m_renderState = 0;
	CModeStack *modeStack = 0;
	COptionsFactory *optionsFactory = 0;
	CAttributesFactory *attributesFactory = 0;

	try {
		modeStack = getNewModeStack();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !modeStack ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		return;
	}

	try {
		optionsFactory = getNewOptionsFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !optionsFactory ) {
		delete modeStack;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an options factory");
		return;
	}

	try {
		attributesFactory = getNewAttributesFactory();
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !attributesFactory ) {
		delete modeStack;
		delete optionsFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
		return;
	}

	try {
		m_renderState = new CRenderState(*modeStack, *optionsFactory, *attributesFactory);
	} catch (ExceptRiCPPError &err) {
		ricppErrHandler().handleError(err);
		return;
	} catch (...) {
	}

	if ( !m_renderState ) {
		delete modeStack;
		delete optionsFactory;
		delete attributesFactory;
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
		return;
	}
}

bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "State not initialized in %s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->validRequest(req) ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_ERROR, "%s()", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->hasOptionsReader() ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "%s() - options not available.", CRequestInfo::requestName(req));
		return false;
	}

	if ( !m_renderState->hasAttributesReader() ) {
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, "%s() - attributes not available.", CRequestInfo::requestName(req));
		return false;
	}

	return true;
}

/** @brief Create new entry in dectaration list
 */
RtToken CBaseRenderer::handleDeclaration(RtString name, RtString declaration, bool isDefault)
{
	RtToken token = RI_NULL;
	CDeclaration *d = 0;

	try {
		token = m_renderState->tokFindCreate(name);
		// if no declaration only tokenize the name
		if ( !emptyStr(declaration) ) {
		
			try {
				d = new CDeclaration(token, declaration, 3, isDefault); // <--- curColorSize if attributes are implemented !!!!
			} catch (ExceptRiCPPError &e) {
				ricppErrHandler().handleError(e);
				return RI_NULL;
			}
			
			if ( !d )
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Declaration of \"%s\": \"%s\"", name, declaration);
			m_renderState->declAdd(d);
		}
	} catch (ExceptRiCPPError &e) {
		ricppErrHandler().handleError(e);
		return RI_NULL;
	}

	return token;
}

RtToken CBaseRenderer::declare(RtString name, RtString declaration)
{
	if ( !preCheck(REQ_DECLARE) )
		return RI_NULL;

	if ( !name || !*name ) {
		ricppErrHandler().handleError(RIE_MISSINGDATA, RIE_ERROR, "name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration));
		return RI_NULL;
	}


	RtToken token = RI_NULL;

	try {
		token = handleDeclaration(name, declaration, false);
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
		return RI_NULL;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDeclare *m = m_macroFactory->newRiDeclare(m_renderState->lineNo(), name, declaration);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDeclare", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
			return RI_NULL;
		}

	} else {

		try {
			doDeclare(token, declaration);
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
			return RI_NULL;
		}

	}

	return token;
}

RtContextHandle CBaseRenderer::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	// Render state is initialized here, there is no mode so it must be not valid
	// This is the case because begin is only called through the frame work
	// A begin at the frontend always creates a new backend.
	if ( m_renderState ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return 0;
	}

	if ( !m_macroFactory )
		m_macroFactory = getNewMacroFactory();

	initRenderState(); // Can throw

	try {
		m_renderState->contextBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'begin'");
		return 0;
	}

	// Default declarations
	handleDeclaration(RI_FLATNESS, "float", true);
	handleDeclaration(RI_FOV, "float", true);

	handleDeclaration(RI_INTENSITY, "float", true);
	handleDeclaration(RI_LIGHTCOLOR, "color", true);
	handleDeclaration(RI_FROM, "point", true);
	handleDeclaration(RI_TO, "point", true);
	handleDeclaration(RI_CONEANGLE, "float", true);
	handleDeclaration(RI_CONEDELTAANGLE, "float", true);
	handleDeclaration(RI_BEAMDISTRIBUTION, "float", true);

	handleDeclaration(RI_KA, "float", true);
	handleDeclaration(RI_KD, "float", true);
	handleDeclaration(RI_KS, "float", true);
	handleDeclaration(RI_ROUGHNESS, "float", true);
	handleDeclaration(RI_KR, "float", true);
	handleDeclaration(RI_TEXTURENAME, "string", true);
	handleDeclaration(RI_SPECULARCOLOR, "color", true);
	handleDeclaration(RI_MINDISTANCE, "float", true);
	handleDeclaration(RI_MAXDISTANCE, "float", true);
	handleDeclaration(RI_BACKGROUND, "color", true);
	handleDeclaration(RI_DISTANCE, "float", true);
	handleDeclaration(RI_AMPLITUDE, "float", true);

	handleDeclaration(RI_P, "vertex point", true);
	handleDeclaration(RI_PZ, "vertex float", true);
	handleDeclaration(RI_PW, "vertex hpoint", true);
	handleDeclaration(RI_N,  "varying point", true);  // Normal
	handleDeclaration(RI_NP, "uniform point", true);
	handleDeclaration(RI_CS, "varying color", true);  // Color
	handleDeclaration(RI_OS, "varying color", true);  // Opacity
	handleDeclaration(RI_S,  "varying float", true);  // Texture coordinates
	handleDeclaration(RI_T,  "varying float", true);
	handleDeclaration(RI_ST, "varying float[2]", true);

	handleDeclaration(RI_ORIGIN, "constant integer[2]", true);   // Origin of the display

	handleDeclaration(RI_NAME, "constant string", true);
	handleDeclaration(RI_WIDTH, "varying float", true);
	handleDeclaration(RI_CONSTANTWIDTH, "float", true);

	doBeginV(name, n, tokens, params); // Can throw

	return 0;
}

RtVoid CBaseRenderer::end(void)
// throw ExceptRiCPPError
{
	if ( !m_renderState ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, "State not initialized in end(), break.");
		return;
	}

	ExceptRiCPPError err;
	if ( m_renderState->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, "Ended context not at begin-state");
	}

	m_renderState->contextEnd();

	try {
		doEnd(); // Can throw, err is lost then
	} catch ( ExceptRiCPPError &err2 )  {
		err = err2;
	}

	// Delete the state anyway
	delete m_renderState;
	m_renderState = 0;

	if ( err.isError() ) {
		ricppErrHandler().handleError(err);
		return;
	}
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_FRAME_BEGIN) )
		return;

	try {
		m_renderState->frameBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'frameBegin', Frame no.: %d", (int)number);
		return;
	}

	m_renderState->frameNumber(number);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameBegin *m = m_macroFactory->newRiFrameBegin(m_renderState->lineNo(), number);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameBegin(number); // Can throw
	}
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_FRAME_END) )
		return;
	
	m_renderState->frameEnd();
	m_renderState->frameNumber(0);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameEnd *m = m_macroFactory->newRiFrameEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameEnd();
	}
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_WORLD_BEGIN) )
		return;

	try {
		m_renderState->worldBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'worldBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiWorldBegin *m = m_macroFactory->newRiWorldBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiWorldBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doWorldBegin();
	}
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_WORLD_END) )
		return;
	
	m_renderState->worldEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiWorldEnd *m = m_macroFactory->newRiWorldEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiWorldEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doWorldEnd();
	}
}

RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_ATTRIBUTE_BEGIN) )
		return;
	
	try {
		m_renderState->attributeBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAttributeBegin *m = m_macroFactory->newRiAttributeBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttributeBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAttributeBegin();
	}
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_ATTRIBUTE_END) )
		return;
	
	m_renderState->attributeEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiAttributeEnd *m = m_macroFactory->newRiAttributeEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiAttributeEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doAttributeEnd();
	}
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_TRANSFORM_BEGIN) )
		return;
	
	try {
		m_renderState->transformBegin();
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_NOMEM, RIE_SEVERE, "Could not allocate memory for the state 'attributeBegin'");
		return;
	}

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTransformBegin *m = m_macroFactory->newRiTransformBegin(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTransformBegin", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doTransformBegin();
	}
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	if ( !preCheck(REQ_TRANSFORM_END) )
		return;

	m_renderState->transformEnd();

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiTransformEnd *m = m_macroFactory->newRiTransformEnd(m_renderState->lineNo());
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiTransformEnd", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doTransformEnd();
	}
}


RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_READ_ARCHIVE) )
		return;

	doReadArchiveV(name, callback, n, tokens, params);
}

RtVoid CBaseRenderer::doReadArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
	CUri sav(m_baseUri);
	const char *oldArchiveName = m_renderState->archiveName();
	long oldLineNo = m_renderState->lineNo();

	CRibParser parser(*m_ri, *m_errorHandler, *m_protocolHandler, *m_ribFilter, *m_renderState, m_baseUri);
	try {
		if ( parser.canParse(name) ) {
			m_baseUri = parser.absUri();
			m_renderState->archiveName(name);
			m_renderState->lineNo(0);
			parser.parse(callback, n, tokens, params);
			m_renderState->archiveName(oldArchiveName);
			m_renderState->lineNo(oldLineNo);
			m_baseUri = sav;
			parser.close();
		}
	} catch(ExceptRiCPPError &err) {
		m_baseUri = sav;
		m_renderState->archiveName(oldArchiveName);
		m_renderState->lineNo(oldLineNo);
		parser.close();
		ricppErrHandler().handleError(err);
		return;
	} catch(...) {
		m_baseUri = sav;
		m_renderState->archiveName(oldArchiveName);
		m_renderState->lineNo(oldLineNo);
		parser.close();
		ricppErrHandler().handleError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "%s", "unknown error");
		return;
	}
}


RtVoid CBaseRenderer::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	if ( !preCheck(REQ_FORMAT) )
		return;

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFormat *m = m_macroFactory->newRiFormat(m_renderState->lineNo(), xres, yres, aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFormat", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFormat(xres, yres, aspect);
	}
}


RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	if ( !preCheck(REQ_FRAME_ASPECT_RATIO) )
		return;

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiFrameAspectRatio *m = m_macroFactory->newRiFrameAspectRatio(m_renderState->lineNo(), aspect);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiFrameAspectRatio", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doFrameAspectRatio(aspect);
	}
}


RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	if ( !preCheck(REQ_SCREEN_WINDOW) )
		return;

	m_renderState->options().screenWindow(left, right, bot, top);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiScreenWindow *m = m_macroFactory->newRiScreenWindow(m_renderState->lineNo(), left, right, bot, top);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiScreenWindow", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doScreenWindow(left, right, bot, top);
	}
}


RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	if ( !preCheck(REQ_CROP_WINDOW) )
		return;

	m_renderState->options().cropWindow(xmin, xmax, ymin, ymax);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiCropWindow *m = m_macroFactory->newRiCropWindow(m_renderState->lineNo(), xmin, xmax, ymin, ymax);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiCropWindow", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doCropWindow(xmin, xmax, ymin, ymax);
	}
}


RtVoid CBaseRenderer::projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_PROJECTION) )
		return;

	m_renderState->options().projectionV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiProjection *m = m_macroFactory->newRiProjection(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiProjection", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doProjectionV(name, n, tokens, params);
	}
}


RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	if ( !preCheck(REQ_CLIPPING) )
		return;

	m_renderState->options().clipping(hither, yon);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiClipping *m = m_macroFactory->newRiClipping(m_renderState->lineNo(), hither, yon);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClipping", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doClipping(hither, yon);
	}
}


RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	if ( !preCheck(REQ_CLIPPING_PLANE) )
		return;

	m_renderState->options().clippingPlane(x, y, z, nx, ny, nz);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiClippingPlane *m = m_macroFactory->newRiClippingPlane(m_renderState->lineNo(), x, y, z, nx, ny, nz);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiClippingPlane", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doClippingPlane(x, y, z, nx, ny, nz);
	}
}


RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	if ( !preCheck(REQ_DEPTH_OF_FIELD) )
		return;

	m_renderState->options().depthOfField(fstop, focallength, focaldistance);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDepthOfField *m = m_macroFactory->newRiDepthOfField(m_renderState->lineNo(), fstop, focallength, focaldistance);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDepthOfField", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDepthOfField(fstop, focallength, focaldistance);
	}
}


RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	if ( !preCheck(REQ_SHUTTER) )
		return;

	m_renderState->options().shutter(smin, smax);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiShutter *m = m_macroFactory->newRiShutter(m_renderState->lineNo(), smin, smax);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiShutter", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doShutter(smin, smax);
	}
}


RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	if ( !preCheck(REQ_PIXEL_VARIANCE) )
		return;

	m_renderState->options().pixelVariance(variation);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelVariance *m = m_macroFactory->newRiPixelVariance(m_renderState->lineNo(), variation);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelVariance", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelVariance(variation);
	}
}


RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( !preCheck(REQ_PIXEL_SAMPLES) )
		return;

	m_renderState->options().pixelSamples(xsamples, ysamples);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelSamples *m = m_macroFactory->newRiPixelSamples(m_renderState->lineNo(), xsamples, ysamples);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelSamples", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelSamples(xsamples, ysamples);
	}
}


RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( !preCheck(REQ_PIXEL_FILTER) )
		return;

	m_renderState->options().pixelFilter(function, xwidth, ywidth);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiPixelFilter *m = m_macroFactory->newRiPixelFilter(m_renderState->lineNo(), function, xwidth, ywidth);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiPixelFilter", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doPixelFilter(function, xwidth, ywidth);
	}
}


RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	if ( !preCheck(REQ_EXPOSURE) )
		return;

	m_renderState->options().exposure(gain, gamma);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiExposure *m = m_macroFactory->newRiExposure(m_renderState->lineNo(), gain, gamma);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiExposure", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doExposure(gain, gamma);
	}
}


RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_IMAGER) )
		return;

	m_renderState->options().imagerV(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiImager *m = m_macroFactory->newRiImager(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiImager", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doImagerV(name, n, tokens,  params);
	}
}


RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( !preCheck(REQ_QUANTIZE) )
		return;

	m_renderState->options().quantize(type, one, qmin, qmax, ampl);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiQuantize *m = m_macroFactory->newRiQuantize(m_renderState->lineNo(), type, one, qmin, qmax, ampl);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiQuantize", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doQuantize(type, one, qmin, qmax, ampl);
	}
}


RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY_CHANNEL) )
		return;

	m_renderState->options().displayChannelV(m_renderState->dict(), channel, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {
		try {
			if ( m_curMacro->valid() ) {
				CRiDisplayChannel *m = m_macroFactory->newRiDisplayChannel(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), channel, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplayChannel", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}
	} else {
		doDisplayChannelV(channel, n, tokens, params);
	}
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_DISPLAY) )
		return;

	m_renderState->options().displayV(m_renderState->dict(), name, type, mode, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiDisplay *m = m_macroFactory->newRiDisplay(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), name, type, mode, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiDisplay", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doDisplayV(name, type, mode, n, tokens, params);
	}
}


RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_HIDER) )
		return;

	m_renderState->options().hiderV(m_renderState->dict(), type, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiHider *m = m_macroFactory->newRiHider(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), type, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiHider", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doHiderV(type, n, tokens, params);
	}
}


RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	if ( !preCheck(REQ_COLOR_SAMPLES) )
		return;

	m_renderState->options().colorSamples(N, nRGB, RGBn);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiColorSamples *m = m_macroFactory->newRiColorSamples(m_renderState->lineNo(), N, nRGB, RGBn);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiColorSamples", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doColorSamples(N, nRGB, RGBn);
	}

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(RIE_RANGE, RIE_WARNING, "Illegal values for the parameters of colorSamples(), using identity mapping.");
	}
}


RtVoid CBaseRenderer::relativeDetail(RtFloat relativedetail)
{
	if ( !preCheck(REQ_RELATIVE_DETAIL) )
		return;

	m_renderState->options().relativeDetail(relativedetail);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiRelativeDetail *m = m_macroFactory->newRiRelativeDetail(m_renderState->lineNo(), relativedetail);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiRelativeDetail", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doRelativeDetail(relativedetail);
	}
}


RtVoid CBaseRenderer::optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( !preCheck(REQ_OPTION) )
		return;

	m_renderState->options().set(m_renderState->dict(), name, n, tokens, params);

	if ( m_macroFactory && m_curMacro ) {

		try {
			if ( m_curMacro->valid() ) {
				CRiOption *m = m_macroFactory->newRiOption(m_renderState->lineNo(), m_renderState->dict(), m_renderState->options().colorSamples(), name, n, tokens, params);
				if ( !m )
					throw (ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "CRiOption", __LINE__, __FILE__));
				m_curMacro->add(m);
			}
		} catch ( ExceptRiCPPError &e2 ) {
			ricppErrHandler().handleError(e2);
		}

	} else {
		doOptionV(name, n, tokens, params);
	}
}
