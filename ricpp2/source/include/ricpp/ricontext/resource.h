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
 */

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_RICPP_RICPPTOKENS_H
#include "ricpp/ricpp/ricpptokens.h"
#endif // _RICPP_RICPP_RICPPTOKENS_H

#include <string>

namespace RiCPP {

class IRiContext;
class CParameterList;
class CTokenMap;
class CAttributes;

class CResource
{
	RtToken m_handle;
	std::string m_name;
public:
	inline CResource(RtString aName=0) : m_name(noNullStr(aName)) {
		m_handle = RI_NULL;
	}
	inline virtual ~CResource() {}
	virtual void operate(IRiContext &ri, const CParameterList &parameters) = 0;
	virtual RtToken type() const = 0;

	/** @brief Gets the name of the resource.
	 *  @return The name of the resource.
	 */
	inline RtString name() const { return m_name.c_str(); }

	/** @brief Sets the name of the resource.
	 *
	 *  @param aName The new name for the resource.
	 */
	inline void name(RtString aName)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Gets the associated handle of the resource.
	 *
	 * @return The associated handle of the resource.
	 */
	inline RtToken handle() const { return m_handle; }

	/** @brief Sets the associated handle of the resource.
	 *
	 * @param h The associated handle of the resource.
	 */
	inline void handle(RtToken h) { m_handle = h; }
};

class CAttributesResource : public CResource
{
	static RtToken s_operation;
	static RtToken s_save;
	static RtToken s_restore;
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

public:
	static void registerOperations(CTokenMap &m);
	static RtToken myType();
	static bool overwrites(const CParameterList &parameters);
	
	CAttributesResource(RtString aName=0);
	virtual ~CAttributesResource();
	virtual void operate(IRiContext &ri, const CParameterList &parameters);
	virtual RtToken type() const;
};

class IResourceFactory
{
public:
	virtual void registerOperations(CTokenMap &m) = 0;
	virtual CResource *getResource(RtString aName) = 0;
	virtual RtToken type() const = 0;
	virtual bool overwrites(const CParameterList &parameters) const = 0;
};

class CAttributesResourceFactory : public IResourceFactory
{
public:
	virtual void registerOperations(CTokenMap &m);
	virtual CResource *getResource(RtString aName);
	virtual RtToken type() const;
	virtual bool overwrites(const CParameterList &parameters) const;
};

}
#endif // _RICPP_RICONTEXT_RESOURCE_H
