#ifndef _RICPP_RICONTEXT_OPTIONSBASE_H
#define _RICPP_RICONTEXT_OPTIONSBASE_H

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

/** @file optionbase.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The base class for options and attributes used for the render state.
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

namespace RiCPP {

	/** @brief Base class for attributes, options and controlls
	 *
	 *  Base class can be used to store generic options (attributes and controlls as well).
	 */
	class COptionsBase {
	private:
		/** @brief Type to map option names (tokenized) to their parameters, stored at m_paramList.
		 */ 
		typedef std::map<RtToken, CNamedParameterList *> Map_type;
		
		/** @brief Options and their parameters.
		 *
		 *  The name of a CNamedParameterList is the name of a option.
		 */ 
		std::list<CNamedParameterList> m_paramList;
		
		/** @brief Maps object tokens to their parameters.
		 */
		Map_type m_paramMap;

		inline void clearMembers()
		{
			m_paramMap.clear();
			m_paramList.clear();
		}

		CColorDescr m_curColorDesc;

	public:
		typedef Map_type::const_iterator const_iterator;
		typedef Map_type::size_type size_type;

		inline COptionsBase() {}
		inline COptionsBase(const CColorDescr cd)
		{
			m_curColorDesc = cd;
		}

		inline COptionsBase(const COptionsBase &ga)
		{
			*this = ga;
		}
		inline virtual ~COptionsBase() {}


		inline virtual void clear()
		{
			clearMembers();
		}

		virtual COptionsBase &operator=(const COptionsBase &ga);

		void set(
			CDeclarationDictionary &dict,
			RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
			
		void set(
			RtToken name, const CParameterList &params);

		CNamedParameterList *get(RtToken name);
		const CNamedParameterList *get(RtToken name) const;

		const CParameter *get(RtToken name, RtToken token) const;


		inline const_iterator begin() const
		{
			return m_paramMap.begin();
		}

		inline const_iterator end() const
		{
			return m_paramMap.end();
		}

		inline size_type size() const
		{
			return m_paramMap.size();
		}

		bool erase(RtToken name);
		bool erase(CNamedParameterList *paramList);

		inline RtVoid colorSamples(RtInt nColorSamples, RtFloat nRGB[], RtFloat RGBn[])
		{
			m_curColorDesc.colorSamples(nColorSamples, nRGB, RGBn);
		}

		inline RtVoid getColorSamples(RtInt &nColorSamples, std::vector<RtFloat> &nRGB, std::vector<RtFloat> &RGBn) const
		{
			return m_curColorDesc.getColorSamples(nColorSamples, nRGB, RGBn);
		}

		inline RtInt colorSamples() const
		{
			return m_curColorDesc.colorSamples();
		}

		const CColorDescr &colorDescr() const
		{
			return m_curColorDesc;
		}
	}; // COptionsBase
}

#endif // _RICPP_RICONTEXT_OPTIONSBASE_H
