#ifndef _RICPP_RI_RI_H
#define _RICPP_RI_RI_H
/* RICPP - RenderMan(R) Interface CPP Language Binding
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
*/
/** @file ri.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Interface for the RenderMan(R) C Binding
 *
 *  Adapter from C-Binding to CPP-Binding
 *
 @verbatim
       RenderMan(R) is a registered trademark of Pixar
   The RenderMan(R) Interface Procedures and Protocol are:
           Copyright 1988, 1989, 2000, 2005 Pixar
                   All rights Reservered
 @endverbatim
 *
 * Original decalarations are part of [RiSPec].
 * Portions of this file are copied from the renderer Pixie.
 *
 * @todo copy the routines that can be used with no context to riprog, ridynload
 */
#ifdef _WIN32
#if defined(RICPP_EXPORTS)
#if !defined(RICPP_EXTERN)
#define	RICPP_EXTERN(type) extern __declspec(dllexport) type
#endif
#if !defined(RICPP_INTERN)
#define	RICPP_INTERN(type) __declspec(dllexport) type
#endif
#else
#if !defined(RICPP_EXTERN)
#define	RICPP_EXTERN(type) extern __declspec(dllimport) type
#endif
/* In implementation dllexport only */
#endif
#else
#if !defined(RICPP_EXTERN)
#define RICPP_EXTERN(type) extern __attribute__((visibility("default"))) type
#endif
#if !defined(RICPP_INTERN)
#define RICPP_INTERN(type) __attribute__((visibility("default"))) type
#endif
#endif
#ifndef _RICPP_RIBASE_RICPPCONST_H
#include "ricpp/ribase/ricppconst.h"
#endif // _RICPP_RIBASE_RICPPCONST_H
 
#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H
#ifndef _RICPP_RIBASE_RICPPDECLS_H
#include "ricpp/ribase/ricppdecls.h"
#endif // _RICPP_RIBASE_RICPPDECLS_H
#ifdef __cplusplus
/* namespace RiCPP {  */
#endif
#ifdef __cplusplus
extern "C" {
#endif
/*//////////////////////////////////////////////////////////////////////
//
//	Additional functions
//
//////////////////////////////////////////////////////////////////////*/
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
/*//////////////////////////////////////////////////////////////////////
//
//	Delarations of All of the RenderMan Interface Subroutines
//
//////////////////////////////////////////////////////////////////////*/
RICPP_EXTERN(RtVoid)
	RiErrorHandler(RtErrorHandler handler);
RICPP_EXTERN(RtToken)
    RiDeclare (RtString name, RtString declaration);
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
    RiProjection (RtString name, ...),
    RiProjectionV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiClipping (RtFloat hither, RtFloat yon),
	RiClippingPlane(RtFloat x,RtFloat y,RtFloat z,RtFloat nx,RtFloat ny,RtFloat nz),
    RiDepthOfField (RtFloat fstop, RtFloat focallength, RtFloat focaldistance),
    RiShutter (RtFloat smin, RtFloat smax),
    RiCamera (RtString name, ...),
    RiCameraV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
    RiPixelVariance (RtFloat variation),
    RiPixelSamples (RtFloat xsamples, RtFloat ysamples),
    RiPixelFilter (RtFilterFunc function, RtFloat xwidth, RtFloat ywidth),
    RiExposure (RtFloat gain, RtFloat gamma),
    RiImager (RtString name, ...),
    RiImagerV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl),
    RiDisplay (RtString name, RtToken type, RtToken mode, ...),
    RiDisplayV (RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]),
	RiDisplayChannel (RtToken channel, ...),
    RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
    RiHider (RtToken type, ...),
    RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[]),
    RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn),
    RiRelativeDetail (RtFloat relativedetail),
    RiOption (RtString name, ...),
    RiOptionV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
    RiAttributeBegin (void), RiAttributeEnd (void),
    RiColor (RtColor Cs), RiOpacity (RtColor Cs),
    RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
RICPP_EXTERN(RtLightHandle)
    RiLightSource (RtString name, ...),
    RiLightSourceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiAreaLightSource (RtString name, ...),
    RiAreaLightSourceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
    RiIlluminate (RtLightHandle light, RtBoolean onoff),
    RiSurface (RtString name, ...),
    RiSurfaceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiAtmosphere (RtString name, ...),
    RiAtmosphereV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiInterior (RtString name, ...),
    RiInteriorV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiExterior (RtString name, ...),
    RiExteriorV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
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
RICPP_EXTERN(RtVoid)
    RiIdentity (void),
    RiTransform (RtMatrix transform), RiConcatTransform (RtMatrix transform),
    RiPerspective (RtFloat fov),
    RiTranslate (RtFloat dx, RtFloat dy, RtFloat dz),
    RiRotate (RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz),
    RiScale (RtFloat dx, RtFloat dy, RtFloat dz),
    RiSkew (RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2),
    RiDeformation (RtString name, ...),
    RiDeformationV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiDisplacement (RtString name, ...),
    RiDisplacementV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]),
    RiCoordinateSystem (RtToken space),
    RiCoordSysTransform (RtToken space);
RICPP_EXTERN(RtPoint *) 
	RiTransformPoints (RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points);
RICPP_EXTERN(RtVoid) 
	RiTransformBegin (void), 
	RiTransformEnd (void);
RICPP_EXTERN(RtVoid)
    RiAttribute (RtString name, ...),
    RiAttributeV (RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
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
    RiMakeTexture (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeTextureV (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeBump (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeBumpV (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeLatLongEnvironment (RtString pic, RtString tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeLatLongEnvironmentV (RtString pic, RtString tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeCubeFaceEnvironment (RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
    RiMakeCubeFaceEnvironmentV (RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]),
    RiMakeShadow (RtString pic, RtString tex, ...),
    RiMakeShadowV (RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]),
	RiMakeBrickMap (RtInt num, RtString *src, RtString dest, ...),
	RiMakeBrickMapV(RtInt num, RtString *src, RtString dest, RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
	RiResource(RtToken handle, RtToken type, ...),
	RiResourceV(RtToken handle, RtToken type,RtInt n, RtToken tokens[], RtPointer params[]),
	RiResourceBegin(void),
	RiResourceEnd(void);
RICPP_EXTERN(RtArchiveHandle)
	RiArchiveBegin(RtToken name, ...),
	RiArchiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
RICPP_EXTERN(RtVoid)
	RiArchiveEnd(void);
RICPP_EXTERN(RtVoid)
	RiIfBegin(RtString expr),
	RiElseIf(RtString expr),
	RiElse(void),
	RiIfEnd(void);
RICPP_EXTERN(RtVoid)
    RiArchiveRecord (RtToken type, RtString format, ...),
    RiReadArchive (RtString filename, RtArchiveCallback callback, ...),
    RiReadArchiveV (RtString filename, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[]);
#ifdef __cplusplus
} /* extern "C" */
#endif
#ifdef __cplusplus
/* } namespace RiCPP */
#endif
#endif /* _RICPP_RI_RI_H */
