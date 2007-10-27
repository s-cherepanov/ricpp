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

/** @file renderstate.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the facade for the render state objects (like modes, options, attributes)
 */

#include "ricpp/ricontext/renderstate.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;

CRenderState::CRenderState(
	CModeStack &aModeStack,
	COptionsFactory &optionsFactory,
	CAttributesFactory &attributesFactory,
	CLightSourceFactory &lightSourceFactory
	// , CRManInterfaceFactory &aMacroFactory
	) :
	m_resourceFactories(true),
	m_lights(lightSourceFactory),
	m_objectMacros("OBJ_"),
	m_archiveMacros("ARC_")
// throw(ExceptRiCPPError)
{
	m_modeStack = &aModeStack;
	m_optionsFactory = &optionsFactory;
	m_attributesFactory = &attributesFactory;
	// m_macroFactory = &aMacroFactory;
	m_frameNumber = 0;
	m_lineNo = -1;

	m_curMacro = 0;
	m_curReplay = 0;

	m_reject = false;
	m_updateStateOnly = false;

	m_postponeReadArchive = true;
	m_postponeObject = true;
	m_postponeArchive = true;
	m_postponeCondition = true;

	m_curCondition = true;

	CFilepath fp;
	std::string s(fp.filepath());
	s+= "/";
	m_baseUri.set("file", "", s.c_str(), 0, 0);
}


inline void CRenderState::pushConditional()
{
	try {
		m_conditions.push_back(m_curCondition);
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in pushConditional(): %s", e.what());
	}
}

inline void CRenderState::popConditional()
{
	try {
		m_conditions.pop_back();
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NESTING, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in popConditional(): %s", e.what());
	}
}

CRenderState::~CRenderState()
{
	while ( popOptions() );
	if ( m_optionsFactory ) {
		delete m_optionsFactory;
	}

	while ( popAttributes() );
	if ( m_attributesFactory ) {
		delete m_attributesFactory;
	}

	m_transformStack.clear();

	if ( m_modeStack ) {
		delete m_modeStack;
	}

	/*
	if ( m_macroFactory ) {
		delete m_macroFactory;
	}
	*/
}

void CRenderState::solidBegin(RtToken type)
{
	m_modeStack->solidBegin();
	m_solidTypes.push_back(type);
}

void CRenderState::solidEnd()
{
	if ( !m_solidTypes.empty() )
		m_solidTypes.pop_back();
	m_modeStack->solidEnd();
}

RtToken CRenderState::solid() const
{
	if ( m_solidTypes.empty() )
		return RI_NULL;
	return m_solidTypes.back();
}

CRiObjectMacro *CRenderState::objectInstance(RtObjectHandle handle)
{
	return m_objectMacros.find(handle);
}

const CRiObjectMacro *CRenderState::objectInstance(RtObjectHandle handle) const
{
	return m_objectMacros.find(handle);
}

RtObjectHandle CRenderState::objectBegin()
{
	pushOptions();
	pushAttributes();
	pushTransform();
	m_modeStack->objectBegin();

	m_macros.push_back(m_curMacro);
	CRiObjectMacro *m = new CRiObjectMacro;
	curMacro(m);

	if ( curCondition() || curMacro() != 0 ) {
		if ( m != 0 ) {
			m->postpone(postponeObject());
			m_objectMacros.insertObject(m);
			return m->handle();
		} else {
			return illObjectHandle;
		}
	}
	
	return illObjectHandle;
}

void CRenderState::objectEnd()
{
	m_modeStack->objectEnd();
	popTransform();
	popAttributes();
	popOptions();
	
	if ( curCondition()  || curMacro() != 0 ) {
		if ( m_curMacro != 0 )
			m_curMacro->close();
		if ( !m_macros.empty() ) {
			m_curMacro = m_macros.back();
			m_macros.pop_back();
		} else {
			m_curMacro = 0;
		}
	}
}

CRiArchiveMacro *CRenderState::archiveInstance(RtArchiveHandle handle)
{
	return m_archiveMacros.find(handle);
}

const CRiArchiveMacro *CRenderState::archiveInstance(RtArchiveHandle handle) const
{
	return m_archiveMacros.find(handle);
}

RtArchiveHandle CRenderState::archiveBegin(const char *aName)
{
	pushOptions();
	pushAttributes();
	pushTransform();
	m_modeStack->archiveBegin();

	if ( curCondition() || curMacro() != 0 ) {
		m_macros.push_back(m_curMacro);
		CRiArchiveMacro *m = new CRiArchiveMacro(aName);
		curMacro(m);

		if ( m != 0 ) {
			m->postpone(postponeArchive());
			m_archiveMacros.insertObject(m);
			return m->handle();
		} else {
			return illArchiveHandle;
		}
	}

	return illArchiveHandle;
}

void CRenderState::archiveEnd() {
	m_modeStack->archiveEnd();
	popTransform();
	popAttributes();
	popOptions();

	if ( curCondition() || curMacro() != 0 ) {
		if ( !m_macros.empty() ) {
			if ( m_curMacro != 0 )
				m_curMacro->close();
			m_curMacro = m_macros.back();
			m_macros.pop_back();
		} else {
			m_curMacro = 0;
		}
	}
}


void CRenderState::resource(IRiContext &ri, RtString handle, RtString type, const CParameterList &params)
{
	RtToken t = tokFind(type);
	if ( !t ) {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "not a token for a resource factory in resource(%s, Token: %s)", noNullStr(handle), noNullStr(type));
	}

	IResourceFactory *f = m_resourceFactories.findObj(t);
	if ( !f ) {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "no resource factory in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
	}

	CResource *r = 0;
	if ( f && f->overwrites(params) ) {
		r = m_resourceStack.find(m_resourceStack.identify(handle), true);
		if ( !r ) {
			r = f->getResource(handle);
			if ( !r ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "no resource factory in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
			}
			m_resourceStack.insertObject(r);
		}
	} else {
		r = m_resourceStack.find(m_resourceStack.identify(handle), false);
		if ( !r ) {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "no valid resource handle found in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
		}
	}
	r->operate(ri, params);
}

void CRenderState::pushOptions()
{
	try {

		if ( m_optionsStack.empty() ) {
			m_optionsStack.push_back(m_optionsFactory->newOptions());
		} else {
			m_optionsStack.push_back(m_optionsFactory->newOptions(*m_optionsStack.back()));
		}
		if ( m_optionsStack.back() == 0 ) {
			m_optionsStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushOptions()", __LINE__, __FILE__);
		}

	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushOptions(): %s", e.what());
	}
}

bool CRenderState::popOptions()
{
	if ( !m_optionsStack.empty() ) {
		m_optionsFactory->deleteOptions(m_optionsStack.back());
		m_optionsStack.pop_back();
	}
	return !m_optionsStack.empty();
}

void CRenderState::pushAttributes()
{
	try {

		if ( m_attributesStack.empty() ) {
			m_attributesStack.push_back(m_attributesFactory->newAttributes(m_optionsStack.back()->colorDescr()));
		} else {
			m_attributesStack.push_back(m_attributesFactory->newAttributes(*m_attributesStack.back()));
		}
		if ( m_attributesStack.back() == 0 ) {
			m_attributesStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushAttributes()", __LINE__, __FILE__);
		}

	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushAttributes(): %s", e.what());
	}
}

bool CRenderState::popAttributes()
{
	if ( !m_attributesStack.empty() ) {
		m_attributesFactory->deleteAttributes(m_attributesStack.back());
		m_attributesStack.pop_back();
	}
	return !m_attributesStack.empty();
}

void CRenderState::pushTransform()
{
	try {
		m_transformStack.push_back(CTransformation());
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushTransform(): %s", e.what());
	}
}

bool CRenderState::popTransform()
{
	if ( !m_transformStack.empty() ) {
		m_transformStack.pop_back();
	}
	return !m_transformStack.empty();
}


void CRenderState::startAreaLightSource(RtLightHandle h)
{
	try {
		m_modeStack->startAreaLightSource(h);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::endAreaLightSource()
{
	try {
		m_modeStack->endAreaLightSource();
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}


void CRenderState::parseParameters(CParameterList &p, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		p.set(counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::parseParameters(const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		m_curParams.set(counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

/*
CRManInterfaceFactory &CRenderState::macroFactory()
{
	assert(m_macroFactory != 0);
	return *m_macroFactory;
}

const CRManInterfaceFactory &CRenderState::macroFactory() const
{
	assert(m_macroFactory != 0);
	return *m_macroFactory;
}
*/

RtToken CRenderState::storedArchiveName(RtString archiveName) const
{
	return m_archiveMacros.identify(archiveName);
}

void CRenderState::registerResourceFactory(IResourceFactory *f)
{
	if ( !f )
		return;
	RtToken t = tokFindCreate(f->type());
	f->registerOperations(tokenMap());
	m_resourceFactories.registerObj(t, f);
}
