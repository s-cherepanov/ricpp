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

/** @file resource.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the resource handling and specifically the attribute resource.
 */

// #include "ricpp/ricontext/resource.h"

#ifndef _RICPP_RICONTEXT_RICONTEXT_H
#include "ricpp/ricontext/ricontext.h"
#endif // _RICPP_RICONTEXT_RICONTEXT_H

using namespace RiCPP;


RtToken CAttributesResource::s_operation = RI_NULL;
RtToken CAttributesResource::s_save = RI_NULL;
RtToken CAttributesResource::s_restore = RI_NULL;
RtToken CAttributesResource::s_concat = RI_NULL;
RtToken CAttributesResource::s_subset = RI_NULL;
RtToken CAttributesResource::s_shading = RI_NULL;
RtToken CAttributesResource::s_transform = RI_NULL;
RtToken CAttributesResource::s_all = RI_NULL;
RtToken CAttributesResource::s_geometrymodification = RI_NULL;
RtToken CAttributesResource::s_geometrydefinition = RI_NULL;

RtToken CAttributesResource::myType()
{
	return "attributes";
}

void CAttributesResource::registerOperations(CTokenMap &m)
{
	s_operation = m.findCreate("operation");

	s_save = m.findCreate("save");
	s_restore = m.findCreate("restore");
	s_concat = m.findCreate("concat");
	
	s_subset = m.findCreate("subset");

	s_shading = m.findCreate("shading");
	s_transform = m.findCreate("transform");
	s_all = m.findCreate("all");
	s_geometrymodification = m.findCreate("geometrymodification");
	s_geometrydefinition = m.findCreate("geometrydefinition");
}

CAttributesResource::CAttributesResource(RtToken anId, unsigned long aHandleNo, bool isFromHandleId)
	: CResource(anId, aHandleNo, isFromHandleId)
{
	m_attributes = 0;
}

CAttributesResource::~CAttributesResource()
{
	if ( m_attributes )
		delete m_attributes;
}


RtToken CAttributesResource::getOperation(const std::string &anOperation)
{
	if ( anOperation.empty() )
		return RI_NULL;

	if ( anOperation == std::string(s_save) )
		return s_save;
	if ( anOperation == std::string(s_restore) )
		return s_restore;
	if ( anOperation == std::string(s_concat) )
		return s_concat;

	
	return RI_NULL;
}

RtToken CAttributesResource::getSubset(const std::string &aSubset)
{
	if ( aSubset.empty() )
		return RI_NULL;

	if ( aSubset == std::string(s_shading) )
		return s_shading;
	if ( aSubset == std::string(s_transform) )
		return s_transform;
	if ( aSubset == std::string(s_all) )
		return s_all;
	if ( aSubset == std::string(s_geometrymodification) )
		return s_geometrymodification;
	if ( aSubset == std::string(s_geometrydefinition) )
		return s_geometrydefinition;

	return RI_NULL;
}

void CAttributesResource::save(IRiContext &ri)
{
	if ( m_attributes )
		delete m_attributes;

	m_attributes = dynamic_cast<CAttributes *>(ri.renderState()->attributes().duplicate());
}

void CAttributesResource::restore(IRiContext &ri, RtToken subset)
{
	if ( !m_attributes )
		return;

	if (subset == s_all ) {
		ri.renderState()->attributes() = *m_attributes;
	} else if (subset == s_shading ) {
	} else if (subset == s_transform ) {
	} else if (subset == s_geometrymodification ) {
	} else if (subset == s_geometrydefinition ) {
	} else {
	}
}


void CAttributesResource::concat(IRiContext &ri, RtToken subset)
{
	if ( !m_attributes )
		return;
	
	if (subset == s_all ) {
		ri.renderState()->attributes() = *m_attributes;
	} else if (subset == s_shading ) {
	} else if (subset == s_transform ) {
		// Concat transformation
	} else if (subset == s_geometrymodification ) {
	} else if (subset == s_geometrydefinition ) {
	} else {
	}
}

void CAttributesResource::operate(IRiContext &ri, RtToken operation, RtToken subset)
{
	if (operation == s_save ) {
		save(ri);
	} else if (operation == s_restore ) {
		restore(ri, subset);
	} else  {
	}
}


void CAttributesResource::operate(IRiContext &ri, const CParameterList &parameters)
{
	const CParameter *operp = parameters.get(s_operation);
	if ( !operp )
		return;

	if ( operp->type() == TYPE_STRING && !operp->strings().empty() ) {
		RtToken operation = getOperation(operp->strings()[0]);

		RtToken subset = s_all;
		const CParameter *subp = parameters.get(s_subset);
		if ( subp && subp->type() == TYPE_STRING && !subp->strings().empty() ) {
			subset = getSubset(subp->strings()[0]);
		}

		operate(ri, operation, subset);
	}
}

RtToken CAttributesResource::type() const
{
	return CAttributesResource::myType();
}

bool CAttributesResource::overwrites(const CParameterList &parameters)
{
	const CParameter *p = parameters.get(s_operation);
	if ( !p )
		return false;

	if ( p->type() == TYPE_STRING && !p->strings().empty() ) {
		std::string save(s_save);
		return p->strings()[0] == save;
	}
	return false;
}


void CAttributesResourceFactory::registerOperations(CTokenMap &m)
{
	CAttributesResource::registerOperations(m);
}

CResource *CAttributesResourceFactory::getResource(RtToken anId, unsigned long aHandleNo, bool isFromHandleId)
{
	return new CAttributesResource(anId, aHandleNo, isFromHandleId);
}

RtToken CAttributesResourceFactory::type() const
{
	return CAttributesResource::myType();
}

bool CAttributesResourceFactory::overwrites(const CParameterList &parameters) const
{
	return CAttributesResource::overwrites(parameters);
}
