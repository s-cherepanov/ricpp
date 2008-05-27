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

RICPP_INTERN(const RtMatrix) RiIdentityMatrix = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtContextHandle const) illContextHandle = (RtContextHandle)0;
RICPP_INTERN(const RtObjectHandle) illObjectHandle = (RtObjectHandle)0;
RICPP_INTERN(RtLightHandle const) illLightHandle = (RtLightHandle)0;
RICPP_INTERN(RtArchiveHandle const) illArchiveHandle = (RtArchiveHandle)0;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtBoolean) RI_FALSE       = 0;   /**<  false */
RICPP_INTERN(const RtBoolean) RI_TRUE        = 1;   /**<  true  */

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtFloat) RI_INFINITY      = (RtFloat)(1.0e38);
RICPP_INTERN(const RtFloat) RI_EPSILON       = (RtFloat)(1.0e-10);

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtInt) RI_BEZIERSTEP      = 3;
RICPP_INTERN(const RtInt) RI_BSPLINESTEP     = 1;
RICPP_INTERN(const RtInt) RI_CATMULLROMSTEP  = 1;
RICPP_INTERN(const RtInt) RI_HERMITESTEP     = 2;
RICPP_INTERN(const RtInt) RI_POWERSTEP       = 4;

RICPP_INTERN(const RtBasis) RiBezierBasis = {
	{(RtFloat)-1.0, (RtFloat) 3.0, (RtFloat)-3.0, (RtFloat) 1.0},
	{(RtFloat) 3.0, (RtFloat)-6.0, (RtFloat) 3.0, (RtFloat) 0.0},
	{(RtFloat)-3.0, (RtFloat) 3.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};

RICPP_INTERN(const RtBasis) RiBSplineBasis = {
	{(RtFloat)(-1.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat)(-3.0/6.0), (RtFloat)(1.0/6.0)},
	{(RtFloat)( 3.0/6.0), (RtFloat)(-6.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)(-3.0/6.0), (RtFloat)  0.0,      (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)( 1.0/6.0), (RtFloat)( 4.0/6.0), (RtFloat)( 1.0/6.0), (RtFloat) 0.0}
};

RICPP_INTERN(const RtBasis) RiCatmullRomBasis = {
	{(RtFloat)(-1.0/2.0), (RtFloat)( 3.0/2.0), (RtFloat)(-3.0/2.0), (RtFloat)( 1.0/2.0)},
	{(RtFloat)( 2.0/2.0), (RtFloat)(-5.0/2.0), (RtFloat)( 4.0/2.0), (RtFloat)(-1.0/2.0)},
	{(RtFloat)(-1.0/2.0), (RtFloat)  0.0,      (RtFloat)( 1.0/2.0), (RtFloat)  0.0},
	{(RtFloat)  0.0,      (RtFloat)( 2.0/2.0), (RtFloat)  0.0,      (RtFloat)  0.0}
};

RICPP_INTERN(const RtBasis) RiHermiteBasis = {
	{(RtFloat) 2.0, (RtFloat) 1.0, (RtFloat)-2.0, (RtFloat) 1.0},
	{(RtFloat)-3.0, (RtFloat)-2.0, (RtFloat) 3.0, (RtFloat)-1.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};

RICPP_INTERN(const RtBasis) RiPowerBasis = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtInt) RI_BLOBBY_CONSTANT        = 1000;
RICPP_INTERN(const RtInt) RI_BLOBBY_ELLIPSOID       = 1001;
RICPP_INTERN(const RtInt) RI_BLOBBY_SEGMENT_BLOB    = 1002;
RICPP_INTERN(const RtInt) RI_BLOBBY_REPELLING_PLANE = 1003;

RICPP_INTERN(const RtInt) RI_BLOBBY_OP_ADD = 0;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_MUL = 1;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_MAX = 2;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_MIN = 3;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_SUB = 4;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_DIV = 5;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_NEG = 6;
RICPP_INTERN(const RtInt) RI_BLOBBY_OP_IDP = 7;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtInt) RIE_NOERROR        =  0;

RICPP_INTERN(const RtInt) RIE_NOMEM          =  1;
RICPP_INTERN(const RtInt) RIE_SYSTEM         =  2;
RICPP_INTERN(const RtInt) RIE_NOFILE         =  3;
RICPP_INTERN(const RtInt) RIE_BADFILE        =  4;
RICPP_INTERN(const RtInt) RIE_VERSION        =  5;
RICPP_INTERN(const RtInt) RIE_DISKFULL       =  6;

RICPP_INTERN(const RtInt) RIE_INCAPABLE      = 11;
RICPP_INTERN(const RtInt) RIE_UNIMPLEMENT    = 12;
RICPP_INTERN(const RtInt) RIE_LIMIT          = 13;
RICPP_INTERN(const RtInt) RIE_BUG            = 14;

RICPP_INTERN(const RtInt) RIE_NOTSTARTED     = 23;
RICPP_INTERN(const RtInt) RIE_NESTING        = 24;
RICPP_INTERN(const RtInt) RIE_NOTOPTIONS     = 25;
RICPP_INTERN(const RtInt) RIE_NOTATTRIBS     = 26;
RICPP_INTERN(const RtInt) RIE_NOTPRIMS       = 27;
RICPP_INTERN(const RtInt) RIE_ILLSTATE       = 28;
RICPP_INTERN(const RtInt) RIE_BADMOTION      = 29;
RICPP_INTERN(const RtInt) RIE_BADSOLID       = 30;

RICPP_INTERN(const RtInt) RIE_BADTOKEN       = 41;
RICPP_INTERN(const RtInt) RIE_RANGE          = 42;
RICPP_INTERN(const RtInt) RIE_CONSISTENCY    = 43;
RICPP_INTERN(const RtInt) RIE_BADHANDLE      = 44;
RICPP_INTERN(const RtInt) RIE_NOSHADER       = 45;
RICPP_INTERN(const RtInt) RIE_MISSINGDATA    = 46;
RICPP_INTERN(const RtInt) RIE_SYNTAX         = 47;

RICPP_INTERN(const RtInt) RIE_MATH           = 61;
RICPP_INTERN(const RtInt) RIE_LASTERROR      = 61;

RICPP_INTERN(const RtInt) RIE_INFO           =  0;
RICPP_INTERN(const RtInt) RIE_WARNING        =  1;
RICPP_INTERN(const RtInt) RIE_ERROR          =  2;
RICPP_INTERN(const RtInt) RIE_SEVERE         =  3;
RICPP_INTERN(const RtInt) RIE_LASTSEVERITY   =  3;

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtToken const) RI_NULL = 0;

RICPP_INTERN(RtToken const) RI_FRAMEBUFFER = "framebuffer";
RICPP_INTERN(RtToken const) RI_FILE = "file";

RICPP_INTERN(RtToken const) RI_RGB = "rgb";
RICPP_INTERN(RtToken const) RI_RGBA = "rgba";
RICPP_INTERN(RtToken const) RI_RGBZ = "rgbz";
RICPP_INTERN(RtToken const) RI_RGBAZ = "rgbaz";
RICPP_INTERN(RtToken const) RI_A = "a";
RICPP_INTERN(RtToken const) RI_Z = "z";
RICPP_INTERN(RtToken const) RI_AZ = "az";

RICPP_INTERN(RtToken const) RI_PERSPECTIVE = "perspective";
RICPP_INTERN(RtToken const) RI_ORTHOGRAPHIC = "orthographic";

RICPP_INTERN(RtToken const) RI_HIDDEN = "hidden";
RICPP_INTERN(RtToken const) RI_PAINT = "paint";

RICPP_INTERN(RtToken const) RI_CONSTANT = "constant";
RICPP_INTERN(RtToken const) RI_SMOOTH = "smooth";

RICPP_INTERN(RtToken const) RI_FLATNESS = "flatness";

RICPP_INTERN(RtToken const) RI_FOV = "fov";

RICPP_INTERN(RtToken const) RI_AMBIENTLIGHT = "ambientlight";
RICPP_INTERN(RtToken const) RI_POINTLIGHT = "pointlight";
RICPP_INTERN(RtToken const) RI_DISTANTLIGHT = "distantlight";
RICPP_INTERN(RtToken const) RI_SPOTLIGHT = "spotlight";

RICPP_INTERN(RtToken const) RI_INTENSITY = "intensity";
RICPP_INTERN(RtToken const) RI_LIGHTCOLOR = "lightcolor";
RICPP_INTERN(RtToken const) RI_FROM = "from";
RICPP_INTERN(RtToken const) RI_TO = "to";
RICPP_INTERN(RtToken const) RI_CONEANGLE = "coneangle";
RICPP_INTERN(RtToken const) RI_CONEDELTAANGLE = "conedeltaangle";
RICPP_INTERN(RtToken const) RI_BEAMDISTRIBUTION = "beamdistribution";

RICPP_INTERN(RtToken const) RI_MATTE = "matte";
RICPP_INTERN(RtToken const) RI_METAL = "metal";
RICPP_INTERN(RtToken const) RI_SHINYMETAL = "shinymetal";
RICPP_INTERN(RtToken const) RI_PLASTIC = "plastic";
RICPP_INTERN(RtToken const) RI_PAINTEDPLASTIC = "paintedplastic";

RICPP_INTERN(RtToken const) RI_KA = "Ka";
RICPP_INTERN(RtToken const) RI_KD = "Kd";
RICPP_INTERN(RtToken const) RI_KS = "Ks";
RICPP_INTERN(RtToken const) RI_ROUGHNESS = "roughness";
RICPP_INTERN(RtToken const) RI_KR = "Kr";
RICPP_INTERN(RtToken const) RI_TEXTURENAME = "texturename";
RICPP_INTERN(RtToken const) RI_SPECULARCOLOR = "specularcolor";

RICPP_INTERN(RtToken const) RI_DEPTHCUE = "depthcue";
RICPP_INTERN(RtToken const) RI_FOG = "fog";

RICPP_INTERN(RtToken const)  RI_BUMPY = "bumpy";

RICPP_INTERN(RtToken const) RI_MINDISTANCE = "mindistance";
RICPP_INTERN(RtToken const) RI_MAXDISTANCE = "maxdistance";
RICPP_INTERN(RtToken const) RI_BACKGROUND = "background";
RICPP_INTERN(RtToken const) RI_DISTANCE = "distance";
RICPP_INTERN(RtToken const) RI_AMPLITUDE = "amplitude";

RICPP_INTERN(RtToken const) RI_RASTER = "raster";
RICPP_INTERN(RtToken const) RI_SCREEN = "screen";
RICPP_INTERN(RtToken const) RI_CAMERA = "camera";
RICPP_INTERN(RtToken const) RI_WORLD = "world";
RICPP_INTERN(RtToken const) RI_OBJECT = "object";

RICPP_INTERN(RtToken const) RI_INSIDE = "inside";
RICPP_INTERN(RtToken const) RI_OUTSIDE = "outside";
RICPP_INTERN(RtToken const) RI_LH = "lh";
RICPP_INTERN(RtToken const) RI_RH = "rh";

RICPP_INTERN(RtToken const) RI_P = "P";
RICPP_INTERN(RtToken const) RI_PZ = "Pz";
RICPP_INTERN(RtToken const) RI_PW = "Pw";
RICPP_INTERN(RtToken const) RI_N = "N";
RICPP_INTERN(RtToken const) RI_NP = "NP";
RICPP_INTERN(RtToken const) RI_CS = "Cs";
RICPP_INTERN(RtToken const) RI_OS = "Os";
RICPP_INTERN(RtToken const) RI_S = "s";
RICPP_INTERN(RtToken const) RI_T = "t";
RICPP_INTERN(RtToken const) RI_ST = "st";

RICPP_INTERN(RtToken const) RI_BILINEAR = "bilinear";
RICPP_INTERN(RtToken const) RI_BICUBIC = "bicubic";

RICPP_INTERN(RtToken const) RI_PRIMITIVE = "primitive";
RICPP_INTERN(RtToken const) RI_INTERSECTION = "intersection";
RICPP_INTERN(RtToken const) RI_UNION = "union";
RICPP_INTERN(RtToken const) RI_DIFFERENCE = "difference";

RICPP_INTERN(RtToken const) RI_PERIODIC = "periodic";
RICPP_INTERN(RtToken const) RI_NONPERIODIC = "nonperiodic";
RICPP_INTERN(RtToken const) RI_CLAMP = "clamp";
RICPP_INTERN(RtToken const) RI_BLACK = "black";

RICPP_INTERN(RtToken const) RI_IGNORE = "ignore";
RICPP_INTERN(RtToken const) RI_PRINT = "print";
RICPP_INTERN(RtToken const) RI_ABORT = "abort";
RICPP_INTERN(RtToken const) RI_HANDLER = "handler";


RICPP_INTERN(RtToken const) RI_CATMULLCLARK = "catmull-clark";
RICPP_INTERN(RtToken const) RI_HOLE = "hole";
RICPP_INTERN(RtToken const) RI_CREASE = "crease";
RICPP_INTERN(RtToken const) RI_CORNER = "corner";
RICPP_INTERN(RtToken const) RI_INTERPOLATEBOUNDARY = "interpolateboundary";

RICPP_INTERN(RtToken const)  RI_ORIGIN = "origin";
RICPP_INTERN(RtToken const) RI_NAME = "name";

RICPP_INTERN(RtToken const)  RI_COMMENT = "comment";
RICPP_INTERN(RtToken const) RI_STRUCTURE = "structure";
RICPP_INTERN(RtToken const) RI_VERBATIM = "verbatim";

RICPP_INTERN(RtToken const)  RI_LINEAR = "linear";
RICPP_INTERN(RtToken const) RI_CUBIC = "cubic";

RICPP_INTERN(RtToken const)  RI_WIDTH = "width";
RICPP_INTERN(RtToken const) RI_CONSTANTWIDTH = "constantwidth";

RICPP_INTERN(RtToken const) RI_DISPXRES = "dispxres";
RICPP_INTERN(RtToken const) RI_DISPYRES = "dispyres";
RICPP_INTERN(RtToken const) RI_PIXELASPECT = "pixelaspect";

RICPP_INTERN(RtToken const) RI_CURRENT = "current";
RICPP_INTERN(RtToken const) RI_EYE = "eye";
RICPP_INTERN(RtToken const) RI_SHADER = "shader";
RICPP_INTERN(RtToken const) RI_NDC = "NDC";

RICPP_INTERN(RtToken const) RI_FILEPOINTER = "filepointer";
RICPP_INTERN(RtToken const) RI_MEMORY = "memory";

RICPP_INTERN(RtToken const) RI_ARCHIVE = "archive";
RICPP_INTERN(RtToken const) RI_DRAFT = "draft";
RICPP_INTERN(RtToken const) RI_REALISTIC = "realistic";

RICPP_INTERN(RtToken const) RI_FLOAT   = "float";
RICPP_INTERN(RtToken const) RI_INTEGER = "integer";
RICPP_INTERN(RtToken const) RI_STRING  = "string";
RICPP_INTERN(RtToken const) RI_POINT   = "point";
RICPP_INTERN(RtToken const) RI_VECTOR  = "vector";
RICPP_INTERN(RtToken const) RI_NORMAL  = "normal";
RICPP_INTERN(RtToken const) RI_HPOINT  = "hpoint";
RICPP_INTERN(RtToken const) RI_MATRIX  = "matrix";
RICPP_INTERN(RtToken const) RI_COLOR   = "color";

RICPP_INTERN(RtToken const) RI_UNIFORM = "uniform";
RICPP_INTERN(RtToken const) RI_VARYING = "varying";
RICPP_INTERN(RtToken const) RI_VERTEX = "vertex";
RICPP_INTERN(RtToken const) RI_FACEVARYING = "facevarying";
RICPP_INTERN(RtToken const) RI_FACEVERTEX = "facevertex";

RICPP_INTERN(RtToken const) RI_HANDLEID = "__handleid";

RICPP_INTERN(RtToken const) RI_PROJECTION = "Projection";
RICPP_INTERN(RtToken const) RI_IMAGER = "Imager";
RICPP_INTERN(RtToken const) RI_DISPLAY = "Display";
RICPP_INTERN(RtToken const) RI_HIDER = "Hider";
RICPP_INTERN(RtToken const) RI_OPTION = "Option";
RICPP_INTERN(RtToken const) RI_LIGHT_SOURCE = "LightSource";
RICPP_INTERN(RtToken const) RI_AREA_LIGHT_SOURCE = "AreaLightSource";
RICPP_INTERN(RtToken const) RI_SURFACE = "Surface";
RICPP_INTERN(RtToken const) RI_ATMOSPHERE = "Atmosphere";
RICPP_INTERN(RtToken const) RI_INTERIOR = "Interior";
RICPP_INTERN(RtToken const) RI_EXTERIOR = "Exterior";
RICPP_INTERN(RtToken const) RI_DISPLACEMENT = "Displacement";
RICPP_INTERN(RtToken const) RI_DEFORMATION = "Deformation";
RICPP_INTERN(RtToken const) RI_ATTRIBUTE = "Attribute";
RICPP_INTERN(RtToken const) RI_TEXTURE = "Texture";
RICPP_INTERN(RtToken const) RI_CONTROL = "Control";
RICPP_INTERN(RtToken const) RI_BEGIN = "Begin";
RICPP_INTERN(RtToken const) RI_RESOURCE = "Resource";

RICPP_INTERN(RtToken const) RI_FRAME = "Frame";

RICPP_INTERN(RtToken const) RI_DEVIATION = "deviation";
RICPP_INTERN(RtToken const) RI_TESSELATION = "tesselation";

RICPP_INTERN(RtToken const) RI_POINTS  = "points";
RICPP_INTERN(RtToken const) RI_LINES   = "lines";

RICPP_INTERN(RtToken const) RI_SENSE  = "sense";

RICPP_INTERN(RtToken const) RI_DELAYED_READ_ARCHIVE = "DelayedReadArchive";
RICPP_INTERN(RtToken const) RI_RUN_PROGRAM = "RunProgram";
RICPP_INTERN(RtToken const) RI_DYNAMIC_LOAD = "DynamicLoad";
RICPP_INTERN(RtToken const) RI_FREE = "Free";

RICPP_INTERN(RtToken const) RI_GAUSSIAN = "gaussian";
RICPP_INTERN(RtToken const) RI_BOX = "box";
RICPP_INTERN(RtToken const) RI_TRIANGLE = "triangle";
RICPP_INTERN(RtToken const) RI_SINC = "sinc";

RICPP_INTERN(RtToken const) RI_BEZIER = "bezier";
RICPP_INTERN(RtToken const) RI_B_SPLINE = "b-spline";
RICPP_INTERN(RtToken const) RI_CATMULL_ROM = "catmull-rom";
RICPP_INTERN(RtToken const) RI_CATMULLROM = "catmullrom";
RICPP_INTERN(RtToken const) RI_HERMITE = "hermite";
RICPP_INTERN(RtToken const) RI_POWER = "power";

RICPP_INTERN(RtToken const) RI_BUMP = "Bump";
RICPP_INTERN(RtToken const) RI_SHADOW = "Shadow";
RICPP_INTERN(RtToken const) RI_LAT_LONG_ENVIRONMENT  = "LatLongEnvironment";
RICPP_INTERN(RtToken const) RI_CUBE_FACE_ENVIRONMENT = "CubeFaceEnvironment";
RICPP_INTERN(RtToken const) RI_BRICK_MAP = "BrickMap";

RICPP_INTERN(RtToken const) RI_INT     = "int";

RICPP_INTERN(RtToken const) RI_NULL_LIT = "null";
RICPP_INTERN(RtToken const) RI_UNKNOWN = "unknown";
RICPP_INTERN(RtToken const) RI_EMPTY   = "";

}
