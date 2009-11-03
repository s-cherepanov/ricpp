#ifndef _RICPP_RICONTEXT_RIMACROCLASSES_H
#define _RICPP_RICONTEXT_RIMACROCLASSES_H

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

/** @file rimacroclasses.h
 *  @brief Forward declarations of the macro classes
 *
 *  @author Andreas Pidde (andreas@pidde.de)
 */

namespace RiCPP {

	// Factory
	class CRManInterfaceFactory;

	// Container
	class CRiMacro;

	// Base classes
	class CRManInterfaceCall;
	class CVarParamRManInterfaceCall;
	class CGeometryRManInterfaceCall;
	class CUVRManInterfaceCall;
	class CPolygonRManInterfaceCall;
	
	// ErrorHandler
	class CRiErrorHandler;

	// Declarations
	class CRiDeclare;

	// Synchronization
	class CRiSynchronize;

	// System command
	class CRiSystem;

	// Control, RiCPP specific controls
	class CRiControl;

	// Comments
	class CRiArchiveRecord;

	// Modes
	class CRiBegin;
	class CRiEnd;
	class CRiResourceBegin;
	class CRiResourceEnd;
	class CRiFrameBegin;
	class CRiFrameEnd;
	class CRiWorldBegin;
	class CRiWorldEnd;
	class CRiAttributeBegin;
	class CRiAttributeEnd;
	class CRiTransformBegin;
	class CRiTransformEnd;
	class CRiSolidBegin;
	class CRiSolidEnd;
	class CRiObjectBegin;
	class CRiObjectEnd;
	class CRiArchiveBegin;
	class CRiArchiveEnd;
	class CRiMotionBegin;
	class CRiMotionEnd;
	class CRiIfBegin;
	class CRiElseIfBegin;
	class CRiElseBegin;
	class CRiIfEnd;

	// Instances
	class CRiResource;
	class CRiReadArchive;
	class CRiObjectInstance;
	class CRiProcedural;

	// Lights (also kind of instances)
	class CRiLightSource;
	class CRiAreaLightSource;
	
	// Options
	class CRiOption;
	class CRiFormat;
	class CRiFrameAspectRatio;
	class CRiScreenWindow;
	class CRiCropWindow;
	class CRiProjection;
	class CRiClipping;
	class CRiClippingPlane;
	class CRiDepthOfField;
	class CRiShutter;
	class CRiPixelVariance;
	class CRiPixelSamples;
	class CRiPixelFilter;
	class CRiExposure;
	class CRiImager;
	class CRiQuantize;
	class CRiDisplayChannel;
	class CRiDisplay;
	class CRiHider;
	class CRiColorSamples;
	class CRiRelativeDetail;
	class CRiCamera;

	// Attributes
	class CRiAttribute;
	class CRiColor;
	class CRiOpacity;
	class CRiSurface;
	class CRiAtmosphere;
	class CRiInterior;
	class CRiExterior;
	class CRiDisplacement;
	class CRiTextureCoordinates;
	class CRiShadingRate;
	class CRiShadingInterpolation;
	class CRiMatte;
	class CRiBound;
	class CRiDetail;
	class CRiDetailRange;
	class CRiGeometricApproximation;
	class CRiGeometricRepresentation;
	class CRiOrientation;
	class CRiReverseOrientation;
	class CRiSides;
	class CRiBasis;
	class CRiTrimCurve;
	class CRiIlluminate;

	// Transforms
	class CRiIdentity;
	class CRiTransform;
	class CRiConcatTransform;
	class CRiPerspective;
	class CRiTranslate;
	class CRiRotate;
	class CRiScale;
	class CRiSkew;
	class CRiDeformation;
	class CRiScopedCoordinateSystem;
	class CRiCoordinateSystem;
	class CRiCoordSysTransform;
	class CRiTransformPoints;

	// Primitives
	class CRiPolygon;
	class CRiGeneralPolygon;
	class CRiPointsPolygons;
	class CRiPointsGeneralPolygons;
	class CRiPatch;
	class CRiPatchMesh;
	class CRiNuPatch;
	class CRiSubdivisionMesh;
	class CRiHierarchicalSubdivisionMesh;
	class CRiSphere;
	class CRiCone;
	class CRiCylinder;
	class CRiHyperboloid;
	class CRiParaboloid;
	class CRiDisk;
	class CRiTorus;
	class CRiPoints;
	class CRiCurves;
	class CRiBlobby;
	class CRiGeometry;

	// Externals
	class CRiMakeTexture;
	class CRiMakeBump;
	class CRiMakeLatLongEnvironment;
	class CRiMakeCubeFaceEnvironment;
	class CRiMakeShadow;
	class CRiMakeBrickMap;
}

#endif // _RICPP_RICONTEXT_RIMACROCLASSES_H
