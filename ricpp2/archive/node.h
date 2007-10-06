#ifndef _RICPP_RICPP_NODE_H
#define _RICPP_RICPP_NODE_H

/*! \file node.h
 *  \brief List structure, used as base for render primitives
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_TOOLS_PLATFORM_H
#include "ricpp/tools/platform.h"
#endif // _RICPP_TOOLS_PLATFORM_H

#include <stack>
#include <cassert>

namespace RiCPP {

//! Base class
/*! This base class gives some definitions for duplication, assignment (deep copy) and
 *  runtime type checking. The member functions should be implemented in all
 *  child classes of CBaseClass
 */
class CBaseClass {
public:
	//! The only purpose of the static member myClassName is to return the classname as a c-string.
	/*! \return The name of the class as c-string
	 */
	inline static const char *myClassName(void) { return "CBaseClass"; }

	//! The standard constructor of the class, does nothing
	inline CBaseClass() { }
	//! The copy constructor of the class, calls assign()
	/*! \param o Instance to copy from.
	 */
	inline CBaseClass(const CBaseClass &o) { *this = o; }
	//! The destructor is declared to be virtual
	inline virtual ~CBaseClass() {}

	//! The duplicate() returns a duplication of the instance (clone).
	/*! \return A copy of the instance itself, a reference to a clone
	 *  \exception TSystemException if no new instance could be created
	 */
	inline virtual CBaseClass &duplicate() {
		CBaseClass *o = new CBaseClass(*this);
		if ( !o )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)0, __LINE__, __FILE__);
		return *o;
	}

	//! Returns the true class name of an isntance
	/*! Since this member function is virtual, it returns the name of
	 *  the last child.
	 *  \return The name of the class of an instance as c-string
	 */
	inline virtual const char *className() const { return CBaseClass::myClassName(); }

	//! Ask if an instance inherits from a given class
	/*! \param aClass Name of a class to test if inherited from
	 *  \return true: if class is a child from a class with the name aClass, false: otherwise
	 */
	virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		return !strcasecmp(aClass, CBaseClass::myClassName());
	}

	//! Assigns (deep copies) the values of an instance to this
	/*! \param o Instance to copy from
	 *  \return reference to *this
	 *  \exception TSystemException if o and this are not compatible for assignment
	 */
	inline CBaseClass &operator=(const CBaseClass &o) {
		if ( &o == this )
			return *this;

		return *this;
	}
};

//! Base class for all nodes of the graph
class CNode : public CBaseClass {
	CNode *m_next;		//!< Next element
	CNode *m_descendent;	//!< Descendent element
	bool m_linkDescendent;	//!< Descendent is a link and will not be destroyed

public:
	inline static const char *myClassName(void) { return "CNode"; }
	inline virtual const char *className() const { return CNode::myClassName(); }

	//! Duplication, note is not linked
	inline virtual CBaseClass &duplicate() {
		CNode *o = new CNode(*this);
		if ( !o )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)0, __LINE__, __FILE__);
		return *o;
	}

	//! Ask if an instance inherits from a given class
	virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		return !strcasecmp(aClass, CNode::myClassName()) || CBaseClass::isKindOf(aClass);
	}

	//! Assigns the values of an instance to this, instance is not linked
	inline CNode &operator=(const CNode &o) {
		if ( &o == this )
			return *this;

		m_next = NULL;
		m_descendent = NULL;
		m_linkDescendent = false;

		CBaseClass::operator=(o);

		return *this;
	}

	//! Copy constructor, clears the elements (node is not linked !)
	inline CNode(const CNode &node) :
		m_next(NULL), m_descendent(NULL), m_linkDescendent(false)
	{
		*this = node;
	}

	//! Standard constructor, clears the elements.
	inline CNode() :
		m_next(NULL), m_descendent(NULL), m_linkDescendent(false)
	{
	}

	//! Destructor, destroys a graph recursively.
	virtual ~CNode();

	//! The next element.
	/*! \return Pointer to the next node.
	 */
	inline virtual CNode * &next() { return m_next; }
	//! The descendent element.
	/*! \return Pointer to the descendent node.
	 */
	inline virtual CNode * &descendent() { return m_descendent; }

	//! The last element.
	/*! \return Pointer to the last node at the "next"-chain.
	 */
	inline virtual CNode *last() {
		CNode *i = this;
		for ( i = this; i->next(); i = i->next() );
		return i;
	}

	//! Appends a node.
	/*! \return Appends a node at the last node at the "next"-chain.
	 */
	inline virtual void append(CNode *n) { last()->next() = n; }

	//! Links a descendent.
	/*! Simply links the decendent pointer, don't care if already set.
	 *  \return Appends a node as the descendent.
	 */
	inline virtual void linkDescendent(CNode *link) {
		m_linkDescendent = true;
		m_descendent = link;
	}
}; // CNode

}

#endif // _RICPP_RICPP_NODE_H
