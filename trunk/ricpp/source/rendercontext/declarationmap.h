#ifndef _RICPP_RENDERCONTEXT_DECLARATIONMAP_H
#define _RICPP_RENDERCONTEXT_DECLARATIONMAP_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file declarationmap.h
 *  \brief Declaration of TDeclarationMap to store contents of RiDeclare calls
 *         (Maps the declaration name to the declaration) and the auxillary
 *         class TRenderException, a exception thrown by TDeclarationMap::declare() if
 *         a declaration could not get parsed
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parameterdeclaration.h"

#include <map>
#include <deque>
#include <stack>

/*! The Exception is thrown by TDeclarationMap::declare() if a declaration could
 *  not get parsed, should get catched by the renderer (TIntermediateRenderer) itself.
 */
class TRenderException
{
        RtInt m_code,                   //!< The error code RIE_...
			  m_severity;               //!< The seveity of the error RIE_...
        char  m_msgBuf[ERROR_STR_SIZE]; //!< Additional error message
public:
		//! Set error codes and additional error message (printf() style)
		/*! \param code The error code, RIE_...
		 *  \param severity The error severity code, RIE_...
		 *  \param message Additional error message (printf() style)
		 */
		inline void setErrorCode(RtInt code, RtInt severity, const char *message, va_list argList) {
			m_msgBuf[0] = (char)0;
			if ( message ) _vsnprintf(m_msgBuf, ERROR_STR_SIZE-1, message, argList);
			m_msgBuf[ERROR_STR_SIZE-1] = (char)0;
			m_code = code;
			m_severity = severity;
		}

		//! Set error codes and additional error message.
		/*! \param code The error code, RIE_...
		 *  \param severity The error severity code, RIE_...
		 *  \param message Additional error message.
		 */
		inline void setErrorCode(RtInt code, RtInt severity, const char *message) {
			m_msgBuf[0] = (char)0;
			if ( message ) strncpy(m_msgBuf, message, ERROR_STR_SIZE);
			m_msgBuf[ERROR_STR_SIZE-1] = (char)0;
			m_code = code;
			m_severity = severity;
		}

		//! Standard constructor, clears all members.
		inline TRenderException() : m_code(0), m_severity(0) {
			m_msgBuf[0] = (char)0;
		}

		//! Constructor, initialises codes and message.
		/*! \param code Code of the error (RIE_...).
		 *  \param severity Severity of the error (RIE_...).
		 *  \param message Additional message.
		 */
        inline TRenderException(RtInt code, RtInt severity, const char *message) { 
		   setErrorCode(code, severity, message);
		}

		//! Constructor, initialises codes and message (printf() style).
		/*! \param code Code of the error (RIE_...).
		 *  \param severity Severity of the error (RIE_...).
		 *  \param message Additional message format, printf() style.
		 */
		inline TRenderException(RtInt code, RtInt severity, const char *message, ...) { 
		   va_list argList;
		   va_start(argList, message);
		   setErrorCode(code, severity, message, argList);
		   va_end(argList);
		}

		//! Get the error code.
		/*! \return The error code (RIE_...).
		 */
		inline RtInt code() const {
			return m_code;
		}

		//! Get the error severity code.
		/*! \return The error severity code (RIE_...).
		 */
		inline RtInt severity() const {
			return m_severity; 
		}

		//! Get an additional error string.
		/*! \return The sadditional error string.
		 */
		inline const char *what() const {
			return m_msgBuf; 
		}
}; // TRenderException


//! Map to store the tokens (unique pointers) to the names in TDeclarationMap
typedef std::map<std::string, const char *, std::less<std::string> > STRING2TOKEN;

//! Map to store declaration to the names in TDeclarationMap
typedef std::map<const char *, TParameterDeclaration, std::less<const char *> > STRING2DECL;

////////////////////////////////////////////////////////////////////////////////
/*!Holds a map of TParameterDeclaration, maps the name to its declaration,
 * s.a TRi::Declare()
 */
class TDeclarationMap
{
	//! list of all actual declarations
	STRING2DECL m_declarations;

	/*! Declaration stack - while loading a RIB archive (pos. nested)
	 *  temporary declarations are used.
	 */
	std::stack<STRING2DECL> m_declarationStack;

	/*! Container (deque) of all strings - this is used to get
	 *  fixed (const char *) pointers for the tokens
	 */
	std::deque<std::string> m_stringContainer;

	/*! To search tokens, i.e. a string is mapped to the fixed (const char *)
	 *  of m_stringContainer. The map is only cleared (releaseAll()) if
	 *  the object is destroyed
	 */
	STRING2TOKEN m_stringMap;

public:
	//! Destructor, releases all memory used by the map
	inline ~TDeclarationMap() {
		releaseAll();
	}

	//! Copy the complete map, use the (const char *) Tokens of declMap
	/*! \param declmap Declaration map to copy from.
	 */
	void copyDecls(const TDeclarationMap &declMap);

	//! Adds one declaration by parsing.
	/*! \param name The name of the declaration or inline declaration.
	 *  \param declaration The declaration, if name is not an inline declaration.
	 *  \param isDef true if default declaration, false if user defined declaration.
	 *  \return A unique pointer (token) of the declaration.
	 */
	RtToken declare(const char *name,
					const char *declaration,
					bool isDef);

	//! Delivers the declaration for a given name.
	/*! \param name The name of the declaration to find
	 *  \returns A pointer to the declaration for a given name
	 */
	const TParameterDeclaration *findDeclaration(const char *name);

	//! by now, returns the name of a declaration, later should get a token
	//! (unique pointer to a tokenmap of the a renderer context) of a name of a declaration.
	/*! \param name A name of a declaration
	 *  \return The token of an name, TRi::RI_NULL if declaration is not found
	 */
	RtToken getToken(const char *name);			

	void push();                          //!< Pushes the current declarations to a stack
	void pop();                           //!< Restores the last declarations

	void releaseAll();                    //!< Frees all memory
	void releaseDeclarations();           //!< Frees all declarations
	void releaseNonDefaultDeclarations(); //!< Frees all non default declarations (user defined declarations)
}; // TDeclarationMap

#endif // _RICPP_RENDERCONTEXT_DECLARATIONMAP_H
