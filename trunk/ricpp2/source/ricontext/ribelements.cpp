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

/** @file ribelements.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a class for RIB writing
 */

#include "ricpp/ricontext/ribelements.h"

#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H

#include <cassert>
#include <iostream>

using namespace RiCPP;

CRibElementsWriter::CRibElementsWriter(std::basic_streambuf<char, std::char_traits<char> > *ribout, IRequestNotification &notify)
: m_ostream(ribout), m_ascii(true), m_countStrings(0), m_firstRequestWritten(false), m_notify(notify), m_linecnt(1)
{
	memset(m_reqEncoding, 0, sizeof(m_reqEncoding)*sizeof(unsigned char));	
}

/*
CRibElementsWriter::CRibElementsWriter(TemplFrontStreambuf<char> *ribout, IRequestNotification &notify)
: m_ostream(ribout), m_ascii(true), m_countStrings(0), m_firstRequestWritten(false), m_notify(notify), m_linecnt(1)
{
	memset(m_reqEncoding, 0, sizeof(m_reqEncoding)*sizeof(unsigned char));
}
*/


void CRibElementsWriter::putNewLine()
{
	if ( m_ascii ) {
		m_ostream << std::endl;
		++m_linecnt;
	}
}


void CRibElementsWriter::putBlank()
{
	if ( m_ascii ) {
		m_ostream << ' ';
	}
}


void CRibElementsWriter::putEncodedChar(char ce)
{
	switch ( ce ) {
		case '\n' :
			m_ostream << "\\n";
			break;
		case '\r' :
			m_ostream << "\\r";
			break;
		case '\t' :
			m_ostream << "\\t";
			break;
		case '\b' :
			m_ostream << "\\b";
			break;
		case '\f' :
			m_ostream << "\\f";
			break;
		case '\\' :
			m_ostream << "\\\\";
			break;
		case '"' :
			m_ostream << "\\\"";
			break;
		default:
			if ( ce >= ' ' && ce <= '~' ) {
				m_ostream << ce;
			} else {
				unsigned char c = (unsigned char)ce;
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
			break;
	}
}

void CRibElementsWriter::putChar(char c)
{
	// if (c < 0) {
	// 	putEncodedChar(c);
	// 	return;
	// }
	m_ostream << c;
	if ( c == '\n' )
		++m_linecnt;
}


void CRibElementsWriter::putChars(const char *cs)
{
	if ( !cs )
		return;
	
	for ( ; *cs; ++cs )
		putChar(*cs);
}


void CRibElementsWriter::putComment(RtToken type, const char *cs)
{
	bool sav_ascii = m_ascii;
	m_ascii = true;

	if ( type != RI_STRUCTURE ) {
		m_firstRequestWritten = true;
	}

	if ( type != RI_VERBATIM ) {
		putChar('#');
	}
	if ( type == RI_STRUCTURE ) {
		// Additional
		putChar('#');
	}

	putChars(cs);
	
	if ( type != RI_VERBATIM || !cs || !*cs || cs[strlen(cs)-1] == '\n' ) {
		putNewLine();
	}

	m_ascii = sav_ascii;

	m_notify.requestWritten(REQ_COMMENT);
}


void CRibElementsWriter::putBinValue(float aFloat)
{
	unsigned char code;
	unsigned long tmp = 0;
//	tmp = *((unsigned long *)(void *)&aFloat);
	memcpy(&tmp, &aFloat, sizeof(unsigned long));

	code = (unsigned char)((tmp >> 24) & 0xffUL);
	m_ostream << code;			

	code = (unsigned char)((tmp >> 16) & 0xffUL);
	m_ostream << code;			

	code = (unsigned char)((tmp >> 8) & 0xffUL);
	m_ostream << code;			

	code = (unsigned char)(tmp & 0xffUL);
	m_ostream << code;			
}


void CRibElementsWriter::putBinValue(double aFloat)
{
		unsigned char *v; // value of float accessed as bytes
		int i;

		v = (unsigned char*)(void*)&aFloat;

		// Write double.
#ifdef LITTLE_ENDIAN         
		for ( i = 7; i > -1; i-- )
#else
		for ( i = 0; i < 8; i++ )
#endif        
		{
			m_ostream << v[i];
		}
}


void CRibElementsWriter::putRequest(EnumRequests aRequest)
{
	m_firstRequestWritten = true;
	if ( m_ascii ) {
		putChars(CRequestInfo::requestName(aRequest));
	} else {
		if ( !m_reqEncoding[aRequest] ) {
			unsigned char code = 0314;
			m_reqEncoding[aRequest] =  (unsigned char)aRequest;
			m_ostream << code << m_reqEncoding[aRequest];
			putString(CRequestInfo::requestName(aRequest));
			// Also write the request hereafter
		}
		unsigned char code = 0246;
		m_ostream << code << m_reqEncoding[aRequest];
	}
	m_notify.requestWritten(aRequest);
}


void CRibElementsWriter::putLength(unsigned char code, unsigned long length)
{
	unsigned char bytes = 0;
	if ( length > 0xffUL )
		bytes = 1;
	if ( length > 0xffffUL )
		bytes = 2;
	if ( length > 0xffffffUL )
		bytes = 3;
	code += bytes;
	m_ostream << code;

	if ( bytes == 3 ) {
		code = (unsigned char)((length >> 24) & 0xffUL);
		m_ostream << code;			
	}
	if ( bytes >= 2 ) {
		code = (unsigned char)((length >> 16) & 0xffUL);
		m_ostream << code;			
	}
	if ( bytes >= 1 ) {
		code = (unsigned char)((length >> 8) & 0xffUL);
		m_ostream << code;			
	}
	code = (unsigned char)(length & 0xffUL);
	m_ostream << code;			
}


void CRibElementsWriter::putArray()
{
	m_ostream << "[ ]";
}

void CRibElementsWriter::putArray(const RtMatrix m)
{
	if ( !m ) {
		putArray();
		return;
	}
	putArray(sizeof(RtMatrix)/sizeof(m[0][0]), &(m[0][0]));
}

void CRibElementsWriter::putArray(const RtBound b)
{
	if ( !b ) {
		putArray();
		return;
	}
	putArray(sizeof(RtBound)/sizeof(b[0]), &(b[0]));
}

void CRibElementsWriter::putArray(const std::vector<float> &floats)
{
	putArray((unsigned long)floats.size(), floats.size() ? &floats[0] : 0);
}


void CRibElementsWriter::putArray(unsigned long length, const float *floats)
{
	assert ((length > 0) ? floats != 0 : true); 
	if ( m_ascii ) {
		m_ostream << "[";
		for ( unsigned long i = 0; i< length; ++i ) {
			m_ostream << " " << floats[i];
		}
		m_ostream << " ]";
	} else {
		unsigned char code = 0310;
		putLength(code, length);
		for ( unsigned long i = 0; i< length; ++i ) {
			putBinValue(floats[i]);
		}
	}
}


void CRibElementsWriter::putArray(const std::vector<double> &floats)
{
	putArray((unsigned long)floats.size(), floats.size() ? &floats[0] : 0);
}


void CRibElementsWriter::putArray(unsigned long length, const double *floats)
{
	assert ((length > 0) ? floats != 0 : true); 
	m_ostream << "[";
	for ( unsigned long i = 0; i < length; ++i ) {
		m_ostream << " ";
		putValue(floats[i]);
	}
	m_ostream << " ]";
}


void CRibElementsWriter::putArray(const std::vector<RtInt> &integers)
{
	putArray((unsigned long)integers.size(), integers.size() ? &integers[0] : 0);
}


void CRibElementsWriter::putArray(unsigned long length, const RtInt *integers)
{
	assert ((length > 0) ? integers != 0 : true); 
	m_ostream << "[";
	for ( size_t i = 0; i< length; ++i ) {
		m_ostream << " ";
		putValue(integers[i]);
	}
	m_ostream << " ]";
}


void CRibElementsWriter::putArray(const std::vector<RtString> &strings)
{
	putArray((unsigned long)strings.size(), strings.size() ? &strings[0] : 0);
}


void CRibElementsWriter::putArray(unsigned long length, const RtString *strings)
{
	assert ((length > 0) ? strings != 0 : true); 
	m_ostream << "[";
	for ( size_t i = 0; i< length; ++i ) {
		m_ostream << " ";
		putString(strings[i]);
	}
	m_ostream << " ]";
}


void CRibElementsWriter::putBasis(const RtBasis basis)
{
	if ( basis == 0 ) {
		putArray();
		return;
	}
	RtToken tbasis = CTypeInfo::basisName(basis);
	if ( tbasis != RI_NULL )
		putStringToken(tbasis);
	else
		putArray(basis);
}


void CRibElementsWriter::putTokenArray(const std::vector<RtString> &strings)
{
	putTokenArray((unsigned long)strings.size(), strings.size() ? &strings[0] : 0);
}


void CRibElementsWriter::putTokenArray(unsigned long length, const RtString *strings)
{
	assert ((length > 0) ? strings != 0 : true); 
	m_ostream << "[";
	for ( size_t i = 0; i< length; ++i ) {
		m_ostream << " ";
		putStringToken(strings[i]);
	}
	m_ostream << " ]";
}


void CRibElementsWriter::putValue(float aFloat)
{
	if ( m_ascii ) {
		m_ostream << aFloat;
	} else {
		unsigned char code = 0244;
		m_ostream << code;
		putBinValue(aFloat);
	}
}


void CRibElementsWriter::putValue(double aFloat)
{
	if ( m_ascii ) {
		m_ostream << aFloat;
	} else {
		unsigned char code = 0245;
		m_ostream << code;
		putBinValue(aFloat);
	}
}


void CRibElementsWriter::putValue(int anInteger)
{
	if ( m_ascii ) {
		m_ostream << anInteger;
	} else {
		unsigned char code = 0200;
		putLength(code, (unsigned long)anInteger);
	}
}


void CRibElementsWriter::putValue(unsigned long anInteger)
{
	if ( m_ascii ) {
		m_ostream << anInteger;
	} else {
		unsigned char code = 0200;
		putLength(code, (unsigned long)anInteger);
	}
}


void CRibElementsWriter::putString(RtString aString)
{
	if ( !aString )
		aString = RI_EMPTY;

	if ( m_ascii ) {
	
		m_ostream << "\"";
		for ( ; *aString; ++aString ) {
			putEncodedChar( *aString );
		}
		m_ostream << "\"";
		
	} else {
	
		unsigned long len = (unsigned long)strlen(aString);

		if ( len < 16 ) {
			unsigned char code = 0220+(unsigned char)len;
			m_ostream << code;
			putChars(aString);
			return;
		}

		unsigned char code = 0240;
		putLength(code, len);
		putChars(aString);
	}
}


void CRibElementsWriter::putStringTokenNum(unsigned char code, unsigned long tok)
{
	unsigned char bytes = 0;
	if ( tok > 0xffUL )
		bytes = 1;
	code += bytes;
	m_ostream << code;			

	if ( bytes >= 1 ) {
		code = (unsigned char)((tok >> 8) & 0xffUL);
		m_ostream << code;			
	}
	code = (unsigned char)(tok & 0xffUL);
	m_ostream << code;
}


void CRibElementsWriter::putStringToken(RtString aString)
{
	if ( !aString )
		aString = RI_EMPTY;
		
	if ( m_ascii ) {
		putString(aString);
		return;
	}

	std::string strval(aString);
	std::map<std::string, RtInt>::iterator i = m_stringTokens.find(strval);
	unsigned long tok = m_countStrings;

	if ( i == m_stringTokens.end() ) {

		if ( m_countStrings > 0xffffUL) {
			putString(aString);
			return;
		}
		m_stringTokens.insert(std::map<std::string, RtInt>::value_type(strval, m_countStrings));
		tok = m_countStrings;
		++m_countStrings;

		putStringTokenNum(0315, tok);
		putString(aString);

	} else {

		tok = (*i).second;
	}
	
	putStringTokenNum(0317, tok);
}
