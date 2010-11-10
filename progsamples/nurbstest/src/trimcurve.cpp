/*
 *  trimcurve.cpp
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#include "trimcurve.h"

using namespace RiCPP;

void CTrimCurveData::CTrimCurveDataInfo::trimCurve(RtInt nloops,
        const RtInt ncurves[],
        const RtInt order[],
        const RtFloat knot[],
        const RtFloat amin[],
        const RtFloat amax[],
        const RtInt n[],
        const RtFloat u[],
        const RtFloat v[],
        const RtFloat w[])
{
    m_nloops = 0;
    m_total  = 0;
    m_npoints = 0;
    m_nknots  = 0;

    if ( nloops == 0 || ncurves==0 || order==0 || knot==0 ||
            amin==0 || amax==0 || n==0 || u==0 || v==0 || w==0 )
    {

        return;
    }

    m_nloops = nloops;
    int i;	// Counter

    for ( i = 0; i < nloops; ++i ) {
        m_total += ncurves[i];
    }

    for ( i = 0; i < m_total; ++i ) {
        m_npoints += n[i];
        m_nknots += n[i] + order[i];
    }

    if ( m_total==0 || m_npoints==0 || m_nknots==0 ) {
        // Not a valid trim curve
        m_nloops = nloops;
        m_total  = 0;	// Total number of curves
        m_npoints = 0;	// Total number of control points
        m_nknots  = 0;	// Total number of knots

        return;
    }
}


void CTrimCurveData::CTrimCurveDataInfo::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "Type: CTrimCurveDataInfo" << std::endl;

    out << prefix << "m_nloops = " << m_nloops << std::endl;
    out << prefix << "m_total = " << m_total << std::endl;
    out << prefix << "m_npoints = " << m_npoints << std::endl;
    out << prefix << "m_nknots = " << m_nknots << std::endl;
}

// ----------------------------------------------------------------------------

inline CTrimCurveData &CTrimCurveData::operator=(const CTrimCurveData &curve)
{
    if ( this == &curve )
        return *this;
    if ( curve.m_data.m_nloops > 0 )
        trimCurve(curve.m_data.m_nloops,
                  curve.m_data.m_nloops == 0 || curve.m_nCurves.empty() ? 0 : &curve.m_nCurves[0],
                  curve.m_data.m_nloops == 0 || curve.m_order.empty() ? 0 : &curve.m_order[0],
                  curve.m_data.m_nloops == 0 || curve.m_knots.empty() ? 0 : &curve.m_knots[0],
                  curve.m_data.m_nloops == 0 || curve.m_min.empty() ? 0 : &curve.m_min[0],
                  curve.m_data.m_nloops == 0 || curve.m_max.empty() ? 0 : &curve.m_max[0],
                  curve.m_data.m_nloops == 0 || curve.m_n.empty() ? 0 : &curve.m_n[0],
                  curve.m_data.m_nloops == 0 || curve.m_u.empty() ? 0 : &curve.m_u[0],
                  curve.m_data.m_nloops == 0 || curve.m_v.empty() ? 0 : &curve.m_v[0],
                  curve.m_data.m_nloops == 0 || curve.m_w.empty() ? 0 : &curve.m_w[0]
                 );
    else {
        releaseAll();
    }
    return *this;
}

bool CTrimCurveData::operator==(const CTrimCurveData &curve) const
{
    // segments arn't compared, because it's number is calculated using the other values
    if ( m_data.m_nloops != curve.m_data.m_nloops ) return false;
    if ( m_nCurves != curve.m_nCurves ) return false;
    if ( m_order != curve.m_order )  return false;
    if ( m_knots != curve.m_knots )  return false;
    if ( m_min != curve.m_min )  return false;
    if ( m_max != curve.m_max )  return false;
    if ( m_n != curve.m_n )  return false;
    if ( m_u != curve.m_u )  return false;
    if ( m_v != curve.m_v )  return false;
    if ( m_w != curve.m_w )  return false;
    if ( m_points != curve.m_points )  return false;
    return true;
}

void CTrimCurveData::trimCurve(RtInt nloops,
                               const RtInt ncurves[],
                               const RtInt order[],
                               const RtFloat knot[],
                               const RtFloat amin[],
                               const RtFloat amax[],
                               const RtInt n[],
                               const RtFloat u[],
                               const RtFloat v[],
                               const RtFloat w[])
{
    releaseAll();

    m_data.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

    if ( m_data.m_nloops == 0 )
    {
        return;
    }

	int i, j, sum, offs;	// Counter

    m_nCurves.resize(m_data.m_nloops);
    m_nCurves.assign(ncurves, ncurves+m_data.m_nloops);

    m_nCurveOffs.resize(m_data.m_nloops);
    offs = 0;
    for ( IndexType cnt = 0; cnt < m_nCurveOffs.size(); ++cnt ) {
        m_nCurveOffs[cnt] = offs;
        offs += m_nCurves[cnt];
    }

    m_order.resize(m_data.m_total);
    m_order.assign(order, order+m_data.m_total);
    m_knots.resize(m_data.m_nknots);
    m_knots.assign(knot, knot+m_data.m_nknots);
    m_min.resize(m_data.m_total);
    m_min.assign(amin, amin+m_data.m_total);
    m_max.resize(m_data.m_total);
    m_max.assign(amax, amax+m_data.m_total);
    m_n.resize(m_data.m_total);
    m_n.assign(n, n+m_data.m_total);
    m_u.resize(m_data.m_npoints);
    m_u.assign(u, u+m_data.m_npoints);
    m_v.resize(m_data.m_npoints);
    m_v.assign(v, v+m_data.m_npoints);
    m_w.resize(m_data.m_npoints);
    m_w.assign(w, w+m_data.m_npoints);

    m_points.resize(m_data.m_npoints*3);
    sum = 0;
    for ( i = 0; i < m_data.m_total; ++i ) {
        for ( j = 0; j < n[i]; ++j, ++sum ) {
            m_points[sum*3+0] = m_u[sum];
            m_points[sum*3+1] = m_v[sum];
            m_points[sum*3+2] = m_w[sum];
        }
    }

    m_segments.resize(m_data.m_total);
    sum = 0;
    for ( i = 0; i < m_data.m_total; ++i ) {
        m_segments[i] = nuNumSegs(m_n[i], m_order[i]);
        sum += m_segments[i];
    }
    m_nsegments = sum;
}

void CTrimCurveData::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "Type: CTrimCurveData" << std::endl;

    out << prefix << "m_data" << std::endl;
    m_data.write(out, prefix+"    ");

    out << prefix << "m_nsegments = " << m_nsegments << std::endl;
    writeVector(m_segments, "m_segments", out, prefix);

    writeVector(m_nCurves, "m_nCurves", out, prefix);
    writeVector(m_nCurveOffs, "m_nCurveOffs", out, prefix);
    writeVector(m_order, "m_order", out, prefix);
    writeVector(m_knots, "m_knots", out, prefix);
    writeVector(m_min, "m_min", out, prefix);
    writeVector(m_max, "m_max", out, prefix);
    writeVector(m_n, "m_n", out, prefix);
    writeVector(m_u, "m_u", out, prefix);
    writeVector(m_v, "m_v", out, prefix);
    writeVector(m_w, "m_w", out, prefix);
    writeVector(m_points, "m_points", out, prefix, 3, true);
}

// -----------------------------

void CTrimCurve::insertCurve(int loopIdx, int curveIdx, const CTrimCurveData &trimCurveData)
{
    curveIdx += trimCurveData.curveOffs(loopIdx);
    assert(curveIdx < trimCurveData.curves());

    m_min = trimCurveData.minKnot(curveIdx);
    m_max = trimCurveData.maxKnot(curveIdx);
    m_order = trimCurveData.order(curveIdx);

    int startKnot = trimCurveData.startKnot(curveIdx);
    int endKnot = startKnot + trimCurveData.nKnots(curveIdx);
    assert(endKnot - startKnot > 0);
    m_knots.resize(endKnot - startKnot);
    for (int kidx = 0; startKnot != endKnot; kidx++, startKnot++ ) {
        m_knots[kidx] = trimCurveData.knotVect()[startKnot];
    }

    int startPoint = trimCurveData.startPoint(curveIdx);
    int endPoint = trimCurveData.endPoint(curveIdx);
    assert(endPoint - startPoint > 0);
    m_uvw.resize((endPoint - startPoint) * 3);
    for (int pidx = 0; startPoint != endPoint; startPoint++ ) {
        m_uvw[pidx++] = trimCurveData.uVect()[startPoint];
        m_uvw[pidx++] = trimCurveData.vVect()[startPoint];
        m_uvw[pidx++] = trimCurveData.wVect()[startPoint];
    }
    m_curTess = 0;
}

const std::vector<RtFloat> &CTrimCurve::tesselate(IndexType tesselation)
{
    if ( tesselation <= 0 ) {
        tesselation = 1;
    }
    if ( m_curTess != tesselation ) {
        m_basis.reset(m_uvw.size()/3, m_order, m_knots, m_min, m_max, tesselation);
        m_curTess = tesselation;
        m_basis.nuBlendP2W(m_uvw, 0, m_coordinates, 0);
    }
    return m_coordinates;
}


void CTrimCurve::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "Type: CTrimCurve" << std::endl;

    out << prefix << "m_min     = " << m_min << std::endl;
    out << prefix << "m_max     = " << m_max << std::endl;
    out << prefix << "m_order   = " << m_order << std::endl;
    out << prefix << "m_curTess = " << m_curTess << std::endl;

    writeVector(m_knots, "m_knots", out, prefix);
    writeVector(m_uvw, "m_uvw", out, prefix, 3, true);
    writeVector(m_coordinates, "m_coordinates", out, prefix, 2, true);

    out << prefix << "m_basis" << std::endl;
    m_basis.write(out, prefix+"    ");
}

// -----------------------------

void CTrimPath::trimCurve(const CTrimCurveData &trimCurveData)
{
    m_trimLoops.clear();
    m_trimLoops.resize(trimCurveData.loops());
    for ( IndexType i = 0; i < m_trimLoops.size(); ++i ) {
        m_trimLoops[i].insertCurves(i, trimCurveData);
    }
}

void CTrimPath::tesselate(IndexType tesselation)
{
    for ( IndexType i = 0; i < m_trimLoops.size(); ++i ) {
        m_trimLoops[i].tesselate(tesselation);
    }
}

void CTrimPath::tesselate(std::vector<CTrimCoords> &trimLoopCoords, IndexType tesselation)
{
    trimLoopCoords.clear();
    trimLoopCoords.resize(m_trimLoops.size());
    for ( IndexType i = 0; i < m_trimLoops.size(); ++i ) {
        m_trimLoops[i].tesselate(trimLoopCoords[i], tesselation);
    }
}

void CTrimPath::getTrimCoords(std::vector<CTrimCoords> &trimLoopCoords) const
{
    trimLoopCoords.clear();
    trimLoopCoords.resize(m_trimLoops.size());
    for ( IndexType i = 0; i < m_trimLoops.size(); ++i ) {
        m_trimLoops[i].getTrimCoords(trimLoopCoords[i]);
    }
}

void CTrimPath::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "Type: CTrimPath" << std::endl;
    out << prefix << "m_trimLoops - Size: " << m_trimLoops.size() << std::endl;
    for ( IndexType i = 0; i < m_trimLoops.size(); ++i ) {
        out << prefix << "m_trimLoops[" << i << "]" << std::endl;
        m_trimLoops[i].write(out, prefix + "    ");
    }
}
