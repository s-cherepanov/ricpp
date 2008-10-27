#ifndef _RICPP_RENDERCONTEXT_BASECLASS_H
#define _RICPP_RENDERCONTEXT_BASECLASS_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
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

/*! \file baseclass.h
 *  \brief Declares TBaseClass, a base class for classes with type checking etc., all inline
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rendererloader/sysexcept.h"

//! Base class
/*! This base class gives some definitions for duplication, assignment (deep copy) and
 *  runtime type checking. The member functions should be implemented in all
 *  child classes of TBaseClass
 */
class TBaseClass {
protected:
	//! Assigns the values of another TBaseClass to this.
	/*! The non virtual assignment for this class does nothin
	 *  since TBaseClass has no data members. Only an illustration of
	 *  assign(). assign() should only copy those values that are
	 *  declared directly eithin the class and leave the inherited
	 *  data mebers alone. These are copied by the operator=.
	 *  \param o Instance to copy the values from.
	 */
	void assign(const TBaseClass &o) { }
public:
	//! The only purpose of the static member myClassName is to return the classname as a c-string.
	/*! \return The name of the class as c-string
	 */
	inline static const char *myClassName(void) { return "TBaseClass"; }

	//! The standard constructor of the class, does nothing
	inline TBaseClass() { }
	//! The copy constructor of the class, calls assign()
	/*! \param o Instance to copy from.
	 */
	inline TBaseClass(const TBaseClass &o) { assign(o); }
	//! The destructor is declared to be virtual
	inline virtual ~TBaseClass() {}

	//! The duplicate() returns a duplication of the instance (clone).
	/*! \return A copy of the instance itself, a reference to a clone
	 *  \exception TSystemException if no new instance could be created
	 */
	inline virtual TBaseClass &duplicate() {
		TBaseClass *o = new TBaseClass(*this);
		if ( !o )
			throw TSystemException(__FILE__, __LINE__);
		return *o;
	}

	//! Returns the true class name of an isntance
	/*! Since this member function is virtual, it returns the name of
	 *  the last child.
	 *  \return The name of the class of an instance as c-string
	 */
	inline virtual const char *className() const { return TBaseClass::myClassName(); }

	//! Ask if an instance inherits from a given class
	/*! \param aClass Name of a class to test if inherited from
	 *  \return true: if class is a child from a class with the name aClass, false: otherwise
	 */
	virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		return !stricmp(aClass, TBaseClass::myClassName());
	}

	//! Assigns (deep copies) the values of an instance to this
	/*! \param o Instance to copy from
	 *  \return reference to *this
	 *  \exception TSystemException if o and this are not compatible for assignment
	 */
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TBaseClass::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		TBaseClass::assign(o);
		return *this;
	}
};

#endif // _RICPP_RENDERCONTEXT_BASECLASS_H

