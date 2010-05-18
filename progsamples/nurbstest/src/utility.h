/*
 *  utility.h
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#ifndef _RICPP_UTILITY_H
#define _RICPP_UTILITY_H

#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

namespace RiCPP {

typedef unsigned long IndexType; //!< Type of various indicees
typedef int RtInt;               //!< Integer number of the RI
typedef float RtFloat;           //!< floating point number of the RI

/** @brief Gets the epsilon limit value
 *
 * eps is the smallest positive value eps+a != a
 *
 *  @return eps
 */
template <typename type> inline type eps() {
    return std::numeric_limits<type>::epsilon();
}

/** @brief Validates min <= max, swaps parameters if min > max
 *  @param min Minimum
 *  @param max Maximum
 *  @return false, numbers are swapped
 */
template<typename type> inline bool validateMinMax(type &min, type &max)
{
    if ( min > max ) {
        using std::swap;
        swap(min,max);
        return false;
    }
    return true;
}

template <typename type> inline bool inOpenInterval(type f, type min, type max) {
    validateMinMax(min, max);
    return f > min && f < max;
}
template <typename type> inline bool inClosedInterval(type f, type min, type max) {
    validateMinMax(min, max);
    return f >= min && f <= max;
}
template <typename type> inline bool inLeftClosedInterval(type f, type min, type max) {
    validateMinMax(min, max);
    return f >= min && f < max;
}
template <typename type> inline bool inRightClosedInterval(type f, type min, type max) {
    validateMinMax(min, max);
    return f > min && f <= max;
}

template <typename type> inline bool positive(type f) {
    return f >= 0;
}
template <typename type> inline bool negative(type f) {
    return f < 0;
}
template <typename type> inline bool nearlyZero(type f) {
    return inOpenInterval(f, -eps<type>(), eps<type>());
}
template <typename type> inline bool nearlyEqual(type f1, type f2) {
    return nearlyZero(f1-f2);
}

/** @brief Get the sign of a value
 *  @param f The value to test
 *  @return -1 if f is negative, 1 if positive, 0 if zero
 */
template <typename type> inline type sign(type f)
{
    return f < 0 ?
           static_cast<type>(-1) :
           (nearlyZero(f) ? static_cast<type>(0) : static_cast<type>(1));
}

/** @brief Clamps a value the values boundmin, boundmax.
 *
 *  The bounds are exchanged if @a boundmin > @a boundmax.
 *
 *  @param val value to clamp.
 *  @param boundmin Size of the string buffer.
 *  @param boundmax Number to convert
 *  @return The clambed value (>= @a boundmin and <= @a boundmax)
 */
template<typename type> inline type clamp(type val, type boundmin, type boundmax)
{
    validateMinMax(boundmin, boundmax);
    if ( val < boundmin )
        return boundmin;
    if ( val > boundmax )
        return boundmax;

    return val;
}

// ===================================================================================

/**
 */
template <typename type> inline type nuNumSegs(type ncpts, type order)
{
    return 1 + ncpts - order;
}

/**
 */
template <typename type> inline type nuNumKnots(type ncpts, type order)
{
    return ncpts + order;
}

// ===================================================================================

template<typename type> inline void writeVector(type &v,
        const std::string &name,
        std::ostream &out,
        const std::string &prefix,
        int components=1,
        bool onePerLine=false)
{
    out << prefix;
    if ( !name.empty() ) {
        out << name << " - ";
    }
    out << "Size: " << v.size();

    if ( components > 1 ) {
        out << " (" << v.size()/components << ")";
    }

    out << std::endl;

    for ( int i=0; i<v.size()/components; ++i ) {
        if ( i==0 || onePerLine ) {
            out << prefix + "    ";
        }
        if ( onePerLine ) {
            out << i << ". ";
        }
        if ( !onePerLine && i > 0 ) {
            out << ", ";
        }
        if ( components > 1 ) {
            out << "(";
        }
        for ( int j=0; j<components; ++j ) {
            if ( j > 0 ) {
                out << ", ";
            }
            out << v[i*components+j];
        }
        if ( components > 1 ) {
            out << ")";
        }
        if ( onePerLine ) {
            out << std::endl;
        }
    }

    if ( !onePerLine ) {
        out << std::endl;
    }
}

} // namespace RiCPP

#endif // _RICPP_UTILITY_H
