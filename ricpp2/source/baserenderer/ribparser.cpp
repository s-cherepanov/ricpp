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

/** @file ribparser.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the parser for rib
 */

#include "ricpp/baserenderer/ribparser.h"
#include "ricpp/baserenderer/paramclasses.h"
#include "ricpp/ricpp/ricpptokens.h"
#include "ricpp/tools/templatefuncs.h"

using namespace RiCPP;

CRibParameter::CRibParameter() {
	m_lineCount = 0;
	m_isArray = false;
	m_typeID = BASICTYPE_UNKNOWN;
	m_cstrVector = 0;
}

CRibParameter::~CRibParameter() {
	freeValue();

	if ( m_cstrVector ) {
		delete m_cstrVector;
	}
	m_cstrVector = 0;
}

CRibParameter::CRibParameter(const CRibParameter &p) {
	m_lineCount = 0;
	m_cstrVector = 0;
	m_isArray = false;
	m_typeID = BASICTYPE_UNKNOWN;
	*this = p;
}

void CRibParameter::freeValue() {
	m_vInt.clear();
	m_vFloat.clear();
	m_vString.clear();
}

CRibParameter &CRibParameter::operator=(const CRibParameter &p) {
	if ( &p == this )
		return *this;

	freeValue();

	m_lineCount = p.m_lineCount;

	if ( m_cstrVector )
		delete m_cstrVector;
	m_cstrVector = 0;

	m_typeID = p.m_typeID;
	m_isArray = p.m_isArray;

	m_vInt = p.m_vInt;
	m_vFloat = p.m_vFloat;
	m_vString = p.m_vString;

	return *this;
}


void *CRibParameter::getValue() {
	// Is there at least one value?
	if ( getCard() == 0 )
		return 0;

	// return a pointer to the values
	switch ( m_typeID ) {
		case BASICTYPE_UNKNOWN:
			break;
		case BASICTYPE_INTEGER:
			return m_vInt.empty() ? 0 : (void *)&m_vInt[0];
		case BASICTYPE_FLOAT:
			return m_vFloat.empty() ? 0 : (void *)&m_vFloat[0];
		case BASICTYPE_STRING:
			// returns const char **, a pointer to strings
			// get pointers first
			if ( !m_cstrVector ) {
				m_cstrVector = new std::vector<const char *>;
				if ( !m_cstrVector )
					return 0;
			}
			size_t i;
			size_t size = m_vString.size();
			m_cstrVector->clear();
			m_cstrVector->resize(size);
			for ( i = 0; i < size; ++i ) {
				(*m_cstrVector)[i] = m_vString[i].c_str();
			}
			// returns char **
			return m_cstrVector->empty() ? 0 : (void *)&(*m_cstrVector)[0];
	}
	return 0;
}


void *CRibParameter::getValue(size_t i) const {
	if ( i >= getCard() )
		return 0;

	switch ( m_typeID ) {
		case BASICTYPE_UNKNOWN:
			break;
		case BASICTYPE_INTEGER:
			return (void *)&m_vInt[i];
		case BASICTYPE_FLOAT:
			return (void *)&m_vFloat[i];
		case BASICTYPE_STRING:
			// ! returns char * not char **
			return (void *)m_vString[i].c_str();
	}
	return 0;
}


size_t CRibParameter::getCard() const {

	// No valid typeID - no value is set
	if ( m_typeID < 0 || m_typeID >= N_BASICTYPES )
		return 0;

	// vector allocated - size is the cardinality
	switch ( m_typeID ) {
		case BASICTYPE_UNKNOWN:
			break;
		case BASICTYPE_INTEGER:
			return m_vInt.size();
		case BASICTYPE_FLOAT:
			return m_vFloat.size();
		case BASICTYPE_STRING:
			return m_vString.size();
	}
	return 0;
}


bool CRibParameter::convertIntToFloat() {
	if ( m_typeID == BASICTYPE_FLOAT )
		return true;

	if ( m_typeID != BASICTYPE_INTEGER )
		return false;

	size_t i;
	size_t size = m_vInt.size();
	m_vFloat.resize(size);
	for ( i = 0; i < size; ++i ) {
		m_vFloat[i]=(RtFloat)m_vInt[i];
	}
	m_vInt.clear();
	m_typeID = BASICTYPE_FLOAT;
	return true;
}

bool CRibParameter::convertFloatToInt() {
	if ( m_typeID == BASICTYPE_INTEGER )
		return true;

	if ( m_typeID != BASICTYPE_FLOAT )
		return false;

	size_t i;
	size_t size = m_vFloat.size();
	m_vInt.resize(size);
	for ( i = 0; i < size; ++i ) {
		m_vInt[i]=(RtInt)m_vFloat[i];
	}
	m_vFloat.clear();
	m_typeID = BASICTYPE_INTEGER;
	return true;
}



// ----------------------------------------------------------------------------

/** @brief Handles RIB request Sphere.
 */
class CSphereRequest : public CRibRequest {
public:
	virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
	inline virtual EnumRequests getId() const { return REQ_SPHERE; }
}; // CSphereRequest

void CSphereRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
#if 0
	CQuadricClasses p;

	if ( request.size() >= 4 && !request.isArray() ) {
		// Sphere radius zmin zmax thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtFloat radius, zmin, zmax, thetamax;
		bool b0 = p0.getFloat(radius),
		     b1 = p1.getFloat(zmin),
		     b2 = p2.getFloat(zmax),
			 b3 = p3.getFloat(thetamax);

		if ( b0 && b1 && b2 && b3 ) {
			int n = request.getTokenList(4, p);
			if ( n > 0 ) {
				parser.ribFilter().sphereV(radius, zmin, zmax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().sphereV(radius, zmin, zmax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 1 (radius) is not numeric",
					p0.lineCount(), parser.resourceName(), RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 2 (zmin) is not numeric",
					p1.lineCount(), parser.resourceName(), RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: 'Sphere' argument 3 (zmax) is not numeric",
					p2.lineCount(), parser.resourceName(), RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 4 (thetamax) is not numeric",
					p3.lineCount(), parser.resourceName(), RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Sphere [ radius zmin zmax thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: 'Sphere' arguments (radius, zmin, zmax, thetamax) missing",
				p0.lineCount(), parser.resourceName(), RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: 'Sphere' additional arguments ignored",
				p0.lineCount(), parser.resourceName(), RI_NULL);
		}
		if (  p0.typeID() != TYPEID_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: 'Sphere' argument array 1, not all elements are numeric",
				p0.lineCount(), parser.resourceName(), RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			break;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: 'Sphere' could not store arguments",
				p0.lineCount(), parser.resourceName(), RI_NULL);
			break;
		}

		RtFloat radius   = vals[0],
		        zmin     = vals[1],
		        zmax     = vals[2],
		        thetamax = vals[3];

		if ( n > 0 ) {
			parser.ribFilter().sphereV(
				radius, zmin, zmax, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().sphereV(
				radius, zmin, zmax, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: 'Sphere' arguments (radius, zmin, zmax, thetamax) missing",
			parser.lineno(), parser.resourceName(), RI_NULL);
	}
#endif
}


// ----------------------------------------------------------------------------

bool CArchiveParser::canParse(RtString name)
{
	CUri refUri(name);
	if ( !CUri::makeAbsolute(m_absUri, m_baseUri, name, true) )
	{
		return false;
	}
	m_ob.base(m_baseUri);
	return m_ob.open(refUri);
}

bool CArchiveParser::close()
{
	return m_ob.close();
}

void CArchiveParser::putback(unsigned char c) {
	m_hasPutBack = true;
	if ( c == '\n' && m_lineno > 0 )
		--m_lineno;
	m_putback = c;
}


unsigned char CArchiveParser::getchar() {
	unsigned char val;

	if ( m_hasPutBack ) {
		val = m_putback;
		m_hasPutBack = false;
		if ( val == '\n' )
			++m_lineno;
		return val;
	}

	m_istream >> val;
	if ( !m_istream ) {
		val = 0;
	}

	if ( val == '\n' )
		++m_lineno;

	return val;
}

// ----------------------------------------------------------------------------

int CRibRequestData::getTokenList(
	size_t start,
	RtInt vertices,
	RtInt corners,
	RtInt facets,
	RtInt faceVertices,
	RtInt faceCorners)
{
	size_t size = m_parameters.size();
	m_tokenList.clear();
	m_valueList.clear();

	if ( start >= size ) {
		m_tokenList.push_back(0);
		m_valueList.push_back(0);
		return 0;
	}

	m_tokenList.reserve((size-start)/2);
	m_valueList.reserve((size-start)/2);

	// RtInt aClass, aType, aCardinality;

	const char *token = 0;
	void *value = 0;
	size_t i;

	int parameterCount = 0;

	for ( i = start; i < size; ++i ) {

		// Find the parameter name
		if ( !m_parameters[i].getString(token) ) {
			// parameter name must be a string
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badparamlist: '%s' malformed parameter list, parameter name at position %d is not a string",
				m_parameters[i].lineCount(), resourceName(), m_curRequest.c_str(),
				(int)i, RI_NULL);
			continue;
		}

		// Get the value to the parameter, value is always an array, if not convert parameter
		// to an array (try to read parameters until a new string is found - if value is a string
		// and not an array only one value is taken)

		++i;

		if ( i >= size ) {
			// last Token value missing, don't use this parameter
			value = 0;
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, no value found",
				m_parameters[i-1].lineCount(), resourceName(), m_curRequest.c_str(),
				token, (int)i-1, RI_NULL);
			break;
		}

		bool useParameter = true;
		size_t currParam = i;
		m_parameters[currParam].convertIntToFloat(); // if values are integers convert them to floats, else do nothing.

		if ( m_parameters[currParam].typeID() != BASICTYPE_STRING && !m_parameters[currParam].isArray() ) {
			while ( ++i < size ) {
				if ( m_parameters[i].typeID() == BASICTYPE_STRING ) {
					break;
				}
				RtFloat v = 0.0;
				if ( !m_parameters[i].getFloat(v) ) {
					errHandler().handleError(
						RIE_BUG, RIE_ERROR,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't convert value to float, using 0.0",
						m_parameters[i].lineCount(), resourceName(), m_curRequest.c_str(),
						token, (int)i, RI_NULL);
				}
				if ( useParameter && !m_parameters[currParam].setFloat(v) ) {
					errHandler().handleError(
						RIE_BUG, RIE_ERROR,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't set value to float, using 0.0",
						m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
						token, (int)currParam, RI_NULL);
					useParameter = false;
				}
			}
			--i;
			// i == start-1 xor m_parameters[i] contains the next parameter name
		}

		value = m_parameters[currParam].getValue();  // Get the value as a pointer

		if ( useParameter && m_checkParameters ) {
			// Test token/value, if OK push to Paramter list, don't use otherwise
			// RtInt numComps;
			// long int numBytes;
			const CDeclaration *d = stateReader().declFind(token);
			/*
			if ( !m_ricb->queryParam(token, vertices, corners, facets, faceVertices, faceCorners, aClass, aType, aCardinality, numComps, numBytes) ) {
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d has no declaration",
					m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
					token, (int)currParam, RI_NULL);
				// since there is an unknown type, don't use the parameter
				useParameter = false;
			} else {
				// assert(aCardinality>=1)
				// Check the type
				int simpleType = m_parameters[currParam].typeID();
				size_t size = m_parameters[currParam].getCard();
				switch ( aType ) {
					case TYPE_UNKNOWN: useParameter=false; break;
					case TYPE_BOOLEAN: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_INTEGER: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_FLOAT: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_TOKEN: useParameter=(simpleType==TYPEID_STRING); break;
					case TYPE_COLOR: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_POINT: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_VECTOR: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_NORMAL: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_HPOINT: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_MPOINT: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_MATRIX: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_BASIS: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_BOUND: useParameter=(simpleType==TYPEID_FLOAT); break;
					case TYPE_STRING: useParameter=(simpleType==TYPEID_STRING); break;
					case TYPE_POINTER: useParameter=false; break;
					case TYPE_VOID: useParameter=false; break;
					case TYPE_FILTERFUNC: useParameter=false; break;
					case TYPE_ERRORHANDLER: useParameter=false; break;
					case TYPE_PROCSUBDIVFUNC: useParameter=false; break;
					case TYPE_PROCFREEFUNC: useParameter=false; break;
					case TYPE_ARCHIVECALLBACK: useParameter=false; break;
					case TYPE_OBJECTHANDLE: useParameter=false; break;
					case TYPE_LIGHTHANDLE: useParameter=false; break;
					case TYPE_CONTEXTHANDLE: useParameter=false; break;
				}
				if ( !useParameter ) {
					if ( aType == 0 ) {
						errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d not declared or inline syntax error",
							m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, RI_NULL);
					} else {
						errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d type mismatch",
							m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, RI_NULL);
					}
				} else {
					// Check the class/size
					if ( size < (size_t)numComps ) {
						useParameter=false;
						errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d number of values (%d) less then expected (%d)",
							m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, (int)size, (int)numComps, RI_NULL);
					}
					if ( size > (size_t)numComps ) {
						errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d number of values (%d) more then expected (%d), values at end are ignored",
							m_parameters[currParam].lineCount(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, (int)size, (int)numComps, RI_NULL);
					}
				}
			}
			*/
		}

		if ( useParameter ) {
			++parameterCount;
			m_tokenList.push_back(token);
			m_valueList.push_back(value);
		}
	}

	if ( parameterCount == 0 ) {
		m_tokenList.push_back(0);
		m_valueList.push_back(0);
	}

	return parameterCount;
}

// -----------------------------------------------------------------------------

// Various status codes, sequence is important
const int CRibParser::RIBPARSER_REQUEST = 1;
const int CRibParser::RIBPARSER_NORMAL_COMMENT = 2;
const int CRibParser::RIBPARSER_STRUCTURED_COMMENT = 3;
const int CRibParser::RIBPARSER_STRING = 4;
const int CRibParser::RIBPARSER_ARRAY_START = 5;
const int CRibParser::RIBPARSER_ARRAY_END = 6;
const int CRibParser::RIBPARSER_NUMBER = 7;

const int CRibParser::RIBPARSER_NOT_A_TOKEN = 0;
const int CRibParser::RIBPARSER_EOF = -1;

void CRibParser::initRequestMap()
{
	m_requestMap.clear();
	m_requestMap.insert(std::make_pair("Sphere", CSphereRequest()));
}

EnumRequests CRibParser::findIdentifier()
{
	m_token.push_back(0); // Terminate string
	std::map<std::string, CRibRequest>::const_iterator i;
	if ( (i = m_requestMap.find(&m_token[0])) != m_requestMap.end() ) {
		return i->second.getId();
	}
	return REQ_UNKNOWN;
}

bool CRibParser::call(const std::string &request)
{
	std::map<std::string, CRibRequest>::const_iterator i;
	if ( (i = m_requestMap.find(request)) != m_requestMap.end() ) {
		i->second(*this, m_request);
		return true;
	}

	return false;
}



// handleBinary()
// Handle binary encoded RIB -
// some code is taken from readfunc.c found in the affine lib
// by Thomas E. Burge
int CRibParser::handleBinary(unsigned char c) {

	assert(c > 0177);

	if ( c < 0220 ) {           // 0200..0217 encoded integers and fixed numbers
		// 0200 + (d*4) + w | <value>
		int w = (0x03 & c) + 1;     // Number of bytes of <value> 1..4
		int d = (0x0c & c) >> 2;    // Number of bytes after the decimal point 0..w
		int i;

		if ( w < d )
		{
			// Error, more bytes after the decimal point than bytes at all
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", protocolbotch: More bytes after the decimal point than bytes at all",
				lineno(), resourceName(), RI_NULL);
			// skip values
			for ( i = 0; i < w; i++ ) {
				c = getchar();
			}
			return 0;
		}

		// Read all bytes to an unsigned long (w has 1..4 bytes)
		unsigned long tmp = 0;
		for ( i = 0; i < w; i++ ) {
			if ( !m_istream ) {   // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			tmp = tmp << 8;
			tmp |= c;
		}

		i = w*8;
		if  ( tmp & ( 1 << (i-1) )) // value is negative
			tmp |= 0xffffffff << i; // change 0 to 1 in the upper bits

		RtFloat flt = (RtFloat)((double)tmp / (double)(1 << (d * 8)));
		return insertNumber(flt);

	} else if ( c < 0240 ) {    // encoded strings of no more than 15 characters

		// 0220 + w | <ASCII string>, w=[0..15]
		int w = c - 0220;
		m_token.clear();
		m_token.reserve(w+1);
		while ( w-- > 0 ) {
			if ( !m_istream ) {   // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			m_token.push_back(c);
		}
		return handleString();

	} else if ( c < 0244 ) {    // encoded strings longer than 15 characters
		// 0240 + l | <length> | <ASCII string>, l=[0..3]
		// Assign l be the number of bytes needed to specify 
		// the length of the string.
		unsigned int l = c - 0240 + 1;

		/* Read all l number of bytes. */
		unsigned long utmp = 0;
		while ( l-- > 0 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			utmp = utmp << 8;
			utmp |= c;
		}

		m_token.clear();
		m_token.reserve(utmp+1);
		while ( utmp-- > 0 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			m_token.push_back(c);
		}
		return handleString();

	} else if ( c < 0245 ) {    // encoded single precision IEEE floating point value
		// 0244 | <four bytes>
		// sign bit, 8 bit exponent, 23 bit mantissa

		unsigned long tmp = 0;
		int i;
		for ( i = 0; i < 4; i++ ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			tmp = tmp << 8;
			tmp |= c;
		}
		float flt = *((float *)(void*)&tmp);

		return insertNumber((RtFloat)flt);
	} else if ( c < 0246 ) {    // encoded double precision IEEE floating point value
		// 0245 | <eight bytes>
		// sign bit, 11 bit exponent, 52 bit mantissa

		// NOTE:  Encoded doubles don't appear to be used by anybody.  
		//        But the following code was tested once with a hacked 
		//        binary rib file.  Refer to the test file:
		//        affine/ribdump/dbltest.rib.  Use ribdump, typerib,
		//        or Pixar's catrib to view the file since it is in
		//        binary encoded form. (see affine lib)

		unsigned char *v; // value of float accessed as bytes
		double dbl;
		int i;

		v = (unsigned char*)(void*)&dbl;

		// Read double.
#ifdef LITTLE_ENDIAN         
		for ( i = 7; i > -1; i-- )
#else
		for ( i = 0; i < 8; i++ )
#endif        
		{
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			v[i] = c;
		}

		// Storing data as a double doesn't get you much right 
		// now since it is going to be stored as a 32-bit float
		// anyway.  RtFloat is of type float as of prman 3.6c. (see affine lib)
		return insertNumber((RtFloat)dbl);
	} else if ( c < 0247 ) {    // encoded RI request
		// 0246 | <code>
		if ( !m_istream ) {  // EOF is not expected here
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
				lineno(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		EnumRequests op = m_ribEncode[c];
		if ( op == REQ_UNKNOWN ) {
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badtoken: invalid binary token",
				lineno(), resourceName(), RI_NULL);
			return RIBPARSER_NOT_A_TOKEN;
		}
		// return m_ribEncode[c]; // RIBPARSER_NOT_A_TOKEN if Token was not defined or not found
		const char *cptr = CRequestInfo::requestName(op);
		m_token.clear();
		if ( cptr ) {
			m_token.assign(cptr, cptr+strlen(cptr));
		}
		m_token.push_back(0);
		return RIBPARSER_REQUEST;
	} else if ( c < 0310 ) {    // nothing (reserved)
		return RIBPARSER_NORMAL_COMMENT;  // Treat as comment
	} else if ( c < 0314 ) {    // encoded single precision array (length follows)
		int l = c - 0310 + 1;
		int i;
		unsigned long utmp = 0;
		unsigned long tmp = 0;

		while ( l-- > 0 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			utmp = utmp << 8;
			utmp |= c;
		}

		handleArrayStart();
		while ( utmp-- > 0 ) {
			for ( i = 0; i < 4; i++ ) {
				if ( !m_istream ) {  // EOF is not expected here
					errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
						lineno(), resourceName(), RI_NULL);
					return 0;
				}
				c = getchar();
				tmp = tmp << 8;
				tmp |= c;
			}
			float flt = *((float *)(void*)&tmp);
			insertNumber((RtFloat)flt);
		}
		return handleArrayEnd();
	} else if ( c < 0315 ) {    // define encoded request
		// 0314 | code | <string>
		if ( !m_istream ) {  // EOF is not expected here
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
				lineno(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		m_code = c;
		return RIBPARSER_NORMAL_COMMENT; // Treat as comment
	} else if ( c < 0317 ) {    // define encoded string token
		// 0315+w | <token> | string
		int w = c == 0315 ? 1 : 2;
		if ( !m_istream ) {  // EOF is not expected here
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
				lineno(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			tmp = tmp << 8;
			tmp |= c;
		}
		m_defineString = tmp;
		return RIBPARSER_NORMAL_COMMENT;
	} else if ( c < 0321 ) {    // interpolate defined string
		// 0317+w | <token> | string
		int w = c == 0317 ? 1 : 2;
		if ( !m_istream ) {  // EOF is not expected here
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
				lineno(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineno(), resourceName(), RI_NULL);
				return 0;
			}
			c = getchar();
			tmp = tmp << 8;
			tmp |= c;
		}

		NUM2STRING::const_iterator i = m_stringMap.find(tmp);
		if ( i != m_stringMap.end() ) {
			m_token.assign((*i).second.c_str(), (*i).second.c_str()+(*i).second.length());
			return handleString();
		}

		// Error string not found
		errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badstringtoken: Undefined encoded Stiring token",
			lineno(), resourceName(), RI_NULL);
		return RIBPARSER_NORMAL_COMMENT;
	}

	// else c >= 0321
	// nothing (reserved)

	return RIBPARSER_NORMAL_COMMENT; // Treat as comment
}

int CRibParser::handleComment(std::vector<char> &token, bool isStructured)
{
	// If a comment is inbetween an interface call,
	// it is handled after that call
	token.push_back((char)0);

	CComment cc;
	m_deferedCommentList.push_back(cc);

	CComment &c = m_deferedCommentList[m_deferedCommentList.size()-1];
	c.m_comment = token;
	c.m_isStructured = isStructured;
	c.m_lineCount = lineno();

	return isStructured ? RIBPARSER_STRUCTURED_COMMENT : RIBPARSER_NORMAL_COMMENT;
}

void CRibParser::handleDeferedComments() {
	for ( unsigned int i = 0; i < m_deferedCommentList.size(); ++i ) {
		CComment &c = m_deferedCommentList[i];
		if ( c.m_isStructured ) {
			ribFilter().archiveRecordV(RI_STRUCTURE, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(frontend(), RI_STRUCTURE, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			}
		} else {
			ribFilter().archiveRecordV(RI_COMMENT, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(frontend(), RI_COMMENT, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			}
		}
	}
	m_deferedCommentList.clear();
}

int CRibParser::handleString() {
	m_token.push_back((unsigned char)0);
	if ( m_code >= 0 ) {
		if ( m_defineString >= 0 ) {
			// error
			m_defineString = -1;
		}
		// String was a rib call to encode
		EnumRequests op = findIdentifier();
		m_ribEncode[m_code] = op;
		// Error if m_ribEncode[m_code] == REQ_UNKNOWN
		if ( op == REQ_UNKNOWN ) {
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badribcode: invalid encoded RIB request code \"%s\"",
				lineno(), resourceName(), &m_token[0], RI_NULL);
		}
		m_code = -1;    // Clear
		return RIBPARSER_NORMAL_COMMENT;
	}
	if ( m_defineString >= 0 ) {
		m_stringMap[m_defineString] = &m_token[0];
		m_defineString = -1;
		return RIBPARSER_NORMAL_COMMENT;
	}

	if ( m_braketDepth ) {
		CRibParameter &p = m_request.back();
		if ( !p.setString(&m_token[0]) ) {
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Mixed types in array",
				lineno(), resourceName(), RI_NULL);
		}
	} else {
		CRibParameter p;
		p.lineCount(lineno());
		p.setString(&m_token[0]);
		m_request.push_back(p);
	}

	return RIBPARSER_STRING;
}

int CRibParser::handleArrayStart() {
	CRibParameter param;
	param.lineCount(lineno());
	m_request.push_back(param);
	CRibParameter &p = m_request.back();
	p.startArray();
	++m_braketDepth;
	return RIBPARSER_ARRAY_START;
}

int CRibParser::handleArrayEnd() {
	--m_braketDepth;
	if ( m_braketDepth < 0 ) {
		m_braketDepth = 0;
		errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badarray: Too many closing brakets",
			lineno(), resourceName(), RI_NULL);
	}
	return RIBPARSER_ARRAY_END;
}


int CRibParser::insertNumber(RtFloat flt) {
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_request.back();
		if ( !p.setFloat(flt) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Mixed types in array",
				lineno(), resourceName(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineCount(lineno());
		p.setFloat(flt);
		m_request.push_back(p);
	}
	return RIBPARSER_NUMBER;
}

int CRibParser::insertNumber(RtInt num) {
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_request.back();
		if ( !p.setInt(num) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Mixed types in array",
				lineno(), resourceName(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineCount(lineno());
		p.setInt(num);
		m_request.push_back(p);
	}
	return RIBPARSER_NUMBER;
}

int CRibParser::handleNumber(bool isInteger) {
	m_token.push_back((unsigned char)0);

	if ( isInteger ) {
		long l = ::atol(&m_token[0]);
		return insertNumber((RtInt)l);
	}

	RtFloat f = (RtFloat)::atof(&m_token[0]);
	return insertNumber(f);
}

int CRibParser::nextToken() {
	int state = 0;          // the state of the scanner (switch)
	// bool binary = false;    // currently handling binary data
	unsigned char c;        // the character read
	bool mask,              // mask the next character in the string (\ is found) 
		loop;               // will be false if EOF
	char tmp;               // used to evaluate an 'octal' (\xxx) for a string
	int tokenState = 0;     // State variable to identify token

	m_token.clear();        // clear the current

	loop = (m_istream != false); // stop if EOF

	// read the token from m_pifStream
	while ( loop ) {
		if ( !m_istream ) {
			// EOF: finish last request
			c = '\n';
			loop = false;
		} else {
			c = getchar();
			if ( !m_istream ) {
				// EOF: finish last request
				c = '\n';
				loop = false;
			} else {
				if ( c == '\r' )
					continue;
			}
		}

		if (
			c == '#' &&
			state != 0 &&
			state != 6 &&
			state != 7 &&
			state != 8 &&
			state != 9 &&
			state != 10 &&
			state != 11 &&
			state != 12 )
		{
			// 'inline' comment
			{
				std::vector<char> tempToken;
				bool startToken = true;
				bool isStructured = false;
				while ( m_istream ) {
					c = getchar();
					if ( m_istream ) {
						if ( startToken ) {
							isStructured = (c == '#');
							startToken = false;
							continue;
						}
						if ( c == '\r' )
							continue;
						if ( c == '\n' ) {
							// end of comment
							handleComment(tempToken, isStructured);
							break;
						}
						tempToken.push_back(c);
					}
				}
			}

			if ( !m_istream ) {
				// EOF: End of comment
				c = '\n';
				loop = false;
			}
		}

		// handle the c read
		switch ( state ) {
		case 0:
			if ( isspace(c) )   // skip whitespace
				continue;
			if ( c > 127 ) {    // handle a binary value
				int rval = handleBinary(c);
				if ( rval == RIBPARSER_NORMAL_COMMENT ) {
					// Error, but don't care
					continue;
				}
				return rval;
			}
			if ( c == '#' ) {   // a comment (maybe structured comment)
				state = 6;
				continue;
			}
			if ( c == '\"' ) {  // a string
				mask = false;
				state = 8;
				continue;
			}
			if ( c == '[' )     // an array
				return handleArrayStart();
			if ( c == ']' )     // end of an array
				return handleArrayEnd();

			if ( c == '-' || c == '+' ) {   // a number
				state = 2;
				m_token.push_back(c);
				continue;
			}
			if ( c >= '0' && c <= '9' ) {   // a number
				state = 2;
				m_token.push_back(c);
				continue;
			}
			if ( c == '.' ) {               // a number
				state = 3;
				m_token.push_back(c);
				continue;
			}
			if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {   // a RIB token
				state = 1;
				m_token.push_back(c);
				tokenState = 1;
				continue;
			}

			// unknown c, treat as whitespace but warn
			errHandler().handleError(
				RIE_SYNTAX, RIE_WARNING,
				"Line %ld, File \"%s\": Invalid character found '%c', treated as whitespace",
				lineno(), resourceName(), c, RI_NULL);
			break;

		case 1: // Identifier
			{
				if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {   // a RIB token
					m_token.push_back(c);
					continue;
				}

				putback(c);

				m_token.push_back(0); // Terminate string
				m_request.curRequest(&m_token[0]);
				return RIBPARSER_REQUEST;
			}
		case 2: // Number (left of a decimal point)
			if ( c >= '0' && c <= '9' ) {
				m_token.push_back(c);
				continue;
			}
			if ( c == '.' ) {
				state = 3;
				m_token.push_back(c);
				continue;
			}
			if ( c == 'E' || c == 'e' ) {
				state = 4;
				m_token.push_back(c);
				continue;
			}
			putback(c);
			return handleNumber(true);

		case 3: // Number (right of a decimal point)
			if ( c >= '0' && c <= '9' ) {
				m_token.push_back(c);
				continue;
			}
			if ( c == 'E' || c == 'e' ) {
				state = 4;
				m_token.push_back(c);
				continue;
			}
			putback(c);
			return handleNumber(false);

		case 4: // Number (exponent)
			if ( c == '+' || c == '-' ) {
				state = 5;
				m_token.push_back(c);
				continue;
			}
			if ( c >= '0' && c <= '9' ) {
				state = 5;
				m_token.push_back(c);
				continue;
			}
			putback(c);
			return handleNumber(false);

		case 5: // Number (exponent)
			if ( c >= '0' && c <= '9' ) {
				m_token.push_back(c);
				continue;
			}
			putback(c);
			return handleNumber(false);

		case 6: // comment after first '#'
			if ( c == '#' ) {
				state = 7;
				continue;
			}
			if ( c == '\n' ) {
				return handleComment(m_token, false);
			}
			m_token.push_back(c);
			state = 10;
			break;

		case 7: // structured comment
			if ( c == '\n' ) {
				return handleComment(m_token, true);
			}
			m_token.push_back(c);
			break;

		case 8: // string
			if ( c == '\\' ) {
				state = 9;
				continue;
			}
			if ( c == '\r' || c == '\n' ) {
				// error \n in string
				return handleString();
			}
			if ( c == '\"' ) {
				// End of string
				return handleString();
			}
			m_token.push_back(c);
			break;

		case 9: // string ( \. )
			state = 8;
			if ( c == '\n' ) {
				continue;
			}
			if ( c == 'n' ) {
				m_token.push_back('\n');
				continue;
			}
			if ( c == 'r' ) {
				m_token.push_back('\r');
				continue;
			}
			if ( c == 't' ) {
				m_token.push_back('\t');
				continue;
			}
			if ( c == 'b' ) {
				m_token.push_back('\b');
				continue;
			}
			if ( c == 'f' ) {
				m_token.push_back('\f');
				continue;
			}
			if ( c == '\\' ) {
				m_token.push_back('\\');
				continue;
			}
			if ( c == '"' ) {
				m_token.push_back('"');
				continue;
			}
			if ( c >= '0' && c <= '7' ) {
				tmp = c-'0';
				state = 11;
				continue;
			}
			m_token.push_back(c);
			break;

		case 10: // comment
			if ( c == '\n' ) {
				return handleComment(m_token, false);
			}
			m_token.push_back(c);
			break;

		case 11: // Octal 2 in string
			if ( c >= '0' && c <= '7' ) {
				tmp <<= 4;
				tmp |= c-'0';
				state = 12;
				continue;
			} else {
				putback(c);
				m_token.push_back(tmp);
				state = 8;
			}
			break;

		case 12: // Octal 3 in string
			if ( c >= '0' && c <= '7' ) {
				tmp <<= 4;
				tmp |= c-'0';
			} else {
				putback(c);
			}
			m_token.push_back(tmp);
			state = 8;
			break;

		default: // This is never reached
			errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\": Invalid internal parser state %d in \"RtInt TRibParser::nextToken()\"",
				lineno(), resourceName(), state, RI_NULL);
			return 0;
		}
	}

	return RIBPARSER_EOF;
}

int CRibParser::parseNextCall()
{
	// Find first/next call (lookahead)
	while ( m_lookahead != RIBPARSER_EOF && !isRequestToken(m_lookahead) ) {
		m_code = -1;
		m_defineString = -1;
		m_lookahead = nextToken();
		if ( !isValidToken(m_lookahead) ) {
			errHandler().handleError(
				RIE_BADTOKEN, RIE_ERROR,
				"Line %ld, File \"%s\", syntax: bad token \"%s\"",
				lineno(), resourceName(), &m_token[0], RI_NULL);
		} else if ( !(isCommentToken(m_lookahead) || isRequestToken(m_lookahead) || m_lookahead == RIBPARSER_EOF) ) {
			errHandler().handleError(
				RIE_BADTOKEN, RIE_ERROR,
				"Line %ld, File \"%s\", syntax: \"%s\"",
				lineno(), resourceName(), &m_token[0], RI_NULL);
		}
	}
	// Lookahead is eof or an ri token

	// handle any comments found (e.g. the first comments in a file)
	handleDeferedComments();

	int t = m_lookahead; // Call id of the current request, RIBPARSER_EOF if EOF

	// Clear parameter list
	m_braketDepth = 0;
	m_request.clear();

	// If there is a token ( t != EOF ), read the parameters
	if ( t >= 0 ) {
		// find the next token as lookahead
		do {
			m_code = -1;            // encoded request number (binary decoder)
			m_defineString = -1;    // encoded string number (binary decoder)
			m_lookahead = nextToken();  // get the next token
			if ( m_lookahead == RIBPARSER_NOT_A_TOKEN ) {	// Token not found
				errHandler().handleError(
					RIE_BADTOKEN, RIE_ERROR,
					"Line %ld, File \"%s\", syntax: bad token \"%s\"",
					lineno(), resourceName(),
					&m_token[0], RI_NULL);
				break;
			}
		} while ( m_lookahead != RIBPARSER_EOF && !isRequestToken(m_lookahead) ); // while parameters are found
		// m_lookahead is the number of the next request or RIBPARSER_EOF if EOF is found

		if ( m_braketDepth > 0 ) {
			m_braketDepth = 0;
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Missing closing brakets",
				lineno(), resourceName(), RI_NULL);
		}

		if ( m_braketDepth < 0 ) {
			m_braketDepth = 0;
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Too many closing brakets",
				lineno(), resourceName(), RI_NULL);
		}

		// handles the RIB request of the previous look ahead
		if ( !call(m_request.curRequest()) ) {
			// *** Error
		}
		handleDeferedComments(); // handles any comments found
	}

	return m_lookahead;
}

void CRibParser::parseFile() {
	// Clear data array used by the binary decoder
	for ( int i = 0; i < 256; ++i )
		m_ribEncode[i] = REQ_UNKNOWN;

	// Clear the list of strings (binary decoder)
	m_stringMap.clear();

	// Clear handle maps (Handle number -> handle)
	m_mapLightHandle.clear();
	m_mapLightStrHandle.clear();
	m_mapObjectHandle.clear();
	m_mapObjectStrHandle.clear();

	// Clears any defered comments
	m_deferedCommentList.clear();

	m_code = -1;            // No defined token (binary)
	m_defineString = -1;	// No defind string (binary)
	m_lookahead = RIBPARSER_NOT_A_TOKEN;  // Initialize, no Token found

	while ( parseNextCall() != RIBPARSER_EOF ); // Parse all requests

	// Clear the handle maps
	m_mapLightHandle.clear();
	m_mapLightStrHandle.clear();
	m_mapObjectHandle.clear();
	m_mapObjectStrHandle.clear();
}

bool CRibParser::canParse(RtString name)
{
	if ( !CArchiveParser::canParse(name) )
		return false;
	return true;
}

void CRibParser::parse(
	const IArchiveCallback *callback,
	RtInt n,
	RtToken tokens[],
	RtPointer params[])
{
	CArchiveParser::parse(callback, n, tokens, params);
	parseFile();
}
