#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#define _RICPP_TOOLS_OBJPTRREGISTRY_H

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

/** @file objptrregistry.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Container template for named object pointers
 */

#include <map>

namespace RiCPP {

/** @brief Template that maps names (KeyType, std::string, long, ...) to object
 *         pointers (ValueType, class *), can manage the object deletion.
 */
template<typename KeyType, typename ValueType> class TObjPtrRegistry {
	std::map<KeyType, ValueType>m_map; //< Container for the key, value pairs
	bool m_destructMembers; //< Destruct all members if container is deleted or key is unregistered
public:
	/** @brief Const iterator for the elements.
	 */
	typedef typename std::map<KeyType, ValueType>::const_iterator const_iterator;
	/** @brief Size type for the number of stored elements
	 */
	typedef typename std::map<KeyType, ValueType>::size_type size_type;

	/** @brief Initializes the object, the map is empty at the beginning.
	 * @param destructMembers Deletes members if container is destructed or the key is unregistered.
	 */
	TObjPtrRegistry(bool destructMembers);

	/** @brief Destructor, deletes also the members (values) if m_destructMembers is true.
	 */
	~TObjPtrRegistry();

	/** @brief Registers an object (pointer).
	 * @param key Key for the registered object pointer
	 * @param value The object pointer
	 * @return true, the object could be registered, false the key was already used
	 */
	bool registerObj(const KeyType &key, ValueType value);

	/** @brief Finds a registered object pointer for a key.
	 *  @param key Search key.
	 *  @return Pointer if found or 0 if not found.
	 */
	ValueType findObj(const KeyType &key) const;

	/** @brief Removes a registered object pointer for a key, deletes the referenced object,
	 *  if m_destructMembers is true.
	 *  @param key Search key.
	 *  @return true if object was unregistered.
	 */
	bool unRegisterObj(const KeyType &key);

	/** @brief Constant iterator to access the elements (beginning).
	 *  @return Iterator with the first elements as current element.
	 */
	inline const_iterator begin() const
	{
		return m_map.begin();
	}

	/** @brief Constant iterator to access the elements (behind the last element).
	 *  @return Iterator to query the end of the iteration
	 *          (like sthe std iterators does not refer a valid element).
	 */
	inline const_iterator end() const
	{
		return m_map.end();
	}

	/** @brief Gets the size of the element map
	 * @return The number of stored elements.
	 */
	inline size_type size() const
	{
		return m_map.size();
	}
}; // TObjPtrRegistry

template<typename KeyType, typename ValueType>TObjPtrRegistry<KeyType, ValueType>::
TObjPtrRegistry(bool destructMembers) :
m_destructMembers(destructMembers)
{
}

template<typename KeyType, typename ValueType>
TObjPtrRegistry<KeyType, ValueType>::
~TObjPtrRegistry()
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
bool TObjPtrRegistry<KeyType, ValueType>::
registerObj(const KeyType &key, ValueType value)
{
	if ( m_map.find(key) == m_map.end() ) {
		m_map[key] = value;
		return true;
	}
	return false;
}


template<typename KeyType, typename ValueType>
ValueType TObjPtrRegistry<KeyType, ValueType>::
findObj(const KeyType &key) const
{
	
	typename std::map<KeyType, ValueType>::const_iterator i;
	if ( (i = m_map.find(key)) != m_map.end() ) {
		return (*i).second;
	}
	return 0;
}

template<typename KeyType, typename ValueType>
bool TObjPtrRegistry<KeyType, ValueType>::
unRegisterObj(const KeyType &key)
{
	typename std::map<KeyType, ValueType>::iterator i;
	if ( (i = m_map.find(key)) != m_map.end() ) {
		if ( m_destructMembers && (*i).second != NULL ) {
			ValueType ptr = (*i).second;
			delete ptr;
		}
		m_map.erase(key);
		return true;
	}
	return false;
}

};

#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H
