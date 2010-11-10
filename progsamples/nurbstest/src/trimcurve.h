/*
 *  trimcurve.h
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#ifndef _RICPP_TRIMCURVE_H
#define _RICPP_TRIMCURVE_H

#include "bsplinebasis.h"

namespace RiCPP {

/** @brief Contains one trim curve of the Rendertman Interface, consisting of a number of loops
 */
class CTrimCurveData {

    /** @brief Array sizes of a trim curve.
     */
    struct CTrimCurveDataInfo {
        RtInt m_nloops; ///< Number of closed loops.

        int m_total;	///< Total number of curves.
        int m_npoints;	///< Total number of control points.
        int m_nknots;	///< Total number of knots.

        /** @brief Standard constructor, initializes with 0 loops.
         */
        inline CTrimCurveDataInfo() {
            m_nloops = 0;
            m_total = 0;
            m_npoints = 0;
            m_nknots = 0;
        }

        /** @brief Constructor, fills instances with the values of a trim curve interface call.
         *
         *  @param nloops   Number if loops (curves with holes).
         *  @param ncurves  Curves of the loops.
         *  @param order    Order of the curves.
         *  @param knot     Knot vectors of the curves.
         *  @param amin     Minimum parametric values of the curves.
         *  @param amax     Maximum parametric values of the curves.
         *  @param n        Number of the homogene u,v,w coordinates.
         *  @param u        u coordinates of the curves.
         *  @param v        v coordinates of the curves.
         *  @param w        w coordinates of the curves.
         */
        inline CTrimCurveDataInfo(RtInt nloops, const RtInt *ncurves,
                                  const RtInt *order, const RtFloat *knot,
                                  const RtFloat *amin, const RtFloat *amax,
                                  const RtInt *n, const RtFloat *u, const RtFloat *v, const RtFloat *w)
        {
            trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
        }

        /** @brief Fills instances with the values of a trim curve interface call.
         *
         *  @param nloops   Number if loops (curves with holes).
         *  @param ncurves  Curves of the loops.
         *  @param order    Order of the curves.
         *  @param knot     Knot vectors of the curves.
         *  @param amin     Minimum parameter of a curve.
         *  @param amax     Maximum parameter of a curve.
         *  @param n        Number of the homogene u,v,w coordinates.
         *  @param u        u coordinates of the curves.
         *  @param v        v coordinates of the curves.
         *  @param w        w coordinates of the curves.
         */
        void trimCurve(RtInt nloops, const RtInt ncurves[], const RtInt order[],
                       const RtFloat knot[], const RtFloat amin[], const RtFloat amax[],
                       const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[]);

        void write(std::ostream &out, const std::string &prefix) const;
    }; // CTrimCurveDataInfo

    CTrimCurveDataInfo   m_data;        ///< Sizes.

    int m_nsegments;	                ///< Total number of segments (sum(m_segments))
    std::vector<RtInt>   m_segments;    ///< Number of segments per curve (1 + number of control points - order)

    std::vector<RtInt>   m_nCurves;     ///< Number of curves per loops
    std::vector<RtInt>   m_nCurveOffs;  ///< Number of offsets to the curves of the loops

    std::vector<RtInt>   m_order;       ///< Order of the curves.
    std::vector<RtFloat> m_knots;       ///< Knot vectors of the curves.
    std::vector<RtFloat> m_min;         ///< Minimum parametric values of the curves.
    std::vector<RtFloat> m_max;         ///< Maximum parametric values of the curves.
    std::vector<RtInt>   m_n;           ///< Number of coordinates.
    std::vector<RtFloat> m_u;           ///< u coordinates of the curves.
    std::vector<RtFloat> m_v;           ///< v coordinates of the curves.
    std::vector<RtFloat> m_w;           ///< w coordinates of the curves.
    std::vector<RtFloat> m_points;      ///< Points filled with (u[0], v[0], w[0], ... ).

    /////////////////////////////////////////////////////////////////////////////////////////
    // Member functions
public:
    /** @brief Standard constructor.
     */
    inline CTrimCurveData() {
        m_nsegments = 0;
    }

    /** @brief Constructor, fills instances with the values of a trim curve interface call.
     *
     *  @param nloops   Number if loops (curves with holes).
     *  @param ncurves  Curves of the loops.
     *  @param order    Order of the curves.
     *  @param knot     Knot vectors of the curves.
     *  @param amin     Minimum parametric values of the curves.
     *  @param amax     Maximum parametric values of the curves.
     *  @param n        Number of the homogene u,v,w coordinates.
     *  @param u        u coordinates of the curves.
     *  @param v        v coordinates of the curves.
     *  @param w        w coordinates of the curves.
     */
    inline CTrimCurveData(RtInt nloops, const RtInt ncurves[],
                          const RtInt order[], const RtFloat knot[],
                          const RtFloat amin[], const RtFloat amax[],
                          const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[])
    {
        trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
    }

    /** @brief Copy constructor.
     *
     *  @param curve Used to initialize copy.
     */
    inline CTrimCurveData(const CTrimCurveData &curve)
    {
        *this = curve;
    }

    /** @brief Destructor.
     */
    inline ~CTrimCurveData()
    {
        releaseAll();
    }


    /** @brief Duplication.
     *  @return Clone of this.
     */
    inline CTrimCurveData *duplicate() const
    {
        return new CTrimCurveData(*this);
    }

    /** @brief Assignment of another curve.
     *
     *  @param curve The curve to assign.
     *  @return *this.
     */
    CTrimCurveData &operator=(const CTrimCurveData &curve);

    /** @brief Compares to objects by components.
     *
     *  @param curve The curve to compare to.
     *  @return true, all components of *this and @a curve are equal.
     */
    bool operator==(const CTrimCurveData &curve) const;

    /** @brief Clears instance.
     *
     *  Zero loops menas, there is no trim curve.
     */
    inline void releaseAll()
    {
        m_data.m_nloops = 0;
    }

    /** @brief Fills instances with the values of a trim curve interface call.
     *
     *  @param nloops   Number if loops (curves with holes).
     *  @param ncurves  Curves of the loops.
     *  @param order    Order of the curves.
     *  @param knot     Knot vectors of the curves.
     *  @param amin     Minimum parameter of a curve.
     *  @param amax     Maximum parameter of a curve.
     *  @param n        Number of the homogene u,v,w coordinates.
     *  @param u        u coordinates of the curves.
     *  @param v        v coordinates of the curves.
     *  @param w        w coordinates of the curves.
     */
    void trimCurve(RtInt nloops, const RtInt ncurves[], const RtInt order[],
                   const RtFloat knot[], const RtFloat amin[], const RtFloat amax[],
                   const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[]);

    inline int nSegments() const {
        return m_nsegments;
    }
    inline int segments(int curve) {
        return m_segments[curve];
    }

    /** Gets the number of close loops.
     *  @return Number of loops.
     */
    inline RtInt loops() const {
        return m_data.m_nloops;
    }

    /** Gets the total number of curves.
     *  @return Total number of curves.
     */
    inline int curves() const {
        return m_data.m_total;
    }

    /** Gets the number of curves for a loop.
     *  @param loop The index of the loop (0 <= loop < loops())
     *  @return Total number of curves.
     */
    inline int curves(RtInt loop) const {
        return m_nCurves[loop];
    }

    /** Gets the offset to the curves of a loop.
     *  @param loop The index of the loop (0 <= loop < loops())
     *  @return Index of the first curve of a loop
     */
    inline int curveOffs(RtInt loop) const {
        return m_nCurveOffs[loop];
    }

    inline RtInt   order(RtInt curve) const {
        return m_order[curve];
    }
    inline RtFloat minKnot(RtInt curve) const {
        return m_min[curve];
    }
    inline RtFloat maxKnot(RtInt curve) const {
        return m_max[curve];
    }

    inline int startKnot(RtInt curve) const {
        int start = 0;
        for ( RtInt i = 0; i < curve; i++ ) {
            start += nKnots(i);
        }
        return start;
    }

    inline int endKnot(RtInt curve) const {
        return startKnot(curve) + nKnots(curve);
    }

    inline int nKnots(RtInt curve) const {
        return nuNumKnots(nPoints(curve), order(curve));
    }

    inline int startPoint(RtInt curve) const {
        int start = 0;
        for ( RtInt i = 0; i < curve; i++ ) {
            start += nPoints(i);
        }
        return start;
    }

    inline int endPoint(RtInt curve) const {
        return startPoint(curve) + nPoints(curve);
    }

    inline int nPoints(RtInt curve) const {
        return m_n[curve];
    }

    inline const std::vector<RtFloat> &knotVect() const {
        return m_knots;
    }
    inline const std::vector<RtFloat> &uVect() const {
        return m_u;
    }
    inline const std::vector<RtFloat> &vVect() const {
        return m_v;
    }
    inline const std::vector<RtFloat> &wVect() const {
        return m_w;
    }
    inline const std::vector<RtFloat> &pointsVect() const {
        return m_points;
    }

    /** Gets the total number of control points.
     *  @return Total number of control points.
     */
    inline int controlPoints() const {
        return m_data.m_npoints;
    }

    /** Gets the total number of knots.
     *  @return Total number of knots.
     */
    inline int knots() const {
        return m_data.m_nknots;
    }

    void write(std::ostream &out, const std::string &prefix) const;
}; // CTrimCurveData


/** Coordinates of a tesselated trim loop
 */
class CTrimCoords {
    std::vector<RtFloat> m_coordinates; ///< u, v coordinate pairs, size is even, tesselation is more as or equal to the number of pairs - 1
    // bool m_cw;
public:
    inline void appendCoordinates(const std::vector<RtFloat> &uv) {
        m_coordinates.reserve(m_coordinates.size() + uv.size());
        m_coordinates.insert(m_coordinates.end(), uv.begin(), uv.end());
    }
    inline void clear() {
        m_coordinates.clear();
    }
    const std::vector<RtFloat> &coordinates() const {
        return m_coordinates;
    }
    const IndexType size() const {
        return m_coordinates.size()/2;
    }
    RtFloat u(IndexType idx) const
    {
        assert(idx*2<m_coordinates.size());
        return m_coordinates[idx*2];
    }
    RtFloat v(IndexType idx) const
    {
        assert(idx*2+1<m_coordinates.size());
        return m_coordinates[idx*2+1];
    }
    void getUV(IndexType idx, RtFloat &u, RtFloat &v) const
    {
        IndexType myIdx = idx*2;
        assert(myIdx+1<m_coordinates.size());
        u = m_coordinates[myIdx];
        v = m_coordinates[myIdx++];
    }
    inline void write(std::ostream &out, const std::string &prefix) const
    {
        out << prefix << "Type: CTrimCoords" << std::endl;
        writeVector(m_coordinates, "m_coordinates", out, prefix, 2, true);
    }
}; // CTrimCoords

/** @brief Writes coordinates to a stream
 */
inline std::ostream &operator<<(std::ostream &out, const CTrimCoords &object) {
    object.write(out, "");
    return out;
}

/**
 */
class CTrimCurve {
    RtFloat m_min;                   ///< Minimum knot clip value.
    RtFloat m_max;                   ///< Maximum knot clip value.
    RtInt m_order;                   ///< Order of the curve.
    std::vector<RtFloat> m_knots;    ///< Knot vector of the curve.
    std::vector<RtFloat> m_uvw;      ///< (u, v, w) coordinates of the curve.
    CBSplineBasis m_basis;           ///< BSpline basis of the curve
    IndexType m_curTess;             ///< Current tesselation
    std::vector<RtFloat> m_coordinates; ///< u, v coordinate pairs, size is even, values are calculated tesselate()
public:
    inline CTrimCurve()
    {
        m_curTess = 0;
        m_min = m_max = 0;
        m_order = 0;
    }

    inline const std::vector<RtFloat> &coordinates() const
    {
        return m_coordinates;
    }

    void insertCurve(int loopIdx, int curveIdx, const CTrimCurveData &trimCurveData);
    const std::vector<RtFloat> &tesselate(IndexType tesselation);
    void write(std::ostream &out, const std::string &prefix) const;
}; // CTrimCurve


/**
 */
class CTrimLoop {
    std::vector<CTrimCurve> m_trimCurves;
public:
    inline CTrimLoop() {}

    inline void insertCurves(int loopIdx, const CTrimCurveData &trimCurveData)
    {
        m_trimCurves.resize(trimCurveData.curves(loopIdx));
        for ( IndexType i = 0; i < m_trimCurves.size(); ++i ) {
            m_trimCurves[i].insertCurve(loopIdx, i, trimCurveData);
        }
    }

    inline void tesselate(IndexType tesselation)
    {
        for ( IndexType i = 0; i < m_trimCurves.size(); ++i ) {
            m_trimCurves[i].tesselate(tesselation);
        }
    }

    inline void tesselate(CTrimCoords &coords, IndexType tesselation)
    {
        for ( IndexType i = 0; i < m_trimCurves.size(); ++i ) {
            coords.appendCoordinates(m_trimCurves[i].tesselate(tesselation));
        }
    }

    inline void getTrimCoords(CTrimCoords &coords) const
    {
        coords.clear();
        for ( IndexType i = 0; i < m_trimCurves.size(); ++i ) {
            coords.appendCoordinates(m_trimCurves[i].coordinates());
        }
    }

    inline void write(std::ostream &out, const std::string &prefix) const
    {
        out << prefix << "Type: CTrimLoop" <<  std::endl;
        out << prefix << "m_trimCurves - Size: " <<  m_trimCurves.size() << std::endl;
        for ( IndexType i = 0; i < m_trimCurves.size(); ++i ) {
            out << prefix << "m_trimCurves[" <<  i << "]" << std::endl;
            m_trimCurves[i].write(out, prefix + "    ");
        }
    }
}; // CTrimLoop


/**
 */
class CTrimPath {
    std::vector<CTrimLoop> m_trimLoops;

public:
    inline CTrimPath() { }

    inline CTrimPath(RtInt nloops, const RtInt ncurves[],
                     const RtInt order[], const RtFloat knot[],
                     const RtFloat amin[], const RtFloat amax[],
                     const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[])
    {
        trimCurve(nloops, ncurves,
                  order, knot,
                  amin, amax,
                  n, u, v, w);
    }

    inline CTrimPath(const CTrimCurveData &trimCurveData)
    {
        trimCurve(trimCurveData);
    }

    void trimCurve(const CTrimCurveData &trimCurveData);
    inline void trimCurve(RtInt nloops, const RtInt ncurves[],
                          const RtInt order[], const RtFloat knot[],
                          const RtFloat amin[], const RtFloat amax[],
                          const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[])
    {
        CTrimCurveData trimCurveData(nloops, ncurves,
                                     order, knot,
                                     amin, amax,
                                     n, u, v, w);
        trimCurve(trimCurveData);
    }

    void tesselate(IndexType tesselation);
    void tesselate(std::vector<CTrimCoords> &trimLoopCoords, IndexType tesselation);
    void getTrimCoords(std::vector<CTrimCoords> &trimLoopCoords) const;
    void write(std::ostream &out, const std::string &prefix) const;
}; // CTrimPath
} // namespace RiCPP

#endif // _RICPP_TRIMCURVE_H
