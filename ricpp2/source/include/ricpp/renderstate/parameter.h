#ifndef _RICPP_RENDERSTATE_PARAMETER_H
#define _RICPP_RENDERSTATE_PARAMETER_H

#ifndef _RICPP_DECLARATION_DECLDICT_H
#include "ricpp/declaration/decldict.h"
#endif // _RICPP_DECLARATION_DECLDICT_H

#include <string>
#include <list>
#include <cassert>

namespace RiCPP {

	/** @brief Class to store a parameter
	 *
	 *  Stores all data needed to specify one parameter, incl. a pointer to the declaration.
	 *  The number of faces, corners etc. needed to calculate the number of parameters
	 *  can be determined by using the TParameterClasses classes found in the file paramclasses.h.
	 */
	class CParameter {
	private:
		const CDeclaration *m_declaration; //!< Declaration of the parameter
		unsigned int m_position;           //!< Original position in parameter list.

		std::vector<RtInt> m_ints;
		std::vector<RtFloat> m_floats;
		std::vector<std::string> m_strings;
		std::vector<RtString> m_stringPtrs;

		void copyStringPtr();

	public:
		inline CParameter()
		{
			m_declaration = 0;
			m_position = 0;
		}

		inline CParameter(const CParameter &param)
		{
			m_declaration = 0;
			m_position = 0;
			*this = param;
		}

		inline CParameter(
			RtToken theName,
			RtPointer theData,
			unsigned int thePosition,
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr)
		{
			m_declaration = 0;
			m_position = 0;
			set(theName, theData, thePosition, counts, dict, curColorDescr);
		}

		inline ~CParameter()
		{
			if ( m_declaration && m_declaration->isInline() )
				delete m_declaration;
		}

		void clear();

		CParameter &operator=(const CParameter &param);

		void set(
			RtToken theName,
			RtPointer theData,
			unsigned int thePosition,
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		const char *name() const;
		RtToken token() const;

		RtPointer valptr();

		/** @brief Gets the basic type of the parameter
		 *
		 *  E.g. float is the basic type of point
		 *
		 *  @return Basic type
		 */
		inline EnumBasicTypes basicType() const
		{
			return m_declaration ? m_declaration->basicType() : BASICTYPE_UNKNOWN;
		}

		/** @brief Gets the type of the parameter
		 *
		 *  @return type
		 */
		inline EnumTypes type() const
		{
			return m_declaration ? m_declaration->type() : TYPE_UNKNOWN;
		}

		const std::vector<RtInt> &ints() const { return m_ints; }
		const std::vector<RtFloat> &floats() const { return m_floats; }
		const std::vector<std::string> &strings() const { return m_strings; }
		const std::vector<RtString> &stringPtrs() const { return m_stringPtrs; }

		const CDeclaration *declaration()
		{
			return m_declaration;
		}

	}; // CParameter


	class CParameterList {
	public:
		typedef std::list<CParameter>::const_iterator const_iterator;
		typedef std::list<CParameter>::size_type size_type;

	private:
		typedef std::map<RtToken, CParameter *> Map_type;
		std::list<CParameter> m_params;
		Map_type m_paramMap;

		std::vector<RtToken> m_tokenPtr;
		std::vector<RtPointer> m_paramPtr;

		void rebuild();

	public:
		inline CParameterList() {}
		inline CParameterList(const CParameterList &params) { *this = params; }
		CParameterList(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);
		inline virtual ~CParameterList() {}

		CParameterList &operator=(const CParameterList &params);

		inline const_iterator begin() const
		{
			return m_params.begin();
		}

		inline const_iterator end() const
		{
			return m_params.end();
		}

		inline size_type size() const
		{
			return m_params.size();
		}

		inline void clear()
		{
			m_params.resize(0);
			m_paramMap.clear();
			m_tokenPtr.resize(0);
			m_paramPtr.resize(0);
		}

		virtual void set(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		virtual void add(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		CParameter *get(RtToken var);
		const CParameter *get(RtToken var) const;
		bool erase(RtToken var);
		bool erase(CParameter *param);

		inline RtToken *tokenPtr()
		{
			assert(m_tokenPtr.size() == size());
			return m_tokenPtr.empty() ? 0 : &m_tokenPtr[0];
		}
		inline RtPointer *valuePtr()
		{
			assert(m_paramPtr.size() == size());
			return m_paramPtr.empty() ? 0 : &m_paramPtr[0];
		}

		bool hasColor() const;
	}; // CParameterList


	class CNamedParameterList : public CParameterList {
	private:
		std::string m_name;

		inline virtual void set(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			CParameterList::set(counts, dict, curColorDescr, n, tokens, params);
		}

	public:
		inline CNamedParameterList(const char *aName = 0)
		{
			name(aName);
		}

		inline CNamedParameterList(const CNamedParameterList &params)
		{
			*this = params;
		}

		inline CNamedParameterList(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			const char *aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: CParameterList(counts, dict, curColorDescr, n, tokens, params)
		{
			name(aName);
		}

		CNamedParameterList &operator=(const CNamedParameterList &params);

		virtual void set(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			const char *aName,
			RtInt n, RtToken tokens[], RtPointer params[]);

		virtual void add(
			const CValueCounts &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		inline void name(const char *aName)
		{
			m_name = noNullStr(aName);
		}

		inline const char *name() const
		{
			return m_name.c_str();
		}
	}; // CNamedParameterList

}

#endif // _RICPP_RENDERSTATE_PARAMETER_H
