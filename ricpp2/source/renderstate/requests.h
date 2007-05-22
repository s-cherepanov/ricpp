#ifndef RICPP_RICPP_REQUESTS_H
#define RICPP_RICPP_REQUESTS_H

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

/** @file requests.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Constants (enumerations) for the various requests (interface routines)
 */

namespace RiCPP {

/** Index for each interface function (request)
 */
enum EnumRequests {
	// General routines
	REQ_ERROR_HANDLER          = 0, //!< Index for ri function IRi::errorHandler()
	REQ_DECLARE,                    //!< Index for ri function IRi::declare()

	// Context Switching
	REQ_GET_CONTEXT,				//!< Index for ri function IRi::getContext() (no RIB binding)
	REQ_CONTEXT,					//!< Index for ri function IRi::context() (no RIB binding)

	// Modes

	// Begin-end block
	REQ_BEGIN,						//!< Index for ri function IRi::begin() (no RIB binding)
	REQ_END,						//!< Index for ri function IRi::end() (no RIB binding)

	// Frame block
	REQ_FRAME_BEGIN,				//!< Index for ri function IRi::frameBegin()
	REQ_FRAME_END,					//!< Index for ri function IRi::frameEnd()

	// World block
	REQ_WORLD_BEGIN,				//!< Index for ri function IRi::worldBegin()
	REQ_WORLD_END,					//!< Index for ri function IRi::worldEnd()

	// Attribute block
	REQ_ATTRIBUTE_BEGIN,			//!< Index for ri function IRi::attributeBegin()
	REQ_ATTRIBUTE_END,				//!< Index for ri function IRi::attributeEnd()

	// Transformation block
	REQ_TRANSFORM_BEGIN,			//!< Index for ri function IRi::transformBegin()
	REQ_TRANSFORM_END,				//!< Index for ri function IRi::transformEnd()

	// Solid block
	REQ_SOLID_BEGIN,				//!< Index for ri function IRi::solidBegin()
	REQ_SOLID_END,					//!< Index for ri function IRi::solidEnd()

	// Object block
	REQ_OBJECT_BEGIN,				//!< Index for ri function IRi::objectBegin()
	REQ_OBJECT_END,					//!< Index for ri function IRi::objectEnd()

	// Motion block
	REQ_MOTION_BEGIN,				//!< Index for ri function IRi::motionBegin()
	REQ_MOTION_END,					//!< Index for ri function IRi::motionEnd()

	// Options
	REQ_FORMAT,						//!< Index for ri function IRi::format()
	REQ_FRAME_ASPECT_RATIO,			//!< Index for ri function IRi::frameAspectRatio()
	REQ_SCREEN_WINDOW,				//!< Index for ri function IRi::screenWindow()
	REQ_CROP_WINDOW,				//!< Index for ri function IRi::cropWindow()
	REQ_PROJECTION,					//!< Index for ri function IRi::projection()
	REQ_CLIPPING,					//!< Index for ri function IRi::clipping()
	REQ_CLIPPING_PLANE,				//!< Index for ri function IRi::clippingPlane()
	REQ_DEPTH_OF_FIELD,				//!< Index for ri function IRi::depthOfField()
	REQ_SHUTTER,					//!< Index for ri function IRi::shutter()
	REQ_PIXEL_VARIANCE,				//!< Index for ri function IRi::pixelVariance()
	REQ_PIXEL_SAMPLES,				//!< Index for ri function IRi::pixelSamples()
	REQ_PIXEL_FILTER,				//!< Index for ri function IRi::pixelFilter()
	REQ_EXPOSURE,					//!< Index for ri function IRi::exposure()
	REQ_IMAGER,						//!< Index for ri function IRi::imager()
	REQ_QUANTIZE,					//!< Index for ri function IRi::quantize()
	REQ_DISPLAY,					//!< Index for ri function IRi::display()
	REQ_HIDER,						//!< Index for ri function IRi::hider()
	REQ_COLOR_SAMPLES,				//!< Index for ri function IRi::colorSamples()
	REQ_RELATIVE_DETAIL,			//!< Index for ri function IRi::relativeDetail()
	REQ_OPTION,						//!< Index for ri function IRi::option()

	// Lights
	REQ_LIGHT_SOURCE,				//!< Index for ri function IRi::lightSource()
	REQ_AREA_LIGHT_SOURCE,			//!< Index for ri function IRi::areaLightSource()

	// Attributes
	REQ_ATTRIBUTE,					//!< Index for ri function IRi::attribute()
	REQ_COLOR,						//!< Index for ri function IRi::color()
	REQ_OPACITY,					//!< Index for ri function IRi::opacity()
	REQ_SURFACE,					//!< Index for ri function IRi::surface()
	REQ_ATMOSPHERE,					//!< Index for ri function IRi::atmosphere()
	REQ_INTERIOR,					//!< Index for ri function IRi::interior()
	REQ_EXTERIOR,					//!< Index for ri function IRi::exterior()
	REQ_ILLUMINATE,					//!< Index for ri function IRi::illuminate()
	REQ_DISPLACEMENT,				//!< Index for ri function IRi::displacement()
	REQ_TEXTURE_COORDINATES,		//!< Index for ri function IRi::textureCoordinates()
	REQ_SHADING_RATE,				//!< Index for ri function IRi::shadingRate()
	REQ_SHADING_INTERPOLATION,		//!< Index for ri function IRi::shadingInterpolation()
	REQ_MATTE,						//!< Index for ri function IRi::matte()
	REQ_BOUND,						//!< Index for ri function IRi::bound()
	REQ_DETAIL,						//!< Index for ri function IRi::detail()
	REQ_DETAIL_RANGE,				//!< Index for ri function IRi::detailRange()
	REQ_GEOMETRIC_APPROXIMATION,	//!< Index for ri function IRi::geometricApproximation()
	REQ_GEOMETRIC_REPRESENTATION,	//!< Index for ri function IRi::geometricRepresentation()
	REQ_ORIENTATION,				//!< Index for ri function IRi::orientation()
	REQ_REVERSE_ORIENTATION,		//!< Index for ri function IRi::reverseOrientation()
	REQ_SIDES,						//!< Index for ri function IRi::sides()
	REQ_BASIS,						//!< Index for ri function IRi::basis()
	REQ_TRIM_CURVE,					//!< Index for ri function IRi::trimCurve()

	// Transformations
	REQ_IDENTITY,					//!< Index for ri function IRi::identity()
	REQ_TRANSFORM,					//!< Index for ri function IRi::transform()
	REQ_CONCAT_TRANSFORM,			//!< Index for ri function IRi::concatTransform()
	REQ_PERSPECTIVE,				//!< Index for ri function IRi::perspective()
	REQ_TRANSLATE,					//!< Index for ri function IRi::translate()
	REQ_ROTATE,						//!< Index for ri function IRi::rotate()
	REQ_SCALE,						//!< Index for ri function IRi::scale()
	REQ_SKEW,						//!< Index for ri function IRi::skew()
	REQ_DEFORMATION,				//!< Index for ri function IRi::deformation()
	REQ_COORDINATE_SYSTEM,			//!< Index for ri function IRi::coordinateSystem()
	REQ_COORD_SYS_TRANSFORM,		//!< Index for ri function IRi::coordSysTransform()
	REQ_TRANSFORM_POINTS,			//!< Index for ri function IRi::transformPoints() (no RIB binding)

	// Polygons
	REQ_POLYGON,					//!< Index for ri function IRi::polygon()
	REQ_GENERAL_POLYGON,			//!< Index for ri function IRi::generalPolygon()
	REQ_POINTS_POLYGONS,			//!< Index for ri function IRi::pointsPolygons()
	REQ_POINTS_GENERAL_POLYGONS,	//!< Index for ri function IRi::pointsGeneralPolygons()

	// Patches
	REQ_PATCH,						//!< Index for ri function IRi::patch()
	REQ_PATCH_MESH,					//!< Index for ri function IRi::patchMesh()
	REQ_NU_PATCH,					//!< Index for ri function IRi::nuPatch()

	// Subdivision Surfaces
	REQ_SUBDIVISION_MESH,			//!< Index for ri function IRi::subdivisionMesh()

	// Quadrics
	REQ_SPHERE,						//!< Index for ri function IRi::sphere()
	REQ_CONE,						//!< Index for ri function IRi::cone()
	REQ_CYLINDER,					//!< Index for ri function IRi::cylinder()
	REQ_HYPERBOLOID,				//!< Index for ri function IRi::hyperboloid()
	REQ_PARABOLOID,					//!< Index for ri function IRi::paraboloid()
	REQ_DISK,						//!< Index for ri function IRi::disk()
	REQ_TORUS,						//!< Index for ri function IRi::torus()

	// Point and curve primitives
	REQ_POINTS,						//!< Index for ri function IRi::points()
	REQ_CURVES,						//!< Index for ri function IRi::curves()

	// Blobby implicit surfaces
	REQ_BLOBBY,						//!< Index for ri function IRi::blobby()

	// Procedural primitives
	REQ_PROCEDURAL,					//!< Index for ri function IRi::procedural()
	REQ_PROC_DELAYED_READ_ARCHIVE,	//!< Index for ri function IRi::procDelayedReadArchive()
	REQ_PROC_RUN_PROGRAM,			//!< Index for ri function IRi::procRunProgram()
	REQ_PROC_DYNAMIC_LOAD,			//!< Index for ri function IRi::procDynamicLoad()

	// General objects
	REQ_GEOMETRY,					//!< Index for ri function IRi::geometry()

	// retained geometry
	REQ_OBJECT_INSTANCE,			//!< Index for ri function IRi::objectInstance()

	// Map-making
	REQ_MAKE_TEXTURE,				//!< Index for ri function IRi::makeTexture()
	REQ_MAKE_BUMP,					//!< Index for ri function IRi::makeBump()
	REQ_MAKE_LAT_LONG_ENVIRONMENT,	//!< Index for ri function IRi::makeLatLongEnvironment()
	REQ_MAKE_CUBE_FACE_ENVIRONMENT,	//!< Index for ri function IRi::makeCubeFaceEnvironment()
	REQ_MAKE_SHADOW,				//!< Index for ri function IRi::makeShadow()

	// Archive files
	REQ_ARCHIVE_RECORD,				//!< Index for ri function IRi::archiveRecord() (no RIB binding)
	REQ_READ_ARCHIVE,				//!< Index for ri function IRi::readArchive()

	// Additional
	REQ_VERSION                     //!< Index for version (additional, in RIB only)
};

/** Number of requests
 */
const int N_REQUESTS = (int)REQ_VERSION + 1;

} // namespace RiCPP

#endif // RICPP_RICPP_REQUESTS_H
