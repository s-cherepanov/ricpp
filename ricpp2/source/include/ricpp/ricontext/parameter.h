#ifndef _RICPP_RICONTEXT_PARAMETER_H
#define _RICPP_RICONTEXT_PARAMETER_H

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

/** @file parameter.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief RI prameters and parameter lists.
 */

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
	 *  can be determined by using the CParameterClasses classes found in the file paramclasses.h.
	 *  Class handles also inline declarations.
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
			const CParameterClasses &counts,
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

		bool setDeclaration(
			RtToken theName,
			unsigned int thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		void set(
			RtToken theName,
			RtPointer theData,
			unsigned int thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		void get(CValue &p, unsigned long pos) const;

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

		const CDeclaration *declarationPtr() const
		{
			return m_declaration;
		}

		const CDeclaration &declaration() const;

	}; // CParameter

	class CParameterList {
	public:
		typedef std::list<CParameter>::iterator iterator;
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
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);
		inline virtual ~CParameterList() {}

		CParameterList &operator=(const CParameterList &params);

		inline const_iterator begin() const
		{
			return m_params.begin();
		}

		inline iterator begin()
		{
			return m_params.begin();
		}

		inline const_iterator end() const
		{
			return m_params.end();
		}

		inline iterator end()
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
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		inline virtual void set(const CParameterList &params)
		{
			*this = params;
		}

		virtual void add(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		virtual void add(const CParameterList &params);

		CParameter *get(RtToken var);
		const CParameter *get(RtToken var) const;

		bool erase(RtToken var);
		bool erase(CParameter *param);

		inline RtToken *tokenPtr()
		{
			assert(m_tokenPtr.size() == size());
			return m_tokenPtr.empty() ? 0 : &m_tokenPtr[0];
		}

		inline const RtToken *tokenPtr() const
		{
			assert(m_tokenPtr.size() == size());
			return m_tokenPtr.empty() ? 0 : &m_tokenPtr[0];
		}

		inline RtPointer *valuePtr()
		{
			assert(m_paramPtr.size() == size());
			return m_paramPtr.empty() ? 0 : &m_paramPtr[0];
		}

		inline const RtPointer *valuePtr() const
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
			const CParameterClasses &counts,
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
			const CParameterClasses &counts,
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
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			const char *aName,
			RtInt n, RtToken tokens[], RtPointer params[]);

		inline virtual void set(const char *aName, const CParameterList &params)
		{
			name(aName);
			CParameterList::set(params);
		}

		virtual void add(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		inline virtual void add(const CParameterList &params)
		{
			CParameterList::add(params);
		}

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

#endif // _RICPP_RICONTEXT_PARAMETER_H
