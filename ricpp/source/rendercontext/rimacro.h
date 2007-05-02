#ifndef _RICPP_RENDERCONTEXT_RIMACRO_H
#define _RICPP_RENDERCONTEXT_RIMACRO_H

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

/*! \file rimacro.h
 *  \brief Declaration of the macro classes, records interface calls.
 *
 *  There is a class for each interface call, the classes can be used to store interface
 *  calls in memory and 'replay' them later. TRiMacro is the class to store a list of macros.
 *  The renderer base class TIntermediateRenderer manages the construction of macros while
 *  loading a RIB file.
 *
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "riscene.h"

#include "parametermap.h"
#include "validcontext.h"
#include "trimcurve.h"
#include "handles.h"

#include "ribparser/paramclasses.h"

#include <list>


///////////////////////////////////////////////////////////////////////////////
//! Type for the indices of the vertices.
/*! The type is unsigned int because that's common for OpenGL.
 */
typedef unsigned int TVertexIndexType;

///////////////////////////////////////////////////////////////////////////////
//! Index numbers for geometric data used to render a mesh
/*! The data is organized either in strips, fans or as a set of unordered triangles
 */
class TGeometricIndices {
public:
	//! Type of the order of the indices.
	enum EGeoDataType {
		strip,     //!< Indices are ordered as triangle strips.
		fan,       //!< Indices are ordered as triangle fans.
		triangles, //!< Indices are triangles.
		polygon    //!< Indices forms a convrx polygon.
	};

private:
	EGeoDataType m_type;                     //!< Type of the indices (order).

public:
	std::vector<TVertexIndexType> m_indices; //!< Indices of the vertices.

	//! Default Constructor
	//! \param aType Order type for the indices, default order is stripe
	inline TGeometricIndices(EGeoDataType aType=strip) : m_type(aType) {}

	//! Destructor
	inline virtual ~TGeometricIndices() {}

	//! Copy constructor, sets all to the value of data.
	/*! \param data Values to copy from.
	 */
	inline TGeometricIndices(const TGeometricIndices &data) { *this = data; }

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TGeometricIndices &duplicate() const {
		TGeometricIndices *retVal = new TGeometricIndices(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}

	//! Assignment operator
	/*! \param data values to copy from.
	 *  \return *this
	 */
	inline TGeometricIndices &operator=(const TGeometricIndices &data) {
		if ( &data == this )
			return *this;
		m_type = data.m_type;
		m_indices = data.m_indices;
		return *this;
	}

	/*! \return The order type of the triangle data (triangle strip, triangle fan or unordered triangles)
	 */
	inline EGeoDataType type() const {return m_type;}

	/*! \param aType Sets the order type of the triangle data.
	 *  \return The type of the triangle data used before (triangle strip, triangle fan or unordered triangles)
	 */
	inline EGeoDataType type(EGeoDataType aType) {
		EGeoDataType old = m_type;
		m_type = aType;
		return old;
	}

	/*! \return A pointer to the index vector of the triangle data.
	 */
	inline const TVertexIndexType *indexPointer() const {
		return m_indices.empty() ? NULL : &(m_indices[0]);
	}

	/*! \return The number of fans, strips or unordered triangles stored.
	 */
	inline IndexType size() const {
		return (IndexType)m_indices.size();
	}
}; // TGeometricIndices


///////////////////////////////////////////////////////////////////////////////
/*! Interface of a face that can be rendered. The data has to be stored index relativ
 *  using the TGeometricIndices structure.
 */
class TGeometricPrimFace {
public:
	//! Standard destructor, has to be virtual.
	inline virtual ~TGeometricPrimFace() {}

	//! Interpolated data either (face) vertex or (face) varying
	/*! \return A pointer to RtFloat TNamedVector, since interpolated data are always floats
	 */
	virtual TNamedVector<RtFloat> *getInterpolatedData(const char *name) = 0;

	//! Constant (since this is a single face) data either uniform or constant
	/*! \return A pointer to a TParameter, since constant parameters can be of various types
	 */
	virtual const TParameter *getUniformData(const char *name, RtPointer *value) = 0;

	/*! \return A pointer to the RtPoint (as RtFloat *) (x,y,z) vertices
	 */
	virtual const RtFloat *getVertices() = 0;

	/*! \param inverted Boolean indicates if the normals have to be inverted.
	 *  \return A pointer to the RtPoint (as RtFloat *) (x,y,z) normals
	 */
	virtual const RtFloat *getNormals(IndexType &size, bool inverted=false) = 0;

	/* \return A reference to the vertex indices. A face can consist of many
	 * TGeometricIndices, e.g. the TSphere consists of a bunch of stripes.
	 */
	virtual std::deque<TGeometricIndices> &getVertexIndices() = 0;

	/*! \param s1 Current texture coordinate attribute
	 *  \param t1 Current texture coordinate attribute
	 *  \param s2 Current texture coordinate attribute
	 *  \param t2 Current texture coordinate attribute
	 *  \param s3 Current texture coordinate attribute
	 *  \param t3 Current texture coordinate attribute
	 *  \param s4 Current texture coordinate attribute
	 *  \param t4 Current texture coordinate attribute
	 *  \return A pointer to the texture coordinates, can be the content of setST
	 */
	virtual const RtFloat *getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
}; // TGeometricPrimFace

class TDrawPrim;

///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls
class TRInterfaceCall : public TRiNode {
protected:
	long m_lineNo; //!< Place to store the line number of a call in a RIB file, -1 if there is no line number
public:
	inline static const char *myClassName(void) { return "TRInterfaceCall"; }
	inline virtual const char *className() const { return TRInterfaceCall::myClassName(); }

	//! Standard constructor, the m_lineNo is initialized to -1 (a line number is not known)
	inline TRInterfaceCall() : m_lineNo(-1) { }
	//! Constructor, to set a line number
	/*! \param aLineNo The line number to store
	 */
	inline TRInterfaceCall(long aLineNo): m_lineNo(aLineNo) {}
	//! Destructor
	inline virtual ~TRInterfaceCall() {}
	//! Number for the interface call, an integer tag used internally by RiCPP to distinguish TRInterfaceCall instances, \sa riconstants.h
	/*! \return The number to identify the interface call (constant IDX_.., e.g. IDX_BEGIN)
	 */
	inline virtual int interfaceIdx() const { return -1;}
	//! Replay the interface call
	/*! \param ri The renderer used for replay
	 */
	inline virtual void replay(TRi &ri) { }
	//! Replay the interface call - use pretriangulated data if poossible
	/*! \param ri The renderer used for replay
	 */
	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) { replay(ri); }
	//! Ask for the line number
	/*! \return The line number, -1 if there is none
	 */
	inline long lineNo() const { return m_lineNo; }
	//! Set the line number
	/*! \param aLineNo The new line number, -1 if there is none
	 *  \return The previous line number, -1 if there was none
	 */
	inline long lineNo(long aLineNo) { long old = m_lineNo; m_lineNo=aLineNo; return old; }
}; // TInterfaceCall

///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls with parameter lists
class TVarParamRInterfaceCall : public TRInterfaceCall {
protected:
	TParameterMap m_parameters;             //!< Parameters of an interface call.
public:
	inline static const char *myClassName(void) { return "TVarParamRInterfaceCall"; }
	inline virtual const char *className() const { return TVarParamRInterfaceCall::myClassName(); }

	//! Standard constructor (an empty parameter list)
	inline TVarParamRInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline TVarParamRInterfaceCall(long aLineNo) : TRInterfaceCall(aLineNo) {}

	//! Destructor
	inline virtual ~TVarParamRInterfaceCall() {}
	
	//! Fill the parameter list
	virtual void setParams(TDeclarationMap &declmap, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]);
	//! Fill the parameter list
	inline virtual void setParams(TDeclarationMap &declmap, const TParameterClasses &p, RtInt colorComps, RtInt n, RtToken tokens[], RtPointer params[]) {
		setParams(declmap, p.vertex(), p.varying(), p.uniform(), p.faceVertex(), p.faceVarying(), colorComps, n, tokens, params);
	}
	
	// Assignment is not implemented, yet
	/* param p A TVarParamRInterfaceCall to assing
	 * return *this
	 */
	// inline TVarParamRInterfaceCall &operator=(const TVarParamRInterfaceCall &p) {
	// 	return *this;
	// }

	inline virtual RtInt getSize() { return m_parameters.getSize(); }
	inline virtual RtToken *getTokens() { return (RtToken *)m_parameters.getTokens(); }
	inline virtual RtPointer *getParams() { return (RtPointer *)m_parameters.getParams(); }
	inline virtual const TParameter *getParameterOf(const char *name) { return m_parameters.getParameterOf(name); }
	inline virtual const TParameter *getParameterOf(IndexType pos) { return m_parameters.getParameterOf(pos); }
}; // TVarParamInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls for geometry
class TGeometryRInterfaceCall : public TVarParamRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TGeometryRInterfaceCall"; }
	inline virtual const char *className() const { return TGeometryRInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline TGeometryRInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline TGeometryRInterfaceCall(long aLineNo) : TVarParamRInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~TGeometryRInterfaceCall() {}
}; // TGeometryRInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls based on UV Meshes (Splines, Quadrics)
class TUVRInterfaceCall : public TGeometryRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TUVRInterfaceCall"; }
	inline virtual const char *className() const { return TUVRInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline TUVRInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline TUVRInterfaceCall(long aLineNo) : TGeometryRInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~TUVRInterfaceCall() {}
}; // TUVRInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls based on Polygon meshes
class TPolygonRInterfaceCall : public TGeometryRInterfaceCall {
protected:
public:
	inline static const char *myClassName(void) { return "TPolygonRInterfaceCall"; }
	inline virtual const char *className() const { return TPolygonRInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline TPolygonRInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline TPolygonRInterfaceCall(long aLineNo) : TGeometryRInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~TPolygonRInterfaceCall() {}
}; // TPolygonRInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Macro container
class TRiMacro {
protected:
	std::list<TRInterfaceCall *> m_callList;  //!< List of all macros
	std::list<TRInterfaceCall *> m_frameList; //!< Pointer to frames
	bool m_isObject;                          //!< true, if macro is an RenderMan object (TRi::objectInstance())
	bool m_isDefining;                        //!< true, if macro is not closed (object end or file end not reached)
	bool m_valid;                             //!< true if the macro could be defined without errors
	std::string m_name;                       //!< Name of the macro (eg. file name)
public:
	int m_frames; //!< Number of frames defined inside this macros (not used)

	//! Constructor initializes the macro.
	/*! \param aName Name of the macro, eg. file name
	 *  \param isObject Macro defines an geometric object
	 */
	inline TRiMacro(const char *aName="", bool isObject=false)
		: m_isObject(isObject), m_isDefining(true), m_valid(true), m_name(aName?aName:""), m_frames(0)
	{
	}
	//! Destructor to free resources.
	~TRiMacro();

	//! The name of the macro.
	/*! \return The name of the macro.
	 */
	inline const char *name() const { return m_name.c_str(); }

	//! Sets the name of the macro
	/*! \param name The new name for the macro.
	 *  \return name itself.
	 */
	inline const char *name(const char *aName ) { m_name = aName; return m_name.c_str(); }

	//! Archive replay with callback for RIB archives
	/*! \param ri The renderer used to replay
	 *  \param ricb Callback to set line number, doing error handling etc.
	 *  \param callback Used for archive callbacks (esp. for TRi::readArchive())
	 */
	void replay(TRi &ri, TRiCallbackInterface &ricb, RtArchiveCallback callback, TDrawPrim *dp=NULL);

	//! Archive replay with callback for RenderMan objects
	/*! \param ri The renderer used to replay
	 *  \param ricb Callback to set line number, doing error handling etc.
	 */
	void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp=NULL); // Object replay

	// Replay single frames not implemented, yet
	// void replay(TRi &ri, RtInt lastFrameNo, RtInt frameNo);
	
	//! Adds an interface call to the macro
	/*! \param c Interface call instance
	 *  \return true if ok
	 */
	bool add(TRInterfaceCall *c);
	
	//! Checks whether macro is a recorded RI object or not (for RI objects \sa TRi::objectBegin(), TRi::objectEnd(), TRi::objectInstance())
	/*! \return true if macro is a recorded RI macro
	 */
	inline bool isObject() const { return m_isObject; }
	
	//! Checks whether macro is still recording
	/*! \return true if macro is still recording
	 */
	inline bool isDefining() const { return m_isDefining; }

	//! Closes the macro definition, isDefinig() is false after a call of this function.
	inline void endMacro() { m_isDefining = false; }


	//! \return whether macros is valid or not
	inline bool valid() const {
		return m_valid;
	}

	//! Sets the validity of the macro - e.g. if an error while parsing occured.
	//! \param isvalid The new validity.
	//! \return The validity set befor.
	inline bool valid(bool isValid) {
		bool old = m_valid;
		m_valid = isValid;
		return old;
	}

	// Assignment is not implemented, yet
	/*
	inline TRiMacro &operator=(const TRiMacro &macro) {
	if ( this == &macro )
	return *this
	// ...
	return *this;
	}
	*/
}; // TMacro


///////////////////////////////////////////////////////////////////////////////

//! Interface calls
//@{
///////////////////////////////////////////////////////////////////////////////
//! Stores the content of a TRi::declare() call
/*! A declaration of an interface variable is stored in an instance of this class.
 *
 *  declare name declaration
 */
class TRiDeclare : public TRInterfaceCall {
protected:
	std::string m_name;        //!< Name of the declarated variable
	std::string m_declaration; //!< The declaration, like: varying float[3]
public:
	inline static const char *myClassName(void) { return "TRiDeclare"; }
	inline virtual const char *className() const { return TRiDeclare::myClassName(); }

	//! Default constructor, records the parameters in member variables
	/*! \param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file
	 *  \param name The name of the declared variable
	 *  \param declaration The declaration string
	 */
	inline TRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
		: TRInterfaceCall(aLineNo),
		  m_name(name?name:""),
		  m_declaration(declaration?declaration:"")
	{}
	
	//! To get the interface number, an integer tag used internally by RiCPP to distinguish TRInterfaceCall instances, \sa riconstants.h
	/*! \return The constant IDX_DECLARE that indicates a declare statement.
	 */
	inline virtual int interfaceIdx() const { return IDX_DECLARE; }

	//! Replays the declare statement for a given renderer
	/*! \param ri Instance of a renderer, used to replay the declare statement
	 */
	inline virtual void replay(TRi &ri) { ri.declare(m_name.c_str(), m_declaration.c_str()); }

	// inline TRiDeclare &operator=(const TRiDeclare &) {
	// 	return *this;
	// }
}; // TRiDeclare

///////////////////////////////////////////////////////////////////////////////
//! Stores the content of a TRi::errorHandler() call
/*! Stores the setting of an error handler.
 *
 *  errorhandler function
 */
class TRiErrorHandler : public TRInterfaceCall {
protected:
	RtErrorHandler m_handler; //!< Pointer to the error handler function
public:
	inline static const char *myClassName(void) { return "TRiErrorHandler"; }
	inline virtual const char *className() const { return TRiErrorHandler::myClassName(); }

	//! Default constructor, records the parameters in member variables
	/*! \param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file
	 *  \param handler Pointer to the error handler function that should be used
	 */
	inline TRiErrorHandler(
		long aLineNo,
		RtErrorHandler handler)
		: TRInterfaceCall(aLineNo),
		  m_handler(handler)
	{}

	//! To get the interface number, an integer tag used internally by RiCPP to distinguish TRInterfaceCall instances, \sa riconstants.h
	/*! \return The constant IDX_ERROR_HANDLER that indicates an errorHandler statement.
	 */
	inline virtual int interfaceIdx() const { return IDX_ERROR_HANDLER; }

	//! Replays the errorhandler statement for a given renderer
	/*! \param ri Instance of a renderer, used to replay the errorhandler statement
	 */
	inline virtual void replay(TRi &ri) { ri.errorHandler(m_handler); }

	// inline TRiErrorHandler &operator=(const TRiErrorHandler &) {
	// 	return *this;
	// }
}; // TRiErrorHandler

// Not suported
// IDX_CONTEXT
// IDX_GET_CONTEXT

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiBegin : public TRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiBegin"; }
	inline virtual const char *className() const { return TRiBegin::myClassName(); }

	inline TRiBegin(long aLineNo, RtToken name) : TRInterfaceCall(aLineNo), m_name(name?name:"") {}
	inline virtual int interfaceIdx() const { return IDX_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.begin(); }
	// inline TRiBegin &operator=(const TRiBegin &) {
	//	return *this;
	// }
}; // TRiBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiEnd"; }
	inline virtual const char *className() const { return TRiEnd::myClassName(); }

	inline TRiEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_END; }
	inline virtual void replay(TRi &ri) { ri.end(); }
	// inline TRiEnd &operator=(const TRiEnd &) {
	//	return *this;
	// }
}; // TRiEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiFrameBegin : public TRInterfaceCall {
protected:
	RtInt m_framenumber; //!< A number to identify the frame.
public:
	inline static const char *myClassName(void) { return "TRiFrameBegin"; }
	inline virtual const char *className() const { return TRiFrameBegin::myClassName(); }

	inline TRiFrameBegin(long aLineNo, RtInt number) : TRInterfaceCall(aLineNo), m_framenumber(number) {}
	inline virtual int interfaceIdx() const { return IDX_FRAME_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.frameBegin(m_framenumber); }
	// inline TRiFrameBegin &operator=(const TRiFrameBegin &) {
	//	return *this;
	// }
}; // TRiFrameBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiFrameEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiFrameEnd"; }
	inline virtual const char *className() const { return TRiFrameEnd::myClassName(); }

	inline TRiFrameEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_FRAME_END; }
	inline virtual void replay(TRi &ri) { ri.frameEnd(); }
	// inline TRiFrameEnd &operator=(const TRiFrameEnd &) {
	// 	return *this;
	// }
}; // TRiFrameEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiWorldBegin : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiWorldBegin"; }
	inline virtual const char *className() const { return TRiWorldBegin::myClassName(); }

	inline TRiWorldBegin() : TRInterfaceCall(-1) {}
	inline TRiWorldBegin(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_WORLD_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.worldBegin(); }
	// inline TRiWorldBegin &operator=(const TRiWorldBegin &) {
	// 	return *this;
	// }
}; // TRiWorldBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiWorldEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiWorldEnd"; }
	inline virtual const char *className() const { return TRiWorldEnd::myClassName(); }

	inline TRiWorldEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_WORLD_END; }
	inline virtual void replay(TRi &ri) { ri.worldEnd(); }
	// inline TRiWorldEnd &operator=(const TRiWorldEnd &) {
	// 	return *this;
	// }
}; // TRiWorldEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiAttributeBegin : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiAttributeBegin"; }
	inline virtual const char *className() const { return TRiAttributeBegin::myClassName(); }

	inline TRiAttributeBegin(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_ATTRIBUTE_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.attributeBegin(); }
	// inline TRiAttributeBegin &operator=(const TRiAttributeBegin &) {
	// 	return *this;
	// }
}; // TRiAttributeBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiAttributeEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiAttributeEnd"; }
	inline virtual const char *className() const { return TRiAttributeEnd::myClassName(); }

	inline TRiAttributeEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_ATTRIBUTE_END; }
	inline virtual void replay(TRi &ri) { ri.attributeEnd(); }
	// inline TRiAttributeEnd &operator=(const TRiAttributeEnd &) {
	// 	return *this;
	// }
}; // TRiAttributeEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiTransformBegin : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiTransformBegin"; }
	inline virtual const char *className() const { return TRiTransformBegin::myClassName(); }

	inline TRiTransformBegin(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_TRANSFORM_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.transformBegin(); }
	// inline TRiTransformBegin &operator=(const TRiTransformBegin &) {
	// 	return *this;
	// }
}; // TRiTransformBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiTransformEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiTransformEnd"; }
	inline virtual const char *className() const { return TRiTransformEnd::myClassName(); }

	inline TRiTransformEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_TRANSFORM_END; }
	inline virtual void replay(TRi &ri) { ri.transformEnd(); }
	// inline TRiTransformEnd &operator=(const TRiTransformEnd &) {
	// 	return *this;
	// }
}; // TRiTransformEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiSolidBegin : public TGeometryRInterfaceCall {
protected:
	std::string m_operation; //!< String that indicates the solid operation (like "union")
public:
	inline static const char *myClassName(void) { return "TRiSolidBegin"; }
	inline virtual const char *className() const { return TRiSolidBegin::myClassName(); }

	inline TRiSolidBegin(long aLineNo, RtToken operation) : TGeometryRInterfaceCall(aLineNo), m_operation(operation) { }
	inline virtual int interfaceIdx() const { return IDX_SOLID_BEGIN; }
	inline virtual void replay(TRi &ri) { ri.solidBegin(m_operation.c_str()); }
	// inline TRiSolidBegin &operator=(const TRiSolidBegin &) {
	// 	return *this;
	// }
}; // TRiSolidBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiSolidEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiSolidEnd"; }
	inline virtual const char *className() const { return TRiSolidEnd::myClassName(); }

	inline TRiSolidEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_SOLID_END; }
	inline virtual void replay(TRi &ri) { ri.solidEnd(); }
	// inline TRiSolidEnd &operator=(const TRiSolidEnd &) {
	// 	return *this;
	// }
}; // TRiSolidEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiObjectBegin : public TRInterfaceCall {
protected:
	TObjectHandle m_handle; //!< (out) Used to identify the internal object handle with the object number of a rib file.
public:
	inline static const char *myClassName(void) { return "TRiObjectBegin"; }
	inline virtual const char *className() const { return TRiObjectBegin::myClassName(); }

	inline TRiObjectBegin(long aLineNo, RtObjectHandle h=(RtObjectHandle)-1) : TRInterfaceCall(aLineNo) { m_handle.m_handle = h; }
	inline virtual int interfaceIdx() const { return IDX_OBJECT_BEGIN; }
	inline virtual void replay(TRi &ri) {
		m_handle.m_handle = ri.objectBegin();
	}
	inline virtual RtObjectHandle handleRef() { return (RtObjectHandle)&m_handle; }
	inline virtual RtObjectHandle handle2() { return m_handle.m_handle; }
	inline virtual RtObjectHandle handle2(RtObjectHandle handle) {
		RtLightHandle old = m_handle.m_handle;
		m_handle.m_handle = handle;
		return old;
	}
	// inline TRiObjectBegin &operator=(const TRiObjectBegin &) {
	//	return *this;
	// }
}; // TRiObjectBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiObjectEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiObjectEnd"; }
	inline virtual const char *className() const { return TRiObjectEnd::myClassName(); }

	inline TRiObjectEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_OBJECT_END; }
	inline virtual void replay(TRi &ri) { ri.objectEnd(); }
	// inline TRiObjectEnd &operator=(const TRiObjectEnd &) {
	// 	return *this;
	// }
}; // TRiObjectEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiMotionBegin : public TRInterfaceCall {
protected:
	std::vector<RtFloat> m_motionVars; //!< The motion "time stamps"
public:
	inline static const char *myClassName(void) { return "TRiMotionBegin"; }
	inline virtual const char *className() const { return TRiMotionBegin::myClassName(); }

	inline TRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) : TRInterfaceCall(aLineNo), m_motionVars(f, f+n) {
	/*
	m_motionVars.resize(n);
	m_motionVars.assign(f, f+n);
		*/
	}
	inline virtual int interfaceIdx() const { return IDX_MOTION_BEGIN; }
	inline virtual void replay(TRi &ri) {
		ri.motionBeginV(
			(RtInt)m_motionVars.size(),
			m_motionVars.empty() ? NULL : &m_motionVars[0]
		);
	}
	// inline TRiMotionBegin &operator=(const TRiMotionBegin &) {
	// 	return *this;
	// }
}; // TRiMotionBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiMotionEnd : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiMotionEnd"; }
	inline virtual const char *className() const { return TRiMotionEnd::myClassName(); }

	inline TRiMotionEnd(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_MOTION_END; }
	inline virtual void replay(TRi &ri) { ri.motionEnd(); }
	// inline TRiMotionEnd &operator=(const TRiMotionEnd &) {
	// 	return *this;
	// }
}; // TRiMotionEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiFormat : public TRInterfaceCall {
protected:
	RtInt m_xres,     //!< width
	      m_yres;     //!< height
	RtFloat m_aspect; //!< pixel aspect ratio
public:
	inline static const char *myClassName(void) { return "TRiFormat"; }
	inline virtual const char *className() const { return TRiFormat::myClassName(); }

	inline TRiFormat(long aLineNo, RtInt xres, RtInt yres, RtFloat aspect) : TRInterfaceCall(aLineNo), m_xres(xres), m_yres(yres), m_aspect(aspect) {}
	inline virtual int interfaceIdx() const { return IDX_FORMAT; }
	inline virtual void replay(TRi &ri) { ri.format(m_xres, m_yres, m_aspect); }
	// inline TRiFormat &operator=(const TRiFormat &) {
	//	return *this;
	// }
}; // TRiFormat

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiFrameAspectRatio : public TRInterfaceCall {
protected:
	RtFloat m_aspect; //!< frame aspect ratio
public:
	inline static const char *myClassName(void) { return "TRiFrameAspectRatio"; }
	inline virtual const char *className() const { return TRiFrameAspectRatio::myClassName(); }

	inline TRiFrameAspectRatio(long aLineNo, RtFloat aspect) : TRInterfaceCall(aLineNo), m_aspect(aspect) {}
	inline virtual int interfaceIdx() const { return IDX_FRAME_ASPECT_RATIO; }
	inline virtual void replay(TRi &ri) { ri.frameAspectRatio(m_aspect); }
	// inline TRiFrameAspectRatio &operator=(const TRiFrameAspectRatio &) {
	// 	return *this;
	// }
}; // TRiFrameAspectRatio

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiScreenWindow : public TRInterfaceCall {
protected:
	RtFloat m_left,  //!< coordinate of the left screen window edge
	        m_right, //!< coordinate of the right screen window edge
	        m_bot,   //!< coordinate of the bottom screen window edge
	        m_top;   //!< coordinate of the top screen window edge
public:
	inline static const char *myClassName(void) { return "TRiScreenWindow"; }
	inline virtual const char *className() const { return TRiScreenWindow::myClassName(); }

	inline TRiScreenWindow(long aLineNo, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) : TRInterfaceCall(aLineNo), m_left(left), m_right(right), m_bot(bot), m_top(top) {}
	inline virtual int interfaceIdx() const { return IDX_SCREEN_WINDOW; }
	inline virtual void replay(TRi &ri) { ri.screenWindow(m_left, m_right, m_bot, m_top); }
	// inline TRiScreenWindow &operator=(const TRiScreenWindow &) {
	// 	return *this;
	// }
}; // TRiScreenWindow

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiCropWindow : public TRInterfaceCall {
protected:
	RtFloat m_xmin,  //!< Crop window minimal x
	        m_xmax,  //!< Crop window maximal x
	        m_ymin,  //!< Crop window minimal y
	        m_ymax;  //!< Crop window maximal y
public:
	inline static const char *myClassName(void) { return "TRiCropWindow"; }
	inline virtual const char *className() const { return TRiCropWindow::myClassName(); }

	inline TRiCropWindow(long aLineNo, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) : TRInterfaceCall(aLineNo), m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax) {}
	inline virtual int interfaceIdx() const { return IDX_CROP_WINDOW; }
	inline virtual void replay(TRi &ri) { ri.cropWindow(m_xmin, m_xmax, m_ymin, m_ymax); }
	// inline TRiCropWindow &operator=(const TRiCropWindow &) {
	//	return *this;
	// }
}; // TRiCropWindow

///////////////////////////////////////////////////////////////////////////////
//! \sa TVarParamRInterfaceCall
class TRiProjection : public TVarParamRInterfaceCall {
protected:
	std::string m_name; //!< projection name ("perspective", "orthographic")
public:
	inline static const char *myClassName(void) { return "TRiProjection"; }
	inline virtual const char *className() const { return TRiProjection::myClassName(); }

	inline TRiProjection(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_PROJECTION; }
	inline virtual void replay(TRi &ri) { ri.projectionV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	// inline TRiProjection &operator=(const TRiProjection &) {
	// 	return *this;
	// }
}; // TRiProjection

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiClipping : public TRInterfaceCall {
protected:
	RtFloat m_hither, //!< Front clipping plane
	        m_yon;    //!< Far clipping plane
public:
	inline static const char *myClassName(void) { return "TRiClipping"; }
	inline virtual const char *className() const { return TRiClipping::myClassName(); }

	inline TRiClipping(long aLineNo, RtFloat hither, RtFloat yon)
		: TRInterfaceCall(aLineNo), m_hither(hither), m_yon(yon) {}
	inline virtual int interfaceIdx() const { return IDX_CLIPPING; }
	inline virtual void replay(TRi &ri) { ri.clipping(m_hither, m_yon); }
	// inline TRiClipping &operator=(const TRiClipping &) {
	//	return *this;
	// }
}; // TRiClipping

///////////////////////////////////////////////////////////////////////////////
//! \sa TRInterfaceCall
class TRiClippingPlane : public TRInterfaceCall {
protected:
	RtFloat m_x,  //!< Point of an additional clipping plane (x coordinate)
	        m_y,  //!< Point of an additional clipping plane (y coordinate)
	        m_z,  //!< Point of an additional clipping plane (z coordinate)
	        m_nx, //!< Normal of an additional clipping plane (x coordinate)
	        m_ny, //!< Normal of an additional clipping plane (y coordinate)
	        m_nz; //!< Normal of an additional clipping plane (z coordinate)
public:
	inline static const char *myClassName(void) { return "TRiClippingPlane"; }
	inline virtual const char *className() const { return TRiClippingPlane::myClassName(); }

	inline TRiClippingPlane(long aLineNo, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) : TRInterfaceCall(aLineNo), m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz) {}
	inline virtual int interfaceIdx() const { return IDX_CLIPPING_PLANE; }
	inline virtual void replay(TRi &ri) { ri.clippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz); }
	// inline TRiClippingPlane &operator=(const TRiClippingPlane &) {
	// 	return *this;
	// }
}; // TRiClippingPlane

///////////////////////////////////////////////////////////////////////////////
class TRiDepthOfField : public TRInterfaceCall {
protected:
	RtFloat m_fstop, m_focallength, m_focaldistance;
public:
	inline static const char *myClassName(void) { return "TRiDepthOfField"; }
	inline virtual const char *className() const { return TRiDepthOfField::myClassName(); }

	inline TRiDepthOfField(long aLineNo, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) : TRInterfaceCall(aLineNo), m_fstop(fstop), m_focallength(focallength), m_focaldistance(focaldistance) {}
	inline virtual int interfaceIdx() const { return IDX_DEPTH_OF_FIELD; }
	inline virtual void replay(TRi &ri) { ri.depthOfField(m_fstop, m_focallength, m_focaldistance); }
	inline TRiDepthOfField &operator=(const TRiDepthOfField &) {
		return *this;
	}
}; // TRiDepthOfField

///////////////////////////////////////////////////////////////////////////////
class TRiShutter : public TRInterfaceCall {
protected:
	RtFloat m_smin, m_smax;
public:
	inline static const char *myClassName(void) { return "TRiShutter"; }
	inline virtual const char *className() const { return TRiShutter::myClassName(); }

	inline TRiShutter(long aLineNo, RtFloat smin, RtFloat smax) : TRInterfaceCall(aLineNo), m_smin(smin), m_smax(smax) {}
	inline virtual int interfaceIdx() const { return IDX_SHUTTER; }
	inline virtual void replay(TRi &ri) { ri.shutter(m_smin, m_smax); }
	inline TRiShutter &operator=(const TRiShutter &) {
		return *this;
	}
}; // TRiShutter

///////////////////////////////////////////////////////////////////////////////
class TRiPixelVariance : public TRInterfaceCall {
	RtFloat m_variation;
public:
	inline static const char *myClassName(void) { return "TRiPixelVariance"; }
	inline virtual const char *className() const { return TRiPixelVariance::myClassName(); }

	inline TRiPixelVariance(long aLineNo, RtFloat variation) : TRInterfaceCall(aLineNo), m_variation(variation) {}
	inline virtual int interfaceIdx() const { return IDX_PIXEL_VARIANCE; }
	inline virtual void replay(TRi &ri) { ri.pixelVariance(m_variation); }
	inline TRiPixelVariance &operator=(const TRiPixelVariance &) {
		return *this;
	}
}; // TRiPixelVariance

///////////////////////////////////////////////////////////////////////////////
class TRiPixelSamples : public TRInterfaceCall {
protected:
	RtFloat m_xsamples, m_ysamples;
public:
	inline static const char *myClassName(void) { return "TRiPixelSamples"; }
	inline virtual const char *className() const { return TRiPixelSamples::myClassName(); }

	inline TRiPixelSamples(long aLineNo, RtFloat xsamples, RtFloat ysamples) : TRInterfaceCall(aLineNo), m_xsamples(xsamples), m_ysamples(ysamples) {}
	inline virtual int interfaceIdx() const { return IDX_PIXEL_SAMPLES; }
	inline virtual void replay(TRi &ri) { ri.pixelSamples(m_xsamples, m_ysamples); }
	inline TRiPixelSamples &operator=(const TRiPixelSamples &) {
		return *this;
	}
}; // TRiPixelSamples

///////////////////////////////////////////////////////////////////////////////
class TRiPixelFilter : public TRInterfaceCall {
protected:
	RtFilterFunc m_function;
	RtFloat m_xwidth, m_ywidth;
public:
	inline static const char *myClassName(void) { return "TRiPixelFilter"; }
	inline virtual const char *className() const { return TRiPixelFilter::myClassName(); }

	inline TRiPixelFilter(long aLineNo, RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) : TRInterfaceCall(aLineNo), m_function(function), m_xwidth(xwidth), m_ywidth(ywidth) {}
	inline virtual int interfaceIdx() const { return IDX_PIXEL_FILTER; }
	inline virtual void replay(TRi &ri) { ri.pixelFilter(m_function, m_xwidth, m_ywidth); }
	inline TRiPixelFilter &operator=(const TRiPixelFilter &) {
		return *this;
	}
}; // TRiPixelFilter

///////////////////////////////////////////////////////////////////////////////
class TRiExposure : public TRInterfaceCall {
protected:
	RtFloat m_gain, m_gamma;
public:
	inline static const char *myClassName(void) { return "TRiExposure"; }
	inline virtual const char *className() const { return TRiExposure::myClassName(); }

	inline TRiExposure(long aLineNo, RtFloat gain, RtFloat gamma) : TRInterfaceCall(aLineNo), m_gain(gain), m_gamma(gamma) {}
	inline virtual int interfaceIdx() const { return IDX_EXPOSURE; }
	inline virtual void replay(TRi &ri) { ri.exposure(m_gain, m_gamma); }
	inline TRiExposure &operator=(const TRiExposure &) {
		return *this;
	}
}; // TRiExposure

///////////////////////////////////////////////////////////////////////////////
class TRiImager : public TVarParamRInterfaceCall {
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiImager"; }
	inline virtual const char *className() const { return TRiImager::myClassName(); }

	inline TRiImager(long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_IMAGER; }
	inline virtual void replay(TRi &ri) { ri.imagerV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiImager &operator=(const TRiImager &) {
		return *this;
	}
}; // TRiImager

///////////////////////////////////////////////////////////////////////////////
class TRiQuantize : public TRInterfaceCall {
protected:
	std::string m_type;
	RtInt m_one, m_qmin, m_qmax;
	RtFloat m_ampl;
public:
	inline static const char *myClassName(void) { return "TRiQuantize"; }
	inline virtual const char *className() const { return TRiQuantize::myClassName(); }

	inline TRiQuantize(long aLineNo, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) : TRInterfaceCall(aLineNo), m_type(type), m_one(one), m_qmin(qmin), m_qmax(qmax), m_ampl(ampl) {}
	inline virtual int interfaceIdx() const { return IDX_QUANTIZE; }
	inline virtual void replay(TRi &ri) { ri.quantize(m_type.c_str(), m_one, m_qmin, m_qmax, m_ampl); }
	inline TRiQuantize &operator=(const TRiQuantize &) {
		return *this;
	}
}; // TRiQuantize

///////////////////////////////////////////////////////////////////////////////
class TRiDisplay : public TVarParamRInterfaceCall {
protected:
	std::string m_name, m_type, m_mode;
public:
	inline static const char *myClassName(void) { return "TRiDisplay"; }
	inline virtual const char *className() const { return TRiDisplay::myClassName(); }

	inline TRiDisplay(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken name, RtToken type, RtToken mode,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name), m_type(type), m_mode(mode)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_DISPLAY; }
	inline virtual void replay(TRi &ri) { ri.displayV(m_name.c_str(), m_type.c_str(), m_mode.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiDisplay &operator=(const TRiDisplay &) {
		return *this;
	}
}; // TRiDisplay

///////////////////////////////////////////////////////////////////////////////
class TRiHider : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiHider"; }
	inline virtual const char *className() const { return TRiHider::myClassName(); }

	inline TRiHider(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_HIDER; }
	inline virtual void replay(TRi &ri) { ri.hiderV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiHider &operator=(const TRiHider &) {
		return *this;
	}
}; // TRiHider

///////////////////////////////////////////////////////////////////////////////
class TRiColorSamples : public TRInterfaceCall {
protected:
	RtInt m_N;
	std::vector<RtFloat> m_nRGB, m_RGBn;
public:
	inline static const char *myClassName(void) { return "TRiColorSamples"; }
	inline virtual const char *className() const { return TRiColorSamples::myClassName(); }

	inline TRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn) : TRInterfaceCall(aLineNo), m_N(N) {
		m_nRGB.reserve(N);
		m_RGBn.reserve(N);
		RtInt i;
		for ( i = 0; i < N; ++i ) {
			m_nRGB.push_back(nRGB[i]);
			m_RGBn.push_back(RGBn[i]);
		}
	}
	inline virtual int interfaceIdx() const { return IDX_COLOR_SAMPLES; }
	inline virtual void replay(TRi &ri) {
		ri.colorSamples(
			m_N,
			m_nRGB.empty() ? NULL : &m_nRGB[0],
			m_RGBn.empty() ? NULL : &m_RGBn[0]);
	}
	inline TRiColorSamples &operator=(const TRiColorSamples &) {
		return *this;
	}
}; // TRiColorSamples

///////////////////////////////////////////////////////////////////////////////
class TRiRelativeDetail : public TRInterfaceCall {
protected:
	RtFloat m_relativedetail;
public:
	inline static const char *myClassName(void) { return "TRiRelativeDetail"; }
	inline virtual const char *className() const { return TRiRelativeDetail::myClassName(); }

	inline TRiRelativeDetail(long aLineNo, RtFloat relativedetail) : TRInterfaceCall(aLineNo), m_relativedetail(relativedetail) {}
	inline virtual int interfaceIdx() const { return IDX_RELATIVE_DETAIL; }
	inline virtual void replay(TRi &ri) { ri.relativeDetail(m_relativedetail); }
	inline TRiRelativeDetail &operator=(const TRiRelativeDetail &) {
		return *this;
	}
}; // TRiRelativeDetail

///////////////////////////////////////////////////////////////////////////////
class TRiOption : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiOption"; }
	inline virtual const char *className() const { return TRiOption::myClassName(); }

	inline TRiOption(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_OPTION; }
	inline virtual void replay(TRi &ri) { ri.optionV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiOption &operator=(const TRiOption &) {
		return *this;
	}
}; // TRiOption

///////////////////////////////////////////////////////////////////////////////
class TRiLightSource : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
	TLightHandle m_handle;
public:
	inline static const char *myClassName(void) { return "TRiLightSource"; }
	inline virtual const char *className() const { return TRiLightSource::myClassName(); }

	inline TRiLightSource(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name, RtLightHandle handle,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name), m_handle(handle)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_LIGHT_SOURCE; }
	inline virtual void replay(TRi &ri) { m_handle.m_handle = ri.lightSourceV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline virtual RtLightHandle handleRef() { return (RtLightHandle)&m_handle; }
	inline virtual RtLightHandle handle2() const { return m_handle.m_handle; }
	inline virtual RtLightHandle handle2(RtLightHandle handle) {
		RtLightHandle old = m_handle.m_handle;
		m_handle.m_handle = handle;
		return old;
	}
	inline TRiLightSource &operator=(const TRiLightSource &) {
		return *this;
	}
}; // TRiLightSource

///////////////////////////////////////////////////////////////////////////////
class TRiAreaLightSource : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
	TLightHandle m_handle;
public:
	inline static const char *myClassName(void) { return "TRiAreaLightSource"; }
	inline virtual const char *className() const { return TRiAreaLightSource::myClassName(); }

	inline TRiAreaLightSource(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name, RtLightHandle handle,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name), m_handle(handle)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_AREA_LIGHT_SOURCE; }
	inline virtual void replay(TRi &ri) { m_handle.m_handle = ri.areaLightSourceV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline virtual RtLightHandle handleRef() { return (RtLightHandle)&m_handle; }
	inline virtual RtLightHandle handle2() const { return m_handle.m_handle; }
	inline virtual RtLightHandle handle2(RtLightHandle handle) {
		RtLightHandle old = m_handle.m_handle;
		m_handle.m_handle = handle;
		return old;
	}
	inline TRiAreaLightSource &operator=(const TRiAreaLightSource &) {
		return *this;
	}
}; // TRiAreaLightSource

///////////////////////////////////////////////////////////////////////////////
class TRiIlluminate : public TRInterfaceCall {
protected:
	TLightHandle *m_handle;
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "TRiIlluminate"; }
	inline virtual const char *className() const { return TRiIlluminate::myClassName(); }

	inline TRiIlluminate(long aLineNo, RtLightHandle handle, RtBoolean onoff) : TRInterfaceCall(aLineNo), m_handle((TLightHandle *)handle), m_onoff(onoff) {}
	inline virtual int interfaceIdx() const { return IDX_ILLUMINATE; }
	inline virtual RtLightHandle handleRef() { return (RtLightHandle)m_handle; }
	inline virtual RtLightHandle handle2() const { return m_handle ? m_handle->m_handle : (RtLightHandle)-1; }
	inline virtual void replay(TRi &ri) { ri.illuminate(m_handle ? m_handle->m_handle : (RtLightHandle)-1, m_onoff); }
	inline TRiIlluminate &operator=(const TRiIlluminate &) {
		return *this;
	}
}; // TRiIlluminate

///////////////////////////////////////////////////////////////////////////////
class TRiAttribute : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiAttribute"; }
	inline virtual const char *className() const { return TRiAttribute::myClassName(); }

	inline TRiAttribute(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_ATTRIBUTE; }
	inline virtual void replay(TRi &ri) { ri.attributeV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiAttribute &operator=(const TRiAttribute &) {
		return *this;
	}
}; // TRiAttribute

///////////////////////////////////////////////////////////////////////////////
class TRiColor : public TRInterfaceCall {
protected:
	std::vector<RtFloat> m_color;
public:
	inline static const char *myClassName(void) { return "TRiColor"; }
	inline virtual const char *className() const { return TRiColor::myClassName(); }

	inline TRiColor(long aLineNo, RtInt size, RtColor Cs) : TRInterfaceCall(aLineNo) {
		int i;
		for ( i=0; i<size; ++i )
			m_color.push_back(Cs[i]);
	}
	inline virtual int interfaceIdx() const { return IDX_COLOR; }
	inline virtual void replay(TRi &ri) {
		ri.color(m_color.empty() ? NULL : &m_color[0]);
	}
	inline TRiColor &operator=(const TRiColor &) {
		return *this;
	}
}; // TRiColor

///////////////////////////////////////////////////////////////////////////////
class TRiOpacity : public TRInterfaceCall {
protected:
	std::vector<RtFloat> m_opacity;
public:
	inline static const char *myClassName(void) { return "TRiOpacity"; }
	inline virtual const char *className() const { return TRiOpacity::myClassName(); }

	inline TRiOpacity(long aLineNo, RtInt size, RtColor Cs) : TRInterfaceCall(aLineNo) {
		int i;
		for ( i=0; i<size; ++i )
			m_opacity.push_back(Cs[i]);
	}
	inline virtual int interfaceIdx() const { return IDX_OPACITY; }
	inline virtual void replay(TRi &ri) {
		ri.opacity(m_opacity.empty() ? NULL : &m_opacity[0]);
	}
	inline TRiOpacity &operator=(const TRiOpacity &) {
		return *this;
	}
}; // TRiOpacity

///////////////////////////////////////////////////////////////////////////////
class TRiSurface : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiSurface"; }
	inline virtual const char *className() const { return TRiSurface::myClassName(); }

	inline TRiSurface(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_SURFACE; }
	inline virtual void replay(TRi &ri) { ri.surfaceV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiSurface &operator=(const TRiSurface &) {
		return *this;
	}
}; // TRiSurface

///////////////////////////////////////////////////////////////////////////////
class TRiAtmosphere : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiAtmosphere"; }
	inline virtual const char *className() const { return TRiAtmosphere::myClassName(); }

	inline TRiAtmosphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_ATMOSPHERE; }
	inline virtual void replay(TRi &ri) { ri.atmosphereV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiAtmosphere &operator=(const TRiAtmosphere &) {
		return *this;
	}
}; // TRiAtmosphere

///////////////////////////////////////////////////////////////////////////////
class TRiInterior : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiInterior"; }
	inline virtual const char *className() const { return TRiInterior::myClassName(); }

	inline TRiInterior(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_INTERIOR; }
	inline virtual void replay(TRi &ri) { ri.interiorV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiInterior &operator=(const TRiInterior &) {
		return *this;
	}
}; // TRiInterior

///////////////////////////////////////////////////////////////////////////////
class TRiExterior : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiExterior"; }
	inline virtual const char *className() const { return TRiExterior::myClassName(); }

	inline TRiExterior(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_EXTERIOR; }
	inline virtual void replay(TRi &ri) { ri.exteriorV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiExterior &operator=(const TRiExterior &) {
		return *this;
	}
}; // TRiExterior

///////////////////////////////////////////////////////////////////////////////
class TRiDisplacement : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiDisplacement"; }
	inline virtual const char *className() const { return TRiDisplacement::myClassName(); }

	inline TRiDisplacement(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_DISPLACEMENT; }
	inline virtual void replay(TRi &ri) { ri.displacementV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiDisplacement &operator=(const TRiDisplacement &) {
		return *this;
	}
}; // TRiDisplacement

///////////////////////////////////////////////////////////////////////////////
class TRiTextureCoordinates : public TRInterfaceCall {
protected:
	RtFloat m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4;
public:
	inline static const char *myClassName(void) { return "TRiTextureCoordinates"; }
	inline virtual const char *className() const { return TRiTextureCoordinates::myClassName(); }

	inline TRiTextureCoordinates(long aLineNo, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) : TRInterfaceCall(aLineNo), m_s1(s1), m_t1(t1), m_s2(s2), m_t2(t2), m_s3(s3), m_t3(t3), m_s4(s4), m_t4(t4) {}
	inline virtual int interfaceIdx() const { return IDX_TEXTURE_COORDINATES; }
	inline virtual void replay(TRi &ri) { ri.textureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4); }
	inline TRiTextureCoordinates &operator=(const TRiTextureCoordinates &) {
		return *this;
	}
}; // TRiTextureCoordinates

///////////////////////////////////////////////////////////////////////////////
class TRiShadingRate : public TRInterfaceCall {
protected:
	RtFloat m_size;
public:
	inline static const char *myClassName(void) { return "TRiShadingRate"; }
	inline virtual const char *className() const { return TRiShadingRate::myClassName(); }

	inline TRiShadingRate(long aLineNo, RtFloat size) : TRInterfaceCall(aLineNo), m_size(size) {}
	inline virtual int interfaceIdx() const { return IDX_SHADING_RATE; }
	inline virtual void replay(TRi &ri) { ri.shadingRate(m_size); }
	inline TRiShadingRate &operator=(const TRiShadingRate &) {
		return *this;
	}
}; // TRiShadingRate

///////////////////////////////////////////////////////////////////////////////
class TRiShadingInterpolation : public TRInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "TRiShadingInterpolation"; }
	inline virtual const char *className() const { return TRiShadingInterpolation::myClassName(); }

	inline TRiShadingInterpolation(long aLineNo, RtToken type) : TRInterfaceCall(aLineNo), m_type(type) {}
	inline virtual int interfaceIdx() const { return IDX_SHADING_INTERPOLATION; }
	inline virtual void replay(TRi &ri) { ri.shadingInterpolation(m_type.c_str()); }
	inline TRiShadingInterpolation &operator=(const TRiShadingInterpolation &) {
		return *this;
	}
}; // TRiShadingInterpolation

///////////////////////////////////////////////////////////////////////////////
class TRiMatte : public TRInterfaceCall {
protected:
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "TRiMatte"; }
	inline virtual const char *className() const { return TRiMatte::myClassName(); }

	inline TRiMatte(long aLineNo, RtBoolean onoff) : TRInterfaceCall(aLineNo), m_onoff(onoff) {}
	inline virtual int interfaceIdx() const { return IDX_MATTE; }
	inline virtual void replay(TRi &ri) { ri.matte(m_onoff); }
	inline TRiMatte &operator=(const TRiMatte &) {
		return *this;
	}
}; // TRiMatte

///////////////////////////////////////////////////////////////////////////////
class TRiBound : public TRInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "TRiBound"; }
	inline virtual const char *className() const { return TRiBound::myClassName(); }

	inline TRiBound(long aLineNo, RtBound bound) : TRInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual int interfaceIdx() const { return IDX_BOUND; }
	inline virtual void replay(TRi &ri) { ri.bound(m_bound); }
	inline TRiBound &operator=(const TRiBound &) {
		return *this;
	}
}; // TRiBound

///////////////////////////////////////////////////////////////////////////////
class TRiDetail : public TRInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "TRiDetail"; }
	inline virtual const char *className() const { return TRiDetail::myClassName(); }

	inline TRiDetail(long aLineNo, RtBound bound) : TRInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual int interfaceIdx() const { return IDX_DETAIL; }
	inline virtual void replay(TRi &ri) { ri.bound(m_bound); }
	inline TRiDetail &operator=(const TRiDetail &) {
		return *this;
	}
}; // TRiDetail

///////////////////////////////////////////////////////////////////////////////
class TRiDetailRange : public TRInterfaceCall {
protected:
	RtFloat m_minvis, m_lowtran, m_uptran, m_maxvis;
public:
	inline static const char *myClassName(void) { return "TRiDetailRange"; }
	inline virtual const char *className() const { return TRiDetailRange::myClassName(); }

	inline TRiDetailRange(long aLineNo, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) : TRInterfaceCall(aLineNo), m_minvis(minvis), m_lowtran(lowtran), m_uptran(uptran), m_maxvis(maxvis) {}
	inline virtual int interfaceIdx() const { return IDX_DETAIL_RANGE; }
	inline virtual void replay(TRi &ri) { ri.detailRange(m_minvis, m_lowtran, m_uptran, m_maxvis); }
	inline TRiDetailRange &operator=(const TRiDetailRange &) {
		return *this;
	}
}; // TRiDetailRange

///////////////////////////////////////////////////////////////////////////////
class TRiGeometricApproximation : public TRInterfaceCall {
protected:
	std::string m_type;
	RtFloat m_value;
public:
	inline static const char *myClassName(void) { return "TRiGeometricApproximation"; }
	inline virtual const char *className() const { return TRiGeometricApproximation::myClassName(); }

	inline TRiGeometricApproximation(long aLineNo, RtToken type, RtFloat value) : TRInterfaceCall(aLineNo), m_type(type), m_value(value) {}
	inline virtual int interfaceIdx() const { return IDX_GEOMETRIC_APPROXIMATION; }
	inline virtual void replay(TRi &ri) { ri.geometricApproximation(m_type.c_str(), m_value); }
	inline TRiGeometricApproximation &operator=(const TRiGeometricApproximation &) {
		return *this;
	}
}; // TRiGeometricApproximation

///////////////////////////////////////////////////////////////////////////////
class TRiGeometricRepresentation : public TRInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "TRiGeometricRepresentation"; }
	inline virtual const char *className() const { return TRiGeometricRepresentation::myClassName(); }

	inline TRiGeometricRepresentation(long aLineNo, RtToken type) : TRInterfaceCall(aLineNo), m_type(type) {}
	inline virtual int interfaceIdx() const { return IDX_GEOMETRIC_REPRESENTATION; }
	inline virtual void replay(TRi &ri) { ri.geometricRepresentation(m_type.c_str()); }
	inline TRiGeometricRepresentation &operator=(const TRiGeometricRepresentation &) {
		return *this;
	}
}; // TRiGeometricRepresentation

///////////////////////////////////////////////////////////////////////////////
class TRiOrientation : public TRInterfaceCall {
protected:
	std::string m_orientation;
public:
	inline static const char *myClassName(void) { return "TRiOrientation"; }
	inline virtual const char *className() const { return TRiOrientation::myClassName(); }

	inline TRiOrientation(long aLineNo, RtToken orientation) : TRInterfaceCall(aLineNo), m_orientation(orientation) {}
	inline virtual int interfaceIdx() const { return IDX_ORIENTATION; }
	inline virtual void replay(TRi &ri) { ri.orientation(m_orientation.c_str()); }
	inline TRiOrientation &operator=(const TRiOrientation &) {
		return *this;
	}
}; // TRiOrientation

///////////////////////////////////////////////////////////////////////////////
class TRiReverseOrientation : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiReverseOrientation"; }
	inline virtual const char *className() const { return TRiReverseOrientation::myClassName(); }

	inline TRiReverseOrientation(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_REVERSE_ORIENTATION; }
	inline virtual void replay(TRi &ri) { ri.reverseOrientation(); }
	inline TRiReverseOrientation &operator=(const TRiReverseOrientation &) {
		return *this;
	}
}; // TRiReverseOrientation

///////////////////////////////////////////////////////////////////////////////
class TRiSides : public TRInterfaceCall {
protected:
	RtInt m_nsides;
public:
	inline static const char *myClassName(void) { return "TRiSides"; }
	inline virtual const char *className() const { return TRiSides::myClassName(); }

	inline TRiSides(long aLineNo, RtInt nsides) : TRInterfaceCall(aLineNo), m_nsides(nsides) {}
	inline virtual int interfaceIdx() const { return IDX_SIDES; }
	inline virtual void replay(TRi &ri) { ri.sides(m_nsides); }
	inline TRiSides &operator=(const TRiSides &) {
		return *this;
	}
}; // TRiSides

///////////////////////////////////////////////////////////////////////////////
class TRiBasis : public TRInterfaceCall {
protected:
	RtInt m_ustep, m_vstep;
	RtBasis m_ubasis, m_vbasis;
public:
	inline static const char *myClassName(void) { return "TRiBasis"; }
	inline virtual const char *className() const { return TRiBasis::myClassName(); }

	inline TRiBasis(long aLineNo, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) : TRInterfaceCall(aLineNo), m_ustep(ustep), m_vstep(vstep) {
		memcpy(m_ubasis, ubasis, sizeof(RtBasis));
		memcpy(m_vbasis, vbasis, sizeof(RtBasis));
	}
	inline virtual int interfaceIdx() const { return IDX_BASIS; }
	inline virtual void replay(TRi &ri) { ri.basis(m_ubasis, m_ustep, m_vbasis, m_vstep); }
	inline TRiBasis &operator=(const TRiBasis &) {
		return *this;
	}
}; // TRiBasis

///////////////////////////////////////////////////////////////////////////////
class TRiTrimCurve : public TRInterfaceCall {
protected:
	TTrimCurveData m_trimCurve;

public:
	inline static const char *myClassName(void) { return "TRiTrimCurve"; }
	inline virtual const char *className() const { return TRiTrimCurve::myClassName(); }

	inline TRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) : TRInterfaceCall(aLineNo), m_trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w) {}
	inline TRiTrimCurve(long aLineNo, const TTrimCurveData &trimCurve)  : TRInterfaceCall(aLineNo), m_trimCurve(trimCurve) {}
	inline virtual int interfaceIdx() const { return IDX_TRIM_CURVE; }
	inline virtual void replay(TRi &ri) {
		ri.trimCurve(m_trimCurve.m_nLoops,
			m_trimCurve.m_nCurves.empty() ? NULL : &m_trimCurve.m_nCurves[0],
			m_trimCurve.m_order.empty() ? NULL : &m_trimCurve.m_order[0],
			m_trimCurve.m_knots.empty() ? NULL : &m_trimCurve.m_knots[0],
			m_trimCurve.m_min.empty() ? NULL : &m_trimCurve.m_min[0],
			m_trimCurve.m_max.empty() ? NULL : &m_trimCurve.m_max[0],
			m_trimCurve.m_n.empty() ? NULL : &m_trimCurve.m_n[0],
			m_trimCurve.m_u.empty() ? NULL : &m_trimCurve.m_u[0],
			m_trimCurve.m_v.empty() ? NULL : &m_trimCurve.m_v[0],
			m_trimCurve.m_w.empty() ? NULL : &m_trimCurve.m_w[0]);
	}
	inline TRiTrimCurve &operator=(const TRiTrimCurve &) {
		return *this;
	}
	inline const TTrimCurveData &trimCurve() { return m_trimCurve; }
}; // TRiTrimCurve

///////////////////////////////////////////////////////////////////////////////
class TRiIdentity : public TRInterfaceCall {
public:
	inline static const char *myClassName(void) { return "TRiIdentity"; }
	inline virtual const char *className() const { return TRiIdentity::myClassName(); }

	inline TRiIdentity(long aLineNo) : TRInterfaceCall(aLineNo) {}
	inline virtual int interfaceIdx() const { return IDX_IDENTITY; }
	inline virtual void replay(TRi &ri) { ri.identity(); }
	inline TRiIdentity &operator=(const TRiIdentity &) {
		return *this;
	}
}; // TRiIdentity

///////////////////////////////////////////////////////////////////////////////
class TRiTransform : public TRInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "TRiTransform"; }
	inline virtual const char *className() const { return TRiTransform::myClassName(); }

	inline TRiTransform(long aLineNo, RtMatrix transform) : TRInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual int interfaceIdx() const { return IDX_TRANSFORM; }
	inline virtual void replay(TRi &ri) { ri.transform(m_transform); }
	inline TRiTransform &operator=(const TRiTransform &) {
		return *this;
	}
}; // TRiTransform

///////////////////////////////////////////////////////////////////////////////
class TRiConcatTransform : public TRInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "TRiConcatTransform"; }
	inline virtual const char *className() const { return TRiConcatTransform::myClassName(); }

	inline TRiConcatTransform(long aLineNo, RtMatrix transform) : TRInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual int interfaceIdx() const { return IDX_CONCAT_TRANSFORM; }
	inline virtual void replay(TRi &ri) { ri.concatTransform(m_transform); }
	inline TRiConcatTransform &operator=(const TRiConcatTransform &) {
		return *this;
	}
}; // TRiConcatTransform

///////////////////////////////////////////////////////////////////////////////
class TRiPerspective : public TRInterfaceCall {
protected:
	RtFloat m_fov;
public:
	inline static const char *myClassName(void) { return "TRiPerspective"; }
	inline virtual const char *className() const { return TRiPerspective::myClassName(); }

	inline TRiPerspective(long aLineNo, RtFloat fov) : TRInterfaceCall(aLineNo), m_fov(fov) {}
	inline virtual int interfaceIdx() const { return IDX_PERSPECTIVE; }
	inline virtual void replay(TRi &ri) { ri.perspective(m_fov); }
	inline TRiPerspective &operator=(const TRiPerspective &) {
		return *this;
	}
}; // TRiPerspective

///////////////////////////////////////////////////////////////////////////////
class TRiTranslate : public TRInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "TRiTranslate"; }
	inline virtual const char *className() const { return TRiTranslate::myClassName(); }

	inline TRiTranslate(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : TRInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual int interfaceIdx() const { return IDX_TRANSLATE; }
	inline virtual void replay(TRi &ri) { ri.translate(m_dx, m_dy, m_dz); }
	inline TRiTranslate &operator=(const TRiTranslate &) {
		return *this;
	}
}; // TRiTranslate

///////////////////////////////////////////////////////////////////////////////
class TRiRotate : public TRInterfaceCall {
protected:
	RtFloat m_angle, m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "TRiRotate"; }
	inline virtual const char *className() const { return TRiRotate::myClassName(); }

	inline TRiRotate(long aLineNo, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) : TRInterfaceCall(aLineNo), m_angle(angle), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual int interfaceIdx() const { return IDX_ROTATE; }
	inline virtual void replay(TRi &ri) { ri.rotate(m_angle, m_dx, m_dy, m_dz); }
	inline TRiRotate &operator=(const TRiRotate &) {
		return *this;
	}
}; // TRiRotate

///////////////////////////////////////////////////////////////////////////////
class TRiScale : public TRInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "TRiScale"; }
	inline virtual const char *className() const { return TRiScale::myClassName(); }

	inline TRiScale(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : TRInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual int interfaceIdx() const { return IDX_SCALE; }
	inline virtual void replay(TRi &ri) { ri.scale(m_dx, m_dy, m_dz); }
	inline TRiScale &operator=(const TRiScale &) {
		return *this;
	}
}; // TRiScale

///////////////////////////////////////////////////////////////////////////////
class TRiSkew : public TRInterfaceCall {
protected:
	RtFloat m_angle;
	RtFloat m_dx1, m_dy1, m_dz1;
	RtFloat m_dx2, m_dy2, m_dz2;
public:
	inline static const char *myClassName(void) { return "TRiSkew"; }
	inline virtual const char *className() const { return TRiSkew::myClassName(); }

	inline TRiSkew(long aLineNo, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) : TRInterfaceCall(aLineNo), m_angle(angle), m_dx1(dx1), m_dy1(dy1), m_dz1(dz1), m_dx2(dx2), m_dy2(dy2), m_dz2(dz2) {}
	inline virtual int interfaceIdx() const { return IDX_SKEW; }
	inline virtual void replay(TRi &ri) { ri.skew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2); }
	inline TRiSkew &operator=(const TRiSkew &) {
		return *this;
	}
}; // TRiSkew

///////////////////////////////////////////////////////////////////////////////
class TRiDeformation : public TVarParamRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiDeformation"; }
	inline virtual const char *className() const { return TRiDeformation::myClassName(); }

	inline TRiDeformation(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_DEFORMATION; }
	inline virtual void replay(TRi &ri) { ri.deformationV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiDeformation &operator=(const TRiDeformation &) {
		return *this;
	}
}; // TRiDeformation

///////////////////////////////////////////////////////////////////////////////
class TRiCoordinateSystem : public TRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiCoordinateSystem"; }
	inline virtual const char *className() const { return TRiCoordinateSystem::myClassName(); }

	inline TRiCoordinateSystem(long aLineNo, const char *name) : TRInterfaceCall(aLineNo), m_name(name) {}
	inline virtual int interfaceIdx() const { return IDX_COORDINATE_SYSTEM; }
	inline virtual void replay(TRi &ri) { ri.coordinateSystem(m_name.c_str()); }
	inline TRiCoordinateSystem &operator=(const TRiCoordinateSystem &) {
		return *this;
	}
}; // TRiCoordinateSystem

///////////////////////////////////////////////////////////////////////////////
class TRiCoordSysTransform : public TRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiCoordSysTransform"; }
	inline virtual const char *className() const { return TRiCoordSysTransform::myClassName(); }

	inline TRiCoordSysTransform(long aLineNo, const char *name) : TRInterfaceCall(aLineNo), m_name(name) {}
	inline virtual int interfaceIdx() const { return IDX_COORDINATE_SYSTEM; }
	inline virtual void replay(TRi &ri) { ri.coordSysTransform(m_name.c_str()); }
	inline TRiCoordSysTransform &operator=(const TRiCoordSysTransform &) {
		return *this;
	}
}; // TRiCoordSysTransform

///////////////////////////////////////////////////////////////////////////////
// ????
class TRiTransformPoints : public TRInterfaceCall {
protected:
	std::string m_fromSpace, m_toSpace;
	RtInt m_nPoints;
	RtPoint *m_points;
	// std::vector<RtFloat> m_fromPoints;
	// std::vector<RtFloat> m_toPoints;
public:
	inline static const char *myClassName(void) { return "TRiTransformPoints"; }
	inline virtual const char *className() const { return TRiTransformPoints::myClassName(); }

	inline TRiTransformPoints(long aLineNo, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points) : TRInterfaceCall(aLineNo), m_fromSpace(fromspace), m_toSpace(tospace), m_nPoints(npoints), m_points(points) {
	/*
	m_fromPoints.resize(m_npoints*3);
	m_toPoints.resize(m_npoints*3);
	if ( points )
	m_fromPoints.assign(&m_points[0][0], (&m_points[0][0])+m_npoints*3);
		*/
	}
	inline virtual int interfaceIdx() const { return IDX_COORDINATE_SYSTEM; }
	inline virtual void replay(TRi &ri) {
	/*
	m_toPoints = m_fromPoints;
	RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, (RtPoint *)&m_toPoints[0]);
		*/
		RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, m_points);
		p = p;
	}
	inline TRiTransformPoints &operator=(const TRiTransformPoints &) {
		return *this;
	}
}; // TRiTransformPoints

///////////////////////////////////////////////////////////////////////////////
class TRiPolygon : public TPolygonRInterfaceCall {
protected:
	RtInt m_nVertices;
public:
	inline static const char *myClassName(void) { return "TRiPolygon"; }
	inline virtual const char *className() const { return TRiPolygon::myClassName(); }

	inline TRiPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TPolygonRInterfaceCall(aLineNo), m_nVertices(nvertices)
	{
		TPolygonClasses p(nvertices);
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_POLYGON; }
	inline virtual void replay(TRi &ri) { ri.polygonV(m_nVertices, getSize(), getTokens(), getParams()); }
	inline TRiPolygon &operator=(const TRiPolygon &) {
		return *this;
	}
}; // TRiPolygon

///////////////////////////////////////////////////////////////////////////////
class TRiGeneralPolygon : public TPolygonRInterfaceCall {
protected:
	RtInt m_nLoops;
	std::vector<RtInt> m_nVerts;
public:
	inline static const char *myClassName(void) { return "TRiGeneralPolygon"; }
	inline virtual const char *className() const { return TRiGeneralPolygon::myClassName(); }

	TRiGeneralPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_GENERAL_POLYGON; }
	inline virtual void replay(TRi &ri) {
		ri.generalPolygonV(m_nLoops,
			m_nVerts.empty() ? NULL : &m_nVerts[0],
			getSize(), getTokens(), getParams());
	}
	inline TRiGeneralPolygon &operator=(const TRiGeneralPolygon &) {
		return *this;
	}
}; // TRiGeneralPolygon

///////////////////////////////////////////////////////////////////////////////
class TRiPointsPolygons : public TPolygonRInterfaceCall {
protected:
	RtInt m_nPolys;
	std::vector<RtInt> m_nVerts;
	std::vector<RtInt> m_verts;
public:
	inline static const char *myClassName(void) { return "TRiPointsPolygons"; }
	inline virtual const char *className() const { return TRiPointsPolygons::myClassName(); }

	TRiPointsPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_POINTS_POLYGONS; }
	inline virtual void replay(TRi &ri) {
		ri.pointsPolygonsV(
			m_nPolys,
			m_nVerts.empty() ? NULL : &m_nVerts[0],
			m_verts.empty() ? NULL : &m_verts[0],
			getSize(), getTokens(), getParams()); }
	inline TRiPointsPolygons &operator=(const TRiPointsPolygons &) {
		return *this;
	}
}; // TRiPointsPolygons

///////////////////////////////////////////////////////////////////////////////
class TRiPointsGeneralPolygons : public TPolygonRInterfaceCall {
protected:
	RtInt m_nPolys;
	std::vector<RtInt> m_nLoops;
	std::vector<RtInt> m_nVerts;
	std::vector<RtInt> m_verts;
public:
	inline static const char *myClassName(void) { return "TRiPointsGeneralPolygons"; }
	inline virtual const char *className() const { return TRiPointsGeneralPolygons::myClassName(); }

	TRiPointsGeneralPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_POINTS_POLYGONS; }
	inline virtual void replay(TRi &ri) {
		ri.pointsGeneralPolygonsV(
			m_nPolys,
			m_nLoops.empty() ? NULL : &m_nLoops[0],
			m_nVerts.empty() ? NULL : &m_nVerts[0],
			m_verts.empty() ? NULL : &m_verts[0],
			getSize(), getTokens(), getParams()); }
	inline TRiPointsGeneralPolygons &operator=(const TRiPointsGeneralPolygons &) {
		return *this;
	}
}; // TRiPointsGeneralPolygons

///////////////////////////////////////////////////////////////////////////////
class TRiPatch : public TUVRInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "TRiPatch"; }
	inline virtual const char *className() const { return TRiPatch::myClassName(); }

	inline TRiPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo), m_type(type)
	{
		TPatchClasses p(type);
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_PATCH; }
	inline virtual void replay(TRi &ri) { ri.patchV(m_type.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiPatch &operator=(const TRiPatch &) {
		return *this;
	}
}; // TRiPatch

///////////////////////////////////////////////////////////////////////////////
class TRiPatchMesh : public TUVRInterfaceCall {
protected:
	std::string m_type, m_uwrap, m_vwrap;
	RtInt m_nu, m_nv;
	RtInt m_ustep, m_vstep;
public:
	inline static const char *myClassName(void) { return "TRiPatchMesh"; }
	inline virtual const char *className() const { return TRiPatchMesh::myClassName(); }

	TRiPatchMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_PATCH; }
	inline virtual void replay(TRi &ri) { ri.patchMeshV(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiPatchMesh &operator=(const TRiPatchMesh &) {
		return *this;
	}
}; // TRiPatchMesh

///////////////////////////////////////////////////////////////////////////////
class TRiNuPatch : public TGeometryRInterfaceCall {
protected:
	RtInt m_nu, m_uorder, m_nv, m_vorder;
	std::vector<RtFloat> m_uknot, m_vknot;
	RtFloat m_umin, m_umax, m_vmin, m_vmax;
public:
	inline static const char *myClassName(void) { return "TRiNuPatch"; }
	inline virtual const char *className() const { return TRiNuPatch::myClassName(); }

	TRiNuPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_PATCH; }
	inline virtual void replay(TRi &ri) {
		ri.nuPatchV(m_nu, m_uorder,
			m_uknot.empty() ? NULL : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? NULL : &m_vknot[0],
			m_vmin, m_vmax, getSize(), getTokens(), getParams()); }
	inline TRiNuPatch &operator=(const TRiNuPatch &) {
		return *this;
	}
	inline virtual int segments() const { return (1+m_nu-m_uorder)*(1+m_nv-m_vorder); }
}; // TRiNuPatch

///////////////////////////////////////////////////////////////////////////////
class TRiSubdivisionMesh : public TGeometryRInterfaceCall {
protected:
//	struct TRiSubdivisionMeshData {
		std::string m_scheme;
		RtInt m_nfaces, m_ntags, m_nvertices;
		RtInt m_nedges; // number of edges in the surface
		std::vector<RtInt> m_nverts, m_verts, m_nargs, m_intargs;
		std::vector<RtFloat>    m_floargs;
		std::deque<std::string> m_strtags;
		std::vector<RtToken>    m_tags;
//	};
//	std::vector<TRiSubdivisionMeshData> m_data;
public:
	inline static const char *myClassName(void) { return "TRiSubdivisionMesh"; }
	inline virtual const char *className() const { return TRiSubdivisionMesh::myClassName(); }

	TRiSubdivisionMesh(long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);
	inline virtual int interfaceIdx() const { return IDX_SUBDIVISION_MESH; }
	inline virtual void replay(TRi &ri) {
		ri.subdivisionMeshV(
			m_scheme.c_str(), m_nfaces,
			m_nverts.empty() ? NULL : &(m_nverts[0]),
			m_verts.empty() ? NULL : &(m_verts[0]),
			m_ntags,
			m_tags.empty() ? NULL : &(m_tags[0]),
			m_nargs.empty() ? NULL : &(m_nargs[0]),
			m_intargs.empty() ? NULL : &(m_intargs[0]),
			m_floargs.empty() ? NULL : &(m_floargs[0]),
			getSize(), getTokens(), getParams()
			);

		/*
		int idx = 0;
		ri.subdivisionMeshV(
			m_data[idx].m_scheme.c_str(), m_data[idx].m_nfaces, &(m_data[idx].m_nverts[0]), &(m_data[idx].m_verts[0]),
			m_data[idx].m_ntags, &(m_data[idx].m_tags[0]), &(m_data[idx].m_nargs[0]), &(m_data[idx].m_intargs[0]), &(m_data[idx].m_floargs[0]),
			getSize(), getTokens(), getParams()
			);
		*/
	}
	inline TRiSubdivisionMesh &operator=(const TRiSubdivisionMesh &) {
		return *this;
	}
}; // TRiSubdivisionMesh

///////////////////////////////////////////////////////////////////////////////
class TRiSphere : public TUVRInterfaceCall {
protected:
	RtFloat m_radius, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiSphere"; }
	inline virtual const char *className() const { return TRiSphere::myClassName(); }

	inline TRiSphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_SPHERE; }
	inline virtual void replay(TRi &ri) { ri.sphereV(m_radius, m_zmin, m_zmax, m_thetamax, getSize(), getTokens(), getParams()); }
	inline TRiSphere &operator=(const TRiSphere &) {
		return *this;
	}
}; // TRiSphere

///////////////////////////////////////////////////////////////////////////////
class TRiCone : public TUVRInterfaceCall {
protected:
	RtFloat m_height, m_radius, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiCone"; }
	inline virtual const char *className() const { return TRiCone::myClassName(); }

	inline TRiCone(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_CONE; }
	inline virtual void replay(TRi &ri) {
		ri.coneV(m_height, m_radius, m_thetamax, getSize(), getTokens(), getParams());
	}
	inline TRiCone &operator=(const TRiCone &) {
		return *this;
	}
}; // TRiCone

///////////////////////////////////////////////////////////////////////////////
class TRiCylinder : public TUVRInterfaceCall {
protected:
	RtFloat m_radius, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiCylinder"; }
	inline virtual const char *className() const { return TRiCylinder::myClassName(); }

	inline TRiCylinder(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_SPHERE; }
	inline virtual void replay(TRi &ri) { ri.cylinderV(m_radius, m_zmin, m_zmax, m_thetamax, getSize(), getTokens(), getParams()); }
	inline TRiCylinder &operator=(const TRiCylinder &) {
		return *this;
	}
}; // TRiCylinder

///////////////////////////////////////////////////////////////////////////////
class TRiHyperboloid : public TUVRInterfaceCall {
protected:
	RtPoint m_point1, m_point2;
	RtFloat m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiHyperboloid"; }
	inline virtual const char *className() const { return TRiHyperboloid::myClassName(); }

	inline TRiHyperboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo), m_thetamax(thetamax)
	{
		int i;
		for ( i = 0; i < 3; ++i ) {
			m_point1[i] = point1[i];
			m_point2[i] = point2[i];
		}
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_HYPERBOLOID; }
	inline virtual void replay(TRi &ri) { ri.hyperboloidV(m_point1, m_point2, m_thetamax, getSize(), getTokens(), getParams()); }
	inline TRiHyperboloid &operator=(const TRiHyperboloid &) {
		return *this;
	}
}; // TRiHyperboloid

///////////////////////////////////////////////////////////////////////////////
class TRiParaboloid : public TUVRInterfaceCall {
protected:
	RtFloat m_rmax, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiParaboloid"; }
	inline virtual const char *className() const { return TRiParaboloid::myClassName(); }

	inline TRiParaboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_rmax(rmax), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_PARABOLOID; }
	inline virtual void replay(TRi &ri) { ri.paraboloidV(m_rmax, m_zmin, m_zmax, m_thetamax, getSize(), getTokens(), getParams()); }
	inline TRiParaboloid &operator=(const TRiParaboloid &) {
		return *this;
	}
}; // TRiParaboloid

///////////////////////////////////////////////////////////////////////////////
class TRiDisk : public TUVRInterfaceCall {
protected:
	RtFloat m_height, m_radius, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiDisk"; }
	inline virtual const char *className() const { return TRiDisk::myClassName(); }

	inline TRiDisk(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_DISK; }
	inline virtual void replay(TRi &ri) {
		ri.diskV(m_height, m_radius, m_thetamax, getSize(), getTokens(), getParams());
	}
	inline TRiDisk &operator=(const TRiDisk &) {
		return *this;
	}
}; // TRiDisk

///////////////////////////////////////////////////////////////////////////////
class TRiTorus : public TUVRInterfaceCall {
protected:
	RtFloat m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "TRiTorus"; }
	inline virtual const char *className() const { return TRiTorus::myClassName(); }

	inline TRiTorus(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TUVRInterfaceCall(aLineNo),
		  m_majorrad(majorrad), m_minorrad(minorrad),
		  m_phimin(phimin), m_phimax(phimax), m_thetamax(thetamax)
	{
		TQuadricClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_TORUS; }
	inline virtual void replay(TRi &ri) { ri.torusV(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, getSize(), getTokens(), getParams()); }
	inline TRiTorus &operator=(const TRiTorus &) {
		return *this;
	}
}; // TRiTorus

///////////////////////////////////////////////////////////////////////////////
class TRiPoints : public TGeometryRInterfaceCall {
protected:
	RtInt m_npts;
public:
	inline static const char *myClassName(void) { return "TRiPoints"; }
	inline virtual const char *className() const { return TRiPoints::myClassName(); }

	inline TRiPoints(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TGeometryRInterfaceCall(aLineNo), m_npts(npts)
	{
		TPointsClasses p(npts);
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_POINTS; }
	inline virtual void replay(TRi &ri) { ri.pointsV(m_npts, getSize(), getTokens(), getParams()); }
	inline TRiPoints &operator=(const TRiPoints &) {
		return *this;
	}
}; // TRiPoints

///////////////////////////////////////////////////////////////////////////////
class TRiCurves : public TGeometryRInterfaceCall {
protected:
	std::string m_type, m_wrap;
	RtInt m_ncurves, m_ustep, m_vstep;
	std::vector<RtInt> m_nverts;
public:
	inline static const char *myClassName(void) { return "TRiCurves"; }
	inline virtual const char *className() const { return TRiCurves::myClassName(); }

	TRiCurves(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_CURVES; }
	inline virtual void replay(TRi &ri) {
		ri.curvesV(m_type.c_str(), m_ncurves,
			m_nverts.empty() ? NULL : &m_nverts[0],
			m_wrap.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiCurves &operator=(const TRiCurves &) {
		return *this;
	}
}; // TRiCurves

///////////////////////////////////////////////////////////////////////////////
class TRiBlobby : public TGeometryRInterfaceCall {
protected:
	RtInt m_nleaf, m_ncode, m_nflt, m_nstr;
	std::vector<RtInt> m_code;
	std::vector<RtFloat> m_flt;
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;
public:
	inline static const char *myClassName(void) { return "TRiBlobby"; }
	inline virtual const char *className() const { return TRiBlobby::myClassName(); }

	TRiBlobby(long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual int interfaceIdx() const { return IDX_BLOBBY; }
	inline virtual void replay(TRi &ri) {
		ri.blobbyV(m_nleaf,
			m_ncode, m_code.empty() ? (RtInt *)NULL : &m_code[0],
			m_nflt, m_flt.empty() ? (RtFloat *)NULL : &m_flt[0],
			m_nstr, m_str.empty() ? (RtString *)NULL : &m_str[0],
			getSize(), getTokens(), getParams());
	}
	inline TRiBlobby &operator=(const TRiBlobby &) {
		return *this;
	}
}; // TRiBlobby

///////////////////////////////////////////////////////////////////////////////
class TRiProcedural : public TRInterfaceCall {
protected:
	RtBound m_bound;
	RtProcSubdivFunc m_subdivfunc;
	RtProcFreeFunc m_freefunc;
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;

	void insertData(RtPointer data, TRi &ri);
public:
	inline static const char *myClassName(void) { return "TRiProcedural"; }
	inline virtual const char *className() const { return TRiProcedural::myClassName(); }

	inline TRiProcedural(long aLineNo, RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc, TRi &ri)
		: TRInterfaceCall(aLineNo), m_subdivfunc(subdivfunc), m_freefunc(freefunc)
	{
		memcpy(m_bound, bound, sizeof(RtBound));
		insertData(data, ri);
	}
	inline virtual int interfaceIdx() const { return IDX_PROCEDURAL; }
	inline virtual void replay(TRi &ri) {
		ri.procedural(m_str.empty() ? (RtPointer)NULL :
			m_str.empty() ? NULL : (RtPointer)&m_str[0],
			m_bound, m_subdivfunc, m_freefunc);
	}
	inline TRiProcedural &operator=(const TRiProcedural &) {
		return *this;
	}
}; // TRiProcedural

   /*
   // PROC_... are not called directly, only parameters for RiProcedural()
   #define IDX_PROC_DELAYED_READ_ARCHIVE	 94
   #define IDX_PROC_RUN_PROGRAM			 95
   #define IDX_PROC_DYNAMIC_LOAD			 96
*/

///////////////////////////////////////////////////////////////////////////////
class TRiGeometry : public TGeometryRInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "TRiGeometry"; }
	inline virtual const char *className() const { return TRiGeometry::myClassName(); }

	inline TRiGeometry(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TGeometryRInterfaceCall(aLineNo), m_name(name)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_GEOMETRY; }
	inline virtual void replay(TRi &ri) { ri.geometryV(m_name.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiGeometry &operator=(const TRiGeometry &) {
		return *this;
	}
}; // TRiGeometry

///////////////////////////////////////////////////////////////////////////////
class TRiObjectInstance : public TRInterfaceCall {
protected:
	RtObjectHandle m_handle;
public:
	inline static const char *myClassName(void) { return "TRiObjectInstance"; }
	inline virtual const char *className() const { return TRiObjectInstance::myClassName(); }

	inline TRiObjectInstance(long aLineNo, RtObjectHandle handle) : TRInterfaceCall(aLineNo), m_handle(handle) {}
	inline virtual int interfaceIdx() const { return IDX_OBJECT_INSTANCE; }
	inline virtual void replay(TRi &ri) { ri.objectInstance(m_handle); }
	// inline virtual RtObjectHandle handleRef() { return (RtObjectHandle)m_handle; }
	// inline virtual RtObjectHandle handle2() { return m_handle ? m_handle.m_handle : (RtObjectHandle)-1); }
	inline TRiObjectInstance &operator=(const TRiObjectInstance &) {
		return *this;
	}
}; // TRiObjectInstance

///////////////////////////////////////////////////////////////////////////////
class TRiMakeTexture : public TVarParamRInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	RtFilterFunc m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "TRiMakeTexture"; }
	inline virtual const char *className() const { return TRiMakeTexture::myClassName(); }

	inline TRiMakeTexture(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_filterfunc(filterfunc), m_swidth(swidth), m_twidth(twidth)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_MAKE_TEXTURE; }
	inline virtual void replay(TRi &ri) { ri.makeTextureV(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), m_filterfunc, m_swidth, m_twidth, getSize(), getTokens(), getParams()); }
	inline TRiMakeTexture &operator=(const TRiMakeTexture &) {
		return *this;
	}
}; // TRiMakeTexture

///////////////////////////////////////////////////////////////////////////////
class TRiMakeBump : public TVarParamRInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	RtFilterFunc m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "TRiMakeBump"; }
	inline virtual const char *className() const { return TRiMakeBump::myClassName(); }

	inline TRiMakeBump(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_filterfunc(filterfunc), m_swidth(swidth), m_twidth(twidth)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_MAKE_BUMP; }
	inline virtual void replay(TRi &ri) { ri.makeBumpV(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), m_filterfunc, m_swidth, m_twidth, getSize(), getTokens(), getParams()); }
	inline TRiMakeBump &operator=(const TRiMakeBump &) {
		return *this;
	}
}; // TRiMakeBump

///////////////////////////////////////////////////////////////////////////////
class TRiMakeLatLongEnvironment : public TVarParamRInterfaceCall {
protected:
	std::string m_pic, m_tex;
	RtFilterFunc m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "TRiMakeLatLongEnvironment"; }
	inline virtual const char *className() const { return TRiMakeLatLongEnvironment::myClassName(); }

	inline TRiMakeLatLongEnvironment(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_filterfunc(filterfunc), m_swidth(swidth), m_twidth(twidth)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_MAKE_LAT_LONG_ENVIRONMENT; }
	inline virtual void replay(TRi &ri) { ri.makeLatLongEnvironmentV(m_pic.c_str(), m_tex.c_str(), m_filterfunc, m_swidth, m_twidth, getSize(), getTokens(), getParams()); }
	inline TRiMakeLatLongEnvironment &operator=(const TRiMakeLatLongEnvironment &) {
		return *this;
	}
}; // TRiMakeLatLongEnvironment

///////////////////////////////////////////////////////////////////////////////
class TRiMakeCubeFaceEnvironment : public TVarParamRInterfaceCall {
protected:
	std::string m_px, m_nx, m_py, m_ny, m_pz, m_nz, m_tex;
	RtFloat m_fov;
	RtFilterFunc m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "TRiMakeCubeFaceEnvironment"; }
	inline virtual const char *className() const { return TRiMakeCubeFaceEnvironment::myClassName(); }

	inline TRiMakeCubeFaceEnvironment(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_px(px), m_nx(nx), m_py(py), m_ny(ny), m_pz(pz), m_nz(nz), m_tex(tex), m_fov(fov), m_filterfunc(filterfunc), m_swidth(swidth), m_twidth(twidth)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_MAKE_CUBE_FACE_ENVIRONMENT; }
	inline virtual void replay(TRi &ri) { ri.makeCubeFaceEnvironmentV(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, m_filterfunc, m_swidth, m_twidth, getSize(), getTokens(), getParams()); }
	inline TRiMakeCubeFaceEnvironment &operator=(const TRiMakeCubeFaceEnvironment &) {
		return *this;
	}
}; // TRiMakeCubeFaceEnvironment

///////////////////////////////////////////////////////////////////////////////
class TRiMakeShadow : public TVarParamRInterfaceCall {
protected:
	std::string m_pic, m_tex;
public:
	inline static const char *myClassName(void) { return "TRiMakeShadow"; }
	inline virtual const char *className() const { return TRiMakeShadow::myClassName(); }

	inline TRiMakeShadow(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_pic(pic), m_tex(tex)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_MAKE_SHADOW; }
	inline virtual void replay(TRi &ri) { ri.makeShadowV(m_pic.c_str(), m_tex.c_str(), getSize(), getTokens(), getParams()); }
	inline TRiMakeShadow &operator=(const TRiMakeShadow &) {
		return *this;
	}
}; // TRiMakeShadow

///////////////////////////////////////////////////////////////////////////////
class TRiArchiveRecord : public TRInterfaceCall {
protected:
	std::string m_type;
	std::string m_line;
public:
	inline static const char *myClassName(void) { return "TRiArchiveRecord"; }
	inline virtual const char *className() const { return TRiArchiveRecord::myClassName(); }

	inline TRiArchiveRecord(long aLineNo, RtToken type, const char *line) : TRInterfaceCall(aLineNo), m_type(type), m_line(line) {}
	inline virtual int interfaceIdx() const { return IDX_ARCHIVE_RECORD; }
	inline virtual void replay(TRi &ri) { ri.archiveRecordV(m_type.c_str(), m_line.c_str()); }
	inline virtual void replay(TRi &ri, RtArchiveCallback callback) {
		ri.archiveRecordV(m_type.c_str(), m_line.c_str());
		if ( callback )
			(*callback)(&ri, m_type.c_str(), m_line.c_str());
	}
	inline TRiArchiveRecord &operator=(const TRiArchiveRecord &) {
		return *this;
	}
}; // TRiArchiveRecord

///////////////////////////////////////////////////////////////////////////////
class TRiReadArchive : public TVarParamRInterfaceCall {
protected:
	std::string m_filename;
	RtArchiveCallback m_callback;
public:
	inline static const char *myClassName(void) { return "TRiReadArchive"; }
	inline virtual const char *className() const { return TRiReadArchive::myClassName(); }

	inline TRiReadArchive(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtString filename, RtArchiveCallback callback,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TVarParamRInterfaceCall(aLineNo), m_filename(filename), m_callback(callback)
	{
		TParameterClasses p;
		setParams(declmap, p, colorComps, n, tokens, params);
	}
	inline virtual int interfaceIdx() const { return IDX_READ_ARCHIVE; }
	inline virtual void replay(TRi &ri, RtArchiveCallback callback) { ri.readArchiveV(m_filename.c_str(), callback, getSize(), getTokens(), getParams()); }
	inline virtual void replay(TRi &ri) { ri.readArchiveV(m_filename.c_str(), m_callback, getSize(), getTokens(), getParams()); }
	inline TRiReadArchive &operator=(const TRiReadArchive &) {
		return *this;
	}
}; // TRiReadArchive


///////////////////////////////////////////////////////////////////////////////
//! Interface, return a new instance of a TRi... class.
class TNewInterface {
protected:
	inline virtual TRiDeclare *newRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
	{
		return new TRiDeclare(aLineNo, name, declaration);	
	}

	inline virtual TRiErrorHandler *newRiErrorHandler(
		long aLineNo,
		RtErrorHandler handler)
	{
		return new TRiErrorHandler(aLineNo, handler);
	}

	inline virtual TRiBegin *newRiBegin(long aLineNo, RtToken name) {
		return new TRiBegin(aLineNo, name);
	}

	inline virtual TRiEnd *newRiEnd(long aLineNo) {
		return new TRiEnd(aLineNo);
	}

	inline virtual TRiFrameBegin *newRiFrameBegin(long aLineNo, RtInt number) {
		return new TRiFrameBegin(aLineNo, number);
	}

	inline virtual TRiFrameEnd *newRiFrameEnd(long aLineNo) {
		return new TRiFrameEnd(aLineNo);
	}

	inline virtual TRiWorldBegin *newRiWorldBegin(long aLineNo) {
		return new TRiWorldBegin(aLineNo);
	}

	inline virtual TRiWorldEnd *newRiWorldEnd(long aLineNo) {
		return new TRiWorldEnd(aLineNo);
	}

	inline virtual TRiAttributeBegin *newRiAttributeBegin(long aLineNo) {
		return new TRiAttributeBegin(aLineNo);
	}

	inline virtual TRiAttributeEnd *newRiAttributeEnd(long aLineNo) {
		return new TRiAttributeEnd(aLineNo);
	}

	inline virtual TRiTransformBegin *newRiTransformBegin(long aLineNo) {
		return new TRiTransformBegin(aLineNo);
	}

	inline virtual TRiTransformEnd *newRiTransformEnd(long aLineNo) {
		return new TRiTransformEnd(aLineNo);
	}

	inline virtual TRiSolidBegin *newRiSolidBegin(long aLineNo, RtToken operation) {
		return new TRiSolidBegin(aLineNo, operation);
	}

	inline virtual TRiSolidEnd *newRiSolidEnd(long aLineNo) {
		return new TRiSolidEnd(aLineNo);
	}

	inline virtual TRiObjectBegin *newRiObjectBegin(long aLineNo, RtObjectHandle h) {
		return new TRiObjectBegin(aLineNo, h);
	}

	inline virtual TRiObjectEnd *newRiObjectEnd(long aLineNo) {
		return new TRiObjectEnd(aLineNo);
	}

	inline virtual TRiMotionBegin *newRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) {
		return new TRiMotionBegin(aLineNo, n, f);
	}

	inline virtual TRiMotionEnd *newRiMotionEnd(long aLineNo) {
		return new TRiMotionEnd(aLineNo);
	}

	inline virtual TRiFormat *newRiFormat(long aLineNo, RtInt xres, RtInt yres, RtFloat aspect) {
		return new TRiFormat(aLineNo, xres, yres, aspect);
	}

	inline virtual TRiFrameAspectRatio *newRiFrameAspectRatio(long aLineNo, RtFloat aspect) {
		return new TRiFrameAspectRatio(aLineNo, aspect);
	}

	inline virtual TRiScreenWindow *newRiScreenWindow(long aLineNo, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
		return new TRiScreenWindow(aLineNo, left, right, bot, top);
	}

	inline virtual TRiCropWindow *newRiCropWindow(long aLineNo, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
		return new TRiCropWindow(aLineNo, xmin, xmax, ymin, ymax);
	}

	inline virtual TRiProjection *newRiProjection(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiProjection(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiClipping *newRiClipping(long aLineNo, RtFloat hither, RtFloat yon) {
		return new TRiClipping(aLineNo, hither, yon);
	}

	inline virtual TRiClippingPlane *newRiClippingPlane(long aLineNo, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
		return new TRiClippingPlane(aLineNo, x, y, z, nx, ny, nz);
	}

	inline virtual TRiDepthOfField *newRiDepthOfField(long aLineNo, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
		return new TRiDepthOfField(aLineNo, fstop, focallength, focaldistance);
	}

	inline virtual TRiShutter *newRiShutter(long aLineNo, RtFloat smin, RtFloat smax) {
		return new TRiShutter(aLineNo, smin, smax);
	}

	inline virtual TRiPixelVariance *newRiPixelVariance(long aLineNo, RtFloat variation) {
		return new TRiPixelVariance(aLineNo, variation);
	}

	inline virtual TRiPixelSamples *newRiPixelSamples(long aLineNo, RtFloat xsamples, RtFloat ysamples) {
		return new TRiPixelSamples(aLineNo, xsamples, ysamples);
	}

	inline virtual TRiPixelFilter *newRiPixelFilter(long aLineNo, RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) {
		return new TRiPixelFilter(aLineNo, function, xwidth, ywidth);
	}

	inline virtual TRiExposure *newRiExposure(long aLineNo, RtFloat gain, RtFloat gamma) {
		return new TRiExposure(aLineNo, gain, gamma);
	}

	inline virtual TRiImager *newRiImager(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiImager(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiQuantize *newRiQuantize(long aLineNo, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
		return new TRiQuantize(aLineNo, type, one, qmin, qmax, ampl);
	}

	inline virtual TRiDisplay *newRiDisplay(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken name, RtToken type, RtToken mode,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiDisplay(aLineNo, declmap, colorComps, name, type, mode, n, tokens, params);
	}

	inline virtual TRiHider *newRiHider(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiHider(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiColorSamples *newRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
		return new TRiColorSamples(aLineNo, N, nRGB, RGBn);
	}

	inline virtual TRiRelativeDetail *newRiRelativeDetail(long aLineNo, RtFloat relativedetail) {
		return new TRiRelativeDetail(aLineNo, relativedetail);
	}

	inline virtual TRiOption *newRiOption(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiOption(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiLightSource *newRiLightSource(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name, RtLightHandle handle,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiLightSource(aLineNo, declmap, colorComps, name, handle, n, tokens, params);
	}

	inline virtual TRiAreaLightSource *newRiAreaLightSource(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name, RtLightHandle handle,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiAreaLightSource(aLineNo, declmap, colorComps, name, handle, n, tokens, params);
	}

	inline virtual TRiIlluminate *newRiIlluminate(long aLineNo, RtLightHandle handle, RtBoolean onoff) {
		return new TRiIlluminate(aLineNo, handle, onoff);
	}

	inline virtual TRiAttribute *newRiAttribute(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiAttribute(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiColor *newRiColor(long aLineNo, RtInt size, RtColor Cs) {
		return new TRiColor(aLineNo, size, Cs);
	}

	inline virtual TRiOpacity *newRiOpacity(long aLineNo, RtInt size, RtColor Cs) {
		return new TRiOpacity(aLineNo, size, Cs);
	}

	inline virtual TRiSurface *newRiSurface(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiSurface(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiAtmosphere *newRiAtmosphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiAtmosphere(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiInterior *newRiInterior(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiInterior(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiExterior *newRiExterior(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiExterior(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiDisplacement *newRiDisplacement(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiDisplacement(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiTextureCoordinates *newRiTextureCoordinates(long aLineNo, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
		return new TRiTextureCoordinates(aLineNo, s1, t1, s2, t2, s3, t3, s4, t4);
	}

	inline virtual TRiShadingRate *newRiShadingRate(long aLineNo, RtFloat size) {
		return new TRiShadingRate(aLineNo, size);
	}

	inline virtual TRiShadingInterpolation *newRiShadingInterpolation(long aLineNo, RtToken type) {
		return new TRiShadingInterpolation(aLineNo, type);
	}

	inline virtual TRiMatte *newRiMatte(long aLineNo, RtBoolean onoff) {
		return new TRiMatte(aLineNo, onoff);
	}

	inline virtual TRiBound *newRiBound(long aLineNo, RtBound bound) {
		return new TRiBound(aLineNo, bound);
	}

	inline virtual TRiDetail *newRiDetail(long aLineNo, RtBound bound) {
		return new TRiDetail(aLineNo, bound);
	}

	inline virtual TRiDetailRange *newRiDetailRange(long aLineNo, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
		return new TRiDetailRange(aLineNo, minvis, lowtran, uptran, maxvis);
	}

	inline virtual TRiGeometricApproximation *newRiGeometricApproximation(long aLineNo, RtToken type, RtFloat value) {
		return new TRiGeometricApproximation(aLineNo, type, value);
	}

	inline virtual TRiGeometricRepresentation *newRiGeometricRepresentation(long aLineNo, RtToken type) {
		return new TRiGeometricRepresentation(aLineNo, type);
	}

	inline virtual TRiOrientation *newRiOrientation(long aLineNo, RtToken orientation) {
		return new TRiOrientation(aLineNo, orientation);
	}

	inline virtual TRiReverseOrientation *newRiReverseOrientation(long aLineNo) {
		return new TRiReverseOrientation(aLineNo);
	}

	inline virtual TRiSides *newRiSides(long aLineNo, RtInt nsides) {
		return new TRiSides(aLineNo, nsides);
	}

	inline virtual TRiBasis *newRiBasis(long aLineNo, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
		return new TRiBasis(aLineNo, ubasis, ustep, vbasis, vstep);
	}

	inline virtual TRiTrimCurve *newRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
		return new TRiTrimCurve(aLineNo, nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	inline virtual TRiTrimCurve *newRiTrimCurve(long aLineNo, const TTrimCurveData &trimCurve) {
		return new TRiTrimCurve(aLineNo, trimCurve);
	}

	inline virtual TRiIdentity *newRiIdentity(long aLineNo) {
		return new TRiIdentity(aLineNo);
	}

	inline virtual TRiTransform *newRiTransform(long aLineNo, RtMatrix transform) {
		return new TRiTransform(aLineNo, transform);
	}

	inline virtual TRiConcatTransform *newRiConcatTransform(long aLineNo, RtMatrix transform) {
		return new TRiConcatTransform(aLineNo, transform);
	}

	inline virtual TRiPerspective *newRiPerspective(long aLineNo, RtFloat fov) {
		return new TRiPerspective(aLineNo, fov);
	}

	inline virtual TRiTranslate *newRiTranslate(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new TRiTranslate(aLineNo, dx, dy, dz);
	}

	inline virtual TRiRotate *newRiRotate(long aLineNo, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new TRiRotate(aLineNo, angle, dx, dy, dz);
	}

	inline virtual TRiScale *newRiScale(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new TRiScale(aLineNo, dx, dy, dz);
	}

	inline virtual TRiSkew *newRiSkew(long aLineNo, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
		return new TRiSkew(aLineNo, angle, dx1, dy1, dz1, dx2, dy2, dz2);
	}

	inline virtual TRiDeformation *newRiDeformation(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiDeformation(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiCoordinateSystem *newRiCoordinateSystem(long aLineNo, const char *name) {
		return new TRiCoordinateSystem(aLineNo, name);
	}

	inline virtual TRiCoordSysTransform *newRiCoordSysTransform(long aLineNo, const char *name) {
		return new TRiCoordSysTransform(aLineNo, name);
	}

	inline virtual TRiTransformPoints *newRiTransformPoints(long aLineNo, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points) {
		return new TRiTransformPoints(aLineNo, fromspace, tospace, npoints, points);
	}

	inline virtual TRiPolygon *newRiPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPolygon(aLineNo, declmap, colorComps, nvertices, n, tokens, params);
	}

	inline virtual TRiGeneralPolygon *newRiGeneralPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiGeneralPolygon(aLineNo, declmap, colorComps, nloops, nverts, n, tokens, params);
	}

	inline virtual TRiPointsPolygons *newRiPointsPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPointsPolygons(aLineNo, declmap, colorComps, npolys, nverts, verts, n, tokens, params);
	}

	inline virtual TRiPointsGeneralPolygons *newRiPointsGeneralPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPointsGeneralPolygons(aLineNo, declmap, colorComps, npolys, nloops, nverts, verts, n, tokens, params);
	}

	inline virtual TRiPatch *newRiPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPatch(aLineNo, declmap, colorComps, type, n, tokens, params);
	}

	inline virtual TRiPatchMesh *newRiPatchMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPatchMesh(aLineNo, declmap, colorComps, ustep, vstep, type, nu, uwrap, nv, vwrap, n, tokens, params);
	}

	inline virtual TRiNuPatch *newRiNuPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiNuPatch(aLineNo, declmap, colorComps, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	}

	inline virtual TRiSubdivisionMesh *newRiSubdivisionMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiSubdivisionMesh(aLineNo, declmap, colorComps, scheme, nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs, n, tokens, params);
	}

	inline virtual TRiSphere *newRiSphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiSphere(aLineNo, declmap, colorComps, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiCone *newRiCone(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiCone(aLineNo, declmap, colorComps, height, radius, thetamax, n, tokens, params);
	}

	inline virtual TRiCylinder *newRiCylinder(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiCylinder(aLineNo, declmap, colorComps, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiHyperboloid *newRiHyperboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiHyperboloid(aLineNo, declmap, colorComps, point1, point2, thetamax, n, tokens, params);
	}

	inline virtual TRiParaboloid *newRiParaboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiParaboloid(aLineNo, declmap, colorComps, rmax, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual TRiDisk *newRiDisk(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiDisk(aLineNo, declmap, colorComps, height, radius, thetamax, n, tokens, params);
	}

	inline virtual TRiTorus *newRiTorus(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiTorus(aLineNo, declmap, colorComps, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	}

	inline virtual TRiPoints *newRiPoints(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiPoints(aLineNo, declmap, colorComps, npts, n, tokens, params);
	}

	inline virtual TRiCurves *newRiCurves(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiCurves(aLineNo, declmap, colorComps, ustep, vstep, type, ncurves, nverts, wrap, n, tokens, params);
	}

	inline virtual TRiBlobby *newRiBlobby(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiBlobby(aLineNo, declmap, colorComps, nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	}

	inline virtual TRiProcedural *newRiProcedural(long aLineNo, RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc, TRi &ri) {
		return new TRiProcedural(aLineNo, data, bound, subdivfunc, freefunc, ri);
	}

	inline virtual TRiGeometry *newRiGeometry(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiGeometry(aLineNo, declmap, colorComps, name, n, tokens, params);
	}

	inline virtual TRiObjectInstance *newRiObjectInstance(long aLineNo, RtObjectHandle handle) {
		return new TRiObjectInstance(aLineNo, handle);
	}

	inline virtual TRiMakeTexture *newRiMakeTexture(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiMakeTexture(aLineNo, declmap, colorComps, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual TRiMakeBump *newRiMakeBump(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiMakeBump(aLineNo, declmap, colorComps, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual TRiMakeLatLongEnvironment *newRiMakeLatLongEnvironment(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiMakeLatLongEnvironment(aLineNo, declmap, colorComps, pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual TRiMakeCubeFaceEnvironment *newRiMakeCubeFaceEnvironment(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiMakeCubeFaceEnvironment(aLineNo, declmap, colorComps, px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual TRiMakeShadow *newRiMakeShadow(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiMakeShadow(aLineNo, declmap, colorComps, pic, tex, n, tokens, params);
	}

	inline virtual TRiArchiveRecord *newRiArchiveRecord(long aLineNo, RtToken type, const char *line)
	{
		return new TRiArchiveRecord(aLineNo, type, line);
	}

	inline virtual TRiReadArchive *newRiReadArchive(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtString filename, RtArchiveCallback callback,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new TRiReadArchive(aLineNo, declmap, colorComps, filename, callback, n, tokens, params);
	}

}; // TNewInterface

///////////////////////////////////////////////////////////////////////////////
//! Interface for the method that draws a TGeometricPrimFace
class TDrawPrim {
public:
	virtual void drawGeometricPrim(TGeometricPrimFace &prim) = 0;
}; // TDrawPrim

#endif // _RICPP_RENDERCONTEXT_RIMACRO_H
