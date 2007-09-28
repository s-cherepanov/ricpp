#include "ricpp/renderstate/attributes.h"

using namespace RiCPP;

// ----------------------------------------------------------------------------

CAttributes::~CAttributes()
{
	if ( m_reader )
		deleteReader(m_reader);
}

CAttributes &CAttributes::operator=(const CAttributes &ra)
{
	if ( this == &ra )
		return *this;
	
	COptionsBase::operator=(ra);

	return *this;
}

void CAttributes::deleteReader(CAttributesReader *r)
{
	if ( r )
		delete r;
}


CAttributesReader *CAttributes::newReader()
{
	return new CAttributesReader(*this);
}


RtVoid CAttributes::color(RtColor Cs)
{
}


RtVoid CAttributes::opacity(RtColor Os)
{
}

RtVoid CAttributes::surfaceV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CAttributes::atmosphereV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CAttributes::interiorV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CAttributes::exteriorV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CAttributes::illuminate(RtLightHandle light, RtBoolean onoff)
{
}

RtVoid CAttributes::displacementV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CAttributes::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
}

RtVoid CAttributes::shadingRate(RtFloat size)
{
}

RtVoid CAttributes::shadingInterpolation(RtToken type)
{
}

RtVoid CAttributes::matte(RtBoolean onoff)
{
}

RtVoid CAttributes::bound(RtBound aBound)
{
}

RtVoid CAttributes::detail(RtBound aBound)
{
}

RtVoid CAttributes::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
}

RtVoid CAttributes::geometricApproximation(RtToken type, RtFloat value)
{
}

RtVoid CAttributes::geometricRepresentation(RtToken type)
{
}

RtVoid CAttributes::orientation(RtToken anOrientation)
{
}

RtVoid CAttributes::reverseOrientation(void)
{
}

RtVoid CAttributes::sides(RtInt nsides)
{
}

RtVoid CAttributes::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
}

RtVoid CAttributes::trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
}

