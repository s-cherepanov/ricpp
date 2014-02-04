/*
 *  bsplinebasis.cpp
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#include "bsplinebasis.h"

using namespace RiCPP;

void CBSplineBasis::bsplineBasisDerivate(RtFloat t, RtInt span,
        std::vector<RtFloat> &N,
        std::vector<RtFloat> &Nd)
{
    const RtInt size  = m_ncpts + m_order - 1;

    // Init N
    N.assign(size, 0.0);
    N[span] = 1.0;

    // Calculate
    RtInt i, j;
    RtFloat temp;

    for ( j=1; j < m_order; ++j ) {
        for ( i=0; i < size-j; ++i ) {

            // Calc the first Derivate
            if ( j == 1 ) {
                temp = m_knots[i+j] - m_knots[i];
                if ( temp >= eps<RtFloat>() ) {
                    temp = N[i] / temp;
                }
                Nd[i] = temp;
                temp = m_knots[i+j+1] - m_knots[i+1];
                if ( temp >= eps<RtFloat>() ) {
                    temp = N[i+1] / temp;
                    Nd[i] -= temp;
                }
            } else {
                temp = m_knots[i+j] - m_knots[i];
                if ( temp >= eps<RtFloat>() ) {
                    temp = (N[i] + (t-m_knots[i])*Nd[i]) / temp;
                }
                Nd[i] = temp;
                temp = m_knots[i+j+1] - m_knots[i+1];
                if ( temp >= eps<RtFloat>() ) {
                    temp = (N[i+1] - (m_knots[i+j+1]-t)*Nd[i+1]) / temp;
                    Nd[i] -= temp;
                }
            }

            // Calc the basis
            if ( N[i] >= eps<RtFloat>() ) {
                temp = m_knots[i+j]-m_knots[i];
                if ( temp >= eps<RtFloat>() ) {
                    temp = N[i] * ((t - m_knots[i]) / temp);
                }
                N[i] = temp;
            }

            if ( N[i+1] >= eps<RtFloat>() ) {
                temp = m_knots[i+j+1]-m_knots[i+1];
                if ( temp >= eps<RtFloat>() ) {
                    temp = N[i+1] * ((m_knots[i+j+1] - t) / temp);
                    N[i] += temp;
                }
            }
        }
    }
}


void CBSplineBasis::sortKnots()
{
    std::sort(m_knots.begin(), m_knots.end());
}

bool CBSplineBasis::empty() const
{
    return knotSize() == 0;
}

void CBSplineBasis::validate()
{
    m_ncpts = clamp(m_ncpts, 0, m_ncpts);
    m_order = clamp(m_order, 0, m_order);
    if ( empty() )
        return;

    m_tess = clamp<IndexType>(m_tess, 1, m_tess);

    sortKnots();

    if ( m_knots.size() < knotSize() ) {
        RtFloat lastVal = m_knots.size() > 0 ? m_knots[m_knots.size()-1] : (RtFloat)0;
        size_t sz = m_knots.size();
        m_knots.resize(knotSize());
        for ( size_t i = sz; i < knotSize(); ++i )
            m_knots[i] = lastVal;
    }

    if ( m_knots.size() > knotSize() )
        m_knots.resize(knotSize());

    validateMinMax(m_tmin, m_tmax);
    m_tmin = clamp(m_tmin, m_knots[m_order-1], m_knots[m_ncpts]);
    m_tmax = clamp(m_tmax, m_knots[m_order-1], m_knots[m_ncpts]);

}


void CBSplineBasis::calc()
{
    // Test parameter conditions
    m_segments = 0;
    if ( empty() )
        return;

    RtFloat ts, te;
    RtFloat t, dt;
    IndexType cnt, lastCnt;

    m_segments = nuNumSegs(m_ncpts, m_order);

    m_tVals.clear();
    m_tVals.reserve(m_segments*(m_tess+1));
    m_valCnts.clear();
    m_valCnts.resize(m_ncpts);
    m_valCnts.assign(m_ncpts, 0);
    m_valOffs.clear();
    m_valOffs.resize(m_ncpts);
    m_valOffs.assign(m_ncpts, 0);

    lastCnt = 0;
    for ( RtInt knotIdx = m_order-1; knotIdx < m_ncpts; ++knotIdx ) {
        if ( knotIdx > 0 ) {
            m_valOffs[knotIdx] = m_valOffs[knotIdx-1]+lastCnt;
            lastCnt = 0;
        }
        ts = m_knots[knotIdx];
        te = m_knots[knotIdx+1];
        if ( (te-ts) < eps<RtFloat>() ) continue;

        dt = (te-ts)/(RtFloat)m_tess;

        if ( dt < eps<RtFloat>() )
            dt = eps<RtFloat>();

        if ( m_tmax < ts )
            break;
        if ( m_tmin >= te )
            continue;

        // m_tmax >= ts && m_tmin < te

        if ( m_tmin > ts )
            ts = m_tmin;
        if ( m_tmax < te )
            te = m_tmax;

        m_valCnts[knotIdx] = (RtInt)m_tVals.size();
        m_tVals.push_back(ts);
        for ( t = m_knots[knotIdx], cnt = 0;
                t < te && cnt < m_tess;
                t += dt, ++cnt )
        {
            if ( t <= ts )
                continue;
            m_tVals.push_back(t);
        }
        m_tess=cnt;
        m_tVals.push_back(te);
        lastCnt = m_valCnts[knotIdx] = (RtInt)m_tVals.size()-m_valCnts[knotIdx];
    }

    std::vector<RtFloat> Ntemp(m_ncpts+m_order-1);     // BASIS, result can be found in N[0..n-1]
    std::vector<RtFloat> Ndtemp(m_ncpts+m_order-1);    // BASIS derivative, result can be found in  Nd[0..n-1]

    m_basis.clear();
    m_basis.resize(m_tVals.size()*m_order);
    m_basisDeriv.clear();
    m_basisDeriv.resize(m_tVals.size()*m_order);

    RtInt GIdx = 0;
    RtInt i, j;
    std::vector<RtFloat>::iterator iter = m_tVals.begin();
    for ( RtInt span = m_order-1; span < m_ncpts; ++span ) {
        for ( j = m_valCnts[span];
                j > 0 && iter != m_tVals.end();
                iter++, --j )
        {
            bsplineBasisDerivate(*iter, span, Ntemp, Ndtemp);
            for ( i = span-m_order+1; i <= span; ++i, ++GIdx ) {
                // Cache the results (basis and derivative) for parameter t
                m_basis[GIdx] = Ntemp[i];
                m_basisDeriv[GIdx] = Ndtemp[i];
            }
        }
    }
}


CBSplineBasis &CBSplineBasis::operator=(const CBSplineBasis &sp)
{
    if ( &sp == this )
        return *this;

    m_ncpts = sp.m_ncpts;
    m_order = sp.m_order;
    m_tmin  = sp.m_tmin;
    m_tmax  = sp.m_tmax;
    m_knots = sp.m_knots;
    m_tess = sp.m_tess;
    m_segments = sp.m_segments;
    m_tVals = sp.m_tVals;
    m_valCnts = sp.m_valCnts;
    m_valOffs = sp.m_valOffs;

    m_basis = sp.m_basis;
    m_basisDeriv = sp.m_basisDeriv;

    return *this;
}


void CBSplineBasis::clearResults()
{
    m_segments = 0;
    m_tVals.clear();
    m_valCnts.clear();
    m_valOffs.clear();
    m_basis.clear();
    m_basisDeriv.clear();
}

void CBSplineBasis::insertVals(RtInt theNCpts, RtInt theOrder,
                               RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
    m_ncpts = theNCpts;
    m_order = theOrder;
    m_tmin  = theTMin;
    m_tmax  = theTMax;
    m_tess = theTess;

}

void CBSplineBasis::insertKnots(const std::vector<RtFloat> &theKnots)
{
    m_knots = theKnots;
}

void CBSplineBasis::insertKnots(const std::vector<RtFloat> &theKnots, IndexType theKnotOffs)
{
    assert(theKnotOffs < theKnots.size());
    if ( theKnotOffs >= theKnots.size() )
        return;

    m_knots.resize(knotSize());

    IndexType i, end = knotSize();

    if ( theKnotOffs + end > theKnots.size() ) {
        end = (IndexType)theKnots.size() - theKnotOffs;
    }

    RtFloat lastKnot = 0;
    for (i = 0; i < end; ++i) {
        RtFloat lastKnot =  theKnots[theKnotOffs+i];
        m_knots[i] = lastKnot;
    }
    for (i = end; i < knotSize(); ++i) {
        m_knots[i] = lastKnot;
    }
}

void CBSplineBasis::reset(RtInt theNCpts, RtInt theOrder,
                          const std::vector<RtFloat> &theKnots,
                          RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
    insertVals(theNCpts, theOrder, theTMin, theTMax, theTess);
    insertKnots(theKnots);
    validate();
    calc();
}


void CBSplineBasis::reset(RtInt theNCpts, RtInt theOrder,
                          const std::vector<RtFloat> &theKnots, IndexType theKnotOffs,
                          RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
    insertVals(theNCpts, theOrder, theTMin, theTMax, theTess);
    insertKnots(theKnots, theKnotOffs);
    validate();
    calc();
}

RtInt CBSplineBasis::nuBlendP2W(const std::vector<RtFloat> &source,
                                RtInt offs,
                                RtInt seg,
                                std::vector<RtFloat> &results,
                                RtInt resultOffs) const
{
    if ( numParameters(seg) <= 0 ) {
        // results.clear();
        return 0;
    }

    if ( results.size() < resultOffs + static_cast<size_t>(numParameters(seg)*2) ) {
        results.resize(resultOffs + numParameters(seg)*2);
    }

    RtInt oi, pcnt;
    RtFloat u, v, w, U, V, W, baseElem;
    for ( pcnt = 0; pcnt < numParameters(seg); ++pcnt ) {
        U = V = W = 0;
        for ( oi = 0; oi < order(); ++oi ) {
            baseElem = basisElem(seg, pcnt, oi);
            u = source[offs+(seg+oi)*3  ];
            v = source[offs+(seg+oi)*3+1];
            w = source[offs+(seg+oi)*3+2];
            U += baseElem * u;
            V += baseElem * v;
            W += baseElem * w;
        }
        if ( W > 0 ) {
            U /= W;
            V /= W;
        }
        results[resultOffs+2*pcnt  ] = U;
        results[resultOffs+2*pcnt+1] = V;
    }
    return 2*pcnt;
}

RtInt CBSplineBasis::nuBlendP2W(const std::vector<RtFloat> &source,
                                RtInt offs,
                                std::vector<RtFloat> &results,
                                RtInt resultOffs) const
{
    RtInt sz = 0;
    for ( RtInt seg = 0; seg < numSegments(); ++seg ) {
        sz += nuBlendP2W(source, offs, seg, results, resultOffs + sz);
    }

    return sz;
}

void CBSplineBasis::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "Type: CBSplineBasis" << std::endl;

    out << prefix << "m_ncpts = " << m_ncpts << std::endl;
    out << prefix << "m_order = " << m_order << std::endl;
    out << prefix << "m_tmin = " << m_tmin << std::endl;
    out << prefix << "m_tmax = " << m_tmax << std::endl;
    out << prefix << "m_tess = " << m_tess << std::endl;
    writeVector(m_knots, "m_knots", out, prefix);


    out << prefix << "m_segments = " << m_segments << std::endl;
    writeVector(m_valOffs, "m_valOffs", out, prefix);
    writeVector(m_valCnts, "m_valCnts", out, prefix);
    writeVector(m_tVals, "m_tVals", out, prefix);
    writeVector(m_basis, "m_basis", out, prefix);
    writeVector(m_basisDeriv, "m_basisDeriv", out, prefix);
}
