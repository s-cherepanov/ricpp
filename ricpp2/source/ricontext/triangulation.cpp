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

/** @file triangulation.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements triangulation of the surface primitives.
 */

#include "ricpp/ricontext/triangulation.h"

#ifdef _DEBUG
#include <iostream>
// #define _TRACE
#endif

using namespace RiCPP;


// =============================================================================


void CTesselator::releaseSurface(CSurface *surf)
{
	if ( surf )
		delete surf;
}


CSurface *CPointsGeneralPolygonsTriangulator::triangulate(CRiPointsGeneralPolygons &obj, const IPolygonTriangulationStrategy &strategy)
{
#ifdef _TRACE
	std::cout << "-> CPointsGeneralPolygonsTriangulator::triangulate()" << std::endl;
#endif
	
	// RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params
	if ( obj.nPolys() == 0 ) {
#ifdef _TRACE
		std::cout << "<- triangulate(), npolys == 0" << std::endl;
#endif
		return 0;
	}
	
	const std::vector<CTriangulatedPolygon> &tp = obj.triangulate(strategy);
	if ( tp.size() == 0 ) {
#ifdef _TRACE
		std::cout << "<- triangulate(), triangles == 0" << std::endl;
#endif
		return 0;
	}
	
	CSurface *surf = new CSurface;
	if ( !surf ) {
#ifdef _TRACE
		std::cout << "<- triangulate(), no surface" << std::endl;
#endif
		return 0;
	}

	IndexType offs = 0;
	RtInt face = 0;
	RtInt nloops = 0;
	RtInt nverts = 0;
	RtInt nvertsOffs = 0;
	RtInt vertsOffs = 0;
	for (
		 std::vector<CTriangulatedPolygon>::const_iterator iter = tp.begin();
		 iter != tp.end();
		 iter++, face++, vertsOffs+=nverts )
	{
#ifdef _TRACE
		std::cout << "-- triangulate(), face " << face << std::endl;
#endif
		
		nloops = obj.nLoops()[face];
		nverts = 0;
		for ( RtInt loopi = 0; loopi < nloops; ++loopi ) {
			nverts += obj.nVerts()[nvertsOffs++];
		}
		
		if ( nverts <= 0 || (*iter).triangles().size() <= 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLES);
		
		std::vector<IndexType> &idx = f.indices();
		idx = (*iter).triangles();

#ifdef _TRACE
		std::cout << "-- triangulate(), subtract offsets" << std::endl;
#endif

		for ( std::vector<IndexType>::iterator idxIter = idx.begin(); idxIter != idx.end(); idxIter++ ) {
			(*idxIter) -= offs;
		}
		
#ifdef _TRACE
		std::cout << "-- triangulate(), iterate parameters" << std::endl;
#endif

		for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
			const CParameter &p = (*piter);
#ifdef _TRACE
			std::cout << "-- triangulate(), parameter " << p.parameterName() << std::endl;
#endif
			
			
			const CDeclaration *decl = p.declarationPtr();
			if ( !decl )
				continue;
			switch ( decl->storageClass() ) {

				case CLASS_CONSTANT: {
#ifdef _TRACE
					std::cout << "-- triangulate(), CLASS_CONSTANT" << std::endl;
#endif
					switch ( decl->basicType() ) {
						case BASICTYPE_INTEGER: {
							TemplPrimVar<RtInt> &var = f.reserveInts(*decl);
							var.value() = p.ints();
							break;
						}
						case BASICTYPE_FLOAT: {
							TemplPrimVar<RtFloat> &var = f.reserveFloats(*decl);
							var.value() = p.floats();
							break;
						}
						case BASICTYPE_STRING: {
							TemplPrimVar<std::string> &var = f.reserveStrings(*decl);
							var.value() = p.strings();
							break;
						}
						default:
							break;
					}
					break;
				}

				case CLASS_UNIFORM: {
#ifdef _TRACE
					std::cout << "-- triangulate(), CLASS_UNIFORM" << std::endl;
#endif
					switch ( decl->basicType() ) {
						case BASICTYPE_INTEGER: {
							TemplPrimVar<RtInt> &var = f.reserveInts(*decl);
							var.value().resize(p.declaration().elemSize());
							std::vector<RtInt>::iterator iter = var.value().begin();
							p.extract(face, iter);
							break;
						}
						case BASICTYPE_FLOAT: {
							TemplPrimVar<RtFloat> &var = f.reserveFloats(*decl);
							var.value().resize(p.declaration().elemSize());
							std::vector<RtFloat>::iterator iter = var.value().begin();
							p.extract(face, iter);
							break;
						}
						case BASICTYPE_STRING: {
							TemplPrimVar<std::string> &var = f.reserveStrings(*decl);
							var.value().resize(p.declaration().elemSize());
							std::vector<std::string>::iterator iter = var.value().begin();
							p.extract(face, iter);
							break;
						}
						default:
							break;
					}
					break;
				}
					
				case CLASS_VARYING:
					// NO break;
				case CLASS_VERTEX: {
#ifdef _TRACE
					std::cout << "-- triangulate(), CLASS_VARYING, CLASS_VERTEX, nverts = " << nverts << std::endl;
#endif
					switch ( decl->basicType() ) {
						case BASICTYPE_INTEGER: {
							TemplPrimVar<RtInt> &var = f.reserveInts(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<RtInt>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(obj.verts()[vertsOffs+i], iter);
							}
							break;
						}
						case BASICTYPE_FLOAT: {
							TemplPrimVar<RtFloat> &var = f.reserveFloats(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<RtFloat>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(obj.verts()[vertsOffs+i], iter);
							}
							break;
						}
						case BASICTYPE_STRING: {
							TemplPrimVar<std::string> &var = f.reserveStrings(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<std::string>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(obj.verts()[vertsOffs+i], iter);
							}
							break;
						}
						default:
							break;
					}
					break;
				}
					
				case CLASS_FACEVARYING:
					// NO break;
				case CLASS_FACEVERTEX: {
#ifdef _TRACE
					std::cout << "-- triangulate(), CLASS_FACEVARYING, CLASS_FACEVERTEX, nverts = " << nverts << std::endl;
#endif
					switch ( decl->basicType() ) {
						case BASICTYPE_INTEGER: {
							TemplPrimVar<RtInt> &var = f.reserveInts(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<RtInt>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(vertsOffs+i, iter);
							}
							break;
						}
						case BASICTYPE_FLOAT: {
							TemplPrimVar<RtFloat> &var = f.reserveFloats(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<RtFloat>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(vertsOffs+i, iter);
							}
							break;
						}
						case BASICTYPE_STRING: {
							TemplPrimVar<std::string> &var = f.reserveStrings(*decl);
							var.value().resize(p.declaration().elemSize()*nverts);
							std::vector<std::string>::iterator iter = var.value().begin();
							for ( IndexType i = 0; i < nverts; ++i ) {
								p.extract(vertsOffs+i, iter);
							}
							break;
						}
						default:
							break;
					}
					break;
				}
					
				default:
					break;
			}
		}
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}
		
		f.sizes().resize(1);
		f.sizes()[0] = idx.size();
		
		// Next polygon (face)
		offs += idx.size();
	}
	

#ifdef _TRACE
	std::cout << "<- triangulate(), regular" << std::endl;
#endif
	return surf;
}
