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
// License as published by the Free Software ; either
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

/** @file tokenmap.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Maps strings to a unique token, implementation.
 */

#include "ricpp/declaration/tokenmap.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#include <iostream>

using namespace RiCPP;

void CTokenMap::clear()
{
	m_tokenMapper.clear();
	std::list<const char *>::iterator i;
	for ( i = m_strList.begin(); i != m_strList.end(); i++ ) {
		if ( (*i) != 0 )
			delete [](*i);
	}
}

CTokenMap::~CTokenMap()
{
	clear();
}


void CTokenMap::defaultTokens()
{
	try {

		// Standard Tokens
		
		m_tokenMapper[CToken(RI_NULL)] = RI_NULL;

		m_tokenMapper[CToken(RI_FRAMEBUFFER)] = RI_FRAMEBUFFER;
		m_tokenMapper[CToken(RI_FILE)] = RI_FILE;

		m_tokenMapper[CToken(RI_RGB)] = RI_RGB;
		m_tokenMapper[CToken(RI_RGBA)] = RI_RGBA;
		m_tokenMapper[CToken(RI_RGBZ)] = RI_RGBZ;
		m_tokenMapper[CToken(RI_RGBAZ)] = RI_RGBAZ;
		m_tokenMapper[CToken(RI_A)] = RI_A;
		m_tokenMapper[CToken(RI_Z)] = RI_Z;
		m_tokenMapper[CToken(RI_AZ)] = RI_AZ;

		m_tokenMapper[CToken(RI_PERSPECTIVE)] = RI_PERSPECTIVE;
		m_tokenMapper[CToken(RI_ORTHOGRAPHIC)] = RI_ORTHOGRAPHIC;

		m_tokenMapper[CToken(RI_HIDDEN)] = RI_HIDDEN;
		m_tokenMapper[CToken(RI_PAINT)] = RI_PAINT;

		m_tokenMapper[CToken(RI_CONSTANT)] = RI_CONSTANT;
		m_tokenMapper[CToken(RI_SMOOTH)] = RI_SMOOTH;

		m_tokenMapper[CToken(RI_FLATNESS)] = RI_FLATNESS;

		m_tokenMapper[CToken(RI_FOV)] = RI_FOV;

		m_tokenMapper[CToken(RI_AMBIENTLIGHT)] = RI_AMBIENTLIGHT;
		m_tokenMapper[CToken(RI_POINTLIGHT)] = RI_POINTLIGHT;
		m_tokenMapper[CToken(RI_DISTANTLIGHT)] = RI_DISTANTLIGHT;
		m_tokenMapper[CToken(RI_SPOTLIGHT)] = RI_SPOTLIGHT;

		m_tokenMapper[CToken(RI_INTENSITY)] = RI_INTENSITY;
		m_tokenMapper[CToken(RI_LIGHTCOLOR)] = RI_LIGHTCOLOR;
		m_tokenMapper[CToken(RI_FROM)] = RI_FROM;
		m_tokenMapper[CToken(RI_TO)] = RI_TO;
		m_tokenMapper[CToken(RI_CONEANGLE)] = RI_CONEANGLE;
		m_tokenMapper[CToken(RI_CONEDELTAANGLE)] = RI_CONEDELTAANGLE;
		m_tokenMapper[CToken(RI_BEAMDISTRIBUTION)] = RI_BEAMDISTRIBUTION;

		m_tokenMapper[CToken(RI_MATTE)] = RI_MATTE;
		m_tokenMapper[CToken(RI_METAL)] = RI_METAL;
		m_tokenMapper[CToken(RI_SHINYMETAL)] = RI_SHINYMETAL;
		m_tokenMapper[CToken(RI_PLASTIC)] = RI_PLASTIC;
		m_tokenMapper[CToken(RI_PAINTEDPLASTIC)] = RI_PAINTEDPLASTIC;
		m_tokenMapper[CToken(RI_KA)] = RI_KA;
		m_tokenMapper[CToken(RI_KD)] = RI_KD;
		m_tokenMapper[CToken(RI_KS)] = RI_KS;
		m_tokenMapper[CToken(RI_ROUGHNESS)] = RI_ROUGHNESS;
		m_tokenMapper[CToken(RI_KR)] = RI_KR;
		m_tokenMapper[CToken(RI_TEXTURENAME)] = RI_TEXTURENAME;
		m_tokenMapper[CToken(RI_SPECULARCOLOR)] = RI_SPECULARCOLOR;

		m_tokenMapper[CToken(RI_DEPTHCUE)] = RI_DEPTHCUE;
		m_tokenMapper[CToken(RI_FOG)] = RI_FOG;

		m_tokenMapper[CToken(RI_BUMPY)] = RI_BUMPY;

		m_tokenMapper[CToken(RI_MINDISTANCE)] = RI_MINDISTANCE;
		m_tokenMapper[CToken(RI_MAXDISTANCE)] = RI_MAXDISTANCE;
		m_tokenMapper[CToken(RI_BACKGROUND)] = RI_BACKGROUND;
		m_tokenMapper[CToken(RI_DISTANCE)] = RI_DISTANCE;
		m_tokenMapper[CToken(RI_AMPLITUDE)] = RI_AMPLITUDE;

		m_tokenMapper[CToken(RI_RASTER)] = RI_RASTER;
		m_tokenMapper[CToken(RI_SCREEN)] = RI_SCREEN;
		m_tokenMapper[CToken(RI_CAMERA)] = RI_CAMERA;
		m_tokenMapper[CToken(RI_WORLD)] = RI_WORLD;
		m_tokenMapper[CToken(RI_OBJECT)] = RI_OBJECT;

		m_tokenMapper[CToken(RI_INSIDE)] = RI_INSIDE;
		m_tokenMapper[CToken(RI_OUTSIDE)] = RI_OUTSIDE;
		m_tokenMapper[CToken(RI_LH)] = RI_LH;
		m_tokenMapper[CToken(RI_RH)] = RI_RH;

		m_tokenMapper[CToken(RI_P)] = RI_P;
		m_tokenMapper[CToken(RI_PZ)] = RI_PZ;
		m_tokenMapper[CToken(RI_PW)] = RI_PW;
		m_tokenMapper[CToken(RI_N)] = RI_N;
		m_tokenMapper[CToken(RI_NP)] = RI_NP;
		m_tokenMapper[CToken(RI_CS)] = RI_CS;
		m_tokenMapper[CToken(RI_OS)] = RI_OS;
		m_tokenMapper[CToken(RI_S)] = RI_S;
		m_tokenMapper[CToken(RI_T)] = RI_T;
		m_tokenMapper[CToken(RI_ST)] = RI_ST;

		m_tokenMapper[CToken(RI_BILINEAR)] = RI_BILINEAR;
		m_tokenMapper[CToken(RI_BICUBIC)] = RI_BICUBIC;

		m_tokenMapper[CToken(RI_PRIMITIVE)] = RI_PRIMITIVE;
		m_tokenMapper[CToken(RI_INTERSECTION)] = RI_INTERSECTION;
		m_tokenMapper[CToken(RI_UNION)] = RI_UNION;
		m_tokenMapper[CToken(RI_DIFFERENCE)] = RI_DIFFERENCE;

		m_tokenMapper[CToken(RI_PERIODIC)] = RI_PERIODIC;
		m_tokenMapper[CToken(RI_NONPERIODIC)] = RI_NONPERIODIC;
		m_tokenMapper[CToken(RI_CLAMP)] = RI_CLAMP;
		m_tokenMapper[CToken(RI_BLACK)] = RI_BLACK;

		m_tokenMapper[CToken(RI_IGNORE)] = RI_IGNORE;
		m_tokenMapper[CToken(RI_PRINT)] = RI_PRINT;
		m_tokenMapper[CToken(RI_ABORT)] = RI_ABORT;
		m_tokenMapper[CToken(RI_HANDLER)] = RI_HANDLER;

		// Additional tokens

		m_tokenMapper[CToken(RI_CATMULLCLARK)] = RI_CATMULLCLARK;
		m_tokenMapper[CToken(RI_HOLE)] = RI_HOLE;
		m_tokenMapper[CToken(RI_CREASE)] = RI_CREASE;
		m_tokenMapper[CToken(RI_CORNER)] = RI_CORNER;
		m_tokenMapper[CToken(RI_INTERPOLATEBOUNDARY)] = RI_INTERPOLATEBOUNDARY;

		m_tokenMapper[CToken(RI_ORIGIN)] = RI_ORIGIN;
		m_tokenMapper[CToken(RI_NAME)] = RI_NAME;

		m_tokenMapper[CToken(RI_COMMENT)] = RI_COMMENT;
		m_tokenMapper[CToken(RI_STRUCTURE)] = RI_STRUCTURE;
		m_tokenMapper[CToken(RI_VERBATIM)] = RI_VERBATIM;

		m_tokenMapper[CToken(RI_LINEAR)] = RI_LINEAR;
		m_tokenMapper[CToken(RI_CUBIC)] = RI_CUBIC;

		m_tokenMapper[CToken(RI_WIDTH)] = RI_WIDTH;
		m_tokenMapper[CToken(RI_CONSTANTWIDTH)] = RI_CONSTANTWIDTH;

		m_tokenMapper[CToken(RI_DISPWIDTH)] = RI_DISPWIDTH;
		m_tokenMapper[CToken(RI_DISPHEIGHT)] = RI_DISPHEIGHT;
		m_tokenMapper[CToken(RI_PIXELASPECT)] = RI_PIXELASPECT;

		m_tokenMapper[CToken(RI_CURRENT)] = RI_CURRENT;
		m_tokenMapper[CToken(RI_EYE)] = RI_EYE;
		m_tokenMapper[CToken(RI_SHADER)] = RI_SHADER;
		m_tokenMapper[CToken(RI_NDC)] = RI_NDC;

		m_tokenMapper[CToken(RI_FILEPOINTER)] = RI_FILEPOINTER;
		m_tokenMapper[CToken(RI_MEMORY)] = RI_MEMORY;

		m_tokenMapper[CToken(RI_ARCHIVE)] = RI_ARCHIVE;
		m_tokenMapper[CToken(RI_DRAFT)] = RI_DRAFT;
		m_tokenMapper[CToken(RI_REALISTIC)] = RI_REALISTIC;

		m_tokenMapper[CToken(RI_FLOAT)] = RI_FLOAT;
		m_tokenMapper[CToken(RI_INTEGER)] = RI_INTEGER;
		m_tokenMapper[CToken(RI_STRING)] = RI_STRING;
		m_tokenMapper[CToken(RI_POINT)] = RI_POINT;
		m_tokenMapper[CToken(RI_VECTOR)] = RI_VECTOR;
		m_tokenMapper[CToken(RI_NORMAL)] = RI_NORMAL;
		m_tokenMapper[CToken(RI_HPOINT)] = RI_HPOINT;
		m_tokenMapper[CToken(RI_MATRIX)] = RI_MATRIX;
		m_tokenMapper[CToken(RI_COLOR)] = RI_COLOR;

		m_tokenMapper[CToken(RI_UNIFORM)] = RI_UNIFORM;
		m_tokenMapper[CToken(RI_VARYING)] = RI_VARYING;
		m_tokenMapper[CToken(RI_VERTEX)] = RI_VERTEX;
		m_tokenMapper[CToken(RI_FACEVARYING)] = RI_FACEVARYING;
		m_tokenMapper[CToken(RI_FACEVERTEX)] = RI_FACEVERTEX;

		m_tokenMapper[CToken(RI_HANDLEID)] = RI_HANDLEID;

		m_tokenMapper[CToken(RI_PROJECTION)] = RI_PROJECTION;
		m_tokenMapper[CToken(RI_IMAGER)] = RI_IMAGER;
		m_tokenMapper[CToken(RI_DISPLAY)] = RI_DISPLAY;
		m_tokenMapper[CToken(RI_HIDER)] = RI_HIDER;
		m_tokenMapper[CToken(RI_OPTION)] = RI_OPTION;
		m_tokenMapper[CToken(RI_LIGHT_SOURCE)] = RI_LIGHT_SOURCE;
		m_tokenMapper[CToken(RI_AREA_LIGHT_SOURCE)] = RI_AREA_LIGHT_SOURCE;
		m_tokenMapper[CToken(RI_SURFACE)] = RI_SURFACE;
		m_tokenMapper[CToken(RI_ATMOSPHERE)] = RI_ATMOSPHERE;
		m_tokenMapper[CToken(RI_INTERIOR)] = RI_INTERIOR;
		m_tokenMapper[CToken(RI_EXTERIOR)] = RI_EXTERIOR;
		m_tokenMapper[CToken(RI_DISPLACEMENT)] = RI_DISPLACEMENT;
		m_tokenMapper[CToken(RI_DEFORMATION)] = RI_DEFORMATION;
		m_tokenMapper[CToken(RI_ATTRIBUTE)] = RI_ATTRIBUTE;
		m_tokenMapper[CToken(RI_TEXTURE)] = RI_TEXTURE;
		m_tokenMapper[CToken(RI_CONTROL)] = RI_CONTROL;
		m_tokenMapper[CToken(RI_BEGIN)] = RI_BEGIN;
		m_tokenMapper[CToken(RI_RESOURCE)] = RI_RESOURCE;
		
		m_tokenMapper[CToken(RI_FRAME)] = RI_FRAME;

		m_tokenMapper[CToken(RI_DEVIATION)] = RI_DEVIATION;
		m_tokenMapper[CToken(RI_TESSELATION)] = RI_TESSELATION;

		m_tokenMapper[CToken(RI_POINTS)] = RI_POINTS;
		m_tokenMapper[CToken(RI_LINES)] = RI_LINES;

		m_tokenMapper[CToken(RI_SENSE)] = RI_SENSE;

		m_tokenMapper[CToken(RI_DELAYED_READ_ARCHIVE)] = RI_DELAYED_READ_ARCHIVE;
		m_tokenMapper[CToken(RI_RUN_PROGRAM)] = RI_RUN_PROGRAM;
		m_tokenMapper[CToken(RI_DYNAMIC_LOAD)] = RI_DYNAMIC_LOAD;
		m_tokenMapper[CToken(RI_FREE)] = RI_FREE;

		m_tokenMapper[CToken(RI_GAUSSIAN)] = RI_GAUSSIAN;
		m_tokenMapper[CToken(RI_BOX)] = RI_BOX;
		m_tokenMapper[CToken(RI_TRIANGLE)] = RI_TRIANGLE;
		m_tokenMapper[CToken(RI_SINC)] = RI_SINC;

		m_tokenMapper[CToken(RI_BEZIER)] = RI_BEZIER;
		m_tokenMapper[CToken(RI_B_SPLINE)] = RI_B_SPLINE;
		m_tokenMapper[CToken(RI_CATMULL_ROM)] = RI_CATMULL_ROM;
		m_tokenMapper[CToken(RI_CATMULLROM)] = RI_CATMULLROM;
		m_tokenMapper[CToken(RI_HERMITE)] = RI_HERMITE;
		m_tokenMapper[CToken(RI_POWER)] = RI_POWER;

		m_tokenMapper[CToken(RI_BUMP)] = RI_BUMP;
		m_tokenMapper[CToken(RI_SHADOW)] = RI_SHADOW;
		m_tokenMapper[CToken(RI_LAT_LONG_ENVIRONMENT)] = RI_LAT_LONG_ENVIRONMENT;
		m_tokenMapper[CToken(RI_CUBE_FACE_ENVIRONMENT)] = RI_CUBE_FACE_ENVIRONMENT;
		m_tokenMapper[CToken(RI_BRICK_MAP)] = RI_BRICK_MAP;

		m_tokenMapper[CToken(RI_INT)] = RI_INT;
		m_tokenMapper[CToken(RI_NULL_LIT)] = RI_NULL_LIT;
		m_tokenMapper[CToken(RI_UNKNOWN)] = RI_UNKNOWN;
		// RI_EMPTY same token as RI_NULL

	} catch (...) {
		// If there was an error, a token was not created.
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Could not initialize tokenmap");
	}
}


RtToken CTokenMap::findCreate(const char *name)
// throw ExceptRiCPPError;
{
	if ( !name )
		return RI_NULL;
	if ( !*name )
		return RI_NULL; // don't use RI_EMPTY as token to simplify compare
		
	const_iterator iter;
	char *newtok = 0;

	if ( (iter = m_tokenMapper.find(name)) == m_tokenMapper.end() ) {
		
		// Trim leading and trailing blanks
		std::string aName = name;
		trimBoth(aName);
		name = aName.c_str();

		if ( (iter = m_tokenMapper.find(name)) == m_tokenMapper.end() ) {
			try {
				size_t size = strlen(name)+1;
				newtok = new char[size];
				if ( newtok ) {
#ifdef _WIN32
					strcpy_s(newtok, size, name);
#else
					strcpy(newtok, name);
#endif
					m_strList.push_back(newtok);
					m_tokenMapper[CToken(newtok)] = newtok;
				}

			} catch(...) {
				// If there was an error, the token was not created. Handled by the next few lines.
			}

			iter = m_tokenMapper.find(CToken(name));
			if ( iter == m_tokenMapper.end() ) {
				if ( newtok )
					delete[] newtok;
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Could not create token \"%s\"", name);
			}
		}
	}

	return iter->second;
}


RtToken CTokenMap::find(const char *name) const
{
	if ( !name )
		return RI_NULL;
	if ( !*name )
		return RI_NULL; // don't use RI_EMPTY as token to simplify compare
		
	const_iterator iter;
	if ( (iter = m_tokenMapper.find(CToken(name))) == m_tokenMapper.end() ) {
		return RI_NULL;
	}

	return iter->second;
}
