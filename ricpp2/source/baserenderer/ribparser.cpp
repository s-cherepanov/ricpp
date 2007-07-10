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
	m_cstrVector = NULL;
}

CRibParameter::~CRibParameter() {
	freeValue();

	if ( m_cstrVector ) {
		delete m_cstrVector;
	}
	m_cstrVector = NULL;
}

CRibParameter::CRibParameter(const CRibParameter &p) {
	m_lineCount = 0;
	m_cstrVector = NULL;
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
	m_cstrVector = NULL;

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
		return NULL;

	// return a pointer to the values
	switch ( m_typeID ) {
		case BASICTYPE_UNKNOWN:
			break;
		case BASICTYPE_INTEGER:
			return m_vInt.empty() ? NULL : (void *)&m_vInt[0];
		case BASICTYPE_FLOAT:
			return m_vFloat.empty() ? NULL : (void *)&m_vFloat[0];
		case BASICTYPE_STRING:
			// returns const char **, a pointer to strings
			// get pointers first
			if ( !m_cstrVector ) {
				m_cstrVector = new std::vector<const char *>;
				if ( !m_cstrVector )
					return NULL;
			}
			size_t i;
			size_t size = m_vString.size();
			m_cstrVector->clear();
			m_cstrVector->resize(size);
			for ( i = 0; i < size; ++i ) {
				(*m_cstrVector)[i] = m_vString[i].c_str();
			}
			// returns char **
			return m_cstrVector->empty() ? NULL : (void *)&(*m_cstrVector)[0];
	}
	return NULL;
}


void *CRibParameter::getValue(size_t i) const {
	if ( i >= getCard() )
		return NULL;

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
	return NULL;
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
	virtual void operator()(IRibParserState &parser) const;
	inline virtual EnumRequests getId() const { return REQ_SPHERE; }
}; // CSphereRequest

void CSphereRequest::operator()(IRibParserState &parser) const
{
	// parser.ribFilter().sphereV();
}


// ----------------------------------------------------------------------------

bool CArchiveParser::canParse(RtString name)
{
	return true;
}

void CArchiveParser::putback(unsigned char c) {
	m_hasPutBack = true;
	assert(m_lineno > 0);
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

	if ( val == '\n' )
		++m_lineno;

	return val;
}

// ----------------------------------------------------------------------------

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
		i->second(*this);
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
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: More bytes after the decimal point than bytes at all", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
			return 0;
		}
		c = getchar();
		EnumRequests op = m_ribEncode[c];
		if ( op == REQ_UNKNOWN ) {
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badtoken: invalid binary token", lineno(), absUri().toString().c_str(), RI_NULL);
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
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
					errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
			return 0;
		}
		c = getchar();
		m_code = c;
		return RIBPARSER_NORMAL_COMMENT; // Treat as comment
	} else if ( c < 0317 ) {    // define encoded string token
		// 0315+w | <token> | string
		int w = c == 0315 ? 1 : 2;
		if ( !m_istream ) {  // EOF is not expected here
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", protocolbotch: EOF is not expected here", lineno(), absUri().toString().c_str(), RI_NULL);
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
		errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badstringtoken: Undefined encoded Stiring token", lineno(), absUri().toString().c_str(), RI_NULL);
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
			ribFilter().archiveRecordV(RI_STRUCTURE, c.m_comment.empty() ? NULL : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(frontend(), RI_STRUCTURE, c.m_comment.empty() ? NULL : &c.m_comment[0]);
			}
		} else {
			ribFilter().archiveRecordV(RI_COMMENT, c.m_comment.empty() ? NULL : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(frontend(), RI_COMMENT, c.m_comment.empty() ? NULL : &c.m_comment[0]);
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
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badribcode: invalid encoded RIB request code \"%s\"", lineno(), absUri().toString().c_str(), &m_token[0], RI_NULL);
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
		CRibParameter &p = m_parameters.back();
		if ( !p.setString(&m_token[0]) ) {
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Mixed types in array", lineno(), absUri().toString().c_str(), RI_NULL);
		}
	} else {
		CRibParameter p;
		p.lineCount(lineno());
		p.setString(&m_token[0]);
		m_parameters.push_back(p);
	}

	return RIBPARSER_STRING;
}

int CRibParser::handleArrayStart() {
	CRibParameter param;
	param.lineCount(lineno());
	m_parameters.push_back(param);
	CRibParameter &p = m_parameters.back();
	p.startArray();
	++m_braketDepth;
	return RIBPARSER_ARRAY_START;
}

int CRibParser::handleArrayEnd() {
	--m_braketDepth;
	if ( m_braketDepth < 0 ) {
		m_braketDepth = 0;
		errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Too many closing brakets", lineno(), absUri().toString().c_str(), RI_NULL);
	}
	return RIBPARSER_ARRAY_END;
}


int CRibParser::insertNumber(RtFloat flt) {
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_parameters.back();
		if ( !p.setFloat(flt) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Mixed types in array", lineno(), absUri().toString().c_str(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineCount(lineno());
		p.setFloat(flt);
		m_parameters.push_back(p);
	}
	return RIBPARSER_NUMBER;
}

int CRibParser::insertNumber(RtInt num) {
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_parameters.back();
		if ( !p.setInt(num) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Mixed types in array", lineno(), absUri().toString().c_str(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineCount(lineno());
		p.setInt(num);
		m_parameters.push_back(p);
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
			if ( c == '\r' )
				continue;
			if ( c == 0 ) {
				// EOF: finish last request
				c = '\n';
				loop = false;
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
			errHandler().handleError(RIE_SYNTAX, RIE_WARNING, "Line %ld, File \"%s\": Invalid character found '%c', treated as whitespace", lineno(), absUri().toString().c_str(), c, RI_NULL);
			break;

		case 1: // Identifier
			{
				if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {   // a RIB token
					m_token.push_back(c);
					continue;
				}

				putback(c);

				m_token.push_back(0); // Terminate string
				m_curRequest = &m_token[0];
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
			errHandler().handleError(RIE_BUG, RIE_ERROR, "Line %ld, File \"%s\": Invalid internal parser state %d in \"RtInt TRibParser::nextToken()\"", lineno(), absUri().toString().c_str(), state, RI_NULL);
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
			errHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Line %ld, File \"%s\", syntax: bad token \"%s\"", lineno(), absUri().toString().c_str(), &m_token[0], RI_NULL);
		} else if ( !(isCommentToken(m_lookahead) || isRequestToken(m_lookahead) || m_lookahead == RIBPARSER_EOF) ) {
			errHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Line %ld, File \"%s\", syntax: \"%s\"", lineno(), absUri().toString().c_str(), &m_token[0], RI_NULL);
		}
	}
	// Lookahead is eof or an ri token

	// handle any comments found (e.g. the first comments in a file)
	handleDeferedComments();

	int t = m_lookahead; // Call id of the current request, RIBPARSER_EOF if EOF

	// Clear parameter list
	m_braketDepth = 0;
	m_parameters.clear();

	// If there is a token ( t != EOF ), read the parameters
	if ( t >= 0 ) {
		// find the next token as lookahead
		do {
			m_code = -1;            // encoded request number (binary decoder)
			m_defineString = -1;    // encoded string number (binary decoder)
			m_lookahead = nextToken();  // get the next token
			if ( m_lookahead == RIBPARSER_NOT_A_TOKEN ) {	// Token not found
				errHandler().handleError(RIE_BADTOKEN, RIE_ERROR, "Line %ld, File \"%s\", syntax: bad token \"%s\"", lineno(), absUri().toString().c_str(), &m_token[0], RI_NULL);
				break;
			}
		} while ( m_lookahead != RIBPARSER_EOF && !isRequestToken(m_lookahead) ); // while parameters are found
		// m_lookahead is the number of the next request or RIBPARSER_EOF if EOF is found

		if ( m_braketDepth > 0 ) {
			m_braketDepth = 0;
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Missing closing brakets", lineno(), absUri().toString().c_str(), RI_NULL);
		}

		if ( m_braketDepth < 0 ) {
			m_braketDepth = 0;
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: Too many closing brakets", lineno(), absUri().toString().c_str(), RI_NULL);
		}

		// handles the RIB request of the previous look ahead
		if ( !call(m_curRequest.c_str()) ) {
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
