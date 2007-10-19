#ifndef _RICPP_RENDERSTATE_HANDLESTACK_H
#define _RICPP_RENDERSTATE_HANDLESTACK_H

#ifndef _RICPP_DECLARATION_TOKENMAP_H
#include "ricpp/declaration/tokenmap.h"
#endif // _RICPP_DECLARATION_TOKENMAP_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif //_RICPP_TOOLS_INLINETOOLS_H

#include <vector>
#include <string>

#include <cstdlib>

namespace RiCPP {

/** @brief Template to handle RI objects and archives
 */
template<typename ValueType> class TemplHandleStack {
public:
	typedef std::map<RtToken, ValueType *> TypeHandleMap;
	typedef std::vector<RtToken> TypeHandleStack;

private:
	TypeHandleStack m_stack;      ///< Stack to push handles and markers.
	TypeHandleMap m_map;          ///< Container for the key, value pairs to find an object for a handle
	unsigned long m_maxHandleIdx; ///< Counter to generate handles
	CTokenMap m_tokens;           ///< RtToken as handles
	std::string m_prefix;         ///< Prefix for the tokens

public:
	TemplHandleStack(RtString prefix="")
	{
		m_maxHandleIdx = 0;
		m_stack.push_back(RI_NULL);
		m_prefix = noNullStr(prefix);
	}
	
	~TemplHandleStack()
	{
		typename TypeHandleMap::iterator i;
		for ( i = m_map.begin(); i != m_map.end(); i++ ) {
			if ( (*i).second != NULL ) {
				ValueType *ptr = (*i).second;
				delete ptr;
			}
		}
		m_map.clear();
		m_stack.clear();
	}
	
	RtToken newHandle(RtString name = 0)
	{
		std::string str(m_prefix);
		if ( emptyStr(name) ) {
			char strnum[64];

			strnum[0] = (char)0;
#ifdef WIN32
			sprintf_s(strnum, sizeof(strnum), "%ld", ++m_maxHandleIdx);
#else
			snprintf(strnum, sizeof(strnum)-1, "%ld", ++m_maxHandleIdx);
#endif
			strnum[sizeof(strnum)-1] = (char)0;

			str += strnum;
			RtToken tok = m_tokens.findCreate(str.c_str());
			return tok;
		}

		RtToken tok = m_tokens.findCreate(name);
		return tok;
	}

	RtToken identify(RtString name) const
	{
		if ( emptyStr(name) )
			return RI_NULL;

		return m_tokens.find(name);
	}

	ValueType *find(RtToken tok)
	{
		typename TypeHandleMap::iterator i;
		i = m_map.find(tok);
		if ( i == m_map.end() )
			return 0;
		return (*i).second;
	}

	ValueType *back()
	{
		return find(m_stack.back());
	}

	const ValueType *find(RtToken tok) const
	{
		typename TypeHandleMap::const_iterator i;
		i = m_map.find(tok);
		if ( i == m_map.end() )
			return 0;
		return (*i).second;
	}

	const ValueType *back() const
	{
		return find(m_stack.back());
	}

	bool deleteObject(RtToken tok)
	{
		ValueType *v = find(tok);
		if ( v != 0 ) {
			m_map.erase(tok);
			delete v;
			m_stack.erase(tok);
			return true;
		}
		return false;
	}
	
	bool deleteObject(ValueType *obj)
	{
		if ( !obj )
			return false;
		return deleteObj(obj->handle());
	}

	RtToken insertObject(ValueType *o)
	{
		if ( !o )
			return RI_NULL;

		RtToken tok = newHandle(o->name());
		m_stack.push_back(tok);
		o->handle(tok);
		m_map[tok] = o;
		return tok; 
	}

	void mark()
	{
		m_stack.push_back(RI_NULL);
	}
	
	void clearToMark()
	{
		while ( !m_stack.empty() ) {
			RtToken tok = m_stack.back();
			m_stack.pop_back();
			if ( tok == RI_NULL )
				return;
			deleteObject(tok);
		}
	}
};

}

#endif // _RICPP_RENDERSTATE_HANDLESTACK_H
