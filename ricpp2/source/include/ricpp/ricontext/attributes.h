#ifndef _RICPP_RICONTEXT_ATTRIBUTES_H
#define _RICPP_RICONTEXT_ATTRIBUTES_H


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

/** @file attributes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling of the render attributes (used by render state).
 */

#ifndef _RICPP_RICONTEXT_OPTIONSBASE_H
#include "ricpp/ricontext/optionsbase.h"
#endif // _RICPP_RICONTEXT_OPTIONSBASE_H

#ifndef _RICPP_RICPP_RICPPCONST_H
#include "ricpp/ricpp/ricppconst.h"
#endif // _RICPP_RICPP_RICPPCONST_H

namespace RiCPP {

	/** @brief Shading attributes.
	 */
	class CAttributes : public COptionsBase {
	public:
		static const RtFloat defColorComponent;
		static const RtFloat defOpacityComponent;

		static const RtFloat defTextureS1;
		static const RtFloat defTextureT1;
		static const RtFloat defTextureS2;
		static const RtFloat defTextureT2;
		static const RtFloat defTextureS3;
		static const RtFloat defTextureT3;
		static const RtFloat defTextureS4;
		static const RtFloat defTextureT4;

		static const RtFloat defShadingRate;
		static const RtToken defShadingInterpolation;
		static const RtBoolean defMatte;
		
		static const RtFloat defMinVis;
		static const RtFloat defLowTran;
		static const RtFloat defUpTran;
		static const RtFloat defMaxVis;

		static const RtToken defGeometricApproximationType;
		static const RtFloat defGeometricApproximationValue;

		static const RtToken defGeometricRepresentation;

		static const RtToken defOrientation;

		static const RtInt defNSides;

		/** @brief Vector of (illuminated) light sources.
		 */
		typedef std::vector<RtLightHandle> TypeLightHandles;

	private:
		std::vector<RtFloat> m_color;          //!< Current reflective color (white - all 1.0), number of components may changed by option, norm is r, g, b.
		std::vector<RtFloat> m_opacity;        //!< Current opacity of an object (opaque - all 1.0), components as in color.

		TypeLightHandles m_illuminated;        //!< Illuminated lights, default: empty.

		RtToken m_surfaceName;                 //!< Surface shader name.
		CParameterList m_surfaceParams;        //!< Surface shader parameters.

		RtToken m_atmosphereName;              //!< Atmosphere shader name.
		CParameterList m_atmosphereParams;     //!< Atmosphere shader parameters.

		RtToken m_interiorName;                //!< Interior shader name.
		CParameterList m_interiorParams;       //!< Interior shader parameters.

		RtToken m_exteriorName;                //!< Exterior shader name.
		CParameterList m_exteriorParams;       //!< Exterior shader parameters.

		RtToken m_displacementName;            //!< Displacement shader name.
		CParameterList m_displacementParams;   //!< Displacement shader parameters.

		RtFloat m_textureCoordinates[8];       //!< Texture coordinates s1,t1 .. s4,t4 unit square [0,0, 1,0, 0,1, 1,1].

		RtFloat m_shadingRate;                 //!< Current shading rate in pixels (def. 1), infinity -> once per primitive.
		RtToken m_shadingInterpolation;        //!< Interpolation between pixels, "constant", "smooth" (def. RI_SMOOTH).
		RtBoolean m_matte;                     //!< subsequent object are 'matte' objects? (def. RI_FALSE).

		RtBound   m_bound;                     //!< Bounding box for subsequent primitives
		bool      m_boundCalled;               //!< Bounding box is set by an interface call

		RtBound   m_detail;                    //!< Level of detail
		bool      m_detailCalled;              //!< Level of detail is set by an interface call

		RtFloat   m_detailRange[4];            //!< The detail ranges
		bool      m_detailRangeCalled;         //!< Detail ranges are set by an interface call

		RtToken m_geometricApproximationType;  //!< The geometric approximation type (e.g. RI_FLATNESS)
		RtFloat m_geometricApproximationValue; //!< The value for the approximation type

		RtToken m_geometricRepresentation;     //!< The geometric representation

		RtToken m_orientation;                 //!< started RI_OUTSIDE, set by orientation and changed by transformations

		RtInt   m_nSides;                      //!< 1 or 2, def. is 2 (inside and outside)

		RtInt m_uStep,                         //!< Step size in u direction used for control points of bicubic spline meshes
			  m_vStep;                         //!< Step size in V direction used for control points of spline meshes
		RtBasis m_uBasis,                      //!< Basis matrix for bicubic splines in u direction
				m_vBasis;                      //!< Basis matrix for splines in v direction

		CTrimCurveData m_trimCurve;            //!< Tirmcurve, default: empty

		void init();

		void initColor();
		void initOpacity();
		void initTextureCoordinates();
		void initSurface();
		void initAtmosphere();
		void initInterior();
		void initExterior();
		void initDisplacement();
		void initShadingRate();
		void initShadingInterpolation();
		void initMatte();
		void initBound();
		void initDetail();
		void initDetailRange();
		void initGeometricApproximation();
		void initGeometricRepresentation();
		void initOrientation();
		void initSides();
		void initBasis();

		void initTrimCurve();

	public:
		inline CAttributes(const CColorDescr &c)
			: COptionsBase(c)
		{
			init();
		}

		inline CAttributes(const CAttributes &ra)
			: COptionsBase(ra.colorDescr())
		{
			*this = ra;
		}

		virtual ~CAttributes();

		CAttributes &operator=(const CAttributes &ra);

		inline virtual CAttributes *duplicate()
		{
			return new CAttributes(*this);
		}

		virtual RtVoid color(RtColor Cs);
		virtual RtFloat color(RtInt i) const;
		// inline virtual const std::vector<RtFloat> &color() const
		// {
		// 	return m_color;
		// }

		virtual RtVoid opacity(RtColor Os);
		virtual RtFloat opacity(RtInt i) const;
		// inline virtual const std::vector<RtFloat> &opacity() const
		// {
		// 	return m_opacity;
		// }

		virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff);
		virtual RtBoolean illuminated(RtLightHandle light) const;

		inline virtual TypeLightHandles::size_type illuminatedSize() const
		{
			return m_illuminated.size();
		}

		inline virtual TypeLightHandles::const_iterator illuminatedBegin() const
		{
			return m_illuminated.begin();
		}

		inline virtual TypeLightHandles::const_iterator illuminatedEnd() const
		{
			return m_illuminated.end();
		}

		inline virtual TypeLightHandles::iterator illuminatedBegin()
		{
			return m_illuminated.begin();
		}

		inline virtual TypeLightHandles::iterator illuminatedEnd()
		{
			return m_illuminated.end();
		}

		virtual RtVoid surface(RtToken name, const CParameterList &params);
		inline virtual RtToken surfaceName() const
		{
			return m_surfaceName;
		}
		inline virtual const CParameterList &surfaceParameters() const
		{
			return m_surfaceParams;
		}

		virtual RtVoid atmosphere(RtToken name, const CParameterList &params);
		inline virtual RtToken atmosphereName() const
		{
			return m_atmosphereName;
		}
		inline virtual const CParameterList &atmosphereParameters() const
		{
			return m_atmosphereParams;
		}

		virtual RtVoid interior(RtToken name, const CParameterList &params);
		inline virtual RtToken interiorName() const
		{
			return m_interiorName;
		}
		inline virtual const CParameterList &interiorParameters() const
		{
			return m_interiorParams;
		}

		virtual RtVoid exterior(RtToken name, const CParameterList &params);
		inline virtual RtToken exteriorName() const
		{
			return m_exteriorName;
		}
		inline virtual const CParameterList &exteriorParameters() const
		{
			return m_exteriorParams;
		}

		virtual RtVoid displacement(RtToken name, const CParameterList &params);
		inline virtual RtToken displacementName() const
		{
			return m_exteriorName;
		}
		inline virtual const CParameterList &displacementParameters() const
		{
			return m_exteriorParams;
		}

		virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
		virtual void getTextureCoordinates(RtFloat &s1, RtFloat &t1, RtFloat &s2, RtFloat &t2, RtFloat &s3, RtFloat &t3, RtFloat &s4, RtFloat &t4) const;

		virtual RtVoid shadingRate(RtFloat size);
		inline virtual RtFloat shadingRate() const
		{
				return m_shadingRate;
		}

		virtual RtVoid shadingInterpolation(RtToken type);
		inline virtual RtToken shadingInterpolation() const
		{
				return m_shadingInterpolation;
		}

		virtual RtVoid matte(RtBoolean onoff);
		inline virtual RtBoolean matte() const
		{
				return m_matte;
		}

		virtual RtVoid bound(RtBound aBound);
		inline virtual bool boundCalled() const
		{
			return m_boundCalled;
		}

		virtual RtVoid detail(RtBound aBound);
		inline virtual bool detailCalled() const
		{
			return m_detailCalled;
		}

		virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
		inline virtual bool detailRangeCalled() const
		{
			return m_detailRangeCalled;
		}
		virtual void getDetailRange(RtFloat &minvis, RtFloat &lowtran, RtFloat &uptran, RtFloat &maxvis) const;

		virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
		inline virtual RtToken geometricApproximationType() const
		{
			return m_geometricApproximationType;
		}
		inline virtual RtFloat geometricApproximationValue() const
		{
			return m_geometricApproximationValue;
		}

		virtual RtVoid geometricRepresentation(RtToken type);
		inline virtual RtToken geometricRepresentation() const
		{
			return m_geometricRepresentation;
		}

		virtual RtVoid orientation(RtToken anOrientation);
		inline virtual RtToken orientation(RtToken anOrientation) const
		{
			return m_orientation;
		}

		virtual RtVoid reverseOrientation(void);

		virtual RtVoid sides(RtInt nsides);
		inline virtual RtInt sides() const
		{
			return m_nSides;
		}

		virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
		virtual void getBasis(RtBasis ubasis, RtInt &ustep, RtBasis vbasis, RtInt &vstep) const;
		inline virtual RtInt uStep() const
		{
			return m_uStep;
		}
		inline virtual RtInt vStep() const
		{
			return m_vStep;
		}

		virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);
		virtual RtVoid trimCurve(const CTrimCurveData &trimCurveData);
		inline virtual const CTrimCurveData &trimCurve() const
		{
			return m_trimCurve;
		}
	}; // CAttributes


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
		virtual CAttributes *newAttributes(const CColorDescr &c);
		virtual CAttributes *newAttributes(const CAttributes &attr);
		inline virtual void deleteAttributes(CAttributes *a)
		{
			if ( a )
				delete a;
		}
	}; // CAttributesFactory
}

#endif // _RICPP_RICONTEXT_ATTRIBUTES_H
