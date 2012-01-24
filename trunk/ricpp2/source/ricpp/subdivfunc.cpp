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

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

#ifndef _RICPP_TOOLS_ENV_H
#include "ricpp/tools/env.h"
#endif // _RICPP_TOOLS_ENV_H

using namespace RiCPP;

RtToken CProcDelayedReadArchive::myName() {return RI_DELAYED_READ_ARCHIVE; }
RtVoid CProcDelayedReadArchive::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{
	detail = detail+0;

	if ( !data )
		return;

	RtString filename = ((RtString *)data)[0];
	if ( !filename || !filename[0] )
		return;
	ri.readArchiveV(filename, 0, 0, 0, 0);
}

CProcDelayedReadArchive CProcDelayedReadArchive::func;

RtToken CProcRunProgram::myName() {return RI_RUN_PROGRAM; }
RtVoid CProcRunProgram::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{
	detail = detail+0;

	if ( !data )
		return;

	RtString cmd = ((RtString *)data)[0];
	RtString genRequestData = ((RtString *)data)[1];

	if ( !cmd || !cmd[0] )
		return;

	std::string tmpPath;
	if ( !CEnv::getTempFilename(tmpPath, ".rib", false) ) {
		return;
	}

	std::string cmdline;
	if ( genRequestData && genRequestData[0] ) {
		cmdline += "echo ";
		cmdline += genRequestData;
		cmdline += " | ";
	}
	cmdline += cmd;
	cmdline += " >";
	cmdline += tmpPath;


	system(cmdline.c_str()); // Insecure !!!

	ri.readArchive(tmpPath.c_str(), 0, RI_NULL);
	
	/// @todo delete tmpfile or better use CreateProcess() in win32 and similar on other OS.
}

CProcRunProgram CProcRunProgram::func;

typedef void (CDECL *TypeSetRenderer)(IRi &);
typedef RtPointer (CDECL *TypeConvertParameters)(char *);
typedef void (CDECL *TypeSubdivide)(RtPointer, RtFloat);
typedef void (CDECL *TypeFree)(RtPointer);

RtToken CProcDynamicLoad::myName() {return RI_DYNAMIC_LOAD; }
RtVoid CProcDynamicLoad::operator()(IRi &ri, RtPointer data, RtFloat detail) const
{

	/** @todo searchpath for dynamic load procedurals */
	
	IRi *pri = &ri;

	pri = pri+0;
	detail = detail+0;

	if ( !data )
		return;

	RtString modname = ((RtString *)data)[0];

	if ( !modname || !modname[0] )
		return;

	char *initialdata = ((char **)data)[1];

	CDynLib *d = CDynLibFactory::newDynLib(modname, 0);
	if ( d ) {
		d->load();
		ILibFunc *setRendererPtr = d->getFunc("SetRenderer");
		ILibFunc *convertParametersPtr = d->getFunc("ConvertParameters");
		ILibFunc *subdividePtr = d->getFunc("Subdivide");
		ILibFunc *freePtr = d->getFunc("Free");

		RtPointer blinddata = 0;

		if ( setRendererPtr ) {
			TypeSetRenderer cp = (TypeSetRenderer)setRendererPtr->funcPtr();
			if ( cp )
				cp(ri);
		}

		if ( convertParametersPtr ) {
			TypeConvertParameters cp = (TypeConvertParameters)convertParametersPtr->funcPtr();
			if ( cp )
				blinddata = cp(initialdata);
		}

		if ( subdividePtr ) {
			TypeSubdivide sd = (TypeSubdivide)subdividePtr->funcPtr();
			if ( sd )
				sd(blinddata, detail);
		}

		if ( freePtr ) {
			TypeFree fd = (TypeFree)freePtr->funcPtr();
			if ( fd )
				fd(blinddata);
		}

		d->unload();
		CDynLibFactory::deleteDynLib(d);
	}
}

CProcDynamicLoad CProcDynamicLoad::func;

RtToken CProcFree::myName() {return RI_FREE; }
RtVoid CProcFree::operator()(RtPointer data) const
{
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
