#ifndef _RICPP_RENDERSTATE_RESOURCE_H
#define _RICPP_RENDERSTATE_RESOURCE_H

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
	 *  @param name The new name for the resource.
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
#endif // _RICPP_RENDERSTATE_RESOURCE_H
