#ifndef _RICPP_RENDERSTATE_TRANSFORMATON_H
#define _RICPP_RENDERSTATE_TRANSFORMATON_H

#ifndef _RICPP_DECLAREATION_TYPES_H
#include "ricpp/ricpp/types.h"
#endif //  _RICPP_DECLAREATION_TYPES_H

namespace RiCPP {

//! Class with the transformation stack and composit transformation matrix
class CTransformation {
	//! Composit transformation matrix
	CMatrix3D m_CTM;

	//! Inverse of the composit transformation matrix
	CMatrix3D m_inverseCTM;

	//! false, did an operation that could not calculate the inverse.
	bool m_isValid;

public:
	//! matrices will be identity.
	CTransformation();

	inline CTransformation(const CTransformation &rt)
	{
		*this = rt;
	}
	//! Destructor
	inline virtual ~CTransformation() { }

	//! Returns a (deep) copy of this.
	/*! \retrun a copy of this.
	 *  \exception ExceptRiCPPError Thrown if there is not enough memory.
	 */
	inline CTransformation &duplicate() const;

	//! Assigns instance o to this instance.
	/*! \param o Instance to copy from.
	 *  \return A reference to *this.
	 */
	CTransformation &operator=(const CTransformation &o);

	inline CMatrix3D &getCTM()
	{
		return m_CTM;
	}

	inline virtual const CMatrix3D &getCTM() const
	{
		return m_CTM;
	}

	inline CMatrix3D &getInverseCTM()
	{
		return m_inverseCTM;
	}

	inline virtual const CMatrix3D &getInverseCTM() const
	{
		return m_inverseCTM;
	}

	inline virtual bool isValid() const { return m_isValid; }

	RtVoid identity(void);
	RtVoid transform(RtMatrix aTransform);
	RtVoid concatTransform(RtMatrix aTransform);
	RtVoid perspective(RtFloat fov);
	RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
}; // CTransformation

}

#endif // _RICPP_RENDERSTATE_TRANSFORMATON_H
