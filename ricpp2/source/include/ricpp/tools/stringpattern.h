#ifndef _RICPP_TOOLS_STRINGPATTERN_H
#define _RICPP_TOOLS_STRINGPATTERN_H

#include <string>

namespace RiCPP {

/** @brief Glob style pattern matcher.
 *
 *  Simple backtracking glob style Pattern matcher with '*' and '?' wildcards.
 *  Escapes: "\*" matches "*", "\?" matches "?", "\\" matches "\", "\a" etc. matches "\a".
 */
class CStringPattern {
private:
	std::string m_pattern;
	bool doMatch(const char *patternPos, const char **aString) const;
public:
	inline CStringPattern(const char *aPattern = 0)
	{
		pattern(aPattern);
	}

	inline const char *pattern() const
	{
		return m_pattern.c_str();
	}

	void pattern(const char *aPattern);
	bool matches(const char *aString) const;
}; // CStringPattern

}

#endif // _RICPP_TOOLS_STRINGPATTERN_H
