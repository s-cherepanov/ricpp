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

#ifndef _RICPP_RICONTEXT_TRANSFORMATION_H
#include "ricpp/ricontext/transformation.h"
#endif // _RICPP_RICONTEXT_TRANSFORMATION_H

#include <deque>

namespace RiCPP {
	class CAttributesFactory;

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
		friend class CAttributesFactory;
		CAttributesFactory *m_factory;

	public:
		const CAttributesFactory *factory() const { return m_factory; }

	protected:
		void factory(CAttributesFactory *aFactory) { m_factory = aFactory; }

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

	protected:
		struct SBasisParts {
			RtInt m_step;      ///< Step size used for control points of bicubic spline meshes
			RtBasis m_basis;   ///< Basis matrix for bicubic splines

			inline SBasisParts() {
				m_step = 0;
				memset(m_basis, 0, sizeof(m_basis));
			}
			inline SBasisParts(const SBasisParts &c) { *this = c; }
			inline virtual ~SBasisParts() {}
			inline virtual SBasisParts *duplicate() const { return new SBasisParts(*this); }
			inline SBasisParts &operator=(const SBasisParts &s)
			{
				if ( this == &s )
					return *this;

				m_step = s.m_step;
				memcpy(m_basis, s.m_basis, sizeof(RtBasis));

				return *this;
			}
		}; // SBasisParts

		struct SShaderParts {
			RtToken m_name;
			CParameterList m_params;

			inline SShaderParts() { m_name = RI_NULL; }
			inline SShaderParts(const SShaderParts &c) { *this = c; }
			inline virtual ~SShaderParts() {}
			inline virtual SShaderParts *duplicate() const { return new SShaderParts(*this); }
			inline SShaderParts &operator=(const SShaderParts &s)
			{
				if ( this == &s )
					return *this;

				m_name = s.m_name;
				m_params = s.m_params;

				return *this;
			}
		}; // SShaderParts

		class IMovedValue {
		public:
			inline IMovedValue() {}
			inline virtual ~IMovedValue() {}
			
			virtual void clear() = 0;
			virtual void fill(RtInt n) = 0;
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times) = 0;
			virtual void sampleReset() = 0;
		}; // IMovedValue
		
		template<typename ValueType> class TMovedAttribute : public IMovedValue {
		public:
			ValueType m_value;
			std::vector<ValueType> m_movedValue;
			unsigned long m_motionBegin, m_motionEnd;

			inline TMovedAttribute() {}
			inline TMovedAttribute(const TMovedAttribute &c) { *this = c; }
			inline virtual ~TMovedAttribute() {}
			inline virtual IMovedValue *duplicate() const { return new TMovedAttribute(*this); }
			
			inline virtual void clear()
			{
				m_motionBegin = m_motionEnd = 0;
				m_movedValue.clear();
			}

			inline virtual void fill(RtInt n)
			{
				if ( n == 0 ) {
					return;
				}
				for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
					m_movedValue[i] = m_movedValue[i-1];
				}
			}

			inline virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times)
			{
			}

			inline virtual void sampleReset()
			{
				if ( m_movedValue.size() >= 1 )
					m_value = m_movedValue[0];
			}

			inline TMovedAttribute &operator=(const TMovedAttribute &c)
			{
				if ( this == &c )
					return *this;

				clear();
				m_value = c.m_value;
				m_movedValue = c.m_movedValue;
				m_motionBegin = c.m_motionBegin;
				m_motionEnd = c.m_motionEnd;

				return *this;
			}

			inline void set(const ValueType aValue, RtInt n, unsigned long moBegin, unsigned long moEnd)
			{
				if ( moBegin > moEnd ) {
					std::swap(moBegin, moEnd);		
				}
				
				m_motionBegin = moBegin;
				m_motionEnd = moEnd;

				if ( n == 0 ) {
					m_value = aValue;
				}

				if ( moBegin < moEnd ) {
					if ( m_movedValue.size() < moEnd - moBegin ) {
						m_movedValue.resize(moEnd - moBegin);
					}
					if ( (unsigned long)n >= moEnd - moBegin ) {
						// ERROR
						return;
					}
					m_movedValue[n] = aValue;
				}
			}

			inline void set(const ValueType aValue)
			{
				m_motionBegin = 0;
				m_motionEnd = 0;
				m_movedValue.clear();

				m_value = aValue;
			}
			
			inline void get(ValueType &aValue) const
			{
				aValue = m_value;
			}
		}; // TMovedAttribute

		class CAttributeInt : public TMovedAttribute<RtInt> {
		public:
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
		}; // CAttributeInt
		
		class CAttributeToken : public TMovedAttribute<RtToken> {
		public:
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
		}; // CAttributeToken

		class CAttributeFloat : public TMovedAttribute<RtFloat> {
		public:
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
		}; // CAttributeFloat

		class CAttributeTrimCurve : public TMovedAttribute<CTrimCurveData> {
		public:
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
		}; // CAttributeTrimCurve
		
		class CAttributeBasis : public TMovedAttribute<SBasisParts> {
		public:
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
			void set(const RtBasis basis, RtInt step, RtInt n, unsigned long moBegin, unsigned long moEnd);
			void set(const RtBasis basis, RtInt step);
		}; // CAttributeBasis
		
		class CAttributeShader : public TMovedAttribute<SShaderParts> {
		public:
			typedef TMovedAttribute<SShaderParts> TypeParent;

			CTransformation *m_shaderTransform;

			inline CAttributeShader() { m_shaderTransform = 0; }
			inline virtual ~CAttributeShader() {}
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
			virtual void sampleReset();
			
			void set(RtToken name, const CParameterList &params, const CTransformation &transform, RtInt n, unsigned long moBegin, unsigned long moEnd);
			void set(RtToken name, const CParameterList &params, const CTransformation &transform);

			inline CAttributeShader &operator=(const CAttributeShader &c) {
				if ( this == &c )
					return *this;

				if ( m_shaderTransform ) delete m_shaderTransform;
				m_shaderTransform = c.m_shaderTransform ? c.m_shaderTransform->duplicate() : 0;

				TypeParent::operator=(c);
				return *this;
			}
		}; // CAttributeShader

		class CAttributeFloatArray : public IMovedValue {
		public:
			std::vector<RtFloat> m_value;
			std::vector<RtFloat> m_movedValue;
			unsigned long m_motionBegin, m_motionEnd;

			inline CAttributeFloatArray() {}
			inline CAttributeFloatArray(const CAttributeFloatArray &c) { *this = c; }
			inline virtual ~CAttributeFloatArray() {}
			inline virtual IMovedValue *duplicate() const { return new CAttributeFloatArray(*this); }

			virtual void clear();
			virtual void fill(RtInt n);
			virtual void sample(RtFloat shutterTime, const TypeMotionTimes &times);
			virtual void sampleReset();
			CAttributeFloatArray &operator=(const CAttributeFloatArray &c);
			void set(RtFloat aValue, RtInt n, unsigned long moBegin, unsigned long moEnd);
			void set(RtFloat *aValue, RtInt n, unsigned long moBegin, unsigned long moEnd);
			void set(RtFloat aValue, RtInt aCard);
			void set(RtFloat *aValue, RtInt aCard);
			void set(RtFloat aValue);
			void set(RtFloat *aValue);
			bool get(RtFloat *aValue) const;
			inline unsigned long card() const { return (unsigned long)m_value.size(); }
		}; // CAttributeFloatArray

		enum EnumAttributeIndex {
			AIDX_COLOR,
			AIDX_OPACITY,
			AIDX_TEXTURE_COORDINATES,
			AIDX_SHADING_RATE,
			AIDX_BOUND,
			AIDX_DETAIL,
			AIDX_DETAIL_RANGE,
			AIDX_GEOMETRIC_APPROXIMATION_VALUE,
			
			AIDX_SURFACE,
			AIDX_ATMOSPHERE,
			AIDX_INTERIOR,
			AIDX_EXTERIOR,
			AIDX_DISPLACEMENT,
			AIDX_DEFORMATION,
			
			AIDX_SHADING_INTERPOLATION,
			AIDX_GEOMETRIC_APPROXIMATION_TYPE,
			AIDX_GEOMETRIC_REPRESENTATION,
			AIDX_ORIENTATION,
			
			AIDX_MATTE,
			AIDX_SIDES,
			
			AIDX_BASIS,

			AIDX_TRIM_CURVE,

			AIDX_ENDMARKER
		}; ///!< Indizees for al atributes, used for @c m_allAttributes
		
		std::vector<IMovedValue *> m_allAttributes; ///< Pointer to all attributes of this class
		virtual void initAttributeVector();    ///< Initializes @c m_allAttributes with pointers to all attributes

		const CMotionState *m_motionState; ///< Points to motion state in motion blocks, 0 otherwise
		EnumAttributeIndex m_lastValue;    ///< Index o the previous request in motion block (used to fill blocks at motionEnd())
		
		CAttributeFloatArray m_color;          ///< Current reflective color (white - all 1.0), RtColor, number of components may changed by option, norm is r, g, b.

		CAttributeFloatArray m_opacity;        ///< Current opacity of an object (opaque - all 1.0), RtColor, components as in color.

		TypeLightHandles m_illuminated;        ///< Illuminated lights, default: empty. @todo default light source

		CAttributeShader m_surface;            ///< Surface shader
		CAttributeShader m_atmosphere;         ///< Atmosphere shader
		CAttributeShader m_interior;           ///< Interior shader name.
		CAttributeShader m_exterior;           ///< Exterior shader
		CAttributeShader m_displacement;       ///< Displacement shader name.
		CAttributeShader m_deformation;        ///< Deformation shader name.

		CAttributeFloatArray m_textureCoordinates; ///< Texture coordinates s1,t1 .. s4,t4 unit square [0,0, 1,0, 0,1, 1,1].

		CAttributeFloat m_shadingRate;         ///< Current shading rate in pixels (def. 1). If infinity, once per primitive.

		CAttributeToken m_shadingInterpolation; ///< Interpolation between pixels, "constant", "smooth" (def. RI_SMOOTH).
		CAttributeInt   m_matte;                ///< subsequent object are 'matte' objects? (def. RI_FALSE).

		CAttributeFloatArray m_bound;          ///< Bounding box for subsequent primitives (RtBound)
		bool                 m_boundCalled;    ///< Bounding box is set by an interface call

		CAttributeFloatArray m_detail;         ///< Level of detail (RtBound)
		bool                 m_detailCalled;   ///< Level of detail is set by an interface call

		CAttributeFloatArray m_detailRange;    ///< The detail ranges (4 floats)
		bool      m_detailRangeCalled;         ///< Detail ranges are set by an interface call
		bool      m_detailRangeCalledInBlock;  ///< Detail ranges are set by an interface call within the current attribute block.

		CAttributeToken m_geometricApproximationType;  ///< The geometric approximation type (e.g. RI_FLATNESS, "motionfactor") @todo There are many independend flags, not only one - have to store token-value pairs
		CAttributeFloat m_geometricApproximationValue; ///< The value for the approximation type

		CAttributeToken m_geometricRepresentation;     ///< The geometric representation
		
		CAttributeToken m_orientation;                 ///< started RI_OUTSIDE, set by orientation
		
		RtToken m_coordSysOrientation;                 ///< started RI_LH, changed by transformations
		RtToken m_primitiveOrientation;                ///< started RI_LH, set by orientation and changed by transformations
		
		CAttributeInt   m_nSides;                      ///< 1 or 2, def. is 2 (inside and outside)

		CAttributeBasis m_uBasis,                      ///< Basis matrix for bicubic splines in u direction
		                m_vBasis;                      ///< Basis matrix for splines in v direction

		CAttributeTrimCurve m_trimCurve;               ///< Trim curve, default: empty
		
		bool m_inAreaLight;                    ///< An area light source was created.
		
		unsigned long m_storeCounter;          ///< A simple counter (not copied) for additional attribute blocks stored for this at a stack (used for detailrange)

	public:
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

		/** @brief Initializes the deformation shader (empty).
		 */
		void initDeformation();

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
			init();
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
		virtual COptionsBase *duplicate() const;

		/** @brief Gets the area light definition state.
		 *  @return true, an area light definition is active
		 */
		bool CAttributes::inAreaLight() const;

		/** @brief Sets the area light definition state.
		 *  @param flag true, an area light definition is active
		 */
		void CAttributes::inAreaLight(bool flag);
		
		/** @brief Store counter is incremented by one to indicate that another attribute block is stored
		 */
		inline void incStoreCounter() { m_storeCounter++; }

		/** @brief Store counter is decremented by one to indicate that another attribute block is removed
		 */
		inline void decStoreCounter() { m_storeCounter--; }
		
		/** @brief Number of additional attribute blocks stored
		 */
		inline unsigned long storeCounter() const { return m_storeCounter; }

		/** @brief Sets the number of additional attribute blocks stored
		 */
		inline void storeCounter(unsigned long cnt) { m_storeCounter = cnt; }

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
			return m_color.m_value;
		}

		virtual bool getColor(RtColor Cs) const;

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
			return m_opacity.m_value;
		}

		virtual bool getOpacity(RtColor Os) const;

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
		 *  @param transform Current transformation
		 */
		virtual RtVoid surface(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current surface shader.
		 *  @return The name of the current surface shader.
		 */
		inline virtual RtToken surfaceName() const
		{
			return m_surface.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current surface shader.
		 *  @return The constant parameter list of the current surface shader.
		 */
		inline virtual const CParameterList &surfaceParameters() const
		{
			return m_surface.m_value.m_params;
		}

		/** @brief Gets the transformation of the current surface shader.
		 *  @return The constant transformation of the current surface shader.
		 */
		inline virtual const CTransformation *surfaceTransformation() const
		{
			return m_surface.m_shaderTransform;
		}

		/** @brief Sets the current atmosphere volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid atmosphere(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current atmosphere volume shader.
		 *  @return The name of the current atmosphere volume shader.
		 */
		inline virtual RtToken atmosphereName() const
		{
			return m_atmosphere.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current atmosphere volume shader.
		 *  @return The constant parameter list of the current atmosphere volume shader.
		 */
		inline virtual const CParameterList &atmosphereParameters() const
		{
			return m_atmosphere.m_value.m_params;
		}

		/** @brief Gets the transformation of the current atmosphere shader.
		 *  @return The constant transformation of the current atmosphere shader.
		 */
		inline virtual const CTransformation *atmosphereTransformation() const
		{
			return m_atmosphere.m_shaderTransform;
		}

		/** @brief Sets the current interior volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid interior(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current interior volume shader.
		 *  @return The name of the current interior volume shader.
		 */
		inline virtual RtToken interiorName() const
		{
			return m_interior.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current interior volume shader.
		 *  @return The constant parameter list of the current interior volume shader.
		 */
		inline virtual const CParameterList &interiorParameters() const
		{
			return m_interior.m_value.m_params;
		}

		/** @brief Gets the transformation of the current interior shader.
		 *  @return The constant transformation of the current interior shader.
		 */
		inline virtual const CTransformation *interiorTransformation() const
		{
			return m_interior.m_shaderTransform;
		}

		/** @brief Sets the current exterior volume shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid exterior(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current exterior volume shader.
		 *  @return The name of the current exterior volume shader.
		 */
		inline virtual RtToken exteriorName() const
		{
			return m_exterior.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current exterior volume shader.
		 *  @return The constant parameter list of the current exterior volume shader.
		 */
		inline virtual const CParameterList &exteriorParameters() const
		{
			return m_exterior.m_value.m_params;
		}

		/** @brief Gets the transformation of the current exterior shader.
		 *  @return The constant transformation of the exterior interior shader.
		 */
		inline virtual const CTransformation *exteriorTransformation() const
		{
			return m_exterior.m_shaderTransform;
		}

		/** @brief Sets the current displacement shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid displacement(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current displacement shader.
		 *  @return The name of the current exterior displacement shader.
		 */
		inline virtual RtToken displacementName() const
		{
			return m_displacement.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current displacement shader.
		 *  @return The constant parameter list of the current displacement shader.
		 */
		inline virtual const CParameterList &displacementParameters() const
		{
			return m_displacement.m_value.m_params;
		}

		/** @brief Gets the transformation of the current displacement shader.
		 *  @return The constant transformation of the displacement interior shader.
		 */
		inline virtual const CTransformation *displacementTransformation() const
		{
			return m_displacement.m_shaderTransform;
		}

		/** @brief Sets the current deformation shader.
		 *
		 *  @param name Name of the shader.
		 *  @param params Parameter list of the shader.
		 */
		virtual RtVoid deformation(RtToken name, const CParameterList &params, const CTransformation &transform);

		/** @brief Gets the name of the current deformation shader.
		 *  @return The name of the current exterior deformation shader.
		 */
		inline virtual RtToken deformationName() const
		{
			return m_deformation.m_value.m_name;
		}

		/** @brief Gets the constant parameter list of the current deformation shader.
		 *  @return The constant parameter list of the current deformation shader.
		 */
		inline virtual const CParameterList &deformationParameters() const
		{
			return m_deformation.m_value.m_params;
		}

		/** @brief Gets the transformation of the current deformation shader.
		 *  @return The constant transformation of the deformation interior shader.
		 */
		inline virtual const CTransformation *deformationTransformation() const
		{
			return m_deformation.m_shaderTransform;
		}

		virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
		inline virtual const std::vector<RtFloat> &textureCoordinates() const
		{
			return m_textureCoordinates.m_value;
		}
		virtual bool getTextureCoordinates(RtFloat &s1, RtFloat &t1, RtFloat &s2, RtFloat &t2, RtFloat &s3, RtFloat &t3, RtFloat &s4, RtFloat &t4) const;

		virtual RtVoid shadingRate(RtFloat size);
		virtual RtFloat shadingRate() const;

		virtual RtVoid shadingInterpolation(RtToken type);
		inline virtual RtToken shadingInterpolation() const
		{
				return m_shadingInterpolation.m_value;
		}

		virtual RtVoid matte(RtBoolean onoff);
		inline virtual RtBoolean matte() const
		{
			return m_matte.m_value ? RI_TRUE: RI_FALSE;
		}

		virtual RtVoid bound(RtBound aBound);
		inline virtual const std::vector<RtFloat> &bound() const
		{
			return m_bound.m_value;
		}
		virtual bool getBound(RtBound aBound) const;
		inline virtual bool boundCalled() const
		{
			return m_boundCalled;
		}

		virtual RtVoid detail(RtBound aBound);
		inline virtual const std::vector<RtFloat> &detail() const
		{
			return m_detail.m_value;
		}
		virtual bool getDetail(RtBound aBound) const;
		inline virtual bool detailCalled() const
		{
			return m_detailCalled;
		}

		virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
		inline virtual bool detailRangeCalled() const
		{
			return m_detailRangeCalled;
		}
		inline virtual bool detailRangeCalledInBlock() const
		{
			return m_detailRangeCalledInBlock;
		}
		inline virtual void clearDetailRangeCalledInBlock()
		{
			m_detailRangeCalledInBlock = false;
		}
		inline virtual const std::vector<RtFloat> &detailRange() const
		{
			return m_detailRange.m_value;
		}
		virtual bool getDetailRange(RtFloat &minvis, RtFloat &lowtran, RtFloat &uptran, RtFloat &maxvis) const;

		virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
		inline virtual RtToken geometricApproximationType() const
		{
			return m_geometricApproximationType.m_value;
		}
		inline virtual RtFloat geometricApproximationValue() const
		{
			return m_geometricApproximationValue.m_value;
		}

		virtual RtVoid geometricRepresentation(RtToken type);
		inline virtual RtToken geometricRepresentation() const
		{
			return m_geometricRepresentation.m_value;
		}

		RtVoid resetCoordSysOrientation(RtToken anOrientation);

		virtual RtVoid orientation(RtToken anOrientation);
		inline virtual RtToken orientation(RtToken anOrientation) const
		{
			return m_orientation.m_value;
		}

		virtual RtVoid reverseOrientation(void);

		virtual RtVoid toggleOrientation();
		virtual RtVoid coordSysOrientation(RtToken anOrientation);
		inline virtual RtToken CAttributes::primitiveOrientation() const
		{
			return m_primitiveOrientation;
		}
		inline virtual RtToken CAttributes::coordSysOrientation() const
		{
			return m_coordSysOrientation;
		}
		
		virtual RtVoid sides(RtInt nsides);
		inline virtual RtInt sides() const
		{
			return m_nSides.m_value;
		}

		virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
		virtual bool getBasis(RtBasis ubasis, RtInt &ustep, RtBasis vbasis, RtInt &vstep) const;
		inline virtual RtInt uStep() const
		{
			return m_uBasis.m_value.m_step;
		}
		inline virtual RtInt vStep() const
		{
			return m_vBasis.m_value.m_step;
		}

		virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);
		virtual RtVoid trimCurve(const CTrimCurveData &trimCurveData);
		inline virtual const CTrimCurveData &trimCurve() const
		{
			return m_trimCurve.m_value;
		}

		virtual RtVoid motionBegin(const CMotionState &state);
		virtual RtVoid motionEnd();
		virtual RtVoid motionSuspend();
		
		virtual RtVoid sample(RtFloat shutterTime, const TypeMotionTimes &times);
		virtual RtVoid sampleReset();
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
		inline virtual CAttributes *newAttributesInstance(const CColorDescr &c) const
		{
			return new CAttributes(c);
		}

		/** @brief Factory method to get a copy of an attribute set.
		 *
		 *  Must be overwritten to supply an own attribute set.
		 *
		 *  @param attr Attribute set to copy.
		 */
		inline virtual CAttributes *newAttributesInstance(const CAttributes &attr) const
		{
			return new CAttributes(attr);
		}

		/** @brief Deletes an attribute set delivered by this factory.
		 *
		 *  Because the destructor is virtual, this method don_t need to be overwritten.
		 *
		 *	@param a Attribute set to delete, had to be constructed previously by this factory.
		 */
		inline virtual void deleteAttributesInstance(CAttributes *a) const
		{
			if ( a )
				delete a;
		}
	public:
		/** @brief Virtual destructor.
		 */
		inline virtual ~CAttributesFactory() {}

		/** @brief Gets a new attribute set.
		 *  @param c Current color descriptor.
		 *  @return A new Attribute set
		 *  @exception ExceptRiCPPError If there was not enough memory
		 */
		CAttributes *newAttributes(const CColorDescr &c) const;

		/** @brief Gets a copy of an attribute set, handles memory array.
		 *  @param attr Attribute set to copy.
		 *  @return A new Attribute set
		 *  @exception ExceptRiCPPError If there was not enough memory
		 */
		CAttributes *newAttributes(const CAttributes &attr) const;

		static void deleteAttributes(CAttributes *a);

	}; // CAttributesFactory
}

#endif // _RICPP_RICONTEXT_ATTRIBUTES_H
