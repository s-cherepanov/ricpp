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
	std::string m_pattern; //!< The pattern used for matching
	
	/** @brief Recursive matching.
	 *
	 * @param patternPos Position in pattern (m_pattern).
	 * @param aString Address of the string to match.
	 */
	bool doMatch(const char *patternPos, const char **aString) const;
public:
	/** @brief Default constructor.
	 *
	 *  @param aPattern Pattern used for matching.
	 */
	inline CStringPattern(const char *aPattern = 0)
	{
		pattern(aPattern);
	}

	/** @brief Gets the current pattern.
	 *
	 *  @return Current pattern  
	 */
	inline const char *pattern() const
	{
		return m_pattern.c_str();
	}

	/** @brief Sets the current pattern.
	 *
	 *  @param aPattern Pattern to use.  
	 */
	void pattern(const char *aPattern);

	/** @brief Matches the pattern stored in this object to a string.
	 *
	 *  @param aString String to be matched.  
	 */
	bool matches(const char *aString) const;
}; // CStringPattern

}

#endif // _RICPP_TOOLS_STRINGPATTERN_H
