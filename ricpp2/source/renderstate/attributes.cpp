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
