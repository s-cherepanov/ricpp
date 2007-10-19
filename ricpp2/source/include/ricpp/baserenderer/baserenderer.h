#ifndef _RICPP_BASERENDERER_BASERENDERER_H
#define _RICPP_BASERENDERER_BASERENDERER_H

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

/** @file baserenderer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the backend base class of a renderer context.
 */

#ifndef _RICPP_BASERENDERER_DORENDER_H
#include "ricpp/baserenderer/dorender.h"
#endif // _RICPP_BASERENDERER_DORENDER_H

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

/** @brief This class is used to implement the basis of a renderer context.
 *  
 *  Finds the unique strings (tokens) for RtTokens and
 *  it does type checking and calls the IDoRenderer do...() members if the checking was successful.
 *  Maintains a render state.
 *  The concrete context inherits from CBaseRenderer and implements the IDoRenderer members for
 *  its rendering jobs. The CBaseRenderer is the foundation of the backend renderer context.
 */
class CBaseRenderer : public IDoRender {
	/** @brief Initializes the m_renderState, called by begin()
	 */
	void initRenderState();

	/** @brief The error handler throws exceptions, default handler
	 *
	 *  Never used directly, ricppErrHandler() is used for error handling
	 */
	CErrorExceptionHandler m_errorExceptionHandler;

protected:
	/** @brief The render state contains all render state objects of this context
	 *
	 *  The render state is initialized at begin()
	 */
	CRenderState *m_renderState;

	/** @brief Input handler for byte streams
	 */
	CBackBufferProtocolHandlers *m_protocolHandler;

	/** @brief Creates a new modestack, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own modestack
	 *
	 * @return A stack object for render modes
	 */
	inline virtual CModeStack *getNewModeStack()
	{
		return new CModeStack;
	}

	/** @brief Creates a new options factory, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized options set.
	 *
	 * @return A factory object for the render options.
	 */
	inline virtual COptionsFactory *getNewOptionsFactory()
	{
		return new COptionsFactory;
	}

	/** @brief Creates a new attributes factory, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized attributes set.
	 *
	 * @return A factory object for the render attributes.
	 */
	inline virtual CAttributesFactory *getNewAttributesFactory()
	{
		return new CAttributesFactory;
	}

	/** @brief Creates a new lightsource factory, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized lightsources.
	 *
	 * @return A factory object for the lightsources.
	 */
	inline virtual CLightSourceFactory *getNewLightSourceFactory()
	{
		return new CLightSourceFactory;
	}

	/** @brief Factory method to create a macro factory
	 */
	virtual CRManInterfaceFactory *getNewMacroFactory();

	/** @brief The backend's error handler
	 *
	 *  A child class can overload ricppErrHandler() to use it's own handler
	 *  However it also should return an error handler that throws an ExceptRiCPPError
	 * @return The error handler used by the backend
	 */
	virtual IRiCPPErrorHandler &ricppErrHandler()
	{
		// @todo fill m_errorExceptionHandler with lineno and filename
		return m_errorExceptionHandler;
	}

	/** @brief Call the default declarations.
	 *
	 * Overload this to add more default declarations, by calling preDeclare().
	 */
	virtual void defaultDeclarations();

	inline virtual CRiMacro *curMacro() {return 0;}

	virtual void processRequest(CRManInterfaceCall &aRequest);
	virtual void replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb);

public:

	virtual RtVoid archiveInstanceV(RtArchiveHandle handle, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid preArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params);
	virtual RtVoid doArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params);
	virtual RtVoid postArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params);

public:

	/** @brief Check validity of state for common requests.
	 * @throw ExceptRiCPPError
	 * @param req Number of the requests
	 * @return false if checking fails
	 */
	virtual bool preCheck(EnumRequests req);

	inline virtual const CRenderState *renderState() const { return m_renderState; }
	inline virtual CRenderState *renderState() { return m_renderState; }

	/** @brief Constructor, initializes member variables.
	 */
	CBaseRenderer();

	/** @brief Virtual destruction
	 */
	virtual ~CBaseRenderer();

	/** @brief Is called by the frontend, if the renderer is aborted due to severe errors.
	 */
	inline virtual RtVoid abort(void) {}

	/** @brief Is called, if the renderer is activated by context()
	 *
	 * If the context will be the new active rendering context, it is activated by it's
	 * context creator.
	 */
	inline virtual RtVoid activate(void) {}

	/** @brief Is called, if the renderer is deactivated by context()
	 *
	 * If the context will be the suspended rendering context, it is deactivated by it's
	 * context creator.
	 */
	inline virtual RtVoid deactivate(void) {}

	inline virtual RtVoid registerProtocolHandler(CBackBufferProtocolHandlers &protocolHandler)
	{
		m_protocolHandler = &protocolHandler;
	}

	/** @brief Error handler
	 *
	 *  @se IRiContext::preErrorHandler()
	 *
	 *  @param handler The error handler
	 */
	inline virtual RtVoid errorHandler(const IErrorHandler &handler) {}

	RtToken declare(RtString name, RtString declaration);

	inline virtual RtVoid synchronize(RtToken name) {}
	inline virtual RtVoid system(RtToken cmd) {}
	inline virtual RtResourceHandle resourceV(RtToken name, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) { return illResourceHandle; }

	/** @brief Initializes a new render context.
	 *
	 *  beginV() is called exactly one time by the context creator
	 *  after the rendering context object is created.
	 *  This method must be called by child objects begin() at first
	 *  for initialization before they do their own job.
	 *
	 *  Has no return type on purpose, because the CBaseRender is the back end context.
	 *
	 *  @param name Passed by the front end's begin()-method.
	 *  @param n Passed by the front end's begin()-method.
	 *  @param tokens Passed by the front end's begin()-method.
	 *  @param params Passed by the front end's begin()-method.
	 *  @return 0 (instance is the context, the handle is managed by CContextCreator)
	 *  @see IRiCPPBridge::beginV()
	 */
	virtual RtContextHandle beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Clears the context.
	 *
	 *  end() is called exactly one time by the context creator before deletion. It
	 *  can be overwritten, but child classes must call CBaseRenderer::end()
	 *  as their last action.
	 */
	virtual RtVoid end(void);

	virtual RtVoid frameBegin(RtInt number);
	virtual RtVoid frameEnd(void);

	virtual RtVoid worldBegin(void);
	virtual RtVoid worldEnd(void);

	virtual RtVoid attributeBegin(void);
	virtual RtVoid attributeEnd(void);

	virtual RtVoid transformBegin(void);
	virtual RtVoid transformEnd(void);

	virtual RtVoid solidBegin(RtToken type);
	virtual RtVoid solidEnd(void);

	virtual RtObjectHandle objectBegin(void);
	virtual RtVoid objectEnd(void);
	virtual RtVoid objectInstance(RtObjectHandle handle);

	virtual RtVoid motionBeginV(RtInt N, RtFloat times[]);
	virtual RtVoid motionEnd(void);

	virtual RtVoid resourceBegin(void);
	virtual RtVoid resourceEnd(void);

	virtual RtArchiveHandle archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid archiveEnd(void);

	virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);
	virtual RtVoid frameAspectRatio(RtFloat aspect);
	virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
	virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid clipping(RtFloat hither, RtFloat yon);
	virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
	virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
	virtual RtVoid shutter(RtFloat smin, RtFloat smax);
	virtual RtVoid pixelVariance(RtFloat variation);
	virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples);
	virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
	virtual RtVoid exposure(RtFloat gain, RtFloat gamma);
	virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
	virtual RtVoid displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn);
	virtual RtVoid relativeDetail(RtFloat relativedetail);
	virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid color(RtColor Cs);
	virtual RtVoid opacity(RtColor Os);
	virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff);
	virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
	virtual RtVoid shadingRate(RtFloat size);
	virtual RtVoid shadingInterpolation(RtToken type);
	virtual RtVoid matte(RtBoolean onoff);
	virtual RtVoid bound(RtBound aBound);
	virtual RtVoid detail(RtBound aBound);
	virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
	virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
	virtual RtVoid geometricRepresentation(RtToken type);
	virtual RtVoid orientation(RtToken anOrientation);
	virtual RtVoid reverseOrientation(void);
	virtual RtVoid sides(RtInt nsides);
	virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
	virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);

	virtual RtVoid identity(void);
	virtual RtVoid transform(RtMatrix aTransform);
	virtual RtVoid concatTransform(RtMatrix aTransform);
	virtual RtVoid perspective(RtFloat fov);
	virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

	virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid scopedCoordinateSystem(RtToken space);
	virtual RtVoid coordinateSystem(RtToken space);
	virtual RtVoid coordSysTransform(RtToken space);
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

	virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc);

	virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeBrickMapV(RtInt nNames, RtString *ptcnames, RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid archiveRecordV(RtToken type, RtString line);
	/** @todo parser to frontend
	 */
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid ifBegin(RtString expr);
	virtual RtVoid elseIfBegin(RtString expr);
	virtual RtVoid elseBegin(void);
	virtual RtVoid ifEnd(void);

public:
	/** @brief Error handler
	 *
	 *  @se IRiContext::preErrorHandler()
	 *
	 *  @param handler The error handler
	 */
	inline virtual RtVoid preErrorHandler(const IErrorHandler &handler) {}

	/** @brief Create new entry in dectaration list
	 */
	virtual RtVoid preDeclare(RtToken name, RtString declaration, bool isDefault);
	inline virtual RtVoid preSynchronize(RtToken name) {}
	inline virtual RtVoid preSystem(RtToken cmd) {}
	inline virtual RtVoid preResource(RtToken name, RtToken type, const CParameterList &params) {}

	virtual RtVoid preBegin(RtString name, const CParameterList &params);
	virtual RtVoid preEnd(void);

	virtual RtVoid preFrameBegin(RtInt number);
	virtual RtVoid preFrameEnd(void);

	virtual RtVoid preWorldBegin(void);
	virtual RtVoid preWorldEnd(void);

	virtual RtVoid preAttributeBegin(void);
	virtual RtVoid preAttributeEnd(void);

	virtual RtVoid preTransformBegin(void);
	virtual RtVoid preTransformEnd(void);

	virtual RtVoid preSolidBegin(RtToken type);
	virtual RtVoid preSolidEnd(void);

	virtual RtObjectHandle preObjectBegin(void);
	virtual RtVoid preObjectEnd(void);
	virtual RtVoid preObjectInstance(RtObjectHandle handle);

	inline virtual RtVoid preMotionBegin(RtInt N, RtFloat times[]) {}
	inline virtual RtVoid preMotionEnd(void) {}

	inline virtual RtVoid preResourceBegin(void) {}
	inline virtual RtVoid preResourceEnd(void) {}

	virtual RtArchiveHandle preArchiveBegin(RtToken name, const CParameterList &params);
	virtual RtVoid preArchiveEnd(void);

	virtual RtVoid preFormat(RtInt xres, RtInt yres, RtFloat aspect);
	virtual RtVoid preFrameAspectRatio(RtFloat aspect);

	virtual RtVoid preScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
	virtual RtVoid preCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	virtual RtVoid preProjection(RtToken name, const CParameterList &params);
	inline virtual RtVoid preClipping(RtFloat hither, RtFloat yon) {}
	inline virtual RtVoid preClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
	inline virtual RtVoid preDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
	inline virtual RtVoid preShutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid prePixelVariance(RtFloat variation) {}
	inline virtual RtVoid prePixelSamples(RtFloat xsamples, RtFloat ysamples) {}
	inline virtual RtVoid prePixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
	inline virtual RtVoid preExposure(RtFloat gain, RtFloat gamma) {}
	inline virtual RtVoid preImager(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
	inline virtual RtVoid preDisplayChannel(RtString channel, const CParameterList &params) {}
	inline virtual RtVoid preDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) {}
	inline virtual RtVoid preHider(RtToken type, const CParameterList &params) {}
	inline virtual RtVoid preColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
	inline virtual RtVoid preRelativeDetail(RtFloat relativedetail) {}
	inline virtual RtVoid preOption(RtString name, const CParameterList &params) {}

	inline virtual RtLightHandle preLightSource(RtString name, const CParameterList &params) { return illLightHandle; }
	inline virtual RtLightHandle preAreaLightSource(RtString name, const CParameterList &params) { return illLightHandle; }

	inline virtual RtVoid preAttribute(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preColor(RtColor Cs) {}
	inline virtual RtVoid preOpacity(RtColor Os) {}
	inline virtual RtVoid preSurface(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preAtmosphere(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preInterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preExterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preIlluminate(RtLightHandle light, RtBoolean onoff) {}
	inline virtual RtVoid preDisplacement(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
	inline virtual RtVoid preShadingRate(RtFloat size) {}
	inline virtual RtVoid preShadingInterpolation(RtToken type) {}
	inline virtual RtVoid preMatte(RtBoolean onoff) {}
	inline virtual RtVoid preBound(RtBound bound) {}
	inline virtual RtVoid preDetail(RtBound bound) {}
	inline virtual RtVoid preDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
	inline virtual RtVoid preGeometricApproximation(RtToken type, RtFloat value) {}
	inline virtual RtVoid preGeometricRepresentation(RtToken type) {}
	inline virtual RtVoid preOrientation(RtToken anOrientation) {}
	inline virtual RtVoid preReverseOrientation(void) {}
	inline virtual RtVoid preSides(RtInt nsides) {}
	inline virtual RtVoid preBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
	inline virtual RtVoid preTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}

	inline virtual RtVoid preIdentity(void) {}
	inline virtual RtVoid preTransform(RtMatrix aTransform) {}
	inline virtual RtVoid preConcatTransform(RtMatrix aTransform) {}
	inline virtual RtVoid prePerspective(RtFloat fov) {}
	inline virtual RtVoid preTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid preRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid preScale(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid preSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid preDeformation(RtString name, const CParameterList &params) {}
	inline virtual RtVoid preScopedCoordinateSystem(RtToken space) {}
	inline virtual RtVoid preCoordinateSystem(RtToken space) {}
	inline virtual RtVoid preCoordSysTransform(RtToken space) {}

	inline virtual RtVoid prePolygon(RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid preGeneralPolygon(RtInt nloops, RtInt *nverts, const CParameterList &params) {}
	inline virtual RtVoid prePointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, const CParameterList &params) {}
	inline virtual RtVoid prePointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, const CParameterList &params) {}

	inline virtual RtVoid prePatch(RtToken type, const CParameterList &params) {}
	inline virtual RtVoid prePatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
	inline virtual RtVoid preNuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid preSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid preHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid preSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid preTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

	inline virtual RtVoid prePoints(RtInt npts, const CParameterList &params) {}
	inline virtual RtVoid preCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid preBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	inline virtual RtVoid preProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid preGeometry(RtToken type, const CParameterList &params) {}

	inline virtual RtVoid preMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid preMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid preMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid preMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid preMakeShadow(RtString pic, RtString tex, const CParameterList &params) {}
	inline virtual RtVoid preMakeBrickMap(RtInt nNames, RtString *ptcnames, RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid preArchiveRecord(RtToken type, RtString line) {}
	inline virtual RtVoid preReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params) {}

	inline virtual RtVoid preIfBegin(RtString expr) {}
	inline virtual RtVoid preElseIfBegin(RtString expr) {}
	inline virtual RtVoid preElseBegin(void) {}
	inline virtual RtVoid preIfEnd(void) {}



public:
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}

	/** @brief Error handler
	 *
	 *  @se IRiContext::preErrorHandler()
	 *
	 *  @param handler The error handler
	 */
	inline virtual RtVoid doErrorHandler(const IErrorHandler &handler) {}

	inline virtual RtVoid doDeclare(RtToken name, RtString declaration) {}
	inline virtual RtVoid doSynchronize(RtToken name) {}
	inline virtual RtVoid doSystem(RtToken cmd) {}
	inline virtual RtVoid doResource(RtToken name, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid doBegin(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doEnd(void) {}

	inline virtual RtVoid doFrameBegin(RtInt number) {}
	inline virtual RtVoid doFrameEnd(void) {}

	inline virtual RtVoid doWorldBegin(void) {}
	inline virtual RtVoid doWorldEnd(void) {}

	inline virtual RtVoid doAttributeBegin(void) {}
	inline virtual RtVoid doAttributeEnd(void) {}

	inline virtual RtVoid doTransformBegin(void) {}
	inline virtual RtVoid doTransformEnd(void) {}

	inline virtual RtVoid doSolidBegin(RtToken type) {}
	inline virtual RtVoid doSolidEnd(void) {}

	inline virtual RtVoid doObjectBegin(RtObjectHandle h) {}
	inline virtual RtVoid doObjectEnd(void) {}
	virtual RtVoid doObjectInstance(RtObjectHandle handle);

	inline virtual RtVoid doMotionBegin(RtInt N, RtFloat times[]) {}
	inline virtual RtVoid doMotionEnd(void) {}

	inline virtual RtVoid doResourceBegin(void) {}
	inline virtual RtVoid doResourceEnd(void) {}

	inline virtual RtVoid doArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doArchiveEnd(void) {}

	inline virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) {}
	inline virtual RtVoid doFrameAspectRatio(RtFloat aspect) {}

	inline virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
	inline virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
	inline virtual RtVoid doProjection(RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doClipping(RtFloat hither, RtFloat yon) {}
	inline virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
	inline virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
	inline virtual RtVoid doShutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid doPixelVariance(RtFloat variation) {}
	inline virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) {}
	inline virtual RtVoid doPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
	inline virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) {}
	inline virtual RtVoid doImager(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
	inline virtual RtVoid doDisplayChannel(RtString channel, const CParameterList &params) {}
	inline virtual RtVoid doDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) {}
	inline virtual RtVoid doHider(RtToken type, const CParameterList &params) {}
	inline virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
	inline virtual RtVoid doRelativeDetail(RtFloat relativedetail) {}
	inline virtual RtVoid doOption(RtString name, const CParameterList &params) {}

	inline virtual RtVoid doLightSource(RtLightHandle h, RtString name, const CParameterList &params) { }
	inline virtual RtVoid doAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params) { }

	inline virtual RtVoid doAttribute(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doColor(RtColor Cs) {}
	inline virtual RtVoid doOpacity(RtColor Os) {}
	inline virtual RtVoid doSurface(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doAtmosphere(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doInterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doExterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) {}
	inline virtual RtVoid doDisplacement(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
	inline virtual RtVoid doShadingRate(RtFloat size) {}
	inline virtual RtVoid doShadingInterpolation(RtToken type) {}
	inline virtual RtVoid doMatte(RtBoolean onoff) {}
	inline virtual RtVoid doBound(RtBound bound) {}
	inline virtual RtVoid doDetail(RtBound bound) {}
	inline virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
	inline virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) {}
	inline virtual RtVoid doGeometricRepresentation(RtToken type) {}
	inline virtual RtVoid doOrientation(RtToken anOrientation) {}
	inline virtual RtVoid doReverseOrientation(void) {}
	inline virtual RtVoid doSides(RtInt nsides) {}
	inline virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
	inline virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}

	inline virtual RtVoid doIdentity(void) {}
	inline virtual RtVoid doTransform(RtMatrix aTransform) {}
	inline virtual RtVoid doConcatTransform(RtMatrix aTransform) {}
	inline virtual RtVoid doPerspective(RtFloat fov) {}
	inline virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid doDeformation(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doScopedCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordSysTransform(RtToken space) {}

	// no do-funcltion
	// inline virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return &points[0]; }

	inline virtual RtVoid doPolygon(RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid doGeneralPolygon(RtInt nloops, RtInt *nverts, const CParameterList &params) {}
	inline virtual RtVoid doPointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, const CParameterList &params) {}
	inline virtual RtVoid doPointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, const CParameterList &params) {}

	inline virtual RtVoid doPatch(RtToken type, const CParameterList &params) {}
	inline virtual RtVoid doPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
	inline virtual RtVoid doNuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid doSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid doHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid doSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
	inline virtual RtVoid doTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

	inline virtual RtVoid doPoints(RtInt npts, const CParameterList &params) {}
	inline virtual RtVoid doCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid doBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	inline virtual RtVoid doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid doGeometry(RtToken type, const CParameterList &params) {}

	inline virtual RtVoid doMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid doMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid doMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid doMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
	inline virtual RtVoid doMakeShadow(RtString pic, RtString tex, const CParameterList &params) {}
	inline virtual RtVoid doMakeBrickMap(RtInt nNames, RtString *ptcnames, RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid doArchiveRecord(RtToken type, RtString line) {}
	virtual RtVoid doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params);

	inline virtual RtVoid doIfBegin(RtString expr) {}
	inline virtual RtVoid doElseIfBegin(RtString expr) {}
	inline virtual RtVoid doElseBegin(void) {}
	inline virtual RtVoid doIfEnd(void) {}


	inline virtual RtVoid postErrorHandler(const IErrorHandler &handler) {}

	inline virtual RtVoid postDeclare(RtToken name, RtString declaration) {}
	inline virtual RtVoid postSynchronize(RtToken name){}
	inline virtual RtVoid postSystem(RtToken cmd){}
	inline virtual RtVoid postResource(RtToken name, RtToken type, const CParameterList &params){}

	inline virtual RtVoid postBegin(RtString name, const CParameterList &params){}
	inline virtual RtVoid postEnd(void){}

	inline virtual RtVoid postFrameBegin(RtInt number){}
	inline virtual RtVoid postFrameEnd(void){}

	inline virtual RtVoid postWorldBegin(void){}
	inline virtual RtVoid postWorldEnd(void){}

	inline virtual RtVoid postAttributeBegin(void){}
	inline virtual RtVoid postAttributeEnd(void){}

	inline virtual RtVoid postTransformBegin(void){}
	inline virtual RtVoid postTransformEnd(void){}

    inline virtual RtVoid postSolidBegin(RtToken type){}
    inline virtual RtVoid postSolidEnd(void){}

	inline virtual RtVoid postObjectBegin(RtObjectHandle h){}
	inline virtual RtVoid postObjectEnd(void){}
    virtual RtVoid postObjectInstance(RtObjectHandle handle);

    inline virtual RtVoid postMotionBegin(RtInt N, RtFloat times[]){}
    inline virtual RtVoid postMotionEnd(void){}

	inline virtual RtVoid postResourceBegin(void){}
	inline virtual RtVoid postResourceEnd(void){}

	inline virtual RtVoid postArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params){}
	inline virtual RtVoid postArchiveEnd(void){}

    inline virtual RtVoid postFormat(RtInt xres, RtInt yres, RtFloat aspect){}
    inline virtual RtVoid postFrameAspectRatio(RtFloat aspect){}

	inline virtual RtVoid postScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top){}
    inline virtual RtVoid postCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax){}
	inline virtual RtVoid postProjection(RtToken name, const CParameterList &params){}
	inline virtual RtVoid postClipping(RtFloat hither, RtFloat yon){}
    inline virtual RtVoid postClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz){}
    inline virtual RtVoid postDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance){}
    inline virtual RtVoid postShutter(RtFloat smin, RtFloat smax){}
	inline virtual RtVoid postPixelVariance(RtFloat variation){}
    inline virtual RtVoid postPixelSamples(RtFloat xsamples, RtFloat ysamples){}
    inline virtual RtVoid postPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth){}
    inline virtual RtVoid postExposure(RtFloat gain, RtFloat gamma){}
    inline virtual RtVoid postImager(RtString name, const CParameterList &params){}
	inline virtual RtVoid postQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl){}
    inline virtual RtVoid postDisplayChannel(RtString channel, const CParameterList &params){}
    inline virtual RtVoid postDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params){}
    inline virtual RtVoid postHider(RtToken type, const CParameterList &params){}
    inline virtual RtVoid postColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn){}
    inline virtual RtVoid postRelativeDetail(RtFloat relativedetail){}
    inline virtual RtVoid postOption(RtString name, const CParameterList &params){}
	
    inline virtual RtVoid postLightSource(RtLightHandle h, RtString name, const CParameterList &params){}
	inline virtual RtVoid postAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params){}
	
    inline virtual RtVoid postAttribute(RtString name, const CParameterList &params){}
	inline virtual RtVoid postColor(RtColor Cs){}
	inline virtual RtVoid postOpacity(RtColor Os){}
    inline virtual RtVoid postSurface(RtString name, const CParameterList &params){}
    inline virtual RtVoid postAtmosphere(RtString name, const CParameterList &params){}
    inline virtual RtVoid postInterior(RtString name, const CParameterList &params){}
	inline virtual RtVoid postExterior(RtString name, const CParameterList &params){}
	inline virtual RtVoid postIlluminate(RtLightHandle light, RtBoolean onoff){}
    inline virtual RtVoid postDisplacement(RtString name, const CParameterList &params){}
	inline virtual RtVoid postTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4){}
    inline virtual RtVoid postShadingRate(RtFloat size){}
	inline virtual RtVoid postShadingInterpolation(RtToken type){}
    inline virtual RtVoid postMatte(RtBoolean onoff){}
	inline virtual RtVoid postBound(RtBound bound){}
	inline virtual RtVoid postDetail(RtBound bound){}
	inline virtual RtVoid postDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis){}
    inline virtual RtVoid postGeometricApproximation(RtToken type, RtFloat value){}
	inline virtual RtVoid postGeometricRepresentation(RtToken type){}
	inline virtual RtVoid postOrientation(RtToken anOrientation){}
	inline virtual RtVoid postReverseOrientation(void){}
	inline virtual RtVoid postSides(RtInt nsides){}
    inline virtual RtVoid postBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep){}
    inline virtual RtVoid postTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w){}

	inline virtual RtVoid postIdentity(void){}
	inline virtual RtVoid postTransform(RtMatrix aTransform){}
	inline virtual RtVoid postConcatTransform(RtMatrix aTransform){}
	inline virtual RtVoid postPerspective(RtFloat fov){}
	inline virtual RtVoid postTranslate(RtFloat dx, RtFloat dy, RtFloat dz){}
	inline virtual RtVoid postRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz){}
	inline virtual RtVoid postScale(RtFloat dx, RtFloat dy, RtFloat dz){}
    inline virtual RtVoid postSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2){}
	inline virtual RtVoid postDeformation(RtString name, const CParameterList &params){}
	inline virtual RtVoid postScopedCoordinateSystem(RtToken space){}
	inline virtual RtVoid postCoordinateSystem(RtToken space){}
	inline virtual RtVoid postCoordSysTransform(RtToken space){}

    inline virtual RtVoid postPolygon(RtInt nvertices, const CParameterList &params){}
	inline virtual RtVoid postGeneralPolygon(RtInt nloops, RtInt *nverts, const CParameterList &params){}
	inline virtual RtVoid postPointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, const CParameterList &params){}
    inline virtual RtVoid postPointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, const CParameterList &params){}

	inline virtual RtVoid postPatch(RtToken type, const CParameterList &params){}
	inline virtual RtVoid postPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params){}
    inline virtual RtVoid postNuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, const CParameterList &params){}

	inline virtual RtVoid postSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params){}
	inline virtual RtVoid postHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params){}

	inline virtual RtVoid postSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params){}
    inline virtual RtVoid postTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params){}

    inline virtual RtVoid postPoints(RtInt npts, const CParameterList &params){}
    inline virtual RtVoid postCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params){}

	inline virtual RtVoid postBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params){}

	inline virtual RtVoid postProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc){}

	inline virtual RtVoid postGeometry(RtToken type, const CParameterList &params){}

	inline virtual RtVoid postMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params){}
    inline virtual RtVoid postMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params){}
    inline virtual RtVoid postMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params){}
    inline virtual RtVoid postMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params){}
    inline virtual RtVoid postMakeShadow(RtString pic, RtString tex, const CParameterList &params){}
    inline virtual RtVoid postMakeBrickMap(RtInt nNames, RtString *ptcnames, RtString bkmname, const CParameterList &params){}

	inline virtual RtVoid postArchiveRecord(RtToken type, RtString line){}
	inline virtual RtVoid postReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params){}

	inline virtual RtVoid postIfBegin(RtString expr){}
	inline virtual RtVoid postElseIfBegin(RtString expr){}
	inline virtual RtVoid postElseBegin(void){}
	inline virtual RtVoid postIfEnd(void){}
}; // CBaseRenderer

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_BASERENDERER_H
