#include "ricpp/renderstate/resource.h"

#ifndef _RICPP_DECLARATION_TOKENMAP_H
#include "ricpp/declaration/tokenmap.h"
#endif // _RICPP_DECLARATION_TOKENMAP_H

#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#include "ricpp/baserenderer/ricontext.h"
#endif // _RICPP_BASERENDERER_RICONTEXT_H

#ifndef _RICPP_RENDERSTATE_ATTRIBUTES_H
#include "ricpp/renderstate/attributes.h"
#endif //  _RICPP_RENDERSTATE_ATTRIBUTES_H

using namespace RiCPP;


RtToken CAttributesResource::s_operation = RI_NULL;
RtToken CAttributesResource::s_save = RI_NULL;
RtToken CAttributesResource::s_restore = RI_NULL;
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

	s_subset = m.findCreate("subset");

	s_shading = m.findCreate("shading");
	s_transform = m.findCreate("transform");
	s_all = m.findCreate("all");
	s_geometrymodification = m.findCreate("geometrymodification");
	s_geometrydefinition = m.findCreate("geometrydefinition");
}

CAttributesResource::CAttributesResource(RtString aName) : CResource(aName)
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

	m_attributes = ri.renderState()->attributes().duplicate();
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

CResource *CAttributesResourceFactory::getResource(RtString aName)
{
	return new CAttributesResource(aName);
}

RtToken CAttributesResourceFactory::type() const
{
	return CAttributesResource::myType();
}

bool CAttributesResourceFactory::overwrites(const CParameterList &parameters) const
{
	return CAttributesResource::overwrites(parameters);
}
