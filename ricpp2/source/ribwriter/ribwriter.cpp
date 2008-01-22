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
: m_ostream(&ribout), m_ascii(true), m_countStrings(0)
{
	memset(m_reqEncoding, 0, sizeof(m_reqEncoding)*sizeof(unsigned char));
}


void CRibElementsWriter::putNewLine()
{
	if ( m_ascii ) {
		// m_ostream << std::endl;
		m_ostream << '\n';
	}
}


void CRibElementsWriter::putBlank()
{
	if ( m_ascii ) {
		m_ostream << ' ';
	}
}


void CRibElementsWriter::putChar(char c)
{
	assert(c >= 0);
	m_ostream << c;
}


void CRibElementsWriter::putChars(const char *cs)
{
	if ( !cs )
		return;
		
	m_ostream << cs;
}


void CRibElementsWriter::putComment(const char *cs)
{
	m_ostream << '#';
	putChars(cs);
	m_ostream << std::endl;
}


void CRibElementsWriter::putBinValue(float aFloat)
{
	unsigned char code;
	unsigned long tmp = *((unsigned long *)(void*)&aFloat);

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
	if ( m_ascii ) {
		putChars(CRequestInfo::requestName(aRequest));
	} else {
		if ( !m_reqEncoding[aRequest] ) {
			unsigned char code = 0314;
			m_reqEncoding[aRequest] =  (unsigned char)aRequest;
			m_ostream << code << m_reqEncoding[aRequest];
			putString(CRequestInfo::requestName(aRequest));
		} else {
			unsigned char code = 0246;
			m_ostream << code << m_reqEncoding[aRequest];
		}
	}
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


void CRibElementsWriter::putValue(RtInt anInteger)
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
		return;
		
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

RtToken CRibWriter::RI_COMPRESS = "compress";


bool CRibWriter::testValid() const
{
	return  m_writer != 0 && m_buffer != 0;
}

bool CRibWriter::postTestValid() const
{
	// 2do Test if post... should print RIB request

	return testValid();
}

void CRibWriter::defaultDeclarations()
{
	CBaseRenderer::defaultDeclarations();

	processDeclare(RI_COMPRESS, "integer", true);
}

void CRibWriter::writeTrailer()
{
}

void CRibWriter::writeParameterList(const CParameterList &params)
{
	for ( CParameterList::const_iterator i = params.begin();
		  i != params.end();
		  i++ )
	{
		m_writer->putBlank();

		const CParameter &p = (*i);
		if ( p.declaration().isInline() )
			m_writer->putString(p.name());
		else
			m_writer->putStringToken(p.token());

		m_writer->putBlank();

		switch ( p.basicType() ) {
			case BASICTYPE_INTEGER:
				m_writer->putArray(p.ints());
				break;
			case BASICTYPE_FLOAT:
				m_writer->putArray(p.floats());
				break;
			case BASICTYPE_STRING:
				m_writer->putArray(p.stringPtrs());
				break;
			default:
				// Error
				m_writer->putChars("[ ]");
				break;
		}
	}

	m_writer->putNewLine();
}


RtVoid CRibWriter::controlV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	CBaseRenderer::controlV(name, n, tokens, params);
}


RtVoid CRibWriter::ribVersion()
{
}


RtVoid CRibWriter::postBegin(RtString name, const CParameterList &params)
{
	if ( m_buffer ) delete m_buffer;
	m_buffer = 0;
	if ( m_writer ) delete m_writer;
	m_writer = 0;

	RtInt compress = 0;
	std::string filename = "";
	CParameterList::const_iterator  i = params.begin();
	for ( ; i != params.end(); ++i ) {
		const CParameter &p = (*i);
		if ( p.token() == RI_FILE && p.strings().size()>0 ) {
			filename = p.strings()[0];
			CStringList stringList;
			stringList.explode(0, filename.c_str(), true, true, true); // Only one string, replace environment variables
			filename.clear();
			CStringList::const_iterator first = stringList.begin();
			if ( first != stringList.end() ) {
				filename = (*first).c_str();
			}
		}
		if ( !strcmp(p.token(), RI_COMPRESS) && p.ints().size()>0 ) {
			compress = p.ints()[0];
		}
	}

	if ( m_parserCallback == 0 ) {
		return;
	}

	if ( filename.size() > 0 && filename[0] != '|' ) {
		m_buffer = new TemplFrontStreambuf<char>(m_parserCallback->protocolHandlers());
		if ( !m_buffer ) {
			return;
		}
		const char *ptr = strrchr(filename.c_str(), '.');
		if ( ptr && !strcmp(ptr, ".ribz") && compress < 0 ) {
			compress = -1;
		}
		if ( !m_buffer->open(filename.c_str(), std::ios_base::out|std::ios_base::binary, compress) ) {
			return;
		}
		m_writer = new CRibElementsWriter(*m_buffer);
		if ( !m_writer ) {
			return;
		}
	} else {
		// stdio or pipe
		if ( filename.size() == 0 ) {
			// stdio
		} else {
			// pipe
		}
	}
}

RtVoid CRibWriter::postEnd(void)
{
	if ( m_writer ) delete m_writer;
	m_writer = 0;
	if ( m_buffer ) {
		m_buffer->close();
		delete m_buffer;
	}
	m_buffer = 0;
}


RtVoid CRibWriter::postErrorHandler(const IErrorHandler &handler)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ERROR_HANDLER);
	m_writer->putBlank();
	m_writer->putStringToken(handler.name());
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDeclare(RtToken name, RtString declaration)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DECLARE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putString(declaration);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSynchronize(RtToken name)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SYNCHRONIZE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSystem(RtToken cmd)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SYSTEM);
	m_writer->putBlank();
	m_writer->putString(cmd);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResource(RtString handle, RtString type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_RESOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(handle);
	m_writer->putBlank();
	m_writer->putString(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postFrameBegin(RtInt number)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_FRAME_BEGIN);
	m_writer->putBlank();
	m_writer->putValue(number);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postFrameEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_FRAME_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postWorldBegin(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_WORLD_BEGIN);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postWorldEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_WORLD_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postAttributeBegin(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ATTRIBUTE_BEGIN);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postAttributeEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ATTRIBUTE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransformBegin(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_TRANSFORM_BEGIN);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransformEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_TRANSFORM_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSolidBegin(RtToken type)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SOLID_BEGIN);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSolidEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SOLID_END);
	m_writer->putNewLine();
}

RtVoid CRibWriter::postObjectBegin(RtObjectHandle h)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_OBJECT_BEGIN);
	m_writer->putBlank();
	m_writer->putValue(extractHandleNo(h));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postObjectEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_OBJECT_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postObjectInstance(RtObjectHandle handle)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_OBJECT_INSTANCE);
	m_writer->putBlank();
	m_writer->putValue(extractHandleNo(handle));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postMotionBegin(RtInt N, RtFloat times[])
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_MOTION_BEGIN);
	m_writer->putBlank();
	m_writer->putArray(N, N>0 ? &times[0] : 0);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postMotionEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_MOTION_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResourceBegin(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_RESOURCE_BEGIN);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResourceEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_RESOURCE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ARCHIVE_BEGIN);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postArchiveEnd(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ARCHIVE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_FORMAT);
	m_writer->putBlank();
	m_writer->putValue(xres);
	m_writer->putBlank();
	m_writer->putValue(yres);
	m_writer->putBlank();
	m_writer->putValue(aspect);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postFrameAspectRatio(RtFloat aspect)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_FRAME_ASPECT_RATIO);
	m_writer->putBlank();
	m_writer->putValue(aspect);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SCREEN_WINDOW);
	m_writer->putBlank();
	m_writer->putValue(left);
	m_writer->putBlank();
	m_writer->putValue(right);
	m_writer->putBlank();
	m_writer->putValue(bot);
	m_writer->putBlank();
	m_writer->putValue(top);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_CROP_WINDOW);
	m_writer->putBlank();
	m_writer->putValue(xmin);
	m_writer->putBlank();
	m_writer->putValue(xmax);
	m_writer->putBlank();
	m_writer->putValue(ymin);
	m_writer->putBlank();
	m_writer->putValue(ymax);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postProjection(RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_PROJECTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postClipping(RtFloat hither, RtFloat yon)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_CLIPPING);
	m_writer->putBlank();
	m_writer->putValue(hither);
	m_writer->putBlank();
	m_writer->putValue(yon);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_CLIPPING_PLANE);
	m_writer->putBlank();
	m_writer->putValue(x);
	m_writer->putBlank();
	m_writer->putValue(y);
	m_writer->putBlank();
	m_writer->putValue(z);
	m_writer->putBlank();
	m_writer->putValue(nx);
	m_writer->putBlank();
	m_writer->putValue(ny);
	m_writer->putBlank();
	m_writer->putValue(nz);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DEPTH_OF_FIELD);
	m_writer->putBlank();
	m_writer->putValue(fstop);
	m_writer->putBlank();
	m_writer->putValue(focallength);
	m_writer->putBlank();
	m_writer->putValue(focaldistance);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postShutter(RtFloat smin, RtFloat smax)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SHUTTER);
	m_writer->putBlank();
	m_writer->putValue(smin);
	m_writer->putBlank();
	m_writer->putValue(smax);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPixelVariance(RtFloat variation)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_PIXEL_VARIANCE);
	m_writer->putBlank();
	m_writer->putValue(variation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_PIXEL_SAMPLES);
	m_writer->putBlank();
	m_writer->putValue(xsamples);
	m_writer->putBlank();
	m_writer->putValue(ysamples);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_PIXEL_FILTER);
	m_writer->putBlank();
	m_writer->putStringToken(function.name());
	m_writer->putBlank();
	m_writer->putValue(xwidth);
	m_writer->putBlank();
	m_writer->putValue(ywidth);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postExposure(RtFloat gain, RtFloat gamma)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_EXPOSURE);
	m_writer->putBlank();
	m_writer->putValue(gain);
	m_writer->putBlank();
	m_writer->putValue(gamma);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postImager(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_IMAGER);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_QUANTIZE);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putBlank();
	m_writer->putValue(one);
	m_writer->putBlank();
	m_writer->putValue(qmin);
	m_writer->putBlank();
	m_writer->putValue(qmax);
	m_writer->putBlank();
	m_writer->putValue(ampl);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDisplayChannel(RtString channel, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DISPLAY_CHANNEL);
	m_writer->putBlank();
	m_writer->putString(channel);
	writeParameterList(params);
}


RtVoid CRibWriter::postDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DISPLAY);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putBlank();
	m_writer->putStringToken(mode);
	writeParameterList(params);
}


RtVoid CRibWriter::postHider(RtToken type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_HIDER);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_COLOR_SAMPLES);
	m_writer->putBlank();
	m_writer->putArray(N*3, nRGB);
	m_writer->putBlank();
	m_writer->putArray(N*3, RGBn);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postRelativeDetail(RtFloat relativedetail)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_RELATIVE_DETAIL);
	m_writer->putBlank();
	m_writer->putValue(relativedetail);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOption(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_OPTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putValue(h);
	writeParameterList(params);
}


RtVoid CRibWriter::postAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_AREA_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putValue(h);
	writeParameterList(params);
}


RtVoid CRibWriter::postAttribute(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ATTRIBUTE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postColor(RtColor Cs)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_COLOR);
	m_writer->putBlank();
	unsigned long n = (unsigned long)(renderState()->options().colorDescr().colorSamples());
	m_writer->putArray(n, Cs);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOpacity(RtColor Os)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_OPACITY);
	m_writer->putBlank();
	unsigned long n = (unsigned long)(renderState()->options().colorDescr().colorSamples());
	m_writer->putArray(n, Os);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSurface(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SURFACE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postAtmosphere(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ATMOSPHERE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postInterior(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_INTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postExterior(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_EXTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postIlluminate(RtLightHandle light, RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ILLUMINATE);
	m_writer->putBlank();
	m_writer->putValue(light);
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDisplacement(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DISPLACEMENT);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_TEXTURE_COORDINATES);
	m_writer->putBlank();
	m_writer->putValue(s1);
	m_writer->putBlank();
	m_writer->putValue(t1);
	m_writer->putBlank();
	m_writer->putValue(s2);
	m_writer->putBlank();
	m_writer->putValue(t2);
	m_writer->putBlank();
	m_writer->putValue(s3);
	m_writer->putBlank();
	m_writer->putValue(t3);
	m_writer->putBlank();
	m_writer->putValue(s4);
	m_writer->putBlank();
	m_writer->putValue(t4);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postShadingRate(RtFloat size)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SHADING_RATE);
	m_writer->putBlank();
	m_writer->putValue(size);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postShadingInterpolation(RtToken type)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SHADING_INTERPOLATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postMatte(RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_MATTE);
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBound(RtBound bound)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_BOUND);
	m_writer->putBlank();
	m_writer->putArray(sizeof RtBound / sizeof bound[0], &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetail(RtBound bound)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_DETAIL);
	m_writer->putBlank();
	m_writer->putArray(sizeof RtBound / sizeof bound[0], &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SHADING_RATE);
	m_writer->putBlank();
	m_writer->putValue(minvis);
	m_writer->putBlank();
	m_writer->putValue(lowtran);
	m_writer->putBlank();
	m_writer->putValue(uptran);
	m_writer->putBlank();
	m_writer->putValue(maxvis);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postGeometricApproximation(RtToken type, RtFloat value)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_GEOMETRIC_APPROXIMATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putBlank();
	m_writer->putValue(value);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postGeometricRepresentation(RtToken type)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_GEOMETRIC_REPRESENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOrientation(RtToken anOrientation)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_ORIENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(anOrientation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postReverseOrientation(void)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_REVERSE_ORIENTATION);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSides(RtInt nsides)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_SIDES);
	m_writer->putBlank();
	m_writer->putValue(nsides);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( !postTestValid() )
		return;

	writeTrailer();
	m_writer->putRequest(REQ_BASIS);
	m_writer->putBlank();
	m_writer->putArray(sizeof RtBasis / sizeof ubasis[0][0], &ubasis[0][0]);
	m_writer->putBlank();
	m_writer->putValue(ustep);
	m_writer->putBlank();
	m_writer->putArray(sizeof RtBasis / sizeof vbasis[0][0], &vbasis[0][0]);
	m_writer->putBlank();
	m_writer->putValue(vstep);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	if ( !postTestValid() )
		return;

	CTrimCurveDataInfo tc(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

	writeTrailer();
	m_writer->putRequest(REQ_TRIM_CURVE);
	m_writer->putBlank();
	m_writer->putArray(tc.m_nloops, ncurves);
	m_writer->putBlank();
	m_writer->putArray(tc.m_total, order);
	m_writer->putBlank();
	m_writer->putArray(tc.m_nknots, knot);
	m_writer->putBlank();
	m_writer->putArray(tc.m_total, amin);
	m_writer->putBlank();
	m_writer->putArray(tc.m_total, amax);
	m_writer->putBlank();
	m_writer->putArray(tc.m_total, n);
	m_writer->putBlank();
	m_writer->putArray(tc.m_npoints, u);
	m_writer->putBlank();
	m_writer->putArray(tc.m_npoints, v);
	m_writer->putBlank();
	m_writer->putArray(tc.m_npoints, w);
	m_writer->putNewLine();
}
