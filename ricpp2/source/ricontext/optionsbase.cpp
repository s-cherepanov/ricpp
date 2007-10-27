#include "ricpp/ricontext/optionsbase.h"

using namespace RiCPP;


COptionsBase &COptionsBase::operator=(const COptionsBase &ga)
{
	if ( this == &ga )
		return *this;
	
	clearMembers();

	
	m_curColorDesc = ga.m_curColorDesc;
	
	for (
		const_iterator i = ga.begin();
		i != ga.end();
		++i )
	{
		const CNamedParameterList *c = i->second;
		m_paramList.push_back(*c);
		m_paramMap[i->first] = &m_paramList.back();
	}

	return *this;
}


void COptionsBase::set(
	CDeclarationDictionary &dict,
	RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	CNamedParameterList *pl = getWriteable(name);
	if ( !pl ) {
		m_paramList.push_back(CNamedParameterList(name));
		pl = &m_paramList.back();
		m_paramMap[name] = pl;
	}

	CValueCounts counts;
	pl->add(counts, dict, m_curColorDesc, n, tokens, params);
}

void COptionsBase::set(
	RtString name, const CParameterList &params)
{
	CNamedParameterList *pl = getWriteable(name);
	if ( !pl ) {
		m_paramList.push_back(CNamedParameterList(name));
		pl = &m_paramList.back();
		m_paramMap[name] = pl;
	}

	pl->add(params);
}

const CNamedParameterList *COptionsBase::get(const char *name) const
{
	Map_type::const_iterator i = m_paramMap.find(name);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}


const CParameter *COptionsBase::get(const char *name, const char *token) const
{
	const CNamedParameterList *pl = get(name);
	if ( !pl )
		return 0;

	return pl->get(token);
}

CNamedParameterList *COptionsBase::getWriteable(const char *name)
{
	Map_type::iterator i = m_paramMap.find(name);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}

bool COptionsBase::erase(const char *name)
{
	if ( !name )
		return false;

	const CNamedParameterList *paramList = get(name);
	if ( !paramList )
		return false;

	std::list<CNamedParameterList>::iterator i;
	for ( i = m_paramList.begin(); i != m_paramList.end(); ++i ) {
		if ( paramList == &(*i) ) {
			m_paramMap.erase(name);
			m_paramList.erase(i);
			return true;
		}
	}

	return false;
}


bool COptionsBase::erase(CNamedParameterList *paramList)
{
	if ( !paramList )
		return false;

	return erase(paramList->name());
}

