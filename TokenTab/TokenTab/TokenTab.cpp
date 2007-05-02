// TokenTab.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <deque>
#include <list>
#include <iostream>

#include "tab.cpp"

#define CHARCONVLEN 256
#define STATELEN 52
#define ERROR -8

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
	IDX_MAKE_LAT_int_ENVIRONMENT,	//! Index for ri function MakeLatintEnvironment
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

class State {
public:
	int state[STATELEN];
	State() {
		for (int i=0; i<STATELEN; ++i) state[i] = ERROR;
	}
	State(const State &astate) {
		*this = astate;
	}
	virtual ~State() {}
	State &operator=(const State &astate) {
		for (int i = 0; i < STATELEN; ++i) state[i] = astate.state[i];
		return *this;
	}
	int &operator[](int index) { return state[index]; }
};

class Final {
public:
	int state;
	int value;
	Final() {
		state=ERROR;
		value=0;
	}
	Final(const Final &afinal) {
		*this = afinal;
	}
	virtual ~Final() {}
	Final(int astate, int avalue) : state(astate), value(avalue) {}
	Final &operator=(const Final &aFinal) {
		state = aFinal.state;
		value = aFinal.value;
		return *this;
	}
	bool operator<(const Final &aFinal) const {
		return state < aFinal.state;
	}
};

class TokenTab {
public:
	int charconv[CHARCONVLEN];
	std::deque<State> statetab;
	std::list<Final> finaltab;

	int insert(int c, int state) {
		int &nstate = (statetab[state])[c];
		if ( nstate == ERROR ) {
			statetab.push_back(State());
			nstate = (int)statetab.size()-1;
		}
		return nstate;
	}
	void finalize() {
		finaltab.sort();
	}

	void InitCharConv() {
		for ( int i = 0; i < CHARCONVLEN; ++i ) charconv[i] = ERROR;
		for ( unsigned char c = 'a'; c <= 'z'; ++c ) charconv[c]=c-(unsigned char)'a';
		for ( unsigned char c = 'A'; c <= 'Z'; ++c ) charconv[c]=c-(unsigned char)'A'+(unsigned char)'z'-(unsigned char)'a'+1;
	}

	TokenTab() {
		InitCharConv();
		statetab.clear();
		statetab.push_back(State());
		finaltab.clear();
	}
	int add (int avalue, const char *atoken) {
		const unsigned char *ptr = (const unsigned char *)atoken;
		if ( !ptr || !*ptr )
			return ERROR;

		int statenum = 0;
		while ( *ptr ) {
			unsigned c = charconv[*ptr];
			if ( c == ERROR )
				return ERROR;
			statenum = insert(c, statenum);
			++ptr;
		}
		finaltab.push_back(Final(statenum, avalue));
		return statenum;
	}
	int findToken(const char *token) const {
		const char *ptr = token;
		int retval = ERROR;
		int state = 0;
		int c = 0;
		if ( !ptr )
			return retval;
		for ( ; *ptr; ++ptr ) {
			c = charconv[*ptr];
			if ( c == ERROR )
				return ERROR;
			state = ::statetab[state][c];
			if ( state == ERROR )
				return ERROR;
		}
		return ::finaltab[state];
	}

	int printToken(std::ostream &os, int needVal, const char *token) const {
		int val = findToken(token);
		if ( val == needVal ) {
			os << token << " == " << val << std::endl;
		} else if ( val == ERROR ) {
			os << "// " << token << " NOT FOUND " << std::endl;
		} else {
			os << "// " << token << " WRONG VALUE " << val << "(should be " << needVal << ")" << std::endl;
		}
		return val;
	}
};

std::ostream &operator<<(std::ostream &os, TokenTab &tt) {
	tt.finalize();
	std::deque<State>::iterator sit = tt.statetab.begin();
	int cnt;
	int tabsize = (int)tt.statetab.size();

	os << "int statetab[" << tabsize << "][" << STATELEN << "] = {" << std::endl;
	for ( ; sit != tt.statetab.end(); sit++ ) {
		if ( sit != tt.statetab.begin() ) {
			os << "," << std::endl;
		}
		os << "{ ";
		for ( cnt = 0; cnt < STATELEN; ++cnt ) {
			if ( cnt != 0 ) {
				os << ", ";
			}
			os << sit->state[cnt];
		}
		os << "} ";
	}
	os << "};" << std::endl;

	std::list<Final>::iterator fit = tt.finaltab.begin();

	os << "int finaltab[" << tabsize << "] = {" << std::endl;
	cnt = 0;
	for ( ; fit != tt.finaltab.end(); fit++, ++cnt ) {
		for ( ; cnt != fit->state; ++cnt ) {
			if ( cnt != 0 ) {
				os << ", ";
			}
			os << ERROR;
		}
		if ( cnt != 0 ) {
			os << ", ";
		}
		os << fit->value;
	}
	for ( ; cnt != tabsize; ++cnt ) {
		if ( cnt != 0 ) {
			os << ", ";
		}
		os << ERROR;
	}
	os << "};" << std::endl;

	return os;
}


void buildTokenTab() {
	TokenTab tokenTab;

	tokenTab.add(IDX_ERROR_HANDLER, "ErrorHandler");
	tokenTab.add(IDX_DECLARE, "Declare");
	tokenTab.add(IDX_BEGIN, "Begin");
	tokenTab.add(IDX_END, "End");
	tokenTab.add(IDX_FRAME_BEGIN, "FrameBegin");
	tokenTab.add(IDX_FRAME_END, "FrameEnd");
	tokenTab.add(IDX_WORLD_BEGIN, "WorldBegin");
	tokenTab.add(IDX_WORLD_END, "WorldEnd");
	tokenTab.add(IDX_ATTRIBUTE_BEGIN, "AttributeBegin");
	tokenTab.add(IDX_ATTRIBUTE_END, "AttributeEnd");
	tokenTab.add(IDX_TRANSFORM_BEGIN, "TransformBegin");
	tokenTab.add(IDX_TRANSFORM_END, "TransformEnd");
	tokenTab.add(IDX_SOLID_BEGIN, "SolidBegin");
	tokenTab.add(IDX_SOLID_END, "SolidEnd");
	tokenTab.add(IDX_OBJECT_BEGIN, "ObjectBegin");
	tokenTab.add(IDX_OBJECT_END, "ObjectEnd");
	tokenTab.add(IDX_MOTION_BEGIN, "MotionBegin");
	tokenTab.add(IDX_MOTION_END, "MotionEnd");
	tokenTab.add(IDX_FORMAT, "Format");
	tokenTab.add(IDX_FRAME_ASPECT_RATIO, "FrameAspectRatio");
	tokenTab.add(IDX_SCREEN_WINDOW, "ScreenWindow");
	tokenTab.add(IDX_CROP_WINDOW, "CropWindow");
	tokenTab.add(IDX_PROJECTION, "Projection");
	tokenTab.add(IDX_CLIPPING, "Clipping");
	tokenTab.add(IDX_CLIPPING_PLANE, "ClippingPlane");
	tokenTab.add(IDX_DEPTH_OF_FIELD, "DepthOfField");
	tokenTab.add(IDX_SHUTTER, "Shutter");
	tokenTab.add(IDX_PIXEL_VARIANCE, "PixelVariance");
	tokenTab.add(IDX_PIXEL_SAMPLES, "PixelSamples");
	tokenTab.add(IDX_PIXEL_FILTER, "PixelFilter");
	tokenTab.add(IDX_EXPOSURE, "Exposure");
	tokenTab.add(IDX_IMAGER, "Imager");
	tokenTab.add(IDX_QUANTIZE, "Quantize");
	tokenTab.add(IDX_DISPLAY, "Display");
	tokenTab.add(IDX_HIDER, "Hider");
	tokenTab.add(IDX_COLOR_SAMPLES, "ColorSamples");
	tokenTab.add(IDX_RELATIVE_DETAIL, "RelativeDetail");
	tokenTab.add(IDX_OPTION, "Option");
	tokenTab.add(IDX_LIGHT_SOURCE, "LightSource");
	tokenTab.add(IDX_AREA_LIGHT_SOURCE, "AreaLightSource");
	tokenTab.add(IDX_ATTRIBUTE, "Attribute");
	tokenTab.add(IDX_COLOR, "Color");
	tokenTab.add(IDX_OPACITY, "Opacity");
	tokenTab.add(IDX_SURFACE, "Surface");
	tokenTab.add(IDX_ATMOSPHERE, "Atosphere");
	tokenTab.add(IDX_INTERIOR, "Interior");
	tokenTab.add(IDX_EXTERIOR, "Exterior");
	tokenTab.add(IDX_ILLUMINATE, "Illuminate");
	tokenTab.add(IDX_DISPLACEMENT, "Displacement");
	tokenTab.add(IDX_TEXTURE_COORDINATES, "TextureCoordinates");
	tokenTab.add(IDX_SHADING_RATE, "ShadingRate");
	tokenTab.add(IDX_SHADING_INTERPOLATION, "ShadingInterpolation");
	tokenTab.add(IDX_MATTE, "Matte");
	tokenTab.add(IDX_BOUND, "Bound");
	tokenTab.add(IDX_DETAIL, "Detail");
	tokenTab.add(IDX_DETAIL_RANGE, "DetailRange");
	tokenTab.add(IDX_GEOMETRIC_APPROXIMATION, "GeometricApproximation");
	tokenTab.add(IDX_GEOMETRIC_REPRESENTATION, "GeometricRepresentation");
	tokenTab.add(IDX_ORIENTATION, "Orientation");
	tokenTab.add(IDX_REVERSE_ORIENTATION, "ReverseOrientation");
	tokenTab.add(IDX_SIDES, "Sides");
	tokenTab.add(IDX_BASIS, "Basis");
	tokenTab.add(IDX_TRIM_CURVE, "TrimCurve");
	tokenTab.add(IDX_IDENTITY, "Identity");
	tokenTab.add(IDX_TRANSFORM, "Transform");
	tokenTab.add(IDX_CONCAT_TRANSFORM, "ConcatTransform");
	tokenTab.add(IDX_PERSPECTIVE, "Perspective");
	tokenTab.add(IDX_TRANSLATE, "Translate");
	tokenTab.add(IDX_ROTATE, "Rotate");
	tokenTab.add(IDX_SCALE, "Scale");
	tokenTab.add(IDX_SKEW, "Skew");
	tokenTab.add(IDX_DEFORMATION, "Deformation");
	tokenTab.add(IDX_COORDINATE_SYSTEM, "CoordinateSystem");
	tokenTab.add(IDX_COORD_SYS_TRANSFORM, "CoordSysTransform");
	tokenTab.add(IDX_TRANSFORM_POINTS, "TransformPoints");
	tokenTab.add(IDX_POLYGON, "Polygon");
	tokenTab.add(IDX_GENERAL_POLYGON, "GeneralPolygon");
	tokenTab.add(IDX_POINTS_POLYGONS, "PointsPolygons");
	tokenTab.add(IDX_POINTS_GENERAL_POLYGONS, "PointsGeneralPolygons");
	tokenTab.add(IDX_PATCH, "Patch");
	tokenTab.add(IDX_PATCH_MESH, "PatchMesh");
	tokenTab.add(IDX_NU_PATCH, "NuPatch");
	tokenTab.add(IDX_SUBDIVISION_MESH, "SubdivisionMesh");
	tokenTab.add(IDX_SPHERE, "Sphere");
	tokenTab.add(IDX_CONE, "Cone");
	tokenTab.add(IDX_CYLINDER, "Cylinder");
	tokenTab.add(IDX_HYPERBOLOID, "Hyperboloid");
	tokenTab.add(IDX_PARABOLOID, "Paraboloid");
	tokenTab.add(IDX_DISK, "Disk");
	tokenTab.add(IDX_TORUS, "Torus");
	tokenTab.add(IDX_POINTS, "Points");
	tokenTab.add(IDX_CURVES, "Curves");
	tokenTab.add(IDX_BLOBBY, "Blobby");
	tokenTab.add(IDX_PROCEDURAL, "Procedural");
	tokenTab.add(IDX_PROC_DELAYED_READ_ARCHIVE, "ProcDelayedReadArchive");
	tokenTab.add(IDX_PROC_RUN_PROGRAM, "ProcRunProgram");
	tokenTab.add(IDX_PROC_DYNAMIC_LOAD, "ProcDynamicLoad");
	tokenTab.add(IDX_GEOMETRY, "Geometry");
	tokenTab.add(IDX_OBJECT_INSTANCE, "ObjectInstance");
	tokenTab.add(IDX_MAKE_TEXTURE, "MakeTexture");
	tokenTab.add(IDX_MAKE_BUMP, "MakeBump");
	tokenTab.add(IDX_MAKE_LAT_int_ENVIRONMENT, "MakeLatintEnvironment");
	tokenTab.add(IDX_MAKE_CUBE_FACE_ENVIRONMENT, "MakeCubeFaceEnvironment");
	tokenTab.add(IDX_MAKE_SHADOW, "MakeShadow");
	tokenTab.add(IDX_ARCHIVE_RECORD, "ArchiveRecord");
	tokenTab.add(IDX_READ_ARCHIVE, "ReadArchive");
	tokenTab.add(IDX_GET_CONTEXT, "GetContext");
	tokenTab.add(IDX_CONTEXT, "Context");
	tokenTab.add(IDX_VERSION, "version");
	std::cout << tokenTab;
}

void printTokenTab() {
	TokenTab tokenTab;
	std::cout << "/*" << std::endl;
//	tokenTab.printToken(std::cout, 4711, "Begin");
//	tokenTab.printToken(std::cout, 4712, "GibtEsNicht");
	tokenTab.printToken(std::cout, IDX_ERROR_HANDLER, "ErrorHandler");
	tokenTab.printToken(std::cout, IDX_DECLARE, "Declare");
	tokenTab.printToken(std::cout, IDX_BEGIN, "Begin");
	tokenTab.printToken(std::cout, IDX_END, "End");
	tokenTab.printToken(std::cout, IDX_FRAME_BEGIN, "FrameBegin");
	tokenTab.printToken(std::cout, IDX_FRAME_END, "FrameEnd");
	tokenTab.printToken(std::cout, IDX_WORLD_BEGIN, "WorldBegin");
	tokenTab.printToken(std::cout, IDX_WORLD_END, "WorldEnd");
	tokenTab.printToken(std::cout, IDX_ATTRIBUTE_BEGIN, "AttributeBegin");
	tokenTab.printToken(std::cout, IDX_ATTRIBUTE_END, "AttributeEnd");
	tokenTab.printToken(std::cout, IDX_TRANSFORM_BEGIN, "TransformBegin");
	tokenTab.printToken(std::cout, IDX_TRANSFORM_END, "TransformEnd");
	tokenTab.printToken(std::cout, IDX_SOLID_BEGIN, "SolidBegin");
	tokenTab.printToken(std::cout, IDX_SOLID_END, "SolidEnd");
	tokenTab.printToken(std::cout, IDX_OBJECT_BEGIN, "ObjectBegin");
	tokenTab.printToken(std::cout, IDX_OBJECT_END, "ObjectEnd");
	tokenTab.printToken(std::cout, IDX_MOTION_BEGIN, "MotionBegin");
	tokenTab.printToken(std::cout, IDX_MOTION_END, "MotionEnd");
	tokenTab.printToken(std::cout, IDX_FORMAT, "Format");
	tokenTab.printToken(std::cout, IDX_FRAME_ASPECT_RATIO, "FrameAspectRatio");
	tokenTab.printToken(std::cout, IDX_SCREEN_WINDOW, "ScreenWindow");
	tokenTab.printToken(std::cout, IDX_CROP_WINDOW, "CropWindow");
	tokenTab.printToken(std::cout, IDX_PROJECTION, "Projection");
	tokenTab.printToken(std::cout, IDX_CLIPPING, "Clipping");
	tokenTab.printToken(std::cout, IDX_CLIPPING_PLANE, "ClippingPlane");
	tokenTab.printToken(std::cout, IDX_DEPTH_OF_FIELD, "DepthOfField");
	tokenTab.printToken(std::cout, IDX_SHUTTER, "Shutter");
	tokenTab.printToken(std::cout, IDX_PIXEL_VARIANCE, "PixelVariance");
	tokenTab.printToken(std::cout, IDX_PIXEL_SAMPLES, "PixelSamples");
	tokenTab.printToken(std::cout, IDX_PIXEL_FILTER, "PixelFilter");
	tokenTab.printToken(std::cout, IDX_EXPOSURE, "Exposure");
	tokenTab.printToken(std::cout, IDX_IMAGER, "Imager");
	tokenTab.printToken(std::cout, IDX_QUANTIZE, "Quantize");
	tokenTab.printToken(std::cout, IDX_DISPLAY, "Display");
	tokenTab.printToken(std::cout, IDX_HIDER, "Hider");
	tokenTab.printToken(std::cout, IDX_COLOR_SAMPLES, "ColorSamples");
	tokenTab.printToken(std::cout, IDX_RELATIVE_DETAIL, "RelativeDetail");
	tokenTab.printToken(std::cout, IDX_OPTION, "Option");
	tokenTab.printToken(std::cout, IDX_LIGHT_SOURCE, "LightSource");
	tokenTab.printToken(std::cout, IDX_AREA_LIGHT_SOURCE, "AreaLightSource");
	tokenTab.printToken(std::cout, IDX_ATTRIBUTE, "Attribute");
	tokenTab.printToken(std::cout, IDX_COLOR, "Color");
	tokenTab.printToken(std::cout, IDX_OPACITY, "Opacity");
	tokenTab.printToken(std::cout, IDX_SURFACE, "Surface");
	tokenTab.printToken(std::cout, IDX_ATMOSPHERE, "Atosphere");
	tokenTab.printToken(std::cout, IDX_INTERIOR, "Interior");
	tokenTab.printToken(std::cout, IDX_EXTERIOR, "Exterior");
	tokenTab.printToken(std::cout, IDX_ILLUMINATE, "Illuminate");
	tokenTab.printToken(std::cout, IDX_DISPLACEMENT, "Displacement");
	tokenTab.printToken(std::cout, IDX_TEXTURE_COORDINATES, "TextureCoordinates");
	tokenTab.printToken(std::cout, IDX_SHADING_RATE, "ShadingRate");
	tokenTab.printToken(std::cout, IDX_SHADING_INTERPOLATION, "ShadingInterpolation");
	tokenTab.printToken(std::cout, IDX_MATTE, "Matte");
	tokenTab.printToken(std::cout, IDX_BOUND, "Bound");
	tokenTab.printToken(std::cout, IDX_DETAIL, "Detail");
	tokenTab.printToken(std::cout, IDX_DETAIL_RANGE, "DetailRange");
	tokenTab.printToken(std::cout, IDX_GEOMETRIC_APPROXIMATION, "GeometricApproximation");
	tokenTab.printToken(std::cout, IDX_GEOMETRIC_REPRESENTATION, "GeometricRepresentation");
	tokenTab.printToken(std::cout, IDX_ORIENTATION, "Orientation");
	tokenTab.printToken(std::cout, IDX_REVERSE_ORIENTATION, "ReverseOrientation");
	tokenTab.printToken(std::cout, IDX_SIDES, "Sides");
	tokenTab.printToken(std::cout, IDX_BASIS, "Basis");
	tokenTab.printToken(std::cout, IDX_TRIM_CURVE, "TrimCurve");
	tokenTab.printToken(std::cout, IDX_IDENTITY, "Identity");
	tokenTab.printToken(std::cout, IDX_TRANSFORM, "Transform");
	tokenTab.printToken(std::cout, IDX_CONCAT_TRANSFORM, "ConcatTransform");
	tokenTab.printToken(std::cout, IDX_PERSPECTIVE, "Perspective");
	tokenTab.printToken(std::cout, IDX_TRANSLATE, "Translate");
	tokenTab.printToken(std::cout, IDX_ROTATE, "Rotate");
	tokenTab.printToken(std::cout, IDX_SCALE, "Scale");
	tokenTab.printToken(std::cout, IDX_SKEW, "Skew");
	tokenTab.printToken(std::cout, IDX_DEFORMATION, "Deformation");
	tokenTab.printToken(std::cout, IDX_COORDINATE_SYSTEM, "CoordinateSystem");
	tokenTab.printToken(std::cout, IDX_COORD_SYS_TRANSFORM, "CoordSysTransform");
	tokenTab.printToken(std::cout, IDX_TRANSFORM_POINTS, "TransformPoints");
	tokenTab.printToken(std::cout, IDX_POLYGON, "Polygon");
	tokenTab.printToken(std::cout, IDX_GENERAL_POLYGON, "GeneralPolygon");
	tokenTab.printToken(std::cout, IDX_POINTS_POLYGONS, "PointsPolygons");
	tokenTab.printToken(std::cout, IDX_POINTS_GENERAL_POLYGONS, "PointsGeneralPolygons");
	tokenTab.printToken(std::cout, IDX_PATCH, "Patch");
	tokenTab.printToken(std::cout, IDX_PATCH_MESH, "PatchMesh");
	tokenTab.printToken(std::cout, IDX_NU_PATCH, "NuPatch");
	tokenTab.printToken(std::cout, IDX_SUBDIVISION_MESH, "SubdivisionMesh");
	tokenTab.printToken(std::cout, IDX_SPHERE, "Sphere");
	tokenTab.printToken(std::cout, IDX_CONE, "Cone");
	tokenTab.printToken(std::cout, IDX_CYLINDER, "Cylinder");
	tokenTab.printToken(std::cout, IDX_HYPERBOLOID, "Hyperboloid");
	tokenTab.printToken(std::cout, IDX_PARABOLOID, "Paraboloid");
	tokenTab.printToken(std::cout, IDX_DISK, "Disk");
	tokenTab.printToken(std::cout, IDX_TORUS, "Torus");
	tokenTab.printToken(std::cout, IDX_POINTS, "Points");
	tokenTab.printToken(std::cout, IDX_CURVES, "Curves");
	tokenTab.printToken(std::cout, IDX_BLOBBY, "Blobby");
	tokenTab.printToken(std::cout, IDX_PROCEDURAL, "Procedural");
	tokenTab.printToken(std::cout, IDX_PROC_DELAYED_READ_ARCHIVE, "ProcDelayedReadArchive");
	tokenTab.printToken(std::cout, IDX_PROC_RUN_PROGRAM, "ProcRunProgram");
	tokenTab.printToken(std::cout, IDX_PROC_DYNAMIC_LOAD, "ProcDynamicLoad");
	tokenTab.printToken(std::cout, IDX_GEOMETRY, "Geometry");
	tokenTab.printToken(std::cout, IDX_OBJECT_INSTANCE, "ObjectInstance");
	tokenTab.printToken(std::cout, IDX_MAKE_TEXTURE, "MakeTexture");
	tokenTab.printToken(std::cout, IDX_MAKE_BUMP, "MakeBump");
	tokenTab.printToken(std::cout, IDX_MAKE_LAT_int_ENVIRONMENT, "MakeLatintEnvironment");
	tokenTab.printToken(std::cout, IDX_MAKE_CUBE_FACE_ENVIRONMENT, "MakeCubeFaceEnvironment");
	tokenTab.printToken(std::cout, IDX_MAKE_SHADOW, "MakeShadow");
	tokenTab.printToken(std::cout, IDX_ARCHIVE_RECORD, "ArchiveRecord");
	tokenTab.printToken(std::cout, IDX_READ_ARCHIVE, "ReadArchive");
	tokenTab.printToken(std::cout, IDX_GET_CONTEXT, "GetContext");
	tokenTab.printToken(std::cout, IDX_CONTEXT, "Context");
	tokenTab.printToken(std::cout, IDX_VERSION, "version");
	std::cout << "*/" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	buildTokenTab();
	printTokenTab();
	return 0;
}
