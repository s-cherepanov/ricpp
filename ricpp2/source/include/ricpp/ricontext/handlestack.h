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

	inline typename TypeHandleStack::const_reverse_iterator rfind(RtToken tok, bool toMark) const
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

	inline typename TypeHandleStack::reverse_iterator rfind(RtToken tok, bool toMark)
	{
		/** @todo use the typename TypeHandleStack::const_reverse_iterator rfind() with const away cast
		 */
		typename TypeHandleStack::reverse_iterator i;
		
		for( i = m_stack.rbegin(); i != m_stack.rend(); i++ ) {
			if ( (*i) && (*i)->handle() == tok )
				break;
			if ( !(*i) && toMark )
				return m_stack.rend();
		}
		
		return i;
	}
public:
	inline TemplHandleStack(RtString prefix="") :
		m_maxHandleIdx(0),
		m_prefix(noNullStr(prefix))
	{
		m_stack.push_back(0);
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
#ifdef _WIN32
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

	inline RtToken newHandle(RtString name, unsigned long &num)
	{
		/** @todo Implement a function that returns real unique (worldwide) string handles.
		 */
		RtToken t = newHandle(name);
		num = m_maxHandleIdx;
		return t;
	}

	inline RtToken identify(RtString name) const
	{
		if ( emptyStr(name) )
			return RI_NULL;

		return m_tokens.find(name);
	}

	inline ValueType *find(RtToken tok, bool toMark=false)
	{
		typename TypeHandleStack::reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() )
			return (*i);
		return 0;
	}

	inline ValueType *back()
	{
		return m_stack.back();
	}

	inline const ValueType *find(RtToken tok, bool toMark=false) const
	{
		typename TypeHandleStack::const_reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() )
			return (*i);
		return 0;
	}

	inline const ValueType *back() const
	{
		return m_stack.back();
	}

	inline bool deleteObject(RtToken tok, bool toMark=false)
	{
		typename TypeHandleStack::const_reverse_iterator i = rfind(tok, toMark);
		if ( i != m_stack.rend() ) {
			delete (*i);
			m_stack.erase(i);
			return true;
		}
		return false;
	}
	
	inline bool deleteObject(ValueType *obj, bool toMark=false)
	{
		if ( !obj )
			return false;
		return deleteObj(obj->handle(), toMark);
	}

	inline RtToken insertObject(ValueType *o)
	{
		if ( !o )
			return RI_NULL;

		RtToken tok = newHandle(o->handle());
		o->handle(tok);
		m_stack.push_back(o);
		return tok; 
	}

	inline RtToken insertObject(RtToken handle, ValueType *o)
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

	inline void mark()
	{
		m_stack.push_back(0);
	}
	
	inline void clearToMark()
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

	inline unsigned long maxIdx() const
	{
		return m_maxHandleIdx;
	}
	
	inline bool empty() const {
		typename TypeHandleStack::const_iterator i = m_stack.begin();
		for ( ; i != m_stack.end(); i++ ) {
			if ( (*i) != 0 ) {
				// no mark
				return false;
			}
		}
		return true;
	}
};

}

#endif // _RICPP_RICONTEXT_HANDLESTACK_H
