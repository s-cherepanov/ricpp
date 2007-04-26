#ifndef _RICPP_RENDERERFACTORY_RICPPAF_H
#define _RICPP_RENDERERFACTORY_RICPPAF_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2007, Andreas Pidde
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

/**
 *  @file ricppaf.h
 *  @brief Abstract factory to create an interface to a renderer
 *  @author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {
/**
 * Abstract factory to create an interface for a RiCPP renderer
 */
class IRiCPPAbstractFactory {
	/**
	 * Create an iterface to a RiCPP renderer
	 * @return Reference to the interface. The interface is likely to be
	 *         a singleton
	 */
	virtual IRi *createRenderer() = 0;
}; // IRiCPPAbstractFactory
}

#endif //  _RICPP_RENDERERFACTORY_RICPPAF_H
