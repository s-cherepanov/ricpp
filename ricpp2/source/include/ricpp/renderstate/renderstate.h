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

#ifndef _RICPP_DECLARATION_DECLDICT_H
#include "ricpp/declaration/decldict.h"
#endif // _RICPP_DECLARATION_DECLDICT_H

namespace RiCPP {

/** @brief Interface to read the state of a renderer.
 *
 *  Implemented by CRenderState, the class that handles the rendering state.
 *  Seperating the reader from the complete state allows objects
 *  to query the state without being able to modify it. The member function
 *  of this calss are typically const for this reason.
 */
class IRenderStateReader {
public:
	/** @defgroup modestack_interface_group CModeStack functions
	 *  @brief Reading the modestack.
	 */
	//@{
	/** @brief Test if a request @a req is valid in the current mode.
	 *  @param req Index of the request to test.
	 *  @return true, if the request req is valid in the current mode.
	 *  @see CModeStack::validRequest(), EnumRequests  
	 */
	virtual bool validRequest(EnumRequests req) const = 0;

	/** @brief The current mode
	 *  @return The current mode
	 *  @see CModeStack::curMode(), EnumRequests  
	 */
	virtual EnumModes curMode() const = 0;


	/** @brief The current mode bits
	 *  @return The current mode bits
	 *  @see CModeStack::curModeBits(), EnumRequests  
	 */
	virtual EnumModes curModeBits() const = 0;

	virtual CModeStack::const_iterator modesBegin() const = 0;
	virtual CModeStack::const_iterator modesEnd() const = 0;
	virtual CModeStack::size_type modesSize() const = 0;
	//@}

	/** @defgroup tokenizer_interface_group CTokenizer functions
	 *  @brief Reading the tokenizer.
	 */
	//@{
	/** @brief Searches for a token
	 *  @param name A pointer to a token name
	 *  @return RI_NULL if token not found, token otherwise
	 */
	virtual RtToken tokFind(const char *name) const = 0;

	virtual CTokenizer::const_iterator tokBegin() const = 0;
	virtual CTokenizer::const_iterator tokEnd() const = 0;
	virtual CTokenizer::size_type tokSize() const = 0;
	//@}

	/** @defgroup decldict_interface_group CDeclarationDictionary functions
	 *  @brief Reading the declaration dictionary.
	 */
	//@{
	virtual const CDeclaration *declFind(RtToken name) const = 0;
	virtual const CDeclaration *declFind(const char *tableNamespace, const char *table, const char *var, const CTokenizer &tokenizer) const = 0;
	virtual CDeclarationDictionary::const_iterator declBegin() const = 0;
	virtual CDeclarationDictionary::const_iterator declEnd() const = 0;
	virtual CDeclarationDictionary::size_type declSize() const = 0;
	//@}
}; // IRenderStateReader

/** @brief The facade for the render state objects.
 *
 * The CRenderState object is a part of CBaseRenderer.
 */
class CRenderState : IRenderStateReader {
	CModeStack *m_modeStack;           ///< Pointer to a mode stack

	RtInt m_frameNumber;               ///< Frame number

	CDeclarationDictionary m_decldict; ///< Dictionary for declarations
	CTokenizer m_tokenizer;            ///< Registered tokens
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
	 *  @exception ERendererError if the token cannot be created (out of memory while filling map).
	 */
	inline CRenderState(CModeStack &aModeStack)
	// throw(ERendererError)
	{
		m_modeStack = &aModeStack;
		m_frameNumber = 0;
		// Attribute stack and Option stack follows after implemented
	}

	/** @brief Destroys the object
	 *
	 *  State objects are deleted
	 */
	inline ~CRenderState()
	{
		if ( m_modeStack ) {
			delete m_modeStack;
		}
	}

	/** @brief The current frame number (frames are not nested)
	 *
	 *  @param number The frame number
	 */
	inline void frameNumber(RtInt number) { m_frameNumber = number; }

	/** @brief Gets The current frame number (frames are not nested).
	 *
	 *  @return The frame number.
	 */
	inline RtInt frameNumber() const { return m_frameNumber; }

	/** @defgroup modestack_group  CModeStack functions
	 *  @brief Facading the modestack.
	 *  The modes are handled by the @c ...Begin(), @c ...End() functions
	 *  of CBaseRenderer
	 *  @see CModeStack
	 */
	//@{
	inline void contextBegin() { m_modeStack->contextBegin(); }
	inline void contextEnd() { m_modeStack->contextEnd(); }

	inline void frameBegin() { m_modeStack->frameBegin(); }
	inline void frameEnd() { m_modeStack->frameEnd(); }

	inline void worldBegin() { m_modeStack->worldBegin(); }
	inline void worldEnd() { m_modeStack->worldEnd(); }

	inline void attributeBegin() { m_modeStack->attributeBegin(); }
	inline void attributeEnd() { m_modeStack->attributeEnd(); }

	inline void transformBegin() { m_modeStack->transformBegin(); }
	inline void transformEnd() { m_modeStack->transformEnd(); }

    inline void solidBegin() { m_modeStack->solidBegin(); }
    inline void solidEnd() { m_modeStack->solidEnd(); }

	inline void objectBegin() { m_modeStack->objectBegin(); }
	inline void objectEnd() { m_modeStack->objectEnd(); }

    inline void archiveBegin() { m_modeStack->archiveBegin(); }
    inline void archiveEnd() { m_modeStack->archiveEnd(); }

    inline void resourceBegin() { m_modeStack->resourceBegin(); }
    inline void resourceEnd() { m_modeStack->resourceEnd(); }

    inline void ifBegin() { m_modeStack->ifBegin(); }
    inline void ifElseBegin() { m_modeStack->iElsefBegin(); }
    inline void elseBegin() { m_modeStack->elseBegin(); }
    inline void ifEnd() { m_modeStack->resourceEnd(); }

	inline void motionBegin() { return m_modeStack->motionBegin(); }
    inline void motionEnd() { return m_modeStack->motionEnd(); }

	inline virtual bool validRequest(EnumRequests req) const { return m_modeStack->validRequest(req); }
	inline virtual EnumModes curMode() const { return m_modeStack->curMode(); }
	inline virtual EnumModes curModeBits() const { return m_modeStack->curModeBits(); }

	inline CModeStack::const_iterator modesBegin() const { return m_modes.begin(); }
	inline CModeStack::const_iterator modesEnd() const { return m_modes.end(); }
	inline CModeStack::size_type modesSize() const { return m_modes.size(); }

	//@}

	/** @defgroup tokenizer_group CTokenizer functions
	 *  @brief Facading the tokenizer.
	 *
	 *  The tokenizer can be used to have unique strings as tokens RtToken.
	 *  RtToken can be compared by their address fastly.
	 *  @see CTokenizer
	 */
	//@{
	/** @brief Searches for a token and creates one if token name is not found
	 *  @param name A pointer to a token name
	 *  @return Unique RtToken
	 *  @exception ERendererError if the token cannot be created (out of memory).
	 */
	inline RtToken tokFindCreate(const char *name)
	// throw(ERendererException)
	{
		return m_tokenizer.findCreate(name);
	}

	inline virtual RtToken tokFind(const char *name) const { return m_tokenizer.find(name); }

	inline virtual CTokenizer::const_iterator tokBegin() const { return m_tokenizer.begin(); }
	inline virtual CTokenizer::const_iterator tokEnd() const { return m_tokenizer.end(); }
	inline virtual CTokenizer::size_type tokSize() const { return m_tokenizer.size(); }
	//@}

	/** @defgroup decldict_group CDeclarationDictionary functions
	 *  @brief Facading the dictionary of declarations.
	*
	 *  The dictionary is filled by CBaseRenderer::declare()
	 */
	//@{
	inline virtual const CDeclaration *declFind(RtToken name) const { return m_decldict.find(name); }
	inline virtual const CDeclaration *declFind(const char *tableNamespace, const char *table, const char *var, const CTokenizer &tokenizer) const { return m_decldict.find(tableNamespace, table, var, tokenizer); }
	inline virtual CDeclarationDictionary::const_iterator declBegin() const { return m_decldict.begin(); }
	inline virtual CDeclarationDictionary::const_iterator declEnd() const { return m_decldict.end(); }
	inline virtual CDeclarationDictionary::size_type declSize() const { return m_decldict.size(); }
	inline const CDeclaration *declFindAndUpdate(RtToken name, unsigned int curColorSize) { return m_decldict.findAndUpdate(name, curColorSize); }
	inline const CDeclaration *declFindAndUpdate(
		const char*tableNamespace,
		const char *table,
		const char *var,
		const CTokenizer &tokenizer,
		unsigned int curColorSize)
	{
		return m_decldict.findAndUpdate(tableNamespace, table, var, tokenizer, curColorSize);
	}
	inline void declAdd(CDeclaration *decl) { return m_decldict.add(decl); }
	//@}
}; // CRenderState

} // namespace RiCPP

#endif // _RICPP_RENDERSTATE_RENDERSTATE_H
