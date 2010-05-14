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
#define RICPP_EXPORTS
#include "ricpp/ribase/ricppconst.h"
#include "ricpp/ribase/ricpptokens.h"
#include "ricpp/ribase/ricppdecls.h"
#include "ricpp/tools/templatefuncs.h"


#include <cmath>
#include <cstring>
#include <iostream>

extern "C" {


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
		s = (RtFloat)1.0;
	else
		s = (RtFloat)(sin(x)/x);

	if ( y > -0.001 && y < 0.001 )
		t = (RtFloat)1.0;
	else
		t = (RtFloat)(sin(y)/y);

	return s*t;
}

	
/// @todo Remove code duplication with the CMatrix3D
	
RICPP_INTERN(RtVoid)
RiCPPMult3DPoint (const RtPoint point, const RtMatrix matrix, RtPoint result)
{
	int i, k;
	RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
	RtFloat p[4] = {point[1], point[2], point[3], 1.0};

	// premultiply result = p x m
	for ( i=0; i<4; ++i) for ( k=0; k<4; ++k) d[i] += p[k] * matrix[k][i];
	
	if ( fabs(d[3]) < std::numeric_limits<RtFloat>::epsilon() ) {
		result[0] = d[0];
		result[0] = d[1];
		result[1] = d[2];
	} else {
		result[0] = d[0]/d[3];
		result[0] = d[1]/d[3];
		result[1] = d[2]/d[3];
	}
}
	
RICPP_INTERN(RtVoid)
RiCPPMult3DPoints (const RtPoint *points, RtInt count, const RtMatrix matrix, RtPoint *result)
{
	int i, j, k;
	
	RtInt size = count * 3;
	const RtFloat *p = &points[0][0];
	RtFloat *r = &result[0][0];
	
	for ( j = 0; j < size; j+=3 ) {
		RtFloat t[4] = {p[j], p[j+1], p[j+2], 1.0};
		RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
		for ( i=0; i<4; ++i)
			for ( k=0; k<4; ++k)
				d[i] += matrix[i][k] * t[k];
		
		if ( fabs(d[3]) < std::numeric_limits<RtFloat>::epsilon() ) {
			r[j]   = d[0];
			r[j+1] = d[1];
			r[j+2] = d[2];
		} else {
			r[j]   = d[0]/d[3];
			r[j+1] = d[1]/d[3];
			r[j+2] = d[2]/d[3];
		}
	}
}


RICPP_INTERN(RtVoid)
RiCPPIntersectLinePlane (const RtPoint *line, const RtPoint pnorm, const RtPoint ppoint,
						 RtPoint *psect)
{
	if ( !psect ) {
		return;
	}
	
	if ( !line ) {
		return;
	}
	
	/// @note pnorm is not the normal, pnorm - ppoint is (???). And psect is really a pointer, maybe to fill arrays (see 3DKit Dokumentation).
	/// @see For line plane intersection: Paul Bourke, Solution 1 http://local.wasp.uwa.edu.au/~pbourke/geometry/planeline/

	RtFloat pNormVect[3];
	RiCPP::vvSub3(pNormVect, (RtFloat *)&pnorm[0], (RtFloat *)&ppoint[0]);
	
	RtFloat normLDir[3];
	RiCPP::vvSub3(normLDir, (RtFloat *)&line[1][0], (RtFloat *)&line[0][0]);
	
	RtFloat vectLineToPlane[3];
	RiCPP::vvSub3(vectLineToPlane, (RtFloat *)&ppoint[0], (RtFloat *)&line[0][0]);
	
	RtFloat ratio = RiCPP::dot3(pNormVect, vectLineToPlane)/RiCPP::dot3(pNormVect, normLDir); // can be NaN -> Result is also NaN
	
	(*psect)[0] = line[0][0] + ratio*normLDir[0];
	(*psect)[1] = line[0][1] + ratio*normLDir[1];
	(*psect)[2] = line[0][2] + ratio*normLDir[2];
}

RICPP_INTERN(RtVoid)
RiCPPMultiplyMatrix (const RtMatrix pre, const RtMatrix post, RtMatrix result)
{
	RtMatrix s;
	int i, j, k;
	for ( i=0; i<4; ++i ) {
		for ( j=0; j<4; ++j ) {
			s[i][j] = 0.0;
			for ( k=0; k<4; ++k ) {
				s[i][j] += pre[i][k] * post[k][j];
			}
		}
	}
	memcpy(result, s, sizeof(s));
}

	
	static RtFloat det2x2(RtFloat a, RtFloat b, RtFloat c, RtFloat d)
	{
		return (a * d - b * c);
	}
	
	static RtFloat det3x3(RtFloat a1, RtFloat a2, RtFloat a3,
						  RtFloat b1, RtFloat b2, RtFloat b3,
						  RtFloat c1, RtFloat c2, RtFloat c3)
	{
		return
		a1 * det2x2(b2, b3, c2, c3) -
		b1 * det2x2(a2, a3, c2, c3) +
		c1 * det2x2(a2, a3, b2, b3);
	}
	
	static bool isIdentity(const RtMatrix matrix)
	{
        int i, j;
        bool eq = true;
		
        for ( i = 0; i < 4 ; ++i ) {
			for ( j = 0; j < 4 ; ++j ) {
				eq = eq && ((matrix[i][j] == 0.0) || ((i==j) && (matrix[i][j] == 1.0)));
			}
        }
		
        return eq;
	}
	
	static void setIdentity(RtMatrix matrix)
	{
        int i, j;
		
        for ( i = 0; i < 4 ; ++i ) {
			for ( j = 0; j < 4 ; ++j ) {
				matrix[i][j] =  0;
			}
        }
        for ( i = 0; i < 4 ; ++i ) {
			matrix[i][i] = 1;
        }
	}
	
RICPP_INTERN(RtFloat) RiCPPDeterminant(RtMatrix matrix)
{
	// Assign to individual variable names to aid selecting correct elements
	RtFloat a1 = matrix[0][0];
	RtFloat b1 = matrix[0][1];
	RtFloat c1 = matrix[0][2];
	RtFloat d1 = matrix[0][3];
		
	RtFloat a2 = matrix[1][0];
	RtFloat b2 = matrix[1][1];
	RtFloat c2 = matrix[1][2];
	RtFloat d2 = matrix[1][3];
		
	RtFloat a3 = matrix[2][0];
	RtFloat b3 = matrix[2][1];
	RtFloat c3 = matrix[2][2];
	RtFloat d3 = matrix[2][3];
		
	RtFloat a4 = matrix[3][0];
	RtFloat b4 = matrix[3][1];
	RtFloat c4 = matrix[3][2];
	RtFloat d4 = matrix[3][3];
		
	return
	a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
	b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
	c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
	d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
}
	
	
RICPP_INTERN(RtBoolean)
RiCPPInvertMatrix (const RtMatrix matrix, RtMatrix inverse)
{
	if ( isIdentity(matrix) )
	{
		memcpy(inverse, matrix, sizeof(matrix));
		return RI_TRUE;
	}
	
	RtMatrix b; // b evolves from identity into inverse(a)
	RtMatrix a; // a evolves from original matrix into identity
	memcpy(a, matrix, sizeof(matrix));
	
	setIdentity(b);
	
	int i;
	int j;
	int i1;
	
	// Loop over cols of a from left to right, eliminating above and below diag
	for (j = 0; j < 4; j++)        // Find largest pivot in column j among rows j..3
	{
		i1 = j;
		for(i = j + 1; i < 4; i++)
		{
			if(fabs(a[i][j]) > fabs(a[i1][j]))
			{
				i1 = i;
			}
		}
		
		if (i1 != j)
		{
			// Swap rows i1 and j in a and b to put pivot on diagonal
			RtFloat temp;
			
			temp = a[i1][0];
			a[i1][0] = a[j][0];
			a[j][0] = temp;
			temp = a[i1][1];
			a[i1][1] = a[j][1];
			a[j][1] = temp;
			temp = a[i1][2];
			a[i1][2] = a[j][2];
			a[j][2] = temp;
			temp = a[i1][3];
			a[i1][3] = a[j][3];
			a[j][3] = temp;
			
			temp = b[i1][0];
			b[i1][0] = b[j][0];
			b[j][0] = temp;
			temp = b[i1][1];
			b[i1][1] = b[j][1];
			b[j][1] = temp;
			temp = b[i1][2];
			b[i1][2] = b[j][2];
			b[j][2] = temp;
			temp = b[i1][3];
			b[i1][3] = b[j][3];
			b[j][3] = temp;
		}
		
		// Scale row j to have a unit diagonal
		if( a[j][j] == 0.0 )
		{
			// Can't invert a singular matrix!
			return RI_FALSE;
		}
		RtFloat scale = (RtFloat)(1.0 / a[j][j]);
		b[j][0] *= scale;
		b[j][1] *= scale;
		b[j][2] *= scale;
		b[j][3] *= scale;
		// all elements to left of a[j][j] are already zero
		for (i1=j+1; i1<4; i1++)
		{
			a[j][i1] *= scale;
		}
		a[j][j] = 1.0;
		
		// Eliminate off-diagonal elements in column j of a, doing identical ops to b
		for(i = 0; i < 4; i++)
		{
			if(i != j)
			{
				scale = a[i][j];
				b[i][0] -= scale * b[j][0];
				b[i][1] -= scale * b[j][1];
				b[i][2] -= scale * b[j][2];
				b[i][3] -= scale * b[j][3];
				
				// all elements to left of a[j][j] are zero
				// a[j][j] is 1.0
				for (i1=j+1; i1<4; i1++)
				{
					a[i][i1] -= scale * a[j][i1];
				}
				a[i][j] = 0.0;
			}
		}
	}

	memcpy(inverse, b, sizeof(b));
	return RI_TRUE;
}

	
	
/*   */
#undef RICPP_INTERN
#define RICPP_INTERN(type) type

// ----------------------------------------------------------------------------
RICPP_INTERN(RtInt) RiLastError = RIE_NOERROR;
	
// ----------------------------------------------------------------------------

RICPP_INTERN(const RtMatrix) RiIdentityMatrix = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(RtContextHandle const) illContextHandle = static_cast<RtContextHandle>(0);
RICPP_INTERN(const RtObjectHandle) illObjectHandle = static_cast<RtObjectHandle>(0);
RICPP_INTERN(RtLightHandle const) illLightHandle = static_cast<RtLightHandle>(0);
RICPP_INTERN(RtArchiveHandle const) illArchiveHandle = static_cast<RtArchiveHandle>(0);

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtBoolean) RI_FALSE       = 0;   /**<  false */
RICPP_INTERN(const RtBoolean) RI_TRUE        = 1;   /**<  true  */

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtFloat) RI_INFINITY      = static_cast<RtFloat>(1.0e38);
RICPP_INTERN(const RtFloat) RI_EPSILON       = static_cast<RtFloat>(1.0e-10);

/* --------------------------------------------------------------------------------------------------- */

RICPP_INTERN(const RtInt) RI_BEZIERSTEP      = 3;
RICPP_INTERN(const RtInt) RI_BSPLINESTEP     = 1;
RICPP_INTERN(const RtInt) RI_CATMULLROMSTEP  = 1;
RICPP_INTERN(const RtInt) RI_HERMITESTEP     = 2;
RICPP_INTERN(const RtInt) RI_POWERSTEP       = 4;

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


	RICPP_INTERN(RtToken const) RI_CATMULL_CLARK = "catmull-clark";
RICPP_INTERN(RtToken const) RI_HOLE = "hole";
RICPP_INTERN(RtToken const) RI_CREASE = "crease";
RICPP_INTERN(RtToken const) RI_CORNER = "corner";
RICPP_INTERN(RtToken const) RI_INTERPOLATEBOUNDARY = "interpolateboundary";
RICPP_INTERN(RtToken const) RI_FACEINTERPOLATEBOUNDARY = "faceinterpolateboundary";
	
RICPP_INTERN(RtToken const) RI_VERTEXEDIT = "vertexedit";
RICPP_INTERN(RtToken const) RI_EDGEEDIT = "edgeedit";
RICPP_INTERN(RtToken const) RI_FACEEDIT = "faceedit";
RICPP_INTERN(RtToken const) RI_ADD = "add";
RICPP_INTERN(RtToken const) RI_SET = "set";
RICPP_INTERN(RtToken const) RI_VALUE = "value";
RICPP_INTERN(RtToken const) RI_SHARPNESS = "sharpness";
RICPP_INTERN(RtToken const) RI_ATTRIBUTES = "attributes";
	
RICPP_INTERN(RtToken const) RI_ORIGIN = "origin";
RICPP_INTERN(RtToken const) RI_NAME = "name";

RICPP_INTERN(RtToken const) RI_COMMENT = "comment";
RICPP_INTERN(RtToken const) RI_STRUCTURE = "structure";
RICPP_INTERN(RtToken const) RI_VERBATIM = "verbatim";

RICPP_INTERN(RtToken const) RI_LINEAR = "linear";
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
RICPP_INTERN(RtToken const) RI_HERMITE = "hermite";
RICPP_INTERN(RtToken const) RI_POWER = "power";

RICPP_INTERN(RtToken const) RI_RESTART = "restart";
RICPP_INTERN(RtToken const) RI_TEAPOT = "teapot";

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
