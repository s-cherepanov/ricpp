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
{
	m_ribout = &ribout;
	m_ascii = true;
	memset(m_reqEncoding, 0, sizeof(m_reqEncoding)*sizeof(unsigned char));
}


CRibElementsWriter::~CRibElementsWriter()
{
}


void CRibElementsWriter::putNewLine()
{
}


void CRibElementsWriter::putSpace()
{
}


void CRibElementsWriter::putChar(char c)
{
}


void CRibElementsWriter::putChars(const char *cs)
{
}


void CRibElementsWriter::putRequest(EnumRequests aRequest, bool encoded)
{
}


void CRibElementsWriter::putFloatArray(const std::vector<float> &aFloat)
{
}

void CRibElementsWriter::putFloatArray(size_t length, const float *aFloat)
{
}

void CRibElementsWriter::putFloat(float aFloat)
{
}


void CRibElementsWriter::putFloat(double aFloat)
{
}


void CRibElementsWriter::putIntegerArray(size_t length, long integers)
{
}


void CRibElementsWriter::putInteger(long anInteger)
{
}


void CRibElementsWriter::fixedPoint(short number, unsigned short decimal)
{
}


void CRibElementsWriter::putStringArray(size_t length, RtString aString)
{
}


void CRibElementsWriter::putString(RtString aString)
{
}


RtInt CRibElementsWriter::defineStringToken(RtString aString, bool encoded)
{
	return REQ_UNKNOWN;
}


RtInt CRibElementsWriter::putStringToken(RtInt aToken)
{
	return REQ_UNKNOWN;
}


RtInt CRibElementsWriter::putStringToken(RtString aString, bool encoded)
{
	return REQ_UNKNOWN;
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
