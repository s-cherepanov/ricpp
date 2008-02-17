#ifndef _RICPP_RICONTEXT_HANDLESTACK_H
#define _RICPP_RICONTEXT_HANDLESTACK_H

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

/** @file handlestack.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief General stack for handles (objects, rib macros)
 */

#ifndef _RICPP_DECLARATION_TOKENMAP_H
#include "ricpp/declaration/tokenmap.h"
#endif // _RICPP_DECLARATION_TOKENMAP_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif //_RICPP_TOOLS_INLINETOOLS_H

#include <vector>

namespace RiCPP {

/** @brief Class that is used for indirect light handles.
 */
class CHandle
{
private:
	RtToken m_identifier;     ///< @brief Token as identifier.
	unsigned long m_handleNo; ///< @brief Sequence number.
	bool m_fromHandleId;      ///< @brief Created from parameter list RI_HANDLEID
public:
	inline CHandle(RtToken anId = RI_NULL, unsigned long aHandleNo = 0, bool isFromHandleId = false)
	{
		m_identifier = anId;
		m_handleNo = aHandleNo;
		m_fromHandleId = isFromHandleId;
	}

	inline CHandle(const CHandle &h)
	{
		*this = h;
	}

	inline virtual ~CHandle()
	{
	}

	inline virtual CHandle *duplicate() const
	{
		return new CHandle(*this);
	}

	// inline const char *name() const { return m_identifier; }
	inline RtToken handle() const { return m_identifier; }
	inline void handle(RtToken anId) { m_identifier = anId; }
	
	inline unsigned long handleNo() const { return m_handleNo; }
	inline void handleNo(unsigned long aHandleNo) { m_handleNo = aHandleNo; }

	inline bool fromHandleId() const { return m_fromHandleId; }
	inline void fromHandleId(bool isFromHandleId) { m_fromHandleId = isFromHandleId; }

	inline CHandle &operator=(const CHandle &h)
	{
		handle(h.handle());
		handleNo(h.handleNo());
		fromHandleId(h.fromHandleId());
		return *this;
	}
}; // TemplHandle


/** @brief Template to handle RI objects and archives
 */
template<typename ValueType> class TemplHandleStack {
public:
	typedef std::vector<ValueType *> TypeHandleStack;

private:
	TypeHandleStack m_stack;      ///< Stack to push handles and markers.
	unsigned long m_maxHandleIdx; ///< Counter to generate handles
	CTokenMap m_tokens;           ///< RtToken as handles
	std::string m_prefix;         ///< Prefix for the tokens

	inline typename TypeHandleStack::reverse_iterator rfind(RtToken tok, bool toMark)
	{
		typename TypeHandleStack::reverse_iterator i;

		for( i = m_stack.rbegin(); i != m_stack.rend(); i++ ) {
			if ( (*i) && (*i)->handle() == tok )
				break;
			if ( !(*i) && toMark )
				return m_stack.rend();
		}

		return i;
	}

	typename TypeHandleStack::const_reverse_iterator rfind(RtToken tok, bool toMark) const
	{
		typename TypeHandleStack::const_reverse_iterator i;

		for( i = m_stack.rbegin(); i != m_stack.rend(); i++ ) {
			if ( (*i) && (*i)->handle() == tok )
				break;
			if ( !(*i) && toMark )
				return m_stack.rend();
		}

		return i;
	}

public:
	TemplHandleStack(RtString prefix="")
	{
		m_maxHandleIdx = 0;
		m_stack.push_back(0);
		m_prefix = noNullStr(prefix);
	}
	
	~TemplHandleStack()
	{
		typename TypeHandleStack::iterator i;
		for ( i = m_stack.begin(); i != m_stack.end(); i++ ) {
			if ( (*i) != 0 ) {
				delete (*i);
			}
		}
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
		++m_maxHandleIdx;
		return tok;
	}

	RtToken newHandle(RtString name, unsigned long &num)
	{
		/** @todo Implement a function that returns real unique (worldwide) string handles.
		 */
		RtToken t = newHandle(name);
		num = m_maxHandleIdx;
		return t;
	}

	RtToken identify(RtString name) const
	{
		if ( emptyStr(name) )
			return RI_NULL;

		return m_tokens.find(name);
	}

	ValueType *find(RtToken tok, bool toMark=false)
	{
		typename TypeHandleStack::reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() )
			return (*i);
		return 0;
	}

	ValueType *back()
	{
		return m_stack.back();
	}

	const ValueType *find(RtToken tok, bool toMark=false) const
	{
		typename TypeHandleStack::const_reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() )
			return (*i);
		return 0;
	}

	const ValueType *back() const
	{
		return m_stack.back();
	}

	bool deleteObject(RtToken tok, bool toMark=false)
	{
		typename TypeHandleStack::const_reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() ) {
			delete (*i);
			m_stack.erase(i);
			return true;
		}
		return false;
	}
	
	bool deleteObject(ValueType *obj, bool toMark=false)
	{
		if ( !obj )
			return false;
		return deleteObj(obj->handle(), toMark);
	}

	RtToken insertObject(ValueType *o)
	{
		if ( !o )
			return RI_NULL;

		RtToken tok = newHandle(o->handle());
		o->handle(tok);
		m_stack.push_back(o);
		return tok; 
	}

	RtToken insertObject(RtToken handle, ValueType *o)
	{
		if ( !o )
			return RI_NULL;

		if ( !m_tokens.find(handle) ) {
			handle = newHandle(handle);
		}

		o->handle(handle);
		m_stack.push_back(o);
		return handle; 
	}

	void mark()
	{
		m_stack.push_back(0);
	}
	
	void clearToMark()
	{
		while ( !m_stack.empty() ) {
			ValueType *v = back();
			m_stack.pop_back();
			if ( v == 0 ) {
				return;
			}
			delete v;
		}
	}

	unsigned long maxIdx() const
	{
		return m_maxHandleIdx;
	}
};

}

#endif // _RICPP_RICONTEXT_HANDLESTACK_H
