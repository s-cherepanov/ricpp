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
: m_ostream(&ribout), m_ascii(true), m_countString(0)
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


void CRibElementsWriter::putChar(char c)
{
	m_ostream << c;
}


void CRibElementsWriter::putChars(const char *cs)
{
	if ( !cs )
		return;
		
	for ( ; *cs; ++cs ) {
		m_ostream << *cs;
	}
}


void CRibElementsWriter::putRequest(EnumRequests aRequest)
{
	if ( m_ascii ) {
		putChars(CRequestInfo::requestName(aRequest));
	} else {
	}
}


void CRibElementsWriter::putArray(const std::vector<float> &floats)
{
	putArray(floats.size(), floats.size() ? &floats[0] : 0);
}


void CRibElementsWriter::putArray(size_t length, const float *floats)
{
	assert ((length > 0) ? floats != 0 : true); 
	if ( m_ascii ) {
		m_ostream << "[";
		for ( size_t i = 0; i< length; ++i ) {
			m_ostream << " " << floats[i];
		}
		m_ostream << " ]";
	} else {
	}
}


void CRibElementsWriter::putArray(const std::vector<double> &floats)
{
	putArray(floats.size(), floats.size() ? &floats[0] : 0);
}


void CRibElementsWriter::putArray(size_t length, const double *floats)
{
	assert ((length > 0) ? floats != 0 : true); 
	if ( m_ascii ) {
		m_ostream << "[";
		for ( size_t i = 0; i< length; ++i ) {
			m_ostream << " " << floats[i];
		}
		m_ostream << " ]";
	} else {
	}
}

void CRibElementsWriter::putValue(float aFloat)
{
	if ( m_ascii ) {
		m_ostream << aFloat;
	} else {
	}
}


void CRibElementsWriter::putValue(double aFloat)
{
	if ( m_ascii ) {
		m_ostream << aFloat;
	} else {
	}
}


void CRibElementsWriter::putArray(size_t length, const RtInt *integers)
{
	if ( m_ascii ) {
		m_ostream << "[";
		for ( size_t i = 0; i< length; ++i ) {
			m_ostream << " " << integers[i];
		}
		m_ostream << " ]";
	} else {
	}
}


void CRibElementsWriter::putValue(RtInt anInteger)
{
	if ( m_ascii ) {
		m_ostream << anInteger;
	} else {
	}
}


void CRibElementsWriter::putString(RtString aString)
{
	if ( m_ascii ) {
	
		m_ostream << "\"";
		for ( ; *aString; ++aString ) {
			switch ( *aString ) {
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
					if ( *aString >= ' ' && *aString <= '~' ) {
						m_ostream << *aString;
					} else {
						unsigned char c = (unsigned char)*aString;
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
	
		unsigned long len = strlen(aString);

		if ( len < 16 ) {
			unsigned char code = 0220+(unsigned char)len;
			m_ostream << code;
			putChars(aString);
			return;
		}

		unsigned char code = 0240;
		unsigned char bytes = 0;
		if ( len > 0xFFUL )
			bytes = 1;
		if ( len > 0xFFFFUL )
			bytes = 2;
		if ( len > 0xFFFFFFUL )
			bytes = 3;
		code += bytes;
		m_ostream << code;

		if ( bytes == 3 ) {
			code = len >> 24;
			m_ostream << code;			
		}
		if ( bytes >= 2 ) {
			code = (unsigned char)((len & 0x00ffffffUL) >> 16);
			m_ostream << code;			
		}
		if ( bytes >= 1 ) {
			code = (unsigned char)((len & 0x0000ffffUL) >> 8);
			m_ostream << code;			
		}
		code = (unsigned char)(len & 0x000000ffUL);
		m_ostream << code;			

		putChars(aString);
	}
}


void CRibElementsWriter::putStringTokenNum(unsigned char code, long tok)
{
	unsigned char bytes = 0;
	if ( tok > 0xFFL )
		bytes = 1;
	code += bytes;
	m_ostream << code;			

	if ( bytes >= 1 ) {
		code = (unsigned char)((tok & 0xffffL) >> 8);
		m_ostream << code;			
	}
	code = (unsigned char)(tok & 0x00ffL);
	m_ostream << code;
}


void CRibElementsWriter::putStringToken(RtString aString)
{
	if ( !aString )
		return;
		
	if ( m_ascii ) {
		putString(aString);
		return;
	}

	std::string strval(aString);
	std::map<std::string, RtInt>::iterator i = m_stringTokens.find(strval);
	long tok = m_countString;

	if ( i == m_stringTokens.end() ) {

		if ( m_countString > 0xffffL) {
			putString(aString);
			return;
		}
		m_stringTokens.insert(std::map<std::string, RtInt>::value_type(strval, m_countString));
		tok = m_countString;
		++m_countString;

		putStringTokenNum(0315, tok);
		putString(aString);

	} else {

		tok = (*i).second;
	}
	
	putStringTokenNum(0317, tok);
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
