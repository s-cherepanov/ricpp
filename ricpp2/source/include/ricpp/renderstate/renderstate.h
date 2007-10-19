#ifndef _RICPP_RENDERSTATE_RENDERSTATE_H
#define _RICPP_RENDERSTATE_RENDERSTATE_H

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

/** @file renderstate.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Facade for the render state objects (like modes, options, attributes)
 */

#ifndef _RICPP_RENDERSTATE_MODESTACK_H
#include "ricpp/renderstate/modestack.h"
#endif // _RICPP_RENDERSTATE_MODESTACK_H

#ifndef _RICPP_RENDERSTATE_OPTIONS_H
#include "ricpp/renderstate/options.h"
#endif //  _RICPP_RENDERSTATE_OPTIONS_H

#ifndef _RICPP_RENDERSTATE_ATTRIBUTES_H
#include "ricpp/renderstate/attributes.h"
#endif //  _RICPP_RENDERSTATE_ATTRIBUTES_H

#ifndef _RICPP_RENDERSTATE_TRANSFORMATION_H
#include "ricpp/renderstate/transformation.h"
#endif // _RICPP_RENDERSTATE_TRANSFORMATION_H

#ifndef _RICPP_RENDERSTATE_LIGHTS_H
#include "ricpp/renderstate/lights.h"
#endif // _RICPP_RENDERSTATE_LIGHTS_H

#ifndef _RICPP_RENDERSTATE_HANDLESTACK_H
#include "ricpp/renderstate/handlestack.h"
#endif // _RICPP_RENDERSTATE_HANDLESTACK_H

#ifndef _RICPP_STREAMS_URI_H
#include "ricpp/streams/uri.h"
#endif // _RICPP_STREAMS_URI_H

#ifndef _RICPP_RENDERSTATE_RESOURCE_H
#include "ricpp/renderstate/resource.h"
#endif // _RICPP_RENDERSTATE_RESOURCE_H

namespace RiCPP {

class CRManInterfaceFactory;
class CRiMacro;
class CRiObjectMacro;
class CRiArchiveMacro;

/** @brief The facade for the render state objects.
 *
 * The CRenderState object is a part of CBaseRenderer.
 */
class CRenderState {
	CModeStack *m_modeStack;                       ///< Pointer to a mode stack.

	RtInt m_frameNumber;                           ///< Frame number.

	std::string m_archiveName;                     ///< Current archive name, optional.
	long m_lineNo;                                 ///< Current line number in the file, -1 if not available.

	std::vector<RtToken> m_solidTypes;             ///< Stack with the types of solid blocks (if currently opened)

	CDeclarationDictionary m_declDict;             ///< Dictionary for declarations.

	TemplObjPtrRegistry<RtToken, IResourceFactory *> m_resourceFactories; ///< new RiResource handlers
	TemplHandleStack<CResource> m_resourceStack;

	COptionsFactory *m_optionsFactory;             ///< Create new Options.
	std::vector<COptions *> m_optionsStack;        ///< Current option stack.

	CAttributesFactory *m_attributesFactory;       ///< Create new attributes.
	std::vector<CAttributes *> m_attributesStack;  ///< Current attributes stack.

	std::vector<CTransformation> m_transformStack; ///< Current Stack of transformations and theire inverses.

	CParameterList m_curParams;                    ///< Params of the last interface request with variable parameters.

	CLights m_lights;                              ///< Global light list.
	
	bool m_reject;                                 ///< Reject requests while running, e.g. for appropriate if-then-else blocks

	/** @brief Only update the state, do no rendering (legacy).
	 *
	 * Update the state (handled by the base renderer), but do no more. E.g. if the RIB
	 * writer wants to postpone the archive reading, object instanciating etc.
	 *
	 * @todo remove this
	 */
	bool m_updateStateOnly;

	/** @brief Postpone the archive reading to the backend.
	 *
	 *  The RIB writer must be capable to print a ReadArchive request instead of
	 *  printing the contents of the archive. However, the
	 *  archive must be interpreted to maintain the render state.
	 *  m_postponeArchive is used to postpone BeginArchive/EndArchive macros.
	 */
	bool m_postponeReadArchive;

	bool m_postponeObject; ///< Postpone object instanciation where applicable (e.g. in RIB writer)
	bool m_postponeArchive; ///< Postpone archive instanciation where applicable (e.g. in RIB writer, @see m_postponeReadArchive to postpone archive reading)
	bool m_postponeCondition; ///< Postpone conditionals where applicable (e.g. in RIB writer)

	CUri m_baseUri;                                ///< Base URI for RIB archive files

	/** @brief Factory for macro interfaces
	 */
	CRManInterfaceFactory *m_macroFactory;

	/** @brief Points to current writeable macro
	 */
	CRiMacro *m_curMacro;

	CRiMacro *m_curReplay;

	TemplHandleStack<CRiObjectMacro> m_objectMacros;
	TemplHandleStack<CRiArchiveMacro> m_archiveMacros;
	std::vector<CRiMacro *> m_macros;

	std::vector<bool> m_conditions;
	bool m_curCondition; //!< Render (true outside if-else-blocks, conditional inside the blocks

	void pushOptions();
	bool popOptions();

	void pushAttributes();
	bool popAttributes();

	void pushTransform();
	bool popTransform();

	void curMacro(CRiMacro *m)
	{
		m_curMacro = m;
	}

public:

	/** @brief Initializes the object
	 *
	 * The state object is created and
	 * initialized, done at CBaseRenderer::begin() of the rendering context.
	 * All state objects must get a value != 0, begin() shall throw
	 * an error if some objects cannot be created.
	 * Rendering is not possible (context should be aborted)
	 * in the case that the state cannot be fully initialized.
	 *
	 *  @param aModeStack A reference to a valid mode stack
	 *  @exception ExceptRiCPPError if the token cannot be created (out of memory while filling map).
	 */
	CRenderState(
		CModeStack &aModeStack,
		COptionsFactory &optionsFactory,
		CAttributesFactory &attributesFactory,
		CLightSourceFactory &lightSourceFactory,
		CRManInterfaceFactory &aMacroFactory);

	/** @brief Destroys the object
	 *
	 *  State objects are deleted
	 */
	virtual ~CRenderState();

	/** @brief The current frame number (frames are not nested)
	 *
	 *  @param number The frame number
	 */
	inline virtual void frameNumber(RtInt number) { m_frameNumber = number; }

	/** @brief The current frame number.
	 *
	 *  Set 0 if outside a frame, but has no meaning, can also be inside
	 *  rame with number 0.
	 *
	 *  @return The current frame Number.
	 */
	inline virtual RtInt frameNumber() const { return m_frameNumber; }

	/** @defgroup modestack_group  CModeStack functions
	 *  @brief Facading the modestack.
	 *  The modes are handled by the @c ...Begin(), @c ...End() functions
	 *  of CBaseRenderer
	 *  @see CModeStack
	 */
	//@{
	inline void contextBegin()
	{
		m_lights.mark();
		pushOptions();
		pushAttributes();
		pushTransform();
		m_modeStack->contextBegin();
	}
	inline void contextEnd()
	{
		m_modeStack->contextEnd();
		popTransform();
		popAttributes();
		popOptions();
		m_lights.clearToMark();
	}

	inline void frameBegin(RtInt number)
	{
		m_lights.mark();
		pushOptions();
		pushAttributes();
		pushTransform();
		m_modeStack->frameBegin();
		frameNumber(number);
	}
	inline void frameEnd()
	{
		frameNumber(0);
		m_modeStack->frameEnd();
		popTransform();
		popAttributes();
		popOptions();
		m_lights.clearToMark();
	}

	inline void worldBegin()
	{
		pushTransform();
		pushAttributes();
		m_modeStack->worldBegin();
	}
	inline void worldEnd()
	{
		m_modeStack->worldEnd();
		popAttributes();
		popTransform();
		m_lights.clearToMark();
	}

	inline void attributeBegin()
	{
		pushTransform();
		pushAttributes();
		m_modeStack->attributeBegin();
	}
	inline void attributeEnd()
	{
		m_modeStack->attributeEnd();
		popAttributes();
		popTransform();
	}

	inline void transformBegin()
	{
		pushTransform();
		m_modeStack->transformBegin();
	}
	inline void transformEnd()
	{
		m_modeStack->transformEnd();
		popTransform();
	}

    void solidBegin(RtToken type);
    void solidEnd();
	RtToken solid() const;

	RtObjectHandle objectBegin();
	void objectEnd();

	CRiObjectMacro *objectInstance(RtObjectHandle handle);
	const CRiObjectMacro *objectInstance(RtObjectHandle handle) const;

	RtArchiveHandle archiveBegin(const char *aName);
	void archiveEnd();

	CRiArchiveMacro *archiveInstance(RtArchiveHandle handle);
	const CRiArchiveMacro *archiveInstance(RtArchiveHandle handle) const;

	inline void resourceBegin()
	{
		m_modeStack->resourceBegin();
		m_resourceStack.mark();
	}

    inline void resourceEnd()
	{
		m_modeStack->resourceEnd();
		m_resourceStack.clearToMark();
	}

    void resource(IRiContext &ri, RtString handle, RtString type, const CParameterList &params);

	inline void motionBegin() { return m_modeStack->motionBegin(); }
    inline void motionEnd() { return m_modeStack->motionEnd(); }

	inline bool curCondition() const
	{
		return m_curCondition;
	}

	void pushConditional();
	void popConditional();

	inline void ifBegin(bool condition) {
		m_modeStack->ifBegin();
		pushConditional();
		m_curCondition = condition;
	}
	inline void elseIfBegin(bool condition) {
		m_modeStack->elseIfBegin();
		m_curCondition = !m_curCondition && condition;
	}
	inline void elseBegin(bool condition)
	{
		m_modeStack->elseBegin();
		m_curCondition = !m_curCondition;
	}
	inline void ifEnd()
	{
		m_modeStack->ifEnd();
		popConditional();
	}

	/** @brief Tests if a request @a req is valid in the current mode.
	 *  @param req Index of the request to test.
	 *  @return true, if the request req is valid in the current mode.
	 *  @see CModeStack::validRequest(), EnumRequests  
	 */
	inline virtual bool validRequest(EnumRequests req) const { return m_modeStack->validRequest(req); }

	/** @brief Gets the current mode.
	 *
	 *  @return The current mode.
	 *  @see CModeStack::curMode(), EnumRequests  
	 */
	inline virtual EnumModes curMode() const { return m_modeStack->curMode(); }

	/** @brief Gets the current mode bits.
	 *
	 *  @return The current mode bits.
	 *  @see CModeStack::curModeBits(), EnumRequests  
	 */
	inline virtual TypeModeBits curModeBits() const { return m_modeStack->curModeBits(); }

	/** @brief Tests if the renderer is inside a world block.
	 *
	 *  @return true, the renderer is inside a world block.
	 */
	inline virtual bool inWorldBlock() const
	{
		return (curModeBits() & MODE_BIT_WORLD) == MODE_BIT_WORLD;
	}


	inline CModeStack::const_iterator modesBegin() const { return m_modeStack->begin(); }
	inline CModeStack::const_iterator modesEnd() const { return m_modeStack->end(); }
	inline CModeStack::size_type modesSize() const { return m_modeStack->size(); }

	/** @brief Get the area light source handle, if inside a area light definition.
	 *
	 *  @return The handle of the actually defined area light source or
	 *          illLightHandle, if outside an are light definition.
	 */
	inline virtual RtLightHandle areaLightSourceHandle() const { return m_modeStack->areaLightSourceHandle(); }

	/** @brief The nesting depth of begin of the area light source.
	 *
	 *  The area light source is closed if the enclosing attribute (world, frame)
	 *  block is closed. Area light sources cannot be nested.
	 *
	 *  @return Nesting level of mode of the current light source or 0 if
	 *          not defining area light source.
	 */
	inline virtual CModeStack::size_type areaLightSourceDepth() const { return m_modeStack->areaLightSourceDepth(); }

	virtual void startAreaLightSource(RtLightHandle h);
	virtual void endAreaLightSource();
	//@}

	/** @defgroup tokenmap_group CTokenMap functions
	 *  @brief Facading the tokenmap.
	 *
	 *  The tokenmap can be used to have unique strings as tokens RtToken.
	 *  RtToken can be compared by their address fastly.
	 *  @see CTokenMap
	 */
	//@{
	/** @brief Searches for a token and creates one if token name is not found
	 *  @param name A pointer to a token name
	 *  @return Unique RtToken
	 *  @exception ExceptRiCPPError if the token cannot be created (out of memory).
	 */
	inline RtToken tokFindCreate(const char *name)
	// throw(ERendererException)
	{
		return tokenMap().findCreate(name);
	}

	inline virtual const CTokenMap &tokenMap() const {return m_declDict.tokenMap();}
	inline CTokenMap &tokenMap() {return m_declDict.tokenMap();}

	/** @brief Searches for a token
	 *  @param name A pointer to a token name
	 *  @return RI_NULL if token not found, token otherwise
	 */
	inline virtual RtToken tokFind(const char *name) const { return tokenMap().find(name); }
	virtual void parseParameters(CParameterList &p, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);
	virtual void parseParameters(const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);

	inline virtual RtInt numTokens() const
	{
		return static_cast<RtInt>(m_curParams.size());
	}
	inline virtual CTokenMap::const_iterator tokBegin() const { return tokenMap().begin(); }
	inline virtual CTokenMap::const_iterator tokEnd() const { return tokenMap().end(); }
	inline virtual CTokenMap::size_type tokSize() const { return tokenMap().size(); }
	
	inline virtual CParameterList &curParamList() { return m_curParams; }
	inline virtual const CParameterList &curParamList() const { return m_curParams; }
	inline virtual RtToken *tokens() { return m_curParams.tokenPtr(); }
	inline virtual RtPointer *params() { return m_curParams.valuePtr(); }
	//@}

	/** @defgroup decldict_group CDeclarationDictionary functions
	 *  @brief Facading the dictionary of declarations.
	*
	 *  The dictionary is filled by CBaseRenderer::declare()
	 */
	//@{
	inline virtual const CDeclaration *declFind(RtToken name) const { return m_declDict.find(name); }
	inline virtual const CDeclaration *declFind(RtToken tableNamespace, RtToken table, RtToken var) const { return m_declDict.find(tableNamespace, table, var); }
	
	inline const CDeclaration *declFindAndUpdate(RtToken name, const CColorDescr &curColorDescr) { return m_declDict.findAndUpdate(name, curColorDescr); }
	inline const CDeclaration *declFindAndUpdate(
		const char*tableNamespace,
		const char *table,
		const char *var,
		const CColorDescr &curColorDescr)
	{
		return m_declDict.findAndUpdate(tableNamespace, table, var, curColorDescr);
	}
	
	inline void declAdd(CDeclaration *decl)
	{
		return m_declDict.add(decl);
	}
	
	inline virtual CDeclarationDictionary::const_iterator declBegin() const { return m_declDict.begin(); }
	inline virtual CDeclarationDictionary::const_iterator declEnd() const { return m_declDict.end(); }
	inline virtual CDeclarationDictionary::size_type declSize() const { return m_declDict.size(); }

	inline CDeclarationDictionary &dict()
	{
		return m_declDict;
	}
	//@}

	inline COptions &options()
	{
		assert(m_optionsStack.back() != 0);
		return *(m_optionsStack.back());
	}

	inline virtual const COptions &options() const
	{
		assert(m_optionsStack.back() != 0);
		return *(m_optionsStack.back());
	}

	inline CAttributes &attributes()
	{
		assert(m_attributesStack.back() != 0);
		return *(m_attributesStack.back());
	}

	inline virtual const CAttributes &attributes() const
	{
		assert(m_attributesStack.back() != 0);
		return *(m_attributesStack.back());
	}

	inline CTransformation &transform()
	{
		assert(!m_transformStack.empty());
		return m_transformStack.back();
	}

	inline virtual const CTransformation &transform() const
	{
		assert(!m_transformStack.empty());
		return m_transformStack.back();
	}

	inline CLights &lights() { return m_lights; }

	inline virtual const CLights &lights() const { return m_lights; }

	inline virtual bool hasMacroFactory() const {return m_macroFactory != 0;}

	inline virtual bool hasOptions() const {return !m_optionsStack.empty() && m_optionsStack.back() != 0;}
	inline virtual bool hasAttributes() const {return !m_attributesStack.empty() && m_attributesStack.back() != 0;}
	inline virtual bool hasTransform() const {return !m_transformStack.empty();}

	inline virtual const CUri &baseUri() const { return m_baseUri; }
	inline virtual CUri &baseUri() { return m_baseUri; }

	inline virtual const char *archiveName() const
	{
		return m_archiveName.c_str();
	}

	inline virtual void archiveName(const char *anArchiveName)
	{
		m_archiveName = noNullStr(anArchiveName);
	}

	inline virtual long lineNo() const
	{
		return m_lineNo;
	}

	inline virtual void lineNo(long aLineNo)
	{
		m_lineNo = aLineNo;
	}

	inline virtual bool reject() const
	{
		return m_reject;
	}

	inline virtual void reject(bool doRecect)
	{
		m_reject = doRecect;
	}
	
	inline virtual bool updateStateOnly() const
	{
		return m_updateStateOnly;
	}

	inline virtual void updateStateOnly(bool doOnlyUpdate)
	{
		m_updateStateOnly = doOnlyUpdate;
	}
	
	inline virtual bool postponeReadArchive() const
	{
		return m_postponeReadArchive;
	}

	inline virtual void postponeReadArchive(bool doPostponeReadArchive)
	{
		m_postponeReadArchive = doPostponeReadArchive;
	}

	inline virtual bool postponeArchive() const
	{
		return m_postponeArchive;
	}

	inline virtual void postponeArchive(bool doPostponeArchive)
	{
		m_postponeArchive = doPostponeArchive;
	}

	inline virtual bool postponeObject() const
	{
		return m_postponeObject;
	}

	inline virtual void postponeObject(bool doPostponeObject)
	{
		m_postponeObject = doPostponeObject;
	}

	inline virtual bool postponeCondition() const
	{
		return m_postponeCondition;
	}

	inline virtual void postponeCondition(bool doPostponeCondition)
	{
		m_postponeCondition = doPostponeCondition;
	}

	inline virtual const char *printName(const char *aFileName) const
	{
		if ( !m_archiveName.empty() )
			return m_archiveName.c_str();
		return noNullStr(aFileName);
	}
	
	inline virtual long printLineNo(long aLineNo) const
	{
		if ( !m_archiveName.empty() )
			return m_lineNo;
		return aLineNo;
	}

	virtual CRManInterfaceFactory &macroFactory();
	virtual const CRManInterfaceFactory &macroFactory() const;

	inline CRiMacro *curMacro()
	{
		return m_curMacro;
	}

	inline const CRiMacro *curMacro() const
	{
		return m_curMacro;
	}

	inline void curReplay(CRiMacro *m)
	{
		m_curReplay = m;
	}

	inline CRiMacro *curReplay()
	{
		return m_curReplay;
	}

	inline const CRiMacro *curReplay() const
	{
		return m_curReplay;
	}

	RtToken storedArchiveName(RtString archiveName) const;

	void registerResourceFactory(IResourceFactory *f);
}; // CRenderState

} // namespace RiCPP

#endif // _RICPP_RENDERSTATE_RENDERSTATE_H
