#ifndef _RICPP_RICONTEXT_DORENDER_H
#define _RICPP_RICONTEXT_DORENDER_H

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

/** @file dorender.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of an interface for handling the rendering jobs of render contexts.
 *
 *         Overloaded preProcess(), doProcess(), postProcess() using parameter object only.
 *         E.g.: virtual RtVoid preProcess(CRiSphere &obj) instead of a
 *         virtual RtVoid preSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
 *                                  RtInt n, RtToken tokens[], RtPointer params[])
 *         member function.
 */

#ifndef _RICPP_RICONTEXT_RICONTEXT_H
#include "ricpp/ricontext/ricontext.h"
#endif // _RICPP_RICONTEXT_RICONTEXT_H

namespace RiCPP {

/** @brief Members called called with checked parameters (here from @a CBaseRenderer).
 *
 *  Renderer contexts can inherit from @a CBaseRenderer to implement these
 *  functions to do the rendering. Since this are callbacks, all functions have no return values.
*/
class IDoRender : public IRiContext {
public:
	/** @brief The virtual destructor of the interface
	 */
	inline virtual ~IDoRender() {}

	/** @brief Checks validity of state for common requests.
	 *  @param req Number of the requests.
	 *  @return false if checking fails.
	 *  @exception ExceptRiCPPError Implementation can also throw an exception on errors.
	 */
	virtual bool preCheck(EnumRequests req) = 0;

	/** Replays a single interface call
	 */
	virtual void replayRequest(class CRManInterfaceCall &aRequest, const IArchiveCallback *cb) = 0;

	/** @brief Special context handling (@see IRiContext::abort(), IRiContext::activate(), IRiContext::deactivate()
	 */
	//@{
	virtual RtVoid doAbort(void) = 0;
	virtual RtVoid doActivate(void) = 0;
	virtual RtVoid doDeactivate(void) = 0;
	//@}

	////////////////////////////////

	/** The interface functions similar to IRiCPP, RtToken are tokenized strings here (CBaseRenderer used CTokenMap)
	 */
	//@{
	virtual RtVoid preProcess(CRiSynchronize &obj) = 0;

	virtual RtVoid preProcess(CRiErrorHandler &obj) = 0;
	virtual RtVoid preProcess(CRiDeclare &obj) = 0;
    virtual RtVoid preProcess(CRiControl &obj) = 0;

	virtual RtVoid preProcess(CRiSystem &obj) = 0;

	virtual RtVoid preProcess(CRiBegin &obj) = 0;
	virtual RtVoid preProcess(CRiEnd &obj) = 0;

	virtual RtVoid preProcess(CRiFrameBegin &obj) = 0;
	virtual RtVoid preProcess(CRiFrameEnd &obj) = 0;

	virtual RtVoid preProcess(CRiWorldBegin &obj) = 0;
	virtual RtVoid preProcess(CRiWorldEnd &obj) = 0;

	virtual RtVoid preProcess(CRiAttributeBegin &obj) = 0;
	virtual RtVoid preProcess(CRiAttributeEnd &obj) = 0;

	virtual RtVoid preProcess(CRiTransformBegin &obj) = 0;
	virtual RtVoid preProcess(CRiTransformEnd &obj) = 0;

    virtual RtVoid preProcess(CRiSolidBegin &obj) = 0;
    virtual RtVoid preProcess(CRiSolidEnd &obj) = 0;

	virtual RtVoid preProcess(CRiObjectBegin &obj) = 0;
	virtual RtVoid preProcess(CRiObjectEnd &obj) = 0;
    virtual RtVoid preProcess(CRiObjectInstance &obj) = 0;

    virtual RtVoid preProcess(CRiMotionBegin &obj) = 0;
    virtual RtVoid preProcess(CRiMotionEnd &obj) = 0;

	virtual RtVoid preProcess(CRiResourceBegin &obj) = 0;
	virtual RtVoid preProcess(CRiResourceEnd &obj) = 0;
	virtual RtVoid preProcess(CRiResource &obj) = 0;

	virtual RtVoid preProcess(CRiArchiveBegin &obj) = 0;
	virtual RtVoid preProcess(CRiArchiveEnd &objArchiveEnd) = 0;

    virtual RtVoid preProcess(CRiFormat &obj) = 0;
    virtual RtVoid preProcess(CRiFrameAspectRatio &obj) = 0;

	virtual RtVoid preProcess(CRiScreenWindow &obj) = 0;
    virtual RtVoid preProcess(CRiCropWindow &obj) = 0;
	virtual RtVoid preProcess(CRiProjection &obj) = 0;
	virtual RtVoid preProcess(CRiClipping &obj) = 0;
    virtual RtVoid preProcess(CRiClippingPlane &obj) = 0;
    virtual RtVoid preProcess(CRiDepthOfField &obj) = 0;
    virtual RtVoid preProcess(CRiShutter &obj) = 0;
	virtual RtVoid preProcess(CRiPixelVariance &obj) = 0;
    virtual RtVoid preProcess(CRiPixelSamples &obj) = 0;
    virtual RtVoid preProcess(CRiPixelFilter &obj) = 0;
    virtual RtVoid preProcess(CRiExposure &obj) = 0;
    virtual RtVoid preProcess(CRiImager &obj) = 0;
	virtual RtVoid preProcess(CRiQuantize &obj) = 0;
    virtual RtVoid preProcess(CRiDisplayChannel &obj) = 0;
    virtual RtVoid preProcess(CRiDisplay &obj) = 0;
    virtual RtVoid preProcess(CRiHider &obj) = 0;
    virtual RtVoid preProcess(CRiColorSamples &obj) = 0;
    virtual RtVoid preProcess(CRiRelativeDetail &obj) = 0;
    virtual RtVoid preProcess(CRiCamera &obj) = 0;
    virtual RtVoid preProcess(CRiOption &obj) = 0;
	
    virtual RtVoid preProcess(CRiLightSource &obj) = 0;
	virtual RtVoid preProcess(CRiAreaLightSource &obj) = 0;
	
    virtual RtVoid preProcess(CRiAttribute &obj) = 0;
	virtual RtVoid preProcess(CRiColor &obj) = 0;
	virtual RtVoid preProcess(CRiOpacity &obj) = 0;
    virtual RtVoid preProcess(CRiSurface &obj) = 0;
    virtual RtVoid preProcess(CRiAtmosphere &obj) = 0;
    virtual RtVoid preProcess(CRiInterior &obj) = 0;
	virtual RtVoid preProcess(CRiExterior &obj) = 0;
	virtual RtVoid preProcess(CRiIlluminate &obj) = 0;
    virtual RtVoid preProcess(CRiDisplacement &obj) = 0;
	virtual RtVoid preProcess(CRiTextureCoordinates &obj) = 0;
    virtual RtVoid preProcess(CRiShadingRate &obj) = 0;
	virtual RtVoid preProcess(CRiShadingInterpolation &obj) = 0;
    virtual RtVoid preProcess(CRiMatte &obj) = 0;
	virtual RtVoid preProcess(CRiBound &obj) = 0;
	virtual RtVoid preProcess(CRiDetail &obj) = 0;
	virtual RtVoid preProcess(CRiDetailRange &obj) = 0;
    virtual RtVoid preProcess(CRiGeometricApproximation &obj) = 0;
	virtual RtVoid preProcess(CRiGeometricRepresentation &obj) = 0;
	virtual RtVoid preProcess(CRiOrientation &obj) = 0;
	virtual RtVoid preProcess(CRiReverseOrientation &obj) = 0;
	virtual RtVoid preProcess(CRiSides &obj) = 0;
    virtual RtVoid preProcess(CRiBasis &obj) = 0;
    virtual RtVoid preProcess(CRiTrimCurve &obj) = 0;

	virtual RtVoid preProcess(CRiIdentity &obj) = 0;
	virtual RtVoid preProcess(CRiTransform &obj) = 0;
	virtual RtVoid preProcess(CRiConcatTransform &obj) = 0;
	virtual RtVoid preProcess(CRiPerspective &obj) = 0;
	virtual RtVoid preProcess(CRiTranslate &obj) = 0;
	virtual RtVoid preProcess(CRiRotate &obj) = 0;
	virtual RtVoid preProcess(CRiScale &obj) = 0;
    virtual RtVoid preProcess(CRiSkew &obj) = 0;
	virtual RtVoid preProcess(CRiDeformation &obj) = 0;
	virtual RtVoid preProcess(CRiScopedCoordinateSystem &obj) = 0;
	virtual RtVoid preProcess(CRiCoordinateSystem &obj) = 0;
	virtual RtVoid preProcess(CRiCoordSysTransform &obj) = 0;
	virtual RtVoid preProcess(CRiTransformPoints &obj) = 0;

    virtual RtVoid preProcess(CRiPolygon &obj) = 0;
	virtual RtVoid preProcess(CRiGeneralPolygon &obj) = 0;
	virtual RtVoid preProcess(CRiPointsPolygons &obj) = 0;
    virtual RtVoid preProcess(CRiPointsGeneralPolygons &obj) = 0;

	virtual RtVoid preProcess(CRiPatch &obj) = 0;
	virtual RtVoid preProcess(CRiPatchMesh &obj) = 0;
    virtual RtVoid preProcess(CRiNuPatch &obj) = 0;

	virtual RtVoid preProcess(CRiSubdivisionMesh &obj) = 0;
	virtual RtVoid preProcess(CRiHierarchicalSubdivisionMesh &obj) = 0;

	virtual RtVoid preProcess(CRiSphere &obj) = 0;
    virtual RtVoid preProcess(CRiCone &obj) = 0;
    virtual RtVoid preProcess(CRiCylinder &obj) = 0;
    virtual RtVoid preProcess(CRiHyperboloid &obj) = 0;
    virtual RtVoid preProcess(CRiParaboloid &obj) = 0;
    virtual RtVoid preProcess(CRiDisk &obj) = 0;
    virtual RtVoid preProcess(CRiTorus &obj) = 0;

    virtual RtVoid preProcess(CRiPoints &obj) = 0;
    virtual RtVoid preProcess(CRiCurves &obj) = 0;

	virtual RtVoid preProcess(CRiBlobby &obj) = 0;

	virtual RtVoid preProcess(CRiProcedural &obj) = 0;

	virtual RtVoid preProcess(CRiGeometry &obj) = 0;

	virtual RtVoid preProcess(CRiMakeTexture &obj) = 0;
    virtual RtVoid preProcess(CRiMakeBump &obj) = 0;
    virtual RtVoid preProcess(CRiMakeLatLongEnvironment &obj) = 0;
    virtual RtVoid preProcess(CRiMakeCubeFaceEnvironment &obj) = 0;
    virtual RtVoid preProcess(CRiMakeShadow &obj) = 0;
    virtual RtVoid preProcess(CRiMakeBrickMap &obj) = 0;

	virtual RtVoid preProcess(CRiArchiveRecord &obj) = 0;
	virtual RtVoid preProcess(CRiReadArchive &obj) = 0;

	virtual RtVoid preProcess(CRiIfBegin &obj) = 0;
	virtual RtVoid preProcess(CRiElseIfBegin &obj) = 0;
	virtual RtVoid preProcess(CRiElseBegin &obj) = 0;
	virtual RtVoid preProcess(CRiIfEnd &obj) = 0;

	////////////////////////////////

	virtual RtVoid doProcess(CRiSynchronize &obj) = 0;
	
	virtual RtVoid doProcess(CRiErrorHandler &obj) = 0;
	virtual RtVoid doProcess(CRiDeclare &obj) = 0;
    virtual RtVoid doProcess(CRiControl &obj) = 0;
	
	virtual RtVoid doProcess(CRiSystem &obj) = 0;
	
	virtual RtVoid doProcess(CRiBegin &obj) = 0;
	virtual RtVoid doProcess(CRiEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiFrameBegin &obj) = 0;
	virtual RtVoid doProcess(CRiFrameEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiWorldBegin &obj) = 0;
	virtual RtVoid doProcess(CRiWorldEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiAttributeBegin &obj) = 0;
	virtual RtVoid doProcess(CRiAttributeEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiTransformBegin &obj) = 0;
	virtual RtVoid doProcess(CRiTransformEnd &obj) = 0;
	
    virtual RtVoid doProcess(CRiSolidBegin &obj) = 0;
    virtual RtVoid doProcess(CRiSolidEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiObjectBegin &obj) = 0;
	virtual RtVoid doProcess(CRiObjectEnd &obj) = 0;
    virtual RtVoid doProcess(CRiObjectInstance &obj) = 0;
	
    virtual RtVoid doProcess(CRiMotionBegin &obj) = 0;
    virtual RtVoid doProcess(CRiMotionEnd &obj) = 0;
	
	virtual RtVoid doProcess(CRiResourceBegin &obj) = 0;
	virtual RtVoid doProcess(CRiResourceEnd &obj) = 0;
	virtual RtVoid doProcess(CRiResource &obj) = 0;
	
	virtual RtVoid doProcess(CRiArchiveBegin &obj) = 0;
	virtual RtVoid doProcess(CRiArchiveEnd &objArchiveEnd) = 0;
	
    virtual RtVoid doProcess(CRiFormat &obj) = 0;
    virtual RtVoid doProcess(CRiFrameAspectRatio &obj) = 0;
	
	virtual RtVoid doProcess(CRiScreenWindow &obj) = 0;
    virtual RtVoid doProcess(CRiCropWindow &obj) = 0;
	virtual RtVoid doProcess(CRiProjection &obj) = 0;
	virtual RtVoid doProcess(CRiClipping &obj) = 0;
    virtual RtVoid doProcess(CRiClippingPlane &obj) = 0;
    virtual RtVoid doProcess(CRiDepthOfField &obj) = 0;
    virtual RtVoid doProcess(CRiShutter &obj) = 0;
	virtual RtVoid doProcess(CRiPixelVariance &obj) = 0;
    virtual RtVoid doProcess(CRiPixelSamples &obj) = 0;
    virtual RtVoid doProcess(CRiPixelFilter &obj) = 0;
    virtual RtVoid doProcess(CRiExposure &obj) = 0;
    virtual RtVoid doProcess(CRiImager &obj) = 0;
	virtual RtVoid doProcess(CRiQuantize &obj) = 0;
    virtual RtVoid doProcess(CRiDisplayChannel &obj) = 0;
    virtual RtVoid doProcess(CRiDisplay &obj) = 0;
    virtual RtVoid doProcess(CRiHider &obj) = 0;
    virtual RtVoid doProcess(CRiColorSamples &obj) = 0;
    virtual RtVoid doProcess(CRiRelativeDetail &obj) = 0;
    virtual RtVoid doProcess(CRiCamera &obj) = 0;
    virtual RtVoid doProcess(CRiOption &obj) = 0;
	
    virtual RtVoid doProcess(CRiLightSource &obj) = 0;
	virtual RtVoid doProcess(CRiAreaLightSource &obj) = 0;
	
    virtual RtVoid doProcess(CRiAttribute &obj) = 0;
	virtual RtVoid doProcess(CRiColor &obj) = 0;
	virtual RtVoid doProcess(CRiOpacity &obj) = 0;
    virtual RtVoid doProcess(CRiSurface &obj) = 0;
    virtual RtVoid doProcess(CRiAtmosphere &obj) = 0;
    virtual RtVoid doProcess(CRiInterior &obj) = 0;
	virtual RtVoid doProcess(CRiExterior &obj) = 0;
	virtual RtVoid doProcess(CRiIlluminate &obj) = 0;
    virtual RtVoid doProcess(CRiDisplacement &obj) = 0;
	virtual RtVoid doProcess(CRiTextureCoordinates &obj) = 0;
    virtual RtVoid doProcess(CRiShadingRate &obj) = 0;
	virtual RtVoid doProcess(CRiShadingInterpolation &obj) = 0;
    virtual RtVoid doProcess(CRiMatte &obj) = 0;
	virtual RtVoid doProcess(CRiBound &obj) = 0;
	virtual RtVoid doProcess(CRiDetail &obj) = 0;
	virtual RtVoid doProcess(CRiDetailRange &obj) = 0;
    virtual RtVoid doProcess(CRiGeometricApproximation &obj) = 0;
	virtual RtVoid doProcess(CRiGeometricRepresentation &obj) = 0;
	virtual RtVoid doProcess(CRiOrientation &obj) = 0;
	virtual RtVoid doProcess(CRiReverseOrientation &obj) = 0;
	virtual RtVoid doProcess(CRiSides &obj) = 0;
    virtual RtVoid doProcess(CRiBasis &obj) = 0;
    virtual RtVoid doProcess(CRiTrimCurve &obj) = 0;
	
	virtual RtVoid doProcess(CRiIdentity &obj) = 0;
	virtual RtVoid doProcess(CRiTransform &obj) = 0;
	virtual RtVoid doProcess(CRiConcatTransform &obj) = 0;
	virtual RtVoid doProcess(CRiPerspective &obj) = 0;
	virtual RtVoid doProcess(CRiTranslate &obj) = 0;
	virtual RtVoid doProcess(CRiRotate &obj) = 0;
	virtual RtVoid doProcess(CRiScale &obj) = 0;
    virtual RtVoid doProcess(CRiSkew &obj) = 0;
	virtual RtVoid doProcess(CRiDeformation &obj) = 0;
	virtual RtVoid doProcess(CRiScopedCoordinateSystem &obj) = 0;
	virtual RtVoid doProcess(CRiCoordinateSystem &obj) = 0;
	virtual RtVoid doProcess(CRiCoordSysTransform &obj) = 0;
	virtual RtVoid doProcess(CRiTransformPoints &obj) = 0;
	
    virtual RtVoid doProcess(CRiPolygon &obj) = 0;
	virtual RtVoid doProcess(CRiGeneralPolygon &obj) = 0;
	virtual RtVoid doProcess(CRiPointsPolygons &obj) = 0;
    virtual RtVoid doProcess(CRiPointsGeneralPolygons &obj) = 0;
	
	virtual RtVoid doProcess(CRiPatch &obj) = 0;
	virtual RtVoid doProcess(CRiPatchMesh &obj) = 0;
    virtual RtVoid doProcess(CRiNuPatch &obj) = 0;
	
	virtual RtVoid doProcess(CRiSubdivisionMesh &obj) = 0;
	virtual RtVoid doProcess(CRiHierarchicalSubdivisionMesh &obj) = 0;
	
	virtual RtVoid doProcess(CRiSphere &obj) = 0;
    virtual RtVoid doProcess(CRiCone &obj) = 0;
    virtual RtVoid doProcess(CRiCylinder &obj) = 0;
    virtual RtVoid doProcess(CRiHyperboloid &obj) = 0;
    virtual RtVoid doProcess(CRiParaboloid &obj) = 0;
    virtual RtVoid doProcess(CRiDisk &obj) = 0;
    virtual RtVoid doProcess(CRiTorus &obj) = 0;
	
    virtual RtVoid doProcess(CRiPoints &obj) = 0;
    virtual RtVoid doProcess(CRiCurves &obj) = 0;
	
	virtual RtVoid doProcess(CRiBlobby &obj) = 0;
	
	virtual RtVoid doProcess(CRiProcedural &obj) = 0;
	
	virtual RtVoid doProcess(CRiGeometry &obj) = 0;
	
	virtual RtVoid doProcess(CRiMakeTexture &obj) = 0;
    virtual RtVoid doProcess(CRiMakeBump &obj) = 0;
    virtual RtVoid doProcess(CRiMakeLatLongEnvironment &obj) = 0;
    virtual RtVoid doProcess(CRiMakeCubeFaceEnvironment &obj) = 0;
    virtual RtVoid doProcess(CRiMakeShadow &obj) = 0;
    virtual RtVoid doProcess(CRiMakeBrickMap &obj) = 0;
	
	virtual RtVoid doProcess(CRiArchiveRecord &obj) = 0;
	virtual RtVoid doProcess(CRiReadArchive &obj) = 0;
	
	virtual RtVoid doProcess(CRiIfBegin &obj) = 0;
	virtual RtVoid doProcess(CRiElseIfBegin &obj) = 0;
	virtual RtVoid doProcess(CRiElseBegin &obj) = 0;
	virtual RtVoid doProcess(CRiIfEnd &obj) = 0;
	
	////////////////////////////////

	virtual RtVoid postProcess(CRiSynchronize &obj) = 0;
	
	virtual RtVoid postProcess(CRiErrorHandler &obj) = 0;
	virtual RtVoid postProcess(CRiDeclare &obj) = 0;
    virtual RtVoid postProcess(CRiControl &obj) = 0;
	
	virtual RtVoid postProcess(CRiSystem &obj) = 0;
	
	virtual RtVoid postProcess(CRiBegin &obj) = 0;
	virtual RtVoid postProcess(CRiEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiFrameBegin &obj) = 0;
	virtual RtVoid postProcess(CRiFrameEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiWorldBegin &obj) = 0;
	virtual RtVoid postProcess(CRiWorldEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiAttributeBegin &obj) = 0;
	virtual RtVoid postProcess(CRiAttributeEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiTransformBegin &obj) = 0;
	virtual RtVoid postProcess(CRiTransformEnd &obj) = 0;
	
    virtual RtVoid postProcess(CRiSolidBegin &obj) = 0;
    virtual RtVoid postProcess(CRiSolidEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiObjectBegin &obj) = 0;
	virtual RtVoid postProcess(CRiObjectEnd &obj) = 0;
    virtual RtVoid postProcess(CRiObjectInstance &obj) = 0;
	
    virtual RtVoid postProcess(CRiMotionBegin &obj) = 0;
    virtual RtVoid postProcess(CRiMotionEnd &obj) = 0;
	
	virtual RtVoid postProcess(CRiResourceBegin &obj) = 0;
	virtual RtVoid postProcess(CRiResourceEnd &obj) = 0;
	virtual RtVoid postProcess(CRiResource &obj) = 0;
	
	virtual RtVoid postProcess(CRiArchiveBegin &obj) = 0;
	virtual RtVoid postProcess(CRiArchiveEnd &objArchiveEnd) = 0;
	
    virtual RtVoid postProcess(CRiFormat &obj) = 0;
    virtual RtVoid postProcess(CRiFrameAspectRatio &obj) = 0;
	
	virtual RtVoid postProcess(CRiScreenWindow &obj) = 0;
    virtual RtVoid postProcess(CRiCropWindow &obj) = 0;
	virtual RtVoid postProcess(CRiProjection &obj) = 0;
	virtual RtVoid postProcess(CRiClipping &obj) = 0;
    virtual RtVoid postProcess(CRiClippingPlane &obj) = 0;
    virtual RtVoid postProcess(CRiDepthOfField &obj) = 0;
    virtual RtVoid postProcess(CRiShutter &obj) = 0;
	virtual RtVoid postProcess(CRiPixelVariance &obj) = 0;
    virtual RtVoid postProcess(CRiPixelSamples &obj) = 0;
    virtual RtVoid postProcess(CRiPixelFilter &obj) = 0;
    virtual RtVoid postProcess(CRiExposure &obj) = 0;
    virtual RtVoid postProcess(CRiImager &obj) = 0;
	virtual RtVoid postProcess(CRiQuantize &obj) = 0;
    virtual RtVoid postProcess(CRiDisplayChannel &obj) = 0;
    virtual RtVoid postProcess(CRiDisplay &obj) = 0;
    virtual RtVoid postProcess(CRiHider &obj) = 0;
    virtual RtVoid postProcess(CRiColorSamples &obj) = 0;
    virtual RtVoid postProcess(CRiRelativeDetail &obj) = 0;
    virtual RtVoid postProcess(CRiCamera &obj) = 0;
    virtual RtVoid postProcess(CRiOption &obj) = 0;
	
    virtual RtVoid postProcess(CRiLightSource &obj) = 0;
	virtual RtVoid postProcess(CRiAreaLightSource &obj) = 0;
	
    virtual RtVoid postProcess(CRiAttribute &obj) = 0;
	virtual RtVoid postProcess(CRiColor &obj) = 0;
	virtual RtVoid postProcess(CRiOpacity &obj) = 0;
    virtual RtVoid postProcess(CRiSurface &obj) = 0;
    virtual RtVoid postProcess(CRiAtmosphere &obj) = 0;
    virtual RtVoid postProcess(CRiInterior &obj) = 0;
	virtual RtVoid postProcess(CRiExterior &obj) = 0;
	virtual RtVoid postProcess(CRiIlluminate &obj) = 0;
    virtual RtVoid postProcess(CRiDisplacement &obj) = 0;
	virtual RtVoid postProcess(CRiTextureCoordinates &obj) = 0;
    virtual RtVoid postProcess(CRiShadingRate &obj) = 0;
	virtual RtVoid postProcess(CRiShadingInterpolation &obj) = 0;
    virtual RtVoid postProcess(CRiMatte &obj) = 0;
	virtual RtVoid postProcess(CRiBound &obj) = 0;
	virtual RtVoid postProcess(CRiDetail &obj) = 0;
	virtual RtVoid postProcess(CRiDetailRange &obj) = 0;
    virtual RtVoid postProcess(CRiGeometricApproximation &obj) = 0;
	virtual RtVoid postProcess(CRiGeometricRepresentation &obj) = 0;
	virtual RtVoid postProcess(CRiOrientation &obj) = 0;
	virtual RtVoid postProcess(CRiReverseOrientation &obj) = 0;
	virtual RtVoid postProcess(CRiSides &obj) = 0;
    virtual RtVoid postProcess(CRiBasis &obj) = 0;
    virtual RtVoid postProcess(CRiTrimCurve &obj) = 0;
	
	virtual RtVoid postProcess(CRiIdentity &obj) = 0;
	virtual RtVoid postProcess(CRiTransform &obj) = 0;
	virtual RtVoid postProcess(CRiConcatTransform &obj) = 0;
	virtual RtVoid postProcess(CRiPerspective &obj) = 0;
	virtual RtVoid postProcess(CRiTranslate &obj) = 0;
	virtual RtVoid postProcess(CRiRotate &obj) = 0;
	virtual RtVoid postProcess(CRiScale &obj) = 0;
    virtual RtVoid postProcess(CRiSkew &obj) = 0;
	virtual RtVoid postProcess(CRiDeformation &obj) = 0;
	virtual RtVoid postProcess(CRiScopedCoordinateSystem &obj) = 0;
	virtual RtVoid postProcess(CRiCoordinateSystem &obj) = 0;
	virtual RtVoid postProcess(CRiCoordSysTransform &obj) = 0;
	virtual RtVoid postProcess(CRiTransformPoints &obj) = 0;
	
    virtual RtVoid postProcess(CRiPolygon &obj) = 0;
	virtual RtVoid postProcess(CRiGeneralPolygon &obj) = 0;
	virtual RtVoid postProcess(CRiPointsPolygons &obj) = 0;
    virtual RtVoid postProcess(CRiPointsGeneralPolygons &obj) = 0;
	
	virtual RtVoid postProcess(CRiPatch &obj) = 0;
	virtual RtVoid postProcess(CRiPatchMesh &obj) = 0;
    virtual RtVoid postProcess(CRiNuPatch &obj) = 0;
	
	virtual RtVoid postProcess(CRiSubdivisionMesh &obj) = 0;
	virtual RtVoid postProcess(CRiHierarchicalSubdivisionMesh &obj) = 0;
	
	virtual RtVoid postProcess(CRiSphere &obj) = 0;
    virtual RtVoid postProcess(CRiCone &obj) = 0;
    virtual RtVoid postProcess(CRiCylinder &obj) = 0;
    virtual RtVoid postProcess(CRiHyperboloid &obj) = 0;
    virtual RtVoid postProcess(CRiParaboloid &obj) = 0;
    virtual RtVoid postProcess(CRiDisk &obj) = 0;
    virtual RtVoid postProcess(CRiTorus &obj) = 0;
	
    virtual RtVoid postProcess(CRiPoints &obj) = 0;
    virtual RtVoid postProcess(CRiCurves &obj) = 0;
	
	virtual RtVoid postProcess(CRiBlobby &obj) = 0;
	
	virtual RtVoid postProcess(CRiProcedural &obj) = 0;
	
	virtual RtVoid postProcess(CRiGeometry &obj) = 0;
	
	virtual RtVoid postProcess(CRiMakeTexture &obj) = 0;
    virtual RtVoid postProcess(CRiMakeBump &obj) = 0;
    virtual RtVoid postProcess(CRiMakeLatLongEnvironment &obj) = 0;
    virtual RtVoid postProcess(CRiMakeCubeFaceEnvironment &obj) = 0;
    virtual RtVoid postProcess(CRiMakeShadow &obj) = 0;
    virtual RtVoid postProcess(CRiMakeBrickMap &obj) = 0;
	
	virtual RtVoid postProcess(CRiArchiveRecord &obj) = 0;
	virtual RtVoid postProcess(CRiReadArchive &obj) = 0;
	
	virtual RtVoid postProcess(CRiIfBegin &obj) = 0;
	virtual RtVoid postProcess(CRiElseIfBegin &obj) = 0;
	virtual RtVoid postProcess(CRiElseBegin &obj) = 0;
	virtual RtVoid postProcess(CRiIfEnd &obj) = 0;
	//@}
}; // IDoRender
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_DORENDER_H
