// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2003, Andreas Pidde
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

/*! \file riscene.cpp
 *  \brief List structure, used as scene graph
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "riscene.h"

TRiNode::~TRiNode() {
	if ( !m_linkDescendent && m_descendent )
		delete m_descendent;
	if ( m_next )
		delete m_next;
}

void TRiSceneBuilder::moveToEnd() {
	m_position.m_current = first();
	if ( !m_position.m_current ) {
		m_position.m_insert = &m_sentinel.descendent();
	} else {
		m_position.m_current = m_position.m_current->last();
		m_position.m_insert = &(m_position.m_current->next());;
	}
}

void TRiSceneBuilder::reset(TRiNode *first) {
	while ( !m_block.empty() )
		m_block.pop();
	while ( !m_mark.empty() )
		m_mark.pop();
	m_sentinel.descendent() = first;
	moveToEnd();
}

TRiNode *TRiSceneIterator::operator()() {
	TRiNode *rval = m_current;
	if ( m_current ) {
		if ( m_current->descendent() ) {
			m_block.push(m_current);
			m_current = m_current->descendent();
		} else if ( m_current->next() ) {
			m_current = m_current->next();
		} else {
			m_current = NULL;
			if ( !m_block.empty() ) {
				m_current = m_block.top();
				m_block.pop();
			}
		}
	}
	return rval;
}

/*
TRiNode *TRiSceneIterator::findHandle(TRiNode *handle) {
	TRiNode *ptr = m_first;
	while ( ptr ) {
		if ( ptr->next() == handle )
			return ptr->next();
		ptr = ptr->next();
	}
	return NULL;
}
*/
