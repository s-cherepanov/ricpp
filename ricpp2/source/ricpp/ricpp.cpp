/*
 *  ricpp.cpp
 *  ricppframework
 *
 *  Created by Andreas Pidde on 26.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#define RICPP_CREATEDLL

#include "ricpp/ricpp/ricpp.h"

using namespace RiCPP;

RICPP_EXTERN(const RtMatrix) RiIdentityMatrix = {
	{(RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0, (RtFloat) 0.0},
	{(RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 0.0, (RtFloat) 1.0}
};

RICPP_EXTERN(const RtToken) RI_NULL = (RtToken)0L;
RICPP_EXTERN(const RtToken) RI_FRAMEBUFFER = "framebuffer", RI_FILE = "file";

IFilterFunc::IFilterFunc() {}
IFilterFunc::~IFilterFunc() {}
