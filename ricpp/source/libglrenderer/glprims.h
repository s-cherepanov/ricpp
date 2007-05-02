#ifndef _RICPP_LIBGLRENDERER_GLPRIMS_H
#define _RICPP_LIBGLRENDERER_GLPRIMS_H

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

/*! \file glprims.h
 *  \brief RenderMan C++ Binding for OpenGL output
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "glcpp/opengl.h"
#include "rendercontext/parametermap.h"
#include "rendercontext/matrix3d.h"

#include <tiffio.h>
#include <vector>
#include <string>

class TGLTransformRGB {
public:
	virtual void transformRGB(GLfloat *surfacecolor, IndexType ncolors, RtFloat *colorarray) = 0;
};

class TGLTextureMap {
	std::string m_filename;
	GLuint      m_texName;
	GLint       m_maxTexSize;
	IndexType   m_texSize;
	IndexType   m_npixels;
	GLubyte    *m_texture;
	GLint       m_tw, m_th;
public:
	inline TGLTextureMap() {
		m_npixels = 0;
		m_texSize = 0;
		m_maxTexSize = 0;
		m_npixels = 0;
		m_texture = NULL;
		m_tw = m_th = 0;
		m_texName = 0;
	}
	inline TGLTextureMap(const char* filename) {
		m_npixels = 0;
		m_texSize = 0;
		m_maxTexSize = 0;
		m_texture = NULL;
		m_tw = m_th = 0;
		m_texName = 0;
		load(filename);
	}
	inline ~TGLTextureMap() {
		unload();
	}
	bool load(const char* filename);
	inline void unload() {
		if ( m_texture )
			delete[] m_texture;
		m_texSize = 0;
		m_npixels = 0;
		m_texture = NULL;
		m_tw = m_th = 0;
		m_texName = 0;
	}
	bool reload();
	// TGLTextureMap &operator=(const TGLTextureMap &);
	bool configure();
	bool activate();
	void deactivate();
	void texImage2D(int level);
	inline const char *texturename() const { return m_filename.c_str(); }
	inline bool isLoaded() const { return m_texture != NULL; }
};

#endif // _RICPP_LIBGLRENDERER_GLPRIMS_H
