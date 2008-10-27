#ifndef _RICPP_RENDERCONTEXT_RENDERCONTEXT_H
#define _RICPP_RENDERCONTEXT_RENDERCONTEXT_H

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

/*! \file rendercontext.h
 *  \brief Contains declarations of TRiCPPContext, TIntermediateRenderer and  TIntermediateRendererInterface.
 *         TRiCPPContext a class for all contextinformation used by TIntermediateRenderer.
 *         TIntermediateRenderer is the layer between abstract TRiCPPBinding, own renderers are noramlly subclasses of
 *         TIntermediateRenderer. TIntermediateRenderer calls after a success in verifying the parameters the interface
 *         call a TIntermediateRendererInterface method. If you write your own renderer class you can inherit from
 *         TIntermediateRenderer and TIntermediateRendererInterface, and implement the 'work' in overloaded
 *         TIntermediateRendererInterface methods. Subclasses can also use subclasses of TRiCPPContext,
 *         TRenderAttributes and TRenderObjects.
 *  \author Andreas Pidde (andreas@pidde.de)
 */


#include "renderoptions.h"
#include "renderattributes.h"
#include "pathstack.h"
#include "ricppbinding.h"
#include "parametric.h"
#include "polygons.h"
#include "subdivision.h"

//! Default frame/screen/window width
#define DEFAULT_SCREEN_WID 512
//! Default frame/screen/window height
#define DEFAULT_SCREEN_HT  384

//! Maps a string to a matrix (CTM) for named coordinate systems, \sa TRiCPPContext::m_mapCoordSys
typedef std::map<std::string, TMatrix3D, std::less<std::string> > STRING2MATRIX;

//! Maps a string to a macro for RIB archives and (RI)objects, \sa TRiCPPContext::m_macros TRiCPPContext::m_objects
typedef std::map<std::string, TRiMacro *, std::less<std::string> > STRING2RIMACRO;


//! Forward declaration of TIntermediateRenderer, because TIntermediateRenderer is used by TRiCPPContext and vice versa
class TIntermediateRenderer;

////////////////////////////////////////////////////////////////////////////////
//! State of a renderer (for multiple views)
/*!
 *  Class for render contexts, used for multiple views,
 *  contains all state data of a renderer.
 *  Higher level renderer objects will have contexts
 *  inheriting from TRiCPPContext. A virtual method
 *  TIntermediateRenderer::getNewContext returns an
 *  instance of the appropriate context. This is not
 *  a context like world block.
 */
class TRiCPPContext : public TBaseClass {
public:
	//! Linenumber in current RIB archive (-1 if none)
	long m_lineNo;

	//! Name of current RIB archive (if there is one)
	std::string m_ribArchive;

	//! Name of the context (parameter of begin)
	std::string m_name;

	//! List of all actual declarations
	TDeclarationMap m_declarationMap;

	//! To check validity of routines in a given context
	TValidContext m_contextCheck;

	//! Pathinfo to Rib-Archives, used a stack because archives may be nested
	TPathStack m_pathStack;

	//! Index numbers for retained geometry (counts from 0, incremented by one)
	size_t m_objectCnt;

	//! Replays an archive or object if m_replay > 0, number is the nesting depth
	RtInt m_replay;

	//! Reads an archive if m_readArchive > 0, number is the nesting depth
	RtInt m_readArchive;

	//! In object definition if m_defineObject > 0, number is the nesting depth
	RtInt m_defineObject;

	// Macro (for Rib Archives and retained geometry)
	RtInt m_stopRecord;						//!< Do not record primaries
	STRING2RIMACRO m_macros;				//!< List of RIB archives.
	STRING2RIMACRO m_objects;				//!< List of object blocks.
	TRiMacro *m_macroWrite;					//!< Current macro to store to.
	std::vector<TRiMacro *> m_macroStack;	//!< Stack for nested macros.

	//! Current block state (CTX_...) begin, motion, etc.
	RtInt m_iState;

	//! Stack of block state numbers (CTX_...).
	std::vector<RtInt> m_stateStack;

	//! Samples in motion blocks.
	std::vector<RtFloat> m_samples;

	//! Current solid operation.
	RtToken m_tSolid;

	//! Stack of solid operations.
	std::vector<RtToken> m_solidStack;

	//! Current frame number within a frame block.
	RtInt m_frameNumber;

	//! List of lights, the number of lights can only increase.
	/*! Lights are on when they are created. The are off outside
	 *  their attribute block. RiIlluminate() can be used to switch
	 *  lights on or off. The light handle is the index number.
	 *  The list of illuminated light sources is part of the
	 *  attribute set.
	 */
	std::vector<TLight> m_lightList;

	//! Option stack
	std::vector<TRenderOptions *> m_options;
	//! Current option set.
	TRenderOptions *m_currentOptions;

	//! Attribute stack
	std::vector<TRenderAttributes *> m_attributes;
	//! Current attribute set.
	TRenderAttributes *m_currentAttributes;

	// Transformations
	TRenderTransform *m_currentTransform;

	//! Maps name to coord sys(CTM).
	STRING2MATRIX m_mapCoordSys;

public:
	TRiCPPContext();          //!< Standard constructor, initializes an empty object
	virtual ~TRiCPPContext(); //!< Destructor, frees all resources used by a context

	//! The static class name as C string.
	/*! \return The classname of this class: "TRiCPPContext".
	 */
	inline static const char *myClassName(void) {return "TRiCPPContext";}

	//! The class name as C string.
	/*! \return The classname for this instance, here: "TRiCPPContext".
	 */
	inline virtual const char *className() const { return TRiCPPContext::myClassName(); }

	//! Inheritance, looks if instance is a kind of the class given by the name aClass.
	/*! \param aClass Class to test for inheritance.
	 *  \return true, if instances inherits from aClass.
	 */
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TRiCPPContext::myClassName()) ) {
			return true;
		}
		return TBaseClass::isKindOf(aClass);
	}

	//! Used to initialize a context, this is done by TRi::begin() or TRi::synchronize(TRi::RI_BEGIN)
	/*!
	 * \param ri Renderer, used for error handling and establishing of options and attributes.
	 * \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort()), if
	 *            there is not enough memory TRi::RIE_NOMEM.
	 */
	virtual void initialize(TIntermediateRenderer &ri);

	//! Checks if an interface call can be issued in a given context (e.g. call of TRi::sphere() in a world block)
	/*! \param idxRoutine The index of an interface call, constant IDX_..., e.g. IDX_SPHERE
	 *  \param idxContext The index of a context, constant CTX..., e.g. CTX_WORLD
	 *  \return true if call is valid, false call not valid
	 */
	virtual bool isValid(unsigned int idxRoutine, unsigned int idxContext);

	//! Pushes the number of a state (e.g. for an attribute block CTX_ATTRIBUTE) for nested sates (e.g. attribute block in a world block)
	/*! The state number is simply pushed to m_stateStack
	 *  \param idxState The index of a state, constant CTX..., e.g. CTX_ATTRIBUTE
	 */
	virtual void pushContextState(RtInt stateNum);

	//! Pops a state number
	/*! A state number is popped from m_stateStack. If the stack is empty: ricb.handleError() is called. This may cause
	 *  an exception.
	 */
	//! \param routineName Name of the interface call used to end a block (like "TIntermediateRenderer::attributeEnd()"), used if an error occures.
	//! \param ri For error handling.
	//! \return true, state number is popped, false: stack was empty.
	//! \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort()).
	virtual bool popContextState(const char *routineName, TIntermediateRenderer &ri);

	//! Pushes the type of a solid block for nested solid blocks.
	/*! Pushes the token to m_solidStack. The type of the block is not checked.
	 *  \param solidType Type of a solid block: RI_PRIMITIVE, RI_INTERSECTION, RI_UNION or RI_DIFFERENCE
	 */
	virtual void pushSolidState(RtToken solidType);

	//! Pops a type of a solid block
	/*! A context number is popped from m_solidStack. If the stack is empty: ricb.handleError() is called. This may cause
	 *  an exception.
	 *  \param routineName Name of the interface call used to end a block ("TIntermediateRenderer::solideEnd()"), used if an error occures.
	 *  \param ri For error handling.
	 *  \return true, solid type is popped, false: stack was empty.
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort()).
	 */
	virtual bool popSolidState(const char *routineName, TIntermediateRenderer &ri);

	//! Pushes the CTM of the current attributes, using the transformation stack of the current attribute set
	/*! 
	 *  \param ri For error handling.
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool pushTransformation(TIntermediateRenderer &ri);

	//! Pops the CTM for the current attributes, using the transformation stack of the current attribute set
	/*! 
	 *  \param ri For error handling and establishing the transformation
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool popTransformation(TIntermediateRenderer &ri);  

	//! Pushes the current attribute set, including the transformation to m_attributes
	/*! 
	 *  \param ri For error handling.
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool pushAttributes(TIntermediateRenderer &ri);

	//! Pops the current attribute set, including the transformation from m_attributes
	/*! 
	 *  \param ri For error handling and establishing the attributes
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool popAttributes(TIntermediateRenderer &ri);  

	//! Pushes the current option set to m_options
	/*! 
	 *  \param ri For error handling.
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool pushOptions(TIntermediateRenderer &ri);

	//! Pops the current option set from m_options
	/*! 
	 *  \param ri For error handling and establishing the options
	 *  \return true, if operation is performed
	 *  \exception TRendererError Can be thrown by an error handler (like TRi:errorAbort())
	 */
	virtual bool popOptions(TIntermediateRenderer &ri);
}; // TRiCPPContext



////////////////////////////////////////////////////////////////////////////////
//! Set of the interface functions.
/*!
 * Implemented by TIntermediateRenderer with protected empty bodies (except archive and options). TIntermediateRenderer
 * calls a function of this interface after it has checked all parameters. You can overwrite this methods in
 * your renderer (subclass of TIntermediateRenderer) to "do the work", for example \sa TGLRenderer.
 */
class TIntermediateRendererInterface {
public:
	//! General routines
	//@{
	//! declare types
	virtual RtToken doDeclare(RtToken token, const char *name, const char *declaration) = 0;
	//! Set an error handler
	virtual RtVoid doErrorHandler(RtErrorHandler handler) = 0;
	//@}

	//! Graphics state
	//@{

	//! Begin-end block
	//@{
	virtual RtVoid doBegin(RtToken name) = 0;
	virtual RtVoid doEnd(void) = 0;
	//@}

	//! Context switching
	//@{
	virtual RtContextHandle doGetContext(RtContextHandle handle) = 0;
	virtual RtVoid doContext(RtContextHandle handle) = 0;
	//@}

	//! Frame block
	//@{
	virtual RtVoid doFrameBegin(RtInt number) = 0;
	virtual RtVoid doFrameEnd(void) = 0;
	//@}

	//! World block
	//@{
	virtual RtVoid doWorldBegin(void) = 0;
	virtual RtVoid doWorldEnd(void) = 0;
	//@}

	//! Attribute block
	//@{
	virtual RtVoid doAttributeBegin(void) = 0;
	virtual RtVoid doAttributeEnd(void) = 0;
	//@}

	//! Transformation block
	//@{
	virtual RtVoid doTransformBegin(void) = 0;
	virtual RtVoid doTransformEnd(void) = 0;
	//@}

	//! Solid block
	//@{
	virtual RtVoid doSolidBegin(RtToken type) = 0;
	virtual RtVoid doSolidEnd(void) = 0;
	//@}

	//! Object block
	//@{
	virtual RtObjectHandle doObjectBegin(RtObjectHandle handle) = 0;
	virtual RtVoid doObjectEnd(void) = 0;

	//! Retained geometry (objectBegin(), objectEnd())
	virtual RtVoid doObjectInstance(RtObjectHandle handle) = 0;
	//@}

	//! Motion block
	//@{
	virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]) = 0;
	virtual RtVoid doMotionEnd(void) = 0;
	//@}

	//@}

	//! Synchronize state (abort)
	virtual RtVoid doSynchronize(RtToken name) = 0;

	//! Options
	//@{
	virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid doFrameAspectRatio(RtFloat aspect) = 0;
    virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
    virtual RtVoid doProjectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doClipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid doShutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid doPixelVariance(RtFloat variance) = 0;
    virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid doPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid doImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) = 0;
    virtual RtVoid doRelativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid doOptionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Lights
	//@{
	virtual RtLightHandle doLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtLightHandle doAreaLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Attributes
	//@{
	virtual RtVoid doAttributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doColor(RtColor Cs) = 0;
	virtual RtVoid doOpacity(RtColor Cs) = 0;
    virtual RtVoid doSurfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doAtmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doInteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doExteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid doDisplacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid doShadingRate(RtFloat size) = 0;
    virtual RtVoid doShadingInterpolation(RtToken type) = 0;
    virtual RtVoid doMatte(RtBoolean onoff) = 0;
	virtual RtVoid doBound(RtBound bound) = 0;
	virtual RtVoid doDetail(RtBound bound) = 0;
    virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) = 0;
    virtual RtVoid doGeometricRepresentation(RtToken type) = 0;
    virtual RtVoid doOrientation(RtToken orientation) = 0;
	virtual RtVoid doReverseOrientation(void) = 0;
    virtual RtVoid doSides(RtInt nsides) = 0;
    virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) = 0;
	//@}

	//! Transformations
	//@{
	virtual RtVoid doIdentity(void) = 0;
	virtual RtVoid doTransform(RtMatrix transform) = 0;
	virtual RtVoid doConcatTransform(RtMatrix transform) = 0;
	virtual RtVoid doPerspective(RtFloat fov) = 0;
	virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid doDeformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doCoordinateSystem(RtToken space) = 0;
	virtual RtVoid doCoordSysTransform(RtToken space) = 0;
	virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;
	//@}

	//! Polygons
	//@{
	virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Patches
	//@{
	virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Subdivision Surfaces
	//@{
	virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Quadrics
	//@{
	virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Point and curve primitives
	//@{
	virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}
    
	//! Blobby implicit surfaces
	//@{
	virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Procedural primitives
	virtual RtVoid doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) = 0;

    //! General objects
	//@{
	virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

    //! Map-making
	//@{
	virtual RtVoid doMakeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//! Archive files
	//@{
	virtual RtVoid doArchiveRecordV(RtToken type, const char *line) = 0;
	virtual RtVoid doReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//@}
    // End of do-functions
};

////////////////////////////////////////////////////////////////////////////////
//! Intermediate class that implements the task that are likely common for
//! the renderers (like keeping track the context, attributes, options).
class TIntermediateRenderer : public TRiCPPBinding, protected TIntermediateRendererInterface, protected TNewInterface {
	//! TRiCPPContext calls some functions, like the one that delivers a new attribut set getNewAttributes()
	friend class TRiCPPContext;

	//! Set default declarations, called in constructor
	void defaultDeclarations();
protected:
	TIntermediateRendererInterface *m_callri;	//!< Is called for the TIntermediateRendererInterface do-Functions, it is initialize with this, so the own functions are called.
	const char *m_rendererName;                 //!< The name of the renderer, set by begin()

	//! Should record a macro exception for object block functions.
	bool isRecording() { return m_currentContext && m_currentContext->m_macroWrite && m_currentContext->m_replay <= 0 && m_currentContext->m_stopRecord == 0; }

	//! Is in an Object definition -> Do-functions are not called, exception the object functions and archive function
	bool isDefiningObject() { return (m_currentContext && ( m_currentContext->m_defineObject > 0 )); }

	//! Additional tokens RenderMan options and attributes
	//@{
	static RtToken ri_Archive;
	static RtToken ri_Texture;
	static RtToken ri_Shader;
	static RtToken ri_Procedural;
	static RtToken ri_EndOfFrame;
	static RtToken ri_EyeSplits;
	static RtToken ri_GridSize;
	static RtToken ri_BucketSize;

	static RtToken ri_Sphere;
	static RtToken ri_CoordinateSystem;
	// static RtToken ri_Name; // is RI_NAME
	static RtToken ri_Sense;

	static RtToken ri_Jitter;
	static RtToken ri_Shadows;
	//@}

	//! Additional tokens RiCPP specific
	//@{
	static RtToken ri_NullToken;				//!< "null"

	static RtToken ri_RiCPPControl;				//!< Control options
	static RtToken ri_RiCPPControlCamTrans;		//!< a camera transformation matrix
	static RtToken ri_RiCPPControlWriteArchive; //!< function of TRi::readArchive
	static RtToken ri_RiCPPControlDisableFormat;//!< disable TRi::format
	static RtToken ri_RiCPPControlWriteObject;  //!< function of TRi::objectInstance
	//@}

	//! Archive tokens
	//@{
	static RtToken ri_RiCPPCacheArchive; //!< Cache the archives read by readArchive()
	//@}

	//! List of default declarations (those done with no context active)
	//! \sa getDeclMap() to get a pointer to the current declarations
	TDeclarationMap m_defaultDeclarationMap;

	//! Declares types in m_defaultDeclarationMap
	RtToken declareGlobal(const char *name, const char *declaration);

	//! Get the current declarationmap
	TDeclarationMap &getDeclMap();

	//! Default option set. The default options should get copied
	//! to the current contect in copyDefaultOption. This function is
	//! called by begin().
	TRenderOptions *m_defaultOptions;

	//! Query if there is a valid context.
	inline virtual bool validContext() { return m_currentContext != NULL; }

	//! Procedurals
	//@{
	virtual RtVoid handleDelayedReadArchive(RtPointer data, RtFloat detail);
	virtual RtVoid handleRunProgram(RtPointer data, RtFloat detail);
	virtual RtVoid handleDynamicLoad(RtPointer data, RtFloat detail);
	virtual RtVoid handleFree(RtPointer data);
	//@}

	//! special pointers to tokens
	//@{
	virtual RtToken getFilterTok(RtFilterFunc filterfunc);     //!< Token for a pixel filter function
	virtual RtToken getSubdivTok(RtProcSubdivFunc subdivfunc); //!< Token for a subdivision function (procedural)
	//@}

	//! Standard light source shaders
	//@{
	inline virtual void ambientLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor) {}
	inline virtual void distantLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to) {}
	inline virtual void pointLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from) {}
	inline virtual void spotLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to, RtFloat coneangle, RtFloat conedeltaangle, RtFloat beamdistribution) {}
	inline virtual void nonStandardLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor) {}
	//@}

	//! Called after TRi::lightSource, distributes to ambientLight etc.
	virtual RtBoolean establishLight(TLight &light, RtLightHandle handle);
	
	//! Called by establishAttributes() if an attribute set is popped (like in attributeEnd())
	virtual void resetLights();

	//! Restores the orientation, uses CTM and orientation token of the
	//! current attributes, notifies orientation changes,
	//! if new orientation != previous orientation
	virtual void restoreOrientation(bool prevOrientation);

	//! Standard shaders
	//@{
	inline virtual void constantShader(int shaderType) {}
	inline virtual void matteShader(int shaderType, RtFloat Ka, RtFloat Kd) {}
	inline virtual void metalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat roughness) {}
	inline virtual void shinyMetalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat Kr, RtFloat roughness, const char *texturename) {}
	inline virtual void plasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor) {}
	inline virtual void paintedPlasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor, const char *texturename) {}
	inline virtual void bumpyShader(int shaderType, RtFloat amplitude, const char *texturename) {}
	inline virtual void depthCueShader(int shaderType, RtFloat mindistance, RtFloat maxdistance, RtFloat *background) {}
	inline virtual void fogShader(int shaderType, RtFloat distance, RtFloat *background) {}
	inline virtual void nonStandardShader(int shaderType) {}
	//@}

	//! Set a shader defined by attributes, called by surface() etc.
	virtual void establishShader(int shaderType);

	//! Set the projection defined by the options
	virtual void establishProjection();

	//! Winding orientation changed
	virtual RtVoid notifyOrientationChanged();
	//! Swap from inside to outside and vice versa
	virtual RtVoid swapOrientation();

	// Option stack
	// virtual void pushOptions();			// called by RiFrameBegin
	// virtual void popOptions();			// called by RiFrameEnd
	virtual void establishOptions();	//!< Called if option set has changed

	// Attribute stack
	// virtual void pushAttributes();   // called by RiAttributeBegin
	// virtual void popAttributes();    // called by RiAttributeEnd
	virtual void establishAttributes();	//!< called if attribute set has changed
										
	// Transformation stack
	// virtual void pushTransformation();		// called by RiTransformBegin
	// virtual void popTransformation();		// called by RiTransformEnd
	virtual void establishTransformation();	//!< called if transformation has changed

	inline virtual TRenderOptions &options() const {
		assert(m_currentContext != NULL);
		assert(m_currentContext->m_currentOptions != NULL);
		return *(m_currentContext->m_currentOptions);
	}

	inline virtual TRenderAttributes &attributes() const {
		assert(m_currentContext != NULL);
		assert(m_currentContext->m_currentAttributes != NULL);
		return *(m_currentContext->m_currentAttributes);
	}

	inline virtual TMatrix3D &CTM() const {
		assert(m_currentContext != NULL);
		assert(m_currentContext->m_currentTransform != NULL);
		return m_currentContext->m_currentTransform->m_CTM;
	}

	inline virtual TMatrix3D &inverseCTM() const {
		assert(m_currentContext != NULL);
		assert(m_currentContext->m_currentTransform != NULL);
		return m_currentContext->m_currentTransform->m_inverseCTM;
	}

	//! Returns a new attribute set can be overloaded by other
	//! renderers to include special attributes
	//@{
	virtual TRenderAttributes *getNewAttributes();
	virtual TRenderAttributes *getNewAttributes(const TRenderAttributes *);
	//@}

	//! Returns a new attribute set can be overloaded by other
	//! renderers to include special attributes
	//@{
	virtual TRenderTransform *getNewTransform();
	virtual TRenderTransform *getNewTransform(const TRenderTransform *);
	//@}

	// Copies default attributes from the renderer to the renderer context
	// virtual void copyDefaultAttributes();

	//! Returns a new option set can be overloaded by other
	//! renderers to include special options
	//@{
	virtual TRenderOptions *getNewOptions();
	virtual TRenderOptions *getNewOptions(const TRenderOptions *);
	//@}

	// Copies default options from the renderer to the renderer context
	// virtual void copyDefaultOptions();

	//! Context handling
	//@{
	RtContextHandle m_currentContextHandle;			//!< Index+1 of the current context or RI_NULL
	TRiCPPContext *m_currentContext;				//!< Current context or NULL
	std::vector<TRiCPPContext *> m_contextList;		//!< List of contexts (possibly with 'holes')
	//@}

	//! Line Numbers
	//@{
	inline virtual long lineNo() const { return m_currentContext ? m_currentContext->m_lineNo : -1; }
	inline virtual long lineNo(long aLineNo) { return m_currentContext ? m_currentContext->m_lineNo=aLineNo : -1; }
	//@}

	//! RIB Archive
	//@{
	inline virtual const char *archiveName() const { return m_currentContext ? m_currentContext->m_ribArchive.c_str() : NULL; }
	inline virtual const char *archiveName(const char *anArchiveName ) {
		if ( !m_currentContext)
			return NULL;
		m_currentContext->m_ribArchive = anArchiveName ? anArchiveName : "";
		return m_currentContext->m_ribArchive.c_str();
	}
	//@}

	//! Get a new context, maybe overloaded to return special contexts
	//! called in begin()
	virtual TRiCPPContext *getNewContext();

	//! Insert a context and set as active context,
	//! called from begin()
	virtual bool insertContext(TRiCPPContext *c);

	//! deletes the current context, it is called from end()
	//! there is no current context anymore after this function is called
	//@{
	virtual void deleteCurrentContext();
	virtual void deleteAllContexts();
	//@}

	//! Check the validity of the current context and interface routine without message
	//@{
	virtual bool isValid(const char *message=NULL);	// checks only pointers
	virtual bool isValid(unsigned int idxRoutine, const char *message=NULL);
	//@}
	// virtual bool isValid(unsigned int idxRoutine);

	//! Geometry, the default teapot
    virtual void teapot();

	//! Reads RIB archive and call interface routines
	virtual void handleReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]);
	
	//! Starts recording for objects
	virtual RtObjectHandle handleObjectBegin(RtObjectHandle handle);

	//! Ends recording for objects
	virtual void handleObjectEnd(void);

	//! Replays recorded objects
    virtual void handleObjectInstance(RtObjectHandle handle);

	//! Test parameter
	virtual bool queryParam(
		const char *token,
		RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners,
		RtInt &aClass, RtInt &aType, RtInt &aCardinality, RtInt &numComps, IndexType &numBytes
	);

	//! Current ustep (PatchMesh)
	virtual RtInt curUStep() const;
	//! Current vstep (PatchMesh)
	virtual RtInt curVStep() const;
	//! Current components of colors and opacities
	virtual RtInt curColorComponents() const;
	//! \return current u basis of TRi::basis()
	virtual const RtMatrix *curUBasis() const;
	//! \return current v basis of TRi::basis()
	virtual const RtMatrix *curVBasis() const;
	//! \return current tesselation in parametric u
	virtual RtFloat curUTesselation() const;
	//! \return current tesselation in parametric v
	virtual RtFloat curVTesselation() const;
	//! \return get current trimCurve
	virtual TTrimCurveData *curTrimCurve() const;

	//! Returns an interface to draw triangulated primaries.
	inline virtual TDrawPrim *getDrawPrim() {return NULL;}

	//! do-Functions, overload them to do the work, they are called after parameter checking
	//@{
protected:
	//! General routines
	//@{
	//! declare types
	inline virtual RtToken doDeclare(RtToken token, const char *name, const char *declaration) {
		// "token" is the token returnd by delare()
		return token;
	}
	//! Set an error handler
	inline virtual RtVoid doErrorHandler(RtErrorHandler handler) {}
	//@}

	//! Graphics state
	//@{

	//! Begin-end block
	//@{
	inline virtual RtVoid doBegin(RtToken name = (RtToken)RI_NULL) {}
	inline virtual RtVoid doEnd(void) {}
	//@}

	//! Context switching
	//@{
	inline virtual RtContextHandle doGetContext(RtContextHandle handle) {
		// "handle" is the context-handle used by the interface (returnd by getContextHandle())
		return handle;
	}
	inline virtual RtVoid doContext(RtContextHandle handle) {}
	//@}

	//! Frame block
	//@{
	inline virtual RtVoid doFrameBegin(RtInt number) {}
	inline virtual RtVoid doFrameEnd(void) {}
	//@}

	//! World block
	//@{
	inline virtual RtVoid doWorldBegin(void) {}
	inline virtual RtVoid doWorldEnd(void) {}
	//@}

	//! Attribute block
	//@{
	inline virtual RtVoid doAttributeBegin(void) {}
	inline virtual RtVoid doAttributeEnd(void) {}
	//@}

	//! Transformation block
	//@{
	inline virtual RtVoid doTransformBegin(void) {}
	inline virtual RtVoid doTransformEnd(void) {}
	//@}

	//! Solid block
	//@{
	inline virtual RtVoid doSolidBegin(RtToken type) {}
	inline virtual RtVoid doSolidEnd(void) {}
	//@}

	//! Object block
	//@{
	inline virtual RtObjectHandle doObjectBegin(RtObjectHandle handle) {
		// "handle" is the object-handle used by the interface (returnd by objectBegin())
		return handleObjectBegin(handle);
	}
	inline virtual RtVoid doObjectEnd(void) {
		handleObjectEnd();
	}

	//! Retained geometry (objectBegin(), objectEnd())
	inline virtual RtVoid doObjectInstance(RtObjectHandle handle) {
		handleObjectInstance(handle);
	}
	//@}

	//! Motion block
	//@{
	inline virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]) {}
	inline virtual RtVoid doMotionEnd(void) {}
	//@}

	//@}

	//! Synchronize state (abort)
	inline virtual RtVoid doSynchronize(RtToken name) {}

	//! Options
	//@{
	inline virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid doFrameAspectRatio(RtFloat aspect) {}
    inline virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
    inline virtual RtVoid doProjectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doClipping(RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid doShutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid doPixelVariance(RtFloat variance) {}
    inline virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid doPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid doImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
    inline virtual RtVoid doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
    inline virtual RtVoid doRelativeDetail(RtFloat relativedetail) {}
    inline virtual RtVoid doOptionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Lights
	//@{
	inline virtual RtLightHandle doLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) { return handle; }
	inline virtual RtLightHandle doAreaLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) { return handle; }
	//@}

	//! Attributes
	//@{
	inline virtual RtVoid doAttributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doColor(RtColor Cs) {}
	inline virtual RtVoid doOpacity(RtColor Cs) {}
    inline virtual RtVoid doSurfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doAtmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doInteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doExteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) {}
    inline virtual RtVoid doDisplacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid doShadingRate(RtFloat size) {}
    inline virtual RtVoid doShadingInterpolation(RtToken type) {}
    inline virtual RtVoid doMatte(RtBoolean onoff) {}
	inline virtual RtVoid doBound(RtBound bound) {}
	inline virtual RtVoid doDetail(RtBound bound) {}
    inline virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) {}
    inline virtual RtVoid doGeometricRepresentation(RtToken type) {}
    inline virtual RtVoid doOrientation(RtToken orientation) {}
	inline virtual RtVoid doReverseOrientation(void) {}
    inline virtual RtVoid doSides(RtInt nsides) {}
    inline virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}
	//@}

	//! Transformations
	//@{
	inline virtual RtVoid doIdentity(void) {}
	inline virtual RtVoid doTransform(RtMatrix transform) {}
	inline virtual RtVoid doConcatTransform(RtMatrix transform) {}
	inline virtual RtVoid doPerspective(RtFloat fov) {}
	inline virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid doDeformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordSysTransform(RtToken space) {}
	inline virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
		// result are the points returned by transformPoints
		return &points[0];
	}
	//@}

	//! Polygons
	//@{
	inline virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Patches
	//@{
	inline virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Subdivision Surfaces
	//@{
	inline virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Quadrics
	//@{
	inline virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Point and curve primitives
	//@{
	inline virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}
    
	//! Blobby implicit surfaces
	//@{
	inline virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Procedural primitives
	inline virtual RtVoid doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) {}

    //! General objects
	//@{
	inline virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

    //! Map-making
	//@{
	inline virtual RtVoid doMakeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) {}
	//@}

	//! Archive files
	//@{
	inline virtual RtVoid doArchiveRecordV(RtToken type, const char *line) {}
	inline virtual RtVoid doReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) {
		handleReadArchiveV(filename, callback, n, tokens, params);
	}
	//@}

	//@}
    // End of do-functions

public:
	//////////////////////////
	// Constructor, destructor

	//! Constructor, sets some variables, sets default declarations, ...
	TIntermediateRenderer();

	//! Destructor to clear up
	virtual ~TIntermediateRenderer();

	// initialization, e.g. creates default options and calls other
	// virtual member functions for initialization. must be called after
	// after creating a renderer object.
	// virtual void initialize();

	//! Implementation of the RenderMan interface functions
	//@{
	///////////////////
	// General routines

	//! Declares variables
	virtual RtToken declare(const char *name, const char *declaration);

	//! Sets the error handler m_errHandler
    virtual RtVoid errorHandler(RtErrorHandler handler);

	//////////////////////////
	// Block structure

	// RiBegin - starts the rendering, must always called first
	virtual RtVoid begin(RtToken name = RI_NULL);

	// RiEnd - ends the rendering, no rendering function are accepet afterwards, except RiBegin
	virtual RtVoid end(void);

	// Context switching
	virtual RtContextHandle getContext(void);
	virtual void context(RtContextHandle handle);

	// RiFrameBegin - starts a new frame, stores the options, cannot be nested
    virtual RtVoid frameBegin(RtInt number);
	
	// RiFrameEnd - ends a frame, recalls the options
	virtual RtVoid frameEnd(void);

	// RiWorldBegin - starts a world block, cannot be nested, but you can have multiple world/blocks per frame, stores the attributes
	//                only attributs can be changed in a worldblock, it´s an error to change options
	virtual RtVoid worldBegin(void);

	// RiWorldEnd - ends a world block, recalls the attributes
	virtual RtVoid worldEnd(void);

	// RiAttributeBegin - starts new set of attributes
	virtual RtVoid attributeBegin(void);

	// RiAttributeEnd - restores set of attributes
	virtual RtVoid attributeEnd(void);

	// RiTransformBegin - starts new transformations
	virtual RtVoid transformBegin(void);

	// RiTransformEnd - restores transformations
	virtual RtVoid transformEnd(void);

	// RiSolidBegin - starts new solid block
    virtual RtVoid solidBegin(RtToken type);

	// RiSolidEnd - ends a solid block
    virtual RtVoid solidEnd(void);

	// RiObjectBegin - starts new object definition
	virtual RtObjectHandle objectBegin(void);

	// RiObjectEnd - ends an object definition
	virtual RtVoid objectEnd(void);

	// RiMotionBegin - starts a motion block
    virtual RtVoid motionBeginV(RtInt N, RtFloat times[]);

	// RiMotionEnd - ends a motion block
    virtual RtVoid motionEnd(void);

	// Synchronize
    virtual RtVoid synchronize(RtToken name);

	////////////////////////////////////
	// Options
	
	// viewing and camera model

	// RiFormat - setting the resolution of the output devices
    virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);

	// RiFrameAspectRatio - aspect=framewidth/frameheight
    virtual RtVoid frameAspectRatio(RtFloat aspect);

	// RiScreenWindow - dimension of the frame in normalized 2D display coordinates
    virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);

	// RiCropWindow - rectangle subregion of the frame to render
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);

	// RiProjection - 3D ->2D RI_PERSPECTIVE, RI_ORTHOGRAPHIC
    virtual RtVoid projectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);

    // RiClipping - Sets hither and yonder clipping planes
	virtual RtVoid clipping(RtFloat hither, RtFloat yon);

	// RiClip - Adds additional Clipping planes
    virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);

	// RiDepthOfFiled - Specifies the depth of field of the camera
    virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);

	// RiShutter - Sets the shutting time of a camera (motion blur)
    virtual RtVoid shutter(RtFloat smin, RtFloat smax);

	// Options: quantisation, filtering and sampling

	// RiPixelVariance - limits the acceptable variance in the output value of pixels
	virtual RtVoid pixelVariance(RtFloat variance);

	// RiPixelSamples - determines how frequently the scene is sampled
    virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples);

	// RiPixelFilter - declares a filterfunction and the range of influence
    virtual RtVoid pixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth);

	// RiExposure - color[output] = (color[input]*gain)**1/gamma
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma);

	// RiImager, Option, imager is written in the SL, changes the color dimension
    virtual RtVoid imagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	// RiQuantize - quantization and dithering
    virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);

	// RiDisplay - declaring an output device
    virtual RtVoid displayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]);

	// Detail
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn);
    virtual RtVoid relativeDetail(RtFloat relativedetail);
    virtual RtVoid optionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);

	// Light Source
    virtual RtLightHandle lightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtLightHandle areaLightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);

	// Attributes
	virtual RtVoid color(RtColor Cs);
	virtual RtVoid opacity(RtColor Cs);
    virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

	// Shader
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff);
    virtual RtVoid surfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid atmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid interiorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid exteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid shadingRate(RtFloat size);
    virtual RtVoid shadingInterpolation(RtToken type);
    virtual RtVoid matte(RtBoolean onoff);

	// Model
	virtual RtVoid bound(RtBound bound);
	virtual RtVoid detail(RtBound bound);
    virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
    virtual RtVoid geometricRepresentation(RtToken type);
    virtual RtVoid orientation(RtToken orientation);
	virtual RtVoid reverseOrientation(void);
    virtual RtVoid sides(RtInt nsides);

	// Transformations
	virtual RtVoid identity(void);
    virtual RtVoid transform(RtMatrix trans);
	virtual RtVoid concatTransform(RtMatrix trans);
    virtual RtVoid perspective(RtFloat fov);
    virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
    virtual RtVoid deformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid displacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid coordinateSystem(RtToken space);
    virtual RtVoid coordSysTransform(RtToken space);
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

	// Attributes
    virtual RtVoid attributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);

	// Patches
    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);

	// Quadrics, Primitives
    virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);

    virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	// Curves, Subdiv, Blobs
    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);

	// Procedural
    virtual RtVoid procedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc);

	// Objects
    virtual RtVoid objectInstance(RtObjectHandle handle);

	// Texture
    virtual RtVoid makeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]);

	// Archives
	virtual RtVoid archiveRecordV(RtToken type, const char *line);
    virtual RtVoid readArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}


	inline virtual TRiPatch *newRiPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( type && !strcmp(type, RI_BILINEAR) ) {
			return new TBilinearPatch(aLineNo, declmap, colorComps, n, tokens, params);
		}
		
		if ( type && !strcmp(type, RI_BICUBIC) ) {
			return new TBicubicPatch(aLineNo, declmap, colorComps, n, tokens, params);
		}

		return new TRiPatch(aLineNo, declmap, colorComps, type, n, tokens, params);
	}

	inline virtual TRiPatchMesh *newRiPatchMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtToken type,
		RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( type && !strcmp(type, RI_BILINEAR) ) {
			return new TBilinearPatchMesh(aLineNo, declmap, colorComps, nu, uwrap, nv, vwrap, n, tokens, params);
		}
		
		if ( type && !strcmp(type, RI_BICUBIC) ) {
			return new TBicubicPatchMesh(aLineNo, declmap, colorComps, vstep, ustep, nu, uwrap, nv, vwrap, n, tokens, params);
		}

		return new TRiPatchMesh(aLineNo, declmap, colorComps, ustep, vstep, type, nu, uwrap, nv, vwrap, n, tokens, params);
	}

	inline virtual TRiSubdivisionMesh *newRiSubdivisionMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TSubdivisionMesh(aLineNo, declmap, colorComps, scheme, nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs, n, tokens, params);
	}

	inline virtual TRiSphere *newRiSphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TSphere(aLineNo, declmap, colorComps, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiCone *newRiCone(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TCone(aLineNo, declmap, colorComps, height, radius, thetamax, n, tokens, params);
	}

	inline virtual TRiCylinder *newRiCylinder(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TCylinder(aLineNo, declmap, colorComps, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiHyperboloid *newRiHyperboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new THyperboloid(aLineNo, declmap, colorComps, point1, point2, thetamax, n, tokens, params);
	}

	inline virtual TRiParaboloid *newRiParaboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TParaboloid(aLineNo, declmap, colorComps, rmax, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiDisk *newRiDisk(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TDisk(aLineNo, declmap, colorComps, height, radius, thetamax, n, tokens, params);
	}

	inline virtual TRiTorus *newRiTorus(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TTorus(aLineNo, declmap, colorComps, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	}

	inline virtual TRiPolygon *newRiPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TPolygon(aLineNo, declmap, colorComps, nvertices, n, tokens, params);
	}

	inline virtual TRiPointsPolygons *newRiPointsPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TPointsPolygons(aLineNo, declmap, colorComps, npolys, nverts, verts, n, tokens, params);
	}

	inline virtual TRiTrimCurve *newRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
		return new TTrimCurve(aLineNo, nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	inline virtual TRiNuPatch *newRiNuPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TNuPatch(aLineNo, declmap, colorComps, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	}
};


#endif // _RICPP_RENDERCONTEXT_RENDERCONTEXT_H
