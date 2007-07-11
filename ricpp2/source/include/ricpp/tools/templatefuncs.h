#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#define _RICPP_TOOLS_TEMPLATEFUNCS_H

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

/** @brief Returns the maximum of two elements.
 *  @param a First element
 *  @param b Second element
 *  @return Maximum of a and b
 */
template<typename _TYPENAME>
typename _TYPENAME max(
	typename _TYPENAME a,
	typename _TYPENAME b)
{
	return a > b ? a : b;
}


/** @brief Returns the maximum of a vector.
 *  @param n The index type of the vector
 *  @param v The vector
 *  @return Maximum found in v
 */
template<typename _IT, typename _T> typename _T max(typename _IT n, const typename _T *v) {
	typename _T result = (typename _T)0;
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
template<class _T> typename _T min(typename _T a, typename _T b) {
	return a < b ? a : b;
}


//! Returns the minimum of a vector
/*! \param n The index type of the vector
 *  \param v The vector
 *  \return Minimum found in v
 */
template<typename _IT, typename _T> typename _T min(typename _IT n, const typename _T *v) {
	typename _T result = 0;
	if (n > 0) {
		result = v[--n];
		while ( n-- ) {
			if ( v[n] < result )
				result = v[n];
		}
	}
	return result;
}

/** @brief Returns the sum of the elements in a vector
 *  @param n The index type of the vector
 *  @param v The vector
 *  @return Sum of elements of v
 */
template<typename _IT, typename _T> typename _T sum(typename _IT n, typename _T *v) {
	typename _T result = 0;
	while ( n-- > 0 ) {
		result += v[n];
	}
	return result;
}

//! Linear interpolation
template<typename _C> typename _C lerp(typename _C u, typename _C minu, typename _C maxu) {
	return minu + u * (maxu - minu);
}

//! Bilinear interpolation
template<typename _C> typename _C bilerp(typename _C u, typename _C v, typename _C x1, typename _C x2, typename _C x3, typename _C x4) {
	return lerp(v, lerp(u, x1, x2), lerp(u, x3, x4));
}

#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H