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

#ifndef _RICPP_RICONTEXT_SURFACE_H
#include "ricpp/ricontext/surface.h"
#endif // _RICPP_RICONTEXT_SURFACE_H

namespace RiCPP {

/** @todo Need to rethink the CRenderState - CBaseRenderer 1:1 relationship, the seperation of affairs is ugly by design.
 *        E.g. replay mode maybe better a part of the state.
 */
	
/** @brief This class is used to implement the basis of a renderer context.
 *  
 *  Finds the unique strings (tokens) for RtTokens and
 *  it does type checking and calls the IDoRenderer do...() members if the checking was successful.
 *  Maintains a render state.
 *  The concrete context inherits from CBaseRenderer and implements the IDoRenderer members for
 *  its rendering jobs. The CBaseRenderer is the foundation of the backend renderer context.
 */
class CBaseRenderer : public IDoRender {
	
protected:
	typedef IDoRender TypeParent;

private:
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

	///! Delayed requests
	class CDelayedRequest {
	public:
		CRManInterfaceCall *m_req; //!< Request (graphics primitive for delayed call)
		CAttributes *m_attributes; //!< Attribute set that was valid while request was called
		CTransformation *m_transformation; //!< To world transformation (cuurent valid while request was called)
		inline CDelayedRequest(CRManInterfaceCall *req, CAttributes *attrib, CTransformation *trans)
		: m_req(req), m_attributes(attrib), m_transformation(trans) {}
	};
	std::list<CDelayedRequest> m_delayedRequests; //!< used to store delayed requests
	bool m_replayDelayedMode; //!< true, while replaying of delayed requests (controlled via doWorldEnd())
	CAttributes *m_attributes; //!< Attributes of delayed request
	CTransformation *m_transformation; //!< Transformation of delayed request

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
	 *  Overload this to add more default declarations, by calling renderState().declare() with
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

	/** @brief Hides (renders) a single face of a surface
	 *  @param f Single face of a surface
	 */
	inline virtual void hide(const CFace &f) {}
	
	/** @brief Hides (renders) a surface, calls hide for each face
	 *  @param s Surface
	 */
	virtual void hide(const CSurface *s);

	const CAttributes &attributes() const;
	CAttributes &attributes();
	const CTransformation &transformation() const;
	CTransformation &transformation();
	
	inline bool replayMode() const { return m_replayDelayedMode; }
	void initDelayed();
	void replayDelayed();

	virtual bool delayRequest(CRManInterfaceCall &obj);
	
	/** @brief Gets the current to camera matrix
	 *
	 *  Matrix is only valid inside a world block.
	 *
	 *  @return Matrix transforms from current to camera coordinate system.
	 */
	CMatrix3D toCamera() const;
	CMatrix3D toRaster() const;

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

	bool isOpaque(const std::vector<RtFloat> &opacity);
	RtFloat opacityToAlpha(const std::vector<RtFloat> &opacity);	
	void getColor3f(const std::vector<RtFloat> &fromC, RtFloat toC[3], const CColorDescr &c, RtFloat gain, RtFloat gamma);

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
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}
	
public:
	inline virtual RtVoid preProcess(CRiSynchronize &obj) {}
	
	inline virtual RtVoid preProcess(CRiErrorHandler &obj) {}
	inline virtual RtVoid preProcess(CRiDeclare &obj) {}
    virtual RtVoid preProcess(CRiControl &obj);
	
	inline virtual RtVoid preProcess(CRiSystem &obj) {}
	
	virtual RtVoid preProcess(CRiBegin &obj);
	virtual RtVoid preProcess(CRiEnd &obj);
	
	virtual RtVoid preProcess(CRiFrameBegin &obj);
	virtual RtVoid preProcess(CRiFrameEnd &obj);
	
	virtual RtVoid preProcess(CRiWorldBegin &obj);
	virtual RtVoid preProcess(CRiWorldEnd &obj);
	
	virtual RtVoid preProcess(CRiAttributeBegin &obj);
	virtual RtVoid preProcess(CRiAttributeEnd &obj);
	
	virtual RtVoid preProcess(CRiTransformBegin &obj);
	virtual RtVoid preProcess(CRiTransformEnd &obj);
	
    virtual RtVoid preProcess(CRiSolidBegin &obj);
    virtual RtVoid preProcess(CRiSolidEnd &obj);
	
	inline virtual RtVoid preProcess(CRiObjectBegin &obj) {}
	inline virtual RtVoid preProcess(CRiObjectEnd &obj) {}
    inline virtual RtVoid preProcess(CRiObjectInstance &obj) {}
	
    virtual RtVoid preProcess(CRiMotionBegin &obj);
    virtual RtVoid preProcess(CRiMotionEnd &obj);
	
	virtual RtVoid preProcess(CRiResourceBegin &obj);
	virtual RtVoid preProcess(CRiResourceEnd &obj);
	virtual RtVoid preProcess(CRiResource &obj);
	
	inline virtual RtVoid preProcess(CRiArchiveBegin &obj) {}
	inline virtual RtVoid preProcess(CRiArchiveEnd &objArchiveEnd) {}
	
    virtual RtVoid preProcess(CRiFormat &obj);
    virtual RtVoid preProcess(CRiFrameAspectRatio &obj);
	
	virtual RtVoid preProcess(CRiScreenWindow &obj);
    virtual RtVoid preProcess(CRiCropWindow &obj);
	virtual RtVoid preProcess(CRiProjection &obj);
	virtual RtVoid preProcess(CRiClipping &obj);
    virtual RtVoid preProcess(CRiClippingPlane &obj);
    virtual RtVoid preProcess(CRiDepthOfField &obj);
    virtual RtVoid preProcess(CRiShutter &obj);
	virtual RtVoid preProcess(CRiPixelVariance &obj);
    virtual RtVoid preProcess(CRiPixelSamples &obj);
    virtual RtVoid preProcess(CRiPixelFilter &obj);
    virtual RtVoid preProcess(CRiExposure &obj);
    virtual RtVoid preProcess(CRiImager &obj);
	virtual RtVoid preProcess(CRiQuantize &obj);
    virtual RtVoid preProcess(CRiDisplayChannel &obj);
    virtual RtVoid preProcess(CRiDisplay &obj);
    virtual RtVoid preProcess(CRiHider &obj);
    virtual RtVoid preProcess(CRiColorSamples &obj);
    virtual RtVoid preProcess(CRiRelativeDetail &obj);
    virtual RtVoid preProcess(CRiOption &obj);
	
    virtual RtVoid preProcess(CRiLightSource &obj);
	virtual RtVoid preProcess(CRiAreaLightSource &obj);
	
    virtual RtVoid preProcess(CRiAttribute &obj);
	virtual RtVoid preProcess(CRiColor &obj);
	virtual RtVoid preProcess(CRiOpacity &obj);
    virtual RtVoid preProcess(CRiSurface &obj);
    virtual RtVoid preProcess(CRiAtmosphere &obj);
    virtual RtVoid preProcess(CRiInterior &obj);
	virtual RtVoid preProcess(CRiExterior &obj);
	virtual RtVoid preProcess(CRiIlluminate &obj);
    virtual RtVoid preProcess(CRiDisplacement &obj);
	virtual RtVoid preProcess(CRiTextureCoordinates &obj);
    virtual RtVoid preProcess(CRiShadingRate &obj);
	virtual RtVoid preProcess(CRiShadingInterpolation &obj);
    virtual RtVoid preProcess(CRiMatte &obj);
	virtual RtVoid preProcess(CRiBound &obj);
	virtual RtVoid preProcess(CRiDetail &obj);
	virtual RtVoid preProcess(CRiDetailRange &obj);
    virtual RtVoid preProcess(CRiGeometricApproximation &obj);
	virtual RtVoid preProcess(CRiGeometricRepresentation &obj);
	virtual RtVoid preProcess(CRiOrientation &obj);
	virtual RtVoid preProcess(CRiReverseOrientation &obj);
	virtual RtVoid preProcess(CRiSides &obj);
	virtual RtVoid preProcess(CRiBasis &obj);
	virtual RtVoid preProcess(CRiTrimCurve &obj);
	
	virtual RtVoid preProcess(CRiIdentity &obj);
	virtual RtVoid preProcess(CRiTransform &obj);
	virtual RtVoid preProcess(CRiConcatTransform &obj);
	virtual RtVoid preProcess(CRiPerspective &obj);
	virtual RtVoid preProcess(CRiTranslate &obj);
	virtual RtVoid preProcess(CRiRotate &obj);
	virtual RtVoid preProcess(CRiScale &obj);
    virtual RtVoid preProcess(CRiSkew &obj);
	virtual RtVoid preProcess(CRiDeformation &obj);
	virtual RtVoid preProcess(CRiScopedCoordinateSystem &obj);
	virtual RtVoid preProcess(CRiCoordinateSystem &obj);
	virtual RtVoid preProcess(CRiCoordSysTransform &obj);
	inline virtual RtVoid preProcess(CRiTransformPoints &obj) {}
	
    inline virtual RtVoid preProcess(CRiPolygon &obj) {}
	inline virtual RtVoid preProcess(CRiGeneralPolygon &obj) {}
	inline virtual RtVoid preProcess(CRiPointsPolygons &obj) {}
    inline virtual RtVoid preProcess(CRiPointsGeneralPolygons &obj) {}

	inline virtual RtVoid preProcess(CRiPatch &obj) {}
	inline virtual RtVoid preProcess(CRiPatchMesh &obj) {}
    inline virtual RtVoid preProcess(CRiNuPatch &obj) {}

	inline virtual RtVoid preProcess(CRiSubdivisionMesh &obj) {}
	inline virtual RtVoid preProcess(CRiHierarchicalSubdivisionMesh &obj) {}

	inline virtual RtVoid preProcess(CRiSphere &obj) {}
    inline virtual RtVoid preProcess(CRiCone &obj) {}
    inline virtual RtVoid preProcess(CRiCylinder &obj) {}
    inline virtual RtVoid preProcess(CRiHyperboloid &obj) {}
    inline virtual RtVoid preProcess(CRiParaboloid &obj) {}
    inline virtual RtVoid preProcess(CRiDisk &obj) {}
    inline virtual RtVoid preProcess(CRiTorus &obj) {}

    inline virtual RtVoid preProcess(CRiPoints &obj) {}
    inline virtual RtVoid preProcess(CRiCurves &obj) {}

	inline virtual RtVoid preProcess(CRiBlobby &obj) {}

	virtual RtVoid preProcess(CRiProcedural &obj);

	inline virtual RtVoid preProcess(CRiGeometry &obj) {}

	inline virtual RtVoid preProcess(CRiMakeTexture &obj) {}
    inline virtual RtVoid preProcess(CRiMakeBump &obj) {}
    inline virtual RtVoid preProcess(CRiMakeLatLongEnvironment &obj) {}
    inline virtual RtVoid preProcess(CRiMakeCubeFaceEnvironment &obj) {}
    inline virtual RtVoid preProcess(CRiMakeShadow &obj) {}
    inline virtual RtVoid preProcess(CRiMakeBrickMap &obj) {}

	inline virtual RtVoid preProcess(CRiArchiveRecord &obj) {}
	inline virtual RtVoid preProcess(CRiReadArchive &obj) {}

	virtual RtVoid preProcess(CRiIfBegin &obj);
	virtual RtVoid preProcess(CRiElseIfBegin &obj);
	virtual RtVoid preProcess(CRiElseBegin &obj);
	virtual RtVoid preProcess(CRiIfEnd &obj);

public:
	inline virtual RtVoid doProcess(CRiSynchronize &obj) {}
	
	inline virtual RtVoid doProcess(CRiErrorHandler &obj) {}
	inline virtual RtVoid doProcess(CRiDeclare &obj) {}
    inline virtual RtVoid doProcess(CRiControl &obj) {}
	
	inline virtual RtVoid doProcess(CRiSystem &obj) {}
	
	inline virtual RtVoid doProcess(CRiBegin &obj) {}
	inline virtual RtVoid doProcess(CRiEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiFrameBegin &obj) {}
	inline virtual RtVoid doProcess(CRiFrameEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiWorldBegin &obj) {}
	inline virtual RtVoid doProcess(CRiWorldEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiAttributeBegin &obj) {}
	inline virtual RtVoid doProcess(CRiAttributeEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiTransformBegin &obj) {}
	inline virtual RtVoid doProcess(CRiTransformEnd &obj) {}
	
    inline virtual RtVoid doProcess(CRiSolidBegin &obj) {}
    inline virtual RtVoid doProcess(CRiSolidEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiObjectBegin &obj) {}
	inline virtual RtVoid doProcess(CRiObjectEnd &obj) {}
    virtual RtVoid doProcess(CRiObjectInstance &obj);

    inline virtual RtVoid doProcess(CRiMotionBegin &obj) {}
    inline virtual RtVoid doProcess(CRiMotionEnd &obj) {}
	
	inline virtual RtVoid doProcess(CRiResourceBegin &obj) {}
	inline virtual RtVoid doProcess(CRiResourceEnd &obj) {}
	inline virtual RtVoid doProcess(CRiResource &obj) {}
	
	inline virtual RtVoid doProcess(CRiArchiveBegin &obj) {}
	inline virtual RtVoid doProcess(CRiArchiveEnd &objArchiveEnd) {}
	
    inline virtual RtVoid doProcess(CRiFormat &obj) {}
    inline virtual RtVoid doProcess(CRiFrameAspectRatio &obj) {}
	
	inline virtual RtVoid doProcess(CRiScreenWindow &obj) {}
    inline virtual RtVoid doProcess(CRiCropWindow &obj) {}
	inline virtual RtVoid doProcess(CRiProjection &obj) {}
	inline virtual RtVoid doProcess(CRiClipping &obj) {}
    inline virtual RtVoid doProcess(CRiClippingPlane &obj) {}
    inline virtual RtVoid doProcess(CRiDepthOfField &obj) {}
    inline virtual RtVoid doProcess(CRiShutter &obj) {}
	inline virtual RtVoid doProcess(CRiPixelVariance &obj) {}
    inline virtual RtVoid doProcess(CRiPixelSamples &obj) {}
    inline virtual RtVoid doProcess(CRiPixelFilter &obj) {}
    inline virtual RtVoid doProcess(CRiExposure &obj) {}
    inline virtual RtVoid doProcess(CRiImager &obj) {}
	inline virtual RtVoid doProcess(CRiQuantize &obj) {}
    inline virtual RtVoid doProcess(CRiDisplayChannel &obj) {}
    inline virtual RtVoid doProcess(CRiDisplay &obj) {}
    inline virtual RtVoid doProcess(CRiHider &obj) {}
    inline virtual RtVoid doProcess(CRiColorSamples &obj) {}
    inline virtual RtVoid doProcess(CRiRelativeDetail &obj) {}
    inline virtual RtVoid doProcess(CRiOption &obj) {}
	
    virtual RtVoid doProcess(CRiLightSource &obj);
	virtual RtVoid doProcess(CRiAreaLightSource &obj);
	
    inline virtual RtVoid doProcess(CRiAttribute &obj) {}
	inline virtual RtVoid doProcess(CRiColor &obj) {}
	inline virtual RtVoid doProcess(CRiOpacity &obj) {}
    inline virtual RtVoid doProcess(CRiSurface &obj) {}
    inline virtual RtVoid doProcess(CRiAtmosphere &obj) {}
    inline virtual RtVoid doProcess(CRiInterior &obj) {}
	inline virtual RtVoid doProcess(CRiExterior &obj) {}
	inline virtual RtVoid doProcess(CRiIlluminate &obj) {}
    inline virtual RtVoid doProcess(CRiDisplacement &obj) {}
	inline virtual RtVoid doProcess(CRiTextureCoordinates &obj) {}
    inline virtual RtVoid doProcess(CRiShadingRate &obj) {}
	inline virtual RtVoid doProcess(CRiShadingInterpolation &obj) {}
    inline virtual RtVoid doProcess(CRiMatte &obj) {}
	inline virtual RtVoid doProcess(CRiBound &obj) {}
	inline virtual RtVoid doProcess(CRiDetail &obj) {}
	inline virtual RtVoid doProcess(CRiDetailRange &obj) {}
    inline virtual RtVoid doProcess(CRiGeometricApproximation &obj) {}
	inline virtual RtVoid doProcess(CRiGeometricRepresentation &obj) {}
	inline virtual RtVoid doProcess(CRiOrientation &obj) {}
	inline virtual RtVoid doProcess(CRiReverseOrientation &obj) {}
	inline virtual RtVoid doProcess(CRiSides &obj) {}
    inline virtual RtVoid doProcess(CRiBasis &obj) {}
    inline virtual RtVoid doProcess(CRiTrimCurve &obj) {}
	
	inline virtual RtVoid doProcess(CRiIdentity &obj) {}
	inline virtual RtVoid doProcess(CRiTransform &obj) {}
	inline virtual RtVoid doProcess(CRiConcatTransform &obj) {}
	inline virtual RtVoid doProcess(CRiPerspective &obj) {}
	inline virtual RtVoid doProcess(CRiTranslate &obj) {}
	inline virtual RtVoid doProcess(CRiRotate &obj) {}
	inline virtual RtVoid doProcess(CRiScale &obj) {}
    inline virtual RtVoid doProcess(CRiSkew &obj) {}
	inline virtual RtVoid doProcess(CRiDeformation &obj) {}
	inline virtual RtVoid doProcess(CRiScopedCoordinateSystem &obj) {}
	inline virtual RtVoid doProcess(CRiCoordinateSystem &obj) {}
	inline virtual RtVoid doProcess(CRiCoordSysTransform &obj) {}	
	virtual RtVoid doProcess(CRiTransformPoints &obj);

    inline virtual RtVoid doProcess(CRiPolygon &obj) {}
	inline virtual RtVoid doProcess(CRiGeneralPolygon &obj) {}
	inline virtual RtVoid doProcess(CRiPointsPolygons &obj) {}
    inline virtual RtVoid doProcess(CRiPointsGeneralPolygons &obj) {}
	
	inline virtual RtVoid doProcess(CRiPatch &obj) {}
	inline virtual RtVoid doProcess(CRiPatchMesh &obj) {}
    inline virtual RtVoid doProcess(CRiNuPatch &obj) {}
	
	inline virtual RtVoid doProcess(CRiSubdivisionMesh &obj) {}
	inline virtual RtVoid doProcess(CRiHierarchicalSubdivisionMesh &obj) {}
	
	inline virtual RtVoid doProcess(CRiSphere &obj) {}
    inline virtual RtVoid doProcess(CRiCone &obj) {}
    inline virtual RtVoid doProcess(CRiCylinder &obj) {}
    inline virtual RtVoid doProcess(CRiHyperboloid &obj) {}
    inline virtual RtVoid doProcess(CRiParaboloid &obj) {}
    inline virtual RtVoid doProcess(CRiDisk &obj) {}
    inline virtual RtVoid doProcess(CRiTorus &obj) {}
	
    inline virtual RtVoid doProcess(CRiPoints &obj) {}
    inline virtual RtVoid doProcess(CRiCurves &obj) {}
	
	inline virtual RtVoid doProcess(CRiBlobby &obj) {}
	
	virtual RtVoid doProcess(CRiProcedural &obj);

	inline virtual RtVoid doProcess(CRiGeometry &obj) {}

	inline virtual RtVoid doProcess(CRiMakeTexture &obj) {}
    inline virtual RtVoid doProcess(CRiMakeBump &obj) {}
    inline virtual RtVoid doProcess(CRiMakeLatLongEnvironment &obj) {}
    inline virtual RtVoid doProcess(CRiMakeCubeFaceEnvironment &obj) {}
    inline virtual RtVoid doProcess(CRiMakeShadow &obj) {}
    inline virtual RtVoid doProcess(CRiMakeBrickMap &obj) {}

	inline virtual RtVoid doProcess(CRiArchiveRecord &obj) {}
	virtual RtVoid doProcess(CRiReadArchive &obj);

	inline virtual RtVoid doProcess(CRiIfBegin &obj) {}
	inline virtual RtVoid doProcess(CRiElseIfBegin &obj) {}
	inline virtual RtVoid doProcess(CRiElseBegin &obj) {}
	inline virtual RtVoid doProcess(CRiIfEnd &obj) {}

public:
	inline virtual RtVoid postProcess(CRiSynchronize &obj) {}
	
	inline virtual RtVoid postProcess(CRiErrorHandler &obj) {}
	inline virtual RtVoid postProcess(CRiDeclare &obj) {}
    inline virtual RtVoid postProcess(CRiControl &obj) {}
	
	inline virtual RtVoid postProcess(CRiSystem &obj) {}
	
	inline virtual RtVoid postProcess(CRiBegin &obj) {}
	inline virtual RtVoid postProcess(CRiEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiFrameBegin &obj) {}
	inline virtual RtVoid postProcess(CRiFrameEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiWorldBegin &obj) {}
	inline virtual RtVoid postProcess(CRiWorldEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiAttributeBegin &obj) {}
	inline virtual RtVoid postProcess(CRiAttributeEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiTransformBegin &obj) {}
	inline virtual RtVoid postProcess(CRiTransformEnd &obj) {}
	
    inline virtual RtVoid postProcess(CRiSolidBegin &obj) {}
    inline virtual RtVoid postProcess(CRiSolidEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiObjectBegin &obj) {}
	inline virtual RtVoid postProcess(CRiObjectEnd &obj) {}
    inline virtual RtVoid postProcess(CRiObjectInstance &obj) {}
	
    inline virtual RtVoid postProcess(CRiMotionBegin &obj) {}
    inline virtual RtVoid postProcess(CRiMotionEnd &obj) {}
	
	inline virtual RtVoid postProcess(CRiResourceBegin &obj) {}
	inline virtual RtVoid postProcess(CRiResourceEnd &obj) {}
	inline virtual RtVoid postProcess(CRiResource &obj) {}
	
	inline virtual RtVoid postProcess(CRiArchiveBegin &obj) {}
	inline virtual RtVoid postProcess(CRiArchiveEnd &objArchiveEnd) {}
	
    inline virtual RtVoid postProcess(CRiFormat &obj) {}
    inline virtual RtVoid postProcess(CRiFrameAspectRatio &obj) {}
	
	inline virtual RtVoid postProcess(CRiScreenWindow &obj) {}
    inline virtual RtVoid postProcess(CRiCropWindow &obj) {}
	inline virtual RtVoid postProcess(CRiProjection &obj) {}
	inline virtual RtVoid postProcess(CRiClipping &obj) {}
    inline virtual RtVoid postProcess(CRiClippingPlane &obj) {}
    inline virtual RtVoid postProcess(CRiDepthOfField &obj) {}
    inline virtual RtVoid postProcess(CRiShutter &obj) {}
	inline virtual RtVoid postProcess(CRiPixelVariance &obj) {}
    inline virtual RtVoid postProcess(CRiPixelSamples &obj) {}
    inline virtual RtVoid postProcess(CRiPixelFilter &obj) {}
    inline virtual RtVoid postProcess(CRiExposure &obj) {}
    inline virtual RtVoid postProcess(CRiImager &obj) {}
	inline virtual RtVoid postProcess(CRiQuantize &obj) {}
    inline virtual RtVoid postProcess(CRiDisplayChannel &obj) {}
    inline virtual RtVoid postProcess(CRiDisplay &obj) {}
    inline virtual RtVoid postProcess(CRiHider &obj) {}
    inline virtual RtVoid postProcess(CRiColorSamples &obj) {}
    inline virtual RtVoid postProcess(CRiRelativeDetail &obj) {}
    inline virtual RtVoid postProcess(CRiOption &obj) {}
	
    inline virtual RtVoid postProcess(CRiLightSource &obj) {}
	inline virtual RtVoid postProcess(CRiAreaLightSource &obj) {}
	
    inline virtual RtVoid postProcess(CRiAttribute &obj) {}
	inline virtual RtVoid postProcess(CRiColor &obj) {}
	inline virtual RtVoid postProcess(CRiOpacity &obj) {}
    inline virtual RtVoid postProcess(CRiSurface &obj) {}
    inline virtual RtVoid postProcess(CRiAtmosphere &obj) {}
    inline virtual RtVoid postProcess(CRiInterior &obj) {}
	inline virtual RtVoid postProcess(CRiExterior &obj) {}
	inline virtual RtVoid postProcess(CRiIlluminate &obj) {}
    inline virtual RtVoid postProcess(CRiDisplacement &obj) {}
	inline virtual RtVoid postProcess(CRiTextureCoordinates &obj) {}
    inline virtual RtVoid postProcess(CRiShadingRate &obj) {}
	inline virtual RtVoid postProcess(CRiShadingInterpolation &obj) {}
    inline virtual RtVoid postProcess(CRiMatte &obj) {}
	inline virtual RtVoid postProcess(CRiBound &obj) {}
	inline virtual RtVoid postProcess(CRiDetail &obj) {}
	inline virtual RtVoid postProcess(CRiDetailRange &obj) {}
    inline virtual RtVoid postProcess(CRiGeometricApproximation &obj) {}
	inline virtual RtVoid postProcess(CRiGeometricRepresentation &obj) {}
	inline virtual RtVoid postProcess(CRiOrientation &obj) {}
	inline virtual RtVoid postProcess(CRiReverseOrientation &obj) {}
	inline virtual RtVoid postProcess(CRiSides &obj) {}
    inline virtual RtVoid postProcess(CRiBasis &obj) {}
    inline virtual RtVoid postProcess(CRiTrimCurve &obj) {}
	
	inline virtual RtVoid postProcess(CRiIdentity &obj) {}
	inline virtual RtVoid postProcess(CRiTransform &obj) {}
	inline virtual RtVoid postProcess(CRiConcatTransform &obj) {}
	inline virtual RtVoid postProcess(CRiPerspective &obj) {}
	inline virtual RtVoid postProcess(CRiTranslate &obj) {}
	inline virtual RtVoid postProcess(CRiRotate &obj) {}
	inline virtual RtVoid postProcess(CRiScale &obj) {}
    inline virtual RtVoid postProcess(CRiSkew &obj) {}
	inline virtual RtVoid postProcess(CRiDeformation &obj) {}
	inline virtual RtVoid postProcess(CRiScopedCoordinateSystem &obj) {}
	inline virtual RtVoid postProcess(CRiCoordinateSystem &obj) {}
	inline virtual RtVoid postProcess(CRiCoordSysTransform &obj) {}
	inline virtual RtVoid postProcess(CRiTransformPoints &obj) {}
	
    inline virtual RtVoid postProcess(CRiPolygon &obj) {}
	inline virtual RtVoid postProcess(CRiGeneralPolygon &obj) {}
	inline virtual RtVoid postProcess(CRiPointsPolygons &obj) {}
    inline virtual RtVoid postProcess(CRiPointsGeneralPolygons &obj) {}
	
	inline virtual RtVoid postProcess(CRiPatch &obj) {}
	inline virtual RtVoid postProcess(CRiPatchMesh &obj) {}
    inline virtual RtVoid postProcess(CRiNuPatch &obj) {}
	
	inline virtual RtVoid postProcess(CRiSubdivisionMesh &obj) {}
	inline virtual RtVoid postProcess(CRiHierarchicalSubdivisionMesh &obj) {}
	
	inline virtual RtVoid postProcess(CRiSphere &obj) {}
    inline virtual RtVoid postProcess(CRiCone &obj) {}
    inline virtual RtVoid postProcess(CRiCylinder &obj) {}
    inline virtual RtVoid postProcess(CRiHyperboloid &obj) {}
    inline virtual RtVoid postProcess(CRiParaboloid &obj) {}
    inline virtual RtVoid postProcess(CRiDisk &obj) {}
    inline virtual RtVoid postProcess(CRiTorus &obj) {}
	
    inline virtual RtVoid postProcess(CRiPoints &obj) {}
    inline virtual RtVoid postProcess(CRiCurves &obj) {}
	
	inline virtual RtVoid postProcess(CRiBlobby &obj) {}
	
	inline virtual RtVoid postProcess(CRiProcedural &obj) {}
	
	inline virtual RtVoid postProcess(CRiGeometry &obj) {}
	
	inline virtual RtVoid postProcess(CRiMakeTexture &obj) {}
    inline virtual RtVoid postProcess(CRiMakeBump &obj) {}
    inline virtual RtVoid postProcess(CRiMakeLatLongEnvironment &obj) {}
    inline virtual RtVoid postProcess(CRiMakeCubeFaceEnvironment &obj) {}
    inline virtual RtVoid postProcess(CRiMakeShadow &obj) {}
    inline virtual RtVoid postProcess(CRiMakeBrickMap &obj) {}
	
	inline virtual RtVoid postProcess(CRiArchiveRecord &obj) {}
	inline virtual RtVoid postProcess(CRiReadArchive &obj) {}
	
	inline virtual RtVoid postProcess(CRiIfBegin &obj) {}
	inline virtual RtVoid postProcess(CRiElseIfBegin &obj) {}
	inline virtual RtVoid postProcess(CRiElseBegin &obj) {}
	inline virtual RtVoid postProcess(CRiIfEnd &obj) {}
}; // CBaseRenderer

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_BASERENDERER_H
