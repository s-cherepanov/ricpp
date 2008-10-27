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

/*! \file parametermap.cpp
 *  \brief Implementation of a container (TParameterMap) for a list
 *         of parameters (TParameter) of a function call.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "parametermap.h"

////////////////////////////////////////////////////////////////////////////////
/*! copy constructor, initialize *this with a copy of params.
 *  \params parameter map to copy from.
 */
TParameterMap::TParameterMap(const TParameterMap &params) {
	*this = params;
}

////////////////////////////////////////////////////////////////////////////////
/*! ~TParameterMap - destructor, frees all use resources by calling releaseAll().
 */
TParameterMap::~TParameterMap() {
	releaseAll();
}

////////////////////////////////////////////////////////////////////////////////
/* Duplicates all members of *this, allocatesd a new instance.
 * \return A deep copy of *this.
 * \exception TSystemException Throws this exception, if there is not enough
 *            memory for a new instance of TParameterMap.
 */
TParameterMap &TParameterMap::duplicate() const {
	TParameterMap *p = new TParameterMap(*this);
	if ( !p )
		throw TSystemException(__FILE__, __LINE__);
	return *p;
}

////////////////////////////////////////////////////////////////////////////////
/*! Assigns another parameter map (pars) to *this, frees memory used by
 *  *this first.
 *  \param pars Instance to copy from.
 *  \return *this.
 *  \exception TSystemException Throws this exception, if there is not enough
 *             memory To duplicate the parameters.
 */
TParameterMap &TParameterMap::operator=(const TParameterMap &pars) {
	if ( this == &pars )
		return *this;
	releaseAll();
	m_mapName = pars.m_mapName;
	m_parameters = pars.m_parameters;
	fillTokensParams(pars.getSize());
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*! frees all resources (i. e. all parameters).
 */
void TParameterMap::releaseAll() {
	m_parameters.clear();
	m_tokensVect.clear();
	m_paramsVect.clear();
}


////////////////////////////////////////////////////////////////////////////////
/*! Fills (first releases old data) structure with a list of parameters.
 *
 *  \param decls        Current declarations of the variables of a renderer context.
 *  \param vertices     Number of vertices (used by vertex class).
 *  \param corners      Number of corners (used by varying class).
 *  \param facets       Number of facets (used by uniform class).
 *  \param faceVertices Number of all vertices per face (used by facevertex class).
 *  \param faceCorners  Number of all corners per face (used by facevarying class).
 *  \param colorComps   Number of components per color.
 *  \param n            Number of Token-values in parameterlist.
 *  \param tokens       Tokens of of parameter list.
 *  \param values       Values of parameter list.
 *
 *  \return true success values are set, false otherwise.
 *
 *  \exception Throws TSystemException (via TParameterMap::add, TParamter::set()).
 *  \exception Throws TRendererError (via TParameterMap::add, TParamter::set()).
 */
bool TParameterMap::set(TDeclarationMap &decls, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]) {
	releaseAll();
	return add(decls, vertices, corners, facets, faceVertices, faceCorners, colorComps, n, tokens, params); // Throw
}


////////////////////////////////////////////////////////////////////////////////
/*! Adds a list of parameters to the structure (doesn't remove old data).
 *
 *  \param decls        Current declarations of the variables of a renderer context.
 *  \param vertices     Number of vertices (used by vertex class).
 *  \param corners      Number of corners (used by varying class).
 *  \param facets       Number of facets (used by uniform class).
 *  \param faceVertices Number of all vertices per face (used by facevertex class).
 *  \param faceCorners  Number of all corners per face (used by facevarying class).
 *  \param colorComps   Number of components per color.
 *  \param n            Number of Token-values in parameterlist.
 *  \param tokens       Tokens of of parameter list.
 *  \param values       Values of parameter list.
 *
 *  \return true success values are set, false otherwise.
 *
 * \exception TSystemException Via TParamter::set().
 * \exception TRendererError   Via TParamter::set().
 */
bool TParameterMap::add(TDeclarationMap &decls, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]) {
	bool rVal = true;

	for ( IndexType i = 0; i < (IndexType)n; ++i ) {
		const TParameterDeclaration *decl = decls.findDeclaration(tokens[i]);
		if ( decl ) {
			// Declared type, name of declaration is also the token for the parameter
			TParameter &p = m_parameters[decl->getParameterName()];
			rVal = p.set(decl, vertices, corners, facets, faceVertices, faceCorners, colorComps, i, NULL, params[i]) && rVal;
		} else {
			// Possibly inline declaration
			TParameterDeclaration inlinedecl(NULL, tokens[i], false); // parse declaration to get the tokenname
			TParameter &p = m_parameters[inlinedecl.getParameterName()];
			rVal = p.set(NULL, vertices, corners, facets, faceVertices, faceCorners, colorComps, i, tokens[i], params[i]) && rVal;
		}
	}

	fillTokensParams(n);
	return rVal;
}

////////////////////////////////////////////////////////////////////////////////
/*! Searches for the stored parameter for a name.
 *  \param name Name of the parameter to search for.
 *  \return A pointer to the parameter, if the parameter is found, NULL otherwise.
 */
const TParameter *TParameterMap::getParameterOf(const char *name) const {
	STRING2PARAM::const_iterator i = m_parameters.find(name);
	if ( i != m_parameters.end() ) {
		return &(i->second);
	}
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
/*! Searches parameter for a given position.
 *  \param position in paramter list.
 *  \return A pointer to the parameter, if the position is less then the size
 *          of the map, NULL otherwise.
 */
const TParameter *TParameterMap::getParameterOf(IndexType position) const{
	if ( position >= (IndexType)m_parameters.size() )
		return NULL;

	STRING2PARAM::const_iterator i = m_parameters.begin();
	while ( i != m_parameters.end() ) {
		if ( i->second.getPosition() == position )
			return &(i->second);
		i++;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/*! Searches the value for a given parameter name.
 *  \param name Name of the parameter to search for.
 *  \return A pointer to the value of a parameter, if the parameter is found,
 *          NULL otherwise.
 */
const RtPointer TParameterMap::getValueOf(const char *name) const {
	const TParameter *p = getParameterOf(name);
	return p ?
		p->getValue() :
		(const RtPointer)TRi::RI_NULL;
}


////////////////////////////////////////////////////////////////////////////////
/*! Sets the name of the parameter map, especially for used for TRi::options().
 *  \param name the name of the parameter map.
 */
void TParameterMap::setName(const char *name) {
	m_mapName = name ? name : "";
}


////////////////////////////////////////////////////////////////////////////////
/*! Gets the the name of the map as C string
 *  \return The name of the paramter map.
 *  \sa setName()
 */
const char *TParameterMap::getName() const {
	return m_mapName.c_str();
}

////////////////////////////////////////////////////////////////////////////////
/*! Function is called after m_parameters is filled.
 *  \param n Number of token value pairs stored in m_parameters.
 */
RtVoid TParameterMap::fillTokensParams(IndexType n) {
	m_tokensVect.clear();
	m_paramsVect.clear();
	m_tokensVect.resize(n);
	m_paramsVect.resize(n);
	m_tokensVect.assign(n, (RtToken)0L);
	m_paramsVect.assign(n, (RtPointer)0L);

	IndexType t;
	STRING2PARAM::const_iterator i = m_parameters.begin();
	while ( i != m_parameters.end() ) {
		t = i->second.getPosition();
		if ( t >= 0 && t < n ) {
			m_tokensVect[t] = i->second.getName();
			m_paramsVect[t] = i->second.getValue();
		}
		i++;
	}
}
