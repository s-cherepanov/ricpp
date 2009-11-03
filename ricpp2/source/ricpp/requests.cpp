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

/** @file requests.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Various requests names (interface routines)
 */

#include "ricpp/ricpp/requests.h"

#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H

using namespace RiCPP;

const char *CRequestInfo::ms_requestNames[N_REQUESTS] =
{
	// General routines
	RI_UNKNOWN,
	"ErrorHandler",
	"Synchronize",
	"System",
	"Declare",

	// Context Switching
	"GetContext",
	"Context",

	// Resource
	"Resource",

	// Modes

	// Begin-end block
	"Begin",
	"End",

	// Frame block
	"FrameBegin",
	"FrameEnd",

	// World block
	"WorldBegin",
	"WorldEnd",

	// Attribute block
	"AttributeBegin",
	"AttributeEnd",

	// Transformation block
	"TransformBegin",
	"TransformEnd",

	// Solid block
	"SolidBegin",
	"SolidEnd",

	// Object block
	"ObjectBegin",
	"ObjectEnd",

	// Motion block
	"MotionBegin",
	"MotionEnd",

	// Resource block
	"ResourceBegin",
	"ResourceEnd",

	// Archive block
	"ArchiveBegin",
	"ArchiveEnd",

	// Options
	"Format",
	"FrameAspectRatio",
	"ScreenWindow",
	"CropWindow",
	"Projection",
	"Clipping",
	"ClippingPlane",
	"DepthOfField",
	"Shutter",
	"PixelVariance",
	"PixelSamples",
	"PixelFilter",
	"Exposure",
	"Imager",
	"Quantize",
	"DisplayChannel",
	"Display",
	"Hider",
	"ColorSamples",
	"RelativeDetail",
	"Camera",
	"Option",

	// Lights
	"LightSource",
	"AreaLightSource",

	// Attributes
	"Attribute",
	"Color",
	"Opacity",
	"Surface",
	"Atmosphere",
	"Interior",
	"Exterior",
	"Illuminate",
	"Displacement",
	"TextureCoordinates",
	"ShadingRate",
	"ShadingInterpolation",
	"Matte",
	"Bound",
	"Detail",
	"DetailRange",
	"GeometricApproximation",
	"GeometricRepresentation",
	"Orientation",
	"ReverseOrientation",
	"Sides",
	"Basis",
	"TrimCurve",

	// Transformations
	"Identity",
	"Transform",
	"ConcatTransform",
	"Perspective",
	"Translate",
	"Rotate",
	"Scale",
	"Skew",
	"Deformation",
	"ScopedCoordinateSystem",
	"CoordinateSystem",
	"CoordSysTransform",
	"TransformPoints",

	// Polygons
	"Polygon",
	"GeneralPolygon",
	"PointsPolygons",
	"PointsGeneralPolygons",

	// Patches
	"Patch",
	"PatchMesh",
	"NuPatch",

	// Subdivision Surfaces
	"SubdivisionMesh",
	"HierarchicalSubdivisionMesh",

	// Quadrics
	"Sphere",
	"Cone",
	"Cylinder",
	"Hyperboloid",
	"Paraboloid",
	"Disk",
	"Torus",

	// Point and curve primitives
	"Points",
	"Curves",

	// Blobby implicit surfaces
	"Blobby",

	// Procedural primitives
	"Procedural",
	"ProcDelayedReadArchive",
	"ProcRunProgram",
	"ProcDynamicLoad",

	// General objects
	"Geometry",

	// retained models
	"ObjectInstance",
	"ArchiveInstance",

	// Map-making
	"MakeTexture",
	"MakeBump",
	"MakeLatLongEnvironment",
	"MakeCubeFaceEnvironment",
	"MakeShadow",
	"MakeBrickMap",

	// Archive files
	"ArchiveRecord",
	"ReadArchive",

	// Conditions
	"IfBegin",
	"ElseIf",
	"Else",
	"IfEnd",

	// Comment
	"#",

	// Additional
	"Control",

	// also end marker, version starts with a small 'v'
	"version"
};

std::map<std::string, EnumRequests> CRequestInfo::ms_requestMap;

const char *CRequestInfo::requestName(EnumRequests req)
{
	if ( req < REQ_UNKNOWN || req >= N_REQUESTS ) {
		return ms_requestNames[REQ_UNKNOWN];
	}
	return ms_requestNames[req];
}

EnumRequests CRequestInfo::requestNumber(const char *req)
{
	if ( req == 0 || !*req )
		return REQ_UNKNOWN;

	if ( ms_requestMap.empty() ) {
		for ( int req = REQ_UNKNOWN+1; req != N_REQUESTS; ++req ) {
			ms_requestMap[requestName(static_cast<EnumRequests>(req))] = static_cast<EnumRequests>(req);
		}
	}

	std::map<std::string, EnumRequests>::const_iterator iter = ms_requestMap.find(std::string(req));
	if ( iter != ms_requestMap.end() ) {
		return (*iter).second;
	}
	return REQ_UNKNOWN;
}
