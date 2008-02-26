#ifndef _RICPP_RI_RIINTERNAL_H
#define _RICPP_RI_RIINTERNAL_H

/*
 *  riinternal.h
 *  ricppframework
 *
 *  Created by Andreas Pidde on 26.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <cstdarg>

namespace RiCPP { /* */

class IRiRoot;
extern IRiRoot *_ricppRoot();
extern void _ricppRoot(IRiRoot *aRoot);
extern RtInt _ricppGetArgs(va_list marker, RtToken token, RtToken **tokens, RtPointer **params);


#define PREAMBLE { \
	if ( RiCPP::_ricppRoot() == 0 ) { \
		return; \
	} \
}

#define POSTAMBLE

#define PREAMBLE_RET(RETVAL) { \
	if ( RiCPP::_ricppRoot() == 0 ) { \
		return (RETVAL); \
	} \
}

#define POSTAMBLE_RET(RETVAL) \
	return (RETVAL);

#define GETARGS(VAR) \
	RtToken *tokens; \
	RtPointer *params; \
	va_list	args; \
	va_start(args,VAR); \
	RtInt n = RiCPP::_ricppGetArgs(args,(VAR), &tokens, &params);

} /* namespace RiCPP */

#endif // _RICPP_RI_RIINTERNAL_H
