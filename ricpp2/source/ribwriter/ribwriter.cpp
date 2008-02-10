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

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

/* To translate (cache rib archive is part of baserenderer)

postpone: ObjectInstance, ReadArchive, Procedural - Befehl ausgeben statt ausführen, wenn
          referenziertes Objekt vorhanden ist.

RIB control
-----------

control object postpone_object 1 | 0, default 1

<>0 - Object Definition ausgeben und ausführen, Instance-Befehl ausgeben (falls object postponed)
0   - Inszanzieren des objekts (keine object definition)

Control ribarchive postpone_procedural    1 | 0, default 1

<>0 - falls delayed read archive & !postponed Macro: delayed ReadArchive ausführen
      sonst: als Text ausgeben, nicht ausführen

 0 - delayed readarchive ReadArchive ausführen, falls Macro

Control ribarchive postpone_delayed_file    1 | 0, default 1

<>0 - delayed ReadArchive als Text ausgeben, nicht ausführen, falls Datei

 0 - delayed readarchive ReadArchive ausführen, falls Datei

Control ribarchive postpone_file    1 | 0 | -1, default 0

>0 - ReadArchive als Text ausgeben, nicht ausführen, falls Datei

 0 - ReadArchive ausführen, falls Datei

<0 - ReadArchive 1x ausführen, falls Datei, dann postpone_file auf 1 (nicht mehr ausführen)
     Damit kann die erste Datei gelesen werden

Control ribarchive postpone_macro [constant integer] 1 | 0 default 1

<>0 - ArchiveBegin-End ausgeben und Makro aufbauen
    falls postponed Macro: delayed ReadArchive als Text ausgeben, nicht ausführen
    falls nicht posponed Macro: Ausführen (da Definition nicht ausgegeben wurde)

0 - ArchiveBegin-End nicht ausgeben, nur Makro aufbauen
    ReadArchive ausführen falls macro

Control ribarchive skip_header [constant integer] 1 | 0 | -1 default 1 (zum Test -1)

 >0 - Header aus Dateien nicht ausgeben
  0 - Header immer ausgeben
 <0 - Nur der erste Header wird ausgeben

control ribarchive skip_version [constant integer] 1 | 0 default 0
<>0 - version wird nicht ausgegeben.
  0 - version wird nur 1x ausgegeben.


Control ribarchive cachefile 1 0
<>0 Caches files like inline archive
1   Won't cahce files

*/


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


void CRibElementsWriter::putChar(char c)
{
	assert (c >= 0);
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
		
	// m_ostream << cs;
}


void CRibElementsWriter::putComment(RtToken type, const char *cs)
{
	bool sav_ascii = m_ascii;
	m_ascii = true;

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
	m_firstRequestWritten = true;
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


CRibWriter::CRibWriter()
{
	m_writer = 0;
	m_buffer = 0;

	RI_COMPRESS = RI_NULL;
	RI_POSTPONE_PROCEDURALS = RI_NULL;
	RI_POSTPONE_OBJECTS = RI_NULL;
	RI_POSTPONE_FILE_ARCHIVES = RI_NULL;
	RI_POSTPONE_INLINE_ARCHIVES = RI_NULL;
	RI_SKIP_HEADERS = RI_NULL;
	RI_SKIP_VERSION = RI_NULL;
	RI_INDENT = RI_NULL;
	RI_INDENT_STRING = RI_NULL;
	RI_SUPPRESS_OUTPUT = RI_NULL;
	RI_BINARY_OUTPUT = RI_NULL;

	RI_RIBWRITER = RI_NULL;

	m_postponeProcedural = 1;
	m_postponeObject = 1;
	m_postponeFileArchive = 0;
	m_postponeInlineArchive = 1;
	m_skipHeader =  0;
	m_skipVersion = 0;
	m_indent = true;
	m_indentString = "    ";
	m_binary = false;

	m_controlSuppressOutput = false;
	m_suppressOutput = false;
	
	m_header = false;
	m_execute = false;
	nestingDepth(0);
}

CRibWriter::~CRibWriter()
{
	if ( m_writer ) delete m_writer;
	if ( m_buffer ) delete m_buffer;
}


void CRibWriter::requestWritten(EnumRequests aRequest)
{
	m_header = false;
	// Header is skipt after first regular request
	if ( m_skipHeader < 0 )
		m_skipHeader = 1;
}

bool CRibWriter::testValid() const
{
	if ( m_controlSuppressOutput )
		return false;
	return  m_writer != 0;
}

bool CRibWriter::postTestValid() const
{
	if ( m_suppressOutput )
		return false;

	return testValid();
}

void CRibWriter::defaultDeclarations()
{
	CBaseRenderer::defaultDeclarations();

	// Declarations
	RI_COMPRESS = processDeclare("compress", "constant integer", true);

	RI_POSTPONE_PROCEDURALS =     processDeclare("postpone-procedurals",     "constant integer", true);
	RI_POSTPONE_OBJECTS =         processDeclare("postpone-objects",         "constant integer", true);
	RI_POSTPONE_FILE_ARCHIVES =   processDeclare("postpone-file-archives",   "constant integer", true);
	RI_POSTPONE_INLINE_ARCHIVES = processDeclare("postpone-inline-archives", "constant integer", true);
	RI_SKIP_HEADERS =             processDeclare("skip-headers",             "constant integer", true);
	RI_SKIP_VERSION =             processDeclare("skip-version",             "constant integer", true);
	RI_INDENT =                   processDeclare("indent",                   "constant integer", true);
	RI_INDENT_STRING =            processDeclare("indent-string",            "constant string", true);
	RI_SUPPRESS_OUTPUT =          processDeclare("suppress-output",          "constant integer", true);
	RI_BINARY_OUTPUT =            processDeclare("binary-output",            "constant integer", true);

	// Tokens
	RI_RIBWRITER = renderState()->tokFindCreate("ribwriter");
}

void CRibWriter::writePrefix(bool isArchiveRecord)
{
	if ( !isArchiveRecord && !m_writer->firstRequestWritten() ) {
		version();
	}
	if ( m_indent && m_writer->ascii() ) {
		for ( unsigned long i = 0; i < nestingDepth(); ++i ) {
			m_writer->putChars(m_indentString.c_str());
		}
	}
}

void CRibWriter::writeParameterList(const CParameterList &params)
{
	bool paramError = false;
	for ( CParameterList::const_iterator i = params.begin();
		  i != params.end();
		  i++ )
	{
		m_writer->putBlank();

		const CParameter &p = (*i);
		/*
		if ( p.declaration().isInline() ) {
			std::string decl;
			m_writer->putString(p.declaration().getInlineString(decl));
		}
		else
			m_writer->putStringToken(p.name());
		*/

		if ( p.isInline() )
			m_writer->putString(p.tokenStr());
		else
			m_writer->putStringToken(p.tokenStr());

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
				paramError = true;
				m_writer->putChars("[ ]");
				break;
		}
	}

	m_writer->putNewLine();
	if ( paramError ) {
		// throw error
	}
}


RtVoid CRibWriter::doControl(RtToken name, const CParameterList &params)
{
	CBaseRenderer::doControl(name, params);

	if ( name == RI_RIBWRITER ) {
		CParameterList::const_iterator i;
		for ( i = params.begin(); i != params.end(); i++ ) {
			if ( (*i).token() == RI_POSTPONE_PROCEDURALS ) {
				(*i).get(0, m_postponeProcedural);
			} else if ( (*i).token() == RI_POSTPONE_OBJECTS ) {
				(*i).get(0, m_postponeObject);
			} else if ( (*i).token() == RI_POSTPONE_FILE_ARCHIVES ) {
				(*i).get(0, m_postponeFileArchive);
			} else if ( (*i).token() == RI_POSTPONE_INLINE_ARCHIVES ) {
				(*i).get(0, m_postponeInlineArchive);
			} else if ( (*i).token() == RI_SKIP_HEADERS ) {
				(*i).get(0, m_skipHeader);
			} else if ( (*i).token() == RI_SKIP_VERSION ) {
				(*i).get(0, m_skipVersion);
			} else if ( (*i).token() == RI_INDENT ) {
				RtInt intval;
				if ( (*i).get(0, intval) ) {
					m_indent = intval != 0;
				}
			} else if ( (*i).token() == RI_INDENT_STRING ) {
				RtString strval = 0;
				if ( (*i).get(0, strval) ) {
					m_indentString = noNullStr(strval);;
				}
			} else if ( (*i).token() == RI_SUPPRESS_OUTPUT ) {
				RtInt intval;
				if ( (*i).get(0, intval) ) {
					m_controlSuppressOutput = intval != 0;
				}
			} else if ( (*i).token() == RI_BINARY_OUTPUT ) {
				RtInt intval;
				if ( (*i).get(0, intval) ) {
					m_binary = intval != 0;
					m_writer->ascii(!m_binary);
				}
			}
		}
	}
}


RtVoid CRibWriter::version()
{
	if ( !postTestValid() )
		return;

	if ( m_skipHeader < 0 )
		m_skipHeader = 1;
		
	m_header = false;

	if ( m_skipVersion != 0 )
		return;

	m_writer->putChars("version 3.03");
	m_writer->putNewLine();
	m_skipVersion = 1; // only output once
}


RtVoid CRibWriter::postBegin(RtString name, const CParameterList &params)
{
	if ( m_buffer ) delete m_buffer;
	m_buffer = 0;
	if ( m_writer ) delete m_writer;
	m_writer = 0;
	
	nestingDepth(0);

	RtInt compress = 0;
	std::string filename;
	CParameterList::const_iterator  i = params.begin();
	for ( ; i != params.end(); ++i ) {
		const CParameter &p = (*i);
		if ( p.token() == RI_FILE && p.strings().size()>0 ) {
			CStringList stringList;
			stringList.expand(filename, p.strings()[0].c_str(), true);
			CFilepathConverter::convertToURL(filename);
		}
		if ( p.token() == RI_COMPRESS && p.ints().size() > 0 ) {
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
		m_writer = new CRibElementsWriter(m_buffer, *this);
		if ( !m_writer ) {
			return;
		}
		m_writer->ascii(!m_binary);
	} else {
		// stdio or pipe
		if ( filename.size() == 0 ) {
			// stdio
			m_writer = new CRibElementsWriter(std::cout.rdbuf(), *this);
			if ( !m_writer ) {
				return;
			}
			m_writer->ascii(!m_binary);
		} else {
			// @todo pipe
			m_writer = new CRibElementsWriter(std::cout.rdbuf(), *this);
			if ( !m_writer ) {
				return;
			}
			m_writer->ascii(!m_binary);
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

	writePrefix();
	m_writer->putRequest(REQ_ERROR_HANDLER);
	m_writer->putBlank();
	m_writer->putStringToken(handler.name());
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDeclare(RtToken name, RtString declaration)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_SYNCHRONIZE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSystem(RtString cmd)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SYSTEM);
	m_writer->putBlank();
	m_writer->putString(cmd);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResource(RtToken handle, RtString type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_FRAME_BEGIN);
	m_writer->putBlank();
	m_writer->putValue(number);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postFrameEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_FRAME_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postWorldBegin(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_WORLD_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postWorldEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_WORLD_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postAttributeBegin(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postAttributeEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransformBegin(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postTransformEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSolidBegin(RtToken type)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SOLID_BEGIN);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postSolidEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_SOLID_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postObjectBegin(RtObjectHandle h)
{
	if ( !testValid() )
		return;

	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);

	if ( m ) {
		m->postpone(m_postponeObject != 0);
	}

	m_suppressOutputVector.push_back(m_suppressOutput);

	if ( !m || m->postpone() ) {
		writePrefix();
		m_writer->putRequest(REQ_OBJECT_BEGIN);
		m_writer->putBlank();
		m_writer->putValue(extractHandleNo(h));
		m_writer->putNewLine();
		m_suppressOutput = false;
		incNestingDepth();
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preObjectEnd(void)
{
	CRiMacro *m = renderState()->curMacro();

	CBaseRenderer::preObjectEnd();

	if ( !testValid() ) 
		return;

	assert (m != 0);
	if ( !m || m->postpone() ) {
		if ( testValid() ) {
			decNestingDepth();
			writePrefix();
			m_writer->putRequest(REQ_OBJECT_END);
			m_writer->putNewLine();
		}
	}
}

RtVoid CRibWriter::postObjectEnd(void)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}

RtVoid CRibWriter::doObjectInstance(RtObjectHandle handle)
{
	// Only write the ObjectInstance Request
	// Do this only if a definition was written to output (macro is postponed).
	// This is the "normal" case.
	if ( m_postponeObject ) {
		CRiObjectMacro *m = renderState()->objectInstance(handle);
		if ( !m || m->postpone() ) {
			return;
		}
	}


	// Put out the requests that form the object.
	// It doesn't matter if the Object definition was
	// written to output or not.
	CBaseRenderer::doObjectInstance(handle);
}


RtVoid CRibWriter::postObjectInstance(RtObjectHandle handle)
{
	if ( m_postponeObject ) {
		if ( !postTestValid() )
			return;
		CRiObjectMacro *m = renderState()->objectInstance(handle);
		if ( !m || m->postpone() ) {
			// Put out ObjectInstance only if the object was postponed.
			writePrefix();
			m_writer->putRequest(REQ_OBJECT_INSTANCE);
			m_writer->putBlank();
			m_writer->putValue(extractHandleNo(handle));
			m_writer->putNewLine();
		} else if ( renderState()->recordMode() ) {
			CBaseRenderer::doObjectInstance(handle);
		}
	} else if ( renderState()->recordMode() ) {
		CBaseRenderer::doObjectInstance(handle);
	}
}


RtVoid CRibWriter::postMotionBegin(RtInt N, RtFloat times[])
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MOTION_BEGIN);
	m_writer->putBlank();
	m_writer->putArray(N, N>0 ? &times[0] : 0);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postMotionEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_MOTION_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResourceBegin(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_RESOURCE_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postResourceEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_RESOURCE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params)
{
	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);

	if ( m ) {
		m->postpone(m_postponeInlineArchive != 0);
	}

	m_suppressOutputVector.push_back(m_suppressOutput);

	if ( !m || m->postpone() ) {
		m_suppressOutput = false;
		if ( postTestValid() ) {
			writePrefix();
			m_writer->putRequest(REQ_ARCHIVE_BEGIN);
			m_writer->putBlank();
			m_writer->putStringToken(name);
			writeParameterList(params);
		}
		incNestingDepth();
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preArchiveEnd(void)
{
	CRiMacro *m = renderState()->curMacro();

	CBaseRenderer::preArchiveEnd();

	assert (m != 0);
	if ( !m || m->postpone() ) {
		decNestingDepth();
		if ( postTestValid() ) {
			writePrefix();
			m_writer->putRequest(REQ_ARCHIVE_END);
			m_writer->putNewLine();
		}
	}
}


RtVoid CRibWriter::postArchiveEnd(void)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}


RtVoid CRibWriter::postFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_FRAME_ASPECT_RATIO);
	m_writer->putBlank();
	m_writer->putValue(aspect);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_PROJECTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postClipping(RtFloat hither, RtFloat yon)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
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

	writePrefix();
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

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_PIXEL_VARIANCE);
	m_writer->putBlank();
	m_writer->putValue(variation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
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

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_IMAGER);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_DISPLAY_CHANNEL);
	m_writer->putBlank();
	m_writer->putString(channel);
	writeParameterList(params);
}


RtVoid CRibWriter::postDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_HIDER);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_RELATIVE_DETAIL);
	m_writer->putBlank();
	m_writer->putValue(relativedetail);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOption(RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_OPTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putValue((unsigned long)h);
	writeParameterList(params);
}


RtVoid CRibWriter::postAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_AREA_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	m_writer->putValue((unsigned long)h);
	writeParameterList(params);
}


RtVoid CRibWriter::postAttribute(RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postColor(RtColor Cs)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_SURFACE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postAtmosphere(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATMOSPHERE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postInterior(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_INTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postExterior(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_EXTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postIlluminate(RtLightHandle light, RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ILLUMINATE);
	m_writer->putBlank();
	m_writer->putValue((unsigned long)light);
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDisplacement(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DISPLACEMENT);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	if ( !postTestValid() )
		return;

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_SHADING_RATE);
	m_writer->putBlank();
	m_writer->putValue(size);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postShadingInterpolation(RtToken type)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SHADING_INTERPOLATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postMatte(RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MATTE);
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBound(RtBound bound)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_BOUND);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtBound)/sizeof(bound[0]), &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetail(RtBound bound)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DETAIL);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtBound)/sizeof(bound[0]), &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DETAIL_RANGE);
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

	writePrefix();
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

	writePrefix();
	m_writer->putRequest(REQ_GEOMETRIC_REPRESENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOrientation(RtToken anOrientation)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ORIENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(anOrientation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postReverseOrientation(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_REVERSE_ORIENTATION);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSides(RtInt nsides)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SIDES);
	m_writer->putBlank();
	m_writer->putValue(nsides);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( !postTestValid() )
		return;

	RtToken basisToken;

	writePrefix();
	m_writer->putRequest(REQ_BASIS);
	m_writer->putBlank();
	basisToken = renderState()->basisName(ubasis);
	if ( basisToken != RI_NULL ) {
		m_writer->putStringToken(basisToken);
	} else {
		m_writer->putArray(sizeof(RtBasis)/sizeof(ubasis[0][0]), &(ubasis[0][0]));
	}
	m_writer->putBlank();
	m_writer->putValue(ustep);
	m_writer->putBlank();
	basisToken = renderState()->basisName(vbasis);
	if ( basisToken != RI_NULL ) {
		m_writer->putStringToken(basisToken);
	} else {
		m_writer->putArray(sizeof(RtBasis)/sizeof(vbasis[0][0]), &(vbasis[0][0]));
	}
	m_writer->putBlank();
	m_writer->putValue(vstep);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	if ( !postTestValid() )
		return;

	CTrimCurveDataInfo tc(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

	writePrefix();
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


RtVoid CRibWriter::postIdentity(void)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_IDENTITY);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransform(RtMatrix aTransform)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtMatrix)/sizeof(aTransform[0][0]), &(aTransform[0][0]));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postConcatTransform(RtMatrix aTransform)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_CONCAT_TRANSFORM);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtMatrix)/sizeof(aTransform[0][0]), &(aTransform[0][0]));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPerspective(RtFloat fov)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PERSPECTIVE);
	m_writer->putBlank();
	m_writer->putValue(fov);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TRANSLATE);
	m_writer->putBlank();
	m_writer->putValue(dx);
	m_writer->putBlank();
	m_writer->putValue(dy);
	m_writer->putBlank();
	m_writer->putValue(dz);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ROTATE);
	m_writer->putBlank();
	m_writer->putValue(angle);
	m_writer->putBlank();
	m_writer->putValue(dx);
	m_writer->putBlank();
	m_writer->putValue(dy);
	m_writer->putBlank();
	m_writer->putValue(dz);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postScale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SCALE);
	m_writer->putBlank();
	m_writer->putValue(dx);
	m_writer->putBlank();
	m_writer->putValue(dy);
	m_writer->putBlank();
	m_writer->putValue(dz);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SKEW);
	m_writer->putBlank();
	m_writer->putValue(angle);
	m_writer->putBlank();
	m_writer->putValue(dx1);
	m_writer->putBlank();
	m_writer->putValue(dy1);
	m_writer->putBlank();
	m_writer->putValue(dz1);
	m_writer->putBlank();
	m_writer->putValue(dx2);
	m_writer->putBlank();
	m_writer->putValue(dy2);
	m_writer->putBlank();
	m_writer->putValue(dz2);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDeformation(RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DEFORMATION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postScopedCoordinateSystem(RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SCOPED_COORDINATE_SYSTEM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postCoordinateSystem(RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_COORDINATE_SYSTEM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postCoordSysTransform(RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_COORD_SYS_TRANSFORM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPolygon(RtInt nvertices, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POLYGON);
	writeParameterList(params);
}


RtVoid CRibWriter::postGeneralPolygon(RtInt nloops, RtInt nverts[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_GENERAL_POLYGON);
	m_writer->putBlank();
	m_writer->putArray(nloops, nverts);
	writeParameterList(params);
}


RtVoid CRibWriter::postPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POINTS_POLYGONS);
	m_writer->putBlank();
	m_writer->putArray(npolys, nverts);
	m_writer->putBlank();
	m_writer->putArray(sum(npolys, nverts), verts);
	writeParameterList(params);
}


RtVoid CRibWriter::postPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POINTS_GENERAL_POLYGONS);
	m_writer->putBlank();
	m_writer->putArray(npolys, nloops);
	RtInt n = sum(npolys, nloops);
	m_writer->putBlank();
	m_writer->putArray(n, nverts);
	m_writer->putBlank();
	m_writer->putArray(sum(n, nverts), verts);
	writeParameterList(params);
}


RtVoid CRibWriter::postPatch(RtToken type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PATCH);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PATCH_MESH);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putBlank();
	m_writer->putValue(nu);
	m_writer->putBlank();
	m_writer->putStringToken(uwrap);
	m_writer->putBlank();
	m_writer->putValue(nv);
	m_writer->putBlank();
	m_writer->putStringToken(vwrap);
	writeParameterList(params);
}


RtVoid CRibWriter::postNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_NU_PATCH);
	m_writer->putBlank();
	m_writer->putValue(nu);
	m_writer->putBlank();
	m_writer->putValue(uorder);
	m_writer->putBlank();
	m_writer->putArray(nu+uorder, uknot);
	m_writer->putBlank();
	m_writer->putValue(umin);
	m_writer->putBlank();
	m_writer->putValue(umax);
	m_writer->putBlank();
	m_writer->putValue(nv);
	m_writer->putBlank();
	m_writer->putValue(vorder);
	m_writer->putBlank();
	m_writer->putArray(nv+vorder, vknot);
	m_writer->putBlank();
	m_writer->putValue(vmin);
	m_writer->putBlank();
	m_writer->putValue(vmax);
	writeParameterList(params);
}


RtVoid CRibWriter::postSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SUBDIVISION_MESH);
	m_writer->putBlank();
	m_writer->putStringToken(scheme);
	m_writer->putBlank();
	m_writer->putArray(nfaces, nvertices);
	m_writer->putBlank();
	m_writer->putArray(sum(nfaces, nvertices), vertices);
	m_writer->putBlank();
	m_writer->putTokenArray(ntags, tags);
	m_writer->putBlank();
	m_writer->putArray(ntags*2, nargs);
	m_writer->putBlank();

	RtInt nInts=0, nFlos=0;
	for ( RtInt i = 0; i < ntags*2; ) {
		nInts += nargs[i++];
		nFlos += nargs[i++];
	}
	m_writer->putArray(nInts, intargs);
	m_writer->putBlank();
	m_writer->putArray(nFlos, floatargs);
	m_writer->putBlank();

	writeParameterList(params);
}


RtVoid CRibWriter::postHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_HIERARCHICAL_SUBDIVISION_MESH);
	m_writer->putBlank();
	m_writer->putStringToken(scheme);
	m_writer->putBlank();
	m_writer->putArray(nfaces, nvertices);
	m_writer->putBlank();
	m_writer->putArray(sum(nfaces, nvertices), vertices);
	m_writer->putBlank();
	m_writer->putTokenArray(ntags, tags);
	m_writer->putBlank();
	m_writer->putArray(ntags*3, nargs);
	m_writer->putBlank();

	RtInt nInts=0, nFlos=0, nToks=0;
	for ( RtInt i = 0; i < ntags*3; ) {
		nInts += nargs[i++];
		nFlos += nargs[i++];
		nToks += nargs[i++];
	}

	m_writer->putArray(nInts, intargs);
	m_writer->putBlank();
	m_writer->putArray(nFlos, floatargs);
	m_writer->putBlank();
	m_writer->putTokenArray(nToks, stringargs);
	m_writer->putBlank();

	writeParameterList(params);
}


RtVoid CRibWriter::postSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SPHERE);
	m_writer->putBlank();
	m_writer->putValue(radius);
	m_writer->putBlank();
	m_writer->putValue(zmin);
	m_writer->putBlank();
	m_writer->putValue(zmax);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_CONE);
	m_writer->putBlank();
	m_writer->putValue(height);
	m_writer->putBlank();
	m_writer->putValue(radius);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_CYLINDER);
	m_writer->putBlank();
	m_writer->putValue(radius);
	m_writer->putBlank();
	m_writer->putValue(zmin);
	m_writer->putBlank();
	m_writer->putValue(zmax);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_HYPERBOLOID);
		m_writer->putBlank();
	// m_writer->putArray(sizeof(RtPoint)/sizeof(point1[0]), point1);
	m_writer->putValue(point1[0]);
	m_writer->putBlank();
	m_writer->putValue(point1[1]);
	m_writer->putBlank();
	m_writer->putValue(point1[2]);
	m_writer->putBlank();
	// m_writer->putArray(sizeof(RtPoint)/sizeof(point2[0]), point1);
	m_writer->putValue(point2[0]);
	m_writer->putBlank();
	m_writer->putValue(point2[1]);
	m_writer->putBlank();
	m_writer->putValue(point2[2]);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
writeParameterList(params);
}


RtVoid CRibWriter::postParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PARABOLOID);
	m_writer->putBlank();
	m_writer->putValue(rmax);
	m_writer->putBlank();
	m_writer->putValue(zmin);
	m_writer->putBlank();
	m_writer->putValue(zmax);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DISK);
	m_writer->putBlank();
	m_writer->putValue(height);
	m_writer->putBlank();
	m_writer->putValue(radius);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TORUS);
	m_writer->putBlank();
	m_writer->putValue(majorrad);
	m_writer->putBlank();
	m_writer->putValue(minorrad);
	m_writer->putBlank();
	m_writer->putValue(phimin);
	m_writer->putBlank();
	m_writer->putValue(phimax);
	m_writer->putBlank();
	m_writer->putValue(thetamax);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postPoints(RtInt npts, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POINTS);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_CURVES);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putBlank();
	m_writer->putArray(ncurves, nverts);
	m_writer->putBlank();
	m_writer->putStringToken(wrap);
	writeParameterList(params);
}


RtVoid CRibWriter::postBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_BLOBBY);
	m_writer->putBlank();
	m_writer->putValue(nleaf);
	m_writer->putBlank();
	m_writer->putArray(ncode, code);
	m_writer->putBlank();
	m_writer->putArray(nflt, flt);
	m_writer->putBlank();
	m_writer->putArray(nstr, str);
	m_writer->putBlank();
	writeParameterList(params);
}



RtVoid CRibWriter::doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
{
	if ( !m_postponeProcedural )
		CBaseRenderer::doProcedural(data, bound, subdivfunc, freefunc);
}

RtVoid CRibWriter::postProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc)
{
	if ( m_postponeProcedural ) {
		if ( !postTestValid() )
			return;

		writePrefix();
		m_writer->putRequest(REQ_PROCEDURAL);
		m_writer->putBlank();
		m_writer->putStringToken(subdivfunc.name());
		m_writer->putBlank();
		m_writer->putArray(subdivfunc.numArgs(), (const RtString *)data);
		m_writer->putBlank();
		m_writer->putArray(sizeof(RtBound)/sizeof(bound[0]), bound);
		m_writer->putNewLine();
	} else if ( renderState()->recordMode() ) {
		doProcedural(data, bound, subdivfunc, freefunc);
	}
}


RtVoid CRibWriter::postGeometry(RtToken type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_GEOMETRY);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	if ( params.size() > 0 )
		m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_TEXTURE);
	m_writer->putBlank();
	m_writer->putString(pic);
	m_writer->putBlank();
	m_writer->putString(tex);
	m_writer->putBlank();
	m_writer->putStringToken(swrap);
	m_writer->putBlank();
	m_writer->putStringToken(twrap);
	m_writer->putBlank();
	m_writer->putStringToken(filterfunc.name());
	m_writer->putBlank();
	m_writer->putValue(swidth);
	m_writer->putBlank();
	m_writer->putValue(twidth);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_BUMP);
	m_writer->putBlank();
	m_writer->putString(pic);
	m_writer->putBlank();
	m_writer->putString(tex);
	m_writer->putBlank();
	m_writer->putStringToken(swrap);
	m_writer->putBlank();
	m_writer->putStringToken(twrap);
	m_writer->putBlank();
	m_writer->putStringToken(filterfunc.name());
	m_writer->putBlank();
	m_writer->putValue(swidth);
	m_writer->putBlank();
	m_writer->putValue(twidth);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_LAT_LONG_ENVIRONMENT);
	m_writer->putBlank();
	m_writer->putString(pic);
	m_writer->putBlank();
	m_writer->putString(tex);
	m_writer->putBlank();
	m_writer->putStringToken(filterfunc.name());
	m_writer->putBlank();
	m_writer->putValue(swidth);
	m_writer->putBlank();
	m_writer->putValue(twidth);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_CUBE_FACE_ENVIRONMENT);
	m_writer->putBlank();
	m_writer->putString(px);
	m_writer->putBlank();
	m_writer->putString(nx);
	m_writer->putBlank();
	m_writer->putString(py);
	m_writer->putBlank();
	m_writer->putString(ny);
	m_writer->putBlank();
	m_writer->putString(pz);
	m_writer->putBlank();
	m_writer->putString(nz);
	m_writer->putBlank();
	m_writer->putString(tex);
	m_writer->putBlank();
	m_writer->putValue(fov);
	m_writer->putBlank();
	m_writer->putStringToken(filterfunc.name());
	m_writer->putBlank();
	m_writer->putValue(swidth);
	m_writer->putBlank();
	m_writer->putValue(twidth);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeShadow(RtString pic, RtString tex, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_SHADOW);
	m_writer->putBlank();
	m_writer->putString(pic);
	m_writer->putBlank();
	m_writer->putString(tex);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postMakeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MAKE_BRICK_MAP);
	m_writer->putBlank();
	m_writer->putArray(nNames, ptcnames);
	m_writer->putBlank();
	m_writer->putString(bkmname);
	writeParameterList(params);
}



RtVoid CRibWriter::postArchiveRecord(RtToken type, RtString line)
{
	if ( !postTestValid() )
		return;

	// Skip the Header
	if ( m_header && type == RI_STRUCTURE && m_skipHeader > 0 ) {
			return;
	}


	m_header = false;
	writePrefix(type == RI_STRUCTURE);
	m_writer->putComment(type, line);
}


bool CRibWriter::willExecuteMacro(RtString name) {
	bool isFile = true;
	bool macroPostponed = true;

	RtToken tname = renderState()->storedArchiveName(name);
	if ( tname ) {
		CRiArchiveMacro *m = renderState()->archiveInstance(tname);
		assert (m != 0);
		if ( m ) {
			isFile = m->macroType() == CRiArchiveMacro::MACROTYPE_FILE;
			macroPostponed = m->postpone();
		}
	}

	bool doExecute = false;

	if ( isFile ) {
		if ( !macroPostponed || m_postponeFileArchive <= 0) {
			doExecute = true;
			if ( m_postponeFileArchive < 0 ) {
				m_postponeFileArchive = 1;
			}
		}
	} else {
		if ( !macroPostponed || m_postponeInlineArchive == 0 ) {
			doExecute = true;
		}
	}

	return doExecute;
}

RtVoid CRibWriter::preReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	CBaseRenderer::preReadArchive(name, callback, params);
	m_execute = willExecuteMacro(name);
}


RtVoid CRibWriter::doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	if ( m_execute ) {
		bool exec = m_execute;
		m_header = true;
		CBaseRenderer::doReadArchive(name, callback, params);
		m_execute = exec;
	}
}


RtVoid CRibWriter::postReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	if ( !m_execute ) {
		if ( !postTestValid() )
			return;
		writePrefix();
		m_writer->putRequest(REQ_READ_ARCHIVE);
		m_writer->putBlank();
		m_writer->putString(name);
		m_writer->putBlank();
		writeParameterList(params);
	} else if ( renderState()->recordMode() ) {
	    // Also print the recorded macros (do-methodes are not called)
		// m_execute is true here, doReadArchive is not called because in record state
		doReadArchive(name, callback, params);
	}
}


RtVoid CRibWriter::postIfBegin(RtString expr)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_IF_BEGIN);
	m_writer->putBlank();
	m_writer->putString(expr);
	m_writer->putBlank();
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postElseIfBegin(RtString expr)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_ELSE_IF);
	m_writer->putBlank();
	m_writer->putString(expr);
	m_writer->putBlank();
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postElseBegin(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_ELSE);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postIfEnd(void)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_IF_END);
	m_writer->putNewLine();
}

