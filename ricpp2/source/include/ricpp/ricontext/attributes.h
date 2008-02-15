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

#ifndef _RICPP_RICONTEXT_LIGHTS_H
#include "ricpp/ricontext/lights.h"
#endif // _RICPP_RICONTEXT_LIGHTS_H

namespace RiCPP {

	/** @brief Render attributes.
	 *
	 *  CAttribute is the basic set of RI attributes as described in [RiSpec].
	 *  A renderer can use an enlarged set of attributes by supplying an own
	 *  class inheriting from CAttributes, an additional matching factory
	 *  (specializes CAttributesFactory), and overloading the factory methods
	 *  CBaseRenderer::getNewAttributesFactory() and CBaseRenderer::deleteAttributesFactory().
	 *  Also some CAttributes members need to be overwritten (Copy constructor, assignment, duplication).
	 *  However, since generic attributes (Attribute request) are used and can be queried, a special class
	 *  for render specific attributes is possibly not necessary.
	 *
	 *  @see COptions
	 */
	class CAttributes : public COptionsBase {
	public:
		// Default color values
		static const RtFloat defColorComponent;   ///< Default value for color components (1.0) - not the number of components.

		// Default opacity values
		static const RtFloat defOpacityComponent; ///< Default value for opacity components (1.0) - not the number of components.

		// Default texture coordinates, corresponding the rectangle of parameteric
		// coordinates (u, v) of a parametric surface.
		static const RtFloat defTextureS1;        ///< Default texture coordinate s1 (0)
		static const RtFloat defTextureT1;        ///< Default texture coordinate t1 (0)
		static const RtFloat defTextureS2;        ///< Default texture coordinate s2 (1)
		static const RtFloat defTextureT2;        ///< Default texture coordinate t2 (0)
		static const RtFloat defTextureS3;        ///< Default texture coordinate s3 (0)
		static const RtFloat defTextureT3;        ///< Default texture coordinate t3 (1)
		static const RtFloat defTextureS4;        ///< Default texture coordinate s4 (1)
		static const RtFloat defTextureT4;        ///< Default texture coordinate t4 (1)

		// Default shading attributes.
		static const RtFloat defShadingRate;          ///< Default minimum shading rate, (1: once per pixel)
		static const RtToken defShadingInterpolation; ///< Default shading interpolation (RI_CONSTANT)
		static const RtBoolean defMatte;              ///< Default matte mode (false: off)

		// Default level of detail range, per default the model is visible.
		static const RtFloat defMinVis;  ///< Minimal area covered by a model to be visible (0)
		static const RtFloat defLowTran; ///< Minimal area covered by a model to fully visible (0)
		static const RtFloat defUpTran;  ///< Maximal area covered by a model to fully visible (RI_INFINITY)
		static const RtFloat defMaxVis;  ///< Maximal area covered by a model to be visible (RI_INFINITY)

		// Type of geometric approximation
		static const RtToken defGeometricApproximationType;  ///< Default approximation type (RI_FLATNESS, chordal deviation)
		static const RtFloat defGeometricApproximationValue; ///< Largest deviation of a point of an approximated surface to the rendered pixel in raster coordinates (0.5 ad hoc)

		static const RtToken defGeometricRepresentation; ///< Default representation is RI_PRIMITIVE, a shaded primitive

		static const RtToken defOrientation; ///< Default surface orientation is RI_OUTSIDE

		static const RtInt defNSides;        ///< Default number of rendered surface sides (2)

		typedef std::vector<CLightSource *> TypeLightHandles; ///< Vector of (illuminated, switched on) light sources, not the list of global lights.

	private:
		std::vector<RtFloat> m_color;          ///< Current reflective color (white - all 1.0), number of components may changed by option, norm is r, g, b.
		std::vector<RtFloat> m_opacity;        ///< Current opacity of an object (opaque - all 1.0), components as in color.

		TypeLightHandles m_illuminated;        ///< Illuminated lights, default: empty.

		RtToken m_surfaceName;                 ///< Surface shader name.
		CParameterList m_surfaceParams;        ///< Surface shader parameters.

		RtToken m_atmosphereName;              ///< Atmosphere shader name.
		CParameterList m_atmosphereParams;     ///< Atmosphere shader parameters.

		RtToken m_interiorName;                ///< Interior shader name.
		CParameterList m_interiorParams;       ///< Interior shader parameters.

		RtToken m_exteriorName;                ///< Exterior shader name.
		CParameterList m_exteriorParams;       ///< Exterior shader parameters.

		RtToken m_displacementName;            ///< Displacement shader name.
		CParameterList m_displacementParams;   ///< Displacement shader parameters.

		RtFloat m_textureCoordinates[8];       ///< Texture coordinates s1,t1 .. s4,t4 unit square [0,0, 1,0, 0,1, 1,1].

		RtFloat m_shadingRate;                 ///< Current shading rate in pixels (def. 1). If infinity, once per primitive.
		RtToken m_shadingInterpolation;        ///< Interpolation between pixels, "constant", "smooth" (def. RI_SMOOTH).
		RtBoolean m_matte;                     ///< subsequent object are 'matte' objects? (def. RI_FALSE).

		RtBound   m_bound;                     ///< Bounding box for subsequent primitives
		bool      m_boundCalled;               ///< Bounding box is set by an interface call

		RtBound   m_detail;                    ///< Level of detail
		bool      m_detailCalled;              ///< Level of detail is set by an interface call

		RtFloat   m_detailRange[4];            ///< The detail ranges
		bool      m_detailRangeCalled;         ///< Detail ranges are set by an interface call

		RtToken m_geometricApproximationType;  ///< The geometric approximation type (e.g. RI_FLATNESS)
		RtFloat m_geometricApproximationValue; ///< The value for the approximation type

		RtToken m_geometricRepresentation;     ///< The geometric representation

		RtToken m_orientation;                 ///< started RI_OUTSIDE, set by orientation and changed by transformations

		RtInt   m_nSides;                      ///< 1 or 2, def. is 2 (inside and outside)

		RtInt m_uStep,                         ///< Step size in u direction used for control points of bicubic spline meshes
			  m_vStep;                         ///< Step size in V direction used for control points of spline meshes
		RtBasis m_uBasis,                      ///< Basis matrix for bicubic splines in u direction
				m_vBasis;                      ///< Basis matrix for splines in v direction

		CTrimCurveData m_trimCurve;            ///< Tirmcurve, default: empty

		/** @brief Initializes all members with their standard values.
		 */
		void init();

		/** @brief Initializes the color members.
		 */
		void initColor();

		/** @brief Initializes the opacity members.
		 */
		void initOpacity();

		/** @brief Initializes the (s, t) texture coordinate members.
		 */
		void initTextureCoordinates();

		/** @brief Initializes the surface shader (empty).
		 */
		void initSurface();

		/** @brief Initializes the atmosphere volume shader (empty).
		 */
		void initAtmosphere();

		/** @brief Initializes the interior volume shader (empty).
		 */
		void initInterior();

		/** @brief Initializes the exterior volume shader (empty).
		 */
		void initExterior();

		/** @brief Initializes the displacement shader (empty).
		 */
		void initDisplacement();

		/** @brief Initializes the shading rate.
		 */
		void initShadingRate();

		/** @brief Initializes the shading interpolation.
		 */
		void initShadingInterpolation();

		/** @brief Initializes the matte flag.
		 */
		void initMatte();

		/** @brief Initializes the bounding box (infinite).
		 */
		void initBound();

		/** @brief Initializes the current level of detail (infinite).
		 */
		void initDetail();

		/** @brief Initializes the current range for level of detail calculation.
		 */
		void initDetailRange();

		/** @brief Initializes the geometric approximation.
		 */
		void initGeometricApproximation();

		/** @brief Initializes the geometric representation.
		 */
		void initGeometricRepresentation();

		/** @brief Initializes the surface orientation.
		 */
		void initOrientation();

		/** @brief Initializes the number of surface sides.
		 */
		void initSides();

		/** @brief Initializes the spline basis matrix.
		 */
		void initBasis();


		/** @brief Initializes the NURBS trim curve.
		 */
		void initTrimCurve();

	public:
		/** @brief Constructor.
		 *
		 *  A color descriptor must be supplied to handle
		 *  the color and opacity values.
		 *
		 *  @param c Current color descriptor.
		 */
		inline CAttributes(const CColorDescr &c)
			: COptionsBase(c)
		{
			init();
		}

		/** @brief Copy constructor.
		 *
		 *  @param ra Attribute container to copy from.
		 */
		inline CAttributes(const CAttributes &ra)
			: COptionsBase(ra.colorDescr())
		{
			*this = ra;
		}

		/** @brief Virtual destructor.
		 */
		inline virtual ~CAttributes() {}

		/** @brief Assignment operator.
		 *
		 *  @param ra Attribute container to copy from.
		 *  @return *this.
		 */
		CAttributes &operator=(const CAttributes &ra);

		/** @brief Object duplication.
		 *
		 *  @return A clone of *this.
		 */
		inline virtual CAttributes *duplicate()
		{
			return new CAttributes(*this);
		}

		/** @brief Sets the current surface color.
		 *
		 *  @param Cs Surface color.
		 */
		virtual RtVoid color(RtColor Cs);

		/** @brief Gets a component of the current surface color.
		 *
		 *  @return The component @a i of the current surface color, 0 if @a i is out of range.
		 */
		virtual RtFloat color(RtInt i) const;

		/** @brief Gets the current surface color.
		 *
		 *  @return Constant vector containing the components of the current surface color.
		 */
		inline virtual const std::vector<RtFloat> &color() const
		{
			return m_color;
		}

		/** @brief Sets the current surface opacity.
		 *
		 *  @param Os Surface opacity.
		 */
		virtual RtVoid opacity(RtColor Os);

		/** @brief Gets a component of the current surface opacity.
		 *
		 *  The number of components is the same as the one of a surface color.
		 *
		 *  @return The component @a i of the current surface opacity, 0 if @a i is out of range.
		 */
		virtual RtFloat opacity(RtInt i) const;

		/** @brief Gets the current surface opacity.
		 *
		 *  @return Constant vector containing the components of the current surface opacity.
		 */
		inline virtual const std::vector<RtFloat> &opacity() const
		{
			return m_opacity;
		}

		/** @brief Switch a lightsource on or off.
		 *
		 *  @param light Handle of a light source to switch
		 *  @param onoff True, to switch on. False, to switch off.
		 */
		virtual RtVoid illuminate(CLightSource *light, RtBoolean onoff);

		/** @brief Gets the state of a light source.
		 *
		 *  @param light Handle of the light source
		 *  @return True, if switched on. false, to switched off.
		 */
		virtual RtBoolean illuminated(CLightSource *light) const;

		/** @brief Gets the number of illuminated light sources.
		 *
		 *  @return Number of illuminated light sources.
		 */
		inline virtual TypeLightHandles::size_type illuminatedSize() const
		{
			return m_illuminated.size();
		}

		/** @brief Gets the constant iterator for the illuminated light source.
		 *
		 *  @return Constant iterator, points the first illuminated light source.
		 */
		inline virtual TypeLightHandles::const_iterator illuminatedBegin() const
		{
			return m_illuminated.begin();
		}

		/** @brief Gets the constant iterator (end) for the illuminated light source.
		 *
		 *  @return Constant iterator, points behind the last illuminated light source.
		 */
		inline virtual TypeLightHandles::const_iterator illuminatedEnd() const
		{
			return m_illuminated.end();
		}

		/** @brief Gets the iterator for the illuminated light source.
		 *
		 *  @return Iterator, points the first illuminated light source.
		 */
		inline virtual TypeLightHandles::iterator illuminatedBegin()
		{
			return m_illuminated.begin();
		}

		/** @brief Gets the Iterator (end) for the illuminated light source.
		 *
		 *  @return Iterator, points behind the last illuminated light source.
		 */
		inline virtual TypeLightHandles::iterator illuminatedEnd()
		{
			return m_illuminated.end();
		}

		/** @brief Sets the current surface shader.
		 *
		 *  @param name Name of the surface shader.
		 *  @param params Parameter list of the surface shader.
		 */
		virtual RtVoid surface(RtToken name, const CParameterList &params);

		/** @brief Gets the name of the current surface shader.
		 *  @return The name of the current surface shader.
		 */
		inline virtual RtToken surfaceName() const
		{
			return m_surfaceName;
		}

		/** @brief Gets the constant parameter list of the current surface shader.
		 *  @return The constant parameter list of the current surface shader.
		 */
		inline virtual const CParameterList &surfaceParameters() const
		{
			return m_surfaceParams;
		}

		/** @brief Sets the current atmosphere volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid atmosphere(RtToken name, const CParameterList &params);

		/** @brief Gets the name of the current atmosphere volume shader.
		 *  @return The name of the current atmosphere volume shader.
		 */
		inline virtual RtToken atmosphereName() const
		{
			return m_atmosphereName;
		}

		/** @brief Gets the constant parameter list of the current atmosphere volume shader.
		 *  @return The constant parameter list of the current atmosphere volume shader.
		 */
		inline virtual const CParameterList &atmosphereParameters() const
		{
			return m_atmosphereParams;
		}

		/** @brief Sets the current interior volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid interior(RtToken name, const CParameterList &params);

		/** @brief Gets the name of the current interior volume shader.
		 *  @return The name of the current interior volume shader.
		 */
		inline virtual RtToken interiorName() const
		{
			return m_interiorName;
		}

		/** @brief Gets the constant parameter list of the current interior volume shader.
		 *  @return The constant parameter list of the current interior volume shader.
		 */
		inline virtual const CParameterList &interiorParameters() const
		{
			return m_interiorParams;
		}

		/** @brief Sets the current exterior volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid exterior(RtToken name, const CParameterList &params);

		/** @brief Gets the name of the current exterior volume shader.
		 *  @return The name of the current exterior volume shader.
		 */
		inline virtual RtToken exteriorName() const
		{
			return m_exteriorName;
		}

		/** @brief Gets the constant parameter list of the current exterior volume shader.
		 *  @return The constant parameter list of the current exterior volume shader.
		 */
		inline virtual const CParameterList &exteriorParameters() const
		{
			return m_exteriorParams;
		}

		/** @brief Sets the current displacement shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid displacement(RtToken name, const CParameterList &params);

		/** @brief Gets the name of the current displacement shader.
		 *  @return The name of the current exterior displacement shader.
		 */
		inline virtual RtToken displacementName() const
		{
			return m_exteriorName;
		}

		/** @brief Gets the constant parameter list of the current displacement shader.
		 *  @return The constant parameter list of the current displacement shader.
		 */
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

		virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);
		virtual RtVoid trimCurve(const CTrimCurveData &trimCurveData);
		inline virtual const CTrimCurveData &trimCurve() const
		{
			return m_trimCurve;
		}
	}; // CAttributes


	/** @brief Create new attributes containers.
	 *
	 *  An instance of the CAttributesFactory is used to create new instances
	 *  of CAttributes containers. Renderers can supply an own factory to
	 *  create an expanded set of attributes.
	 */
	class CAttributesFactory
	{
	protected:
		/** @brief Factory method to get a new attribute set.
		 *
		 *  Must be overwritten to supply an own attribute set.
		 *
		 *  @param c Current color descriptor
		 */
		inline virtual CAttributes *newAttributesInstance(const CColorDescr &c)
		{
			return new CAttributes(c);
		}

		/** @brief Factory method to get a copy of an attribute set.
		 *
		 *  Must be overwritten to supply an own attribute set.
		 *
		 *  @param attr Attribute set to copy.
		 */
		inline virtual CAttributes *newAttributesInstance(const CAttributes &attr)
		{
			return new CAttributes(attr);
		}

	public:
		/** @brief Virtual destructor.
		 */
		inline virtual ~CAttributesFactory() {}

		/** @brief Gets a new attribute set.
		 *  @param c Current color descriptor.
		 *  @return A new Attribute set
		 */
		virtual CAttributes *newAttributes(const CColorDescr &c);

		/** @brief Gets a copy of an attribute set.
		 *  @param attr Attribute set to copy.
		 *  @return A new Attribute set
		 */
		virtual CAttributes *newAttributes(const CAttributes &attr);

		/** @brief Deletes an attribute set delivered by this factory.
		 *
		 *  Since the destructor is virtual, this method don_t need to be overwritten.
		 *
		 *	@brief a Attribute set to delete, had to be constructed previously by this factory.
		 */
		inline virtual void deleteAttributes(CAttributes *a)
		{
			if ( a )
				delete a;
		}
	}; // CAttributesFactory
}

#endif // _RICPP_RICONTEXT_ATTRIBUTES_H
