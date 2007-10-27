#include "ricpp/ricontext/attributes.h"
#include "ricpp/ricpp/ricpperror.h"

using namespace RiCPP;

// ----------------------------------------------------------------------------


const RtFloat CAttributes::defColorComponent = (RtFloat)1.0;
const RtFloat CAttributes::defOpacityComponent = (RtFloat)0.0;

const RtFloat CAttributes::defTextureS1 = 0;
const RtFloat CAttributes::defTextureT1 = 0;
const RtFloat CAttributes::defTextureS2 = 1;
const RtFloat CAttributes::defTextureT2 = 0;
const RtFloat CAttributes::defTextureS3 = 0;
const RtFloat CAttributes::defTextureT3 = 1;
const RtFloat CAttributes::defTextureS4 = 1;
const RtFloat CAttributes::defTextureT4 = 1;

const RtFloat CAttributes::defShadingRate = 1;
const RtToken CAttributes::defShadingInterpolation = RI_CONSTANT;
const RtBoolean CAttributes::defMatte = RI_FALSE;

const RtFloat CAttributes::defMinVis  = 0;
const RtFloat CAttributes::defLowTran = 0;
const RtFloat CAttributes::defUpTran  = RI_INFINITY;
const RtFloat CAttributes::defMaxVis  = RI_INFINITY;

const RtToken CAttributes::defGeometricApproximationType = RI_NULL;
const RtFloat CAttributes::defGeometricApproximationValue = 1;

const RtToken CAttributes::defGeometricRepresentation = RI_SMOOTH;

const RtToken CAttributes::defOrientation = RI_OUTSIDE;

const RtInt CAttributes::defNSides = 2;

CAttributes::~CAttributes()
{
}

void CAttributes::init()
{
	m_illuminated.clear();

	initColor();
	initOpacity();
	initTextureCoordinates();
	initSurface();
	initAtmosphere();
	initInterior();
	initExterior();
	initDisplacement();
	initShadingRate();
	initShadingInterpolation();
	initMatte();
	initBound();
	initDetail();
	initDetailRange();
	initGeometricApproximation();
	initGeometricRepresentation();
	initOrientation();
	initSides();
	initBasis();

	initTrimCurve();
}

CAttributes &CAttributes::operator=(const CAttributes &ra)
{
	if ( this == &ra )
		return *this;
	
	m_color = ra.m_color;
	m_opacity = ra.m_opacity;

	m_illuminated = ra.m_illuminated;

	m_surfaceName = ra.m_surfaceName;
	m_surfaceParams = ra.m_surfaceParams;

	m_atmosphereName = ra.m_atmosphereName;
	m_atmosphereParams = ra.m_atmosphereParams;

	m_interiorName = ra.m_interiorName;
	m_interiorParams = ra.m_interiorParams;

	m_exteriorName = ra.m_exteriorName;
	m_exteriorParams = ra.m_exteriorParams;

	m_displacementName = ra.m_displacementName;
	m_displacementParams = ra.m_displacementParams;

	memcpy(m_textureCoordinates, ra.m_textureCoordinates, sizeof(m_textureCoordinates));

	m_shadingRate = ra.m_shadingRate;
	m_shadingInterpolation = ra.m_shadingInterpolation;
	m_matte = ra.m_matte;

	memcpy(m_bound, ra.m_bound, sizeof(m_bound));
	memcpy(m_detail, ra.m_detail, sizeof(m_detail));
	m_boundCalled = ra.m_boundCalled;
	m_detailCalled = ra.m_detailCalled;

	memcpy(m_detailRange, ra.m_detailRange, sizeof(m_detailRange));
	m_detailRangeCalled = ra.m_detailRangeCalled;

	m_geometricApproximationType = ra.m_geometricApproximationType;
	m_geometricApproximationValue = ra.m_geometricApproximationValue;

	m_geometricRepresentation = ra.m_geometricRepresentation;

	m_orientation = ra.m_orientation;
	m_orientation = ra.m_orientation;
	m_nSides = ra.m_nSides;

	memcpy(m_uBasis, ra.m_uBasis, sizeof(m_uBasis));
	m_uStep = ra.m_uStep;
	memcpy(m_vBasis, ra.m_vBasis, sizeof(m_vBasis));
	m_vStep = ra.m_vStep;

	m_trimCurve = ra.m_trimCurve;

	COptionsBase::operator=(ra);

	return *this;
}

void CAttributes::initColor()
{
	RtInt n = colorSamples();
	m_color.resize(n);

	for ( RtInt i = 0; i < n; ++i )
	{
		m_color[i] = defColorComponent;
	}
}

RtVoid CAttributes::color(RtColor Cs)
{
	RtInt n = colorSamples();
	m_color.resize(n);

	for ( RtInt i = 0; i < n; ++i )
	{
		m_color[i] = Cs[i];
	}
}

RtFloat CAttributes::color(RtInt i) const
{
	if ( i < 0 )
		return 0;

	if ( i >= colorSamples() )
		return 0;

	return m_color[i];
}

void CAttributes::initOpacity()
{
	RtInt n = colorSamples();
	m_opacity.resize(n);

	for ( RtInt i = 0; i < n; ++i )
	{
		m_opacity[i] = defOpacityComponent;
	}
}

RtVoid CAttributes::opacity(RtColor Os)
{
	RtInt n = colorSamples();
	m_opacity.resize(n);

	for ( RtInt i = 0; i < n; ++i )
	{
		m_opacity[i] = Os[i];
	}
}

RtFloat CAttributes::opacity(RtInt i) const
{
	if ( i < 0 )
		return 0;

	if ( i >= colorSamples() )
		return 0;

	return m_opacity[i];
}

RtVoid CAttributes::illuminate(RtLightHandle light, RtBoolean onoff)
{
	TypeLightHandles::iterator i = find(m_illuminated.begin(), m_illuminated.end(), light);
	if ( onoff ) {
		if ( i == m_illuminated.end() ) {
			m_illuminated.push_back(light);
		}
	} else {
		if ( i != m_illuminated.end() ) {
			m_illuminated.erase(i);
		}
	}
}

RtBoolean CAttributes::illuminated(RtLightHandle light) const
{
	TypeLightHandles::const_iterator i = find(m_illuminated.begin(), m_illuminated.end(), light);
	return (i != m_illuminated.end()) ? RI_TRUE : RI_FALSE;
}

void CAttributes::initSurface()
{
	m_surfaceName = RI_NULL;
	m_surfaceParams.clear();
}

RtVoid CAttributes::surface(RtToken name, const CParameterList &params)
{
	m_surfaceName = name;
	m_surfaceParams = params;
}

void CAttributes::initAtmosphere()
{
	m_atmosphereName = RI_NULL;
	m_atmosphereParams.clear();
}

RtVoid CAttributes::atmosphere(RtToken name, const CParameterList &params)
{
	m_atmosphereName = name;
	m_atmosphereParams = params;
}

void CAttributes::initInterior()
{
	m_interiorName = RI_NULL;
	m_interiorParams.clear();
}

RtVoid CAttributes::interior(RtToken name, const CParameterList &params)
{
	m_interiorName = name;
	m_interiorParams = params;
}

void CAttributes::initExterior()
{
	m_exteriorName = RI_NULL;
	m_exteriorParams.clear();
}

RtVoid CAttributes::exterior(RtToken name, const CParameterList &params)
{
	m_exteriorName = name;
	m_exteriorParams = params;
}

void CAttributes::initDisplacement()
{
	m_displacementName = RI_NULL;
	m_displacementParams.clear();
}

RtVoid CAttributes::displacement(RtToken name, const CParameterList &params)
{
	m_displacementName = name;
	m_displacementParams = params;
}

RtVoid CAttributes::initTextureCoordinates()
{
	m_textureCoordinates[0] = defTextureS1;
	m_textureCoordinates[1] = defTextureT1;
	m_textureCoordinates[2] = defTextureS2;
	m_textureCoordinates[3] = defTextureT2;
	m_textureCoordinates[4] = defTextureS3;
	m_textureCoordinates[5] = defTextureT3;
	m_textureCoordinates[6] = defTextureS4;
	m_textureCoordinates[7] = defTextureT4;
}

RtVoid CAttributes::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	m_textureCoordinates[0] = s1;
	m_textureCoordinates[1] = t1;
	m_textureCoordinates[2] = s2;
	m_textureCoordinates[3] = t2;
	m_textureCoordinates[4] = s3;
	m_textureCoordinates[5] = t3;
	m_textureCoordinates[6] = s4;
	m_textureCoordinates[7] = t4;
}

void CAttributes::getTextureCoordinates(RtFloat &s1, RtFloat &t1, RtFloat &s2, RtFloat &t2, RtFloat &s3, RtFloat &t3, RtFloat &s4, RtFloat &t4) const
{
	s1 = m_textureCoordinates[0];
	t1 = m_textureCoordinates[1];
	s2 = m_textureCoordinates[2];
	t2 = m_textureCoordinates[3];
	s3 = m_textureCoordinates[4];
	t3 = m_textureCoordinates[5];
	s4 = m_textureCoordinates[6];
	t4 = m_textureCoordinates[7];
}


void CAttributes::initShadingRate()
{
	m_shadingRate = defShadingRate;
}

RtVoid CAttributes::shadingRate(RtFloat size)
{
	m_shadingRate = size;
}


void CAttributes::initShadingInterpolation()
{
	m_shadingInterpolation = defShadingInterpolation;
}

RtVoid CAttributes::shadingInterpolation(RtToken type)
{
	m_shadingInterpolation = type;
}


void CAttributes::initMatte()
{
	m_matte = defMatte;
}

RtVoid CAttributes::matte(RtBoolean onoff)
{
	m_matte = onoff;
}

void CAttributes::initBound()
{
	m_bound[0] = m_bound[1] = m_bound[2] = -RI_INFINITY;
	m_bound[3] = m_bound[4] = m_bound[5] =  RI_INFINITY;
	m_boundCalled = false;
}

RtVoid CAttributes::bound(RtBound aBound)
{
	m_boundCalled = true;
	for ( RtInt i=0; i<6 ; ++i ) {
		m_bound[i] = aBound[i];
	}
}

void CAttributes::initDetail()
{
	m_detail[0] = m_detail[1] = m_detail[2] = -RI_INFINITY;
	m_detail[3] = m_detail[4] = m_detail[5] =  RI_INFINITY;
	m_detailCalled = false;
}

RtVoid CAttributes::detail(RtBound aBound)
{
	m_detailCalled = true;
	for ( RtInt i=0; i<6 ; ++i ) {
		m_detail[i] = aBound[i];
	}
}

void CAttributes::initDetailRange()
{
	m_detailRangeCalled = false;
	m_detailRange[0] = defMinVis;
	m_detailRange[1] = defLowTran; 
	m_detailRange[2] = defUpTran;
	m_detailRange[3] = defMaxVis; 
}

RtVoid CAttributes::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	m_detailRangeCalled = true;
	m_detailRange[0] = minvis;
	m_detailRange[1] = lowtran; 
	m_detailRange[2] = uptran;
	m_detailRange[3] = maxvis; 
}

void CAttributes::getDetailRange(RtFloat &minvis, RtFloat &lowtran, RtFloat &uptran, RtFloat &maxvis) const
{
	minvis = m_detailRange[0];
	lowtran = m_detailRange[1]; 
	uptran = m_detailRange[2];
	maxvis = m_detailRange[3];
}

void CAttributes::initGeometricApproximation()
{
	m_geometricApproximationType = defGeometricApproximationType;
	m_geometricApproximationValue = defGeometricApproximationValue;
}

RtVoid CAttributes::geometricApproximation(RtToken type, RtFloat value)
{
	m_geometricApproximationType = type;
	m_geometricApproximationValue = value;
}

void CAttributes::initGeometricRepresentation()
{
	m_geometricRepresentation = defGeometricRepresentation;
}

RtVoid CAttributes::geometricRepresentation(RtToken type)
{
	m_geometricRepresentation = type;
}

void CAttributes::initOrientation()
{
	m_orientation = defOrientation;
}

RtVoid CAttributes::orientation(RtToken anOrientation)
{
	if ( anOrientation != RI_OUTSIDE &&
		 anOrientation != RI_INSIDE &&
		 anOrientation != RI_LH &&
		 anOrientation != RI_RH )
	{
		anOrientation = RI_OUTSIDE;
	}
	m_orientation = anOrientation;
}

RtVoid CAttributes::reverseOrientation(void)
{
	if ( m_orientation == RI_INSIDE ) m_orientation = RI_OUTSIDE;
	else if ( m_orientation == RI_OUTSIDE ) m_orientation = RI_INSIDE;
	else if ( m_orientation == RI_LH ) m_orientation = RI_RH;
	else if ( m_orientation == RI_RH ) m_orientation = RI_LH;
}

void CAttributes::initSides()
{
	m_nSides = defNSides;
}

RtVoid CAttributes::sides(RtInt nsides)
{
	m_nSides = nsides;
}

void CAttributes::initBasis()
{
	memcpy(m_uBasis, RiBezierBasis, sizeof(RtBasis));
	m_uStep = RI_BEZIERSTEP;
	memcpy(m_vBasis, RiBezierBasis, sizeof(RtBasis));
	m_vStep = RI_BEZIERSTEP;
}

RtVoid CAttributes::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	memcpy(m_uBasis, ubasis, sizeof(RtBasis));
	m_uStep = ustep;
	memcpy(m_vBasis, ubasis, sizeof(RtBasis));
	m_vStep = vstep;
}

void CAttributes::getBasis(RtBasis ubasis, RtInt &ustep, RtBasis vbasis, RtInt &vstep) const
{
	memcpy(ubasis, m_uBasis, sizeof(RtBasis));
	ustep = m_uStep;
	memcpy(ubasis, m_vBasis, sizeof(RtBasis));
	vstep = m_vStep;
}

void CAttributes::initTrimCurve()
{
	m_trimCurve.releaseAll();
}

RtVoid CAttributes::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	m_trimCurve.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
}

RtVoid CAttributes::trimCurve(const CTrimCurveData &trimCurveData)
{
	m_trimCurve = trimCurveData;
}

// ----------------------------------------------------------------------------

CAttributes *CAttributesFactory::newAttributes(const CColorDescr &c)
{
	CAttributes *a = newAttributesInstance(c);
	if ( !a ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newAttributes(const CColorDescr &)", __LINE__, __FILE__);
	}
	return a;
}
CAttributes *CAttributesFactory::newAttributes(const CAttributes &attr)
{
	CAttributes *a = newAttributesInstance(attr);
	if ( !a ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newAttributes(const CAttributes &)", __LINE__, __FILE__);
	}
	return a;
}
