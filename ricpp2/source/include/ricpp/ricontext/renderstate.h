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

#ifndef _RICPP_RICONTEXT_RIMACROCLASSES_H
#include "ricpp/ricontext/rimacroclasses.h"
#endif // _RICPP_RICONTEXT_RIMACROCLASSES_H

#ifndef _RICPP_RICPP_VARSUBST_H
#include "ricpp/ricpp/varsubst.h"
#endif // _RICPP_RICPP_VARSUBST_H

namespace RiCPP {
	
	/** @brief The facade for the render state objects.
	 *
	 * The CRenderState object is a part of CBaseRenderer.
	 *
	 * @todo Remove STL from public interfaces of shared libraries (@see Creating Compatible Libraries in Apples C++ Runtime Environment Programming Guide), clean up CRenderState interface
	 */
	class CRenderState : public IVarSubstCallback {
		typedef std::map<RtToken, CTransformation *> TypeTransformationMap;

		CModeStack *m_modeStack;                       ///< Pointer to the mode stack, has to be set

		RtInt m_frameNumber;                           ///< Current frame number.

		std::string m_archiveName;                     ///< Current archive name, optional.
		long m_lineNo;                                 ///< Current line number in the file, -1 if not available.

		bool m_cacheFileArchives;                      ///< Cache archive files

		std::vector<RtToken> m_solidTypes;             ///< Stack with the nested types of solid blocks (if currently opened solid block)

		CDeclarationDictionary m_declDict;             ///< Dictionary for declarations.

		CMotionState m_motionState;                    ///< Current request id and motion state.

		TemplObjPtrRegistry<RtToken, IResourceFactory *> m_resourceFactories; ///< Creates new RiResource handlers
		TemplHandleStack<CResource> m_resourceStack;   ///< Saved resources

		COptionsBase m_controls;                       ///< Current control values.

		COptionsFactory *m_optionsFactory;              ///< Creates new options set.
		std::deque<COptions *> m_optionsStack;         ///< Current option stack.

		CAttributesFactory *m_attributesFactory;        ///< Create new attributes set.
		std::deque<CAttributes *> m_attributesStack;   ///< Current attributes stack.
		std::deque<CAttributes *> m_motionAttributesStack;   ///< Current attributes stack saved for motion blocks (moved instances)

		CTransformationFactory *m_transformationFactory;  ///< Create new transformation set.
		std::deque<CTransformation *> m_transformationStack;  ///< Current stack of transformations and their inverses.
		std::deque<CTransformation *> m_motionTransformationStack;  ///< Current stack of transformations and their inverses for motion blocks (moved instances)

		std::list<CAttributes *> m_rememberedAttributes;   ///< remembered attributes.
		std::list<CTransformation *> m_rememberedTransformations;   ///< remembered attributes.
		
		CRManInterfaceFactory *m_macroFactory; ///< Used for deleteion of defered requests.
		std::deque<CRManInterfaceCall *> m_deferedRequests;  ///< Requests with defered deletion
		
		CTransformation *m_NDCToRaster;     ///< Maps normalized display coordinates to raster space
		CTransformation *m_screenToNDC;     ///< Maps screen space to normalized display coordinates
		CTransformation *m_cameraToScreen;  ///< Transforms camera space to screen space
		CTransformation *m_worldToCamera;   ///< Transforms world space to camera space
		CTransformation *m_idTransform;     ///< Identity (world to world)
		
		RtToken m_viewingOrientation;  ///< Orientation of the viewing (camera to raster) in world block (RI_LH or RI_RH)
		RtToken m_cameraOrientation; ///< Orientation of the camera in world block (RI_LH or RI_RH)
		
		TypeTransformationMap m_globalTransforms;      ///< Global transformation map.
		std::list<TypeTransformationMap> m_scopedTransforms; ///< Scoped transformation maps.

		CParameterList m_curParams;                    ///< Params of the last interface request with variable parameters.

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

		CUri m_baseUri;           ///< Base URI for current RIB archive file

		/** @brief Points to current writeable macro
		 */
		CRiMacro *m_curMacro;

		/** @brief Points to current replayed macro
		 */
		CRiMacro *m_curReplay;

		TemplHandleStack<CRiObjectMacro> m_objectMacros;   ///< Stack of all object macros (objectBegin/End)
		TemplHandleStack<CRiArchiveMacro> m_archiveMacros; ///< Stack of all archive macros (archiveBegin/End)
		TemplHandleStack<CRiArchiveMacro> m_cachedArchive; ///< Stack of all archive macros (cached rib)
		
		TemplHandleStack<CHandle> m_lightSourceHandles;  ///< Stack of indirect light source handles
		TemplHandleStack<CLightSource> m_lightSources;   ///< Stack of all created light sources

		std::vector<CRiMacro *> m_macros; ///< Vector with pointer to all macros (m_objectMacros and m_archiveMacros)

		std::vector<bool> m_conditions; ///< Stack of m_executeConditional and m_ifCondition for nested ifs.
		bool m_executeConditional;      ///< Render (true outside if-else-blocks, conditional inside the blocks).
		bool m_accumulateConditional;   ///< Render (true outside if-else-blocks, cummulated m_executeConditional inside the blocks).
		bool m_ifCondition;             ///< true, if an if or elseif condition was true;

		virtual bool adjustProjectionMatrix(CMatrix3D &projectionMatrix, CMatrix3D &inverseProjectionMatrix);

		void pushOptions();    ///< Pushes the current options set.
		bool popOptions();     ///< Pops an options set, restores the set to the last condition.

		void pushAttributes(bool useCounter=false); ///< Pushes the current attributes set.
		bool popAttributes(bool useCounter=true);  ///< Pops an attributes set, restores the set to the last condition.

		void pushTransform(bool useCounter=false);   ///< Pushes the current transformation matrix and is inverse.
		bool popTransform(bool useCounter=true);    ///< Pops the current transformation matrices, restores the matrices to the last condition.
		
		void pushConditional(); ///< Pushes conditionals within nested if-then-else blocks
		void popConditional();  ///< Restores the last state of the conditionals

		bool devideName(RtString identifier, RtToken *aQualifier, RtToken *tablename, RtToken *varname) const;
		bool getAttribute(CValue &p, RtToken tablename, RtToken varname) const;
		bool getOption(CValue &p, RtToken tablename, RtToken varname) const;
		bool getControl(CValue &p, RtToken tablename, RtToken varname) const;

		void deleteTransMapCont(TypeTransformationMap &m);

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
			/** @brief Identifier
			 */
			bool match_word(
				const char *matchStr,
				const unsigned char **str,
				std::string &result) const;

			/** @brief Operator.
			 */
			bool match_op(
				const char *matchStr,
				const unsigned char **str,
				std::string &result) const;

			/** @brief White space.
			 */
			unsigned char ws(
				const unsigned char **str,
				std::string &result) const;

			/** @brief Sequence of white spaces.
			 */
			bool wss(
				const unsigned char **str,
				std::string &result) const;

			/** @brief Characters possible for identifiers.
			*
			* Characters that are allowed but have no special purpose.
			* If found, the character is appended to @a result and is returned.
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
			unsigned char idchar(
				const unsigned char **str,
				std::string &result) const;

			/** @brief Character is a plus or minus sign.
			*
			@verbatim
			sign_char = '+' | '-'
			@endverbatim
			*
			* @param str Address of a character pointer to the
			* input string (address of input pointer).
			* @retval result String to store the characters matched.
			* @retval d Returns 1 if character was positive, -1 if negative, 0 if no sign character was found.
			* @return 0 or the character that matches.
			*/
			unsigned char sign_char(
				const unsigned char **str,
				std::string &result,
				signed char &d) const;

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
		}; // CIfExprParser

		// Additional Tokens
		RtToken RI_RIB;                 ///< Token "rib" for options
		
		RtToken RI_CACHE_FILE_ARCHIVES; ///< Token "cache-file-archives" for control
		RtToken RI_VARSUBST;            ///< Token "varsubst" for option
		
		RtToken RI_QUAL_CACHE_FILE_ARCHIVES; ///< Qualified Token "Control:rib:cache-file-archives" for control
		RtToken RI_QUAL_VARSUBST;            ///< Token "Option:rib:varsubst" for option
		
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
		 *  @param aModeStack A reference to a valid mode stack.
		 *  @param optionsFactory References a factory to create options containers.
		 *  @param attributesFactory References a factory to create attributes containers.
		 *  @param filterFuncFactory References a factory to create filter functions.
		 *  @exception ExceptRiCPPError If the token cannot be created (out of memory while filling map).
		 */
		CRenderState(
			CModeStack &aModeStack,
			COptionsFactory &optionsFactory,
			CAttributesFactory &attributesFactory,
			CTransformationFactory &transformationFactory,
			CFilterFuncFactory &filterFuncFactory,
			CRManInterfaceFactory &macroFactory);

		/** @brief Destroys the object
		 *
		 *  State objects are deleted
		 */
		virtual ~CRenderState();
		
		void rememberState();
		CAttributes *rememberedAttributes();
		const CAttributes *rememberedAttributes() const;
		CTransformation *rememberedTransformation();
		const CTransformation *rememberedTransformation() const;

		void deferRequest(CRManInterfaceCall *aRequest);
		void deleteDeferedRequests();

		/*
		inline CTransformation *NDCToRaster() { return m_NDCToRaster; }
		inline CTransformation *screenToNDC() { return m_screenToNDC; }
		inline CTransformation *cameraToScreen() { return m_cameraToScreen; }
		inline CTransformation *worldToCamera() { return m_worldToCamera; }
		*/
		
		inline RtToken viewingOrientation() const { return m_viewingOrientation; }
		inline RtToken cameraSysOrientation() const { return m_cameraOrientation; }
		inline RtToken currentCoordSysOrientation() const { return attributes().coordSysOrientation(); }
		inline const CTransformation *NDCToRaster() const { return m_NDCToRaster; }
		inline const CTransformation *screenToNDC() const { return m_screenToNDC; }
		inline const CTransformation *cameraToScreen() const { return m_cameraToScreen; }
		inline const CTransformation *worldToCamera() const { return m_worldToCamera; }
		
		bool getRasterToCamera(CMatrix3D &m) const;
		bool getCameraToRaster(CMatrix3D &m) const;
		bool getCameraToCurrent(CMatrix3D &m) const;
		bool getCurrentToCamera(CMatrix3D &m) const;
		
		void getProjectedScreenWindow(RtFloat &left, RtFloat &right, RtFloat &bot, RtFloat &top) const;
		void calcNDCToRaster();
		void calcScreenToNDC();
		void setCameraToScreen();
		void setWorldToCamera();

		/** @brief Sets the projection option and resets the CTM.
		 *  @param name Name of the projection
		 *  @param params Parmeters for the projection
		 */
		virtual void projection(RtToken name, const CParameterList &params);

		/** @brief Call the default declarations.
		 *
		 *  Overload this to add more default declarations, by calling processDeclare() with
		 *  the @a isDefault attribute set to true.
		 */
		virtual void defaultDeclarations();

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
		virtual void contextBegin();
		virtual void contextEnd();

		virtual void frameBegin(RtInt number);
		virtual void frameEnd();

		virtual void worldBegin();
		virtual void worldEnd();

		virtual void attributeBegin();
		virtual void attributeEnd();
		virtual void storeAttributes();
		virtual void restoreAttributes();

		virtual void transformBegin();
		virtual void transformEnd();

		virtual void solidBegin(RtToken type);
		virtual void solidEnd();
		virtual RtToken solid() const;

		virtual RtObjectHandle objectBegin(RtString name, CRManInterfaceFactory &aFactory);
		virtual void objectEnd();

		virtual CRiObjectMacro *objectInstance(RtObjectHandle handle);
		virtual const CRiObjectMacro *objectInstance(RtObjectHandle handle) const;
		
		virtual RtString findHandleId(CParameterList &params) const;

		/** @brief Use a default light because there are no lights?
		 *
		 *  Use default lightsource (distant light to (0,0,1) in camera space),
		 *  true if no area/lightsource called, i.e. global lightlists are empty.
		 *  Doesn't matter if these lights are illuminated or not.
		 *
		 *  @return True, no lights use a default light. False, there are lights. 
		 */
		bool noLights() const;

		virtual RtLightHandle newLightHandle(RtToken lightSourceName, CParameterList &params);
		virtual CHandle *lightSourceHandle(RtLightHandle handle);
		virtual const CHandle *lightSourceHandle(RtLightHandle handle) const;

		virtual CLightSource *lightSourceInstance(RtLightHandle handle);
		virtual const CLightSource *lightSourceInstance(RtLightHandle handle) const;

		virtual CLightSource *newLightSource(
			RtLightHandle handle,
			bool isArea,
			const char *name, const CParameterList &params);

		virtual RtArchiveHandle archiveBegin(const char *aName, CRManInterfaceFactory &aFactory);
		virtual void archiveEnd();

		virtual RtArchiveHandle archiveFileBegin(const char *aName, CRManInterfaceFactory &aFactory);
		virtual void archiveFileEnd();

		virtual CRiArchiveMacro *findArchiveInstance(RtArchiveHandle handle);
		virtual const CRiArchiveMacro *findArchiveInstance(RtArchiveHandle handle) const;

		virtual void resourceBegin();
		virtual void resourceEnd();

		virtual RtToken resource(IRiContext &ri, RtToken handle, RtToken type, const CParameterList &params);

		virtual void motionBegin(RtInt N, RtFloat times[]);
		virtual void motionEnd();
		virtual void moveArchiveBegin();
		virtual void moveArchiveEnd();

		inline virtual bool executeConditionial() const
		{
			return m_accumulateConditional && m_executeConditional;
		}

		virtual void ifBegin(RtString expr);
		virtual void elseIfBegin(RtString expr);
		virtual void elseBegin();
		virtual void ifEnd();

		virtual bool exists(RtString identifier) const;
		virtual bool getValue(CValue &p, RtString identifier) const;
		virtual std::string &varSubst(std::string &aStr, char varId, RtString stdPath=RI_NULL, RtString curPath=RI_NULL) const;
		virtual std::string &varSubst(std::string &aStr, RtString stdPath=RI_NULL, RtString curPath=RI_NULL) const;
		virtual bool eval(RtString expr) const;

		/** @brief Tests if a request @a req is valid in the current mode.
		 *  @param req Index of the request to test.
		 *  @return true, if the request req is valid in the current mode.
		 *  @see CModeStack::validRequest(), EnumRequests  
		 */
		virtual bool validRequest(EnumRequests req);

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
		inline const CTokenMap &tokenMap() const {return m_declDict.tokenMap();}
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
		inline RtToken tokFind(const char *name) const { return tokenMap().find(name); }

		virtual void parseParameters(CParameterList &p, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);
		virtual void parseParameters(const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);
		virtual void parseParameters(CParameterList &p, const char *aQualifier, const char *aTable, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);
		virtual void parseParameters(const char *aQualifier, const char *aTable, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[]);

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
		inline virtual const CDeclaration *declFind(RtToken aQualifier, RtToken table, RtToken var) { return m_declDict.find(aQualifier, table, var); }
		
		inline virtual const CDeclaration *declFindAndUpdate(RtToken name, const CColorDescr &curColorDescr) { return m_declDict.findAndUpdate(name, curColorDescr); }
		inline virtual const CDeclaration *declFindAndUpdate(
			const char*aQualifier,
			const char *table,
			const char *var,
			const CColorDescr &curColorDescr)
		{
			return m_declDict.findAndUpdate(aQualifier, table, var, curColorDescr);
		}
		
		inline virtual void declAdd(CDeclaration *decl)
		{
			return m_declDict.add(decl);
		}
		
		inline virtual CDeclarationDictionary::const_iterator declBegin() const { return m_declDict.begin(); }
		inline virtual CDeclarationDictionary::const_iterator declEnd() const { return m_declDict.end(); }
		inline virtual CDeclarationDictionary::size_type declSize() const { return m_declDict.size(); }

		inline virtual CDeclarationDictionary &dict()
		{
			return m_declDict;
		}
		//@}

		inline virtual COptionsBase &controls()
		{
			return m_controls;
		}
		
		inline virtual const COptionsBase &controls() const
		{
			return m_controls;
		}

		virtual RtVoid control(RtToken name, const CParameterList &params);

		inline virtual COptions &options()
		{
			assert(!m_optionsStack.empty() && m_optionsStack.back() != 0);
			return *(m_optionsStack.back());
		}

		inline virtual const COptions &options() const
		{
			assert(!m_optionsStack.empty() && m_optionsStack.back() != 0);
			return *(m_optionsStack.back());
		}

		virtual RtVoid option(RtToken name, const CParameterList &params);

		inline virtual CAttributes &attributes()
		{
			assert(!m_attributesStack.empty() && m_attributesStack.back() != 0);
			return *(m_attributesStack.back());
		}

		inline virtual const CAttributes &attributes() const
		{
			assert(!m_attributesStack.empty() && m_attributesStack.back() != 0);
			return *(m_attributesStack.back());
		}

		virtual RtVoid attribute(RtToken name, const CParameterList &params);

		inline virtual CTransformation &curTransform()
		{
			assert(!m_transformationStack.empty() && m_transformationStack.back() != 0);
			return *(m_transformationStack.back());
		}

		inline virtual const CTransformation &curTransform() const
		{
			assert(!m_transformationStack.empty() && m_transformationStack.back() != 0);
			return *(m_transformationStack.back());
		}

		inline virtual void coordinateSystem(RtToken space)
		{
			m_globalTransforms[space] = m_transformationFactory->newTransformation(curTransform());
		}

		inline virtual void scopedCoordinateSystem(RtToken space)
		{
			assert(!m_scopedTransforms.empty());
			m_scopedTransforms.back()[space] = m_transformationFactory->newTransformation(curTransform());
		}

		virtual const CTransformation *findTransform(RtToken space) const;
		virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) const;
		virtual void coordSysTransform(RtToken space);
		
		// inline virtual bool hasMacroFactory() const {return m_macroFactory != 0;}

		inline virtual bool hasOptions() const {return !m_optionsStack.empty() && m_optionsStack.back() != 0;}
		inline virtual bool hasAttributes() const {return !m_attributesStack.empty() && m_attributesStack.back() != 0;}
		inline virtual bool hasTransform() const {return !m_transformationStack.empty() && m_transformationStack.back() != 0;}

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

		inline virtual CRiMacro *curMacro()
		{
			return m_curMacro;
		}

		inline virtual const CRiMacro *curMacro() const
		{
			return m_curMacro;
		}

		inline virtual void curReplay(CRiMacro *m)
		{
			m_curReplay = m;
		}

		inline virtual CRiMacro *curReplay()
		{
			return m_curReplay;
		}

		inline virtual const CRiMacro *curReplay() const
		{
			return m_curReplay;
		}
		
		virtual RtArchiveHandle storedArchiveName(RtString archiveName) const;

		virtual void registerResourceFactory(IResourceFactory *f);

		virtual CMotionState &motionState() { return m_motionState; }
		virtual const CMotionState &motionState() const { return m_motionState; }

		virtual const IFilterFunc *filterFunc(RtToken name) const;

		virtual inline bool cacheFileArchives() const { return m_cacheFileArchives; }
		virtual inline void cacheFileArchives(bool cache) { m_cacheFileArchives = cache; }

		/** @brief Processes a declarations.
		 *
		 *  Processes a single declaration. The declaration is entered
		 *  into the declaration dictionary. The current sice of the colors is
		 *  taken from the current options.
		 *
		 *  @param name Name of the declaration as in IRiCPP::declare().
		 *  @param declaration Declaration string as in IRiCPP::declare().
		 *  @param isDefault set this to true to indicate adefault declaration
		 *         (eg. declaration of RI_P as point). The defaultDeclarations()
		 *         member function is used to declare the defaults.
		 *  @return The token value for @name
		 */
		virtual RtToken declare(RtToken name, RtString declaration, bool isDefault);
	}; // CRenderState

} // namespace RiCPP

#endif // _RICPP_RICONTEXT_RENDERSTATE_H
