#ifndef _RICPP_RICPP_RICPP_H
#define _RICPP_RICPP_RICPP_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright � of RiCPP 2007, Andreas Pidde
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

/** @file ricpp.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Abstract interface for the RenderMan(R) C++ Binding
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 *
 * ricpp.h means the same to RiCPP as ri.h to the standard C binding.
 * For an interface description, you can use
 * @anchor [ARM2000] Apodaca, Gritz et all; Advanced RenderMan; 2000, Morgan Kaufman
 * @anchor [RISPEC3.2] Pixar; The RenderMan Interface Version 3.2.1; Nov 2005, PDF document
 * @anchor [STE2003] Ian Stephenson; Essential RenderMan fast; 2003, Springer
 * @anchor [UPS89] Steve Upstill; The RenderMan Companion; 1989, Addison Wesley
 * @anchor [RAG2005] Saty Raghavachary; Rendering for Beginners; 2005, Elsevier
 * @anchor [STE] Production Rendering; 2005, Springer (another OO-framework)
 */

#include <limits>

/** All of the interface is placed within the RiCPP namespace
 */
namespace RiCPP {

/** Forward declaration of the RI (RenderMan Interface)
 */
class IRi;

//@{
/** Some basic types are copied from ri.h (s.a. \ref [RISPEC3.2], Appendix C)
 */
typedef short   RtBoolean; ///< Booleans can be RI_TRUE or RI_FALSE
typedef int     RtInt;     ///< Integers
typedef float   RtFloat;   ///< Floats

typedef const char *RtToken;     ///< Tokens, changed from 'char *' to 'const char *', Tokens are either pre-declared or declared with IRi::declare()

typedef RtFloat *RtColor;        ///< Colors, changed from 'RtFloat[3]' to 'RtFloat *' to manage color using other than 3 components
typedef RtFloat  RtPoint[3];     ///< Points in 3D
typedef RtFloat  RtVector[3];    ///< Vectors in 3D
typedef RtFloat  RtNormal[3];    ///< Normals in 3D
typedef RtFloat  RtHpoint[4];    ///< Points in homgenous 3D
typedef RtFloat  RtMpoint[4][4]; ///< 'Map Points' introduced in App Note #31 (RiBlobby)
typedef RtFloat  RtMatrix[4][4]; ///< Homogenous matrix for 3D transformations
typedef RtFloat  RtBasis[4][4];  ///< Base matrix for spline interpolations
typedef RtFloat  RtBound[6];     ///< Bound of 3D object: Left, right, bottom, top, front, back
typedef const char *RtString;    ///< Character string, which is not a predefined token, changed from 'char *' to 'const char *'

typedef void   *RtPointer;       ///< Pointer to arbitrary data
#define RtVoid  void             ///< Simple 'void' datatype, used as 'return type' for functions returning nothing (C++: typedef is not working for void)

typedef RtPointer RtObjectHandle;   ///< Handle for a object instance
typedef RtPointer RtLightHandle;    ///< Handle for a light handle
typedef RtPointer RtContextHandle;  ///< handle for a render context handle
// typedef RtString  RtArchiveHandle;  // handle for a rib archive (3Delight)
//@}

/** @brief Interface for the error handler (was RtErrorHandler)
 */
class IErrorHandler {
public:
	virtual const char *name() const = 0;
	virtual RtVoid operator()(RtInt code, RtInt severity, RtString msg) const = 0;
};

/** @brief Interface for a pixel filter
 */
class IFilterFunc {
public:
	virtual const char *name() const = 0;
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const = 0;
};

/** @brief Interface, subdivision for procedurals changed to include renderer instance
 */
class ISubdivFunc {
public:
	/** Name of the subdivision function
	 */
	virtual const char *name() const = 0;
	/** The subdivision function as operator()()
	 */
	virtual RtVoid operator()(IRi &, RtPointer, RtFloat) const = 0;
};

/** @brief Free function for procedurals
 */
class IFreeFunc {
public:
	/** Name of the free function
	 */
	virtual const char *name() const = 0;
	/** The free function as operator()()
	 */
	virtual RtVoid operator()(IRi &, RtPointer) const = 0;
};

/** @brief Callback function to handle structural comments in rib files (IRi::readArchive), changed to include renderer instance and to const char
 */
class IArchiveCallback {
public:
	/** Name of the callback function
	 */
	virtual const char *name() const = 0;
	/** The callback function as operator()()
	 */
	virtual RtVoid operator()(IRi &, RtToken, RtString, ...) const = 0;
};

//@{
/** @brief Boolean values
 */
const RtBoolean RI_FALSE       = 0;           ///< false
const RtBoolean RI_TRUE        = (!RI_FALSE); ///< true
//@}

//@{
/** Limits
 */
const RtFloat RI_INFINITY      = std::numeric_limits<RtFloat>::max();     ///< Maximum value for RtFloat, was 1.0e38
const RtFloat RI_EPSILON       = std::numeric_limits<RtFloat>::epsilon(); ///< Minimum value for v+riEpsilon != v, was 1.0e-10
//@}

//@{
/** Step sizes (correspondence to the basis matrices of the bicubic patches)
 */
const RtInt RI_BEZIERSTEP      = 3; ///< Stepsize for control points of bezier meshes
const RtInt RI_BSPLINESTEP     = 1; ///< Stepsize for control points of b-spline meshes
const RtInt RI_CATMULLROMSTEP  = 1; ///< Stepsize for control points of catmull-rom spline meshes
const RtInt RI_HERMITESTEP     = 2; ///< Stepsize for control points of hermite spline meshes
const RtInt RI_POWERSTEP       = 4; ///< Stepsize for control points of power matrix spline meshes
//@}

//@{
/** Basis matrices for bicubic patches
 */
const RtBasis RiBezierBasis = {
	{(RtFloat)-1.0, (RtFloat) 3.0, (RtFloat)-3.0, (RtFloat) 1.0},
	{(RtFloat) 3.0, (RtFloat)-6.0, (RtFloat) 3.0, (RtFloat) 0.0},
	{(RtFloat)-3.0, (RtFloat) 3.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
}; ///< Bezier patch
const RtBasis RiBSplineBasis = {
	{(RtFloat)(-1.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat)(-3.0/6.0), (RtFloat)(1.0/6.0)},
	{(RtFloat)( 3.0/6.0), (RtFloat)(-6.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)(-3.0/6.0), (RtFloat)  0.0,      (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)( 1.0/6.0), (RtFloat)( 4.0/6.0), (RtFloat)( 1.0/6.0), (RtFloat) 0.0}
}; ///< B-spline patch
const RtBasis RiBCatmullRomBasis = {
	{(RtFloat)(-1.0/2.0), (RtFloat)( 3.0/2.0), (RtFloat)(-3.0/2.0), (RtFloat)( 1.0/2.0)},
	{(RtFloat)( 2.0/2.0), (RtFloat)(-5.0/2.0), (RtFloat)( 4.0/2.0), (RtFloat)(-1.0/2.0)},
	{(RtFloat)(-1.0/2.0), (RtFloat)  0.0,      (RtFloat)( 1.0/2.0), (RtFloat)  0.0},
	{(RtFloat)  0.0,      (RtFloat)( 2.0/2.0), (RtFloat)  0.0,      (RtFloat)  0.0}
}; ///< Catmull-Rom patch
const RtBasis RiHermiteBasis = {
	{(RtFloat) 2.0, (RtFloat) 1.0, (RtFloat)-2.0, (RtFloat) 1.0},
	{(RtFloat)-3.0, (RtFloat)-2.0, (RtFloat) 3.0, (RtFloat)-1.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
}; ///< Hermite patch
const RtBasis RiPowerBasis = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
}; ///< Power-basis patch
//@}

//@{
/** Added constants for IRi::blobby(): Primitives always followed by an index to the (float) operands
 */
const RtInt RI_BLOBBY_CONSTANT        = 1000; ///< 1 index to  1 float
const RtInt RI_BLOBBY_ELLIPSOID       = 1001; ///< 1 index to 16 floats
const RtInt RI_BLOBBY_SEGMENT_BLOB    = 1002; ///< 1 index to 23 floats
const RtInt RI_BLOBBY_REPELLING_PLANE = 1003; ///< 2 Indices, first for 1 string param, 2nd for 4 floats
//@}


//@{
/** Added constants for IRi::blobby() Operations, Substract (4) and Divide (5) are exchanged - seems to be a bug in the [RISPEC]
 */
const RtInt RI_BLOBBY_OP_ADD = 0; ///< Addition:  count, ...
const RtInt RI_BLOBBY_OP_MUL = 1; ///< Multiply:  count, ...
const RtInt RI_BLOBBY_OP_MAX = 2; ///< Maximum:   count, ...
const RtInt RI_BLOBBY_OP_MIN = 3; ///< Minimum:   count, ...
const RtInt RI_BLOBBY_OP_SUB = 4; ///< Substract: subtrahend, minuend
const RtInt RI_BLOBBY_OP_DIV = 5; ///< Divide:    dividend, divisor
const RtInt RI_BLOBBY_OP_NEG = 6; ///< Negate:    negand
const RtInt RI_BLOBBY_OP_IDP = 7; ///< Identity:  idempotentate
//@}

//@{
/** Error codes
 *  -  1-10
 *    -# System and File errors
 *  - 11-20
 *    -# Program Limitations
 *  - 21-40
 *    -# State Errors
 *  - 41-60
 *    -# Parameter and Protocol Errors
 *  - 61-80
 *    -# Execution Errors
 */
const RtInt RIE_NOERROR        =  0;       ///< No error occured

const RtInt RIE_NOMEM          =  1;       ///< Out of memory
const RtInt RIE_SYSTEM         =  2;       ///< Miscellaneous system error
const RtInt RIE_NOFILE         =  3;       ///< File not exists
const RtInt RIE_BADFILE        =  4;       ///< Bad file format
const RtInt RIE_VERSION        =  5;       ///< File version mismatch
const RtInt RIE_DISKFULL       =  6;       ///< Target disk is full

const RtInt RIE_INCAPABLE      = 11;       ///< Optional RI feature
const RtInt RIE_UNIMPLEMENT    = 12;       ///< Unimplemented feature
const RtInt RIE_LIMIT          = 13;       ///< Arbitrary program limit
const RtInt RIE_BUG            = 14;       ///< Probably a bug in renderer

const RtInt RIE_NOTSTARTED     = 23;       ///< 'RiBegin' not called
const RtInt RIE_NESTING        = 24;       ///< Bad begin-end nesting
const RtInt RIE_NOTOPTIONS     = 25;       ///< Invalid state for options
const RtInt RIE_NOTATTRIBS     = 26;       ///< Invalid state for attributes
const RtInt RIE_NOTPRIMS       = 27;       ///< Invalid state for primitives
const RtInt RIE_ILLSTATE       = 28;       ///< Other invalid state
const RtInt RIE_BADMOTION      = 29;       ///< Badly formed motion block
const RtInt RIE_BADSOLID       = 30;       ///< Badly formed solid block

const RtInt RIE_BADTOKEN       = 41;       ///< Invalid token for request
const RtInt RIE_RANGE          = 42;       ///< Parameter out of range
const RtInt RIE_CONSISTENCY    = 43;       ///< Parameters inconsistent
const RtInt RIE_BADHANDLE      = 44;       ///< Bad object/light/context handle
const RtInt RIE_NOSHADER       = 45;       ///< Can't load requested shader
const RtInt RIE_MISSINGDATA    = 46;       ///< Required parameters not provided
const RtInt RIE_SYNTAX         = 47;       ///< Declare type syntax error

const RtInt RIE_MATH           = 61;       ///< Zerodivide, noninvert matrix, etc.
//@}

//@{
/** Error severity levels (0-3)
 */
const RtInt RIE_INFO           =  0;       ///< Rendering stats & other info
const RtInt RIE_WARNING        =  1;       ///< Something seems wrong, maybe okay
const RtInt RIE_ERROR          =  2;       ///< Problem.  Results may be wrong
const RtInt RIE_SEVERE         =  3;       ///< So bad you should probably abort
//@}

//@{
/** Tokens
 */
const RtToken  RI_NULL = (RtToken)0L;
const RtToken  RI_FRAMEBUFFER = "framebuffer", RI_FILE = "file";
const RtToken  RI_RGB = "rgb", RI_RGBA = "rgba", RI_RGBZ = "rgbz", RI_RGBAZ = "rgbaz",
         RI_A = "a", RI_Z = "z", RI_AZ = "az";
const RtToken  RI_PERSPECTIVE = "perspective", RI_ORTHOGRAPHIC = "orthographic";
const RtToken  RI_HIDDEN = "hidden", RI_PAINT = "paint";
const RtToken  RI_CONSTANT = "constant", RI_SMOOTH = "smooth";
const RtToken  RI_FLATNESS = "flatness", RI_FOV = "fov";

const RtToken  RI_AMBIENTLIGHT = "ambientlight", RI_POINTLIGHT = "pointlight", RI_DISTANTLIGHT = "distantlight", RI_SPOTLIGHT = "spotlight";
const RtToken  RI_INTENSITY = "intensity", RI_LIGHTCOLOR = "lightcolor", RI_FROM = "from", RI_TO = "to", RI_CONEANGLE = "coneangle",
         RI_CONEDELTAANGLE = "conedeltaangle", RI_BEAMDISTRIBUTION = "beamdistribution";
const RtToken  RI_MATTE = "matte", RI_METAL = "metal", RI_SHINYMETAL = "shinymetal",
         RI_PLASTIC = "plastic", RI_PAINTEDPLASTIC = "paintedplastic";
const RtToken  RI_KA = "Ka", RI_KD = "Kd", RI_KS = "Ks", RI_ROUGHNESS = "roughness", RI_KR = "Kr",
         RI_TEXTURENAME = "texturename", RI_SPECULARCOLOR = "specularcolor";
const RtToken  RI_DEPTHCUE = "depthcue", RI_FOG = "fog", RI_BUMPY = "bumpy";
const RtToken  RI_MINDISTANCE = "mindistance", RI_MAXDISTANCE = "maxdistance", RI_BACKGROUND = "background",
         RI_DISTANCE = "distance", RI_AMPLITUDE = "amplitude";

const RtToken  RI_INSIDE = "inside", RI_OUTSIDE = "outside", RI_LH = "lh", RI_RH = "rh";
const RtToken  RI_P = "P", RI_PZ = "Pz", RI_PW = "Pw", RI_N = "N", RI_NP = "NP", RI_CS = "Cs",
         RI_OS = "Os", RI_S = "s", RI_T = "t", RI_ST = "st";
const RtToken  RI_BILINEAR = "bilinear", RI_BICUBIC = "bicubic";
const RtToken  RI_PRIMITIVE = "primitive", RI_INTERSECTION = "intersection", RI_UNION = "union", RI_DIFFERENCE = "difference";
const RtToken  RI_PERIODIC = "periodic", RI_NONPERIODIC = "nonperiodic", RI_CLAMP = "clamp", RI_BLACK = "black";
const RtToken  RI_IGNORE = "ignore", RI_PRINT = "print", RI_ABORT = "abort", RI_HANDLER = "handler";
const RtToken  RI_ORIGIN = "origin", RI_IDENTIFIER = "identifier", RI_NAME = "name";
const RtToken  RI_COMMENT = "comment", RI_STRUCTURE = "structure", RI_VERBATIM = "verbatim";
const RtToken  RI_LINEAR = "linear", RI_CUBIC = "cubic", RI_WIDTH = "width", RI_CONSTANTWIDTH = "constantwidth";

const RtToken  RI_CURRENT = "current", RI_WORLD = "world", RI_OBJECT = "object", RI_SHADER = "shader";
const RtToken  RI_RASTER = "raster", RI_NDC = "NDC", RI_SCREEN = "screen", RI_CAMERA = "camera", RI_EYE = "eye";

const RtToken  RI_CATMULLCLARK = "catmull-clark", RI_HOLE = "hole", RI_CREASE = "crease", RI_CORNER = "corner", RI_INTERPOLATEBOUNDARY = "interpolateboundary";
//@}

/** @brief RenderMan Interface as (nearly) pure virtual C++ class
 */
class IRi {
public:

	/* The virtual destructor
	 */
	inline virtual ~IRi() {}

	//@{
	/** Standard build-in filter functions
	 */
	virtual const IFilterFunc &boxFilter() const = 0;
	virtual const IFilterFunc &catmullRomFilter() const = 0;
	virtual const IFilterFunc &gaussianFilter() const = 0;
	virtual const IFilterFunc &sincFilter() const = 0;
	virtual const IFilterFunc &triangleFilter() const = 0;
	//@}

	//@{
	/** Standard build-in procedural primitives
	 */
	virtual const ISubdivFunc &procDelayedReadArchive() const = 0;
	virtual const ISubdivFunc &procRunProgram() const = 0;
	virtual const ISubdivFunc &procDynamicLoad() const = 0;
	virtual const IFreeFunc &procFree() const = 0;
	//@}

	//@{
	/** Standard build-in error handler
	 */
	virtual const IErrorHandler &errorAbort() const = 0;
	virtual const IErrorHandler &errorIgnore() const = 0;
	virtual const IErrorHandler &errorPrint() const = 0;
	//@}

	/** Gets the last error number occured.
	 */
	virtual RtInt lastError() = 0;

	/** @brief Sets an error handler
	 *  @param handler The error handler
	 *
	 *  The error handler (predifined are
	 *  errorAbort, errorPrint, errorIgnore)
	 *  is called if an error occurs.
	 */
	virtual RtVoid errorHandler(const IErrorHandler &handler) = 0;


	/** @brief Declares a token (parameter type)
	 *  @param name name of the token
	 *  @param declaration declaration string of the token
	 *  @return A unique token
	 *  s.a. @ref [ARM2000], Parameter Declarations, 64-66,
	 *  s.a. @ref [RISPEC3.2], RiDeclare, pp. 13-15,
	 *  s.a. @ref [STE2003], Declaring parameter types, 150-152,
	 *  s.a. @ref [UPS89], Extending the set of variables, 242-244
	 */
	virtual RtToken declare(RtString name, RtString declaration) = 0;
 
	//! Synchronize the rendering state
	/*! \param name Type of synchronization, e.g. RI_ABORT to abort the rendering
	 */
    virtual RtVoid synchronize(RtToken name) = 0;

	//@{
	/** Contexts
	 */
	/** @brief Gets the current context
	 * @return The current handle
	 */
	virtual RtContextHandle getContext(void) = 0;

	/** @brief Context switching, sets a new active context
	 * @param handle Active context, handle is a context, previously returned by getContext()
	 */
	virtual RtVoid context(RtContextHandle handle) = 0;
	//@}

	//@{
	/** Modes
	 */
	 
	/** @brief Creates and initializes a new rendering context
	 * @param name indicates either a rib file for output or an identifier for a renderer
	 */
	virtual RtVoid begin(RtString name) = 0;
	
	// better stay with tzhe standard
	// @param arguments for optional arguments (added)
	// virtual RtVoid begin(RtString name, RtString arguments=RI_NULL) = 0;

	/** @brief Terminates the current rendering context, does cleanup operations
	 */
	virtual RtVoid end(void) = 0;

	//! Starts a new single frame block of an animation sequence
	/*! \param number of the frame
	 */
	virtual RtVoid frameBegin(RtInt number) = 0;

	//! Ends a frame block
	virtual RtVoid frameEnd(void) = 0;

	//! Starts a new world block, the description of the scene geometry
	virtual RtVoid worldBegin(void) = 0;

	//! Ends a world block
	virtual RtVoid worldEnd(void) = 0;

	//! Starts a new attribute block, pushes the current set of attriubes
	virtual RtVoid attributeBegin(void) = 0;

	//! Ends an attribute block, pops the set of attributes
	virtual RtVoid attributeEnd(void) = 0;

	//! Starts a transformation block, pushes the composit transformation matrix
	virtual RtVoid transformBegin(void) = 0;

	//! Ends a transformation block, pops the composit transformation matrix
	virtual RtVoid transformEnd(void) = 0;

	//! Starts a solid block, a CSG solid primitive
	/*! \param token Type of the solid block either RI_PRIMITIVE, RI_INTERSECTION, RI_UNION or RI_DIFFERENCE
	 */
    virtual RtVoid solidBegin(RtToken type) = 0;

	//! Ends a solid block
    virtual RtVoid solidEnd(void) = 0;

	//! Starts a new object block, the definition fo an object
	/*! \return The handle is used to later reference the object in IRi::objectInstance()
	 */
	virtual RtObjectHandle objectBegin(void) = 0;

	//! Ends an object block, the object can now be created with IRi::objectInstance()
	virtual RtVoid objectEnd(void) = 0;

	//! Retained geometry (objectBegin(), objectEnd())
	/*! Creates a new object referenced by handle
	 * \param handle Reference to an object, returned by objectBegin
	 */
    virtual RtVoid objectInstance(RtObjectHandle handle) = 0;

	//! Starts a motion block for a moving primitive
	/*! \param N     Number of samples (length of times)
	 *  \param times Vector of samples
	 *  \sa TRiCPP::motionBeginV()
	 */
    virtual RtVoid motionBegin(RtInt N, RtFloat sample, ...) = 0;

	//! Starts a motion block, \sa motionBegin()
	/*! \param N     Number of samples (length of times)
	 *  \param times Vector of samples
	 */
    virtual RtVoid motionBeginV(RtInt N, RtFloat times[]) = 0;

	//! End of a motion block
    virtual RtVoid motionEnd(void) = 0;
	//@}

	//@{
	/** Options
	 */

	//! Sets display resolution
	/*! Sets the hroizontal (xres) and vertical (yres) resolution (in pixels)
	 *  of the image to be rendered.
	 *  \param xres Horizontal resolution (pixel)
	 *  \param yres Vertical resolution (pixel)
	 *  \pram aspect pixel aspect ratio pixelwidth/pixelheight
	 */
    virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect) = 0;

	//! Sets the aspect ration of the frame
	/*! Frameaspectratio is the ratio of the width to the height of the desired image.
	 *  \param aspect frame aspect ratio width/height
	 */
    virtual RtVoid frameAspectRatio(RtFloat aspect) = 0;

	//! Sets the coordinates of the screen window (screen coordinates left <= right, bot <= top)
	/*! Defines a rectangle in the image plane that gets mapped to the raster
	 *  coordinate system and that corresponds to the display areea selected.
	 *  \param left screen coordinate of the left window border
	 *  \param right screen coordinates of the right window border
	 *  \param bot screen coordinates of the bottom window border
	 *  \param top screen coordinates of the top window border
	 */
    virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;

	//! Renders only a subrectangle of an image.
	/*! Sets a crop window in normalized device coordinates "NDC", (0,0) is upper left
	 *  \param xmin left side
	 *  \param xmax right side
	 *  \param ymin upper side
	 *  \param ymax lower side
	 */
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;

	//! Sets the type of 3D to 2D projection
	/*! Sets the type of the projection and marks the current coordinate system
	 *  before projection as the camera coordinate system.
	 *  \param name Type od the projection, e.g. RI_PERSPECTIVE, RI_ORTHOGRAPHIC
	 *  \param Additional parameterlist for the projection, e.g. RI_FOV float
	 */
    virtual RtVoid projection(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa projection()
    virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! The hither and yonder clipping plane, orthogonal to the camera view
	/*! \param hither distance of the front clipping plane
	 *  \param yon distance of the back clipping plane
	 */
	virtual RtVoid clipping(RtFloat hither, RtFloat yon) = 0;

	//! Additional clipping plane
	/*! Clip all geometry on the positive side of the plane described
	 *  by a point and a normal
	 *  \param x x coordinate of a point of the plane
	 *  \param y y coordinate of a point of the plane
	 *  \param z z coordinate of a point of the plane
	 *  \param nx x of the normal
	 *  \param ny y of the normal
	 *  \param nz z of the normal
	 */
    virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0; // *New 3.2*

	//! Depth of field
	/*! focaldistance sets the distance along the directionof view at which
	 *  objects will be in focus
	 *  \param fstop
	 *  \param focallength
	 *  \param focaldistance
	 */
    virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;

	//! Shutter times used for motion blur
	/*! Sets the time at which the shutter opens and closes
	 *  \param smin Start time, shutter opens
	 *  \param smax End time, shutter closes
	 */
    virtual RtVoid shutter(RtFloat smin, RtFloat smax) = 0;

	//! Pixel variance for shading
	/*! Sets the amount computed image values are allowed to
	 *  deviate from the true image values
	 *  \param variation Value of the deviation
	 */
	virtual RtVoid pixelVariance(RtFloat variation) = 0;

	//! Samples used for one pixel
	/*! Sets the effective sampling rate in the horizontal and vertical direction
	 *  \param xsamples sampling rate in horizontal direction
	 *  \param ysamples sampling rate in vertical direction
	 */
    virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;

	//! Sets a pixel filter influenced by an area of xwidth*ywidth pixels
	/*! Performs antialiasing by filtering the geometry (or supersampling) and
	 *  then sampling at pixel locations.
	 *  \param function filter function
	 *  \param xwidth size of influence in horizontal direction
	 *  \param ywidth size of influence in vertical direction
	 */
    virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;

	//! Controls the intensity
	/*! Controls the sensitivity and non-linearity of the exposure process.
	 *  Color_out = (Color_in*gain)^1/gamma
	 *  \param gain sensitivity component
	 *  \param gamma non-linearity component
	 */
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma) = 0;

	//! Sets an imager shader
	/*! Selects an imager function programmed in the Shading Language
	 * \param name Name of the shader
	 * \param token first token of the parameterlist
	 */
    virtual RtVoid imager(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa imager()
    virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the quantization parameters and dithering for colors or depth.
	/*! The result value is calculated by ivalue=fvalue*one+ditheramplitude*random()
	 *  \param type The type of quantization RI_RGBA (colors) or RI_Z (depth)
	 *  \param one  Multiply every value by this number
	 *  \param qmin Minimum value, smaller values are clamed
	 *  \param qmax Maximum value, higher values are clamed
	 *  \param ampl Ditheramplitude, random value added
	 */
	virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;

	//! Choose a display (or file) and sets the type for the output being rendered.
	/*! \param name Name for the display or filename
	 *  \param type Type of the output, e.g. RI_FRAMEBUFFER, RI_FILE
	 *  \param mode Mode of the output, e.g. RI_RGBA, RI_Z
	 *  \param token Display specific parameterlist
	 */
	virtual RtVoid display(RtString name, RtToken type, RtToken mode, RtToken token = RI_NULL, ...) = 0;

	//! \sa display()
    virtual RtVoid displayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Choose the hidden surface algorithm
	/*! \param type Algorithm, default types are RI_HIDER, RI_PAINT and RI_NULL
	 *  \param token Hider specific parameterlist
	 */
	virtual RtVoid hider(RtToken type, RtToken token = RI_NULL, ...) = 0;

	//! \sa hider()
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Controls the number of color components or samples to be used in specific colors
	/*! \param N number of samples
	 *  \param nRGB Nx3 matrix, converts N sample to a 3 component RGB value
	 *  \param RGBn 3xN matrix, converts 3 component RGB value to a N sample
	 */
    virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) = 0;

	//! Set the relative detail
	/*! Scales the result of all level of detail calculations
	 *  \param relativedetail Factor used for scaling
	 */
    virtual RtVoid relativeDetail(RtFloat relativedetail) = 0;

	//! Sets a implementation-specific option
	/*! \param name Name of the option
	 *  \param token First token of parameterlist used to set single values of the option
	 */
    virtual RtVoid option(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa option()
    virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}
	
	//@{
	/** Lights
	 */
	
	//! Sets a light source shader
	/*! Creates a non-area light, turns it on, and adds it to the current light source
	 *  list. Standard light sources are
	 *  RI_AMBIENTLIGHT, RI_POINTLIGHT, RI_DISTANTLIGHT and RI_SPOTLIGHT
	 *  \param name Name of the light source
	 *  \param token Light source specific parameterlist, RI_INTENSITY with a float value can be used
	 *  \return A light source handle to reference the light source in IRi::illuminate
	 */
    virtual RtLightHandle lightSource(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa lightSource()
    virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets an area light source shader
	/*! Creates an area light and makes it the current area light source. Each subsequent
	 *  geometric prim itive is added to the list of durfaces that define the area light source.
	 *  \param name Name of the area light source
	 *  \param token Light source specific parameterlist, RI_INTENSITY with a float value can be used
	 *  \return A light source handle to reference the light source in IRi::illuminate
	 */
	virtual RtLightHandle areaLightSource(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa areaLightSource()
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}
	

	//@{
	/** Attributes
	 */
	
	//! Sets a implementation-specific attribute
	/*! \param name Name of the attribute
	 *  \param token First token of parameterlist used to set single values of the attribute
	 */
	virtual RtVoid attribute(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa attribute()
    virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current color
	/*! \param Cs The current color
	 */
	virtual RtVoid color(RtColor Cs) = 0;

	//! Sets the current opacity
	/*! \param Cs The current opacity
	 */
	virtual RtVoid opacity(RtColor Cs) = 0;

	//! Sets the current surface shader
	/*! \param name Name of the surface shader
	 *  \param token Parameterlist of the surface shader
	 */
	virtual RtVoid surface(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa surface
    virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current atmosphere shader
	/*! \param name Name of the atmosphere shader
	 *  \param token Parameterlist of the atmosphere shader
	 */
    virtual RtVoid atmosphere(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa atmosphere
    virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current interior shader
	/*! \param name Name of the interior shader
	 *  \param token Parameterlist of the interior shader
	 */
	virtual RtVoid interior(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa interior
    virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current exterior shader
	/*! \param name Name of the exterior shader
	 *  \param token Parameterlist of the exterior shader
	 */
    virtual RtVoid exterior(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa exterior
	virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Turns a specific light source on or off
	/*! \param light Handle that references a light source (area or non-area light source)
	 *  \param onoff, RI_TRUE turns a light source on, RI_FALSE turns it off.
	 */
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) = 0;

	//! Sets the current displacement shader
	/*! \param name Name of the displacement shader
	 *  \param token Parameterlist of the displacement shader
	 */
	virtual RtVoid displacement(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! \sa displacement
    virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current set of texture coordinates to the values passed as arguments.
	/*! Declares a projection from the unit square [(0,0), (1,0), (0,1), (1,1)] in
	 *  u,v parameter space to quadrilateral [(s1,t1), (s2,t2), (s3,t3), (s4,t4)]
	 *  \param s1 'upper left' texture coordinate for s
	 *  \param t1 'upper left' texture coordinate for t
	 *  \param s2 'upper right' texture coordinate for s
	 *  \param t2 'upper right' texture coordinate for t
	 *  \param s3 'lower left' texture coordinate for s
	 *  \param t3 'lower left' texture coordinate for t
	 *  \param s4 'lower right' texture coordinate for s
	 *  \param t4 'lower right' texture coordinate for t
	 */
	virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;

	//! Sets the current shading rate to size
	/*! Controls the frequency of shading a surface.
	 *  \param size The new current shading rate (area in pixels)
	 */
    virtual RtVoid shadingRate(RtFloat size) = 0;

	//! Sets the shading interpolation type
	/*! Controls how values are interpolated between shading samples
	 *  (usually across a polygon).
	 *  \param type Type of the shading interpolation, RI_SMOOTH or RI_CONSTANT
	 */
	virtual RtVoid shadingInterpolation(RtToken type) = 0;

	//! Controls rendering as a matte, can be either on or off.
	/*! \parameter onoff RI_TRUE subsequent primitives are matte objects,
	 *                   RI_FALSE subsequent primitives are rendered normal.
	 */
    virtual RtVoid matte(RtBoolean onoff) = 0;

	//! Sets the current bounding box to bound
	/*! \param bound The bounding box, subsequent primitives are within this bound
	 */
	virtual RtVoid bound(RtBound bound) = 0;

	//! Sets the current level of detail to the area of the bounding box in raster coordinate system
	/*! \param bound The bounding box to calculate the level of detail
	 */
	virtual RtVoid detail(RtBound bound) = 0;

	//! Sets the current detail range
	/*! The detail range is taken for subsequent primitives to the next call of
     *  detailRange() or the next attributeEnd() or worldEnd()
	 *  \param minvis model is not used for values smaller than this level of detail (LOD)
	 *  \param lowtran model is most important for this LOD (start), always present for values bigger than this and smaller than uptran
	 *  \param uptran model is most important for this LOD (end), always present for values smaller than this and bigger than lowtran
	 *  \param maxvis model is not used for values bigger than this LOD
	 */
	virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;

	//! Sets the criterion for approximating the surface primitives with other, more easily rendered, primitives (usually polygons)
	/*!  \param type Type of criterion for approximation, e.g. RI_TESSELATION in TGLRenderer
	 *  \param value metric for the criterion, e.g. the number of polygons generated by a tesselation in TGLRenderer
	 */
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value) = 0;

	//! Sets the type of the geometric representation used to represent the primitives
	/*! \param type Type of representation, e.g. RI_POINTS for points at vertices
	 */
	virtual RtVoid geometricRepresentation(RtToken type) = 0; /* New 3.2 */

	//! Sets the current orientation
	/*! The orientation can be set expicitly to be either left-handed or right-handed or
	 *  set to the inside or the outside (the side is then rendered if IRi::sides(1))
	 *  \param orientation The new orientation,
	 *                     RI_LH, TRi_RH, RI_INSIDE or RI_OUTSIDE
	 */
	virtual RtVoid orientation(RtToken orientation) = 0;

	//! Flips current orientation, left-handed becomes right-handed and vice versa
	virtual RtVoid reverseOrientation(void) = 0;

	//! Sets the number of sides of a surface being rendered
    /*! If sides is 2, subsequent surfaces are considerd two-sided and both the inside
	 *  and the outside of a surface will be visible.
	 *  \param nsides Visible sides of a surface (0 <= nsides <= 2)
	 */
	virtual RtVoid sides(RtInt nsides) = 0;

	//! Sets the current basis matrices used by bicubic patches
	/*! \param ubasis The u-basis matrix
	 *  \param ustep The step size in u direction (bicubic patch meshes)
	 *  \param vbasis The v-basis matrix
	 *  \param vstep The step size in v direction (bicubic patch meshes)
	 */
    virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;

	//! Sets the current trim curve
	/**  The current trim curve consists of NURB curves in homogeneous parameter space,
	 *  and is used to trim IRi::nuPatch().
	 *  \param nloops Number of curve loops (curves with holes)
	 *  \param ncurves Number of curves per loop, 
	 *                 the first curve is the outline, subsequent are holes,
	 *                 size of the array is nloops,
	 *                 the sum of all elements in this array is the number of curves
	 *  \param order   The order of all curves
	 *  \param knot    The concatenated knot vectors of all curves
	 *  \param amin    The minimal parametric value for each curve
	 *  \param amax    The maximal parametric value for each curve
	 *  \param n       The number of control points for each curve
	 *  \param u       The u coordinates of the control points
	 *  \param v       The v coordinates of the control points
	 *  \param w       The w coordinates of the control points
	 */
    virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) = 0;
	//@}

	//@{
	//! Transformations of the cartesian coordinate system
	/*! Transformations modify the composite transformation matrix (CTM),
	 *  concatenation is done by premultiplying a matrix to the CTM.
	 */

	//! Sets the CTM to the identity matrix
	virtual RtVoid identity(void) = 0;

	//! Sets the CTM to transform
	//! \param transform The new transformation matrix
	virtual RtVoid transform(RtMatrix transform) = 0;

	//! Concatenates transform onto the CTM
	//! \param concatTransform CTM is premultiplied by transform
	virtual RtVoid concatTransform(RtMatrix transform) = 0;

	//! Concatenates a perspective transformation matrix onto the CTM
	/*! \param fov The field of view angle used to calculate the perspective transformation
	 */
	virtual RtVoid perspective(RtFloat fov) = 0;

	//! Concatenates a translation onto the CTM
	/*! \param dx translation in direction x
	 *  \param dy translation in direction y
	 *  \param dz translation in direction z
	 */
	virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;

	//! Concatenates a rotation onto the CTM
	/* Rotates angle degrees around the axis vector (dx, dy, dz)
	 */
	/*! \param angel Degrees to rotate
	 *  \param dx x component of the rotation axis
	 *  \param dy y component of the rotation axis
	 *  \param dz z component of the rotation axis
	 */
	virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;

	//! Concatenates a scaling onto the CTM
	/*! \param dx Scales in x direction
	 *  \param dx Scales in y direction
	 *  \param dx Scales in z direction
	 */
	virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	//! Concatenates a skew onto the CTM
	/*! Skews angle degrees on the plane defined by the vectors (dx1, dy1, dz1) and (dx2, dy2, dz2)
	 * \param angle degrees to skew
	 * \param dx1 x coordinate of the first vector for the plane
	 * \param dy1 y coordinate of the first vector for the plane
	 * \param dz1 z coordinate of the first vector for the plane
	 * \param dx2 x coordinate of the second vector for the plane
	 * \param dy2 y coordinate of the second vector for the plane
	 * \param dz2 z coordinate of the second vector for the plane
	 */
    virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;

	//! Sets a deformation shader
	/*! \param name Name of the deformation shader
	 *  \param token Shader specific parameter list
	 */
	virtual RtVoid deformation(RtString name, RtToken token = RI_NULL, ...) = 0;
	//! \sa deformation()
	virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Marks the CTM
	/*! Marks the coordinate system defined by the current transformation with the name 'space' and saves it.
	 *  \param space Name for the CTM, the CTM can be referenced by space later
	 */
	virtual RtVoid coordinateSystem(RtToken space) = 0;

	//! Assigns the matrix previously marked by space (with IRi::coordinateSystem) to the CTM
	/*! \param space Name of the matrix that becomes the new CTM
	 */
	virtual RtVoid coordSysTransform(RtToken space) = 0; /* New 3.2 */

	//! Transform points
	/*! Transforms an array of points from the coordinate system fromspace to the coordinatesystem tospace.
	 *  \param fromspace name of the coordinate system from which points are transformed
	 *  \param tospace name of the coordinate system to which points are transformed
	 *  \param points the points that are to be transformed, the values are modified
	 *  \return points, containing the modified points, NULL if an error orccured
	 */
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;
	//@}

	
	//@{
	/** Graphics primitives
	 */

	//@{
	/** Polygons
	 */

	//! Requests a single closed planar convex polygon
	/*! nvertices is the number of vertices in a single closed planar convex polygon.
	 *  The parameterlist is a list of token-array pairs where each token
	 *  is one of the standard geometric primitive variables or a variable
	 *  which has been defined by IRi::declare()
	 *  \param nvertices Number of verticces
	 *  \param token Parameterlist, containing at least the points for the vertices, containing at least the points for the vertices
	 */
	virtual RtVoid polygon(RtInt nvertices, RtToken token = RI_NULL, ...) = 0;

	//! \sa polygon()
    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a single general concave planar polygon with holes
	/*! \param nloops number of loops defining the general polygon
	 *  \param nvertices Number of vertices, the first is the outline, followed by holes
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
	virtual RtVoid generalPolygon(RtInt nloops, RtInt *nverts, RtToken token = RI_NULL, ...) = 0;

	//! \sa generalPolygon
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a mesh of convex planar polygons with shared vertices
	/*! \param npolys number of polygons
	 *  \param nverts Number of vertices per polygon
	 *  \param verts indices of the vertices
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
	virtual RtVoid pointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...) = 0;

	//! \sa pointsPolygons()
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a mesh of general concave planar polygons with holes and shared vertices
	/*! \param npolys number of polygons
	 *  \param nloops number of loops defining the general polygons
	 *  \param nverts Number of vertices per polygon
	 *  \param verts indices of the vertices
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
	virtual RtVoid pointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...) = 0;

	//! \sa pointsGeneralPolygon()
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//@{
	/** Patches
	 */

	//! Requests a single bilinear or bicubic patch
	/*! \param type Either RI_BILINEAR or RI_BICUBIC
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
    virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...) = 0;

	//! \sa patch()
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a bilinear or bicubic patch mesh
	/*! \param type Either RI_BILINEAR or RI_BICUBIC
	 *  \param nu patches in u direction
	 *  \param uwrap periodic (RI_PERIODIC) or not periodic (RI_UNPERIODIC) in u direction
	 *  \param nv patches in v direction
	 *  \param vwrap periodic (RI_PERIODIC) or not periodic (RI_UNPERIODIC) in v direction
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
    virtual RtVoid patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token = RI_NULL, ...) = 0;

	//! \sa patchMesh()
    virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a single tensor product rational or polynomal non-uniform B-spline (NURB) surface patch
	/*! \param nu     Number of knots in u direction
	 *  \param uorder The order of the nu-patch in u direction
	 *  \param uknot  The knot vector for the u direction
	 *  \param umin   Minimal parameter value for u
	 *  \param umax   Maximal parameter value for u
	 *  \param nv     Number of knots in v direction
	 *  \param vorder The order of the nu-patch in u direction
	 *  \param vknot  The knot vector for the v direction
	 *  \param vmin   Minimal parameter value for v
	 *  \param vmax   Maximal parameter value for v
	 *  \param token  Parameterlist, containing at least the points for the vertices
	 */
	virtual RtVoid nuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtToken token = RI_NULL, ...) = 0;


	//! \sa nuPatch()
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}


	//@{
	/** Subdivision Surfaces
	 */

	//! Request a subdivison surface mesh
	/*! \param scheme Scheme of the subdivion mesh, currently RI_CATMULLCLARK only
	 *  \param nfaces Number of initial faces
	 *  \param nvertices Number of the vertices per face
	 *  \param vertices Indices of the vertices
	 *  \param ntags Number of tags
	 *  \param tags The tags
	 *  \param nargs Number of the arguments of a tag, number of intargs followed by the number of the floatargs
	 *  \param intargs Integer arguments for the tags
	 *  \param floatargs Float arguments for the tags
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
    virtual RtVoid subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa subdivisionMesh()
    virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}


	//@{
	/** Quadrics, created by sweeping a curve around z, thetamax is given in degrees
	 */

	//! Requests a sphere surface
	/*! The sphere is created by rotating an arc in the +x,z half plane, with center at O, around the z axis
	 *  \param radius Radius of the sphere
	 *  \param zmin Minimal z value of the sphere
	 *  \param zmax Minimal z value of the sphere
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa sphere
    virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a conic surface
	/*! The cone is created by rotating a line that hits the x (or is completely on x) and the z axis in the +x,+z quater around the z axis
	 *  \param height Height of the cone
	 *  \param radius Radius of the cone base
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa cone()
    virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a cylindric surface
	/*! The cylinder is created by rotating a line parallel to x axis in the +x,+z quater around the z axis
	 *  \param radius Radius of the cylinder
	 *  \param zmin
	 *  \param zmin Minimal z value of the cylinder
	 *  \param zmax Minimal z value of the cylinder
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa cylinder
    virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a hyperbolic surface
	/*! The hyperboloid is created by rotating a line around the z axis
	 *  \param point1 Startpoint of the line
	 *  \param point2 Endpoint of the line
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa hyperboloid()
    virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a parabolic surface
	/*! The hyperboloid is created by rotating a parabolic curve in the +x,+z quater around the z axis
	 *  \param rmax radius at zmax (maximal radius)
	 *  \param zmin minimal z value
	 *  \param zmax maximal z value
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa paraboloid()
    virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a flat disk, same as a cone with height == 0
	/*! The disk is created by rotating a line on parallel to the x axsis (on the positive side) that hits the z axis around the z axis
	 *  \param height translation in z
	 *  \param radius Radius of the disk
	 *  \param thetamax Rotation angle
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa disk()
    virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a torodial surface
	/*! Creates first an arc on the +x,z plane using minorrad, phimin, phimax
	 *  with center at (majorad, 0, 0) and then a torus by rotating
	 *  this arc thetmax degrees around z
	 *  \param majorrad Distance between center of the torus and center of the arc being swwept
	 *  \param minorrad Radius of the arc
	 *  \phimin startangle of the arc
	 *  \phimax endangle of the arc
	 *  \param thetamax Rotation angle around z
	 *  \param token Parameterlist with additional parameters
	 */
	virtual RtVoid torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	//! \sa torus()
    virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** Point and curve primitives
	 */

	//! Creates a point cloud
	/*! \param npts Number of points
	 *  \param token Parameterlist, must contain at least the positions of the points
	 */
	virtual RtVoid points(RtInt npts, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa points
    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	//! Creates linear or cubic curves
	/*! \param type Type of the curves RI_LINEAR or RI_CUBIC
	 *  \param ncurves Number of curves
	 *  \param nverts number of the vertices of the curves
	 *  \param wrap RI_PERIODIC (curve wraps) or RI_NONPERIODIC
	 *  \param token Parameterlist, contains at last the positions of the vertices
	 */
    virtual RtVoid curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa curves()
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}

	//@{
	/** Blobby implicit surfaces
	 */

	//! Creates implicit surfaces
	/*! Blobby surfaces may be composed of spherical and sausage-like line-segment
	 *  primitives with extremly flexible control over blending. The surface type
	 *  also provides for repulsion to avoid intersection with irregular ground
	 *  planes represented by depth maps, s.a. \ref [RISPEC3.2].
	 * \param nleaf Number of primitive blobs
	 * \param ncode Size of the code array
	 * \param code Code array, s.a. \ref [RISPEC3.2] for the opcodes
	 * \param nflt Size of the flt array
	 * \param flt The float operands for the code
	 * \param nstr Size of the str array
	 * \param str The string operants for the code
	 * \param token Parameterlist
	 */
	virtual RtVoid blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa blobby()
    virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}


	//@{
	//! Procedural primitives
	/*! Calls a procedural primitive
	 */
	virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) = 0;
	//@}

	//@{
    /** General objects
	 */

	//! Generates a predefined geometry, RI_TEAPOT is the only predifined one
	/*! \param type type of the geometry (RI_TEAPOT)
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid geometry(RtToken type, RtToken token = RI_NULL, ...) = 0;

	//! \sa geometry()
    virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//@}

	//@{
    /** Map-making, functions to create texture map from (rendered) images
	 */

	//! Creates a texture map.
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the texture map (output).
	 *  \param swrap Periodic in s direction.
	 *  \param twrap Periodic in t direction.
	 *  \param filterfunc Pixel filter
	 *  \param swidth Width of the filter in s direction
	 *  \param twidth Width of the filter in t direction
	 *  \param token parameterlist with additional token-array pairs
	 */
	virtual RtVoid makeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeTexture()
    virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a bump map
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the bump map (output).
	 *  \param swrap Periodic in s direction.
	 *  \param twrap Periodic in t direction.
	 *  \param filterfunc Pixel filter
	 *  \param swidth Width of the filter in s direction
	 *  \param twidth Width of the filter in t direction
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeBump()
    virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a lattitude/longitude environment map
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the environment map (output).
	 *  \param filterfunc Pixel filter
	 *  \param swidth Width of the filter in s direction
	 *  \param twidth Width of the filter in t direction
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeLatLongEnvironment()
    virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a cube face environment map
	/*! S.a. \ref [RISPEC3.2]
	 *  \param px Filename input image, viewed from axis: forward +X, Up +Y, Right -Z
	 *  \param nx Filename input image, viewed from axis: forward -X, Up +Y, Right +Z
	 *  \param py Filename input image, viewed from axis: forward +Y, Up -Z, Right +X
	 *  \param ny Filename input image, viewed from axis: forward -Y, Up +Z, Right +X
	 *  \param pz Filename input image, viewed from axis: forward +Z, Up +Y, Right +X
	 *  \param nz Filename input image, viewed from axis: forward -Z, Up +Y, Right -X
	 *  \param tex Filename for the environment map (output).
	 *  \param fov Field of view to create the environment map.
	 *  \param filterfunc Pixel filter
	 *  \param swidth Width of the filter in s direction
	 *  \param twidth Width of the filter in t direction
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeCubeFaceEnvironment()
    virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a shadow map
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the shadow map (output).
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeShadow(RtString pic, RtString tex, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeShadow()
    virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//@{
	/** Archive files
	 */

	//! Prints into a RIB file
	/*! \param type Type of Record RI_COMMENT (#text), RI_STRUCTURE (##text), RI_VERBATIM (text)
	 *  \param format Format string like for printf()
	 */
    virtual RtVoid archiveRecord(RtToken type, RtString format, ...) = 0; /* New 3.2 */

	//! \sa archiveRecord(), line contauns the formatted string
	virtual RtVoid archiveRecordV(RtToken type, RtString line) = 0; /* Added, e.g. line is the formatted line from archive record */

	//! Reads a RIB archive from a file and renders it's content
	/*! \param filename Name of the RIB file
	 *  \param callback Called for each archive record
	 *  \param token Parameterlist with additional tokens
	 */
    virtual RtVoid readArchive(RtString name, const IArchiveCallback *callback, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa readArchive()
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}

	/*
	// -> RenderMan 11.5.2
	virtual RtVoid scopedCoordinateSystem(RtToken space) = 0;

	// -> QRM ?, Pixie (http://www.george-graphics.co.uk/pixiewiki/Main_Page)
	virtual RtVoid resource(RtToken handle, RtToken type, ...) = 0;
	virtual RtVoid resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid resourceBegin(RtVoid),
	virtual RtVoid resourceEnd(RtVoid);

	// -> Pixie, 3Delight (archiveInstance)
	virtual RtArchiveHandle archiveBegin(RtString name, ...) = 0;
    virtual RtArchiveHandle archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid archiveEnd(void) = 0;
	virtual RtVoid archiveInstance(RtArchiveHandle handle) = 0;

	// -> Pixie, RenderMan 2004 ???
	virtual RtVoid ifBegin(RtString expr, ...) = 0;
	virtual RtVoid ifBeginV(RtString expr, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid elseIf(RtString expr, ...) = 0;
	virtual RtVoid elseIfV(RtString expr, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid elseBegin(void) = 0; // was RiElse
	virtual RtVoid ifEnd(void) = 0;
	*/
}; // IRi
} // namespace RiCPP
#endif // _RICPP_RICPP_RICPP_H
