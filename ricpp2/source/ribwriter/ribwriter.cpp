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
		if ( p.token() == RI_FILE && p.strings().size() > 0 && !p.strings()[0].empty() ) {
			filename = p.strings()[0].c_str();
			renderState()->varSubst(filename, '$');
		}
		if ( p.token() == RI_COMPRESS && p.ints().size() > 0 ) {
			compress = p.ints()[0];
		}
	}

	if ( m_parserCallback == 0 ) {
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
			m_buffer = new TemplFrontStreambuf<char>(m_parserCallback->protocolHandlers());
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

		// std::cerr << "# " << cmdline << std::endl;
		::system(cmdline.c_str());
	}

	m_cmd = "";
	m_nativepath = "";
}


RtVoid CRibWriter::postErrorHandler(CRiErrorHandler &obj, const IErrorHandler &handler)
TEST_WRITE_RIB

RtVoid CRibWriter::postDeclare(CRiDeclare &obj, RtToken name, RtString declaration)
TEST_WRITE_RIB

RtVoid CRibWriter::postSynchronize(CRiSynchronize &obj, RtToken name)
TEST_WRITE_RIB

RtVoid CRibWriter::postSystem(CRiSystem &obj, RtString cmd)
TEST_WRITE_RIB

RtVoid CRibWriter::postResource(CRiResource &obj, RtToken handle, RtString type, const CParameterList &params)
TEST_WRITE_RIB

RtVoid CRibWriter::postFrameBegin(CRiFrameBegin &obj, RtInt number)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postFrameEnd(CRiFrameEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postWorldBegin(CRiWorldBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postWorldEnd(CRiWorldEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postAttributeBegin(CRiAttributeBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postAttributeEnd(CRiAttributeEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postTransformBegin(CRiTransformBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postTransformEnd(CRiTransformEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postSolidBegin(CRiSolidBegin &obj, RtToken type)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postSolidEnd(CRiSolidEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postObjectBegin(CRiObjectBegin &obj, RtString name)
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
			WRITE_RIB
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
		if ( !m ) {
			ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not found to objectInstance \"%s\"", noNullStr(handle));
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
			CBaseRenderer::doObjectInstance(obj, handle);
		}
	} else if ( renderState()->recordMode() ) {
		CBaseRenderer::doObjectInstance(obj, handle);
	}
}


RtVoid CRibWriter::postMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[])
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postMotionEnd(CRiMotionEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}


RtVoid CRibWriter::postResourceBegin(CRiResourceBegin &obj)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postResourceEnd(CRiResourceEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
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
			WRITE_RIB
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
			WRITE_RIB
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
TEST_WRITE_RIB


RtVoid CRibWriter::postFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect)
TEST_WRITE_RIB


RtVoid CRibWriter::postScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
TEST_WRITE_RIB


RtVoid CRibWriter::postCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
TEST_WRITE_RIB


RtVoid CRibWriter::postProjection(CRiProjection &obj, RtToken name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postClipping(CRiClipping &obj, RtFloat hither, RtFloat yon)
TEST_WRITE_RIB


RtVoid CRibWriter::postClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
TEST_WRITE_RIB


RtVoid CRibWriter::postDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
TEST_WRITE_RIB


RtVoid CRibWriter::postShutter(CRiShutter &obj, RtFloat smin, RtFloat smax)
TEST_WRITE_RIB


RtVoid CRibWriter::postPixelVariance(CRiPixelVariance &obj, RtFloat variation)
TEST_WRITE_RIB


RtVoid CRibWriter::postPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples)
TEST_WRITE_RIB


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
	WRITE_RIB
}


RtVoid CRibWriter::postExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma)
TEST_WRITE_RIB


RtVoid CRibWriter::postImager(CRiImager &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
TEST_WRITE_RIB


RtVoid CRibWriter::postDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postHider(CRiHider &obj, RtToken type, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[])
TEST_WRITE_RIB


RtVoid CRibWriter::postRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail)
TEST_WRITE_RIB


RtVoid CRibWriter::postOption(CRiOption &obj, RtToken name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postLightSource(CRiLightSource &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;

	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for LightSource \"%s\"", noNullStr(name));
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


RtVoid CRibWriter::postAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params)
{
	if ( !postTestValid() )
		return;
	
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not created for AreaLightSource \"%s\"", noNullStr(name));
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


RtVoid CRibWriter::postAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postColor(CRiColor &obj, RtColor Cs)
TEST_WRITE_RIB


RtVoid CRibWriter::postOpacity(CRiOpacity &obj, RtColor Os)
TEST_WRITE_RIB


RtVoid CRibWriter::postSurface(CRiSurface &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postInterior(CRiInterior &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postExterior(CRiExterior &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postDeformation(CRiDeformation &obj, RtString name, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
TEST_WRITE_RIB


RtVoid CRibWriter::postShadingRate(CRiShadingRate &obj, RtFloat size)
TEST_WRITE_RIB


RtVoid CRibWriter::postShadingInterpolation(CRiShadingInterpolation &obj, RtToken type)
TEST_WRITE_RIB


RtVoid CRibWriter::postMatte(CRiMatte &obj, RtBoolean onoff)
TEST_WRITE_RIB


RtVoid CRibWriter::postBound(CRiBound &obj, RtBound bound)
TEST_WRITE_RIB


RtVoid CRibWriter::postDetail(CRiDetail &obj, RtBound bound)
TEST_WRITE_RIB


RtVoid CRibWriter::postDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
TEST_WRITE_RIB


RtVoid CRibWriter::postGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value)
TEST_WRITE_RIB


RtVoid CRibWriter::postGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type)
TEST_WRITE_RIB


RtVoid CRibWriter::postOrientation(CRiOrientation &obj, RtToken anOrientation)
TEST_WRITE_RIB


RtVoid CRibWriter::postReverseOrientation(CRiReverseOrientation &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postSides(CRiSides &obj, RtInt nsides)
TEST_WRITE_RIB


RtVoid CRibWriter::postBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
TEST_WRITE_RIB


RtVoid CRibWriter::postTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
TEST_WRITE_RIB


RtVoid CRibWriter::postIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff)
{
	if ( !postTestValid() )
		return;
	
	CHandle *handle = renderState()->lightSourceHandle(light);
	if ( !handle ) {
		ricppErrHandler().handleError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Handle not found to illuminate \"%s\"", noNullStr(light));
		return;
	}
	
	writePrefix();
	if ( handle->fromHandleId() ) {
		obj.writeRIB(*m_writer, handle->handle());
	} else {
		obj.writeRIB(*m_writer, handle->handleNo());
	}
}


RtVoid CRibWriter::postIdentity(CRiIdentity &obj)
TEST_WRITE_RIB


RtVoid CRibWriter::postTransform(CRiTransform &obj, RtMatrix aTransform)
TEST_WRITE_RIB


RtVoid CRibWriter::postConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform)
TEST_WRITE_RIB


RtVoid CRibWriter::postPerspective(CRiPerspective &obj, RtFloat fov)
TEST_WRITE_RIB


RtVoid CRibWriter::postTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz)
TEST_WRITE_RIB


RtVoid CRibWriter::postRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
TEST_WRITE_RIB


RtVoid CRibWriter::postScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz)
TEST_WRITE_RIB


RtVoid CRibWriter::postSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
TEST_WRITE_RIB


RtVoid CRibWriter::postScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space)
TEST_WRITE_RIB


RtVoid CRibWriter::postCoordinateSystem(CRiCoordinateSystem &obj, RtToken space)
TEST_WRITE_RIB


RtVoid CRibWriter::postCoordSysTransform(CRiCoordSysTransform &obj, RtToken space)
TEST_WRITE_RIB


RtVoid CRibWriter::postPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postPatch(CRiPatch &obj, RtToken type, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postPoints(CRiPoints &obj, RtInt npts, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params)
TEST_WRITE_RIB


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
		TEST_WRITE_RIB
	} else if ( renderState()->recordMode() ) {
		doProcedural(obj, data, bound, subdivfunc, freefunc);
	}
}


RtVoid CRibWriter::postMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params)
TEST_WRITE_RIB


RtVoid CRibWriter::postArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line)
{
	// Skip the Header
	if ( m_header && type == RI_STRUCTURE && m_skipHeader > 0 ) {
		return;
	}

	TEST_WRITE_RIB_ARCHIVE_REC(type == RI_STRUCTURE)

	if ( type == RI_STRUCTURE )
		m_header = false;
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
		TEST_WRITE_RIB
	} else if ( renderState()->recordMode() ) {
	    // Also print the recorded macros (do-methodes are not called)
		// m_execute is true here, doReadArchive is not called because in record state
		doReadArchive(obj, name, callback, params);
	}
}


RtVoid CRibWriter::postIfBegin(CRiIfBegin &obj, RtString expr)
{
	TEST_WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postElseIfBegin(CRiElseIfBegin &obj, RtString expr)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postElseBegin(CRiElseBegin &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
	incNestingDepth();
}


RtVoid CRibWriter::postIfEnd(CRiIfEnd &obj)
{
	if ( !postTestValid() )
		return;

	decNestingDepth();
	WRITE_RIB
}
