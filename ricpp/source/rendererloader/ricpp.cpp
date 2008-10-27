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

/*! \file ricpp.cpp
 *  \brief Abstract class for RenderMan C++ Binding, RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "ricpp.h"
#include "sysexcept.h"

#include <math.h>

// Renderer Error

// Error messages RIE_ as clear text
const char *TRendererError::sErrorMessages[] = {
	"RIE_NOERROR",
	"RIE_NOMEM: Out of memory",
	"RIE_SYSTEM: Miscellaneous system error",
	"RIE_NOFILE: File nonexistant",
	"RIE_BADFILE: Bad file format",
	"RIE_VERSION: File version mismatch",
	"RIE_DISKFULL: Target disk is full",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"RIE_INCAPABLE, RIE_OPTIONAL: Optional RI feature",
	"RIE_UNIMPLEMENT: Unimplemented feature",
	"RIE_LIMIT: Arbitrary program limit",
	"RIE_BUG: Probably a bug in renderer",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"RIE_NOTSTARTED: RiBegin not called",
	"RIE_NESTING: Bad begin-end nesting",
	"RIE_NOTOPTIONS: Invalid state for options",
	"RIE_NOTATTRIBS: Invalid state for attributes",
	"RIE_NOTPRIMS: Invalid state for primitives",
	"RIE_ILLSTATE: Other invalid state",
	"RIE_BADMOTION: Badly formed motion block",
	"RIE_BADSOLID: Badly formed solid block",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"RIE_BADTOKEN: Invalid token for request",
	"RIE_RANGE: Parameter out of range",
	"RIE_CONSISTENCY: Parameters inconsistent",
	"RIE_BADHANDLE: Bad object/light handle",
	"RIE_NOSHADER: Can't load requested shader",
	"RIE_MISSINGDATA: Required parameters not provided",
	"RIE_SYNTAX: Declare type syntax error",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"RIE_MATH: Zerodivide, noninvert matrix, etc.",
	NULL
};

// display Message

const char *TRendererError::formatError(std::string &strCode, RtInt aCode, RtInt aSeverity)
{
	char buffer[64];

	strCode = "Error [";
	sprintf(buffer, "%d", aCode);
	strCode += buffer;
	strCode += ": ";
	strCode += errorMessage(aCode);
	strCode += "] Severity [";
	sprintf(buffer, "%d", aSeverity);
	strCode += buffer;
	strCode += ": ";
	strCode += errorSeverity(aSeverity);
	strCode += "]";

	return strCode.c_str();
}


void TRendererError::displayMessage() {
	std::string strCaption;
	DISPLAY_MESSAGE(m_message.c_str(), formatError(strCaption, m_code, m_severity));
}


void TRendererError::printMessage() {
	std::string strCaption;
	PRINT_MESSAGE(m_message.c_str(), formatError(strCaption, m_code, m_severity));
}


// Constants

const RtBoolean TRi::RI_FALSE =   0;
const RtBoolean TRi::RI_TRUE  =   1;
// const RtFloat TRi::RI_INFINITY = (RtFloat)1.0e38; // Defined in interface, however, the limit of the system is used
const RtFloat TRi::RI_INFINITY  = riInfinity;
// const RtFloat TRi::RI_EPSILON  = (RtFloat)1.0e-10; // Defined in interface, but it is too small
const RtFloat TRi::RI_EPSILON  = riEpsilon;

char *TRi::RI_NULL = 0L;

const RtInt TRi::RI_BEZIERSTEP       = ((RtInt)3);
const RtInt TRi::RI_BSPLINESTEP      = ((RtInt)1);
const RtInt TRi::RI_CATMULLROMSTEP   = ((RtInt)1);
const RtInt TRi::RI_HERMITESTEP      = ((RtInt)2);
const RtInt TRi::RI_POWERSTEP        = ((RtInt)4);

// Tokens RI_... as clear text
RtToken  TRi::RI_FRAMEBUFFER = "framebuffer", TRi::RI_FILE = "file";
RtToken  TRi::RI_RGB = "rgb", TRi::RI_RGBA = "rgba", TRi::RI_RGBZ = "rgbz", TRi::RI_RGBAZ = "rgbaz",
         TRi::RI_A = "a", TRi::RI_Z = "z", TRi::RI_AZ = "az";
RtToken  TRi::RI_PERSPECTIVE = "perspective", TRi::RI_ORTHOGRAPHIC = "orthographic";
RtToken  TRi::RI_HIDDEN = "hidden", TRi::RI_PAINT = "paint";
RtToken  TRi::RI_CONSTANT = "constant", TRi::RI_SMOOTH = "smooth";
RtToken  TRi::RI_FLATNESS = "flatness", TRi::RI_FOV = "fov";

RtToken  TRi::RI_AMBIENTLIGHT = "ambientlight", TRi::RI_POINTLIGHT = "pointlight", TRi::RI_DISTANTLIGHT = "distantlight", TRi::RI_SPOTLIGHT = "spotlight";
RtToken  TRi::RI_INTENSITY = "intensity", TRi::RI_LIGHTCOLOR = "lightcolor", TRi::RI_FROM = "from", TRi::RI_TO = "to", TRi::RI_CONEANGLE = "coneangle",
         TRi::RI_CONEDELTAANGLE = "conedeltaangle", TRi::RI_BEAMDISTRIBUTION = "beamdistribution";
RtToken  TRi::RI_MATTE = "matte", TRi::RI_METAL = "metal", TRi::RI_SHINYMETAL = "shinymetal",
         TRi::RI_PLASTIC = "plastic", TRi::RI_PAINTEDPLASTIC = "paintedplastic";
RtToken  TRi::RI_KA = "Ka", TRi::RI_KD = "Kd", TRi::RI_KS = "Ks", TRi::RI_ROUGHNESS = "roughness", TRi::RI_KR = "Kr",
         TRi::RI_TEXTURENAME = "texturename", TRi::RI_SPECULARCOLOR = "specularcolor";
RtToken  TRi::RI_DEPTHCUE = "depthcue", TRi::RI_FOG = "fog", TRi::RI_BUMPY = "bumpy";
RtToken  TRi::RI_MINDISTANCE = "mindistance", TRi::RI_MAXDISTANCE = "maxdistance", TRi::RI_BACKGROUND = "background",
         TRi::RI_DISTANCE = "distance", TRi::RI_AMPLITUDE = "amplitude";

RtToken  TRi::RI_INSIDE = "inside", TRi::RI_OUTSIDE = "outside", TRi::RI_LH = "lh", TRi::RI_RH = "rh";
RtToken  TRi::RI_P = "P", TRi::RI_PZ = "Pz", TRi::RI_PW = "Pw", TRi::RI_N = "N", TRi::RI_NP = "NP", TRi::RI_CS = "Cs",
         TRi::RI_OS = "Os", TRi::RI_S = "s", TRi::RI_T = "t", TRi::RI_ST = "st";
RtToken  TRi::RI_BILINEAR = "bilinear", TRi::RI_BICUBIC = "bicubic";
RtToken  TRi::RI_PRIMITIVE = "primitive", TRi::RI_INTERSECTION = "intersection", TRi::RI_UNION = "union", TRi::RI_DIFFERENCE = "difference";
RtToken  TRi::RI_PERIODIC = "periodic", TRi::RI_NONPERIODIC = "nonperiodic", TRi::RI_CLAMP = "clamp", TRi::RI_BLACK = "black";
RtToken  TRi::RI_IGNORE = "ignore", TRi::RI_PRINT = "print", TRi::RI_ABORT = "abort", TRi::RI_HANDLER = "handler";
RtToken  TRi::RI_ORIGIN = "origin", TRi::RI_IDENTIFIER = "identifier", TRi::RI_NAME = "name";
RtToken  TRi::RI_COMMENT = "comment", TRi::RI_STRUCTURE = "structure", TRi::RI_VERBATIM = "verbatim";
RtToken  TRi::RI_LINEAR = "linear", TRi::RI_CUBIC = "cubic", TRi::RI_WIDTH = "width", TRi::RI_CONSTANTWIDTH = "constantwidth";

RtToken  TRi::RI_CURRENT = "current", TRi::RI_WORLD = "world", TRi::RI_OBJECT = "object", TRi::RI_SHADER = "shader";
RtToken  TRi::RI_RASTER = "raster", TRi::RI_NDC = "NDC", TRi::RI_SCREEN = "screen", TRi::RI_CAMERA = "camera", TRi::RI_EYE = "eye";

RtToken  TRi::RI_CATMULLCLARK = "catmull-clark", TRi::RI_HOLE = "hole", TRi::RI_CREASE = "crease", TRi::RI_CORNER = "corner", TRi::RI_INTERPOLATEBOUNDARY = "interpolateboundary";

// Basis matrices for splines s.a. [RISPEC3.2] Appendix F 

RtBasis TRi::bezierBasis = {
	{(RtFloat)-1.0, (RtFloat) 3.0, (RtFloat)-3.0, (RtFloat) 1.0},
	{(RtFloat) 3.0, (RtFloat)-6.0, (RtFloat) 3.0, (RtFloat) 0.0},
	{(RtFloat)-3.0, (RtFloat) 3.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};
RtBasis TRi::bSplineBasis = {
	{(RtFloat)(-1.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat)(-3.0/6.0), (RtFloat)(1.0/6.0)},
	{(RtFloat)( 3.0/6.0), (RtFloat)(-6.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)(-3.0/6.0), (RtFloat)  0.0,      (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)( 1.0/6.0), (RtFloat)( 4.0/6.0), (RtFloat)( 1.0/6.0), (RtFloat) 0.0}
};
RtBasis TRi::catmullRomBasis = {
	{(RtFloat)(-1.0/2.0), (RtFloat)( 3.0/2.0), (RtFloat)(-3.0/2.0), (RtFloat)( 1.0/2.0)},
	{(RtFloat)( 2.0/2.0), (RtFloat)(-5.0/2.0), (RtFloat)( 4.0/2.0), (RtFloat)(-1.0/2.0)},
	{(RtFloat)(-1.0/2.0), (RtFloat)  0.0,      (RtFloat)( 1.0/2.0), (RtFloat)  0.0},
	{(RtFloat)  0.0,      (RtFloat)( 2.0/2.0), (RtFloat)  0.0,      (RtFloat)  0.0}
};
RtBasis TRi::hermiteBasis = {
	{(RtFloat) 2.0, (RtFloat) 1.0, (RtFloat)-2.0, (RtFloat) 1.0},
	{(RtFloat)-3.0, (RtFloat)-2.0, (RtFloat) 3.0, (RtFloat)-1.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
};
RtBasis TRi::powerBasis = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// Filter functions - see [RISPEC3.2] Appendix E

//
// RiGaussianFilter
//
RtFloat	TRi::gaussianFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth)
{
	x *= (RtFloat)(2.0 / xwidth);
	y *= (RtFloat)(2.0 / ywidth);
	return (RtFloat)exp(-2.0 * (x*x + y*y));
}


//
// RiBoxFilter
//
RtFloat	TRi::boxFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth)
{
	return (RtFloat)1.0;
}


//
// RiTriangleFilter
//
RtFloat	TRi::triangleFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth)
{
	return (RtFloat)(((1.0 - fabs(x)) / (xwidth * 0.5)) *
		   ((1.0 - fabs(y)) / (ywidth * 0.5)));
}


//
// RiCatmullRomFilter
//
RtFloat	TRi::catmullRomFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth)
{
	RtFloat r2 = (x*x + y*y);
	RtFloat r = (RtFloat)sqrt(r2);
	return (RtFloat)((r > 2.0) ? 0.0 :
	                 (r < 1.0) ? (3.0*r*r2 - 5.0*r2 + 2.0) :
		                         (-r*r2 + 5.0*r2 - 8.0*r + 4.0));
}


//
// RiSincFilter
//
RtFloat	TRi::sincFilter(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth)
{
	RtFloat s, t;

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


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

////////////////////////
// Default Error Handler

RtVoid TRi::errorAbort(RtInt code, RtInt severity, const char *message) throw (TRendererError &) {
	if ( severity == RIE_SEVERE )
		throw TRendererError(code, severity, message);
    TRendererError e(code, severity, message);
	e.printMessage();
}

RtVoid TRi::errorIgnore(RtInt code, RtInt severity, const char *message) {
	code = code;
	severity = severity;
	message = message;
}

RtVoid TRi::errorPrint(RtInt code, RtInt severity, const char *message) {
    TRendererError e(code, severity, message);
	e.printMessage();
}
