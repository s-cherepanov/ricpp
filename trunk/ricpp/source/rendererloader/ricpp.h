#ifndef _RICPP_RENDERLOADER_RICPP_H
#define _RICPP_RENDERLOADER_RICPP_H

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

/*! \file ricpp.h
 *  \author Andreas Pidde (andreas@pidde.de)
 *  \brief Base class for RenderMan C++ Binding
 *
 *  ricpp.h means the same to RiCPP as ri.h to the standard C binding.
 *  For an interface description, you can use
 *  \anchor [ARM2000]   Apodaca, Gritz et all; Advanced RenderMan; 2000, Morgan Kaufman
 *  \anchor [RISPEC3.2] Pixar; The RenderMan Interface Version 3.2; 2000, PDF document
 *  \anchor [STE2003]   Ian Stephenson; Essential RenderMan fast; Springer
 *  \anchor [UPS89]     Steve Upstill; The RenderMan Companion; 1989, Addison Wesley
 */
#include <string>
#include <limits>

//! Forward declaration of the RI (RenderMan Interface) base class
class TRi;

// Start of copy from ri.h (s.a. \ref [RISPEC3.2], Appendix C)

typedef short   RtBoolean; //!< Booleans can be TRi::RI_TRUE or TRi::RI_FALSE
typedef int     RtInt;     //!< Integers
typedef float   RtFloat;   //!< Floats

typedef const char *RtToken;     //!< Tokens, changed from 'char *' to 'const char *'

typedef RtFloat *RtColor;        //!< Colors, changed from 'RtFloat[3]' to 'RtFloat *' to manage color using other than 3 components
typedef RtFloat  RtPoint[3];     //!< Points in 3D
typedef RtFloat  RtVector[3];    //!< Vectors in 3D
typedef RtFloat  RtNormal[3];    //!< Normals in 3D
typedef RtFloat  RtHpoint[4];    //!< Points in homgenous 3D
typedef RtFloat  RtMpoint[4][4]; //!< 'Map Points' introduced in App Note #31 (RiBlobby)
typedef RtFloat  RtMatrix[4][4]; //!< Homogenous matrix for 3D transformations
typedef RtFloat  RtBasis[4][4];  //!< Base matrix for spline interpolations
typedef RtFloat  RtBound[6];     //!< Bound of 3D object: Left, right, bottom, top, front, back
typedef const char *RtString;    //!< Character string, which is not a predefined token, changed from 'char *' to 'const char *'

typedef void   *RtPointer;       //!< Pointer to arbitrary data
#define RtVoid  void             //!< Simple 'void' datatype, used as 'return type' for functions returning nothing

typedef RtFloat (*RtFilterFunc)(RtFloat, RtFloat, RtFloat, RtFloat);            //!< Function pointer to a pixel filter
typedef RtVoid  (*RtErrorHandler)(RtInt code, RtInt severity, const char *msg); //!< Function pointer to an error handler function, changed to const char
typedef RtVoid  (*RtProcSubdivFunc)(TRi *, RtPointer, RtFloat);                 //!< subdivision for procedurals changed to include renderer instance
typedef RtVoid  (*RtProcFreeFunc)(RtPointer);                                   //!< free function (procedural)
typedef RtVoid  (*RtArchiveCallback)(TRi *, RtToken, const char *, ...);        //!< callback function to handle structural comments in rib files (RiReadArchive), changed to include renderer instance and to const char

typedef RtVoid  (*RtFunc)();        //!< Pointer to a procedure

typedef RtPointer RtObjectHandle;   //!< Handle for a object instance
typedef RtPointer RtLightHandle;    //!< Handle for a light handle
typedef RtPointer RtContextHandle;  //!< handle for a render context handle

//! New for dynamic load
//@{
typedef RtPointer (WINAPI * RiCppConvertParameters)(const char * initialdata);          //!< Parameter conversion
typedef void (WINAPI *RiCppSubdivide)(TRi *, RtPointer blinddata, RtFloat detailsize ); //!< subdivision, changed to include renderer
typedef void (WINAPI *RiCppFree)(RtPointer blinddata);                                  //!< Free arbitrary data
//@}

//! Minimum value for v+riEpsilon != v
const RtFloat riEpsilon = std::numeric_limits<float>::epsilon();

//! Maximum value for RtFloat
const RtFloat riInfinity = std::numeric_limits<float>::max();

//! Added constants for RiBlobby(): Primitives always followed by an index to the (float) operands
//@{
#define RI_BLOBBY_CONSTANT        1000 //  1 index to  1 float
#define RI_BLOBBY_ELLIPSOID       1001 //  1 index to 16 floats
#define RI_BLOBBY_SEGMENT_BLOB    1002 //  1 index to 23 floats
#define RI_BLOBBY_REPELLING_PLANE 1003 //  2 Indices, first for 1 string param, 2nd for 4 floats
//@}

//! Added constants for RiBlobby() Operations, Substract (4) and Divide (5) are exchanged - seems to be a bug in the [RISPEC]
//@{
#define RI_BLOBBY_OP_ADD 0 // Addition:      count, ...
#define RI_BLOBBY_OP_MUL 1 // Multiply:      count, ...
#define RI_BLOBBY_OP_MAX 2 // Maximum:       count, ...
#define RI_BLOBBY_OP_MIN 3 // Minimum:       count, ...
#define RI_BLOBBY_OP_SUB 4 // Substract:     subtrahend, minuend
#define RI_BLOBBY_OP_DIV 5 // Divide:        dividend, divisor
#define RI_BLOBBY_OP_NEG 6 // Negate:        negand
#define RI_BLOBBY_OP_IDP 7 // Identity:      idempotentate
//@}

/*! Error codes
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
//@{
#define RIE_NOERROR         0		//!< No error occured

#define RIE_NOMEM           1       //!< Out of memory
#define RIE_SYSTEM          2       //!< Miscellaneous system error
#define RIE_NOFILE          3       //!< File not exists
#define RIE_BADFILE         4       //!< Bad file format
#define RIE_VERSION         5       //!< File version mismatch
#define RIE_DISKFULL        6       //!< Target disk is full

#define RIE_INCAPABLE      11       //!< Optional RI feature
#define RIE_OPTIONAL       11       //!< Optional RI feature
#define RIE_UNIMPLEMENT    12       //!< Unimplemented feature
#define RIE_LIMIT          13       //!< Arbitrary program limit
#define RIE_BUG            14       //!< Probably a bug in renderer

#define RIE_NOTSTARTED     23       //!< 'RiBegin' not called
#define RIE_NESTING        24       //!< Bad begin-end nesting
#define RIE_NOTOPTIONS     25       //!< Invalid state for options
#define RIE_NOTATTRIBS     26       //!< Invalid state for attributes
#define RIE_NOTPRIMS       27       //!< Invalid state for primitives
#define RIE_ILLSTATE       28       //!< Other invalid state
#define RIE_BADMOTION      29       //!< Badly formed motion block
#define RIE_BADSOLID       30       //!< Badly formed solid block

#define RIE_BADTOKEN       41       //!< Invalid token for request
#define RIE_RANGE          42       //!< Parameter out of range
#define RIE_CONSISTENCY    43       //!< Parameters inconsistent
#define RIE_BADHANDLE      44       //!< Bad object/light/context handle
#define RIE_NOSHADER       45       //!< Can't load requested shader
#define RIE_MISSINGDATA    46       //!< Required parameters not provided
#define RIE_SYNTAX         47       //!< Declare type syntax error

#define RIE_MATH           61       //!< Zerodivide, noninvert matrix, etc.
//@}

//! Error severity levels (0-3)
//@{
#define RIE_INFO            0       //!< Rendering stats & other info
#define RIE_WARNING         1       //!< Something seems wrong, maybe okay
#define RIE_ERROR           2       //!< Problem.  Results may be wrong
#define RIE_SEVERE          3       //!< So bad you should probably abort
//@}

// End of copy from ri.h

//! Error thrown by TRi::Abort() if render error occurs
class TRendererError {
	RtInt m_severity;		//!< Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	RtInt m_code;			//!< Which of error occured, 'RIE_...'
	std::string m_message;	//!< Formatted string describing occured error

	static const char *sErrorMessages[];	//! Static list of 'RIE_...' error codes as text

public:
	//! Constructor, sets error codes and additional error message
	/*! \param code Which error ('RIE_...')
	 *  \param severity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  \param message Additional describing error string
	 */
	inline TRendererError(RtInt aCode, RtInt aSeverity = RIE_ERROR, const char *aMessage = NULL)
		: m_severity(aSeverity), m_code(aCode), m_message(aMessage) { }

	//! Formats the error/severity numbers to a string

	/*! The format is "Error [<number>:<description>] Severity [<number>:<description>]",
	 *  \retVal strCode The error is formatted to this string
	 *  \param aCode The error code (RIE_...)
	 *  \param aSeverity The Severity (RIE_Info, e.t.c.)
	 *  \return strCode.c_str()
	 */
	static const char *formatError(std::string &strCode, RtInt aCode, RtInt aSeverity);

	//! Displays error message using DISPLAY_MESSAGE()
	void displayMessage();

	//! Prints error message using PRINT_MESSAGE()
	void printMessage();

	//! Gets the current error string
	/*! \return Pointer to the error message stored in TRendererError::m_message
	 */
	inline const char *what() const { return m_message.c_str(); }

	//! Gets the current error code
	/*! \return error code stored in TRendererError::m_code
	 */
	inline RtInt code() const { return m_code; }

	//! Returns the descriptive string for an error code
	//! \param aCode The error code
	//! \return error string for the error code aCode
	inline static const char *errorMessage(RtInt aCode) {
		if ( aCode < 0 || aCode > 61 )
			return "unknown";

		return sErrorMessages[(int)aCode];
	}

	//! Get the current error severity
	/*! \return error severity stored in TRendererError::m_severity
	 */
	inline RtInt severity() const { return m_severity; }

	//! \param aSeverity severity level
	//! \return The descriptive string for an error severity level aSeverity
	inline static const char *errorSeverity(RtInt aSeverity) {
		switch ( aSeverity ) {
			case RIE_INFO:    return "Info";
			case RIE_WARNING: return "Warning";
			case RIE_ERROR:   return "Error";
			case RIE_SEVERE:  return "Severe";
			default:          return "unknown";
		}
	}
}; // TRendererError

class TTrimCurveData;

/*! Additional interface to query a renderer, renderers using TRibParser imlement this interface
 *  used by parser and macro replay
 */
class TRiCallbackInterface {
public:
	//! Handle a renderer error, calls handleErrorV()
	//! \param code error code
	//! \param severity error severity level
	//! \param message additional error message with format (like printf())
	//! \param ... Additional parameters, formatting like printf()
	virtual RtVoid handleError(RtInt code, RtInt severity, const char *message=NULL, ...) = 0;

	//! Handle a renderer error
	//! \param code error code
	//! \param severity error severity level
	//! \param message additional error message with format (like printf())
	//! \param argList Additional parameterlist, formatting like printf()
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, const char *message, va_list argList) = 0;

	//! Retrieve data about a parameter type
	//! \param decl The name of the parameter or an inline declaration
	//! \param vertices The Number of vertices
	//! \param corners The Number of corners
	//! \param facets The Number of facets
	//! \param faceVertices The Number of vertices per face
	//! \param faceCorners The Number of corners per face
	//! \retval aClass The storage class
	//! \retval aType The type (RenderMan type)
	//! \retval aCardinality The cardinality if the type is an array, 1 else
	//! \retval numComps The number of all components (components * cardinality)
	//! \retval numBytes The number of bytes used for all element (size * components * cardinality), strings count as pointer
	//! \return true if the return values are set.
	virtual bool queryParam(
		const char *decl,
		RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners,
		RtInt &aClass, RtInt &aType, RtInt &aCardinality, RtInt &numComps, IndexType &numBytes) = 0;

	//! \return current u step of TRi::basis()
	virtual RtInt curUStep() const = 0;

	//! \return current v step of TRi::basis()
	virtual RtInt curVStep() const = 0;

	//! \return current u basis of TRi::basis()
	virtual const RtBasis *curUBasis() const = 0;

	//! \return current v basis of TRi::basis()
	virtual const RtBasis *curVBasis() const = 0;

	//! \return current tesselation in parametric u
	virtual RtFloat curUTesselation() const = 0;

	//! \return current tesselation in parametric v
	virtual RtFloat curVTesselation() const = 0;

	//! \return current tesselation of a single parametric direction (u is default) for curves and trim curves
	virtual RtFloat curTesselation() const { return curUTesselation(); }
	virtual RtFloat curTessTrim()    const { return curUTesselation(); }

	//! \return current number of components of a color value
	virtual RtInt curColorComponents() const = 0;

	//! \return get current trimCurve
	virtual TTrimCurveData *curTrimCurve() const = 0;

	//! \return current linenumber (if read from rib file)
	virtual long lineNo() const = 0;

	//! \return set current linenumber (if read from rib file)
	virtual long lineNo(long aLineNo ) = 0;

	//! \return get filename (if read from rib file)
	virtual const char *archiveName() const = 0;

	//! \param archiveName name of the archive currently parsed/handled
	//! \return set filename (if read from rib file)
	virtual const char *archiveName(const char *anArchiveName ) = 0;
};

//! The C++ RIB-Binding as abstract class
/*! This class does nothing, but
 *  specify the calling interface and some static data.
 *  It is the basis of all renderer classes of RiCPP. If you want
 *  to write your own renderer on top RiCPP, TIntermediateRenderer
 *  is the class you will likely inherit from.
 *
 *  The interface is very near to the C binding. You can use
 *  the common RenderMan books as reference, \ref [RISPEC3.2] serves
 *  well.
 *
 *  The parameterlists are the lists of token-array pairs known from
 *  the C-binding, also ended by a single TRi::RI_NULL. For each
 *  interface routine with a variable parameter list another
 *  interface routine, with a V appended, is provided where the
 *  parameterlist can be supplied by to array: one for the tokens
 *  and one for the value pointers.
 */
class TRi {
protected:
	//! Name of the renderer (Typename of a 'heir' class, only for debugging)
    const char *m_rendererName;

	//! Procedural primitives
	//! \param data Function specific data
	//! \param detail Level of detail
	//@{

	//! Called to load a RIB archive
	//! \param data Path of the archive
	//! \param detail Level of detail
	virtual RtVoid handleDelayedReadArchive(RtPointer data, RtFloat detail) = 0;

	//! Called to execute a program
	//! \param data Path of the program
	//! \param detail Level of detail
	virtual RtVoid handleRunProgram(RtPointer data, RtFloat detail) = 0;

	//! Called to execute function of a dynamic library
	//! \param data Path library
	//! \param detail Level of detail
	virtual RtVoid handleDynamicLoad(RtPointer data, RtFloat detail) = 0;

	//! Free data
	//! \param data References a data block
	virtual RtVoid handleFree(RtPointer data) = 0;

	//@}

public:
	//! Initialize, clears lastError and m_rendererName
	inline TRi() { lastError = RIE_NOERROR; m_rendererName = "";}

	//! Empty virtual destructor
	inline virtual ~TRi() {}

	//! Additional number of last error, independent of contexts
	RtInt lastError;

	//! logical constants
	//@{
	//! RtBoolean false value
	static const RtBoolean RI_FALSE;

	//! RtBoolean true value
	static const RtBoolean RI_TRUE;
	//@}

	//! Numeric constants
	//@{
	//! RtFloat value for infinity, e.g. for TRi::clipping()
	static const RtFloat RI_INFINITY;

	//! RtFloat value for minimum, e.g. for TRi::clipping()
	static const RtFloat RI_EPSILON;
	//@}

	//! Numeric constants for TRi::basis()
	/*! Step size for basis matrices of splines,
	 *  s.a. \ref [Ups89] Selecting the bicubic patch type, pp. 92-94
	 */
	//{@
	//! RtInt stepsize for bezier patches with bezierBasis basis matrix, s.a TRi::basis()
	static const RtInt RI_BEZIERSTEP;

	//! RtInt stepsize for b-spline patches with bSplineBasis basis matrix, s.a TRi::basis()
	static const RtInt RI_BSPLINESTEP;

	//! RtInt stepsize for catmull-rom patches with catmullRomBasis basis matrix, s.a TRi::basis()
	static const RtInt RI_CATMULLROMSTEP;

	//! RtInt stepsize for hermite patches with hermiteBasis basis matrix, s.a TRi::basis()
	static const RtInt RI_HERMITESTEP;

	//! RtInt stepsize for ??? patches with powerBasis basis matrix, s.a TRi::basis()
	static const RtInt RI_POWERSTEP;
	//@}

	//! Standard pixel filters for TRi::pixelFilter()
	/*! Weights samples according to their distance from the pixel center. Outside the
	 *  area of influence (xwidth, ywidth), the filter functions will return 0.
	 *  S.a. \ref [Ups89] Filtering the supersamples, RiPixelFilter(), pp. 176-178
	 *
	 *  \param  x Displacement from the pixel center, -(xwidth/2) <= x <= (xwidth/2)
	 *  \param  y Displacement from the pixel center, -(ywidth/2) <= y <= (ywidth/2) 
	 *  \param  xwidth Width of the influence of the filter function in x direction
	 *  \param  ywidth Width of the influence of the filter function in y direction
	 *  \return Weight of the filter (usally a value between -1 and +1) at the displacement x, y
	 */
	//@{
	//! Box filter for TRi::pixelFilter()
	static RtFloat boxFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

	//! Catmull-rom filter for TRi::pixelFilter()
	static RtFloat catmullRomFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

	//! Gaussian filter for TRi::pixelFilter() (default)
	static RtFloat gaussianFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

	//! Sinc filter for TRi::pixelFilter()
	static RtFloat sincFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

	//! Triangle filter for TRi::pixelFilter()
	static RtFloat triangleFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);
	//@}

	//! Error handlers for TRi::errorHandler()
	/*! If an error occures while rendering, an error handler is called with
	 *  parameters describing the error.
	 *  \param code Error code RIE_-constant
	 *  \param severity level RIE_INFO, RIE_WARNING, RIE_ERROR or RIE_SEVERE
	 *  \param message additional text describing the error,
	 *  s.a. \ref [Ups89] Error Handling, pp. 38-39 and
	 *  s.a. \ref [RISPEC3.2] Appendix E
	 */
	//@{
	//! Breaks execution if an error occurs by throwing an exception.
	//! \exception TRendererError Contains error number and addition text
	static RtVoid errorAbort (RtInt code, RtInt severity, const char *message) throw (TRendererError &);

	//! Ignores an error, continues execution
	static RtVoid errorIgnore (RtInt code, RtInt severity, const char *message);

	//! Prints an error message, continues execution. Uses TRendererError::printMessage.
	static RtVoid errorPrint (RtInt code, RtInt severity, const char *message);
	//@}

	//! procedural primitives
	/*! The functions only call the appropriate handle-functions oft
	 *  the renderer instance ri
	 *  \param ri Instance which handle-menmber functions are called
	 *  \param data handler specific data
	 *  \param detail level of detail
	 *  s.a. \ref [ARM2000] Procedural Primitives, pp. 116-121,
	 *  s.a. \ref [Ups89] Procedural Models, pp. 201-208
	 */
	//@{
	//! Calls the TRi::handleDelayedReadArchive of the renderer ri
	inline static RtVoid procDelayedReadArchive(TRi *ri, RtPointer data, RtFloat detail) { /* New 3.2 */
		if ( ri )
			ri->handleDelayedReadArchive(data, detail);
	}

	//! Calls the TRi::handleRunProgram of the renderer ri
	inline static RtVoid procRunProgram(TRi *ri, RtPointer data, RtFloat detail) { /* New 3.2 */
		if ( ri )
			ri->handleRunProgram(data, detail);
	}

	//! Calls the TRi::handleDynamicLoad of the renderer ri
	inline static RtVoid procDynamicLoad(TRi *ri, RtPointer data, RtFloat detail) { /* New 3.2 */
		if ( ri )
			ri->handleDynamicLoad(data, detail);
	}

	//! Calls the TRi::handleFree of the renderer ri
	inline static RtVoid procFree(TRi *ri, RtPointer data) { /* New 3.2 */
		if ( ri )
			ri->handleFree(data);
	}
	//@}

	//! Special token, a null pointer, end marker for parameterlists
	static char *RI_NULL;

	//! Tokens of the RenderMan interface
	//@{
	static RtToken
		RI_FRAMEBUFFER, RI_FILE,
		RI_RGB, RI_RGBA, RI_RGBZ, RI_RGBAZ, RI_A, RI_Z, RI_AZ,
		RI_PERSPECTIVE, RI_ORTHOGRAPHIC,
		RI_HIDDEN, RI_PAINT,
		RI_CONSTANT, RI_SMOOTH,
		RI_FLATNESS, RI_FOV,

		RI_AMBIENTLIGHT, RI_POINTLIGHT, RI_DISTANTLIGHT, RI_SPOTLIGHT,
		RI_INTENSITY, RI_LIGHTCOLOR, RI_FROM, RI_TO, RI_CONEANGLE,
		RI_CONEDELTAANGLE, RI_BEAMDISTRIBUTION,
		RI_MATTE, RI_METAL, RI_SHINYMETAL,
		RI_PLASTIC, RI_PAINTEDPLASTIC,
		RI_KA, RI_KD, RI_KS, RI_ROUGHNESS, RI_KR,
		RI_TEXTURENAME, RI_SPECULARCOLOR,
		RI_DEPTHCUE, RI_FOG, RI_BUMPY,
		RI_MINDISTANCE, RI_MAXDISTANCE, RI_BACKGROUND,
		RI_DISTANCE, RI_AMPLITUDE,

		RI_INSIDE, RI_OUTSIDE, RI_LH, RI_RH,
		RI_P, RI_PZ, RI_PW, RI_N, RI_NP, RI_CS,
		RI_OS, RI_S, RI_T, RI_ST,
		RI_BILINEAR, RI_BICUBIC,
		RI_PRIMITIVE, RI_INTERSECTION, RI_UNION, RI_DIFFERENCE,
		RI_PERIODIC, RI_NONPERIODIC, RI_CLAMP, RI_BLACK,
		RI_IGNORE, RI_PRINT, RI_ABORT, RI_HANDLER,
		RI_ORIGIN, RI_IDENTIFIER, RI_NAME,
		RI_COMMENT, RI_STRUCTURE, RI_VERBATIM,
		RI_LINEAR, RI_CUBIC, RI_WIDTH, RI_CONSTANTWIDTH,

		RI_CURRENT, RI_WORLD, RI_OBJECT, RI_SHADER,
		RI_RASTER, RI_NDC, RI_SCREEN, RI_CAMERA, RI_EYE,

		RI_CATMULLCLARK, RI_HOLE, RI_CREASE, RI_CORNER, RI_INTERPOLATEBOUNDARY; // New
	//@}

	//! Base matrices for TRi::Basis()
	/*! Basis matrices of splines
	 *  s.a, \ref [Ups89] Selecting the bicubic patch type, pp. 92-94,
	 *  s.a. \ref [RISPEC3.2] Appendix F
	 */
	//@{
	static RtBasis
		bezierBasis,		//!< Bezier splines
		bSplineBasis,		//!< B-Splines
		catmullRomBasis,	//!< Catmull-Rom splines
		hermiteBasis,		//!< Hermite splines
		powerBasis;			//!< ??? splines
	//@}

	//! General routines
	//@{
	//! declares a variable (parrameter type)
	/*!
	 *  \param name name of the variable
	 *  \param declaration declaration string of the variable
	 *  \returns A unique token, that stands for the variable
	 *  s.a. \ref [ARM2000], Parameter Declarations, 64-66,
	 *  s.a. \ref [RISPEC3.2], RiDeclare, pp. 13-15,
	 *  s.a. \ref [STE2003], Declaring parameter types, 150-152,
	 *  s.a. \ref [UPS89], Extending the set of variables, 242-244
	 */
	virtual RtToken declare(const char *name, const char *declaration) = 0;

	//! Sets an error handler
	/*! The error handler (predifined are
	 *  TRiCPP::errorAbort, TRiCPP::errorPrint, TRiCPP::errorIgnore)
	 *  is called if an error occurs.
	 *  \param handler The error handler
	 */
	virtual RtVoid errorHandler(RtErrorHandler handler) = 0;
	//@}

	//! Graphics state
	//@{

	//! Creates and initializes a new rendering context
	virtual RtVoid begin(RtToken name = (RtToken)RI_NULL) = 0;

	//! Terminates the current rendering contest, cleanup operations
	virtual RtVoid end(void) = 0;

	//! Get current context
	//! \return The current handle
	virtual RtContextHandle getContext(void) = 0;

	//! Context switching
	//! Sets a new active context
	//! \param handle Active context, handle is a context, previously returned by getContext()
	virtual RtVoid context(RtContextHandle handle) = 0;

	//! Starts a new single frame block of an animation sequence
	//! \param number of the frame
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
	/*! \return The handle is used to later reference the object in TRi::objectInstance()
	 */
	virtual RtObjectHandle objectBegin(void) = 0;

	//! Ends an object block, the object can now be created with TRi::objectInstance()
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

	//! Synchronize the rendering state
	/*! \param name Type of synchronization, e.g. TRi::RI_ABORT to abort the rendering
	 */
    virtual RtVoid synchronize(RtToken name) = 0;

	//@}


	//! Options
	//@{

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
	/*! Sets the type of the prjection and marks the current coordinate system
	 *  before projection as the camera coordinate system.
	 *  \param name Type od the projection, e.g. RI_PERSPECTIVE, RI_ORTHOGRAPHIC
	 *  \param Additional parameterlist for the projection, e.g. RI_FOV float
	 */
    virtual RtVoid projection(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa projection()
    virtual RtVoid projectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
    virtual RtVoid pixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) = 0;

	//! Controls the intensity
	/*! Controls the sensitivity and non-linearity of the exposure process.
	 *  Color_out = (Color_in*gain)^1/gamma
	 *  \param gain sensitivity component
	 *  \param gamma non-linearity component
	 */
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma) = 0;

	//! Sets an imager shader
	/*! Selects an mager function programmed in the Shading Language
	 * \param name Name of the shader
	 * \param token first token of the parameterlist
	 */
    virtual RtVoid imager(RtToken name, RtToken token = RI_NULL, ...) = 0;

	//! \sa imager()
    virtual RtVoid imagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
	 *  \param type Type of the output, e.g. TRi::RI_FRAMEBUFFER, TRi::RI_FILE
	 *  \param mode Mode of the output, e.g. TRi::RI_RGBA, TRi::RI_Z
	 *  \param token Display specific parameterlist
	 */
	virtual RtVoid display(RtToken name, RtToken type, RtToken mode, RtToken token = RI_NULL, ...) = 0;

	//! \sa display()
    virtual RtVoid displayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Choose the hidden surface algorithm
	/*! \param type Algorithm, default types are TRi::RI_HIDER, TRi::RI_PAINT and TRi::RI_NULL
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
    virtual RtVoid option(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa option()
    virtual RtVoid optionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
	//@}


	//! Lights
	//@{
	
	//! Sets a light source shader
	/*! Creates a non-area light, turns it on, and adds it to the current light source
	 *  list. Standard light sources are
	 *  TRi::RI_AMBIENTLIGHT, TRi::RI_POINTLIGHT, TRi::RI_DISTANTLIGHT and TRi::RI_SPOTLIGHT
	 *  \param name Name of the light source
	 *  \param token Light source specific parameterlist, TRi::RI_INTENSITY with a float value can be used
	 *  \return A light source handle to reference the light source in TRi::illuminate
	 */
    virtual RtLightHandle lightSource(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa lightSource()
    virtual RtLightHandle lightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets an area light source shader
	/*! Creates an area light and makes it the current area light source. Each subsequent
	 *  geometric prim itive is added to the list of durfaces that define the area light source.
	 *  \param name Name of the area light source
	 *  \param token Light source specific parameterlist, TRi::RI_INTENSITY with a float value can be used
	 *  \return A light source handle to reference the light source in TRi::illuminate
	 */
	virtual RtLightHandle areaLightSource(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa areaLightSource()
	virtual RtLightHandle areaLightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
	//@}


	//! Attributes
	//@{
	
	//! Sets a implementation-specific attribute
	/*! \param name Name of the attribute
	 *  \param token First token of parameterlist used to set single values of the attribute
	 */
	virtual RtVoid attribute(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa attribute()
    virtual RtVoid attributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
	virtual RtVoid surface(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa surface
    virtual RtVoid surfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current atmosphere shader
	/*! \param name Name of the atmosphere shader
	 *  \param token Parameterlist of the atmosphere shader
	 */
    virtual RtVoid atmosphere(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa atmosphere
    virtual RtVoid atmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current interior shader
	/*! \param name Name of the interior shader
	 *  \param token Parameterlist of the interior shader
	 */
	virtual RtVoid interior(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa interior
    virtual RtVoid interiorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current exterior shader
	/*! \param name Name of the exterior shader
	 *  \param token Parameterlist of the exterior shader
	 */
    virtual RtVoid exterior(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa exterior
	virtual RtVoid exteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Turns a specific light source on or off
	/*! \param light Handle that references a light source (area or non-area light source)
	 *  \param onoff, TRi::RI_TRUE turns a light source on, TRi::RI_FALSE turns it off.
	 */
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) = 0;

	//! Sets the current displacement shader
	/*! \param name Name of the displacement shader
	 *  \param token Parameterlist of the displacement shader
	 */
	virtual RtVoid displacement(const char *name, RtToken token = RI_NULL, ...) = 0;

	//! \sa displacement
    virtual RtVoid displacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
	 *  \param type Type of the shading interpolation, TRi::RI_SMOOTH or TRi::RI_CONSTANT
	 */
	virtual RtVoid shadingInterpolation(RtToken type) = 0;

	//! Controls rendering as a matte, can be either on or off.
	/*! \parameter onoff TRi::RI_TRUE subsequent primitives are matte objects,
	 *                   TRi::RI_FALSE subsequent primitives are rendered normal.
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
	/*  \param type Type of criterion for approximation, e.g. TRi::RI_TESSELATION in TGLRenderer
	 *  \param value metric for the criterion, e.g. the number of polygons generated by a tesselation in TGLRenderer
	 */
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value) = 0;

	//! Sets the type of the geometric representation used to represent the primitives
	/*! \param type Type of representation, e.g. TRi::RI_POINTS for points at vertices
	 */
	virtual RtVoid geometricRepresentation(RtToken type) = 0; /* New 3.2 */

	//! Sets the current orientation
	/*! The orientation can be set expicitly to be either left-handed or right-handed or
	 *  set to the inside or the outside (the side is then rendered if TRi::sides(1))
	 *  \param orientation The new orientation,
	 *                     TRi::RI_LH, TRi_RH, TRi::RI_INSIDE or TRi::RI_OUTSIDE
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
	/*  The current trim curve consists of NURB curves in homogeneous parameter space,
	 *  and is used to trim TRi::nuPatch().
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


	//! Transformations of the cartesian coordinate system
	/*! Transformations modify the composite transformation matrix (CTM),
	 *  concatenation is done by premultiplying a matrix to the CTM.
	 */
	//@{

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
	virtual RtVoid deformation(const char *name, RtToken token = RI_NULL, ...) = 0;
	//! \sa deformation()
	virtual RtVoid deformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Marks the CTM
	/*! Marks the coordinate system defined by the current transformation with the name 'space' and saves it.
	 *  \param space Name for the CTM, the CTM can be referenced by space later
	 */
	virtual RtVoid coordinateSystem(RtToken space) = 0;

	//! Assigns the matrix previously marked by space (with TRi::coordinateSystem) to the CTM
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

	
	//! Polygons
	//@{

	//! Requests a single closed planar convex polygon
	/*! nvertices is the number of vertices in a single closed planar convex polygon.
	 *  The parameterlist is a list of token-array pairs where each token
	 *  is one of the standard geometric primitive variables or a variable
	 *  which has been defined by TRi::declare()
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

	//! Patches
	//@{

	//! Requests a single bilinear or bicubic patch
	/*! \param type Either TRi::RI_BILINEAR or TRi::RI_BICUBIC
	 *  \param token Parameterlist, containing at least the points for the vertices
	 */
    virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...) = 0;

	//! \sa patch()
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Requests a bilinear or bicubic patch mesh
	/*! \param type Either TRi::RI_BILINEAR or TRi::RI_BICUBIC
	 *  \param nu patches in u direction
	 *  \param uwrap periodic (TRi::RI_PERIODIC) or not periodic (TRi::RI_UNPERIODIC) in u direction
	 *  \param nv patches in v direction
	 *  \param vwrap periodic (TRi::RI_PERIODIC) or not periodic (TRi::RI_UNPERIODIC) in v direction
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


	//! Subdivision Surfaces
	//@{

	//! Request a subdivison surface mesh
	/*! \param scheme Scheme of the subdivion mesh, currently TRi::RI_CATMULLCLARK only
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


	//! Quadrics, created by sweeping a curve around z, thetamax is given in degrees
	//@{

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

	//@}


	//! Point and curve primitives
	//@{

	//! Creates a point cloud
	/*! \param npts Number of points
	 *  \param token Parameterlist, must contain at least the positions of the points
	 */
	virtual RtVoid points(RtInt npts, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa points
    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	//! Creates linear or cubic curves
	/*! \param type Type of the curves TRi::RI_LINEAR or TRi::RI_CUBIC
	 *  \param ncurves Number of curves
	 *  \param nverts number of the vertices of the curves
	 *  \param wrap TRi::RI_PERIODIC (curve wraps) or TRi::RI_NONPERIODIC
	 *  \param token Parameterlist, contains at last the positions of the vertices
	 */
    virtual RtVoid curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa curves()
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	//@}


	//! Blobby implicit surfaces
	//@{

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


	//! Procedural primitives
	/*! Calls a procedural primitive
	 */
	virtual RtVoid procedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) = 0;

    //! General objects
	//@{

	//! Generates a predefined geometry, TRi::RI_TEAPOT is the only predifined one
	/*! \param type type of the geometry (TRi::RI_TEAPOT)
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid geometry(RtToken type, RtToken token = RI_NULL, ...) = 0;

	//! \sa geometry()
    virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//@}

    //! Map-making, functions to create texture map from (rendered) images
	//@{

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
	virtual RtVoid makeTexture(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeTexture()
    virtual RtVoid makeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
    virtual RtVoid makeBump(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeBump()
    virtual RtVoid makeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a lattitude/longitude environment map
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the environment map (output).
	 *  \param filterfunc Pixel filter
	 *  \param swidth Width of the filter in s direction
	 *  \param twidth Width of the filter in t direction
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeLatLongEnvironment(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeLatLongEnvironment()
    virtual RtVoid makeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

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
    virtual RtVoid makeCubeFaceEnvironment(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeCubeFaceEnvironment()
    virtual RtVoid makeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Creates a shadow map
	/*! \param pic Filename of an image (input).
	 *  \param tex Filename for the shadow map (output).
	 *  \param token parameterlist with additional token-array pairs
	 */
    virtual RtVoid makeShadow(const char *pic, const char *tex, RtToken token = RI_NULL, ...) = 0;

	//! \sa makeShadow()
    virtual RtVoid makeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//@}


	//! Archive files
	//@{

	//! Prints into a RIB file
	/*! \param type Type of Record TRi::RI_COMMENT (#text), TRi::RI_STRUCTURE (##text), TRi::RI_VERBATIM (text)
	 *  \param format Format string like for printf()
	 */
    virtual RtVoid archiveRecord(RtToken type, const char *format, ...) = 0; /* New 3.2 */

	//! \sa archiveRecord(), line contauns the formatted string
	virtual RtVoid archiveRecordV(RtToken type, const char *line) = 0; /* Added, e.g. line is the formatted line from archive record */

	//! Reads a RIB archive from a file and renders it's content
	/*! \param filename Name of the RIB file
	 *  \param callback Called for each archive record
	 *  \param token Parameterlist with additional tokens
	 */
    virtual RtVoid readArchive(RtString filename, RtArchiveCallback callback, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	//! \sa readArchive()
	virtual RtVoid readArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	//@}

}; // TRi

#endif // _RICPP_RENDERLOADER_RICPP_H
