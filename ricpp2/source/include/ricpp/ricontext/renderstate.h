#ifndef _RICPP_RICONTEXT_RENDERSTATE_H
#define _RICPP_RICONTEXT_RENDERSTATE_H

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

#ifndef _RICPP_RICONTEXT_MODESTACK_H
#include "ricpp/ricontext/modestack.h"
#endif // _RICPP_RICONTEXT_MODESTACK_H

#ifndef _RICPP_RICONTEXT_OPTIONS_H
#include "ricpp/ricontext/options.h"
#endif //  _RICPP_RICONTEXT_OPTIONS_H

#ifndef _RICPP_RICONTEXT_ATTRIBUTES_H
#include "ricpp/ricontext/attributes.h"
#endif //  _RICPP_RICONTEXT_ATTRIBUTES_H

#ifndef _RICPP_RICONTEXT_TRANSFORMATION_H
#include "ricpp/ricontext/transformation.h"
#endif // _RICPP_RICONTEXT_TRANSFORMATION_H

#ifndef _RICPP_RICONTEXT_LIGHTS_H
#include "ricpp/ricontext/lights.h"
#endif // _RICPP_RICONTEXT_LIGHTS_H

#ifndef _RICPP_RICONTEXT_HANDLESTACK_H
#include "ricpp/ricontext/handlestack.h"
#endif // _RICPP_RICONTEXT_HANDLESTACK_H

#ifndef _RICPP_STREAMS_URI_H
#include "ricpp/streams/uri.h"
#endif // _RICPP_STREAMS_URI_H

#ifndef _RICPP_RICONTEXT_RESOURCE_H
#include "ricpp/ricontext/resource.h"
#endif // _RICPP_RICONTEXT_RESOURCE_H

#ifndef _RICPP_RICONTEXT_MOTIONSTATE_H
#include "ricpp/ricontext/motionstate.h"
#endif // _RICPP_RICONTEXT_MOTIONSTATE_H

#include <list>

namespace RiCPP {

class CRManInterfaceFactory;
class CRiMacro;
class CRiObjectMacro;
class CRiArchiveMacro;

/** @brief The facade for the render state objects.
 *
 * The CRenderState object is a part of CBaseRenderer.
 *
 * @todo Remove STL from public interfaces of shared libraries (@see Creating Compatible Libraries in Apples C++ Runtime Environment Programming Guide)
 */
class CRenderState {
	typedef std::map<RtToken, CTransformation> TypeTransformationMap;

	CModeStack *m_modeStack;                       ///< Pointer to the mode stack, has to be set

	RtInt m_frameNumber;                           ///< Current frame number.

	std::string m_archiveName;                     ///< Current archive name, optional.
	long m_lineNo;                                 ///< Current line number in the file, -1 if not available.

	std::vector<RtToken> m_solidTypes;             ///< Stack with the nested types of solid blocks (if currently opened solid block)

	CDeclarationDictionary m_declDict;             ///< Dictionary for declarations.

	CMotionState m_motionState;                    ///< Current request id and motion state.

	TemplObjPtrRegistry<RtToken, IResourceFactory *> m_resourceFactories; ///< Creates new RiResource handlers
	TemplHandleStack<CResource> m_resourceStack;   ///< Saved resources

	COptionsBase m_controls;                       ///< Current control values.

	COptionsFactory *m_optionsFactory;             ///< Creates new Options.
	std::vector<COptions *> m_optionsStack;        ///< Current option stack.

	CAttributesFactory *m_attributesFactory;       ///< Create new attributes.
	std::vector<CAttributes *> m_attributesStack;  ///< Current attributes stack.

	std::vector<CTransformation> m_transformStack; ///< Current stack of transformations and their inverses.

	TypeTransformationMap m_globalTransforms;      ///< Global transformation map.
	std::list<TypeTransformationMap> m_scopedTransforms; ///< Scoped transformation maps.

	CParameterList m_curParams;                    ///< Params of the last interface request with variable parameters.

	CLights m_lights;                              ///< Global light list (state of the lightsources is part of the attributes)
	
	CFilterFuncFactory *m_filterFuncFactory;       ///< Factory for pixel filter functions.

	bool m_reject;                                 ///< true, rejects requests while running, e.g. for appropriate if-then-else blocks

	/** @brief Backend is in record mode.
	 *
	 *  The request are recorded in a macro only, the do-methods are not called (@see CBaserenderer::processRequest())
	 *  If a rib stream content is cached in a macro via readArchive, the do-methods have to be called if not
	 *  within a macro definition (ArchiveBegin/End or ObjectBegin/End with a relaxed set of valid interface calls).
	 *
	 *  @see objectbegin(), objectEnd(), archiveBegin(), archiveEnd(), archiveFileBegin(), archiveFileEnd()
	 */
	bool m_recordMode;
	
	/** @brief Stack of recordmodes, @see m_recordMode
	 */
	std::vector<bool> m_recordModes;

	CUri m_baseUri;           //!< Base URI for current RIB archive file

	/* @brief Factory for macro interfaces (not used)
	 */
	// CRManInterfaceFactory *m_macroFactory;

	/** @brief Points to current writeable macro
	 */
	CRiMacro *m_curMacro;

	/** @brief Points to current replayed macro
	 */
	CRiMacro *m_curReplay;

	TemplHandleStack<CRiObjectMacro> m_objectMacros;   //!< Stack of all object macros (objectBegin/End)
	TemplHandleStack<CRiArchiveMacro> m_archiveMacros; //!< Stack of all archive macros (archiveBegin/End or cached rib)
	
	std::vector<CRiMacro *> m_macros; //!< Vector with pointer to all macros (m_objectMacros and m_archiveMacros)

	std::vector<bool> m_conditions; //!< Stack of m_executeConditional and m_ifCondition for nested ifs.
	bool m_executeConditional;      //!< Render (true outside if-else-blocks, conditional inside the blocks).
	bool m_accumulateConditional;   //!< Render (true outside if-else-blocks, cummulated m_executeConditional inside the blocks).
	bool m_ifCondition;             //!< true, if an if or elseif condition was true;

	void pushOptions();    //!< Pushes the current options set.
	bool popOptions();     //!< Pops an options set, restores the set to the last condition.

	void pushAttributes(); //!< Pushes the current attributes set.
	bool popAttributes();  //!< Pops an attributes set, restores the set to the last condition.

	void pushTransform();   //!< Pushes the current transformation matrix and is inverse.
	bool popTransform();    //!< Pops the current transformation matrices, restores the matrices to the last condition.
	
	void pushConditional(); //!< Pushes conditionals within nested if-then-else blocks
	void popConditional();  //!< Restores the last state of the conditionals

	bool varsplit(RtString identifier, RtToken *namespaceQual, RtToken *varname, RtToken *valuename) const;
	bool getAttribute(CValue &p, RtToken varname, RtToken valuename) const;
	bool getOption(CValue &p, RtToken varname, RtToken valuename) const;
	bool getControl(CValue &p, RtToken varname, RtToken valuename) const;

	/** @brief Parser for RIB if-expression.
	 *
	 * The grammer of the if-expression, white spaces (wss) between the tokens are omitted.
	 *
	 @verbatim
		if-expr         : expr '\0';

		alpha           : ['a'-'z' 'A'-'Z'];
		digit_not_null  : ['1'-'9'];
		digit           : '0' | digit;
		hexdig          : ['a'-'f' 'A'-'F'] | digit;
		octdig          : ['0'-'7'];

		ws              : ' ' | '\t' | '\n' | '\r' | '\f';
		wss             : ws (ws)*;
		idchar          : alpha | digit | '_';
		sign            : '+' | '-';
		character       : '\' ['\' ''''];
		                | [^ '''' '\0'];

		name            : (idchar)+;
		                | (idchar)+ ':' (idchar)+;
		                | (idchar)+ ':' (idchar)+ ':' (idchar)+;
		integer_const   : digit_not_null (digit)*;
		                | '0' 'x' (hexdig)+;
		                | '0' (octdig)+;
		                | '0';
		integer_part    : (digit)+;
		exponent        : ('e' | 'E') (sign)? integer_part;
		float_const     : integer_part exponent;
		                | integer_part '.' (integer_part)? (exponent)?;
						|              '.' integer_part (exponent)?;
		number          : float_const | integer_const;
		quotestring     : '''' (character)* '''';
		calcvar         : '$('expr')';
		var             : '$' name;
		                | calcvar;
		varstr          : var | quotestring;
		varstrlist      : varstr (varstr)*;
		litvar          : number | 'true' | 'false' | varstrlist;

		expr            : log_or_expr;
		log_or_expr     : log_and_expr ('||' log_and_expr)*;
		log_and_expr    : incl_or_expr ('&&' incl_or_expr)*;
		incl_or_expr    : excl_or_expr ('|' excl_or_expr)*;
		excl_or_expr    : and_expr ('^' and_expr)*;
		and_expr        : eq_expr ('&' eq_expr)*;
		eq_expr         : rel_expr (('==' | '!=') rel_expr)*;
		rel_expr        : match_expr (('>=' | '<=' | '>' | '<' ) match_expr)*;
		match_expr      : add_expr ('=~' add_expr)?;
		add_expr        : mul_expr (['+' '-'] mul_expr)+;
		mul_expr        : pow_expr (['*' '/'] pow_expr)+;
		pow_expr        : unary_expr (['**'] unary_expr)+;
		unary_expr      : (['!' '-' '+'])? primary_expr;
		primary_expr    : litvar
		                | 'concat' '(' expr ',' expr ')'
		                | 'defined' '(' name ')'
		                | '(' expr ')';
	 @endverbatim
	 *
	 */
	class CIfExprParser : protected CRecursiveDescentParser {
	private:
		const CRenderState *m_outer;

	protected:
		inline bool match_word(
			const char *matchStr,
			const unsigned char **str,
			std::string &result) const
		{
			const unsigned char *sav = *str;
			std::string res;
			if ( match(matchStr, str, res) &&
				 (la(str) == 0 || !(isalnum(la(str)) || la(str) == '_')) )
			{
				result += res;
				return true;
			}
			*str = sav;
			return false;
		}

		inline bool match_op(
			const char *matchStr,
			const unsigned char **str,
			std::string &result) const
		{
			const unsigned char *sav = *str;
			std::string res;
			if ( match(matchStr, str, res) &&
				 (la(str) != matchStr[0]) )
			{
				result += res;
				return true;
			}
			*str = sav;
			return false;
		}

		/** @brief White space.
		 */
		inline unsigned char ws(
			const unsigned char **str,
			std::string &result) const
		{
			return matchOneOf(" \t\n\r\f", str, result);
		}

		/** @brief Sequence of white spaces.
		 */
		inline bool wss(
			const unsigned char **str,
			std::string &result) const
		{
			if ( !ws(str, result) )
				return false;
			while ( ws(str, result) );
			return true;
		}

		/** @brief Characters possible for identifiers.
		*
		* Characters that are allowed but have no special purpose.
		* If found, the character is appended to \a result and is returned.
		*
		@verbatim
		idchar = '_' | alphanum
		@endverbatim
		*
		* @param str Address of a character pointer to the
		* input string (address of input pointer).
		* @retval result String to store the characters matched.
		* @return 0 or the character that matches.
		* @see alphanum()
		*/
		inline unsigned char idchar(
			const unsigned char **str,
			std::string &result) const
		{
			if ( match("_", str, result) ) {
				return '_';
			}

			return alphanum(str, result);
		}

		/** @brief Character is a plus or minus sign.
		*
		@verbatim
		sign_char = '+' | '-'
		@endverbatim
		*
		* @param str Address of a character pointer to the
		* input string (address of input pointer).
		* @retval result String to store the characters matched.
		* @return 0 or the character that matches.
		*/
		inline unsigned char sign_char(
			const unsigned char **str,
			std::string &result,
			signed char &d) const
		{
			if ( match("+", str, result) ) {
				d = 1;
				return '+';
			}

			if ( match("-", str, result) ) {
				d = -1;
				return '-';
			}

			return 0;
		}


		/** @brief Character of a string.
		 */
		unsigned char character(
			const unsigned char **str,
			std::string &result) const;

		/** @brief Name of an identifier.
		 */
		bool name(
			const unsigned char **str,
			std::string &result) const;

		/** @brief An integer.
		 */
		bool integer_const(
			const unsigned char **str,
			std::string &result, unsigned long &longresult) const;

		/** @brief Integer part of a float.
		 */
		bool integer_part(
			const unsigned char **str,
			std::string &result,
			unsigned long &longresult) const;

		/** @brief Exponential part of a float.
		 */
		bool exponent(
			const unsigned char **str,
			std::string &result,
			signed long &longresult) const;

		/** @brief A float.
		 */
		bool float_const(
			const unsigned char **str,
			std::string &result,
			double &floatresult) const;

		/** @brief Integer or float.
		 */
		bool number(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief A string in quotes 'string'.
		 */
		bool quotestring(
			const unsigned char **str,
			std::string &result,
			std::string &strval) const;

		/** @brief Variable with a calculated name.
		 */
		bool calcvar(
			const unsigned char **str,
			std::string &resul,
			CValue &valt) const;

		/** @brief Variable.
		 */
		bool var(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Variable or string.
		 */
		bool varstr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Sequence of variables or strings.
		 */
		bool varstrlist(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Literal or variable (sequence of variables).
		 */
		bool litvar(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression.
		 */
		bool expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression with logical or.
		 */
		bool log_or_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression with logical and.
		 */
		bool log_and_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression with bitwise inclusive or.
		 */
		bool incl_or_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression with bitwise exclusive or.
		 */
		bool excl_or_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Expression with bitwise and.
		 */
		bool and_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Equal/not equal comparsion.
		 */
		bool eq_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Relational expression.
		 */
		bool rel_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief String matches pattern.
		 */
		bool match_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Addition/subtraction
		 */
		bool add_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Multiplication/division
		 */
		bool mul_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Power
		 */
		bool pow_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Unary expression
		 */
		bool unary_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Primary expression
		 */
		bool primary_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;

		/** @brief Complete expresion of a RIB if request, terminated by 0
		 */
		bool if_expr(
			const unsigned char **str,
			std::string &result,
			CValue &val) const;
	public:
		inline CIfExprParser(const CRenderState &outer)
		{
			m_outer = &outer;
		}
		bool parse(RtString expr) const;
	};

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
		CFilterFuncFactory &filterFuncFactory);

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
		m_transformStack.back().spaceType(RI_CAMERA);

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
		m_transformStack.back().identity();
		m_transformStack.back().spaceType(RI_WORLD);
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

	RtArchiveHandle archiveFileBegin(const char *aName);
	void archiveFileEnd();

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

	inline void motionBegin(RtInt N, RtFloat times[])
	{
		m_modeStack->motionBegin();
		m_motionState.open(N, times);
	}

    inline void motionEnd()
	{
		m_modeStack->motionEnd();
		m_motionState.close();
	}

	inline bool executeConditionial() const
	{
		return m_accumulateConditional && m_executeConditional;
	}

	void ifBegin(RtString expr);
	void elseIfBegin(RtString expr);
	void elseBegin();
	void ifEnd();

	bool exists(RtString identifier) const;
	bool getValue(CValue &p, RtString identifier) const;
	bool eval(RtString expr) const;

	/** @brief Tests if a request @a req is valid in the current mode.
	 *  @param req Index of the request to test.
	 *  @return true, if the request req is valid in the current mode.
	 *  @see CModeStack::validRequest(), EnumRequests  
	 */
	inline virtual bool validRequest(EnumRequests req) const
	{
		return m_modeStack->validRequest(req);
	}

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
	inline virtual const CTokenMap &tokenMap() const {return m_declDict.tokenMap();}
	inline CTokenMap &tokenMap() {return m_declDict.tokenMap();}

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

	inline COptionsBase &controls()
	{
		return m_controls;
	}
	
	inline const COptionsBase &controls() const
	{
		return m_controls;
	}

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

	inline CTransformation &curTransform()
	{
		assert(!m_transformStack.empty());
		return m_transformStack.back();
	}

	inline virtual const CTransformation &curTransform() const
	{
		assert(!m_transformStack.empty());
		return m_transformStack.back();
	}

	inline virtual void coordinateSystem(RtToken space)
	{
		m_globalTransforms[space] = curTransform();
	}

	inline virtual void scopedCoordinateSystem(RtToken space)
	{
		assert(!m_scopedTransforms.empty());
		m_scopedTransforms.back()[space] = curTransform();
	}

	virtual const CTransformation *findTransform(RtToken space) const;

	inline CLights &lights() { return m_lights; }

	inline virtual const CLights &lights() const { return m_lights; }

	// inline virtual bool hasMacroFactory() const {return m_macroFactory != 0;}

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
	
	inline virtual bool recordMode() const
	{
		return m_recordMode;
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

	// virtual CRManInterfaceFactory &macroFactory();
	// virtual const CRManInterfaceFactory &macroFactory() const;

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

	CMotionState &motionState() { return m_motionState; }
	const CMotionState &motionState() const { return m_motionState; }

	const IFilterFunc *filterFunc(RtToken name) const;

	virtual bool getBasis(RtToken basisName, RtBasis basis) const;
	virtual RtToken basisName(const RtBasis basis) const;

}; // CRenderState

} // namespace RiCPP

#endif // _RICPP_RICONTEXT_RENDERSTATE_H
