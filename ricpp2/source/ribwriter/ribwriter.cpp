// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/** @file ribwriter.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a context to write RIB streams
 */

#include "ricpp/ribwriter/ribwriter.h"

using namespace RiCPP;


CRibElementsWriter::CRibElementsWriter(TemplFrontStreambuf<char> &ribout)
: m_ostream(&ribout), m_ascii(true)
{
	memset(m_reqEncoding, 0, sizeof(m_reqEncoding)*sizeof(unsigned char));
}


CRibElementsWriter::~CRibElementsWriter()
{
}


void CRibElementsWriter::putNewLine()
{
	m_ostream << std::endl;
}


void CRibElementsWriter::putSpace()
{
	m_ostream << ' ';
}


void CRibElementsWriter::putChar(char c)
{
	if ( c >= 0 ) 
		m_ostream << c;
}


void CRibElementsWriter::putChars(const char *cs)
{
	if ( !cs )
		return;
	if ( m_ascii ) {
		m_ostream << "\"";
		for ( ; *cs; ++cs ) {
			switch(*cs) {
				case '\n' :
					m_ostream << "\\n";
				case '\r' :
					m_ostream << "\\r";
				case '\t' :
					m_ostream << "\\t";
				case '\b' :
					m_ostream << "\\b";
				case '\f' :
					m_ostream << "\\f";
				case '\\' :
					m_ostream << "\\\\";
				case '\"' :
					m_ostream << "\\\"";
				default:
					if ( *cs < ' ' || *cs > '~' ) {
						m_ostream << *cs;
					} else {
						unsigned char c = (unsigned char)*cs;
						int d3 = c / 64;
						c %= 64;
						int d2 = c / 8;
						c %= 8;
						int d1 = c;
						m_ostream << '\\';
						m_ostream << (char)('0'+d3);
						m_ostream << (char)('0'+d2);
						m_ostream << (char)('0'+d1);
					}
			}
		}
		m_ostream << "\"";
	} else {
		if ( strlen(cs) > 15 ) {
		} else {
		}
	}
}


void CRibElementsWriter::putRequest(EnumRequests aRequest)
{
	if ( m_ascii ) {
		m_ostream << CRequestInfo::requestName(aRequest);
	} else {
	}
}


void CRibElementsWriter::putFloatArray(const std::vector<float> &aFloat)
{
	if ( m_ascii ) {
		m_ostream << "[";
		m_ostream << "]";
	} else {
	}
}

void CRibElementsWriter::putFloatArray(size_t length, const float *aFloat)
{
	if ( m_ascii ) {
		m_ostream << "[";
		m_ostream << "]";
	} else {
	}
}

void CRibElementsWriter::putFloat(float aFloat)
{
	if ( m_ascii ) {
	} else {
	}
}


void CRibElementsWriter::putFloat(double aFloat)
{
	if ( m_ascii ) {
	} else {
	}
}


void CRibElementsWriter::putIntegerArray(size_t length, long integers)
{
	if ( m_ascii ) {
		m_ostream << "[";
		m_ostream << "]";
	} else {
	}
}


void CRibElementsWriter::putInteger(long anInteger)
{
	if ( m_ascii ) {
	} else {
	}
}


void CRibElementsWriter::fixedPoint(short number, unsigned short decimal)
{
	if ( m_ascii ) {
	} else {
	}
}


void CRibElementsWriter::putStringArray(size_t length, RtString aString)
{
	if ( m_ascii ) {
	} else {
	}
}


void CRibElementsWriter::putString(RtString aString)
{
	if ( m_ascii ) {
	} else {
	}
}


RtInt CRibElementsWriter::defineStringToken(RtString aString)
{
	if ( m_ascii ) {
	} else {
	}
	return -1;
}


RtInt CRibElementsWriter::putStringToken(RtInt aToken)
{
	if ( m_ascii ) {
	} else {
	}
	return aToken;
}


RtInt CRibElementsWriter::putStringToken(RtString aString)
{
	if ( m_ascii ) {
	} else {
	}
	return -1;
}


// -----------------------------------------------------------------------------

const char *CRibWriterCreator::myName() { return RIBWRITERCREATOR_NAME; }
const char *CRibWriterCreator::myType() { return CContextCreator::myType(); }
unsigned long CRibWriterCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }
unsigned long CRibWriterCreator::myMinorVersion() { return 1; }
unsigned long CRibWriterCreator::myRevision() { return 1; }

const char *CRibWriter::myName() { return RIBWRITER_NAME; }
const char *CRibWriter::myType() { return IRiContext::myType(); }
unsigned long CRibWriter::myMajorVersion() { return IRiContext::myMajorVersion(); }
unsigned long CRibWriter::myMinorVersion() { return 1; }
unsigned long CRibWriter::myRevision() { return 1; }
RtToken CRibWriter::myRendererType() { return RI_ARCHIVE; }
