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
#endif _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

#ifndef _RICPP_TOOLS_FILEPATH_H
// #include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;

#ifdef _DEBUG
#define _TRACE
#endif

/*
unsigned char CRenderState::character(
	const unsigned char **str,
	std::string &result) const
{
	std::string dummy;

	if ( match("\\", str, dummy) ) {
		unsigned char d;
		unsigned char uchar;
		if ( octdig(str, dummy, d) ) {
			uchar = d;
			if ( octdig(str, dummy, d) ) {
				uchar *= 8;
				uchar += d;
			}
			if ( octdig(str, dummy, d) ) {
				uchar *= 8;
				uchar += d;
			}
			result += uchar;
			return true;
		}

		unsigned char c = matchOneOf("\\'\"nrtbf\n", str, dummy);
		if ( c != 0 ) {
			switch ( c ) {
				case '\\' :
					result += "\\";
					break;
				case '\'' :
					result += "'";
					break;
				case '\"' :
					result += "\"";
					break;
				case 'n' :
					result += "\n";
					break;
				case 'r' :
					result += "\r";
					break;
				case 't' :
					result += "\t";
					break;
				case 'b' :
					result += "\b";
					break;
				case 'f' :
					result += "\f";
					break;
				case '\n' : // ignore
					break;
			}
			return true;
		}
		result += '\\';
		return true;
	}

	unsigned char c = la(str);
	if ( c != '\0' && c != '\'' ) {
		advance(str, result);
		return true;
	}

	return false;
}
*/

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
			RtFloat fval;
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


CRenderState::CRenderState(
	CModeStack &aModeStack,
	COptionsFactory &optionsFactory,
	CAttributesFactory &attributesFactory,
	CTransformationFactory &transformationFactory,
	CFilterFuncFactory &filterFuncFactory
	) :
	m_resourceFactories(false),
	m_resourceStack("RES_"),
	m_objectMacros("OBJ_"),
	m_archiveMacros("ARC_"),
	m_cachedArchive("CACHE_"),
	m_lightSourceHandles("LIH_"),
	m_lightSources("LIH_")
{
	m_modeStack = &aModeStack;
	m_optionsFactory = &optionsFactory;
	m_attributesFactory = &attributesFactory;
	m_transformationFactory = &transformationFactory;
	m_filterFuncFactory = &filterFuncFactory;
	m_frameNumber = 0;
	m_lineNo = -1;

	RI_RIB = RI_NULL;
	RI_CACHE_FILE_ARCHIVES = RI_NULL;
	RI_VARSUBST = RI_NULL;
	RI_QUAL_CACHE_FILE_ARCHIVES = RI_NULL;
	RI_QUAL_VARSUBST = RI_NULL;

	m_curMacro = 0;
	m_curReplay = 0;
	m_cacheFileArchives = false;

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

	// Create options, attributes and transforms
	pushOptions();
	pushAttributes();
	pushTransform();
}

CRenderState::~CRenderState()
{
	deleteTransMapCont(m_globalTransforms);
	while ( !m_optionsStack.empty() ) popOptions();
	while ( !m_attributesStack.empty() ) popAttributes();
	while ( !m_motionAttributesStack.empty() ) {
		CAttributes *a = m_motionAttributesStack.back();
		m_attributesFactory->deleteAttributes(a);
		m_motionAttributesStack.pop_back();
	}
	while ( !m_transformationStack.empty() ) popTransform();
	while ( !m_motionTransformationStack.empty() ) {
		CTransformation *trans = m_motionTransformationStack.back();
		m_transformationFactory->deleteTransformation(trans);
		m_motionTransformationStack.pop_back();
	}

	m_transformationFactory->deleteTransformation(m_NDCToRaster);
	m_transformationFactory->deleteTransformation(m_screenToNDC);
	m_transformationFactory->deleteTransformation(m_cameraToScreen);
	m_transformationFactory->deleteTransformation(m_worldToCamera);
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

		RtInt xres, yres, xresTemp, yresTemp;
		RtFloat pa;
		options().getFormat(xres, yres, pa);

		// Frameaspect (horizontal/vertical) is used to calc the used size 
		RtFloat fAspect = options().frameAspectRatio();
		
		if ( pa > 1 ) {
			xresTemp = fAspect * yres;
			yresTemp = yres / pa;
		} else {
			xresTemp = fAspect * yres * pa;
			yresTemp = yres;
		}
		
		if ( xresTemp > xres ) {
			if ( pa > 1 ) {
				xresTemp = xres;
				yresTemp = invert(fAspect / xres) / pa;
			} else {
				xresTemp = xres * pa;
				yresTemp = invert(fAspect / xres);
			}
		}
		
		assert(xresTemp <= xres && yresTemp <= yres);
		
		m_NDCToRaster->identity();
		m_NDCToRaster->scale((RtFloat)xresTemp, (RtFloat)yresTemp, 1);
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
		
		// scale screenwindow and mirror on x
		RtFloat swid = options().screenWindowRight()-options().screenWindowLeft();
		RtFloat sht  = options().screenWindowTop()-options().screenWindowBottom();
		if ( swid > 0 && sht > 0 ) {
			m_screenToNDC->scale((RtFloat)1.0/swid, (RtFloat)-1.0/sht, 1);
		}
		// Translate to screen origin
		m_screenToNDC->translate(-options().screenWindowLeft(), -options().screenWindowTop(), 0);

		// Screen

#       ifdef _TRACE
		std::cout << "ScreenToNDC " << std::endl;
		printMatrix(m_screenToNDC->getCTM().getMatrix());
#       endif
	}

	calcNDCToRaster();
}

void CRenderState::clearCameraToScreen()
{
	m_transformationFactory->deleteTransformation(m_cameraToScreen);
	m_cameraToScreen = 0;
}

void CRenderState::setCameraToScreen()
{
	m_cameraToScreen = m_transformationFactory->newTransformation();
	if ( !m_cameraToScreen ) {
		// error
	} else {
		m_cameraToScreen->spaceType(RI_SCREEN);
		if ( options().preProjectionMatrix() ) {
			*m_cameraToScreen = *(options().preProjectionMatrix());
		} else {
			m_cameraToScreen->identity();
		}
	}
	
	calcScreenToNDC();
}

void CRenderState::setWorldToCamera()
{
	m_transformationFactory->deleteTransformation(m_worldToCamera);
	m_worldToCamera = curTransform().duplicate();
	if ( m_worldToCamera )
		m_worldToCamera->spaceType(RI_CAMERA);
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
	m_lightSources.mark();
	m_lightSourceHandles.mark();

	m_modeStack->contextBegin();
}

void CRenderState::contextEnd()
{
	// AreaLightSource declared within this block
	if ( areaLightSourceHandle() != illLightHandle &&
	     areaLightSourceDepth() == modesSize() )
	{
		// close the light source
		endAreaLightSource();
	}

	m_modeStack->contextEnd();

	m_lightSourceHandles.clearToMark();
	m_lightSources.clearToMark();
	m_archiveMacros.clearToMark();
	m_objectMacros.clearToMark();

	popTransform();
	popAttributes();
	popOptions();
}

void CRenderState::frameBegin(RtInt number)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_objectMacros.mark();
	m_archiveMacros.mark();
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
	m_archiveMacros.clearToMark();
	m_objectMacros.clearToMark();

	popTransform();
	popAttributes();
	popOptions();
}

void CRenderState::worldBegin()
{
	setWorldToCamera();

	pushTransform();
	curTransform().reset();

	// Set camera to screen matrix if not already done
	// if ( cameraToScreen() == 0 ) {
		setCameraToScreen();
	// }
	
	curTransform().spaceType(RI_WORLD);

	pushAttributes();

	m_objectMacros.mark();
	m_archiveMacros.mark();
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
	m_archiveMacros.clearToMark();
	m_objectMacros.clearToMark();

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

CRiObjectMacro *CRenderState::objectInstance(RtObjectHandle handle)
{
	return m_objectMacros.find(handle);
}

const CRiObjectMacro *CRenderState::objectInstance(RtObjectHandle handle) const
{
	return m_objectMacros.find(handle);
}

RtString CRenderState::findHandleId(CParameterList &params) const
{
	const CParameter *p = params.get(RI_HANDLEID);
	if ( p && p->basicType() == BASICTYPE_STRING )
		return p->stringPtrs()[0];
	return RI_NULL;
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


RtObjectHandle CRenderState::objectBegin(RtString name, CRManInterfaceFactory &aFactory)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_modeStack->objectBegin();
	
	m_macros.push_back(m_curMacro);
	m_recordModes.push_back(m_recordMode);
	m_recordMode = true;
	unsigned long num;
	RtToken t = m_objectMacros.newHandle(name, num);
	CRiObjectMacro *m = new CRiObjectMacro(t, num, notEmptyStr(name), &aFactory);
	m_curMacro = m;

	if ( executeConditionial() || m_curMacro != 0 ) {
		if ( m != 0 ) {
			m_objectMacros.insertObject(m->handle(), m);
			return m->handle();
		} else {
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
}

CRiArchiveMacro *CRenderState::findArchiveInstance(RtArchiveHandle handle)
{
	CRiArchiveMacro *m = m_archiveMacros.find(handle);
	if ( m ) return m;
	return m_cachedArchive.find(handle);
}

const CRiArchiveMacro *CRenderState::findArchiveInstance(RtArchiveHandle handle) const
{
	const CRiArchiveMacro *m = m_archiveMacros.find(handle);
	if ( m ) return m;
	return m_cachedArchive.find(handle);
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
		CRiArchiveMacro *m = new CRiArchiveMacro(t, num, notEmptyStr(aName), &aFactory);
		m_curMacro = m;

		if ( m != 0 ) {
			m_archiveMacros.insertObject(m->handle(), m);
			return m->handle();
		} else {
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
}

RtArchiveHandle CRenderState::archiveFileBegin(const char *aName, CRManInterfaceFactory &aFactory)
{
	if ( executeConditionial() || m_curMacro != 0 ) {
		m_macros.push_back(m_curMacro);
		unsigned long num;
		RtToken t = m_cachedArchive.newHandle(aName, num);
		CRiArchiveMacro *m = new CRiArchiveMacro(t, num, notEmptyStr(aName), &aFactory, CRiMacro::MACROTYPE_FILE);
		m_curMacro = m;
		
		// Does not influence nesting, because called within a IRi::readArchiveV()

		if ( m != 0 ) {
			m_cachedArchive.insertObject(m->handle(), m);
			return m->handle();
		} else {
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
	RtToken t = tokFind(type);
	if ( !t ) {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "not a token for a resource factory in resource(%s, Token: %s)", noNullStr(handle), noNullStr(type));
	}

	IResourceFactory *f = m_resourceFactories.findObj(t);
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
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_WARNING, "in motionEnd(), too few requests in motion block, last request has been duplicated.", __LINE__, __FILE__);
	}
}

void CRenderState::moveArchiveBegin()
{
	if ( m_motionState.curState() == CMotionState::MOT_OUTSIDE )
		return;

	if ( m_motionState.curSampleIdx() >= 0 ) {
		// Start and continue
		/// @todo sample the current move values
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


	if ( space == RI_WORLD || space == RI_OBJECT )
		return &curTransform();

	if ( space == RI_CAMERA )
		return m_worldToCamera;

	if ( space == RI_SCREEN )
		return m_cameraToScreen;

	if ( space == RI_NDC )
		return m_screenToNDC;

	if ( space == RI_RASTER )
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

RtPoint *CRenderState::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
{
	if ( npoints <= 0 || points == 0 ) {
		return 0;
	}
	
	const CTransformation *from = findTransform(fromspace);
	const CTransformation *to   = findTransform(tospace);
	
	RtToken fs = fromspace, ts = tospace;
	
	if ( from )
		fs = from->spaceType();
	if ( to )
		ts = to->spaceType();
	
	RtToken spaces[] = {RI_OBJECT, RI_WORLD, RI_CAMERA, RI_SCREEN, RI_NDC, RI_RASTER, RI_NULL};
	
	int fi, ti;
	
	fi = 0;
	while ( spaces[fi] && spaces[fi] != fs ) ++fi;
	ti = 0;
	while ( spaces[ti] && spaces[ti] != ts ) ++ti;
	
	if ( !spaces[fi] || !spaces[ti] ) {
		throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints(%s (%s found), %s (%s found)), spaces not found", noNullStr(fromspace), spaces[fi] ? "was" : "not", noNullStr(tospace), spaces[ti] ? "was" : "not");
	}
	
	if ( fi == ti ) {
		// Same space
		return 0;
	}
	
	CMatrix3D m;
	while ( fi < ti ) {
		from = findTransform(spaces[ti]);
		assert(from!=0);
		if ( from ) {
			m.concatTransform(from->getCTM());
		} else {
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints: space (%s) not defined", noNullStr(spaces[ti]));
		}
		ti--;
	}
	
	while ( ti < fi ) {
		ti++;
		from = findTransform(spaces[ti]);
		assert(from!=0);
		if ( from ) {
			m.concatTransform(from->getInverseCTM());
		} else {
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_WARNING, printLineNo(__LINE__), printName(__FILE__), "transformPoints: space (%s) not defined", noNullStr(spaces[ti]));
		}
	}
	
	m.transformPoints(npoints, points);
	return &points[0];
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
	return m_cachedArchive.identify(archiveName);
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
	// Additional Tokens
	RI_RIB = tokFindCreate("rib");
	RI_CACHE_FILE_ARCHIVES = tokFindCreate("cache-file-archives");
	RI_VARSUBST = tokFindCreate("varsubst");

	// Additional render specific declarations
	RI_QUAL_CACHE_FILE_ARCHIVES = declare("Control:rib:cache-file-archives", "constant integer", true);
	RI_QUAL_VARSUBST = declare("Option:rib:varsubst", "string", true);
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
