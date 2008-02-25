#ifndef _RICPP_RI_RI_H
#define _RICPP_RI_RI_H

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

/** @file ri.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Interface for the RenderMan(R) C Binding
 *
 @verbatim
       RenderMan(R) is a registered trademark of Pixar
   The RenderMan(R) Interface Procedures and Protocol are:
           Copyright 1988, 1989, 2000, 2005 Pixar
                   All rights Reservered
 @endverbatim
 *
 * Original dekalarations are part of [RiSPec].
 * Portions of this file are copied from the renderer Pixie.
 *
 * @todo copy the routines that can be used with no context to riprog, ridynload
 */

#ifndef _RICPP_RICPP_RICPPCONST_H
#include "ricpp/ricpp/ricppconst.h"
#endif _RICPP_RICPP_RICPPCONST_H
 
#ifndef _RICPP_RICPP_RICPPTOKENS_H
#include "ricpp/ricpp/ricpptokens.h"
#endif _RICPP_RICPP_RICPPTOKENS_H

#ifdef __cplusplus
namespace RiCPP {
#endif

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//
//	Missing type definitions
//
////////////////////////////////////////////////////////////////////////
typedef RtVoid		(*RtErrorHandler)(RtInt code, RtInt severity, char *msg);
typedef RtVoid      (*RtProcSubdivFunc)(RtPointer, RtFloat);
typedef RtVoid      (*RtProcFreeFunc)(RtPointer);
typedef RtVoid		(*RtArchiveCallback)(const char *,...);


////////////////////////////////////////////////////////////////////////
//
//	Delarations of missing symbols of the RenderMan Interface Subroutines
//
////////////////////////////////////////////////////////////////////////
RICPP_EXTERN(RtInt)		RiLastError;

////////////////////////////////////////////////////////////////////////
//
//	Additional functions
//
////////////////////////////////////////////////////////////////////////
RICPP_EXTERN(RtContextHandle)
    RiCPPBegin (RtToken name, ...),
	RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
	RiCPPEnd (void);

RICPP_EXTERN(RtObjectHandle) 
	RiCPPObjectBegin (RtToken name);

RICPP_EXTERN(RtVoid)
	RiCPPObjectEnd (void);

RICPP_EXTERN(RtVoid)
    RiCPPControl (RtToken name, ...),
	RiCPPControlV (RtToken name, int n, RtToken tokens[], RtPointer params[]);

////////////////////////////////////////////////////////////////////////
//
//	Delarations of All of the RenderMan Interface Subroutines
//
////////////////////////////////////////////////////////////////////////

RICPP_EXTERN(RtToken)
    RiDeclare (const char *name,const char *declaration);

RICPP_EXTERN(RtContextHandle)
	RiGetContext(void);

RICPP_EXTERN(RtVoid)
	RiContext(RtContextHandle),
	RiSynchronize(RtToken),
	RiSystem(RtString);

RICPP_EXTERN(RtVoid)
    RiBegin (RtToken name),
	RiEnd (void),
	RiFrameBegin (RtInt number), RiFrameEnd (void),
    RiWorldBegin (void), RiWorldEnd (void);

RICPP_EXTERN(RtVoid)
    RiFormat (RtInt xres, RtInt yres, RtFloat aspect),
    RiFrameAspectRatio (RtFloat aspect),
    RiScreenWindow (RtFloat left, RtFloat right, RtFloat bot, RtFloat top),
    RiCropWindow (RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax),
    RiProjection (char *name, ...),
    RiProjectionV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiClipping (RtFloat hither, RtFloat yon),
	RiClippingPlane(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz),
    RiDepthOfField (RtFloat fstop, RtFloat focallength, RtFloat focaldistance),
    RiShutter (RtFloat smin, RtFloat smax);

RICPP_EXTERN(RtVoid)
    RiPixelVariance (RtFloat variation),
    RiPixelSamples (RtFloat xsamples, RtFloat ysamples),
    RiPixelFilter (RtFilterFunc function, RtFloat xwidth, RtFloat ywidth),
    RiExposure (RtFloat gain, RtFloat gamma),
    RiImager (char *name, ...),
    RiImagerV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl),
    RiDisplay (char *name, RtToken type, RtToken mode, ...),
    RiDisplayV (char *name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]),
	RiDisplayChannel (RtToken channel, ...),
    RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[]);

// Implemented in filters.cpp
RICPP_EXTERN(RtFloat)
    RiGaussianFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiBoxFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiTriangleFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiCatmullRomFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiSincFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

RICPP_EXTERN(RtVoid)
    RiHider (RtToken type, ...),
    RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[]),
    RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn),
    RiRelativeDetail (RtFloat relativedetail),
    RiOption (char *name, ...),
    RiOptionV (char *name, RtInt n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
    RiAttributeBegin (void), RiAttributeEnd (void),
    RiColor (RtColor Cs), RiOpacity (RtColor Cs),
    RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

RICPP_EXTERN(RtLightHandle)
    RiLightSource (char *name, ...),
    RiLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiAreaLightSource (char *name, ...),
    RiAreaLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
    RiIlluminate (RtLightHandle light, RtBoolean onoff),
    RiSurface (char *name, ...),
    RiSurfaceV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiAtmosphere (char *name, ...),
    RiAtmosphereV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiInterior (char *name, ...),
    RiInteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiExterior (char *name, ...),
    RiExteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiShadingRate (RtFloat size),
    RiShadingInterpolation (RtToken type),
    RiMatte (RtBoolean onoff);

RICPP_EXTERN(RtVoid)
    RiBound (RtBound bound), RiDetail (RtBound bound),
    RiDetailRange (RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis),
    RiGeometricApproximation (RtToken type, RtFloat value),
    RiGeometricRepresentation (RtToken type),
    RiOrientation (RtToken orientation), RiReverseOrientation (void),
    RiSides (RtInt nsides);

/*
RICPP_EXTERN(RtVoid)
    RiIdentity (void),
    RiTransform (RtMatrix transform), RiConcatTransform (RtMatrix transform),
    RiPerspective (RtFloat fov),
    RiTranslate (RtFloat dx, RtFloat dy, RtFloat dz),
    RiRotate (RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz),
    RiScale (RtFloat dx, RtFloat dy, RtFloat dz),
    RiSkew (RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2),
    RiDeformation (char *name, ...),
    RiDeformationV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiDisplacement (char *name, ...),
    RiDisplacementV (char *name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiCoordinateSystem (RtToken space),
    RiCoordSysTransform (RtToken space);

RICPP_EXTERN(RtPoint *) 
	RiTransformPoints (RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points);

RICPP_EXTERN(RtVoid) 
	RiTransformBegin (void), 
	RiTransformEnd (void);

RICPP_EXTERN(RtVoid)
    RiAttribute (char *name, ...),
    RiAttributeV (char *name, RtInt n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
    RiPolygon (RtInt nvertices, ...),
    RiPolygonV (RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]),
    RiGeneralPolygon (RtInt nloops, RtInt *nverts, ...),
    RiGeneralPolygonV (RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]),
    RiPointsPolygons (RtInt npolys, RtInt *nverts, RtInt *verts, ...),
    RiPointsPolygonsV (RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]),
    RiPointsGeneralPolygons (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, ...),
    RiPointsGeneralPolygonsV (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]),
    RiBasis (RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep),
    RiPatch (RtToken type, ...),
    RiPatchV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[]),
    RiPatchMesh (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, ...),
    RiPatchMeshV (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]),
    RiNuPatch (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, ...),
    RiNuPatchV (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiTrimCurve (RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);

RICPP_EXTERN(RtVoid)
    RiSphere (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...),
    RiSphereV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiCone (RtFloat height, RtFloat radius, RtFloat thetamax, ...),
    RiConeV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiCylinder (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...),
    RiCylinderV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiHyperboloid (RtPoint point1, RtPoint point2, RtFloat thetamax, ...),
    RiHyperboloidV (RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiParaboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...),
    RiParaboloidV (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiDisk (RtFloat height, RtFloat radius, RtFloat thetamax, ...),
    RiDiskV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiTorus (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...),
    RiTorusV (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]),
    RiCurves (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, ...),
    RiCurvesV (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]),
	RiProcedural(RtPointer data, RtBound bound, RtVoid (*subdivfunc)(RtPointer, RtFloat), RtVoid (*freefunc)(RtPointer)),
    RiGeometry (RtToken type, ...),
    RiGeometryV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
    RiCurves (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, ...),
    RiCurvesV (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]),
    RiPoints (RtInt npts,...),
    RiPointsV (RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]),
    RiSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], ...),
    RiSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtInt n, RtToken tokens[], RtPointer params[]),
    RiHierarchicalSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], ...),
    RiHierarchicalSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], RtInt n, RtToken tokens[], RtPointer params[]),
    RiBlobby (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], ...),
    RiBlobbyV (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid) 
	RiProcDelayedReadArchive (RtPointer data, RtFloat detail),
    RiProcRunProgram (RtPointer data, RtFloat detail),
    RiProcDynamicLoad (RtPointer data, RtFloat detail),
	RiProcFree(RtPointer);

RICPP_EXTERN(RtVoid)
	RiSolidBegin(RtToken type), 
	RiSolidEnd(void);

RICPP_EXTERN(RtObjectHandle) 
	RiObjectBegin (void);

RICPP_EXTERN(RtVoid)
    RiObjectEnd (void),
    RiObjectInstance (RtObjectHandle handle),
    RiMotionBegin (RtInt N, ...),
    RiMotionBeginV (RtInt N, RtFloat times[]),
    RiMotionEnd (void);

RICPP_EXTERN(RtVoid)
    RiMakeTexture (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeTextureV (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeBump (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeBumpV (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeLatLongEnvironment (char *pic, char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeLatLongEnvironmentV (char *pic, char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeCubeFaceEnvironment (char *px, char *nx, char *py, char *ny, char *pz, char *nz, char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeCubeFaceEnvironmentV (char *px, char *nx, char *py, char *ny, char *pz, char *nz, char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeShadow (char *pic, char *tex, ...),
    RiMakeShadowV (char *pic, char *tex, RtInt n, RtToken tokens[], RtPointer params[]),
	RiMakeBrickMap (int n,char **src, char *dest, ...),
	RiMakeBrickMapV(int n,char **src, char *dest,RtInt numTokens, RtToken tokens[], RtPointer params[]);

RICPP_EXTERN(RtVoid)
    RiErrorHandler (RtErrorHandler handler),
    RiErrorIgnore (RtInt code, RtInt severity, char *message),
    RiErrorPrint (RtInt code, RtInt severity, char *message),
    RiErrorAbort (RtInt code, RtInt severity, char *message);

RICPP_EXTERN(RtVoid)
	RiResource(RtToken handle, RtToken type, ...),
	RiResourceV(RtToken handle, RtToken type,RtInt n, RtToken tokens[], RtPointer parms[]),
	RiResourceBegin(void),
	RiResourceEnd(void);

RICPP_EXTERN(RtArchiveHandle)
	RiArchiveBegin(RtToken name, ...),
	RiArchiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);

RICPP_EXTERN(RtVoid)
	RiArchiveEnd(void);

RICPP_EXTERN(RtVoid)
	RiIfBegin(char *expr, ...),
	RiIfBeginV(char *expr, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiElseIf(char *expr, ...),
	RiElseIfV(char *expr, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiElse(void),
	RiIfEnd(void);

RICPP_EXTERN(RtVoid)
    RiArchiveRecord (RtToken type, char *format, ...),
    RiReadArchive (RtString filename, RtArchiveCallback callback, ...),
    RiReadArchiveV (RtString filename, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[]);
*/

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace RiCPP
#endif


#endif // _RICPP_RICPP_RICPPTYPES_H
