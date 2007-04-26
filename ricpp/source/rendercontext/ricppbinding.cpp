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

/*! \file ricppbinding.cpp
 *  \brief Implementation of the base class for RenderMan C++ Binding with parameter handling, RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "ricppbinding.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

RtVoid TRiCPPBinding::handleErrorV(RtInt code, RtInt severity, const char *message, va_list argList) {
	char str[ERROR_STR_SIZE]; // hope that´s enough - helpers.h

	lastError = code;

	str[0]=(char)0;
	if ( !message )
		message = "";
	if ( lineNo() >= 0 && archiveName() && archiveName()[0] ) {
		_snprintf(str, ERROR_STR_SIZE-1, "Line %ld, File \"%s\", ", lineNo(), archiveName()?archiveName():"(NULL)");
	}
	size_t len = strlen(str);
	_vsnprintf(str+strlen(str), ERROR_STR_SIZE-1-len, message, argList);
	str[ERROR_STR_SIZE-1]=(char)0;
	if ( m_errHandler )
		(*m_errHandler)(code, severity, str);
}


/////////////////////////////////////////////////////////////////////////////////////////
// getTokens() - get token and value arrays from parameter lists
//
//               Called from interface functions with variable parameters in that way:
//                 va_list marker;
//                 va_start( marker, token);
//                 int n = getTokens(token, marker);
//               to call V-function (e.g. RiCircleV())
//   	           xxxxV(additionalParameters, n, &m_tokens[0], &m_params[0]);
//
// Parameters:
// token:       First token in parameterlist, 
// marker:      Parameter list as va_list, RI_NULL marks the end
//
// Returns:
// Number of found parameters, fills m_tokens and m_params
//
RtInt TRiCPPBinding::getTokens(RtToken token, va_list marker)
{
	RtInt n;
	RtPointer param;

	m_tokens.clear();
	m_params.clear();

	for ( n = 0; token != RI_NULL; ++n, token = va_arg(marker, RtToken) ) {
		param = va_arg(marker, RtPointer);
		m_tokens.push_back(token);
		m_params.push_back(param);
	}
	va_end(marker);

	if ( n == 0 ) {
		m_tokens.resize(1);
		m_params.resize(1);
		m_tokens[0] = NULL;
		m_params[0] = NULL;
	}

	return n;
}


/////////////////////////////////////////////////////////////////////////////////////////
// getValue() - searches a value of a token of a V-function
//
// Parameters:
// token:  token to find
// value:  returns the value
// n:      number of token value pairs
// tokens: tokens of the interface call
// value:  value of the interface call
//
// returns: index of the found position in the array and the value in value
//          -1 if not found
//
RtInt TRiCPPBinding::getValue(
	RtToken token, RtPointer &value,
	RtInt n, RtToken tokens[], RtPointer params[]
) {
	if ( token ) {
		while ( n-- ) {
			if ( !strcmp(token, tokens[n]) ) {
				value = params[n];
				return n;
			}
		}
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////
// getSizes() - 
//
// Parameters:
//
RtInt TRiCPPBinding::getSizes(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt n, RtToken tokens[], RtPointer params[]) {
	TRiParameterSize paramSize;
	RtInt i;
	m_sizes.resize(0);
	m_sizes.reserve(n);
	// RtInt colorComponents = curColorComponents();
	for ( i=0; i<n; ++i ) {
		if ( !queryParam(
				tokens[i], vertices, corners, facets, faceVertices, faceCorners, 
				paramSize.m_class, paramSize.m_type, paramSize.m_cardinality,
				paramSize.m_numberOfElements, paramSize.m_memoryNeeded)
				)
		{
			// Error
			return -1;
		}
		if ( paramSize.m_type == TYPE_STRING || paramSize.m_type == TYPE_TOKEN ) {
			const char **ptr = (const char **)(params[i]);
			if ( !ptr ) {
				// Error
				return -1;
			}
			paramSize.m_memoryNeeded = 0;
			RtInt j;
			for ( j=0; j<paramSize.m_numberOfElements; ++j ) {
				paramSize.m_memoryNeeded += ptr[j] ? strlen(ptr[j])+1 : 1;
			}
		}
		m_sizes.push_back(paramSize);
	}

	return n;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// "..." functions

RtVoid TRiCPPBinding::motionBegin(RtInt N, RtFloat sample, ...) {
	va_list marker;
	va_start(marker, sample);
	std::vector<RtFloat> samples;

	RtInt n;
	samples.reserve(N);

	for ( n = 0; n < N; ++n ) {
		samples.push_back(sample);
		sample = (RtFloat)va_arg(marker, double); // RtFloat to double ????
	}
	va_end(marker);

	motionBeginV(N, samples.empty() ? NULL : &samples[0]);
}

RtVoid TRiCPPBinding::projection(const char *name, RtToken token, ...) {
	va_list marker;
	va_start( marker, token);
    int n = getTokens(token, marker);
	projectionV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::display(RtToken name, RtToken type, RtToken mode,
							   RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	displayV(name, type, mode, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::imager(RtToken name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	imagerV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::hider(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	hiderV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::option(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	optionV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle TRiCPPBinding::lightSource(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	return lightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtLightHandle TRiCPPBinding::areaLightSource(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	return areaLightSourceV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::surface(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	surfaceV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::atmosphere(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	atmosphereV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::interior(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	interiorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::exterior(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	exteriorV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::deformation(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	deformationV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::displacement(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	displacementV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::attribute(const char *name, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	attributeV(name, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::polygon(RtInt nvertices, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	polygonV(nvertices, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::generalPolygon(RtInt nloops, RtInt *nverts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	generalPolygonV(nloops, nverts, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::pointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	pointsPolygonsV(npolys, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::pointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::patch(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	patchV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	patchMeshV(type, nu, uwrap, nv, vwrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::nuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	curvesV(type, ncurves, nverts, wrap, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...)
{
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	coneV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    RtInt n = getTokens(token, marker);
	cylinderV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	diskV(height, radius, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	hyperboloidV(point1, point2, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::paraboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	paraboloidV(rmax, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start( marker, token );
    int n = getTokens(token, marker);
	sphereV(radius, zmin, zmax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token, ...) {
	va_list marker;
	va_start( marker, token );
    int n = getTokens(token, marker);
	torusV(majorrad, minorrad, phimin, phimax, thetamax, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::geometry(RtToken type, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	geometryV(type, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::points(RtInt npts, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	pointsV(npts, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::makeTexture(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::makeBump(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::makeLatLongEnvironment(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	makeLatLongEnvironmentV(pic, tex, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::makeCubeFaceEnvironment(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::makeShadow(const char *pic, const char *tex, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	makeShadowV(pic, tex, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::archiveRecord(RtToken type, const char *format, ...) {
	char archiveBuf[ARCHIVE_BUF_SIZE];
	va_list argList;
	va_start(argList, format);
	archiveBuf[0] = (char)0;
	int rval = _vsnprintf(archiveBuf, ARCHIVE_BUF_SIZE-1, format ? format : "", argList);
	rval = rval;
	archiveBuf[ARCHIVE_BUF_SIZE-1] = (char)0;
	va_end(argList);
	archiveRecordV(type, archiveBuf);
}

RtVoid TRiCPPBinding::readArchive(RtString filename, RtArchiveCallback callback, RtToken token, ...) {
	va_list marker;
	va_start(marker, token);
    int n = getTokens(token, marker);
	readArchiveV(filename, callback, n, &m_tokens[0], &m_params[0]);
}

RtVoid TRiCPPBinding::handleError(RtInt code, RtInt severity, const char *message, ...)
{
	va_list argList;
	va_start(argList, message);
	handleErrorV(code, severity, message, argList);
	va_end(argList);
}
