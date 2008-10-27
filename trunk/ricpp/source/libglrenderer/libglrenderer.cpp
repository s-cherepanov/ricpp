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

/*! \file glrenderer.cpp
 *  \brief RenderMan output for OpenGL
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "libglrenderer.h"

#include "rendercontext/subdivision.h"
#include "tools/gettemp.h"

#include <assert.h>

// #define OWNVIEWING
#undef OWNVIEWING

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// #include "../../../context3d/logfile.h"
// TLogfile logfile("c:\\temp\\glrenderer.log", "TGLRenderer");

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Tesselator callback routines, data is always an instance of the renderer
extern "C" void __stdcall tessBeginData(GLenum type, void *userData) {
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
	glBegin(type);
}

extern "C" void __stdcall tessEdgeFlagData(GLboolean flag, void *userData) {
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
}

extern "C" void __stdcall tessVertexData(void *vertexData, void *userData) {
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
	glVertex3dv((GLdouble *)vertexData);
}

extern "C" void __stdcall tessEndData(void *userData) {
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
	glEnd();
}

extern "C" void __stdcall tessErrorData(GLenum errno, void *userData) {
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
}

extern "C" void __stdcall tessCombineData(GLdouble coords[3],
					 void *vertexData[4],
					 GLfloat weight[4],
					 void **outData,
					 void *userData)
{
	// TGLRenderer *renderer = (TGLRenderer *)userData;
	userData = userData;
}

extern "C" void __stdcall nurbsError(GLenum errorCode) {
	const GLubyte *estring;
	estring = ::gluErrorString(errorCode);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TGLDisplayDesc::TGLDisplayDesc() {
	m_isMemoryContext = TRiCPPBinding::RI_FALSE;
	m_hdc = NULL;
	#ifdef WIN32
	m_hwnd = NULL;
	#endif
	m_displayDesc = NULL;

	#ifdef LINUX
	m_hwnd = 0L;
	m_visInfo = NULL;
	#endif // LINUX
}

TGLDisplayDesc::~TGLDisplayDesc() {
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RtToken TGLRenderer::ri_DeviceContext  = "devicecontext";
RtToken TGLRenderer::ri_WindowHandle   = "windowhandle";
RtToken TGLRenderer::ri_VisualInfo     = "visualinfo";

RtToken TGLRenderer::ri_Tesselation    = "tesselation";
RtToken TGLRenderer::ri_TesselationU   = "tesselationu";
RtToken TGLRenderer::ri_TesselationV   = "tesselationv";

RtToken TGLRenderer::ri_ConstantPoints = "constantpoints";
RtToken TGLRenderer::ri_ConstantLines  = "constantlines";
RtToken TGLRenderer::ri_Points         = "points";
RtToken TGLRenderer::ri_Lines          = "lines";

void TGLRenderer::glRendererDefaultDeclarations()
{
	// Insert default declarations
	try {
		declareGlobal(ri_DeviceContext, "constant pointer");// Device context
		declareGlobal(ri_WindowHandle, "constant pointer");	// Window handle
		declareGlobal(ri_VisualInfo, "constant pointer");	// X Visual Info (Linux)
		declareGlobal(ri_Tesselation, "constant float");	// Attribute tesselation (RiGeometricApproximation)
		declareGlobal(ri_TesselationU, "constant float");	// Attribute tesselation parametric u (RiGeometricApproximation)
		declareGlobal(ri_TesselationV, "constant float");	// Attribute tesselation parametric v (RiGeometricApproximation)
		declareGlobal(ri_ConstantPoints, RI_NULL);			// Attribute constantpoints (RiGeometricRepresentation)
		declareGlobal(ri_ConstantLines, RI_NULL);			// Attribute constantlines (RiGeometricRepresentation)
		declareGlobal(ri_Points, RI_NULL);					// Attribute points (RiGeometricRepresentation)
		declareGlobal(ri_Lines, RI_NULL);					// Attribute lines (RiGeometricRepresentation)
	} catch (TRenderException &e) {
		handleError(e.code(), e.severity(), e.what());
	}
}

TGLRenderer::TGLRenderer()  {
    m_rendererName = "TGLRenderer";
	m_maxLights = GLRENDERER_MAXLIGHTS;
	m_openglIsActive = false;
	m_currTMap = NULL;
	m_currBMap = NULL;

	glRendererDefaultDeclarations();

	// some default attributes
	// currentAttributes().m_tesselation[0] = (RtFloat)16.0;
	// currentAttributes().m_tesselation[1] = (RtFloat)16.0;

	m_bilinearTesselation[0] = 4.0;
	m_bilinearTesselation[1] = 4.0;

	m_bicubicTesselation[0] = 0.0;
	m_bicubicTesselation[1] = 0.0;

	m_tesselator = ::gluNewTess();
	if ( m_tesselator ) {
		::gluTessCallback(m_tesselator, GLU_TESS_BEGIN_DATA, (GLvoid (__stdcall *)())&tessBeginData);
		// ::gluTessCallback(m_tesselator, GLU_TESS_EDGE_FLAG_DATA, (GLvoid (__stdcall *)())&tessEdgeFlagData);
		::gluTessCallback(m_tesselator, GLU_TESS_VERTEX_DATA, (GLvoid (__stdcall *)())&tessVertexData);
		::gluTessCallback(m_tesselator, GLU_TESS_END_DATA, (GLvoid (__stdcall *)())&tessEndData);
		::gluTessCallback(m_tesselator, GLU_TESS_ERROR_DATA, (GLvoid (__stdcall *)())&tessErrorData);
		// ::gluTessCallback(m_tesselator, GLU_TESS_COMBINE_DATA, (GLvoid (__stdcall *)())&tessCombineData);
	}

	m_nurbs = ::gluNewNurbsRenderer();
	if ( m_nurbs ) {
		::gluNurbsProperty(m_nurbs, GLU_SAMPLING_TOLERANCE, 25.0);
		::gluNurbsProperty(m_nurbs, GLU_DISPLAY_MODE, GLU_FILL);
//		::gluNurbsProperty(m_nurbs, GLU_ERROR, nurbsError);
	}
}


TGLRenderer::~TGLRenderer() {
	if ( m_tesselator )
		::gluDeleteTess(m_tesselator);
	m_tesselator = NULL;
	if ( m_nurbs )
		::gluDeleteNurbsRenderer(m_nurbs);
	m_nurbs = NULL;

	if ( m_glrc.getRC() ) {
		m_glrc.deleteRC();
	}
}

TRenderAttributes *TGLRenderer::getNewAttributes() {
	return new TGLAttributes;
}
TRenderAttributes *TGLRenderer::getNewAttributes(const TRenderAttributes *attr) {
	if ( attr && attr->isKindOf(TGLAttributes::myClassName()) )
		return new TGLAttributes(*(TGLAttributes *)attr);
	return getNewAttributes();
}

TRenderOptions *TGLRenderer::getNewOptions() {
	return new TGLOptions;
}


TRenderOptions *TGLRenderer::getNewOptions(const TRenderOptions *o) {
	if ( o && o->isKindOf(TGLOptions::myClassName()) )
		return new TGLOptions(*(TGLOptions *)o);
	return getNewOptions();
}

TRiCPPContext *TGLRenderer::getNewContext() {
 	return new TGLContext;
}

RtFloat TGLRenderer::curUTesselation() const {
	return currentAttributes().m_tesselation[0];
}

RtFloat TGLRenderer::curVTesselation() const {
	return currentAttributes().m_tesselation[1];
}

RtFloat TGLRenderer::curTessTrim() const {
	return currentAttributes().m_tessTrim;
}

///////////////////////////////////////////////////////////////////////////////

void TGLRenderer::drawGeometricPrim(TGeometricPrimFace &primFace) {
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL )
		return;

	const RtFloat *vect = primFace.getVertices();
	if ( vect == NULL )
		return;

	TGL::disableClientState(GL_EDGE_FLAG_ARRAY);
	TGL::disableClientState(GL_INDEX_ARRAY);
	TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);

	TGL::enableClientState(GL_VERTEX_ARRAY);
	TGL::vertexPointer(3, 0, vect);

	IndexType size;
	// vect = primFace.getNormals(size, getCurrentFrontFace());
	vect = primFace.getNormals(size, false);
	if ( vect ) {
		if ( size > 3 ) {
			TGL::enableClientState(GL_NORMAL_ARRAY);
			TGL::normalPointer(0, vect);
		} else if ( size == 3 ) {
			TGL::disableClientState(GL_NORMAL_ARRAY);
			TGL::normal(vect[0], vect[1], vect[2]);
		} else {
			TGL::disableClientState(GL_NORMAL_ARRAY);
		}
	} else {
		TGL::disableClientState(GL_NORMAL_ARRAY);
	}

	const TNamedVector<RtFloat> *namedVect = primFace.getInterpolatedData(RI_CS);
	bool changedColor = false;
	if ( namedVect &&
	     (namedVect->components() == (unsigned long)options().m_iNColor) &&
		 (namedVect->cardinality() == 1) )
	{
		TGL::enableClientState(GL_COLOR_ARRAY);
		TGLContext &glc = currentContext();
		glc.m_colorVect.resize(0);
		glc.m_colorVect.resize(4*namedVect->numElems());
		transformRGB(&(glc.m_colorVect[0]), namedVect->numElems(), namedVect->begin());
		TGL::colorPointer(4, 0, &(glc.m_colorVect[0]));
	} else {
		TGL::disableClientState(GL_COLOR_ARRAY);
		RtPointer col;
		const TParameter *p = primFace.getUniformData(RI_CS, &col);
		if ( p && p->getBasicType() == TYPE_FLOAT && (p->getComponents() == options().m_iNColor) && (p->getCardinality() == 1) ) {
			changedColor = true;
			RtFloat ricolor[4];
			transformRGB(ricolor, 1, (RtFloat *)col);
			TGL::color4(ricolor);
		}
	}

	vect = NULL;
	if ( m_currTMap && m_currTMap->isLoaded() ) {
		const RtFloat *c = &(attributes().m_textureCoordinates[0]);
		vect = primFace.getTextureST(size, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
		if ( vect && size == 2 ) {
			TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);
			TGL::texCoord2(vect);
		} else if ( vect ) {
			TGL::enableClientState(GL_TEXTURE_COORD_ARRAY);
			TGL::texCoordPointer(2, 0, vect);
		}
	}
	if ( !vect ) {
		TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	std::deque<TGeometricIndices> &vidx = primFace.getVertexIndices();
	IndexType vertSize = vidx.size();
	for ( IndexType i = 0; i < vertSize; ++i ) {
		IndexType size = vidx[i].size();
		if ( size > 0 ) {
			switch ( vidx[i].type() ) {
				case TGeometricIndices::strip:
					TGL::drawElements(GL_TRIANGLE_STRIP, size, vidx[i].indexPointer());
					break;
				case TGeometricIndices::fan:
					TGL::drawElements(GL_TRIANGLE_FAN, size, vidx[i].indexPointer());
					break;
				case TGeometricIndices::triangles:
					TGL::drawElements(GL_TRIANGLES, size, vidx[i].indexPointer());
					break;
				case TGeometricIndices::polygon:
					TGL::drawElements(GL_POLYGON, size, vidx[i].indexPointer());
					break;
			}
		}
	}

	TGL::disableClientState(GL_NORMAL_ARRAY);
	TGL::disableClientState(GL_VERTEX_ARRAY);
	TGL::disableClientState(GL_COLOR_ARRAY);
	TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);

	if ( changedColor ) {
		establishShader(IDX_SHADER_SURFACE);
	}
}


void TGLRenderer::finishGL() {
	if ( m_openglIsActive ) {
		// Finishing the rendering
		TGL::loadIdentity();
		#ifdef WIN32
		m_glrc.swapBuffers(m_currGLDisplayDesc.m_hdc);
		#endif
		#ifdef LINUX
		m_glrc.swapBuffers(m_currGLDisplayDesc.m_hdc, m_currGLDisplayDesc.m_hwnd);
		#endif
		TGL::bindTexture(GL_TEXTURE_2D, 0);
		{
			for ( STRING2TMAP::iterator it = m_mapTMap.begin(); it != m_mapTMap.end(); it++ ) {
				TGLTextureMap *cur = it->second;
				if ( cur )
					cur->deactivate();
			}
		}
		{
			for ( STRING2TMAP::iterator it = m_mapBMap.begin(); it != m_mapBMap.end(); it++ ) {
				TGLTextureMap *cur = it->second;
				if ( cur )
					cur->deactivate();
			}
		}
		TGL::flush();
		TGL::finish();

		#ifndef LINUX
		  m_glrc.releaseCurrentRC();
		#endif
		m_openglIsActive = false;
	}
	// m_currGLDisplayDesc.m_hdc = NULL;
	// m_currGLDisplayDesc.m_hwnd = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TGLRenderer::transformRGB(RtFloat *endcolor, IndexType ncolors, RtFloat *colorarray, bool applyCorrection) {
	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::transformRGB(), m_currentContext == NULL || m_currentContext->m_currentOptions == NULL");
		return;
	}
	if ( !applyCorrection ) {
		IndexType i;
		for ( i = 0; i < ncolors; ++i ) {
			options().colorRGB(&endcolor[i*4], &colorarray[i*options().m_iNColor]);
			endcolor[i*4+0] = endcolor[i*4+0];
			endcolor[i*4+1] = endcolor[i*4+1];
			endcolor[i*4+2] = endcolor[i*4+2];
			endcolor[i*4+3] = (RtFloat)1.0;
		}
		return;
	}

	RtFloat gamma = (RtFloat)((1.0 / options().m_fGamma));
	IndexType i;
	for ( i = 0; i < ncolors; ++i ) {
		options().colorRGB(&endcolor[i*4], &colorarray[i*options().m_iNColor]);
		endcolor[i*4+0] = (RtFloat)pow(endcolor[i*4+0]*options().m_fGain, gamma);
		endcolor[i*4+1] = (RtFloat)pow(endcolor[i*4+1]*options().m_fGain, gamma);
		endcolor[i*4+2] = (RtFloat)pow(endcolor[i*4+2]*options().m_fGain, gamma);
		endcolor[i*4+3] = (RtFloat)1.0;
	}
}

///////////////////////////////////////////////////////////////////////////////

void TGLRenderer::establishTransformation() {
	TIntermediateRenderer::establishTransformation();

	assert (m_currentContext != NULL && m_currentContext->m_currentAttributes != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::establishTransformation(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

///////////////////////////////////////////////////////////////////////////////

// Get an light index, returns idx 0..m_maxLights-1
// use in GL as GL_LIGHT0+idx
// -1 not used, idx <= -2 no light available
int TGLRenderer::allocLightIndex() {
	std::set<int, std::less<int> >::iterator iter = m_unusedLightSources.begin();

	if ( iter != m_unusedLightSources.end() ) {
		int i = *iter;
		m_unusedLightSources.erase(iter);
		return i;
	}

	return -2;
}

// make lightIdx available, 0 <= lightIndex < m_maxLights-1
void TGLRenderer::freeLightIndex(int lightIdx) {
	m_unusedLightSources.insert(lightIdx);
}

// calles after attributeset has changed (or at the begining) to light all available
// lights
void TGLRenderer::resetLights() {
	if ( m_openglIsActive ) {
		m_ambientLights.clear();
		m_unusedLightSources.clear();
		int i;
		for ( i=0; i < m_maxLights; ++i ) {
			m_unusedLightSources.insert(i);
			TGL::disable(GL_LIGHT0 + i);
		}
		GLfloat tempAmbient[4] = {0.0, 0.0, 0.0, 1.0};
		TGL::lightModel(GL_LIGHT_MODEL_AMBIENT, tempAmbient);
	}
	TIntermediateRenderer::resetLights();
}

void TGLRenderer::ambientLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor) {
	// parameters intensity and lightcolor are not used here, because the ambient light source
	// is an accumulation of all ambient light sources currently switched on

	m_ambientLights.push_back(handle);

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::ambientLight(), m_currentContext == NULL || m_currentContext->m_currentOptions == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	// if ( intensity > 1.0 )
	//	intensity = 1.0;

	// accumulation of all ambient light sources
	std::vector<RtLightHandle>::iterator i;
	int j;
	std::vector<GLfloat> vf;

	vf.resize(options().m_iNColor);
	for ( j = 0; j < options().m_iNColor; ++j ) {
		vf[j] = 0.0;
	}

	for ( i = m_ambientLights.begin(); i != m_ambientLights.end(); i++ ) {
		RtFloat intens = 1.0;
		TParameterMap &p = m_currentContext->m_lightList[(long)(*i)].lightParameters();

		RtPointer ptr = p.getValueOf(RI_INTENSITY);
		if ( ptr )
			intens = *(RtFloat *)ptr;

		ptr = p.getValueOf(RI_LIGHTCOLOR);
		if ( ptr ) {
			for ( j = 0; j < options().m_iNColor; ++j ) {
				vf[j] += ((RtFloat *)ptr)[j] * intens;
			}
		} else {
			for ( j = 0; j < options().m_iNColor; ++j ) {
				vf[j] += intens;
			}
		}
	}
	for ( j = 0; j < options().m_iNColor; ++j ) {
		if ( vf[j] > 1.0 )
			vf[j] = 1.0;
	}
	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &vf[0]);

	TGL::lightModel(GL_LIGHT_MODEL_AMBIENT, ricolor);
}

void TGLRenderer::distantLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::distantLight(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive ) return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &lightcolor[0]);

    GLfloat vf[4];
	GLfloat f;

	int idx = allocLightIndex();
	if ( idx < 0 ) return;

	// if ( intensity > 1.0 )
	//	intensity = 1.0;

	if ( light.globalLight() ) {
		TMatrix3D m;
		m.identity();
		RtMatrix mat;
		m.get(mat);
		m = m_currentContext->m_mapCoordSys[RI_CAMERA];
		m.getInverse(mat);
		TGL::loadMatrix((const RtFloat *)&mat[0][0]);
		TGL::multMatrix((const RtFloat *)light.transformation());
	} else {
		TGL::loadMatrix((const RtFloat *)light.transformation());
	}

	GLenum lightHandle = GL_LIGHT0 + idx;
	TGL::enable(lightHandle);

	vf[0] = 0.0;
	vf[1] = 0.0;
	vf[2] = 0.0;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_AMBIENT, vf);
	vf[0] = ricolor[0] * intensity;
	vf[1] = ricolor[1] * intensity;
	vf[2] = ricolor[2] * intensity;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_DIFFUSE, vf);
	TGL::light(lightHandle, GL_SPECULAR, vf);
/// ????????????????????????
	vf[0] = from[0] - to[0];
	vf[1] = from[1] - to[1];
	vf[2] = from[2] - to[2];
/*
	vf[0] = to[0] - from[0];
	vf[1] = to[1] - from[1];
	vf[2] = to[2] - from[2];
*/

	vf[3] = 0.0;

	TGL::light(lightHandle, GL_POSITION, vf);

	/*
	vf[0] = to[0];
	vf[1] = to[1];
	vf[2] = to[2];
	vf[3] = 0.0;
	TGL::light(lightHandle, GL_SPOT_DIRECTION, vf);
	*/

	f = 0.0;
	TGL::light(lightHandle, GL_SPOT_EXPONENT, f);
	f = 180.0;
	TGL::light(lightHandle, GL_SPOT_CUTOFF, f);
	f = 1.0;
	TGL::light(lightHandle, GL_CONSTANT_ATTENUATION, f);
	f = 0.0;
	TGL::light(lightHandle, GL_LINEAR_ATTENUATION, f);
	f = 0.0;
	TGL::light(lightHandle, GL_QUADRATIC_ATTENUATION, f);

	TGL::loadMatrix((const RtFloat *)CTM());
}

void TGLRenderer::pointLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::pointLight(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive ) return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &lightcolor[0]);

    GLfloat vf[4];
	GLfloat f;

	int idx = allocLightIndex();
	if ( idx < 0 ) return;

	// if ( intensity > 1.0 )
	//	intensity = 1.0;

	if ( light.globalLight() ) {
		TMatrix3D m;
		m.identity();
		RtMatrix mat;
		m.get(mat);
		m = m_currentContext->m_mapCoordSys[RI_CAMERA];
		m.getInverse(mat);
		TGL::loadMatrix((const RtFloat *)&mat[0][0]);
		TGL::multMatrix((const RtFloat *)light.transformation());
	} else {
		TGL::loadMatrix((const RtFloat *)light.transformation());
	}

	GLenum lightHandle = GL_LIGHT0 + idx;
	vf[0] = 0.0;
	vf[1] = 0.0;
	vf[2] = 0.0;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_AMBIENT, vf);
	vf[0] = ricolor[0] * intensity;
	vf[1] = ricolor[1] * intensity;
	vf[2] = ricolor[2] * intensity;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_DIFFUSE, vf);
	TGL::light(lightHandle, GL_SPECULAR, vf);
	vf[0] = from[0];
	vf[1] = from[1];
	vf[2] = from[2];
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_POSITION, vf);

	vf[0] = (RtFloat)0.0-from[0];
	vf[1] = (RtFloat)0.0-from[1];
	vf[2] = (RtFloat)0.0-from[2];
	vf[3] = 0.0;
	TGL::light(lightHandle, GL_SPOT_DIRECTION, vf);
	f = 0.0;
	TGL::light(lightHandle, GL_SPOT_EXPONENT, f);
	f = 180.0;
	TGL::light(lightHandle, GL_SPOT_CUTOFF, f);
	f = 0.0;
	TGL::light(lightHandle, GL_CONSTANT_ATTENUATION, f);
	f = 0.0;
	TGL::light(lightHandle, GL_LINEAR_ATTENUATION, f);
	f = 1.0;
	TGL::light(lightHandle, GL_QUADRATIC_ATTENUATION, f);
	TGL::enable(lightHandle);
	TGL::loadMatrix((const RtFloat *)CTM());
}

void TGLRenderer::spotLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to, RtFloat coneangle, RtFloat conedeltaangle, RtFloat beamdistribution) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::spotLight(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive ) return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &lightcolor[0]);

    GLfloat vf[4];
	GLfloat f;

	int idx = allocLightIndex();
	if ( idx < 0 ) return;

	// if ( intensity > 1.0 )
	//	intensity = 1.0;

	if ( light.globalLight() ) {
		TMatrix3D m;
		m.identity();
		RtMatrix mat;
		m.get(mat);
		m = m_currentContext->m_mapCoordSys[RI_CAMERA];
		m.getInverse(mat);
		TGL::loadMatrix((const RtFloat *)&mat[0][0]);
		TGL::multMatrix((const RtFloat *)light.transformation());
	} else {
		TGL::loadMatrix((const RtFloat *)light.transformation());
	}

	GLenum lightHandle = GL_LIGHT0 + idx;
	vf[0] = 0.0;
	vf[1] = 0.0;
	vf[2] = 0.0;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_AMBIENT, vf);
	vf[0] = ricolor[0] * intensity;
	vf[1] = ricolor[1] * intensity;
	vf[2] = ricolor[2] * intensity;
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_DIFFUSE, vf);
	TGL::light(lightHandle, GL_SPECULAR, vf);
	vf[0] = from[0];
	vf[1] = from[1];
	vf[2] = from[2];
	vf[3] = 1.0;
	TGL::light(lightHandle, GL_POSITION, vf);
	vf[0] = to[0]-from[0];
	vf[1] = to[1]-from[1];
	vf[2] = to[2]-from[2];
	vf[3] = 0.0;
	TGL::light(lightHandle, GL_SPOT_DIRECTION, vf);
	f = beamdistribution;
	TGL::light(lightHandle, GL_SPOT_EXPONENT, f);
	f = coneangle;
	TGL::light(lightHandle, GL_SPOT_CUTOFF, f);
	f = 1.0;
	TGL::light(lightHandle, GL_CONSTANT_ATTENUATION, f);
	f = 0.0;
	TGL::light(lightHandle, GL_LINEAR_ATTENUATION, f);
	f = 0.0;
	TGL::light(lightHandle, GL_QUADRATIC_ATTENUATION, f);
	TGL::enable(lightHandle);
	TGL::loadMatrix((const RtFloat *)CTM());
}

void TGLRenderer::nonStandardLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor) {
	if ( !m_openglIsActive ) return;

	// if ( intensity > 1.0 )
	//	intensity = 1.0;

	ambientLight(light, handle, intensity, lightcolor);
}


///////////////////////////////////////////////////////////////////////////////
// shader

void TGLRenderer::setTMap(const char *texturename) {
	assert (m_currentContext != NULL);
	if ( m_currentContext == NULL) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::setTMap(), m_currentContext == NULL");
		return;
	}

	// TGLTextureMap *prev = m_currTMap;
	if ( !m_openglIsActive ) {
		m_currTMap = NULL;
		return;
	}

	if ( !texturename || !texturename[0] ) {
		m_currTMap = NULL;
		return;
	}

	std::string path = texturename;
	m_currentContext->m_pathStack.expandPath(path);

	STRING2TMAP::iterator i = m_mapTMap.find(path.c_str());
	if ( i != m_mapTMap.end() ) {
		m_currTMap = i->second;
		/*
		if ( m_currTMap && !m_currTMap->reload() )
			m_currTMap = NULL;
		*/
		if ( /* prev != m_currTMap && */  m_currTMap ) {
			// m_currTMap->configure();
			m_currTMap->activate();
		}
		return;
	}
	TGLTextureMap *map = new TGLTextureMap();
	if ( map ) {
		if ( map->load(path.c_str()) ) {
			m_mapTMap[path.c_str()] = map;
			m_currTMap = map;
			// m_currTMap->configure();
			// m_currTMap->activate();
			return;
		} else {
			delete map;
			m_mapTMap[path.c_str()] = NULL;
		}
	}
	m_currTMap = NULL;
}

void TGLRenderer::setBMap(const char *texturename) {
	assert (m_currentContext != NULL);
	if ( m_currentContext == NULL) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::setBMap(), m_currentContext == NULL");
		return;
	}
#if 0
	// TGLTextureMap *prev = m_currBMap;
	if ( !m_openglIsActive ) {
		m_currBMap = NULL;
		return;
	}

	if ( !texturename || !texturename[0] ) {
		m_currBMap = NULL;
		return;
	}

	std::string path = texturename;
	m_currentContext->m_pathStack.expandPath(path);

	STRING2TMAP::iterator i = m_mapBMap.find(path.c_str());
	if ( i != m_mapBMap.end() ) {
		m_currBMap = i->second;
		/*
		if ( m_currBMap && !m_currBMap->reload() )
			m_currBMap = NULL;
		*/
		if ( /* prev != m_currBMap && */  m_currBMap ) {
			// m_currBMap->configure();
			m_currBMap->activate();
		}
		return;
	}
	TGLTextureMap *map = new TGLTextureMap();
	if ( map ) {
		if ( map->load(path.c_str()) ) {
			m_mapBMap[path.c_str()] = map;
			m_currBMap = map;
			// m_currBMap->configure();
			// m_currBMap->activate();
			return;
		} else {
			delete map;
			m_mapBMap[path.c_str()] = NULL;
		}
	}
	m_currBMap = NULL;
#endif
}

void TGLRenderer::constantShader(int shaderType) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::constantShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	/*
	RtFloat Ka = 1.0, Kd = 1.0;

	GLfloat noShine = 0.0;
	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0};
	GLfloat colorD[4] = {ricolor[0]*Kd, ricolor[1]*Kd, ricolor[2]*Kd, 1.0};

	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, colorD);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, noMat);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, noShine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);
	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);
	*/
	TGL::color4(ricolor);
	TGL::disable(GL_LIGHTING);
	TGL::disable(GL_COLOR_MATERIAL);

	m_currTMap = NULL;
}

void TGLRenderer::matteShader(int shaderType, RtFloat Ka, RtFloat Kd) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::matteShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	RtFloat Kg = Ka + Kd;
	if ( Kg > 1 ) {
		Ka /= Kg;
		Kd /= Kg;
	}

	GLfloat noShine = 0.0;
	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0};
	GLfloat colorD[4] = {ricolor[0]*Kd, ricolor[1]*Kd, ricolor[2]*Kd, 1.0};

	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, colorD);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, noMat);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, noShine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);
	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);
	if ( m_currTMap ) m_currTMap->deactivate();
	m_currTMap = NULL;
}

void TGLRenderer::metalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat roughness) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::metalShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	GLfloat shine = 50.0;
	/*
	roughness += 1;
	if ( roughness > 1.0 )
		shine = (GLfloat)(1.0/roughness);
	*/

	RtFloat Kg = Ka + Ks;
	if ( Kg > 1 ) {
		Ka /= Kg;
		Ks /= Kg;
	}

	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0 };
	GLfloat colorS[4] = {ricolor[0]*Ks, ricolor[1]*Ks, ricolor[2]*Ks, 1.0 };

	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, noMat);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, colorS);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);
	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);
	if ( m_currTMap ) m_currTMap->deactivate();
	m_currTMap = NULL;
}

void TGLRenderer::shinyMetalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat Kr, RtFloat roughness, const char *texturename) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::shinyMetalShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	GLfloat shine = 50.0;
	/*
	roughness += 1;
	if ( roughness > 1.0 )
		shine = (GLfloat)(1.0/roughness);
	*/

	RtFloat Kg = Ka + Ks;
	if ( Kg > 1 ) {
		Ka /= Kg;
		Ks /= Kg;
	}

	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0 };
	GLfloat colorS[4] = {ricolor[0]*Ks, ricolor[1]*Ks, ricolor[2]*Ks, 1.0 };

	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, noMat);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, colorS);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);
	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);

	setTMap(texturename);
}

void TGLRenderer::plasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::plasticShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	RtFloat rispeccolor[4];
	transformRGB(rispeccolor, 1, &specularColor[0]);

	GLfloat shine = 50.0;
	/*
	roughness += 1;
	if ( roughness > 1.0 )
		shine = (GLfloat)(1.0/roughness);
	*/

	/*
	RtFloat Kg = Ka + Kd + Ks;
	if ( Kg > 1 ) {
		Ka /= Kg;
		Kd /= Kg;
		Ks /= Kg;
	}
	*/

	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0 };
	GLfloat colorD[4] = {ricolor[0]*Kd, ricolor[1]*Kd, ricolor[2]*Kd, 1.0 };
	GLfloat colorS[4] = {rispeccolor[0]*Ks, rispeccolor[1]*Ks, rispeccolor[2]*Ks, 1.0 };
	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, colorD);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, colorS);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);

	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);

	if ( m_currTMap ) m_currTMap->deactivate();
	m_currTMap = NULL;
}

void TGLRenderer::paintedPlasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor, const char *texturename) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::paintedPlasticShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( !m_openglIsActive )
		return;

	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);

	RtFloat rispeccolor[4];
	transformRGB(rispeccolor, 1, &specularColor[0]);

	GLfloat shine = 50.0;
	/*
	roughness += 1;
	if ( roughness > 1.0 )
		shine = (GLfloat)(1.0/roughness);
	*/

	RtFloat Kg = Ka + Kd + Ks;
	if ( Kg > 1 ) {
		Ka /= Kg;
		Kd /= Kg;
		Ks /= Kg;
	}

	GLfloat noMat[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat colorA[4] = {ricolor[0]*Ka, ricolor[1]*Ka, ricolor[2]*Ka, 1.0 };
	GLfloat colorD[4] = {ricolor[0]*Kd, ricolor[1]*Kd, ricolor[2]*Kd, 1.0 };
	GLfloat colorS[4] = {rispeccolor[0]*Ks, rispeccolor[1]*Ks, rispeccolor[2]*Ks, 1.0 };

	TGL::material(GL_FRONT_AND_BACK, GL_AMBIENT, colorA);
	TGL::material(GL_FRONT_AND_BACK, GL_DIFFUSE, colorD);
	TGL::material(GL_FRONT_AND_BACK, GL_SPECULAR, colorS);
	TGL::material(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	TGL::material(GL_FRONT_AND_BACK, GL_EMISSION, noMat);
	TGL::enable(GL_LIGHTING);
	TGL::enable(GL_COLOR_MATERIAL);

	setTMap(texturename);
}

void TGLRenderer::bumpyShader(int shaderType, RtFloat amplitude, const char *texturename) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::bumpyShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	setBMap(texturename);
}

void TGLRenderer::depthCueShader(int shaderType, RtFloat mindistance, RtFloat maxdistance, RtFloat *background) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::depthCueShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

}

void TGLRenderer::fogShader(int shaderType, RtFloat distance, RtFloat *background) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::fogShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

}

void TGLRenderer::nonStandardShader(int shaderType) {

	assert (m_currentContext != NULL && m_currentContext->m_currentOptions != NULL);
	if ( m_currentContext == NULL || m_currentContext->m_currentOptions == NULL ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::fogShader(), m_currentContext == NULL || m_currentContext->m_currentAttributes == NULL");
		return;
	}

	if ( shaderType == 0 )
		TGL::enable(GL_LIGHTING);
	m_currTMap = NULL;
}

///////////////////////////////////////////////////////////////////////////////

RtVoid TGLRenderer::coneDisplacedV(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( radius <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax > (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	// Render a cone
	if ( m_openglIsActive ) {
		/*
		TGLCone *q = new TGLCone(height, radius, thetamax, displacement, currentAttributes().m_tesselation[0], currentAttributes().m_tesselation[1], options().m_iNColor, getDeclMap(), *this, n, tokens, params);
		if ( q ) {
			q->setTexture(m_currTMap, attributes().m_textureCoordinates);
			q->tesselate(getCurrentFrontFace());
			q->display(&attributes().m_color[0], *this);
		}
		delete q;
		*/
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// The interface
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////



/////////////////
// blockstructure


RtVoid TGLRenderer::doEnd(void) {
	release_second(m_mapTMap);
}


////////////////////////////////////////////////////////////////////////////////////////////
// RiWorldBegin - starts the rendering
// s.a. [Ups89]: The Digital Image
//
RtVoid TGLRenderer::doWorldBegin(void) {
	if ( m_currGLDisplayDesc.m_hdc ) {
		if ( !m_glrc.getRC() ) {
			// Creating an OpenGL Rendering Context (only once a program´s live time)
			#ifdef WIN32
			m_glrc.createRC(m_currGLDisplayDesc.m_hdc);
			#endif
			#ifdef LINUX
			m_glrc.createRC(m_currGLDisplayDesc.m_hdc, m_currGLDisplayDesc.m_visInfo);
			#endif
		}
		if ( m_glrc.getRC() ) {
			// Making our rendering context current
			#ifdef WIN32
			m_glrc.makeCurrentRC(m_currGLDisplayDesc.m_hdc);
			#endif
			#ifdef LINUX
			m_glrc.makeCurrentRC(m_currGLDisplayDesc.m_hdc, m_currGLDisplayDesc.m_hwnd);
			#endif
			m_openglIsActive = true;
		}
	}

	if ( m_openglIsActive ) {
		RECT windowRect = {0, options().m_vpCurrentViewPort.m_iWidth,
		                   0, options().m_vpCurrentViewPort.m_iHeight};

		GetClientRect(m_currGLDisplayDesc.m_hwnd, &windowRect);

		// Enable lighting
		TGL::enable(GL_LIGHTING);
		TGL::lightModel(GL_LIGHT_MODEL_TWO_SIDE, 1);
		GLfloat tempAmbient[4] = {0.0, 0.0, 0.0, 1.0};
		TGL::lightModel(GL_LIGHT_MODEL_AMBIENT, tempAmbient);
		TGL::get(GL_MAX_LIGHTS, &m_maxLights);
		resetLights();

		// Surface type
		// TGL::shadeModel(GL_FLAT);
		TGL::shadeModel(GL_SMOOTH);
		TGL::enable(GL_COLOR_MATERIAL);
		TGL::colorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		// Setting the view transformation
		TGL::matrixMode(GL_PROJECTION);

		TGL::loadIdentity();

		// Viewport

		// offset (! viewport of OpenGL has it's origin at the lower left corner,
		//           Ri at the upper left !)
		GLint x =  options().m_vpCurrentViewPort.m_iOriginX;
		GLint y = -options().m_vpCurrentViewPort.m_iOriginY;
		y += windowRect.bottom - options().m_vpCurrentViewPort.m_iHeight;

		TGL::viewport(
			x, y,
			options().m_vpCurrentViewPort.m_iWidth,
			options().m_vpCurrentViewPort.m_iHeight);

		// Crop

		if ( options().m_bCropWindowCalled ) {
			GLint cx = (GLint)options().m_iRxMin + x;
			GLint cy = (GLint)(options().m_vpCurrentViewPort.m_iHeight-(options().m_iRyMax+1)) + y;
			GLsizei cw = (GLsizei)(options().m_iRxMax-options().m_iRxMin+1);
			GLsizei ch = (GLsizei)(options().m_iRyMax-options().m_iRyMin+1);
			TGL::scissor(cx, cy, cw, ch);
			TGL::enable(GL_SCISSOR_TEST);
		} else {
			TGL::disable(GL_SCISSOR_TEST);
		}

		// Projection

		// ZBuffer range
		TGL::depthRange(0.0, 1.0);

		TMatrix3D m;
		TGL::loadIdentity();

#ifdef OWNVIEWING

		// Problem here: Clipping planes...
		// If using this change near, far (Perspective) in TGLRenderer::establishProjection()

		// Left handed: look along positive z-axis, mirror x coord
		m.scale(1.0F, 1.0F, -1.0F);
		TGL::multMatrix((const RtFloat *)m);

		TGL::multMatrix((const RtFloat *)options().m_mPreProjectionMatrix);

		m = m_currentContext->m_mapCoordSys[RI_SCREEN];
		TGL::multMatrix((const RtFloat *)m);

#else

		m.scale(1.0F, 1.0F, -1.0F);

		TGL::multMatrix((const RtFloat *)m);
		TGL::multMatrix((const RtFloat *)options().m_mPreProjectionMatrix);
		TGL::multMatrix((const RtFloat *)m);

		if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {
			// RI_PERSPECTIVE
			TGL::frustum(options().m_fScreenWindowLeft * options().m_fCameraNearClip,
					     options().m_fScreenWindowRight * options().m_fCameraNearClip,
						 options().m_fScreenWindowBottom * options().m_fCameraNearClip,
						 options().m_fScreenWindowTop * options().m_fCameraNearClip,
						 options().m_fCameraNearClip, options().m_fCameraFarClip);
		} else if ( !strcmp(options().m_tCameraProjection, RI_ORTHOGRAPHIC) ) {
			// RI_ORTHOGRAPHIC or Identity (RI_NULL)
			TGL::ortho(options().m_fScreenWindowLeft,
				options().m_fScreenWindowRight,
				options().m_fScreenWindowBottom,
				options().m_fScreenWindowTop,
				options().m_fCameraNearClip, options().m_fCameraFarClip);
		} else if ( !strcmp(options().m_tCameraProjection, "null") ) {
			// How to set the 6 Clipping Planes ??? Taking ortho
			TGL::ortho(options().m_fScreenWindowLeft,
				options().m_fScreenWindowRight,
				options().m_fScreenWindowBottom,
				options().m_fScreenWindowTop,
				options().m_fCameraNearClip, options().m_fCameraFarClip);
		}

		// Left handed, look along positive z-axis, mirror on y-z plane

		// gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
		// TMatrix3D m;
		// m.scale(-1.0F, 1.0F, 1.0F);

		// Using default gluLookAt
		// gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
		m.identity();
		m.scale(1.0F, 1.0F, -1.0F);
		TGL::multMatrix((const RtFloat *)m);

#endif
		// Camera Transformation
		m = m_currentContext->m_mapCoordSys[RI_CAMERA];
		TGL::multMatrix((const RtFloat *)m);

		// Post Transformation
		// m = options().m_mCamTrans;
		// TGL::multMatrix((const RtFloat *)m);

		// Switch to object coordinates
		TGL::matrixMode(GL_MODELVIEW);

		// World starts
		TGL::loadIdentity();

		// Left handed coordinate system
		notifyOrientationChanged();
		TGL::enable(GL_TEXTURE_2D);
		TGL::bindTexture(GL_TEXTURE_2D, 0);
//		TGL::frontFace(GL_CW);
//		TGL::polygonMode(GL_FRONT_AND_BACK, GL_POINT);
//		TGL::polygonMode(GL_FRONT_AND_BACK, GL_LINE);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		TGL::cullFace(GL_FRONT);
//		TGL::enable(GL_CULL_FACE);
		TGL::disable(GL_CULL_FACE);
		TGL::enable(GL_DEPTH_TEST);
		TGL::clearDepth(1.0);
		TGL::clearColor(0.0F, 0.0F, 0.0F, 0.0F);   // alpha to 0.0
		TGL::clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		establishAttributes();
	}
}

RtVoid TGLRenderer::doWorldEnd(void) {
	finishGL();
}

// Synchronize
RtVoid TGLRenderer::doSynchronize(RtToken name) {
	if ( name && !strcmp(name, "reset") ) {
		m_currGLDisplayDesc.m_displayDesc = NULL;
		m_currGLDisplayDesc.m_hdc = NULL;
		#ifdef WIN32
		m_currGLDisplayDesc.m_hwnd = NULL;
		#endif
		#ifdef LINUX
		m_currGLDisplayDesc.m_hwnd = 0L;
		#endif
		m_openglIsActive = false;
	}
	if ( name && !strcmp(name, RI_ABORT) ) {
		// finishGL();
		if ( m_openglIsActive ) {
			// Finishing the rendering
			TGL::finish();
			TGL::bindTexture(GL_TEXTURE_2D, 0);
			m_glrc.releaseCurrentRC();
		}
		release_second(m_mapTMap);
		m_currGLDisplayDesc.m_displayDesc = NULL;
		m_currGLDisplayDesc.m_hdc = NULL;
		#ifdef WIN32
		m_currGLDisplayDesc.m_hwnd = NULL;
		#endif
		#ifdef LINUX
		m_currGLDisplayDesc.m_hwnd = 0L;
		#endif
		m_openglIsActive = false;
	}
	if ( name == RI_NULL ) {
		if ( m_openglIsActive ) {
			// Making our rendering context current
			#ifdef WIN32
			m_glrc.makeCurrentRC(m_currGLDisplayDesc.m_hdc);
			#endif
			#ifdef LINUX
			m_glrc.makeCurrentRC(m_currGLDisplayDesc.m_hdc, m_currGLDisplayDesc.m_hwnd);
			#endif
		}
	}
}

RtVoid TGLRenderer::establishProjection() {

	if ( !isValid() )
		return;

	// Display size is default for RiFormat

	// Setting the Pixel Aspect ratio
	if ( m_currGLDisplayDesc.m_hdc ) {
		if ( !options().m_bFormatCalled || options().m_fDisplayPixelAspect <= 0 ) {
			RtFloat hsize = (RtFloat)GetDeviceCaps(m_currGLDisplayDesc.m_hdc, HORZSIZE);
			RtFloat vsize = (RtFloat)GetDeviceCaps(m_currGLDisplayDesc.m_hdc, VERTSIZE);
			RtFloat hres = (RtFloat)GetDeviceCaps(m_currGLDisplayDesc.m_hdc, HORZRES);
			RtFloat vres = (RtFloat)GetDeviceCaps(m_currGLDisplayDesc.m_hdc, VERTRES);
			options().m_fDisplayPixelAspect = (hsize/hres) / (vsize/vres);
		}
	}

	// Setting the Display size
	if ( m_currGLDisplayDesc.m_hwnd ) {
		RECT windowRect;
		GetClientRect(m_currGLDisplayDesc.m_hwnd, &windowRect);
		if ( !options().m_bFormatCalled || options().m_iDisplayHRes < 0 ) {
			options().m_iDisplayHRes = windowRect.right;
		}
		if ( !options().m_bFormatCalled || options().m_iDisplayVRes < 0  ) {
			options().m_iDisplayVRes = windowRect.bottom;
		}
	}

	// If the distance of the near plane is to small, errors occur
	if ( options().m_fCameraNearClip < (RtFloat)0.01 )
		options().m_fCameraNearClip = (RtFloat)0.01;

#ifdef OWNVIEWING
	if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {
		std::swap(options().m_fCameraNearClip, options().m_fCameraFarClip);
	}
#endif

	TIntermediateRenderer::establishProjection();

#ifdef OWNVIEWING
	if ( !strcmp(options().m_tCameraProjection, RI_PERSPECTIVE) ) {
		std::swap(options().m_fCameraNearClip, options().m_fCameraFarClip);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
// RiDisplay - setting the display device, throws RIE_NOMEM
//             supports only one display, framebuffer (Window)
//             - later file and printer (over MemoryContext)
//             parameters "DEVICECONTEXT" HDC, "WINDOWHANDLE" HWND
RtVoid TGLRenderer::doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {
	// First scanning for default Parameters name, type and RI_ORIGIN and
	// push it to options().m_displays
	TDisplayDesc &d = options().m_displays.back();
	void *p = d.m_parameterMap.getValueOf(ri_DeviceContext);
	if ( p ) {
		m_currGLDisplayDesc.m_hdc = *(HDC *)p;
	}
	p = d.m_parameterMap.getValueOf(ri_WindowHandle);
	if ( p ) {
		m_currGLDisplayDesc.m_hwnd = *(HWND *)p;
	}
	#ifdef LINUX
	p = d.m_parameterMap.getValueOf(ri_VisualInfo);
	if ( p ) {
		m_currGLDisplayDesc.m_visInfo = *(XVisualInfo **)p;
	}
	#endif // LINUX
}


///////////////////////////////////////////////////////////////////////////////
// attribute blocks

RtVoid TGLRenderer::doAttributeEnd(void) {
	if ( m_openglIsActive ) {
		TGL::flush();
	}
}


void TGLRenderer::establishAttributes() {
	TIntermediateRenderer::establishAttributes();

	if ( !isValid() )
		return;

	if ( !m_openglIsActive ) return;

	establishShader(IDX_SHADER_SURFACE);
	if ( attributes().m_nSides == 1 ) {
		TGL::cullFace(GL_FRONT);
		TGL::enable(GL_CULL_FACE);
	} else {
		TGL::disable(GL_CULL_FACE);
	}
	m_callri->doGeometricRepresentation(attributes().m_geometricRepresentation.c_str());
}

void TGLRenderer::establishShader(int shaderType) {
	if ( !isValid() )
		return;
	if ( !m_openglIsActive ) return;

	if ( attributes().m_matte ) {
		GLfloat color[4] = {0.0, 0.0, 0.0, 1.0};
		TGL::color4(&color[0]);
		TGL::disable(GL_LIGHTING);
		return;
	}
	RtFloat ricolor[4];
	transformRGB(ricolor, 1, &attributes().m_color[0]);
	TGL::color4(ricolor);
	TIntermediateRenderer::establishShader(shaderType);
}

///////////////////////////////////////////////////////////////////////////////
// attributes

RtVoid TGLRenderer::doColor(RtColor Cs) {
	establishShader(IDX_SHADER_SURFACE);
}

RtVoid TGLRenderer::doOpacity(RtColor Cs) {
	establishShader(IDX_SHADER_SURFACE);
}


///////////////////////////////////////////////////////////////////////////////
// Shader

RtVoid TGLRenderer::doMatte(RtBoolean onoff) {
	establishShader(IDX_SHADER_SURFACE);
}

///////////////////////////////////////////////////////////////////////////////
// model

RtVoid TGLRenderer::doGeometricApproximation(RtToken type, RtFloat value) {
	if ( value >= 1.0 ) {
		if ( !strcmp(type, ri_Tesselation)) {
			currentAttributes().m_tesselation[0] = value;
			currentAttributes().m_tesselation[1] = value;
		} else if ( !strcmp(type, ri_TesselationU) ) {
			currentAttributes().m_tesselation[0] = value;
		} else if ( !strcmp(type, ri_TesselationV) ) {
			currentAttributes().m_tesselation[1] = value;
		}
	}
}

RtVoid TGLRenderer::doGeometricRepresentation(RtToken type) {
	if ( !m_openglIsActive ) return;

	if ( !strcmp(type, RI_SMOOTH) || !strcmp(type, RI_PRIMITIVE) ) {
		TGL::shadeModel(GL_SMOOTH);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if ( !strcmp(type, ri_Points) ) {
		TGL::shadeModel(GL_SMOOTH);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} else if ( !strcmp(type, ri_Lines) ) {
		TGL::shadeModel(GL_SMOOTH);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else if ( !strcmp(type, RI_CONSTANT) ) {
		TGL::shadeModel(GL_FLAT);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if ( !strcmp(type, ri_ConstantPoints) ) {
		TGL::shadeModel(GL_FLAT);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} else if ( !strcmp(type, ri_ConstantLines) ) {
		TGL::shadeModel(GL_FLAT);
		TGL::polygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} 
}

RtVoid TGLRenderer::doSides(RtInt nsides) {
	if ( !m_openglIsActive ) return;
	if ( nsides == 1 ) {
		TGL::cullFace(GL_FRONT);
		TGL::enable(GL_CULL_FACE);
	} else {
		TGL::disable(GL_CULL_FACE);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Transformation


// tesselate if true: CW, false: CCW
bool TGLRenderer::getCurrentFrontFace() {
	if ( !isValid() ) {
		handleError(RIE_BUG, RIE_ERROR, "TGLRenderer::getCurrentFrontFace() - Pointers are NULL");
		return true;
	}

	bool frontFace = attributes().m_currentOrientation;
	// bool coordSysOrientation = sign(CTM().determinant()) >= 0;

	return frontFace;
	// return frontFace==coordSysOrientation;
	// return true;
}

RtVoid TGLRenderer::notifyOrientationChanged() {
	TIntermediateRenderer::notifyOrientationChanged();

	if ( !isValid() )
		return;

	if ( m_currentContext->m_iState < CTX_WORLD )
		return;

	if ( !m_openglIsActive )
		return;

	bool frontFace = attributes().m_currentOrientation;

	bool cameraSwap = sign(m_currentContext->m_mapCoordSys[RI_CAMERA].determinant()) < 0;
	if ( cameraSwap )
		frontFace = !frontFace;

	if ( frontFace ) {
		TGL::frontFace(GL_CW);
	} else {
		TGL::frontFace(GL_CCW);
	}
}

RtVoid TGLRenderer::doIdentity(void) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doTransform(RtMatrix transform) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doConcatTransform(RtMatrix transform) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doPerspective(RtFloat fov) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doScale(RtFloat dx, RtFloat dy, RtFloat dz) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

RtVoid TGLRenderer::doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
	if ( m_openglIsActive ) {
		TGL::loadMatrix((const RtFloat *)CTM());
	}
}

///////////////////////////////////////////////////////////////////////////////
// geometric primitives

RtVoid TGLRenderer::doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( nvertices < 3 )
		return;

	if ( m_openglIsActive ) {
		TPolygon *p = new TPolygon(lineNo(), getDeclMap(), options().m_iNColor, nvertices, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	}
}

RtVoid TGLRenderer::doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( nloops < 1 || !nverts )
		return;

	RtFloat *ps = NULL; // for position
	RtFloat *nm = NULL; // for normal
	RtFloat *cs = NULL; // for surface color

	int i, j, k;

	int verts = 0;
	for ( i=0; i<nloops; ++i ) {
		verts += nverts[i];
		if ( nverts[i] < 3 )
			return;
	}

	for ( i=0; i<n; ++i ) {
		if ( !strcmp(tokens[i], RI_P) ) {
			ps = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_N) ) {
			nm = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_CS) ) {
			cs = (RtFloat *)params[i];
		}
	}

	if ( !ps )
		return;

	if ( m_openglIsActive ) {
		TGL::disableClientState(GL_EDGE_FLAG_ARRAY);
		TGL::disableClientState(GL_INDEX_ARRAY);
		TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);

		TGL::disableClientState(GL_COLOR_ARRAY);
		TGL::disableClientState(GL_NORMAL_ARRAY);
		TGL::disableClientState(GL_VERTEX_ARRAY);

		GLdouble *d = new GLdouble[verts*3];
		if (!d ) return;

		if ( !nm ) {
			RtPoint norm;
			TGL::disableClientState(GL_NORMAL_ARRAY);
			if ( planeNorm(&ps[6], &ps[0], &ps[3], norm) ) {
				TGL::normal(norm[0], norm[1], norm[2]);
			}
		}

		::gluTessBeginPolygon(m_tesselator, this);
		    int sumVerts = 0;
			for ( i=0, k=0; i<nloops; ++i ) {
				::gluTessBeginContour(m_tesselator);
				    sumVerts += nverts[i];
					for ( j=0; j<nverts[i]; ++j, k+=3 ) {
						d[k+0]=ps[(sumVerts-j)*3-3];
						d[k+1]=ps[(sumVerts-j)*3-2];
						d[k+2]=ps[(sumVerts-j)*3-1];
						::gluTessVertex(m_tesselator, &d[k], &d[k]);
						if ( nm ) {
							::gluTessNormal(m_tesselator, nm[k], nm[k+1], nm[k+2]);
						}
						// if ( cs ) {
						//	TGL::color(cs[k], cs[k+1], cs[k+2]);
						// }
					}
				::gluTessEndContour(m_tesselator);
			}
		::gluTessEndPolygon(m_tesselator);

		delete[] d;
	}
}

RtVoid TGLRenderer::doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( npolys < 1 || !nverts || !verts )
		return;

	if ( m_openglIsActive ) {
		TPointsPolygons *p = new TPointsPolygons(lineNo(), getDeclMap(), options().m_iNColor, npolys, nverts, verts, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	}
}

RtVoid TGLRenderer::doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( npolys < 1 || !nloops || !nverts || !verts )
		return;

	RtFloat *ps = NULL; // for position
	RtFloat *nm = NULL; // for surface normal
	RtFloat *cs = NULL; // for surface color

	int i, j, k, ipol;

	int maxverts = 0;
	int cverts   = 0;
	for ( i=0, k=0; i<npolys; ++i ) {
		for ( j=0; j<nloops[i]; ++j, ++k ) {
			cverts += nverts[k];
		}
		if ( maxverts < cverts ) maxverts = cverts;
	}

	for ( i=0; i<n; ++i ) {
		if ( !strcmp(tokens[i], RI_P) ) {
			ps = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_N) ) {
			nm = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_CS) ) {
			cs = (RtFloat *)params[i];
		}
	}

	if ( !ps )
		return;

	if ( m_openglIsActive ) {
		TGL::disableClientState(GL_EDGE_FLAG_ARRAY);
		TGL::disableClientState(GL_INDEX_ARRAY);
		TGL::disableClientState(GL_TEXTURE_COORD_ARRAY);

		TGL::disableClientState(GL_COLOR_ARRAY);
		TGL::disableClientState(GL_NORMAL_ARRAY);
		TGL::disableClientState(GL_VERTEX_ARRAY);

		GLdouble *d = new GLdouble[maxverts*3];
		if (!d ) return;

		RtInt *pnverts = nverts;
		RtInt *pverts  = verts;

		for ( ipol = 0; ipol < npolys; ++ipol) {
			if ( !nm ) {
				RtPoint norm;
				if ( planeNorm(&ps[pverts[2]*3], &ps[pverts[0]*3], &ps[pverts[1]*3], norm) ) {
					TGL::normal(norm[0], norm[1], norm[2]);
				}
			}
			::gluTessBeginPolygon(m_tesselator, this);
				for ( i=0, k=0; i<nloops[ipol]; ++i ) {
					::gluTessBeginContour(m_tesselator);
						for ( j=pnverts[i]; j>0; k+=3 ) {
							--j;
							d[k+0]=ps[pverts[j]*3+0];
							d[k+1]=ps[pverts[j]*3+1];
							d[k+2]=ps[pverts[j]*3+2];
							::gluTessVertex(m_tesselator, &d[k], &d[k]);
						}
						pverts += pnverts[i];
					::gluTessEndContour(m_tesselator);
				}
				pnverts+=nloops[ipol];
			::gluTessEndPolygon(m_tesselator);
		}

		delete[] d;
	}
}


RtVoid TGLRenderer::doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !strcmp(type, RI_BILINEAR) ) {
		TBilinearPatch *p = new TBilinearPatch(lineNo(), getDeclMap(), options().m_iNColor, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	} else if ( !strcmp(type, RI_BICUBIC) ) {
		TBicubicPatch *p = new TBicubicPatch(lineNo(), getDeclMap(), options().m_iNColor, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	}
}

RtVoid TGLRenderer::doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !strcmp(type, RI_BILINEAR) ) {
		TBilinearPatchMesh *p = new TBilinearPatchMesh(lineNo(), getDeclMap(), options().m_iNColor, nu, uwrap, nv, vwrap, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	} else if ( !strcmp(type, RI_BICUBIC) ) {
		TBicubicPatchMesh *p = new TBicubicPatchMesh(lineNo(), getDeclMap(), options().m_iNColor, attributes().m_uStep, attributes().m_vStep, nu, uwrap, nv, vwrap, n, tokens, params);
		if ( p ) {
			p->replay(*this, *this, this);
			delete p;
		}
	}
}


RtVoid TGLRenderer::doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
	currentAttributes().m_tessTrim = curUTesselation();
}

RtVoid TGLRenderer::doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !(m_nurbs && m_openglIsActive) )
		return;

	if ( nu+uorder <= 0 || nv+vorder <= 0 )
		return;

//	if ( attributes().m_trimCurve.m_nLoops == 0 ) {
		TNuPatch *nuPatch = new TNuPatch(lineNo(), getDeclMap(), options().m_iNColor, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
		if ( nuPatch ) {
			nuPatch->replay(*this, *this, this);
			// nuPatch->tesselate((IndexType)curUTesselation(), (IndexType)curVTesselation());
			delete nuPatch;
			return;
		}
//	}
#if 0
	RtFloat *ps = NULL,
			*pw = NULL;		// position
	RtFloat *nm = NULL;		// surface normal
	RtFloat *cs = NULL;		// surface color
	RtFloat *os = NULL;		// surface opacity

	RtInt i;

	for ( i = 0; i < n; ++i ) {
		if ( !strcmp(tokens[i], RI_P) ) {
			ps = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_PW) ) {
			pw = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_N) ) {
			nm = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_CS) ) {
			cs = (RtFloat *)params[i];
		} else if ( !strcmp(tokens[i], RI_OS) ) {
			os = (RtFloat *)params[i];
		}
	}

	if ( !(ps || pw) ) {
		return;
	}

	for ( i=0; i<nu+uorder; ++i ) {
		if ( i>0 && uknot[i] < uknot[i-1] ) {
			// Error
		}
	}
	for ( i=0; i<nv+vorder; ++i ) {
		if ( i>0 && vknot[i] < vknot[i-1] ) {
			// Error
		}
	}

	// Usage of Nurbs like in AYAM Renderer
	TGL::enable(GL_AUTO_NORMAL);
	TGL::enable(GL_NORMALIZE);
	::gluBeginSurface(m_nurbs);
		if ( ps ) {
			::gluNurbsSurface(m_nurbs, nu+uorder, uknot, nv+vorder, vknot, 3, nu*3, ps, uorder, vorder, GL_MAP2_VERTEX_3);
		} else if ( pw ) {
			::gluNurbsSurface(m_nurbs, nu+uorder, uknot, nv+vorder, vknot, 4, nu*4, pw, uorder, vorder, GL_MAP2_VERTEX_4);
		}
		// Trimcurves go here
		/*
		if ( umin > uknot[0] || umax < uknot[nu+uorder-1] || vmin > vknot[0] || vmax < vknot[nv+vorder-1] ) {
			GLfloat uknottrim[7] = { 0, 0, 0.25, 0.5, 0.75, 1.0, 1.0 };
			GLfloat ctlarray[15] = {
				umin, vmin, 1,
				umax, vmin, 1,
				umax, vmax, 1,
				umin, vmax, 1,
				umin, vmin, 1
			};

			::gluBeginTrim(m_nurbs);
			::gluNurbsCurve(m_nurbs, 7, uknottrim, 3, ctlarray, 2, GLU_MAP1_TRIM_3);
			::gluEndTrim(m_nurbs);
		}
		*/

		if ( attributes().m_trimCurve.m_nLoops > 0 ) {
			int j, sum;
			int nknots, order, num;

			GLfloat *uknottrim = &attributes().m_trimCurve.m_knots[0];
			GLfloat *ctlarray = &attributes().m_trimCurve.m_points[0];

			sum = 0;
			for ( i = 0; i < attributes().m_trimCurve.m_nLoops; ++i ) {
				::gluBeginTrim(m_nurbs);
				for ( j = 0; j < attributes().m_trimCurve.m_nCurves[i]; ++j, ++sum ) {
					num = attributes().m_trimCurve.m_n[sum];
					order = attributes().m_trimCurve.m_order[sum];
					nknots  = num + order;

					::gluNurbsCurve(m_nurbs, nknots, uknottrim, 3, ctlarray, order, GLU_MAP1_TRIM_3);
					uknottrim += nknots;
					ctlarray += num*3;
				}
				::gluEndTrim(m_nurbs);
			}
		}

	::gluEndSurface(m_nurbs);
	TGL::disable(GL_NORMALIZE);
	TGL::disable(GL_AUTO_NORMAL);
#endif
}


///////////////////////////////////////////////////////////////////////////////
// quadrics

RtVoid TGLRenderer::doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	// coneDisplacedV(height, radius, thetamax, (RtFloat)0.0, n, tokens, params);
	if ( radius <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax > (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	TCone *q = new TCone(lineNo(), getDeclMap(), options().m_iNColor, height, radius, thetamax, n, tokens, params);
	if ( q ) {
		q->replay(*this, *this, this);
		delete q;
	}
}

RtVoid TGLRenderer::doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( radius <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 || currentAttributes().m_tesselation[1] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax >= (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	if ( zmin > zmax ) {
		RtFloat t = zmin; zmin = zmax; zmax = t;
	}

	if ( zmin == zmax ) return;

	// Render a cylinder
	if ( m_openglIsActive ) {
		TCylinder *q = new TCylinder(lineNo(), getDeclMap(), options().m_iNColor, radius, zmin, zmax, thetamax, n, tokens, params);
		if ( q ) {
			q->replay(*this, *this, this);
			delete q;
		}
	}
}

RtVoid TGLRenderer::doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	// coneDisplacedV((RtFloat)0.0, radius, thetamax, height, n, tokens, params);
	if ( radius <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax > (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	TDisk *q = new TDisk(lineNo(), getDeclMap(), options().m_iNColor, height, radius, thetamax, n, tokens, params);
	if ( q ) {
		// Disk is a special form of a cone
		q->replay(*this, *this, this);
		delete q;
	}
}

RtVoid TGLRenderer::doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( currentAttributes().m_tesselation[0] <= 0 || currentAttributes().m_tesselation[1] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax >= (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	if ( point1[0] == point2[0] && point1[1] == point2[1] && point1[2] == point2[2] ) return;

	bool changeOrientation = point1[2] < point2[2];
	if ( point1[2] == point2[2] ) {
		changeOrientation = sqrt(point1[0]*point1[0] + point1[1]*point1[1]) < sqrt(point2[0]*point2[0] + point2[1]*point2[1]);
	}

	// Render a hyperboloid
	if ( m_openglIsActive ) {
		THyperboloid *q = new THyperboloid(lineNo(), getDeclMap(), options().m_iNColor, point1, point2, thetamax, n, tokens, params);
		if ( q ) {
			q->replay(*this, *this, this);
			delete q;
		}
	}
}


RtVoid TGLRenderer::doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( rmax <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 || currentAttributes().m_tesselation[1] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( zmin < (RtFloat)0.0 ) return;
	if ( zmax <= (RtFloat)0.0 ) return;
	if ( zmin == zmax ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax > (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	if ( zmin > zmax ) {
		RtFloat t = zmin; zmin = zmax; zmax = t;
	}

	// Render a paraboloid
	if ( m_openglIsActive ) {
		TParaboloid *q = new TParaboloid(lineNo(), getDeclMap(), options().m_iNColor, rmax, zmin, zmax, thetamax, n, tokens, params);
		if ( q ) {
			q->replay(*this, *this, this);
			delete q;
		}
	}
}

RtVoid TGLRenderer::doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	// Check the parameters

	if ( radius <= (RtFloat)0.0 ) return;
	if ( currentAttributes().m_tesselation[0] <= (RtFloat)0.0 || currentAttributes().m_tesselation[1] <= (RtFloat)0.0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax >= (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	if ( zmin > zmax ) {
		RtFloat t = zmin; zmin = zmax; zmax = t;
	}

	if ( zmax > radius )  zmax = radius;
	if ( zmax < -radius ) zmax = -radius;
	if ( zmin < -radius ) zmin = -radius;
	if ( zmin > radius )  zmin = radius;

	if ( zmin == zmax ) return;

	// Render a sphere
	if ( m_openglIsActive ) {
		TSphere *q = new TSphere(lineNo(), getDeclMap(), options().m_iNColor, radius, zmin, zmax, thetamax, n, tokens, params);
		if ( q ) {
			q->replay(*this, *this, this);
			delete q;
		}
	}
}

RtVoid TGLRenderer::doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( majorrad <= 0 ) return;
	if ( minorrad <= 0 ) return;

	if ( currentAttributes().m_tesselation[0] <= 0 || currentAttributes().m_tesselation[1] <= 0 ) return;
	if ( thetamax == (RtFloat)0.0 ) return;
	if ( thetamax <= (RtFloat)-360.0 ) thetamax = (RtFloat)-360.0;
	if ( thetamax > (RtFloat)360.0 ) thetamax = (RtFloat)360.0;

	if ( phimax > (RtFloat)360.0 ) phimax = (RtFloat)360.0;
	if ( phimax < -(RtFloat)360.0 ) phimax = -(RtFloat)360.0;

	if ( phimin > (RtFloat)360.0 ) phimin = (RtFloat)360.0;
	if ( phimin < -(RtFloat)360.0 ) phimin = -(RtFloat)360.0;

	if ( phimin == phimax ) return;

	if ( phimin > phimax ) {
		RtFloat f = phimax; phimax = phimin; phimin = f;
	}

	// Render a torus
	if ( m_openglIsActive ) {
		TTorus *q = new TTorus(lineNo(), getDeclMap(), options().m_iNColor, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
		if ( q ) {
			q->replay(*this, *this, this);
			delete q;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

RtVoid TGLRenderer::doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( !strcmp(type, "teapot") ) {
		teapot();
	}
}

///////////////////////////////////////////////////////////////////////////////
// curves, subdiv, blobs

RtVoid TGLRenderer::doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( nfaces < 1 || !nverts || !verts )
		return;

	if ( strcmp(scheme, this->RI_CATMULLCLARK) ) {
		handleError(RIE_SYNTAX, RIE_WARNING,
			"Subdivision scheme \"%s\" unknown, using \"catmull-clark\" instead", scheme, RI_NULL);
		scheme = this->RI_CATMULLCLARK;
	}

	TSubdivisionMesh *s = new TSubdivisionMesh(lineNo(), getDeclMap(), currentAttributes().m_colorComponents, scheme, nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs, n, tokens, params);
	if ( s ) {
		s->replay(*this, *this, this);
		delete s;
	}
}

RtVoid TGLRenderer::doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {
	RtInt i, fidx, leaf;

	// Using only color to display, only the leafs: ellipse and segment are shown
	RtInt     nLeafTokens   = 0;
	RtToken   leafTokens[2] = { "uniform color Cs", RI_NULL };
	RtPointer leafParams[2] = { RI_NULL, RI_NULL };

	const TParameter *Cs = NULL;
	TRiBlobby *blobbyVar = new TRiBlobby(lineNo(), getDeclMap(), options().m_iNColor, nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	if ( blobbyVar ) 
		Cs = blobbyVar->getParameterOf(RI_CS);
	if ( Cs && Cs->getCardinality() > 0 ) {
		leafParams[0] = Cs->getValue(0);
		nLeafTokens = 1;
	}

	for ( i = 0, leaf = 0; i < ncode-1; ++i ) {

		switch( code[i] ) {
			case RI_BLOBBY_OP_ADD: // Addition:      count, ...
				++i;
				i += code[i];
				break;
			case RI_BLOBBY_OP_MUL: // Multiply:      count, ...
				++i;
				i += code[i];
				break;
			case RI_BLOBBY_OP_MAX: // Maximum:       count, ...
				++i;
				i += code[i];
				break;
			case RI_BLOBBY_OP_MIN: // Minimum:       count, ...
				++i;
				i += code[i];
				break;
			case RI_BLOBBY_OP_SUB: // Substract:     subtrahend, minuend
				i += 2;
				break;
			case RI_BLOBBY_OP_DIV: // Divide:        dividend, divisor
				i += 2;
				break;
			case RI_BLOBBY_OP_NEG: // Negate:        negand
				i += 1;
				break;
			case RI_BLOBBY_OP_IDP: // Identity:      idempotentate
				i += 1;
				break;

			// Leafs

			case RI_BLOBBY_CONSTANT: //  1 floats
				++leaf;
				i += 1;
				break;

			case RI_BLOBBY_ELLIPSOID: // 16 floats
				if ( Cs && Cs->getNumberOfElements() > (unsigned long)leaf ) {
					leafParams[0] = Cs->getValue(leaf);
				}
				++leaf;
				i += 1;
				fidx = code[i];
				if ( fidx+16 <= nflt ) {
					RtMatrix t;
					t[0][0]=flt[fidx++];
					t[0][1]=flt[fidx++];
					t[0][2]=flt[fidx++];
					t[0][3]=flt[fidx++];
					t[1][0]=flt[fidx++];
					t[1][1]=flt[fidx++];
					t[1][2]=flt[fidx++];
					t[1][3]=flt[fidx++];
					t[2][0]=flt[fidx++];
					t[2][1]=flt[fidx++];
					t[2][2]=flt[fidx++];
					t[2][3]=flt[fidx++];
					t[3][0]=flt[fidx++];
					t[3][1]=flt[fidx++];
					t[3][2]=flt[fidx++];
					t[3][3]=flt[fidx++];

					transformBegin();
					concatTransform(t);
					sphereV((RtFloat).5, (RtFloat)-.5, (RtFloat).5, (RtFloat)360.0, nLeafTokens, leafTokens, leafParams);
					transformEnd();
				}
				break;

			case RI_BLOBBY_SEGMENT_BLOB: // 23 floats (start, end, radius, matrix)
				if ( Cs && Cs->getNumberOfElements() > (unsigned long)leaf ) {
					leafParams[0] = Cs->getValue(leaf);
				}
				++leaf;
				i += 1;
				fidx = code[i];
				if ( fidx+23 <= nflt ) {
					RtPoint start, end;
					RtPoint direction, normal;
					RtFloat radius, length, r;
					RtMatrix t;

					start[0]=flt[fidx++];
					start[1]=flt[fidx++];
					start[2]=flt[fidx++];

					end[0]=flt[fidx++];
					end[1]=flt[fidx++];
					end[2]=flt[fidx++];

					radius=flt[fidx++]/(RtFloat)2.0; // ??? Seems to be the diameter in the params, not the radius

					t[0][0]=flt[fidx++];
					t[0][1]=flt[fidx++];
					t[0][2]=flt[fidx++];
					t[0][3]=flt[fidx++];
					t[1][0]=flt[fidx++];
					t[1][1]=flt[fidx++];
					t[1][2]=flt[fidx++];
					t[1][3]=flt[fidx++];
					t[2][0]=flt[fidx++];
					t[2][1]=flt[fidx++];
					t[2][2]=flt[fidx++];
					t[2][3]=flt[fidx++];
					t[3][0]=flt[fidx++];
					t[3][1]=flt[fidx++];
					t[3][2]=flt[fidx++];
					t[3][3]=flt[fidx++];

					direction[0]=end[0]-start[0];
					direction[1]=end[1]-start[1];
					direction[2]=end[2]-start[2];

					length=sqrt(direction[0]*direction[0]+direction[1]*direction[1]+direction[2]*direction[2]);
					r = 0.0;
					normal[0] = normal[1] = normal[2] = 0.0;
					if ( length > 0.0 ) {
						direction[0]/=length;
						direction[1]/=length;
						direction[2]/=length;

						normal[0] = -direction[1];
						normal[1] = direction[0];

						r = rad2deg(acos(direction[2]));
						if ( normal[0]==0 && normal[1]==0 && normal[2]==0 && r != 0.0 )
							normal[2] = 1.0;
					}

					transformBegin();
						concatTransform(t);
						translate(start[0], start[1], start[2]);
						if ( r != 0.0 ) {
							rotate(r, normal[0], normal[1], normal[2]);
						}
						sphereV((RtFloat)radius, (RtFloat)-radius, (RtFloat)0.0, (RtFloat)360.0, nLeafTokens, leafTokens, leafParams);
						if ( length > 0.0 ) {
							cylinderV((RtFloat)radius,(RtFloat)0.0, length, (RtFloat)360.0, nLeafTokens, leafTokens, leafParams);
							translate(0.0, 0.0, length);
						}
						sphereV((RtFloat)radius, (RtFloat)0.0, (RtFloat)radius, (RtFloat)360.0, nLeafTokens, leafTokens, leafParams);
					transformEnd();
				}
				break;

			case RI_BLOBBY_REPELLING_PLANE: //  2 Indices, first for 1 string param, 2nd for 4 floats
				++leaf;
				i += 2;
				break;

			default:
				break;
		}
	}
	delete blobbyVar;
}



RtVoid TGLRenderer::doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {
	if ( npts <= 0 )
		return;

	const TParameter *P = NULL;
	TRiPoints *pointsVar = new TRiPoints(lineNo(), getDeclMap(), options().m_iNColor, npts, n, tokens, params);

	if ( pointsVar ) {
		P = pointsVar->getParameterOf(RI_P);
		if ( !P ) {
			delete pointsVar;
			return;
		}
		const RtFloat *p = (RtFloat *)P->getValue();

		if ( p ) {		
			RtInt i;
			RtFloat radius = (RtFloat)0.004, p1, p2, p3;
			// const TParameter *Cs = pointsVar->getParameterOf(RI_CS);
			// const TParameter *N = pointsVar->getParameterOf(RI_N);
			attributeBegin();
				geometricApproximation("tesselation", (RtFloat)4.0);
				sides(1);
				orientation(RI_LH);
				for ( i= 0; i<npts; ++i ) {
					transformBegin();
						p1 = *(p++);
						p2 = *(p++);
						p3 = *(p++);
						translate(p1, p2, p3);
						sphereV((RtFloat)radius, (RtFloat)-radius, (RtFloat)0.0, (RtFloat)360.0, 0, NULL, NULL);
					transformEnd();
				}
			attributeEnd();
		}
	}
	delete pointsVar;
}


///////////////////////////////////////////////////////////////////////////////
// procedural

RtVoid TGLRenderer::doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) {
	if ( subdivfunc ) {
		subdivfunc(this, data, RI_INFINITY);
	}

	// Don't care freefunc by now
}

RtVoid TGLRenderer::handleDelayedReadArchive(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleDelayedReadArchive(data, detail);
	if ( data ) {
		RtString *dataptr = (RtString *)data;
		readArchiveV(dataptr[0], NULL, 0, NULL, NULL);
	}
	// Don't care freefunc by now
}

RtVoid TGLRenderer::handleRunProgram(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleRunProgram(data, detail);
	if ( data ) {
		// remark: better run Program and interpret stdin as RIB
		RtString *dataptr = (RtString *)data;
		if ( dataptr[0] ) {
			RtString procname = dataptr[0];
			RtString option = dataptr[1];
			std::string proccall;
			std::string filenamestr;
			proccall = procname;

			// TODO get temporary filename
			proccall += " >";
			proccall += getTemp();
			proccall += "\\proctemp.rib";
			if ( option ) {
				proccall += " ";
				proccall += option;
			}
			system(proccall.c_str());
			filenamestr = getTemp();
			filenamestr += "\\proctemp.rib";
			TRibParser ribParser(filenamestr.c_str(), *this, *this, NULL, 0, NULL, NULL);
			ribParser.parseFile();
		}
	}

	// Don't care freefunc by now
}

RtVoid TGLRenderer::handleDynamicLoad(RtPointer data, RtFloat detail) {
	TIntermediateRenderer::handleDynamicLoad(data, detail);

	// Load dynamic program (dll) and call it's internal sundivfunc
	if ( data ) {
		RtString *dataptr = (RtString *)data;
		if ( dataptr[0] ) {
			RtString procname = dataptr[0];
			RtString option = dataptr[1];

			if ( procname ) {
				HINSTANCE hinst;
				if ( (hinst = LoadLibrary(procname)) != 0L ) {
					// Get pointers to the functions
					RiCppConvertParameters convertParameters = (RiCppConvertParameters)GetProcAddress(hinst, "RiCppConvertParameters");
					RiCppSubdivide subdivide = (RiCppSubdivide)GetProcAddress(hinst, "RiCppSubdivide");
					// RiCppFree freeFunc = (RiCppFree)GetProcAddress(hinst, "RiCppFree");

					RtPointer params = NULL;
					if ( convertParameters ) {
						params = convertParameters(option);
					} else if ( option ) {
						// error
					}

					if ( subdivide ) {
						subdivide(this, params, RI_INFINITY);
					} else {
						// error
					}

					// Ignore free here

					FreeLibrary(hinst);
				} else {
					// Error
				}
			}
		}
	}

	// Don't care freefunc by now
}
