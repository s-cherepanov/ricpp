#ifndef _RICPP_RENDERCONTEXT_MATRIX3D_H
#define _RICPP_RENDERCONTEXT_MATRIX3D_H

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

/*! \file matrix3d.h
 *  \brief Declaration of TMatrix3D, a class for homogene 3D matrices and basic matrix operations
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "tools/helpers.h"

//! A class for a 3D homogene matrix (4x4)
/*! The class implements all matrix operations needed by the implementation of the RenderMan interface.
 */
class TMatrix3D {
	//! The value of the 4x4 matrix, init with identity
	/*! Index [i][j]:\n
	 *  [0][0] [0][1] [0][2] [0][3]\n
	 *  [1][0] [1][1] [1][2] [1][3]\n
	 *  [2][0] [2][1] [2][2] [2][3]\n
	 *  [3][0] [3][1] [3][2] [3][3]\n
	 */
	RtMatrix m_Matrix;

	//! true (default), if premultiply when concatenate matrices (e.g. by transform())
	bool m_preMultiply;

	// void extract(RtFloat *result, const RtFloat *mat, int i, int size) const;
	// RtFloat det(const RtFloat *mat, int size) const;
public:
	TMatrix3D();
	TMatrix3D(const TMatrix3D &mat);
	TMatrix3D(RtMatrix mat);
	~TMatrix3D();

	// TMatrix3D &duplicate() const;
	TMatrix3D &operator=(const TMatrix3D &mat);
	TMatrix3D &operator=(RtMatrix mat);

	operator const RtFloat *() const { return (const RtFloat *)&m_Matrix[0]; }
	// RtFloat *getPointer() const { return (RtFloat *)&m_Matrix[0]; }

	inline void setPreMultiply(bool preMultiply=true) {
		m_preMultiply = preMultiply;
	}
	inline bool getPreMultiply() const {
		return m_preMultiply;
	}

	inline void set(const RtMatrix mat) {
		memcpy(m_Matrix, mat, 16*sizeof(RtFloat));
	}
	inline void set(const RtFloat *mat) {
		memcpy(m_Matrix, mat, 16*sizeof(RtFloat));
	}
	void get(RtMatrix &mat) const;

	bool operator==(const TMatrix3D &mat) const;
	bool operator==(RtMatrix mat) const;
	bool operator!=(const TMatrix3D &mat) const;
	bool operator!=(RtMatrix mat) const;

	void clear();
	void identity();
	void transpose();
	void translate(RtFloat dx, RtFloat dy, RtFloat dz);
	void scale(RtFloat sx, RtFloat sy, RtFloat sz);

	void transform(RtMatrix mat);
	void transform(TMatrix3D &mat);

	void postMultiply(RtMatrix mat);
	void postMultiply(TMatrix3D mat);
	void preMultiply(RtMatrix mat);
	void preMultiply(TMatrix3D mat);

	void concatTransform(RtMatrix mat);
	void concatTransform(TMatrix3D mat);

	void transformPoints(RtFloat &x, RtFloat &y, RtFloat &z);

	void rotateX(RtFloat w);
	void rotateY(RtFloat w);
	void rotateZ(RtFloat w);
	void rotate(RtFloat w, RtFloat x, RtFloat y, RtFloat z);

	void skew(RtFloat w, RtFloat x1, RtFloat y1, RtFloat z1, RtFloat x2, RtFloat y2, RtFloat z2);

	void perspectiveProject(RtFloat fov);

	RtFloat determinant() const;
	bool isIdentity() const;
	bool getInverse(RtMatrix &mat) const;
};

#endif // _RICPP_RENDERCONTEXT_MATRIX3D_H

