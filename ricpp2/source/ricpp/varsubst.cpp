#include "ricpp/ricpp/varsubst.h"

#ifndef _RICPP_TOOLS_ENV_H
#include "ricpp/tools/env.h"
#endif // _RICPP_TOOLS_ENV_H

namespace RiCPP {

	// Try environment variable
	bool getEnvValue(CValue &p, const char *identifier)
	{
		std::string var;
		CEnv::find(var, identifier, false);
		if ( !var.empty() ) {
			p.set(var.c_str());
			return true;
		}
		return false;
	}

	std::string &varSubst(std::string &aStr, char varId, const IVarSubstCallback *cb, const char *stdPath, const char *curPath)
	{
		/**
		 * Rib variables (RiSPEC draft) are written like $var $table:var ${table:var} (however,
		 * in RiSPEC variables in 'if expressions' are also given as $(calcvar) - these calculated variables are not evaluated
		 * here).
		 *
		 * Expansion only if Option "rib" "string varsubst" is set to a character (e.g. "$"). Will need some changes
		 * in future, also some in CStringList (remove the substitution). Maybe - also move the expansion of the variable
		 * from parsing to the backend, to allow the C++ binding to use variables - that will be more difficult
		 * to implement of course. Problem: in an if expression in an Archive the variable $Frame occurs. $Frame is substituted
		 * with the current frame number - being different to the number of the time of instanciation (leads also to errors in
		 * cached file archives). However, e.g. $Frame and ${Frame} is expanded while parsing, $(Frame) in if expressions while
		 * instanciating. Maybe will add a control to disable the evaluation of variables for the ribwriter. 
		 * @see getValue() CStringList
		 */
		 
		// No substitution?
		if ( !varId && !stdPath && !curPath )
			return aStr;

		bool found = false;
		std::string result, resval;
		std::string varname;
		CValue p;
		std::string::const_iterator i = aStr.begin();
		while ( i != aStr.end() ) {
			if ( (*i) == '@' && stdPath ) {
				found = true;
				p.get(resval);
				result += stdPath;
				++i;
				continue;
			}
			
			if ( (*i) == '&' && curPath ) {
				found = true;
				result += curPath;
				++i;
				continue;
			}
			
			if ( varId && (*i) == varId ) {
				++i;
				if ( i != aStr.end() ) {
					varname = "";
					p.clear();
					if ( (*i) == '{' ) {
						++i;
						int depth = 0;
						for ( ; i != aStr.end() && !((*i) == '}' && !depth); ++i ) {
							if ((*i) == '{')
								++depth;
							if ((*i) == '}')
								--depth;
							varname += (*i);
						}
						if ( i != aStr.end() )
							++i;
						varSubst(varname, varId, cb, stdPath, curPath); // recursive e.g. ${$var1$var2};
						if ( (cb && cb->getValue(p, varname.c_str())) || getEnvValue(p, varname.c_str()) ) {
							found = true;
							p.get(resval);
							result += resval;
						} else {
							result += "${";
							result += varname;
							result += "}";
						}
					} else {
						for ( ; i != aStr.end() && (isalnum(*i) || (*i) == ':'); ++i ) {
							char c = (*i);
							if ( c == ':' ) {
								++i;
								if ( (*i) == varId ) {
									--i;
									break;
								}
								--i;
							}
							varname += c;
						}
						// No i++
						if ( (cb && cb->getValue(p, varname.c_str())) || getEnvValue(p, varname.c_str()) ) {
							found = true;
							p.get(resval);
							result += resval;
						} else {
							result += "$";
							result += varname;
						}
					}
				}
			} else {
				result += (*i);
				++i;
			}
		}

		if ( found )
			aStr = result;

		return aStr;
	}

}