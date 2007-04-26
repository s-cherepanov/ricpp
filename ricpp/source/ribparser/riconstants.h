#ifndef _RICPP_RIBPARSER_RICONSTANTS_H
#define _RICPP_RIBPARSER_RICONSTANTS_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file riconstants.h
 *  \brief Some additional constants for context checking, declarations and parsing
 *  \author Andreas Pidde (andreas@pidde.de)
 */

////////////////////////////////////////////////////////////////////////////////
//! Possible simple types
enum ETypeID {
	TYPEID_UNKNOWN = -1,//!< Unknown type
	TYPEID_INT     = 0, //!< Integer type
	TYPEID_FLOAT,       //!< Float type
	TYPEID_STRING       //!< String type
};
//! Number of simple types
const int N_TYPEID = (int)TYPEID_STRING+1;


////////////////////////////////////////////////////////////////////////////////
//!possible Types
//!{
enum EType {
	TYPE_UNKNOWN       = 0, //!< Unknown type
	TYPE_BOOLEAN,			//!< Boolean
	TYPE_INTEGER,			//!< Integer
	TYPE_FLOAT,				//!< Float
	TYPE_TOKEN,				//!< Token
	TYPE_COLOR,				//!< Colour
	TYPE_POINT,				//!< Point (three floats)
	TYPE_VECTOR,			//!< Vector (three floats)
	TYPE_NORMAL,			//!< Normal (three floats)
	TYPE_HPOINT,			//!< Homogene point (four floats)
	TYPE_MPOINT,			//!< Map point (16 floats)
	TYPE_MATRIX,			//!< Homogene matrix (16 floats)
	TYPE_BASIS,				//!< Base matrix (16 floats)
	TYPE_BOUND,				//!< Bounding box (6 floats)
	TYPE_STRING,			//!< String
	TYPE_POINTER,			//!< Arbitrary pointer
	TYPE_VOID,				//!< Void type
	TYPE_FILTERFUNC,		//!< Pointer to a filter function RtFilterFunc
	TYPE_ERRORHANDLER,		//!< Pointer to an errorhandler RtErrorHandler
	TYPE_PROCSUBDIVFUNC,	//!< Pointer to a subdivision procedure RtProcSubdivFunc
	TYPE_PROCFREEFUNC,		//!< Pointer to a cleanup function RtProcFreeFunc
	TYPE_ARCHIVECALLBACK,	//!< Pointer to an archive callback function RtArchiveCallback
	TYPE_OBJECTHANDLE,		//!< Handle of an object RtObjectHandle
	TYPE_LIGHTHANDLE,		//!< Handle of a light source RtLightSource
	TYPE_CONTEXTHANDLE,		//!< Handle of a renderer context RtContextHandle
	TYPE_INT				//!< Shortcut for integer
};
//!}

////////////////////////////////////////////////////////////////////////////////
//! Constants for numbers of types and classes
//!{
const int N_TYPESIZES  = (int)TYPE_INT+1; //!< number of possible typesizes
//!}

////////////////////////////////////////////////////////////////////////////////
//! Possible parameter storage classes

//!{
enum EClass {
	CLASS_UNKNOWN	 = 0, //!< Unknown class
	CLASS_CONSTANT,	      //!< Constant class (one value)
	CLASS_UNIFORM,	      //!< Constant class (one value per face, shared corners count one)
	CLASS_VARYING,	      //!< Varying class (one value per corner, shared vertex count one)
	CLASS_VERTEX,	      //!< Vertex class (one value per vertex)
	CLASS_FACEVARYING,	  //!< Vertex class (one value per corner of each face)
	CLASS_FACEVERTEX	  //!< Face Vertex class (one value per vertex of each face)
};
//!}

const int N_CLASSNAMES = (int)CLASS_FACEVERTEX+1;      //!< number of possible classes


////////////////////////////////////////////////////////////////////////////////
//! possible contexts, i.e. blocks opend by the renderer
/*! e.g.:  CTX_BEGIN for RiBegin()/RiEnd()
 * (outside: no block is open)
 */
//!{

// Indices
enum EContexts {
	CTX_OUTSIDE	   = 0, //!< Index outside Begin/End

	CTX_BEGIN,		    //!< Index directly inside Begin/End
	CTX_FRAME,		    //!< Index directly inside frame block
	CTX_WORLD,		    //!< Index directly inside world block

	CTX_ATTRIBUTE,	    //!< Index directly inside attribute block
	CTX_TRANSFORM,	    //!< Index directly inside transformation block
	CTX_SOLID,		    //!< Index directly inside solid block
	CTX_MOTION,		    //!< Index directly inside motion block

	CTX_OBJECT		    //!< Index directly inside object block
};

//! Number of contexts
const int N_CONTEXTS = (int)CTX_OBJECT+1;

// Bits
enum EContextsBits {
	CTX_BIT_OUTSIDE   =   1, //!< Bit for outside Begin/End

	CTX_BIT_BEGIN     =   2, //!< Bit for directly inside Begin/End
	CTX_BIT_FRAME     =   4, //!< Bit for directly inside frame block
	CTX_BIT_WORLD     =   8, //!< Bit for directly inside world block

	CTX_BIT_ATTRIBUTE =  16, //!< Bit for directly inside attribute block
	CTX_BIT_TRANSFORM =  32, //!< Bit for directly inside transform block
	CTX_BIT_SOLID     =  64, //!< Bit for directly inside solid block
	CTX_BIT_MOTION    = 128, //!< Bit for directly inside motion block

	CTX_BIT_OBJECT    = 256  //!< Bit for directly inside object block
};
//!}

////////////////////////////////////////////////////////////////////////////////
//! Index for each interface function, used by TValidContect
//!{
enum ETokens {
	// General routines
	IDX_ERROR_HANDLER          = 0, //! Index for ri function ErrorHandler
	IDX_DECLARE,                    //! Index for ri function Declare

	// Begin-end block
	IDX_BEGIN,						//! Index for ri function Begin (no RIB binding)
	IDX_END,						//! Index for ri function End (no RIB binding)

	// Frame block
	IDX_FRAME_BEGIN,				//! Index for ri function FrameBegin
	IDX_FRAME_END,					//! Index for ri function FrameEnd

	// World block
	IDX_WORLD_BEGIN,				//! Index for ri function WorldBegin
	IDX_WORLD_END,					//! Index for ri function WorldEnd

	// Attribute block
	IDX_ATTRIBUTE_BEGIN,			//! Index for ri function AttributeBegin
	IDX_ATTRIBUTE_END,				//! Index for ri function AttributeEnd

	// Transformation block
	IDX_TRANSFORM_BEGIN,			//! Index for ri function TransformBegin
	IDX_TRANSFORM_END,				//! Index for ri function TransformEnd

	// Solid block
	IDX_SOLID_BEGIN,				//! Index for ri function SolidBegin
	IDX_SOLID_END,					//! Index for ri function SolidEnd

	// Object block
	IDX_OBJECT_BEGIN,				//! Index for ri function ObjectBegin
	IDX_OBJECT_END,					//! Index for ri function ObjectEnd

	// Motion block
	IDX_MOTION_BEGIN,				//! Index for ri function MotionBegin
	IDX_MOTION_END,					//! Index for ri function MotionEnd

	// Options
	IDX_FORMAT,						//! Index for ri function Format
	IDX_FRAME_ASPECT_RATIO,			//! Index for ri function FameAspectRatio
	IDX_SCREEN_WINDOW,				//! Index for ri function ScreenWindow
	IDX_CROP_WINDOW,				//! Index for ri function CropWindow
	IDX_PROJECTION,					//! Index for ri function Projection
	IDX_CLIPPING,					//! Index for ri function Clipping
	IDX_CLIPPING_PLANE,				//! Index for ri function ClippingPlane
	IDX_DEPTH_OF_FIELD,				//! Index for ri function DepthOfField
	IDX_SHUTTER,					//! Index for ri function Shutter
	IDX_PIXEL_VARIANCE,				//! Index for ri function PixelVariance
	IDX_PIXEL_SAMPLES,				//! Index for ri function PixelSamples
	IDX_PIXEL_FILTER,				//! Index for ri function PixelFilter
	IDX_EXPOSURE,					//! Index for ri function Exposure
	IDX_IMAGER,						//! Index for ri function Imager
	IDX_QUANTIZE,					//! Index for ri function Quantize
	IDX_DISPLAY,					//! Index for ri function Display
	IDX_HIDER,						//! Index for ri function Hider
	IDX_COLOR_SAMPLES,				//! Index for ri function ColorSamples
	IDX_RELATIVE_DETAIL,			//! Index for ri function RelativeDetail
	IDX_OPTION,						//! Index for ri function Option

	// Lights
	IDX_LIGHT_SOURCE,				//! Index for ri function LightSource
	IDX_AREA_LIGHT_SOURCE,			//! Index for ri function AreaLightSource

	// Attributes
	IDX_ATTRIBUTE,					//! Index for ri function Attribute
	IDX_COLOR,						//! Index for ri function Color
	IDX_OPACITY,					//! Index for ri function Opacity
	IDX_SURFACE,					//! Index for ri function Surface
	IDX_ATMOSPHERE,					//! Index for ri function Atmosphere
	IDX_INTERIOR,					//! Index for ri function Interior
	IDX_EXTERIOR,					//! Index for ri function Exterior
	IDX_ILLUMINATE,					//! Index for ri function Illuminate
	IDX_DISPLACEMENT,				//! Index for ri function Displacement
	IDX_TEXTURE_COORDINATES,		//! Index for ri function TextureCoordinates
	IDX_SHADING_RATE,				//! Index for ri function ShadingRate
	IDX_SHADING_INTERPOLATION,		//! Index for ri function ShadingInterpolation
	IDX_MATTE,						//! Index for ri function Matte
	IDX_BOUND,						//! Index for ri function Bound
	IDX_DETAIL,						//! Index for ri function Detail
	IDX_DETAIL_RANGE,				//! Index for ri function DetailRange
	IDX_GEOMETRIC_APPROXIMATION,	//! Index for ri function GeometrixApproximation
	IDX_GEOMETRIC_REPRESENTATION,	//! Index for ri function GeometricRepresentation
	IDX_ORIENTATION,				//! Index for ri function Orientation
	IDX_REVERSE_ORIENTATION,		//! Index for ri function ReverseOrientation
	IDX_SIDES,						//! Index for ri function Sides
	IDX_BASIS,						//! Index for ri function Basis
	IDX_TRIM_CURVE,					//! Index for ri function TrimCurve

	// Transformations
	IDX_IDENTITY,					//! Index for ri function Identity
	IDX_TRANSFORM,					//! Index for ri function Transform
	IDX_CONCAT_TRANSFORM,			//! Index for ri function ConcatTransform
	IDX_PERSPECTIVE,				//! Index for ri function Perspective
	IDX_TRANSLATE,					//! Index for ri function Translate
	IDX_ROTATE,						//! Index for ri function Rotate
	IDX_SCALE,						//! Index for ri function Scale
	IDX_SKEW,						//! Index for ri function Skew
	IDX_DEFORMATION,				//! Index for ri function Deformation
	IDX_COORDINATE_SYSTEM,			//! Index for ri function CoordinateSystem
	IDX_COORD_SYS_TRANSFORM,		//! Index for ri function CoordSysTransform
	IDX_TRANSFORM_POINTS,			//! Index for ri function TransformPoints (no RIB binding)

	// Polygons
	IDX_POLYGON,					//! Index for ri function Polygon
	IDX_GENERAL_POLYGON,			//! Index for ri function GeneralPolygon
	IDX_POINTS_POLYGONS,			//! Index for ri function PointsPolygon
	IDX_POINTS_GENERAL_POLYGONS,	//! Index for ri function PointsGeneralPolygon

	// Patches
	IDX_PATCH,						//! Index for ri function Patch
	IDX_PATCH_MESH,					//! Index for ri function PatchMesh
	IDX_NU_PATCH,					//! Index for ri function NUPatch

	// Subdivision Surfaces
	IDX_SUBDIVISION_MESH,			//! Index for ri function SubdivisionMesh

	// Quadrics
	IDX_SPHERE,						//! Index for ri function Sphere
	IDX_CONE,						//! Index for ri function Cone
	IDX_CYLINDER,					//! Index for ri function Cylinder
	IDX_HYPERBOLOID,				//! Index for ri function Hyperboloid
	IDX_PARABOLOID,					//! Index for ri function Paraboloid
	IDX_DISK,						//! Index for ri function Disk
	IDX_TORUS,						//! Index for ri function Torus

	// Point and curve primitives
	IDX_POINTS,						//! Index for ri function Points
	IDX_CURVES,						//! Index for ri function Curves

	// Blobby implicit surfaces
	IDX_BLOBBY,						//! Index for ri function Blobby

	// Procedural primitives
	IDX_PROCEDURAL,					//! Index for ri function Procedural
	IDX_PROC_DELAYED_READ_ARCHIVE,	//! Index for ri function ProcDelayedReadArchive
	IDX_PROC_RUN_PROGRAM,			//! Index for ri function ProcRunProgram
	IDX_PROC_DYNAMIC_LOAD,			//! Index for ri function ProcDynamicLoad

	// General objects
	IDX_GEOMETRY,					//! Index for ri function Geometry

	// retained geometry
	IDX_OBJECT_INSTANCE,			//! Index for ri function ObjectInstance

	// Map-making
	IDX_MAKE_TEXTURE,				//! Index for ri function MakeTexture
	IDX_MAKE_BUMP,					//! Index for ri function MakeBumb
	IDX_MAKE_LAT_LONG_ENVIRONMENT,	//! Index for ri function MakeLatLongEnvironment
	IDX_MAKE_CUBE_FACE_ENVIRONMENT,	//! Index for ri function MakeCubeFaceEnvironment
	IDX_MAKE_SHADOW,				//! Index for ri function MakeShadow

	// Archive files
	IDX_ARCHIVE_RECORD,				//! Index for ri function ArchiveRecord (no RIB binding)
	IDX_READ_ARCHIVE,				//! Index for ri function ReadArchive

	// Context Switching
	IDX_GET_CONTEXT,				//! Index for ri function GetContext (no RIB binding)
	IDX_CONTEXT,					//! Index for ri function Context (no RIB binding)

	// Additional
	IDX_VERSION                     //! Index for (additional) ri function version
};
//!}
//! Number of tokens = Number of RI-Routines
const int N_TOKENS = (int)IDX_VERSION + 1;

// Array length
//!{
//! max. size of diverse error messages (declarationmap.h/.cpp, ricppbinding.h/.cpp)
const int ERROR_STR_SIZE   = 256;

//! archiveBuf TRiCPPBinding::archiveRecord maximal size for structural comments (ricppbinding.h/.cpp)
const int ARCHIVE_BUF_SIZE = 256;
//!}

#endif // _RICPP_RIBPARSER_RICONSTANTS_H
