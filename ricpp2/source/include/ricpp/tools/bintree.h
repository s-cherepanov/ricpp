#ifndef _RICPP_TOOLS_BINTREE_H
#define _RICPP_TOOLS_BINTREE_H

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

/** @file bintree.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Simple template for a binary tree and a node template.
 */

template<typename PayloadType, typename LinkType>
class TemplTreeNode {
private:
	LinkType m_ref, m_left, m_right, m_parent;
	PayloadType m_content;

public:
	inline TemplTreeNode() { m_ref = m_left = m_right = m_parent = LinkType(0); }
	inline virtual ~TemplTreeNode() {}

	inline PayloadType &content() { return m_content; }
	inline const PayloadType &content() const { return m_content; }

	inline LinkType &left() { return m_left; }
	inline const LinkType &left() const { return m_left; }

	inline LinkType &right() { return m_right; }
	inline const LinkType &right() const { return m_right; }

	inline LinkType &parent() { return m_parent; }
	inline const LinkType &parent() const { return m_parent; }

	inline LinkType &ref() { return m_ref; }
	inline const LinkType &ref() const { return m_ref; }
}; // TemplTreeNode


template<typename PayloadType, typename LinkType, typename NodeContainer>
class TemplBinTree {
private:
	LinkType m_root;

	inline virtual void insert(LinkType atNodeRef, LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( nodeContainer[aNodeRef].content() < nodeContainer[atNodeRef].content() ) {
			if ( nodeContainer[atNodeRef].left() != LinkType(0) ) {
				insert(nodeContainer[atNodeRef].left(), aNodeRef, nodeContainer);
			} else {
				nodeContainer[atNodeRef].left() = aNodeRef;
				nodeContainer[aNodeRef].parent() = atNodeRef;
			}
		} else {
			if ( nodeContainer[atNodeRef].right() != LinkType(0) ) {
				insert(nodeContainer[atNodeRef].right(), aNodeRef, nodeContainer);
			} else {
				nodeContainer[atNodeRef].right() = aNodeRef;
				nodeContainer[aNodeRef].parent() = atNodeRef;
			}
		}
	}

	inline virtual LinkType find(LinkType aNodeRef, PayloadType aValue, NodeContainer &nodeContainer)
	{
		if ( !aNodeRef )
			return false;
		if ( aValue < nodeContainer[aNodeRef].content() )
			return find(nodeContainer[aNodeRef].left(), aValue, nodeContainer);
		if ( aValue > nodeContainer[aNodeRef].content() )
			return find(nodeContainer[aNodeRef].right(), aValue, nodeContainer);
		return aNodeRef;
	}

public:
	inline TemplBinTree() { m_root = LinkType(0); }

	inline virtual ~TemplBinTree() {}

	inline virtual LinkType &root() { return m_root; }

	inline virtual const LinkType &root() const { return m_root; }

	inline virtual void clear() { root() = LinkType(0); }

	inline virtual bool empty() const { return root() == LinkType(0); }

	inline virtual void insert(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( root() == LinkType(0) ) {
			root() = aNodeRef;
			nodeContainer[aNodeRef].parent() = LinkType(0);
		} else {
			insert(root(), aNodeRef, nodeContainer);
		}
	}

	inline virtual void remove(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return;

		if ( nodeContainer[aNodeRef].left() == LinkType(0) ) {

			if ( aNodeRef == root() ) {
				root() = nodeContainer[aNodeRef].right();
				if ( root() != LinkType(0) ) {
					nodeContainer[root()].parent() = LinkType(0);
				}
			} else {
				if ( nodeContainer[aNodeRef].right() != LinkType(0) )
					nodeContainer[nodeContainer[aNodeRef].right()].parent() = nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() == aNodeRef ) {
					nodeContainer[nodeContainer[aNodeRef].parent()].left() = nodeContainer[aNodeRef].right();
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() = nodeContainer[aNodeRef].right();
				}
			}

		} else if ( nodeContainer[aNodeRef].right() == LinkType(0) ) {

			if ( aNodeRef == root() ) {
				root() = nodeContainer[aNodeRef].left();
				if ( root() != LinkType(0) ) {
					nodeContainer[root()].parent() = LinkType(0);
				}
			} else {
				if ( nodeContainer[aNodeRef].left() != LinkType(0) )
					nodeContainer[nodeContainer[aNodeRef].left()].parent() = nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() == aNodeRef ) {
					nodeContainer[nodeContainer[aNodeRef].parent()].left() = nodeContainer[aNodeRef].left();
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() = nodeContainer[aNodeRef].left();
				}
			}

		} else {

			LinkType maxofmin = maxNode(nodeContainer[aNodeRef].left(), nodeContainer);
			remove(maxofmin, nodeContainer);
			nodeContainer[maxofmin].left() = nodeContainer[aNodeRef].left();
			if ( nodeContainer[aNodeRef].left() != LinkType(0) ) {
				nodeContainer[nodeContainer[aNodeRef].left()].parent() = maxofmin;
			}
			nodeContainer[maxofmin].right() = nodeContainer[aNodeRef].right();
			if ( nodeContainer[aNodeRef].right() != LinkType(0) ) {
				nodeContainer[nodeContainer[aNodeRef].right()].parent() = maxofmin;
			}
			if ( aNodeRef == root() ) {
				root() = maxofmin;
				nodeContainer[root()].parent() = LinkType(0);
			} else {
				nodeContainer[maxofmin].parent() = nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() == aNodeRef ) {
					nodeContainer[nodeContainer[aNodeRef].parent()].left() = maxofmin;
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() = maxofmin;
				}
			}
		}

		nodeContainer[aNodeRef].parent() = LinkType(0);
		nodeContainer[aNodeRef].left()   = LinkType(0);
		nodeContainer[aNodeRef].right()  = LinkType(0);
	}

	inline virtual LinkType find(PayloadType aValue, NodeContainer &nodeContainer)
	{
		return find(root(), aValue, nodeContainer);
	}

	inline virtual LinkType minNode(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return LinkType(0);
		LinkType ref = aNodeRef;
		while ( nodeContainer[ref].left() != LinkType(0) )
			ref = nodeContainer[ref].left();
		return ref;
	}

	inline virtual LinkType maxNode(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return LinkType(0);
		LinkType ref = aNodeRef;
		while ( nodeContainer[ref].right() != LinkType(0) )
			ref = nodeContainer[ref].right();
		return ref;
	}
}; // TemplBinTree

#endif // _RICPP_TOOLS_BINTREE_H
