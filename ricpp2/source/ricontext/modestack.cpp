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

#include "ricpp/ricontext/modestack.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#include <cassert>

using namespace RiCPP;

CValidModes::CValidModes()
{
    // Resource blocks are transparent to other requests.
    // Conditional blocks are transparent to other requests.
	const TypeModeBits nowhereBits = 0;
	const TypeModeBits everywhereBits = 0xFFFF;
	const TypeModeBits insideBits = everywhereBits & ~MODE_BIT_OUTSIDE;
	const TypeModeBits optionBits =  MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_ARCHIVE;
	const TypeModeBits textureBits =  MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_ARCHIVE;
	const TypeModeBits attributeBits = insideBits & ~MODE_BIT_OBJECT;
	const TypeModeBits geometryBits =  MODE_BIT_WORLD | MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM | MODE_BIT_SOLID | MODE_BIT_ARCHIVE | MODE_BIT_OBJECT | MODE_BIT_MOTION | MODE_BIT_ARCHIVE;
	const TypeModeBits transformBits = insideBits;
	
	for ( int i=0; i < N_REQUESTS; ++i )
		m_requests[i] = nowhereBits; 

	m_requests[REQ_UNKNOWN] = nowhereBits;
	
	m_requests[REQ_ERROR_HANDLER] = everywhereBits & ~MODE_BIT_MOTION;
	
	m_requests[REQ_DECLARE] = insideBits & ~MODE_BIT_MOTION;
	m_requests[REQ_RESOURCE] = insideBits & ~MODE_BIT_MOTION;

	m_requests[REQ_GET_CONTEXT] = everywhereBits;
	m_requests[REQ_CONTEXT] = everywhereBits;

	m_requests[REQ_BEGIN] = everywhereBits;
	m_requests[REQ_END] = MODE_BIT_BEGIN;

	m_requests[REQ_FRAME_BEGIN] = MODE_BIT_BEGIN| MODE_BIT_ARCHIVE;
	m_requests[REQ_FRAME_END] = MODE_BIT_FRAME;

	m_requests[REQ_WORLD_BEGIN] = MODE_BIT_BEGIN | MODE_BIT_FRAME | MODE_BIT_ARCHIVE;
	m_requests[REQ_WORLD_END] = MODE_BIT_WORLD;

	m_requests[REQ_ATTRIBUTE_BEGIN] = insideBits & ~MODE_BIT_MOTION; // geometryBits & ~MODE_BIT_MOTION
	m_requests[REQ_ATTRIBUTE_END] = MODE_BIT_ATTRIBUTE;

	m_requests[REQ_TRANSFORM_BEGIN] = insideBits & ~MODE_BIT_MOTION; // geometryBits & ~MODE_BIT_MOTION
	m_requests[REQ_TRANSFORM_END] = MODE_BIT_TRANSFORM;

	m_requests[REQ_SOLID_BEGIN] = geometryBits & ~MODE_BIT_MOTION;
	m_requests[REQ_SOLID_END] = MODE_BIT_SOLID;

	m_requests[REQ_OBJECT_BEGIN] = insideBits & ~MODE_BIT_OBJECT & ~MODE_BIT_MOTION;
	m_requests[REQ_OBJECT_END] = MODE_BIT_OBJECT;

	m_requests[REQ_RESOURCE_BEGIN] = insideBits & ~MODE_BIT_RESOURCE & ~MODE_BIT_MOTION;
	m_requests[REQ_RESOURCE_END] = MODE_BIT_RESOURCE;

	m_requests[REQ_ARCHIVE_BEGIN] = insideBits & ~MODE_BIT_MOTION;
	m_requests[REQ_ARCHIVE_END] = MODE_BIT_ARCHIVE;

	m_requests[REQ_MOTION_BEGIN] = geometryBits & ~MODE_BIT_MOTION;
	m_requests[REQ_MOTION_END] = MODE_BIT_MOTION;
	
	m_requests[REQ_IF_BEGIN] = insideBits & ~MODE_BIT_MOTION;
	m_requests[REQ_ELSE_IF] = MODE_BIT_IF;
	m_requests[REQ_ELSE] = MODE_BIT_IF | MODE_BIT_ELSE_IF;
	m_requests[REQ_IF_END] = MODE_BIT_IF | MODE_BIT_ELSE_IF | MODE_BIT_ELSE;
	

	m_requests[REQ_FORMAT] = optionBits;
	m_requests[REQ_FRAME_ASPECT_RATIO] = optionBits;
	m_requests[REQ_SCREEN_WINDOW] = optionBits;
	m_requests[REQ_CROP_WINDOW] = optionBits;
	m_requests[REQ_PROJECTION] = optionBits;
	m_requests[REQ_CLIPPING] = optionBits;
	m_requests[REQ_CLIPPING_PLANE] = optionBits;
	m_requests[REQ_DEPTH_OF_FIELD] = optionBits;
	m_requests[REQ_SHUTTER] = optionBits;
	m_requests[REQ_PIXEL_VARIANCE] = optionBits;
	m_requests[REQ_PIXEL_SAMPLES] = optionBits;
	m_requests[REQ_PIXEL_FILTER] = optionBits;
	m_requests[REQ_EXPOSURE] = optionBits;
	m_requests[REQ_IMAGER] = optionBits;
	m_requests[REQ_QUANTIZE] = optionBits;
	m_requests[REQ_DISPLAY_CHANNEL] = optionBits;
	m_requests[REQ_DISPLAY] = optionBits;
	m_requests[REQ_HIDER] = optionBits;
	m_requests[REQ_COLOR_SAMPLES] = optionBits;
	m_requests[REQ_RELATIVE_DETAIL] = optionBits;
	m_requests[REQ_OPTION] = MODE_BIT_OUTSIDE | optionBits;

	m_requests[REQ_LIGHT_SOURCE] = attributeBits;
	m_requests[REQ_AREA_LIGHT_SOURCE] = attributeBits;

	m_requests[REQ_ATTRIBUTE] = attributeBits;
	m_requests[REQ_COLOR] = attributeBits;
	m_requests[REQ_OPACITY] = attributeBits;
	m_requests[REQ_SURFACE] = attributeBits;
	m_requests[REQ_ATMOSPHERE] = attributeBits;
	m_requests[REQ_INTERIOR] = attributeBits;
	m_requests[REQ_EXTERIOR] = attributeBits;
	m_requests[REQ_ILLUMINATE] = attributeBits;
	m_requests[REQ_DISPLACEMENT] = attributeBits;
	m_requests[REQ_TEXTURE_COORDINATES] = attributeBits;
	m_requests[REQ_SHADING_RATE] = attributeBits;
	m_requests[REQ_SHADING_INTERPOLATION] = attributeBits;
	m_requests[REQ_MATTE] = attributeBits;
	m_requests[REQ_BOUND] = attributeBits;
	m_requests[REQ_DETAIL] = attributeBits;
	m_requests[REQ_DETAIL_RANGE] = attributeBits & ~MODE_BIT_MOTION;
	m_requests[REQ_GEOMETRIC_APPROXIMATION] = attributeBits;
	m_requests[REQ_GEOMETRIC_REPRESENTATION] = attributeBits;
	m_requests[REQ_ORIENTATION] = attributeBits;
	m_requests[REQ_REVERSE_ORIENTATION] = attributeBits;
	m_requests[REQ_SIDES] = attributeBits;
	m_requests[REQ_BASIS] = attributeBits | MODE_BIT_OBJECT;
	m_requests[REQ_TRIM_CURVE] = attributeBits | MODE_BIT_OBJECT;

	m_requests[REQ_IDENTITY] = transformBits;
	m_requests[REQ_TRANSFORM] = transformBits;
	m_requests[REQ_CONCAT_TRANSFORM] = transformBits;
	m_requests[REQ_PERSPECTIVE] = transformBits;
	m_requests[REQ_TRANSLATE] = transformBits;
	m_requests[REQ_ROTATE] = transformBits;
	m_requests[REQ_SCALE] = transformBits;
	m_requests[REQ_SKEW] = transformBits;
	m_requests[REQ_DEFORMATION] = transformBits;
	m_requests[REQ_COORDINATE_SYSTEM] = transformBits & ~MODE_BIT_MOTION;
	m_requests[REQ_SCOPED_COORDINATE_SYSTEM] = transformBits & ~MODE_BIT_MOTION;
	m_requests[REQ_COORD_SYS_TRANSFORM] = transformBits & ~MODE_BIT_MOTION;
	m_requests[REQ_TRANSFORM_POINTS] = transformBits & ~MODE_BIT_MOTION;

	m_requests[REQ_POLYGON] = geometryBits;
	m_requests[REQ_GENERAL_POLYGON] = geometryBits;
	m_requests[REQ_POINTS_POLYGONS] = geometryBits;
	m_requests[REQ_POINTS_GENERAL_POLYGONS] = geometryBits;

	m_requests[REQ_PATCH] = geometryBits;
	m_requests[REQ_PATCH_MESH] = geometryBits;
	m_requests[REQ_NU_PATCH] = geometryBits;

	m_requests[REQ_SUBDIVISION_MESH] = geometryBits;
	m_requests[REQ_HIERARCHICAL_SUBDIVISION_MESH] = geometryBits;

	m_requests[REQ_SPHERE] = geometryBits;
	m_requests[REQ_CONE] = geometryBits;
	m_requests[REQ_CYLINDER] = geometryBits;
	m_requests[REQ_HYPERBOLOID] = geometryBits;
	m_requests[REQ_PARABOLOID] = geometryBits;
	m_requests[REQ_DISK] = geometryBits;
	m_requests[REQ_TORUS] = geometryBits;

	m_requests[REQ_POINTS] = geometryBits;
	m_requests[REQ_CURVES] = geometryBits;

	m_requests[REQ_BLOBBY] = geometryBits;

	m_requests[REQ_PROCEDURAL] = geometryBits;
	m_requests[REQ_PROC_DELAYED_READ_ARCHIVE] = geometryBits;
	m_requests[REQ_PROC_RUN_PROGRAM] = geometryBits;
	m_requests[REQ_PROC_DYNAMIC_LOAD] = geometryBits;

	m_requests[REQ_GEOMETRY] = geometryBits;

	m_requests[REQ_OBJECT_INSTANCE] = geometryBits;
	m_requests[REQ_ARCHIVE_INSTANCE] = everywhereBits;

	m_requests[REQ_MAKE_TEXTURE] = textureBits;
	m_requests[REQ_MAKE_BUMP] = textureBits;
	m_requests[REQ_MAKE_LAT_LONG_ENVIRONMENT] = textureBits;
	m_requests[REQ_MAKE_CUBE_FACE_ENVIRONMENT] = textureBits;
	m_requests[REQ_MAKE_SHADOW] = textureBits;
	m_requests[REQ_MAKE_BRICK_MAP] = textureBits;

	m_requests[REQ_ARCHIVE_RECORD] = everywhereBits & ~MODE_BIT_MOTION;
	m_requests[REQ_READ_ARCHIVE] = everywhereBits;
	
	m_requests[REQ_CONTROL] = everywhereBits & ~MODE_BIT_MOTION;

	m_requests[REQ_SYSTEM] = MODE_BIT_OUTSIDE | MODE_BIT_BEGIN | MODE_BIT_FRAME;
	m_requests[REQ_VERSION] = MODE_BIT_BEGIN;
}

bool CValidModes::isValid(EnumRequests idxRequest, TypeModeBits idxModeBit) const
{
	return (m_requests[(int)idxRequest] & idxModeBit) != 0;
}


// --------------------
bool CModeStack::validRequest(EnumRequests req) const
{
	/*  Attribute and transform blocks are valid outside the world block, too. But
	 *  the contents of those blocks not necessarily - make attribute and transform blocks "transparent".
	 */
	TypeModeBits modeBit = curModeBits();
	
	if ( req != REQ_ATTRIBUTE_END && req != REQ_TRANSFORM_END ) {
		size_t theSize = m_modeBits.size();
		while ( theSize > 1 && modeBit & (MODE_BIT_ATTRIBUTE | MODE_BIT_TRANSFORM) != 0 ) {
			--theSize;
			modeBit = m_modeBits[theSize-1];
		}
	}
	
	return m_validModes.isValid(req, curModeBits());
}

void CModeStack::push(EnumModes newMode, TypeModeBits newModeBits)
{
	try {
		m_modes.push_back(newMode);
		m_modeBits.push_back(newModeBits);
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in CModeStack:push(): %s", e.what());
	}
}

void CModeStack::pop()
{
	if ( !m_modes.empty() ) {	
		m_modes.pop_back();
	}
	if ( !m_modeBits.empty() ) {	
		m_modeBits.pop_back();
	}
}

void CModeStack::clear()
{
	m_modes.clear();
	m_modeBits.clear();
}

void CModeStack::contextBegin()
{
	if ( MODE_OUTSIDE != curMode() ) {
		m_modes.clear();
	}
	push(MODE_BEGIN, MODE_BIT_BEGIN);
}


void CModeStack::contextEnd()
{
	clear();
}


void CModeStack::frameBegin()
{
	push(MODE_FRAME, MODE_BIT_FRAME);
}


void CModeStack::frameEnd()
{
	assert(curMode() == MODE_FRAME);
	pop();
}


void CModeStack::worldBegin()
{
	push(MODE_WORLD, MODE_BIT_WORLD);
}


void CModeStack::worldEnd()
{
	assert(curMode() == MODE_WORLD);
	pop();
}


void CModeStack::attributeBegin()
{
	push(MODE_ATTRIBUTE, MODE_BIT_ATTRIBUTE);
}


void CModeStack::attributeEnd()
{
	assert(curMode() == MODE_ATTRIBUTE);
	pop();
}


void CModeStack::transformBegin()
{
	push(MODE_TRANSFORM, MODE_BIT_TRANSFORM);
}


void CModeStack::transformEnd()
{
	assert(curMode() == MODE_TRANSFORM);
	pop();
}


void CModeStack::solidBegin()
{
	push(MODE_SOLID, MODE_BIT_SOLID);
}


void CModeStack::solidEnd()
{
	assert(curMode() == MODE_SOLID);
	pop();
}


void CModeStack::objectBegin()
{
	push(MODE_OBJECT, MODE_BIT_OBJECT);
}


void CModeStack::objectEnd()
{
	assert(curMode() == MODE_OBJECT);
	pop();
}


void CModeStack::archiveBegin()
{
	push(MODE_ARCHIVE, MODE_BIT_ARCHIVE);
}


void CModeStack::archiveEnd()
{
	assert(curMode() == MODE_ARCHIVE);
	pop();
}

void CModeStack::resourceBegin() {
	push(MODE_RESOURCE, MODE_BIT_RESOURCE|curModeBits());
}

void CModeStack::resourceEnd() {
	assert(curMode() == MODE_RESOURCE);
	pop();
}

void CModeStack::ifBegin() {
	push(MODE_IF, MODE_BIT_IF|curModeBits());
}

void CModeStack::elseIfBegin() {
	assert(curMode() == MODE_IF);
	pop();
	push(MODE_ELSE_IF, MODE_BIT_ELSE_IF|curModeBits());
}

void CModeStack::elseBegin() {
	assert(curMode() == MODE_IF || curMode() == MODE_ELSE_IF);
	pop();
	push(MODE_ELSE, MODE_BIT_ELSE|curModeBits());
}

void CModeStack::ifEnd() {
	assert(curMode() == MODE_IF || curMode() == MODE_ELSE_IF || curMode() == MODE_ELSE);
	pop();
}

void CModeStack::motionBegin()
{
	push(MODE_MOTION, MODE_BIT_MOTION);
}


void CModeStack::motionEnd()
{
	assert(curMode() == MODE_MOTION);
	pop();
}

void CModeStack::startAreaLightSource(RtLightHandle h)
{
	if ( h == illLightHandle ) {
		endAreaLightSource();
		return;
	}
	
	if ( m_areaLightSourceHandle != illLightHandle ) {
		endAreaLightSource();		
	}

	m_areaLightSourceHandle = h;
	m_areaLightSourceDepth = size();
}

void CModeStack::endAreaLightSource()
{
	if ( size() != m_areaLightSourceDepth ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR,  __LINE__, __FILE__, "%s", "AreaLightSource block closed at wrong nesting level.");
	}
	m_areaLightSourceHandle = illLightHandle;
	m_areaLightSourceDepth = 0;
}
