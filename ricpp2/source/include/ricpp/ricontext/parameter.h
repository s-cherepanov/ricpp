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
 *  @brief RI parameters and parameter lists.
 */

#ifndef _RICPP_DECLARATION_DECLDICT_H
#include "ricpp/declaration/decldict.h"
#endif // _RICPP_DECLARATION_DECLDICT_H

#include <string>
#include <list>
#include <cassert>

namespace RiCPP {

	// ------------------------------------------------------------------------
	/** @brief Class to store a parameter value with its declaration.
	 *
	 *  Stores all data needed to specify one parameter, incl. a pointer to the declaration.
	 *  The number of faces, corners etc. is needed to calculate the number of parameters. They
	 *  can be determined by using CParameterClasses objects found in the file paramclasses.h.
	 *  Class also supports inline declarations. A declaration will be updated if the color
	 *  descriptor has been changed and the value type is color.
	 *
	 *  A parameter can be either a vector of integer, flaot or string values.
	 */
	class CParameter {
	private:
		const CDeclaration *m_declaration; //!< Declaration (and name) of the parameter, possibly inline.
		unsigned int m_position;           //!< Original position within a parameter list.
		
		std::string m_tokenStr;             //!< Token as found in original parameter list.

		std::vector<RtInt> m_ints;          //!< Container for integer values.
		std::vector<RtFloat> m_floats;      //!< Container for float values.
		std::vector<std::string> m_strings; //!< Container for string values.
		std::vector<RtString> m_stringPtrs; //!< Containter for the pointers to the string values.

		/** @brief Fills the m_stringPtrs with pointers to the c-strings of m_strings.
		 */
		void copyStringPtr();

	public:
		/** @brief Standard constructor, empty parameter.
		 */
		inline CParameter()
		{
			m_declaration = 0;
			m_position = 0;
		}

		/** @brief Copy constructor.
		 *
		 * @param param Parameter to copy
		 */
		inline CParameter(const CParameter &param)
		{
			m_declaration = 0;
			m_position = 0;
			*this = param;
		}

		/** @brief Constructs a parameter.
		 *
		 *  @param theName Name (not yet a token) or inline declaration of the parameter.
		 *  @param theData Pointer to the parameter values (type is given by the declaration of @a theName).
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 */
		inline CParameter(
			RtString theName,
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

		/** @brief Destructor
		 *
		 *  Deletes only an inline declarition.
		 */
		inline ~CParameter()
		{
			if ( m_declaration && m_declaration->isInline() )
				delete m_declaration;
		}

		/** @brief Duplicates a parameter.
		 *
		 *  @return Clone of this object.
		 */
		inline CParameter *duplicate() const
		{
			return new CParameter(*this);
		}
		
		/** @brief Clears the contents of a parameter.
		 */
		void clear();

		/** @brief Assigns a parameter.
		 *
		 *  Assigns the values of @param to this object. Makes a deep copy
		 *  of inline declarations.
		 *
		 *  @param param Parameter to assign
		 *  @return *this
		 */
		CParameter &operator=(const CParameter &param);

		/** @brief finds the declaration of a parameter.
		 *
		 *  @param theName Name (not yet a token) of a declaration or inline declaration.
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 */
		bool setDeclaration(
			RtString theName,
			unsigned int thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		/** @brief Sets the contents of a parameter.
		 *
		 *  @param theName Name (not yet a token) or inline declaration of the parameter.
		 *  @param theData Pointer to the parameter values (type is given by the declaration of @a theName).
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 */
		void set(
			RtString theName,
			RtPointer theData,
			unsigned int thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		/** @brief Gets a copy of single value out of the parameter.
		 *
		 *  @retval p Copy of the value.
		 *  @param pos Position of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, CValue &p) const;
		bool get(unsigned long pos, RtInt &result) const;
		bool get(unsigned long pos, RtFloat &result) const;
		bool get(unsigned long pos, std::string &result) const;
		bool get(unsigned long pos, RtString &result) const;

		/** @brief Name of the declaration (with qualifiers if available)
		 *
		 *  @return Name of the declaration.
		 */
		inline const char *name() const
		{
			return m_declaration ? m_declaration->name() : "";
		}

		/** @brief Gets the declaration as c-string.
		 *
		 *  Inline declaration are given as complete declaration, normal
		 *  declarations as name only.
		 *
		 *  @return Declaration as C-string.
		 */
		inline const char *tokenStr() const
		{
			return m_tokenStr.c_str();
		}

		/** @brief Gets the token of the declaration.
		 *
		 *  @brief Token of the declaration.
		 */
		RtToken token() const
		{
			return m_declaration ? m_declaration->token() : RI_NULL;
		}

		/** @brief Gets true if parameter was declared inline.
		 *
		 *  @brief True, parameter was declared inline.
		 */
		bool isInline() const
		{
			return m_declaration ? m_declaration->isInline() : false;
		}

		/** @brief Gets a pointer to the values.
		 *
		 *  @return Pointer to the values.
		 */
		RtPointer valptr();

		/** @brief Gets the number of values.
		 *
		 *  @return Number of values.
		 */
		unsigned long size();

		/** @brief Gets the basic type of the parameter.
		 *
		 *  E.g. float is the basic type of point.
		 *
		 *  @return Basic type of the values.
		 */
		inline EnumBasicTypes basicType() const
		{
			return m_declaration ? m_declaration->basicType() : BASICTYPE_UNKNOWN;
		}

		/** @brief Gets the type of the parameter.
		 *
		 *  @return Type of the values.
		 */
		inline EnumTypes type() const
		{
			return m_declaration ? m_declaration->type() : TYPE_UNKNOWN;
		}

		/** @brief Gets the integer values.
		 *
		 *  if basicType() is BASICTYPE_INTEGER, m_ints contains the integer values.
		 *
		 *  @return Integer values.
		 */
		const std::vector<RtInt> &ints() const { return m_ints; }

		/** @brief Gets the float values.
		 *
		 *  if basicType() is BASICTYPE_FLOAT, m_floats contains the float values.
		 *
		 *  @return Float values.
		 */
		const std::vector<RtFloat> &floats() const { return m_floats; }

		/** @brief Gets the string values.
		 *
		 *  if basicType() is BASICTYPE_STRING, m_strings contains the string values.
		 *
		 *  @return String values.
		 */
		const std::vector<std::string> &strings() const { return m_strings; }

		/** @brief Gets the c-string pointer values.
		 *
		 *  if basicType() is BASICTYPE_STRING, m_stringPtrs contains pointers of the string values.
		 *
		 *  @return C-string pointers.
		 */
		const std::vector<RtString> &stringPtrs() const { return m_stringPtrs; }

		/** @brief Gets a pointer to the declaration of the praameter.
		 *
		 *  @return Pointer to the declaration of the parameter.
		 */
		const CDeclaration *declarationPtr() const
		{
			return m_declaration;
		}

		/** @brief Gets a reference of the declaration of the parameter.
		 *
		 *  Throws an ExceptRiCPPError if declaration is not set.
		 *
		 *  @return Reference of the declaration of the parameter.
		 *  @throw ExceptRiCPPError
		 */
		const CDeclaration &declaration() const;
	}; // CParameter


	// ------------------------------------------------------------------------
	/** @brief Stores a list of parameters.
	 *
	 *  Used to store the token/value pairs of a ri request.
	 */
	class CParameterList {
	public:
		typedef std::list<CParameter>::iterator iterator;             //!< Iterator for parameters.
		typedef std::list<CParameter>::const_iterator const_iterator; //!< Constant iterator for parameters.
		typedef std::list<CParameter>::size_type size_type;           //!< Type for the size of the parameter list.

	private:
		/** @brief Type to map tokens to their parameters.
		 */
		typedef std::map<RtToken, CParameter *> Map_type;
		
		std::list<CParameter> m_params;    //!< List of parameters
		Map_type m_paramMap;               //!< Maps tokens to their parameters.
		std::vector<RtToken> m_tokenPtr;   //!< Vector of the tokens for ri token/value parameters.
		std::vector<RtPointer> m_paramPtr; //!< Pointer to the values for ri token/value parameters.

		/** @brief Rebuilds the parameter pointer vectors m_tokenPtr and m_paramPtr from m_params.
		 */
		void rebuild();

	public:
		/** @brief Standard constructor.
		 */
		inline CParameterList() {}

		/** Copy constructor.
		 */
		inline CParameterList(const CParameterList &params) { *this = params; }
		
		/** Costructor, sets the contents of the list.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		CParameterList(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);
			
		/** @brief Virtual destructor.
		 */
		inline virtual ~CParameterList() {}

		/** @brief Duplicates a parameter list.
		 *
		 *  @return Clone of this object.
		 */
		inline virtual CParameterList *duplicate() const
		{
			return new CParameterList(*this);
		}
		
		/** @brief Assigns a parameter list.
		 *
		 *  @param params Parameter list to assign.
		 *  @return *this
		 */
		CParameterList &operator=(const CParameterList &params);

		/** @brief Gets a constant iterator.
		 *
		 *  @return Constant iterator, starts at first parameter.
		 */
		inline const_iterator begin() const
		{
			return m_params.begin();
		}

		/** @brief Gets a iterator.
		 *
		 *  @return Iterator, starts at first parameter.
		 */
		inline iterator begin()
		{
			return m_params.begin();
		}

		/** @brief Gets the end for a constant iterator.
		 *
		 *  @return Constant iterator, end of parameter list.
		 */
		inline const_iterator end() const
		{
			return m_params.end();
		}

		/** @brief Gets the end for a iterator.
		 *
		 *  @return Iterator, end of parameter list.
		 */
		inline iterator end()
		{
			return m_params.end();
		}

		/** @brief Gets the size of the parameter list.
		 *
		 *  @return Size of the parameter list.
		 */
		inline size_type size() const
		{
			return m_params.size();
		}

		/** @brief Clears the contents of the parameter list.
		 */
		inline virtual void clear()
		{
			m_params.resize(0);
			m_paramMap.clear();
			m_tokenPtr.resize(0);
			m_paramPtr.resize(0);
		}

		/** @brief Copies the parameters.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		virtual void set(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		/** @brief Copies the parameter list.
		 *
		 *  @param params Parameter list to copy.
		 */
		inline virtual void set(const CParameterList &params)
		{
			*this = params;
		}

		/** @brief Adds parameters list to the existing list.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		virtual void add(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		/** @brief Adds the contents of an parameter list to the existing list.
		 *
		 *  @param params Parameter list to add.
		 */
		virtual void add(const CParameterList &params);

		/** @brief Gets pointer to a parameter (token/value pair).
		 *
		 *  @param var Token to identify a parameter.
		 *  @return Pointer to a parameter identified by @a token.
		 */
		CParameter *get(RtToken var);
		
		/** @brief Gets pointer to a constant parameter (token/value pair).
		 *
		 *  @param var Token to identify a parameter.
		 *  @return Pointer to a constant parameter identified by @a token.
		 */
		const CParameter *get(RtToken var) const;

		/** @brief Erases a specific parameter (token/value pair).
		 *
		 *  @param var Token to identify the parameter.
		 */
		bool erase(RtToken var);

		/** @brief Erases a specific parameter (token/value pair).
		 *
		 *  @param param pointer to a parameter.
		 */
		bool erase(CParameter *param);

		/** @brief Gets a pointer to the tokens as used by the RenderMan interface.
		 *
		 *  @return Pointer to the tokens as used by RenderMan interface.
		 */
		inline RtToken *tokenPtr()
		{
			assert(m_tokenPtr.size() == size());
			return m_tokenPtr.empty() ? 0 : &m_tokenPtr[0];
		}

		/** @brief Gets a pointer to the constant tokens as used by the RenderMan interface.
		 *
		 *  @return Pointer to the constant tokens as used by RenderMan interface.
		 */
		inline const RtToken *tokenPtr() const
		{
			assert(m_tokenPtr.size() == size());
			return m_tokenPtr.empty() ? 0 : &m_tokenPtr[0];
		}

		/** @brief Gets a pointer to the parameters as used by the RenderMan interface.
		 *
		 *  @return Pointer to the parameters as used by RenderMan interface.
		 */
		inline RtPointer *valuePtr()
		{
			assert(m_paramPtr.size() == size());
			return m_paramPtr.empty() ? 0 : &m_paramPtr[0];
		}

		/** @brief Gets a pointer to the constant parameters as used by the RenderMan interface.
		 *
		 *  @return Pointer to the constant parameters as used by RenderMan interface.
		 */
		inline const RtPointer *valuePtr() const
		{
			assert(m_paramPtr.size() == size());
			return m_paramPtr.empty() ? 0 : &m_paramPtr[0];
		}

		/** @brief is there at least one color value within the parameters?
		 *
		 *  @return True, there is at least one color value within the parameters. 
		 */
		bool hasColor() const;
	}; // CParameterList


	// ------------------------------------------------------------------------
	/** @brief A parameter list with a token as identifier.
	 *
	 *  Used to store attributes, options and controlls. 
	 */
	class CNamedParameterList : public CParameterList {
	private:
		RtToken m_name; //!< Token as identifier

		/** @brief Copies the parameters.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline virtual void set(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			CParameterList::set(counts, dict, curColorDescr, n, tokens, params);
		}

	public:
		/** @brief Default constructor.
		 *  
		 *  @param aName Name of the parameterlist, RI_NULL as default
		 */
		inline CNamedParameterList(RtToken aName=RI_NULL)
		{
			m_name = aName;
		}

		/** @brief Default constructor.
		 *  
		 *  @param aName Name of the parameter list.
		 *  @param params unnamed parameter list.
		 */
		inline CNamedParameterList(RtToken aName, const CParameterList &params) : CParameterList(params)
		{
			m_name = aName;
		}

		/** @brief Copy constructor.
		 *  
		 *  @param params Named parametrlist to copy
		 */
		inline CNamedParameterList(const CNamedParameterList &params)
		{
			*this = params;
		}

		/** @brief Constructs a parameter list.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param aName Name of the parameter list.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline CNamedParameterList(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: CParameterList(counts, dict, curColorDescr, n, tokens, params)
		{
			m_name = aName;
		}

		/** @brief Duplicates a named parameter list.
		 *
		 *  @return Clone of this object.
		 */
		inline virtual CParameterList *duplicate() const
		{
			return new CNamedParameterList(*this);
		}
		
		/** @brief Assigns a parameter list
		 *
		 * @param params Named parameter list to assign
		 * @return *this
		 */
		CNamedParameterList &operator=(const CNamedParameterList &params);

		/** @brief Sets a parameter list to specific values.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param aName Name of the parameter list.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline virtual void set(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			const char *aName,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			name(aName);
			CParameterList::set(counts, dict, curColorDescr, n, tokens, params);
		}

		/** @brief Sets a name and copies an unnamed parameter list.
		 *  
		 *  @param aName Name of the parameter list to set.
		 *  @param params Unnamed parameter list to copy.
		 */
		inline virtual void set(const char *aName, const CParameterList &params)
		{
			name(aName);
			CParameterList::set(params);
		}

		/** @brief Copies a parameter list.
		 *  
		 *  @param params Parameter list to copy.
		 */
		inline virtual void set(const char *aName, const CNamedParameterList &params)
		{
			*this = params;
		}

		/** @brief Adds parameters to the existing list.
		 *
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline virtual void add(
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			CParameterList::add(counts, dict, curColorDescr, n, tokens, params);
		}

		/** @brief Adds the contents of an parameter list to the existing list.
		 *
		 *  @param params Unnamed parameter list to add.
		 */
		inline virtual void add(const CParameterList &params)
		{
			CParameterList::add(params);
		}

		/** @brief Sets the name of the list.
		 *
		 *  @param aName Name to set.
		 */
		inline void name(const char *aName)
		{
			m_name = aName;
		}

		/** @brief Gets the name of the list.
		 *
		 *  @return Name of the list.
		 */
		inline RtToken name() const
		{
			return m_name;
		}
	}; // CNamedParameterList

}

#endif // _RICPP_RICONTEXT_PARAMETER_H
