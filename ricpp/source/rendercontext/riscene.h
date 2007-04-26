// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2003, Andreas Pidde
//
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

/*! \file riscene.h
 *  \brief List structure, used as scene graph, used by libmesh
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include <stack>
#include <assert.h>
#include "baseclass.h"

//! Base class for all nodes of the graph
class TRiNode : public TBaseClass {
	TRiNode *m_next;		//!< Next element
	TRiNode *m_descendent;	//!< Descendent element
	bool m_linkDescendent;	//!< Descendent is a link and will not be destroyed
public:
	inline static const char *myClassName(void) { return "TRiNode"; }
	inline virtual const char *className() const { return TRiNode::myClassName(); }

	//! Duplication, note is not linked
	inline virtual TBaseClass &duplicate() {
		TRiNode *o = new TRiNode(*this);
		if ( !o )
			throw TSystemException(__FILE__, __LINE__);
		return *o;
	}

	//! Ask if an instance inherits from a given class
	virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		return !stricmp(aClass, TRiNode::myClassName()) || TBaseClass::isKindOf(aClass);
	}

	//! Assigns the values of an instance to this, instance is not linked
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TRiNode::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		m_next = NULL;
		m_descendent = NULL;
		m_linkDescendent = false;
		return TBaseClass::operator=(o);
	}

	//! Copy constructor, clears the elements (node is not linked !)
	inline TRiNode(const TRiNode &node) : m_next(NULL), m_descendent(NULL), m_linkDescendent(false) { }

	//! Standard constructor, clears the elements.
	inline TRiNode() : m_next(NULL), m_descendent(NULL), m_linkDescendent(false) { }

	//! Destructor, destroys a graph recursively.
	virtual ~TRiNode();

	//! The next element.
	/*! \return Pointer to the next node.
	 */
	inline virtual TRiNode * &next() { return m_next; }
	//! The descendent element.
	/*! \return Pointer to the descendent node.
	 */
	inline virtual TRiNode * &descendent() { return m_descendent; }

	//! The last element.
	/*! \return Pointer to the last node at the "next"-chain.
	 */
	inline virtual TRiNode *last() {
		TRiNode *i = this;
		for ( i = this; i->next(); i = i->next() );
		return i;
	}

	//! Appends a node.
	/*! \return Appends a node at the last node at the "next"-chain.
	 */
	inline virtual void append(TRiNode *n) { last()->next() = n; }

	//! Links a descendent.
	/*! Simply links the decendent pointer, don't care if already set.
	 *  \return Appends a node as the descendent.
	 */
	inline virtual void linkDescendent(TRiNode *link) {
		m_linkDescendent = true;
		m_descendent = link;
	}
}; // TRiNode


//! Class is used to build a scene graph of TRiNode, the graph can be detached after building
class TRiSceneBuilder {
	TRiNode m_sentinel;			//!< First element in List is m_sentinel.m_descendent()

	struct TRiPosition {
		TRiNode *m_current;     //!< current element
		TRiNode **m_insert;     //!< current insert position (next or descendent)
	};
	TRiPosition m_position;

	std::stack<TRiNode *> m_block;	//!< depth blocks
	std::stack<TRiPosition> m_mark;	//!< Positions

public:
	//! Moves the insert point to the end of a "next"-chain
	void moveToEnd();

	//! Clears internal data, inserts a new graph first, and moves insertpointers to the end of the top "next"-chain.
	/*! \param first new graph for the scene builder
	 */
	void reset(TRiNode *first);

	//! Standard constructor, empty graph
	inline TRiSceneBuilder()  {
		reset(NULL);
	}

	//! Standard destructor, clears via member destructors
	inline ~TRiSceneBuilder() { }

	//! Inserts a node at the current position
	/*! \param node Node that is inserted
	 */
	inline void insert(TRiNode *node) {
		assert (node);
		*m_position.m_insert = node;
		m_position.m_current = node;
	}

	//! Moves insert position to the next position of a chain
	inline void nextPosition() {
		m_position.m_insert = &(m_position.m_current->next());
	}

	//! Moves insert position to the descendent of the current position
	inline void descendentPosition() {
		m_block.push(m_position.m_current);
		m_position.m_insert = &(m_position.m_current->descendent());
	}

	//! Moves insert position to the next insert position of an ascendent
	inline void ascentPosition() {
		assert(!m_block.empty());
		m_position.m_current = m_block.top();
		m_block.pop();
		nextPosition();
	}

	//! Inserts a node and moves to the next position of the "next"-chain (e.g. a primitive)
	/*! \param node Node to insert
	 */
	inline void insertAndNext(TRiNode *node) {
		insert(node);
		nextPosition();
	}

	//! Inserts a node and moves to the descendent position of the current position (e.g. an RiFrameBegin)
	/*! \param node Node to insert
	 */
	inline void insertAndDescent(TRiNode *node) {
		insert(node);
		descendentPosition();
	}

	//! Inserts a node and ascends (e.g. an RiFrameEnd)
	/*! \param node Node to insert
	 */
	inline void insertAndAscent(TRiNode *node) {
		insert(node);
		ascentPosition();
	}

	//! Inserts a node, links a descendent and moves to the next position of the "next"-chain (e.g. an RiObjectInstance)
	/*! \param node Node to insert
	 *  \param node link to link as descendent
	 */
	inline void insertAndLink(TRiNode *node, TRiNode *link) {
		insert(node);
		m_position.m_current->linkDescendent(link);
		nextPosition();
	}

	//! Looks if graph is empty
	/*! \return true if graph is empty, false otherwise
	 */
	inline bool empty() { return first() == NULL; }

	/*! \return A pointer to the first (top) node of the graph
	 */
	inline TRiNode *first() { return m_sentinel.descendent(); }

	/*! \return A pointer to current insert node
	 */
	inline TRiNode *current() { return m_position.m_current; }

	/*! \return A pointer to current insert position (pointer to a next or descendent pointer of the current node)
	 */
	inline TRiNode **insertPosition() { return m_position.m_insert; }

	//! Detaches the graph, graph of this instance is empty after detached
	/*! \return The pointer to the graph
	 */
	inline TRiNode *detach() {
		TRiNode *rval = first();
		reset(NULL);
		return rval;
	}

	//! Attaches graph
	/*! \param firstNode First (top) node of the graph that will be attached
	 *  \return The pointer to the old graph
	 */
	inline TRiNode *attach(TRiNode *firstNode) {
		TRiNode *rval = first();
		reset(firstNode);
		return rval;
	}

	//! Marks an insert position (used for block structure)
	inline void pushMark() {
		m_mark.push(m_position);
	}

	//! Clears a mark of an insert position (used for block structure)
	inline void popMark()  {
		assert (!m_mark.empty() );
		m_position = m_mark.top();
		m_mark.pop();
	}
}; // TRiSceneBuilder


//! Used to walk through a graph of TRiNode build by TRiSceneBuilder
class TRiSceneIterator {
	TRiNode *m_first;               //!< First element in List
	TRiNode *m_current;             //!< Current element
	std::stack<TRiNode *> m_block;	//!< Depth blocks
public:
	//! Standard constructor, initializes for the graph node
	/*! \param node First (top) node of a graph
	 */
	inline TRiSceneIterator(TRiNode *node) {
		m_current = m_first = node;
	}

	//! Resets the iterator to point to the first (top) node of it's graph.
	inline void reset() {
		m_current = m_first;
		while (!m_block.empty())
			m_block.pop();
	}

	//! Iterates the graph
	inline TRiNode *operator()();

	// Finds a handle
	// inline TRiNode *findHandle(TRiNode *handle);
}; // TRiSceneIterator
