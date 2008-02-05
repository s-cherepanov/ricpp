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

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;

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

	if ( match("$(", str, retval) ) {
		wss(str, retval);
		if ( expr(str, retval, val) ) {
			wss(str, retval);
			if ( match(")", str, retval) ) {
				std::string strname;
				val.get(strname);
				result += retval;
				if ( m_outer->getValue(val, strname.c_str()) ) {
					return true;
				}
				// throw error: value strname not found
				return false;
			}
		}
	}

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
			result += "$";
			result += strname;
			if ( m_outer->getValue(val, strname.c_str()) ) {
				return true;
			}
			// throw error: value strname not found
			return false;
		}
	}

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
	CLightSourceFactory &lightSourceFactory,
	CFilterFuncFactory &filterFuncFactory
	// , CRManInterfaceFactory &aMacroFactory
	) :
	m_resourceFactories(false),
	m_lights(lightSourceFactory),
	m_objectMacros("OBJ_"),
	m_archiveMacros("ARC_")
// throw(ExceptRiCPPError)
{
	m_modeStack = &aModeStack;
	m_optionsFactory = &optionsFactory;
	m_attributesFactory = &attributesFactory;
	m_filterFuncFactory = &filterFuncFactory;
	// m_macroFactory = &aMacroFactory;
	m_frameNumber = 0;
	m_lineNo = -1;

	m_curMacro = 0;
	m_curReplay = 0;

	m_reject = false;
	m_recordMode = false;

	m_executeConditional = true;
	m_accumulateConditional = true;
	m_ifCondition = false;
	
	CFilepath fp;
	std::string s(fp.filepath());
	s+= "/";
	m_baseUri.set("file", "", s.c_str(), 0, 0);
}

CRenderState::~CRenderState()
{
	while ( popOptions() );
	while ( popAttributes() );
	m_transformStack.clear();
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

RtObjectHandle CRenderState::objectBegin()
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_modeStack->objectBegin();
	
	m_macros.push_back(m_curMacro);
	m_recordModes.push_back(m_recordMode);
	m_recordMode = true;
	CRiObjectMacro *m = new CRiObjectMacro();
	m_curMacro = m;

	if ( executeConditionial() || m_curMacro != 0 ) {
		if ( m != 0 ) {
			m_objectMacros.insertObject(m);
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

CRiArchiveMacro *CRenderState::archiveInstance(RtArchiveHandle handle)
{
	return m_archiveMacros.find(handle);
}

const CRiArchiveMacro *CRenderState::archiveInstance(RtArchiveHandle handle) const
{
	return m_archiveMacros.find(handle);
}

RtArchiveHandle CRenderState::archiveBegin(const char *aName)
{
	pushOptions();
	pushAttributes();
	pushTransform();

	m_modeStack->archiveBegin();
	
	if ( executeConditionial() || m_curMacro != 0 ) {
		m_macros.push_back(m_curMacro);
		m_recordModes.push_back(m_recordMode);
		m_recordMode = true;
		CRiArchiveMacro *m = new CRiArchiveMacro(aName);
		m_curMacro = m;

		if ( m != 0 ) {
			m_archiveMacros.insertObject(m);
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

RtArchiveHandle CRenderState::archiveFileBegin(const char *aName)
{
	if ( executeConditionial() || m_curMacro != 0 ) {
		m_macros.push_back(m_curMacro);
		CRiArchiveMacro *m = new CRiArchiveMacro(aName, CRiMacro::MACROTYPE_FILE);
		m_curMacro = m;
		
		// Does not influence nesting, because called within a IRi::sreadArchiveV()

		if ( m != 0 ) {
			m_archiveMacros.insertObject(m);
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

bool CRenderState::varsplit(RtString identifier, RtToken *namespaceQual, RtToken *varname, RtToken *valuename) const
{
	*namespaceQual=0;
	*varname=0;
	*valuename=0;

	if ( emptyStr(identifier) )
		return false;

	CStringList sl;

	sl.explode(':', identifier, false, false, false);

	if ( sl.empty() || sl.size() > 3 )
		return false;

	*valuename = sl.back().c_str();
	*valuename = tokFind(*valuename);
	sl.pop_back();

	if ( !sl.empty() ) {
		*varname = sl.back().c_str();
		*varname = tokFind(*varname);
		sl.pop_back();
	}

	if ( !sl.empty() ) {
		*namespaceQual = sl.back().c_str();
		*namespaceQual = tokFind(*namespaceQual);
		sl.pop_back();
	}

	return true;
}

bool CRenderState::exists(RtString identifier) const
{
	CValue p;
	return getValue(p, identifier);
}

bool CRenderState::getAttribute(CValue &p, RtToken varname, RtToken valuename) const
{
	const CParameter *param = attributes().get(varname, valuename);
	if ( param ) {
		param->get(p, 0);
		return true;
	}
	return false;
}

bool CRenderState::getOption(CValue &p, RtToken varname, RtToken valuename) const
{
	const CParameter *param = options().get(varname, valuename);
	if ( param ) {
		param->get(p, 0);
		return true;
	}
	return false;
}

bool CRenderState::getValue(CValue &p, RtString identifier) const
{
	RtToken namespaceQual, varname, valuename;
	if ( !varsplit(identifier, &namespaceQual, &varname, &valuename) )
		return false;

	if ( !namespaceQual && !varname )
	{
		if ( valuename == RI_FRAME ) {
			p.set(frameNumber());
			return true;
		}
	} else if ( !namespaceQual ) {
		if ( getAttribute(p, varname, valuename) )
			return true;
		if ( getOption(p, varname, valuename) )
			return true;
	} else {
		if ( namespaceQual == RI_ATTRIBUTE )
			return getAttribute(p, varname, valuename);
		if ( namespaceQual == RI_OPTION )
			return getOption(p, varname, valuename);
	}

	return false;
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

void CRenderState::resource(IRiContext &ri, RtString handle, RtString type, const CParameterList &params)
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
			r = f->getResource(handle);
			if ( !r ) {
				throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "no resource factory in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
			}
			m_resourceStack.insertObject(r);
		}
	} else {
		r = m_resourceStack.find(m_resourceStack.identify(handle), false);
		if ( !r ) {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, __LINE__, __FILE__, "no valid resource handle found in resource(Handle: %s, Token: %s)", noNullStr(handle), noNullStr(type));
		}
	}
	r->operate(ri, params);
}

void CRenderState::pushOptions()
{
	try {

		if ( m_optionsStack.empty() ) {
			m_optionsStack.push_back(m_optionsFactory->newOptions());
		} else {
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
	if ( !m_optionsStack.empty() ) {
		m_optionsFactory->deleteOptions(m_optionsStack.back());
		m_optionsStack.pop_back();
	}
	return !m_optionsStack.empty();
}

void CRenderState::pushAttributes()
{
	try {
		TypeTransformationMap tm;
		m_scopedTransforms.push_back(tm);

		if ( m_attributesStack.empty() ) {
			m_attributesStack.push_back(m_attributesFactory->newAttributes(options().colorDescr()));
		} else {
			m_attributesStack.push_back(m_attributesFactory->newAttributes(*m_attributesStack.back()));
		}
		if ( m_attributesStack.back() == 0 ) {
			m_attributesStack.pop_back();
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in pushAttributes()", __LINE__, __FILE__);
		}

	} catch ( ExceptRiCPPError &e2 ) {
		throw e2;
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushAttributes(): %s", e.what());
	}
}

bool CRenderState::popAttributes()
{
	if ( !m_attributesStack.empty() ) {
		m_attributesFactory->deleteAttributes(m_attributesStack.back());
		m_attributesStack.pop_back();
	}
	if ( !m_scopedTransforms.empty() ) {
		m_scopedTransforms.pop_back();
	}
	return !m_attributesStack.empty();
}

void CRenderState::pushTransform()
{
	try {
		if ( m_transformStack.empty() ) {
			m_transformStack.push_back(CTransformation());
		} else {
			m_transformStack.push_back(m_transformStack.back());
		}
	} catch (std::exception &e) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE,  __LINE__, __FILE__, "in pushTransform(): %s", e.what());
	}
}

bool CRenderState::popTransform()
{
	if ( !m_transformStack.empty() ) {
		m_transformStack.pop_back();
	}
	return !m_transformStack.empty();
}

const CTransformation *CRenderState::findTransform(RtToken space) const
{
	std::map<RtString, CTransformation>::const_iterator pos;

	if ( !m_scopedTransforms.empty() ) {
		for( std::list<TypeTransformationMap>::const_reverse_iterator i = m_scopedTransforms.rbegin();
		     i != m_scopedTransforms.rend();
			 i++ )
		{
			pos = (*i).find(space);
			if ( pos != (*i).end() ) {
				return &((*pos).second);
			}
		}
	}

	pos = m_globalTransforms.find(space);
	if ( pos != m_globalTransforms.end() ) {
		return &((*pos).second);
	}
	return 0;
}

void CRenderState::startAreaLightSource(RtLightHandle h)
{
	try {
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
		p.set(counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
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
		m_curParams.set(counts, m_declDict, options().colorDescr(), n, theTokens, theParams);
	} catch (ExceptRiCPPError &err) {
		if ( archiveName() != 0 ) {
			err.line(lineNo());
			err.file(archiveName());
		}
		throw err;
	}
}

/*
CRManInterfaceFactory &CRenderState::macroFactory()
{
	assert(m_macroFactory != 0);
	return *m_macroFactory;
}

const CRManInterfaceFactory &CRenderState::macroFactory() const
{
	assert(m_macroFactory != 0);
	return *m_macroFactory;
}
*/

RtToken CRenderState::storedArchiveName(RtString archiveName) const
{
	return m_archiveMacros.identify(archiveName);
}

void CRenderState::registerResourceFactory(IResourceFactory *f)
{
	if ( !f )
		return;
	RtToken t = tokFindCreate(f->type());
	f->registerOperations(tokenMap());
	m_resourceFactories.registerObj(t, f);
}


bool CRenderState::getBasis(RtToken basisName, RtBasis basis) const
{
	return CTypeInfo::getBasis(basisName, basis);
}


RtToken CRenderState::basisName(const RtBasis basis) const
{
	return CTypeInfo::basisName(basis);
}


const IFilterFunc *CRenderState::filterFunc(RtToken name) const
{
	return m_filterFuncFactory->singleton(name);
}
