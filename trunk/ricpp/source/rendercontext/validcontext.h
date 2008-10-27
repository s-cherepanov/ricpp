#ifndef _RICPP_RENDERCONTEXT_VALIDCONTEXT_H
#define _RICPP_RENDERCONTEXT_VALIDCONTEXT_H

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

/*! \file validcontext.h
 *  \brief Declaration of TValidContext, a class to test if RenderMan interface calls are valid in contexts (e.g. a world block)
 *  \author Andreas Pidde (andreas@pidde.de)
 *  \anchor [RISPEC3.2] Pixar; The RenderMan Interface Version 3.2; 2000, PDF document
 *  \anchor [UPS89]     Steve Upstill; The RenderMan Companion; 1989, Addison Wesley
 */

#include "ribparser/riconstants.h"

////////////////////////////////////////////////////////////////////////////////
//! Class to test the valid contexts for RenderMan interface calls
class TValidContext
{
	//! Stores the information for valid contexts for routine in a table.
	/*! m_ucInterfaceContext uses the IDX_ constants (e.g. IDX_SPHERE) as indicees. It stores
	 *  a bitfield as content: CTX_BIT_ constants (e.g. CTX_BIT_WORLD) for the bits.
	 *  see also \ref [UPS89] Valid contexts for RenderMan Procedurte calls, pp.56
     *  and \ref [RISPEC3.2] definition of the new RI3.2 procs
	 */
	unsigned short m_ucInterfaceContext[N_TOKENS];
public:
	//! Initializes the valid contexts for all RI routines
	TValidContext();

	//! Queries the validity of a routine for a given context
	/*! \param idxRoutine Index of a routine (interface call) to test IDX_ constant (e.g. IDX_SPHERE)
	 *  \param idxContext Index of a context to test for CTX_ constant (e.g. CTX_WORLD)
	 *  \return true if call is valid, false otherwise.
	 */
	bool isValid(unsigned int idxRoutine, unsigned int idxContext) const;
}; // TValidContext

#endif // _RICPP_RENDERCONTEXT_VALIDCONTEXT_H

