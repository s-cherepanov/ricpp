#include "ricpp/baserenderer/node.h"

using namespace RiCPP;

CNode::~CNode() {
	if ( !m_linkDescendent && m_descendent )
		delete m_descendent;
	if ( m_next )
		delete m_next;
}
