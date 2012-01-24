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

#include <vector>

namespace RiCPP {

// =============================================================================
/** @brief Node for TemplBinTree binary tree.
 *
 *  PayloadType is the data tye of the content, LinkType the type of
 *  the references (e.g. unsigned long if nodes are srtored within a
 *  vector). LinkType(0) indicates the NULL pointer equivalent.
 */
template<typename PayloadType, typename LinkType = unsigned long>
class TemplTreeNode {

private:
	LinkType    m_ref;     ///< Own reference.
	LinkType    m_left;    ///< Left child (<).
	LinkType    m_right;   ///< Right chlid (>=).
	LinkType    m_parent;  ///< Parent.
	PayloadType m_content; ///< Data.

public:
	/** @brief Constructor, just clears the links.
	 */
	inline TemplTreeNode()
	{
		m_ref = m_left = m_right = m_parent = LinkType(0);
	}
	
	/** @brief Destructor, to be virtual.
	 */
	inline virtual ~TemplTreeNode() {}

	/** @brief The included data.
	 *  @return Reference to the data of the node, writeable.
	 */
	inline PayloadType &content() { return m_content; }
	
	/** @brief The included data.
	 *  @return Reference to the data of the node, read only.
	 */
	inline const PayloadType &content() const { return m_content; }

	/** @brief The left child (less than current).
	 *  @return Reference to the left child, writeable.
	 */
	inline LinkType &left() { return m_left; }

	/** @brief The left child (less than current).
	 *  @return Reference to the left child, read only.
	 */
	inline const LinkType &left() const { return m_left; }

	/** @brief The right child (greater than or equal current).
	 *  @return Reference to the right child, writeable.
	 */
	inline LinkType &right() { return m_right; }

	/** @brief The right child (greater than or equal current).
	 *  @return Reference to the right child, read only.
	 */
	inline const LinkType &right() const { return m_right; }

	/** @brief Predecessor node.
	 *  @return Reference to the predecessor node, writeable.
	 */
	inline LinkType &parent() { return m_parent; }

	/** @brief Predecessor node.
	 *  @return Reference to the predecessor node, read only.
	 */
	inline const LinkType &parent() const { return m_parent; }

	/** @brief Own reference
	 *  @return Reference of this node, writeable.
	 */
	inline LinkType &ref() { return m_ref; }

	/** @brief Own reference
	 *  @return Reference of this node, read only.
	 */
	inline const LinkType &ref() const { return m_ref; }
}; // TemplTreeNode


// =============================================================================
/** @brief Template for a binary tree structure.
 *
 *  Uses TemplTreeNode to build a binary sorting tree.
 *  PayloadType is the data tye of the content, LinkType the type of
 *  the references (e.g. unsigned long if nodes are srtored within a
 *  vector). LinkType(0) indicates the NULL pointer equivalent. NodeContainer
 *  is the container type of all nodes, e.g. a vector of TemplTreeNode
 */
template <
	typename PayloadType,
	typename LinkType = unsigned long,
	typename NodeContainer=std::vector<TemplTreeNode<PayloadType, LinkType> >
>
class TemplBinTree {

private:
	LinkType m_root; ///< References the root of the tree structure, LinkeType(0) if empty.

public:
	/** Constructor for an empty tree.
	 */
	inline TemplBinTree() { m_root = LinkType(0); }
	
	/** Gets the root of the tree.
	 *
	 *  @return The root reference of the tree, writeable
	 */
	inline LinkType &root() { return m_root; }
	
	/** Gets the root of the tree
	 *
	 *  @return The root reference of the tree, read only.
	 */
	inline const LinkType &root() const { return m_root; }
	
	/** Clears the contenst of the tree by clearing the root reference.
	 */
	inline void clear() { root() = LinkType(0); }
	
	/** Queries whether the tree is empty.
	 *
	 *  @return True, if the tree is empty (root is the 0 reference).
	 */
	inline bool empty() const { return root() == LinkType(0); }

	/** @brief Looks whether a node is part of the tree.
	 *
	 *  @param aNodeRef Reference of a node.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline bool isLinked(LinkType aNodeRef, NodeContainer &nodeContainer) const
	{
		return !empty() && (root() == aNodeRef || nodeContainer[aNodeRef].parent() != LinkType(0));
	}

	/** @brief Sorted insert of a node.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param aNodeRef References node to linked in.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline void insert(LinkType atNodeRef, LinkType aNodeRef,
							   NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) || aNodeRef == LinkType(0) || isLinked(aNodeRef, nodeContainer) )
			return;

		if ( nodeContainer[aNodeRef].content() <
			 nodeContainer[atNodeRef].content() )
		{
			if ( nodeContainer[atNodeRef].left() != LinkType(0) ) {
				insert(nodeContainer[atNodeRef].left(), aNodeRef,
					   nodeContainer);
			} else {
				nodeContainer[atNodeRef].left() = aNodeRef;
				nodeContainer[aNodeRef].parent() = atNodeRef;
			}
		} else {
			if ( nodeContainer[atNodeRef].right() != LinkType(0) ) {
				insert(nodeContainer[atNodeRef].right(), aNodeRef,
					   nodeContainer);
			} else {
				nodeContainer[atNodeRef].right() = aNodeRef;
				nodeContainer[aNodeRef].parent() = atNodeRef;
			}
		}
	}

	/** @brief Sorted insert of a node, starts searching at root.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline void insert(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return;
		if ( root() == LinkType(0) ) {
			root() = aNodeRef;
			nodeContainer[aNodeRef].parent() = LinkType(0);
		} else {
			insert(root(), aNodeRef, nodeContainer);
		}
	}

	/** @brief Finds a node for given data.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param aValue Value of a node to find.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline LinkType find(LinkType aNodeRef, PayloadType aValue,
								 NodeContainer &nodeContainer)
	{
		if ( !aNodeRef )
			return LinkType(0);

		if ( aValue < nodeContainer[aNodeRef].content() )
			return find(nodeContainer[aNodeRef].left(), aValue, nodeContainer);

		if ( aValue == nodeContainer[aNodeRef].content() )
			return aNodeRef;

		return find(nodeContainer[aNodeRef].right(), aValue, nodeContainer);
	}

	/** @brief Finds a node for given data, starts searching at root.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline LinkType find(PayloadType aValue,
						 NodeContainer &nodeContainer)
	{
		return find(root(), aValue, nodeContainer);
	}
	
	/** @brief Finds the node with the smallest value within a subtree.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline LinkType minNode(LinkType aNodeRef,
							NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return LinkType(0);
		LinkType ref = aNodeRef;
		while ( nodeContainer[ref].left() != LinkType(0) )
			ref = nodeContainer[ref].left();
		return ref;
	}
	
	/** @brief Finds the node with the biggest value within a subtree.
	 *
	 *  @param atNodeRef References node to start from.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline LinkType maxNode(LinkType aNodeRef,
							NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) )
			return LinkType(0);
		LinkType ref = aNodeRef;
		while ( nodeContainer[ref].right() != LinkType(0) )
			ref = nodeContainer[ref].right();
		return ref;
	}

	/** @brief Removes a linked node from the tree.
	 *
	 *  @param aNodeRef References a node to delete.
	 *  @param nodeContainer Container of all nodes.
	 */
	inline void remove(LinkType aNodeRef, NodeContainer &nodeContainer)
	{
		if ( aNodeRef == LinkType(0) || !isLinked(aNodeRef, nodeContainer)  )
			return;

		if ( nodeContainer[aNodeRef].left() == LinkType(0) ) {

			if ( aNodeRef == root() ) {
				root() = nodeContainer[aNodeRef].right();
				if ( root() != LinkType(0) ) {
					nodeContainer[root()].parent() = LinkType(0);
				}
			} else {
				if ( nodeContainer[aNodeRef].right() != LinkType(0) )
					nodeContainer[nodeContainer[aNodeRef].right()].parent() =
						nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() ==
					 aNodeRef )
				{
					nodeContainer[nodeContainer[aNodeRef].parent()].left() =
						nodeContainer[aNodeRef].right();
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() =
						nodeContainer[aNodeRef].right();
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
					nodeContainer[nodeContainer[aNodeRef].left()].parent() =
						nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() ==
					 aNodeRef )
				{
					nodeContainer[nodeContainer[aNodeRef].parent()].left() =
						nodeContainer[aNodeRef].left();
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() =
						nodeContainer[aNodeRef].left();
				}
			}

		} else {

			LinkType maxofmin = maxNode(nodeContainer[aNodeRef].left(),
										nodeContainer);
			remove(maxofmin, nodeContainer);
			nodeContainer[maxofmin].left() = nodeContainer[aNodeRef].left();
			if ( nodeContainer[aNodeRef].left() != LinkType(0) ) {
				nodeContainer[nodeContainer[aNodeRef].left()].parent() =
					maxofmin;
			}
			nodeContainer[maxofmin].right() = nodeContainer[aNodeRef].right();
			if ( nodeContainer[aNodeRef].right() != LinkType(0) ) {
				nodeContainer[nodeContainer[aNodeRef].right()].parent() =
					maxofmin;
			}
			if ( aNodeRef == root() ) {
				root() = maxofmin;
				nodeContainer[root()].parent() = LinkType(0);
			} else {
				nodeContainer[maxofmin].parent() =
					nodeContainer[aNodeRef].parent();
				if ( nodeContainer[nodeContainer[aNodeRef].parent()].left() ==
					aNodeRef )
				{
					nodeContainer[nodeContainer[aNodeRef].parent()].left() =
						maxofmin;
				} else {
					nodeContainer[nodeContainer[aNodeRef].parent()].right() =
						maxofmin;
				}
			}
		}

		nodeContainer[aNodeRef].parent() = LinkType(0);
		nodeContainer[aNodeRef].left()   = LinkType(0);
		nodeContainer[aNodeRef].right()  = LinkType(0);
	}
}; // TemplBinTree

} // namespace RiCPP

#endif // _RICPP_TOOLS_BINTREE_H
