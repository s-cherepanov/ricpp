#ifndef _RICPP_RICPP_VARSUBST_H
#define _RICPP_RICPP_VARSUBST_H

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif

namespace RiCPP {

	class IVarSubstCallback {
	public:
		inline virtual ~IVarSubstCallback() {}
		virtual bool getValue(CValue &p, const char *identifier) const = 0;
	};

	extern bool getEnvValue(CValue &p, const char *identifier);
	extern std::string &varSubst(std::string &aStr, char varId='$', const IVarSubstCallback *cb=0, const char *stdPath=0, const char *curPath=0);
}

#endif // _RICPP_RICPP_VARSUBST_H
