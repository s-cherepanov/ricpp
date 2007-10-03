#ifndef _RICPP_RENDERSTATE_ATTRIBUTES_H
#define _RICPP_RENDERSTATE_ATTRIBUTES_H

#ifndef _RICPP_RENDERSTATE_OPTIONSBASE_H
#include "ricpp/renderstate/optionsbase.h"
#endif //  _RICPP_RENDERSTATE_OPTIONSBASE_H

namespace RiCPP {

	class CAttributesReader;

	/** @brief
	 */
	class CAttributes : public COptionsBase {
	private:
		CAttributesReader *m_reader;

		//
		// shading attributes
		//
		std::vector<RtFloat> m_color;		//!< Current reflective color (white - all 1.0), number of components may changed by option, norm is r, g, b.
		std::vector<RtFloat> m_opacity;		//!< Current opacity of an object (opaque - all 1.0), components as in color.

		bool m_inAreaLightSource;

	protected:
		virtual void deleteReader(CAttributesReader *reader);
		virtual CAttributesReader *newReader();

	public:
		inline CAttributes(const CColorDescr &c)
			: COptionsBase(c)
		{
			m_reader = 0;
		}

		inline CAttributes(const CAttributes &ra)
			: COptionsBase(ra.colorDescr())
		{
			m_reader = 0;
			*this = ra;
		}

		virtual ~CAttributes();

		CAttributes &operator=(const CAttributes &ra);


		inline const CAttributesReader &createReader()
		{
			if ( !m_reader ) {
				m_reader = newReader();
				if ( !m_reader ) {
					// throw;
				}
			}
			return *m_reader;
		}

		inline const CAttributesReader *reader() const
		{
				return m_reader;
		}

		virtual RtVoid color(RtColor Cs);
		inline virtual const std::vector<RtFloat> &color() const
		{
			return m_color;
		}

		virtual RtVoid opacity(RtColor Os);
		inline virtual const std::vector<RtFloat> &opacity() const
		{
			return m_opacity;
		}

		virtual RtVoid surfaceV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		virtual RtVoid atmosphereV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		virtual RtVoid interiorV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		virtual RtVoid exteriorV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff);
		virtual RtVoid displacementV(CDeclarationDictionary &dict, RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
		virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
		virtual RtVoid shadingRate(RtFloat size);
		virtual RtVoid shadingInterpolation(RtToken type);
		virtual RtVoid matte(RtBoolean onoff);
		virtual RtVoid bound(RtBound aBound);
		virtual RtVoid detail(RtBound aBound);
		virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
		virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
		virtual RtVoid geometricRepresentation(RtToken type);
		virtual RtVoid orientation(RtToken anOrientation);
		virtual RtVoid reverseOrientation(void);
		virtual RtVoid sides(RtInt nsides);
		virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
		virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);

	}; // CAttributes


	/** @brief
	 */
	class CAttributesReader : public COptionsBaseReader {
	private:
		const CAttributes *m_riAttribs;

	public:
		CAttributesReader(CAttributes &attribs)
			: COptionsBaseReader(attribs)
		{
			m_riAttribs = &attribs;
		}

		inline virtual const std::vector<RtFloat> &color() const
		{
			return m_riAttribs->color();
		}

		inline virtual const std::vector<RtFloat> &opacity() const
		{
			return m_riAttribs->opacity();
		}
	}; // CAttributesReader

	/** @brief
	 */
	class CAttributesFactory
	{
	protected:
		inline virtual CAttributes *newAttributesInstance(const CColorDescr &c)
		{
			return new CAttributes(c);
		}
		inline virtual CAttributes *newAttributesInstance(const CAttributes &attr)
		{
			return new CAttributes(attr);
		}

	public:
		inline virtual ~CAttributesFactory() {}
		inline virtual CAttributes *newAttributes(const CColorDescr &c)
		{
			CAttributes *a = newAttributesInstance(c);
			if ( a ) {
				a->createReader(); 
			} else {
				// throw
			}
			return a;
		}
		inline virtual CAttributes *newAttributes(const CAttributes &attr)
		{
			CAttributes *a = newAttributesInstance(attr);
			if ( a ) {
				a->createReader(); 
			} else {
				// throw
			}
			return a;
		}
		inline virtual void deleteAttributes(CAttributes *a)
		{
			if ( a )
				delete a;
		}
	}; // CAttributesFactory
}

#endif // _RICPP_RENDERSTATE_ATTRIBUTES_H