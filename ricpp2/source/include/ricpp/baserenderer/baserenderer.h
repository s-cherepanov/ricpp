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

#ifndef _RICPP_RICONTEXT_DORENDER_H
#include "ricpp/ricontext/dorender.h"
#endif // _RICPP_RICONTEXT_DORENDER_H

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
	/** @brief For initialization
	 */
	CTypeInfo c;

	/** @brief Initializes the m_renderState, called by begin()
	 */
	void initRenderState();

	/** @brief The error handler throws exceptions, default handler
	 *
	 *  Never used directly, ricppErrHandler() is used for error handling
	 */
	CErrorExceptionHandler m_errorExceptionHandler;

	/** @brief The render state contains all render state objects of this context
	 *
	 *  The render state is initialized at begin() by a call to initRenderState().
	 */
	CRenderState *m_renderState;

	/** @brief The mode stack.
	 *
	 *  Contains the current nesting of modes (frame, world, attribute, transform, ...).
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see getNewModeStack(), deleteModeStack()
	 */
	CModeStack *m_modeStack;

	/** @brief Options factory.
	 *
	 *  Used to create option objects (COption). Derived renderers can use an
	 *  extended set of options.
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see getNewOptionsFactory(), deleteOptionsFactory()
	 */
	COptionsFactory *m_optionsFactory;

	/** @brief Attributes factory.
	 *
	 *  Used to create attribute objects (CAttribute). Derived renderers can use an
	 *  extended set of attributes.
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see getNewAttributesFactory(), deleteAttributesFactory()
	 */
	CAttributesFactory *m_attributesFactory;

	/** @brief Transformation factory.
	 *
	 *  Used to create transformation objects (CTransformation). Derived renderers can use an
	 *  extended set of transformation.
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see getNewTransformationFactory(), deleteTransformationFactory()
	 */
	CTransformationFactory *m_transformationFactory;

	/** @brief Factory for ri macros
	 *
	 *  Used to create ri request objects (interface functions implemented as objects)
	 */
	CRManInterfaceFactory *m_macroFactory;

	/** @brief Factory for pixel filter functions.
	 *
	 *  Used to create pixel filter functions (IFilterFunc). Derived renderers can use an
	 *  extended set of filters.
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see getNewFilterFuncFactory(), deleteFilterFonFactory()
	 */
	CFilterFuncFactory *m_filterFuncFactory;

	/** @brief Resource set of attributes.
	 *
	 *  Used to create attribute resources for use in IRiCPP::resourceV(). Derived renderers can use an
	 *  extended set of attribute resources and also different resources by creating their own factories.
	 *  Initialized while begin() by a call of initRenderState().
	 *
	 *  @see registerResources(), getNewAttributesResourceFactory(), delteAttributesResourceFactory()
	 */
	CAttributesResourceFactory *m_attributesResourceFactory;

protected:
	/** @brief Callbacks for the rib parser to the front end.
	 */
	IRibParserCallback *m_parserCallback;

	/** @brief Creates a new modestack, called by initRenderState().
	 *
	 *  Overwrite this method if you want to return an own modestack.
	 *
	 *  @return A stack object for render modes.
	 */
	inline virtual CModeStack *getNewModeStack()
	{
		return new CModeStack;
	}

	/** @brief Deletes a mode stack, called at destruction.
	 *
	 *  Since the destructor of the mode stack is virtual, there
	 *  is no need to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr A mode stack created by getNewModeStack()
	 */
	inline void deleteModeStack(CModeStack *ptr)
	{
		if ( ptr )
			delete ptr;
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

	/** @brief Deletes an options factory, called at destruction.
	 *
	 *  Since the destructor of the options factory is virtual, there
	 *  is no need to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr An options factory created by getNewOptionsFactory()
	 */
	inline void deleteOptionsFactory(COptionsFactory *ptr)
	{
		if ( ptr )
			delete ptr;
	}

	/** @brief Creates a new attributes factory, called by initRenderState().
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized attributes set.
	 *
	 *  @return A factory object for the render attributes.
	 */
	inline virtual CAttributesFactory *getNewAttributesFactory()
	{
		return new CAttributesFactory;
	}


	/** @brief Deletes an attributes factory, called at destruction.
	 *
	 *  Since the destructor of the attributes factory is virtual, there
	 *  is no need  to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr An attributes factory created by getNewAttributesFactory()
	 */
	inline void deleteAttributesFactory(CAttributesFactory *ptr)
	{
		if ( ptr )
			delete ptr;
	}

	/** @brief Creates a new transformation factory, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized transformation set.
	 *
	 * @return A factory object for the render options.
	 */
	inline virtual CTransformationFactory *getNewTransformationFactory()
	{
		return new CTransformationFactory;
	}

	/** @brief Deletes a transformation factory, called at destruction.
	 *
	 *  Since the destructor of the transformation factory is virtual, there
	 *  is no need to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr An transformation factory created by getNewTransformationFactory()
	 */
	inline void deleteTransformationFactory(CTransformationFactory *ptr)
	{
		if ( ptr )
			delete ptr;
	}

	/** @brief Creates a new pixel filter function factory, called by initRenderState()
	 *
	 *  Overwrite this method if you want to return an own factory
	 *  for a customized pixel filters.
	 *
	 * @return A factory object for the pixel filters.
	 */
	inline virtual CFilterFuncFactory *getNewFilterFuncFactory()
	{
		return new CFilterFuncFactory;
	}

	/** @brief Deletes a pixel filter function factory, called at destruction.
	 *
	 *  Since the destructor of the filter functions factory is virtual, there
	 *  is no need to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr A pixel filter function factory created by getNewFilterFuncFactory()
	 */
	inline void deleteFilterFuncFactory(CFilterFuncFactory *ptr)
	{
		if ( ptr )
			delete ptr;
	}

	/** @brief Create the standard macro factory
	 *  @return Newly created macro factory.
	 */
	virtual CRManInterfaceFactory *getNewMacroFactory();

	/** @brief Method to delete a macro factory
	 *
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 * @param ptr Macro factory to delete (created by getNewMacroFactory())
	 */
	void deleteMacroFactory(CRManInterfaceFactory *ptr);

	/** @brief Gets the current macro factory
	 *  @return macro factory that should be used by the interface
	 */
	virtual CRManInterfaceFactory &macroFactory();

	/** @brief Query if macroFactory() is available
	 *
	 *  This is called by preCheck() to query if the macro factory is available.
	 *
	 *  @return true, macro factory is available via macroFactory()
	 */
	virtual bool hasMacroFactory() const { return m_macroFactory != 0; }


	/** @brief Registers resource factories.
	 *
	 *  Overwrite this if own (other than attributes) resources have to be registered, also.
	 *  The function is called by initRenderState().
	 */
	virtual void registerResources();

	/** @brief Gets a factory for the "attributes" resource.
	 *
	 *  Overwrite this to modify the attributes resource factory. Attribute
	 *  resource objects are used to store and load attribute sets using the
	 *  IRi::resourceV() conmmand.
	 *
	 *  @return A factory for the "attributes" resource.
	 */
	virtual CAttributesResourceFactory *getNewAttributesResourceFactory();

	/** @brief Deletes a attributes resource factory, called at destruction.
	 *
	 *  Since the destructor of the attributes resource factory is virtual, there
	 *  is no need to overwrite this method.
	 *  Because the method is called
	 *  out of the destructor, the method is not virtual.
	 *
	 *  @param ptr An attributes resource factory created by getNewResourceFactory()
	 */
	inline void deleteAttributesResourceFactory(CAttributesResourceFactory *ptr)
	{
		if ( ptr )
			delete ptr;
	}

	/** @brief The backend's error handler
	 *
	 *  A child class can overload ricppErrHandler() to use its own handler
	 *  However it also should return an error handler that throws an ExceptRiCPPError.
	 *
	 *  @return The error handler used by the backend
	 */
	virtual IRiCPPErrorHandler &ricppErrHandler()
	{
		if ( m_parserCallback )
			return m_parserCallback->ricppErrHandler();
		return m_errorExceptionHandler;
	}

	/** @brief Call the default declarations.
	 *
	 *  Overload this to add more default declarations, by calling processDeclare() with
	 *  the @a isDefault attribute set to true.
	 */
	virtual void defaultDeclarations();

	/** @brief Records request in current macro.
	 *
	 *  Called by processRequest
	 *
	 *  @param aRequest Data of a parsed and validated request.
	 */
	virtual void recordRequest(CRManInterfaceCall *aRequest);

	/** @brief Processes a request.
	 *
	 *  Since the processing of the request is nearly always the same,
	 *  it is programmed in this member function. The interface functions
	 *  call this member function with their data stored in a corresponding
	 *  @a aRequest object.
	 *
	 *  @param aRequest Data of a parsed and validated request.
	 *  @param immediatly Executes immediatly, does not store request at a macro, does nt use if/then/else state.
	 */
	virtual void processRequest(CRManInterfaceCall *aRequest, bool immediatly = false);

	/** @brief Replays a stored request.
	 *
	 *  Like processRequest() but used a previously stored request
	 *  (archive or object)
	 *
	 *  @param aRequest A parsed and validated request.
	 *  @param cb The optional callback (from IRiCPP::readArchive())
	 */
	virtual void replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb);

	/** @brief Processes the replay of an archive.
	 */
	virtual void processArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params);

	/** @brief Reads an archive from a stream.
	 *
	 *  @param name Name of the stream (@a name == RI_NULL for stdin)
	 *  @param parserCallback Info for parser
	 *  @param callback Ri archive callback
	 *  @param params Copy of the parameters of the readArchiveV() call
	 */
	virtual void readArchiveFromStream(RtString name, IRibParserCallback &parserCallback, const IArchiveCallback *callback, const CParameterList &params);

	/** @brief Reads an archive from memory or stream.
	 *
	 *  @param name Name of the archive (@a name == RI_NULL for stdin)
	 *  @param callback Ri archive callback
	 *  @param params Copy of the parameters of the readArchiveV() call
	 */
	virtual void processReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params);

public:

	virtual bool init(const CDeclarationDictionary &theDeclDict, const COptions &theOptions, const COptionsBase &theControls);

	virtual bool preCheck(EnumRequests req);

	inline virtual const CRenderState *renderState() const { return m_renderState; }
	inline virtual CRenderState *renderState() { return m_renderState; }

	/** @brief Constructor, initializes member variables.
	 */
	CBaseRenderer();

	/** @brief Virtual destruction
	 */
	virtual ~CBaseRenderer();

	virtual void registerRibParserCallback(IRibParserCallback &cb);
	inline virtual IRi *frontend() { return m_parserCallback ? &m_parserCallback->frontend() : 0; }
	inline virtual CBackBufferProtocolHandlers *protocolHandlers() { return m_parserCallback ? &m_parserCallback->protocolHandlers() : 0; }

	/** @brief Is called by the frontend, if the renderer is aborted due to severe errors.
	 */
	inline virtual RtVoid abort(void) {}

	/** @brief Is called, if the renderer is activated by context()
	 *
	 * If the context will be the new active rendering context, it is activated by its
	 * context creator.
	 */
	inline virtual RtVoid activate(void) {}

	/** @brief Is called, if the renderer is deactivated by context()
	 *
	 * If the context will be the suspended rendering context, it is deactivated by its
	 * context creator.
	 */
	inline virtual RtVoid deactivate(void) {}

	inline virtual RtVoid version() {}

	inline virtual RtInt lastError()
	{
		if ( m_parserCallback )
			return m_parserCallback->frontend().lastError();
		return RIE_NOERROR;
	}

	virtual RtVoid errorHandler(const IErrorHandler &handler);

	virtual RtToken declare(RtToken RtToken, RtString declaration);

	virtual RtVoid controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid synchronize(RtToken name);
	virtual RtVoid system(RtString cmd);
	virtual RtVoid resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

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

	virtual RtObjectHandle objectBegin(RtString name);
	virtual RtVoid objectEnd(void);
	virtual RtVoid objectInstance(RtObjectHandle handle);

	virtual RtVoid motionBeginV(RtInt N, RtFloat times[]);
	virtual RtVoid motionEnd(void);

	virtual RtVoid resourceBegin(void);
	virtual RtVoid resourceEnd(void);

	virtual RtArchiveHandle archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid archiveEnd(void);

	virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);
	virtual RtVoid frameAspectRatio(RtFloat aspect);
	virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
	virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	virtual RtVoid projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
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
	virtual RtVoid colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]);
	virtual RtVoid relativeDetail(RtFloat relativedetail);
	virtual RtVoid optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
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
	virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

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
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[],  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], RtInt n, RtToken tokens[], RtPointer params[]);

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

	virtual RtVoid procedural(RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc);

	virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid archiveRecordV(RtToken type, RtString line);
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid ifBegin(RtString expr);
	virtual RtVoid elseIfBegin(RtString expr);
	virtual RtVoid elseBegin(void);
	virtual RtVoid ifEnd(void);

public:
	virtual RtVoid preSynchronize(CRiSynchronize &obj, RtToken name);

	virtual RtVoid preErrorHandler(CRiErrorHandler &obj, const IErrorHandler &handler);
	virtual RtVoid preDeclare(CRiDeclare &obj, RtToken name, RtString declaration);
    virtual RtVoid preControl(CRiControl &obj, RtToken name, const CParameterList &params);

	virtual RtVoid preSystem(CRiSystem &obj, RtString cmd);

	virtual RtVoid preBegin(CRiBegin &obj, RtString name, const CParameterList &params);
	virtual RtVoid preEnd(CRiEnd &obj);

	virtual RtVoid preFrameBegin(CRiFrameBegin &obj, RtInt number);
	virtual RtVoid preFrameEnd(CRiFrameEnd &obj);

	virtual RtVoid preWorldBegin(CRiWorldBegin &obj);
	virtual RtVoid preWorldEnd(CRiWorldEnd &obj);

	virtual RtVoid preAttributeBegin(CRiAttributeBegin &obj);
	virtual RtVoid preAttributeEnd(CRiAttributeEnd &obj);

	virtual RtVoid preTransformBegin(CRiTransformBegin &obj);
	virtual RtVoid preTransformEnd(CRiTransformEnd &obj);

    virtual RtVoid preSolidBegin(CRiSolidBegin &obj, RtToken type);
    virtual RtVoid preSolidEnd(CRiSolidEnd &obj);

	virtual RtVoid preObjectBegin(CRiObjectBegin &obj, RtString name);
	virtual RtVoid preObjectEnd(CRiObjectEnd &obj);
    virtual RtVoid preObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle);

    virtual RtVoid preMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]);
    virtual RtVoid preMotionEnd(CRiMotionEnd &obj);

	virtual RtVoid preResourceBegin(CRiResourceBegin &obj);
	virtual RtVoid preResourceEnd(CRiResourceEnd &obj);
	virtual RtVoid preResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params);

	virtual RtVoid preArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params);
	virtual RtVoid preArchiveEnd(CRiArchiveEnd &objArchiveEnd);

    virtual RtVoid preFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid preFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect);

	virtual RtVoid preScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid preCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	virtual RtVoid preProjection(CRiProjection &obj, RtToken name, const CParameterList &params);
	virtual RtVoid preClipping(CRiClipping &obj, RtFloat hither, RtFloat yon);
    virtual RtVoid preClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
    virtual RtVoid preDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
    virtual RtVoid preShutter(CRiShutter &obj, RtFloat smin, RtFloat smax);
	virtual RtVoid prePixelVariance(CRiPixelVariance &obj, RtFloat variation);
    virtual RtVoid prePixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples);
    virtual RtVoid prePixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
    virtual RtVoid preExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma);
    virtual RtVoid preImager(CRiImager &obj, RtString name, const CParameterList &params);
	virtual RtVoid preQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
    virtual RtVoid preDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params);
    virtual RtVoid preDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params);
    virtual RtVoid preHider(CRiHider &obj, RtToken type, const CParameterList &params);
    virtual RtVoid preColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]);
    virtual RtVoid preRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail);
    virtual RtVoid preOption(CRiOption &obj, RtToken name, const CParameterList &params);
	
    virtual RtVoid preLightSource(CRiLightSource &obj, RtString name, const CParameterList &params);
	virtual RtVoid preAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params);
	
    virtual RtVoid preAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params);
	virtual RtVoid preColor(CRiColor &obj, RtColor Cs);
	virtual RtVoid preOpacity(CRiOpacity &obj, RtColor Os);
    virtual RtVoid preSurface(CRiSurface &obj, RtString name, const CParameterList &params);
    virtual RtVoid preAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params);
    virtual RtVoid preInterior(CRiInterior &obj, RtString name, const CParameterList &params);
	virtual RtVoid preExterior(CRiExterior &obj, RtString name, const CParameterList &params);
	virtual RtVoid preIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff);
    virtual RtVoid preDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params);
	virtual RtVoid preTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
    virtual RtVoid preShadingRate(CRiShadingRate &obj, RtFloat size);
	virtual RtVoid preShadingInterpolation(CRiShadingInterpolation &obj, RtToken type);
    virtual RtVoid preMatte(CRiMatte &obj, RtBoolean onoff);
	virtual RtVoid preBound(CRiBound &obj, RtBound bound);
	virtual RtVoid preDetail(CRiDetail &obj, RtBound bound);
	virtual RtVoid preDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid preGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value);
	virtual RtVoid preGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type);
	virtual RtVoid preOrientation(CRiOrientation &obj, RtToken anOrientation);
	virtual RtVoid preReverseOrientation(CRiReverseOrientation &obj);
	virtual RtVoid preSides(CRiSides &obj, RtInt nsides);
    virtual RtVoid preBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid preTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

	virtual RtVoid preIdentity(CRiIdentity &obj);
	virtual RtVoid preTransform(CRiTransform &obj, RtMatrix aTransform);
	virtual RtVoid preConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform);
	virtual RtVoid prePerspective(CRiPerspective &obj, RtFloat fov);
	virtual RtVoid preTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid preRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid preScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid preSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
	virtual RtVoid preDeformation(CRiDeformation &obj, RtString name, const CParameterList &params);
	virtual RtVoid preScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space);
	virtual RtVoid preCoordinateSystem(CRiCoordinateSystem &obj, RtToken space);
	virtual RtVoid preCoordSysTransform(CRiCoordSysTransform &obj, RtToken space);
	virtual RtVoid preTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

    inline virtual RtVoid prePolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid preGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) {}
	inline virtual RtVoid prePointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) {}
    inline virtual RtVoid prePointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) {}

	inline virtual RtVoid prePatch(CRiPatch &obj, RtToken type, const CParameterList &params) {}
	inline virtual RtVoid prePatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
    inline virtual RtVoid preNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid preSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid preHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid preSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid preTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

    inline virtual RtVoid prePoints(CRiPoints &obj, RtInt npts, const CParameterList &params) {}
    inline virtual RtVoid preCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid preBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	virtual RtVoid preProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc);

	inline virtual RtVoid preGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid preMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid preMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid preMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid preMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid preMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) {}
    inline virtual RtVoid preMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid preArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) {}
	inline virtual RtVoid preReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params) {}

	virtual RtVoid preIfBegin(CRiIfBegin &obj, RtString expr);
	virtual RtVoid preElseIfBegin(CRiElseIfBegin &obj, RtString expr);
	virtual RtVoid preElseBegin(CRiElseBegin &obj);
	virtual RtVoid preIfEnd(CRiIfEnd &obj);

public:
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}

	inline virtual RtVoid doSynchronize(CRiSynchronize &obj, RtToken name) {}

	inline virtual RtVoid doErrorHandler(CRiErrorHandler &obj,const IErrorHandler &handler) {}
	inline virtual RtVoid doDeclare(CRiDeclare &obj,RtToken name, RtString declaration) {}
    inline virtual RtVoid doControl(CRiControl &obj,RtToken name, const CParameterList &params) {}

	inline virtual RtVoid doSystem(CRiSystem &obj, RtString cmd) {}

	inline virtual RtVoid doBegin(CRiBegin &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doEnd(CRiEnd &obj) {}

	inline virtual RtVoid doFrameBegin(CRiFrameBegin &obj, RtInt number) {}
	inline virtual RtVoid doFrameEnd(CRiFrameEnd &obj) {}

	inline virtual RtVoid doWorldBegin(CRiWorldBegin &obj) {}
	inline virtual RtVoid doWorldEnd(CRiWorldEnd &obj) {}

	inline virtual RtVoid doAttributeBegin(CRiAttributeBegin &obj) {}
	inline virtual RtVoid doAttributeEnd(CRiAttributeEnd &obj) {}

	inline virtual RtVoid doTransformBegin(CRiTransformBegin &obj) {}
	inline virtual RtVoid doTransformEnd(CRiTransformEnd &obj) {}

    inline virtual RtVoid doSolidBegin(CRiSolidBegin &obj, RtToken type) {}
    inline virtual RtVoid doSolidEnd(CRiSolidEnd &obj) {}

	inline virtual RtVoid doObjectBegin(CRiObjectBegin &obj, RtString name) {}
	inline virtual RtVoid doObjectEnd(CRiObjectEnd &obj) {}
    virtual RtVoid doObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle);

    inline virtual RtVoid doMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]) {}
    inline virtual RtVoid doMotionEnd(CRiMotionEnd &obj) {}

	inline virtual RtVoid doResourceBegin(CRiResourceBegin &obj) {}
	inline virtual RtVoid doResourceEnd(CRiResourceEnd &obj) {}
	inline virtual RtVoid doResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid doArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doArchiveEnd(CRiArchiveEnd &objArchiveEnd) {}

    inline virtual RtVoid doFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid doFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect) {}

	inline virtual RtVoid doScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid doCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
	virtual RtVoid doProjection(CRiProjection &obj, RtToken name, const CParameterList &params);
	inline virtual RtVoid doClipping(CRiClipping &obj, RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid doClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid doDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid doShutter(CRiShutter &obj, RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid doPixelVariance(CRiPixelVariance &obj, RtFloat variation) {}
    inline virtual RtVoid doPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid doPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid doExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid doImager(CRiImager &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
    inline virtual RtVoid doDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params) {}
    inline virtual RtVoid doDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params) {}
    inline virtual RtVoid doHider(CRiHider &obj, RtToken type, const CParameterList &params) {}
    inline virtual RtVoid doColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]) {}
    inline virtual RtVoid doRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail) {}
    inline virtual RtVoid doOption(CRiOption &obj, RtToken name, const CParameterList &params) {}
	
    virtual RtVoid doLightSource(CRiLightSource &obj, RtString name, const CParameterList &params);
	virtual RtVoid doAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params);
	
    inline virtual RtVoid doAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doColor(CRiColor &obj, RtColor Cs) {}
	inline virtual RtVoid doOpacity(CRiOpacity &obj, RtColor Os) {}
    inline virtual RtVoid doSurface(CRiSurface &obj, RtString name, const CParameterList &params) {}
    inline virtual RtVoid doAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params) {}
    inline virtual RtVoid doInterior(CRiInterior &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doExterior(CRiExterior &obj, RtString name, const CParameterList &params) {}
	virtual RtVoid doIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff);
    inline virtual RtVoid doDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid doShadingRate(CRiShadingRate &obj, RtFloat size) {}
	inline virtual RtVoid doShadingInterpolation(CRiShadingInterpolation &obj, RtToken type) {}
    inline virtual RtVoid doMatte(CRiMatte &obj, RtBoolean onoff) {}
	inline virtual RtVoid doBound(CRiBound &obj, RtBound bound) {}
	inline virtual RtVoid doDetail(CRiDetail &obj, RtBound bound) {}
	inline virtual RtVoid doDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid doGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value) {}
	inline virtual RtVoid doGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type) {}
	inline virtual RtVoid doOrientation(CRiOrientation &obj, RtToken anOrientation) {}
	inline virtual RtVoid doReverseOrientation(CRiReverseOrientation &obj) {}
	inline virtual RtVoid doSides(CRiSides &obj, RtInt nsides) {}
    inline virtual RtVoid doBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid doTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) {}

	inline virtual RtVoid doIdentity(CRiIdentity &obj) {}
	inline virtual RtVoid doTransform(CRiTransform &obj, RtMatrix aTransform) {}
	inline virtual RtVoid doConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform) {}
	inline virtual RtVoid doPerspective(CRiPerspective &obj, RtFloat fov) {}
	inline virtual RtVoid doTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz) {}
    inline virtual RtVoid doSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid doDeformation(CRiDeformation &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space) {}
	inline virtual RtVoid doCoordinateSystem(CRiCoordinateSystem &obj, RtToken space) {}
	inline virtual RtVoid doCoordSysTransform(CRiCoordSysTransform &obj, RtToken space) {}
	virtual RtVoid doTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

    inline virtual RtVoid doPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid doGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) {}
	inline virtual RtVoid doPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) {}
    inline virtual RtVoid doPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) {}

	inline virtual RtVoid doPatch(CRiPatch &obj, RtToken type, const CParameterList &params) {}
	inline virtual RtVoid doPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
    inline virtual RtVoid doNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid doSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid doHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid doSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

    inline virtual RtVoid doPoints(CRiPoints &obj, RtInt npts, const CParameterList &params) {}
    inline virtual RtVoid doCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid doBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	virtual RtVoid doProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc);

	inline virtual RtVoid doGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid doMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) {}
    inline virtual RtVoid doMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid doArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) {}
	virtual RtVoid doReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params);

	inline virtual RtVoid doIfBegin(CRiIfBegin &obj, RtString expr) {}
	inline virtual RtVoid doElseIfBegin(CRiElseIfBegin &obj, RtString expr) {}
	inline virtual RtVoid doElseBegin(CRiElseBegin &obj) {}
	inline virtual RtVoid doIfEnd(CRiIfEnd &obj) {}

public:
	inline virtual RtVoid postSynchronize(CRiSynchronize &obj, RtToken name) {}

	inline virtual RtVoid postErrorHandler(CRiErrorHandler &obj,const IErrorHandler &handler) {}
	inline virtual RtVoid postDeclare(CRiDeclare &obj,RtToken name, RtString declaration) {}
    inline virtual RtVoid postControl(CRiControl &obj,RtToken name, const CParameterList &params) {}

	inline virtual RtVoid postSystem(CRiSystem &obj, RtString cmd) {}

	inline virtual RtVoid postBegin(CRiBegin &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postEnd(CRiEnd &obj) {}

	inline virtual RtVoid postFrameBegin(CRiFrameBegin &obj, RtInt number) {}
	inline virtual RtVoid postFrameEnd(CRiFrameEnd &obj) {}

	inline virtual RtVoid postWorldBegin(CRiWorldBegin &obj) {}
	inline virtual RtVoid postWorldEnd(CRiWorldEnd &obj) {}

	inline virtual RtVoid postAttributeBegin(CRiAttributeBegin &obj) {}
	inline virtual RtVoid postAttributeEnd(CRiAttributeEnd &obj) {}

	inline virtual RtVoid postTransformBegin(CRiTransformBegin &obj) {}
	inline virtual RtVoid postTransformEnd(CRiTransformEnd &obj) {}

    inline virtual RtVoid postSolidBegin(CRiSolidBegin &obj, RtToken type) {}
    inline virtual RtVoid postSolidEnd(CRiSolidEnd &obj) {}

	inline virtual RtVoid postObjectBegin(CRiObjectBegin &obj, RtString name) {}
	inline virtual RtVoid postObjectEnd(CRiObjectEnd &obj) {}
    inline virtual RtVoid postObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle) {}

    inline virtual RtVoid postMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]) {}
    inline virtual RtVoid postMotionEnd(CRiMotionEnd &obj) {}

	inline virtual RtVoid postResourceBegin(CRiResourceBegin &obj) {}
	inline virtual RtVoid postResourceEnd(CRiResourceEnd &obj) {}
	inline virtual RtVoid postResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid postArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid postArchiveEnd(CRiArchiveEnd &objArchiveEnd) {}

    inline virtual RtVoid postFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid postFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect) {}

	inline virtual RtVoid postScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid postCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
	inline virtual RtVoid postProjection(CRiProjection &obj, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid postClipping(CRiClipping &obj, RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid postClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid postDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid postShutter(CRiShutter &obj, RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid postPixelVariance(CRiPixelVariance &obj, RtFloat variation) {}
    inline virtual RtVoid postPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid postPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid postExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid postImager(CRiImager &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
    inline virtual RtVoid postDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params) {}
    inline virtual RtVoid postDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params) {}
    inline virtual RtVoid postHider(CRiHider &obj, RtToken type, const CParameterList &params) {}
    inline virtual RtVoid postColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]) {}
    inline virtual RtVoid postRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail) {}
    inline virtual RtVoid postOption(CRiOption &obj, RtToken name, const CParameterList &params) {}
	
    inline virtual RtVoid postLightSource(CRiLightSource &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params) {}
	
    inline virtual RtVoid postAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid postColor(CRiColor &obj, RtColor Cs) {}
	inline virtual RtVoid postOpacity(CRiOpacity &obj, RtColor Os) {}
    inline virtual RtVoid postSurface(CRiSurface &obj, RtString name, const CParameterList &params) {}
    inline virtual RtVoid postAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params) {}
    inline virtual RtVoid postInterior(CRiInterior &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postExterior(CRiExterior &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff) {}
    inline virtual RtVoid postDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid postShadingRate(CRiShadingRate &obj, RtFloat size) {}
	inline virtual RtVoid postShadingInterpolation(CRiShadingInterpolation &obj, RtToken type) {}
    inline virtual RtVoid postMatte(CRiMatte &obj, RtBoolean onoff) {}
	inline virtual RtVoid postBound(CRiBound &obj, RtBound bound) {}
	inline virtual RtVoid postDetail(CRiDetail &obj, RtBound bound) {}
	inline virtual RtVoid postDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid postGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value) {}
	inline virtual RtVoid postGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type) {}
	inline virtual RtVoid postOrientation(CRiOrientation &obj, RtToken anOrientation) {}
	inline virtual RtVoid postReverseOrientation(CRiReverseOrientation &obj) {}
	inline virtual RtVoid postSides(CRiSides &obj, RtInt nsides) {}
    inline virtual RtVoid postBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid postTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) {}

	inline virtual RtVoid postIdentity(CRiIdentity &obj) {}
	inline virtual RtVoid postTransform(CRiTransform &obj, RtMatrix aTransform) {}
	inline virtual RtVoid postConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform) {}
	inline virtual RtVoid postPerspective(CRiPerspective &obj, RtFloat fov) {}
	inline virtual RtVoid postTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid postRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid postScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz) {}
    inline virtual RtVoid postSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid postDeformation(CRiDeformation &obj, RtString name, const CParameterList &params) {}
	inline virtual RtVoid postScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space) {}
	inline virtual RtVoid postCoordinateSystem(CRiCoordinateSystem &obj, RtToken space) {}
	inline virtual RtVoid postCoordSysTransform(CRiCoordSysTransform &obj, RtToken space) {}
	inline virtual RtVoid postTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {}

    inline virtual RtVoid postPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid postGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) {}
	inline virtual RtVoid postPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) {}
    inline virtual RtVoid postPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) {}

	inline virtual RtVoid postPatch(CRiPatch &obj, RtToken type, const CParameterList &params) {}
	inline virtual RtVoid postPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
    inline virtual RtVoid postNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid postSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid postHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid postSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid postTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

    inline virtual RtVoid postPoints(CRiPoints &obj, RtInt npts, const CParameterList &params) {}
    inline virtual RtVoid postCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid postBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	inline virtual RtVoid postProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc) {}

	inline virtual RtVoid postGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) {}

	inline virtual RtVoid postMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid postMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid postMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid postMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid postMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) {}
    inline virtual RtVoid postMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid postArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) {}
	inline virtual RtVoid postReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params) {}

	inline virtual RtVoid postIfBegin(CRiIfBegin &obj, RtString expr) {}
	inline virtual RtVoid postElseIfBegin(CRiElseIfBegin &obj, RtString expr) {}
	inline virtual RtVoid postElseBegin(CRiElseBegin &obj) {}
	inline virtual RtVoid postIfEnd(CRiIfEnd &obj) {}
}; // CBaseRenderer

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_BASERENDERER_H
