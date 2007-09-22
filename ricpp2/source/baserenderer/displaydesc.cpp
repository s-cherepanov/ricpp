/*! \file displaydesc.cpp
 *  \brief Implementation of the CDisplayDescr display descriptor
 *         (stores contents of a RiDisplay call)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ricpp/baserenderer/displaydesc.h"

using namespace RiCPP;

CDisplayChannelDescr::CDisplayChannelDescr()
{
	m_channel = 0;
}

CDisplayChannelDescr::CDisplayChannelDescr(const CDisplayChannelDescr &dcd)
{
	m_channel = 0;
	*this = dcd;
}

CDisplayChannelDescr::~CDisplayChannelDescr()
{
	if ( m_channel )
		delete m_channel;
}

CDisplayChannelDescr &CDisplayChannelDescr::operator=(const CDisplayChannelDescr &dcd)
{
	if ( this == &dcd )
		return *this;

	if ( m_channel )
		delete m_channel;

	m_channel = 0;
	if ( dcd.m_channel )
		m_channel = new CDeclaration(*dcd.m_channel);

	CParameterList::operator=(dcd);
	return *this;
}

bool CDisplayChannelDescr::operator==(const CDisplayChannelDescr &dcd) const
{
	if ( m_channel == dcd.channel() )
		return true;
	if ( !m_channel )
		return false;
	if ( !dcd.channel() )
		return false;

	return !strcmp(noNullStr(m_channel->name()), noNullStr(dcd.channel()->name()));
}

bool CDisplayChannelDescr::operator==(std::string name) const
{
	return *this == name.c_str();
}

bool CDisplayChannelDescr::operator==(const char *name) const
{
	if ( !m_channel )
		return false;
	if ( name == m_channel->name() )
		return true;

	return !strcmp(noNullStr(m_channel->name()), noNullStr(name));
}

RtVoid CDisplayChannelDescr::displayChannelV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString aChannel, RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( m_channel )
		delete m_channel;

	m_channel = new CDeclaration(aChannel, colorDescr.colorSamples());
	set(CValueCounts(), dict, colorDescr, aChannel, n, tokens, params);
}


// ----------------------------------------------------------------------------
// CDisplayDescr

CDisplayDescr::CDisplayDescr()
{
	m_type = RI_NULL;
	m_mode = RI_NULL;
	m_origin[0] = m_origin[1] = 0;
}


////////////////////////////////////////////////////////////////////////////////
/*! \param tdd instance to copy from
 */
CDisplayDescr::CDisplayDescr(const CDisplayDescr &dd) {
	m_type = RI_NULL;
	m_mode = RI_NULL;
	m_origin[0] = m_origin[1] = 0;
	*this = dd;
}


////////////////////////////////////////////////////////////////////////////////
/*! \param dd instance to assign from
 *  \return A reference of '*this"
 */
CDisplayDescr &CDisplayDescr::operator=(const CDisplayDescr &dd) {
	if ( &dd == this )
		return *this;

	m_type = dd.m_type;
	m_mode = dd.m_mode;
	m_origin[0] = dd.m_origin[0];
	m_origin[1] = dd.m_origin[1];

	m_channels = dd.m_channels;
	m_channelNames = dd.m_channelNames;

	CNamedParameterList::operator=(dd);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*! Fills a CDisplayDescr instance with a display description given in
 *  a TRi::displayV() call
 *
 *  \param dict       Current declarations
 *  \param curColorSize Number of components per color
 *  \param aName      Token with name of the
 *  \param aType      Token with type of the display
 *  \param aMode      Token with the display mode
 *  \param n          Number of additional parameters
 *  \param tokens     Tokens of the additional parameters
 *  \param params     Values of the parameters
 *  \return false if parameters could not be parsed, true if ok
 */
void CDisplayDescr::displayV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, const CDisplayDescr::DisplayChannels_type &channels, RtToken aName, RtToken aType, RtString aMode, RtInt n, RtToken tokens[], RtPointer params[]) {
	m_type = aType;
	m_mode = aMode;

	m_origin[0] = 0;
	m_origin[1] = 0;
	
	set(CValueCounts(), dict, colorDescr, aName, n, tokens, params);

	const CParameter *p = get(RI_ORIGIN);
	if ( p ) {
		const std::vector<RtInt> &ints = p->ints();
		if ( ints.size() >= 2 )
		{
			m_origin[0] = ints[0];
			m_origin[1] = ints[1];
		}
	}

	m_channels.clear();

	m_channelNames.clear();
	m_channelNames.explode(',', aMode, false, false, false);
	for ( CStringList::const_iterator iter = m_channelNames.begin();
	      iter != m_channelNames.end();
	      ++iter )
	{
		for ( CDisplayDescr::DisplayChannels_type::const_iterator dciter = channels.begin();
			  dciter != channels.end();
			  ++dciter )
		{
			if ( *dciter == *iter ) {
				m_channels.push_back(*dciter);
			}
		}
	}
}
