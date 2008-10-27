// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file rendercontext.cpp
 *  \brief Contains implementations of TRiCPPContext, TIntermediateRenderer and  TIntermediateRendererInterface.
 *         TRiCPPContext a class for all contextinformation used by TIntermediateRenderer.
 *         TIntermediateRenderer is the layer between abstract TRiCPPBinding, own renderers are noramlly subclasses of
 *         TIntermediateRenderer. TIntermediateRenderer calls after a success in verifying the parameters the interface
 *         call a TIntermediateRendererInterface method. If you write your own renderer class you can inherit from
 *         TIntermediateRenderer and TIntermediateRendererInterface, and implement the 'work' in overloaded
 *         TIntermediateRendererInterface methods. Subclasses can also use subclasses of TRiCPPContext,
 *         TRenderAttributes and TRenderObjects.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "rendercontext.h"

#include <string.h>
#include <algorithm>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
//// TRiCPPContext - class for render contexts, used for multiple views
////                 contains all state data of a renderer
////                 Higher level renderer objects will have contexts
////                 inheriting from TiCPPContext. A virtual method
////                 getNewContext returns an instance of the appr.
////                 context.
////


TRiCPPContext::TRiCPPContext() {
	m_name = "";
	m_objectCnt = 0;
	m_replay = 0;
	m_readArchive = 0;
	m_defineObject = 0;
	m_stopRecord = 0;
	m_macroWrite = NULL;
	m_currentOptions = NULL;
	m_currentAttributes = NULL;
	m_currentTransform = NULL;
	m_tSolid = NULL;
	m_frameNumber = -1;
	m_iState = CTX_OUTSIDE;
}


TRiCPPContext::~TRiCPPContext() {
	// cleanup
	if ( m_currentOptions )
		delete m_currentOptions;
	if ( m_currentAttributes )
		delete m_currentAttributes;
	if ( m_currentTransform )
		delete m_currentTransform;
	release_second(m_macros);
	release_second(m_objects);
	release_all(m_options);
	release_all(m_attributes);
}


// Initialize(), used to initialize a context, this is done in RiBegin() or
//               RiSynchronize(begin)
void TRiCPPContext::initialize(TIntermediateRenderer &ri) {
	m_pathStack.clear();
	m_stateStack.clear();
	m_mapCoordSys.clear();

	m_objectCnt = 0;
	m_replay = 0;
	m_readArchive = 0;
	m_defineObject = 0;
	m_stopRecord = 0;

	// release_second(m_objects);	// Objects stays untouched

	// release_second(m_macros);	// Macros stays untouched
	m_macroWrite = NULL;
	m_macroStack.clear();

	// m_declarationMap.releaseDeclarations();
	m_declarationMap.releaseAll();
	m_declarationMap.copyDecls(ri.m_defaultDeclarationMap);

	// Entering a block
	m_iState = CTX_BEGIN;

	// frame number : -1 i.e. outside RiFrameBegin/RiFrameEnd block
	m_frameNumber = -1;

	release_all(m_options);			// reset options
	if ( m_currentOptions )
		delete m_currentOptions;
	m_currentOptions = NULL;
	release_all(m_attributes);		// reset attributes
	if ( m_currentAttributes )
		delete m_currentAttributes;
	m_currentAttributes = NULL;

	if ( m_currentTransform )
		delete m_currentTransform;
	m_currentTransform = NULL;

	// Copies all options issued so far as defaults
	m_currentOptions = ri.getNewOptions();
	if ( !m_currentOptions ) {
		ri.handleError(RIE_NOMEM, RIE_SEVERE, "TRiCPPContext::initialize() - creating options failed");
		return;
	}
	m_currentOptions->initialize();
	// ri.copyDefaultOptions();
	ri.establishOptions();

	// Creating a new initialized attribute set
	m_currentAttributes = ri.getNewAttributes();
	if ( !m_currentAttributes ) {
		ri.handleError(RIE_NOMEM, RIE_SEVERE, "TRiCPPContext::initialize() - creating attributes failed");
		return;
	}
	m_currentAttributes->initialize();
	// ri.copyDefaultAttributes();
	ri.establishAttributes();

	// Creating a new initialized attribute set
	m_currentTransform = ri.getNewTransform();
	if ( !m_currentTransform ) {
		ri.handleError(RIE_NOMEM, RIE_SEVERE, "TRiCPPContext::initialize() - creating transform failed");
		return;
	}
	m_currentTransform->initialize();
	ri.establishTransformation();
}



bool TRiCPPContext::isValid(unsigned int idxRoutine, unsigned int idxContext) {
	return m_contextCheck.isValid(idxRoutine, idxContext);
}

void TRiCPPContext::pushContextState(RtInt stateNum) {
	m_stateStack.push_back(m_iState);
	m_iState = stateNum;
}

bool TRiCPPContext::popContextState(const char *routineName, TIntermediateRenderer &ri) {
	if ( m_stateStack.empty() ) {
		std::string msg = routineName;
		msg += " - context stack is empty";
		ri.handleError(RIE_ILLSTATE, RIE_SEVERE, msg.c_str());
		return false;
	}
	m_iState = m_stateStack.back();
	m_stateStack.pop_back();
	return true;
}

void TRiCPPContext::pushSolidState(RtToken solidType) {
	m_solidStack.push_back(m_tSolid);
	m_tSolid = solidType;
}

bool TRiCPPContext::popSolidState(const char *routineName, TIntermediateRenderer &ri) {
	if ( m_solidStack.empty() ) {
		std::string msg = routineName;
		msg += " - solid stack is empty";
		ri.handleError(RIE_ILLSTATE, RIE_SEVERE, msg.c_str());
		return false;
	}
	m_tSolid = m_solidStack.back();
	m_solidStack.pop_back();
	return true;
}


bool TRiCPPContext::pushTransformation(TIntermediateRenderer &ri) {
	if ( m_currentTransform ) {
		m_currentTransform->pushTransformation();
		return true;
	}
	ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::pushTransformation(), no attribute set to push transformation.");
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////
// popTransformation() - restores the current transformation matrix (notify orientation
//                       changes)
//						 Called by FrameEnd (camera transform), AttributeEnd and
//                       TransformEnd
//
bool TRiCPPContext::popTransformation(TIntermediateRenderer &ri) {
	if ( m_currentTransform == NULL ) {
		ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::popTransformation() - No current attribute set");
		return false;
	}

	// restore the matrix, if the orientation changes, notify it
	bool prevOrientation = sign(m_currentTransform->m_CTM.determinant()) >= 0;
	if ( !m_currentTransform->popTransformation() ) {
		ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::popTransformation() - Transformation stack is empty, but should not.");
		return false;
	}
	ri.restoreOrientation(prevOrientation);

	// Restore dependend values
	ri.establishTransformation();
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
// pushAttributes() - pushes the current attribute and transformation set.
//                    Called by RiAttributeBegin, RiFrameBegin, ... handleObjectBegin
//
bool TRiCPPContext::pushAttributes(TIntermediateRenderer &ri) {
	if ( !pushTransformation(ri) )
		return false;

	assert(m_currentAttributes != NULL);

	m_attributes.push_back(m_currentAttributes);
	m_currentAttributes = ri.getNewAttributes(m_currentAttributes);
	if ( !m_currentAttributes ) {
		ri.handleError(RIE_NOMEM, RIE_SEVERE, "TRiCPPContext::pushAttributes() - failed to get mem for attributes");
		return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
// popAttributes() - retrieves the attribute and transformation set from last block.
//                   Called by RiAttributeEnd, RiFrameEnd, ... handleObjectEnd
//
bool TRiCPPContext::popAttributes(TIntermediateRenderer &ri) {
	if ( m_attributes.empty() ) {
		ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::popAttributes(), attribute stack is empty, but should not.");
		return false;
	}

	TRenderAttributes *old = m_currentAttributes;
	m_currentAttributes = m_attributes.back();
	m_attributes.pop_back();

	assert(old != NULL);
	delete old;

	assert(m_currentAttributes != NULL);

	bool rval = popTransformation(ri);

	// Restore dependend values
	ri.establishAttributes();

	return rval;
}


/////////////////////////////////////////////////////////////////////////////////////////
// pushOptions(): pushes the current option set to the option stack
//
bool TRiCPPContext::pushOptions(TIntermediateRenderer &ri) {
	if ( m_currentOptions == NULL ) {
		ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::pushOptions() - No current option set");
		return false;
	}

	m_options.push_back(m_currentOptions);
	m_currentOptions = ri.getNewOptions(m_currentOptions);
	if ( !m_currentOptions ) {
		ri.handleError(RIE_NOMEM, RIE_SEVERE, "TRiCPPContext::pushOptions() - failed to get mem for options");
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
// popOptions(): pops the current option set to the option stack and restaurates them
//
bool TRiCPPContext::popOptions(TIntermediateRenderer &ri) {
	if ( m_options.empty() ) {
		ri.handleError(RIE_BUG, RIE_SEVERE, "TRiCPPContext::popOptions() - Option stack is empty, but should not.");
		return false;
	}

	TRenderOptions *old = m_currentOptions;
	m_currentOptions = m_options.back();
	assert(m_currentOptions != NULL);
	m_options.pop_back();

	assert(old != NULL);
	delete old;

	ri.establishOptions();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Reminder: Function to load a Renderer from a DLL (systemspecific) defined in
//           DLL's as
// extern "C"__declspec( dllexport ) TRiCPPBinding *GetRenderer();
// and must call initialize() after creation
//

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
//// TIntermediateRenderer - intermediate class for renderers
////

// typical implementation-specific options
RtToken TIntermediateRenderer::ri_Archive = "archive";
RtToken TIntermediateRenderer::ri_Texture = "texture";
RtToken TIntermediateRenderer::ri_Shader = "shader";
RtToken TIntermediateRenderer::ri_Procedural = "procedural";
RtToken TIntermediateRenderer::ri_EndOfFrame = "endofframe";
RtToken TIntermediateRenderer::ri_EyeSplits = "eyesplits";
RtToken TIntermediateRenderer::ri_GridSize = "gridsize";
RtToken TIntermediateRenderer::ri_BucketSize = "bucketsize";

// typical implementation-specific attributes
RtToken TIntermediateRenderer::ri_Sphere = "sphere";
RtToken TIntermediateRenderer::ri_CoordinateSystem = "coordinatesystem";
// RtToken TIntermediateRenderer::ri_Name = "name";
RtToken TIntermediateRenderer::ri_Sense = "sense";

RtToken TIntermediateRenderer::ri_Jitter = "jitter";
RtToken TIntermediateRenderer::ri_Shadows = "shadows";

// control option - used to control the behaviour of RiCPP
RtToken TIntermediateRenderer::ri_NullToken                       = "null";
RtToken TIntermediateRenderer::ri_RiCPPControl			          = "ricpp.control";					// Control option
RtToken TIntermediateRenderer::ri_RiCPPControlCamTrans            = "ricpp.control.camtrans";			// Camera pre transformation matrix (Option m_camTranslate)
RtToken TIntermediateRenderer::ri_RiCPPControlWriteArchive        = "ricpp.control.writearchive";		// writes out content of an archive instead of putting a RiReadArchive (Option m_writeArchive)
RtToken TIntermediateRenderer::ri_RiCPPControlDisableFormat       = "ricpp.control.disableformat";		// disable RiFormat (Option m_disableFormat)
RtToken TIntermediateRenderer::ri_RiCPPControlWriteObject         = "ricpp.control.writeobject";        // writes out content of an object instead of using ObjectInstance

RtToken TIntermediateRenderer::ri_RiCPPCacheArchive   = "ricpp.cachearchive";

// RtToken TIntermediateRenderer::ri_RiCPPControlCamPostTrans     = "ricpp.control.camtrans";			// Camera pre transformation matrix
// RtToken TIntermediateRenderer::ri_RiCPPControlWorldPreTrans    = "ricpp.control.worldpretrans";		// World  pre transformation matrix
// RtToken TIntermediateRenderer::ri_RiCPPControlWorldPostTrans   = "ricpp.control.worldposttrans";	// World  pre transformation matrix
// RtToken TIntermediateRenderer::ri_RiCPPControlExecArchive      = "ricpp.control.execarchive";		// Execute an archive instead of issuing the RIB command
// RtToken TIntermediateRenderer::ri_RiCPPControlExecProcedure    = "ricpp.control.execprocedure";	// Execute a procedure  instead of issuing the RIB command


////////////////////////////////////////////////////////////////////////////////
// TIntermediateRenderer() - Standard constructor of the renderer object. The context
//                    is generated in RiBegin(), not in this constructor other
//                    initialization (that needs calls of virtual functions is
//                    done in initialize())
TIntermediateRenderer::TIntermediateRenderer()
{
	// Call own do-functions
	m_callri = this;

	// default options
	m_defaultOptions = NULL;

	// no current context (created by RiBegin() and may set by RiContext())
	m_currentContext = NULL;
	m_currentContextHandle = RI_NULL;

	defaultDeclarations();
}


////////////////////////////////////////////////////////////////////////////////
// ~TIntermediateRenderer() - Standard destructor, removes all contexts and does other
//                     clean up tasks
TIntermediateRenderer::~TIntermediateRenderer() {
	// deletes the default options
	if ( m_defaultOptions )
		delete m_defaultOptions;
	m_defaultOptions = NULL;

	// Deletes all contexts
	deleteAllContexts();
}


/*
////////////////////////////////////////////////////////////////////////////////
// initialize() - Standard initialization, must be called after creating an
//                object. Inits the default options and declarations
void TIntermediateRenderer::initialize() {
	if ( m_defaultOptions )
		delete m_defaultOptions;
	m_defaultOptions = NULL;
	m_defaultOptions = getNewOptions();
	if ( m_defaultOptions )
		m_defaultOptions->initialize();
}
*/

// get the current declarationmap
TDeclarationMap &TIntermediateRenderer::getDeclMap() {
	return ( m_currentContext != NULL ) ?
		m_currentContext->m_declarationMap :
		m_defaultDeclarationMap;
}

// Returns a new transform set can be overloaded by other
// renderers to include special matrices
TRenderTransform *TIntermediateRenderer::getNewTransform() {
	return new TRenderTransform;
}

// Returns a new transformation matrix set can be overloaded by other
// renderers to include special matrices
TRenderTransform *TIntermediateRenderer::getNewTransform(const TRenderTransform *t) {
	if ( t )
		return new TRenderTransform(*t);
	else
		return getNewTransform();
}

// Returns a new attribute set can be overloaded by other
// renderers to include special attributes
TRenderAttributes *TIntermediateRenderer::getNewAttributes() {
	return new TRenderAttributes(options().m_iNColor);
}

// Returns a new attribute set can be overloaded by other
// renderers to include special attributes
TRenderAttributes *TIntermediateRenderer::getNewAttributes(const TRenderAttributes *attr) {
	if ( attr )
		return new TRenderAttributes(*attr);
	else
		return getNewAttributes();
}

// Returns a new option set can be overloaded by other
// renderers to include special options
TRenderOptions *TIntermediateRenderer::getNewOptions() {
	return new TRenderOptions;
}

TRenderOptions *TIntermediateRenderer::getNewOptions(const TRenderOptions *o) {
	if ( o )
		return new TRenderOptions(*o);
	else
		return getNewOptions();
}

// Copies default attributes from the renderer to the renderer context
// void TIntermediateRenderer::copyDefaultAttributes() {
// }


// Copies default options from the renderer to the renderer context
// void TIntermediateRenderer::copyDefaultOptions() {
// }

// Get a new context, maybe overloaded to return special contexts
// called in RiBegin()
TRiCPPContext *TIntermediateRenderer::getNewContext() {
	return new TRiCPPContext;
}

// Insert a context and set as active context,
// called from RiBegin
bool TIntermediateRenderer::insertContext(TRiCPPContext *c) {
	if ( !c )
		return false;

	m_contextList.push_back(c);
	m_currentContext = c;
	m_currentContextHandle = (RtContextHandle)m_contextList.size();
	return true;
}

// deletes the current context, it is called from RiEnd()
// there is no current context anymore after this function is called
void TIntermediateRenderer::deleteCurrentContext() {
	// Remove the declarations
	m_defaultDeclarationMap.releaseAll();
	defaultDeclarations();

	if ( m_currentContext ) {
		std::vector<TRiCPPContext *>::size_type idx;
		for ( idx = 0; idx < m_contextList.size(); ++idx ) {
			if ( m_currentContext == m_contextList[idx] ) {
				m_contextList[idx] = NULL;
			}
		}
		
		delete m_currentContext;
		
        // Compacting the list be removing already deleted contexts
		// from the end
        if ( m_contextList.size() > 0 ) {
            for ( idx = m_contextList.size()-1; m_contextList[idx] == NULL; --idx) {
                m_contextList.pop_back();
                if ( idx == 0 ) {
                    m_contextList.clear();
                    break;
                }
            }
		}
	}
	m_currentContext = NULL;
	m_currentContextHandle = RI_NULL;
}

// deleteAllContexts(): cleans up
void TIntermediateRenderer::deleteAllContexts() {
    m_currentContext = NULL;
	m_currentContextHandle = RI_NULL;
    release_all(m_contextList);
}


// Check the validity of the current pointers
bool TIntermediateRenderer::isValid(const char *message) {
	std::string msg = message ? message : "";
	if ( message && message[0] )
		msg += " - ";

	// is there a valid contexthandle?
	if ( !m_currentContext ) {
		msg += "Current context is missing.";
		handleError(RIE_BADHANDLE, RIE_SEVERE, msg.c_str());
		return false;
	}

	// valid options?
	if ( !(m_currentContext->m_currentOptions) ) {
		std::string msg = message;
		msg += "Current option set is missing.";
		handleError(RIE_BUG, RIE_SEVERE, msg.c_str());
		return false;
	}

	// valid attributes?
	if ( !(m_currentContext->m_currentAttributes) ) {
		std::string msg = message;
		msg += "Current attribute set is missing.";
		handleError(RIE_BUG, RIE_SEVERE, msg.c_str());
		return false;
	}

	return true;
}

/*
// Check the validity of the current context and interface routine without message
bool TIntermediateRenderer::isValid(unsigned int idxRoutine) {
	// is there a valid contexthandle?
	if ( !isValid() ) {
		return false;
	}

	// is the interface routine called in a valid block context?
	if ( !m_currentContext->isValid(idxRoutine, m_currentContext->m_iState) ) {
		return false;
	}

	// OK
	return true;
}
*/

// Check the validity of the current context and interface routine
bool TIntermediateRenderer::isValid(unsigned int idxRoutine, const char *message) {
	if ( !isValid(message) ) {
		return false;
	}

	// is the interface routine called in a valid block context?
	if ( !m_currentContext->isValid(idxRoutine, m_currentContext->m_iState) ) {
		handleError(RIE_NESTING, RIE_SEVERE, message);
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////
// getFilterTok() - returns textual representation of a filter functions
//
// Parameters:
// filterfunc: filter function defined in TRiCPPBinding
//
RtToken TIntermediateRenderer::getFilterTok(RtFilterFunc filterfunc) {
	if ( filterfunc == boxFilter )
		return "box";
	if ( filterfunc == catmullRomFilter )
		return "catmull-rom";
	if ( filterfunc == gaussianFilter )
		return "gaussian";
	if ( filterfunc == sincFilter )
		return "sinc";
	if ( filterfunc == triangleFilter )
		return "triangle";

	return ri_NullToken; // unknown filter function
}


////////////////////////////////////////////////////////////////////////////////
// getSubdivTok() - returns textual representation of a procedural
//
// Parameters:
// subdivfunc: procedural subdivision function defined in TRiCPPBinding
//
RtToken TIntermediateRenderer::getSubdivTok(RtProcSubdivFunc subdivfunc) {
	if ( subdivfunc == procDelayedReadArchive )
		return "DelayedReadArchive";
	if ( subdivfunc == procRunProgram )
		return "RunProgram";
	if ( subdivfunc == procDynamicLoad )
		return "DynamicLoad";

	return ri_NullToken; // unknown procedural
}


////////////////////////////////////////////////////////////////////////////////
// declareGlobal(): Declares a global type in m_defaultDeclarationMap
RtToken TIntermediateRenderer::declareGlobal(const char *name, const char *declaration) {
	RtToken t = RI_NULL;
	try {
		// may throw: RIE_NOMEM, RIE_SYNTAX
		t = m_defaultDeclarationMap.declare(name, declaration, true);
	} catch (TRenderException &e) {
		t = RI_NULL;
		handleError(e.code(), e.severity(), e.what());
	}
	return t;
}


////////////////////////////////////////////////////////////////////////////////
// defaultDeclarations() - default token declarations, get the current
//                         declaration map and insert the default declarations.
//                         If there is a current context, copy the declarations
//                         of the renderer object as default declarations first.
//
void TIntermediateRenderer::defaultDeclarations()
{
	// Insert default declarations
	try {
		declareGlobal(ri_NullToken, RI_NULL);	// Hider, identity projection

		declareGlobal(RI_FRAMEBUFFER, RI_NULL);
		declareGlobal(RI_FILE, RI_NULL);

		declareGlobal(RI_RGB, RI_NULL);
		declareGlobal(RI_RGBA, RI_NULL);
		declareGlobal(RI_RGBZ, RI_NULL);
		declareGlobal(RI_RGBAZ, RI_NULL);
		declareGlobal(RI_A, RI_NULL);
		declareGlobal(RI_Z, RI_NULL);
		declareGlobal(RI_AZ, RI_NULL);

		declareGlobal(RI_PERSPECTIVE, RI_NULL);		// perspective projection
		declareGlobal(RI_ORTHOGRAPHIC, RI_NULL);	// orthographic projection

		declareGlobal(RI_HIDDEN, RI_NULL);			// Hider "hidden"
		declareGlobal(RI_PAINT, RI_NULL);			// Hider "paint"

		declareGlobal(RI_CONSTANT, RI_NULL);
		declareGlobal(RI_SMOOTH, RI_NULL);

		declareGlobal(RI_FLATNESS, "float");
		declareGlobal(RI_FOV, "float");

		declareGlobal(RI_AMBIENTLIGHT, RI_NULL);
		declareGlobal(RI_POINTLIGHT, RI_NULL);
		declareGlobal(RI_DISTANTLIGHT, RI_NULL);
		declareGlobal(RI_SPOTLIGHT, RI_NULL);
		declareGlobal(RI_INTENSITY, "float");
		declareGlobal(RI_LIGHTCOLOR, "color");
		declareGlobal(RI_FROM, "point");
		declareGlobal(RI_TO, "point");
		declareGlobal(RI_CONEANGLE, "float");
		declareGlobal(RI_CONEDELTAANGLE, "float");
		declareGlobal(RI_BEAMDISTRIBUTION, "float");

		declareGlobal(RI_MATTE, RI_NULL);
		declareGlobal(RI_METAL, RI_NULL);
		declareGlobal(RI_SHINYMETAL, RI_NULL);
		declareGlobal(RI_PLASTIC, RI_NULL);
		declareGlobal(RI_PAINTEDPLASTIC, RI_NULL);
		declareGlobal(RI_KA, "float");
		declareGlobal(RI_KD, "float");
		declareGlobal(RI_KS, "float");
		declareGlobal(RI_ROUGHNESS, "float");
		declareGlobal(RI_KR, "float");
		declareGlobal(RI_TEXTURENAME, "string");
		declareGlobal(RI_SPECULARCOLOR, "color");
		declareGlobal(RI_DEPTHCUE, RI_NULL);
		declareGlobal(RI_FOG, RI_NULL);
		declareGlobal(RI_BUMPY, RI_NULL);
		declareGlobal(RI_MINDISTANCE, "float");
		declareGlobal(RI_MAXDISTANCE, "float");
		declareGlobal(RI_BACKGROUND, "color");
		declareGlobal(RI_DISTANCE, "float");
		declareGlobal(RI_AMPLITUDE, "float");

		declareGlobal(RI_INSIDE, RI_NULL);
		declareGlobal(RI_OUTSIDE, RI_NULL);
		declareGlobal(RI_LH, RI_NULL);
		declareGlobal(RI_RH, RI_NULL);

		declareGlobal(RI_P,  "vertex point");   // Position
		declareGlobal(RI_PZ, "vertex float");
		declareGlobal(RI_PW, "vertex hpoint");
		declareGlobal(RI_N,  "varying point");  // Normal
		declareGlobal(RI_NP, "uniform point");
		declareGlobal(RI_CS, "varying color");  // Color
		declareGlobal(RI_OS, "varying color");  // Opacity
		declareGlobal(RI_S,  "varying float");  // Texture coordinates
		declareGlobal(RI_T,  "varying float");
		declareGlobal(RI_ST, "varying float[2]");

		declareGlobal(RI_BILINEAR, RI_NULL);
		declareGlobal(RI_BICUBIC, RI_NULL);
		declareGlobal(RI_PRIMITIVE, RI_NULL);
		declareGlobal(RI_INTERSECTION, RI_NULL);
		declareGlobal(RI_UNION, RI_NULL);
		declareGlobal(RI_DIFFERENCE, RI_NULL);
		declareGlobal(RI_PERIODIC, RI_NULL);
		declareGlobal(RI_NONPERIODIC, RI_NULL);
		declareGlobal(RI_CLAMP, RI_NULL);
		declareGlobal(RI_BLACK, RI_NULL);

		declareGlobal(RI_IGNORE, RI_NULL);
		declareGlobal(RI_PRINT, RI_NULL);
		declareGlobal(RI_ABORT, RI_NULL);
		declareGlobal(RI_HANDLER, RI_NULL);

		declareGlobal(RI_ORIGIN, "constant integer[2]");   // Origin of the display

		declareGlobal(RI_IDENTIFIER, RI_NULL);
		declareGlobal(RI_NAME, "constant string");
		declareGlobal(RI_COMMENT, RI_NULL);
		declareGlobal(RI_STRUCTURE, RI_NULL);
		declareGlobal(RI_LINEAR, RI_NULL);
		declareGlobal(RI_CUBIC, RI_NULL);
		declareGlobal(RI_WIDTH, "varying float");
		declareGlobal(RI_CONSTANTWIDTH, "float");

		declareGlobal(RI_CURRENT, RI_NULL);
		declareGlobal(RI_WORLD, RI_NULL);
		declareGlobal(RI_OBJECT, RI_NULL);
		declareGlobal(RI_SHADER, RI_NULL);
		declareGlobal(RI_RASTER, RI_NULL);
		declareGlobal(RI_NDC, RI_NULL);
		declareGlobal(RI_SCREEN, RI_NULL);
		declareGlobal(RI_CAMERA, RI_NULL);
		declareGlobal(RI_EYE, RI_NULL);

		declareGlobal(RI_CATMULLCLARK, RI_NULL);
		declareGlobal(RI_HOLE, RI_NULL);
		declareGlobal(RI_CREASE, RI_NULL);
		declareGlobal(RI_CORNER, RI_NULL);
		declareGlobal(RI_INTERPOLATEBOUNDARY, RI_NULL);

		// common options and attributes

		declareGlobal(ri_Archive, "constant string");
		declareGlobal(ri_Texture, "constant string");
		declareGlobal(ri_Shader, "constant string");
		declareGlobal(ri_Procedural, "constant string");
		declareGlobal(ri_EndOfFrame, "constant integer");
		declareGlobal(ri_EyeSplits, "constant integer");
		declareGlobal(ri_GridSize, "constant integer");
		declareGlobal(ri_BucketSize, "constant integer[2]");

		declareGlobal(ri_Sphere, "constant float");
		declareGlobal(ri_CoordinateSystem, "constant string");
		// declareGlobal(ri_Name, "constant string");
		declareGlobal(ri_Sense, "constant string");

		declareGlobal(ri_Jitter, "constant integer");
		declareGlobal(ri_Shadows, "constant string");

		// TIntermediadeRenderer specific options
		declareGlobal(ri_RiCPPCacheArchive, "constant boolean");    // Attribute cachearchive (RiReadArchive)

		declareGlobal(ri_RiCPPControl, RI_NULL);                           // Option controls Renderer
		declareGlobal(ri_RiCPPControlCamTrans, "constant matrix");      // Option pretransformation for camera
		declareGlobal(ri_RiCPPControlWriteArchive, "constant boolean");    // Option handling of RiReadArchive
		declareGlobal(ri_RiCPPControlDisableFormat, "constant boolean");   // Disable RiFormat
		declareGlobal(ri_RiCPPControlWriteObject, "constant boolean");     // Option handling of RiObjectInstance
	} catch (TRenderException &e) {
		handleError(e.code(), e.severity(), e.what());
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// notifyOrientationChanged() - notification that the orientation (left handed,
//                              right handed coord sys als well as CCW, CW winding for
//                              calculating normals) changes
//
RtVoid TIntermediateRenderer::notifyOrientationChanged() {
}


/////////////////////////////////////////////////////////////////////////////////////////
// swapOrientation() - called if the orientation changed (determinant of CTM changes 
//                     sign, calls virtual notifyOrientationChanged() to notify
//                     descendants (child classes)
//
RtVoid TIntermediateRenderer::swapOrientation() {
	assert( (m_currentContext != NULL) && (m_currentContext->m_currentAttributes != NULL));

	attributes().m_currentOrientation = !attributes().m_currentOrientation;
	notifyOrientationChanged();
}


/////////////////////////////////////////////////////////////////////////////////////////
// pushOptions(): pushes the current option set to the option stack
//
// void TIntermediateRenderer::pushOptions() {
// 	assert(m_currentContext != NULL);
// 	m_currentContext->pushOptions(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// popOptions(): pops the current option set to the option stack and restaurates them
//
// void TIntermediateRenderer::popOptions() {
// 	assert(m_currentContext != NULL);
// 	m_currentContext->popOptions(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// establishOptions(): make some changes if there is a new option set
//
void TIntermediateRenderer::establishOptions() {
}


/////////////////////////////////////////////////////////////////////////////////////////
// pushAttributes() - pushes the current attribute and transformation set.
//                    Called by RiAttributeBegin
//
// void TIntermediateRenderer::pushAttributes() {
// 	assert(m_currentContext != NULL);
// 	m_currentContext->pushAttributes(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// popAttributes() - retrieves the attribute and transformation set from last block.
//                   Called by RiAttributeEnd
//
// void TIntermediateRenderer::popAttributes() {
// 	assert(m_currentContext != NULL);
// 	m_currentContext->popAttributes(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// establishAttributes() - called by attribute changes
//
void TIntermediateRenderer::establishAttributes() {
	// new attributes, so look after the state of the lights
	resetLights();
	notifyOrientationChanged();
}


/////////////////////////////////////////////////////////////////////////////////////////
// pushTransformation() - save the current transformation matrix.
//						  Called by FrameBegin (camera transform), AttributeBegin and
//                        TransformBegin
// void TIntermediateRenderer::pushTransformation() {
// 	assert(m_currentContext != NULL && m_currentContext->m_currentAttributes != NULL);
// 	m_currentContext->pushTransformation(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// popTransformation() - restores the current transformation matrix (notify orientation
//                       changes)
//						 Called by FrameEnd (camera transform), AttributeEnd and
//                       TransformEnd
//
// void TIntermediateRenderer::popTransformation() {
// 	assert(m_currentContext != NULL && m_currentContext->m_currentAttributes != NULL);
// 	m_currentContext->popTransformation(*this);
// }


/////////////////////////////////////////////////////////////////////////////////////////
// Got new Transformation matrix from stack
 void TIntermediateRenderer::establishTransformation() {
	// Nothing is done
}

 //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//// interface functions


/////////////////////////////////////////////////////////////////////////////////////////
// RiDeclare - (s.a. [Ups89] p.242f Extensing the set of variables
//                   [RIB32] p.13f Relationship to the RenderMan Shading Language)
//
//             Makes a new variable known to the interface for use in the parameterlist.
//             The variable is declared by name and type (declaration).
//             declarition := ´RI_NULL´ | [class] [type] [´[´n´]´]
//             class       := ´constant´, ´uniform´, ´varying´, ´vertex´
//             type        := ´float´, ´integer´, ´string´, ´color´, ´point´, ´vector´,
//                            ´normal´, ´matrix´, ´hpoint´,
//                            ´mpoint´, ´basis´, ´bound´
//             n           := positive integer
//             If class is not specified, ´uniform´ is assumed
//
// Parameters:
// name:        Name of the new variable
// declaration: Declaration of the new variable
//
// Returns the pointer to the token of inserted declaration
//
RtToken TIntermediateRenderer::declare(const char *name, const char *declaration) {
	// declares instantly, need no active context

	// Macro?
	if ( isRecording() ) {
		TRiDeclare *decl = newRiDeclare(lineNo(), name, declaration);
		if ( !decl ) {
			handleError(RIE_NOMEM, RIE_ERROR, "in declare \"%s\" \"%s\"", name ? name : "", declaration ? declaration : "");
			return RI_NULL;
		}
		m_currentContext->m_macroWrite->add(decl);
	}


	// Define the token (even in object definitions)

	RtToken t = RI_NULL;
	try {
		// may throw: RIE_NOMEM, RIE_SYNTAX
		t = getDeclMap().declare(name, declaration, false);
	} catch (TRenderException &e) {
		t = RI_NULL;
		handleError(e.code(), e.severity(), e.what());
	}

	// if ( isDefiningObject() )
	//	return t;

	// Call the doDeclare, normally returns t itself, there is not neccessarily a m_currentContext
	t = m_callri->doDeclare(t, name, declaration);
	return t;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//// Transforms the CTM CTM(), calls swapOrientation if
//// orientation of the transformation matrix changes


//////////////////////////////////////////////////////////////////////////////
// identity() - loads the identity matrix to CTM
//
RtVoid TIntermediateRenderer::identity(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiIdentity(lineNo()));
	}

	if ( !isValid(IDX_IDENTITY, "TIntermediateRenderer::identity()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().identity();
	inverseCTM().identity();
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}

	m_callri->doIdentity();
}


//////////////////////////////////////////////////////////////////////////////
// transform() - loads the matrix transform to CTM
//
// Parameters:
// transform: matrix gets new CTM
//
RtVoid TIntermediateRenderer::transform(RtMatrix trans) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiTransform(lineNo(), trans));
	}

	if ( !isValid(IDX_TRANSFORM, "TIntermediateRenderer::transform()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().set(trans);
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}

	RtMatrix inverse;
	if ( CTM().getInverse(inverse) ) {
		inverseCTM().set(inverse);
	} else {
		// handleError(RIE_MATH, RIE_WARNING, "in transform, inverse matrix not found");
	}

	m_callri->doTransform(trans);
}


//////////////////////////////////////////////////////////////////////////////
// concatTransform() - concats the transform matrix to CTM
//
// Parameters:
// transform: matrix to left multiply with CTM
//
RtVoid TIntermediateRenderer::concatTransform(RtMatrix trans) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiConcatTransform(lineNo(), trans));
	}

	if ( !isValid(IDX_CONCAT_TRANSFORM, "TIntermediateRenderer::concatTransform()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().concatTransform(trans);
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}

	TMatrix3D mat(trans);
	RtMatrix inverse;
	if ( mat.getInverse(inverse) ) {
		inverseCTM().concatTransform(inverse);
	} else {
		// handleError(RIE_MATH, RIE_WARNING, "in concatTransform, inverse matrix not found");
	}

	m_callri->doConcatTransform(trans);
}


//////////////////////////////////////////////////////////////////////////////
// perspective() - concats the perspective matrix with field of view fov
//                 to CTM
//
// Parameters:
// fov: field of view to calculate the perspective matrix
//
RtVoid TIntermediateRenderer::perspective(RtFloat fov) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiPerspective(lineNo(), fov));
	}

	if ( !isValid(IDX_PERSPECTIVE, "TIntermediateRenderer::perspective()") )
		return;
		
	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().perspectiveProject(fov);
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}

	TMatrix3D mat;
	mat.perspectiveProject(fov);
	RtMatrix inverse;
	if ( mat.getInverse(inverse) ) {
		inverseCTM().concatTransform(inverse);
	} else {
		// handleError(RIE_MATH, RIE_WARNING, "in concatTransform, inverse matrix not found");
	}

	m_callri->doPerspective(fov);
}


//////////////////////////////////////////////////////////////////////////////
// translate() - translation to the CTM
//
// Parameters:
// dx: translation to the x - direction
// dy: translation to the y - direction
// dz: translation to the z - direction
//
RtVoid TIntermediateRenderer::translate(RtFloat dx, RtFloat dy, RtFloat dz) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiTranslate(lineNo(), dx, dy, dz));
	}

	if ( !isValid(IDX_TRANSLATE, "TIntermediateRenderer::translate()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	// translate cannot change orientation
//	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().translate(dx, dy, dz);
//	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
//		swapOrientation();
//	}
	inverseCTM().translate(-dx, -dy, -dz);

	m_callri->doTranslate(dx, dy, dz);
}


//////////////////////////////////////////////////////////////////////////////
// rotate() - rotation about dx, dy, dz rotation axis
//
// Parameters:
// angle - rotation
// dx: x component of rotation axis
// dy: y component of rotation axis
// dz: z component of rotation axis
//
RtVoid TIntermediateRenderer::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiRotate(lineNo(), angle, dx, dy, dz));
	}

	if ( !isValid(IDX_ROTATE, "TIntermediateRenderer::rotate()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	// cannot change orientation
//	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().rotate(angle, dx, dy, dz);
//	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
//		swapOrientation();
//	}
	inverseCTM().rotate(-angle, dx, dy, dz);

	m_callri->doRotate(angle, dx, dy, dz);
}


//////////////////////////////////////////////////////////////////////////////
// scale() - scales by sx, sy, sz in the axis directions
//
// Parameters:
// sx: scales in x direction
// sy: scales in y direction
// sz: scales in z direction
//
RtVoid TIntermediateRenderer::scale(RtFloat sx, RtFloat sy, RtFloat sz) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiScale(lineNo(), sx, sy, sz));
	}

	if ( !isValid(IDX_SCALE, "TIntermediateRenderer::scale()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().scale(sx, sy, sz);
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}
	if ( sx != 0.0 && sy != 0.0 && sz != 0.0 ) {
		inverseCTM().scale((RtFloat)(1.0/sx), (RtFloat)(1.0/-sy), (RtFloat)(1.0/-sz));
	} else {
		// handleError(RIE_MATH, RIE_WARNING, "in scale, inverse matrix not found");
	}

	m_callri->doScale(sx, sy, sz);
}


//////////////////////////////////////////////////////////////////////////////
// skew() - a skew by angle from (dx1, dy1, dz1) in direction (dx2, dy2, dz2)
//
// Parameters:
// angle: skewing angle
// dx1:   axis vector for skewing
// dy1:   " 
// dz1:   "
// dx2:   direction vector
// dy2:   "
// dz2:   "
//
RtVoid TIntermediateRenderer::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiSkew(lineNo(), angle, dx1, dy1, dz1, dx2, dy2, dz2));
	}

	if ( !isValid(IDX_SKEW, "TIntermediateRenderer::skew()") )
		return;

	// if ( isDefiningObject() )
	//	return;

	assert(m_currentContext->m_currentTransform != NULL);

	// Cannot change orientation
//	bool sdet = sign(CTM().determinant()) >= 0;
	CTM().skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
//	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
//		swapOrientation();
//	}
	inverseCTM().skew(-angle, dx1, dy1, dz1, dx2, dy2, dz2);

	m_callri->doSkew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//// Blockstructure


//////////////////////////////////////////////////////////////////////////////
// render block (begin/end, contexts and synchronize)

//////////////////////////////////////////////////////////////////////////////
// begin() - starts and initializes a new render context, name is the
//           name of the context (evtl. filename of a RIB file)
RtVoid TIntermediateRenderer::begin(RtToken name) {
	// No macro call existing - this routine is always executed directly

	// RiBegin can be called from everywhere, and no active
	// context is needed, so no checking is done!!!
	// if ( !isValid(IDX_BEGIN, "TIntermediateRenderer::begin()") )
	//	return;

	// Get a new context
	if ( !insertContext(getNewContext()) ) {
		m_currentContext = NULL;
		m_currentContextHandle = RI_NULL;
		handleError(RIE_NOMEM, RIE_SEVERE, "TIntermediateRenderer::begin() - creating context failed");
		return;
	}

	assert(m_currentContext != NULL);
	if ( m_currentContext ) {
		m_currentContext->initialize(*this);
		m_currentContext->m_name = name ? name : "";
		m_currentContext->m_declarationMap.copyDecls(m_defaultDeclarationMap);

		m_callri->doBegin(name);
	}
}


//////////////////////////////////////////////////////////////////////////////
// end() - removes the current context an does some clean up
//
RtVoid TIntermediateRenderer::end(void) {
	// No macro call existing - this routine is always executed directly

	if ( !isValid(IDX_END, "TIntermediateRenderer::end()") ) {
		return;
	}

	if ( !m_currentContext->m_stateStack.empty() ) {
		handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::end() - context stack is not empty, called end with other blocks open");
		return;
	}

	// Call of doEnd() before the valid context is deleted
	m_callri->doEnd();

	// Remove the context
	deleteCurrentContext();

	// There is no valid context any more, until RiContext() or RiBegin() is called
}


//////////////////////////////////////////////////////////////////////////////
// context() - returns the current context handle maybe RI_NULL, if there is
//             no active one
//
RtContextHandle TIntermediateRenderer::getContext(void) { 
	// returns the current context handle ( 0 is NULL is RI_NULL)
	return m_callri->doGetContext(m_currentContextHandle);
}


//////////////////////////////////////////////////////////////////////////////
// context() - sets handle as current context
//
void TIntermediateRenderer::context(RtContextHandle handle) {
	if ( handle == RI_NULL ) {
		// Removes the context
		m_currentContext = NULL;
		m_currentContextHandle = RI_NULL;
	} else {
		// search the corresponding pointer to a context
		std::vector<TRiCPPContext *>::size_type idx = (std::vector<TRiCPPContext *>::size_type)handle - 1;
		if ( idx < m_contextList.size() )  {
			TRiCPPContext *ctx = m_contextList[idx];
			if ( ctx ) {
				m_currentContext = ctx;
				m_currentContextHandle = handle;
			} else {
				// Error illegal handle (already destroyd)
				handleError(RIE_BADHANDLE, RIE_ERROR, "TIntermediateRenderer::context() - context handle not found");
				m_currentContext = NULL;
				m_currentContextHandle = RI_NULL;
				return;
			}
		} else {
			// Error illegal handle
			handleError(RIE_BADHANDLE, RIE_ERROR, "TIntermediateRenderer::context() - context handle not valid");
			return;
		}
	}
	m_callri->doContext(handle);
}


//////////////////////////////////////////////////////////////////////////////
// synchronize() - gives some additional control over the state - have
//                 seen this routine in "Advanced Renderman", maybe used
//                 not in the right way here, but is needfull...
RtVoid TIntermediateRenderer::synchronize(RtToken name) {
	// No macro exists, only CPP-binding

	// Call first before anything is destroyed
	m_callri->doSynchronize(name);

	if ( name && !strcmp(name, RI_ABORT) ) {
		// can be used after a fatal error accured to reset the state
		// or for cleanup and reuse the state
		deleteCurrentContext();
	} else if ( name && !strcmp(name, RI_WORLD) ) {
		// can be used to load rib entities - not tried yet
		if ( m_currentContext ) {
			// m_currentContext->pushContextState(CTX_WORLD);
			m_currentContext->m_iState = CTX_WORLD;
			// m_currentContext->pushAttributes(*this);
			attributes().m_currCoordSysName = RI_WORLD;
			attributes().m_colorComponents = options().m_iNColor;
			attributes().initialize();
		} else {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::synchronize(\"%s\")", RI_WORLD);
			return;
		}
	} else if ( name && !strcmp(name, "reset") ) {
		// reset the state, but don't clean cached data
		if ( m_currentContext ) {
			m_currentContext->initialize(*this);
		} else {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::synchronize(\"reset\")");
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// frame block

//////////////////////////////////////////////////////////////////////////////
// frameBegin() Starts a new frame number - number has no effect here
RtVoid TIntermediateRenderer::frameBegin(RtInt number) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiFrameBegin(lineNo(), number));
	}

	if ( !isValid(IDX_FRAME_BEGIN, "TIntermediateRenderer::frameBegin()") )
		return;

	m_currentContext->pushContextState(CTX_FRAME);

	// if ( isDefiningObject() )
	//	return;

	m_currentContext->pushAttributes(*this);
	m_currentContext->pushOptions(*this);

	m_callri->doFrameBegin(number);
}

//////////////////////////////////////////////////////////////////////////////
// frameEnd()
RtVoid TIntermediateRenderer::frameEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiFrameEnd(lineNo()));
	}

	if ( !isValid(IDX_FRAME_END, "TIntermediateRenderer::frameEnd") )
		return;

	if ( !m_currentContext->popContextState("TIntermediateRenderer::frameEnd()", *this) )
		return;

	// if ( isDefiningObject() )
	//	return;

	m_currentContext->popOptions(*this);
	m_currentContext->popAttributes(*this);

	m_currentContext->m_frameNumber = -1;
	attributes().m_currCoordSysName = RI_SCREEN;

	m_callri->doFrameEnd();
}


//////////////////////////////////////////////////////////////////////////
// world block


//////////////////////////////////////////////////////////////////////////
// worldBegin()
RtVoid TIntermediateRenderer::worldBegin(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiWorldBegin(lineNo()));
	}

	if ( !isValid(IDX_WORLD_BEGIN, "TIntermediateRenderer::worldBegin()") )
		return;

	m_currentContext->pushContextState(CTX_WORLD);

	// if ( isDefiningObject() )
	//	return;

	m_currentContext->pushAttributes(*this);

	assert(m_currentContext->m_currentTransform != NULL);
	assert(m_currentContext->m_currentAttributes != NULL);
	assert(m_currentContext->m_currentOptions != NULL);

	// Initialize with default attributes - inherited from outer block
	// attributes().initialize(
	//	options().m_iNColor
	// );

	if ( !options().m_bProjectionCalled ) {
		establishProjection();
	}

	// Multiply m_mCamTrans
	CTM().concatTransform(options().m_mCamTrans);
	m_currentContext->m_mapCoordSys[RI_CAMERA] = CTM();

	bool sdet = sign(CTM().determinant()) >= 0;
	attributes().m_currCoordSysName = RI_WORLD;
	CTM().identity();
	if ( (sign(CTM().determinant()) >= 0) != sdet ) {
		swapOrientation();
	}

	m_callri->doWorldBegin();
}


//////////////////////////////////////////////////////////////////////////
// worldEnd()
RtVoid TIntermediateRenderer::worldEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiWorldEnd(lineNo()));
	}

	if ( !isValid(IDX_WORLD_END, "TIntermediateRenderer::worldEnd()") )
		return;

	if ( !m_currentContext->popContextState("TIntermediateRenderer::worldEnd()", *this) )
		return;

	// if ( isDefiningObject() )
	//	return;

	// get back the attributes
	m_currentContext->popAttributes(*this);

	attributes().m_currCoordSysName = RI_SCREEN; // ????

	m_callri->doWorldEnd();
}


//////////////////////////////////////////////////////////////////////////
// attribute block


//////////////////////////////////////////////////////////////////////////
// attributeBegin()
RtVoid TIntermediateRenderer::attributeBegin(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiAttributeBegin(lineNo()));
	}

	if ( !isValid(IDX_ATTRIBUTE_BEGIN, "TIntermediateRenderer::attributeBegin()") )
		return;

	m_currentContext->pushContextState(CTX_ATTRIBUTE);

	// if ( isDefiningObject() )
	//	return;

	m_currentContext->pushAttributes(*this);

	m_callri->doAttributeBegin();
}


//////////////////////////////////////////////////////////////////////////
// attributeEnd()
RtVoid TIntermediateRenderer::attributeEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiAttributeEnd(lineNo()));
	}

	if ( !isValid(IDX_ATTRIBUTE_END, "TIntermediateRenderer::attributeEnd()") )
		return;

	if ( !m_currentContext->popContextState("TIntermediateRenderer::attributeEnd()", *this) )
		return;

	// if ( isDefiningObject() )
	//	return;

	// get back the attributes
	m_currentContext->popAttributes(*this);

	m_callri->doAttributeEnd();
}


//////////////////////////////////////////////////////////////////////////
// transformation block


/////////////////////////////////////////////////////////////////////////////////////////
// restoreOrientation
void TIntermediateRenderer::restoreOrientation(bool prevOrientation) {
	assert(m_currentContext->m_currentTransform != NULL);
	assert(m_currentContext->m_currentAttributes != NULL);
	assert(m_currentContext->m_currentOptions != NULL);

	bool coordSysOrientation = sign(CTM().determinant()) >= 0;

	if ( attributes().m_orientation == RI_OUTSIDE ) {
		attributes().m_currentOrientation = coordSysOrientation;
	} else if ( attributes().m_orientation == RI_INSIDE ) {
		attributes().m_currentOrientation = !coordSysOrientation;
	} else if ( attributes().m_orientation == RI_LH ) {
		attributes().m_currentOrientation = true;
	} else if ( attributes().m_orientation == RI_RH ) {
		attributes().m_currentOrientation = false;
	} else {
		handleError(RIE_BUG, RIE_ERROR, "restoreOrientation: orientation \"%s\" unknown token, must be either RI_RH, RI_LH, RI_INSIDE or RI_OUTSIDE", attributes().m_orientation, RI_NULL);
	}

	if ( prevOrientation != attributes().m_currentOrientation )
		notifyOrientationChanged();
}

//////////////////////////////////////////////////////////////////////////
// transformBegin()
RtVoid TIntermediateRenderer::transformBegin(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiTransformBegin(lineNo()));
	}

	if ( !isValid(IDX_TRANSFORM_BEGIN, "TIntermediateRenderer::transformBegin()") )
		return;

	m_currentContext->pushContextState(CTX_TRANSFORM);

	// if ( isDefiningObject() )
	// 	return;

	// save the transformation
	m_currentContext->pushTransformation(*this);

	m_callri->doTransformBegin();
}


//////////////////////////////////////////////////////////////////////////
// transformEnd()
RtVoid TIntermediateRenderer::transformEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiTransformEnd(lineNo()));
	}

	if ( !isValid(IDX_TRANSFORM_END, "TIntermediateRenderer::transformEnd()") )
		return;

	if ( !m_currentContext->popContextState("TIntermediateRenderer::transformEnd()", *this) )
		return;

	// if ( isDefiningObject() )
	// 	return;

	// get back the transformation matrix
	m_currentContext->popTransformation(*this);

	m_callri->doTransformEnd();
}


//////////////////////////////////////////////////////////////////////////
// solid block

//////////////////////////////////////////////////////////////////////////
// solidBegin()
RtVoid TIntermediateRenderer::solidBegin(RtToken type) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiSolidBegin(lineNo(), type));
	}

	if ( !isValid(IDX_SOLID_BEGIN, "TIntermediateRenderer::solidBegin()") )
		return;

	m_currentContext->pushContextState(CTX_SOLID);

	type = getDeclMap().getToken(type);
	if ( !type ||
		(strcmp(type, RI_PRIMITIVE) &&
		 strcmp(type, RI_INTERSECTION) &&
		 strcmp(type, RI_UNION) &&
		 strcmp(type, RI_DIFFERENCE)) ) {

		handleError(RIE_BADSOLID, RIE_ERROR, "TIntermediateRenderer::solidBegin() - illegal type");
	}

	m_currentContext->pushSolidState(type);

	// if ( isDefiningObject() )
	// 	return;

	m_callri->doSolidBegin(type);
}


//////////////////////////////////////////////////////////////////////////
// solidEnd()
RtVoid TIntermediateRenderer::solidEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiSolidEnd(lineNo()));
	}

	if ( !isValid(IDX_SOLID_END, "TIntermediateRenderer::solidEnd()") )
		return;

	if ( !m_currentContext->popContextState("TIntermediateRenderer::solidEnd()", *this) )
		return;

	m_currentContext->popSolidState("TIntermediateRenderer::solidEnd()", *this);

	// if ( isDefiningObject() )
	// 	return;

	m_callri->doSolidEnd();
}


//////////////////////////////////////////////////////////////////////////
// object block

	
//////////////////////////////////////////////////////////////////////////
// objectBegin()
RtObjectHandle TIntermediateRenderer::objectBegin(void) {
	assert(m_currentContext != NULL);
	RtObjectHandle handle;

	// objectBegin is not be recorded in a macro because it is a macro itself

	if ( !isValid(IDX_OBJECT_BEGIN, "TIntermediateRenderer::objectBegin()") )
		return (RtObjectHandle)-1;

	m_currentContext->pushContextState(CTX_OBJECT);

	handle = (RtObjectHandle)(m_currentContext->m_objectCnt++);

	// The standard function triggers a macro recorder.

	RtObjectHandle obj = m_callri->doObjectBegin(handle);
	return obj;
}


//////////////////////////////////////////////////////////////////////////
// objectEnd()
RtVoid TIntermediateRenderer::objectEnd(void) {

	// Macro Object End is not recorded
	if ( !isValid(IDX_OBJECT_END, "TIntermediateRenderer::objectEnd()") )
		return;

	m_currentContext->popContextState("TIntermediateRenderer::objectEnd()", *this);

	m_callri->doObjectEnd();
}


RtVoid TIntermediateRenderer::objectInstance(RtObjectHandle handle) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		TRiObjectInstance *obj = newRiObjectInstance(lineNo(), handle);
		if ( !obj ) {
			handleError(RIE_NOMEM, RIE_ERROR, "TIntermediateRenderer::objectInstance()");
			return;
		}
 		m_currentContext->m_macroWrite->add(obj);
	}

	if ( !isValid(IDX_OBJECT_INSTANCE, "TIntermediateRenderer::objectInstance()") )
		return;

	// Replay is triggered even in an object definition
	m_callri->doObjectInstance(handle);
}

//////////////////////////////////////////////////////////////////////////
// motion block


//////////////////////////////////////////////////////////////////////////
// motionBeginV()
RtVoid TIntermediateRenderer::motionBeginV(RtInt N, RtFloat times[]) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiMotionBegin(lineNo(), N, times));
	}

	if ( !isValid(IDX_MOTION_BEGIN, "TIntermediateRenderer::motionBeginV()") )
		return;

	m_currentContext->m_samples.clear();
	m_currentContext->m_samples.resize(N);
	int n;
	for ( n=0; n < N; ++n ) {
		m_currentContext->m_samples[n] = times[n];
	}

	m_currentContext->pushContextState(CTX_MOTION);

	// if ( isDefiningObject() )
	// 	return;

	m_callri->doMotionBeginV(N, times);
}

//////////////////////////////////////////////////////////////////////////
// motionEnd()
RtVoid TIntermediateRenderer::motionEnd(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiMotionEnd(lineNo()));
	}

	if ( !isValid(IDX_MOTION_END, "TIntermediateRenderer::motionEnd()") )
		return;

	m_currentContext->popContextState("TIntermediateRenderer::motionEnd()", *this);

	// if ( isDefiningObject() )
	// 	return;

	m_callri->doMotionEnd();
}


/////////////////
// Camera options


/////////////////////////////////////////////////////////////////////////////////////////
// establishProjection() - Sets all values needed for viewing projection. Either
// called from RiProjection() or RiWorlBegin()
//
RtVoid TIntermediateRenderer::establishProjection() {
	assert(m_currentContext != NULL);
	assert(m_currentContext->m_currentOptions != NULL);

	// Viewport settings from RiFormat() and RiDisplay() (the frame)
	options().m_vpCurrentViewPort.m_iOriginX = (RtInt)(options().m_iDisplayOriginX);
	options().m_vpCurrentViewPort.m_iOriginY = (RtInt)(options().m_iDisplayOriginY);
	options().m_vpCurrentViewPort.m_iWidth = options().m_iDisplayHRes;
	if ( options().m_vpCurrentViewPort.m_iWidth <= 0 ) {
		options().m_vpCurrentViewPort.m_iWidth = DEFAULT_SCREEN_WID;
	}
	options().m_vpCurrentViewPort.m_iHeight = options().m_iDisplayVRes;
	if ( options().m_vpCurrentViewPort.m_iHeight <= 0 ) {
		options().m_vpCurrentViewPort.m_iHeight = DEFAULT_SCREEN_HT;
	}
	options().m_vpCurrentViewPort.m_fPixelAspect = options().m_fDisplayPixelAspect;
	if ( options().m_vpCurrentViewPort.m_fPixelAspect <= (RtFloat)0.0 ) {
		options().m_vpCurrentViewPort.m_fPixelAspect = (RtFloat)1.0;
	}

	// Frame aspect ratio settings
	if ( !options().m_bFrameAspectRatioCalled ) {
		// Frameaspect from ViewPort
		options().m_fFrameAspectRatio = (RtFloat)options().m_vpCurrentViewPort.m_iWidth * options().m_vpCurrentViewPort.m_fPixelAspect / (RtFloat)options().m_vpCurrentViewPort.m_iHeight;
	} else {
		// Frameaspect determines ViewPort
		if ( options().m_vpCurrentViewPort.m_iWidth * options().m_vpCurrentViewPort.m_fPixelAspect / options().m_fFrameAspectRatio >= options().m_vpCurrentViewPort.m_iHeight ) {
			options().m_vpCurrentViewPort.m_iWidth = (RtInt)(options().m_vpCurrentViewPort.m_iHeight * options().m_fFrameAspectRatio / options().m_vpCurrentViewPort.m_fPixelAspect);
		} else {
			options().m_vpCurrentViewPort.m_iHeight = (RtInt)(options().m_vpCurrentViewPort.m_fPixelAspect * options().m_vpCurrentViewPort.m_iWidth / options().m_fFrameAspectRatio);
		}
	}

	// Screen Window
	if ( !options().m_bScreenWindowCalled ) {
		// Calculate the screen window using frame aspect ratio
		if ( options().m_fFrameAspectRatio >= 1.0 ) {
			options().m_fScreenWindowLeft = -options().m_fFrameAspectRatio;
			options().m_fScreenWindowRight = options().m_fFrameAspectRatio;
			options().m_fScreenWindowBottom = (RtFloat)-1.0;
			options().m_fScreenWindowTop = (RtFloat)1.0;
		} else {
			options().m_fScreenWindowLeft = (RtFloat)-1.0;
			options().m_fScreenWindowRight = (RtFloat)1.0;
			options().m_fScreenWindowBottom = (RtFloat)-1.0/options().m_fFrameAspectRatio;
			options().m_fScreenWindowTop = (RtFloat)1.0/options().m_fFrameAspectRatio;
		}
	}
	
	// Screen window: left < right and top < bottom
	if ( options().m_fScreenWindowLeft > options().m_fScreenWindowRight ) {
		std::swap(options().m_fScreenWindowLeft, options().m_fScreenWindowRight);
	}
	if ( options().m_fScreenWindowBottom > options().m_fScreenWindowTop ) {
		std::swap(options().m_fScreenWindowBottom, options().m_fScreenWindowTop);
	}

    // calcs the pixel coordinates of the cropped window
	options().m_iRxMin = 0;
	options().m_iRxMax = options().m_vpCurrentViewPort.m_iWidth;
	options().m_iRyMin = 0;
	options().m_iRyMax = options().m_vpCurrentViewPort.m_iHeight;

	if ( options().m_bCropWindowCalled ) {
		options().m_iRxMin = (RtInt)ceil(options().m_fCropWindowLeft*options().m_vpCurrentViewPort.m_iWidth);
		options().m_iRxMax = (RtInt)ceil((options().m_fCropWindowRight*options().m_vpCurrentViewPort.m_iWidth)-1.0);
		options().m_iRyMin = (RtInt)ceil(options().m_fCropWindowTop*options().m_vpCurrentViewPort.m_iHeight);
		options().m_iRyMax = (RtInt)ceil((options().m_fCropWindowBottom*options().m_vpCurrentViewPort.m_iHeight)-1.0);
	}

	// Screenwindow from projection
	if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {
		if ( options().m_bFOVSet || !options().m_bScreenWindowCalled ) {
			// Screenwindow from FOV
			RtFloat wid_half = (RtFloat)(tan(deg2rad(options().m_tCameraFOV) / 2.0));
			if ( options().m_fFrameAspectRatio > 1.0 ) {
				options().m_fScreenWindowLeft = -wid_half*options().m_fFrameAspectRatio;
				options().m_fScreenWindowRight = wid_half*options().m_fFrameAspectRatio;
				options().m_fScreenWindowBottom = -wid_half;
				options().m_fScreenWindowTop = wid_half;
			} else {
				options().m_fScreenWindowLeft = -wid_half;
				options().m_fScreenWindowRight = wid_half;
				options().m_fScreenWindowBottom = -wid_half/options().m_fFrameAspectRatio;
				options().m_fScreenWindowTop = wid_half/options().m_fFrameAspectRatio;
			}
		} else {
			// FOV from ScreenWindow (90.0 FOV results from default screen window)
			RtFloat width  = (RtFloat)fabs(options().m_fScreenWindowRight - options().m_fScreenWindowLeft);
			RtFloat height = (RtFloat)fabs(options().m_fScreenWindowTop - options().m_fScreenWindowBottom);
			options().m_tCameraFOV = (RtFloat)2.0 * (RtFloat)atan(min(width, height)*.5);
			options().m_tCameraFOV = rad2deg(options().m_tCameraFOV);
		}
	}


	// Transformation matrices

	RtFloat f = options().m_fCameraFarClip;
	RtFloat n = options().m_fCameraNearClip;
	RtFloat l = options().m_fScreenWindowLeft;
	RtFloat r = options().m_fScreenWindowRight;
	RtFloat b = options().m_fScreenWindowBottom;
	RtFloat t = options().m_fScreenWindowTop;

	// Camera to Raster(+Z)
	
	// NDC to Raster (scales to frame coordinates)
	TMatrix3D screenMatrix;

	screenMatrix.identity();
	
	// Camera to Screen (projection to canonical view volume)
	options().m_mPreProjectionMatrix = CTM();

	if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {
		// Perspective Matrix  Frustum(l, r, b, t, n, f) -> (-1, 1, -1, 1, 0, 1)
		// Matrix from Aqsis
		l *= n;
		r *= n;
		t *= n;
		b *= n;
		RtMatrix projMatrix = {
			 {(RtFloat)(2.0*n/(r-l)),                    0.0,        0.0,  0.0 },
			 {	                 0.0, (RtFloat)(2.0*n/(t-b)),        0.0,  0.0  },
			 {           (r+l)/(r-l),            (t+b)/(t-b),    f/(f-n),  1.0 },
			 {                   0.0,                    0.0, -n*f/(f-n),  0.0 }
		};
		CTM().concatTransform(projMatrix);
		screenMatrix.concatTransform(CTM());
	} else if ( !strcmp(options().m_tCameraProjection, RI_ORTHOGRAPHIC) ) {
		// Orthographic Matrix (l, r, b, t, n, f) -> (-1, 1, -1, 1, 0, 1)
		// Matrix from Aqsis
		RtMatrix projMatrix = {
			 {(RtFloat)(2.0/(r-l)),                  0.0,                   0.0, 0.0 },
			 {	               0.0, (RtFloat)(2.0/(t-b)),                   0.0, 0.0 },
			 {                 0.0,                  0.0, (RtFloat)(-2.0/(f-n)), 0.0 },
    		 {         (r+l)/(r-l),          (t+b)/(t-b),           (f+n)/(f-n), 1.0 }
		};
		CTM().concatTransform(projMatrix);
		screenMatrix.concatTransform(CTM());
	} else if ( !strcmp(options().m_tCameraProjection, ri_NullToken) ) {
		// Identity
		screenMatrix.concatTransform(CTM());
	}

	m_currentContext->m_mapCoordSys[RI_SCREEN] = screenMatrix;

	// Screen to NDC (translate and scale the screenwindow to NDC, top left is 0,0)
	screenMatrix.scale(
		(RtFloat) 0.5,
		(RtFloat)-0.5,
		(RtFloat) 1.0
	);

	screenMatrix.translate(
		(RtFloat) 1.0,
		(RtFloat)-1.0,
		(RtFloat) 0.0
	);

	m_currentContext->m_mapCoordSys[RI_NDC] = screenMatrix;

	screenMatrix.scale(
		(RtFloat)options().m_vpCurrentViewPort.m_iWidth,
		(RtFloat)options().m_vpCurrentViewPort.m_iHeight,
		(RtFloat)1.0
	);

	m_currentContext->m_mapCoordSys[RI_RASTER] = screenMatrix;
}


/////////////////////////////////////////////////////////////////////////////////////////
// RiProjection() - Sets the Projection Option, orthographic and perspective
// (with optional parameter 0 <= FOV < 180) is handled
// name: Type of Projection
//

RtVoid TIntermediateRenderer::projectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);
	assert(m_currentContext->m_currentTransform != NULL);
	assert(m_currentContext->m_currentAttributes != NULL);
	assert(m_currentContext->m_currentOptions != NULL);

	TRiCPPBinding::projectionV(name, n, tokens, params);

	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::projectionV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiProjection(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_PROJECTION, "TIntermediateRenderer::projectionV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_bProjectionCalled = TRUE;
	if ( !name )
		name = ri_NullToken;

	options().m_tCameraProjection = name;

	if ( !strcmp(options().m_tCameraProjection, RI_ORTHOGRAPHIC) ) {

		options().m_tCameraProjection = RI_ORTHOGRAPHIC;

	} else if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {

		options().m_tCameraProjection = RI_PERSPECTIVE;

		// Optional parameter 'fov'
		options().m_bFOVSet = FALSE;
		
		RtPointer val = NULL;
		getValue(RI_FOV, val, n, tokens, params);
		if ( val ) {
			options().m_bFOVSet = RI_TRUE;
			options().m_tCameraFOV = ((RtFloat *)val)[0];
			if ( options().m_tCameraFOV > 180.0 ) {
				options().m_tCameraFOV = 179.0;
				handleError(RIE_RANGE, RIE_ERROR, "TIntermediateRenderer::projectionV(), fov to high, setting to 180.0");
			}
			if ( options().m_tCameraFOV < 0.0 ) {
				options().m_tCameraFOV = (RtFloat)0.1;
				handleError(RIE_RANGE, RIE_ERROR, "TIntermediateRenderer::projectionV(), fov to low, setting to 0.0");
			}
		}
	} else if ( !strcmp(options().m_tCameraProjection, ri_NullToken) ) {
		options().m_tCameraProjection = ri_NullToken;
	} else {
		handleError(RIE_UNIMPLEMENT, RIE_WARNING, "TIntermediateRenderer::projectionV(), projection \"%s\" not implemented", name);
	}

	// Do projection is called before the projection is established, so that nonstandard projection can be established
	m_callri->doProjectionV(name, n, tokens, params);

	// Gets all values from camera to raster projection
	establishProjection();

	// Reset CTM for world to camera transform
	attributes().m_currCoordSysName = RI_CAMERA;
	CTM().identity();
}


/////////////////////////////////////////////////////////////////////////////////////////
// RiClipping(): Setting near (hither) and far (yon) clipping planes
//
RtVoid TIntermediateRenderer::clipping(RtFloat hither, RtFloat yon) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiClipping(lineNo(), hither, yon));
	}

	if ( !isValid(IDX_CLIPPING, "TIntermediateRenderer::clipping") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_bNearFarSet = TRUE;
	if ( hither < RI_EPSILON ) hither = RI_EPSILON;
	if ( hither > RI_INFINITY ) hither = RI_INFINITY;
	if ( yon < RI_EPSILON ) yon = RI_EPSILON;
	if ( yon > RI_INFINITY ) yon = RI_INFINITY;
	if ( hither < yon ) {
		options().m_fCameraNearClip = hither;
		options().m_fCameraFarClip = yon;
	} else {
		options().m_fCameraNearClip = yon;
		options().m_fCameraFarClip = hither;
	}

	m_callri->doClipping(hither, yon);
}


/////////////////////////////////////////////////////////////////////////////////////////
// RiClippingPlane(): Setting further clipping plane
RtVoid TIntermediateRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiClippingPlane(lineNo(), x, y, z, nx, ny, nz));
	}

	if ( !isValid(IDX_CLIPPING_PLANE, "TIntermediateRenderer::clippingPlane()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_cameraClips.push_back(x);
	options().m_cameraClips.push_back(y);
	options().m_cameraClips.push_back(z);
	options().m_cameraClips.push_back(nx);
	options().m_cameraClips.push_back(ny);
	options().m_cameraClips.push_back(nz);

	m_callri->doClippingPlane(x, y, z, nx, ny, nz);
}


/////////////////////////////////////////////////////////////////////////////////////////
// RiDepthOfField
RtVoid TIntermediateRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiDepthOfField(lineNo(), fstop, focallength, focaldistance));
	}

	if ( !isValid(IDX_DEPTH_OF_FIELD, "TIntermediateRenderer::depthOfField()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_fCameraFStop = fstop;
	options().m_fCameraFocalLength = focallength;
	options().m_fCameraFocalDistance = focaldistance;

	m_callri->doDepthOfField(fstop, focallength, focaldistance);
}


/////////////////////////////////////////////////////////////////////////////////////////
// RiShutter()
RtVoid TIntermediateRenderer::shutter(RtFloat smin, RtFloat smax) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiShutter(lineNo(), smin, smax));
	}

	if ( !isValid(IDX_SHUTTER, "TIntermediateRenderer::shutter()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_fCameraShutterOpen = smin;
	options().m_fCameraShutterClose = smax;

	m_callri->doShutter(smin, smax);
}


//////////////////
// Viewing options


/////////////////////////////////////////////////////////////////////////////////////////
// RiDisplay - Choose a display by name and set the type of the
//             output generated, sets an optional parameter RI_ORIGIN.
//             Must be called directly within RiBegin/RiEnd
//
// throws: RIE_NOMEM, RIE_NOTSTARTED
//
// name: literal name of the output device
// type: type of the output device (e.g. RI_FRAMEBUFFER, RI_FILE)
// mode: Channels (e.g. RI_RGB, RI_A, RI_Z, RI_RGBA, RI_RGBZ, RI_RGBAZ)
//
// parameterlist: RI_ORIGIN [int, int] optional, pixel offset of the output, default [0, 0]
//                                     coordinates running from top left to bottom right
//
// Stores the parameters at the rear of m_paDisplayList as a TDisplayDesc record
//

RtVoid TIntermediateRenderer::displayV(RtToken name, RtToken type, RtToken mode,
								RtInt n, RtToken tokens[], RtPointer params[])
{
	assert(m_currentContext != NULL);
	TRiCPPBinding::displayV(name, type, mode, n, tokens, params);

	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::displayV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiDisplay(lineNo(), getDeclMap(), curColorComponents(), name, type, mode, n, tokens, params));
		}
	}

	if ( !isValid(IDX_DISPLAY, "TIntermediateRenderer::displayV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	TDisplayDesc d;
	if ( !d.displayV(getDeclMap(), options().m_iNColor, name, type, mode, RI_ORIGIN, n, tokens, params) )
		handleError(RIE_BADTOKEN, RIE_ERROR, "TIntermediateRenderer::displayV()");
	options().m_displays.push_back(d);

	options().m_iDisplayOriginX = d.m_origin[0];
	options().m_iDisplayOriginY = d.m_origin[1];

	m_callri->doDisplayV(name, type, mode, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////
// RiFormat - setting the resolution and the pixelaspectratio of the image.
//            Must be called directly within the RiBegin/RiEnd block
//
// xres:   horizontal resolution of the output device (in pixels)
// yres:   vertical resolution of the output device (in pixels)
// aspect: pixelaspectratio = pixelwidth/pixelheight
//         if aspect <= 0: the defaults of the output device (set by RiDisplay)
//         will be taken
//
// throws: RIE_NOTSTARTED
//
// Sets m_bFormatCalled = RI_TRUE to indicate that RiFormat is called and the
// corresponding members: m_iDisplayHRes (xres) m_iDisplayVRes (yres) and
// m_fDisplayPixelAspect (aspect).
// Action is deferred to RiWorldBegin
//
RtVoid TIntermediateRenderer::format(RtInt xres, RtInt yres, RtFloat aspect) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiFormat(lineNo(), xres, yres, aspect));
	}

	if ( !isValid(IDX_FORMAT, "TIntermediateRenderer::format()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	// Takes the values explicitly set by RiFormat
	if ( !options().m_disableFormat ) {
		options().m_bFormatCalled = RI_TRUE;
		options().m_iDisplayHRes = (RtInt)xres;
		options().m_iDisplayVRes = (RtInt)yres;
		options().m_fDisplayPixelAspect = (RtFloat)aspect;
	}
	m_callri->doFormat(xres, yres, aspect);
}


////////////////////////////////////////////////////////////////////////////////
// RiFrameAspectRatio - setting the aspect ratio (framewidth/frameheight) of the
//                      frame (screen window). if the frameaspect doesn´t match
//                      the ratio of the screen window, distortion results.
//                      Must be called directly within the RiBegin/RiEnd block
//
// aspect: frameaspectratio = framewidth/frameheight
//
// throws: RIE_NOTSTARTED
//
// Sets m_bFrameAspectRatioCalled = RI_TRUE to indicate that
// RiFrameAspectRatio is called and the
// corresponding members: m_fFrameAspectRatio (aspect)
// Action is deferred to RiWorldBegin
//
RtVoid TIntermediateRenderer::frameAspectRatio(RtFloat aspect) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiFrameAspectRatio(lineNo(), aspect));
	}

	if ( !isValid(IDX_FRAME_ASPECT_RATIO, "TIntermediateRenderer::frameAspectRatio()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	// Takes the values explicitly set by RiFormat
	options().m_bFrameAspectRatioCalled = RI_TRUE;
	options().m_fFrameAspectRatio = aspect;

	m_callri->doFrameAspectRatio(aspect);
}


/////////////////////////////////////////////////////////////////////////////////////
// RiScreenWindow - setting the size of the frame in normalized display coordinates
//                  default values are that -1, 1 fits the smaller edge of the frame.
//                  Directly corresponds to RiFrameAspectRatio
//                  Must be called directly within the RiBegin/RiEnd block
//
// left:	left edge of the frame in display coordinates
// right:   right edge of the frame in display coordinates
// bot:     bottom edge of the frame in display coordinates
// top:     top edge of the frame in display coordinates
//
// throws: RIE_NOTSTARTED
//
// Sets m_bScreenWindowCalled = RI_TRUE to indicate that
// RiFrameAspectRatio is called and the
// corresponding members: m_fScreenWindow_ (left, right, bot, top).
// If the condition left <= right and bot <= top isn´t valid,
// the output is mirrored in the given fashion.
// Action is deferred to RiWorldBegin
//
RtVoid TIntermediateRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiScreenWindow(lineNo(), left, right, bot, top));
	}

	if ( !isValid(IDX_SCREEN_WINDOW, "TIntermediateRenderer::screenWindow()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_bScreenWindowCalled = RI_TRUE;
	options().m_fScreenWindowLeft = left;
	options().m_fScreenWindowRight = right;
	options().m_fScreenWindowBottom = bot;
	options().m_fScreenWindowTop = top;

	m_callri->doScreenWindow(left, right, bot, top);
}


/////////////////////////////////////////////////////////////////////////////////////
// RiCropWindow - setting the size of the crop window in normalized display
//                coordinates between 0 and 1, min < max
//
// xmin:	left edge of the crop window in display coordinates
// xmax:    right edge of the crop window in display coordinates
// ymin:    bottom edge of the crop window in display coordinates
// ymax:    top edge of the crop window in display coordinates
//
// rxmin = ceil(left * xres)
// rxmax = ceil((right * xres) - 1)
// rymin = ceil(top * yres)
// rymax = ceil((bottom*yres)-1)
//
// -> property of perfect tiling...
//
// throws: RIE_NOTSTARTED
//
// Sets m_bCropWindowCalled = RI_TRUE to indicate that
// RiCropWindow is called and the corresponding members:
// m_fCropWindow[0..3] (xmin, xmax, ymin, ymax).
//
RtVoid TIntermediateRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiCropWindow(lineNo(), xmin, xmax, ymin, ymax));
	}

	if ( !isValid(IDX_CROP_WINDOW, "TIntermediateRenderer::cropWindow()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_bCropWindowCalled = RI_TRUE;

	if ( xmin <= xmax ) {
		options().m_fCropWindowLeft = xmin;
		options().m_fCropWindowRight = xmax;
	} else {
		options().m_fCropWindowLeft = xmax;
		options().m_fCropWindowRight = xmin;
	}

	if ( options().m_fCropWindowLeft < 0.0 )
		options().m_fCropWindowLeft = 0.0;
	if ( options().m_fCropWindowRight < 0.0 )
		options().m_fCropWindowRight = 0.0;
	if ( options().m_fCropWindowLeft > 1.0 )
		options().m_fCropWindowLeft = 1.0;
	if ( options().m_fCropWindowRight > 1.0 )
		options().m_fCropWindowRight = 1.0;

	if ( ymin <= ymax ) {
		options().m_fCropWindowTop = ymin;
		options().m_fCropWindowBottom = ymax;
	} else {
		options().m_fCropWindowTop = ymax;
		options().m_fCropWindowBottom = ymin;
	}

	if ( options().m_fCropWindowTop < 0.0 )
		options().m_fCropWindowTop = 0.0;
	if ( options().m_fCropWindowBottom < 0.0 )
		options().m_fCropWindowBottom = 0.0;
	if ( options().m_fCropWindowTop > 1.0 )
		options().m_fCropWindowTop = 1.0;
	if ( options().m_fCropWindowBottom > 1.0 )
		options().m_fCropWindowBottom = 1.0;

	m_callri->doCropWindow(xmin, xmax, ymin, ymax);
}


RtVoid TIntermediateRenderer::pixelVariance(RtFloat variance) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiPixelVariance(lineNo(), variance));
	}

	if ( !isValid(IDX_PIXEL_VARIANCE, "TIntermediateRenderer::pixelVariance()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_bPixelVarianceCalled = TRUE;
	options().m_fPixelVariance = variance;

	m_callri->doPixelVariance(variance);
}


RtVoid TIntermediateRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiPixelSamples(lineNo(), xsamples, ysamples));
	}

	if ( !isValid(IDX_PIXEL_SAMPLES, "TIntermediateRenderer::pixelSamples()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_fXSamples = xsamples;
	options().m_fYSamples = ysamples;

	m_callri->doPixelSamples(xsamples, ysamples);
}


RtVoid TIntermediateRenderer::pixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiPixelFilter(lineNo(), function, xwidth, ywidth));
	}

	if ( !isValid(IDX_PIXEL_FILTER, "TIntermediateRenderer::pixelFilter()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_pFilterFunc = function;
	options().m_fXWidth = xwidth;
	options().m_fYWitdh = ywidth;

	m_callri->doPixelFilter(function, xwidth, ywidth);
}


RtVoid TIntermediateRenderer::exposure(RtFloat gain, RtFloat gamma) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiExposure(lineNo(), gain, gamma));
	}

	if ( !isValid(IDX_EXPOSURE, "TIntermediateRenderer::exposure()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_fGain = gain;
	options().m_fGamma = gamma;

	m_callri->doExposure(gain, gamma);
}


RtVoid TIntermediateRenderer::imagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::imagerV(name, n, tokens, params);

	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::imagerV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiImager(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_IMAGER, "TIntermediateRenderer::imagerV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	RtToken t = getDeclMap().getToken(name);
	options().m_tImager = t ? t : name;
	if ( !options().m_imagerParamMap.set(getDeclMap(), 0, 0, 0, 0, 0, options().m_iNColor, n, tokens, params) ) {
		handleError(RIE_BADTOKEN, RIE_ERROR, "TIntermediateRenderer::imagerV()");
	}

	m_callri->doImagerV(name, n, tokens, params);
}


RtVoid TIntermediateRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl){
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiQuantize(lineNo(), type, one, qmin, qmax, ampl));
	}

	if ( !isValid(IDX_QUANTIZE, "TIntermediateRenderer::quantize()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	if ( !strcmp(type, RI_RGBA) ) {
		options().m_iOneRGBA = one;
		options().m_iMinRGBA = qmin;
		options().m_iMaxRGBA = qmax;
		options().m_fDitherAmplitudeRGBA = ampl;
	} else if ( !strcmp(type, RI_Z) ) {
		options().m_iOneZ = one;
		options().m_iMinZ = qmin;
		options().m_iMaxZ = qmax;
		options().m_fDitherAmplitudeZ = ampl;
	}

	m_callri->doQuantize(type, one, qmin, qmax, ampl);
}

// Hider

RtVoid TIntermediateRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::hiderV(type, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::hiderV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiHider(lineNo(), getDeclMap(), curColorComponents(), type, n, tokens, params));
		}
	}

	if ( !isValid(IDX_HIDER, "TIntermediateRenderer::hiderV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	RtToken t = getDeclMap().getToken(type);
	options().m_tHider = t ? t : type;
	if ( !options().m_hiderParamMap.set(getDeclMap(), 0, 0, 0, 0, 0, options().m_iNColor, n, tokens, params) ) {
		handleError(RIE_BADTOKEN, RIE_ERROR, "TIntermediateRenderer::hiderV()");
	}

	m_callri->doHiderV(type, n, tokens, params);
}


RtVoid TIntermediateRenderer::colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiColorSamples(lineNo(), N, nRGB, RGBn));
	}

	if ( !isValid(IDX_COLOR_SAMPLES, "TIntermediateRenderer::colorSamples()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_nRGB.clear();
	options().m_RGBn.clear();
	options().m_iNColor = N;
	int i;
	for ( i = 0; i < N*3; ++i ) {
		options().m_nRGB.push_back(nRGB[i]);
		options().m_RGBn.push_back(RGBn[i]);
	}

	m_callri->doColorSamples(N, nRGB, RGBn);
}


RtVoid TIntermediateRenderer::relativeDetail(RtFloat relativedetail) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiRelativeDetail(lineNo(), relativedetail));
	}

	if ( !isValid(IDX_RELATIVE_DETAIL, "TIntermediateRenderer::relativeDetail()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	options().m_fRelativeDetail = relativedetail;

	m_callri->doRelativeDetail(relativedetail);
}


RtVoid TIntermediateRenderer::optionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);
	bool catchError;

	TRiCPPBinding::optionV(name, n, tokens, params);

	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::optionV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiOption(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_OPTION, "TIntermediateRenderer::optionV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	if ( !name ) {
		handleError(RIE_SYNTAX, RIE_ERROR, "No name specified in TIntermediateRenderer::optionV()");
		return;
	}

	// Insert option
	catchError = false;
	try {
		options().m_optionMap.setOption(getDeclMap(), options().m_iNColor, name, n, tokens, params);
	} catch (TSystemException &e) {
		handleError(RIE_NOMEM, RIE_SEVERE, "%s in TIntermediateRenderer::optionV()(%s...)", e.what(), name);
		catchError = true;
	}

	if ( catchError )
		return;

	// evaluate special ricpp options
	if ( !strcmp(name, ri_RiCPPControl) ) {
		int i = n;
		while ( i-- ) {
			if ( !strcmp(tokens[i], ri_RiCPPControlCamTrans) ) {
				RtFloat *matrix = params[i] ? *(RtFloat **)params[i] : NULL;
				if ( matrix )
					options().m_mCamTrans.set(matrix);
			} else if ( !strcmp(tokens[i], ri_RiCPPControlWriteArchive) ) {
				RtBoolean *flag = params[i] ? (RtBoolean *)params[i] : NULL;
				if ( flag )
					options().m_writeArchive = *flag;
			} else if ( !strcmp(tokens[i], ri_RiCPPControlWriteObject) ) {
				RtBoolean *flag = params[i] ? (RtBoolean *)params[i] : NULL;
				if ( flag )
					options().m_writeObject = *flag;
			} else if ( !strcmp(tokens[i], ri_RiCPPControlDisableFormat) ) {
				RtBoolean *flag = params[i] ? (RtBoolean *)params[i] : NULL;
				if ( flag )
					options().m_disableFormat = *flag;
			}
		}
	}

	m_callri->doOptionV(name, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Attributes


RtVoid TIntermediateRenderer::color(RtColor Cs) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiColor(lineNo(), curColorComponents(), Cs));
	}

	if ( !isValid(IDX_COLOR, "TIntermediateRenderer::color()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_color.clear();
	attributes().m_color.reserve(options().m_iNColor);
	int i;
	for ( i = 0; i < options().m_iNColor; ++i ) {
		attributes().m_color.push_back(Cs[i]);
	}

	m_callri->doColor(Cs);
}


RtVoid TIntermediateRenderer::opacity(RtColor Cs) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiOpacity(lineNo(), curColorComponents(), Cs));
	}

	if ( !isValid(IDX_OPACITY, "TIntermediateRenderer::opacity()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_opacity.clear();
	attributes().m_opacity.reserve(options().m_iNColor);
	int i;
	for ( i = 0; i < options().m_iNColor; ++i ) {
		attributes().m_opacity.push_back(Cs[i]);
	}

	m_callri->doOpacity(Cs);
}


RtVoid TIntermediateRenderer::textureCoordinates(
	RtFloat s1, RtFloat t1,
	RtFloat s2, RtFloat t2,
	RtFloat s3, RtFloat t3,
	RtFloat s4, RtFloat t4
) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiTextureCoordinates(lineNo(), s1, t1, s2, t2, s3, t3, s4, t4));;
	}

	if ( !isValid(IDX_TEXTURE_COORDINATES, "TIntermediateRenderer::textureCoordinates()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_textureCoordinates[0] = s1;
	attributes().m_textureCoordinates[1] = t1;
	attributes().m_textureCoordinates[2] = s2;
	attributes().m_textureCoordinates[3] = t2;
	attributes().m_textureCoordinates[4] = s3;
	attributes().m_textureCoordinates[5] = t3;
	attributes().m_textureCoordinates[6] = s4;
	attributes().m_textureCoordinates[7] = t4;

	m_callri->doTextureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
}



///////////////////////////////////////////////////////////////////////////////
// --- light

RtBoolean TIntermediateRenderer::establishLight(TLight &light, RtLightHandle handle) {
	assert(m_currentContext);
	assert(m_currentContext->m_currentOptions);

	TParameterMap &p = light.lightParameters();
	std::vector<RtFloat> lightColor;
	lightColor.resize(options().m_iNColor);
	int i;
	for ( i = 0; i < options().m_iNColor; ++i ) {
		lightColor[i] = 1.0;
	}

	RtPointer ptr = p.getValueOf(RI_LIGHTCOLOR);
	if ( ptr ) {
		for ( i = 0; i < options().m_iNColor; ++i ) {
			lightColor[i] = ((RtFloat *)ptr)[i];
		}
	}

	ptr = p.getValueOf(RI_INTENSITY);
	RtFloat intensity = 1.0;
	if ( ptr ) {
		// *******
		intensity = *(const RtFloat *)ptr;
	}

	if ( !strcmp(p.getName(), RI_AMBIENTLIGHT) ) {

		ambientLight(light, handle, intensity, &lightColor[0]);

	} else if ( !strcmp(p.getName(), RI_DISTANTLIGHT) ){

		ptr = p.getValueOf(RI_FROM);
		RtPoint from = {0.0, 0.0, 0.0};
		if ( ptr ) {
			from[0] = ((RtFloat *)ptr)[0];
			from[1] = ((RtFloat *)ptr)[1];
			from[2] = ((RtFloat *)ptr)[2];
		}

		ptr = p.getValueOf(RI_TO);
		RtPoint to = {0.0, 0.0, 1.0};
		if ( ptr ) {
			to[0] = ((RtFloat *)ptr)[0];
			to[1] = ((RtFloat *)ptr)[1];
			to[2] = ((RtFloat *)ptr)[2];
		}

		distantLight(light, handle, intensity, &lightColor[0], from, to);

	} else if ( !strcmp(p.getName(), RI_POINTLIGHT) ) {

		ptr = p.getValueOf(RI_FROM);
		RtPoint from = {0.0, 0.0, 0.0};
		if ( ptr ) {
			from[0] = ((RtFloat *)ptr)[0];
			from[1] = ((RtFloat *)ptr)[1];
			from[2] = ((RtFloat *)ptr)[2];
		}

		pointLight(light, handle, intensity, &lightColor[0], from);

	} else if ( !strcmp(p.getName(), RI_SPOTLIGHT) ) {

		ptr = p.getValueOf(RI_FROM);
		RtPoint from = {0.0, 0.0, 0.0};
		if ( ptr ) {
			from[0] = ((RtFloat *)ptr)[0];
			from[1] = ((RtFloat *)ptr)[1];
			from[2] = ((RtFloat *)ptr)[2];
		}

		ptr = p.getValueOf(RI_TO);
		RtPoint to = {0.0, 0.0, 1.0};
		if ( ptr ) {
			to[0] = ((RtFloat *)ptr)[0];
			to[1] = ((RtFloat *)ptr)[1];
			to[2] = ((RtFloat *)ptr)[2];
		}

		ptr = p.getValueOf(RI_CONEANGLE);
		RtFloat coneangle = 30.0;
		if ( ptr ) coneangle = *(RtFloat *)ptr;

		ptr = p.getValueOf(RI_CONEDELTAANGLE);
		RtFloat conedeltaangle = 5.0;
		if ( ptr ) conedeltaangle = *(RtFloat *)ptr;

		ptr = p.getValueOf(RI_BEAMDISTRIBUTION);
		RtFloat beamdistribution = 2.0;
		if ( ptr ) beamdistribution = *(RtFloat *)ptr;

		spotLight(light, handle, intensity, lightColor.empty() ? NULL : &lightColor[0], from, to, coneangle, conedeltaangle, beamdistribution);

	} else {

		nonStandardLight(light, handle, intensity, lightColor.empty() ? NULL :&lightColor[0]);

	}

	return RI_TRUE;
}

void TIntermediateRenderer::resetLights() {
	assert(m_currentContext);
	assert(m_currentContext->m_currentAttributes);

	RtLightHandle pos;
	std::vector<RtLightHandle>::iterator iter;
	for ( iter = attributes().m_lightsOn.begin();
		  iter != attributes().m_lightsOn.end();
		  ++iter
	) {
		pos = *iter;
		TLight &light = m_currentContext->m_lightList[(long)pos];
		establishLight(light, pos);
	}
}


RtLightHandle TIntermediateRenderer::lightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);
	assert(m_currentContext->m_currentTransform != NULL);
	assert(m_currentContext->m_currentAttributes != NULL);
	assert(m_currentContext->m_currentOptions != NULL);

	RtLightHandle handle = TRiCPPBinding::lightSourceV(name, n, tokens, params);

	// Macro?
	TRiLightSource *obj = NULL;
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::lightSourceV() - Stacks empty");
		} else {
			obj = newRiLightSource(lineNo(), getDeclMap(), curColorComponents(), name, (RtLightHandle)-1, n, tokens, params);
			if ( !obj ) {
				handleError(RIE_NOMEM, RIE_ERROR, "TIntermediateRenderer::lightSourceV() - Out of memory could not record light source.");
				return (RtLightHandle)-1;
			}
			m_currentContext->m_macroWrite->add(obj);
			handle = obj->handleRef();
		}
	}

	if ( !isValid(IDX_LIGHT_SOURCE, "TIntermediateRenderer::lightSourceV()") )
		return (RtLightHandle)-1;

	// if ( isDefiningObject() )
	// 	return handle;

	TLight light(
		getDeclMap(),
		options().m_iNColor,
		false,
		m_currentContext ? m_currentContext->m_iState < CTX_WORLD : false,
		CTM(),
		getDeclMap().getToken(name),
		n, tokens, params);

	m_currentContext->m_lightList.push_back(light);
	TLight &lh = m_currentContext->m_lightList.back();
	RtLightHandle pos = (RtLightHandle)(m_currentContext->m_lightList.size()-1);
	attributes().m_lightsOn.push_back(pos);
	establishLight(lh, pos);

	RtLightHandle myHandle = m_callri->doLightSourceV(pos, name, n, tokens, params);
	if ( obj ) {
		obj->handle2(myHandle);
		return obj->handleRef();
	}
	return myHandle;
}

RtLightHandle TIntermediateRenderer::areaLightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	RtLightHandle handle = TRiCPPBinding::areaLightSourceV(name, n, tokens, params);

	// Macro?
	TRiAreaLightSource *obj = NULL;
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::areaLightSourceV() - Stacks empty");
		} else {
			obj = newRiAreaLightSource(lineNo(), getDeclMap(), curColorComponents(), name, (RtLightHandle)-1, n, tokens, params);
			if ( !obj )
				return (RtLightHandle)-1;
			m_currentContext->m_macroWrite->add(obj);
			handle = obj->handleRef();
		}
	}

	if ( !isValid(IDX_AREA_LIGHT_SOURCE, "TIntermediateRenderer::areaLightSourceV()") )
		return (RtLightHandle)-1;

	// if ( isDefiningObject() )
	// 	return handle;

	attributes().m_areaLightBlock = RI_TRUE;
	
	TLight light(
		getDeclMap(),
		options().m_iNColor,
		true,
		m_currentContext ? m_currentContext->m_iState < CTX_WORLD : false,
		CTM(),
		getDeclMap().getToken(name),
		n, tokens, params);

	m_currentContext->m_lightList.push_back(light);
	TLight &lh = (m_currentContext->m_lightList.back());
	RtLightHandle pos = (RtLightHandle)(m_currentContext->m_lightList.size()-1);
	attributes().m_lightsOn.push_back(pos);
	establishLight(lh, pos);
	RtLightHandle myHandle = m_callri->doAreaLightSourceV(pos, name, n, tokens, params);
	if ( obj ) {
		obj->handle2(myHandle);
		return obj->handleRef();
	}
	return myHandle;
}


RtVoid TIntermediateRenderer::illuminate(RtLightHandle light, RtBoolean onoff) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		TRiIlluminate * obj = newRiIlluminate(lineNo(), light, onoff);
		if ( !obj ) {
			handleError(RIE_NOMEM, RIE_ERROR, "TIntermediateRenderer::illuminate()");
			return;
		}
		m_currentContext->m_macroWrite->add(obj);
		light = obj->handle2();
	}

	if ( !isValid(IDX_ILLUMINATE, "TIntermediateRenderer::illuminate()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	std::vector<TLight *>::size_type pos = (std::vector<TLight *>::size_type)light;
	if ( pos >= m_currentContext->m_lightList.size() ) {
		handleError(RIE_BADHANDLE, RIE_ERROR, "TIntermediateRenderer::illuminate()");
		return;
	}

	std::vector<RtLightHandle>::iterator result = std::find(attributes().m_lightsOn.begin(), attributes().m_lightsOn.end(), (RtLightHandle)pos);
	if ( onoff ) {
		if (  result == attributes().m_lightsOn.end() ) {
			attributes().m_lightsOn.push_back((RtLightHandle)pos);
			establishLight(m_currentContext->m_lightList[pos], (RtLightHandle)pos);	
		}
	} else {
		if ( result != attributes().m_lightsOn.end()  ) {
			attributes().m_lightsOn.erase(result);
			resetLights();
		}
	}

	m_callri->doIlluminate(light, onoff);
}


///////////////////////////////////////////////////////////////////////////////
// Shader

void TIntermediateRenderer::establishShader(int shaderType) {
	TParameterMap &p = attributes().m_shaders[shaderType].shaderParameters();
	const char *shadername = p.getName();

	attributes().m_shaderMatrices[shaderType] = CTM();
	attributes().m_shaderInverseMatrices[shaderType] = inverseCTM();

	if ( !strcmp(shadername, RI_CONSTANT) ) {
		constantShader(shaderType);
	} else if ( !strcmp(shadername, RI_MATTE) ) {
		RtPointer ptr = p.getValueOf(RI_KA);
		RtFloat Ka = 1.0;
		if ( ptr )
			Ka = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KD);
		RtFloat Kd = 1.0;
		if ( ptr )
			Kd = *(const RtFloat *)ptr;

		matteShader(shaderType, Ka, Kd);
	} else if ( !strcmp(shadername, RI_METAL) ) {
		RtPointer ptr = p.getValueOf(RI_KA);
		RtFloat Ka = 1.0;
		if ( ptr )
			Ka = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KS);
		RtFloat Ks = 1.0;
		if ( ptr )
			Ks = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_ROUGHNESS);
		RtFloat roughness = (RtFloat).1;
		if ( ptr )
			roughness = *(const RtFloat *)ptr;

		metalShader(shaderType, Ka, Ks, roughness);
	} else if ( !strcmp(shadername, RI_SHINYMETAL) ) {
		RtPointer ptr = p.getValueOf(RI_KA);
		RtFloat Ka = 1.0;
		if ( ptr )
			Ka = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KS);
		RtFloat Ks = 1.0;
		if ( ptr )
			Ks = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KR);
		RtFloat Kr = 1.0;
		if ( ptr )
			Kr = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_ROUGHNESS);
		RtFloat roughness = (RtFloat).1;
		if ( ptr )
			roughness = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_TEXTURENAME);
		const char *texturename = "";
		if ( ptr && ((const char** )ptr)[0] )
			texturename = ((const char **)ptr)[0];

		shinyMetalShader(shaderType, Ka, Ks, Kr, roughness, texturename);
	} else if ( !strcmp(shadername, RI_PLASTIC) ) {
		RtPointer ptr = p.getValueOf(RI_KA);
		RtFloat Ka = 1.0;
		if ( ptr )
			Ka = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KD);
		RtFloat Kd = 1.0;
		if ( ptr )
			Kd = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KS);
		RtFloat Ks = 1.0;
		if ( ptr )
			Ks = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_ROUGHNESS);
		RtFloat roughness = (RtFloat).1;
		if ( ptr )
			roughness = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_SPECULARCOLOR);
		RtFloat specularcolor[3] = {1.0, 1.0, 1.0};
		if ( ptr ) {
			specularcolor[0] = ((const RtFloat *)ptr)[0];
			specularcolor[1] = ((const RtFloat *)ptr)[1];
			specularcolor[2] = ((const RtFloat *)ptr)[2];
		}

		plasticShader(shaderType, Ka, Kd, Ks, roughness, specularcolor);
	} else if ( !strcmp(shadername, RI_PAINTEDPLASTIC) ) {
		RtPointer ptr = p.getValueOf(RI_KA);
		RtFloat Ka = 1.0;
		if ( ptr )
			Ka = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KD);
		RtFloat Kd = 1.0;
		if ( ptr )
			Kd = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_KS);
		RtFloat Ks = 1.0;
		if ( ptr )
			Ks = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_ROUGHNESS);
		RtFloat roughness = (RtFloat).1;
		if ( ptr )
			roughness = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_SPECULARCOLOR);
		RtFloat specularcolor[3] = {1.0, 1.0, 1.0};
		if ( ptr ) {
			specularcolor[0] = ((const RtFloat *)ptr)[0];
			specularcolor[1] = ((const RtFloat *)ptr)[1];
			specularcolor[2] = ((const RtFloat *)ptr)[2];
		}

		ptr = p.getValueOf(RI_TEXTURENAME);
		const char *texturename = "";
		if ( ptr && ((const char** )ptr)[0] )
			texturename = ((const char **)ptr)[0];

		paintedPlasticShader(shaderType, Ka, Kd, Ks, roughness, specularcolor, texturename);
	} else if ( !strcmp(shadername, RI_BUMPY) ) {
		RtPointer ptr = p.getValueOf(RI_AMPLITUDE);
		RtFloat amplitude = 1.0;
		if ( ptr )
			amplitude = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_TEXTURENAME);
		const char *texturename = "";
		if ( ptr && ((const char** )ptr)[0] )
			texturename = ((const char **)ptr)[0];

		bumpyShader(shaderType, amplitude, texturename);
	} else if ( !strcmp(shadername, RI_DEPTHCUE) ) {
		RtPointer ptr = p.getValueOf(RI_MINDISTANCE);
		RtFloat mindistance = 0.0;
		if ( ptr )
			mindistance = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_MAXDISTANCE);
		RtFloat maxdistance = 1.0;
		if ( ptr )
			maxdistance = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_BACKGROUND);
		RtFloat background[3] = {1.0, 1.0, 1.0};
		if ( ptr ) {
			background[0] = ((const RtFloat *)ptr)[0];
			background[1] = ((const RtFloat *)ptr)[1];
			background[2] = ((const RtFloat *)ptr)[2];
		}
		depthCueShader(shaderType, mindistance, maxdistance, background);
	} else if ( !strcmp(shadername, RI_FOG) ) {
		RtPointer ptr = p.getValueOf(RI_DISTANCE);
		RtFloat distance = 1.0;
		if ( ptr )
			distance = *(const RtFloat *)ptr;

		ptr = p.getValueOf(RI_BACKGROUND);
		RtFloat background[3] = {1.0, 1.0, 1.0};
		if ( ptr ) {
			background[0] = ((const RtFloat *)ptr)[0];
			background[1] = ((const RtFloat *)ptr)[1];
			background[2] = ((const RtFloat *)ptr)[2];
		}
		fogShader(shaderType, distance, background);
	} else {
		nonStandardShader(shaderType);
	}
}


RtVoid TIntermediateRenderer::surfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::surfaceV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::surfaceV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiSurface(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_SURFACE, "TIntermediateRenderer::surfaceV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_SURFACE].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_SURFACE);

	m_callri->doSurfaceV(name, n, tokens, params);
}


RtVoid TIntermediateRenderer::atmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::atmosphereV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::atmosphereV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiAtmosphere(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_ATMOSPHERE, "TIntermediateRenderer::atmosphereV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_ATMOSPHERE].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_ATMOSPHERE);

	m_callri->doAtmosphereV(name, n, tokens, params);
}

RtVoid TIntermediateRenderer::interiorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::interiorV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::interiorV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiInterior(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_INTERIOR, "TIntermediateRenderer::interiorV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_INTERIOR].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_INTERIOR);

	m_callri->doInteriorV(name, n, tokens, params);
}


RtVoid TIntermediateRenderer::exteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::exteriorV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::exteriorV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiExterior(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_EXTERIOR, "TIntermediateRenderer::exteriorV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_EXTERIOR].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_EXTERIOR);

	m_callri->doExteriorV(name, n, tokens, params);
}


RtVoid TIntermediateRenderer::deformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::deformationV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::deformationV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiDeformation(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_DEFORMATION, "TIntermediateRenderer::deformationV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_DEFORMATION].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_DEFORMATION);

	m_callri->doDeformationV(name, n, tokens, params);
}


RtVoid TIntermediateRenderer::displacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::displacementV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::displacementV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiDisplacement(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_DISPLACEMENT, "TIntermediateRenderer::displacementV()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shaders[IDX_SHADER_DISPLACEMENT].shader(getDeclMap(), options().m_iNColor, getDeclMap().getToken(name), n, tokens, params);
	establishShader(IDX_SHADER_DISPLACEMENT);

	m_callri->doDisplacementV(name, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////


RtVoid TIntermediateRenderer::shadingRate(RtFloat size) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiShadingRate(lineNo(), size));
	}

	if ( !isValid(IDX_SHADING_RATE, "TIntermediateRenderer::shadingRate()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shadingRate = size;

	m_callri->doShadingRate(size);
}


RtVoid TIntermediateRenderer::shadingInterpolation(RtToken type) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiShadingInterpolation(lineNo(), type));
	}

	if ( !isValid(IDX_SHADING_INTERPOLATION, "TIntermediateRenderer::shadingInterpolation()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_shadingInterpolation = type;

	m_callri->doShadingInterpolation(type);
}


RtVoid TIntermediateRenderer::matte(RtBoolean onoff) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiMatte(lineNo(), onoff));
	}

	if ( !isValid(IDX_MATTE, "TIntermediateRenderer::matte()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_matte = onoff;

	m_callri->doMatte(onoff);
}


RtVoid TIntermediateRenderer::bound(RtBound bound) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiBound(lineNo(), bound));
	}

	if ( !isValid(IDX_BOUND, "TIntermediateRenderer::bound()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	int i;
	for ( i = 0; i < 6; ++i ) {
		attributes().m_bound[i] = bound[i];
	}
	attributes().m_boundCalled = RI_TRUE;

	m_callri->doBound(bound);
}

RtVoid TIntermediateRenderer::detail(RtBound bound) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiDetail(lineNo(), bound));
	}

	if ( !isValid(IDX_DETAIL, "TIntermediateRenderer::detail()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	int i;
	for ( i = 0; i < 6; ++i ) {
		attributes().m_detail[i] = bound[i];
	}
	attributes().m_detailCalled = RI_TRUE;

	m_callri->doDetail(bound);
}

RtVoid TIntermediateRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiDetailRange(lineNo(), minvis, lowtran, uptran, maxvis));
	}

	if ( !isValid(IDX_DETAIL_RANGE, "TIntermediateRenderer::detailRange()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_detailRange[0] = minvis;
	attributes().m_detailRange[1] = lowtran;
	attributes().m_detailRange[2] = uptran;
	attributes().m_detailRange[3] = maxvis;
	attributes().m_detailRangeCalled = RI_TRUE;

	m_callri->doDetailRange(minvis, lowtran, uptran, maxvis);
}

RtVoid TIntermediateRenderer::geometricApproximation(RtToken type, RtFloat value) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiGeometricApproximation(lineNo(), type, value));
	}

	if ( !isValid(IDX_GEOMETRIC_APPROXIMATION, "TIntermediateRenderer::geometricApproximation()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_geometricApproximationType = type;
	attributes().m_geometricApproximationValue = value;

	m_callri->doGeometricApproximation(type, value);
}

RtVoid TIntermediateRenderer::geometricRepresentation(RtToken type) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiGeometricRepresentation(lineNo(), type));
	}

	if ( !isValid(IDX_GEOMETRIC_REPRESENTATION, "TIntermediateRenderer::geometricRepresentation()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_geometricRepresentation = type;

	m_callri->doGeometricRepresentation(type);
}

RtVoid TIntermediateRenderer::orientation(RtToken orientation) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiOrientation(lineNo(), orientation));
	}

	if ( !isValid(IDX_ORIENTATION, "TIntermediateRenderer::orientation()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	bool prevOrientation = attributes().m_currentOrientation;
	if ( !strcmp(orientation, RI_OUTSIDE) ) {
		attributes().m_orientation = RI_OUTSIDE;
	} else if ( !strcmp(orientation, RI_INSIDE) ) {
		attributes().m_orientation = RI_INSIDE;
	} else if ( !strcmp(orientation, RI_LH) ) {
		attributes().m_orientation = RI_LH;
	} else if ( !strcmp(orientation, RI_RH) ) {
		attributes().m_orientation = RI_RH;
	} else {
		handleError(RIE_SYNTAX, RIE_ERROR, "RiOrientation: orientation \"%s\" unknown, must be either rh, lh, inside or outside", orientation, RI_NULL);
	}
	restoreOrientation(prevOrientation);

	m_callri->doOrientation(orientation);
}

RtVoid TIntermediateRenderer::reverseOrientation(void) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiReverseOrientation(lineNo()));
	}

	if ( !isValid(IDX_REVERSE_ORIENTATION, "TIntermediateRenderer::reverseOrientation()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	bool prevOrientation = attributes().m_currentOrientation;

	if ( attributes().m_orientation == RI_OUTSIDE )
		attributes().m_orientation = RI_INSIDE;
	else if ( attributes().m_orientation == RI_INSIDE )
		attributes().m_orientation = RI_OUTSIDE;
	else if ( attributes().m_orientation == RI_LH )
		attributes().m_orientation = RI_RH;
	else if ( attributes().m_orientation == RI_RH )
		attributes().m_orientation = RI_LH;
	else
		handleError(RIE_BUG, RIE_ERROR, "RiReverseOrientation: current orientation \"%s\" unknown, must be either rh, lh, inside or outside", attributes().m_orientation, RI_NULL);

	restoreOrientation(prevOrientation);

	m_callri->doReverseOrientation();
}

RtVoid TIntermediateRenderer::sides(RtInt nsides) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
		m_currentContext->m_macroWrite->add(newRiSides(lineNo(), nsides));
	}

	if ( !isValid(IDX_SIDES, "TIntermediateRenderer::sides()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	attributes().m_nSides = nsides;

	m_callri->doSides(nsides);
}

///////////////////////////////////////////////////////////////////////////////
// Coordinate system

RtVoid TIntermediateRenderer::coordinateSystem(RtToken space) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiCoordinateSystem(lineNo(), space));
	}

	if ( !isValid(IDX_COORDINATE_SYSTEM, "TIntermediateRenderer::coordinateSystem()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	m_currentContext->m_mapCoordSys[space] = CTM();
	m_callri->doCoordinateSystem(space);
}

RtVoid TIntermediateRenderer::coordSysTransform(RtToken space) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiCoordSysTransform(lineNo(), space));
	}

	if ( !isValid(IDX_COORD_SYS_TRANSFORM, "TIntermediateRenderer::coordSysTransform()") )
		return;

	// if ( isDefiningObject() )
	// 	return;

	STRING2MATRIX::iterator i;

	i = m_currentContext->m_mapCoordSys.find(space);
	if ( i != m_currentContext->m_mapCoordSys.end() ) {
		CTM() = i->second;
	} else {
		handleError(RIE_NESTING, RIE_ERROR, "RiCoordSysTransform(): %s", space);
	}

	m_callri->doCoordSysTransform(space);
}

RtPoint *TIntermediateRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	// No macro

	if ( !isValid(IDX_TRANSFORM_POINTS, "TIntermediateRenderer::transformPoints()") )
		return NULL;

	if ( npoints <= 0 || fromspace == RI_NULL || tospace == RI_NULL )
		return NULL;

	STRING2MATRIX::iterator i;
	TMatrix3D fromMatrix, toMatrix;
	RtMatrix invmat;
	int k;

	if ( strcmp(fromspace, RI_CURRENT) && strcmp(fromspace, RI_OBJECT) ) {
		i = m_currentContext->m_mapCoordSys.find(fromspace);
		if ( i != m_currentContext->m_mapCoordSys.end() ) {
			fromMatrix = i->second;
		} else {
			handleError(RIE_CONSISTENCY, RIE_ERROR, "TIntermediateRenderer::transformPoints(): Could not find fromspace %s", fromspace);
			return NULL;
		}
	} else {
		fromMatrix = CTM();
	}

	if ( strcmp(tospace, RI_CURRENT) && strcmp(tospace, RI_OBJECT) ) {
		i = m_currentContext->m_mapCoordSys.find(tospace);
		if ( i != m_currentContext->m_mapCoordSys.end() ) {
			toMatrix = i->second;
		} else {
			handleError(RIE_CONSISTENCY, RIE_ERROR, "TIntermediateRenderer::transformPoints(): Could not find tospace %s", tospace);
			return NULL;
		}
	} else {
		toMatrix = CTM();
	}

	if ( fromMatrix != toMatrix ) {
		if ( !fromMatrix.getInverse(invmat) ) {
			handleError(RIE_MATH, RIE_ERROR, "TIntermediateRenderer::transformPoints(): Could not calc inverse for requested matrix.");
			return NULL;
		}
		toMatrix.concatTransform(invmat);

		for ( k = 0; k < npoints; ++k ) {
			toMatrix.transformPoints(points[k][0], points[k][1], points[k][2]);
		}
	}

	return m_callri->doTransformPoints(fromspace, tospace, npoints, points);
}

RtVoid TIntermediateRenderer::attributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);
	bool catchError;

	TRiCPPBinding::attributeV(name, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::attributeV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiAttribute(lineNo(), getDeclMap(), curColorComponents(), name, n, tokens, params));
		}
	}

	if ( !isValid(IDX_ATTRIBUTE, "TIntermediateRenderer::attributeV()") )
		return;

	if ( !name ) {
		handleError(RIE_SYNTAX, RIE_ERROR, "TIntermediateRenderer::attributeV(): No name specified.");
		return;
	}

	// if ( isDefiningObject() )
	// 	return;

	// Insert attribute
	catchError = false;
	try {
		attributes().m_attributeMap.setOption(getDeclMap(), options().m_iNColor, name, n, tokens, params);
	} catch (TSystemException &e) {
		catchError = true;
		handleError(RIE_NOMEM, RIE_SEVERE, "%s in TIntermediateRenderer::attributeV()(%s...)", e.what(), name);
	}

	if ( catchError ) {
		return;
	}

	m_callri->doAttributeV(name, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Geometric primitives


RtVoid TIntermediateRenderer::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::polygonV(nvertices, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::polygonV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiPolygon(lineNo(), getDeclMap(), curColorComponents(), nvertices, n, tokens, params));
		}
	}

	if ( !isValid(IDX_POLYGON, "TIntermediateRenderer::polygonV()") )
		return;

	if ( isDefiningObject() )
		return;

	// Find out the vertices 'P' and the normals 'n'
	// if there are none, calc them using the cross product of two edges
	// -> there must be at least 3 vertices

	m_callri->doPolygonV(nvertices, n, tokens, params);
}

RtVoid TIntermediateRenderer::generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::generalPolygonV(nloops, nverts, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::generalPolygonV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiGeneralPolygon(lineNo(), getDeclMap(), curColorComponents(), nloops, nverts, n, tokens, params));
		}
	}

	if ( !isValid(IDX_GENERAL_POLYGON, "TIntermediateRenderer::generalPolygonV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doGeneralPolygonV(nloops, nverts, n, tokens, params);
}

RtVoid TIntermediateRenderer::pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::pointsPolygonsV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiPointsPolygons(lineNo(), getDeclMap(), curColorComponents(), npolys, nverts, verts, n, tokens, params));
		}
	}

	if ( !isValid(IDX_POINTS_POLYGONS, "TIntermediateRenderer::pointsPolygons()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doPointsPolygonsV(npolys, nverts, verts, n, tokens, params);
}

RtVoid TIntermediateRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::pointsGeneralPolygonsV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiPointsGeneralPolygons(lineNo(), getDeclMap(), curColorComponents(), npolys, nloops, nverts, verts, n, tokens, params));
		}
	}

	if ( !isValid(IDX_POINTS_GENERAL_POLYGONS, "TIntermediateRenderer::pointsGeneralPolygons()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doPointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
}


RtVoid TIntermediateRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiBasis(lineNo(), ubasis, ustep, vbasis, vstep));
	}

	/*
	if ( m_currentContext && m_currentContext->m_macroWrite ) {
		size_t size = m_currentContext->m_macroWrite->m_uStep.size();
		if ( size > 0 )
			m_currentContext->m_macroWrite->m_uStep[size-1] = ustep;

		size = m_currentContext->m_macroWrite->m_vStep.size();
		if ( size > 0 )
			m_currentContext->m_macroWrite->m_vStep[size-1] = vstep;

		size = m_currentContext->m_macroWrite->m_uBasis.size();
		if ( size > 0 && ubasis )
			m_currentContext->m_macroWrite->m_uBasis[size-1] = ubasis;

		size = m_currentContext->m_macroWrite->m_vBasis.size();
		if ( size > 0 && vbasis )
			m_currentContext->m_macroWrite->m_vBasis[size-1] = vbasis;
	}
	*/

	if ( !isValid(IDX_BASIS, "TIntermediateRenderer::basis()") )
		return;

	// if ( isDefiningObject() )
	//  return;

	if ( ubasis ) {
		memcpy(attributes().m_uBasis, ubasis, sizeof(RtBasis));
		attributes().m_uStep = ustep;
	}

	if ( vbasis ) {
		memcpy(attributes().m_vBasis, vbasis, sizeof(RtBasis));
		attributes().m_vStep = vstep;
	}

	m_callri->doBasis(ubasis, ustep, vbasis, vstep);
}

RtVoid TIntermediateRenderer::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::patchV(type, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::patchV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiPatch(lineNo(), getDeclMap(), curColorComponents(), type, n, tokens, params));
		}
	}

	if ( !isValid(IDX_PATCH, "TIntermediateRenderer::patchV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doPatchV(type, n, tokens, params);
}

RtVoid TIntermediateRenderer::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::patchMeshV() - Stacks empty");
		} else {
			m_currentContext->m_macroWrite->add(newRiPatchMesh(lineNo(), getDeclMap(), curColorComponents(), curUStep(), curVStep(), type, nu, uwrap, nv, vwrap, n, tokens, params));
		}
	}

	if ( !isValid(IDX_PATCH_MESH, "TIntermediateRenderer::patchMeshV()") )
		return;

	if ( isDefiningObject() )
	 	return;

	m_callri->doPatchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
}

RtVoid TIntermediateRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::nuPatchV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiNuPatch(lineNo(), getDeclMap(), curColorComponents(), nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_NU_PATCH, "TIntermediateRenderer::nuPatchV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doNuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
}

RtVoid TIntermediateRenderer::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::curvesV(type, ncurves, nverts, wrap, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::curvesV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiCurves(lineNo(), getDeclMap(), curColorComponents(), curUStep(), curVStep(), type, ncurves, nverts, wrap, n, tokens, params));
		}
	}

	if ( !isValid(IDX_CURVES, "TIntermediateRenderer::curvesV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doCurvesV(type, ncurves, nverts, wrap, n, tokens, params);
}

RtVoid TIntermediateRenderer::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiTrimCurve(lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w));
	}

	if ( !isValid(IDX_TRIM_CURVE, "TIntermediateRenderer::trimCurve()") )
		return;

	if ( isDefiningObject() )
		return;

	if ( nloops > 0 )
		attributes().m_trimCurve.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	else
		attributes().m_trimCurve.releaseAll();

	m_callri->doTrimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
}

RtVoid TIntermediateRenderer::procedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiProcedural(lineNo(), data, bound, subdivfunc, freefunc, *this));
	}

	if ( !isValid(IDX_PROCEDURAL, "TIntermediateRenderer::procedural()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doProcedural(data, bound, subdivfunc, freefunc);
}


RtVoid TIntermediateRenderer::handleDelayedReadArchive(RtPointer data, RtFloat detail) {
	// no macro

	TRiCPPBinding::handleDelayedReadArchive(data, detail);

//	if ( !isValid(IDX_PROC_DELAYED_READ_ARCHIVE, "TIntermediateRenderer::handleDelayedReadArchive()") )
//		return;
}

RtVoid TIntermediateRenderer::handleRunProgram(RtPointer data, RtFloat detail) {
	// no macro

	TRiCPPBinding::handleRunProgram(data, detail);

//	if ( !isValid(IDX_PROC_RUN_PROGRAM, "TIntermediateRenderer::handleRunProgram()") )
//		return;
}

RtVoid TIntermediateRenderer::handleDynamicLoad(RtPointer data, RtFloat detail) {
	// no macro

	TRiCPPBinding::handleDynamicLoad(data, detail);

//	if ( !isValid(IDX_PROC_DYNAMIC_LOAD, "TIntermediateRenderer::handleDynamicLoad()") )
//		return;
}

RtVoid TIntermediateRenderer::handleFree(RtPointer data) {
	// no macro

	TRiCPPBinding::handleFree(data);

//	if ( !isValid(IDX_PROC_FREE, "TIntermediateRenderer::handleFree()") )
//		return;
}



///////////////////////////////////////////////////////////////////////////////
// Quadrics

RtVoid TIntermediateRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::coneV(height, radius, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::coneV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiCone(lineNo(), getDeclMap(), curColorComponents(), height, radius, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_CONE, "TIntermediateRenderer::coneV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doConeV(height, radius, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	// Macro?
	TRiCPPBinding::cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::cylinderV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiCylinder(lineNo(), getDeclMap(), curColorComponents(), radius, zmin, zmax, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_CYLINDER, "TIntermediateRenderer::cylinderV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doCylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::diskV(height, radius, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::diskV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiDisk(lineNo(), getDeclMap(), curColorComponents(), height, radius, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_DISK, "TIntermediateRenderer::diskV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doDiskV(height, radius, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::hyperboloidV(point1, point2, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::hyperboloidV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiHyperboloid(lineNo(), getDeclMap(), curColorComponents(), point1, point2, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_HYPERBOLOID, "TIntermediateRenderer::hyperboloidV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doHyperboloidV(point1, point2, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::paraboloidV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiParaboloid(lineNo(), getDeclMap(), curColorComponents(), rmax, zmin, zmax, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_PARABOLOID, "TIntermediateRenderer::paraboloidV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doParaboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::sphereV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiSphere(lineNo(), getDeclMap(), curColorComponents(), radius, zmin, zmax, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_SPHERE, "sphereV") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doSphereV(radius, zmin, zmax, thetamax, n, tokens, params);
}

RtVoid TIntermediateRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::torusV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiTorus(lineNo(), getDeclMap(), curColorComponents(), majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params));
		}
	}

	if ( !isValid(IDX_TORUS, "TIntermediateRenderer::torusV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doTorusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////
// teapot() - Call interface routines to write out the teapot geometry
void TIntermediateRenderer::teapot() {
	RtToken ri_p[1] = { RI_P };
	RtPointer patch[1];

	attributeBegin();

    basis(bezierBasis, RI_BEZIERSTEP, bezierBasis, RI_BEZIERSTEP);

	RtFloat patch_1[] = {
		 1.5F,       0.0F,      0.0F,      1.5F,       0.828427F, 0.0F,      0.828427F,  1.5F,      0.0F,      0.0F,       1.5F,      0.0F,
		-0.828427F,  1.5F,      0.0F,     -1.5F,       0.828427F, 0.0F,     -1.5F,       0.0F,      0.0F,     -1.5F,      -0.828427F, 0.0F,
		-0.828427F, -1.5F,      0.0F,      0.0F,      -1.5F,      0.0F,      0.828427F, -1.5F,      0.0F,      1.5F,      -0.828427F, 0.0F,
		 1.5F,       0.0F,      0.0F,      1.5F,       0.0F,      0.075F,    1.5F,       0.828427F, 0.075F,    0.828427F,  1.5F,      0.075F,

		 0.0F,       1.5F,      0.075F,   -0.828427F,  1.5F,      0.075F,   -1.5F,       0.828427F, 0.075F,   -1.5F,       0.0F,      0.075F,
		-1.5F,      -0.828427F, 0.075F,   -0.828427F, -1.5F,      0.075F,    0.0F,      -1.5F,      0.075F,    0.828427F, -1.5F,      0.075F,
		 1.5F,      -0.828427F, 0.075F,    1.5F,       0.0F,      0.075F,    2.0F,       0.0F,      0.3F,      2.0F,       1.10457F,  0.3F,
		 1.10457F,   2.0F,      0.3F,      0.0F,       2.0F,      0.3F,     -1.10457F,   2.0F,      0.3F,     -2.0F,       1.10457F,  0.3F,

		-2.0F,       0.0F,      0.3F,     -2.0F,      -1.10457F,  0.3F,     -1.10457F,  -2.0F,      0.3F,      0.0F,      -2.0F,      0.3F,
		 1.10457F,  -2.0F,      0.3F,      2.0F,      -1.10457F,  0.3F,      2.0F,       0.0F,      0.3F,      2.0F,       0.0F,      0.75F,
		 2.0F,       1.10457F,  0.75F,     1.10457F,   2.0F,      0.75F,     0.0F,       2.0F,      0.75F,    -1.10457F,   2.0F,      0.75F,
		-2.0F,       1.10457F,  0.75F,    -2.0F,       0.0F,      0.75F,    -2.0F,      -1.10457F,  0.75F,    -1.10457F,  -2.0F,      0.75F,

		 0.0F,      -2.0F,      0.75F,     1.10457F,  -2.0F,      0.75F,     2.0F,      -1.10457F,  0.75F,     2.0F,       0.0F,      0.75F,
		 2.0F,       0.0F,      1.2F,      2.0F,       1.10457F,  1.2F,      1.10457F,   2.0F,      1.2F,      0.0F,       2.0F,      1.2F,
		-1.10457F,   2.0F,      1.2F,     -2.0F,       1.10457F,  1.2F,     -2.0F,       0.0F,      1.2F,     -2.0F,      -1.10457F,  1.2F,
		-1.10457F,  -2.0F,      1.2F,      0.0F,      -2.0F,      1.2F,      1.10457F,  -2.0F,      1.2F,      2.0F,      -1.10457F,  1.2F,

		 2.0F,       0.0F,      1.2F,      1.75F,      0.0F,      1.725F,    1.75F,      0.966498F, 1.725F,    0.966498F,  1.75F,     1.725F,
		 0.0F,       1.75F,     1.725F,   -0.966498F,  1.75F,     1.725F,   -1.75F,      0.966498F, 1.725F,   -1.75F,      0.0F,      1.725F,
		-1.75F,     -0.966498F, 1.725F,   -0.966498F, -1.75F,     1.725F,    0.0F,      -1.75F,     1.725F,    0.966498F, -1.75F,     1.725F,
		 1.75F,     -0.966498F, 1.725F,    1.75F,      0.0F,      1.725F,    1.5F,       0.0F,      2.25F,     1.5F,       0.828427F, 2.25F,

		 0.828427F,  1.5F,      2.25F,     0.0F,       1.5F,      2.25F,    -0.828427F,  1.5F,      2.25F,    -1.5F,       0.828427F, 2.25F,
		-1.5F,       0.0F,      2.25F,    -1.5F,      -0.828427F, 2.25F,    -0.828427F, -1.5F,      2.25F,     0.0F,      -1.5F,      2.25F,
		 0.828427F, -1.5F,      2.25F,     1.5F,      -0.828427F, 2.25F,     1.5F,       0.0F,      2.25F,     1.4375F,    0.0F,      2.38125F,
		 1.4375F,    0.793909F, 2.38125F,  0.793909F,  1.4375F,   2.38125F,  0.0F,       1.4375F,   2.38125F, -0.793909F,  1.4375F,   2.38125F,

		-1.4375F,    0.793909F, 2.38125F, -1.4375F,    0.0F,      2.38125F, -1.4375F,   -0.793909F, 2.38125F, -0.793909F, -1.4375F,   2.38125F,
		 0.0F,      -1.4375F,   2.38125F,  0.793909F, -1.4375F,   2.38125F,  1.4375F,   -0.793909F, 2.38125F,  1.4375F,    0.0F,      2.38125F,
		 1.3375F,    0.0F,      2.38125F,  1.3375F,    0.738681F, 2.38125F,  0.738681F,  1.3375F,   2.38125F,  0.0F,       1.3375F,   2.38125F,
		-0.738681F,  1.3375F,   2.38125F, -1.3375F,    0.738681F, 2.38125F, -1.3375F,    0.0F,      2.38125F, -1.3375F,   -0.738681F, 2.38125F,

		-0.738681F, -1.3375F,   2.38125F,  0.0F,      -1.3375F,   2.38125F,  0.738681F, -1.3375F,   2.38125F,  1.3375F,   -0.738681F, 2.38125F,
		 1.3375F,    0.0F,      2.38125F,  1.4F,       0.0F,      2.25F,     1.4F,       0.773198F, 2.25F,     0.773198F,  1.4F,      2.25F,
		 0.0F,       1.4F,      2.25F,    -0.773198F,  1.4F,      2.25F,    -1.4F,       0.773198F, 2.25F,    -1.4F,       0.0F,      2.25F,
		-1.4F,      -0.773198F, 2.25F,    -0.773198F, -1.4F,      2.25F,     0.0F,      -1.4F,      2.25F,     0.773198F, -1.4F,      2.25F,

		 1.4F,      -0.773198F, 2.25F,     1.4F,       0.0F,      2.25F};
	patch[0] = (RtPointer)&patch_1[0];
	patchMeshV(RI_BICUBIC, 13, RI_NONPERIODIC, 10, RI_NONPERIODIC, 1, ri_p, patch); 

	RtFloat patch_2[] = {
		1.3F, 0.0F, 2.25F, 1.3F, 0.71797F, 2.25F, 0.71797F, 1.3F, 2.25F, 0.0F, 1.3F, 2.25F,
		-0.71797F, 1.3F, 2.25F, -1.3F, 0.71797F, 2.25F, -1.3F, 0.0F, 2.25F, -1.3F, -0.71797F, 2.25F,
		-0.71797F, -1.3F, 2.25F, 0.0F, -1.3F, 2.25F, 0.71797F, -1.3F, 2.25F, 1.3F, -0.71797F, 2.25F,
		1.3F, 0.0F, 2.25F, 1.3F, 0.0F, 2.4F, 1.3F, 0.71797F, 2.4F, 0.71797F, 1.3F, 2.4F,
		0.0F, 1.3F, 2.4F, -0.71797F, 1.3F, 2.4F, -1.3F, 0.71797F, 2.4F, -1.3F, 0.0F, 2.4F,
		-1.3F, -0.71797F, 2.4F, -0.71797F, -1.3F, 2.4F, 0.0F, -1.3F, 2.4F, 0.71797F, -1.3F, 2.4F,
		1.3F, -0.71797F, 2.4F, 1.3F, 0.0F, 2.4F, 0.4F, 0.0F, 2.4F, 0.4F, 0.220914F, 2.4F,
		0.220914F, 0.4F, 2.4F, 0.0F, 0.4F, 2.4F, -0.220914F, 0.4F, 2.4F, -0.4F, 0.220914F, 2.4F,
		-0.4F, 0.0F, 2.4F, -0.4F, -0.220914F, 2.4F, -0.220914F, -0.4F, 2.4F, 0.0F, -0.4F, 2.4F,
		0.220914F, -0.4F, 2.4F, 0.4F, -0.220914F, 2.4F, 0.4F, 0.0F, 2.4F, 0.2F, 0.0F, 2.55F,
		0.2F, 0.110457F, 2.55F, 0.110457F, 0.2F, 2.55F, 0.0F, 0.2F, 2.55F, -0.110457F, 0.2F, 2.55F,
		-0.2F, 0.110457F, 2.55F, -0.2F, 0.0F, 2.55F, -0.2F, -0.110457F, 2.55F, -0.110457F, -0.2F, 2.55F,
		0.0F, -0.2F, 2.55F, 0.110457F, -0.2F, 2.55F, 0.2F, -0.110457F, 2.55F, 0.2F, 0.0F, 2.55F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.8F, 0.0F, 3.0F, 0.8F, 0.441828F, 3.0F, 0.441828F, 0.8F, 3.0F,
		0.0F, 0.8F, 3.0F, -0.441828F, 0.8F, 3.0F, -0.8F, 0.441828F, 3.0F, -0.8F, 0.0F, 3.0F,
		-0.8F, -0.441828F, 3.0F, -0.441828F, -0.8F, 3.0F, 0.0F, -0.8F, 3.0F, 0.441828F, -0.8F, 3.0F,
		0.8F, -0.441828F, 3.0F, 0.8F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F
	};
	patch[0] = (RtPointer)&patch_2[0];
	patchMeshV(RI_BICUBIC, 13, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 

	RtFloat patch_3[] = {
		-2.0F, 0.0F, 0.75F, -2.0F, 0.3F, 0.75F, -1.9F, 0.3F, 0.45F, -1.9F, 0.0F, 0.45F,
		-2.5F, 0.0F, 0.975F, -2.5F, 0.3F, 0.975F, -2.65F, 0.3F, 0.7875F, -2.65F, 0.0F, 0.7875F,
		-2.7F, 0.0F, 1.425F, -2.7F, 0.3F, 1.425F, -3.0F, 0.3F, 1.2F, -3.0F, 0.0F, 1.2F,
		-2.7F, 0.0F, 1.65F, -2.7F, 0.3F, 1.65F, -3.0F, 0.3F, 1.65F, -3.0F, 0.0F, 1.65F,
		-2.7F, 0.0F, 1.875F, -2.7F, 0.3F, 1.875F, -3.0F, 0.3F, 2.1F, -3.0F, 0.0F, 2.1F,
		-2.3F, 0.0F, 1.875F, -2.3F, 0.3F, 1.875F, -2.5F, 0.3F, 2.1F, -2.5F, 0.0F, 2.1F,
		-1.6F, 0.0F, 1.875F, -1.6F, 0.3F, 1.875F, -1.5F, 0.3F, 2.1F, -1.5F, 0.0F, 2.1F
	};
	patch[0] = (RtPointer)&patch_3[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 

	RtFloat patch_4[] = {
		2.8F, 0.0F, 2.25F, 2.8F, 0.15F, 2.25F, 3.2F, 0.15F, 2.25F, 3.2F, 0.0F, 2.25F,
		2.9F, 0.0F, 2.325F, 2.9F, 0.25F, 2.325F, 3.45F, 0.15F, 2.3625F, 3.45F, 0.0F, 2.3625F,
		2.8F, 0.0F, 2.325F, 2.8F, 0.25F, 2.325F, 3.525F, 0.25F, 2.34375F, 3.525F, 0.0F, 2.34375F,
		2.7F, 0.0F, 2.25F, 2.7F, 0.25F, 2.25F, 3.3F, 0.25F, 2.25F, 3.3F, 0.0F, 2.25F,
		2.3F, 0.0F, 1.95F, 2.3F, 0.25F, 1.95F, 2.4F, 0.25F, 1.875F, 2.4F, 0.0F, 1.875F,
		2.6F, 0.0F, 1.275F, 2.6F, 0.66F, 1.275F, 3.1F, 0.66F, 0.675F, 3.1F, 0.0F, 0.675F,
		1.7F, 0.0F, 1.275F, 1.7F, 0.66F, 1.275F, 1.7F, 0.66F, 0.45F, 1.7F, 0.0F, 0.45F
	};
	patch[0] = (RtPointer)&patch_4[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 

	RtFloat patch_5[] = {
		-1.9F, 0.0F, 0.45F, -1.9F, -0.3F, 0.45F, -2.0F, -0.3F, 0.75F, -2.0F, 0.0F, 0.75F,
		-2.65F, 0.0F, 0.7875F, -2.65F, -0.3F, 0.7875F, -2.5F, -0.3F, 0.975F, -2.5F, 0.0F, 0.975F,
		-3.0F, 0.0F, 1.2F, -3.0F, -0.3F, 1.2F, -2.7F, -0.3F, 1.425F, -2.7F, 0.0F, 1.425F,
		-3.0F, 0.0F, 1.65F, -3.0F, -0.3F, 1.65F, -2.7F, -0.3F, 1.65F, -2.7F, 0.0F, 1.65F,
		-3.0F, 0.0F, 2.1F, -3.0F, -0.3F, 2.1F, -2.7F, -0.3F, 1.875F, -2.7F, 0.0F, 1.875F,
		-2.5F, 0.0F, 2.1F, -2.5F, -0.3F, 2.1F, -2.3F, -0.3F, 1.875F, -2.3F, 0.0F, 1.875F,
		-1.5F, 0.0F, 2.1F, -1.5F, -0.3F, 2.1F, -1.6F, -0.3F, 1.875F, -1.6F, 0.0F, 1.875F
	};
	patch[0] = (RtPointer)&patch_5[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 

	RtFloat patch_6[] = {
		3.2F, 0.0F, 2.25F, 3.2F, -0.15F, 2.25F, 2.8F, -0.15F, 2.25F, 2.8F, 0.0F, 2.25F,
		3.45F, 0.0F, 2.3625F, 3.45F, -0.15F, 2.3625F, 2.9F, -0.25F, 2.325F, 2.9F, 0.0F, 2.325F,
		3.525F, 0.0F, 2.34375F, 3.525F, -0.25F, 2.34375F, 2.8F, -0.25F, 2.325F, 2.8F, 0.0F, 2.325F,
		3.3F, 0.0F, 2.25F, 3.3F, -0.25F, 2.25F, 2.7F, -0.25F, 2.25F, 2.7F, 0.0F, 2.25F,
		2.4F, 0.0F, 1.875F, 2.4F, -0.25F, 1.875F, 2.3F, -0.25F, 1.95F, 2.3F, 0.0F, 1.95F,
		3.1F, 0.0F, 0.675F, 3.1F, -0.66F, 0.675F, 2.6F, -0.66F, 1.275F, 2.6F, 0.0F, 1.275F,
		1.7F, 0.0F, 0.45F, 1.7F, -0.66F, 0.45F, 1.7F, -0.66F, 1.275F, 1.7F, 0.0F, 1.275F
	};
	patch[0] = (RtPointer)&patch_6[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 
	attributeEnd();
}

RtVoid TIntermediateRenderer::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::geometryV(type, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::geometryV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiGeometry(lineNo(), getDeclMap(), curColorComponents(), type, n, tokens, params));
		}
	}

	if ( !isValid(IDX_GEOMETRY, "TIntermediateRenderer::geometryV()") )
		return;

	if ( isDefiningObject() )
		return;

	// teapot() for type == teapot is not called here, because you can think of implementations,
	// where this is not needed or even wrong

	++m_currentContext->m_stopRecord;
	m_callri->doGeometryV(type, n, tokens, params);
	--m_currentContext->m_stopRecord;
}

///////////////////////////////////////////////////////////////////////////////

RtVoid TIntermediateRenderer::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::pointsV(npts, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::pointsV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiPoints(lineNo(), getDeclMap(), curColorComponents(), npts, n, tokens, params));
		}
	}

	if ( !isValid(IDX_POINTS, "TIntermediateRenderer::pointsV()") )
		return;

	if ( isDefiningObject() )
		return;

	++m_currentContext->m_stopRecord;
	m_callri->doPointsV(npts, n, tokens, params);
	--m_currentContext->m_stopRecord;
}

RtVoid TIntermediateRenderer::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::subdivisionMeshV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiSubdivisionMesh(lineNo(), getDeclMap(), curColorComponents(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params));
		}
	}

	if ( !isValid(IDX_SUBDIVISION_MESH, "TIntermediateRenderer::subdivisionMeshV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
}

RtVoid TIntermediateRenderer::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::blobbyV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiBlobby(lineNo(), getDeclMap(), curColorComponents(), nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params));
		}
	}

	if ( !isValid(IDX_BLOBBY, "TIntermediateRenderer::blobbyV()") )
		return;

	if ( isDefiningObject() )
		return;

	++m_currentContext->m_stopRecord;
	m_callri->doBlobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	--m_currentContext->m_stopRecord;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Texture

RtVoid TIntermediateRenderer::makeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::makeTextureV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiMakeTexture(lineNo(), getDeclMap(), curColorComponents(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params));
		}
	}

	if ( !isValid(IDX_MAKE_TEXTURE, "TIntermediateRenderer::makeTextureV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doMakeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
}

RtVoid TIntermediateRenderer::makeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::makeBumbV() - Stacks empty");
		} else {
 			m_currentContext->m_macroWrite->add(newRiMakeBump(lineNo(), getDeclMap(), curColorComponents(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params));
		}
	}

	if ( !isValid(IDX_MAKE_BUMP, "TIntermediateRenderer::makeBumbV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doMakeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
}

RtVoid TIntermediateRenderer::makeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::makeLatLongEnvironmentV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiMakeLatLongEnvironment(lineNo(), getDeclMap(), curColorComponents(), pic, tex, filterfunc, swidth, twidth, n, tokens, params));
		}
	}

	if ( !isValid(IDX_MAKE_LAT_LONG_ENVIRONMENT, "TIntermediateRenderer::makeLatLongEnvironmentV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doMakeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
}

RtVoid TIntermediateRenderer::makeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::makeCubeFaceEnvironmentV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiMakeCubeFaceEnvironment(lineNo(), getDeclMap(), curColorComponents(), px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params));
		}
	}

	if ( !isValid(IDX_MAKE_CUBE_FACE_ENVIRONMENT, "TIntermediateRenderer::makeCubeFaceEnvironmentV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doMakeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
}

RtVoid TIntermediateRenderer::makeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::makeShadowV(pic, tex, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::makeShadowV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiMakeShadow(lineNo(), getDeclMap(), curColorComponents(), pic, tex, n, tokens, params));
		}
	}

	if ( !isValid(IDX_MAKE_SHADOW, "TIntermediateRenderer::makeShadowV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doMakeShadowV(pic, tex, n, tokens, params);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Error handling

RtVoid TIntermediateRenderer::errorHandler(RtErrorHandler handler) {
	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiErrorHandler(lineNo(), handler));
	}

//	if ( !isValid(IDX_ERROR_HANDLER, "TIntermediateRenderer::errorHandler()") )
//		return;
	TRiCPPBinding::errorHandler(handler);

	// if ( isDefiningObject() )
	// 	return;

	m_callri->doErrorHandler(handler);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Archives

RtVoid TIntermediateRenderer::archiveRecordV(RtToken type, const char *line) {
	assert(m_currentContext != NULL);

	// Macro?
	if ( isRecording() ) {
 		m_currentContext->m_macroWrite->add(newRiArchiveRecord(lineNo(), type, line));
	}

	if ( !isValid(IDX_ARCHIVE_RECORD, "TIntermediateRenderer::archiveRecordV()") )
		return;

	if ( isDefiningObject() )
		return;

	m_callri->doArchiveRecordV(type, line);
}


RtVoid TIntermediateRenderer::readArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) {
	assert(m_currentContext != NULL);

	TRiCPPBinding::readArchiveV(filename, callback, n, tokens, params);
	// Macro?
	if ( isRecording() ) {
		if ( !m_currentContext->m_macroWrite->valid() ) {
			handleError(RIE_ILLSTATE, RIE_ERROR, "TIntermediateRenderer::readArchiveV() - Stacks empty");
		} else {
	 		m_currentContext->m_macroWrite->add(newRiReadArchive(lineNo(), getDeclMap(), curColorComponents(), filename, callback, n, tokens, params));
		}
	}
	if ( !isValid(IDX_READ_ARCHIVE, "TIntermediateRenderer::readArchiveV()") )
		return;

	m_callri->doReadArchiveV(filename, callback, n, tokens, params);
}


void TIntermediateRenderer::handleReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !isValid() )
		return;

	// No Filename
	if ( !filename || !filename[0] ) {
		handleError(RIE_NOFILE, RIE_ERROR, "Cannot open file with no name.", RI_NULL);
		return;
	}

	std::string pathName = filename;
	m_currentContext->m_pathStack.expandPath(pathName);

	STRING2RIMACRO::iterator i = m_currentContext->m_macros.find(pathName.c_str());
	if ( i != m_currentContext->m_macros.end() ) {
		// File already parsed - replay it
		TRiMacro *cur = i->second;
		if ( !cur ) {
			handleError(RIE_BUG, RIE_ERROR, "File \"%s\" already parsed but definition is NULL", filename, RI_NULL);
			return;
		}
		if ( cur->isDefining() ) {
			handleError(RIE_NESTING, RIE_ERROR, "ReadArchive for \"%s\" called within the archive (no recursion allowed)", filename, RI_NULL);
			return;
		}
		if ( !cur->valid() ) {
			handleError(RIE_NESTING, RIE_ERROR, "Archive \"%s\" was not parsed successfully, it is not replayed.", filename, RI_NULL);
			return;
		}

		// Remember the current filename
		m_currentContext->m_pathStack.push(pathName.c_str());
		++m_currentContext->m_replay;
		cur->replay(*this, *this, callback, getDrawPrim());
		--m_currentContext->m_replay;
		// Restore the filename used before
		m_currentContext->m_pathStack.pop();

		return;
	}

	// File is not parsed - parse it, the calls are stored and carried out while parsing.
	bool cacheArchive = true;

	RtInt ii;
	for ( ii = 0; ii < n; ++ii ) {
		if ( tokens[ii] && !strcmp(ri_RiCPPCacheArchive, tokens[ii]) ) {
			RtBoolean *p = (RtBoolean *)(params[ii]);
			cacheArchive = p && *p;
		}
	}

	// Remember the current filename
	m_currentContext->m_pathStack.push(pathName.c_str());

	TRibParser *ribParser = new TRibParser(pathName.c_str(), *this, *this, callback, n, tokens, params);
	if ( ribParser == NULL ) {
		handleError(RIE_NOMEM, RIE_ERROR, "File \"%s\" could not be parsed because out of memory for parsing.", filename, RI_NULL);
		return;
	}

	// Get new macro if archive should be cached
	if ( cacheArchive ) {
		TRiMacro *newMacro = new TRiMacro(filename, false);
		if ( newMacro == NULL ) {
			delete ribParser;
			handleError(RIE_NOMEM, RIE_ERROR, "File \"%s\" could not be parsed because out of memory for macro recording.", filename, RI_NULL);
			return;
		}
		m_currentContext->m_macroStack.push_back(m_currentContext->m_macroWrite);
		m_currentContext->m_macroWrite = newMacro;
		// Remember macro
		m_currentContext->m_macros[pathName.c_str()] = newMacro;
		++m_currentContext->m_readArchive;

		bool parsedfile = ribParser->parseFile();
		delete ribParser;

		--m_currentContext->m_readArchive;
		newMacro->valid(parsedfile);
		newMacro->endMacro();
		m_currentContext->m_macroWrite = m_currentContext->m_macroStack.back();
		m_currentContext->m_macroStack.pop_back();

		/*
		// Replay if the last file is closed
		if ( m_currentContext->m_readArchive == 0 && newMacro->valid() ) {
			++m_currentContext->m_replay;
			newMacro->replay(*this, *this, callback, getDrawPrim());
			--m_currentContext->m_replay;
		}
		*/

		m_currentContext->m_pathStack.pop();

		if ( !newMacro->valid() ) {
			handleError(RIE_NOFILE, RIE_ERROR, "Cannot open or error while parsing \"%s\"", filename, RI_NULL);
		}
	} else {
		bool parsedfile = ribParser->parseFile();
		parsedfile = parsedfile; // For debuggin purposes.
		delete ribParser;
		m_currentContext->m_pathStack.pop();
	}
}


RtObjectHandle TIntermediateRenderer::handleObjectBegin(RtObjectHandle handle) {

	if ( m_currentContext != NULL && m_currentContext->m_currentAttributes != NULL ) {
		char buffer[64];
		sprintf(buffer, "Object:%lu", (unsigned long)handle);
		m_currentContext->m_macroStack.push_back(m_currentContext->m_macroWrite);
		m_currentContext->m_macroWrite = new TRiMacro(buffer, true);
		if ( m_currentContext->m_macroWrite ) {
			// Push declarations
			getDeclMap().push();
			// Push options
			m_currentContext->pushOptions(*this);
			// Push attributes
			m_currentContext->pushAttributes(*this);
			m_currentContext->m_objects[buffer] = m_currentContext->m_macroWrite;
			++m_currentContext->m_defineObject;
			return handle;
		} else {
			handleError(RIE_NOMEM, RIE_ERROR, "Object \"%s\" could not be defined because out of memory for macro recording.", buffer, RI_NULL);
		}
	}

	return (RtObjectHandle)-1L;
}


void TIntermediateRenderer::handleObjectEnd(void) {
	assert(m_currentContext != NULL);
	if ( m_currentContext && m_currentContext->m_macroWrite && m_currentContext->m_macroWrite->isObject() ) {
		if ( !m_currentContext->m_macroStack.empty() ) {
			m_currentContext->m_macroWrite = m_currentContext->m_macroStack.back();
			m_currentContext->m_macroStack.pop_back();
			--m_currentContext->m_defineObject;
			// Pop declarations
			getDeclMap().pop();
			// Pop options
			m_currentContext->popOptions(*this);
			// Pop attributes
			m_currentContext->popAttributes(*this);
		} else {
			m_currentContext->m_macroWrite = NULL;
		}
	}
}


void TIntermediateRenderer::handleObjectInstance(RtObjectHandle handle) {
	assert(m_currentContext != NULL);

	char buffer[64];
	sprintf(buffer, "Object:%lu", (unsigned long)handle);

	STRING2RIMACRO::iterator i = m_currentContext->m_objects.find(buffer);
	if ( i != m_currentContext->m_objects.end() ) {
		TRiMacro *cur = i->second;
		++m_currentContext->m_replay;
		cur->replay(*this, *this, NULL, getDrawPrim());
		--m_currentContext->m_replay;
	} else {
		handleError(RIE_NOFILE, RIE_ERROR, "Cannot find macro for an object %s.", RI_NULL, buffer);
	}
}


bool TIntermediateRenderer::queryParam(
	const char *decl,
	RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners,
	RtInt &aClass, RtInt &aType, RtInt &aCardinality, RtInt &numComps, IndexType &numBytes)
{

	TDeclarationMap &declmap = getDeclMap();
	const TParameterDeclaration *pd = declmap.findDeclaration(decl);
	RtInt colorComps = curColorComponents();

	TParameterDeclaration paramdecl;
	if ( !pd ) {
		// Maybe an inline definition ?
		if ( !paramdecl.parse(NULL, decl) )
			return false;
		pd = &paramdecl;
	}

	aClass =       pd->getClass();
	aType  =       pd->getType();
	aCardinality = pd->getCardinality();
	numComps =     pd->numComponents(vertices, corners, facets, faceVertices, faceCorners, colorComps);
	numBytes =     pd->size(vertices, corners, facets, faceVertices, faceCorners, colorComps);

	return true;
}


RtInt TIntermediateRenderer::curUStep() const {
	assert(m_currentContext != NULL);

	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	//	m_currentContext->m_macroWrite->m_uStep.back();
	// }

	assert(m_currentContext->m_currentAttributes != NULL);
	return attributes().m_uStep;
}


RtInt TIntermediateRenderer::curVStep() const {
	assert(m_currentContext != NULL);

	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	//	m_currentContext->m_macroWrite->m_vStep.back();
	// }

	assert(m_currentContext->m_currentAttributes != NULL);
	return attributes().m_vStep;
}


const RtBasis *TIntermediateRenderer::curUBasis() const {
	assert(m_currentContext != NULL);

	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	// 	&(m_currentContext->m_macroWrite->m_uBasis.back());
	// }

	assert(m_currentContext->m_currentAttributes != NULL);
	return (const RtBasis *)&attributes().m_uBasis;
}

const RtBasis *TIntermediateRenderer::curVBasis() const {
	assert(m_currentContext != NULL);

	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	// 	&(m_currentContext->m_macroWrite->m_vBasis.back());
	// }

	assert(m_currentContext->m_currentAttributes != NULL);
	return (const RtBasis *)&attributes().m_vBasis;
}

RtFloat TIntermediateRenderer::curUTesselation() const {
	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	// 	m_currentContext->m_macroWrite->m_uTesselation.back();
	// }

	return 16;
}

RtFloat TIntermediateRenderer::curVTesselation() const {
	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	// 	m_currentContext->m_macroWrite->m_vTesselation.back();
	// }

	return 16;
}

TTrimCurveData *TIntermediateRenderer::curTrimCurve() const {
	assert(m_currentContext != NULL);

	// Macro?
	// if ( m_currentContext && m_currentContext->m_macroWrite ) {
	// 	&(m_currentContext->m_macroWrite->m_uBasis.back());
	// }

	assert(m_currentContext->m_currentAttributes != NULL);
	return &(attributes().m_trimCurve);
}

RtInt TIntermediateRenderer::curColorComponents() const {
	return attributes().m_colorComponents;
}
