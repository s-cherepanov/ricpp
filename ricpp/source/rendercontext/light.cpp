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

/*! \file light.cpp
 *  \brief Implementation of TLight light descriptors, stores description of a light shader
 *  (content of TRi::LightSource(), TRi::AreaLightSource() calls)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "light.h"

TLight::TLight(TDeclarationMap &decls, RtInt colorComps, bool isArea, bool isGlobal, const TMatrix3D &m, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	lightSource(decls, colorComps, isArea, isGlobal, m, name, n, tokens, params);
}

TLight::TLight(const TLight &light) {
	*this = light;
}

TLight::~TLight() {
	releaseAll();
}

TLight &TLight::duplicate() const {
	TLight *light = new TLight(*this);
	if ( !light )
		throw TSystemException(__FILE__, __LINE__);
	return *light;
}

TLight &TLight::operator=(const TLight &light) {
	if ( this == &light )
		return *this;

	m_isAreaLight = light.m_isAreaLight;
	m_isGlobalLight = light.m_isGlobalLight;
	m_CTM = light.m_CTM;
	m_lightParameters = light.m_lightParameters;

	return *this;
}

void TLight::releaseAll() {
	m_lightParameters.releaseAll();
}

void TLight::lightSource(TDeclarationMap &decls, RtInt colorComps, bool isArea, bool isGlobal, const TMatrix3D &m, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	m_isAreaLight = isArea;
	m_isGlobalLight = isGlobal;
	m_CTM = m;
	m_lightParameters.setName(name);
	m_lightParameters.set(decls, 0, 0, 0, 0, 0, colorComps, n, tokens, params);
}
