// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
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

/** @file subdivfunc.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard subdivision functions and free function for IRi::procedural()
 *         not yet implemented
 */

#include "ricpp/ricpp/subdivfunc.h"

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <cstdlib> //< used for free()

using namespace RiCPP;

RtVoid CProcDelayedReadArchive::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{
	ri = ri;
	data = data;
	detail = detail;
}

CProcDelayedReadArchive CProcDelayedReadArchive::func;

RtVoid CProcRunProgram::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{
	ri = ri;
	data = data;
	detail = detail;
}

CProcRunProgram CProcRunProgram::func;

RtVoid CProcDynamicLoad::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{
	ri = ri;
	data = data;
	detail = detail;
}

CProcDynamicLoad CProcDynamicLoad::func;

RtVoid CProcFree::operator()(IRi &ri, RtPointer data) const
{
	ri = ri;
	if (data)
		free(data);
};

CProcFree CProcFree::func;


//
// CFilterFuncFactory
//

ISubdivFunc *CSubdivFuncFactory::newFunc(RtToken name)
{
	if ( emptyStr(name) )
		return 0;

	if ( CProcDelayedReadArchive::myName() == name )
		return new CProcDelayedReadArchive;

	if ( CProcRunProgram::myName() == name )
		return new CProcRunProgram;

	if ( CProcDynamicLoad::myName() == name )
		return new CProcDynamicLoad;

	return 0;
}

void CSubdivFuncFactory::deleteFunc(ISubdivFunc *f)
{
	if ( !f )
		return;
	delete f;
}


ISubdivFunc *CSubdivFuncFactory::singleton(RtToken name) const
{
	if ( emptyStr(name) )
		return 0;

	if ( CProcDelayedReadArchive::myName() == name )
		return &CProcDelayedReadArchive::func;

	if ( CProcRunProgram::myName() == name )
		return &CProcRunProgram::func;

	if ( CProcDynamicLoad::myName() == name )
		return &CProcDynamicLoad::func;

	return 0;
}

#if 0
//
// CFreeFuncFactory
//

IFreeFunc *CFreeFuncFactory::newFunc(RtToken name)
{
	if ( emptyStr(name) )
		return 0;

	if ( CProcFree::myName() == name )
		return new CProcFree;

	return 0;
}

void CFreeFuncFactory::deleteFunc(IFreeFunc *f)
{
	if ( !f )
		return;
	delete f;
}

IFreeFunc *CFreeFuncFactory::singleton(RtToken name) const
{
	if ( emptyStr(name) )
		return 0;

	if ( CProcFree::myName() == name )
		return &CProcFree::func;

	return 0;
}
#endif // 0
