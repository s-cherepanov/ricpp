#ifndef _RICPP_RICPP_RICPPTOKENS_H
#define _RICPP_RICPP_RICPPTOKENS_H

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

/** @file ricpptokens.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Tokens of the RenderMan(R) interface
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

/** @defgroup ricpp_tokens Ri tokens
 *  @brief Definition of the tokens of the RenderMan interface
 *  @warning Must be inserted into CTokenMap::CTokenMap()
 *  @see CTokenMap::CTokenMap()
 *  @{
 */
const RtToken  RI_NULL = (RtToken)0L; ///< Used to mark undefined tokens and end marker of parameter list.

const RtToken  RI_FRAMEBUFFER = "framebuffer", ///< Framebuffer output device as @a type in @c  CRiRoot::display()
               RI_FILE = "file";               ///< File output as @a type in @c CRiRoot::display()

const RtToken  RI_RGB = "rgb",     ///< Output RGB (red green blue) channels for @a mode in @c CRiRoot::display()
               RI_RGBA = "rgba",   ///< Output RGBA (RGB + alpha opacitiy) channels for @a mode in @c CRiRoot::display()
			   RI_RGBZ = "rgbz",   ///< Output RGBZ (RGB + depth) channels for @a mode in @c CRiRoot::display()
			   RI_RGBAZ = "rgbaz", ///< Output RGBAZ channels for @a mode in @c CRiRoot:::display()
               RI_A = "a",         ///< Alpha (opacity) channel for @a mode in @c CRiRoot::display()
			   RI_Z = "z",         ///< Depth channel for @a mode in @c CRiRoot::display()
			   RI_AZ = "az";       ///< Alpha and Depth channels for @a mode in @c CRiRoot::display()

const RtToken  RI_PERSPECTIVE = "perspective",   ///< Perspective projection for parameter @a name for CRiRoot::projectionV()
               RI_ORTHOGRAPHIC = "orthographic"; ///< Orthographic projection for parameter @a name for CRiRoot::projectionV()

const RtToken  RI_HIDDEN = "hidden", ///< Standard hider 'hidden' for CRiRoot::hiderV()
               RI_PAINT = "paint";   ///< Standard hider painters algorithm for CRiRoot::hiderV()

const RtToken  RI_CONSTANT = "constant", ///< Constant shading parameter @a type for CRiRoot::shadingInterpolation()
               RI_SMOOTH = "smooth";     ///< Smooth shading parameter @a type for CRiRoot::shadingInterpolation()

const RtToken  RI_FLATNESS = "flatness"; ///< Standard approximation criterion for CRiRoot::geometricApproximation()

const RtToken  RI_FOV = "fov"; ///< Field of view value for perspective projection @see RI_PERSPECTIVE, CRiRoot::projectionV()

const RtToken  RI_AMBIENTLIGHT = "ambientlight", ///< Ambient light source shader CRiRoot::lightSourceV()
               RI_POINTLIGHT = "pointlight",     ///< Point light source shader CRiRoot::lightSourceV()
               RI_DISTANTLIGHT = "distantlight", ///< Distant light source shader CRiRoot::lightSourceV()
			   RI_SPOTLIGHT = "spotlight";       ///< Spotlight source shader CRiRoot::lightSourceV()

const RtToken  RI_INTENSITY = "intensity",               ///< Intensity (RtFloat) parameter of a lightsource
               RI_LIGHTCOLOR = "lightcolor",             ///< Lightcolor (RtColor) parameter of a lightsource
			   RI_FROM = "from",                         ///< From position (RtPoint) parameter of a lightsource
			   RI_TO = "to",                             ///< Direction (RtVector) parameter of a lightsource (not point light)
			   RI_CONEANGLE = "coneangle",               ///< Cone angle (RtFloat) parameter of a spotlight
               RI_CONEDELTAANGLE = "conedeltaangle",     ///< Cone delta angle (RtFloat) parameter of a spotlight
			   RI_BEAMDISTRIBUTION = "beamdistribution"; ///< Beamdistribution (RtFloat) parameter of a spotlight

const RtToken  RI_MATTE = "matte",                   ///< Matte surface shader
               RI_METAL = "metal",                   ///< Metal surface shader
			   RI_SHINYMETAL = "shinymetal",         ///< Shiny metal surface shader
               RI_PLASTIC = "plastic",               ///< Plastic surface shader
			   RI_PAINTEDPLASTIC = "paintedplastic"; ///< Painted plastic surface shader

const RtToken  RI_KA = "Ka",                       ///< Ambient terms for the reflection formular of a surface
               RI_KD = "Kd",                       ///< Diffuse terms for the reflection formular of a surface
			   RI_KS = "Ks",                       ///< Specular terms for the reflection formular of a surface
			   RI_ROUGHNESS = "roughness",         ///< Roughness (controls size of the highlight) for the specular reflection of a surface
			   RI_KR = "Kr",                       ///< Term for mix in environment color (shiny metal)
               RI_TEXTURENAME = "texturename",     ///< Texturname of a surface (e.g. RI_SHINYMETAL or RI_PAINTEDPLASTIC)
			   RI_SPECULARCOLOR = "specularcolor"; ///< Color for the hightlight of a specular reflection of a surface 

const RtToken  RI_DEPTHCUE = "depthcue", ///< Depth cue volume shader
               RI_FOG = "fog";           ///< Fog volume shader

const RtToken  RI_BUMPY = "bumpy"; ///< Bumpy displacement shader

const RtToken  RI_MINDISTANCE = "mindistance", ///< Minimal distance of surface where RI_DEPTHCUE is applied  (all surfaces nearer get no background color mixed in)
               RI_MAXDISTANCE = "maxdistance", ///< Maximal distance of surface where RI_DEPTHCUE is applied (all surfaces farer away get background color)
			   RI_BACKGROUND = "background",   ///< Background color the RI_DEPTHCUE and RI_FOG atmosphere shader mixes in
               RI_DISTANCE = "distance",       ///< Distance shader parameter, controls the attenuation of the surface color in RI_FOG shader
			   RI_AMPLITUDE = "amplitude";     ///< Controls (amplifies) the amplitude of bump maps in bumpy displacement shader

const RtToken  RI_RASTER = "raster", ///< Name of the raster coordinate system
			   RI_SCREEN = "screen", ///< Name of the screen coordinate system
			   RI_CAMERA = "camera", ///< Name of the camera coordinate system
               RI_WORLD = "world",   ///< Name of the world coordinate system
			   RI_OBJECT = "object"; ///< Name of the object coordinate system

const RtToken  RI_INSIDE = "inside",   ///< Inside orientation @see CRiRoot::orientation()
               RI_OUTSIDE = "outside", ///< Outside orientation @see CRiRoot::orientation()
			   RI_LH = "lh",           ///< Concrete left handed orientation @see CRiRoot::orientation()
			   RI_RH = "rh";           ///< Concrete right handed orientation @see CRiRoot::orientation()

const RtToken  RI_P = "P",   ///< Token for position parameter
               RI_PZ = "Pz", ///< Token for depth parameter
			   RI_PW = "Pw", ///< Token for homogenous points parameter
			   RI_N = "N",   ///< Token for normal vector parameter
			   RI_NP = "NP", ///< Token for normal vector followed by position parameter
			   RI_CS = "Cs", ///< Token for surface color
               RI_OS = "Os", ///< Token for surface opacity
			   RI_S = "s",   ///< Token for s texture coordinate
			   RI_T = "t",   ///< Token for t texture coordinate
			   RI_ST = "st"; ///< Token for s folowed by t texture coordinate

const RtToken  RI_BILINEAR = "bilinear", ///< Bilinear patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV()
               RI_BICUBIC = "bicubic";   ///< Bicubic patch/mesh @see RiCPP::patchV(), RiCPP::patchMeshV()

const RtToken  RI_PRIMITIVE = "primitive",       ///< Primitive solid (leaf knot), also used as geometric representation @see RiCPP::solidBegin(), RiCPP::solidEnd(), RiCPP::geometricRepresentation()
               RI_INTERSECTION = "intersection", ///< Intersection of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd()
			   RI_UNION = "union",               ///< Union of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd()
			   RI_DIFFERENCE = "difference";     ///< Difference of two solids @see RiCPP::solidBegin(), RiCPP::solidEnd()

const RtToken  RI_PERIODIC = "periodic",       ///< Periodic wrap of a patch meshes, curves or texture coordinates @see RiCPPRoot::patchMeshV(), ricpp_texture,  CRiCPP::curvesV()
               RI_NONPERIODIC = "nonperiodic", ///< Non-periodic wrap of a patch meshes or curves @see RiCPPRoot::patchMeshV(), CRiCPP::curvesV()
               RI_CLAMP = "clamp",             ///< Clamping textures coordinates to their borders @see ricpp_texture
			   RI_BLACK = "black";             ///< Leaving textels outside their borders black @see ricpp_texture

const RtToken  RI_IGNORE = "ignore",   ///< Token indicates the ignore error handler
               RI_PRINT = "print",     ///< Token indicates the print error handler
			   RI_ABORT = "abort",     ///< Token indicates the abort error handler
			   RI_HANDLER = "handler"; ///< General token for handler


/** @defgroup ricpp_tokens_add Additional tokens
 *  @ingroup ricpp_tokens
 *  @brief Additional tokens, used by RiCPP
 *  @{
 */
const RtToken  RI_CATMULLCLARK = "catmull-clark",              ///< Name of Catmull-Clark subdivision surface @see CRiCPP::subdivisionMeshV()
               RI_HOLE = "hole",                               ///< Hole in subdivision surface @see CRiCPP::subdivisionMeshV()
			   RI_CREASE = "crease",                           ///< Sharp edge in subdivision surface @see CRiCPP::subdivisionMeshV()
			   RI_CORNER = "corner",                           ///< Corner in subdivision surface @see CRiCPP::subdivisionMeshV()
			   RI_INTERPOLATEBOUNDARY = "interpolateboundary"; ///< Interpolate the boundery (instead of approximate) subdivision surface @see CRiCPP::subdivisionMeshV()

const RtToken  RI_ORIGIN = "origin",          ///< Token for an origin parameter @see CRiCPP::displayV()
			   RI_NAME = "name";              ///< Token for a name parameter

const RtToken  RI_COMMENT = "comment",     ///< Comment for CRiCPP::ArchiveRecord()
               RI_STRUCTURE = "structure", ///< Structuredcomment for CRiCPP::ArchiveRecord()
			   RI_VERBATIM = "verbatim";   ///< Verbatim output for CRiCPP::ArchiveRecord()

const RtToken  RI_LINEAR = "linear", ///< Linear curves @see CRiCPP::curvesV()
               RI_CUBIC = "cubic";   ///< Cubic curves @see CRiCPP::curvesV()

const RtToken  RI_WIDTH = "width",
			   RI_CONSTANTWIDTH = "constantwidth";

const RtToken  RI_CURRENT = "current", // Current coordinate system
			   RI_EYE = "eye",         // Eye coordinate system
			   RI_SHADER = "shader",   // Shader coordinate system
               RI_NDC = "NDC";         // NDC coordinate system

const RtToken  RI_FILEPOINTER = "filepointer", ///< Input/Output to a file FILE * (resource)
               RI_MEMORY = "memory";           ///< Input from memory resource, not allowed for RIB

// note: Named pipe as PIPE:pipename as normal filename, |cmd for anonymous pipe output, | for input

const RtToken RI_ARCHIVE = "archive",   ///< archive 'renderer' type (@a IRiContext::rendererType()) to archive RIB code, or archive qualifier
              RI_DRAFT = "draft",       ///< fast draft renderer type (@a IRiContext::rendererType()) with limited implementation of the RI
              RI_REALISTIC = "realistic"; ///< photo realistic renderer type (@a IRiContext::rendererType())  implementing the full RI

const RtToken RI_FLOAT   = "float"; ///< Name for type RtFloat
const RtToken RI_INTEGER = "integer"; ///< Name for type RtInteger
const RtToken RI_STRING  = "string"; ///< Name for type RtString
const RtToken RI_POINT   = "point"; ///< Name for type RtPoint
const RtToken RI_VECTOR  = "vector"; ///< Name for type RtVector
const RtToken RI_NORMAL  = "normal"; ///< Name for type RtNormal
const RtToken RI_HPOINT  = "hpoint"; ///< Name for type RtHpoint
const RtToken RI_MATRIX  = "matrix"; ///< Name for type RtMatrix
const RtToken RI_COLOR   = "color"; ///< Name for type RtColor

// const RtToken RI_CONSTANT = "constant"; // already a RtToken
const RtToken RI_UNIFORM = "uniform"; ///< Name for storage class uniform
const RtToken RI_VARYING = "varying"; ///< Name for storage class varying
const RtToken RI_VERTEX = "vertex"; ///< Name for storage class vertex
const RtToken RI_FACEVARYING = "facevarying"; ///< Name for storage class facevarying
const RtToken RI_FACEVERTEX = "facevertex"; ///< Name for storage class facevertex

const RtToken RI_HANDLEID = "__handleid"; ///< Handle id

const RtToken RI_PROJECTION = "Projection"; ///< Projection qualifier
const RtToken RI_IMAGER = "Imager"; ///< Imager qualifier
const RtToken RI_DISPLAY = "Display"; ///< Display qualifier
const RtToken RI_HIDER = "Hider"; ///< Hider qualifier
const RtToken RI_OPTION = "Option"; ///< Option qualifier
const RtToken RI_LIGHT_SOURCE = "LightSource"; ///< Light source qualifier
const RtToken RI_AREA_LIGHT_SOURCE = "AreaLightSource"; ///< Area light source qualifier
const RtToken RI_SURFACE = "Surface"; ///< Surface shader qualifier
const RtToken RI_ATMOSPHERE = "Atmosphere"; ///< Atmosphere shader qualifier
const RtToken RI_INTERIOR = "Interior"; ///< Interior shader qualifier
const RtToken RI_EXTERIOR = "Exterior"; ///< Exterior shader qualifier
const RtToken RI_DISPLACEMENT = "Displacement"; ///< Displacement shader qualifier
const RtToken RI_DEFORMATION = "Deformation"; ///< Deformation shader qualifier
const RtToken RI_ATTRIBUTE = "Attribute"; ///< Attribute qualifier
const RtToken RI_TEXTURE = "Texture"; ///< Texture qualifier
const RtToken RI_CONTROL = "Control"; ///< Control qualifier
const RtToken RI_BEGIN = "Begin"; ///< qualifier for Begin statement
const RtToken RI_RESOURCE = "Resource"; ///< Resource qualifier

const RtToken RI_FRAME = "Frame"; ///< Frame number

const RtToken RI_DEVIATION = "deviation";     ///< Type for RiCPP::geometricApproximation()
const RtToken RI_TESSELATION = "tesselation"; ///< Type for RiCPP::geometricApproximation()

const RtToken RI_POINTS  = "points"; ///< Geometric representation as points
const RtToken RI_LINES   = "lines";  ///< Geometric representation as lines
// const RtToken RI_PRIMITIVE   = "primitive"; // Geometric representation as shaded primitive already a token

const RtToken RI_SENSE  = "sense"; ///< User defined attribute for trimcurve sense RI_INSIDE, RI_OUTSIDE

const RtToken RI_DELAYED_READ_ARCHIVE = "DelayedReadArchive"; ///< Token for procedural CProcDelayedReadArchive
const RtToken RI_RUN_PROGRAM = "RunProgram"; ///< Token for procedural CProcRunProgram
const RtToken RI_DYNAMIC_LOAD = "DynamicLoad"; ///< Token for procedural CProcDynamicLoad
const RtToken RI_FREE = "Free"; ///< Token for CProcFree

const RtToken RI_GAUSSIAN = "gaussian"; ///< Token for the gaussian filter
const RtToken RI_BOX = "box"; ///< Token for the box filter
const RtToken RI_TRIANGLE = "triangle"; ///< Token for the triangle filter
// const RtToken RI_CATMULL_ROM = "catmull-rom"; //  Token for the Catmull Rom filter s. RI_CATMULL_ROM
const RtToken RI_SINC = "sinc"; ///< Token for the sinc filter

const RtToken RI_BEZIER = "bezier"; ///< Bezier basis
const RtToken RI_B_SPLINE = "b-spline"; ///< B-Spline basis
const RtToken RI_CATMULL_ROM = "catmull-rom"; ///< Catmull-rom basis and filter
const RtToken RI_CATMULLROM = "catmullrom"; ///< Catmull-rom basis, filter (alternate writing, not used for output)
const RtToken RI_HERMITE = "hermite"; ///< Hermite basis
const RtToken RI_POWER = "power"; ///< Power basis

// Texture tabes
// RI_TEXTURE already declared
const RtToken RI_BUMP = "Bump"; ///< Bumpmap texture
const RtToken RI_SHADOW = "Shadow"; ///< Shadowmap texture
const RtToken RI_LAT_LONG_ENVIRONMENT  = "LatLongEnvironment"; ///< Latitude/longitude environment texture
const RtToken RI_CUBE_FACE_ENVIRONMENT = "CubeFaceEnvironment";  ///< Cubeface environment texture
const RtToken RI_BRICK_MAP = "BrickMap"; ///< Brickmap texture

const RtToken RI_INT     = "int"; ///< RI_INTEGER is used instead, RI_INT is only used for parsing, should not be used as token

const RtToken RI_NULL_LIT = "null"; ///< The literal "null". Used as standard hider type,  "not" use this instead of RI_NULL!!!
const RtToken RI_UNKNOWN = "unknown"; ///< If something is unknown (some error strings) "not" used instead of RI_NULL, used internally
const RtToken RI_EMPTY   = ""; ///< Empty String marker, not a real token, only used internally,  "not" use this instead of RI_NULL!!!
//@}
//@}

#ifdef __cplusplus
} // namespace RiCPP
#endif

#ifdef __cplusplus
} // extern "C"
#endif


#endif // _RICPP_RICPP_RICPPTOKENS_H
