#include "ricpp/tools/stringpattern.h"
#include "ricpp/tools/inlinetools.h"

using namespace RiCPP;

bool CStringPattern::doMatch(const char *patternPos, const char **aString) const
{
	for ( ; *patternPos && (*aString)[0]; ++patternPos, ++(*aString) ) {
		char p = patternPos[0];
		char c = (*aString)[0];

		if ( p == '*' ) {
			// '*...*' -> '*'
			while ( patternPos[0] == '*' ) {
				++patternPos;
				if ( !patternPos[0] ) {
					// * at the end matches everything
					return true;
				}
			}
			// This can lead to heavy backtracking for patterns with many asterisks.
			const char *sav = *aString;
			while ( !doMatch(patternPos, aString) ) {
				*aString = sav;
				++(*aString);
				sav = *aString;
				if ( !(*aString)[0] )
					return false;
			}
			return true;
		}

		if ( p == '?' ) {
			// Matches everything (but not '\0')
			continue;
		}

		if ( p == '\\' ) {
			// Escaped operator?
			if ( (patternPos)[1]=='*' || (patternPos)[1]=='?' || (patternPos)[1]=='\\' ) {
				++patternPos;
				p = *patternPos;
			}
		}

		if ( p != c ) {
			// Characters don't match
			return false;
		}
	}

	// Both have to be at end '\0'
	return *patternPos == *aString[0];
}

void CStringPattern::pattern(const char *aPattern)
{
	m_pattern = noNullStr(aPattern);
}

bool CStringPattern::matches(const char *aString) const
{
	aString = noNullStr(aString);
	return doMatch(m_pattern.c_str(), &aString);
}
