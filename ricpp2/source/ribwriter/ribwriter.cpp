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
1   Won't cache files

*/

using namespace RiCPP;

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
	m_cmd = "";
	m_nativepath = "";

	RI_RIBWRITER = RI_NULL;

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

	RI_QUAL_POSTPONE_PROCEDURALS = RI_NULL;
	RI_QUAL_POSTPONE_OBJECTS = RI_NULL;
	RI_QUAL_POSTPONE_FILE_ARCHIVES = RI_NULL;
	RI_QUAL_POSTPONE_INLINE_ARCHIVES = RI_NULL;
	RI_QUAL_SKIP_HEADERS = RI_NULL;
	RI_QUAL_SKIP_VERSION = RI_NULL;
	RI_QUAL_INDENT = RI_NULL;
	RI_QUAL_INDENT_STRING = RI_NULL;
	RI_QUAL_SUPPRESS_OUTPUT = RI_NULL;
	RI_QUAL_BINARY_OUTPUT = RI_NULL;

	
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
	
	m_header = true;
	m_execute = false;
	m_doReadArchive = false;
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

	// Additional tokens
	RI_RIBWRITER =                renderState()->tokFindCreate("ribwriter");
	
	RI_POSTPONE_PROCEDURALS =     renderState()->tokFindCreate("postpone-procedurals");
	RI_POSTPONE_OBJECTS =         renderState()->tokFindCreate("postpone-objects");
	RI_POSTPONE_FILE_ARCHIVES =   renderState()->tokFindCreate("postpone-file-archives");
	RI_POSTPONE_INLINE_ARCHIVES = renderState()->tokFindCreate("postpone-inline-archives");
	RI_SKIP_HEADERS =             renderState()->tokFindCreate("skip-headers");
	RI_SKIP_VERSION =             renderState()->tokFindCreate("skip-version");
	RI_INDENT =                   renderState()->tokFindCreate("indent");
	RI_INDENT_STRING =            renderState()->tokFindCreate("indent-string");
	RI_SUPPRESS_OUTPUT =          renderState()->tokFindCreate("suppress-output");
	RI_BINARY_OUTPUT =            renderState()->tokFindCreate("binary-output");
	
	// Declarations
	RI_COMPRESS =                 renderState()->declare("compress", "constant integer", true);

	RI_QUAL_POSTPONE_PROCEDURALS =     renderState()->declare("Control:ribwriter:postpone-procedurals",     "constant integer", true);
	RI_QUAL_POSTPONE_OBJECTS =         renderState()->declare("Control:ribwriter:postpone-objects",         "constant integer", true);
	RI_QUAL_POSTPONE_FILE_ARCHIVES =   renderState()->declare("Control:ribwriter:postpone-file-archives",   "constant integer", true);
	RI_QUAL_POSTPONE_INLINE_ARCHIVES = renderState()->declare("Control:ribwriter:postpone-inline-archives", "constant integer", true);
	RI_QUAL_SKIP_HEADERS =             renderState()->declare("Control:ribwriter:skip-headers",             "constant integer", true);
	RI_QUAL_SKIP_VERSION =             renderState()->declare("Control:ribwriter:skip-version",             "constant integer", true);
	RI_QUAL_INDENT =                   renderState()->declare("Control:ribwriter:indent",                   "constant integer", true);
	RI_QUAL_INDENT_STRING =            renderState()->declare("Control:ribwriter:indent-string",            "constant string",  true);
	RI_QUAL_SUPPRESS_OUTPUT =          renderState()->declare("Control:ribwriter:suppress-output",          "constant integer", true);
	RI_QUAL_BINARY_OUTPUT =            renderState()->declare("Control:ribwriter:binary-output",            "constant integer", true);
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

void CRibWriter::writeParameterList(const CParameterList &params, RtInt n, RtToken ignoreTokens[])
{
	bool paramError = false;
	for ( CParameterList::const_iterator i = params.begin();
		  i != params.end();
		  i++ )
	{
		const CParameter &p = (*i);

		RtInt cnt = 0;
		for ( ; cnt < n; ++cnt ) {
			if ( p.token() == ignoreTokens[cnt] )
				break;
		}
		if ( cnt < n ) continue;
		
		m_writer->putBlank();

		if ( p.isInline() )
			m_writer->putString(p.parameterName());
		else
			m_writer->putStringToken(p.parameterName());

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
				// Error unknown type
				paramError = true;
				m_writer->putChars("[ ]");
				break;
		}
	}

	m_writer->putNewLine();
	if ( paramError ) {
		// throw error
		throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown basic type of parameters indicated by [ ].");
	}
}


RtVoid CRibWriter::doControl(CRiControl &obj, RtToken name, const CParameterList &params)
{
	CBaseRenderer::doControl(obj, name, params);

	if ( name == RI_RIBWRITER ) {
		CParameterList::const_iterator i;
		for ( i = params.begin(); i != params.end(); i++ ) {
			if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_POSTPONE_PROCEDURALS) ) {
				(*i).get(0, m_postponeProcedural);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_POSTPONE_OBJECTS) ) {
				(*i).get(0, m_postponeObject);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_POSTPONE_FILE_ARCHIVES) ) {
				(*i).get(0, m_postponeFileArchive);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_POSTPONE_INLINE_ARCHIVES) ) {
				(*i).get(0, m_postponeInlineArchive);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_SKIP_HEADERS) ) {
				(*i).get(0, m_skipHeader);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_SKIP_VERSION) ) {
				(*i).get(0, m_skipVersion);
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_INDENT) ) {
				RtInt intval;
				if ( (*i).get(0, intval) ) {
					m_indent = intval != 0;
				}
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_INDENT_STRING) ) {
				RtString strval = 0;
				if ( (*i).get(0, strval) ) {
					m_indentString = noNullStr(strval);
				}
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_SUPPRESS_OUTPUT) ) {
				RtInt intval;
				if ( (*i).get(0, intval) ) {
					m_controlSuppressOutput = intval != 0;
				}
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_RIBWRITER, RI_BINARY_OUTPUT) ) {
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

	if ( m_header && m_skipHeader <= 0 ) {
		m_writer->putComment(RI_STRUCTURE, "RenderMan RIB-Structure 1.1");
	}

	if ( m_skipHeader < 0 )
		m_skipHeader = 1;

	m_header = false;

	if ( m_skipVersion != 0 )
		return;

	m_writer->putChars("version 3.03");
	m_writer->putNewLine();
	m_skipVersion = 1; // only output once
}


RtVoid CRibWriter::postBegin(CRiBegin &obj, RtString name, const CParameterList &params)
{
	if ( m_buffer ) delete m_buffer;
	m_buffer = 0;
	if ( m_writer ) delete m_writer;
	m_writer = 0;
	
	nestingDepth(0);

	m_cmd = "";
	m_nativepath = "";

	RtInt compress = 0;
	std::string filename;
	CParameterList::const_iterator  i = params.begin();
	for ( ; i != params.end(); ++i ) {
		const CParameter &p = (*i);
		if ( p.token() == RI_FILE && p.strings().size() > 0 ) {
			CStringList stringList;
			stringList.expand(filename, p.strings()[0].c_str(), true);
		}
		if ( p.token() == RI_COMPRESS && p.ints().size() > 0 ) {
			compress = p.ints()[0];
		}
	}

	if ( m_parserCallback == 0 ) {
		return;
	}

	if ( filename.size() > 0 ) {
		m_buffer = new TemplFrontStreambuf<char>(m_parserCallback->protocolHandlers());
		if ( !m_buffer ) {
			return;
		}

		if ( filename[0] == '|' ) {
			const char *cmd = filename.c_str()+1;
			if ( *cmd ) {
				m_cmd = cmd;

				filename = "";
				const char *tmpfile = CEnv::getTempFilename(filename, "rib", false);
				if ( tmpfile ) {
					m_nativepath = filename;
				} else {
					m_cmd = "";
					// Error
				}
			} else {
				filename = "";
				// stdio
			}

		} else {

			const char *ptr = strrchr(filename.c_str(), '.');
			if ( ptr && !(strcasecmp(ptr, ".ribz") && strcasecmp(ptr, ".z") && strcasecmp(ptr, ".gz")) && compress < 0 ) {
				compress = -1;
			}

		}

		if ( filename.size() > 0 ) {
			CUri fileuri;
			fileuri.encodeFilepath(filename.c_str(), "file");
			if ( !m_buffer->open(fileuri, std::ios_base::out|std::ios_base::binary, compress) ) {
				return;
			}
			m_writer = new CRibElementsWriter(m_buffer, *this);
			if ( !m_writer ) {
				// Error
				return;
			}
			m_writer->ascii(!m_binary);
		}
	}
	
	if ( filename.size() == 0 ) {
		// stdio
		m_writer = new CRibElementsWriter(std::cout.rdbuf(), *this);
		if ( !m_writer ) {
			// Error
			return;
		}
		m_writer->ascii(!m_binary);
	}
}

RtVoid CRibWriter::postEnd(CRiEnd &obj)
{
	if ( m_writer ) delete m_writer;
	m_writer = 0;
	if ( m_buffer ) {
		m_buffer->close();
		delete m_buffer;
	}
	m_buffer = 0;

	if ( m_cmd.size() > 0 ) {
		std::string cmdline;
		cmdline += m_cmd;
		cmdline += " ";
		cmdline += m_nativepath;

		::system(cmdline.c_str());
	}

	m_cmd = "";
	m_nativepath = "";
}


RtVoid CRibWriter::postErrorHandler(CRiErrorHandler &obj, const IErrorHandler &handler)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ERROR_HANDLER);
	m_writer->putBlank();
	m_writer->putStringToken(handler.name());
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDeclare(CRiDeclare &obj, RtToken name, RtString declaration)
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


RtVoid CRibWriter::postSynchronize(CRiSynchronize &obj, RtToken name)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SYNCHRONIZE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSystem(CRiSystem &obj, RtString cmd)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SYSTEM);
	m_writer->putBlank();
	m_writer->putString(cmd);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResource(CRiResource &obj, RtToken handle, RtString type, const CParameterList &params)
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


RtVoid CRibWriter::postFrameBegin(CRiFrameBegin &obj, RtInt number)
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


RtVoid CRibWriter::postFrameEnd(CRiFrameEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_FRAME_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postWorldBegin(CRiWorldBegin &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_WORLD_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postWorldEnd(CRiWorldEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_WORLD_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postAttributeBegin(CRiAttributeBegin &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postAttributeEnd(CRiAttributeEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransformBegin(CRiTransformBegin &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postTransformEnd(CRiTransformEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSolidBegin(CRiSolidBegin &obj, RtToken type)
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


RtVoid CRibWriter::postSolidEnd(CRiSolidEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_SOLID_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postObjectBegin(CRiObjectBegin &obj, RtString name)
{
	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);

	if ( m ) {
		m->postpone(m_postponeObject != 0);
	}

	m_suppressOutputVector.push_back(m_suppressOutput);

	if ( !testValid() ) 
		return;

	if ( !m || m->postpone() ) {
		if ( postTestValid() ) {
			writePrefix();
			m_writer->putRequest(REQ_OBJECT_BEGIN);
			m_writer->putBlank();
			if ( m ) {
				if ( m->fromHandleId() ) {
					m_writer->putStringToken(m->handle());
				} else {
					m_writer->putValue(m->handleNo());
				}
			} else {
				m_writer->putStringToken(m->handle());
			}
			m_writer->putNewLine();
			incNestingDepth();
		}
		m_suppressOutput = false;
		if ( !m ) {
			ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Container not created for ObjectSource \"%s\"", noNullStr(obj.handle()));
		}
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preObjectEnd(CRiObjectEnd &obj)
{
	CBaseRenderer::preObjectEnd(obj);

	if ( !testValid() ) 
		return;

	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);
	if ( !m || m->postpone() ) {
		if ( postTestValid() ) {
			decNestingDepth();
			writePrefix();
			m_writer->putRequest(REQ_OBJECT_END);
			m_writer->putNewLine();
		}
	}
}

RtVoid CRibWriter::postObjectEnd(CRiObjectEnd &obj)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}

RtVoid CRibWriter::doObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle)
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
	CBaseRenderer::doObjectInstance(obj, handle);
}


RtVoid CRibWriter::postObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle)
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
			if ( m ) {
				if ( m->fromHandleId() ) {
					m_writer->putStringToken(m->handle());
				} else {
					m_writer->putValue(m->handleNo());
				}
			} else {
				m_writer->putStringToken(m->handle());
			}
			m_writer->putNewLine();
		} else if ( renderState()->recordMode() ) {
			CBaseRenderer::doObjectInstance(obj, handle);
		}
	} else if ( renderState()->recordMode() ) {
		CBaseRenderer::doObjectInstance(obj, handle);
	}
}


RtVoid CRibWriter::postMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[])
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


RtVoid CRibWriter::postMotionEnd(CRiMotionEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_MOTION_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postResourceBegin(CRiResourceBegin &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_RESOURCE_BEGIN);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postResourceEnd(CRiResourceEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_RESOURCE_END);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params)
{
	if ( !testValid() ) 
		return;

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
			incNestingDepth();
		}
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preArchiveEnd(CRiArchiveEnd &obj)
{
	CBaseRenderer::preArchiveEnd(obj);

	if ( !testValid() ) 
		return;

	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);
	if ( !m || m->postpone() ) {
		if ( postTestValid() ) {
			decNestingDepth();
			writePrefix();
			m_writer->putRequest(REQ_ARCHIVE_END);
			m_writer->putNewLine();
		}
	}
}


RtVoid CRibWriter::postArchiveEnd(CRiArchiveEnd &obj)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}


RtVoid CRibWriter::postFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect)
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


RtVoid CRibWriter::postFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_FRAME_ASPECT_RATIO);
	m_writer->putBlank();
	m_writer->putValue(aspect);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
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


RtVoid CRibWriter::postCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
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


RtVoid CRibWriter::postProjection(CRiProjection &obj, RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PROJECTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postClipping(CRiClipping &obj, RtFloat hither, RtFloat yon)
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


RtVoid CRibWriter::postClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
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


RtVoid CRibWriter::postDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
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


RtVoid CRibWriter::postShutter(CRiShutter &obj, RtFloat smin, RtFloat smax)
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


RtVoid CRibWriter::postPixelVariance(CRiPixelVariance &obj, RtFloat variation)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PIXEL_VARIANCE);
	m_writer->putBlank();
	m_writer->putValue(variation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples)
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


RtVoid CRibWriter::postPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	if ( !postTestValid() )
		return;
	if ( emptyStr(function.name()) ) {
		// throw error
		throw ExceptRiCPPError(
			RIE_MISSINGDATA, RIE_ERROR,
			renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
			"Unknown name of pixel filter.");
	}
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


RtVoid CRibWriter::postExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma)
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


RtVoid CRibWriter::postImager(CRiImager &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_IMAGER);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
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


RtVoid CRibWriter::postDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DISPLAY_CHANNEL);
	m_writer->putBlank();
	m_writer->putString(channel);
	writeParameterList(params);
}


RtVoid CRibWriter::postDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params)
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


RtVoid CRibWriter::postHider(CRiHider &obj, RtToken type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_HIDER);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[])
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


RtVoid CRibWriter::postRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_RELATIVE_DETAIL);
	m_writer->putBlank();
	m_writer->putValue(relativedetail);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOption(CRiOption &obj, RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_OPTION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postLightSource(CRiLightSource &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		m_writer->putValue((unsigned long)0);
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for LightSource \"%s\"", noNullStr(name));
	} else {
		if ( handle->fromHandleId() ) {
			m_writer->putStringToken(handle->handle());
		} else {
			m_writer->putValue(handle->handleNo());
		}
	}
	RtToken ignoreTokens[] = {RI_HANDLEID};
	writeParameterList(params, 1, ignoreTokens);
}


RtVoid CRibWriter::postAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_AREA_LIGHT_SOURCE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	m_writer->putBlank();
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		m_writer->putValue((unsigned long)0);
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for AreaLightSource \"%s\"", noNullStr(name));
	} else {
		if ( handle->fromHandleId() ) {
			m_writer->putStringToken(handle->handle());
		} else {
			m_writer->putValue(handle->handleNo());
		}
	}
	RtToken ignoreTokens[] = {RI_HANDLEID};
	writeParameterList(params, 1, ignoreTokens);
}


RtVoid CRibWriter::postAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATTRIBUTE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postColor(CRiColor &obj, RtColor Cs)
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


RtVoid CRibWriter::postOpacity(CRiOpacity &obj, RtColor Os)
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


RtVoid CRibWriter::postSurface(CRiSurface &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SURFACE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ATMOSPHERE);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postInterior(CRiInterior &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_INTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postExterior(CRiExterior &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_EXTERIOR);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ILLUMINATE);
	m_writer->putBlank();
	CHandle *handle = renderState()->lightSourceHandle(light);
	if ( !handle ) {
		m_writer->putValue((unsigned long)0);
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not found to illuminate \"%s\"", noNullStr(light));
	} else {
		if ( handle->fromHandleId() ) {
			m_writer->putStringToken(handle->handle());
		} else {
			m_writer->putValue(handle->handleNo());
		}
	}
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DISPLACEMENT);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
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


RtVoid CRibWriter::postShadingRate(CRiShadingRate &obj, RtFloat size)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SHADING_RATE);
	m_writer->putBlank();
	m_writer->putValue(size);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postShadingInterpolation(CRiShadingInterpolation &obj, RtToken type)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SHADING_INTERPOLATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postMatte(CRiMatte &obj, RtBoolean onoff)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_MATTE);
	m_writer->putBlank();
	m_writer->putValue((RtInt)(onoff?1:0));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBound(CRiBound &obj, RtBound bound)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_BOUND);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtBound)/sizeof(bound[0]), &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetail(CRiDetail &obj, RtBound bound)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DETAIL);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtBound)/sizeof(bound[0]), &bound[0]);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
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


RtVoid CRibWriter::postGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value)
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


RtVoid CRibWriter::postGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_GEOMETRIC_REPRESENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postOrientation(CRiOrientation &obj, RtToken anOrientation)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_ORIENTATION);
	m_writer->putBlank();
	m_writer->putStringToken(anOrientation);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postReverseOrientation(CRiReverseOrientation &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_REVERSE_ORIENTATION);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postSides(CRiSides &obj, RtInt nsides)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SIDES);
	m_writer->putBlank();
	m_writer->putValue(nsides);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
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


RtVoid CRibWriter::postTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
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


RtVoid CRibWriter::postIdentity(CRiIdentity &obj)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_IDENTITY);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTransform(CRiTransform &obj, RtMatrix aTransform)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_TRANSFORM);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtMatrix)/sizeof(aTransform[0][0]), &(aTransform[0][0]));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_CONCAT_TRANSFORM);
	m_writer->putBlank();
	m_writer->putArray(sizeof(RtMatrix)/sizeof(aTransform[0][0]), &(aTransform[0][0]));
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPerspective(CRiPerspective &obj, RtFloat fov)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PERSPECTIVE);
	m_writer->putBlank();
	m_writer->putValue(fov);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz)
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


RtVoid CRibWriter::postRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
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


RtVoid CRibWriter::postScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz)
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


RtVoid CRibWriter::postSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
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


RtVoid CRibWriter::postDeformation(CRiDeformation &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_DEFORMATION);
	m_writer->putBlank();
	m_writer->putStringToken(name);
	writeParameterList(params);
}


RtVoid CRibWriter::postScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_SCOPED_COORDINATE_SYSTEM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postCoordinateSystem(CRiCoordinateSystem &obj, RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_COORDINATE_SYSTEM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postCoordSysTransform(CRiCoordSysTransform &obj, RtToken space)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_COORD_SYS_TRANSFORM);
	m_writer->putBlank();
	m_writer->putStringToken(space);
	m_writer->putNewLine();
}


RtVoid CRibWriter::postPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POLYGON);
	writeParameterList(params);
}


RtVoid CRibWriter::postGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_GENERAL_POLYGON);
	m_writer->putBlank();
	m_writer->putArray(nloops, nverts);
	writeParameterList(params);
}


RtVoid CRibWriter::postPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params)
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


RtVoid CRibWriter::postPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params)
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


RtVoid CRibWriter::postPatch(CRiPatch &obj, RtToken type, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_PATCH);
	m_writer->putBlank();
	m_writer->putStringToken(type);
	writeParameterList(params);
}


RtVoid CRibWriter::postPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params)
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


RtVoid CRibWriter::postNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params)
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


RtVoid CRibWriter::postSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params)
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


RtVoid CRibWriter::postHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params)
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


RtVoid CRibWriter::postSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params)
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


RtVoid CRibWriter::postPoints(CRiPoints &obj, RtInt npts, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	writePrefix();
	m_writer->putRequest(REQ_POINTS);
	m_writer->putBlank();
	writeParameterList(params);
}


RtVoid CRibWriter::postCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params)
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


RtVoid CRibWriter::postBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params)
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



RtVoid CRibWriter::doProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
{
	if ( !m_postponeProcedural ) {
		m_doReadArchive = true;
		// Can call doReadArchive()
		CBaseRenderer::doProcedural(obj, data, bound, subdivfunc, freefunc);
		m_doReadArchive = false;
	}
}

RtVoid CRibWriter::postProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
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
		doProcedural(obj, data, bound, subdivfunc, freefunc);
	}
}


RtVoid CRibWriter::postGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params)
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


RtVoid CRibWriter::postMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
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


RtVoid CRibWriter::postMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
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


RtVoid CRibWriter::postMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
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


RtVoid CRibWriter::postMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
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


RtVoid CRibWriter::postMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params)
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


RtVoid CRibWriter::postMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params)
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



RtVoid CRibWriter::postArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line)
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
	if ( m_doReadArchive )
		return true;

	bool isFile = true;
	bool macroPostponed = true;

	RtToken tname = renderState()->storedArchiveName(name);
	if ( tname ) {
		CRiArchiveMacro *m = renderState()->findArchiveInstance(tname);
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

RtVoid CRibWriter::preReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	CBaseRenderer::preReadArchive(obj, name, callback, params);
	m_execute = willExecuteMacro(name);
}


RtVoid CRibWriter::doReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
	m_doReadArchive = false; // Clear the procedural flag
	if ( m_execute ) {
		bool exec = m_execute;
		m_header = true;
		CBaseRenderer::doReadArchive(obj, name, callback, params);
		m_execute = exec;
	}
}


RtVoid CRibWriter::postReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params)
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
		doReadArchive(obj, name, callback, params);
	}
}


RtVoid CRibWriter::postIfBegin(CRiIfBegin &obj, RtString expr)
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


RtVoid CRibWriter::postElseIfBegin(CRiElseIfBegin &obj, RtString expr)
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


RtVoid CRibWriter::postElseBegin(CRiElseBegin &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_ELSE);
	m_writer->putNewLine();
	incNestingDepth();
}


RtVoid CRibWriter::postIfEnd(CRiIfEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	writePrefix();
	m_writer->putRequest(REQ_IF_END);
	m_writer->putNewLine();
}

