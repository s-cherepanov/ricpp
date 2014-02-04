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

IndexType CParametricAxis::findGridForCoord(RtFloat coord) const
{
	assert(m_coordinates.size() > 0);
	if ( m_coordinates.size() < 1 ) {
		// error vector is empty
		return 0;
	}

	// assert m_coordinates[i] <= m_coordinates[i+1] for 0 <= i < m_coordinates.size()

	IndexType left = 0, right = (IndexType)m_coordinates.size()-1;
	IndexType mid = (left+right)/2;

	// coordinat is max float?
	if ( coord >= m_coordinates[right].coord() )
		return right;

	while ( mid != left ) {

		assert(mid > left);
		assert(coord >= m_coordinates[left].coord());
		assert(coord < m_coordinates[right].coord());

		if ( coord >= m_coordinates[mid].coord() ) {
			left = mid;
		} else {
			// coord < m_coordinates[mid].coord()
			right = mid;
		}
		mid = (left+right)/2;
	}

	assert(mid < m_coordinates.size()-1);
	assert(coord >= m_coordinates[mid].coord());
	assert(coord < m_coordinates[mid+1].coord());

	return mid;
}


bool CParametricAxis::clip(IndexType gridIdx, int &gridIncrement, RtFloat coord0, RtFloat &clippedCoord) const
{
	gridIncrement = 0;

	if ( nearlyEqual(coord0, m_coordinates[gridIdx].coord()) ) {
		// Special case coord0 lies on grid line

		if ( nearlyEqual(clippedCoord, m_coordinates[gridIdx].coord()) ) {
			// Also the clipped coord lies on the same grid line
			clippedCoord = m_coordinates[gridIdx].coord();
			return true;
		}

		if ( gridIdx > 0 && clippedCoord <= m_coordinates[gridIdx-1].coord() ) {
			// clipped coord to the left of left neighbour grid line
			clippedCoord = m_coordinates[gridIdx-1].coord();
			gridIncrement = -1;
			return true;
		}

		if ( gridIdx+1 < m_coordinates.size() && clippedCoord >= m_coordinates[gridIdx+1].coord() ) {
			// clipped coord to the right of right neighbour grid line
			clippedCoord = m_coordinates[gridIdx+1].coord();
			gridIncrement = 1;
			return true;
		}

		if ( clippedCoord < coord0 ) {
			// clippedCoord lies on the left side of the current grid
			gridIncrement = -1;
		}

		return false;
	}

	// coord0 lies somewhere within a grid

	if ( clippedCoord <= coord0 ) {
		// on the left side of the coordinate
		if ( clippedCoord < m_coordinates[gridIdx].coord() ) {
			clippedCoord = m_coordinates[gridIdx].coord();
			gridIncrement = -1;
			return true;
		}
	} else {
		if ( gridIdx+1 >= m_coordinates.size() )
			return false;
		if ( clippedCoord >= m_coordinates[gridIdx+1].coord() ) {
			clippedCoord = m_coordinates[gridIdx+1].coord();
			gridIncrement = +1;
			return true;
		}
	}

	return false;
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

void CTrimGrid::CTrimFragments::startLoop(
	IndexType loop,
	RtFloat u,
	RtFloat v)
{
	IndexType giu = m_outer->uAxis().findGridForCoord(u);
	IndexType giv = m_outer->vAxis().findGridForCoord(v);

	m_curLoop = loop;

	CFragment frag;
	frag.m_giu = giu;
	frag.m_giv = giv;
	frag.m_startCoord = 0;
	frag.m_startU = u;
	frag.m_startV = v;
	m_fragments.push_back(frag);

	m_lastCoord = 0;
	m_lastU = u;
	m_lastV = v;
}

void CTrimGrid::CTrimFragments::moveTo(IndexType coord, RtFloat u, RtFloat v)
{
	RtFloat clipUU=u, clipUV=v, clipVU=u, clipVV=v;
	bool clippedU, clippedV;
	int gridIncrementU, gridIncrementV;

	do {
		clippedU = m_outer->uAxis().clip(m_fragments.back().m_giu, gridIncrementU, m_lastU, clipUU);
		clippedV = m_outer->vAxis().clip(m_fragments.back().m_giv, gridIncrementV, m_lastV, clipVV);

		if ( !(clippedU || clippedV) ) {
			// segment lies completely within a grid element
			m_lastCoord = coord;
			m_lastU = u;
			m_lastV = v;
			return;
		}

		if ( clippedU ) {
			clipUV = 0;
		}

		if ( clippedV ) {
			clipVU = 0;
		}

		if ( clippedU && gridIncrementU==0 ) {
			// lies on a vertical gridline
		}

		if ( clippedV && gridIncrementV==0 ) {
			// lies on a horizontal  gridline
		}

		CFragment frag;
		frag.m_giu = m_fragments.back().m_giu + gridIncrementU;
		frag.m_giv = m_fragments.back().m_giv + gridIncrementV;
		frag.m_startCoord = 0;
		frag.m_startU = clipUU;
		frag.m_startV = clipVV;
		m_fragments.push_back(frag);
	} while (false);
}

void CTrimGrid::CTrimFragments::endLoop()
{
}


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

CTrimGrid::~CTrimGrid()
{
	for ( std::list<CTrimFragments *>::iterator i = m_fragments.begin(); i != m_fragments.end(); i++ ) {
		delete (*i);
	}
	m_fragments.clear();
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

CTrimGrid::CTrimFragments *CTrimGrid::startLoop(IndexType loop, RtFloat u, RtFloat v) const
{
	CTrimFragments *frags = new CTrimFragments(this);
	if ( !frags ) {
		// memory error
		return 0;
	}
	frags->startLoop(loop, u, v);
	return frags;
}

void CTrimGrid::trim(const CTrimPath &trimPath)
{
    trimPath.getTrimCoords(m_trimLoopCoords);
	for ( IndexType loop = 0; loop < m_trimLoopCoords.size(); loop++ ) {
		IndexType numCoords = m_trimLoopCoords[loop].size();
		if ( !numCoords )
			continue;

		RtFloat u0, v0;
		m_trimLoopCoords[loop].getUV(0, u0, v0);
		CTrimGrid::CTrimFragments *frags = startLoop(loop, u0, v0);

		if ( !frags ) {
			// memory runtime error
			return;
		}

		for ( IndexType coord = 1; coord < numCoords; ++coord ) {
			RtFloat u, v;
			m_trimLoopCoords[loop].getUV(coord, u, v);
			frags->moveTo(coord, u, v);
		}

		frags->endLoop();
		m_fragments.push_back(frags);
	}
    tesselate();
}

void CTrimGrid::write(std::ostream &out, const std::string &prefix) const
{
    out << prefix << "m_axises[uAxisIdx]" << std::endl;
    m_axises[uAxisIdx].write(out, prefix+"    ");
    out << prefix << "m_axises[vAxisIdx]" << std::endl;
    m_axises[vAxisIdx].write(out, prefix+"    ");
}
