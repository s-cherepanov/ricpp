#ifndef _RICPP_RICONTEXT_RESOURCE_H
#define _RICPP_RICONTEXT_RESOURCE_H

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

/** @file resource.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Resource handling and specifically the attribute resource.
 *  @todo Implementation
 */

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H

namespace RiCPP {

class IRiContext;
class CParameterList;
class CTokenMap;
class CAttributes;

class CResource : public CHandle
{
public:
	inline CResource(
		RtToken anId = RI_NULL, unsigned long aHandleNo = 0, bool isFromHandleId = false)
		: CHandle(anId, aHandleNo, isFromHandleId)
	{
	}
	inline virtual ~CResource() {}
	virtual void operate(IRiContext &ri, const CParameterList &parameters) = 0;
	virtual RtToken type() const = 0;
};

class CAttributesResource : public CResource
{
	static RtToken s_operation;
	static RtToken s_save;
	static RtToken s_restore;
	static RtToken s_concat;
	static RtToken s_subset;
	static RtToken s_shading;
	static RtToken s_transform;
	static RtToken s_all;
	static RtToken s_geometrymodification;
	static RtToken s_geometrydefinition;

protected:
	CAttributes *m_attributes;

	virtual RtToken getOperation(const std::string &anOperation);
	virtual RtToken getSubset(const std::string &aSubset);
	virtual void operate(IRiContext &ri, RtToken operation, RtToken subset);

	virtual void save(IRiContext &ri);
	virtual void restore(IRiContext &ri, RtToken subset);
	virtual void concat(IRiContext &ri, RtToken subset);

public:
	static void registerOperations(CTokenMap &m);
	static RtToken myType();
	static bool overwrites(const CParameterList &parameters);
	
	CAttributesResource(RtToken anId = RI_NULL, unsigned long aHandleNo = 0, bool isFromHandleId = false);
	virtual ~CAttributesResource();
	virtual void operate(IRiContext &ri, const CParameterList &parameters);
	virtual RtToken type() const;
};

class IResourceFactory
{
public:
	virtual ~IResourceFactory() {}
	virtual void registerOperations(CTokenMap &m) = 0;
	virtual CResource *getResource(RtToken anId, unsigned long aHandleNo, bool isFromHandleId) = 0;
	virtual RtToken type() const = 0;
	virtual bool overwrites(const CParameterList &parameters) const = 0;
};

class CAttributesResourceFactory : public IResourceFactory
{
public:
	virtual void registerOperations(CTokenMap &m);
	virtual CResource *getResource(RtToken anId, unsigned long aHandleNo, bool isFromHandleId);
	virtual RtToken type() const;
	virtual bool overwrites(const CParameterList &parameters) const;
};

}
#endif // _RICPP_RICONTEXT_RESOURCE_H
