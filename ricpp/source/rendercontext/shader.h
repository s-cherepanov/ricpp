#ifndef _RICPP_RENDERCONTEXT_SHADER_H
#define _RICPP_RENDERCONTEXT_SHADER_H

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

/*! \file shader.h
 *  \brief Declaration of the container of a shader, used for the attribute set
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parametermap.h"

///////////////////////////////////////////////////////////////////////////////
//! Class stores the parameters of a shader
class TShader {
	TParameterMap m_shaderParameters; //!< Container for the shaders name and parameters
public:
	//! Standard constructor, 
	inline TShader() {
	}

	//! Constructor, initializes with the parametrs of a shader call
	/*! \param decls      Declarations valid at a render context
	 *  \param colorComps Number of components per color
	 *  \param name       Name of the shader
	 *  \param n          Number of token value pairs
	 *  \param tokens     Tokens
	 *  \param params     Values
	 */
	inline TShader(TDeclarationMap &decls, RtInt colorComps, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		shader(decls, colorComps, name, n, tokens, params);
	}

	//! Copy constructor
	/*! \param s Shader to copy from
	 */
	inline TShader(const TShader &s) {
		*this = s;
	}

	//! Destructor, cleans up
	inline ~TShader() {
		releaseAll();
	}

	//! Duplication, returns a new instance with the copy of *this
	/*! \returns A new instance containing a copy of *this
	 *  \throws TSystemException if there is not enough memory
	 */
	inline TShader &duplicate() const {
		TShader *shader = new TShader(*this);
		if ( !shader )
			throw TSystemException(__FILE__, __LINE__);
		return *shader;
	}

	//! Assignment, copies the name and parameter of another shader to *this
	/*! \param s Shader to copy from
	 *  \return *this
	 */
	inline TShader &operator=(const TShader &s) {
		releaseAll();
		m_shaderParameters = s.m_shaderParameters;
		return *this;
	}

	//! Cleans up
	inline void releaseAll() {
		m_shaderParameters.releaseAll();
	}

	//! Inserts name and paramters of a shader call
	/*! \param decls      Declarations valid at a render context
	 *  \param colorComps Number of components per color
	 *  \param name       Name of the shader
	 *  \param n          Number of token value pairs
	 *  \param tokens     Tokens
	 *  \param params     Values
	 */
	inline void shader(TDeclarationMap &decls, RtInt colorComps, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		m_shaderParameters.setName(name);
		m_shaderParameters.set(decls, 0, 0, 0, 0, 0, colorComps, n, tokens, params);
	}

	//! Inserts the name of a shader call
	/*! \param name Name of the shader
	 */
	inline void shader(const char *name) {
		m_shaderParameters.setName(name);
	}

	//! Returns the parameter structure
	/*! \return The shader parameters packed to ParameterMap, the reference points to the member variable
	 */
	inline TParameterMap &shaderParameters() {
		return m_shaderParameters;
	}
}; // TShader

#endif // _RICPP_RENDERCONTEXT_SHADER_H
