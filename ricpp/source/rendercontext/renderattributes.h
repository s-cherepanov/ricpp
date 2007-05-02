#ifndef _RICPP_RENDERCONTEXT_RENDERATTRIBUTES_H
#define _RICPP_RENDERCONTEXT_RENDERATTRIBUTES_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file renderattributes.h
 *  \brief Declares TRenderAttributes, an object to store a complete attribute set of a render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "baseclass.h"
#include "optionmap.h"
#include "shader.h"
#include "light.h"
#include "trimcurve.h"

//! The number of the shaders of the RenderMan interface
#define N_SHADERS 6
//! For the surface shader
#define IDX_SHADER_SURFACE 0
//! For the displacement shader
#define IDX_SHADER_DISPLACEMENT 1
//! For the atmosphere shader
#define IDX_SHADER_ATMOSPHERE 2
//! For the interior shader
#define IDX_SHADER_INTERIOR 3
//! For the exterior shader
#define IDX_SHADER_EXTERIOR 4
//! For the deformation shader
#define IDX_SHADER_DEFORMATION 5


//! Class with the transformation stack and composit transformation matrix
class TRenderTransform : public TBaseClass {
protected:
	//! Assign the values of attriubutes
	/*! \param o The attributes to assign to this
	 */
	inline void assign(const TRenderTransform &o) {
		m_matrixStack = o.m_matrixStack;
		m_inverseMatrixStack = o.m_inverseMatrixStack;
		m_CTM = o.m_CTM;
		m_inverseCTM = o.m_inverseCTM;
	}

	//! Stack of transformation matrices
	std::vector<TMatrix3D> m_matrixStack;

	//! Stack of inverse transformation matrices
	std::vector<TMatrix3D> m_inverseMatrixStack;

public:
	//! Composit transformation matrix
	TMatrix3D m_CTM;

	//! Inverse of the composit transformation matrix
	TMatrix3D m_inverseCTM;

	//! Clear all resources used by the instance.
	inline void releaseAll() {
		m_matrixStack.clear();
		m_CTM.identity();
		m_inverseMatrixStack.clear();
		m_inverseCTM.identity();
		m_inverseCTM.setPreMultiply(false);
	}

	//! Initialization with defaults, called by constructor
	inline virtual void initialize() {
		m_matrixStack.clear();
		m_CTM.identity();
		m_inverseMatrixStack.clear();
		m_inverseCTM.identity();
		m_inverseCTM.setPreMultiply(false);
	}

	//! matrices will be identity.
	inline TRenderTransform() { }
	inline TRenderTransform(const TRenderTransform &rt) {
		assign(rt);
	}
	//! Destructor, frees all resources.
	inline virtual ~TRenderTransform() { releaseAll(); }

	//! Returns a (deep) copy of this.
	/*! \retrun a copy of this.
	 *  \exception TSystemException Thrown if there is not enough memory.
	 */
	inline virtual TBaseClass &duplicate() const { 
		TRenderTransform *rt = new TRenderTransform(*this);
		if ( !rt )
			throw TSystemException(__FILE__, __LINE__);
		return *rt;
	}

	//! The class name.
	/*! \return the classname of this class as string "TRenderAttributes".
	 */
	inline static const char *myClassName(void) {return "TRenderTransform";}

	//! The class name.
	/*! \return the classname of the class of this instance.
	 */
	inline virtual const char *className() const { return TRenderTransform::myClassName(); }

	//! Inheritance, looks if instance is a kind of the class given by the name aClass.
	/*! \param aClass Class to test for inheritance.
	 *  \return true, if instances inherits from aClass.
	 */
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TRenderTransform::myClassName()) ) {
			return true;
		}
		return TBaseClass::isKindOf(aClass);
	}

	//! Assigns instance o to this instance.
	/*! \param o Instance to copy from.
	 *  \return A reference to *this.
	 *  \exception TSystemException Thrown if there is no assignment possible (wrong type).
	 */
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TRenderTransform::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		TRenderTransform::assign((const TRenderTransform &)o);
		return TBaseClass::operator=(o);
	}

	//! Pushes the CTM to a internal stack, e.g. by TRi::transformBegin().
	inline void pushTransformation() {
		m_matrixStack.push_back(m_CTM);
		m_inverseMatrixStack.push_back(m_inverseCTM);
	}

	//! Pops the CTM from a internal stack, e.g. by TRi::transformEnd().
	/*! \return true: if transformation is poped, false: transformation stack was empty
	 */
	inline bool popTransformation() {
		if ( m_matrixStack.empty() ||
			 m_inverseMatrixStack.empty() )
		{
			return false;
		}
		m_CTM = m_matrixStack.back();
		m_matrixStack.pop_back();
		m_inverseCTM = m_inverseMatrixStack.back();
		m_inverseMatrixStack.pop_back();
		return true;
	}
};

	
//! Class with all attributes of the RenderMan Interface
/*! Classes inherit from TIntermediateRenderer can overload TIntermediateRenderer::getNewAttributes() to
 *  return a child of TRenderAttributes to implement own attributes.
 */
class TRenderAttributes : public TBaseClass {
protected:
	//! Assign the values of attriubutes
	/*! \param o The attributes to assign to this
	 */
	void assign(const TRenderAttributes &o);
public:
	//
	// shading attributes
	//
	std::vector<RtFloat> m_color;		//!< Current reflective color (white - all 1.0), number of components may changed by option, norm is r, g, b.
	std::vector<RtFloat> m_opacity;		//!< Current opacity of an object (opaque - all 1.0), components as in color.

	RtFloat m_textureCoordinates[8];	//!< Texture coordinates s1,t1 .. s4,t4 unit square [0,0, 1,0, 0,1, 1,1].

	RtFloat m_shadingRate;				//!< Current shading rate in pixels (def. 1), infinity -> once per primitive.
	std::string m_shadingInterpolation;	//!< Interpolation between pixels, "constant", "smooth" (def. RI_SMOOTH).
	RtBoolean m_matte;					//!< subsequent object are 'matte' objects? (def. RI_FALSE).
	
	//! switched on light source list, index to light list in render context
	std::vector<RtLightHandle> m_lightsOn;

	//! TRi::areaLight() is called -> valid till next TRi::AttributeEnd or a call of TRi::areaLight(TRi::NULL) (see ARM)
	RtBoolean m_areaLightBlock;

	// Shader List
	TShader m_shaders[N_SHADERS];                 //!< Current shaders for all shader types
	TMatrix3D m_shaderMatrices[N_SHADERS];        //!< The CTM for each shader type
	TMatrix3D m_shaderInverseMatrices[N_SHADERS]; //!< The inverse of the CTM for each shader type

	//
	// geometric attributes
	//
	RtBound   m_bound;					//!< Bounding box for subsequent primitives
	RtBoolean m_boundCalled;			//!< Bounding box is set by an interface call

	RtBound   m_detail;                 //!< Level of detail
	RtBoolean m_detailCalled;           //!< Level of detail is set by an interface call

	RtFloat m_detailRange[4];           //!< The detail ranges
	RtBoolean m_detailRangeCalled;      //!< Detail ranges are set by an interface call

	// --- To DO string-value pairs ?
	std::string m_geometricApproximationType; //!< The geometric approximation type (e.g. RI_FLATNESS)
	RtFloat m_geometricApproximationValue;    //!< The value for the approximation type

	std::string m_geometricRepresentation;    //!< The geometric representation

	// Orientations - true: LH, false: RH
	RtToken m_orientation;			//!< started RI_OUTSIDE, set by orientation and changed by transformations
	bool	m_currentOrientation;	//!< started with RI_LH (true), changed by transformations - governed to determinante CTM

	RtInt   m_nSides;				//!< 1 or 2, def. is 2 (inside and outside)

	RtInt   m_colorComponents;      //!< Components per Color (opacity)

	std::string m_currCoordSysName;	//!< name of the current coordinate system

	//! Stack of transformation matrices
	// std::vector<TMatrix3D> m_matrixStack;
	//! Composit transformation matrix
	// TMatrix3D m_CTM;

	//! Stack of inverse transformation matrices
	// std::vector<TMatrix3D> m_inverseMatrixStack;
	//! Inverse of the composit transformation matrix
	// TMatrix3D m_inverseCTM;

	//! List of implementation-specific attributes (RiAttributes())
	TOptionMap m_attributeMap;

	// Basismatrices and steps of splines
	RtInt m_uStep,			//!< Step size in u direction used for control points of bicubic spline meshes
		  m_vStep;			//!< Step size in V direction used for control points of spline meshes
	RtBasis m_uBasis,		//!< Basis matrix for bicubic splines in u direction
		    m_vBasis;		//!< Basis matrix for splines in v direction

	// Trimcurve for RiNuCurve()
	TTrimCurveData m_trimCurve; //!< Current trim curve for NURBs

	/////////////////////////////////////////////////////////////////////////////////////////
	// Member functions

	//! Clear all resources used by the instance.
	void releaseAll();
	//! Initialization with defaults
	virtual void initialize();

	//! Standard constructor, initilizes with default values.
	/*! \param colorComponents Number of comonents for a color (opacity) value.
	 */
	TRenderAttributes(int colorComponents = 3);
	//! Copy constructor, initilizes with values of another TRenderAttributes instance.
	/*! \param ra Attributes to assign to the new instance.
	 */
	TRenderAttributes(const TRenderAttributes &ra);
	//! Destructor, frees all resources.
	virtual ~TRenderAttributes();

	//! Returns a (deep) copy of this.
	/*! \retrun a copy of this.
	 *  \exception TSystemException Thrown if there is not enough memory.
	 */
	virtual TBaseClass &duplicate() const;

	//! The class name.
	/*! \return the classname of this class as string "TRenderAttributes".
	 */
	inline static const char *myClassName(void) {return "TRenderAttributes";}
	//! The class name.
	/*! \return the classname of the class of this instance.
	 */
	inline virtual const char *className() const { return TRenderAttributes::myClassName(); }
	//! Inheritance, looks if instance is a kind of the class given by the name aClass.
	/*! \param aClass Class to test for inheritance.
	 *  \return true, if instances inherits from aClass.
	 */
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TRenderAttributes::myClassName()) ) {
			return true;
		}
		return TBaseClass::isKindOf(aClass);
	}

	//! Assigns instance o to this instance.
	/*! \param o Instance to copy from.
	 *  \return A reference to *this.
	 *  \exception TSystemException Thrown if there is no assignment possible (wrong type).
	 */
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TRenderAttributes::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		TRenderAttributes::assign((const TRenderAttributes &)o);
		return TBaseClass::operator=(o);
	}

	//! Pushes the CTM to a internal stack, e.g. by TRi::transformBegin().
	// inline void pushTransformation() {
	// 	m_matrixStack.push_back(m_CTM);
	// 	m_inverseMatrixStack.push_back(m_inverseCTM);
	// }
	//! Pops the CTM from a internal stack, e.g. by TRi::transformEnd().
	/*! \return true: if transformation is poped, false: transformation stack was empty
	 */
	// inline bool popTransformation() {
	// 	if ( m_matrixStack.empty() ||
	// 		 m_inverseMatrixStack.empty() )
	// 	{
	// 		return false;
	// 	}
	// 	m_CTM = m_matrixStack.back();
	// 	m_matrixStack.pop_back();
	// 	m_inverseCTM = m_inverseMatrixStack.back();
	// 	m_inverseMatrixStack.pop_back();
	// 	return true;
	// }
}; // TRenderAttributes

#endif // _RICPP_RENDERCONTEXT_RENDERATTRIBUTES_H
