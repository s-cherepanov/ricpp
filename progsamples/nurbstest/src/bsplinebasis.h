/*
 *  bsplinebasis.h
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#ifndef _RICPP_BSPLINEBASIS_H
#define _RICPP_BSPLINEBASIS_H

#include "utility.h"
#include <vector>
#include <assert.h>

namespace RiCPP {

////////////////////////////////////////////////////////////////////////////////
/** @brief CBSplineBasis is a class to calculate a bspline basis and its first
 *  derivate using the Cox-de Boor recurrence formular.
 */
class CBSplineBasis {
private:
    /** @{
     * @name Input values
     * @brief Copied by reset() or constructor.
     */
    RtInt m_ncpts;                //!< Number of control points (control points are not stored in instances of this class)
    RtInt m_order;                //!< Order of the polynom (degree = k-1), 0 < k <= m_ncpts
    std::vector<RtFloat> m_knots; //!< Knot vector, size = m_ncpts+m_order, knots[i] <= knots[i+1]
    RtFloat m_tmin;               //!< Minimum value for parameter t, tmin >= x[order-1] && tmin < tmax
    RtFloat m_tmax;               //!< Maximum value for parameter t, tmax <= x[ncpts] && tmax > tmin
    IndexType m_tess;             //!< Tesselation of the parameter, i.e. the number of intervals for the parameter range
    /** @}
     */

    /** @{
     * @name Results
     * @brief Calculated by reset() or constructor.
     */
    RtInt m_segments;                  //!< Number of segments of the bspline: 1 + m_ncpts - m_order
    std::vector<RtInt> m_valOffs;      //!< Offset (in m_tVals) per segment, size of vector = m_segments
    std::vector<RtInt> m_valCnts;      //!< Number of parameters (in m_tVals) per segment, size of vector = m_segments, max(m_valCnts[i]) = m_tess+1
    std::vector<RtFloat> m_tVals;      //!< Parameters used to tesselate the spline, parameter range for a segment s is m_tVals[m_valOffs[s]] to m_tVals[m_valOffs[s]+m_valCnts[s]] (excluding), interval can be empty
    std::vector<RtFloat> m_basis;      //!< bspline basis for parameters m_tVals (number of values per parameter = order, only the influencing basis values are stored)
    std::vector<RtFloat> m_basisDeriv; //!< bspline basis first derivate for parameters m_tVals (number of values per parameter = order, only the influencing basis values are stored)
    /** @}
     */

    /** Calculates the bspline basis a parameter value t in a span
     *  and returns the basis in N[span-order...span] and the first derivate in Nd[span-order...span].
     *  Algorithm based on the Cox-de Boor recurrence formular.
     *
     *  assert( !(k > n) );
     *  assert( !(x.size() < (std::vector<RtFloat>::size_type)size+1) );
     *  assert( !(N.size() < (std::vector<RtFloat>::size_type)size) );
     *  assert( !(Nd.size() < (std::vector<RtFloat>::size_type)size) );
     *  assert( !(t > tmax) );
     */
    void bsplineBasisDerivate(RtFloat t, RtInt span,
                              std::vector<RtFloat> &N, std::vector<RtFloat> &Nd);

    void sortKnots();
    void clearResults();
    void insertKnots(const std::vector<RtFloat> &theKnots);
    void insertKnots(const std::vector<RtFloat> &theKnots, IndexType theKnotOffs);
    void insertVals(RtInt theNCPts, RtInt theOrder,
                    RtFloat theTMin, RtFloat theTMax, RtInt theTess);

    //! Checks whether the member variables are valid for bspline-calculation, does basic validation.
    void validate();

    //! Calc all basis values, using Cox-de Boor recurrence function
    void calc();

public:
    /** @brief Standard constructor
     *
     * Simply clears the members, reset() can be used to load the values.
     */
    inline CBSplineBasis()
    {
        m_ncpts = 0;
        m_order = 0;
        m_tmin = (RtFloat)0;
        m_tmax = (RtFloat)0;
        m_tess = 0;
        m_segments = 0;
    }

    //! Constructor to initialize the members, @see reset()
    inline CBSplineBasis(RtInt theNCPts, RtInt theOrder,
                         const std::vector<RtFloat> &theKnots,
                         RtFloat theTMin, RtFloat theTMax, RtInt theTess)
    {
        reset(theNCPts, theOrder, theKnots, 0, theTMin, theTMax, theTess);
    }

    //! Constructor to initialize the members, @see reset()
    inline CBSplineBasis(RtInt theNCPts, RtInt theOrder,
                         const std::vector<RtFloat> &theKnots, IndexType theKnotOffs,
                         RtFloat theTMin, RtFloat theTMax, RtInt theTess)
    {
        reset(theNCPts, theOrder, theKnots, theKnotOffs, theTMin, theTMax, theTess);
    }

    //! copy constructor
    inline CBSplineBasis(const CBSplineBasis &sp)
    {
        *this = sp;
    }

    //! Clone *this
    /** @return A clone of *this
     */
    inline CBSplineBasis *duplicate() const
    {
        return new CBSplineBasis(*this);
    }

    //! Assignment
    CBSplineBasis &operator=(const CBSplineBasis &sp);

    //! Sets a new basis, calls calc()
    void reset(RtInt theNcpts, RtInt theOrder,
               const std::vector<RtFloat> &theKnots,
               RtFloat theTMin, RtFloat theTMax, RtInt theTess);

    //  Sets a new basis, calls calc()
    /*  Used for one segement uses an offset for the knotvector and
     *  ncpts and order for the size of the knot vector.
     */
    void reset(RtInt theNcpts, RtInt theOrder,
               const std::vector<RtFloat> &theKnots, IndexType theKnotOffs,
               RtFloat theTMin, RtFloat theTMax, RtInt theTess);


    //! Blend using control points (homogene 2D), members have to be valid
    RtInt nuBlendP2W(const std::vector<RtFloat> &source,
                     RtInt offs,
                     RtInt seg,
                     std::vector<RtFloat> &results,
                     RtInt resultOffs) const;

    RtInt nuBlendP2W(const std::vector<RtFloat> &source,
                     RtInt offs,
                     std::vector<RtFloat> &results,
                     RtInt resultOffs) const;

    /** @{
     */
    inline RtInt nCPts() const {
        return m_ncpts;
    }
    inline RtInt order() const {
        return m_order;
    }
    inline RtFloat tMin() const {
        return m_tmin;
    }
    inline RtFloat tMax() const {
        return m_tmax;
    }
    inline IndexType tess() const {
        return m_tess;
    }
    inline const std::vector<RtFloat> &knots() const {
        return m_knots;
    }
    /** @}
     */

    /** @{
     */
    bool empty() const;
    inline IndexType knotSize() const {
        return static_cast<IndexType>(m_ncpts+m_order);
    }
    inline RtInt numSegments() const {
        return m_segments;
    }
    inline RtInt span(RtInt seg) const {
        return seg + order() - 1;
    }
    inline RtInt offsParameters(RtInt seg) const {
        return valOffs()[span(seg)];
    }
    inline RtInt numParameters(RtInt seg) const {
        return valCnts()[span(seg)];
    }

    inline RtInt basisIdx(RtInt seg, RtInt paramIdx, RtInt orderIdx) const
    {
        assert(seg >= 0 && seg < numSegments());
        assert(paramIdx >= 0 && paramIdx < numParameters(seg));
        assert(orderIdx >= 0 && orderIdx < order());
        return (offsParameters(seg)+paramIdx) * order() + orderIdx;
    }

    inline RtFloat basisElem(RtInt seg, RtInt paramIdx, RtInt orderIdx) const
    {
        assert((size_t)basisIdx(seg, paramIdx, orderIdx) < basis().size());
        return basis()[basisIdx(seg, paramIdx, orderIdx)];
    }

    inline RtFloat basisDerivElem(RtInt seg, RtInt paramIdx, RtInt orderIdx) const
    {
        assert((size_t)basisIdx(seg, paramIdx, orderIdx) < basisDeriv().size());
        return basisDeriv()[basisIdx(seg, paramIdx, orderIdx)];
    }

    inline RtFloat parameter(RtInt seg, RtInt paramIdx) const
    {
        assert(seg >= 0 && seg < numSegments());
        assert(paramIdx >= 0 && paramIdx < numParameters(seg));
        assert((size_t)(offsParameters(seg)+paramIdx) < tVals().size());
        return tVals()[offsParameters(seg)+paramIdx];
    }

    inline const std::vector<RtInt>   &valOffs() const {
        return m_valOffs;
    }
    inline const std::vector<RtInt>   &valCnts() const {
        return m_valCnts;
    }
    inline const std::vector<RtFloat> &tVals() const {
        return m_tVals;
    }
    inline const std::vector<RtFloat> &basis() const {
        return m_basis;
    }
    inline const std::vector<RtFloat> &basisDeriv() const {
        return m_basisDeriv;
    }
    /** @}
     */
    void write(std::ostream &out, const std::string &prefix) const;
}; // CBSplineBasis

} // namespace RiCPP

#endif // _RICPP_BSPLINEBASIS_H
