#ifndef _RICPP_BASERENDERER_OPTIONSBASE_H
#define _RICPP_BASERENDERER_OPTIONSBASE_H

#ifndef _RICPP_BASERENDERER_PARAMETER_H
#include "ricpp/baserenderer/parameter.h"
#endif // _RICPP_BASERENDERER_PARAMETER_H

namespace RiCPP {

	class COptionsBaseReader;

	/** @brief
	 */
	class COptionsBase {
	private:
		typedef std::map<std::string, const CNamedParameterList *> Map_type;
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

		const CColorDescr &colorDesc() const
		{
			return m_curColorDesc;
		}
	}; // COptionsBase


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

		const CColorDescr &colorDesc() const
		{
			return m_attribs->colorDesc();
		}
	}; // COptionsBaseReader

}

#endif // _RICPP_BASERENDERER_OPTIONSBASE_H
