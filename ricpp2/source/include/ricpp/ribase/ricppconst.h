#ifndef _RICPP_RIBASE_RICPPCONST_H
#define _RICPP_RIBASE_RICPPCONST_H

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

/** @file ricppconst.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Constants of the RenderMan(R) interface
 */

#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif /* _RICPP_RIBASE_RICPPTYPES_H */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
/* namespace RiCPP { /* */
#endif

RICPP_EXTERN(const RtMatrix) RiIdentityMatrix; /**< Identity matrix (added) */

/* ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------
*/

/** @defgroup ricpp_illhandle Illegal handles (added)
 *  @brief Definitions of illegal handles (additional constants)
 *  @{
 */
RICPP_EXTERN(RtContextHandle const) illContextHandle; /**< illegal context handle */
RICPP_EXTERN(const RtObjectHandle)  illObjectHandle;  /**< illegal object handle */
RICPP_EXTERN(RtLightHandle const)   illLightHandle;   /**< illegal light source handle */
RICPP_EXTERN(RtArchiveHandle const) illArchiveHandle; /**< illegal archive handle */
/** @} */


/* ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------
*/

/** @defgroup ricpp_booleans Ri booleans
 *  @brief Boolean true, false values
 *  @{
 */
RICPP_EXTERN(const RtBoolean) RI_FALSE;           /**<  false */
RICPP_EXTERN(const RtBoolean) RI_TRUE;           /**<  true  */
/** @} */

/** @defgroup ricpp_limits Ri limits
 *  @brief Limits for clipping planes
 *  @{
 */
RICPP_EXTERN(const RtFloat) RI_INFINITY;  /**< Maximal value used for RtFloat */
RICPP_EXTERN(const RtFloat) RI_EPSILON; /**< Minimal value used for RtFloat */
 /*
 Note: Other consts in <limits>
 std::numeric_limits<RtFloat>::max()
 std::numeric_limits<RtFloat>::epsilon() minimal |epsilon| for v+epsilon != v
 */
/** @} */

/** @defgroup ricpp_steps Ri stepsizes for bicubic patch meshes
 *  @brief Step sizes (correspondence to the basis matrices of the bicubic patches)
 *  @see CRiCPP::basis(), ricpp_basis
 *  @{
 */
RICPP_EXTERN(const RtInt) RI_BEZIERSTEP;     /**< Stepsize for control points of bezier meshes */
RICPP_EXTERN(const RtInt) RI_BSPLINESTEP;    /**< Stepsize for control points of b-spline meshes */
RICPP_EXTERN(const RtInt) RI_CATMULLROMSTEP; /**< Stepsize for control points of catmull-rom spline meshes */
RICPP_EXTERN(const RtInt) RI_HERMITESTEP;    /**< Stepsize for control points of hermite spline meshes */
RICPP_EXTERN(const RtInt) RI_POWERSTEP;      /**< Stepsize for control points of power matrix spline meshes */
/** @} */

/** @defgroup ricpp_basis Ri basis matrices for bicubic patches
 *  @brief Basis matrices for bicubic patches
 *  @see CRiCPP::basis(), ricpp_steps
 *  @{
 */
RICPP_EXTERN(const RtBasis) RiBezierBasis; /**< Basis matrix for bezier patch meshes */
RICPP_EXTERN(const RtBasis) RiBSplineBasis; /**<  Basis matrix B-spline patch meshes */
RICPP_EXTERN(const RtBasis) RiCatmullRomBasis; /**<  Basis matrix Catmull-Rom patch meshes */
RICPP_EXTERN(const RtBasis) RiHermiteBasis; /**<  Basis matrix Hermite patch meshes */
RICPP_EXTERN(const RtBasis) RiPowerBasis; /**<  Basis matrix Power-basis patch meshes */
/** @} */

/** @defgroup ricpp_blobbyconst Ri constants for CRiRoot::blobbyV() (added)
 *  @brief Added constants for CRiRoot::blobbyV(): Primitives always followed by an index to the (float) operands
 *  @see CRiRoot::blobbyV(), ricpp_blobbyop
 *  @{
 */
RICPP_EXTERN(const RtInt) RI_BLOBBY_CONSTANT;        /**< 1 index to  1 float */
RICPP_EXTERN(const RtInt) RI_BLOBBY_ELLIPSOID;       /**<  1 index to 16 floats */
RICPP_EXTERN(const RtInt) RI_BLOBBY_SEGMENT_BLOB;    /**<  1 index to 23 floats */
RICPP_EXTERN(const RtInt) RI_BLOBBY_REPELLING_PLANE; /**<  2 Indices, first for 1 string param, 2nd for 4 floats */
/** @} */


/** @defgroup ricpp_blobbyop Ri constants for CRiRoot::blobbyV() operations (added)
 *  @brief Added constants for IRi::blobby() Operations, Substract (4) and Divide (5) are exchanged - seems to be a bug in the [RISPEC]
 *  @see CRiRoot::blobbyV(), ricpp_blobbyconst
 *  @{
 */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_ADD; /**<  Addition:  count, ... */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_MUL; /**<  Multiply:  count, ... */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_MAX; /**<  Maximum:   count, ... */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_MIN; /**<  Minimum:   count, ... */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_SUB; /**<  Substract: subtrahend, minuend */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_DIV; /**<  Divide:    dividend, divisor */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_NEG; /**<  Negate:    negand */
RICPP_EXTERN(const RtInt) RI_BLOBBY_OP_IDP; /**<  Identity:  idempotentate */
/** @} */

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
RICPP_EXTERN(const RtInt) RIE_NOERROR;       /**< No error occured */

RICPP_EXTERN(const RtInt) RIE_NOMEM;       /**< Out of memory */
RICPP_EXTERN(const RtInt) RIE_SYSTEM;       /**< Miscellaneous system error */
RICPP_EXTERN(const RtInt) RIE_NOFILE;       /**< File not exists */
RICPP_EXTERN(const RtInt) RIE_BADFILE;       /**< Bad file format */
RICPP_EXTERN(const RtInt) RIE_VERSION;       /**< File version mismatch */
RICPP_EXTERN(const RtInt) RIE_DISKFULL;       /**< Target disk is full */

RICPP_EXTERN(const RtInt) RIE_INCAPABLE;       /**< Optional RI feature */
RICPP_EXTERN(const RtInt) RIE_UNIMPLEMENT;       /**< Unimplemented feature */
RICPP_EXTERN(const RtInt) RIE_LIMIT;       /**< Arbitrary program limit */
RICPP_EXTERN(const RtInt) RIE_BUG;       /**< Probably a bug in renderer */

RICPP_EXTERN(const RtInt) RIE_NOTSTARTED;       /**< 'RiBegin' not called */
RICPP_EXTERN(const RtInt) RIE_NESTING;       /**< Bad begin-end nesting */
RICPP_EXTERN(const RtInt) RIE_NOTOPTIONS;       /**< Invalid state for options */
RICPP_EXTERN(const RtInt) RIE_NOTATTRIBS;       /**< Invalid state for attributes */
RICPP_EXTERN(const RtInt) RIE_NOTPRIMS;       /**< Invalid state for primitives */
RICPP_EXTERN(const RtInt) RIE_ILLSTATE;       /**< Other invalid state */
RICPP_EXTERN(const RtInt) RIE_BADMOTION;       /**< Badly formed motion block */
RICPP_EXTERN(const RtInt) RIE_BADSOLID;       /**< Badly formed solid block */

RICPP_EXTERN(const RtInt) RIE_BADTOKEN;       /**< Invalid token for request */
RICPP_EXTERN(const RtInt) RIE_RANGE;       /**< Parameter out of range */
RICPP_EXTERN(const RtInt) RIE_CONSISTENCY;       /**< Parameters inconsistent */
RICPP_EXTERN(const RtInt) RIE_BADHANDLE;       /**< Bad object/light/context handle */
RICPP_EXTERN(const RtInt) RIE_NOSHADER;       /**< Can't load requested shader */
RICPP_EXTERN(const RtInt) RIE_MISSINGDATA;       /**< Required parameters not provided */
RICPP_EXTERN(const RtInt) RIE_SYNTAX;       /**< Declare type syntax error */

RICPP_EXTERN(const RtInt) RIE_MATH;       /**< Zerodivide, noninvert matrix, etc. */
RICPP_EXTERN(const RtInt) RIE_LASTERROR;       /**< Endmarker, not an error code (added) */
/** @} */

/** @defgroup ricpp_severity Ri error severity
 *  @brief Error severity levels (0-3)
 *  @see CRiCPP::errorHandler(), IErrorHandler, ricpp_errorconst
 *  @{
 */
RICPP_EXTERN(const RtInt) RIE_INFO;       /**< Rendering stats & other info */
RICPP_EXTERN(const RtInt) RIE_WARNING;       /**< Something seems wrong, maybe okay */
RICPP_EXTERN(const RtInt) RIE_ERROR;       /**< Problem.  Results may be wrong */
RICPP_EXTERN(const RtInt) RIE_SEVERE;       /**< So bad you should probably abort */
RICPP_EXTERN(const RtInt) RIE_LASTSEVERITY;       /**< Endmarker, not a severity code (added) */
/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
/* } /* namespace RiCPP */
#endif

#endif /* _RICPP_RIBASE_RICPPCONST_H */
