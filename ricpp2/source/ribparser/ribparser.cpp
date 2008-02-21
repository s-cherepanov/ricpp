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

/** @file ribparser.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the parser for rib
 */

// #include "ricpp/ribparser/ribparser.h"

#ifndef _RICPP_RIBPARSER_RIBMISC_H
#include "ricpp/ribparser/ribmisc.h"
#endif // _RICPP_RIBPARSER_RIBMISC_H

#ifndef _RICPP_RIBPARSER_RIBMODES_H
#include "ricpp/ribparser/ribmodes.h"
#endif // _RICPP_RIBPARSER_RIBMODES_H

#ifndef _RICPP_RIBPARSER_RIBOPTIONS_H
#include "ricpp/ribparser/riboptions.h"
#endif // _RICPP_RIBPARSER_RIBOPTIONS_H

#ifndef _RICPP_RIBPARSER_RIBATTRIBUTES_H
#include "ricpp/ribparser/ribattributes.h"
#endif // _RICPP_RIBPARSER_RIBATTRIBUTES_H

#ifndef _RICPP_RIBPARSER_RIBLIGHTS_H
#include "ricpp/ribparser/riblights.h"
#endif // _RICPP_RIBPARSER_RIBLIGHTS_H

#ifndef _RICPP_RIBPARSER_RIBTRANSFORMS_H
#include "ricpp/ribparser/ribtransforms.h"
#endif // _RICPP_RIBPARSER_RIBTRANSFORMS_H

#ifndef _RICPP_RIBPARSER_RIBPRIMS_H
#include "ricpp/ribparser/ribprims.h"
#endif // _RICPP_RIBPARSER_RIBPRIMS_H

#ifndef _RICPP_RIBPARSER_RIBEXTERNALS_H
#include "ricpp/ribparser/ribexternals.h"
#endif // _RICPP_RIBPARSER_RIBEXTERNALS_H

#ifndef _RICPP_RICPP_PARAMCLASSES_H
#include "ricpp/ricpp/paramclasses.h"
#endif // _RICPP_RICPP_PARAMCLASSES_H

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;

// -----------------------------------------------------------------------------

// Various status codes, sequence number is important
const int CRibParser::RIBPARSER_REQUEST = 1;
const int CRibParser::RIBPARSER_NORMAL_COMMENT = 2;
const int CRibParser::RIBPARSER_STRUCTURED_COMMENT = 3;
const int CRibParser::RIBPARSER_STRING = 4;
const int CRibParser::RIBPARSER_ARRAY_START = 5;
const int CRibParser::RIBPARSER_ARRAY_END = 6;
const int CRibParser::RIBPARSER_NUMBER = 7;

const int CRibParser::RIBPARSER_NOT_A_TOKEN = 0;
const int CRibParser::RIBPARSER_EOF = -1;

std::map<std::string, CRibRequest *> CRibParser::s_requestMap;

// ----------------------------------------------------------------------------

CRibParameter::CRibParameter()
{
	m_lineNo = 1;
	m_isArray = false;
	m_typeID = BASICTYPE_UNKNOWN;
	m_cstrVector = 0;
}


CRibParameter::~CRibParameter()
{
	freeValue();

	if ( m_cstrVector ) {
		delete m_cstrVector;
	}
	m_cstrVector = 0;
}


CRibParameter::CRibParameter(const CRibParameter &p)
{
	m_lineNo = 1;
	m_cstrVector = 0;
	m_isArray = false;
	m_typeID = BASICTYPE_UNKNOWN;
	*this = p;
}


void CRibParameter::freeValue()
{
	m_vInt.clear();
	m_vFloat.clear();
	m_vString.clear();
}


CRibParameter &CRibParameter::operator=(const CRibParameter &p)
{
	if ( &p == this )
		return *this;

	freeValue();

	m_lineNo = p.m_lineNo;

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


void *CRibParameter::getValue()
{
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


void *CRibParameter::getValue(size_t i) const
{
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


size_t CRibParameter::getCard() const
{

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


bool CRibParameter::convertIntToFloat()
{
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


bool CRibRequestData::removePair(size_t start, RtToken token)
{
	size_t size = m_parameters.size();
	if ( start >= size ) {
		return false;
	}
	
	RtToken t, t2;

	std::vector<CRibParameter>::iterator sav;
	std::vector<CRibParameter>::iterator iter = m_parameters.begin();
	size_t i = 0;
	for ( ; i < start; ++i ) {
		++iter;
	}
	
	while ( iter != m_parameters.end() ) {
		if ( !(*iter).getString(t) ) {
			// parameter name must be a string
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badparamlist: '%s' malformed parameter list, parameter name at position %d is not a string",
				(*iter).lineNo(), resourceName(), m_curRequest.c_str(),
				(int)i, RI_NULL);
			++iter; ++i;
			continue;
		}
		
		std::vector<CRibParameter>::iterator sav=iter;

		++iter; ++i;
		if ( !(*iter).getString(t2) ) {
			++iter; ++i;
			while ( !(*iter).getString(t2) ) {
				++iter; ++i;
			}
		} else {
			++iter; ++i;
		}

		if ( t == token ) {
			m_parameters.erase(sav, iter);
			return true;
		}
	}
	
	return false;
}

int CRibRequestData::getTokenList(
	size_t start,
	const char *aQualifier,
	const char *table, 
	RtInt vertices,
	RtInt corners,
	RtInt facets,
	RtInt faceVertices,
	RtInt faceCorners)
{
	CValueCounts valueCounts(vertices, corners, facets, faceVertices, faceCorners);
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
	int parameterPos = 0;

	for ( i = start; i < size; ++i, ++parameterPos ) {

		// Find the parameter name
		if ( !m_parameters[i].getString(token) ) {
			// parameter name must be a string
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badparamlist: '%s' malformed parameter list, parameter name at position %d is not a string",
				m_parameters[i].lineNo(), resourceName(), m_curRequest.c_str(),
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
				m_parameters[i-1].lineNo(), resourceName(), m_curRequest.c_str(),
				token, (int)i-1, RI_NULL);
			break;
		}

		bool useParameter = true;
		size_t currParam = i;

		// If Parameter is a string take only one
		if ( m_parameters[currParam].typeID() != BASICTYPE_STRING && !m_parameters[currParam].isArray() ) {
			// Copy numeric parameters until a string has been found.
			while ( ++i < size ) {
				if ( m_parameters[i].typeID() == BASICTYPE_STRING ) {
					break;
				}
				
				if ( m_parameters[i].typeID() == BASICTYPE_FLOAT || m_parameters[currParam].typeID() == BASICTYPE_FLOAT ) {
				    // If one float has been found take all as float
					if ( m_parameters[currParam].typeID() == BASICTYPE_INTEGER ) {
						m_parameters[currParam].convertIntToFloat();
					}
					RtFloat v = 0.0;
					if ( !m_parameters[i].getFloat(v) ) {
						errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't convert value to float, using 0.0 instaead",
							m_parameters[i].lineNo(), resourceName(), m_curRequest.c_str(),
							token, (int)i, RI_NULL);
					}
					if ( useParameter && !m_parameters[currParam].setFloat(v) ) {
						errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't set value to float, skip parameter",
							m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, RI_NULL);
						useParameter = false;
					}
				} else if ( m_parameters[i].typeID() == BASICTYPE_INTEGER ) {
					RtInt v = 0;
					if ( !m_parameters[i].getInt(v) ) {
						errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't convert value to integer, using 0 instead",
							m_parameters[i].lineNo(), resourceName(), m_curRequest.c_str(),
							token, (int)i, RI_NULL);
					}
					if ( useParameter && !m_parameters[currParam].setInt(v) ) {
						errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't set value to integer, skip parameter",
							m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
							token, (int)currParam, RI_NULL);
						useParameter = false;
					}
				} else {
					// An unknown type
					errHandler().handleError(
						RIE_BUG, RIE_ERROR,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d, couldn't find type, skip parameter",
						m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
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
			CParameter p;
			p.setDeclaration(aQualifier, table, token, parameterPos, valueCounts, renderState().dict(), renderState().options().colorDescr());
			RtInt numComps = p.declaration().selectNumberOf(valueCounts) * p.declaration().elemSize();
			// unsigned long int numBytes = numComps * p.declaration().basicTypeByteSize();

			EnumBasicTypes basicType = m_parameters[currParam].typeID();
			size_t size = m_parameters[currParam].getCard();

			// Convert float to int, if needed
			if ( basicType == BASICTYPE_INTEGER && p.declaration().basicType() == BASICTYPE_FLOAT ) {
				m_parameters[currParam].convertIntToFloat();
				basicType = m_parameters[currParam].typeID();
				value = m_parameters[currParam].getValue();  // Get the value as a pointer
			}

			// Convert int to float with warning
			if ( basicType == BASICTYPE_FLOAT && p.declaration().basicType() == BASICTYPE_INTEGER ) {
				m_parameters[currParam].convertFloatToInt();
				basicType = m_parameters[currParam].typeID();
				value = m_parameters[currParam].getValue();  // Get the value as a pointer
					errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d integer expected, but float found, converting float to integer",
						m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
						token, (int)currParam, RI_NULL);
			}

			if ( basicType != p.declaration().basicType() ) {
				useParameter=false;
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d type mismatch, %s found vs. %s expected",
					m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
					token, (int)parameterPos, CTypeInfo::basicTypeName(basicType), CTypeInfo::basicTypeName(p.declaration().basicType()), RI_NULL);
			} else {
				// Check the class/size
				if ( size < (size_t)numComps ) {
					useParameter=false;
					errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d number of values (%d) less then expected (%d)",
						m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
						token, (int)currParam, (int)size, (int)numComps, RI_NULL);
				}
				if ( size > (size_t)numComps ) {
					errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badparamlist: '%s', parameter name '%s' at position %d number of values (%d) more then expected (%d), values at end are ignored",
						m_parameters[currParam].lineNo(), resourceName(), m_curRequest.c_str(),
						token, (int)currParam, (int)size, (int)numComps, RI_NULL);
				}
			}
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

RtInt CRibRequestData::numVertices(RtInt start, RtInt n)
{
	if ( (int)m_tokenList.size() > n )
		n = (int)m_tokenList.size();

	int i;
	for ( i = 0; i < n; ++i ) {
		if ( !strcmp(m_tokenList[i], RI_P ) ) {
			CRibParameter &p = m_parameters[start + 2*i + 1];
			return (RtInt)(p.getCard()/3);
		}
		if ( !strcmp(m_tokenList[i], RI_PW ) ) {
			CRibParameter &p = m_parameters[start + 2*i + 1];
			return (RtInt)(p.getCard()/4);
		}
		if ( !strcmp(m_tokenList[i], RI_PZ ) ) {
			CRibParameter &p = m_parameters[start + 2*i + 1];
			return (RtInt)(p.getCard());
		}
	}

	return 0;
}


// ----------------------------------------------------------------------------


void CRibParser::putback(unsigned char c)
{
	m_hasPutBack = true;
	if ( c == '\n' && m_lineNo > 0 )
		--m_lineNo;
	m_putBack = c;
}


unsigned char CRibParser::getchar()
{
	unsigned char val;

	if ( m_hasPutBack ) {
		val = m_putBack;
		m_hasPutBack = false;
		if ( val == '\n' )
			++m_lineNo;
		return val;
	}

	val = m_istream.get();
	if ( !m_istream ) {
		val = 0;
	}

	if ( val == '\n' )
		++m_lineNo;

	return val;
}


bool CRibParser::bindObjectHandle(RtObjectHandle handle, RtInt number)
{
	m_mapObjectHandle[number] = handle;
	return true;
}


bool CRibParser::bindObjectHandle(RtObjectHandle handle, const char *name)
{
	m_mapObjectStrHandle[name] = handle;
	return true;
}


bool CRibParser::getObjectHandle(RtObjectHandle &handle, RtInt number) const
{
	NUM2OBJECT::const_iterator i = m_mapObjectHandle.find(number);
	if ( i != m_mapObjectHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}


bool CRibParser::getObjectHandle(RtObjectHandle &handle, const char *name) const
{
	STR2OBJECT::const_iterator i = m_mapObjectStrHandle.find(name);
	if ( i != m_mapObjectStrHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}


bool CRibParser::bindLightHandle(RtLightHandle handle, RtInt number)
{
	m_mapLightHandle[number] = handle;
	return true;
}


bool CRibParser::bindLightHandle(RtLightHandle handle, const char *handleName)
{
	m_mapLightStrHandle[handleName] = handle;
	return true;
}


bool CRibParser::getLightHandle(RtLightHandle &handle, RtInt number) const
{
	NUM2LIGHT::const_iterator i = m_mapLightHandle.find(number);
	if ( i != m_mapLightHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}


bool CRibParser::getLightHandle(RtLightHandle &handle, const char *handleName) const
{
	STR2LIGHT::const_iterator i = m_mapLightStrHandle.find(handleName);
	if ( i != m_mapLightStrHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}


/*
bool CRibParser::bindArchiveHandle(RtArchiveHandle handle, RtInt number)
{
	m_mapArchiveHandle[number] = handle;
	return true;
}
*/


bool CRibParser::bindArchiveHandle(RtArchiveHandle handle, const char *name)
{
	m_mapArchiveStrHandle[name] = handle;
	return true;
}


/*
bool CRibParser::getArchiveHandle(RtArchiveHandle &handle, RtInt number) const
{
	NUM2OBJECT::const_iterator i = m_mapArchiveHandle.find(number);
	if ( i != m_mapArchiveHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}
*/


bool CRibParser::getArchiveHandle(RtArchiveHandle &handle, const char *name) const
{
	STR2OBJECT::const_iterator i = m_mapArchiveStrHandle.find(name);
	if ( i != m_mapArchiveStrHandle.end() ) {
		handle = (*i).second;
		return true;
	}

	return false;
}

void CRibParser::initRequestMap()
{
	if ( s_requestMap.empty() ) {

		static CErrorHandlerRibRequest errorHandler;
		s_requestMap.insert(std::make_pair(errorHandler.requestName(), &errorHandler));

		static CDeclareRibRequest declare;
		s_requestMap.insert(std::make_pair(declare.requestName(), &declare));

		static CReadArchiveRibRequest readArchive;
		s_requestMap.insert(std::make_pair(readArchive.requestName(), &readArchive));

		static CVersionRibRequest version;
		s_requestMap.insert(std::make_pair(version.requestName(), &version));

		static CSystemRibRequest systemReq;
		s_requestMap.insert(std::make_pair(systemReq.requestName(), &systemReq));

		static CResourceBeginRibRequest resourceBegin; 
		s_requestMap.insert(std::make_pair(resourceBegin.requestName(), &resourceBegin));

		static CResourceEndRibRequest resourceEnd; 
		s_requestMap.insert(std::make_pair(resourceEnd.requestName(), &resourceEnd));

		static CResourceRibRequest resource; 
		s_requestMap.insert(std::make_pair(resource.requestName(), &resource));

		static CFrameBeginRibRequest frameBegin; 
		s_requestMap.insert(std::make_pair(frameBegin.requestName(), &frameBegin));

		static CFrameEndRibRequest frameEnd; 
		s_requestMap.insert(std::make_pair(frameEnd.requestName(), &frameEnd));

		static CWorldBeginRibRequest worldBegin; 
		s_requestMap.insert(std::make_pair(worldBegin.requestName(), &worldBegin));

		static CWorldEndRibRequest worldEnd; 
		s_requestMap.insert(std::make_pair(worldEnd.requestName(), &worldEnd));

		static CAttributeBeginRibRequest attributeBegin; 
		s_requestMap.insert(std::make_pair(attributeBegin.requestName(), &attributeBegin));

		static CAttributeEndRibRequest attributeEnd; 
		s_requestMap.insert(std::make_pair(attributeEnd.requestName(), &attributeEnd));

		static CTransformBeginRibRequest transformBegin; 
		s_requestMap.insert(std::make_pair(transformBegin.requestName(), &transformBegin));

		static CTransformEndRibRequest transformEnd; 
		s_requestMap.insert(std::make_pair(transformEnd.requestName(), &transformEnd));

		static CSolidBeginRibRequest solidBegin; 
		s_requestMap.insert(std::make_pair(solidBegin.requestName(), &solidBegin));

		static CSolidEndRibRequest solidEnd; 
		s_requestMap.insert(std::make_pair(solidEnd.requestName(), &solidEnd));

		static CObjectBeginRibRequest objectBegin; 
		s_requestMap.insert(std::make_pair(objectBegin.requestName(), &objectBegin));

		static CObjectEndRibRequest objectEnd; 
		s_requestMap.insert(std::make_pair(objectEnd.requestName(), &objectEnd));

		static CObjectInstanceRibRequest objectInstance; 
		s_requestMap.insert(std::make_pair(objectInstance.requestName(), &objectInstance));

		static CArchiveBeginRibRequest archiveBegin; 
		s_requestMap.insert(std::make_pair(archiveBegin.requestName(), &archiveBegin));

		static CArchiveEndRibRequest archiveEnd; 
		s_requestMap.insert(std::make_pair(archiveEnd.requestName(), &archiveEnd));

		static CMotionBeginRibRequest motionBegin; 
		s_requestMap.insert(std::make_pair(motionBegin.requestName(), &motionBegin));

		static CMotionEndRibRequest motionEnd; 
		s_requestMap.insert(std::make_pair(motionEnd.requestName(), &motionEnd));

		static CIfBeginRibRequest ifBegin; 
		s_requestMap.insert(std::make_pair(ifBegin.requestName(), &ifBegin));

		static CElseIfRibRequest elseIf; 
		s_requestMap.insert(std::make_pair(elseIf.requestName(), &elseIf));

		static CElseRibRequest elsePart; 
		s_requestMap.insert(std::make_pair(elsePart.requestName(), &elsePart));

		static CIfEndRibRequest ifEnd; 
		s_requestMap.insert(std::make_pair(ifEnd.requestName(), &ifEnd));

		static CFormatRibRequest format;
		s_requestMap.insert(std::make_pair(format.requestName(), &format));

		static CFrameAspectRatioRibRequest frameAspectRatio;
		s_requestMap.insert(std::make_pair(frameAspectRatio.requestName(), &frameAspectRatio));

		static CScreenWindowRibRequest screenWindow;
		s_requestMap.insert(std::make_pair(screenWindow.requestName(), &screenWindow));

		static CCropWindowRibRequest cropWindow;
		s_requestMap.insert(std::make_pair(cropWindow.requestName(), &cropWindow));

		static CProjectionRibRequest projection;
		s_requestMap.insert(std::make_pair(projection.requestName(), &projection));

		static CClippingRibRequest clipping;
		s_requestMap.insert(std::make_pair(clipping.requestName(), &clipping));

		static CClippingPlaneRibRequest clippingPlane;
		s_requestMap.insert(std::make_pair(clippingPlane.requestName(), &clippingPlane));

		static CDepthOfFieldRibRequest depthOfField;
		s_requestMap.insert(std::make_pair(depthOfField.requestName(), &depthOfField));

		static CShutterRibRequest shutter;
		s_requestMap.insert(std::make_pair(shutter.requestName(), &shutter));

		static CPixelVarianceRibRequest pixelVariance;
		s_requestMap.insert(std::make_pair(pixelVariance.requestName(), &pixelVariance));

		static CPixelSamplesRibRequest pixelSamples;
		s_requestMap.insert(std::make_pair(pixelSamples.requestName(), &pixelSamples));

		static CPixelFilterRibRequest pixelFilter;
		s_requestMap.insert(std::make_pair(pixelFilter.requestName(), &pixelFilter));

		static CExposureRibRequest exposure;
		s_requestMap.insert(std::make_pair(exposure.requestName(), &exposure));

		static CImagerRibRequest imager;
		s_requestMap.insert(std::make_pair(imager.requestName(), &imager));

		static CQuantizeRibRequest quantize;
		s_requestMap.insert(std::make_pair(quantize.requestName(), &quantize));

		static CDisplayChannelRibRequest displayChannel;
		s_requestMap.insert(std::make_pair(displayChannel.requestName(), &displayChannel));

		static CDisplayRibRequest display;
		s_requestMap.insert(std::make_pair(display.requestName(), &display));

		static CHiderRibRequest hider;
		s_requestMap.insert(std::make_pair(hider.requestName(), &hider));

		static CColorSamplesRibRequest colorSamples;
		s_requestMap.insert(std::make_pair(colorSamples.requestName(), &colorSamples));

		static CRelativeDetailRibRequest relativeDetail;
		s_requestMap.insert(std::make_pair(relativeDetail.requestName(), &relativeDetail));

		static COptionRibRequest option;
		s_requestMap.insert(std::make_pair(option.requestName(), &option));

		static CAttributeRibRequest attribute; 
		s_requestMap.insert(std::make_pair(attribute.requestName(), &attribute));

		static CColorRibRequest color; 
		s_requestMap.insert(std::make_pair(color.requestName(), &color));

		static COpacityRibRequest opacity; 
		s_requestMap.insert(std::make_pair(opacity.requestName(), &opacity));

		static CSurfaceRibRequest surface; 
		s_requestMap.insert(std::make_pair(surface.requestName(), &surface));

		static CAtmosphereRibRequest atmosphere; 
		s_requestMap.insert(std::make_pair(atmosphere.requestName(), &atmosphere));

		static CInteriorRibRequest interior; 
		s_requestMap.insert(std::make_pair(interior.requestName(), &interior));

		static CExteriorRibRequest exterior; 
		s_requestMap.insert(std::make_pair(exterior.requestName(), &exterior));

		static CDisplacementRibRequest displacement; 
		s_requestMap.insert(std::make_pair(displacement.requestName(), &displacement));

		static CTextureCoordinatesRibRequest textureCoordinates; 
		s_requestMap.insert(std::make_pair(textureCoordinates.requestName(), &textureCoordinates));

		static CShadingRateRibRequest shadingRate; 
		s_requestMap.insert(std::make_pair(shadingRate.requestName(), &shadingRate));

		static CShadingInterpolationRibRequest shadingInterpolation; 
		s_requestMap.insert(std::make_pair(shadingInterpolation.requestName(), &shadingInterpolation));

		static CMatteRibRequest matte; 
		s_requestMap.insert(std::make_pair(matte.requestName(), &matte));

		static CBoundRibRequest bound; 
		s_requestMap.insert(std::make_pair(bound.requestName(), &bound));

		static CDetailRibRequest detail; 
		s_requestMap.insert(std::make_pair(detail.requestName(), &detail));

		static CDetailRangeRibRequest detailRange; 
		s_requestMap.insert(std::make_pair(detailRange.requestName(), &detailRange));

		static CGeometricApproximationRibRequest geometricApproximation; 
		s_requestMap.insert(std::make_pair(geometricApproximation.requestName(), &geometricApproximation));

		static CGeometricRepresentationRibRequest geometricRepresentation; 
		s_requestMap.insert(std::make_pair(geometricRepresentation.requestName(), &geometricRepresentation));

		static COrientationRibRequest orientation; 
		s_requestMap.insert(std::make_pair(orientation.requestName(), &orientation));

		static CReverseOrientationRibRequest reverseOrientation; 
		s_requestMap.insert(std::make_pair(reverseOrientation.requestName(), &reverseOrientation));

		static CSidesRibRequest sides; 
		s_requestMap.insert(std::make_pair(sides.requestName(), &sides));

		static CBasisRibRequest basis; 
		s_requestMap.insert(std::make_pair(basis.requestName(), &basis));

		static CTrimCurveRibRequest trimCurve; 
		s_requestMap.insert(std::make_pair(trimCurve.requestName(), &trimCurve));

		static CIdentityRibRequest identity;
		s_requestMap.insert(std::make_pair(identity.requestName(), &identity));

		static CTransformRibRequest transform;
		s_requestMap.insert(std::make_pair(transform.requestName(), &transform));

		static CConcatTransformRibRequest concatTransform;
		s_requestMap.insert(std::make_pair(concatTransform.requestName(), &concatTransform));

		static CPerspectiveRibRequest perspective;
		s_requestMap.insert(std::make_pair(perspective.requestName(), &perspective));

		static CTranslateRibRequest translate;
		s_requestMap.insert(std::make_pair(translate.requestName(), &translate));

		static CRotateRibRequest rotate;
		s_requestMap.insert(std::make_pair(rotate.requestName(), &rotate));

		static CScaleRibRequest scale;
		s_requestMap.insert(std::make_pair(scale.requestName(), &scale));

		static CSkewRibRequest skew;
		s_requestMap.insert(std::make_pair(skew.requestName(), &skew));

		static CDeformationRibRequest deformation;
		s_requestMap.insert(std::make_pair(deformation.requestName(), &deformation));

		static CScopedCoordinateSystemRibRequest scopedCoordinateSystem;
		s_requestMap.insert(std::make_pair(scopedCoordinateSystem.requestName(), &scopedCoordinateSystem));

		static CCoordinateSystemRibRequest coordinateSystem;
		s_requestMap.insert(std::make_pair(coordinateSystem.requestName(), &coordinateSystem));

		static CCoordSysTransformRibRequest coordSysTransform;
		s_requestMap.insert(std::make_pair(coordSysTransform.requestName(), &coordSysTransform));

		static CTransformPointsRibRequest transformPoints;
		s_requestMap.insert(std::make_pair(transformPoints.requestName(), &transformPoints));

		static CLightSourceRibRequest lightSource;
		s_requestMap.insert(std::make_pair(lightSource.requestName(), &lightSource));

		static CAreaLightSourceRibRequest areaLightSource;
		s_requestMap.insert(std::make_pair(areaLightSource.requestName(), &areaLightSource));

		static CIlluminateRibRequest illuminate;
		s_requestMap.insert(std::make_pair(illuminate.requestName(), &illuminate));

		static CPolygonRibRequest polygon;
		s_requestMap.insert(std::make_pair(polygon.requestName(), &polygon));

		static CGeneralPolygonRibRequest generalPolygon;
		s_requestMap.insert(std::make_pair(generalPolygon.requestName(), &generalPolygon));

		static CPointsPolygonsRibRequest pointsPolygon;
		s_requestMap.insert(std::make_pair(pointsPolygon.requestName(), &pointsPolygon));

		static CPointsGeneralPolygonsRibRequest pointsGeneralPolygon;
		s_requestMap.insert(std::make_pair(pointsGeneralPolygon.requestName(), &pointsGeneralPolygon));

		static CPatchRibRequest patch;
		s_requestMap.insert(std::make_pair(patch.requestName(), &patch));

		static CPatchMeshRibRequest patchMesh;
		s_requestMap.insert(std::make_pair(patchMesh.requestName(), &patchMesh));

		static CNuPatchRibRequest nuPatch;
		s_requestMap.insert(std::make_pair(nuPatch.requestName(), &nuPatch));

		static CSubdivisionMeshRibRequest subdivisionMesh;
		s_requestMap.insert(std::make_pair(subdivisionMesh.requestName(), &subdivisionMesh));

		static CHierarchicalSubdivisionMeshRibRequest hierarchicalSubdivisionMesh;
		s_requestMap.insert(std::make_pair(hierarchicalSubdivisionMesh.requestName(), &hierarchicalSubdivisionMesh));

		static CSphereRibRequest sphere; 
		s_requestMap.insert(std::make_pair(sphere.requestName(), &sphere));

		static CConeRibRequest cone;
		s_requestMap.insert(std::make_pair(cone.requestName(), &cone));

		static CCylinderRibRequest cylinder;
		s_requestMap.insert(std::make_pair(cylinder.requestName(), &cylinder));

		static CHyperboloidRibRequest hyperboloid;
		s_requestMap.insert(std::make_pair(hyperboloid.requestName(), &hyperboloid));

		static CParaboloidRibRequest paraboloid;
		s_requestMap.insert(std::make_pair(paraboloid.requestName(), &paraboloid));

		static CDiskRibRequest disk;
		s_requestMap.insert(std::make_pair(disk.requestName(), &disk));

		static CTorusRibRequest torus;
		s_requestMap.insert(std::make_pair(torus.requestName(), &torus));

		static CPointsRibRequest points;
		s_requestMap.insert(std::make_pair(points.requestName(), &points));

		static CCurvesRibRequest curves;
		s_requestMap.insert(std::make_pair(curves.requestName(), &curves));

		static CBlobbyRibRequest blobby;
		s_requestMap.insert(std::make_pair(blobby.requestName(), &blobby));

		static CProceduralRibRequest procedural;
		s_requestMap.insert(std::make_pair(procedural.requestName(), &procedural));

		static CGeometryRibRequest geometry;
		s_requestMap.insert(std::make_pair(geometry.requestName(), &geometry));

		static CMakeTextureRibRequest makeTexture;
		s_requestMap.insert(std::make_pair(makeTexture.requestName(), &makeTexture));

		static CMakeBumpRibRequest makeBump;
		s_requestMap.insert(std::make_pair(makeBump.requestName(), &makeBump));

		static CMakeLatLongEnvironmentRibRequest makeLatLongEnvironment;
		s_requestMap.insert(std::make_pair(makeLatLongEnvironment.requestName(), &makeLatLongEnvironment));

		static CMakeCubeFaceEnvironmentRibRequest makeCubeFaceEnvironment;
		s_requestMap.insert(std::make_pair(makeCubeFaceEnvironment.requestName(), &makeCubeFaceEnvironment));

		static CMakeShadowRibRequest makeShadow;
		s_requestMap.insert(std::make_pair(makeShadow.requestName(), &makeShadow));

		static CMakeBrickMapRibRequest makeBrickMap;
		s_requestMap.insert(std::make_pair(makeBrickMap.requestName(), &makeBrickMap));
	}
}


EnumRequests CRibParser::findIdentifier()
{
	m_token.push_back(0); // Terminate string
	std::map<std::string, CRibRequest *>::const_iterator i;
	if ( (i = s_requestMap.find(&m_token[0])) != s_requestMap.end() ) {
		if ( i->second )
			return (i->second)->interfaceIdx();
	}
	return REQ_UNKNOWN;
}


bool CRibParser::call(const std::string &request)
{
	std::map<std::string, CRibRequest *>::const_iterator i;
	if ( (i = s_requestMap.find(request)) != s_requestMap.end() ) {
		if ( i->second ) {
			(*(i->second))(*this, m_request);
			return true;
		}
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
				lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
				lineNo(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		EnumRequests op = m_ribEncode[c];
		if ( op == REQ_UNKNOWN ) {
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badtoken: invalid binary token",
				lineNo(), resourceName(), RI_NULL);
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
					lineNo(), resourceName(), RI_NULL);
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
						lineNo(), resourceName(), RI_NULL);
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
				lineNo(), resourceName(), RI_NULL);
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
				lineNo(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineNo(), resourceName(), RI_NULL);
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
				lineNo(), resourceName(), RI_NULL);
			return 0;
		}
		c = getchar();
		unsigned long tmp = c;
		if ( w == 2 ) {
			if ( !m_istream ) {  // EOF is not expected here
				errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", protocolbotch: EOF is not expected here",
					lineNo(), resourceName(), RI_NULL);
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
			lineNo(), resourceName(), RI_NULL);
		return RIBPARSER_NORMAL_COMMENT;
	}

	// else c >= 0321
	// nothing (reserved)

	return RIBPARSER_NORMAL_COMMENT; // Treat as comment
}


int CRibParser::handleComment(TOKENTYPE &token, bool isStructured)
{
	// If a comment is inbetween an interface call,
	// it is handled after that call
	token.push_back((char)0);

	CComment cc;
	m_deferedCommentList.push_back(cc);

	CComment &c = m_deferedCommentList[m_deferedCommentList.size()-1];
	c.m_comment = token;
	c.m_isStructured = isStructured;
	c.m_lineNo = lineNo();

	return isStructured ? RIBPARSER_STRUCTURED_COMMENT : RIBPARSER_NORMAL_COMMENT;
}


void CRibParser::handleDeferedComments()
{
	for ( unsigned int i = 0; i < m_deferedCommentList.size(); ++i ) {
		CComment &c = m_deferedCommentList[i];
		if ( c.m_isStructured ) {
			ribFilter().archiveRecordV(RI_STRUCTURE, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(ribFilter(), RI_STRUCTURE, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			}
		} else {
			ribFilter().archiveRecordV(RI_COMMENT, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			if ( callback() ) {
				(*callback())(ribFilter(), RI_COMMENT, c.m_comment.empty() ? 0 : &c.m_comment[0]);
			}
		}
	}
	m_deferedCommentList.clear();
}


int CRibParser::handleString()
{
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
				lineNo(), resourceName(), &m_token[0], RI_NULL);
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
				lineNo(), resourceName(), RI_NULL);
		}
	} else {
		CRibParameter p;
		p.lineNo(lineNo());
		// Handle RIB String Variables, if Option "rib" "string varsubst" ["$"]
		if ( m_renderState )
			m_renderState->varSubst(m_token);
		p.setString(&m_token[0]);
		m_request.push_back(p);
	}

	return RIBPARSER_STRING;
}


int CRibParser::handleArrayStart()
{
	CRibParameter param;
	param.lineNo(lineNo());
	m_request.push_back(param);
	CRibParameter &p = m_request.back();
	p.startArray();
	++m_braketDepth;
	return RIBPARSER_ARRAY_START;
}


int CRibParser::handleArrayEnd()
{
	--m_braketDepth;
	if ( m_braketDepth < 0 ) {
		m_braketDepth = 0;
		errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badarray: Too many closing brakets",
			lineNo(), resourceName(), RI_NULL);
	}
	return RIBPARSER_ARRAY_END;
}


int CRibParser::insertNumber(RtFloat flt)
{
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_request.back();
		if ( !p.setFloat(flt) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Mixed types in array",
				lineNo(), resourceName(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineNo(lineNo());
		p.setFloat(flt);
		m_request.push_back(p);
	}
	return RIBPARSER_NUMBER;
}


int CRibParser::insertNumber(RtInt num)
{
	if ( m_braketDepth ) {
		// Array
		CRibParameter &p = m_request.back();
		if ( !p.setInt(num) ) {
			// This happens if an array contains number and string values
			errHandler().handleError(RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Mixed types in array",
				lineNo(), resourceName(), RI_NULL);
		}
	} else {
		// Single value
		CRibParameter p;
		p.lineNo(lineNo());
		p.setInt(num);
		m_request.push_back(p);
	}
	return RIBPARSER_NUMBER;
}


int CRibParser::handleNumber(bool isInteger)
{
	m_token.push_back((unsigned char)0);

	if ( isInteger ) {
		long l = ::atol(&m_token[0]);
		return insertNumber((RtInt)l);
	}

	RtFloat f = (RtFloat)::atof(&m_token[0]);
	return insertNumber(f);
}


int CRibParser::nextToken()
{
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
				TOKENTYPE tempToken;
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
				lineNo(), resourceName(), c, RI_NULL);
			break;

		case 1: // Identifier
			{
				if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ) {   // a RIB token
					m_token.push_back(c);
					continue;
				}

				putback(c);

				m_token.push_back(0); // Terminate string
				// m_request.curRequest(&m_token[0]);
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
#ifdef _DEBUG
			if ( c == '*' ) {
				// to set breakpoints
				c = c;
			}
#endif
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
				lineNo(), resourceName(), state, RI_NULL);
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
				lineNo(), resourceName(), &m_token[0], RI_NULL);
		}
	}
	// Lookahead is eof or an ri token

	// handle any comments found (e.g. the first comments in a file)
	handleDeferedComments();

	// Clear parameter list
	m_braketDepth = 0;
	int t = m_lookahead; // Call id of the current request, RIBPARSER_EOF if EOF

	// If there is a token ( t != EOF ), read the parameters
	if ( t >= 0 ) {
		m_request.clear();
		m_request.curRequest(&m_token[0]);
		// find the next token as lookahead
		do {
			m_code = -1;            // encoded request number (binary decoder)
			m_defineString = -1;    // encoded string number (binary decoder)
			m_lookahead = nextToken();  // get the next token
			if ( m_lookahead == RIBPARSER_NOT_A_TOKEN ) {	// Token not found
				errHandler().handleError(
					RIE_BADTOKEN, RIE_ERROR,
					"Line %ld, File \"%s\", syntax: bad token \"%s\"",
					lineNo(), resourceName(),
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
				lineNo(), resourceName(), RI_NULL);
		}

		if ( m_braketDepth < 0 ) {
			m_braketDepth = 0;
			errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: Too many closing brakets",
				lineNo(), resourceName(), RI_NULL);
		}

		// handles the RIB request of the previous look ahead
		if ( !call(m_request.curRequest()) ) {
			// *** Error
			errHandler().handleError(
				RIE_BADTOKEN, RIE_ERROR,
				"Line %ld, File \"%s\", not a valid request: %s",
				lineNo(), resourceName(), m_request.curRequest().c_str(), RI_NULL);
		}
		handleDeferedComments(); // handles any comments found
	}

	return m_lookahead;
}


void CRibParser::parseFile()
{
	// Clear data array used by the binary decoder
	for ( int i = 0; i < 256; ++i )
		m_ribEncode[i] = REQ_UNKNOWN;

	// Clear the list of strings (binary decoder)
	m_stringMap.clear();

	// Clear handle maps (Handle number -> handle)
	clearHandleMaps();

	// Clears any defered comments
	m_deferedCommentList.clear();

	m_code = -1;            // No defined token (binary)
	m_defineString = -1;	// No defind string (binary)
	m_lookahead = RIBPARSER_NOT_A_TOKEN;  // Initialize, no Token found

	m_lineNo = 1;

	bool running = true;
	do {
		// Do not stop parsing if an error occurs
		try {
			running = parseNextCall() != RIBPARSER_EOF;
		} catch ( ExceptRiCPPError &e1 ) {
			if ( m_parserCallback ) {
				m_parserCallback->ricppErrHandler().handleError(e1);
			} else {
				throw e1;
			}
		} catch ( std::exception &e2 ) {
			if ( m_parserCallback ) {
				m_parserCallback->ricppErrHandler().handleError(
					RIE_SYSTEM, RIE_ERROR,
					lineNo(),
					resourceName(),
					"Unknown error while parsing: %s", e2.what());
			} else {
				throw e2;
			}
		} catch ( ... ) {
			if ( m_parserCallback ) {
				m_parserCallback->ricppErrHandler().handleError(
					RIE_SYSTEM, RIE_ERROR,
					lineNo(),
					resourceName(),
					"Unknown error while parsing.");
			} else {
				throw;
			}
		}
	} while ( running ); // Parse all requests

	// Clear the handle maps
	clearHandleMaps();
}


bool CRibParser::close()
{
	return m_ob.close();
}


bool CRibParser::canParse(RtString name)
{
	if ( !name || !*name ) {
		m_istream.rdbuf(std::cin.rdbuf());
		return true;
	}
	std::string filename = name;

	CUri refUri;
	if ( !refUri.encodeFilepath(filename.c_str(), 0) )
	{
		return false;
	}

	if ( !CUri::makeAbsolute(m_absUri, m_baseUri, refUri, false) ) {
		return false;
	}
	m_ob.base(m_baseUri);
	m_istream.rdbuf(&m_ob);
	return m_ob.open(refUri);
}


void CRibParser::parse(
	const IArchiveCallback *callback,
	const CParameterList &params)
{
	m_callback = callback;
	m_parameterList = params;
	parseFile();
}
