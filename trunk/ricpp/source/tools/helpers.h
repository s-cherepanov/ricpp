#ifndef _RICPP_TOOLS_HELPERS_H
#define _RICPP_TOOLS_HELPERS_H

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

/*! \file helpers.h
 *  \brief Helper functions, all declared inline, used by RiCPP
 *  \author Andreas Pidde (andreas@pidde.de)
 */
#include "rendererloader/ricpp.h"
#include "rendererloader/sysexcept.h"

#include <math.h>
#include <assert.h>

#include <ostream>
#include <deque>
#include <vector>
#include <string>

//! Define the constant pi
//{
/*
const RtFloat piTimes2=(RtFloat)(8.0*atan(1.0));
const RtFloat pi=(RtFloat)(4.0*atan(1.0));
const RtFloat pi_2=(RtFloat)(2.0*atan(1.0));
const RtFloat pi_4=(RtFloat)(atan(1.0));
const double d_piTimes2=(6.0*atan(1.0));
const double d_pi=(4.0*atan(1.0));
const double d_pi_2=(2.0*atan(1.0));
const double d_pi_4=atan(1.0);
*/

const RtFloat piTimes2   = (RtFloat)6.283185307179586476925286766559;
const RtFloat pi         = (RtFloat)3.1415926535897932384626433832795;
const RtFloat pi_2       = (RtFloat)1.5707963267948966192313216916398;
const RtFloat pi_4       = (RtFloat)0.78539816339744830961566084581988;

const double  d_piTimes2 = 6.283185307179586476925286766559;
const double  d_pi       = 3.1415926535897932384626433832795;
const double  d_pi_2     = 1.5707963267948966192313216916398;
const double  d_pi_4     = 0.78539816339744830961566084581988;
//}

//! Convert degrees to radians
/*! \param Degree The degree value to convert
 *  \return The radian value ((degree * pi) / 180.0)
 */
template <typename _T> inline _T deg2rad(_T degree) {return (_T)((degree * pi) / 180.0);}

//! Convert radians to degrees
/*! \param Degree The radian value to convert
 *  \return The degree value (radian * 180.0) / pi)
 */
template <typename _T> inline _T rad2deg(_T radian) {return (_T)((radian * 180.0) / pi);}

//! Get the sign of a value
/*! \param f The value to test
 *  \return -1 if f is negative, 1 otherwise
 */
template <typename _T> inline int sign(_T f) { return f < 0.0 ? -1 : 1; }

//! Normal vector (normalized) for a plane given by three points
/*! \param p1 Point 1
 *  \param p2 Point 2
 *  \param p3 Point 3
 *  \retval norm Normal
 *  \return true if normal is calculated, false otherwise
 */
template <class _T> inline bool planeNorm(const _T *p1, const _T *p2, const _T *p3, _T *norm) {
	_T v1[3] = {p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]};
	_T v2[3] = {p3[0]-p2[0], p3[1]-p2[1], p3[2]-p2[2]};

	norm[0] = v1[1]*v2[2] - v1[2]*v2[1];
	norm[1] = v1[2]*v2[0] - v1[0]*v2[2];
	norm[2] = v1[0]*v2[1] - v1[1]*v2[0];

	_T length = (_T)sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
	if ( length == 0.0 ) {
		norm[0] = 0.0; norm[1] = 0.0; norm[2] = 1.0;
		return false;
	}

	norm[0] /= length;
	norm[1] /= length;
	norm[2] /= length;

	return true;
}


//! Normal vector (normalized) for a plane given by two vectors
/*! \param v1 Vector 1
 *  \param v2 Vector 2
 *  \param p3 Point 3
 *  \retval norm Normal
 *  \return true if normal is calculated, fales aotherwise
 */
template <class _T> inline bool planeNorm(const _T *p1, const _T *p2, _T *norm) {
	_T v1[3] = {p1[0], p1[1], p1[2]};
	_T v2[3] = {p2[0], p2[1], p2[2]};

	norm[0] = v1[1]*v2[2] - v1[2]*v2[1];
	norm[1] = v1[2]*v2[0] - v1[0]*v2[2];
	norm[2] = v1[0]*v2[1] - v1[1]*v2[0];

	_T length = (_T)sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
	if ( length == 0.0 ) {
		norm[0] = 0.0; norm[1] = 0.0; norm[2] = 1.0;
		return false;
	}

	norm[0] /= length;
	norm[1] /= length;
	norm[2] /= length;

	return true;
}


//! Linear interpolation
template<class _C> inline _C lerp(_C u, _C minu, _C maxu) {
	return minu + u * (maxu - minu);
}

//! Bilinear interpolation
template<class _C> inline _C bilerp(_C u, _C v, _C x1, _C x2, _C x3, _C x4) {
	return lerp(v, lerp(u, x1, x2), lerp(u, x3, x4));
}

//! Deletes all contents of a container and calls destructors of the content objects
/*! \param c Container which contents are to be deleted
 */
template<class _C> inline void release_all(_C &c) {
	typename _C::iterator i;
	if ( c.size() > 0 ) {
		for ( i = c.begin(); i != c.end(); ++i ) {
		  if ( *i != NULL )
		      delete *i;
		}
		c.clear();
	}
}


//! Deletes all contents of a container and calls destructors of the content object bound on second
/*! \param c Container which contents (second) are to be deleted
 */
template<class _C> inline void release_second(_C &c) {
	typename _C::iterator i;
	for ( i = c.begin(); i != c.end(); ++i ) {
		if ( (*i).second != NULL )
			delete (*i).second;
	}
	c.clear();
}


//! Deletes all contents of a container and calls destructors of the content object bound on first and second
/*! \param c Container which contents (first second) are to be deleted
 */
template<class _C> inline void release_pair(_C &c) {
	typename _C::iterator i;
	for ( i = c.begin(); i != c.end(); ++i ) {
		delete (*i).first;
		delete (*i).second;
	}
	c.clear();
}


//! Searches in a container c for an element elem
/*! \param c Container that is searched
 *  \param elem Element to search for
 *  \return true if element is found, false otherwise
 */
template<class _T, class _C> inline bool has_element(_C &c, _T &elem) {
	typename _C::iterator i;
	for ( i = c.begin(); i != c.end(); ++i ) {
		if ( (*i) == elem )
			return true;
	}
	return false;
}


//! Pushes an element to a container c, if it is not already in there
/*! \param c Container in which element is pushed
 *  \param elem Element to push
 *  \return true if element is pushed, false otherwise
 */
template<class _T, class _C> inline bool push_back_unique(_C &c, _T &elem) {
	if ( !has_element(c, elem) ) {
		c.push_back(elem);
		return true;
	}
	return false;
}


// min and max got undefined in tools/ricppplatform.h !!!
// undefine max (maximum), and redefine it as template
// #define max(a,b)            (((a) > (b)) ? (a) : (b))
// #ifdef max
// #undef max
// #endif
// undefine min (minimum), and redefine it as template
// #define min(a,b)            (((a) < (b)) ? (a) : (b))
// #ifdef min
// #undef min
// #endif


//! Returns the maximum of two elements
/*! \param a First element
 *  \param b Second element
 *  \return Maximum of a and b
 */
template<class _T> inline _T max(_T a, _T b) {
	return a > b ? a : b;
}


//! Returns the maximum of a vector
/*! \param n The index type of the vector
 *  \param v The vector
 *  \return Maximum found in v
 */
template<class _IT, class _T> inline _T max(_IT n, const _T *v) {
	_T result = (_T)0;
	if (n > 0) {
		result = v[--n];
		while ( n-- ) {
			if ( v[n] > result )
				result = v[n];
		}
	}
	return result;
}


//! Returns the minimum of two elements
/*! \param a First element
 *  \param b Second element
 *  \return Minimum of a and b
 */
template<class _T> inline _T min(_T a, _T b) {
	return a < b ? a : b;
}


//! Returns the minimum of a vector
/*! \param n The index type of the vector
 *  \param v The vector
 *  \return Minimum found in v
 */
template<class _IT, class _T> inline _T min(_IT n, const _T *v) {
	_T result = (_T)0;
	if (n > 0) {
		result = v[--n];
		while ( n-- ) {
			if ( v[n] < result )
				result = v[n];
		}
	}
	return result;
}


//! Returns the sum of the elements in a vector
/*! \param n The index type of the vector
 *  \param v The vector
 *  \return Sum of elements of v
 */
template<class _IT, class _T> inline _T sum(_IT n, _T *v) {
	_T result = (_T)0;
	while ( n-- > 0 ) {
		result += v[n];
	}
	return result;
}

//! Simple container class for basic types, an vector with a name
/*! Used for vertex data, example: 20 times RtPoint[5]
 *  The m_numElems is 20, the m_cardinality (array size) is 5, the m_components is 3 (RtPoint consists of 2 RtFloat)
 */
template<class _T> class TNamedVector {
	std::string m_name;      //!< Name of the array
	IndexType m_components;  //!< Components of one element, bytes: sizeof(_T)*m_components
	IndexType m_cardinality; //!< Cardinality of one element, bytes: sizeof(_T)*m_components*m_cardinality
	IndexType m_numElems;    //!< Number of elements, bytes: sizeof(_T)*m_components*m_cardinality*m_numElems
	IndexType m_ownData;     //!< m_data is allocated by this instance
	_T *m_data;              //!< pointer to the first element of data
public:
	//! Standard constructor to initialize empty members
	inline TNamedVector()
		: m_name(""), m_components(0), m_cardinality(0), m_numElems(0),
		  m_ownData(false), m_data(NULL)
	{}

	//! Constructor to initialize members with foreign data (only pointer is copied)
	/*! \param name Name of the array
	 *  \param components Size of the components of a single element, e.g. 3 for RtPointer
	 *  \param cardinality if element is an array, he number of elements in this array (e.g. for RtPoint[5], 5 is the cardinality)
	 *  \param numElems Number of (compount) element in the vector
	 *  \param data pointer of data to copy
	 */
	inline TNamedVector(const char *name, IndexType components, IndexType cardinality, IndexType numElems, _T *data)
		: m_name(name ? name : ""), m_components(components), m_cardinality(cardinality), m_numElems(numElems),
		  m_ownData(false), m_data(data)
	{}

	//! Initialize members with foreign data (only pointer is copied)
	/*! \param name Name of the array
	 *  \param components Size of the components of a single element, e.g. 3 for RtPointer
	 *  \param cardinality if element is an array, he number of elements in this array (e.g. for RtPoint[5], 5 is the cardinality)
	 *  \param numElems Number of (compount) element in the vector
	 *  \param data pointer of data to copy
	 */
	inline void initialize(const char *name, IndexType components, IndexType cardinality, IndexType numElems, _T *data)
	{
		m_name = name ? name : "";
		m_components = components;
		m_cardinality = cardinality;
		m_numElems = numElems;
		m_ownData = false;
		m_data = data;
	}


	//! Clears the m_data and frees the memory. m_name is not touched.
	inline void clear()
	{
		m_components = 0;
		m_cardinality = 0;
		m_numElems = 0;
		if ( m_ownData && m_data )
			delete[] m_data;
		m_data = NULL;
		m_ownData = false;
	}

	//! Constructor to initialize members and allocate data
	/*! \param name Name of the array
	 *  \param components Size of the components of a single element, e.g. 3 for RtPointer
	 *  \param cardinality if element is an array, he number of elements in this array (e.g. for RtPoint[5], 5 is the cardinality)
	 *  \param numElems Number of (compount) element in the vector
	 *  \param data pointer of data to copy
	 */
	inline TNamedVector(const char *name, IndexType components, IndexType cardinality, IndexType numElems)
		: m_name(name ? name : ""), m_components(components), m_cardinality(cardinality), m_numElems(numElems), m_ownData(true)
	{
		m_data = NULL;
		if ( size() > 0 ) {
			m_data = new _T[size()];
			if ( !m_data )
				throw TSystemException(__FILE__, __LINE__);
		}
	}

	//! Initialize members and allocate data
	/*! \param name Name of the array
	 *  \param components Size of the components of a single element, e.g. 3 for RtPointer
	 *  \param cardinality if element is an array, he number of elements in this array (e.g. for RtPoint[5], 5 is the cardinality)
	 *  \param numElems Number of (compount) element in the vector
	 *  \param data pointer of data to copy
	 */
	inline void initialize(const char *name, IndexType components, IndexType cardinality, IndexType numElems)
	{
		m_name = name ? name : "";
		m_components = components;
		m_cardinality = cardinality;
		m_numElems = numElems;
		m_ownData = true;
		m_data = NULL;
		if ( size() > 0 ) {
			m_data = new _T[size()];
			if ( !m_data )
				throw TSystemException(__FILE__, __LINE__);
		}
	}

	//! Copy constructor, using the assignment operator to copy data
	/*! \param vd TNamedVector to copy from
	 */
	inline TNamedVector(const TNamedVector<_T> &vec) {
		m_ownData = false;
		*this = vec;
	}

	//! Destructor, remove own data
	inline ~TNamedVector() {
		if ( m_ownData && m_data )
			delete[] m_data;
	}

	//! Clone an instance of TNamedVector
	/*! \return A clone of *this
	 */
	inline TNamedVector<_T> &duplicate() const {
		TNamedVector<_T> *vd = new TNamedVector<_T>(*this);
		if ( !vd )
			throw TSystemException(__FILE__, __LINE__);
		return *vd;
	}

	//! Assignment
	/*! memcpy() is used for own data, if foreign data (!m_ownData) only the pointer is copied.
	 *  \param vd TNamedVector to copy from
	 *  \return return A reference to *this
	 */
	inline TNamedVector<_T> &operator=(const TNamedVector<_T> &vd) {
		if ( this == &vd )
			return *this;
		if ( m_ownData && m_data )
			delete[] m_data;

		m_components = vd.m_components;
		m_cardinality = vd.m_cardinality;
		m_numElems = vd.m_numElems;
		m_name = vd.m_name;
		m_ownData = vd.m_ownData;

		if ( !m_ownData ) {
			m_data = vd.m_data;
		} else {
			m_data = NULL;
			if ( size() > 0 ) {
				m_data = new _T[size()];
				if ( !m_data )
					throw TSystemException(__FILE__, __LINE__);
				memcpy(m_data, vd.m_data, sizeof(_T)*size());
			}
		}
		return *this;
	}

	//! \return Gets the name of the array
	inline const char *name() const { return m_name.c_str(); }

	//! \return Gets the number of basic elements for one compound element (e.g. 3 for RtPoint)
	inline IndexType components() const { return m_components; }

	//! \return Gets the cardinality of an (array) element (e.g. 5 for RtPoint[5])
	inline IndexType cardinality() const { return m_cardinality; }

	//! \return Gets the number of elements of this array
	inline IndexType numElems() const { return m_numElems; }

	//! \return Gets the number of basic elements
	inline IndexType size() const {return m_components*m_cardinality*m_numElems;}

	//! \return Gets a pointer to the first element
	inline _T *begin() const { return m_data; }

	//! \return Gets a pointer behind the last element
	inline _T *end() const { return m_data+size(); }

	//! Gets a pointer to  single element
	/*! \param i Index of the elemet
	 *  \return Pointer to the element with the index i
     */
	inline _T &operator[](IndexType i) const {
#       ifdef _DEBUG
			if ( i < 0 || i >= size() ) {
				throw TSystemException(TSystemException::m_errIndexOverflow(), __FILE__, __LINE__);
			}
#       endif
		return m_data[i];
	}

	//! Prints contents to a stream.
	/*! \param o Stream to print on.
	 *  \param pre String used as first output for each line.
	 */
	inline void dump(std::ostream &o, const std::string &pre) const {
		o << pre << "TNamedVector:" << std::endl;
		o << pre << "   m_name: \"" << m_name << "\", " << std::endl;
		o << pre << "   m_components: " << m_components << ", m_cardinality: " << m_cardinality << ", m_numElems: " << m_numElems << std::endl;
		o << pre << "   m_ownData: " << (m_ownData ? "true" : "false") << ", m_data: 0x" << m_data << std::endl;

		IndexType i, j, k;
		_T *ptr = m_data;
		for ( i = 0; i < m_numElems; ++i ) {
			o << pre << "   [ ";
			for ( j = 0; j < m_cardinality; ++j ) {
				for ( k = 0; k < m_components; ++k, ++ptr ) {
					if ( j > 0 || k > 0 )
						o << ", ";
					o << *ptr;
				}
				if ( j != m_cardinality-1 )
					o << " | ";
			}
			o << " ]";
			if ( i != m_numElems-1 )
				o << ",";
			o << std::endl;
		}
	}

	//! Copies the content of a vector
	/*! \param iter Iterator, size of vector must be >= size of this instance
	 */
	inline void copyFrom(const _T *iter) {
		if ( size() > 0 ) {
			memcpy(m_data, iter, sizeof(_T)*size());
		}
	}
}; // TNamedVector


/*
template<typename _T> inline atype *findNamedVector(const std::deque<TNamedVector<atype> > &deq, const char *name) {
	if ( !name ) name = "";
	typename std::deque<TNamedVector<atype> >::const_iterator i = deq.begin();
	for ( ; i != deq.end(); i++ ) {
		if ( !strcmp((*i).name(), name) )
			return (*i).begin();
	}
	return NULL;
}
*/

//! Prints contents of v to a stream.
/*! \param v Value to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
template<class _T, class _S, class _P> inline void dump(const _T &v, _S &o, const _P &pre) {
	o << pre << v;
}


//! Prints contents of the TNamedVector vec to a stream by calling its dump member function
/*! \param vec TNamedVector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
template<class _T, class _S, class _P> inline void dump(const TNamedVector<_T> &c, _S &o, const _P &pre) {
	c.dump(o, pre);
}


//  Prints contents of the TNamedVector vec to a stream by calling its dump member function
/*  Deprecated, implemented because VC6 couldn't evaluate the templates.
 *  \param vec TNamedVector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dumpV(const TNamedVector<RtFloat> &vec, std::ostream &o, const std::string &pre) {
	vec.dump(o, pre);
}


//! Prints contents of the std::vector c to a stream, seperated by comma.
/*! Calls the template function dump vor each element
 *  \param c std::vector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
template <class _T, class _S, class _P> inline void dump(const std::vector<_T >&c, _S &o, const _P &pre) {
	o << pre << "container, size:" << (IndexType)c.size() << std::endl;
	typename std::vector<_T >::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		if ( i == c.begin() )
			dump(*i, o, pre);
		else
			dump(*i, o, ", ");
	}
	o << std::endl;
}


//! Prints contents of the std::deque c to a stream, seperated by comma.
/*! Calls the template function dump vor each element
 *  \param c std::vector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
template <class _T, class _S, class _P> inline void dump(const std::deque<_T >&c, _S &o, const _P &pre) {
	o << pre << "container, size:" << (IndexType)c.size() << std::endl;
	typename std::vector<_T >::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		if ( i == c.begin() )
			dump(*i, o, pre);
		else
			dump(*i, o, ", ");
	}
	o << std::endl;
}

//  Prints contents of the std::vector vec to a stream, seperated by comma.
/*  Deprecated, implemented because VC6 couldn't evaluate the templates.
 *  Calls the template function dump vor each element
 *  \param vec std::vector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dumpV(const std::vector<long> &c, std::ostream &o, const std::string &pre) {
	o << pre << "std::vector, size:" << (IndexType)c.size() << std::endl;
	std::vector<long>::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		if ( i == c.begin() )
			dump(*i, o, pre);
		else
			dump(*i, o, ", ");
	}
	o << std::endl;
}

inline void dumpV(const std::deque<RtFloat> &c, std::ostream &o, const std::string &pre) {
	o << pre << "std::deque, size:" << (IndexType)c.size() << std::endl;
	std::deque<RtFloat>::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		if ( i == c.begin() )
			dump(*i, o, pre);
		else
			dump(*i, o, ", ");
	}
	o << std::endl;
}

inline void dumpV(RtFloat val, std::ostream &o, const std::string &pre) {
	o << pre << val;
}

//  Prints contents of the std::deque deq to a stream, seperated by comma.
/*  Calls the template function dump vor each element
 *  \param dew std::deque to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 *-/
template<typename atype> inline void dump(const std::deque<atype> &deq, std::ostream &o, const std::string &pre) {
	o << pre << "std::deque, size:" << vec.size() << std::endl;
	typename std::deque<atype>::const_iterator i = vec.begin();
	for ( ; i != vec.end(); i++ ) {
		if ( i == vec.begin() )
			dump(*i, o, pre);
		else
			dump(*i, o, ", ");
	}
	o << std::endl;
}
*/

//! Prints contents of the std::vector vec to a stream, each element in a new line.
/*! Calls the template function dump vor each element
 *  \param vec std::vector to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
template<class _C> inline void dumpLn(const _C &c, std::ostream &o, const std::string &pre) {
	typename _C::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		dump(*i, o, pre);
		o << std::endl;
	}
}

/*
//  Prints contents of the std::deque deq to a stream, each element in a new line.
/-*  Calls the template function dump vor each element
 *  \param dew std::deque to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 *-/
template<typename atype> inline void dumpLn(const std::deque<atype> &deq, std::ostream &o, const std::string pre) {
	o << pre << "std::deque, size:" << (unsigned long)deq.size() << std::endl;
	typename std::deque<atype>::const_iterator i = deq.begin();
	for ( ; i != deq.end(); i++ ) {
		dump(*i, o, pre);
		o << std::endl;
	}
}
*/
//! Prints contents of the std::deque deq to a stream, each element in a new line.
/*! Deprecated, implemented because VC6 couldn't evaluate the templates.
 *  Calls the template function dump vor each element
 *  \param dew std::deque to print to a stream
 *  \param o Stream to print on.
 *  \param pre String used as first output for each line.
 */
inline void dumpLnV(const std::deque<TNamedVector<RtFloat> > &deq, std::ostream &o, const std::string pre) {
	o << pre << "std::deque, size:" << (unsigned long)deq.size() << std::endl;
	std::deque<TNamedVector<RtFloat> >::const_iterator i = deq.begin();
	for ( ; i != deq.end(); i++ ) {
		dumpV(*i, o, pre);
		o << std::endl;
	}
}


//! Finds an element Name in Container c. Does linear serach.
/*!
 *  \param name Element name to search
  * \param c The vector
 */
template<class _T, class _C> inline _T *findVector(const char *name, const _C &c) {
	if ( !name ) name = "";
	typename _C::const_iterator i = c.begin();
	for ( ; i != c.end(); i++ ) {
		if ( !strcmp((*i).name(), name) )
			return (*i).begin();
	}
	return NULL;
}


//! Finds an element with then 'name()' == name in a deque
/*! Calls the elements member function name() to compare with parameter name
 *  using linear search. E.g. _T is TNamedVector,
 *  \param name Name to search for
 *  \param containerto search.
 *  \return Pointer to the element found, NULL if nbothing is found.
 */
template <class _T, class _C> inline _T *findElement(const char *name, const _C &c) {
	if ( !name ) name = "";

	IndexType pos = 0;
	typename _C::const_iterator i = c.begin();
	for ( ; i != c.end(); i++, ++pos )
		if ( !strcmp(i->name(), name) )
			return (_T *)&c[pos];

	return NULL;
}


inline TNamedVector<RtFloat> *findElementVector(const char *name, std::deque<TNamedVector<RtFloat> > &c) {
	if ( !name ) name = "";

	IndexType pos = 0;
	std::deque<TNamedVector<RtFloat> >::const_iterator i = c.begin();
	for ( ; i != c.end(); i++, ++pos )
		if ( !strcmp(i->name(), name) )
			return (TNamedVector<RtFloat> *)&c[pos];

	return NULL;
}

inline RtFloat *findElementPtr(const char *name, std::deque<TNamedVector<RtFloat> > &c) {
	if ( !name ) name = "";

	IndexType pos = 0;
	std::deque<TNamedVector<RtFloat> >::const_iterator i = c.begin();
	for ( ; i != c.end(); i++, ++pos )
		if ( !strcmp(i->name(), name) )
			return i->begin();

	return NULL;
}

#endif // _RICPP_TOOLS_HELPERS_H
