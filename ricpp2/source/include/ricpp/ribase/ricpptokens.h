#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#define _RICPP_RIBASE_RICPPTOKENS_H

/*
// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:	/** @brief Defines a new display channel for display()
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

/** @file ricpptokens.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Tokens of the RenderMan(R) interface
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

RICPP_EXTERN(RtToken) RI_NULL; /**< Used to mark undefined tokens and end marker of parameter list. */

RICPP_EXTERN(RtToken) RI_FRAMEBUFFER, /**< Framebuffer output device as @a type in @c  CRiRoot::display() */
              RI_FILE; /**< File output as @a type in @c CRiRoot::display() */

RICPP_EXTERN(RtToken)  RI_RGB, /**< Output RGB (red green blue) channels for @a mode in @c CRiRoot::display() */
               RI_RGBA, /**< Output RGBA (RGB + alpha opacitiy) channels for @a mode in @c CRiRoot::display() */
			   RI_RGBZ, /**< Output RGBZ (RGB + depth) channels for @a mode in @c CRiRoot::display() */
			   RI_RGBAZ, /**< Output RGBAZ channels for @a mode in @c CRiRoot:::display() */
               RI_A, /**< Alpha (opacity) channel for @a mode in @c CRiRoot::display() */
			   RI_Z, /**< Depth channel for @a mode in @c CRiRoot::display() */
			   RI_AZ; /**< Alpha and Depth channels for @a mode in @c CRiRoot::display() */

RICPP_EXTERN(RtToken)  RI_PERSPECTIVE, /**< Perspective projection for parameter @a name for CRiRoot::projectionV() */
               RI_ORTHOGRAPHIC; /**< Orthographic projection for parameter @a name for CRiRoot::projectionV() */

RICPP_EXTERN(RtToken)  RI_HIDDEN, /**< Standard hider 'hidden' for CRiRoot::hiderV() */
               RI_PAINT; /**< Standard hider painters algorithm for CRiRoot::hiderV() */

RICPP_EXTERN(RtToken)  RI_CONSTANT, /**< Constant shading parameter @a type for CRiRoot::shadingInterpolation() */
               RI_SMOOTH; /**< Smooth shading parameter @a type for CRiRoot::shadingInterpolation() */

RICPP_EXTERN(RtToken)  RI_FLATNESS; /**< Standard approximation criterion for CRiRoot::geometricApproximation() */

RICPP_EXTERN(RtToken)  RI_FOV; /**< Field of view value for perspective projection @see RI_PERSPECTIVE, CRiRoot::projectionV() */

RICPP_EXTERN(RtToken)  RI_AMBIENTLIGHT, /**< Ambient light source shader CRiRoot::lightSourceV() */
               RI_POINTLIGHT, /**< Point light source shader CRiRoot::lightSourceV() */
               RI_DISTANTLIGHT, /**< Distant light source shader CRiRoot::lightSourceV() */
			   RI_SPOTLIGHT; /**< Spotlight source shader CRiRoot::lightSourceV() */

RICPP_EXTERN(RtToken)  RI_INTENSITY, /**< Intensity (RtFloat) parameter of a lightsource */
               RI_LIGHTCOLOR, /**< Lightcolor (RtColor) parameter of a lightsource */
			   RI_FROM, /**< From position (RtPoint) parameter of a lightsource */
			   RI_TO, /**< Direction (RtVector) parameter of a lightsource (not point light) */
			   RI_CONEANGLE, /**< Cone angle (RtFloat) parameter of a spotlight */
               RI_CONEDELTAANGLE, /**< Cone delta angle (RtFloat) parameter of a spotlight */
			   RI_BEAMDISTRIBUTION; /**< Beamdistribution (RtFloat) parameter of a spotlight */

RICPP_EXTERN(RtToken)  RI_MATTE, /**< Matte surface shader */
               RI_METAL, /**< Metal surface shader */
			   RI_SHINYMETAL, /**< Shiny metal surface shader */
               RI_PLASTIC, /**< Plastic surface shader */
			   RI_PAINTEDPLASTIC; /**< Painted plastic surface shader */

RICPP_EXTERN(RtToken)  RI_KA, /**< Ambient terms for the reflection formular of a surface */
               RI_KD, /**< Diffuse terms for the reflection formular of a surface */
			   RI_KS, /**< Specular terms for the reflection formular of a surface */
			   RI_ROUGHNESS, /**< Roughness (controls size of the highlight) for the specular reflection of a surface */
			   RI_KR, /**< Term for mix in environment color (shiny metal) */
               RI_TEXTURENAME, /**< Texturname of a surface (e.g. RI_SHINYMETAL or RI_PAINTEDPLASTIC) */
			   RI_SPECULARCOLOR; /**< Color for the hightlight of a specular reflection of a surface  */

RICPP_EXTERN(RtToken)  RI_DEPTHCUE, /**< Depth cue volume shader */
               RI_FOG; /**< Fog volume shader */

RICPP_EXTERN(RtToken)  RI_BUMPY; /**< Bumpy displacement shader */

RICPP_EXTERN(RtToken)  RI_MINDISTANCE, /**< Minimal distance of surface where RI_DEPTHCUE is applied  (all surfaces nearer get no background color mixed in) */
               RI_MAXDISTANCE, /**< Maximal distance of surface where RI_DEPTHCUE is applied (all surfaces farer away get background color) */
			   RI_BACKGROUND, /**< Background color the RI_DEPTHCUE and RI_FOG atmosphere shader mixes in */
               RI_DISTANCE, /**< controls the attenuation of the surface color in RI_FOG shader */
			   RI_AMPLITUDE; /**< Controls (amplifies) the amplitude of bump maps in bumpy displacement shader */

RICPP_EXTERN(RtToken)  RI_RASTER, /**< Name of the raster coordinate system */
			   RI_SCREEN, /**< Name of the screen coordinate system */
			   RI_CAMERA, /**< Name of the camera coordinate system */
               RI_WORLD, /**< Name of the world coordinate system */
			   RI_OBJECT; /**< Name of the object coordinate system */

RICPP_EXTERN(RtToken)  RI_INSIDE, /**< Inside orientation @see CRiRoot::orientation() */
               RI_OUTSIDE, /**< Outside orientation @see CRiRoot::orientation() */
			   RI_LH, /**< Concrete left handed orientation @see CRiRoot::orientation() */
			   RI_RH; /**< Concrete right handed orientation @see CRiRoot::orientation() */

RICPP_EXTERN(RtToken)  RI_P, /**< Token for position parameter */
               RI_PZ, /**< Token for depth parameter */
			   RI_PW, /**< Token for homogenous points parameter */
			   RI_N, /**< Token for normal vector parameter */
			   RI_NP, /**< Token for normal vector followed by position parameter */
			   RI_CS, /**< Token for surface color */
               RI_OS, /**< Token for surface opacity */
			   RI_S, /**< Token for s texture coordinate */
			   RI_T, /**< Token for t texture coordinate */
			   RI_ST; /**< Token for s folowed by t texture coordinate */

RICPP_EXTERN(RtToken)  RI_BILINEAR, /**< Bilinear patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV() */
               RI_BICUBIC; /**< Bicubic patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV() */

RICPP_EXTERN(RtToken)  RI_PRIMITIVE, /**< Primitive solid (leaf knot), also used as geometric representation @see RiCPP::solidBegin(), RiCPP::solidEnd(), RiCPP::geometricRepresentation() */
               RI_INTERSECTION, /**< Intersection of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */
			   RI_UNION, /**< Union of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */
			   RI_DIFFERENCE; /**< Difference of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */

RICPP_EXTERN(RtToken)  RI_PERIODIC, /**< Periodic wrap of a patch meshes, curves or texture coordinates @see RiCPPRoot::patchMeshV(), ricpp_texture,  CRiCPP::curvesV() */
               RI_NONPERIODIC, /**< Non-periodic wrap of a patch meshes or curves @see RiCPPRoot::patchMeshV(), CRiCPP::curvesV() */
               RI_CLAMP, /**< Clamping textures coordinates to their borders @see ricpp_texture */
			   RI_BLACK; /**< Leaving textels outside their borders black @see ricpp_texture */

RICPP_EXTERN(RtToken)  RI_IGNORE, /**< Token indicates the ignore error handler */
               RI_PRINT, /**< Token indicates the print error handler */
			   RI_ABORT, /**< Token indicates the abort error handler */
			   RI_HANDLER; /**< General token for handler */


/** @defgroup ricpp_tokens_add Additional tokens
 *  @ingroup ricpp_tokens
 *  @brief Additional tokens, used by RiCPP
 *  @{
 */
RICPP_EXTERN(RtToken)  RI_CATMULLCLARK, /**< Name of Catmull-Clark subdivision surface @see CRiCPP::subdivisionMeshV() */
               RI_HOLE, /**< Hole in subdivision surface @see CRiCPP::subdivisionMeshV() */
			   RI_CREASE, /**< Sharp edge in subdivision surface @see CRiCPP::subdivisionMeshV() */
			   RI_CORNER, /**< Corner in subdivision surface @see CRiCPP::subdivisionMeshV() */
			   RI_INTERPOLATEBOUNDARY; /**< Interpolate the boundery (instead of approximate) subdivision surface @see CRiCPP::subdivisionMeshV() */

RICPP_EXTERN(RtToken)  RI_ORIGIN, /**< Token for an origin parameter @see CRiCPP::displayV() */
			   RI_NAME; /**< Token for a name parameter */

RICPP_EXTERN(RtToken)  RI_COMMENT, /**< Comment for CRiCPP::ArchiveRecord() */
               RI_STRUCTURE, /**< Structuredcomment for CRiCPP::ArchiveRecord() */
			   RI_VERBATIM; /**< Verbatim output for CRiCPP::ArchiveRecord() */

RICPP_EXTERN(RtToken)  RI_LINEAR, /**< Linear curves @see CRiCPP::curvesV() */
               RI_CUBIC; /**< Cubic curves @see CRiCPP::curvesV() */

RICPP_EXTERN(RtToken)  RI_WIDTH,
			   RI_CONSTANTWIDTH;

RICPP_EXTERN(RtToken) RI_DISPWIDTH,
	           RI_DISPHEIGHT,
	           RI_PIXELASPECT;

RICPP_EXTERN(RtToken)  RI_CURRENT, /**< Current coordinate system */
			   RI_EYE, /**<  Eye coordinate system */
			   RI_SHADER, /**<  Shader coordinate system */
               RI_NDC; /**<  NDC coordinate system */

RICPP_EXTERN(RtToken)  RI_FILEPOINTER, /**< Input/Output to a file FILE * (resource) */
               RI_MEMORY; /**< Input from memory resource, not allowed for RIB */

/** @todo Maybe named pipe as PIPE:pipename as normal filename, already |cmd for anonymous pipe output, | for stdio */

RICPP_EXTERN(RtToken) RI_ARCHIVE, /**< archive 'renderer' type (@a IRiContext::rendererType()) to archive RIB code, or archive qualifier */
              RI_DRAFT, /**< fast draft renderer type (@a IRiContext::rendererType()) with limited implementation of the RI */
              RI_REALISTIC; /**< photo realistic renderer type (@a IRiContext::rendererType())  implementing the full RI */

RICPP_EXTERN(RtToken) RI_FLOAT; /**< Name for type RtFloat */
RICPP_EXTERN(RtToken) RI_INTEGER; /**< Name for type RtInteger */
RICPP_EXTERN(RtToken) RI_STRING; /**< Name for type RtString */
RICPP_EXTERN(RtToken) RI_POINT; /**< Name for type RtPoint */
RICPP_EXTERN(RtToken) RI_VECTOR; /**< Name for type RtVector */
RICPP_EXTERN(RtToken) RI_NORMAL; /**< Name for type RtNormal */
RICPP_EXTERN(RtToken) RI_HPOINT; /**< Name for type RtHpoint */
RICPP_EXTERN(RtToken) RI_MATRIX; /**< Name for type RtMatrix */
RICPP_EXTERN(RtToken) RI_COLOR; /**< Name for type RtColor */

/* RICPP_EXTERN(RtToken) RI_CONSTANT; // already a RtToken */
RICPP_EXTERN(RtToken) RI_UNIFORM; /**< Name for storage class uniform */
RICPP_EXTERN(RtToken) RI_VARYING; /**< Name for storage class varying */
RICPP_EXTERN(RtToken) RI_VERTEX; /**< Name for storage class vertex */
RICPP_EXTERN(RtToken) RI_FACEVARYING; /**< Name for storage class facevarying */
RICPP_EXTERN(RtToken) RI_FACEVERTEX; /**< Name for storage class facevertex */

RICPP_EXTERN(RtToken) RI_HANDLEID; /**< Handle id */

RICPP_EXTERN(RtToken) RI_PROJECTION; /**< Projection qualifier */
RICPP_EXTERN(RtToken) RI_IMAGER; /**< Imager qualifier */
RICPP_EXTERN(RtToken) RI_DISPLAY; /**< Display qualifier */
RICPP_EXTERN(RtToken) RI_HIDER; /**< Hider qualifier */
RICPP_EXTERN(RtToken) RI_OPTION; /**< Option qualifier */
RICPP_EXTERN(RtToken) RI_LIGHT_SOURCE; /**< Light source qualifier */
RICPP_EXTERN(RtToken) RI_AREA_LIGHT_SOURCE; /**< Area light source qualifier */
RICPP_EXTERN(RtToken) RI_SURFACE; /**< Surface shader qualifier */
RICPP_EXTERN(RtToken) RI_ATMOSPHERE; /**< Atmosphere shader qualifier */
RICPP_EXTERN(RtToken) RI_INTERIOR; /**< Interior shader qualifier */
RICPP_EXTERN(RtToken) RI_EXTERIOR; /**< Exterior shader qualifier */
RICPP_EXTERN(RtToken) RI_DISPLACEMENT; /**< Displacement shader qualifier */
RICPP_EXTERN(RtToken) RI_DEFORMATION; /**< Deformation shader qualifier */
RICPP_EXTERN(RtToken) RI_ATTRIBUTE; /**< Attribute qualifier */
RICPP_EXTERN(RtToken) RI_TEXTURE; /**< Texture qualifier */
RICPP_EXTERN(RtToken) RI_CONTROL; /**< Control qualifier */
RICPP_EXTERN(RtToken) RI_BEGIN; /**< qualifier for Begin statement */
RICPP_EXTERN(RtToken) RI_RESOURCE; /**< Resource qualifier */

RICPP_EXTERN(RtToken) RI_FRAME; /**< Frame number */

RICPP_EXTERN(RtToken) RI_DEVIATION; /**< Type for RiCPP::geometricApproximation() */
RICPP_EXTERN(RtToken) RI_TESSELATION; /**< Type for RiCPP::geometricApproximation() */

RICPP_EXTERN(RtToken) RI_POINTS; /**< Geometric representation as points */
RICPP_EXTERN(RtToken) RI_LINES; /**< Geometric representation as lines */
/* RICPP_EXTERN(RtToken) RI_PRIMITIVE; // Geometric representation as shaded primitive already a token */

RICPP_EXTERN(RtToken) RI_SENSE; /**< User defined attribute for trimcurve sense RI_INSIDE, RI_OUTSIDE */

RICPP_EXTERN(RtToken) RI_DELAYED_READ_ARCHIVE; /**< Token for procedural CProcDelayedReadArchive */
RICPP_EXTERN(RtToken) RI_RUN_PROGRAM; /**< Token for procedural CProcRunProgram */
RICPP_EXTERN(RtToken) RI_DYNAMIC_LOAD; /**< Token for procedural CProcDynamicLoad */
RICPP_EXTERN(RtToken) RI_FREE; /**< Token for CProcFree */

RICPP_EXTERN(RtToken) RI_GAUSSIAN; /**< Token for the gaussian filter */
RICPP_EXTERN(RtToken) RI_BOX; /**< Token for the box filter */
RICPP_EXTERN(RtToken) RI_TRIANGLE; /**< Token for the triangle filter */
/* RICPP_EXTERN(RtToken) RI_CATMULL_ROM; //  Token for the Catmull Rom filter s. RI_CATMULL_ROM */
RICPP_EXTERN(RtToken) RI_SINC; /**< Token for the sinc filter */

RICPP_EXTERN(RtToken) RI_BEZIER; /**< Bezier basis */
RICPP_EXTERN(RtToken) RI_B_SPLINE; /**< B-Spline basis */
RICPP_EXTERN(RtToken) RI_CATMULL_ROM; /**< Catmull-rom basis and filter */
RICPP_EXTERN(RtToken) RI_CATMULLROM; /**< Catmull-rom basis, filter (alternate writing, not used for output) */
RICPP_EXTERN(RtToken) RI_HERMITE; /**< Hermite basis */
RICPP_EXTERN(RtToken) RI_POWER; /**< Power basis */

/* Texture tabes
// RI_TEXTURE already declared
*/
RICPP_EXTERN(RtToken) RI_BUMP; /**< Bumpmap texture */
RICPP_EXTERN(RtToken) RI_SHADOW; /**< Shadowmap texture */
RICPP_EXTERN(RtToken) RI_LAT_LONG_ENVIRONMENT; /**< Latitude/longitude environment texture */
RICPP_EXTERN(RtToken) RI_CUBE_FACE_ENVIRONMENT; /**< Cubeface environment texture */
RICPP_EXTERN(RtToken) RI_BRICK_MAP; /**< Brickmap texture */

RICPP_EXTERN(RtToken) RI_INT; /**< RI_INTEGER is used instead, RI_INT is only used for parsing, should not be used as token */

RICPP_EXTERN(RtToken) RI_NULL_LIT; /**< The literal "null". Used as standard hider type,  "not" use this instead of RI_NULL!!! */
RICPP_EXTERN(RtToken) RI_UNKNOWN; /**< If something is unknown (some error strings) "not" used instead of RI_NULL, used internally */
RICPP_EXTERN(RtToken) RI_EMPTY; /**< Empty String marker, not a real token, only used internally,  "not" use this instead of RI_NULL!!! */
/** @} */
/** @} */

#ifdef __cplusplus
} /* namespace RiCPP */
#endif

#ifdef __cplusplus
/* } /* extern "C" */
#endif

#endif /* _RICPP_RIBASE_RICPPTOKENS_H */
