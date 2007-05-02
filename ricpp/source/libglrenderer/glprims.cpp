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

/*! \file glprims.cpp
 *  \brief RenderMan C++ Binding for OpenGL output, primary tesselation
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "glprims.h"
#include "tools/helpers.h"

#include <GL/glu.h>

bool TGLTextureMap::load(const char* filename) {
	if ( !filename || !filename[0] )
		return false;
	m_filename = filename;
	IndexType i;
	for ( i = 0; i < m_filename.length(); ++i ) {
		if ( m_filename[i] == '/' )
			m_filename[i] = '\\';
	}
	return reload();
}

bool TGLTextureMap::reload() {
	TIFFRGBAImage image;
	uint32 *raster;
	char emsg[1024];

	deactivate();
	unload();
	TIFF *tif = TIFFOpen(m_filename.c_str(), "r");
	if ( tif == NULL )
		return false;
	if ( !TIFFRGBAImageBegin(&image, tif, 0, emsg))
		return false;
	m_npixels = (long)image.width * (long)image.height;
	raster = (uint32 *) _TIFFmalloc(m_npixels * sizeof(uint32));
	if ( raster != NULL ) {
		if ( TIFFRGBAImageGet(&image, raster, image.width, image.height) == 0) {
			return false;
		}
	}
	TIFFRGBAImageEnd(&image);
	TIFFClose(tif);

	m_tw = 1 << (int)ceilf(logf((float)image.width)/logf((float)2.0));
	m_th = 1 << (int)ceilf(logf((float)image.height)/logf((float)2.0));
	m_maxTexSize = 64;
	TGL::get(GL_MAX_TEXTURE_SIZE, &m_maxTexSize);
	if ( m_tw > m_maxTexSize )
		m_tw = m_maxTexSize;
	if ( m_th > m_maxTexSize )
		m_th = m_maxTexSize;
	m_texSize = 4L*m_tw*m_th;
	m_texture = new GLubyte[m_texSize];

	if ( m_texture ) {
		// Swap top down
		int rval = gluScaleImage(GL_RGBA, image.width, image.height, GL_UNSIGNED_BYTE, raster, m_tw, m_th, GL_UNSIGNED_BYTE, m_texture);
		int halfway = m_th/2;
		GLubyte buffer;
		GLubyte *ptr1=m_texture;
		GLubyte *ptr2=m_texture+m_texSize;
		for ( long row = 0; row < halfway; ++row ) {
			ptr2 -= 4L*m_tw;
			for ( long col = 0; col < 4L*m_tw; ++col ) {
				buffer = *ptr2;
				*ptr2  = *ptr1;
				*ptr1  = buffer;
				++ptr1;
				++ptr2;
			}
			ptr2 -= 4L*m_tw;
		}
		if ( rval != 0 ) {
			_TIFFfree(raster);
			unload();
			return false;
		}
		if ( !configure() ) {
			_TIFFfree(raster);
			deactivate();
			unload();
			return false;
		}
	} else {
		long errCode = GetLastError();
		_TIFFfree(raster);
		m_texSize = 0;
		throw TSystemException(errCode, __FILE__, __LINE__);
	}

	_TIFFfree(raster);
	return true;
}

bool TGLTextureMap::configure() {
	bool texImage = m_texName == 0;
	if ( texImage ) 
		TGL::genTextures(1, &m_texName);

	TGL::bindTexture(GL_TEXTURE_2D, m_texName);
	if ( TGL::isTexture(m_texName) ) {
		TGL::pixelStore(GL_UNPACK_ALIGNMENT, 1);
		TGL::texParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		TGL::texParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		TGL::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		TGL::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		TGL::texEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if ( texImage )
			TGL::texImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_tw, m_th, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture);
		return true;
	}

	return false;
}

bool TGLTextureMap::activate() {
	if ( m_texName == 0 )
		return configure();

	if ( TGL::isTexture(m_texName) ) {
		TGL::bindTexture(GL_TEXTURE_2D, m_texName);
		return true;
	}

	return false;
}

void TGLTextureMap::deactivate() {
	if ( m_texName != 0 ) {
		TGL::bindTexture(GL_TEXTURE_2D, 0);
		TGL::deleteTextures(1, &m_texName);
		m_texName = 0;
	}
}

/*
TGLTextureMap &TGLTextureMap::operator=(const TGLTextureMap &map) {
	if ( &map == this )
		return *this;

	unload();
	m_maxTexSize = map.m_maxTexSize;
	m_texSize = map.m_texSize;
	m_texture = new GLubyte[m_texSize];
	if ( m_texture ) {
		m_textureSize = map.m_textureSize;
		memcpy(m_texture, map.m_texture, m_texSize*sizeof(GLubyte));
	} else {
		throw new TSystemException(__FILE__, __LINE__);
	}
	m_npixels = map.m_npixels;
	m_tw = map.m_tw;
	m_th = map.m_th;
	m_texName = 0;
	return *this;
}
*/
