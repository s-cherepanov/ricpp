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

#ifndef _RICPP_TOOLS_ENV_H
#include "ricpp/tools/env.h"
#endif // _RICPP_TOOLS_ENV_H

#ifdef _DEBUG
// #define _TRACE_ARCHIVE
#define _TRACE
#ifndef _RICPP_TOOLS_TRACE_H
#include "ricpp/tools/trace.h"
#endif // _RICPP_TOOLS_TRACE_H
#endif

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


#define WRITE_RIB { \
writePrefix(); \
obj.writeRIB(*m_writer); \
}

#define TEST_WRITE_RIB { \
if ( !postTestValid() ) \
	return; \
writePrefix(); \
obj.writeRIB(*m_writer); \
}

#define TEST_WRITE_RIB_ARCHIVE_REC(AFLAG) { \
if ( !postTestValid() ) \
	return; \
writePrefix(AFLAG); \
obj.writeRIB(*m_writer); \
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
	TypeParent::defaultDeclarations();

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
	/** @todo move formatting to CRibElementsWriter */
	if ( !isArchiveRecord && !m_writer->firstRequestWritten() ) {
		version();
	}
	if ( m_indent && m_writer->ascii() ) {
		for ( unsigned long i = 0; i < nestingDepth(); ++i ) {
			m_writer->putChars(m_indentString.c_str());
		}
	}
}


RtVoid CRibWriter::doProcess(CRiControl &obj)
{
	TypeParent::doProcess(obj);

	if ( obj.name() == RI_RIBWRITER ) {
		CParameterList::const_iterator i;
		for ( i = obj.parameters().begin(); i != obj.parameters().end(); i++ ) {
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

	m_writer->putChars("version 3.04");
	m_writer->putNewLine();
	m_skipVersion = 1; // only output once
}


RtVoid CRibWriter::postProcess(CRiBegin &obj)
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
	CParameterList::const_iterator  i = obj.parameters().begin();
	for ( ; i != obj.parameters().end(); ++i ) {
		const CParameter &p = (*i);
		if ( p.token() == RI_FILE && p.strings().size() > 0 && !p.strings()[0].empty() ) {
			filename = p.strings()[0].c_str();
			renderState()->varSubst(filename, '$');
		}
		if ( p.token() == RI_COMPRESS && p.ints().size() > 0 ) {
			compress = p.ints()[0];
		}
	}

	if ( parserCallback() == 0 ) {
		return;
	}

	if ( filename.size() > 0 ) {

		if ( filename[0] == '|' ) {
			const char *cmd = filename.c_str()+1;
			if ( *cmd ) {
				m_cmd = cmd;

				filename = "";
				const char *tmpfile = CEnv::getTempFilename(filename, ".rib", false);
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
			if ( ptr && !(strcasecmp(ptr, ".ribz") && strcasecmp(ptr, ".z") && strcasecmp(ptr, ".gz")) && compress == 0 ) {
				compress = -1;
			}

		}

		if ( filename.size() > 0 ) {
			m_buffer = new CFrontStreambuf(parserCallback()->protocolHandlers());
			if ( !m_buffer ) {
				return;
			}
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
		std::cout.flush();
		m_writer = new CRibElementsWriter(std::cout.rdbuf(), *this);
		if ( !m_writer ) {
			// Error
			return;
		}
		m_writer->ascii(!m_binary);
	}

}

RtVoid CRibWriter::postProcess(CRiEnd &obj)
{
	if ( m_writer ) delete m_writer;
	m_writer = 0;
	if ( m_buffer ) {
		m_buffer->close();
		delete m_buffer;
	} else {
		std::cout.flush();
	}
	m_buffer = 0;

	if ( m_cmd.size() > 0 ) {
		std::string cmdline;
		cmdline += m_cmd;
		cmdline += " ";
		cmdline += m_nativepath;

		// std::cerr << "# " << cmdline << std::endl;
		::system(cmdline.c_str());
	}

	m_cmd = "";
	m_nativepath = "";
}


RtVoid CRibWriter::postProcess(CRiErrorHandler &obj)
TEST_WRITE_RIB

RtVoid CRibWriter::postProcess(CRiDeclare &obj)
TEST_WRITE_RIB

RtVoid CRibWriter::postProcess(CRiSynchronize &obj)
TEST_WRITE_RIB

RtVoid CRibWriter::postProcess(CRiSystem &obj)
TEST_WRITE_RIB

RtVoid CRibWriter::postProcess(CRiResource &obj)
TEST_WRITE_RIB

RtVoid CRibWriter::postProcess(CRiFrameBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiFrameEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiWorldBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiWorldEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiAttributeBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiAttributeEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiTransformBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiTransformEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiSolidBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiSolidEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiObjectBegin &obj)
{
	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);
	
	m_suppressOutputVector.push_back(m_suppressOutput);

	if ( !m ) {
		m_suppressOutput = false;
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Container not created for ObjectSource \"%s\"", noNullStr(obj.handle()));
		return;
	}

	m->postpone(m_postponeObject != 0);

	if ( !testValid() ) 
		return;

	if ( m->postpone() ) {
		if ( postTestValid() ) {
			writePrefix();
			if ( m->fromHandleId() ) {
				obj.writeRIB(*m_writer, m->handle());
			} else {
				obj.writeRIB(*m_writer, m->handleNo());
			}
			incNestingDepth();
		}
		m_suppressOutput = false;
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preProcess(CRiObjectEnd &obj)
{
	TypeParent::preProcess(obj);

	if ( !testValid() ) 
		return;

	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);
	if ( !m || m->postpone() ) {
		if ( postTestValid() ) {
			decNestingDepth();
			WRITE_RIB
		}
	}
}

RtVoid CRibWriter::postProcess(CRiObjectEnd &obj)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}

RtVoid CRibWriter::doProcess(CRiObjectInstance &obj)
{
	// Only write the ObjectInstance Request
	// Do this only if a definition was written to output (macro is postponed).
	// This is the "normal" case.
	if ( m_postponeObject ) {
		CRiMacro *m = renderState()->objectInstance(obj.handle());
		if ( !m || m->postpone() ) {
			return;
		}
	}


	// Put out the requests that form the object.
	// It doesn't matter if the Object definition was
	// written to output or not.
	TypeParent::doProcess(obj);
}


RtVoid CRibWriter::postProcess(CRiObjectInstance &obj)
{
	if ( m_postponeObject ) {
		if ( !postTestValid() )
			return;

		CRiMacro *m = renderState()->objectInstance(obj.handle());
		if ( !m ) {
			ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not found to objectInstance \"%s\"", noNullStr(obj.handle()));
			return;
		}
		
		if ( m->postpone() ) {
			// Put out ObjectInstance only if the object was postponed.
			writePrefix();
			if ( m->fromHandleId() ) {
				obj.writeRIB(*m_writer, m->handle());
			} else {
				obj.writeRIB(*m_writer, m->handleNo());
			}

		} else if ( renderState()->recordMode() ) {
			TypeParent::doProcess(obj);
		}
	} else if ( renderState()->recordMode() ) {
		TypeParent::doProcess(obj);
	}
}


RtVoid CRibWriter::postProcess(CRiMotionBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiMotionEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiResourceBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiResourceEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiArchiveBegin &obj)
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
			WRITE_RIB
			incNestingDepth();
		}
	} else {
		m_suppressOutput = true;
	}
}


RtVoid CRibWriter::preProcess(CRiArchiveEnd &obj)
{
	TypeParent::preProcess(obj);

	if ( !testValid() ) 
		return;

	CRiMacro *m = renderState()->curMacro();
	assert (m != 0);
	if ( !m || m->postpone() ) {
		if ( postTestValid() ) {
			decNestingDepth();
			WRITE_RIB
		}
	}
}


RtVoid CRibWriter::postProcess(CRiArchiveEnd &obj)
{
	if ( !m_suppressOutputVector.empty() ) {
		m_suppressOutput = m_suppressOutputVector.back();
		m_suppressOutputVector.pop_back();
	}
}


RtVoid CRibWriter::postProcess(CRiFormat &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiFrameAspectRatio &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiScreenWindow &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCropWindow &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiProjection &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiClipping &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiClippingPlane &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDepthOfField &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiShutter &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPixelVariance &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPixelSamples &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPixelFilter &obj)
{
	if ( !postTestValid() )
		return;
	if ( emptyStr(obj.filterFunc().name()) ) {
		// throw error
		throw ExceptRiCPPError(
			RIE_MISSINGDATA, RIE_ERROR,
			renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
			"Unknown name of pixel filter.");
	}
	WRITE_RIB
}


RtVoid CRibWriter::postProcess(CRiExposure &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiImager &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiQuantize &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDisplayChannel &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDisplay &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiHider &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiColorSamples &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiRelativeDetail &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiOption &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiLightSource &obj)
{
	if ( !postTestValid() )
		return;

	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for LightSource \"%s\"", noNullStr(obj.name()));
		return;
	}
	
	RtToken ignoreTokens[] = {RI_HANDLEID};

	writePrefix();
	if ( handle->fromHandleId() ) {
		obj.writeRIB(*m_writer, handle->handle(), 1, ignoreTokens);
	} else {
		obj.writeRIB(*m_writer, handle->handleNo(), 1, ignoreTokens);
	}
}


RtVoid CRibWriter::postProcess(CRiAreaLightSource &obj)
{
	if ( !postTestValid() )
		return;
	
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for AreaLightSource \"%s\"", noNullStr(obj.name()));
		return;
	}
	
	RtToken ignoreTokens[] = {RI_HANDLEID};
	
	writePrefix();
	if ( handle->fromHandleId() ) {
		obj.writeRIB(*m_writer, handle->handle(), 1, ignoreTokens);
	} else {
		obj.writeRIB(*m_writer, handle->handleNo(), 1, ignoreTokens);
	}
	
	if ( !postTestValid() )
		return;
}


RtVoid CRibWriter::postProcess(CRiAttribute &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiColor &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiOpacity &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiSurface &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiAtmosphere &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiInterior &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiExterior &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDisplacement &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDeformation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiTextureCoordinates &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiShadingRate &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiShadingInterpolation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMatte &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiBound &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDetail &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDetailRange &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiGeometricApproximation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiGeometricRepresentation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiOrientation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiReverseOrientation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiSides &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiBasis &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiTrimCurve &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiIlluminate &obj)
{
	if ( !postTestValid() )
		return;
	
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not found to illuminate \"%s\"", noNullStr(obj.handle()));
		return;
	}
	
	writePrefix();
	if ( handle->fromHandleId() ) {
		obj.writeRIB(*m_writer, handle->handle());
	} else {
		obj.writeRIB(*m_writer, handle->handleNo());
	}
}


RtVoid CRibWriter::postProcess(CRiIdentity &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiTransform &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiConcatTransform &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPerspective &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiTranslate &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiRotate &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiScale &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiSkew &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiScopedCoordinateSystem &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCoordinateSystem &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCoordSysTransform &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPolygon &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiGeneralPolygon &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPointsPolygons &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPointsGeneralPolygons &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPatch &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPatchMesh &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiNuPatch &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiSubdivisionMesh &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiHierarchicalSubdivisionMesh &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiSphere &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCone &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCylinder &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiHyperboloid &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiParaboloid &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiDisk &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiTorus &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiPoints &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiCurves &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiBlobby &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiGeometry &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::doProcess(CRiProcedural &obj)
{
	if ( !m_postponeProcedural ) {
		m_doReadArchive = true;
		// Can call doReadArchive()
		TypeParent::doProcess(obj);
		m_doReadArchive = false;
	}
}

RtVoid CRibWriter::postProcess(CRiProcedural &obj)
{
	if ( m_postponeProcedural ) {
		TEST_WRITE_RIB
	} else if ( renderState()->recordMode() ) {
		doProcess(obj);
	}
}


RtVoid CRibWriter::postProcess(CRiMakeTexture &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMakeBump &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMakeLatLongEnvironment &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMakeCubeFaceEnvironment &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMakeShadow &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiMakeBrickMap &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postProcess(CRiArchiveRecord &obj)
{
	// Skip the Header
	if ( m_header && obj.type() == RI_STRUCTURE && m_skipHeader > 0 ) {
		return;
	}

	TEST_WRITE_RIB_ARCHIVE_REC(obj.type() == RI_STRUCTURE)

	if ( obj.type() == RI_STRUCTURE )
		m_header = false;
}


bool CRibWriter::willExecuteMacro(RtString name) {
	if ( m_doReadArchive )
		return true;

	bool isFile = true;
	bool macroPostponed = true;

	RtToken tname = renderState()->storedArchiveName(name);
	if ( tname ) {
		CRiMacro *m = renderState()->findArchiveInstance(tname);
		assert (m != 0);
		if ( m ) {
			isFile = m->macroType() == CRiMacro::MACROTYPE_FILE;
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

RtVoid CRibWriter::preProcess(CRiReadArchive &obj)
{
#ifdef _TRACE_ARCHIVE
	trace("<- CRibWriter::preProcess(CRiReadArchive)");
#endif
	TypeParent::preProcess(obj);
	m_execute = willExecuteMacro(obj.name());
#ifdef _TRACE_ARCHIVE
	trace("-> CRibWriter::preProcess(CRiReadArchive)");
#endif
}


RtVoid CRibWriter::doProcess(CRiReadArchive &obj)
{
#ifdef _TRACE_ARCHIVE
	trace("<- CRibWriter::doProcess(CRiReadArchive)");
#endif
	m_doReadArchive = false; // Clear the procedural flag
	if ( m_execute ) {
		bool exec = m_execute;
		m_header = true;
		TypeParent::doProcess(obj);
		m_execute = exec;
	}
#ifdef _TRACE_ARCHIVE
	trace("-> CRibWriter::doProcess(CRiReadArchive)");
#endif
}


RtVoid CRibWriter::postProcess(CRiReadArchive &obj)
{
	if ( !m_execute ) {
		TEST_WRITE_RIB
	} else if ( renderState()->recordMode() ) {
	    // Also print the recorded macros (do-methodes are not called)
		// m_execute is true here, doReadArchive is not called because in record state
		doProcess(obj);
	}
}


RtVoid CRibWriter::postProcess(CRiIfBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiElseIfBegin &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiElseBegin &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postProcess(CRiIfEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}
