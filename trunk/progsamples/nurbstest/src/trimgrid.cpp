/*
 *  trimgrid.cpp
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#include "trimgrid.h"

using namespace RiCPP;


// ----------------------------------------------------------------------------

CGridCoordinate::~CGridCoordinate()
{
    if ( m_isoLine )
        delete m_isoLine;
    m_isoLine = 0;
}

CParametricLine *CGridCoordinate::isoLine()
{
    return m_isoLine ? m_isoLine : new CParametricLine(m_axisIdx, m_coordType);
}

CParametricLine *CGridCoordinate::duplicateIsoLine() const
{
    return m_isoLine ? m_isoLine->duplicate() : 0;
}

CGridCoordinate &CGridCoordinate::operator=(const CGridCoordinate &gc)
{
    if ( &gc == this )
        return *this;

    m_coord = gc.m_coord;
    m_coordType = gc.m_coordType;
    m_axisIdx = gc.m_axisIdx;

    if ( m_isoLine )
        delete m_isoLine;
    m_isoLine = 0;

    if ( gc.m_isoLine )
        m_isoLine = gc.duplicateIsoLine();

    return *this;
}

void CGridCoordinate::write(std::ostream &out, const std::string &prefix) const
{
    out << m_coord;
}

// ----------------------------------------------------------------------------

void CParametricAxis::init(RtInt n,
                           RtFloat knot[],
                           RtFloat minKnot,
                           RtFloat maxKnot,
                           IndexType tess,
                           EnumAxisIndex anAxisIdx)
{
    assert(n > 1);

    RtInt i;
    IndexType count;
    RtFloat tempKnot;

    m_axisIdx = anAxisIdx;

    // count the parameter values
    // Number of parameters for the Axis, additional 4 for -infinity, min, max, and infinity

    count = 2;
    tempKnot = minKnot;
    for ( i = 0; i < n; i++ ) {
        if ( knot[i] > tempKnot ) {
            RtFloat dist = knot[i] - tempKnot;
            RtFloat delta = dist / (RtFloat)tess;
            for ( IndexType j = 0; j < tess; ++j ) {
                if ( tempKnot > minKnot && tempKnot < maxKnot )
                    count++;
                tempKnot += delta;
            }
            tempKnot = knot[i];
        }
    }
    count += 2;

    m_coordinates.resize(count);

    // Enter the parameter values

    count = 0;
    m_coordinates[count++].coord(-std::numeric_limits<RtFloat>::max());
    m_coordinates[count++].coord(minKnot);

    tempKnot = minKnot;
    for ( i = 0; i < n; i++ ) {
        if ( knot[i] > tempKnot ) {
            RtFloat dist = knot[i] - tempKnot;
            RtFloat delta = dist / (RtFloat)tess;
            for ( IndexType j = 0; j < tess; ++j ) {
                if ( tempKnot > minKnot && tempKnot < maxKnot )
                    m_coordinates[count++].coord(tempKnot);
                tempKnot += delta;
            }
            tempKnot = knot[i];
        }
    }

    m_coordinates[count++].coord(maxKnot);
    m_coordinates[count++].coord(std::numeric_limits<RtFloat>::max());

    assert(count == m_coordinates.size());
}

void CParametricAxis::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "m_axisIdx" << m_axisIdx << std::endl;
    out << prefix << "m_coordinates - Size : " << m_coordinates.size() << std::endl;
    out << prefix;
    for ( std::vector<CGridCoordinate>::const_iterator iter = m_coordinates.begin();
            iter != m_coordinates.end();
            iter++ )
    {
        (*iter).write(out, prefix);
        out << " ";
    }
    out << std::endl;
}


// ----------------------------------------------------------------------------

CParametricLine::CParametricLine(const CParametricLine &copyFrom)
{
    m_min = copyFrom.m_min;
    m_max = copyFrom.m_max;
    m_split = copyFrom.m_split;
    m_lower = 0;
    m_higher = 0;
    m_coordIdx = copyFrom.m_coordIdx;
    m_axisIdx = copyFrom.m_axisIdx;

    if ( copyFrom.m_lower )
        m_lower = copyFrom.duplicateLower();

    if ( copyFrom.m_higher )
        m_lower = copyFrom.duplicateHigher();
}

CParametricLine::CParametricLine(EnumAxisIndex anAxisIdx,
                                 IndexType aCoordIdx)
{
    m_min = std::numeric_limits<RtFloat>::min();
    m_max = std::numeric_limits<RtFloat>::max();
    m_split = 0;
    m_lower = 0;
    m_higher = 0;
    m_coordIdx = aCoordIdx;
    m_axisIdx = anAxisIdx;
}

CParametricLine::CParametricLine(EnumAxisIndex anAxisIdx,
                                 IndexType aCoordIdx,
                                 RtFloat aMin,
                                 RtFloat aMax)
{
    m_min = aMin;
    m_max = aMax;
    m_split = 0;
    m_lower = 0;
    m_higher = 0;
    m_coordIdx = aCoordIdx;
    m_axisIdx = anAxisIdx;
}

CParametricLine::~CParametricLine()
{
    if ( m_lower ) delete m_lower;
    m_lower = 0;
    if ( m_higher ) delete m_higher;
    m_higher = 0;
}

CParametricLine *CParametricLine::splitAt(RtFloat coord)
{
    if ( coord < m_min || coord > m_max )
        return 0;

    if ( m_lower ) {
        assert(m_higher != 0);
        if ( coord < m_split ) {
            return m_lower->splitAt(coord);
        }
        return m_higher->splitAt(coord);
    }

    assert(m_higher == 0);

    m_lower = new CParametricLine(m_axisIdx, m_coordIdx, m_min, coord);
    if ( !m_lower )
        return 0;

    m_higher = new CParametricLine(m_axisIdx, m_coordIdx, coord, m_max);
    if ( !m_higher ) {
        delete m_lower;
        m_lower = 0;
        return 0;
    }
    m_split = coord;

    return this;
}


// ----------------------------------------------------------------------------


CTrimGrid::CTrimGrid(RtInt nu,
                     RtFloat uknot[],
                     RtFloat umin,
                     RtFloat umax,
                     IndexType utess,
                     RtInt nv,
                     RtFloat vknot[],
                     RtFloat vmin,
                     RtFloat vmax,
                     IndexType vtess)
{
    m_axises[uAxisIdx].init(nu, uknot, umin, umax, utess, uAxisIdx);
    m_axises[vAxisIdx].init(nv, vknot, vmin, vmax, vtess, vAxisIdx);
}


void CTrimGrid::buildGrid(RtInt nu,
                          RtFloat uknot[],
                          RtFloat umin,
                          RtFloat umax,
                          IndexType utess,
                          RtInt nv,
                          RtFloat vknot[],
                          RtFloat vmin,
                          RtFloat vmax,
                          IndexType vtess)
{
    m_axises[uAxisIdx].init(nu, uknot, umin, umax, utess, uAxisIdx);
    m_axises[vAxisIdx].init(nv, vknot, vmin, vmax, vtess, vAxisIdx);
    tesselate();
}

void CTrimGrid::tesselate()
{
    if ( m_trimLoopCoords.size() == 0 )
        return;
}

void CTrimGrid::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "m_axises[uAxisIdx]" << std::endl;
    m_axises[uAxisIdx].write(out, prefix+"    ");
    out << prefix << "m_axises[vAxisIdx]" << std::endl;
    m_axises[vAxisIdx].write(out, prefix+"    ");
}
