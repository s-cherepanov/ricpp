#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#define _RICPP_RIBASE_RICPPTOKENS_H
/*
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
	/* namespace RiCPP { */
	#endif
	RICPP_EXTERN(RtToken const) RI_NULL; /**< Used to mark undefined tokens and end marker of parameter list. */
	RICPP_EXTERN(RtToken const) RI_FRAMEBUFFER; /**< Framebuffer output device as @a type in @c  CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_FILE; /**< File output as @a type in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_RGB; /**< Output RGB (red green blue) channels for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_RGBA; /**< Output RGBA (RGB + alpha opacitiy) channels for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_RGBZ; /**< Output RGBZ (RGB + depth) channels for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_RGBAZ; /**< Output RGBAZ channels for @a mode in @c CRiRoot:::display() */
	RICPP_EXTERN(RtToken const) RI_A; /**< Alpha (opacity) channel for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_Z; /**< Depth channel for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_AZ; /**< Alpha and Depth channels for @a mode in @c CRiRoot::display() */
	RICPP_EXTERN(RtToken const) RI_PERSPECTIVE; /**< Perspective projection for parameter @a name for CRiRoot::projectionV() */
    RICPP_EXTERN(RtToken const) RI_ORTHOGRAPHIC; /**< Orthographic projection for parameter @a name for CRiRoot::projectionV() */
	RICPP_EXTERN(RtToken const) RI_HIDDEN; /**< Standard hider 'hidden' for CRiRoot::hiderV() */
	RICPP_EXTERN(RtToken const) RI_PAINT; /**< Standard hider painters algorithm for CRiRoot::hiderV() */
	RICPP_EXTERN(RtToken const) RI_CONSTANT; /**< Constant shading parameter @a type for CRiRoot::shadingInterpolation() */
    RICPP_EXTERN(RtToken const) RI_SMOOTH; /**< Smooth shading parameter @a type for CRiRoot::shadingInterpolation() */
	RICPP_EXTERN(RtToken const) RI_FLATNESS; /**< Standard approximation criterion for CRiRoot::geometricApproximation() */
	RICPP_EXTERN(RtToken const) RI_FOV; /**< Field of view value for perspective projection @see RI_PERSPECTIVE, CRiRoot::projectionV() */
	RICPP_EXTERN(RtToken const) RI_AMBIENTLIGHT; /**< Ambient light source shader CRiRoot::lightSourceV() */
	RICPP_EXTERN(RtToken const) RI_POINTLIGHT; /**< Point light source shader CRiRoot::lightSourceV() */
	RICPP_EXTERN(RtToken const) RI_DISTANTLIGHT; /**< Distant light source shader CRiRoot::lightSourceV() */
	RICPP_EXTERN(RtToken const) RI_SPOTLIGHT; /**< Spotlight source shader CRiRoot::lightSourceV() */
	RICPP_EXTERN(RtToken const) RI_INTENSITY; /**< Intensity (RtFloat) parameter of a lightsource */
	RICPP_EXTERN(RtToken const) RI_LIGHTCOLOR; /**< Lightcolor (RtColor) parameter of a lightsource */
	RICPP_EXTERN(RtToken const) RI_FROM; /**< From position (RtPoint) parameter of a lightsource */
	RICPP_EXTERN(RtToken const) RI_TO; /**< Direction (RtVector) parameter of a lightsource (not point light) */
	RICPP_EXTERN(RtToken const) RI_CONEANGLE; /**< Cone angle (RtFloat) parameter of a spotlight */
	RICPP_EXTERN(RtToken const) RI_CONEDELTAANGLE; /**< Cone delta angle (RtFloat) parameter of a spotlight */
	RICPP_EXTERN(RtToken const) RI_BEAMDISTRIBUTION; /**< Beamdistribution (RtFloat) parameter of a spotlight */
	RICPP_EXTERN(RtToken const) RI_MATTE; /**< Matte surface shader */
	RICPP_EXTERN(RtToken const) RI_METAL; /**< Metal surface shader */
	RICPP_EXTERN(RtToken const) RI_SHINYMETAL; /**< Shiny metal surface shader */
	RICPP_EXTERN(RtToken const) RI_PLASTIC; /**< Plastic surface shader */
	RICPP_EXTERN(RtToken const) RI_PAINTEDPLASTIC; /**< Painted plastic surface shader */
	RICPP_EXTERN(RtToken const)  RI_KA; /**< Ambient terms for the reflection formular of a surface */
	RICPP_EXTERN(RtToken const) RI_KD; /**< Diffuse terms for the reflection formular of a surface */
	RICPP_EXTERN(RtToken const) RI_KS; /**< Specular terms for the reflection formular of a surface */
	RICPP_EXTERN(RtToken const) RI_ROUGHNESS; /**< Roughness (controls size of the highlight) for the specular reflection of a surface */
	RICPP_EXTERN(RtToken const) RI_KR; /**< Term for mix in environment color (shiny metal) */
	RICPP_EXTERN(RtToken const) RI_TEXTURENAME; /**< Texturname of a surface (e.g. RI_SHINYMETAL or RI_PAINTEDPLASTIC) */
	RICPP_EXTERN(RtToken const)  RI_SPECULARCOLOR; /**< Color for the hightlight of a specular reflection of a surface  */
	RICPP_EXTERN(RtToken const) RI_DEPTHCUE; /**< Depth cue volume shader */
	RICPP_EXTERN(RtToken const) RI_FOG; /**< Fog volume shader */
	RICPP_EXTERN(RtToken const) RI_BUMPY; /**< Bumpy displacement shader */
	RICPP_EXTERN(RtToken const) RI_MINDISTANCE; /**< Minimal distance of surface where RI_DEPTHCUE is applied  (all surfaces nearer get no background color mixed in) */
	RICPP_EXTERN(RtToken const) RI_MAXDISTANCE; /**< Maximal distance of surface where RI_DEPTHCUE is applied (all surfaces farer away get background color) */
	RICPP_EXTERN(RtToken const) RI_BACKGROUND; /**< Background color the RI_DEPTHCUE and RI_FOG atmosphere shader mixes in */
	RICPP_EXTERN(RtToken const) RI_DISTANCE; /**< controls the attenuation of the surface color in RI_FOG shader */
	RICPP_EXTERN(RtToken const) RI_AMPLITUDE; /**< Controls (amplifies) the amplitude of bump maps in bumpy displacement shader */
	RICPP_EXTERN(RtToken const) RI_RASTER; /**< Name of the raster coordinate system */
	RICPP_EXTERN(RtToken const) RI_SCREEN; /**< Name of the screen coordinate system */
	RICPP_EXTERN(RtToken const) RI_CAMERA; /**< Name of the camera coordinate system */
	RICPP_EXTERN(RtToken const) RI_WORLD; /**< Name of the world coordinate system */
	RICPP_EXTERN(RtToken const) RI_OBJECT; /**< Name of the object coordinate system */
	RICPP_EXTERN(RtToken const) RI_INSIDE; /**< Inside orientation @see CRiRoot::orientation() */
	RICPP_EXTERN(RtToken const) RI_OUTSIDE; /**< Outside orientation @see CRiRoot::orientation() */
	RICPP_EXTERN(RtToken const) RI_LH; /**< Concrete left handed orientation @see CRiRoot::orientation() */
	RICPP_EXTERN(RtToken const) RI_RH; /**< Concrete right handed orientation @see CRiRoot::orientation() */
	RICPP_EXTERN(RtToken const) RI_P; /**< Token for position parameter */
	RICPP_EXTERN(RtToken const) RI_PZ; /**< Token for depth parameter */
	RICPP_EXTERN(RtToken const) RI_PW; /**< Token for homogenous points parameter */
	RICPP_EXTERN(RtToken const) RI_N; /**< Token for normal vector parameter */
	RICPP_EXTERN(RtToken const) RI_NP; /**< Token for normal vector followed by position parameter */
	RICPP_EXTERN(RtToken const) RI_CS; /**< Token for surface color */
	RICPP_EXTERN(RtToken const) RI_OS; /**< Token for surface opacity */
	RICPP_EXTERN(RtToken const) RI_S; /**< Token for s texture coordinate */
	RICPP_EXTERN(RtToken const) RI_T; /**< Token for t texture coordinate */
	RICPP_EXTERN(RtToken const) RI_ST; /**< Token for s folowed by t texture coordinate */
	RICPP_EXTERN(RtToken const) RI_BILINEAR; /**< Bilinear patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV() */
	RICPP_EXTERN(RtToken const) RI_BICUBIC; /**< Bicubic patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV() */
	RICPP_EXTERN(RtToken const) RI_PRIMITIVE; /**< Primitive solid (leaf knot), also used as geometric representation @see RiCPP::solidBegin(), RiCPP::solidEnd(), RiCPP::geometricRepresentation() */
	RICPP_EXTERN(RtToken const) RI_INTERSECTION; /**< Intersection of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */
	RICPP_EXTERN(RtToken const) RI_UNION; /**< Union of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */
	RICPP_EXTERN(RtToken const) RI_DIFFERENCE; /**< Difference of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd() */
	RICPP_EXTERN(RtToken const) RI_PERIODIC; /**< Periodic wrap of a patch meshes, curves or texture coordinates @see RiCPPRoot::patchMeshV(), ricpp_texture,  CRiCPP::curvesV() */
	RICPP_EXTERN(RtToken const) RI_NONPERIODIC; /**< Non-periodic wrap of a patch meshes or curves @see RiCPPRoot::patchMeshV(), CRiCPP::curvesV() */
	RICPP_EXTERN(RtToken const) RI_CLAMP; /**< Clamping textures coordinates to their borders @see ricpp_texture */
	RICPP_EXTERN(RtToken const) RI_BLACK; /**< Leaving textels outside their borders black @see ricpp_texture */
	RICPP_EXTERN(RtToken const) RI_IGNORE; /**< Token indicates the ignore error handler */
	RICPP_EXTERN(RtToken const) RI_PRINT; /**< Token indicates the print error handler */
	RICPP_EXTERN(RtToken const) RI_ABORT; /**< Token indicates the abort error handler */
	RICPP_EXTERN(RtToken const) RI_HANDLER; /**< General token for handler */
	/** @defgroup ricpp_tokens_add Additional tokens
	 *  @ingroup ricpp_tokens
	 *  @brief Additional tokens, used by RiCPP
	 *  @{
	 */
	RICPP_EXTERN(RtToken const) RI_CATMULL_CLARK; /**< Name of Catmull-Clark subdivision surface @see CRiCPP::subdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_HOLE; /**< Hole in subdivision surface @see CRiCPP::subdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_CREASE; /**< Sharp edge in subdivision surface @see CRiCPP::subdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_CORNER; /**< Corner in subdivision surface @see CRiCPP::subdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_INTERPOLATEBOUNDARY; /**< Interpolate the boundery (instead of approximate) subdivision surface @see CRiCPP::subdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_FACEINTERPOLATEBOUNDARY; /**< Interpolate the boundery for varying parameters (instead of approximate) subdivision surface @see CRiCPP::subdivisionMeshV() */
	
	RICPP_EXTERN(RtToken const) RI_VERTEXEDIT; /**< Vertex edit @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_EDGEEDIT; /**< edge edit @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_FACEEDIT; /**< face edit @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_ADD; /**< Add a value @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_SET; /**< Set a value @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_VALUE; /**< Some value @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_SHARPNESS; /**< Sharpness value @see CRiCPP::hierarchicalSubdivisionMeshV() */
	RICPP_EXTERN(RtToken const) RI_ATTRIBUTES; /**< Attributes value @see CRiCPP::hierarchicalSubdivisionMeshV() */
	
	RICPP_EXTERN(RtToken const) RI_ORIGIN; /**< Token for an origin parameter @see CRiCPP::displayV() */
	RICPP_EXTERN(RtToken const) RI_NAME; /**< Token for a name parameter */
	RICPP_EXTERN(RtToken const) RI_COMMENT; /**< Comment for CRiCPP::ArchiveRecord() */
	RICPP_EXTERN(RtToken const) RI_STRUCTURE; /**< Structuredcomment for CRiCPP::ArchiveRecord() */
	RICPP_EXTERN(RtToken const) RI_VERBATIM; /**< Verbatim output for CRiCPP::ArchiveRecord() */
	RICPP_EXTERN(RtToken const) RI_LINEAR; /**< Linear curves @see CRiCPP::curvesV() */
	RICPP_EXTERN(RtToken const) RI_CUBIC; /**< Cubic curves @see CRiCPP::curvesV() */
	RICPP_EXTERN(RtToken const) RI_WIDTH; /**< Varying width for CRiCPP::pointsV() */
	RICPP_EXTERN(RtToken const) RI_CONSTANTWIDTH; /**< constant width for CRiCPP::pointsV() */
	RICPP_EXTERN(RtToken const) RI_DISPXRES; /**< Display x-resulution in pixels */
	RICPP_EXTERN(RtToken const) RI_DISPYRES; /**< Display y-resulution in pixels */
	RICPP_EXTERN(RtToken const) RI_PIXELASPECT; /**< Display pixel aspect ratio */
	RICPP_EXTERN(RtToken const) RI_CURRENT; /**< Current coordinate system */
	RICPP_EXTERN(RtToken const) RI_EYE; /**<  Eye coordinate system */
	RICPP_EXTERN(RtToken const) RI_SHADER; /**<  Shader coordinate system */
	RICPP_EXTERN(RtToken const) RI_NDC; /**<  NDC coordinate system */
	RICPP_EXTERN(RtToken const) RI_FILEPOINTER; /**< Input/Output to a file FILE * (resource) */
	RICPP_EXTERN(RtToken const) RI_MEMORY; /**< Input from memory resource, not allowed for RIB */
	/** @todo Maybe named pipe as PIPE:pipename as normal filename, already |cmd for anonymous pipe output, | for stdio */
	RICPP_EXTERN(RtToken const) RI_ARCHIVE; /**< archive 'renderer' type (@a IRiContext::rendererType()) to archive RIB code, or archive qualifier */
	RICPP_EXTERN(RtToken const) RI_DRAFT; /**< fast draft renderer type (@a IRiContext::rendererType()) with limited implementation of the RI */
	RICPP_EXTERN(RtToken const) RI_REALISTIC; /**< photo realistic renderer type (@a IRiContext::rendererType())  implementing the full RI */
	RICPP_EXTERN(RtToken const) RI_FLOAT; /**< Name for type RtFloat */
	RICPP_EXTERN(RtToken const) RI_INTEGER; /**< Name for type RtInteger */
	RICPP_EXTERN(RtToken const) RI_STRING; /**< Name for type RtString */
	RICPP_EXTERN(RtToken const) RI_POINT; /**< Name for type RtPoint */
	RICPP_EXTERN(RtToken const) RI_VECTOR; /**< Name for type RtVector */
	RICPP_EXTERN(RtToken const) RI_NORMAL; /**< Name for type RtNormal */
	RICPP_EXTERN(RtToken const) RI_HPOINT; /**< Name for type RtHpoint */
	RICPP_EXTERN(RtToken const) RI_MATRIX; /**< Name for type RtMatrix */
	RICPP_EXTERN(RtToken const) RI_COLOR; /**< Name for type RtColor */
	/* RICPP_EXTERN(RtToken const) RI_CONSTANT; // already a RtToken */
	RICPP_EXTERN(RtToken const) RI_UNIFORM; /**< Name for storage class uniform */
	RICPP_EXTERN(RtToken const) RI_VARYING; /**< Name for storage class varying */
	RICPP_EXTERN(RtToken const) RI_VERTEX; /**< Name for storage class vertex */
	RICPP_EXTERN(RtToken const) RI_FACEVARYING; /**< Name for storage class facevarying */
	RICPP_EXTERN(RtToken const) RI_FACEVERTEX; /**< Name for storage class facevertex */
	RICPP_EXTERN(RtToken const) RI_HANDLEID; /**< Handle id */
	RICPP_EXTERN(RtToken const) RI_PROJECTION; /**< Projection qualifier */
	RICPP_EXTERN(RtToken const) RI_IMAGER; /**< Imager qualifier */
	RICPP_EXTERN(RtToken const) RI_DISPLAY; /**< Display qualifier */
	RICPP_EXTERN(RtToken const) RI_HIDER; /**< Hider qualifier */
	RICPP_EXTERN(RtToken const) RI_OPTION; /**< Option qualifier */
	RICPP_EXTERN(RtToken const) RI_LIGHT_SOURCE; /**< Light source qualifier */
	RICPP_EXTERN(RtToken const) RI_AREA_LIGHT_SOURCE; /**< Area light source qualifier */
	RICPP_EXTERN(RtToken const) RI_SURFACE; /**< Surface shader qualifier */
	RICPP_EXTERN(RtToken const) RI_ATMOSPHERE; /**< Atmosphere shader qualifier */
	RICPP_EXTERN(RtToken const) RI_INTERIOR; /**< Interior shader qualifier */
	RICPP_EXTERN(RtToken const) RI_EXTERIOR; /**< Exterior shader qualifier */
	RICPP_EXTERN(RtToken const) RI_DISPLACEMENT; /**< Displacement shader qualifier */
	RICPP_EXTERN(RtToken const) RI_DEFORMATION; /**< Deformation shader qualifier */
	RICPP_EXTERN(RtToken const) RI_ATTRIBUTE; /**< Attribute qualifier */
	RICPP_EXTERN(RtToken const) RI_TEXTURE; /**< Texture qualifier */
	RICPP_EXTERN(RtToken const) RI_CONTROL; /**< Control qualifier */
	RICPP_EXTERN(RtToken const) RI_BEGIN; /**< qualifier for Begin statement */
	RICPP_EXTERN(RtToken const) RI_RESOURCE; /**< Resource qualifier */
	RICPP_EXTERN(RtToken const) RI_FRAME; /**< Frame number */
	RICPP_EXTERN(RtToken const) RI_DEVIATION; /**< Type for RiCPP::geometricApproximation() */
	RICPP_EXTERN(RtToken const) RI_TESSELATION; /**< Type for RiCPP::geometricApproximation() */
	RICPP_EXTERN(RtToken const) RI_POINTS; /**< Geometric representation as points */
	RICPP_EXTERN(RtToken const) RI_LINES; /**< Geometric representation as lines */
	/* RICPP_EXTERN(RtToken const) RI_PRIMITIVE; // Geometric representation as shaded primitive already a token */
	RICPP_EXTERN(RtToken const) RI_SENSE; /**< User defined attribute for trimcurve sense RI_INSIDE, RI_OUTSIDE */
	RICPP_EXTERN(RtToken const) RI_DELAYED_READ_ARCHIVE; /**< Token for procedural CProcDelayedReadArchive */
	RICPP_EXTERN(RtToken const) RI_RUN_PROGRAM; /**< Token for procedural CProcRunProgram */
	RICPP_EXTERN(RtToken const) RI_DYNAMIC_LOAD; /**< Token for procedural CProcDynamicLoad */
	RICPP_EXTERN(RtToken const) RI_FREE; /**< Token for CProcFree */
	RICPP_EXTERN(RtToken const) RI_GAUSSIAN; /**< Token for the gaussian filter */
	RICPP_EXTERN(RtToken const) RI_BOX; /**< Token for the box filter */
	RICPP_EXTERN(RtToken const) RI_TRIANGLE; /**< Token for the triangle filter */
	/* RICPP_EXTERN(RtToken const) RI_CATMULL_ROM; //  Token for the Catmull Rom filter s. RI_CATMULL_ROM */
	RICPP_EXTERN(RtToken const) RI_SINC; /**< Token for the sinc filter */
	RICPP_EXTERN(RtToken const) RI_BEZIER; /**< Bezier basis */
	RICPP_EXTERN(RtToken const) RI_B_SPLINE; /**< B-Spline basis */
	RICPP_EXTERN(RtToken const) RI_CATMULL_ROM; /**< Catmull-rom basis and filter */
	RICPP_EXTERN(RtToken const) RI_HERMITE; /**< Hermite basis */
	RICPP_EXTERN(RtToken const) RI_POWER; /**< Power basis */
	RICPP_EXTERN(RtToken const) RI_RESTART; /**< Synchronize renderer for restart */
	RICPP_EXTERN(RtToken const) RI_TEAPOT; /**< Teapot geometry */
	/* Texture maps
	// RI_TEXTURE already declared
	*/
	RICPP_EXTERN(RtToken const) RI_BUMP; /**< Bumpmap texture */
	RICPP_EXTERN(RtToken const) RI_SHADOW; /**< Shadowmap texture */
	RICPP_EXTERN(RtToken const) RI_LAT_LONG_ENVIRONMENT; /**< Latitude/longitude environment texture */
	RICPP_EXTERN(RtToken const) RI_CUBE_FACE_ENVIRONMENT; /**< Cubeface environment texture */
	RICPP_EXTERN(RtToken const) RI_BRICK_MAP; /**< Brickmap texture */
	// Internally used tokens
	RICPP_EXTERN(RtToken const) RI_INT; /**< RI_INTEGER is used instead, RI_INT is only used for parsing, should not be used as token */
	RICPP_EXTERN(RtToken const) RI_NULL_LIT; /**< The literal "null". Used as standard hider type,  "not" use this instead of RI_NULL!!! */
	RICPP_EXTERN(RtToken const) RI_UNKNOWN; /**< If something is unknown (some error strings) "not" used instead of RI_NULL, used internally */
	RICPP_EXTERN(RtToken const) RI_EMPTY; /**< Empty String marker, not a real token, only used internally,  "not" use this instead of RI_NULL!!! */
	/** @} */
	/** @} */
	#ifdef __cplusplus
	} /* extern "C" */
	#endif
#ifdef __cplusplus
/* } namespace RiCPP */
#endif
#endif /* _RICPP_RIBASE_RICPPTOKENS_H */
