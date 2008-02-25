#ifndef _RICPP_RICPP_RICPPCONST_H
#define _RICPP_RICPP_RICPPCONST_H

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

/** @file ricppconst.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Constants of the RenderMan(R) interface
 */

#ifndef _RICPP_RICPP_RICPPTYPES_H
#include "ricpp/ricpp/ricpptypes.h"
#endif _RICPP_RICPP_RICPPTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace RiCPP {
#endif

const RtMatrix RiIdentityMatrix = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
}; ///< Identity matrix (added)

// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------


/** @defgroup ricpp_illhandle Illegal handles (added)
 *  @brief Definitions of illegal handles (additional constants)
 *  @{
 */
const RtContextHandle illContextHandle = (RtContextHandle)0; ///< illegal context handle
const RtObjectHandle illObjectHandle = (RtObjectHandle)0; ///< illegal object handle
const RtLightHandle illLightHandle = (RtLightHandle)0; ///< illegal light source handle
const RtArchiveHandle illArchiveHandle = (RtArchiveHandle)0; ///< illegal archive handle
//@}


// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------

/** @defgroup ricpp_booleans Ri booleans
 *  @brief Boolean true, false values
 *  @{
 */
const RtBoolean RI_FALSE       = 0;           ///< false
const RtBoolean RI_TRUE        = 1;           ///< true
//@}

/** @defgroup ricpp_limits Ri limits
 *  @brief Limits for clipping planes
 *  @{
 */
const RtFloat RI_INFINITY      = (RtFloat)(1.0e38);  ///< Maximal value used for RtFloat
const RtFloat RI_EPSILON       = (RtFloat)(1.0e-10); ///< Minimal value used for RtFloat 
/*
 Other <limits>
 std::numeric_limits<RtFloat>::max()
 std::numeric_limits<RtFloat>::epsilon() minimal |epsilon| for v+epsilon != v
 */
//@}

/** @defgroup ricpp_steps Ri stepsizes for bicubic patch meshes
 *  @brief Step sizes (correspondence to the basis matrices of the bicubic patches)
 *  @see CRiCPP::basis(), ricpp_basis
 *  @{
 */
const RtInt RI_BEZIERSTEP      = 3; ///< Stepsize for control points of bezier meshes
const RtInt RI_BSPLINESTEP     = 1; ///< Stepsize for control points of b-spline meshes
const RtInt RI_CATMULLROMSTEP  = 1; ///< Stepsize for control points of catmull-rom spline meshes
const RtInt RI_HERMITESTEP     = 2; ///< Stepsize for control points of hermite spline meshes
const RtInt RI_POWERSTEP       = 4; ///< Stepsize for control points of power matrix spline meshes
//@}

/** @defgroup ricpp_basis Ri basis matrices for bicubic patches
 *  @brief Basis matrices for bicubic patches
 *  @see CRiCPP::basis(), ricpp_steps
 *  @{
 */
const RtBasis RiBezierBasis = {
	{(RtFloat)-1.0, (RtFloat) 3.0, (RtFloat)-3.0, (RtFloat) 1.0},
	{(RtFloat) 3.0, (RtFloat)-6.0, (RtFloat) 3.0, (RtFloat) 0.0},
	{(RtFloat)-3.0, (RtFloat) 3.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
}; ///< Basis matrix for bezier patch meshes
const RtBasis RiBSplineBasis = {
	{(RtFloat)(-1.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat)(-3.0/6.0), (RtFloat)(1.0/6.0)},
	{(RtFloat)( 3.0/6.0), (RtFloat)(-6.0/6.0), (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)(-3.0/6.0), (RtFloat)  0.0,      (RtFloat)( 3.0/6.0), (RtFloat) 0.0},
	{(RtFloat)( 1.0/6.0), (RtFloat)( 4.0/6.0), (RtFloat)( 1.0/6.0), (RtFloat) 0.0}
}; ///< Basis matrix B-spline patch meshes
const RtBasis RiCatmullRomBasis = {
	{(RtFloat)(-1.0/2.0), (RtFloat)( 3.0/2.0), (RtFloat)(-3.0/2.0), (RtFloat)( 1.0/2.0)},
	{(RtFloat)( 2.0/2.0), (RtFloat)(-5.0/2.0), (RtFloat)( 4.0/2.0), (RtFloat)(-1.0/2.0)},
	{(RtFloat)(-1.0/2.0), (RtFloat)  0.0,      (RtFloat)( 1.0/2.0), (RtFloat)  0.0},
	{(RtFloat)  0.0,      (RtFloat)( 2.0/2.0), (RtFloat)  0.0,      (RtFloat)  0.0}
}; ///< Basis matrix Catmull-Rom patch meshes
const RtBasis RiHermiteBasis = {
	{(RtFloat) 2.0, (RtFloat) 1.0, (RtFloat)-2.0, (RtFloat) 1.0},
	{(RtFloat)-3.0, (RtFloat)-2.0, (RtFloat) 3.0, (RtFloat)-1.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0}
}; ///< Basis matrix Hermite patch meshes
const RtBasis RiPowerBasis = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
}; ///< Basis matrix Power-basis patch meshes
//@}

/** @defgroup ricpp_blobbyconst Ri constants for CRiRoot::blobbyV() (added)
 *  @brief Added constants for CRiRoot::blobbyV(): Primitives always followed by an index to the (float) operands
 *  @see CRiRoot::blobbyV(), ricpp_blobbyop
 *  @{
 */
const RtInt RI_BLOBBY_CONSTANT        = 1000; ///< 1 index to  1 float
const RtInt RI_BLOBBY_ELLIPSOID       = 1001; ///< 1 index to 16 floats
const RtInt RI_BLOBBY_SEGMENT_BLOB    = 1002; ///< 1 index to 23 floats
const RtInt RI_BLOBBY_REPELLING_PLANE = 1003; ///< 2 Indices, first for 1 string param, 2nd for 4 floats
//@}


/** @defgroup ricpp_blobbyop Ri constants for CRiRoot::blobbyV() operations (added)
 *  @brief Added constants for IRi::blobby() Operations, Substract (4) and Divide (5) are exchanged - seems to be a bug in the [RISPEC]
 *  @see CRiRoot::blobbyV(), ricpp_blobbyconst
 *  @{
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

/** @defgroup ricpp_errorconst Ri error constants
 *  @brief Error codes.
 *
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
 *
 *  @see CRiCPP::errorHandler(), IErrorHandler, ricpp_severity
 * @{
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
const RtInt RIE_LASTERROR      = 61;       ///< Endmarker, not an error code (added)
//@}

/** @defgroup ricpp_severity Ri error severity
 *  @brief Error severity levels (0-3)
 *  @see CRiCPP::errorHandler(), IErrorHandler, ricpp_errorconst
 *  @{
 */
const RtInt RIE_INFO           =  0;       ///< Rendering stats & other info
const RtInt RIE_WARNING        =  1;       ///< Something seems wrong, maybe okay
const RtInt RIE_ERROR          =  2;       ///< Problem.  Results may be wrong
const RtInt RIE_SEVERE         =  3;       ///< So bad you should probably abort
const RtInt RIE_LASTSEVERITY   =  3;       ///< Endmarker, not a severity code (added)
//@}

#ifdef __cplusplus
} // namespace RiCPP
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _RICPP_RICPP_RICPPCONST_H
