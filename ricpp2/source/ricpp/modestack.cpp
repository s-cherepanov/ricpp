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

/** @file modestack.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of the modes (blocks and nesting) of the interface.
 */

#include "ricpp/ricpp/modestack.h"

using namespace RiCPP;

CValidModes::CValidModes()
{
	m_requests[REQ_ERROR_HANDLER] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DECLARE] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_GET_CONTEXT] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_CONTEXT] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_BEGIN] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_END] = MODE_BIT_BEGIN;

	m_requests[REQ_FRAME_BEGIN] = MODE_BIT_BEGIN;
	m_requests[REQ_FRAME_END] = MODE_BIT_FRAME;

	m_requests[REQ_WORLD_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_WORLD_END] = MODE_BIT_WORLD;

	m_requests[REQ_ATTRIBUTE_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;
	m_requests[REQ_ATTRIBUTE_END] = MODE_BIT_ATTRIBUTE | MODE_BIT_OBJECT;

	m_requests[REQ_TRANSFORM_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;
	m_requests[REQ_TRANSFORM_END] = MODE_BIT_TRANSFORM | MODE_BIT_OBJECT;

	m_requests[REQ_SOLID_BEGIN] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID;
	m_requests[REQ_SOLID_END] = MODE_BIT_SOLID;

	m_requests[REQ_OBJECT_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID;
	m_requests[REQ_OBJECT_END] = MODE_BIT_OBJECT;

	m_requests[REQ_MOTION_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID;
	m_requests[REQ_MOTION_END] = MODE_BIT_MOTION;

	m_requests[REQ_FORMAT] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_FRAME_ASPECT_RATIO] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_SCREEN_WINDOW] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_CROP_WINDOW] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_PROJECTION] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_CLIPPING] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_CLIPPING_PLANE] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_DEPTH_OF_FIELD] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_SHUTTER] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_PIXEL_VARIANCE] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_PIXEL_SAMPLES] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_PIXEL_FILTER] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_EXPOSURE] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_IMAGER] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_QUANTIZE] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_DISPLAY] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_HIDER] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_COLOR_SAMPLES] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_RELATIVE_DETAIL] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_OPTION] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME;

	m_requests[REQ_LIGHT_SOURCE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_MOTION;
	m_requests[REQ_AREA_LIGHT_SOURCE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_MOTION;

	m_requests[REQ_ATTRIBUTE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_COLOR] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_OPACITY] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SURFACE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_ATMOSPHERE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_INTERIOR] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_EXTERIOR] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_ILLUMINATE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DISPLACEMENT] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_TEXTURE_COORDINATES] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SHADING_RATE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SHADING_INTERPOLATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_MATTE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_BOUND] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DETAIL] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DETAIL_RANGE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_GEOMETRIC_APPROXIMATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_GEOMETRIC_REPRESENTATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_ORIENTATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_REVERSE_ORIENTATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SIDES] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_BASIS] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_TRIM_CURVE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_IDENTITY] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_TRANSFORM] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_CONCAT_TRANSFORM] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_PERSPECTIVE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_TRANSLATE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_ROTATE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SCALE] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_SKEW] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DEFORMATION] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_COORDINATE_SYSTEM] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_OBJECT | MODE_BIT_SOLID;
	m_requests[REQ_COORD_SYS_TRANSFORM] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_OBJECT | MODE_BIT_SOLID;
	m_requests[REQ_TRANSFORM_POINTS] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_OBJECT | MODE_BIT_SOLID;

	m_requests[REQ_POLYGON] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_GENERAL_POLYGON] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_POINTS_POLYGONS] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_POINTS_GENERAL_POLYGONS] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_PATCH] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_PATCH_MESH] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_NU_PATCH] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_SUBDIVISION_MESH] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_SPHERE] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_CONE] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_CYLINDER] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_HYPERBOLOID] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_PARABOLOID] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_DISK] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_TORUS] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_POINTS] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_CURVES] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_BLOBBY] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;

	m_requests[REQ_PROCEDURAL] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;
	m_requests[REQ_PROC_DELAYED_READ_ARCHIVE] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;
	m_requests[REQ_PROC_RUN_PROGRAM] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;
	m_requests[REQ_PROC_DYNAMIC_LOAD] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;

	m_requests[REQ_GEOMETRY] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT;

	m_requests[REQ_OBJECT_INSTANCE] = MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID;

	m_requests[REQ_MAKE_TEXTURE] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_MAKE_BUMP] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_MAKE_LAT_LONG_ENVIRONMENT] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_MAKE_CUBE_FACE_ENVIRONMENT] = MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_MAKE_SHADOW] = MODE_BIT_BEGIN | MODE_BIT_FRAME;

	m_requests[REQ_ARCHIVE_RECORD] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	m_requests[REQ_READ_ARCHIVE] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_OBJECT | MODE_BIT_MOTION;
	
	m_requests[REQ_VERSION] = MODE_BIT_BEGIN;
}


bool CValidModes::isValid(EnumRequests idxRequest, EnumModes idxMode) const
{
	return (m_requests[(int)idxRequest] & (unsigned short)1<<(int)idxMode) != 0;
}


void CModeStack::begin()
{
	if ( MODE_OUTSIDE != curMode() ) {
		m_modes.clear();
	}
	push(MODE_BEGIN);
}


void CModeStack::end()
{
	while ( MODE_OUTSIDE != curMode() )
		pop();
}


void CModeStack::frameBegin()
{
	push(MODE_FRAME);
}


void CModeStack::frameEnd()
{
	pop();
}


void CModeStack::worldBegin()
{
	push(MODE_WORLD);
}


void CModeStack::worldEnd()
{
	pop();
}


void CModeStack::attributeBegin()
{
	push(MODE_ATTRIBUTE);
}


void CModeStack::attributeEnd()
{
	pop();
}


void CModeStack::transformBegin()
{
	push(MODE_TRANSFORM);
}


void CModeStack::transformEnd()
{
	pop();
}


void CModeStack::solidBegin()
{
	push(MODE_SOLID);
}


void CModeStack::solidEnd()
{
	pop();
}


void CModeStack::objectBegin()
{
	push(MODE_OBJECT);
}


void CModeStack::objectEnd()
{
	pop();
}


void CModeStack::motionBegin()
{
	push(MODE_MOTION);
}


void CModeStack::motionEnd()
{
	pop();
}
