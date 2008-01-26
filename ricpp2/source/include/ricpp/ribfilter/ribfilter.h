#ifndef _RICPP_RIBFILTER_RIBFILTER_H
#define _RICPP_RIBFILTER_RIBFILTER_H

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

/** @file ribfilter.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Root class for a RIB filter plugin.
 */

#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#include "ricpp/pluginhandler/pluginhandler.h"
#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

#ifndef _RICPP_RICPP_REQUESTS_H
#include "ricpp/ricpp/requests.h"
#endif // _RICPP_RICPP_REQUESTS_H

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** @brief Root class for RIB filters and stub for CRiCPPBridge
 */
class CRibFilter : public IRiRoot, public IPlugin {
	friend class CRiCPPBridge;
	friend class CRibFilterList;

	IRiRoot *m_next; ///< Next filter to call, can also point to the frontend.
	bool m_suspended; ///< Rib requests are suspended until resume() is called
	bool m_enabled[N_REQUESTS]; ///< Used to disable/enable selected requests

	/** @brief beginV() is not a RIB request.
	 */
	inline RtContextHandle beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { return illContextHandle; }

	/** @brief end() is not a RIB request.
	 */
	inline RtVoid end(void) {}

protected:
	/** @brief Tests if a requst can be called.
	 *  @param req Index number of a request (REQ_... constants)
	 */
	inline bool canCall(EnumRequests req) { return callee() != 0 && passthrough() && enabled(req); }

public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline CRibFilter::CRibFilter()
		: m_next(0), m_suspended(false)
	{
		int i;
		for ( i = 0; i < N_REQUESTS; ++i )
			m_enabled[i] = true;
	}
	inline virtual ~CRibFilter() {}

	inline virtual const char *name() const  { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup()
	{
		// Can be overwritten.
	}

	inline virtual void shutdown()
	{
		// Can be overwritten.
	}

	/** @brief No transfering until resume().
	 */
	inline virtual bool suspended() const { return m_suspended; }

	/** @brief Transfering until suspend().
	 */
	inline virtual bool passthrough() const { return !m_suspended; }

	/** @brief Stop the transfering of RIB calls until resume().
	 */
	inline virtual void suspend() { m_suspended = true; }

	/** @brief Continue the transfering of RIB calls.
	 */
	inline virtual void resume() { m_suspended = false; }

	/** @brief Tests if a certain command is enabled.
	 *  @param req Number of a request (REQ_...-constant)
	 *  @return true, command is enabled
	 */
	inline virtual bool enabled(EnumRequests req) const { return m_enabled[req]; }

	/** @brief Tests if a certain command is disabled.
	 *  @param req Number of a request (REQ_...-constant)
	 *  @return true, command is disabled
	 */
	inline virtual bool disabled(EnumRequests req) const { return !m_enabled[req]; }

	/** @brief Enables a certain command.
	 *  @param req Number of the request (REQ_...-constant) to enable.
	 */
	inline virtual void enable(EnumRequests req) { m_enabled[req] = true; }

	/** @brief Disables a certain command.
	 *  @param req Number of the request (REQ_...-constant) to disable.
	 */
	inline virtual void disable(EnumRequests req) { m_enabled[req] = false; }

	/** @brief Interface that is called.
	 *  @return Pointer to the RenderMan interrface that is called by this filter.
	 */
	inline virtual IRiRoot *callee() const { return m_next; }

	/** @brief Sets argument.
	 *
	 *  Overload this functions to set own parameters.
	 *
	 *  @param name Name of the argument.
	 *  @param value Pointer to the parameter.
	 *  @return Pointer to the RenderMan interrface that is called by this filter.
	 */
    inline virtual RtVoid setArg(RtString name, RtPointer value)
	{
		// Can be overwritten
	}

	/** @brief Sets arguments.
	 *
	 *  Overload this functions to set own parameters.
	 *
	 *  @param n Number of name value pairs
	 *  @param name Name of the argument.
	 *  @param value Pointer to the parameter.
	 *  @return Pointer to the RenderMan interrface that is called by this filter.
	 */
    inline virtual RtVoid setArgs(RtInt n, RtString name[], RtPointer value[])
	{
		if ( n <= 0 )
			return;
		for ( RtInt i = 0; i < n; ++i ) {
			setArg(name[i], value[i]);
		}
	}

	/* ********************************************************************* */
	/** @defgroup ricpp_ricalls Filtered ri calls
	 *  @{
	 */
	inline RtToken declare(RtString name, RtString declaration)
	{
		return canCall(REQ_DECLARE) ?
			callee()->declare(name, declaration) :
			RI_NULL;
	}

 	inline virtual RtVoid errorHandler(const IErrorHandler &handler)
	{
		if ( canCall(REQ_ERROR_HANDLER) )
			callee()->errorHandler(handler);
	}

	inline virtual RtVoid synchronize(RtToken name)
	{
		if ( canCall(REQ_SYNCHRONIZE) )
			callee()->synchronize(name);
	}

	inline virtual RtVoid system(RtString cmd)
	{
		if ( canCall(REQ_SYSTEM) )
			callee()->system(cmd);
	}

    inline virtual RtVoid controlV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		// Control is not a part of the RIB binding.
	}

	inline virtual RtVoid version()
	{
		if ( canCall(REQ_VERSION) )
			callee()->version();
	}

	inline virtual RtVoid resourceV(RtString handle, RtString type, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_RESOURCE) )
			callee()->resourceV(handle, type, n, tokens, params);
	}

	inline virtual RtVoid frameBegin(RtInt number)
	{
		if ( canCall(REQ_FRAME_BEGIN) )
			callee()->frameBegin(number);
	}

	inline virtual RtVoid frameEnd(void)
	{
		if ( canCall(REQ_FRAME_END) )
			callee()->frameEnd();
	}

	inline virtual RtVoid worldBegin(void)
	{
		if ( canCall(REQ_WORLD_BEGIN) )
			callee()->worldBegin();
	}

	inline virtual RtVoid worldEnd(void)
	{
		if ( canCall(REQ_WORLD_END) )
			callee()->worldEnd();
	}

	inline virtual RtVoid attributeBegin(void)
	{
		if ( canCall(REQ_ATTRIBUTE_BEGIN) )
			callee()->attributeBegin();
	}
	
	inline virtual RtVoid attributeEnd(void)
	{
		if ( canCall(REQ_ATTRIBUTE_END) )
			callee()->attributeEnd();
	}

	inline virtual RtVoid transformBegin(void)
	{
		if ( canCall(REQ_TRANSFORM_BEGIN) )
			callee()->transformBegin();
	}

	inline virtual RtVoid transformEnd(void)
	{
		if ( canCall(REQ_TRANSFORM_END) )
			callee()->transformEnd();
	}

	inline virtual RtVoid solidBegin(RtToken type)
	{
		if ( canCall(REQ_SOLID_BEGIN) )
			callee()->solidBegin(type);
	}

	inline virtual RtVoid solidEnd(void)
	{
		if ( canCall(REQ_SOLID_END) )
			callee()->solidEnd();
	}

	inline virtual RtObjectHandle objectBegin(void)
	{
		return canCall(REQ_OBJECT_BEGIN) ?
			callee()->objectBegin() :
			illObjectHandle;
	}

	inline virtual RtVoid objectEnd(void)
	{
		if ( canCall(REQ_OBJECT_END) )
			callee()->objectEnd();
	}

    inline virtual RtVoid objectInstance(RtObjectHandle handle)
	{
		if ( canCall(REQ_OBJECT_INSTANCE) )
			callee()->objectInstance(handle);
	}
    
    inline virtual RtVoid motionBeginV(RtInt N, RtFloat times[])
	{
		if ( canCall(REQ_MOTION_BEGIN) )
			callee()->motionBeginV(N, times);
	}

	inline virtual RtVoid motionEnd(void)
	{
		if ( canCall(REQ_MOTION_END) )
			callee()->motionEnd();
	}

	inline virtual RtVoid resourceBegin(void)
	{
		if ( canCall(REQ_RESOURCE_BEGIN) )
			callee()->resourceBegin();
	}

	inline virtual RtVoid resourceEnd(void)
	{
		if ( canCall(REQ_RESOURCE_END) )
			callee()->resourceEnd();
	}

	inline virtual RtArchiveHandle archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		return canCall(REQ_ARCHIVE_BEGIN) ?
			callee()->archiveBeginV(name, n, tokens, params) :
			illArchiveHandle;
	}

	inline virtual RtVoid archiveEnd(void)
	{
		if ( canCall(REQ_ARCHIVE_END) )
			callee()->archiveEnd();
	}

	inline virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect)
	{
		if ( canCall(REQ_FORMAT) )
			callee()->format(xres, yres, aspect);
	}

	inline virtual RtVoid frameAspectRatio(RtFloat aspect)
	{
		if ( canCall(REQ_FRAME_ASPECT_RATIO) )
			callee()->frameAspectRatio(aspect);
	}

	inline virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
	{
		if ( canCall(REQ_SCREEN_WINDOW) )
			callee()->screenWindow(left, right, bot, top);
	}

    inline virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
	{
		if ( canCall(REQ_CROP_WINDOW) )
			callee()->cropWindow(xmin, xmax, ymin, ymax);
	}

    inline virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_PROJECTION) )
			callee()->projectionV(name, n, tokens, params);
	}

	inline virtual RtVoid clipping(RtFloat hither, RtFloat yon)
	{
		if ( canCall(REQ_CLIPPING) )
			callee()->clipping(hither, yon);
	}

	inline virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
	{
		if ( canCall(REQ_CLIPPING_PLANE) )
			callee()->clippingPlane(x, y, z, nx, ny, nz);
	}

	inline virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
	{
		if ( canCall(REQ_DEPTH_OF_FIELD) )
			callee()->depthOfField(fstop, focallength, focaldistance);
	}

	inline virtual RtVoid shutter(RtFloat smin, RtFloat smax)
	{
		if ( canCall(REQ_SHUTTER) )
			callee()->shutter(smin, smax);
	}

	inline virtual RtVoid pixelVariance(RtFloat variation)
	{
		if ( canCall(REQ_PIXEL_VARIANCE) )
			callee()->pixelVariance(variation);
	}

    inline virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples)
	{
		if ( canCall(REQ_PIXEL_SAMPLES) )
			callee()->pixelSamples(xsamples, ysamples);
	}

	inline virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
	{
		if ( canCall(REQ_PIXEL_FILTER) )
			callee()->pixelFilter(function, xwidth, ywidth);
	}

    inline virtual RtVoid exposure(RtFloat gain, RtFloat gamma)
	{
		if ( canCall(REQ_EXPOSURE) )
			callee()->exposure(gain, gamma);
	}

    inline virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_IMAGER) )
			callee()->imagerV(name, n, tokens, params);
	}

	inline virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
	{
		if ( canCall(REQ_QUANTIZE) )
			callee()->quantize(type, one, qmin, qmax, ampl);
	}

	inline virtual RtVoid displayChannelV(RtString channel, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_DISPLAY_CHANNEL) )
			callee()->displayChannelV(channel, n, tokens, params);
	}

	inline virtual RtVoid displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_DISPLAY) )
			callee()->displayV(name, type, mode, n, tokens, params);
	}

    inline virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_HIDER) )
			callee()->hiderV(type, n, tokens, params);
	}

    inline virtual RtVoid colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
	{
		if ( canCall(REQ_COLOR_SAMPLES) )
			callee()->colorSamples(N, nRGB, RGBn);
	}

    inline virtual RtVoid relativeDetail(RtFloat relativedetail)
	{
		if ( canCall(REQ_RELATIVE_DETAIL) )
			callee()->relativeDetail(relativedetail);
	}

    inline virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_OPTION) )
			callee()->optionV(name, n, tokens, params);
	}
	
    inline virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		return canCall(REQ_LIGHT_SOURCE) ?
			callee()->lightSourceV(name, n, tokens, params) :
			illLightHandle;
	}

	inline virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		return canCall(REQ_AREA_LIGHT_SOURCE) ?
			callee()->areaLightSourceV(name, n, tokens, params) :
			illLightHandle;
	}
	
    inline virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_ATTRIBUTE) )
			callee()->attributeV(name, n, tokens, params);
	}

	inline virtual RtVoid color(RtColor Cs)
	{
		if ( canCall(REQ_COLOR) )
			callee()->color(Cs);
	}

	inline virtual RtVoid opacity(RtColor Os)
	{
		if ( canCall(REQ_OPACITY) )
			callee()->opacity(Os);
	}

	inline virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_SURFACE) )
			callee()->surfaceV(name, n, tokens, params);
	}

	inline virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_ATMOSPHERE) )
			callee()->atmosphereV(name, n, tokens, params);
	}

    inline virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_INTERIOR) )
			callee()->interiorV(name, n, tokens, params);
	}

	inline virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_EXTERIOR) )
			callee()->exteriorV(name, n, tokens, params);
	}

	inline virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff)
	{
		if ( canCall(REQ_ILLUMINATE) )
			callee()->illuminate(light, onoff);
	}

    inline virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_DISPLACEMENT) )
			callee()->displacementV(name, n, tokens, params);
	}

	inline virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
	{
		if ( canCall(REQ_TEXTURE_COORDINATES) )
			callee()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	}

    inline virtual RtVoid shadingRate(RtFloat size)
	{
		if ( canCall(REQ_SHADING_RATE) )
			callee()->shadingRate(size);
	}

	inline virtual RtVoid shadingInterpolation(RtToken type)
	{
		if ( canCall(REQ_SHADING_INTERPOLATION) )
			callee()->shadingInterpolation(type);
	}

    inline virtual RtVoid matte(RtBoolean onoff)
	{
		if ( canCall(REQ_MATTE) )
			callee()->matte(onoff);
	}

	inline virtual RtVoid bound(RtBound bound)
	{
		if ( canCall(REQ_BOUND) )
			callee()->bound(bound);
	}

	inline virtual RtVoid detail(RtBound bound)
	{
		if ( canCall(REQ_DETAIL) )
			callee()->detail(bound);
	}

	inline virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
	{
		if ( canCall(REQ_DETAIL_RANGE) )
			callee()->detailRange(minvis, lowtran, uptran, maxvis);
	}

    inline virtual RtVoid geometricApproximation(RtToken type, RtFloat value)
	{
		if ( canCall(REQ_GEOMETRIC_APPROXIMATION) )
			callee()->geometricApproximation(type, value);
	}

	inline virtual RtVoid geometricRepresentation(RtToken type)
	{
		if ( canCall(REQ_GEOMETRIC_REPRESENTATION) )
			callee()->geometricRepresentation(type);
	}

	inline virtual RtVoid orientation(RtToken anOrientation)
	{
		if ( canCall(REQ_ORIENTATION) )
			callee()->orientation(anOrientation);
	}

	inline virtual RtVoid reverseOrientation(void)
	{
		if ( canCall(REQ_REVERSE_ORIENTATION) )
			callee()->reverseOrientation();
	}

	inline virtual RtVoid sides(RtInt nsides)
	{
		if ( canCall(REQ_SIDES) )
			callee()->sides(nsides);
	}

    inline virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
	{
		if ( canCall(REQ_BASIS) )
			callee()->basis(ubasis, ustep, vbasis, vstep);
	}

    inline virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
	{
		if ( canCall(REQ_TRIM_CURVE) )
			callee()->trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	inline virtual RtVoid identity(void)
	{
		if ( canCall(REQ_IDENTITY) )
			callee()->identity();
	}

	inline virtual RtVoid transform(RtMatrix aTransform)
	{
		if ( canCall(REQ_TRANSFORM) )
			callee()->transform(aTransform);
	}

	inline virtual RtVoid concatTransform(RtMatrix aTransform)
	{
		if ( canCall(REQ_CONCAT_TRANSFORM) )
			callee()->concatTransform(aTransform);
	}

	inline virtual RtVoid perspective(RtFloat fov)
	{
		if ( canCall(REQ_PERSPECTIVE) )
			callee()->perspective(fov);
	}

	inline virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz)
	{
		if ( canCall(REQ_TRANSLATE) )
			callee()->translate(dx, dy, dz);
	}

	inline virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
	{
		if ( canCall(REQ_ROTATE) )
			callee()->rotate(angle, dx, dy, dz);
	}

	inline virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz)
	{
		if ( canCall(REQ_SCALE) )
			callee()->scale(dx, dy, dz);
	}

    inline virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
	{
		if ( canCall(REQ_SKEW) )
			callee()->skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	}

	inline virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_DEFORMATION) )
			callee()->deformationV(name, n, tokens, params);
	}

	inline virtual RtVoid scopedCoordinateSystem(RtToken space)
	{
		if ( canCall(REQ_SCOPED_COORDINATE_SYSTEM) )
			callee()->scopedCoordinateSystem(space);
	}

	inline virtual RtVoid coordinateSystem(RtToken space)
	{
		if ( canCall(REQ_COORDINATE_SYSTEM) )
			callee()->coordinateSystem(space);
	}

	inline virtual RtVoid coordSysTransform(RtToken space)
	{
		if ( canCall(REQ_COORD_SYS_TRANSFORM) )
			callee()->coordSysTransform(space);
	}

	inline virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
	{
		return canCall(REQ_TRANSFORM_POINTS) ?
			callee()->transformPoints(fromspace, tospace, npoints, points) :
			0;
	}

    inline virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_POLYGON) )
			callee()->polygonV(nvertices, n, tokens, params);
	}

	inline virtual RtVoid generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_GENERAL_POLYGON) )
			callee()->generalPolygonV(nloops, nverts, n, tokens, params);
	}

	inline virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_POINTS_POLYGONS) )
			callee()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	}

    inline virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_POINTS_GENERAL_POLYGONS) )
			callee()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	}

	inline virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_PATCH) )
			callee()->patchV(type, n, tokens, params);
	}

	inline virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_PATCH_MESH) )
			callee()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	}

    inline virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_NU_PATCH) )
			callee()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	}

	inline virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_SUBDIVISION_MESH) )
			callee()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	}

	inline virtual RtVoid hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_HIERARCHICAL_SUBDIVISION_MESH) )
			callee()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
	}

	inline virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_SPHERE) )
			callee()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	}

    inline virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_CONE) )
			callee()->coneV(height, radius, thetamax, n, tokens, params);
	}

	inline virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_CYLINDER) )
			callee()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	}

    inline virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_HYPERBOLOID) )
			callee()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
	}

    inline virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_PARABOLOID) )
			callee()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	}

    inline virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_DISK) )
			callee()->diskV(height, radius, thetamax, n, tokens, params);
	}

	inline virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_TORUS) )
			callee()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	}

    inline virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_POINTS) )
			callee()->pointsV(npts, n, tokens, params);
	}

    inline virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_CURVES) )
			callee()->curvesV(type, ncurves, nverts, wrap, n, tokens, params);
	}

	inline virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_BLOBBY) )
			callee()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	}

	inline virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
	{
		// Need finer grane - delayed rib reading may be allowed, but not to run programs

		if ( subdivfunc.name() == RI_DELAYED_READ_ARCHIVE ) {
			if ( canCall(REQ_PROC_DELAYED_READ_ARCHIVE) )
				callee()->procedural(data, bound, subdivfunc, freefunc);
			return;
		}

		if ( subdivfunc.name() == RI_RUN_PROGRAM ) {
			if ( canCall(REQ_PROC_RUN_PROGRAM) )
				callee()->procedural(data, bound, subdivfunc, freefunc);
			return;
		}

		if ( subdivfunc.name() == RI_DYNAMIC_LOAD ) {
			if ( canCall(REQ_PROC_DYNAMIC_LOAD) )
				callee()->procedural(data, bound, subdivfunc, freefunc);
			return;
		}

		// All the rest
		if ( canCall(REQ_PROCEDURAL) )
			callee()->procedural(data, bound, subdivfunc, freefunc);
	}

	inline virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_GEOMETRY) )
			callee()->geometryV(type, n, tokens, params);
	}

	inline virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_TEXTURE) )
			callee()->makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_BUMP) )
			callee()->makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_LAT_LONG_ENVIRONMENT) )
			callee()->makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	}

    inline virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_CUBE_FACE_ENVIRONMENT) )
			callee()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	}

    inline virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_SHADOW) )
			callee()->makeShadowV(pic, tex, n, tokens, params);
	}

    inline virtual RtVoid makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_MAKE_SHADOW) )
			callee()->makeBrickMapV(nNames, ptcnames, bkmname, n, tokens, params);
	}

	inline virtual RtVoid archiveRecordV(RtToken type, RtString line)
	{
		if ( canCall(REQ_ARCHIVE_RECORD) )
			callee()->archiveRecordV(type, line);
	}

	inline virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
	{
		if ( canCall(REQ_READ_ARCHIVE) )
			callee()->readArchiveV(name, callback, n, tokens, params);
	}

	inline virtual RtVoid ifBegin(RtString expr)
	{
		if ( canCall(REQ_IF_BEGIN) )
			callee()->ifBegin(expr);
	}

	inline virtual RtVoid elseIfBegin(RtString expr)
	{
		if ( canCall(REQ_ELSE_IF) )
			callee()->elseIfBegin(expr);
	}

	inline virtual RtVoid elseBegin(void)
	{
		if ( canCall(REQ_ELSE) )
			callee()->elseBegin();
	}

	inline virtual RtVoid ifEnd(void)
	{
		if ( canCall(REQ_IF_END) )
			callee()->ifEnd();
	}

	/// @}
}; // CRibFilter


} // namespace RiCPP

#endif // _RICPP_RIBFILTER_RIBFILTER_H
