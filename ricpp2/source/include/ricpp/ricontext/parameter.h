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
		const CDeclaration *m_declaration;  ///< Declaration (and name) of the parameter, possibly inline.
		IndexType m_position;               ///< Original position within a parameter list.
		std::string m_parameterName;        ///< Parametername as found in list
		
		std::vector<RtInt> m_ints;          ///< Container for integer values.
		std::vector<RtFloat> m_floats;      ///< Container for float values.
		std::vector<std::string> m_strings; ///< Container for string values.
		std::vector<RtString> m_stringPtrs; ///< Containter for the pointers to the string values.

		/** @brief Fills the m_stringPtrs with pointers to the c-strings of m_strings.
		 */
		void copyStringPtr();
			
		/** @brief Assigns a parameter to this object.
		 *
		 *  @param p Parameter to assign.
		 *  @throw ExceptRiCPPError if a inline declaration cannot be allocated
		 */
		void assign(const CParameter &p);

	public:
		/** @brief Standard constructor, empty parameter.
		 */
		inline CParameter()
			: m_declaration(0), m_position(0)
		{
		}

		/** @brief Copy constructor.
		 *
		 *  @param param Parameter to copy
		 *  @throw ExceptRiCPPError if a inline declaration cannot be allocated
		 */
		inline CParameter(const CParameter &param)
			: m_declaration(0), m_position(0)
		{
			assign(param);
		}

		/** @brief Copy constructor for remapping
		 *
		 *  @param param Parameter to copy
		 *  @param newDict New dictionary for declarations and tokens
		 *  @throw ExceptRiCPPError if a declaration cannot be allocated
		 */
		inline CParameter(const CParameter &param, CDeclarationDictionary &newDict)
			: m_declaration(0), m_position(0)
		{
			assignRemap(param, newDict);
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
			IndexType thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr)
			: m_declaration(0), m_position(0)
		{
			set(0, 0, theName, theData, thePosition, counts, dict, curColorDescr);
		}

		/** @brief Constructs a parameter.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param theName Name (not yet a token) or inline declaration of the parameter.
		 *  @param theData Pointer to the parameter values (type is given by the declaration of @a theName).
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 */
		inline CParameter(
			RtToken aQualifier, RtToken aTable, 
			RtString theName,
			RtPointer theData,
			IndexType thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr)
			: m_declaration(0), m_position(0)
		{
			set(aQualifier, aTable, theName, theData, thePosition, counts, dict, curColorDescr);
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
		 *  Makes a deep copy of inline declarations.
		 *
		 *  @param p The parameter to assign.
		 *  @return *this
		 */
		CParameter &operator=(const CParameter &p);

		/** @brief Assigns a parameter to this object and remaps the declarations to a new dictonary
		 *
		 *  Used to copy a parameter (of controls, options) from the frontend to the backend.
		 *  because the backend has it's own dictionary, the declarations must be remapped.
		 *  CBaseRenderer::init() used to start this assigning and remapping.
		 *
		 *  @param p Parameter of the front end to assign
		 *  @param newDict Dictionary of the backend, can be filled with declarations found in p
		 *  @return *this
		 *  @throw ExceptRiCPPError if a declaration cannot be allocated
		 */
		CParameter &assignRemap(const CParameter &param, CDeclarationDictionary &newDict);

		/** @brief Gets a pointer to the declaration of the parameter.
		 *
		 *  @return Pointer to the declaration of the parameter.
		 */
		inline const CDeclaration *declarationPtr() const
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
		
		/** @brief Finds the declaration of a parameter and sets the contents.
		 *
		 *  @param aQualifier Optional qualifier (token)
		 *  @param aTable Optional tble name (token)
		 *  @param theName Name (not yet a token) of a declaration or inline declaration.
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 *  @throw ExceptRiCPPError if a inline declaration cannot be allocated or syntax error
		 */
		bool setDeclaration(
			RtToken aQualifier, RtToken aTable, 
			RtString theName,
			IndexType thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		/** @brief Sets the contents of a parameter.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param theName Name (not yet a token) or inline declaration of the parameter.
		 *  @param theData Pointer to the parameter values (type is given by the declaration of @a theName).
		 *  @param thePosition Position within the original parameter list of the ri request.
		 *  @param counts Number of the parameter values.
		 *  @param dict Declaration dictionary, can be updated if color descriptor has changed.
		 *  @param curColorDescr Descriptor for "color" values.
		 */
		void set(
			RtToken aQualifier, RtToken aTable, 
			RtString theName,
			RtPointer theData,
			IndexType thePosition,
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr);

		/** @brief Gets a copy of single value out of the parameter.
		 *
		 *  @param pos Position of the value.
		 *  @retval p Copy of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, CValue &p) const;

		/** @brief Gets a copy of single integer out of the parameter.
		 *
		 *  @param pos Position of the value.
		 *  @retval result Copy of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, RtInt &result) const;

		/** @brief Gets a copy of single float out of the parameter.
		 *
		 *  @param pos Position of the value.
		 *  @retval result Copy of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, RtFloat &result) const;

		/** @brief Gets a copy of single string out of the parameter.
		 *
		 *  @param pos Position of the value.
		 *  @retval result Copy of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, std::string &result) const;

		/** @brief Gets a copy of single c string pointer out of the parameter.
		 *
		 *  @param pos Position of the value.
		 *  @retval result Copy of the value.
		 *  @return True, value has been copied.
		 */
		bool get(unsigned long pos, RtString &result) const;

		/** @brief Name of the declaration (as found with qualifiers and possibly inline declaration)
		 *
		 *  @return Name of the declaration, not necessarily a token.
		 *  @see token()
		 */
		inline const char *parameterName() const
		{
			return m_parameterName.c_str();
		}

		/*  @brief Gets the declaration as c-string.
		 *
		 *  Inline declaration are given as complete declaration, normal
		 *  declarations as name only.
		 *
		 *  @retVal decl Container for the string
		 *  @return Declaration as C-string.
		 *
		inline const char *getDeclString(std::string &decl) const
		{
			return declarationPtr() ? declaration().getDeclString(decl) : RI_NULL;
		}
		 */

		/** @brief Gets the token of the declaration.
		 *
		 *  @return (Qualified) token of the declaration.
		 */
		inline RtToken token() const
		{
			return declarationPtr() ? declaration().token() : RI_NULL;
		}

		/** @brief Gets the unqualified variable name token of the declaration.
		 *
		 *  @brief Token of the unqualified variable name of the declaration.
		 */
		inline RtToken var() const
		{
			return declarationPtr() ? declaration().var() : RI_NULL;
		}

		/** @brief Gets true if parameter was declared inline.
		 *
		 *  @brief True, parameter was declared inline.
		 */
		inline bool isInline() const
		{
			return declarationPtr() ? declaration().isInline() : false;
		}

		/** @brief Query if variable matches declaration
		 *
		 *  @param aQualifier Qualifier id of the variable.
		 *  @param aTable Table token of the variable.
		 *  @param aVar Variable identifier.
		 */
		inline bool matches(EnumQualifiers aQualifier, RtToken aTable, RtToken aVar) const
		{
			return declarationPtr() ? declaration().matches(aQualifier, aTable, aVar) : false;
		}

		/** @brief Query if variable matches declaration
		 *
		 *  @param aQualifier Qualifier token of the variable.
		 *  @param aTable Table token of the variable.
		 *  @param aVar Variable identifier.
		 */
		inline bool matches(RtToken aQualifierName, RtToken aTable, RtToken aVar) const
		{
			return declarationPtr() ? declaration().matches(aQualifierName, aTable, aVar) : false;
		}

		/** @brief Gets a pointer to the values.
		 *
		 *  @return Pointer to the values.
		 */
		RtPointer valptr();

		/** @brief Gets the number of values (basic type).
		 *
		 *  @return Number of values (e.g. size of the vector containing the basic type)
		 */
		IndexType vectorSize() const;

		/** @brief Gets the number of values (full type).
		 *
		 *  @return Number of values per position
		 */
		inline IndexType size() const
		{
			return declarationPtr() ? vectorSize() / declaration().elemSize() : 0;
		}
		
		/** @brief Gets the basic type of the parameter.
		 *
		 *  E.g. float BASICTYPE_FLOAT is the basic type of point.
		 *
		 *  @return Basic type of the values.
		 */
		inline EnumBasicTypes basicType() const
		{
			return declarationPtr() ? declaration().basicType() : BASICTYPE_UNKNOWN;
		}

		/** @brief Gets the type of the parameter.
		 *
		 *  @return Type of the values.
		 */
		inline EnumTypes type() const
		{
			return declarationPtr() ? declaration().type() : TYPE_UNKNOWN;
		}
		
		/** @brief Gets the integer values.
		 *
		 *  if basicType() is BASICTYPE_INTEGER, m_ints contains the integer values.
		 *
		 *  @return Integer values.
		 */
		inline const std::vector<RtInt> &ints() const { return m_ints; }

		/** @brief Gets the float values.
		 *
		 *  if basicType() is BASICTYPE_FLOAT, m_floats contains the float values.
		 *
		 *  @return Float values.
		 */
		inline const std::vector<RtFloat> &floats() const { return m_floats; }

		/** @brief Gets the string values.
		 *
		 *  if basicType() is BASICTYPE_STRING, m_strings contains the string values.
		 *
		 *  @return String values.
		 */
		inline const std::vector<std::string> &strings() const { return m_strings; }

		/** @brief Gets the c-string pointer values.
		 *
		 *  if basicType() is BASICTYPE_STRING, m_stringPtrs contains pointers of the string values.
		 *
		 *  @return C-string pointers.
		 */
		inline const std::vector<RtString> &stringPtrs() const { return m_stringPtrs; }
		
		void extract(IndexType pos, std::vector<RtInt>::iterator &result) const;
		void extract(IndexType from, IndexType to, std::vector<RtInt>::iterator &result) const;

		void extract(IndexType pos, std::vector<RtFloat>::iterator &result) const;
		void extract(IndexType from, IndexType to, std::vector<RtFloat>::iterator &result) const;

		void extract(IndexType pos, std::vector<std::string>::iterator &result) const;
		void extract(IndexType from, IndexType to, std::vector<std::string>::iterator &result) const;
		
		void extract(IndexType pos, std::vector<RtString>::iterator &result) const;
		void extract(IndexType from, IndexType to, std::vector<RtString>::iterator &result) const;

		void bilinearBlend(const IndexType (& cornerIdx)[4],
						   IndexType tessU,
						   IndexType tessV,
						   std::vector<RtFloat> &retvals) const;
	}; // CParameter


	// ------------------------------------------------------------------------
	/** @brief Stores a list of parameters.
	 *
	 *  Used to store the token/value pairs of a ri request.
	 */
	class CParameterList {
	public:
		typedef std::list<CParameter>::iterator iterator;             ///< Iterator for parameters.
		typedef std::list<CParameter>::const_iterator const_iterator; ///< Constant iterator for parameters.
		typedef std::list<CParameter>::size_type size_type;           ///< Type for the size of the parameter list.

	private:
		/** @brief Type to map tokens to their parameters.
		 */
		typedef std::map<RtToken, CParameter *> Map_type;
		
		std::list<CParameter> m_params;    ///< List of parameters
		Map_type m_paramMap;               ///< Maps tokens to their parameters in m_params.
		std::vector<RtToken> m_tokenPtr;   ///< Vector of the tokens for ri token/value parameters.
		std::vector<RtPointer> m_paramPtr; ///< Pointer to the values for ri token/value parameters.
		
		CValueCounts m_valueCounts;        ///< Used to calculate the sizes of parameters

		/** @brief Rebuilds the parameter pointer vectors m_tokenPtr and m_paramPtr from m_params.
		 */
		void rebuild();
		
		/** @brief Assigns the parameter lists instance variables
		 *  @param params Parameter list to assign
		 */
		void assign(const CParameterList &params);

	public:
		/** @brief Standard constructor.
		 */
		inline CParameterList() {}

		/** Copy constructor.
		 *  @param params Parameterlist to copy (declarations of old dictionary)
		 */
		inline CParameterList(const CParameterList &params)
		{
			assign(params);
		}
		
		/** @brief Copy constructor, remaps
		 *  
		 *  @param params Named parameterlist to copy (declarations of old dictionary)
		 *  @param newDict New dictionary for declarations and tokens
		 */
		inline CParameterList(const CParameterList &params, CDeclarationDictionary &newDict)
		{
			assignRemap(params, newDict);
		}

		/** Constructor, sets the contents of the list.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		CParameterList(
			const char *aQualifier, const char *aTable,
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

		/** @brief Assigns a parameterlist to this object and remaps the declarations to a new dictonary
		 *
		 *  Used to copy a parameterlist (of controls, options) from the frontend to the backend.
		 *  because the backend has it's own dictionary, the declarations must be remapped.
		 *  CBaseRenderer::init() used to start this assigning and remapping.
		 *
		 *  @param p Parameterlist of the front end to assign
		 *  @param newDict Dictionary of the backend, can be filled with declarations found in p
		 *  @return *this
		 */
		CParameterList &assignRemap(const CParameterList &params, CDeclarationDictionary &newDict);

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

		/** @brief Resets the contents for a new parameter list.
		 */
		void reset(const CParameterClasses &counts);

		/** @brief Copies the parameters.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		void set(
			const char *aQualifier, const char *aTable, 
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		/** @brief Adds parameters list to the existing list.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		void add(
			const char *aQualifier, const char *aTable, 
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtInt n, RtToken tokens[], RtPointer params[]);

		/** @brief Adds the contents of an parameter list to the existing list.
		 *
		 *  @param params Parameter list to add.
		 */
		void add(const CParameterList &params);

		/** @brief Gets pointer to a parameter (token/value pair).
		 *
		 *  @param var Token to identify a parameter.
		 *  @return Pointer to a parameter identified by @a token, 0 otherwise
		 */
		CParameter *get(RtToken var);
		
		/** @brief Gets pointer to a parameter (token/value pair), if of a specific basic type.
		 *
		 *  @param var Token to identify a parameter.
		 *  @param bt Basic type that should match the parameter
		 *  @return Pointer to a parameter identified by @a token and has type @a bt, 0 otherwise
		 */
		inline CParameter *get(RtToken var, enum EnumBasicTypes bt)
		{
			CParameter *p = get(var);
			return (p && p->basicType() == bt) ? p : 0;
		}
		
		/** @brief Gets pointer to a constant parameter (token/value pair).
		 *
		 *  @param var Token to identify a parameter.
		 *  @return Pointer to a constant parameter identified by @a token.
		 */
		const CParameter *get(RtToken var) const;

		/** @brief Gets pointer to a constant parameter (token/value pair), if of a specific basic type.
		 *
		 *  @param var Token to identify a parameter.
		 *  @param bt Basic type that should match the parameter
		 *  @return Pointer to a constant parameter identified by @a token and has type @a bt, 0 otherwise
		 */
		inline const CParameter *get(RtToken var, enum EnumBasicTypes bt) const
		{
			const CParameter *p = get(var);
			return (p && p->basicType() == bt) ? p : 0;
		}

		/** @brief Can be used to calculate the sizes of the parameters
		 */
		inline const CParameterClasses &valueCounts() const { return m_valueCounts; }
		
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
		RtToken m_name; ///< Token as identifier

		/** @brief Assigns the name of a parameter list
		 *  @param params Parameter list with the name to assign
		 */
		void assign(const CNamedParameterList &params);
	public:
		/** @brief Default constructor.
		 *  
		 *  @param aName Name of the parameterlist, RI_NULL as default
		 */
		inline CNamedParameterList(RtToken aName=RI_NULL) : m_name(aName)
		{
		}

		/** @brief Constructs a named parameterlist using a name and a parameterlist
		 *  
		 *  @param aName Name (token) of the parameter list.
		 *  @param params unnamed parameter list.
		 */
		inline CNamedParameterList(RtToken aName, const CParameterList &params) : CParameterList(params), m_name(aName)
		{
		}

		/** @brief Copy constructor.
		 *  
		 *  @param params Named parameterlist to copy
		 */
		inline CNamedParameterList(const CNamedParameterList &params) : CParameterList(params)
		{
			assign(params);
		}

		/** @brief Copy constructor, remaps
		 *  
		 *  @param params Named parameterlist to copy (declarations of old dictionary)
		 *  @param newDict New dictionary for declarations and tokens
		 */
		CNamedParameterList(const CNamedParameterList &params, CDeclarationDictionary &newDict);

		/** @brief Constructs a parameter list.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param aName Name (token) of the parameter list.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline CNamedParameterList(
			const char *aQualifier, const char *aTable, 
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: CParameterList(aQualifier, aTable, counts, dict, curColorDescr, n, tokens, params), m_name(aName)
		{
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

		/** @brief Assigns a named parameterlist to this object and remaps the declarations to a new dictonary
		 *
		 *  Used to copy a named parameterlist (of controls, options) from the frontend to the backend.
		 *  because the backend has it's own dictionary, the declarations must be remapped.
		 *  CBaseRenderer::init() used to start this assigning and remapping.
		 *
		 *  @param p Parameterlist of the front end to assign
		 *  @param newDict Dictionary of the backend, can be filled with declarations found in p
		 *  @return *this
		 */
		CNamedParameterList &assignRemap(const CNamedParameterList &params, CDeclarationDictionary &newDict);

		/** @brief Sets a parameter list to specific values.
		 *
		 *  @param aQualifier Optional qualifier
		 *  @param aTable Optional tble name
		 *  @param counts Counts for the parameters, driven by the class of a primary (e.g. varying).
		 *  @param dict Dictionary with the declarations of the parameter types.
		 *  @param curColorDescr Current color descriptor for "color" values.
		 *  @param aName Name (token) of the parameter list.
		 *  @param n Number of parameters.
		 *  @param tokens Tokens as parameter identifiers.
		 *  @param params Pointers to the parameters.
		 */
		inline void set(
			const char *aQualifier, const char *aTable, 
			const CParameterClasses &counts,
			CDeclarationDictionary &dict,
			const CColorDescr &curColorDescr,
			RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
		{
			name(aName);
			CParameterList::set(aQualifier, aTable, counts, dict, curColorDescr, n, tokens, params);
		}

		/** @brief Sets a name (token) and copies an unnamed parameter list.
		 *  
		 *  @param aName Name (token) of the parameter list to set.
		 *  @param params Unnamed parameter list to copy.
		 */
		inline void set(RtToken aName, const CParameterList &params)
		{
			name(aName);
			CParameterList::operator=(params);
		}

		/** @brief Sets the name (token) of the list.
		 *
		 *  @param aName Name (token) to set.
		 */
		inline void name(RtToken aName)
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
