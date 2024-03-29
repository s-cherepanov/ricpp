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

/** @file renderstate.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the facade for the render state objects (like modes, options, attributes)
 */

#include "ricpp/ricontext/renderstate.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

using namespace RiCPP;

static const bool _DEF_CACHE_FILE_ARCHIVES=true;

#ifdef _DEBUG
// #define _TRACE
// #define _TRACE_TRANS_PTS
// #define _TRACE_PROJECTION
#endif

bool CRenderState::CIfExprParser::match_word(
					   const char *matchStr,
					   const unsigned char **str,
					   std::string &result) const
{
	const unsigned char *sav = *str;
	std::string res;
	if ( match(matchStr, str, res) &&
		(la(str) == 0 || !(isalnum(la(str)) || la(str) == '_')) )
	{
		result += res;
		return true;
	}
	*str = sav;
	return false;
}

bool CRenderState::CIfExprParser::match_op(
					 const char *matchStr,
					 const unsigned char **str,
					 std::string &result) const
{
	const unsigned char *sav = *str;
	std::string res;
	if ( match(matchStr, str, res) &&
		(la(str) != matchStr[0]) )
	{
		result += res;
		return true;
	}
	*str = sav;
	return false;
}

unsigned char CRenderState::CIfExprParser::ws(
						const unsigned char **str,
						std::string &result) const
{
	return matchOneOf(" \t\n\r\f", str, result);
}

bool CRenderState::CIfExprParser::wss(
				const unsigned char **str,
				std::string &result) const
{
	if ( !ws(str, result) )
		return false;
	while ( ws(str, result) );
	return true;
}

unsigned char CRenderState::CIfExprParser::idchar(
							const unsigned char **str,
							std::string &result) const
{
	if ( match("_", str, result) ) {
		return '_';
	}
	
	return alphanum(str, result);
}

unsigned char CRenderState::CIfExprParser::sign_char(
							   const unsigned char **str,
							   std::string &result,
							   signed char &d) const
{
	if ( match("+", str, result) ) {
		d = 1;
		return '+';
	}
	
	if ( match("-", str, result) ) {
		d = -1;
		return '-';
	}
	
	d = 0;
	return 0;
}

unsigned char CRenderState::CIfExprParser::character(
	const unsigned char **str,
	std::string &result) const
{
	std::string dummy;
	unsigned char c;

	if ( match("\\", str, dummy) ) {
		c = matchOneOf("\\'", str, dummy);
		if ( c != 0 ) {
			switch ( c ) {
				case '\\' :
					result += "\\";
					break;
				case '\'' :
					result += "'";
					break;
			}
			return true;
		}
		result += '\\';
		return true;
	}

	c = la(str);
	if ( c != '\0' && c != '\'' ) {
		advance(str, result);
		return true;
	}

	return false;
}

bool CRenderState::CIfExprParser::name(
	const unsigned char **str,
	std::string &result) const
{
	if ( !idchar(str, result) )
		return false;
	while ( idchar(str, result) );

	const unsigned char *sav = *str;
	if ( match(":", str, result) ) {
		if ( !idchar(str, result) ) {
			*str = sav;
			return true;
		}
		while ( idchar(str, result) );
	}

	sav = *str;
	if ( match(":", str, result) ) {
		if ( !idchar(str, result) ) {
			*str = sav;
			return true;
		}
		while ( idchar(str, result) );
	}

	return true;
}


bool CRenderState::CIfExprParser::integer_const(
	const unsigned char **str,
	std::string &result,
	unsigned long &longresult) const
{
	unsigned char c, d;

	longresult = 0;

	if ( (c = digit_not_null(str, result, d)) != 0 ) {
		longresult = d;
		while ( (c = digit_not_null(str, result, d)) != 0 ) {
			longresult *= 10;
			longresult += d;
		}
		return true;
	}

	if ( match("0", str, result) ) {
		const unsigned char *sav = *str;
		if ( match("x", str, result) ) {
			if ( (c = hexdig(str, result, d)) != 0 ) {
				longresult = d;
				while ( (c = hexdig(str, result, d)) ) {
					longresult *= 16;
					longresult += d;
				}
				return true;
			} else {
				*str = sav;
				longresult = 0;
				return true;
			}
		}

		if ( (c = octdig(str, result, d)) != 0 ) {
			longresult = d;
			while ( (c = octdig(str, result, d)) != 0 ) {
				longresult *= 8;
				longresult += d;
			}
			return true;
		}

		longresult = 0;
		return true;
	}

	return false;
}


bool CRenderState::CIfExprParser::integer_part(
	const unsigned char **str,
	std::string &result,
	unsigned long &longresult) const
{
	unsigned char c, d;

	longresult = 0;

	if ( (c = digit(str, result, d)) == 0 )
		return false;
	
	longresult = d;
	while ( (c = digit(str, result, d)) ) {
		longresult *= 10;
		longresult += d;
	}

	return true;
}


bool CRenderState::CIfExprParser::exponent(
	const unsigned char **str,
	std::string &result,
	signed long &longresult) const
{
	const unsigned char *sav = *str;
	signed char d;

	longresult = 0;

	if ( !matchOneOf("Ee", str, result) )
		return false;

	d = 1;
	sign_char(str, result, d);

	unsigned long res;
	if ( integer_part(str, result, res) ) {
		longresult = d * res;
		return true;
	}

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::float_const(
	const unsigned char **str,
	std::string &result,
	double &floatresult) const
{
	const unsigned char *sav = *str;
	std::string floatStr;
	unsigned long ulval;
	signed long slval;

	if ( integer_part(str, floatStr, ulval) ) {
		if ( match(".", str, floatStr) ) {
			if ( integer_part(str, floatStr, ulval) ) {
				exponent(str, floatStr, slval);
			}
			result += floatStr;
			floatresult = atof(floatStr.c_str());
			return true;
		}
		if ( exponent(str, floatStr, slval) ) {
			result += floatStr;
			floatresult = atof(floatStr.c_str());
			return true;
		}
	} else {
		if ( match(".", str, result) ) {
			if ( integer_part(str, floatStr, ulval) ) {
				exponent(str, floatStr, slval);
				result += floatStr;
				floatresult = atof(floatStr.c_str());
				return true;
			}
		}
	}

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::number(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	double floatresult;
	if ( float_const(str, result, floatresult) ) {
		val.set(static_cast<RtFloat>(floatresult));
		return true;
	}

	unsigned long longresult;
	if ( integer_const(str, result, longresult) ) {
		val.set(static_cast<RtInt>(longresult));
		return true;
	}

	return false;
}


bool CRenderState::CIfExprParser::quotestring(
	const unsigned char **str,
	std::string &result,
	std::string &strval) const
{
	const unsigned char *sav = *str;
	std::string rval;

	if ( match("'", str, rval) ) {
		while ( character(str, strval) );
		if ( match("'", str, rval) ) {
			result += "'";
			result += strval;
			result += "'";
			return true;
		}
	}

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::calcvar(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	const unsigned char *sav = *str;
	std::string retval;

	bool case1 = false;
	bool case2 = match("$(", str, retval);
	if ( !case2 )
		case1 = match("${", str, retval); // However, these are already avaluated while parsing
		
	assert( !(case1 && case2) );
	
	if ( case1 || case2 ) {
		wss(str, retval);
		if ( expr(str, retval, val) ) {
			wss(str, retval);
			if ( case1 ? match("}", str, retval) : match(")", str, retval) ) {
				std::string strname;
				val.get(strname);
				if ( m_outer->getValue(val, strname.c_str()) ) {
					result += retval;
					return true;
				}
				// error: value strname not found
				*str = sav;
				return false;
			}
		}
	} else 

	*str = sav;
	return false;
}

bool CRenderState::CIfExprParser::var(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	if ( calcvar(str, result, val) ) {
		return true;
	}

	const unsigned char *sav = *str;

	std::string strname;
	if ( match("$", str, strname) ) {
		strname = "";
		if ( name(str, strname) ) {
			if ( m_outer->getValue(val, strname.c_str()) ) {
				result += "$";
				result += strname;
				return true;
			}
			// error: value strname not found
			*str = sav;
			return false;
		}
	} /* else if ( match("${", str, strname) ) {
	    // nonrecursive
		strname = "";
		unsigned char c;
		while ( (c = la(str)) != '}' ) {
			if ( !c ) {
				// error variable name not closed
				*str = sav;
				return false;
			}
			advance(str, strname, 1);
		}
		if ( m_outer->getValue(val, strname.c_str()) ) {
			result += "${";
			result += strname;
			advance(str, result, 1); // '}'
			return true;
		}
		// error: value strname not found
		*str = sav;
		return false;
	} */

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::varstr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	if ( var(str, result, val) ) {
		return true;
	}

	std::string strval;
	if ( quotestring(str, result, strval) ) {
		val.set(strval.c_str());
		return true;
	}

	return false;
}

bool CRenderState::CIfExprParser::varstrlist(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	if ( !varstr(str, result, val) ) {
		return false;
	}

	int count = 1;
	CValue val2;

	do {
		wss(str, result);
		if ( count > 1 ) {
			// append string to val
			std::string str;
			val2.get(str);
			val.append(str);
		}
		++count;
	} while ( varstr(str, result, val2) );

	return true;
}

bool CRenderState::CIfExprParser::litvar(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	if ( number(str, result, val) ) {
		wss(str, result);
		return true;
	}

	if ( match_word("true", str, result) ) {
		val.set(1);
		wss(str, result);
		return true;
	}

	if ( match_word("false", str, result) ) {
		val.set(0);
		wss(str, result);
		return true;
	}

	if ( varstrlist(str, result, val) ) {
		return true;
	}

	return false;
}


bool CRenderState::CIfExprParser::expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);
	return log_or_expr(str, result, val);
}


bool CRenderState::CIfExprParser::log_or_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;
	
	if ( log_and_expr(str, res, val) ) {
		wss(str, res);
		while ( match("||", str, res) ) {
			wss(str, res);
			if ( !log_and_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val = val || val2;
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::log_and_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( incl_or_expr(str, res, val) ) {
		wss(str, res);
		while ( match("&&", str, res) ) {
			wss(str, res);
			if ( !incl_or_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val = val && val2;
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}

bool CRenderState::CIfExprParser::incl_or_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( excl_or_expr(str, res, val) ) {
		wss(str, res);
		while ( match_op("|", str, res) ) {
			wss(str, res);
			if ( !excl_or_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val |= val2;
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::excl_or_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( and_expr(str, res, val) ) {
		wss(str, res);
		while ( match("^", str, res) ) {
			wss(str, res);
			if ( !and_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val ^= val2;
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::and_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( eq_expr(str, res, val) ) {
		wss(str, res);
		while ( match_op("&", str, res) ) {
			wss(str, res);
			if ( !eq_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val &= val2;
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::eq_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;
	
	if ( rel_expr(str, res, val) ) {
		bool eq = false;
		bool neq = false;
		wss(str, res);
		while ( (eq = match("==", str, res)) || (neq = match("!=", str, res)) ) {
			wss(str, res);
			if ( !rel_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			if ( eq ) {
				val = val == val2;
			} else {
				val = val != val2;
			}
			wss(str, res);
			eq = false;
			neq = false;
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::rel_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( match_expr(str, res, val) ) {
		bool gt = false;
		bool ge = false;
		bool lt = false;
		bool le = false;
		wss(str, res);
		while ( (ge = match(">=", str, res)) || (le = match("<=", str, res)) || (gt = match(">", str, res)) || (lt = match("<", str, res)) ) {
			wss(str, res);
			if ( !match_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			wss(str, res);
			if ( gt ) {
				val = val > val2;
			} else if ( ge ) {
				val = val >= val2;
			} else if ( lt ) {
				val = val < val2;
			} else {
				// le
				val = val <= val2;
			}
			gt = false;
			ge = false;
			lt = false;
			le = false;
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::match_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( add_expr(str, res, val) ) {
		wss(str, res);
		if ( match("=~", str, res) ) {
			wss(str, res);
			if ( !add_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			std::string s;
			val2.get(s);
			if ( val.matchedBy(s.c_str()) )
				val.set(1);
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::add_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( mul_expr(str, res, val) ) {
		bool add = false;
		bool sub = false;
		wss(str, res);
		while ( (add = match("+", str, res)) || (sub = match("-", str, res)) ) {
			wss(str, res);
			if ( !mul_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			if ( add )
				val += val2;
			else
				val -= val2;
			wss(str, res);
			add = false;
			sub = false;
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::mul_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( pow_expr(str, res, val) ) {
		bool mul = false;
		bool div = false;
		wss(str, res);
		while ( (mul = match_op("*", str, res)) || (div = match_op("/", str, res)) ) {
			wss(str, res);
			if ( !pow_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			if ( mul )
				val *= val2;
			else
				val /= val2;
			wss(str, res);
			mul = false;
			div = false;
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::pow_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	CValue val2;

	if ( unary_expr(str, res, val) ) {
		wss(str, res);
		while ( match("**", str, res) ) {
			wss(str, res);
			if ( !unary_expr(str, res, val2) ) {
				*str = sav;
				return false;
			}
			val.powBy(val2);
			wss(str, res);
		}
		result += res;
		return true;
	}
	
	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::unary_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);

	const unsigned char *sav = *str;
	std::string res;
	
	unsigned char c = 0;
	
	c = match("!", str, res);
	c = c || match("-", str, res);
	match("+", str, res);
	
	wss(str, res);
	if ( primary_expr(str, res, val) ) {
		wss(str, res);
		result += res;
		if ( c == '!' ) {
			val = !val;
		} else if ( c ) {
			val.setNegative();
		}
		return true;
	}

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::primary_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	wss(str, result);
	
	if ( litvar(str, result, val) ) {
		return true;
	}
	
	const unsigned char *sav = *str;
	std::string res;

	if ( match_word("concat", str, res) ) {
		wss(str, res);
		if ( match("(", str, res) ) {
			wss(str, res);
			if ( expr(str, res, val) ) {
				wss(str, res);
				if ( match(",", str, res) ) {
					wss(str, res);
					CValue val2;
					if ( expr(str, res, val2) ) {
							wss(str, res);
							if ( match(")", str, res) ) {
								wss(str, res);
								result += res;
								std::string s2;
								val2.get(s2);
								val.append(s2);
								return true;
							}
					}
				}
			}
		}
	} else if ( match_word("defined", str, res) ) {
		wss(str, res);
		if ( match("(", str, res) ) {
			wss(str, res);
			std::string strname;
			if ( name(str, strname) ) {
				res += strname;
				wss(str, res);
				if ( match(")", str, res) ) {
					CValue val;
					wss(str, res);
					result += res;
					return m_outer->getValue(val, strname.c_str());
				}
			}
		}
	} else if ( match("(", str, res) ) {
		wss(str, res);
		if ( expr(str, res, val) ) {
			wss(str, res);
			if ( match(")", str, res) ) {
				wss(str, res);
				result += res;
				return true;
			}
		}
	}

	*str = sav;
	return false;
}


bool CRenderState::CIfExprParser::if_expr(
	const unsigned char **str,
	std::string &result,
	CValue &val) const
{
	if ( !str || !*str )
		return true;

	const unsigned char *sav = *str;
	wss(str, result);
	if ( (*str)[0] == 0 )
		return true;

	if ( !expr(str, result, val) ) {
		*str = sav;
		return false;
	}
	wss(str, result);

	if ( (*str)[0] == 0 )
		return true;

	*str =  sav;
	return false;
}

bool CRenderState::CIfExprParser::parse(RtString expr) const
{
	if ( !expr )
		return true;

	const unsigned char **str = (const unsigned char **)(&expr);
	std::string result;
	CValue val;

	try {
		if ( if_expr(str, result, val) ) {
			RtFloat fval = 0;
			val.get(fval);
			return fval != 0;
		} else {
			return false;
		}
	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch ( std::exception &e1 ) {
		throw ExceptRiCPPError(
			RIE_MATH, RIE_SEVERE,
			m_outer->printLineNo(__LINE__),
			m_outer->printName(__FILE__),
			"Error %s in expression.", e1.what());
	} catch( ... ) {
		throw ExceptRiCPPError(
			RIE_MATH, RIE_SEVERE,
			m_outer->printLineNo(__LINE__),
			m_outer->printName(__FILE__),
			"Unknown error in expression.");
	}
	return false;
}


/* ************************************************************************** */

CRenderState::CRenderState(
	CModeStack &aModeStack,
	COptionsFactory &optionsFactory,
	CAttributesFactory &attributesFactory,
	CTransformationFactory &transformationFactory,
	CFilterFuncFactory &filterFuncFactory,
	CRManInterfaceFactory &macroFactory
	) :
	m_resourceFactories(false),
	m_resourceStack("RES_"),
	m_objectMacros("OBJ_"),
	m_archiveMacros("ARC_"),
	m_cachedMacros("CACHE_"),
	m_lightSourceHandles("LIH_"),
	m_lightSources("LIH_")
{
	m_modeStack = &aModeStack;
	m_optionsFactory = &optionsFactory;
	m_attributesFactory = &attributesFactory;
	m_transformationFactory = &transformationFactory;
	m_filterFuncFactory = &filterFuncFactory;
	m_macroFactory = &macroFactory;
	m_frameNumber = 0;
	m_lineNo = -1;

	RI_RIB = RI_NULL;
	RI_CACHE_FILE_ARCHIVES = RI_NULL;
	RI_VARSUBST = RI_NULL;
	RI_QUAL_CACHE_FILE_ARCHIVES = RI_NULL;
	RI_QUAL_VARSUBST = RI_NULL;

	m_curMacro = 0;
	m_curReplay = 0;
	m_cacheFileArchives = _DEF_CACHE_FILE_ARCHIVES;

	m_reject = false;
	m_recordMode = false;

	m_executeConditional = true;
	m_accumulateConditional = true;
	m_ifCondition = false;
	
	CFilepath fp;
	std::string s(fp.filepath());
	s += CFilepathConverter::internalPathSeparator();
	m_baseUri.encodeFilepath(s.c_str(), "file");

	m_NDCToRaster = 0;
	m_screenToNDC = 0;
	m_cameraToScreen = 0;
	m_worldToCamera = 0;
	m_idTransform = 0;
	
	m_viewingOrientation = RI_LH;
	m_cameraOrientation = RI_LH;
	
	// Create options, attributes and transforms
	pushOptions();
	pushAttributes();
	pushTransform();
}

void CRenderState::clearStacks()
{
	while ( !m_attributesStack.empty() ) popAttributes();
	while ( !m_motionAttributesStack.empty() ) {
		CAttributes *a = m_motionAttributesStack.back();
		m_attributesFactory->deleteAttributes(a);
		m_motionAttributesStack.pop_back();
	}
	while ( !m_rememberedAttributes.empty() ) {
		CAttributes *a = m_rememberedAttributes.back();
		m_attributesFactory->deleteAttributes(a);
		m_rememberedAttributes.pop_back();
	}
	
	while ( !m_transformationStack.empty() ) popTransform();
	while ( !m_motionTransformationStack.empty() ) {
		CTransformation *trans = m_motionTransformationStack.back();
		m_transformationFactory->deleteTransformation(trans);
		m_motionTransformationStack.pop_back();
	}
	while ( !m_rememberedTransformations.empty() ) {
		CTransformation *trans = m_rememberedTransformations.back();
		m_transformationFactory->deleteTransformation(trans);
		m_rememberedTransformations.pop_back();
	}
	
	// Don't remove the global options
	while ( m_optionsStack.size() > 1 ) popOptions();

	if ( m_modeStack )
		m_modeStack->clear();
}

void CRenderState::freeAll()
{
	deleteTransMapCont(m_globalTransforms);
	deleteDeferedRequests();
	clearStacks();
	// Remove globals also
	while ( !m_optionsStack.empty() ) popOptions();
	
	m_transformationFactory->deleteTransformation(m_NDCToRaster);
	m_NDCToRaster = 0;
	m_transformationFactory->deleteTransformation(m_screenToNDC);
	m_screenToNDC = 0;
	m_transformationFactory->deleteTransformation(m_cameraToScreen);
	m_cameraToScreen = 0;
	m_transformationFactory->deleteTransformation(m_worldToCamera);
	m_worldToCamera = 0;
	m_transformationFactory->deleteTransformation(m_idTransform);
	m_idTransform = 0;
}

CRenderState::~CRenderState()
{
	freeAll();
}


void CRenderState::rememberState()
{
	if ( attributes().dirty() ) {
		m_rememberedAttributes.push_back(dynamic_cast<CAttributes *>(attributes().duplicate()));
		attributes().dirty(false);
	}
	if ( curTransform().dirty() ) {
		m_rememberedTransformations.push_back(curTransform().duplicate());
		curTransform().dirty(false);
	}
}

const CAttributes *CRenderState::rememberedAttributes() const
{
	if ( m_rememberedAttributes.empty() )
		return 0;
	return m_rememberedAttributes.back();
}

CAttributes *CRenderState::rememberedAttributes()
{
	if ( m_rememberedAttributes.empty() )
		return 0;
	return m_rememberedAttributes.back();
}

const CTransformation *CRenderState::rememberedTransformation() const
{
	if ( m_rememberedTransformations.empty() )
		return 0;
	return m_rememberedTransformations.back();
}

CTransformation *CRenderState::rememberedTransformation()
{
	if ( m_rememberedTransformations.empty() )
		return 0;
	return m_rememberedTransformations.back();
}

void CRenderState::deferRequest(CRManInterfaceCall *aRequest)
{
	if ( aRequest )
		m_deferedRequests.push_back(aRequest);
}

void CRenderState::deleteDeferedRequests()
{
	while ( !m_deferedRequests.empty() ) {
		CRManInterfaceCall *req = m_deferedRequests.back();
		m_macroFactory->deleteRequest(req);
		m_deferedRequests.pop_back();
	}
}

void CRenderState::restart()
{
	deleteTransMapCont(m_globalTransforms);
	deleteDeferedRequests();
	contextReset();
	contextBegin();
}

bool CRenderState::getRasterToCamera(CMatrix3D &m) const
{
	m.setPreMultiply(false);
	if ( cameraToScreen() && screenToNDC() && NDCToRaster() ) {
		m = NDCToRaster()->getInverseCTM();
		m.concatTransform(screenToNDC()->getInverseCTM());
		m.concatTransform(cameraToScreen()->getInverseCTM());
		return true;
	}
	m.identity();
	return false;
}

bool CRenderState::getCameraToRaster(CMatrix3D &m) const
{
	m.setPreMultiply(true);
	if ( cameraToScreen() && screenToNDC() && NDCToRaster() ) {
		m = NDCToRaster()->getCTM();
		m.concatTransform(screenToNDC()->getCTM());
		m.concatTransform(cameraToScreen()->getCTM());
		return true;
	}
	m.identity();
	return false;
}

bool CRenderState::getCameraToCurrent(CMatrix3D &m) const
{
	m.setPreMultiply(false);
	if ( worldToCamera() ) {
		m = worldToCamera()->getInverseCTM();
		m.concatTransform(curTransform().getInverseCTM());
		return true;
	}
	m.identity();
	return false;
}

bool CRenderState::getCurrentToCamera(CMatrix3D &m) const
{
	m.setPreMultiply(true);
	if ( worldToCamera() ) {
		m = worldToCamera()->getCTM();
		m.concatTransform(curTransform().getCTM());
		return true;
	}
	m.identity();
	return false;
}

void CRenderState::calcNDCToRaster()
{
	if ( !m_NDCToRaster )
		m_NDCToRaster = m_transformationFactory->newTransformation();

	if ( !m_NDCToRaster ) {
		// error
	} else {
		// Raster
		m_NDCToRaster->spaceType(RI_RASTER);

		RtFloat frameXRes, frameYRes;
		options().getFrameFormat(frameXRes, frameYRes);

		m_NDCToRaster->identity();
		m_NDCToRaster->scale(frameXRes, frameYRes, 1);
	}
}


void CRenderState::getProjectedScreenWindow(RtFloat &left, RtFloat &right, RtFloat &bot, RtFloat &top) const
{
	options().getScreenWindow(left, right, bot, top);
	
	RtToken projection = options().projectionName();
	if ( projection == RI_PERSPECTIVE ) {
		RtFloat fov = options().fov();
		
		if ( !nearlyZero(fov-(RtFloat)180.0) ) {
			RtFloat fovRad_2 = deg2rad(fov)/(RtFloat)2.0;
			RtFloat viewPlane = tan(fovRad_2);
#ifdef _TRACE
			std::cout << "# *** viewplane distance " <<  1.0/viewPlane << std::endl;
#endif
			if ( !nearlyZero(viewPlane) ) {
				left *= viewPlane;
				right *= viewPlane;
				bot *= viewPlane;
				top *= viewPlane;
#ifdef _TRACE
				std::cout << "# *** screenwindow left " <<  left << " right " << right << " bottom " << bot << " top " << top << std::endl;
#endif
			}
		}
	}
}

void CRenderState::calcScreenToNDC()
{
	if ( !m_screenToNDC )
		m_screenToNDC = m_transformationFactory->newTransformation();

	if ( !m_screenToNDC ) {
		// error
	} else {
		// Ndc
		m_screenToNDC->spaceType(RI_NDC);
		m_screenToNDC->identity();
		
		RtFloat left, right, bottom, top;
		options().getScreenWindow(left, right, bottom, top);
		
		// scale screenwindow and mirror on x
		RtFloat swid = right-left;
		RtFloat sht  = top-bottom;
		if ( !nearlyZero(swid) && !nearlyZero(sht) ) {
			m_screenToNDC->scale((RtFloat)1.0/swid, (RtFloat)-1.0/sht, 1);
		}
		// Translate to screen origin
		m_screenToNDC->translate(-left, -top, 0);

		// Screen

#       ifdef _TRACE
		std::cout << "ScreenToNDC " << std::endl;
		printMatrix(m_screenToNDC->getCTM().getMatrix());
#       endif
	}

	calcNDCToRaster();
}


bool CRenderState::adjustProjectionMatrix(CMatrix3D &projectionMatrix, CMatrix3D &inverseProjectionMatrix)
{
	RtToken projection = options().projectionName();

#ifdef _TRACE_PROJECTION
	std::cout << "ProjectionName:" << (projection ? projection : "RI_NULL") << std::endl;
#endif

	if ( projection == RI_ORTHOGRAPHIC ||
	     (projection == RI_PERSPECTIVE && nearlyZero(options().fov())) )
	{

		projectionMatrix.identity();
		inverseProjectionMatrix.identity();
		inverseProjectionMatrix.setPreMultiply(false);

		RtFloat hither = options().hither();
		RtFloat yon = options().yon();
		RtFloat depth = yon - hither;
		
		if ( !nearlyZero(depth) ) {
			projectionMatrix.scale(1, 1, (RtFloat)1.0/depth);
			inverseProjectionMatrix.scale(1, 1, depth);
		}
		projectionMatrix.translate(0, 0, -hither);
		inverseProjectionMatrix.translate(0, 0, hither);

		return true;

	} else if ( projection == RI_PERSPECTIVE ) {

		projectionMatrix.identity();
		inverseProjectionMatrix.identity();
		inverseProjectionMatrix.setPreMultiply(false);

		RtFloat fov = options().fov();
		CMatrix3D pm;
		pm.perspective(fov);
		
		RtFloat hither = options().hither();
		RtFloat yon = options().yon();
		RtPoint toClip[2] = { {0,0,hither}, {0,0,yon}};
		pm.transformPoints(2, &toClip[0]);
		hither = toClip[0][2];
		yon = toClip[1][2];
		RtFloat depth = yon - hither;
		if ( !nearlyZero(depth) ) {
			projectionMatrix.scale(1, 1, (RtFloat)1.0/depth);
			inverseProjectionMatrix.scale(1, 1, depth);
		}
		projectionMatrix.translate(0, 0, -hither);
		inverseProjectionMatrix.translate(0, 0, hither);
		
		projectionMatrix.perspective(fov);
		inverseProjectionMatrix.inversePerspective(fov);

		return true;

	} else if ( projection == RI_NULL ) {

		projectionMatrix.identity();
		inverseProjectionMatrix.identity();
		inverseProjectionMatrix.setPreMultiply(false);

		return true;

	}
	
	return false;
}

void CRenderState::setCameraToScreen()
{
	m_transformationFactory->deleteTransformation(m_cameraToScreen);
	m_cameraToScreen = m_transformationFactory->newTransformation();
	if ( !m_cameraToScreen ) {
		/// @todo: error nomem for m_cameraToScreen
	} else {
		m_cameraToScreen->spaceType(RI_SCREEN);
		if ( options().preProjectionMatrix() ) {
			*m_cameraToScreen = *(options().preProjectionMatrix());
		} else {
			m_cameraToScreen->identity();
		}
		CMatrix3D projectionMatrix, inverseProjectionMatrix;
		adjustProjectionMatrix(projectionMatrix, inverseProjectionMatrix);
		m_cameraToScreen->concatTransform(projectionMatrix.getMatrix(), inverseProjectionMatrix.getMatrix());

		m_cameraToScreen->concatTransform(m_preCamera);
	}
	
	calcScreenToNDC();

	CMatrix3D m;
	getCameraToRaster(m);
	m_viewingOrientation = handedness(m);
}

void CRenderState::setWorldToCamera()
{
	if ( !m_idTransform ) {
		m_idTransform = m_transformationFactory->newTransformation();
		m_idTransform->spaceType(RI_CURRENT);
	}
	
	m_transformationFactory->deleteTransformation(m_worldToCamera);
	m_worldToCamera = curTransform().duplicate();
	if ( !m_worldToCamera ) {
		/// @todo: error nomem for m_worldToCamera
		return;
	}

	m_worldToCamera->spaceType(RI_CAMERA);
	m_cameraOrientation = m_worldToCamera->coordSysOrientation();
}

void CRenderState::projection(RtToken name, const CParameterList &params)
{
	// Store the ctm as preprojection matrix
	if ( motionState().curState() == CMotionState::MOT_OUTSIDE || motionState().curSampleIdx() == 0 ) {
		// cur transform as pre projection matrix, can be motion blured
		options().preProjectionMatrix(curTransform());
	}

	// Sets the option
	options().projection(name, params);

	// Resets current transformation
	curTransform().reset();
}


void CRenderState::camera(RtToken name, const CParameterList &params)
{
	/// @todo Implementation (Store camera options and coordinate system)
}


void CRenderState::deleteTransMapCont(TypeTransformationMap &m)
{
	TypeTransformationMap::iterator pos;
	for( pos = m.begin(); pos != m.end(); pos++ ) {
		if ( (*pos).second != 0 )
			m_transformationFactory->deleteTransformation((*pos).second);
	}
	m.clear();
}


bool CRenderState::validRequest(EnumRequests req)
{
	assert(m_modeStack != 0);

	if ( !m_modeStack )
		return false;

	if ( !m_modeStack->validRequest(req) )
		return false;

	// Test motion block
	if ( m_modeStack->curMode() == MODE_MOTION )
	{
		m_motionState.request(req);
		if ( motionState().curState() != CMotionState::MOT_OUTSIDE &&
		    (motionState().curState() & ~CMotionState::MOT_INSIDE) != 0 )
		{
			return false;
		}
	}

	return true;
}

void CRenderState::contextBegin()
{
	pushOptions();
	pushAttributes();
	pushTransform();
	curTransform().spaceType(RI_CAMERA);

	m_objectMacros.mark();
	m_archiveMacros.mark();
	m_cachedMacros.mark();
	m_lightSources.mark();
	m_lightSourceHandles.mark();

	m_modeStack->contextBegin();
}


void CRenderState::compactArchive(std::list<CRiMacro *> &aList, TemplHandleStack<CRiMacro> &aMacroStack)
{
	CRiMacro *aMacro;
	while ( !aList.empty() ) {
		aMacro = aList.back();
		aList.pop_back();
		if ( aMacro != 0 ) {
			if ( aMacro->macroType() != CRiMacro::MACROTYPE_UNKNOWN ) {
				aMacroStack.insertObject(aMacro->handle(), aMacro); // In reverse order
			} else {
				delete aMacro;
			}
		}
	}
}

void CRenderState::markRoot(std::list<CRiMacro *> &l1, std::list<CRiMacro *> &l2, std::list<CRiMacro *> &l3)
{
	std::list<CRiMacro *> *lptr[3] = {&l1, &l2, &l3};

	for ( std::list<CRiMacro *>::iterator iter = l1.begin(); iter != l1.end(); iter++ ) {
		if ( !(*iter) )
			continue;
		if ( (*iter)->path().size() == 1 ) {
			for ( int listNo = 0; listNo < 3; ++listNo ) {
				for ( std::list<CRiMacro *>::iterator markIter = lptr[listNo]->begin(); markIter != lptr[listNo]->end(); markIter++ ) {
					if ( !(*markIter) )
						continue;
					if ( (*markIter)->path().size() > 0 && (*markIter)->path()[0] == (*iter)->path()[0] ) {
						assert(*iter != curMacro());
						(*markIter)->macroType(CRiMacro::MACROTYPE_UNKNOWN); // Mark for deletion (also the root element (*iter))
					}
				}
			}
		} else if ( (*iter)->path().size() == 0 ) {
			assert(*iter != curMacro());
			(*iter)->macroType(CRiMacro::MACROTYPE_UNKNOWN); // Mark for deletion
		}
	}
}

void CRenderState::compactArchives()
{
	std::list<CRiMacro *> archiveResults;
	std::list<CRiMacro *> objectResults;
	std::list<CRiMacro *> cachedResults;
	
	// Extract in reverse order
	m_archiveMacros.extractToMark(archiveResults);
	m_objectMacros.extractToMark(objectResults);
	m_cachedMacros.extractToMark(cachedResults);

	// Mark root elements for deletion (Elements with one Path element and all successors)
	markRoot(archiveResults, objectResults, cachedResults);
	markRoot(objectResults, cachedResults, archiveResults);
	markRoot(cachedResults, archiveResults, objectResults);

	// Inserted in reversed order again, so it's the same order as before
	compactArchive(archiveResults, m_archiveMacros);
	compactArchive(objectResults, m_objectMacros);
	compactArchive(cachedResults, m_cachedMacros);	
}

void CRenderState::contextReset()
{
	// AreaLightSource declared within this block
	if ( areaLightSourceHandle() != illLightHandle &&
		areaLightSourceDepth() == modesSize() )
	{
		// close the light source
		endAreaLightSource();
	}
	
	m_modeStack->contextEnd();
	
	m_lightSourceHandles.clear();
	m_lightSources.clear();
	
	deleteTransMapCont(m_globalTransforms);
	deleteDeferedRequests();

	clearStacks();
}

void CRenderState::contextEnd()
{
	contextReset();
	m_objectMacros.clear();
	m_archiveMacros.clear();
	m_cachedMacros.clear();
}

void CRenderState::frameBegin(RtInt number)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_objectMacros.mark();
	m_archiveMacros.mark();
	m_cachedMacros.mark();
	m_lightSources.mark();
	m_lightSourceHandles.mark();

	m_modeStack->frameBegin();
	frameNumber(number);
}

void CRenderState::frameEnd()
{
	// AreaLightSource declared within this block
	if ( areaLightSourceHandle() != illLightHandle &&
	     areaLightSourceDepth() == modesSize() )
	{
		// close the light source
		endAreaLightSource();
	}

	frameNumber(0);
	m_modeStack->frameEnd();

	m_lightSourceHandles.clearToMark();
	m_lightSources.clearToMark();
	
	compactArchives();

	popTransform();
	popAttributes();
	popOptions();
}

void CRenderState::worldBegin()
{
	// Sets the viewing transformations
	setCameraToScreen();

	// Sets the camera transformations
	setWorldToCamera();

	pushTransform();
	
	curTransform().reset();
	curTransform().spaceType(RI_WORLD);

	pushAttributes();
	attributes().resetCoordSysOrientation(m_cameraOrientation);

	m_objectMacros.mark();
	m_archiveMacros.mark();
	m_cachedMacros.mark();
	m_lightSources.mark();
	m_lightSourceHandles.mark();

	m_modeStack->worldBegin();
}

void CRenderState::worldEnd()
{
	// AreaLightSource declared within this block
	if ( areaLightSourceHandle() != illLightHandle &&
	     areaLightSourceDepth() == modesSize() )
	{
		// close the light source
		endAreaLightSource();
	}

	m_modeStack->worldEnd();
	
	m_lightSourceHandles.clearToMark();
	m_lightSources.clearToMark();

	compactArchives();

	popAttributes();
	popTransform();
}

void CRenderState::attributeBegin()
{
	pushTransform();
	pushAttributes();

	m_modeStack->attributeBegin();
}

void CRenderState::attributeEnd()
{
	// AreaLightSource declared within this block
	if ( areaLightSourceHandle() != illLightHandle &&
	     areaLightSourceDepth() == modesSize() )
	{
		// close the light source
		endAreaLightSource();
	}

	m_modeStack->attributeEnd();

	popAttributes();
	popTransform();
}

void CRenderState::storeAttributes()
{
	pushAttributes(true);
	pushTransform(true);
	assert(attributes().storeCounter() == curTransform().storeCounter());
}

void CRenderState::restoreAttributes()
{
	assert(attributes().storeCounter() >= 1);
	assert(curTransform().storeCounter() >= 1);
	popAttributes(false);
	popTransform(false);
	storeAttributes();
}

void CRenderState::transformBegin()
{
	pushTransform();
	m_modeStack->transformBegin();
}

void CRenderState::transformEnd()
{
	m_modeStack->transformEnd();
	popTransform();
	if ( !m_transformationStack.empty() && !m_attributesStack.empty() )
		attributes().coordSysOrientation(curTransform().coordSysOrientation());
}

void CRenderState::solidBegin(RtToken type)
{
	m_modeStack->solidBegin();
	m_solidTypes.push_back(type);
}

void CRenderState::solidEnd()
{
	if ( !m_solidTypes.empty() )
		m_solidTypes.pop_back();
	m_modeStack->solidEnd();
}

RtToken CRenderState::solid() const
{
	if ( m_solidTypes.empty() )
		return RI_NULL;
	return m_solidTypes.back();
}


RtString CRenderState::findHandleId(CParameterList &params) const
{
	const CParameter *p = params.get(RI_HANDLEID);
	if ( p && p->basicType() == BASICTYPE_STRING )
		return p->stringPtrs()[0];
	return RI_NULL;
}

bool CRenderState::noLights() const
{
	return m_lightSourceHandles.empty();
}

RtLightHandle CRenderState::renewLightHandle(RtToken lightSourceName, RtString handlename, CParameterList &params)
{
	unsigned long num = 0;
	
	RtLightHandle l = m_lightSourceHandles.newHandle(handlename, num);
	if ( l != illLightHandle ) {
		CHandle *h = new CHandle(l, num, findHandleId(params) != RI_NULL);
		if ( !h ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in newLightHandle(): %s", noNullStr(lightSourceName));
		}
		l = m_lightSourceHandles.insertObject(h->handle(), h);
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in newLightHandle(): %s", noNullStr(lightSourceName));
	}
	return l;
}

RtLightHandle CRenderState::newLightHandle(RtToken lightSourceName, CParameterList &params)
{
	RtString handlename = findHandleId(params);
	unsigned long num = 0;
	
	RtLightHandle l = m_lightSourceHandles.newHandle(handlename, num);
	if ( l != illLightHandle ) {
		CHandle *h = new CHandle(l, num, handlename != RI_NULL);
		if ( !h ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in newLightHandle(): %s", noNullStr(lightSourceName));
		}
		l = m_lightSourceHandles.insertObject(h->handle(), h);
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in newLightHandle(): %s", noNullStr(lightSourceName));
	}
	return l;
}

CHandle *CRenderState::lightSourceHandle(RtLightHandle handle)
{
	handle = m_lightSourceHandles.identify(handle);
	if ( handle != illLightHandle )
		return m_lightSourceHandles.find(handle);
	return 0;
}

const CHandle *CRenderState::lightSourceHandle(RtLightHandle handle) const
{
	handle = m_lightSourceHandles.identify(handle);
	if ( handle != illLightHandle )
		return m_lightSourceHandles.find(handle);
	return 0;
}

CLightSource *CRenderState::lightSourceInstance(RtLightHandle handle)
{
	RtLightHandle l = m_lightSources.identify(handle);
	if ( l != illLightHandle )
		return m_lightSources.find(l);
	return 0;
}

const CLightSource *CRenderState::lightSourceInstance(RtLightHandle handle) const
{
	RtLightHandle l = m_lightSources.identify(handle);
	if ( l != illLightHandle )
		return m_lightSources.find(l);
	return 0;
}

CLightSource *CRenderState::newLightSource(
	RtLightHandle handle,
    bool isArea,
    const char *name, const CParameterList &params)
{
	CLightSource *l = new CLightSource(handle, true, !inWorldBlock(), isArea, name, params);
	if ( l ) {
		m_lightSources.insertObject(l);
	}
	return l;
}

void CRenderState::startArchiveInstance(RtString aName, RtArchiveHandle aHandle)
{
	inputState().replayArchiveBegin(notEmptyStr(aName) ? aName : aHandle, aHandle);
}

void CRenderState::endArchiveInstance()
{
	inputState().replayArchiveEnd();
}

void CRenderState::startFileInstance(RtString aName, RtArchiveHandle aHandle)
{
	inputState().replayFileBegin(notEmptyStr(aName) ? aName : aHandle, aHandle);
}

void CRenderState::endFileInstance()
{
	inputState().replayFileEnd();
}

CRiMacro *CRenderState::objectInstance(RtObjectHandle handle)
{
	return m_objectMacros.find(handle);
}

const CRiMacro *CRenderState::objectInstance(RtObjectHandle handle) const
{
	return m_objectMacros.find(handle);
}

void CRenderState::startObjectInstance(RtObjectHandle handle)
{
	inputState().replayObjectBegin(handle, handle);
}

void CRenderState::endObjectInstance()
{
	inputState().replayObjectEnd();
}

RtObjectHandle CRenderState::objectBegin(RtString aName, CRManInterfaceFactory &aFactory)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_modeStack->objectBegin();
	
	m_macros.push_back(m_curMacro);
	m_recordModes.push_back(m_recordMode);
	m_recordMode = true;
	unsigned long num;
	RtToken t = m_objectMacros.newHandle(aName, num);
	CRiMacro *m = new CRiMacro(t, num, notEmptyStr(aName), &aFactory, CRiMacro::MACROTYPE_OBJECT);
	m_curMacro = m;

	inputState().objectBegin(notEmptyStr(aName) ? aName : t, t);

	if ( executeConditionial() || m_curMacro != 0 ) {
		if ( m != 0 ) {
			m->path() = inputState().path();
			m_objectMacros.insertObject(m->handle(), m);
			return m->handle();
		} else {
			throw ExceptRiCPPError(
				RIE_NOMEM, RIE_SEVERE,
				printLineNo(__LINE__), printName(__FILE__),
				"in objectBegin(%s): %s", aName, "CRiMacro");
			return illObjectHandle;
		}
	}
	
	return illObjectHandle;
}

void CRenderState::objectEnd()
{
	m_modeStack->objectEnd();

	popTransform();
	popAttributes();
	popOptions();

	assert (!m_recordModes.empty());
	if ( !m_recordModes.empty() ) {
		m_recordMode = m_recordModes.back();
		m_recordModes.pop_back();
	} else {
		m_recordMode = false;
	}

	if ( executeConditionial()  || m_curMacro != 0 ) {
		if ( m_curMacro != 0 )
			m_curMacro->close();
		if ( !m_macros.empty() ) {
			m_curMacro = m_macros.back();
			m_macros.pop_back();
		} else {
			m_curMacro = 0;
		}
	}
	inputState().objectEnd();
}

CRiMacro *CRenderState::findArchiveInstance(RtArchiveHandle handle)
{
	CRiMacro *m = m_archiveMacros.find(handle);
	if ( m ) return m;
	return m_cachedMacros.find(handle);
}

const CRiMacro *CRenderState::findArchiveInstance(RtArchiveHandle handle) const
{
	const CRiMacro *m = m_archiveMacros.find(handle);
	if ( m ) return m;
	return m_cachedMacros.find(handle);
}

RtArchiveHandle CRenderState::archiveBegin(const char *aName, CRManInterfaceFactory &aFactory)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_modeStack->archiveBegin();
	
	if ( executeConditionial() || m_curMacro != 0 ) {
		m_macros.push_back(m_curMacro);
		m_recordModes.push_back(m_recordMode);
		m_recordMode = true;
		unsigned long num;
		RtToken t = m_archiveMacros.newHandle(aName, num);
		CRiMacro *m = new CRiMacro(t, num, notEmptyStr(aName), &aFactory, CRiMacro::MACROTYPE_ARCHIVE);
		m_curMacro = m;

		inputState().archiveBegin(notEmptyStr(aName) ? aName : t, t);

		if ( m != 0 ) {
			m->path() = inputState().path();
			m_archiveMacros.insertObject(m->handle(), m);
			return m->handle();
		} else {
			throw ExceptRiCPPError(
				RIE_NOMEM, RIE_SEVERE,
				printLineNo(__LINE__), printName(__FILE__),
				"in archiveBegin(%s): %s", aName, "CRiMacro");
			return illArchiveHandle;
		}
	}

	return illArchiveHandle;
}

void CRenderState::archiveEnd()
{
	m_modeStack->archiveEnd();

	popTransform();
	popAttributes();
	popOptions();

	assert (!m_recordModes.empty());
	if ( !m_recordModes.empty() ) {
		m_recordMode = m_recordModes.back();
		m_recordModes.pop_back();
	} else {
		m_recordMode = false;
	}

	if ( executeConditionial() || m_curMacro != 0 ) {
		if ( m_curMacro != 0 )
			m_curMacro->close();
		if ( !m_macros.empty() ) {
			m_curMacro = m_macros.back();
			m_macros.pop_back();
		} else {
			m_curMacro = 0;
		}
	}
	inputState().archiveEnd();
}

RtArchiveHandle CRenderState::archiveFileBegin(const char *aName, CRManInterfaceFactory &aFactory)
{
	if ( executeConditionial() || m_curMacro != 0 ) {
		m_macros.push_back(m_curMacro);
		unsigned long num;
		RtToken t = m_cachedMacros.newHandle(aName, num);
		CRiMacro *m = new CRiMacro(t, num, notEmptyStr(aName), &aFactory, CRiMacro::MACROTYPE_FILE);
		m_curMacro = m;
		
		inputState().cacheFileBegin(notEmptyStr(aName) ? aName : t, t);
		// Does not influence nesting, because called within a IRi::readArchiveV()

		if ( m != 0 ) {
			m->path() = inputState().path();
			m_cachedMacros.insertObject(m->handle(), m);
			return m->handle();
		} else {
			throw ExceptRiCPPError(
				RIE_NOMEM, RIE_SEVERE,
				printLineNo(__LINE__), printName(__FILE__),
				"in archiveFileBegin(%s): %s", aName, "CRiMacro");
			return illArchiveHandle;
		}
	}

	return illArchiveHandle;
}

void CRenderState::archiveFileEnd()
{
    // Does not influence nesting
	if ( executeConditionial() || m_curMacro != 0 ) {
		if ( m_curMacro != 0 )
			m_curMacro->close();
		if ( !m_macros.empty() ) {
			m_curMacro = m_macros.back();
			m_macros.pop_back();
		} else {
			m_curMacro = 0;
		}
	}
	inputState().cacheFileEnd();
}

void CRenderState::pushConditional()
{
	try {
		m_conditions.push_back(m_executeConditional);
		m_conditions.push_back(m_ifCondition);
		m_accumulateConditional = m_accumulateConditional && m_executeConditional;
		m_conditions.push_back(m_accumulateConditional);
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in pushConditional(): %s", e.what());
	}
}

void CRenderState::popConditional()
{
	try {
		m_accumulateConditional = m_conditions.back();
		m_conditions.pop_back();

		m_ifCondition = m_conditions.back();
		m_conditions.pop_back();

		m_executeConditional = m_conditions.back();
		m_conditions.pop_back();
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NESTING, RIE_SEVERE, printLineNo(__LINE__), printName(__FILE__), "in popConditional(): %s", e.what());
	}
}

bool CRenderState::devideName(RtString identifier, RtToken *aQualifier, RtToken *tablename, RtToken *varname) const
{
	*aQualifier=0;
	*tablename=0;
	*varname=0;

	if ( emptyStr(identifier) )
		return false;

	CStringList sl(':', identifier, false);
	std::string result;

	if ( sl.empty() )
		return false;

	if ( sl.size() > 3 ) {
		*varname = tokFind(identifier);
		return *varname != RI_NULL;
	}

	*varname = sl.back().c_str();
	*varname = tokFind(*varname);
	if ( *varname == RI_NULL ) {
		*varname = tokFind(identifier);
		return *varname != RI_NULL;
	}

	sl.pop_back();

	if ( !sl.empty() ) {
		*tablename = sl.back().c_str();
		*tablename = tokFind(*tablename);
		sl.pop_back();
	}

	if ( !sl.empty() ) {
		*aQualifier = sl.back().c_str();
		*aQualifier = tokFind(*aQualifier);
		sl.pop_back();
	}

	return true;
}

bool CRenderState::exists(RtString identifier) const
{
	CValue p;
	return getValue(p, identifier);
}

bool CRenderState::getAttribute(CValue &p, RtToken tablename, RtToken varname) const
{
	const CParameter *param = attributes().get(tablename, varname);
	return param != 0 ? param->get(0, p) : false;
}

bool CRenderState::getOption(CValue &p, RtToken tablename, RtToken varname) const
{
	const CParameter *param = options().get(tablename, varname);
	return param != 0 ? param->get(0, p) : false;
}

bool CRenderState::getControl(CValue &p, RtToken tablename, RtToken varname) const
{
	const CParameter *param = m_controls.get(tablename, varname);
	return param != 0 ? param->get(0, p) : false;
}

bool CRenderState::getValue(CValue &p, RtString identifier) const
{
	if ( emptyStr(identifier) )
		return false;

	RtToken aQualifier, tablename, varname;
	if ( devideName(identifier, &aQualifier, &tablename, &varname) ) {
		// frame number?
		if ( !aQualifier && !tablename )
		{
			if ( varname == RI_FRAME ) {
				p.set(frameNumber());
				return true;
			}
		}
		
		// Try attribute first, then option
		if ( !aQualifier ) {
			if ( getAttribute(p, tablename, varname) )
				return true;
			if ( getOption(p, tablename, varname) )
				return true;
		} else {
			if ( aQualifier == RI_ATTRIBUTE )
				return getAttribute(p, tablename, varname);
			if ( aQualifier == RI_OPTION )
				return getOption(p, tablename, varname);
		}
	}

	return false;
}

std::string &CRenderState::varSubst(std::string &aStr, char varId, RtString stdPath, RtString curPath) const
{
	return RiCPP::varSubst(aStr, varId, this, stdPath, curPath);
}

std::string &CRenderState::varSubst(std::string &aStr, RtString stdPath, RtString curPath) const
{
	/** @todo call varSubst(std::string aStr, char varId) if option "rib" "string varsubst" is set.
	 */
	const CParameter *p;
	if ( (p=options().get(RI_RIB, RI_VARSUBST)) ) {
		if ( p->basicType() == BASICTYPE_STRING && !p->strings()[0].empty() ) {
			return varSubst(aStr, (p->strings()[0])[0], stdPath, curPath);
		}
	}
	return varSubst(aStr, 0, stdPath, curPath); // maybe stdPath, curPath have to be inserted (not done via parsing)
}

bool CRenderState::eval(RtString expr) const
{
	CIfExprParser parser(*this);
	return parser.parse(expr);
}


void CRenderState::ifBegin(RtString expr) {
	m_modeStack->ifBegin();

	pushConditional();
	m_ifCondition = eval(expr);
	m_executeConditional = m_ifCondition;
	if ( !m_executeConditional ) {
		pushAttributes();
		pushOptions();
	}
}

void CRenderState::elseIfBegin(RtString expr) {
	m_modeStack->elseIfBegin();
	if ( !m_executeConditional ) {
		popAttributes();
		popOptions();
	}
	if ( !m_ifCondition ) {
		m_executeConditional = eval(expr);
		m_ifCondition = m_executeConditional;
	} else {
		m_executeConditional = false;
	}
	if ( !m_executeConditional ) {
		pushAttributes();
		pushOptions();
	}
}

void CRenderState::elseBegin()
{
	m_modeStack->elseBegin();
	if ( !m_executeConditional ) {
		popAttributes();
		popOptions();
	}
	m_executeConditional = !m_ifCondition;
	if ( !m_executeConditional ) {
		pushAttributes();
		pushOptions();
	}
}

void CRenderState::ifEnd()
{
	m_modeStack->ifEnd();

	if ( !m_executeConditional ) {
		popAttributes();
		popOptions();
	}
	popConditional();
}

void CRenderState::resourceBegin()
{
	m_modeStack->resourceBegin();
	m_resourceStack.mark();
}

void CRenderState::resourceEnd()
{
	m_modeStack->resourceEnd();
	m_resourceStack.clearToMark();
}

RtToken CRenderState::resource(IRiContext &ri, RtToken handle, RtToken type, const CParameterList &params)
{
	IResourceFactory *f = m_resourceFactories.findObj(type);
	if ( !f ) {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "no resource factory in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
	}

	CResource *r = 0;
	if ( f && f->overwrites(params) ) {
		r = m_resourceStack.find(m_resourceStack.identify(handle), true);
		if ( !r ) {
			unsigned long num;
			RtToken t = m_resourceStack.newHandle(handle, num);
			r = f->getResource(t, num, notEmptyStr(handle));
			if ( !r ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "creation of resource failed in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
			}
			m_resourceStack.insertObject(r->handle(), r);
		}
	} else {
		r = m_resourceStack.find(m_resourceStack.identify(handle), false);
		if ( !r ) {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "no valid resource handle found in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
		}
	}
	r->operate(ri, params);
	return r->handle();
}

void CRenderState::motionBegin(RtInt N, RtFloat times[])
{
	m_modeStack->motionBegin();
	m_motionState.motionBegin(N, times);
	attributes().motionBegin(m_motionState);
	curTransform().motionBegin(m_motionState);
}

void CRenderState::motionEnd()
{
	bool err = false;
	if ( m_motionState.curSampleIdx() < ((RtInt)m_motionState.curTimes().size()) - 1 ) {
		err = true;
	}
	if ( m_motionState.attributesStored() ) {
		// get Attributes()
		assert(!m_motionAttributesStack.empty() && m_motionAttributesStack.back() != 0);
		assert(!m_motionTransformationStack.empty() && m_motionTransformationStack.back() != 0);

		CTransformation *trans = m_motionTransformationStack.back();
		m_motionTransformationStack.pop_back();
		m_transformationFactory->deleteTransformation(m_transformationStack.back());
		m_transformationStack.pop_back();
		m_transformationStack.push_back(trans);

		CAttributes *attr = m_motionAttributesStack.back();
		m_motionAttributesStack.pop_back();
		m_attributesFactory->deleteAttributes(m_attributesStack.back());
		m_attributesStack.pop_back();
		m_attributesStack.push_back(attr);
	}

	curTransform().motionEnd();
	attributes().motionEnd();
	m_motionState.motionEnd();
	m_modeStack->motionEnd();

	if ( err ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_WARNING, "in motionEnd(), too few requests in motion block.", __LINE__, __FILE__);
	}
}

void CRenderState::moveArchiveBegin()
{
	if ( m_motionState.curState() == CMotionState::MOT_OUTSIDE )
		return;

	if ( m_motionState.curSampleIdx() >= 0 ) {
		// Start and continue
		/// @todo sample the current move values - can be done on the fly in attributes()
		attributeBegin();
	} else {
		// Error
	}
}

void CRenderState::moveArchiveEnd()
{
	if ( m_motionState.curState() == CMotionState::MOT_OUTSIDE )
		return;

	if ( m_motionState.curSampleIdx() == 0 ) {
		// Start

		// Store Attibutes
		assert(!m_attributesStack.empty() && m_attributesStack.back() != 0);
		m_motionAttributesStack.push_back(m_attributesFactory->newAttributes(*m_attributesStack.back()));
		m_motionTransformationStack.push_back(m_transformationFactory->newTransformation(*m_transformationStack.back()));
		m_motionState.attributesStored(true);
		attributeEnd();
		/// @todo restore the move samples
	} else if ( m_motionState.curSampleIdx() > 0 ) {
		// Continue
		attributeEnd();
		/// @todo restore the move samples
	} else {
		// Error
	}
}


void CRenderState::pushOptions()
{
	try {

		if ( m_optionsStack.empty() ) {
			m_optionsStack.push_back(m_optionsFactory->newOptions());
		} else {
			assert(m_optionsStack.back() != 0);
			m_optionsStack.push_back(m_optionsFactory->newOptions(*m_optionsStack.back()));
		}
		if ( m_optionsStack.back() == 0 ) {
			m_optionsStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushOptions()", __LINE__, __FILE__);
		}

	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushOptions(): %s", e.what());
	}
}

bool CRenderState::popOptions()
{
	assert(!m_optionsStack.empty());
	if ( !m_optionsStack.empty() ) {
		m_optionsFactory->deleteOptions(m_optionsStack.back());
		m_optionsStack.pop_back();
	}
	return !m_optionsStack.empty();
}

void CRenderState::pushAttributes(bool useCounter)
{
	unsigned long cnt = 1;
	if ( !useCounter ) {
		// A new attribute, frame, or worldblock is started
		m_motionState.mark();
	}
	try {
		TypeTransformationMap tm;
		m_scopedTransforms.push_back(tm);

		if ( m_attributesStack.empty() ) {
			m_attributesStack.push_back(m_attributesFactory->newAttributes(options().colorDescr()));
		} else {
			assert(m_attributesStack.back() != 0);
			if ( useCounter )
				cnt = attributes().storeCounter() + 1;
			m_attributesStack.push_back(m_attributesFactory->newAttributes(*m_attributesStack.back()));
		}
		if ( m_attributesStack.back() == 0 ) {
			m_attributesStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushAttributes()", __LINE__, __FILE__);
		}

		attributes().storeCounter(cnt);
		if ( !useCounter ) {
			// A new attribute, frame, or worldblock is started
			attributes().clearDetailRangeCalledInBlock();
		}
		attributes().motionSuspend();

	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushAttributes(): %s", e.what());
	}
}

bool CRenderState::popAttributes(bool useCounter)
{
	unsigned long cnt = 1;
	assert(!m_attributesStack.empty());

	if ( !useCounter ) {
		// A new attribute, frame, or worldblock is started
		m_motionState.clearToMark();
	}

	if ( !m_attributesStack.empty() ) {
		if ( useCounter ) {
			cnt = attributes().storeCounter();
		}
		
		while ( cnt > 0 && !m_attributesStack.empty() ) {
			assert(!useCounter || (attributes().storeCounter() == cnt));
			--cnt;
			m_attributesFactory->deleteAttributes(m_attributesStack.back());
			m_attributesStack.pop_back();
			assert(!m_scopedTransforms.empty());
			if ( !m_scopedTransforms.empty() ) {
				TypeTransformationMap &m = m_scopedTransforms.back();
				deleteTransMapCont(m);
				m_scopedTransforms.pop_back();
			}
		}
	}
	return !m_attributesStack.empty();
}

void CRenderState::pushTransform(bool useCounter)
{
	unsigned long cnt = 1;
	try {
		if ( m_transformationStack.empty() ) {
			m_transformationStack.push_back(m_transformationFactory->newTransformation());
			if ( !m_transformationStack.empty() )
				curTransform().spaceType(RI_CAMERA);
		} else {
			assert(m_transformationStack.back() != 0);
			if ( useCounter )
				cnt = curTransform().storeCounter() + 1;
			m_transformationStack.push_back(m_transformationFactory->newTransformation(*m_transformationStack.back()));
		}
		if ( m_transformationStack.back() == 0 ) {
			m_transformationStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushTransform()", __LINE__, __FILE__);
		}
		curTransform().storeCounter(cnt);
		curTransform().motionSuspend();
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushTransform(): %s", e.what());
	}
}

bool CRenderState::popTransform(bool useCounter)
{
	unsigned long cnt = 1;
	assert(!m_transformationStack.empty());
	if ( !m_transformationStack.empty() ) {
		if ( useCounter ) {
			cnt = curTransform().storeCounter();
		}
		while ( cnt > 0 && !m_transformationStack.empty() ) {
			assert(!useCounter || (curTransform().storeCounter() == cnt));
			--cnt;
			m_transformationFactory->deleteTransformation(m_transformationStack.back());
			m_transformationStack.pop_back();
		}
	}
	return !m_transformationStack.empty();
}

const CTransformation *CRenderState::findTransform(RtToken space) const
{
	TypeTransformationMap::const_iterator pos;

	if ( space == RI_CURRENT ) // Current to containing space
		return &curTransform();

	if ( space == RI_WORLD ) // Current to world (defined in world blocks)
		return &curTransform();

	if ( space == RI_CAMERA )  // World to camera
		return m_worldToCamera;

	if ( space == RI_SCREEN )  // Camera to screen
		return m_cameraToScreen;

	if ( space == RI_NDC )   // Screen to NDC
		return m_screenToNDC;

	if ( space == RI_RASTER ) // NDC to raster
		return m_NDCToRaster;

	if ( !m_scopedTransforms.empty() ) {
		for( std::list<TypeTransformationMap>::const_reverse_iterator i = m_scopedTransforms.rbegin();
		     i != m_scopedTransforms.rend();
			 i++ )
		{
			pos = (*i).find(space);
			if ( pos != (*i).end() ) {
				return (*pos).second;
			}
		}
	}

	pos = m_globalTransforms.find(space);
	if ( pos != m_globalTransforms.end() ) {
		return (*pos).second;
	}
	return 0;
}

RtPoint *CRenderState::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) const
{
#ifdef _TRACE_TRANS_PTS
	std::cout << ">transformPoints from:" << fromspace << " to: " << tospace << " n: " << npoints << std::endl;
#endif // _TRACE_TRANS_PTS

	// No points to transform
	if ( npoints <= 0 || points == 0 ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "<transformPoints no points" << std::endl;
#endif // _TRACE_TRANS_PTS
		return points ? &points[0] : 0;
	}
	
	const CTransformation *from = findTransform(fromspace); // From this space
	const CTransformation *to   = findTransform(tospace);   // to that space
	
	if ( !from ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "<transformPoints no from" << std::endl;
#endif // _TRACE_TRANS_PTS
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints - from=='%s' space not found", noNullStr(fromspace));
	}

	if ( !to ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "<transformPoints no to" << std::endl;
#endif // _TRACE_TRANS_PTS
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints - to=='%s' space not found", noNullStr(tospace));
	}

	if ( from == to ) {
		// Same space, no transformation needed
#ifdef _TRACE_TRANS_PTS
		std::cout << "<transformPoints no transformation needed" << std::endl;
#endif // _TRACE_TRANS_PTS
		return &points[0];
	}

	// Main containing spaces of the RI (object space lies in world space)
	static RtToken spaces[] = {RI_WORLD, RI_CAMERA, RI_SCREEN, RI_NDC, RI_RASTER, RI_NULL};

	// Containing main space type (must be found in spaces)
	RtToken fs = from->spaceType(), ts = to->spaceType();
	/*
	if ( fs == RI_CURRENT ) {
		fs = curTransform().spaceType();
	}
	if ( ts == RI_CURRENT ) {
		ts = curTransform().spaceType();
	}
	*/
	
#ifdef _TRACE_TRANS_PTS
	std::cout << "-transformPoints fromSpaceType: " << fs << " toSpaceType: " << ts << std::endl;
#endif // _TRACE_TRANS_PTS

	int fi = 0;
	while ( spaces[fi] && spaces[fi] != fs ) ++fi;
	int ti = 0;
	while ( spaces[ti] && spaces[ti] != ts ) ++ti;
	
	if ( !spaces[fi] || !spaces[ti] ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "<transformPoints no containing main spaces found" << std::endl;
#endif // _TRACE_TRANS_PTS
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints(%s (%s found), %s (%s found)), spaces not found", noNullStr(fromspace), spaces[fi] ? "was" : "not", noNullStr(tospace), spaces[ti] ? "was" : "not");
	}
	
	CMatrix3D m;
	m.setPreMultiply(false);
	
	if ( fromspace != fs ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "-transformPoints concat matrix from: " << fromspace << " to: "<< fs << std::endl;
#endif // _TRACE_TRANS_PTS
		m.concatTransform(from->getCTM());
	}
	
	// Transform to the main space of the destination
	if ( fi < ti ) {
		// Direction to raster
		const CTransformation *trans;
		while ( fi < ti ) {
			++fi;
			trans = findTransform(spaces[fi]);
			assert(trans!=0);
			if ( trans ) {
#ifdef _TRACE_TRANS_PTS
				std::cout << "-transformPoints concat matrix " << spaces[fi] << std::endl;
#endif // _TRACE_TRANS_PTS
				m.concatTransform(trans->getCTM());
			} else {
#ifdef _TRACE_TRANS_PTS
				std::cout << "<transformPoints transformation order wrong" << std::endl;
#endif // _TRACE_TRANS_PTS
				throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints: space (%s) not defined, to raster direction", noNullStr(spaces[ti]));
			}
		}
	} else if ( fi > ti ) {
		// Direction to object
		const CTransformation *trans;
		while ( fi > ti ) {
			trans = findTransform(spaces[fi]);
			assert(trans!=0);
			if ( trans ) {
#ifdef _TRACE_TRANS_PTS
				std::cout << "-transformPoints concat inverse matrix " << spaces[fi] << std::endl;
#endif // _TRACE_TRANS_PTS
				m.concatTransform(trans->getInverseCTM());
			} else {
#ifdef _TRACE_TRANS_PTS
				std::cout << "<transformPoints transformation order wrong" << std::endl;
#endif // _TRACE_TRANS_PTS
				throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints: space (%s) not defined, to object direction", noNullStr(spaces[ti]));
			}
			--fi;
		}
	}
	
	if ( tospace != ts ) {
#ifdef _TRACE_TRANS_PTS
		std::cout << "-transformPoints concat inverse matrix " << tospace << std::endl;
#endif // _TRACE_TRANS_PTS
		m.concatTransform(to->getInverseCTM());
	}
	
	m.setPreMultiply(true);
	m.transformPoints(npoints, points);

#ifdef _TRACE_TRANS_PTS
	std::cout << "<transformPoints transformation done" << std::endl;
#endif // _TRACE_TRANS_PTS
	return &points[0];
}

void CRenderState::coordSysTransform(RtToken space)
{
	if ( space == RI_CURRENT )
		return;
	
	RtToken curSpaceType = curTransform().spaceType();
	if ( curSpaceType == space ) {
		curTransform().transform(RiIdentityMatrix, RiIdentityMatrix);
		return;
	}

	const CTransformation *newTransform = findTransform(space);
	
	if ( newTransform ) {
		RtToken newSpaceType = newTransform->spaceType();
		
		if ( curSpaceType != newSpaceType ) {
			// Main containing spaces of the RI
			static RtToken spaces[] = {RI_WORLD, RI_CAMERA, RI_SCREEN, RI_NDC, RI_RASTER, RI_NULL};
			int fi = 0;
			while ( spaces[fi] && spaces[fi] != newSpaceType ) ++fi;
			int ti = 0;
			while ( spaces[ti] && spaces[ti] != curSpaceType ) ++ti;
			
			// set ctm = t x t->spaceType_To_currentSpacetype
			CMatrix3D m, mi;
			m.setPreMultiply(true);
			mi.setPreMultiply(false);

			// pre concat tspace to cur
			// Transform to the main space of the destination
			if ( fi < ti ) {
				// Direction to raster (not happen)
				assert(false);
				const CTransformation *trans;
				while ( fi < ti ) {
					trans = findTransform(spaces[ti]);
					assert(trans!=0);
					if ( trans ) {
						m.concatTransform(trans->getCTM());
						mi.concatTransform(trans->getInverseCTM());
					} else {
						throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "coordSysTransform: space (%s) not defined, to raster direction", noNullStr(spaces[ti]));
					}
					--ti;
				}
			} else {
				// Direction back to world
				const CTransformation *trans;
				while ( fi > ti ) {
					trans = findTransform(spaces[ti]);
					assert(trans!=0);
					if ( trans ) {
						m.concatTransform(trans->getInverseCTM());
						mi.concatTransform(trans->getCTM());
					} else {
						throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "coordSysTransform: space (%s) not defined, to object direction", noNullStr(spaces[ti]));
					}
					ti++;
				}
			}
			if ( newSpaceType != space ) {
				m.concatTransform(newTransform->getCTM());
				mi.concatTransform(newTransform->getInverseCTM());
			}
			// Set new Transformation
			curTransform().transform(newTransform->getCTM(), newTransform->getInverseCTM());
		} else {
			curTransform().transform(newTransform->getCTM(), newTransform->getInverseCTM());
		}
	}
}

void CRenderState::startAreaLightSource(RtLightHandle h)
{
	try {
		attributes().inAreaLight(true);
		m_modeStack->startAreaLightSource(h);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::endAreaLightSource()
{
	try {
		attributes().inAreaLight(false);
		m_modeStack->endAreaLightSource();
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}


void CRenderState::parseParameters(CParameterList &p, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		p.set(0, 0, counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::parseParameters(const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		m_curParams.set(0, 0, counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::parseParameters(CParameterList &p, const char *aQualifier, const char *aTable, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		p.set(aQualifier, aTable, counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

void CRenderState::parseParameters(const char *aQualifier, const char *aTable, const CValueCounts &counts, RtInt n, RtToken theTokens[], RtPointer theParams[])
{
	try {
		m_curParams.set(aQualifier, aTable, counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

RtArchiveHandle CRenderState::storedArchiveName(RtString archiveName) const
{
	RtArchiveHandle handle = m_archiveMacros.identify(archiveName);
	if ( handle ) return handle;
	return m_cachedMacros.identify(archiveName);
}

void CRenderState::registerResourceFactory(IResourceFactory *f)
{
	if ( !f )
		return;
	RtToken t = tokFindCreate(f->type());
	f->registerOperations(tokenMap());
	m_resourceFactories.registerObj(t, f);
}

const IFilterFunc *CRenderState::filterFunc(RtToken name) const
{
	return m_filterFuncFactory->singleton(name);
}

void CRenderState::defaultDeclarations()
{
	// rib control and options
	RI_RIB = tokFindCreate("rib");
	RI_CACHE_FILE_ARCHIVES = tokFindCreate("cache-file-archives");
	RI_QUAL_CACHE_FILE_ARCHIVES = declare("Control:rib:cache-file-archives", "constant integer", true);
	RI_VARSUBST = tokFindCreate("varsubst");
	RI_QUAL_VARSUBST = declare("Option:rib:varsubst", "string", true);

	// state control
	RI_STATE = tokFindCreate("state");
	RI_STORE_TRANSFORM = tokFindCreate("store-transform");
	RI_QUAL_STORE_TRANSFORM = declare("Control:state:store-transform", "constant string", true);
	RI_LOAD_TRANSFORM = tokFindCreate("load-transform");
	RI_QUAL_LOAD_TRANSFORM = declare("Control:state:load-transform", "constant string", true);
	RI_PRE_CAMERA = tokFindCreate("pre-camera");
}

RtToken CRenderState::declare(RtToken name, RtString declaration, bool isDefault)
{
	return dict().declare(name, declaration, isDefault, options().colorDescr());
}

RtVoid CRenderState::control(RtToken name, const CParameterList &params)
{
	controls().set(name, params);
	if ( name == RI_RIB ) {
		CParameterList::const_iterator i;
		for ( i = params.begin(); i != params.end(); i++ ) {
			if ( (*i).matches(QUALIFIER_CONTROL, RI_RIB, RI_CACHE_FILE_ARCHIVES) ) {
				RtInt intVal;
				(*i).get(0, intVal);
				m_cacheFileArchives = intVal != 0;
			}
		}
	} else if ( name == RI_STATE ) {
		CParameterList::const_iterator i;
		for ( i = params.begin(); i != params.end(); i++ ) {
			if ( (*i).matches(QUALIFIER_CONTROL, RI_STATE, RI_STORE_TRANSFORM) ) {
				std::string strVal;
				(*i).get(0, strVal);
				if ( strVal == std::string(RI_PRE_CAMERA) )
					m_preCamera = curTransform().getCTM();
			}
			if ( (*i).matches(QUALIFIER_CONTROL, RI_STATE, RI_LOAD_TRANSFORM) ) {
				std::string strVal;
				(*i).get(0, strVal);
				if ( strVal == std::string(RI_PRE_CAMERA) )
					curTransform().concatTransform(m_preCamera);
			}
		}
	}
}


RtVoid CRenderState::option(RtToken name, const CParameterList &params)
{
	options().set(name, params);
}


RtVoid CRenderState::attribute(RtToken name, const CParameterList &params)
{
	attributes().set(name, params);
}
