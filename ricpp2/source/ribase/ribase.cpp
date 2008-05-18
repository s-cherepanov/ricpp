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

/** @file ribase.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Definitions of the C-binding of the RenderMan(R) interface internal use only.
 *
 * Error and procedural functions are used only as pointers for the C binding.
 * The Ri Routins maps these pointers to the corresponding RiCPP objects.
 * @see riinternal.h CErrorHandlerSlot CSubdivFuncSlot CFreeFuncSlot
 */
#define RI_EXPORTS
#include "ricpp/ribase/ricppconst.h"
#include "ricpp/ribase/ricpptokens.h"
#include "ricpp/ribase/ricppdecls.h"

#include <cmath>
#include <iostream>

extern "C" {


// ----------------------------------------------------------------------------
RICPP_INTERN(RtInt) RiLastError = RIE_NOERROR;


// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiErrorAbort (RtInt code, RtInt severity, RtString message)
{
	RiLastError = code;
	// Dummy
	std::cerr << "# *** Code [" << code << "], severity [" << severity << "] '" <<
		(message ? message : "") << "'" << std::endl;
	if ( severity == RIE_SEVERE ) {
		std::cerr << "# abort renderer." << std::endl;
		exit(code);
	}
}


RICPP_INTERN(RtVoid) RiErrorPrint (RtInt code, RtInt severity, RtString message)
{
	RiLastError = code;
	// Dummy
	std::cerr << "# *** Code [" << code << "], severity [" << severity << "] '" <<
		(message ? message : "") << "'" << std::endl;
}

RICPP_INTERN(RtVoid) RiErrorIgnore (RtInt code, RtInt severity, RtString message)
{
	RiLastError = code;
	// Dummy
}

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtVoid) RiProcDelayedReadArchive (RtPointer data, RtFloat detail)
{
	// Dummy
}


RICPP_INTERN(RtVoid) RiProcRunProgram (RtPointer data, RtFloat detail)
{
	// Dummy
}


RICPP_INTERN(RtVoid) RiProcDynamicLoad (RtPointer data, RtFloat detail)
{
	// Dummy
}


RICPP_INTERN(RtVoid) RiProcFree(RtPointer)
{
	// Dummy
}

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtFloat)
RiGaussianFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) {
	x *= (RtFloat)(2.0 / xwidth);
	y *= (RtFloat)(2.0 / ywidth);
	return (RtFloat)exp(-2.0 * (x*x + y*y));
}


RICPP_INTERN(RtFloat)
RiBoxFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) {
	x = x;
	y = y;
	xwidth = xwidth;
	ywidth = ywidth;

	return (RtFloat)1.0;
}

RICPP_INTERN(RtFloat)
RiTriangleFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) {
	return (RtFloat)(((1.0 - fabs(x)) / (xwidth * 0.5)) *
		   ((1.0 - fabs(y)) / (ywidth * 0.5)));
}

RICPP_INTERN(RtFloat)
RiCatmullRomFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) {
	RtFloat r2 = (x*x + y*y);
	RtFloat r = (RtFloat)sqrt(r2);

	xwidth = xwidth;
	ywidth = ywidth;

	return (RtFloat)((r > 2.0) ? 0.0 :
	                 (r < 1.0) ? (3.0*r*r2 - 5.0*r2 + 2.0) :
		                         (-r*r2 + 5.0*r2 - 8.0*r + 4.0));
}


RICPP_INTERN(RtFloat)
RiSincFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) {
	RtFloat s, t;

	xwidth = xwidth;
	ywidth = ywidth;

	if ( x > -0.001 && x < 0.001)
		s = 1.0;
	else
		s = (RtFloat)(sin(x)/x);

	if ( y > -0.001 && y < 0.001 )
		t = 1.0;
	else
		t = (RtFloat)(sin(y)/y);

	return s*t;
}


/*   */

RICPP_INTERN(RtMatrix) RiIdentityMatrix = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtContextHandle) illContextHandle = (RtContextHandle)0;
RICPP_INTERN(RtObjectHandle) illObjectHandle = (RtObjectHandle)0;
RICPP_INTERN(RtLightHandle) illLightHandle = (RtLightHandle)0;
RICPP_INTERN(RtArchiveHandle) illArchiveHandle = (RtArchiveHandle)0;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtBoolean) RI_FALSE       = 0;   /**<  false */
RICPP_INTERN(RtBoolean) RI_TRUE        = 1;   /**<  true  */

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtFloat) RI_INFINITY      = (RtFloat)(1.0e38);
RICPP_INTERN(RtFloat) RI_EPSILON       = (RtFloat)(1.0e-10);

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtInt) RI_BEZIERSTEP      = 3;
RICPP_INTERN(RtInt) RI_BSPLINESTEP     = 1;
RICPP_INTERN(RtInt) RI_CATMULLROMSTEP  = 1;
RICPP_INTERN(RtInt) RI_HERMITESTEP     = 2;
RICPP_INTERN(RtInt) RI_POWERSTEP       = 4;

RICPP_INTERN(RtBasis) RiBezierBasis = {
	{(RtFloat)-1.0, (RtFloat) 3.0, (RtFloat)-3.0, (RtFloat) 1.0},
	{(RtFloat) 3.0, (RtFloat)-6.0, (RtFloat) 3.0, (RtFloat) 0.0},
	{(RtFloat)-3.0, (RtFloat) 3.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};

RICPP_INTERN(RtBasis) RiBSplineBasis = {
	{(RtFloat)(-1.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat)(-3.0/6.0), (RtFloat)(1.0/6.0)},
	{(RtFloat)( 3.0/6.0), (RtFloat)(-6.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)(-3.0/6.0), (RtFloat)  0.0,      (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)( 1.0/6.0), (RtFloat)( 4.0/6.0), (RtFloat)( 1.0/6.0), (RtFloat) 0.0}
};

RICPP_INTERN(RtBasis) RiCatmullRomBasis = {
	{(RtFloat)(-1.0/2.0), (RtFloat)( 3.0/2.0), (RtFloat)(-3.0/2.0), (RtFloat)( 1.0/2.0)},
	{(RtFloat)( 2.0/2.0), (RtFloat)(-5.0/2.0), (RtFloat)( 4.0/2.0), (RtFloat)(-1.0/2.0)},
	{(RtFloat)(-1.0/2.0), (RtFloat)  0.0,      (RtFloat)( 1.0/2.0), (RtFloat)  0.0},
	{(RtFloat)  0.0,      (RtFloat)( 2.0/2.0), (RtFloat)  0.0,      (RtFloat)  0.0}
};

RICPP_INTERN(RtBasis) RiHermiteBasis = {
	{(RtFloat) 2.0, (RtFloat) 1.0, (RtFloat)-2.0, (RtFloat) 1.0},
	{(RtFloat)-3.0, (RtFloat)-2.0, (RtFloat) 3.0, (RtFloat)-1.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};

RICPP_INTERN(RtBasis) RiPowerBasis = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtInt) RI_BLOBBY_CONSTANT        = 1000;
RICPP_INTERN(RtInt) RI_BLOBBY_ELLIPSOID       = 1001;
RICPP_INTERN(RtInt) RI_BLOBBY_SEGMENT_BLOB    = 1002;
RICPP_INTERN(RtInt) RI_BLOBBY_REPELLING_PLANE = 1003;

RICPP_INTERN(RtInt) RI_BLOBBY_OP_ADD = 0;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_MUL = 1;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_MAX = 2;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_MIN = 3;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_SUB = 4;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_DIV = 5;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_NEG = 6;
RICPP_INTERN(RtInt) RI_BLOBBY_OP_IDP = 7;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtInt) RIE_NOERROR        =  0;

RICPP_INTERN(RtInt) RIE_NOMEM          =  1;
RICPP_INTERN(RtInt) RIE_SYSTEM         =  2;
RICPP_INTERN(RtInt) RIE_NOFILE         =  3;
RICPP_INTERN(RtInt) RIE_BADFILE        =  4;
RICPP_INTERN(RtInt) RIE_VERSION        =  5;
RICPP_INTERN(RtInt) RIE_DISKFULL       =  6;

RICPP_INTERN(RtInt) RIE_INCAPABLE      = 11;
RICPP_INTERN(RtInt) RIE_UNIMPLEMENT    = 12;
RICPP_INTERN(RtInt) RIE_LIMIT          = 13;
RICPP_INTERN(RtInt) RIE_BUG            = 14;

RICPP_INTERN(RtInt) RIE_NOTSTARTED     = 23;
RICPP_INTERN(RtInt) RIE_NESTING        = 24;
RICPP_INTERN(RtInt) RIE_NOTOPTIONS     = 25;
RICPP_INTERN(RtInt) RIE_NOTATTRIBS     = 26;
RICPP_INTERN(RtInt) RIE_NOTPRIMS       = 27;
RICPP_INTERN(RtInt) RIE_ILLSTATE       = 28;
RICPP_INTERN(RtInt) RIE_BADMOTION      = 29;
RICPP_INTERN(RtInt) RIE_BADSOLID       = 30;

RICPP_INTERN(RtInt) RIE_BADTOKEN       = 41;
RICPP_INTERN(RtInt) RIE_RANGE          = 42;
RICPP_INTERN(RtInt) RIE_CONSISTENCY    = 43;
RICPP_INTERN(RtInt) RIE_BADHANDLE      = 44;
RICPP_INTERN(RtInt) RIE_NOSHADER       = 45;
RICPP_INTERN(RtInt) RIE_MISSINGDATA    = 46;
RICPP_INTERN(RtInt) RIE_SYNTAX         = 47;

RICPP_INTERN(RtInt) RIE_MATH           = 61;
RICPP_INTERN(RtInt) RIE_LASTERROR      = 61;

RICPP_INTERN(RtInt) RIE_INFO           =  0;
RICPP_INTERN(RtInt) RIE_WARNING        =  1;
RICPP_INTERN(RtInt) RIE_ERROR          =  2;
RICPP_INTERN(RtInt) RIE_SEVERE         =  3;
RICPP_INTERN(RtInt) RIE_LASTSEVERITY   =  3;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtToken) RI_NULL = 0;

RICPP_INTERN(RtToken) RI_FRAMEBUFFER = "framebuffer",
              RI_FILE = "file";

RICPP_INTERN(RtToken)  RI_RGB = "rgb",
               RI_RGBA = "rgba",
			   RI_RGBZ = "rgbz",
			   RI_RGBAZ = "rgbaz",
               RI_A = "a",
			   RI_Z = "z",
			   RI_AZ = "az";

RICPP_INTERN(RtToken)  RI_PERSPECTIVE = "perspective",
               RI_ORTHOGRAPHIC = "orthographic";

RICPP_INTERN(RtToken)  RI_HIDDEN = "hidden",
               RI_PAINT = "paint";

RICPP_INTERN(RtToken)  RI_CONSTANT = "constant",
               RI_SMOOTH = "smooth";

RICPP_INTERN(RtToken)  RI_FLATNESS = "flatness";

RICPP_INTERN(RtToken)  RI_FOV = "fov";

RICPP_INTERN(RtToken)  RI_AMBIENTLIGHT = "ambientlight",
               RI_POINTLIGHT = "pointlight",
               RI_DISTANTLIGHT = "distantlight",
			   RI_SPOTLIGHT = "spotlight";

RICPP_INTERN(RtToken)  RI_INTENSITY = "intensity",
               RI_LIGHTCOLOR = "lightcolor",
			   RI_FROM = "from",
			   RI_TO = "to",
			   RI_CONEANGLE = "coneangle",
               RI_CONEDELTAANGLE = "conedeltaangle",
			   RI_BEAMDISTRIBUTION = "beamdistribution";

RICPP_INTERN(RtToken)  RI_MATTE = "matte",
               RI_METAL = "metal",
			   RI_SHINYMETAL = "shinymetal",
               RI_PLASTIC = "plastic",
			   RI_PAINTEDPLASTIC = "paintedplastic";

RICPP_INTERN(RtToken)  RI_KA = "Ka",
               RI_KD = "Kd",
			   RI_KS = "Ks",
			   RI_ROUGHNESS = "roughness",
			   RI_KR = "Kr",
               RI_TEXTURENAME = "texturename",
			   RI_SPECULARCOLOR = "specularcolor";

RICPP_INTERN(RtToken)  RI_DEPTHCUE = "depthcue",
               RI_FOG = "fog";

RICPP_INTERN(RtToken)  RI_BUMPY = "bumpy";

RICPP_INTERN(RtToken)  RI_MINDISTANCE = "mindistance",
               RI_MAXDISTANCE = "maxdistance",
			   RI_BACKGROUND = "background",
               RI_DISTANCE = "distance",
			   RI_AMPLITUDE = "amplitude";

RICPP_INTERN(RtToken)  RI_RASTER = "raster",
			   RI_SCREEN = "screen",
			   RI_CAMERA = "camera",
               RI_WORLD = "world",
			   RI_OBJECT = "object";

RICPP_INTERN(RtToken)  RI_INSIDE = "inside",
               RI_OUTSIDE = "outside",
			   RI_LH = "lh",
			   RI_RH = "rh";

RICPP_INTERN(RtToken)  RI_P = "P",
               RI_PZ = "Pz",
			   RI_PW = "Pw",
			   RI_N = "N",
			   RI_NP = "NP",
			   RI_CS = "Cs",
               RI_OS = "Os",
			   RI_S = "s",
			   RI_T = "t",
			   RI_ST = "st";

RICPP_INTERN(RtToken)  RI_BILINEAR = "bilinear",
               RI_BICUBIC = "bicubic";

RICPP_INTERN(RtToken)  RI_PRIMITIVE = "primitive",
               RI_INTERSECTION = "intersection",
			   RI_UNION = "union",
			   RI_DIFFERENCE = "difference";

RICPP_INTERN(RtToken)  RI_PERIODIC = "periodic",
               RI_NONPERIODIC = "nonperiodic",
               RI_CLAMP = "clamp",
			   RI_BLACK = "black";

RICPP_INTERN(RtToken)  RI_IGNORE = "ignore",
               RI_PRINT = "print",
			   RI_ABORT = "abort",
			   RI_HANDLER = "handler";


RICPP_INTERN(RtToken)  RI_CATMULLCLARK = "catmull-clark",
               RI_HOLE = "hole",
			   RI_CREASE = "crease",
			   RI_CORNER = "corner",
			   RI_INTERPOLATEBOUNDARY = "interpolateboundary";

RICPP_INTERN(RtToken)  RI_ORIGIN = "origin",
			   RI_NAME = "name";

RICPP_INTERN(RtToken)  RI_COMMENT = "comment",
               RI_STRUCTURE = "structure",
			   RI_VERBATIM = "verbatim";

RICPP_INTERN(RtToken)  RI_LINEAR = "linear",
               RI_CUBIC = "cubic";

RICPP_INTERN(RtToken)  RI_WIDTH = "width",
			   RI_CONSTANTWIDTH = "constantwidth";

RICPP_INTERN(RtToken)  RI_DISPWIDTH = "dispwidth",
	RI_DISPHEIGHT = "dispheight",
	RI_PIXELASPECT = "pixelaspect";

RICPP_INTERN(RtToken)  RI_CURRENT = "current",
			   RI_EYE = "eye",
			   RI_SHADER = "shader",
               RI_NDC = "NDC";

RICPP_INTERN(RtToken)  RI_FILEPOINTER = "filepointer",
               RI_MEMORY = "memory";

RICPP_INTERN(RtToken) RI_ARCHIVE = "archive",
              RI_DRAFT = "draft",
              RI_REALISTIC = "realistic";

RICPP_INTERN(RtToken) RI_FLOAT   = "float";
RICPP_INTERN(RtToken) RI_INTEGER = "integer";
RICPP_INTERN(RtToken) RI_STRING  = "string";
RICPP_INTERN(RtToken) RI_POINT   = "point";
RICPP_INTERN(RtToken) RI_VECTOR  = "vector";
RICPP_INTERN(RtToken) RI_NORMAL  = "normal";
RICPP_INTERN(RtToken) RI_HPOINT  = "hpoint";
RICPP_INTERN(RtToken) RI_MATRIX  = "matrix";
RICPP_INTERN(RtToken) RI_COLOR   = "color";

RICPP_INTERN(RtToken) RI_UNIFORM = "uniform";
RICPP_INTERN(RtToken) RI_VARYING = "varying";
RICPP_INTERN(RtToken) RI_VERTEX = "vertex";
RICPP_INTERN(RtToken) RI_FACEVARYING = "facevarying";
RICPP_INTERN(RtToken) RI_FACEVERTEX = "facevertex";

RICPP_INTERN(RtToken) RI_HANDLEID = "__handleid";

RICPP_INTERN(RtToken) RI_PROJECTION = "Projection";
RICPP_INTERN(RtToken) RI_IMAGER = "Imager";
RICPP_INTERN(RtToken) RI_DISPLAY = "Display";
RICPP_INTERN(RtToken) RI_HIDER = "Hider";
RICPP_INTERN(RtToken) RI_OPTION = "Option";
RICPP_INTERN(RtToken) RI_LIGHT_SOURCE = "LightSource";
RICPP_INTERN(RtToken) RI_AREA_LIGHT_SOURCE = "AreaLightSource";
RICPP_INTERN(RtToken) RI_SURFACE = "Surface";
RICPP_INTERN(RtToken) RI_ATMOSPHERE = "Atmosphere";
RICPP_INTERN(RtToken) RI_INTERIOR = "Interior";
RICPP_INTERN(RtToken) RI_EXTERIOR = "Exterior";
RICPP_INTERN(RtToken) RI_DISPLACEMENT = "Displacement";
RICPP_INTERN(RtToken) RI_DEFORMATION = "Deformation";
RICPP_INTERN(RtToken) RI_ATTRIBUTE = "Attribute";
RICPP_INTERN(RtToken) RI_TEXTURE = "Texture";
RICPP_INTERN(RtToken) RI_CONTROL = "Control";
RICPP_INTERN(RtToken) RI_BEGIN = "Begin";
RICPP_INTERN(RtToken) RI_RESOURCE = "Resource";

RICPP_INTERN(RtToken) RI_FRAME = "Frame";

RICPP_INTERN(RtToken) RI_DEVIATION = "deviation";
RICPP_INTERN(RtToken) RI_TESSELATION = "tesselation";

RICPP_INTERN(RtToken) RI_POINTS  = "points";
RICPP_INTERN(RtToken) RI_LINES   = "lines";

RICPP_INTERN(RtToken) RI_SENSE  = "sense";

RICPP_INTERN(RtToken) RI_DELAYED_READ_ARCHIVE = "DelayedReadArchive";
RICPP_INTERN(RtToken) RI_RUN_PROGRAM = "RunProgram";
RICPP_INTERN(RtToken) RI_DYNAMIC_LOAD = "DynamicLoad";
RICPP_INTERN(RtToken) RI_FREE = "Free";

RICPP_INTERN(RtToken) RI_GAUSSIAN = "gaussian";
RICPP_INTERN(RtToken) RI_BOX = "box";
RICPP_INTERN(RtToken) RI_TRIANGLE = "triangle";
RICPP_INTERN(RtToken) RI_SINC = "sinc";

RICPP_INTERN(RtToken) RI_BEZIER = "bezier";
RICPP_INTERN(RtToken) RI_B_SPLINE = "b-spline";
RICPP_INTERN(RtToken) RI_CATMULL_ROM = "catmull-rom";
RICPP_INTERN(RtToken) RI_CATMULLROM = "catmullrom";
RICPP_INTERN(RtToken) RI_HERMITE = "hermite";
RICPP_INTERN(RtToken) RI_POWER = "power";

RICPP_INTERN(RtToken) RI_BUMP = "Bump";
RICPP_INTERN(RtToken) RI_SHADOW = "Shadow";
RICPP_INTERN(RtToken) RI_LAT_LONG_ENVIRONMENT  = "LatLongEnvironment";
RICPP_INTERN(RtToken) RI_CUBE_FACE_ENVIRONMENT = "CubeFaceEnvironment";
RICPP_INTERN(RtToken) RI_BRICK_MAP = "BrickMap";

RICPP_INTERN(RtToken) RI_INT     = "int";

RICPP_INTERN(RtToken) RI_NULL_LIT = "null";
RICPP_INTERN(RtToken) RI_UNKNOWN = "unknown";
RICPP_INTERN(RtToken) RI_EMPTY   = "";

}
