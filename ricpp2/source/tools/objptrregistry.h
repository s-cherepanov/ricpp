#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#define _RICPP_TOOLS_OBJPTRREGISTRY_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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

#include <map>

namespace RiCPP {

template<typename KeyType, typename ValueType> class CObjPtrRegistry {
	std::map<KeyType, ValueType>m_map;
	bool m_destructMembers;
public:
	CObjPtrRegistry(bool destructMembers);
	~CObjPtrRegistry();
	bool registerObj(const KeyType &key, ValueType value);
	ValueType findObj(const KeyType &key);
	bool unRegisterObj(const KeyType &key);
}; // CObjPtrRegistry

template<typename KeyType, typename ValueType>CObjPtrRegistry<KeyType, ValueType>::
CObjPtrRegistry(bool destructMembers) :
m_destructMembers(destructMembers)
{
}

template<typename KeyType, typename ValueType>CObjPtrRegistry<KeyType, ValueType>::
~CObjPtrRegistry()
{
	if ( m_destructMembers ) {
		typename std::map<KeyType, ValueType>::iterator i;
		for ( i = m_map.begin(); i != m_map.end(); i++ ) {
			if ( (*i).second != NULL ) {
				ValueType ptr = (*i).second;
				delete ptr;
			}
		}
		m_map.clear();
	}
}

template<typename KeyType, typename ValueType>
bool CObjPtrRegistry<KeyType, ValueType>::
registerObj(const KeyType &key, ValueType value)
{
	if ( m_map.find(key) == m_map.end() ) {
		m_map[key] = value;
	}
	return false;
}

template<typename KeyType, typename ValueType>
ValueType CObjPtrRegistry<KeyType, ValueType>::
findObj(const KeyType &key)
{
	if ( m_map.find(key) != m_map.end() ) {
		return m_map[key];
	}
	return NULL;
}

template<typename KeyType, typename ValueType>
bool CObjPtrRegistry<KeyType, ValueType>::
unRegisterObj(const KeyType &key)
{
	if ( m_map.find(key) != m_map.end() ) {
		m_map.erase(key);
		return true;
	}
	return false;
}
};

#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H
