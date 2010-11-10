/*
 *  trimgrid.h
 *  test_nurbs
 *
 *  Created by Andreas Pidde on 26.04.10.
 *  Copyright 2010 Universität Koblenz-Landau. All rights reserved.
 *
 */

#ifndef _RICPP_TRIMGRID_H
#define _RICPP_TRIMGRID_H

#include "trimcurve.h"
#include <list>

namespace RiCPP {

///////////////////////////////////////////////////////////////////////////////////
// Parametric Grid

/** u or v axis
 */
enum EnumAxisIndex
{
    uAxisIdx = 0,
    vAxisIdx
}; // EnumAxisIndex

class CParametricLine;

/** A coordinate of a parametric grid axis
 */
class CGridCoordinate {
public:
    enum EnumCoordinateType {
        unknownCoord     = 0,
        segmentCoord     = 1,
        tesselatedCoord  = 2,
        minCoord         = 4,
        maxCoord         = 8
    };
private:
    RtFloat m_coord;                 //<! Value of the parametric coordinate
    EnumCoordinateType m_coordType;  //<! Type of the parametric coordinate
    CParametricLine *m_isoLine;      //<! Splitted Isoline with coordinate m_coordinate
    EnumAxisIndex m_axisIdx;         //<! Index of the axis of the coordinate

    CParametricLine *duplicateIsoLine() const;
public:
    inline CGridCoordinate() {
        m_coord = (RtFloat)0;
        m_coordType = unknownCoord;
        m_axisIdx = uAxisIdx;
        m_isoLine = 0;
    }
    ~CGridCoordinate();

    inline bool hasIsoLine() const {
        return m_isoLine != 0;
    }
    CParametricLine *isoLine();

    inline void coord(RtFloat val) {
        m_coord = val;
    }
    inline RtFloat coord() const {
        return m_coord;
    }

    inline void coordType(EnumCoordinateType aCoordType) {
        m_coordType = aCoordType;
    }
    inline EnumCoordinateType coordType() const {
        return m_coordType;
    }

    inline EnumAxisIndex axisIdx() const {
        return m_axisIdx;
    }
    inline void axisIdx(EnumAxisIndex anAxisIdx) {
        m_axisIdx = anAxisIdx;
    }

    CGridCoordinate &operator=(const CGridCoordinate &gc);

    void write(std::ostream &out, const std::string &prefix) const;
}; // CGridCoordinate


/** Axis of parametric coordinates
 */
class CParametricAxis {
    std::vector<CGridCoordinate> m_coordinates;
    EnumAxisIndex m_axisIdx;
public:
    inline CParametricAxis() {
        m_axisIdx = uAxisIdx;
    }
    void init(RtInt n, RtFloat knot[], RtFloat minKnot, RtFloat maxKnot, IndexType tess, EnumAxisIndex anAxisIdx);

    inline void axisIndex(EnumAxisIndex anAxisIdx) {
        m_axisIdx = anAxisIdx;
    }
    inline EnumAxisIndex axisIndex() const {
        return m_axisIdx;
    }

	IndexType findGridForCoord(RtFloat coord) const;
	
	inline RtFloat coord(IndexType idx) const {
		return m_coordinates[idx].coord();
	}

	inline IndexType size() const {
		return m_coordinates.size();
	}

	bool clip(IndexType gridIdx, int &gridIncrement, RtFloat coord, RtFloat &clippedCoord) const;
    void write(std::ostream &out, const std::string &prefix) const;
}; // CParametricAxis


/** Iso parametric (either u or v) line
 */
class CParametricLine {
    RtFloat m_min, m_max, m_split;
    CParametricLine *m_lower;
    CParametricLine *m_higher;
    IndexType m_coordIdx;
    EnumAxisIndex m_axisIdx;


public:
    inline CParametricLine()
    {
        m_min = std::numeric_limits<RtFloat>::min();
        m_max = std::numeric_limits<RtFloat>::max();
        m_split = 0;
        m_lower = 0;
        m_higher = 0;
        m_coordIdx = 0;
        m_axisIdx = uAxisIdx;
    }

    CParametricLine(const CParametricLine &copyFrom);

    CParametricLine(EnumAxisIndex anAxisIdx,
                    IndexType aCoordIdx);

    CParametricLine(EnumAxisIndex anAxisIdx,
                    IndexType aCoordIdx,
                    RtFloat aMin,
                    RtFloat aMax);

    ~CParametricLine();

    inline CParametricLine *duplicate() const {
        return new CParametricLine(*this);
    }

    inline RtFloat minCoord() const {
        return m_min;
    }
    inline RtFloat maxCoord() const {
        return m_max;
    }
    inline RtFloat splitCoord() const {
        return m_split;
    }
    inline EnumAxisIndex axisIdx() const {
        return m_axisIdx;
    }

    CParametricLine *splitAt(RtFloat coord);
private:
    inline CParametricLine *duplicateLower() const {
        return m_lower ? m_lower->duplicate() : 0;
    }
    inline CParametricLine *duplicateHigher() const {
        return m_higher ? m_higher->duplicate() : 0;
    }
}; // CParametricLine


/** A grid of parametric coordinates for trimming
 */
class CTrimGrid {
	class CTrimFragments {
		const CTrimGrid *m_outer;
		IndexType m_curLoop;

		class CFragment {
		public:
			IndexType m_giu, m_giv;
			IndexType m_startCoord;
			RtFloat m_startU, m_startV;
		};
		std::list<CFragment> m_fragments;

		IndexType m_lastCoord;
		RtFloat m_lastU, m_lastV;

	public:
		inline CTrimFragments(const CTrimGrid *outer) : m_outer(outer) { }
		void startLoop(IndexType loop, RtFloat u, RtFloat v);
		void moveTo(IndexType coord, RtFloat u, RtFloat v);
		void endLoop();
	};

	friend class CTrimFragments;
    CParametricAxis m_axises[2]; //!< u, v axis

    std::vector<CTrimCoords> m_trimLoopCoords;
	std::list<CTrimFragments *> m_fragments;

	CTrimFragments *startLoop(IndexType loop, RtFloat u, RtFloat v) const;

	void tesselate();
public:
    inline CTrimGrid() {}
    CTrimGrid(RtInt nu, RtFloat uknot[], RtFloat umin, RtFloat umax, IndexType utess, RtInt nv, RtFloat vknot[], RtFloat vmin, RtFloat vmax, IndexType vtess);

	~CTrimGrid();

    inline const CParametricAxis &uAxis() const {
        return m_axises[uAxisIdx];
    }
    inline const CParametricAxis &vAxis() const {
        return m_axises[vAxisIdx];
    }
    void buildGrid(RtInt nu, RtFloat uknot[], RtFloat umin, RtFloat umax, IndexType utess, RtInt nv, RtFloat vknot[], RtFloat vmin, RtFloat vmax, IndexType vtess);
    void trim(const CTrimPath &trimPath);

	void write(std::ostream &out, const std::string &prefix) const;
}; // CTrimGrid

} // RiCPP

#endif // _RICPP_TRIMGRID_H
