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

/*! \file libribout.cpp
 *  \brief RIB output
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "libribout.h"

using namespace std;

void TRIBContext::closeStream() {
	if ( m_ribStream ) {
		m_ribStream->close();
		delete m_ribStream;
	}
	m_ribStream = NULL;
}

void TRIBContext::openStream(RtToken name) {
	if ( m_ribStream ) {
		m_ribStream->close();
		delete m_ribStream;
	}
	m_ribStream = NULL;
	setlocale( LC_ALL, "C" );
	if ( name && name[0] )
		m_ribStream = new std::ofstream(name, ios::out);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TRIBOut::TRIBOut() {
    m_rendererName = "TRIBout";
}


TRIBOut::~TRIBOut() {
	if ( m_currentContext )
		((TRIBContext *)m_currentContext)->closeStream();
}

TRiCPPContext *TRIBOut::getNewContext() {
	return new TRIBContext;
}

const char *TRIBOut::strout(std::string &result, const char *input) {
	result = "";
	char buffer[5];
	if ( input ) {
		buffer[1] = (char)0;
		buffer[4] = (char)0;
		for ( ; *input; ++input ) {
			if ( *input >= (char)32 && *input <= (char)126 ) {
				buffer[0]=*input;
				if ( *input == '\\' ) {
					buffer[1]=*input;
					buffer[2]=(char)0;
				}
				result += buffer;
			} else {
				unsigned int i = (unsigned char)*input;
				buffer[0] = '\\';
				buffer[3] = (char)(i%8);
				i /= 8;
				buffer[2] = (char)(i%8);
				i /= 8;
				buffer[1] = (char)(i%8);
				result += buffer;
				buffer[1] = (char)0;
			}
		}
	}
	return result.c_str();
}

void TRIBOut::paramOut(const TParameterDeclaration &decl, std::ofstream &of, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtPointer p) {
	int i;
	int num = decl.numComponents(vertices, corners, facets, faceVertices, faceCorners, colorComps);

	of << "[ ";
	if ( p ) {
		switch ( decl.getType() ) {
			case TYPE_BOOLEAN :
				for ( i=0; i < num; ++i ) {
					of << (((RtBoolean *)p)[i] ? "true " : "false ");
				}
				break;
			case TYPE_INTEGER :
				for ( i=0; i < num; ++i ) {
					// of << ((RtInt *)p)[i] << " ";
					of << ((RtFloat *)p)[i] << " "; // Integer are also treated as float, there are no integers in the parameter list
				}
				break;
			case TYPE_FLOAT :
				for ( i=0; i < num; ++i ) {
					of << ((RtFloat *)p)[i] << " ";
				}
				break;
			case TYPE_TOKEN :
				for ( i=0; i < num; ++i ) {
					of << "\"" << *((RtToken *)p)[i] << "\" ";
				}
				break;
			case TYPE_COLOR  : // nobreak
			case TYPE_POINT  : // nobreak
			case TYPE_VECTOR : // nobreak
			case TYPE_NORMAL : // nobreak
			case TYPE_HPOINT : // nobreak
			case TYPE_MPOINT : // nobreak
			case TYPE_MATRIX : // nobreak
			case TYPE_BASIS  : // nobreak
			case TYPE_BOUND  : // nobreak
				for ( i = 0; i < num; ++i ) {
					of << (((RtFloat *)p)[i]) << " ";
				}
				break;
			case TYPE_STRING : {
				std::string mystring;
				for ( i=0; i < num; ++i ) {
					of << "\"" << strout(mystring, ((const char **)p)[i]) << "\" ";
				}
			}
			break;
			default :
				// error, type not handled
				handleError(RIE_BUG, RIE_WARNING, "Type of parameter is not handled.");
				break;
		}
	}
	of << "] ";
}

void TRIBOut::paramOut(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !isValid() )
		return;

	int i;
	for ( i = 0; i < n; ++i ) {
		getStream() << "\"" << tokens[i] << "\" ";
		const TParameterDeclaration *decl = getDeclMap().findDeclaration(tokens[i]);
		if ( decl ) {
			paramOut(*decl, getStream(), vertices, corners, facets, faceVertices, faceCorners, m_currentContext->m_currentOptions->m_iNColor, params[i]);
		} else {
			// Inline declaration
			TParameterDeclaration decl(NULL, tokens[i]);
			paramOut(decl, getStream(), vertices, corners, facets, faceVertices, faceCorners, m_currentContext->m_currentOptions->m_iNColor, params[i]);
		}
	}
}

void TRIBOut::matrixOut(const RtFloat *m) {
	if ( !isValid() )
		return;

	getStream() << "[ ";
	int i, j;
	for ( i = 0; i < 4; ++i ) {
		for ( j = 0; j < 4; ++j ) {
			getStream() << m[i*4 + j] << " ";
		}
	}
	getStream() << "]";
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// The interface
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////
// declare() - Declaration of options

RtToken TRIBOut::doDeclare(RtToken token, const char *name, const char *declaration) {
	getStream() << "Declare \"" << (name ? name : "") << "\" \"" << declaration << "\"" << endl;
	return token;
}

/////////////////
// blockstructure

RtVoid TRIBOut::doBegin(RtToken name) {
	((TRIBContext *)m_currentContext)->openStream(name);
}

RtVoid TRIBOut::doEnd(void) {
	((TRIBContext *)m_currentContext)->closeStream();
}

RtVoid TRIBOut::doFrameBegin(RtInt number) {
	getStream() << "FrameBegin " << number << endl;
}

RtVoid TRIBOut::doFrameEnd(void) {
	getStream() << "FrameEnd" << endl;
}


////////////////////////////////////////////////////////////////////////////////////////////
// RiWorldBegin - starts the rendering
// s.a. [Ups89]: The Digital Image
//
RtVoid TRIBOut::doWorldBegin(void) {
	getStream() << "WorldBegin" << endl;
	if ( !m_currentContext->m_currentOptions->m_mCamTrans.isIdentity() ) {
		getStream() << "Transform ";
		matrixOut((const RtFloat *)m_currentContext->m_currentOptions->m_mCamTrans);
		getStream() << endl;
	}
}

RtVoid TRIBOut::doWorldEnd(void) {
	getStream() << "WorldEnd" << endl;
}

// Synchronize
RtVoid TRIBOut::doSynchronize(RtToken name) {
	if ( name && !strcmp(name, RI_ABORT) ) {
		((TRIBContext *)m_currentContext)->closeStream();
	}
}

//

RtVoid TRIBOut::doFormat(RtInt xres, RtInt yres, RtFloat aspect) {
	if ( !m_currentContext->m_currentOptions->m_disableFormat )
		getStream() << "Format " << xres << " " << yres << " " << aspect << endl;
}

RtVoid TRIBOut::doFrameAspectRatio(RtFloat aspect) {
	getStream() << "FrameAspectRatio " << aspect << endl;
}

RtVoid TRIBOut::doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
	getStream() << "ScreenWindow " << left << " " << right << " " << bot << " " << top << endl;
}

RtVoid TRIBOut::doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
	getStream() << "CropWindow " << xmin << " " << xmax << " " << ymin << " " << ymax << endl;
}

RtVoid TRIBOut::doProjectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Projection \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doClipping(RtFloat hither, RtFloat yon) {
	getStream() << "Clipping " << hither << " " << yon << endl;
}

RtVoid TRIBOut::doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
	getStream() << "ClippingPlane " << x << " " << y << " " << z << " " << nx << " " << ny << " " << nz << endl;
}

RtVoid TRIBOut::doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
	getStream() << "DepthOfField " << fstop << " " << focallength << " " << focaldistance << endl;
}

RtVoid TRIBOut::doShutter(RtFloat smin, RtFloat smax) {
	getStream() << "Shutter " << smin << " " << smax << endl;
}

///////////////////////////////////////////////////////////////////////////////
// sampling

RtVoid TRIBOut::doPixelVariance(RtFloat variance) {
	getStream() << "PixelVariance " << variance << endl;
}

RtVoid TRIBOut::doPixelSamples(RtFloat xsamples, RtFloat ysamples) {
	getStream() << "PixelSamples " << xsamples << " " << ysamples << endl;
}

RtVoid TRIBOut::doPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) {
	getStream() << "PixelFilter \"" << getFilterTok(function) << "\" " << xwidth << " " << ywidth << endl;
}

RtVoid TRIBOut::doExposure(RtFloat gain, RtFloat gamma) {
	getStream() << "Exposure " << gain << " " << gamma << endl;
}

RtVoid TRIBOut::doImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Imager \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
	getStream() << "Quantize \"" << type << "\" " << one << " " << qmin << " " << qmax << " " << ampl << endl;
}


///////////////////////////////////////////////////////////////////////////////
// RiDisplay - setting the display device, throws RIE_NOMEM
RtVoid TRIBOut::doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {
	// m_currentContext->m_currentOptions->m_displays
	getStream() << "Display \"" << (name ? name : "null") << "\" \"" << type << "\" \"" << mode << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

// hider
RtVoid TRIBOut::doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Hider \"" << type << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doColorSamples(RtInt n, RtFloat *nRGB, RtFloat *RGBn) {
	int i;
	getStream() << "ColorSamples " << n << " [ ";
	for ( i = 0; i < n*3; ++i ) {
		getStream() << nRGB[i] << " ";
	}
	getStream() <<  "] [ ";
	for ( i = 0; i < n*3; ++i ) {
		getStream() << RGBn << " ";
	}
	getStream() << "]" << endl;
}

RtVoid TRIBOut::doRelativeDetail(RtFloat relativedetail) {
	getStream() << "RelativeDetail " << relativedetail << endl;
}

///////////////////////////////////////////////////////////////////////////////
// generic options

RtVoid TRIBOut::doOptionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	// Control option are to control the RiCPP itself and are not written
	if ( name && !strcmp(name, ri_RiCPPControl) ) {
		return;
	}

	getStream() << "Option \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// attribute blocks

RtVoid TRIBOut::doAttributeBegin(void) {
	getStream() << "AttributeBegin" << endl;
}

RtVoid TRIBOut::doAttributeEnd(void) {
	getStream() << "AttributeEnd" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// attributes

RtVoid TRIBOut::doColor(RtColor Cs) {
	getStream() << "Color ";

	if ( Cs ) {
		int i;
		for ( i = 0; i < m_currentContext->m_currentOptions->m_iNColor; ++i ) {
			getStream() << Cs[i] << " ";
		}
	}

	getStream() << endl;
}

RtVoid TRIBOut::doOpacity(RtColor Cs) {
	getStream() << "Opacity ";

	if ( Cs ) {
		int i;
		for ( i = 0; i < m_currentContext->m_currentOptions->m_iNColor; ++i ) {
			getStream() << Cs[i] << " ";
		}
	}

	getStream() << endl;
}

RtVoid TRIBOut::doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	getStream() << "TextureCoordinates " << s1 << " " << t1 << " " << s2 << " " << t2 << " " << s3 << " " << t3 << " " << s4 << " " << t4 << endl;
}

///////////////////////////////////////////////////////////////////////////////
// light source

RtLightHandle TRIBOut::doLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "LightSource \"" << (name ? name : "null") << "\" ";
	getStream() << (long)handle << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;

	return handle;
}

RtLightHandle TRIBOut::doAreaLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "AreaLightSource \"" << (name ? name : "null") << "\" ";
	getStream() << (long)handle << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;

	return handle;
}

RtVoid TRIBOut::doIlluminate(RtLightHandle light, RtBoolean onoff) {
	getStream() << "Illuminate "  << (long)light << " " << (onoff ? "1" : "0") << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Shader

RtVoid TRIBOut::doSurfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Surface \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doAtmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Atmosphere \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doInteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Interior \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doExteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Exterior \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doDisplacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Displacement \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doShadingRate(RtFloat size) {
	getStream() << "ShadingRate " << size << endl;
}

RtVoid TRIBOut::doShadingInterpolation(RtToken type) {
	getStream() << "ShadingInterpolation \"" << type << "\"" <<endl;
}

RtVoid TRIBOut::doMatte(RtBoolean onoff) {
	getStream() << "Matte " << (onoff ? 1 : 0) << endl;
}

///////////////////////////////////////////////////////////////////////////////
// model

RtVoid TRIBOut::doBound(RtBound bound) {
	getStream() << "Bound " << bound[0] << " " << bound[1] << " " << bound[2]  << " " << bound[3]  << " " << bound[4]  << " " << bound[5] << endl;
}

RtVoid TRIBOut::doDetail(RtBound bound) {
	getStream() << "Detail [ " << bound[0] << " " << bound[1] << " " << bound[2]  << " " << bound[3]  << " " << bound[4]  << " " << bound[5] << "] " << endl;
}

RtVoid TRIBOut::doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
	getStream() << "DetailRange [ " << minvis << " " << lowtran << " " << uptran  << " " << maxvis << " ]" << endl;
}

RtVoid TRIBOut::doGeometricApproximation(RtToken type, RtFloat value) {
	getStream() << "GeometricApproximation \"" << type << "\" " << value  << endl;
}

RtVoid TRIBOut::doGeometricRepresentation(RtToken type) {
	getStream() << "GeometricRepresentation \"" << type << "\"" << endl;
}

RtVoid TRIBOut::doOrientation(RtToken orientation) {
	getStream() << "Orientation \"" << orientation << "\"" << endl;
}

RtVoid TRIBOut::doReverseOrientation(void) {
	getStream() << "ReverseOrientation" << endl;
}

RtVoid TRIBOut::doSides(RtInt nsides) {
	getStream() << "Sides " << nsides << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Transformation

RtVoid TRIBOut::doIdentity(void) {
	getStream() << "Identity" << endl;
}

RtVoid TRIBOut::doTransform(RtMatrix transform) {
	getStream() << "Transform ";
	matrixOut((const RtFloat *)transform);
	getStream() << endl;
}

RtVoid TRIBOut::doConcatTransform(RtMatrix transform) {
	getStream() << "ConcatTransform ";
	matrixOut((const RtFloat *)transform);
	getStream() << endl;
}

RtVoid TRIBOut::doPerspective(RtFloat fov) {
	getStream() << "Perspective " << fov << endl;
}

RtVoid TRIBOut::doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {
	getStream() << "Translate " << dx << " " << dy << " " << dz << endl;
}

RtVoid TRIBOut::doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	getStream() << "Rotate " << angle << " " << dx << " " << dy << " " << dz << endl;
}

RtVoid TRIBOut::doScale(RtFloat dx, RtFloat dy, RtFloat dz) {
	getStream() << "Scale " << dx << " " << dy << " " << dz << endl;
}

RtVoid TRIBOut::doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
	getStream() << "Skew " << angle << " " << dx1 << " " << dy1 << " " << dz1 << " " << dx2 << " " << dy2 << " " << dz2 << endl;
}

RtVoid TRIBOut::doDeformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Deformation \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doCoordinateSystem(RtToken space) {
	getStream() << "CoordinateSystem \"" << space << "\"" << endl;
}

RtVoid TRIBOut::doCoordSysTransform(RtToken space) {
	getStream() << "CoordSysTransform \"" << space << "\"" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// transformation blocks

RtVoid TRIBOut::doTransformBegin(void) {
	getStream() << "TransformBegin" << endl;
}

RtVoid TRIBOut::doTransformEnd(void) {
	getStream() << "TransformEnd" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// generic attribute

RtVoid TRIBOut::doAttributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Attribute \"" << (name ? name : "null") << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// geometric primitives

RtVoid TRIBOut::doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Polygon " << " ";
	TPolygonClasses p(nvertices);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "GeneralPolygon [ ";
	int i;
	for ( i = 0; i < nloops; ++i ) {
		getStream() << nverts[i] << " ";
	}
	getStream() << "] ";
	TGeneralPolygonClasses p(nloops, nverts);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "PointsPolygons [ ";
	int i, j;
	for ( i = 0; i < npolys; ++i ) {
		getStream() << nverts[i] << " ";
	}
	getStream() << "] [ ";
	int np;
	for ( np = 0, i = 0; i < npolys; ++i ) {
		for ( j = 0; j < nverts[i]; ++j, ++np ) {
			getStream() << verts[np] << " ";
		}
	}
	getStream() << "] ";
	TPointsPolygonsClasses p(npolys, nverts, verts);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "PointsGeneralPolygons [ ";
	int np, np2, i, j, k;
	for ( i = 0; i < npolys; ++i ) {
		getStream() << nloops[i] << " ";
	}
	getStream() << "] [ ";
	for ( np = 0, i = 0; i < npolys; ++i ) {
		for ( j = 0; j < nloops[i]; ++j, ++np ) {
			getStream() << nverts[np] << " ";
		}
	}
	getStream() << "] [ ";
	for ( np2 = 0, np = 0, i = 0; i < npolys; ++i ) {
		for ( j = 0; j < nloops[i]; ++j, ++np ) {
			for ( k = 0; k < nverts[np]; ++k, ++np2 ) {
				getStream() << verts[np2] << " ";
			}
		}
	}
	getStream() << "] ";
	TPointsGeneralPolygonsClasses p(npolys, nloops, nverts, verts);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}


RtVoid TRIBOut::doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
	getStream() << "Basis ";
	matrixOut((const RtFloat *)ubasis);
	getStream() << " " << ustep << " ";
	matrixOut((const RtFloat *)vbasis);
	getStream() << " " << vstep << endl;
}

RtVoid TRIBOut::doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Patch \"" << (type ? type : "") << "\" ";
	TPatchClasses p(type);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "PatchMesh \"" << (type ? type : "") << "\" " << nu << " \"" << uwrap << "\" " << nv << " \"" << vwrap << "\" ";
	TPatchMeshClasses p(type, nu, m_currentContext->m_currentAttributes->m_uStep, uwrap, nv, m_currentContext->m_currentAttributes->m_vStep, vwrap);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
	int i;
	getStream() << "NuPatch " << nu << " " << uorder << " [ ";
	for ( i = 0; i < nu+uorder; ++i ) {
		getStream() << uknot[i] << " ";
		if ( i > 0 ) {
			if ( uknot[i] < uknot[i-1] ) {
				handleError(RIE_MATH, RIE_WARNING, "uknots must be in ascending order");
			}
		}
	}
	getStream() << "] " << umin << " " << umax << " " << nv << " " << vorder << " [ ";
	for ( i = 0; i < nv+vorder; ++i ) {
		getStream() << vknot[i] << " ";
		if ( i > 0 ) {
			if ( vknot[i] < vknot[i-1] ) {
				handleError(RIE_MATH, RIE_WARNING, "vknots must be in ascending order");
			}
		}
	}
	getStream() << "] " << vmin << " " << vmax << " ";

	TNuPatchClasses p(nu, uorder, nv, vorder);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
	getStream() << "TrimCurve " << " [ ";

	unsigned int i;
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_nCurves.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_nCurves[i] << " ";
	}

	getStream() << "] [ ";

	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_order.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_order[i] << " ";
	}

	getStream() << "] [ ";

	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_knots.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_knots[i] << " ";
	}

	getStream() << "] [ ";
	
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_min.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_min[i] << " ";
	}

	getStream() << "] [ ";

	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_max.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_max[i] << " ";
	}

	getStream() << "] [ ";
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_n.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_n[i] << " ";
	}

	getStream() << "] [ ";
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_u.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_u[i] << " ";
	}

	getStream() << "] [ ";
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_v.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_v[i] << " ";
	}

	getStream() << "] [ ";
	for ( i = 0; i < m_currentContext->m_currentAttributes->m_trimCurve.m_w.size(); ++i ) {
		getStream() << m_currentContext->m_currentAttributes->m_trimCurve.m_w[i] << " ";
	}

	getStream() << "]" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// quadrics

RtVoid TRIBOut::doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Cone " << height << " " << radius << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Cylinder " << radius << " " << zmin << " " << zmax << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Disk " << height << " " << radius << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Hyperboloid " << point1[0] << " " << point1[1] << " " << point1[2] << " " << point2[0] << " " << point2[1] << " " << point2[2] << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}


RtVoid TRIBOut::doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Paraboloid " << rmax << " " << zmin << " " << zmax << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Sphere " << radius << " " << zmin << " " << zmax << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Torus " << majorrad << " " << minorrad << " " << phimin << " " << phimax << " " << thetamax << " ";
	TQuadricClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

///////////////////////////////////////////////////////////////////////////////

RtVoid TRIBOut::doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( type && !strcmp(type, "teapot") ) {
		teapot();
	} else {
		getStream() << "Geometry \"" << (type ? type : "") << "\" ";
		TParameterClasses p;
		paramOut(p, n, tokens, params);
		getStream() << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
// curves, subdiv, plobs

RtVoid TRIBOut::doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Points ";
	TPointsClasses p(npts);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "Curves \"" << type << "\" [ ";
	int i;
	for ( i = 0; i < ncurves; ++i ) {
		getStream() << nverts[i] << " ";
	}
	getStream() << "] \"" << wrap << "\" ";

	TCurvesClasses p(type, ncurves, nverts, wrap, m_currentContext->m_currentAttributes->m_vStep);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[], RtInt n, RtToken tokens[], RtPointer params[]) {
	int i, j;
	getStream() << "SubdivisionMesh \"" << scheme << "\" [ ";
	for ( i = 0; i < nfaces; ++i ) {
		getStream() << nverts[i] << " ";
	}
	getStream() << "] [ ";

	RtInt *pverts = verts;
	for ( i = 0; i < nfaces; ++i ) {
		for ( j=0; j<nverts[i]; ++j ) {
			getStream() << *(pverts++) << " ";
		}
	}

	if ( ntags > 0 ) {
		getStream() << "] [ ";

		for ( i = 0; i < ntags; ++i ) {
			getStream() << "\"" << tags[i] << "\" ";
		}
		getStream() << "] [ ";

		for ( i = 0; i < ntags*2; ++i ) {
			getStream() << nargs[i] << " ";
		}
		getStream() << "] [ ";

		int sumj=0;
		for ( i = 0; i < ntags*2; i += 2 ) {
			for ( j=0; j<nargs[i]; ++j ) {
				getStream() << intargs[sumj++] << " ";
			}
		}

		getStream() << "] [ ";

		sumj=0;
		for ( i = 1; i < ntags*2; i += 2 ) {
			for ( j=0; j<nargs[i]; ++j ) {
				getStream() << floargs[sumj++] << " ";
			}
		}
	}

	getStream() << "] ";
	TSubdivisionMeshClasses p(nfaces, nverts, verts);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {
	int i;
	getStream() << "Blobby " << nleaf << " [ ";
	for ( i = 0; i < ncode; ++i ) {
		getStream() << code[i] << " ";
	}
	getStream() << "] [ ";
	for ( i = 0; i < nflt; ++i ) {
		getStream() << flt[i] << " ";
	}
	getStream() << "] [ ";
	for ( i = 0; i < nstr; ++i ) {
		getStream() << "\"" << str[i] << "\" ";
	}
	getStream() << "] ";
	TBlobbyClasses p(nleaf);
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// procedural

RtVoid TRIBOut::doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) {
	if ( subdivfunc ) {
		subdivfunc(this, data, RI_INFINITY);
	}
	if ( subdivfunc == procDelayedReadArchive || subdivfunc == procRunProgram || subdivfunc == procDynamicLoad )
		getStream() << " [ " << bound[0] << " " << bound[1] << " " << bound[2]  << " " << bound[3]  << " " << bound[4]  << " " << bound[5] << " ]" << endl;

	// Free data
	// if ( freefunc )
	//	freefunc(data);
}

RtVoid TRIBOut::handleDelayedReadArchive(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleDelayedReadArchive(data, detail);
	RtString *args = (RtString *)data;
	if ( args && args[0] ) {
		getStream() << "Procedural \"DelayedReadArchive\" [ \"";
		getStream() << args[0];
		getStream() << "\" ]";
	}
}

RtVoid TRIBOut::handleRunProgram(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleRunProgram(data, detail);
	RtString *args = (RtString *)data;
	if ( args && args[0] ) {
		getStream() << "Procedural \"RunProgram\" [ \"";
		RtString arg1 = args[1];
		if ( !arg1 )
			arg1 = "";
		getStream() << args[0] << "\" \"" << arg1;
		getStream() << "\" ]";
	}
}

RtVoid TRIBOut::handleDynamicLoad(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleDynamicLoad(data, detail);
	RtString *args = (RtString *)data;
	if ( args && args[0] ) {
		getStream() << "Procedural \"DynamicLoad\" [ \"";
		RtString arg1 = args[1];
		if ( !arg1 )
			arg1 = "";
		getStream() << args[0] << "\" \"" << arg1;
		getStream() << "\" ]";
	}
}

RtVoid TRIBOut::handleFree(RtPointer data) {
	TIntermediateRenderer::handleFree(data);
}

///////////////////////////////////////////////////////////////////////////////
// solid blocks

RtVoid TRIBOut::doSolidBegin(RtToken type) {
	getStream() << "SolidBegin \"" << type << "\"" << endl;
}

RtVoid TRIBOut::doSolidEnd(void) {
	getStream() << "SolidEnd" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// objects

RtObjectHandle TRIBOut::doObjectBegin(RtObjectHandle handle) {
	if ( m_currentContext->m_currentOptions->m_writeObject ) {
		return handleObjectBegin(handle);
	} else {
		if ( m_currentContext ) m_currentContext->pushAttributes(*this);
		getStream() << "ObjectBegin "<< (long)handle << endl;
	}
	return handle;
}

RtVoid TRIBOut::doObjectEnd(void) {
	if ( m_currentContext->m_currentOptions->m_writeObject ) {
		handleObjectEnd();
	} else {
		getStream() << "ObjectEnd" << endl;
		if ( m_currentContext ) m_currentContext->popAttributes(*this);
	}
}

RtVoid TRIBOut::doObjectInstance(RtObjectHandle handle) {
	if ( m_currentContext->m_currentOptions->m_writeObject ) {
		handleObjectInstance(handle);
	} else {
		getStream() << "ObjectInstance "<< (long)handle << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
// motion blocks

RtVoid TRIBOut::doMotionBeginV(RtInt N, RtFloat times[]) {
	getStream() << "MotionBegin [" << endl;
	int i;
	for ( i = 0; i < N; ++i ) {
		getStream() << times[i] << " " << endl;
	}
	getStream() << "]" << endl;
}

RtVoid TRIBOut::doMotionEnd(void) {
	getStream() << "MotionEnd" << endl;
}

///////////////////////////////////////////////////////////////////////////////
// texture

RtVoid TRIBOut::doMakeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "MakeTexture \"" << pic << "\" \"" << tex << "\" \"" << swrap << "\" \"" << twrap << "\" \""  << getFilterTok(filterfunc) << "\" "  << swidth << " "  << twidth << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doMakeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "MakeBump \"" << pic << "\" \"" << tex << "\" \"" << swrap << "\" \"" << twrap << "\" \""  << getFilterTok(filterfunc) << "\" "  << swidth << " "  << twidth << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doMakeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "MakeLatLongEnvironment \"" << pic << "\" \"" << tex << "\" \"" << getFilterTok(filterfunc) << "\" "  << swidth << " "  << twidth << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doMakeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "MakeCubeFaceEnvironment \"" << px << "\" \"" << nx << "\" \"" << py << "\" \"" << ny << "\" \"" << pz << "\" \"" << nz << "\" \"" << tex << "\" " << fov << " \"" << getFilterTok(filterfunc) << "\" " << swidth << " " << twidth << " ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

RtVoid TRIBOut::doMakeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) {
	getStream() << "MakeShadow \"" << pic << "\" \"" << tex << "\" ";
	TParameterClasses p;
	paramOut(p, n, tokens, params);
	getStream() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// archives

RtVoid TRIBOut::doArchiveRecordV(RtToken type, const char *line) {
	if ( type && !strcmp(type, RI_COMMENT) ) {
		getStream() << "#";
		if ( line )
			getStream() << line;
		getStream() << endl;
	} else if ( type && !strcmp(type, RI_STRUCTURE) ) {
		getStream() << "##";
		if ( line )
			getStream() << line;
		getStream() << endl;
	} else if ( type && !strcmp(type, RI_VERBATIM) ) {
		if ( line )
			getStream() << line;
	}
}

RtVoid TRIBOut::doReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( m_currentContext->m_currentOptions->m_writeArchive ) {
		handleReadArchiveV(filename, callback, n, tokens, params);
	} else {
		getStream() << "ReadArchive \"" << (filename ? filename : "") << "\"" << endl;
	}
}


/*
RtVoid TRIBOut::doErrorHandler(RtErrorHandler handler) {
	// --> use archiveRecord verbatim to write to ribfile

	getStream() << "ErrorHandler \"";

	if ( handler == errorAbort ) {
		getStream() << RI_ABORT;
	} else if ( handler == errorPrint ) {
		getStream() << RI_PRINT;
	} else {
		getStream() << RI_IGNORE;
	}

	getStream() << "\"" << endl;
}
*/
