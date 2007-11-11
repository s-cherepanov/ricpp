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

	class COptionsBaseReader;

	/** @brief
	 */
	class COptionsBase {
	private:
		typedef std::map<std::string, CNamedParameterList *> Map_type;
		std::list<CNamedParameterList> m_paramList;
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
			RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		void set(
			RtString name, const CParameterList &params);

		CNamedParameterList *getWriteable(const char *name);
		const CNamedParameterList *get(const char *name) const;

		const CParameter *get(const char *name, const char *token) const;


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

		bool erase(const char *name);
		bool erase(CNamedParameterList *paramList);

		inline RtVoid colorSamples(RtInt nColorSamples, RtFloat *nRGB, RtFloat *RGBn)
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

#if 0
	/** @brief
	 */
	class COptionsBaseReader {
	private:
		const COptionsBase *m_attribs;

	public:
		inline COptionsBaseReader(const COptionsBase &attribs)
		{
			m_attribs = &attribs;
		}
		inline virtual ~COptionsBaseReader() {}

		inline const CParameterList *get(const char *name) const
		{
			return m_attribs->get(name);
		}

		inline const CParameter *get(const char *name, const char *token) const
		{
			return m_attribs->get(name, token);
		}

		inline virtual RtVoid getColorSamples(RtInt &nColorSamples, std::vector<RtFloat> &nRGB, std::vector<RtFloat> &RGBn) const
		{
			return m_attribs->getColorSamples(nColorSamples, nRGB, RGBn);
		}

		inline virtual RtInt colorSamples() const
		{
			return m_attribs->colorSamples();
		}

		const CColorDescr &colorDescr() const
		{
			return m_attribs->colorDescr();
		}
	}; // COptionsBaseReader
#endif // 0

}

#endif // _RICPP_RICONTEXT_OPTIONSBASE_H
