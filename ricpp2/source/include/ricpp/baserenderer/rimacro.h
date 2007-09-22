#ifndef _RICPP_BASERENDERER_RIMACRO_H
#define _RICPP_BASERENDERER_RIMACRO_H


/** @file rimacro.h
 *  @brief Declaration of the macro classes, records interface calls.
 *
 *  There is a class for each interface call, the classes can be used to store interface
 *  calls in memory and 'replay' them later. CRiMacro is the class to store a list of macros.
 *  The renderer base class TIntermediateRenderer manages the construction of macros while
 *  loading a RIB file.
 *
 *  @author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_BASERENDERER_PARAMETER_H
#include "ricpp/baserenderer/parameter.h"
#endif // _RICPP_BASERENDERER_PARAMETER_H

#ifndef _RICPP_BASERENDERER_PARAMCLASSES_H
#include "ricpp/baserenderer/paramclasses.h"
#endif // _RICPP_BASERENDERER_PARAMCLASSES_H

#ifndef _RICPP_BASERENDERER_NODE_H
#include "node.h"
#endif // _RICPP_BASERENDERER_NODE_H

#ifndef _RICPP_RENDERSTATE_RENDERSTATE_H
#include "ricpp/renderstate/renderstate.h"
#endif // _RICPP_RENDERSTATE_RENDERSTATE_H

#include <list>

namespace RiCPP {

///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls
class CRManInterfaceCall : public CNode {
protected:
	long m_lineNo; //!< Place to store the line number of a call in a RIB file, -1 if there is no line number

public:
	inline static const char *myClassName(void) { return "CRManInterfaceCall"; }
	inline virtual const char *className() const { return CRManInterfaceCall::myClassName(); }

	//! Standard constructor, the m_lineNo is initialized to -1 (a line number is not known)
	inline CRManInterfaceCall() : m_lineNo(-1) { }

	//! Constructor, to set a line number
	/*! \param aLineNo The line number to store
	 */
	inline CRManInterfaceCall(long aLineNo): m_lineNo(aLineNo) {}

	//! Destructor
	inline virtual ~CRManInterfaceCall() {}

	//! Number for the interface call, an integer tag used internally by RiCPP to distinguish CRManInterfaceCall instances, \sa riconstants.h
	/*! \return The number to identify the interface call (constant REQ_.., e.g. REQ_BEGIN)
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_UNKNOWN;}

	//! Replay the interface call
	/*! \param ri The renderer used for replay
	 */
	inline virtual void replay(IRi &ri) { }

	inline virtual void replay(IRi &ri, CRenderState &state)
	{
		replay(ri);
	}

	//! Replay the interface call
	/*! \param ri The renderer used for replay
	 *! \param cb Archive callback
	 */
	inline virtual void replay(IRi &ri, CRenderState &state, const IArchiveCallback *cb)
	{
		replay(ri, state);
	}

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
class CVarParamRManInterfaceCall : public CRManInterfaceCall {
protected:
	CParameterList m_parameters;             //!< Parameters of an interface call.

public:
	inline static const char *myClassName(void) { return "CVarParamRManInterfaceCall"; }
	inline virtual const char *className() const { return CVarParamRManInterfaceCall::myClassName(); }

	//! Standard constructor (an empty parameter list)
	inline CVarParamRManInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline CVarParamRManInterfaceCall(long aLineNo) :
		CRManInterfaceCall(aLineNo)
	{
	}

	//! Destructor
	inline virtual ~CVarParamRManInterfaceCall() {}
	
	inline RtToken *getTokens()
	{
		return m_parameters.tokenPtr();
	}
	inline RtPointer *getParams()
	{
		return m_parameters.valuePtr();
	}

	inline virtual void set(
		const CValueCounts &counts,
		CDeclarationDictionary &decl,
		unsigned int curColorSize,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		m_parameters.set(counts, decl, curColorSize, n, tokens, params);
	}

	inline virtual const CParameter *get(const char *name) const
	{
		return m_parameters.get(name);
	}

	//! Fill the parameter list
	inline virtual void setParams(
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		unsigned int curColorSize,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		set(CValueCounts(p.vertex(), p.varying(), p.uniform(), p.faceVertex(), p.faceVarying()),
		    decl, curColorSize, n, tokens, params);
	}
	
	inline virtual CParameterList::size_type size() const
	{
		return m_parameters.size();
	}

	inline virtual CParameterList::const_iterator begin() const
	{
		return m_parameters.begin();
	}

	inline virtual CParameterList::const_iterator end() const
	{
		return m_parameters.end();
	}
}; // TVarParamInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls for geometry
class CGeometryRManInterfaceCall : public CVarParamRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CGeometryRManInterfaceCall"; }
	inline virtual const char *className() const { return CGeometryRManInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline CGeometryRManInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline CGeometryRManInterfaceCall(long aLineNo) : CVarParamRManInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~CGeometryRManInterfaceCall() {}
}; // CGeometryRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls based on UV Meshes (Splines, Quadrics)
class CUVRManInterfaceCall : public CGeometryRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CUVRManInterfaceCall"; }
	inline virtual const char *className() const { return CUVRManInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline CUVRManInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline CUVRManInterfaceCall(long aLineNo) : CGeometryRManInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~CUVRManInterfaceCall() {}
}; // CUVRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Base class of all interface calls based on Polygon meshes
class CPolygonRManInterfaceCall : public CGeometryRManInterfaceCall {
protected:
public:
	inline static const char *myClassName(void) { return "CPolygonRManInterfaceCall"; }
	inline virtual const char *className() const { return CPolygonRManInterfaceCall::myClassName(); }

	//! Standard constructor (empty parameter list)
	inline CPolygonRManInterfaceCall() {}

	//! Constructor. empty parameter list, with line number
	/*! \param aLineNo The line number to store
	 */
	inline CPolygonRManInterfaceCall(long aLineNo) : CGeometryRManInterfaceCall(aLineNo) {}

	//! Destructor, frees the resources
	inline virtual ~CPolygonRManInterfaceCall() {}
}; // CPolygonRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
//! Macro container
class CRiMacro {
protected:
	std::list<CRManInterfaceCall *> m_callList;  //!< List of all interface calls for this macro

	bool m_isObject;                          //!< true, if macro is an RenderMan object (IRi::objectInstance())
	bool m_isDefining;                        //!< true, if macro is not closed (object end or file end not reached)
	bool m_valid;                             //!< true if the macro could be defined without errors

	std::string m_name;                       //!< Name of the macro (eg. file name)

public:
	int m_frames; //!< Number of frames defined inside this macros (not used)

	//! Constructor initializes the macro.
	/*! \param aName Name of the macro, eg. file name
	 *  \param isObject Macro defines an geomeCRic object
	 */
	inline CRiMacro(const char *aName="", bool isObject=false)
		: m_isObject(isObject), m_isDefining(true), m_valid(true), m_name(aName?aName:"")
	{
	}

	//! Destructor to free resources.
	virtual ~CRiMacro();

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
	 *  \param callback Used for archive callbacks (esp. for IRi::readArchive())
	 */
	void replay(IRi &ri, CRenderState &state, const IArchiveCallback *callback);

	//! Archive replay with callback for RenderMan objects
	/*! \param ri The renderer used to replay
	 *  \param ricb Callback to set line number, doing error handling etc.
	 */
	void replay(IRi &ri, CRenderState &state); // Object replay

	// Replay single frames not implemented, yet
	// void replay(IRi &ri, RtInt lastFrameNo, RtInt frameNo);
	
	//! Adds an interface call to the macro
	/*! \param c Interface call instance
	 *  \return true if ok
	 */
	bool add(CRManInterfaceCall *c);
	
	//! Checks whether macro is a recorded RI object or not (for RI objects \sa IRi::objectBegin(), IRi::objectEnd(), IRi::objectInstance())
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
	inline bool valid() const { return m_valid; }

	//! Sets the validity of the macro - e.g. if an error while parsing occured.
	//! \param isvalid The new validity.
	//! \return The validity set befor.
	inline bool valid(bool isValid) {
		bool old = m_valid;
		m_valid = isValid;
		return old;
	}

}; // CRiMacro


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//! Stores the content of a IRi::declare() call
/*! A declaration of an interface variable is stored in an instance of this class.
 */
class CRiDeclare : public CRManInterfaceCall {
protected:
	std::string m_name;        //!< Name of the declarated variable
	std::string m_declaration; //!< The declaration, like: varying float[3]
public:
	inline static const char *myClassName(void) { return "CRiDeclare"; }
	inline virtual const char *className() const { return CRiDeclare::myClassName(); }

	//! Default constructor, records the parameters in member variables
	/*! \param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file
	 *  \param name The name of the declared variable
	 *  \param declaration The declaration sCRing
	 */
	inline CRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
		: CRManInterfaceCall(aLineNo),
		  m_name(name?name:""),
		  m_declaration(declaration?declaration:"")
	{}
	
	//! To get the interface number, an integer tag used internally by RiCPP to distinguish CRManInterfaceCall instances, \sa riconstants.h
	/*! \return The constant REQ_DECLARE that indicates a declare statement.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_DECLARE; }

	//! Replays the declare statement for a given renderer
	/*! \param ri Instance of a renderer, used to replay the declare statement
	 */
	inline virtual void replay(IRi &ri) { ri.declare(m_name.c_str(), m_declaration.c_str()); }

	// inline CRiDeclare &operator=(const CRiDeclare &) {
	// 	return *this;
	// }

}; // CRiDeclare

///////////////////////////////////////////////////////////////////////////////
//! Stores the content of a IRi::errorHandler() call
/*! Stores the setting of an error handler.
 *
 *  errorhandler function
 */
class CRiErrorHandler : public CRManInterfaceCall {
protected:
	IErrorHandler *m_handler; //!< Pointer to the error handler function

public:
	inline static const char *myClassName(void) { return "CRiErrorHandler"; }
	inline virtual const char *className() const { return CRiErrorHandler::myClassName(); }

	//! Default constructor, records the parameters in member variables
	/*! \param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file
	 *  \param handler Pointer to the error handler function that should be used
	 */
	inline CRiErrorHandler(
		long aLineNo,
		const IErrorHandler &handler)
		: CRManInterfaceCall(aLineNo)
	{
		m_handler = handler.duplicate();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiErrorHandler", __LINE__, __FILE__);
	}

	inline ~CRiErrorHandler()
	{
		if ( m_handler )
			delete m_handler;
	}
	//! To get the interface number, an integer tag used internally by RiCPP to distinguish CRManInterfaceCall instances, \sa riconstants.h
	/*! \return The constant REQ_ERROR_HANDLER that indicates an errorHandler statement.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_ERROR_HANDLER; }

	//! Replays the errorhandler statement for a given renderer
	/*! \param ri Instance of a renderer, used to replay the errorhandler statement
	 */
	inline virtual void replay(IRi &ri) { ri.errorHandler(*m_handler); }

	// inline CRiErrorHandler &operator=(const CRiErrorHandler &) {
	// 	return *this;
	// }
}; // CRiErrorHandler

// Not suported: Frontend functions
// REQ_BEGIN
// REQ_END
// REQ_CONTEXT
// REQ_GET_CONTEXT


///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiFrameBegin : public CRManInterfaceCall {
protected:
	RtInt m_framenumber; //!< A number to identify the frame.
public:
	inline static const char *myClassName(void) { return "CRiFrameBegin"; }
	inline virtual const char *className() const { return CRiFrameBegin::myClassName(); }

	inline CRiFrameBegin(long aLineNo, RtInt number)
		: CRManInterfaceCall(aLineNo), m_framenumber(number)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_BEGIN; }
	inline virtual void replay(IRi &ri) { ri.frameBegin(m_framenumber); }
	// inline CRiFrameBegin &operator=(const CRiFrameBegin &) {
	//	return *this;
	// }
}; // CRiFrameBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiFrameEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiFrameEnd"; }
	inline virtual const char *className() const { return CRiFrameEnd::myClassName(); }

	inline CRiFrameEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_END; }
	inline virtual void replay(IRi &ri) { ri.frameEnd(); }
	// inline CRiFrameEnd &operator=(const CRiFrameEnd &) {
	// 	return *this;
	// }
}; // CRiFrameEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiWorldBegin : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiWorldBegin"; }
	inline virtual const char *className() const { return CRiWorldBegin::myClassName(); }

	inline CRiWorldBegin(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_BEGIN; }
	inline virtual void replay(IRi &ri) { ri.worldBegin(); }
	// inline CRiWorldBegin &operator=(const CRiWorldBegin &) {
	// 	return *this;
	// }
}; // CRiWorldBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiWorldEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiWorldEnd"; }
	inline virtual const char *className() const { return CRiWorldEnd::myClassName(); }

	inline CRiWorldEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_END; }
	inline virtual void replay(IRi &ri) { ri.worldEnd(); }
	// inline CRiWorldEnd &operator=(const CRiWorldEnd &) {
	// 	return *this;
	// }
}; // CRiWorldEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiAttributeBegin : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiAttributeBegin"; }
	inline virtual const char *className() const { return CRiAttributeBegin::myClassName(); }

	inline CRiAttributeBegin(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_BEGIN; }
	inline virtual void replay(IRi &ri) { ri.attributeBegin(); }
	// inline CRiAttributeBegin &operator=(const CRiAttributeBegin &) {
	// 	return *this;
	// }
}; // CRiAttributeBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiAttributeEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiAttributeEnd"; }
	inline virtual const char *className() const { return CRiAttributeEnd::myClassName(); }

	inline CRiAttributeEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_END; }
	inline virtual void replay(IRi &ri) { ri.attributeEnd(); }
	// inline CRiAttributeEnd &operator=(const CRiAttributeEnd &) {
	// 	return *this;
	// }
}; // CRiAttributeEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiTransformBegin : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiTransformBegin"; }
	inline virtual const char *className() const { return CRiTransformBegin::myClassName(); }

	inline CRiTransformBegin(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_BEGIN; }
	inline virtual void replay(IRi &ri) { ri.transformBegin(); }
	// inline CRiTransformBegin &operator=(const CRiTransformBegin &) {
	// 	return *this;
	// }
}; // CRiTransformBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiTransformEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiTransformEnd"; }
	inline virtual const char *className() const { return CRiTransformEnd::myClassName(); }

	inline CRiTransformEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_END; }
	inline virtual void replay(IRi &ri) { ri.transformEnd(); }
	// inline CRiTransformEnd &operator=(const CRiTransformEnd &) {
	// 	return *this;
	// }
}; // CRiTransformEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiSolidBegin : public CRManInterfaceCall {
protected:
	std::string m_operation; //!< String that indicates the solid operation (like "union")
public:
	inline static const char *myClassName(void) { return "CRiSolidBegin"; }
	inline virtual const char *className() const { return CRiSolidBegin::myClassName(); }

	inline CRiSolidBegin(long aLineNo, RtToken operation) : CRManInterfaceCall(aLineNo), m_operation(operation) { }
	inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_BEGIN; }
	inline virtual void replay(IRi &ri) { ri.solidBegin(m_operation.c_str()); }
	// inline CRiSolidBegin &operator=(const CRiSolidBegin &) {
	// 	return *this;
	// }
}; // CRiSolidBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiSolidEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiSolidEnd"; }
	inline virtual const char *className() const { return CRiSolidEnd::myClassName(); }

	inline CRiSolidEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_END; }
	inline virtual void replay(IRi &ri) { ri.solidEnd(); }
	// inline CRiSolidEnd &operator=(const CRiSolidEnd &) {
	// 	return *this;
	// }
}; // CRiSolidEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiObjectBegin : public CRManInterfaceCall {
protected:
	RtObjectHandle *m_handlePtr; //!< (out) Used to identify the internal object handle with the object number of a rib file.
public:
	inline static const char *myClassName(void) { return "CRiObjectBegin"; }
	inline virtual const char *className() const { return CRiObjectBegin::myClassName(); }

	inline CRiObjectBegin(long aLineNo, RtObjectHandle *h) : CRManInterfaceCall(aLineNo)
	{
		assert(h != 0);
		m_handlePtr = h;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_BEGIN; }
	inline virtual void replay(IRi &ri) {
		assert(m_handlePtr != 0);
		if ( m_handlePtr != 0 ) {
			*m_handlePtr = ri.objectBegin();
		} else {
			ri.objectBegin();
		}
	}
	inline virtual RtObjectHandle *handlePtr() { return m_handlePtr; }
	// inline CRiObjectBegin &operator=(const CRiObjectBegin &) {
	//	return *this;
	// }
}; // CRiObjectBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiObjectEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiObjectEnd"; }
	inline virtual const char *className() const { return CRiObjectEnd::myClassName(); }

	inline CRiObjectEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_END; }
	inline virtual void replay(IRi &ri) { ri.objectEnd(); }
	// inline CRiObjectEnd &operator=(const CRiObjectEnd &) {
	// 	return *this;
	// }
}; // CRiObjectEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiMotionBegin : public CRManInterfaceCall {
protected:
	std::vector<RtFloat> m_motionVars; //!< The motion "time stamps"
public:
	inline static const char *myClassName(void) { return "CRiMotionBegin"; }
	inline virtual const char *className() const { return CRiMotionBegin::myClassName(); }

	inline CRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) : CRManInterfaceCall(aLineNo), m_motionVars(f, f+n) {
	/*
	m_motionVars.resize(n);
	m_motionVars.assign(f, f+n);
		*/
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_BEGIN; }
	inline virtual void replay(IRi &ri) {
		ri.motionBeginV(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
	}
	// inline CRiMotionBegin &operator=(const CRiMotionBegin &) {
	// 	return *this;
	// }
}; // CRiMotionBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiMotionEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiMotionEnd"; }
	inline virtual const char *className() const { return CRiMotionEnd::myClassName(); }

	inline CRiMotionEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_END; }
	inline virtual void replay(IRi &ri) { ri.motionEnd(); }
	// inline CRiMotionEnd &operator=(const CRiMotionEnd &) {
	// 	return *this;
	// }
}; // CRiMotionEnd

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiFormat : public CRManInterfaceCall {
protected:
	RtInt m_xres,     //!< width
	      m_yres;     //!< height
	RtFloat m_aspect; //!< pixel aspect ratio
public:
	inline static const char *myClassName(void) { return "CRiFormat"; }
	inline virtual const char *className() const { return CRiFormat::myClassName(); }

	inline CRiFormat(long aLineNo, RtInt xres, RtInt yres, RtFloat aspect) : CRManInterfaceCall(aLineNo), m_xres(xres), m_yres(yres), m_aspect(aspect) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_FORMAT; }
	inline virtual void replay(IRi &ri) { ri.format(m_xres, m_yres, m_aspect); }
	// inline CRiFormat &operator=(const CRiFormat &) {
	//	return *this;
	// }
}; // CRiFormat

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiFrameAspectRatio : public CRManInterfaceCall {
protected:
	RtFloat m_aspect; //!< frame aspect ratio
public:
	inline static const char *myClassName(void) { return "CRiFrameAspectRatio"; }
	inline virtual const char *className() const { return CRiFrameAspectRatio::myClassName(); }

	inline CRiFrameAspectRatio(long aLineNo, RtFloat aspect) : CRManInterfaceCall(aLineNo), m_aspect(aspect) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_ASPECT_RATIO; }
	inline virtual void replay(IRi &ri) { ri.frameAspectRatio(m_aspect); }
	// inline CRiFrameAspectRatio &operator=(const CRiFrameAspectRatio &) {
	// 	return *this;
	// }
}; // CRiFrameAspectRatio

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiScreenWindow : public CRManInterfaceCall {
protected:
	RtFloat m_left,  //!< coordinate of the left screen window edge
	        m_right, //!< coordinate of the right screen window edge
	        m_bot,   //!< coordinate of the bottom screen window edge
	        m_top;   //!< coordinate of the top screen window edge
public:
	inline static const char *myClassName(void) { return "CRiScreenWindow"; }
	inline virtual const char *className() const { return CRiScreenWindow::myClassName(); }

	inline CRiScreenWindow(long aLineNo, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) : CRManInterfaceCall(aLineNo), m_left(left), m_right(right), m_bot(bot), m_top(top) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SCREEN_WINDOW; }
	inline virtual void replay(IRi &ri) { ri.screenWindow(m_left, m_right, m_bot, m_top); }
	// inline CRiScreenWindow &operator=(const CRiScreenWindow &) {
	// 	return *this;
	// }
}; // CRiScreenWindow

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiCropWindow : public CRManInterfaceCall {
protected:
	RtFloat m_xmin,  //!< Crop window minimal x
	        m_xmax,  //!< Crop window maximal x
	        m_ymin,  //!< Crop window minimal y
	        m_ymax;  //!< Crop window maximal y
public:
	inline static const char *myClassName(void) { return "CRiCropWindow"; }
	inline virtual const char *className() const { return CRiCropWindow::myClassName(); }

	inline CRiCropWindow(long aLineNo, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) : CRManInterfaceCall(aLineNo), m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CROP_WINDOW; }
	inline virtual void replay(IRi &ri) { ri.cropWindow(m_xmin, m_xmax, m_ymin, m_ymax); }
	// inline CRiCropWindow &operator=(const CRiCropWindow &) {
	//	return *this;
	// }
}; // CRiCropWindow

///////////////////////////////////////////////////////////////////////////////
//! \sa CVarParamRManInterfaceCall
class CRiProjection : public CVarParamRManInterfaceCall {
protected:
	std::string m_name; //!< projection name ("perspective", "orthographic")
public:
	inline static const char *myClassName(void) { return "CRiProjection"; }
	inline virtual const char *className() const { return CRiProjection::myClassName(); }

	inline CRiProjection(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PROJECTION; }
	inline virtual void replay(IRi &ri)
	{
		ri.projectionV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams());
	}
	// inline CRiProjection &operator=(const CRiProjection &) {
	// 	return *this;
	// }
}; // CRiProjection

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiClipping : public CRManInterfaceCall {
protected:
	RtFloat m_hither, //!< Front clipping plane
	        m_yon;    //!< Far clipping plane
public:
	inline static const char *myClassName(void) { return "CRiClipping"; }
	inline virtual const char *className() const { return CRiClipping::myClassName(); }

	inline CRiClipping(long aLineNo, RtFloat hither, RtFloat yon)
		: CRManInterfaceCall(aLineNo), m_hither(hither), m_yon(yon) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING; }
	inline virtual void replay(IRi &ri) { ri.clipping(m_hither, m_yon); }
	// inline CRiClipping &operator=(const CRiClipping &) {
	//	return *this;
	// }
}; // CRiClipping

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiClippingPlane : public CRManInterfaceCall {
protected:
	RtFloat m_x,  //!< Point of an additional clipping plane (x coordinate)
	        m_y,  //!< Point of an additional clipping plane (y coordinate)
	        m_z,  //!< Point of an additional clipping plane (z coordinate)
	        m_nx, //!< Normal of an additional clipping plane (x coordinate)
	        m_ny, //!< Normal of an additional clipping plane (y coordinate)
	        m_nz; //!< Normal of an additional clipping plane (z coordinate)
public:
	inline static const char *myClassName(void) { return "CRiClippingPlane"; }
	inline virtual const char *className() const { return CRiClippingPlane::myClassName(); }

	inline CRiClippingPlane(long aLineNo, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) : CRManInterfaceCall(aLineNo), m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING_PLANE; }
	inline virtual void replay(IRi &ri) { ri.clippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz); }
	// inline CRiClippingPlane &operator=(const CRiClippingPlane &) {
	// 	return *this;
	// }
}; // CRiClippingPlane

///////////////////////////////////////////////////////////////////////////////
class CRiDepthOfField : public CRManInterfaceCall {
protected:
	RtFloat m_fstop, m_focallength, m_focaldistance;
public:
	inline static const char *myClassName(void) { return "CRiDepthOfField"; }
	inline virtual const char *className() const { return CRiDepthOfField::myClassName(); }

	inline CRiDepthOfField(long aLineNo, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) : CRManInterfaceCall(aLineNo), m_fstop(fstop), m_focallength(focallength), m_focaldistance(focaldistance) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DEPTH_OF_FIELD; }
	inline virtual void replay(IRi &ri) { ri.depthOfField(m_fstop, m_focallength, m_focaldistance); }
	inline CRiDepthOfField &operator=(const CRiDepthOfField &) {
		return *this;
	}
}; // CRiDepthOfField

///////////////////////////////////////////////////////////////////////////////
class CRiShutter : public CRManInterfaceCall {
protected:
	RtFloat m_smin, m_smax;
public:
	inline static const char *myClassName(void) { return "CRiShutter"; }
	inline virtual const char *className() const { return CRiShutter::myClassName(); }

	inline CRiShutter(long aLineNo, RtFloat smin, RtFloat smax) : CRManInterfaceCall(aLineNo), m_smin(smin), m_smax(smax) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SHUTTER; }
	inline virtual void replay(IRi &ri) { ri.shutter(m_smin, m_smax); }
	inline CRiShutter &operator=(const CRiShutter &) {
		return *this;
	}
}; // CRiShutter

///////////////////////////////////////////////////////////////////////////////
class CRiPixelVariance : public CRManInterfaceCall {
	RtFloat m_variation;
public:
	inline static const char *myClassName(void) { return "CRiPixelVariance"; }
	inline virtual const char *className() const { return CRiPixelVariance::myClassName(); }

	inline CRiPixelVariance(long aLineNo, RtFloat variation) : CRManInterfaceCall(aLineNo), m_variation(variation) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_VARIANCE; }
	inline virtual void replay(IRi &ri) { ri.pixelVariance(m_variation); }
	inline CRiPixelVariance &operator=(const CRiPixelVariance &) {
		return *this;
	}
}; // CRiPixelVariance

///////////////////////////////////////////////////////////////////////////////
class CRiPixelSamples : public CRManInterfaceCall {
protected:
	RtFloat m_xsamples, m_ysamples;
public:
	inline static const char *myClassName(void) { return "CRiPixelSamples"; }
	inline virtual const char *className() const { return CRiPixelSamples::myClassName(); }

	inline CRiPixelSamples(long aLineNo, RtFloat xsamples, RtFloat ysamples) : CRManInterfaceCall(aLineNo), m_xsamples(xsamples), m_ysamples(ysamples) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_SAMPLES; }
	inline virtual void replay(IRi &ri) { ri.pixelSamples(m_xsamples, m_ysamples); }
	inline CRiPixelSamples &operator=(const CRiPixelSamples &) {
		return *this;
	}
}; // CRiPixelSamples

///////////////////////////////////////////////////////////////////////////////
class CRiPixelFilter : public CRManInterfaceCall {
protected:
	IFilterFunc *m_function;
	RtFloat m_xwidth, m_ywidth;
public:
	inline static const char *myClassName(void) { return "CRiPixelFilter"; }
	inline virtual const char *className() const { return CRiPixelFilter::myClassName(); }

	inline CRiPixelFilter(long aLineNo, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
		: CRManInterfaceCall(aLineNo), m_xwidth(xwidth), m_ywidth(ywidth)
	{
		m_function = function.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiPixelFilter, m_function", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiPixelFilter()
	{
		if ( m_function )
			delete m_function;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_FILTER; }
	inline virtual void replay(IRi &ri) {
		assert(m_function != 0);
		if ( m_function )
			ri.pixelFilter(*m_function, m_xwidth, m_ywidth);
	}
	inline CRiPixelFilter &operator=(const CRiPixelFilter &) {
		return *this;
	}
}; // CRiPixelFilter

///////////////////////////////////////////////////////////////////////////////
class CRiExposure : public CRManInterfaceCall {
protected:
	RtFloat m_gain, m_gamma;
public:
	inline static const char *myClassName(void) { return "CRiExposure"; }
	inline virtual const char *className() const { return CRiExposure::myClassName(); }

	inline CRiExposure(long aLineNo, RtFloat gain, RtFloat gamma) : CRManInterfaceCall(aLineNo), m_gain(gain), m_gamma(gamma) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_EXPOSURE; }
	inline virtual void replay(IRi &ri) { ri.exposure(m_gain, m_gamma); }
	inline CRiExposure &operator=(const CRiExposure &) {
		return *this;
	}
}; // CRiExposure

///////////////////////////////////////////////////////////////////////////////
class CRiImager : public CVarParamRManInterfaceCall {
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiImager"; }
	inline virtual const char *className() const { return CRiImager::myClassName(); }

	inline CRiImager(long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IMAGER; }
	inline virtual void replay(IRi &ri) { ri.imagerV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiImager &operator=(const CRiImager &) {
		return *this;
	}
}; // CRiImager

///////////////////////////////////////////////////////////////////////////////
class CRiQuantize : public CRManInterfaceCall {
protected:
	std::string m_type;
	RtInt m_one, m_qmin, m_qmax;
	RtFloat m_ampl;
public:
	inline static const char *myClassName(void) { return "CRiQuantize"; }
	inline virtual const char *className() const { return CRiQuantize::myClassName(); }

	inline CRiQuantize(long aLineNo, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) : CRManInterfaceCall(aLineNo), m_type(type), m_one(one), m_qmin(qmin), m_qmax(qmax), m_ampl(ampl) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_QUANTIZE; }
	inline virtual void replay(IRi &ri) { ri.quantize(m_type.c_str(), m_one, m_qmin, m_qmax, m_ampl); }
	inline CRiQuantize &operator=(const CRiQuantize &) {
		return *this;
	}
}; // CRiQuantize

///////////////////////////////////////////////////////////////////////////////
class CRiDisplay : public CVarParamRManInterfaceCall {
protected:
	std::string m_name, m_type, m_mode;
public:
	inline static const char *myClassName(void) { return "CRiDisplay"; }
	inline virtual const char *className() const { return CRiDisplay::myClassName(); }

	inline CRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken name, RtToken type, RtToken mode,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name), m_type(type), m_mode(mode)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY; }
	inline virtual void replay(IRi &ri) { ri.displayV(m_name.c_str(), m_type.c_str(), m_mode.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiDisplay &operator=(const CRiDisplay &) {
		return *this;
	}
}; // CRiDisplay

///////////////////////////////////////////////////////////////////////////////
class CRiHider : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiHider"; }
	inline virtual const char *className() const { return CRiHider::myClassName(); }

	inline CRiHider(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_HIDER; }
	inline virtual void replay(IRi &ri) { ri.hiderV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiHider &operator=(const CRiHider &h) {
		return *this;
	}
}; // CRiHider

///////////////////////////////////////////////////////////////////////////////
class CRiColorSamples : public CRManInterfaceCall {
protected:
	RtInt m_N;
	std::vector<RtFloat> m_nRGB, m_RGBn;
public:
	inline static const char *myClassName(void) { return "CRiColorSamples"; }
	inline virtual const char *className() const { return CRiColorSamples::myClassName(); }

	inline CRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn) : CRManInterfaceCall(aLineNo), m_N(N) {
		m_nRGB.reserve(N*3);
		m_RGBn.reserve(N*3);
		RtInt i;
		for ( i = 0; i < N*3; ++i ) {
			m_nRGB.push_back(nRGB[i]);
			m_RGBn.push_back(RGBn[i]);
		}
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR_SAMPLES; }
	inline virtual void replay(IRi &ri) {
		ri.colorSamples(
			m_N,
			m_nRGB.empty() ? 0 : &m_nRGB[0],
			m_RGBn.empty() ? 0 : &m_RGBn[0]);
	}
	inline CRiColorSamples &operator=(const CRiColorSamples &) {
		return *this;
	}
}; // CRiColorSamples

///////////////////////////////////////////////////////////////////////////////
class CRiRelativeDetail : public CRManInterfaceCall {
protected:
	RtFloat m_relativedetail;
public:
	inline static const char *myClassName(void) { return "CRiRelativeDetail"; }
	inline virtual const char *className() const { return CRiRelativeDetail::myClassName(); }

	inline CRiRelativeDetail(long aLineNo, RtFloat relativedetail) : CRManInterfaceCall(aLineNo), m_relativedetail(relativedetail) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_RELATIVE_DETAIL; }
	inline virtual void replay(IRi &ri) { ri.relativeDetail(m_relativedetail); }
	inline CRiRelativeDetail &operator=(const CRiRelativeDetail &) {
		return *this;
	}
}; // CRiRelativeDetail

///////////////////////////////////////////////////////////////////////////////
class CRiOption : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiOption"; }
	inline virtual const char *className() const { return CRiOption::myClassName(); }

	inline CRiOption(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OPTION; }
	inline virtual void replay(IRi &ri) { ri.optionV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiOption &operator=(const CRiOption &) {
		return *this;
	}
}; // CRiOption

///////////////////////////////////////////////////////////////////////////////
class CRiLightSource : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
	RtLightHandle *m_handle;
public:
	inline static const char *myClassName(void) { return "CRiLightSource"; }
	inline virtual const char *className() const { return CRiLightSource::myClassName(); }

	inline CRiLightSource(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name, RtLightHandle *handle,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name), m_handle(handle)
	{
		assert(handle != 0);
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_LIGHT_SOURCE; }
	inline virtual void replay(IRi &ri)
	{
		assert(m_handle != 0);
		if ( m_handle )
			*m_handle = ri.lightSourceV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams());
		else
			ri.lightSourceV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline virtual RtLightHandle *handleRef() { return m_handle; }
	inline CRiLightSource &operator=(const CRiLightSource &) {
		return *this;
	}
}; // CRiLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiAreaLightSource : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
	RtLightHandle *m_handle;
public:
	inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
	inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

	inline CRiAreaLightSource(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name, RtLightHandle *handle,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name), m_handle(handle)
	{
		assert(handle != 0);
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_AREA_LIGHT_SOURCE; }
	inline virtual void replay(IRi &ri) {
		assert(m_handle != 0);
		if ( m_handle )
			*m_handle = ri.areaLightSourceV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams());
		else
			ri.areaLightSourceV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline virtual RtLightHandle *handleRef() { return m_handle; }
}; // CRiAreaLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiIlluminate : public CRManInterfaceCall {
protected:
	RtLightHandle *m_handle;
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiIlluminate"; }
	inline virtual const char *className() const { return CRiIlluminate::myClassName(); }

	inline CRiIlluminate(long aLineNo, RtLightHandle *handle, RtBoolean onoff) :
		CRManInterfaceCall(aLineNo), m_handle(handle), m_onoff(onoff)
	{
		assert(handle != 0);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ILLUMINATE; }
	inline virtual RtLightHandle *handleRef() { return m_handle; }

	inline virtual void replay(IRi &ri)
	{
		assert(m_handle != 0);
		ri.illuminate(m_handle ? *m_handle : (RtLightHandle)-1, m_onoff);
	}
	inline CRiIlluminate &operator=(const CRiIlluminate &)
	{
		return *this;
	}
}; // CRiIlluminate

///////////////////////////////////////////////////////////////////////////////
class CRiAttribute : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiAttribute"; }
	inline virtual const char *className() const { return CRiAttribute::myClassName(); }

	inline CRiAttribute(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE; }
	inline virtual void replay(IRi &ri) { ri.attributeV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiAttribute &operator=(const CRiAttribute &) {
		return *this;
	}
}; // CRiAttribute

///////////////////////////////////////////////////////////////////////////////
class CRiColor : public CRManInterfaceCall {
protected:
	std::vector<RtFloat> m_color;
public:
	inline static const char *myClassName(void) { return "CRiColor"; }
	inline virtual const char *className() const { return CRiColor::myClassName(); }

	inline CRiColor(long aLineNo, unsigned int curColorSize, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		unsigned int i;
		for ( i=0; i<curColorSize; ++i )
			m_color.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR; }
	inline virtual void replay(IRi &ri) {
		ri.color(m_color.empty() ? 0 : &m_color[0]);
	}
	inline CRiColor &operator=(const CRiColor &) {
		return *this;
	}
}; // CRiColor

///////////////////////////////////////////////////////////////////////////////
class CRiOpacity : public CRManInterfaceCall {
protected:
	std::vector<RtFloat> m_opacity;
public:
	inline static const char *myClassName(void) { return "CRiOpacity"; }
	inline virtual const char *className() const { return CRiOpacity::myClassName(); }

	inline CRiOpacity(long aLineNo, unsigned int curColorSize, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		unsigned int i;
		for ( i=0; i<curColorSize; ++i )
			m_opacity.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OPACITY; }
	inline virtual void replay(IRi &ri) {
		ri.opacity(m_opacity.empty() ? 0 : &m_opacity[0]);
	}
	inline CRiOpacity &operator=(const CRiOpacity &) {
		return *this;
	}
}; // CRiOpacity

///////////////////////////////////////////////////////////////////////////////
class CRiSurface : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiSurface"; }
	inline virtual const char *className() const { return CRiSurface::myClassName(); }

	inline CRiSurface(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SURFACE; }
	inline virtual void replay(IRi &ri) { ri.surfaceV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiSurface &operator=(const CRiSurface &) {
		return *this;
	}
}; // CRiSurface

///////////////////////////////////////////////////////////////////////////////
class CRiAtmosphere : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiAtmosphere"; }
	inline virtual const char *className() const { return CRiAtmosphere::myClassName(); }

	inline CRiAtmosphere(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATMOSPHERE; }
	inline virtual void replay(IRi &ri) { ri.atmosphereV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiAtmosphere &operator=(const CRiAtmosphere &) {
		return *this;
	}
}; // CRiAtmosphere

///////////////////////////////////////////////////////////////////////////////
class CRiInterior : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiInterior"; }
	inline virtual const char *className() const { return CRiInterior::myClassName(); }

	inline CRiInterior(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_INTERIOR; }
	inline virtual void replay(IRi &ri) { ri.interiorV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiInterior &operator=(const CRiInterior &) {
		return *this;
	}
}; // CRiInterior

///////////////////////////////////////////////////////////////////////////////
class CRiExterior : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiExterior"; }
	inline virtual const char *className() const { return CRiExterior::myClassName(); }

	inline CRiExterior(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_EXTERIOR; }
	inline virtual void replay(IRi &ri) { ri.exteriorV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiExterior &operator=(const CRiExterior &) {
		return *this;
	}
}; // CRiExterior

///////////////////////////////////////////////////////////////////////////////
class CRiDisplacement : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiDisplacement"; }
	inline virtual const char *className() const { return CRiDisplacement::myClassName(); }

	inline CRiDisplacement(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLACEMENT; }
	inline virtual void replay(IRi &ri) { ri.displacementV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiDisplacement &operator=(const CRiDisplacement &) {
		return *this;
	}
}; // CRiDisplacement

///////////////////////////////////////////////////////////////////////////////
class CRiTextureCoordinates : public CRManInterfaceCall {
protected:
	RtFloat m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4;
public:
	inline static const char *myClassName(void) { return "CRiTextureCoordinates"; }
	inline virtual const char *className() const { return CRiTextureCoordinates::myClassName(); }

	inline CRiTextureCoordinates(long aLineNo, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) : CRManInterfaceCall(aLineNo), m_s1(s1), m_t1(t1), m_s2(s2), m_t2(t2), m_s3(s3), m_t3(t3), m_s4(s4), m_t4(t4) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TEXTURE_COORDINATES; }
	inline virtual void replay(IRi &ri) { ri.textureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4); }
	inline CRiTextureCoordinates &operator=(const CRiTextureCoordinates &) {
		return *this;
	}
}; // CRiTextureCoordinates

///////////////////////////////////////////////////////////////////////////////
class CRiShadingRate : public CRManInterfaceCall {
protected:
	RtFloat m_size;
public:
	inline static const char *myClassName(void) { return "CRiShadingRate"; }
	inline virtual const char *className() const { return CRiShadingRate::myClassName(); }

	inline CRiShadingRate(long aLineNo, RtFloat size) : CRManInterfaceCall(aLineNo), m_size(size) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_RATE; }
	inline virtual void replay(IRi &ri) { ri.shadingRate(m_size); }
	inline CRiShadingRate &operator=(const CRiShadingRate &) {
		return *this;
	}
}; // CRiShadingRate

///////////////////////////////////////////////////////////////////////////////
class CRiShadingInterpolation : public CRManInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "CRiShadingInterpolation"; }
	inline virtual const char *className() const { return CRiShadingInterpolation::myClassName(); }

	inline CRiShadingInterpolation(long aLineNo, RtToken type) : CRManInterfaceCall(aLineNo), m_type(type) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_INTERPOLATION; }
	inline virtual void replay(IRi &ri) { ri.shadingInterpolation(m_type.c_str()); }
	inline CRiShadingInterpolation &operator=(const CRiShadingInterpolation &) {
		return *this;
	}
}; // CRiShadingInterpolation

///////////////////////////////////////////////////////////////////////////////
class CRiMatte : public CRManInterfaceCall {
protected:
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiMatte"; }
	inline virtual const char *className() const { return CRiMatte::myClassName(); }

	inline CRiMatte(long aLineNo, RtBoolean onoff) : CRManInterfaceCall(aLineNo), m_onoff(onoff) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MATTE; }
	inline virtual void replay(IRi &ri) { ri.matte(m_onoff); }
	inline CRiMatte &operator=(const CRiMatte &) {
		return *this;
	}
}; // CRiMatte

///////////////////////////////////////////////////////////////////////////////
class CRiBound : public CRManInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "CRiBound"; }
	inline virtual const char *className() const { return CRiBound::myClassName(); }

	inline CRiBound(long aLineNo, RtBound bound) : CRManInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_BOUND; }
	inline virtual void replay(IRi &ri) { ri.bound(m_bound); }
	inline CRiBound &operator=(const CRiBound &) {
		return *this;
	}
}; // CRiBound

///////////////////////////////////////////////////////////////////////////////
class CRiDetail : public CRManInterfaceCall {
protected:
	RtBound m_bound;
public:
	inline static const char *myClassName(void) { return "CRiDetail"; }
	inline virtual const char *className() const { return CRiDetail::myClassName(); }

	inline CRiDetail(long aLineNo, RtBound bound) : CRManInterfaceCall(aLineNo) {
		memcpy(m_bound, bound, sizeof(RtBound));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL; }
	inline virtual void replay(IRi &ri) { ri.bound(m_bound); }
	inline CRiDetail &operator=(const CRiDetail &) {
		return *this;
	}
}; // CRiDetail

///////////////////////////////////////////////////////////////////////////////
class CRiDetailRange : public CRManInterfaceCall {
protected:
	RtFloat m_minvis, m_lowtran, m_uptran, m_maxvis;
public:
	inline static const char *myClassName(void) { return "CRiDetailRange"; }
	inline virtual const char *className() const { return CRiDetailRange::myClassName(); }

	inline CRiDetailRange(long aLineNo, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) : CRManInterfaceCall(aLineNo), m_minvis(minvis), m_lowtran(lowtran), m_uptran(uptran), m_maxvis(maxvis) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL_RANGE; }
	inline virtual void replay(IRi &ri) { ri.detailRange(m_minvis, m_lowtran, m_uptran, m_maxvis); }
	inline CRiDetailRange &operator=(const CRiDetailRange &) {
		return *this;
	}
}; // CRiDetailRange

///////////////////////////////////////////////////////////////////////////////
class CRiGeometricApproximation : public CRManInterfaceCall {
protected:
	std::string m_type;
	RtFloat m_value;
public:
	inline static const char *myClassName(void) { return "CRiGeometricApproximation"; }
	inline virtual const char *className() const { return CRiGeometricApproximation::myClassName(); }

	inline CRiGeometricApproximation(long aLineNo, RtToken type, RtFloat value) : CRManInterfaceCall(aLineNo), m_type(type), m_value(value) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_APPROXIMATION; }
	inline virtual void replay(IRi &ri) { ri.geometricApproximation(m_type.c_str(), m_value); }
	inline CRiGeometricApproximation &operator=(const CRiGeometricApproximation &) {
		return *this;
	}
}; // CRiGeometricApproximation

///////////////////////////////////////////////////////////////////////////////
class CRiGeometricRepresentation : public CRManInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "CRiGeometricRepresentation"; }
	inline virtual const char *className() const { return CRiGeometricRepresentation::myClassName(); }

	inline CRiGeometricRepresentation(long aLineNo, RtToken type) : CRManInterfaceCall(aLineNo), m_type(type) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_REPRESENTATION; }
	inline virtual void replay(IRi &ri) { ri.geometricRepresentation(m_type.c_str()); }
	inline CRiGeometricRepresentation &operator=(const CRiGeometricRepresentation &) {
		return *this;
	}
}; // CRiGeometricRepresentation

///////////////////////////////////////////////////////////////////////////////
class CRiOrientation : public CRManInterfaceCall {
protected:
	std::string m_orientation;
public:
	inline static const char *myClassName(void) { return "CRiOrientation"; }
	inline virtual const char *className() const { return CRiOrientation::myClassName(); }

	inline CRiOrientation(long aLineNo, RtToken orientation) : CRManInterfaceCall(aLineNo), m_orientation(orientation) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ORIENTATION; }
	inline virtual void replay(IRi &ri) { ri.orientation(m_orientation.c_str()); }
	inline CRiOrientation &operator=(const CRiOrientation &) {
		return *this;
	}
}; // CRiOrientation

///////////////////////////////////////////////////////////////////////////////
class CRiReverseOrientation : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiReverseOrientation"; }
	inline virtual const char *className() const { return CRiReverseOrientation::myClassName(); }

	inline CRiReverseOrientation(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_REVERSE_ORIENTATION; }
	inline virtual void replay(IRi &ri) { ri.reverseOrientation(); }
	inline CRiReverseOrientation &operator=(const CRiReverseOrientation &) {
		return *this;
	}
}; // CRiReverseOrientation

///////////////////////////////////////////////////////////////////////////////
class CRiSides : public CRManInterfaceCall {
protected:
	RtInt m_nsides;
public:
	inline static const char *myClassName(void) { return "CRiSides"; }
	inline virtual const char *className() const { return CRiSides::myClassName(); }

	inline CRiSides(long aLineNo, RtInt nsides) : CRManInterfaceCall(aLineNo), m_nsides(nsides) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SIDES; }
	inline virtual void replay(IRi &ri) { ri.sides(m_nsides); }
	inline CRiSides &operator=(const CRiSides &) {
		return *this;
	}
}; // CRiSides

///////////////////////////////////////////////////////////////////////////////
class CRiBasis : public CRManInterfaceCall {
protected:
	RtInt m_ustep, m_vstep;
	RtBasis m_ubasis, m_vbasis;
public:
	inline static const char *myClassName(void) { return "CRiBasis"; }
	inline virtual const char *className() const { return CRiBasis::myClassName(); }

	inline CRiBasis(long aLineNo, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) : CRManInterfaceCall(aLineNo), m_ustep(ustep), m_vstep(vstep) {
		memcpy(m_ubasis, ubasis, sizeof(RtBasis));
		memcpy(m_vbasis, vbasis, sizeof(RtBasis));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_BASIS; }
	inline virtual void replay(IRi &ri) { ri.basis(m_ubasis, m_ustep, m_vbasis, m_vstep); }
	inline CRiBasis &operator=(const CRiBasis &) {
		return *this;
	}
}; // CRiBasis

///////////////////////////////////////////////////////////////////////////////
class CRiTrimCurve : public CRManInterfaceCall {
protected:
	CTrimCurveData m_trimCurve;

public:
	inline static const char *myClassName(void) { return "CRiTrimCurve"; }
	inline virtual const char *className() const { return CRiTrimCurve::myClassName(); }

	inline CRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) : CRManInterfaceCall(aLineNo), m_trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w) {}
	inline CRiTrimCurve(long aLineNo, const CTrimCurveData &CRimCurve)  : CRManInterfaceCall(aLineNo), m_trimCurve(CRimCurve) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRIM_CURVE; }
	inline virtual void replay(IRi &ri) {
		ri.trimCurve(m_trimCurve.m_nLoops,
			m_trimCurve.m_nCurves.empty() ? 0 : &m_trimCurve.m_nCurves[0],
			m_trimCurve.m_order.empty() ? 0 : &m_trimCurve.m_order[0],
			m_trimCurve.m_knots.empty() ? 0 : &m_trimCurve.m_knots[0],
			m_trimCurve.m_min.empty() ? 0 : &m_trimCurve.m_min[0],
			m_trimCurve.m_max.empty() ? 0 : &m_trimCurve.m_max[0],
			m_trimCurve.m_n.empty() ? 0 : &m_trimCurve.m_n[0],
			m_trimCurve.m_u.empty() ? 0 : &m_trimCurve.m_u[0],
			m_trimCurve.m_v.empty() ? 0 : &m_trimCurve.m_v[0],
			m_trimCurve.m_w.empty() ? 0 : &m_trimCurve.m_w[0]);
	}
	inline CRiTrimCurve &operator=(const CRiTrimCurve &) {
		return *this;
	}
	inline const CTrimCurveData &CRimCurve() { return m_trimCurve; }
}; // CRiTrimCurve

///////////////////////////////////////////////////////////////////////////////
class CRiIdentity : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiIdentity"; }
	inline virtual const char *className() const { return CRiIdentity::myClassName(); }

	inline CRiIdentity(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }
	inline virtual void replay(IRi &ri) { ri.identity(); }
	inline CRiIdentity &operator=(const CRiIdentity &) {
		return *this;
	}
}; // CRiIdentity

///////////////////////////////////////////////////////////////////////////////
class CRiTransform : public CRManInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "CRiTransform"; }
	inline virtual const char *className() const { return CRiTransform::myClassName(); }

	inline CRiTransform(long aLineNo, RtMatrix transform) : CRManInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM; }
	inline virtual void replay(IRi &ri) { ri.transform(m_transform); }
	inline CRiTransform &operator=(const CRiTransform &) {
		return *this;
	}
}; // CRiTransform

///////////////////////////////////////////////////////////////////////////////
class CRiConcatTransform : public CRManInterfaceCall {
protected:
	RtMatrix m_transform;
public:
	inline static const char *myClassName(void) { return "CRiConcatTransform"; }
	inline virtual const char *className() const { return CRiConcatTransform::myClassName(); }

	inline CRiConcatTransform(long aLineNo, RtMatrix transform) : CRManInterfaceCall(aLineNo) {
		memcpy(m_transform, transform, sizeof(RtMatrix));
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CONCAT_TRANSFORM; }
	inline virtual void replay(IRi &ri) { ri.concatTransform(m_transform); }
	inline CRiConcatTransform &operator=(const CRiConcatTransform &) {
		return *this;
	}
}; // CRiConcatTransform

///////////////////////////////////////////////////////////////////////////////
class CRiPerspective : public CRManInterfaceCall {
protected:
	RtFloat m_fov;
public:
	inline static const char *myClassName(void) { return "CRiPerspective"; }
	inline virtual const char *className() const { return CRiPerspective::myClassName(); }

	inline CRiPerspective(long aLineNo, RtFloat fov) : CRManInterfaceCall(aLineNo), m_fov(fov) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PERSPECTIVE; }
	inline virtual void replay(IRi &ri) { ri.perspective(m_fov); }
	inline CRiPerspective &operator=(const CRiPerspective &) {
		return *this;
	}
}; // CRiPerspective

///////////////////////////////////////////////////////////////////////////////
class CRiTranslate : public CRManInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiTranslate"; }
	inline virtual const char *className() const { return CRiTranslate::myClassName(); }

	inline CRiTranslate(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSLATE; }
	inline virtual void replay(IRi &ri) { ri.translate(m_dx, m_dy, m_dz); }
	inline CRiTranslate &operator=(const CRiTranslate &) {
		return *this;
	}
}; // CRiTranslate

///////////////////////////////////////////////////////////////////////////////
class CRiRotate : public CRManInterfaceCall {
protected:
	RtFloat m_angle, m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiRotate"; }
	inline virtual const char *className() const { return CRiRotate::myClassName(); }

	inline CRiRotate(long aLineNo, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_angle(angle), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ROTATE; }
	inline virtual void replay(IRi &ri) { ri.rotate(m_angle, m_dx, m_dy, m_dz); }
	inline CRiRotate &operator=(const CRiRotate &) {
		return *this;
	}
}; // CRiRotate

///////////////////////////////////////////////////////////////////////////////
class CRiScale : public CRManInterfaceCall {
protected:
	RtFloat m_dx, m_dy, m_dz;
public:
	inline static const char *myClassName(void) { return "CRiScale"; }
	inline virtual const char *className() const { return CRiScale::myClassName(); }

	inline CRiScale(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) : CRManInterfaceCall(aLineNo), m_dx(dx), m_dy(dy), m_dz(dz) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SCALE; }
	inline virtual void replay(IRi &ri) { ri.scale(m_dx, m_dy, m_dz); }
	inline CRiScale &operator=(const CRiScale &) {
		return *this;
	}
}; // CRiScale

///////////////////////////////////////////////////////////////////////////////
class CRiSkew : public CRManInterfaceCall {
protected:
	RtFloat m_angle;
	RtFloat m_dx1, m_dy1, m_dz1;
	RtFloat m_dx2, m_dy2, m_dz2;
public:
	inline static const char *myClassName(void) { return "CRiSkew"; }
	inline virtual const char *className() const { return CRiSkew::myClassName(); }

	inline CRiSkew(long aLineNo, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) : CRManInterfaceCall(aLineNo), m_angle(angle), m_dx1(dx1), m_dy1(dy1), m_dz1(dz1), m_dx2(dx2), m_dy2(dy2), m_dz2(dz2) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SKEW; }
	inline virtual void replay(IRi &ri) { ri.skew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2); }
	inline CRiSkew &operator=(const CRiSkew &) {
		return *this;
	}
}; // CRiSkew

///////////////////////////////////////////////////////////////////////////////
class CRiDeformation : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiDeformation"; }
	inline virtual const char *className() const { return CRiDeformation::myClassName(); }

	inline CRiDeformation(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DEFORMATION; }
	inline virtual void replay(IRi &ri) { ri.deformationV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiDeformation &operator=(const CRiDeformation &) {
		return *this;
	}
}; // CRiDeformation

///////////////////////////////////////////////////////////////////////////////
class CRiCoordinateSystem : public CRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiCoordinateSystem"; }
	inline virtual const char *className() const { return CRiCoordinateSystem::myClassName(); }

	inline CRiCoordinateSystem(long aLineNo, const char *name) : CRManInterfaceCall(aLineNo), m_name(name) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IRi &ri) { ri.coordinateSystem(m_name.c_str()); }
	inline CRiCoordinateSystem &operator=(const CRiCoordinateSystem &) {
		return *this;
	}
}; // CRiCoordinateSystem

///////////////////////////////////////////////////////////////////////////////
class CRiCoordSysTransform : public CRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiCoordSysTransform"; }
	inline virtual const char *className() const { return CRiCoordSysTransform::myClassName(); }

	inline CRiCoordSysTransform(long aLineNo, const char *name) : CRManInterfaceCall(aLineNo), m_name(name) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IRi &ri) { ri.coordSysTransform(m_name.c_str()); }
	inline CRiCoordSysTransform &operator=(const CRiCoordSysTransform &) {
		return *this;
	}
}; // CRiCoordSysTransform

///////////////////////////////////////////////////////////////////////////////
// ????
class CRiTransformPoints : public CRManInterfaceCall {
protected:
	std::string m_fromSpace, m_toSpace;
	RtInt m_nPoints;
	RtPoint *m_points;
	// std::vector<RtFloat> m_fromPoints;
	// std::vector<RtFloat> m_toPoints;
public:
	inline static const char *myClassName(void) { return "CRiTransformPoints"; }
	inline virtual const char *className() const { return CRiTransformPoints::myClassName(); }

	inline CRiTransformPoints(long aLineNo, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points) : CRManInterfaceCall(aLineNo), m_fromSpace(fromspace), m_toSpace(tospace), m_nPoints(npoints), m_points(points) {
	/*
	m_fromPoints.resize(m_npoints*3);
	m_toPoints.resize(m_npoints*3);
	if ( points )
	m_fromPoints.assign(&m_points[0][0], (&m_points[0][0])+m_npoints*3);
		*/
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	inline virtual void replay(IRi &ri) {
	/*
	m_toPoints = m_fromPoints;
	RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, (RtPoint *)&m_toPoints[0]);
		*/
		RtPoint *p = ri.transformPoints(m_fromSpace.c_str(), m_toSpace.c_str(), m_nPoints, m_points);
		p = p;
	}
	inline CRiTransformPoints &operator=(const CRiTransformPoints &) {
		return *this;
	}
}; // CRiTransformPoints

///////////////////////////////////////////////////////////////////////////////
class CRiPolygon : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nVertices;
public:
	inline static const char *myClassName(void) { return "CRiPolygon"; }
	inline virtual const char *className() const { return CRiPolygon::myClassName(); }

	inline CRiPolygon(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CPolygonRManInterfaceCall(aLineNo), m_nVertices(nvertices)
	{
		CPolygonClasses p(nvertices);
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_POLYGON; }
	inline virtual void replay(IRi &ri) { ri.polygonV(m_nVertices, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPolygon &operator=(const CRiPolygon &) {
		return *this;
	}
}; // CRiPolygon

///////////////////////////////////////////////////////////////////////////////
class CRiGeneralPolygon : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nLoops;
	std::vector<RtInt> m_nVerts;
public:
	inline static const char *myClassName(void) { return "CRiGeneralPolygon"; }
	inline virtual const char *className() const { return CRiGeneralPolygon::myClassName(); }

	CRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_GENERAL_POLYGON; }
	inline virtual void replay(IRi &ri) {
		ri.generalPolygonV(m_nLoops,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiGeneralPolygon &operator=(const CRiGeneralPolygon &) {
		return *this;
	}
}; // CRiGeneralPolygon

///////////////////////////////////////////////////////////////////////////////
class CRiPointsPolygons : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nPolys;
	std::vector<RtInt> m_nVerts;
	std::vector<RtInt> m_verts;
public:
	inline static const char *myClassName(void) { return "CRiPointsPolygons"; }
	inline virtual const char *className() const { return CRiPointsPolygons::myClassName(); }

	CRiPointsPolygons(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }
	inline virtual void replay(IRi &ri) {
		ri.pointsPolygonsV(
			m_nPolys,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPointsPolygons &operator=(const CRiPointsPolygons &) {
		return *this;
	}
}; // CRiPointsPolygons

///////////////////////////////////////////////////////////////////////////////
class CRiPointsGeneralPolygons : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nPolys;
	std::vector<RtInt> m_nLoops;
	std::vector<RtInt> m_nVerts;
	std::vector<RtInt> m_verts;
public:
	inline static const char *myClassName(void) { return "CRiPointsGeneralPolygons"; }
	inline virtual const char *className() const { return CRiPointsGeneralPolygons::myClassName(); }

	CRiPointsGeneralPolygons(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }
	inline virtual void replay(IRi &ri) {
		ri.pointsGeneralPolygonsV(
			m_nPolys,
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPointsGeneralPolygons &operator=(const CRiPointsGeneralPolygons &) {
		return *this;
	}
}; // CRiPointsGeneralPolygons

///////////////////////////////////////////////////////////////////////////////
class CRiPatch : public CUVRManInterfaceCall {
protected:
	std::string m_type;
public:
	inline static const char *myClassName(void) { return "CRiPatch"; }
	inline virtual const char *className() const { return CRiPatch::myClassName(); }

	inline CRiPatch(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo), m_type(type)
	{
		CPatchClasses p(type);
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IRi &ri) { ri.patchV(m_type.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPatch &operator=(const CRiPatch &) {
		return *this;
	}
}; // CRiPatch

///////////////////////////////////////////////////////////////////////////////
class CRiPatchMesh : public CUVRManInterfaceCall {
protected:
	std::string m_type, m_uwrap, m_vwrap;
	RtInt m_nu, m_nv;
	RtInt m_ustep, m_vstep;
public:
	inline static const char *myClassName(void) { return "CRiPatchMesh"; }
	inline virtual const char *className() const { return CRiPatchMesh::myClassName(); }

	CRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IRi &ri) { ri.patchMeshV(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPatchMesh &operator=(const CRiPatchMesh &) {
		return *this;
	}
}; // CRiPatchMesh

///////////////////////////////////////////////////////////////////////////////
class CRiNuPatch : public CGeometryRManInterfaceCall {
protected:
	RtInt m_nu, m_uorder, m_nv, m_vorder;
	std::vector<RtFloat> m_uknot, m_vknot;
	RtFloat m_umin, m_umax, m_vmin, m_vmax;
public:
	inline static const char *myClassName(void) { return "CRiNuPatch"; }
	inline virtual const char *className() const { return CRiNuPatch::myClassName(); }

	CRiNuPatch(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IRi &ri) {
		ri.nuPatchV(m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiNuPatch &operator=(const CRiNuPatch &) {
		return *this;
	}
	inline virtual int segments() const { return (1+m_nu-m_uorder)*(1+m_nv-m_vorder); }
}; // CRiNuPatch

///////////////////////////////////////////////////////////////////////////////
class CRiSubdivisionMesh : public CGeometryRManInterfaceCall {
protected:
//	struct CRiSubdivisionMeshData {
		std::string m_scheme;
		RtInt m_nfaces, m_ntags, m_nvertices;
		RtInt m_nedges; // number of edges in the surface
		std::vector<RtInt> m_nverts, m_verts, m_nargs, m_intargs;
		std::vector<RtFloat>    m_floargs;
		std::deque<std::string> m_strtags;
		std::vector<RtToken>    m_tags;
//	};
//	std::vector<CRiSubdivisionMeshData> m_data;
public:
	inline static const char *myClassName(void) { return "CRiSubdivisionMesh"; }
	inline virtual const char *className() const { return CRiSubdivisionMesh::myClassName(); }

	CRiSubdivisionMesh(long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);
	inline virtual EnumRequests interfaceIdx() const { return REQ_SUBDIVISION_MESH; }
	inline virtual void replay(IRi &ri) {
		ri.subdivisionMeshV(
			m_scheme.c_str(), m_nfaces,
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			m_ntags,
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			static_cast<RtInt>(size()), getTokens(), getParams()
			);

		/*
		int idx = 0;
		ri.subdivisionMeshV(
			m_data[idx].m_scheme.c_str(), m_data[idx].m_nfaces, &(m_data[idx].m_nverts[0]), &(m_data[idx].m_verts[0]),
			m_data[idx].m_ntags, &(m_data[idx].m_tags[0]), &(m_data[idx].m_nargs[0]), &(m_data[idx].m_intargs[0]), &(m_data[idx].m_floargs[0]),
			static_cast<RtInt>(size()), getTokens(), getParams()
			);
		*/
	}
	inline CRiSubdivisionMesh &operator=(const CRiSubdivisionMesh &) {
		return *this;
	}
}; // CRiSubdivisionMesh

///////////////////////////////////////////////////////////////////////////////
class CRiSphere : public CUVRManInterfaceCall {
protected:
	RtFloat m_radius, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiSphere"; }
	inline virtual const char *className() const { return CRiSphere::myClassName(); }

	inline CRiSphere(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }
	inline virtual void replay(IRi &ri) { ri.sphereV(m_radius, m_zmin, m_zmax, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiSphere &operator=(const CRiSphere &) {
		return *this;
	}
}; // CRiSphere

///////////////////////////////////////////////////////////////////////////////
class CRiCone : public CUVRManInterfaceCall {
protected:
	RtFloat m_height, m_radius, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiCone"; }
	inline virtual const char *className() const { return CRiCone::myClassName(); }

	inline CRiCone(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CONE; }
	inline virtual void replay(IRi &ri) {
		ri.coneV(m_height, m_radius, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiCone &operator=(const CRiCone &) {
		return *this;
	}
}; // CRiCone

///////////////////////////////////////////////////////////////////////////////
class CRiCylinder : public CUVRManInterfaceCall {
protected:
	RtFloat m_radius, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiCylinder"; }
	inline virtual const char *className() const { return CRiCylinder::myClassName(); }

	inline CRiCylinder(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }
	inline virtual void replay(IRi &ri) { ri.cylinderV(m_radius, m_zmin, m_zmax, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiCylinder &operator=(const CRiCylinder &) {
		return *this;
	}
}; // CRiCylinder

///////////////////////////////////////////////////////////////////////////////
class CRiHyperboloid : public CUVRManInterfaceCall {
protected:
	RtPoint m_point1, m_point2;
	RtFloat m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiHyperboloid"; }
	inline virtual const char *className() const { return CRiHyperboloid::myClassName(); }

	inline CRiHyperboloid(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo), m_thetamax(thetamax)
	{
		int i;
		for ( i = 0; i < 3; ++i ) {
			m_point1[i] = point1[i];
			m_point2[i] = point2[i];
		}
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_HYPERBOLOID; }
	inline virtual void replay(IRi &ri) { ri.hyperboloidV(m_point1, m_point2, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiHyperboloid &operator=(const CRiHyperboloid &) {
		return *this;
	}
}; // CRiHyperboloid

///////////////////////////////////////////////////////////////////////////////
class CRiParaboloid : public CUVRManInterfaceCall {
protected:
	RtFloat m_rmax, m_zmin, m_zmax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiParaboloid"; }
	inline virtual const char *className() const { return CRiParaboloid::myClassName(); }

	inline CRiParaboloid(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_rmax(rmax), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PARABOLOID; }
	inline virtual void replay(IRi &ri) { ri.paraboloidV(m_rmax, m_zmin, m_zmax, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiParaboloid &operator=(const CRiParaboloid &) {
		return *this;
	}
}; // CRiParaboloid

///////////////////////////////////////////////////////////////////////////////
class CRiDisk : public CUVRManInterfaceCall {
protected:
	RtFloat m_height, m_radius, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiDisk"; }
	inline virtual const char *className() const { return CRiDisk::myClassName(); }

	inline CRiDisk(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISK; }
	inline virtual void replay(IRi &ri) {
		ri.diskV(m_height, m_radius, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiDisk &operator=(const CRiDisk &) {
		return *this;
	}
}; // CRiDisk

///////////////////////////////////////////////////////////////////////////////
class CRiTorus : public CUVRManInterfaceCall {
protected:
	RtFloat m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax;
public:
	inline static const char *myClassName(void) { return "CRiTorus"; }
	inline virtual const char *className() const { return CRiTorus::myClassName(); }

	inline CRiTorus(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_majorrad(majorrad), m_minorrad(minorrad),
		  m_phimin(phimin), m_phimax(phimax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_TORUS; }
	inline virtual void replay(IRi &ri) { ri.torusV(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiTorus &operator=(const CRiTorus &) {
		return *this;
	}
}; // CRiTorus

///////////////////////////////////////////////////////////////////////////////
class CRiPoints : public CGeometryRManInterfaceCall {
protected:
	RtInt m_npts;
public:
	inline static const char *myClassName(void) { return "CRiPoints"; }
	inline virtual const char *className() const { return CRiPoints::myClassName(); }

	inline CRiPoints(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_npts(npts)
	{
		CPointsClasses p(npts);
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }
	inline virtual void replay(IRi &ri) { ri.pointsV(m_npts, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiPoints &operator=(const CRiPoints &) {
		return *this;
	}
}; // CRiPoints

///////////////////////////////////////////////////////////////////////////////
class CRiCurves : public CGeometryRManInterfaceCall {
protected:
	std::string m_type, m_wrap;
	RtInt m_ncurves, m_ustep, m_vstep;
	std::vector<RtInt> m_nverts;
public:
	inline static const char *myClassName(void) { return "CRiCurves"; }
	inline virtual const char *className() const { return CRiCurves::myClassName(); }

	CRiCurves(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_CURVES; }
	inline virtual void replay(IRi &ri) {
		ri.curvesV(m_type.c_str(), m_ncurves,
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiCurves &operator=(const CRiCurves &) {
		return *this;
	}
}; // CRiCurves

///////////////////////////////////////////////////////////////////////////////
class CRiBlobby : public CGeometryRManInterfaceCall {
protected:
	RtInt m_nleaf, m_ncode, m_nflt, m_nstr;
	std::vector<RtInt> m_code;
	std::vector<RtFloat> m_flt;
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;
public:
	inline static const char *myClassName(void) { return "CRiBlobby"; }
	inline virtual const char *className() const { return CRiBlobby::myClassName(); }

	CRiBlobby(long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_BLOBBY; }
	inline virtual void replay(IRi &ri) {
		ri.blobbyV(m_nleaf,
			m_ncode, m_code.empty() ? 0 : &m_code[0],
			m_nflt, m_flt.empty() ? 0 : &m_flt[0],
			m_nstr, m_str.empty() ? 0 : &m_str[0],
			static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiBlobby &operator=(const CRiBlobby &) {
		return *this;
	}
}; // CRiBlobby

///////////////////////////////////////////////////////////////////////////////
class CRiProcedural : public CRManInterfaceCall {
protected:
	RtBound m_bound;
	ISubdivFunc *m_subdivfunc;
	IFreeFunc *m_freefunc;
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;

	void insertData(RtPointer data, IRi &ri);
public:
	inline static const char *myClassName(void) { return "CRiProcedural"; }
	inline virtual const char *className() const { return CRiProcedural::myClassName(); }

	inline CRiProcedural(long aLineNo, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc, IRi &ri)
		: CRManInterfaceCall(aLineNo)
	{
		m_subdivfunc = subdivfunc.duplicate();
		if ( !m_subdivfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiProcedural, m_subdivfunc", __LINE__, __FILE__);
		}
		m_freefunc = freefunc.duplicate();
		if ( !m_freefunc ) {
			delete m_subdivfunc;
			m_subdivfunc = 0;
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiProcedural, m_freefunc", __LINE__, __FILE__);
		}
		memcpy(m_bound, bound, sizeof(RtBound));
		insertData(data, ri);
	}
	inline virtual ~CRiProcedural()
	{
		if ( m_subdivfunc )
			delete m_subdivfunc;
		if ( m_freefunc )
			delete m_freefunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PROCEDURAL; }
	inline virtual void replay(IRi &ri) {
		assert(m_subdivfunc != 0);
		assert(m_freefunc != 0);
		if ( m_subdivfunc != 0 && m_freefunc != 0 )
		ri.procedural(m_str.empty() ? 0 :
			m_str.empty() ? 0 : (RtPointer)&m_str[0],
			m_bound, *m_subdivfunc, *m_freefunc);
	}
	inline CRiProcedural &operator=(const CRiProcedural &) {
		return *this;
	}
}; // CRiProcedural

   /*
   // PROC_... are not called directly, only parameters for RiProcedural()
   #define REQ_PROC_DELAYED_READ_ARCHIVE	 94
   #define REQ_PROC_RUN_PROGRAM			 95
   #define REQ_PROC_DYNAMIC_LOAD			 96
*/

///////////////////////////////////////////////////////////////////////////////
class CRiGeometry : public CGeometryRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiGeometry"; }
	inline virtual const char *className() const { return CRiGeometry::myClassName(); }

	inline CRiGeometry(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }
	inline virtual void replay(IRi &ri) { ri.geometryV(m_name.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiGeometry &operator=(const CRiGeometry &) {
		return *this;
	}
}; // CRiGeometry

///////////////////////////////////////////////////////////////////////////////
class CRiObjectInstance : public CRManInterfaceCall {
protected:
	RtObjectHandle *m_handle;
public:
	inline static const char *myClassName(void) { return "CRiObjectInstance"; }
	inline virtual const char *className() const { return CRiObjectInstance::myClassName(); }

	inline CRiObjectInstance(long aLineNo, RtObjectHandle *handle) : CRManInterfaceCall(aLineNo), m_handle(handle)
	{
		assert(handle != 0);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_INSTANCE; }
	inline virtual void replay(IRi &ri)
	{
		assert(m_handle != 0);
		if ( m_handle != 0 )
			ri.objectInstance(*m_handle);
	}
	inline virtual RtObjectHandle *handleRef() { return m_handle; }
	inline CRiObjectInstance &operator=(const CRiObjectInstance &) {
		return *this;
	}
}; // CRiObjectInstance

///////////////////////////////////////////////////////////////////////////////
class CRiMakeTexture : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeTexture"; }
	inline virtual const char *className() const { return CRiMakeTexture::myClassName(); }

	inline CRiMakeTexture(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeTexture, m_filterfunc", __LINE__, __FILE__);
		}
	}

	inline virtual ~CRiMakeTexture()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_TEXTURE; }
	inline virtual void replay(IRi &ri) {
		assert(m_filterfunc != 0);
		if ( m_filterfunc )
			ri.makeTextureV(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiMakeTexture &operator=(const CRiMakeTexture &) {
		return *this;
	}
}; // CRiMakeTexture

///////////////////////////////////////////////////////////////////////////////
class CRiMakeBump : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeBump"; }
	inline virtual const char *className() const { return CRiMakeBump::myClassName(); }

	inline CRiMakeBump(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeBump, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeBump()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_BUMP; }
	inline virtual void replay(IRi &ri) {
		assert(m_filterfunc != 0);
		if ( m_filterfunc )
			ri.makeBumpV(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiMakeBump &operator=(const CRiMakeBump &) {
		return *this;
	}
}; // CRiMakeBump

///////////////////////////////////////////////////////////////////////////////
class CRiMakeLatLongEnvironment : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeLatLongEnvironment"; }
	inline virtual const char *className() const { return CRiMakeLatLongEnvironment::myClassName(); }

	inline CRiMakeLatLongEnvironment(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeLatLongEnvironment, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeLatLongEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_LAT_LONG_ENVIRONMENT; }
	inline virtual void replay(IRi &ri)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc )
			ri.makeLatLongEnvironmentV(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiMakeLatLongEnvironment &operator=(const CRiMakeLatLongEnvironment &) {
		return *this;
	}
}; // CRiMakeLatLongEnvironment

///////////////////////////////////////////////////////////////////////////////
class CRiMakeCubeFaceEnvironment : public CVarParamRManInterfaceCall {
protected:
	std::string m_px, m_nx, m_py, m_ny, m_pz, m_nz, m_tex;
	RtFloat m_fov;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeCubeFaceEnvironment"; }
	inline virtual const char *className() const { return CRiMakeCubeFaceEnvironment::myClassName(); }

	inline CRiMakeCubeFaceEnvironment(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_px(px), m_nx(nx), m_py(py), m_ny(ny), m_pz(pz), m_nz(nz), m_tex(tex), m_fov(fov), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeCubeFaceEnvironment, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeCubeFaceEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_CUBE_FACE_ENVIRONMENT; }
	inline virtual void replay(IRi &ri) {
		assert(m_filterfunc != 0);
		if ( m_filterfunc )
			ri.makeCubeFaceEnvironmentV(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline CRiMakeCubeFaceEnvironment &operator=(const CRiMakeCubeFaceEnvironment &) {
		return *this;
	}
}; // CRiMakeCubeFaceEnvironment

///////////////////////////////////////////////////////////////////////////////
class CRiMakeShadow : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex;
public:
	inline static const char *myClassName(void) { return "CRiMakeShadow"; }
	inline virtual const char *className() const { return CRiMakeShadow::myClassName(); }

	inline CRiMakeShadow(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_SHADOW; }
	inline virtual void replay(IRi &ri) { ri.makeShadowV(m_pic.c_str(), m_tex.c_str(), static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiMakeShadow &operator=(const CRiMakeShadow &) {
		return *this;
	}
}; // CRiMakeShadow

///////////////////////////////////////////////////////////////////////////////
class CRiArchiveRecord : public CRManInterfaceCall {
protected:
	std::string m_type;
	std::string m_line;
public:
	inline static const char *myClassName(void) { return "CRiArchiveRecord"; }
	inline virtual const char *className() const { return CRiArchiveRecord::myClassName(); }

	inline CRiArchiveRecord(long aLineNo, RtToken type, const char *line) : CRManInterfaceCall(aLineNo), m_type(type), m_line(line) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_RECORD; }
	inline virtual void replay(IRi &ri) { ri.archiveRecordV(m_type.c_str(), m_line.c_str()); }
	inline virtual void replay(IRi &ri, const IArchiveCallback *callback) {
		ri.archiveRecordV(m_type.c_str(), m_line.c_str());
		if ( callback ) 
			(*callback)(ri, m_type.c_str(), m_line.c_str());
	}
	inline CRiArchiveRecord &operator=(const CRiArchiveRecord &) {
		return *this;
	}
}; // CRiArchiveRecord

///////////////////////////////////////////////////////////////////////////////
class CRiReadArchive : public CVarParamRManInterfaceCall {
protected:
	std::string m_filename;
	IArchiveCallback *m_callback;
public:
	inline static const char *myClassName(void) { return "CRiReadArchive"; }
	inline virtual const char *className() const { return CRiReadArchive::myClassName(); }

	inline CRiReadArchive(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtString filename, const IArchiveCallback *callback,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_filename(filename)
	{
		CParameterClasses p;
		setParams(decl, p, curColorSize, n, tokens, params);

		m_callback = 0;
		if ( callback )
			m_callback = callback->duplicate();
	}

	inline virtual ~CRiReadArchive()
	{
		if ( m_callback )
			delete m_callback;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
	inline virtual void replay(IRi &ri, const IArchiveCallback *callback)
	{
		ri.readArchiveV(m_filename.c_str(), callback, static_cast<RtInt>(size()), getTokens(), getParams());
	}
	inline virtual void replay(IRi &ri) { ri.readArchiveV(m_filename.c_str(), m_callback, static_cast<RtInt>(size()), getTokens(), getParams()); }
	inline CRiReadArchive &operator=(const CRiReadArchive &) {
		return *this;
	}
}; // CRiReadArchive


///////////////////////////////////////////////////////////////////////////////
//! Interface, return a new instance of a IRi... class.
class CRManInterfaceFactory {

	inline CRManInterfaceFactory() {}
	inline virtual ~CRManInterfaceFactory() {}

	inline virtual CRiDeclare *newRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
	{
		return new CRiDeclare(aLineNo, name, declaration);	
	}

	inline virtual CRiErrorHandler *newRiErrorHandler(
		long aLineNo,
		const IErrorHandler &handler)
	{
		return new CRiErrorHandler(aLineNo, handler);
	}

	inline virtual CRiFrameBegin *newRiFrameBegin(long aLineNo, RtInt number) {
		return new CRiFrameBegin(aLineNo, number);
	}

	inline virtual CRiFrameEnd *newRiFrameEnd(long aLineNo) {
		return new CRiFrameEnd(aLineNo);
	}

	inline virtual CRiWorldBegin *newRiWorldBegin(long aLineNo) {
		return new CRiWorldBegin(aLineNo);
	}

	inline virtual CRiWorldEnd *newRiWorldEnd(long aLineNo) {
		return new CRiWorldEnd(aLineNo);
	}

	inline virtual CRiAttributeBegin *newRiAtCRibuteBegin(long aLineNo) {
		return new CRiAttributeBegin(aLineNo);
	}

	inline virtual CRiAttributeEnd *newRiAtCRibuteEnd(long aLineNo) {
		return new CRiAttributeEnd(aLineNo);
	}

	inline virtual CRiTransformBegin *newRiTransformBegin(long aLineNo) {
		return new CRiTransformBegin(aLineNo);
	}

	inline virtual CRiTransformEnd *newRiTransformEnd(long aLineNo) {
		return new CRiTransformEnd(aLineNo);
	}

	inline virtual CRiSolidBegin *newRiSolidBegin(long aLineNo, RtToken operation) {
		return new CRiSolidBegin(aLineNo, operation);
	}

	inline virtual CRiSolidEnd *newRiSolidEnd(long aLineNo) {
		return new CRiSolidEnd(aLineNo);
	}

	inline virtual CRiObjectBegin *newRiObjectBegin(long aLineNo, RtObjectHandle *h) {
		return new CRiObjectBegin(aLineNo, h);
	}

	inline virtual CRiObjectEnd *newRiObjectEnd(long aLineNo) {
		return new CRiObjectEnd(aLineNo);
	}

	inline virtual CRiMotionBegin *newRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) {
		return new CRiMotionBegin(aLineNo, n, f);
	}

	inline virtual CRiMotionEnd *newRiMotionEnd(long aLineNo) {
		return new CRiMotionEnd(aLineNo);
	}

	inline virtual CRiFormat *newRiFormat(long aLineNo, RtInt xres, RtInt yres, RtFloat aspect) {
		return new CRiFormat(aLineNo, xres, yres, aspect);
	}

	inline virtual CRiFrameAspectRatio *newRiFrameAspectRatio(long aLineNo, RtFloat aspect) {
		return new CRiFrameAspectRatio(aLineNo, aspect);
	}

	inline virtual CRiScreenWindow *newRiScreenWindow(long aLineNo, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
		return new CRiScreenWindow(aLineNo, left, right, bot, top);
	}

	inline virtual CRiCropWindow *newRiCropWindow(long aLineNo, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
		return new CRiCropWindow(aLineNo, xmin, xmax, ymin, ymax);
	}

	inline virtual CRiProjection *newRiProjection(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiProjection(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiClipping *newRiClipping(long aLineNo, RtFloat hither, RtFloat yon) {
		return new CRiClipping(aLineNo, hither, yon);
	}

	inline virtual CRiClippingPlane *newRiClippingPlane(long aLineNo, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
		return new CRiClippingPlane(aLineNo, x, y, z, nx, ny, nz);
	}

	inline virtual CRiDepthOfField *newRiDepthOfField(long aLineNo, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
		return new CRiDepthOfField(aLineNo, fstop, focallength, focaldistance);
	}

	inline virtual CRiShutter *newRiShutter(long aLineNo, RtFloat smin, RtFloat smax) {
		return new CRiShutter(aLineNo, smin, smax);
	}

	inline virtual CRiPixelVariance *newRiPixelVariance(long aLineNo, RtFloat variation) {
		return new CRiPixelVariance(aLineNo, variation);
	}

	inline virtual CRiPixelSamples *newRiPixelSamples(long aLineNo, RtFloat xsamples, RtFloat ysamples) {
		return new CRiPixelSamples(aLineNo, xsamples, ysamples);
	}

	inline virtual CRiPixelFilter *newRiPixelFilter(long aLineNo, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {
		return new CRiPixelFilter(aLineNo, function, xwidth, ywidth);
	}

	inline virtual CRiExposure *newRiExposure(long aLineNo, RtFloat gain, RtFloat gamma) {
		return new CRiExposure(aLineNo, gain, gamma);
	}

	inline virtual CRiImager *newRiImager(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiImager(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiQuantize *newRiQuantize(long aLineNo, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
		return new CRiQuantize(aLineNo, type, one, qmin, qmax, ampl);
	}

	inline virtual CRiDisplay *newRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken name, RtToken type, RtToken mode,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisplay(aLineNo, decl, curColorSize, name, type, mode, n, tokens, params);
	}

	inline virtual CRiHider *newRiHider(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiHider(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiColorSamples *newRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
		return new CRiColorSamples(aLineNo, N, nRGB, RGBn);
	}

	inline virtual CRiRelativeDetail *newRiRelativeDetail(long aLineNo, RtFloat relativedetail) {
		return new CRiRelativeDetail(aLineNo, relativedetail);
	}

	inline virtual CRiOption *newRiOption(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiOption(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiLightSource *newRiLightSource(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name, RtLightHandle *handle,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiLightSource(aLineNo, decl, curColorSize, name, handle, n, tokens, params);
	}

	inline virtual CRiAreaLightSource *newRiAreaLightSource(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name, RtLightHandle *handle,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAreaLightSource(aLineNo, decl, curColorSize, name, handle, n, tokens, params);
	}

	inline virtual CRiIlluminate *newRiIlluminate(long aLineNo, RtLightHandle *handle, RtBoolean onoff) {
		return new CRiIlluminate(aLineNo, handle, onoff);
	}

	inline virtual CRiAttribute *newRiAtCRibute(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAttribute(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiColor *newRiColor(long aLineNo, RtInt size, RtColor Cs) {
		return new CRiColor(aLineNo, size, Cs);
	}

	inline virtual CRiOpacity *newRiOpacity(long aLineNo, RtInt size, RtColor Cs) {
		return new CRiOpacity(aLineNo, size, Cs);
	}

	inline virtual CRiSurface *newRiSurface(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSurface(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiAtmosphere *newRiAtmosphere(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAtmosphere(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiInterior *newRiInterior(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiInterior(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiExterior *newRiExterior(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiExterior(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiDisplacement *newRiDisplacement(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisplacement(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiTextureCoordinates *newRiTextureCoordinates(long aLineNo, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
		return new CRiTextureCoordinates(aLineNo, s1, t1, s2, t2, s3, t3, s4, t4);
	}

	inline virtual CRiShadingRate *newRiShadingRate(long aLineNo, RtFloat size) {
		return new CRiShadingRate(aLineNo, size);
	}

	inline virtual CRiShadingInterpolation *newRiShadingInterpolation(long aLineNo, RtToken type) {
		return new CRiShadingInterpolation(aLineNo, type);
	}

	inline virtual CRiMatte *newRiMatte(long aLineNo, RtBoolean onoff) {
		return new CRiMatte(aLineNo, onoff);
	}

	inline virtual CRiBound *newRiBound(long aLineNo, RtBound bound) {
		return new CRiBound(aLineNo, bound);
	}

	inline virtual CRiDetail *newRiDetail(long aLineNo, RtBound bound) {
		return new CRiDetail(aLineNo, bound);
	}

	inline virtual CRiDetailRange *newRiDetailRange(long aLineNo, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
		return new CRiDetailRange(aLineNo, minvis, lowtran, uptran, maxvis);
	}

	inline virtual CRiGeometricApproximation *newRiGeomeCRicApproximation(long aLineNo, RtToken type, RtFloat value) {
		return new CRiGeometricApproximation(aLineNo, type, value);
	}

	inline virtual CRiGeometricRepresentation *newRiGeomeCRicRepresentation(long aLineNo, RtToken type) {
		return new CRiGeometricRepresentation(aLineNo, type);
	}

	inline virtual CRiOrientation *newRiOrientation(long aLineNo, RtToken orientation) {
		return new CRiOrientation(aLineNo, orientation);
	}

	inline virtual CRiReverseOrientation *newRiReverseOrientation(long aLineNo) {
		return new CRiReverseOrientation(aLineNo);
	}

	inline virtual CRiSides *newRiSides(long aLineNo, RtInt nsides) {
		return new CRiSides(aLineNo, nsides);
	}

	inline virtual CRiBasis *newRiBasis(long aLineNo, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
		return new CRiBasis(aLineNo, ubasis, ustep, vbasis, vstep);
	}

	inline virtual CRiTrimCurve *newRiCRimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
		return new CRiTrimCurve(aLineNo, nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	inline virtual CRiTrimCurve *newRiCRimCurve(long aLineNo, const CTrimCurveData &CRimCurve) {
		return new CRiTrimCurve(aLineNo, CRimCurve);
	}

	inline virtual CRiIdentity *newRiIdentity(long aLineNo) {
		return new CRiIdentity(aLineNo);
	}

	inline virtual CRiTransform *newRiTransform(long aLineNo, RtMatrix transform) {
		return new CRiTransform(aLineNo, transform);
	}

	inline virtual CRiConcatTransform *newRiConcatTransform(long aLineNo, RtMatrix transform) {
		return new CRiConcatTransform(aLineNo, transform);
	}

	inline virtual CRiPerspective *newRiPerspective(long aLineNo, RtFloat fov) {
		return new CRiPerspective(aLineNo, fov);
	}

	inline virtual CRiTranslate *newRiTranslate(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new CRiTranslate(aLineNo, dx, dy, dz);
	}

	inline virtual CRiRotate *newRiRotate(long aLineNo, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new CRiRotate(aLineNo, angle, dx, dy, dz);
	}

	inline virtual CRiScale *newRiScale(long aLineNo, RtFloat dx, RtFloat dy, RtFloat dz) {
		return new CRiScale(aLineNo, dx, dy, dz);
	}

	inline virtual CRiSkew *newRiSkew(long aLineNo, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
		return new CRiSkew(aLineNo, angle, dx1, dy1, dz1, dx2, dy2, dz2);
	}

	inline virtual CRiDeformation *newRiDeformation(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDeformation(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiCoordinateSystem *newRiCoordinateSystem(long aLineNo, const char *name) {
		return new CRiCoordinateSystem(aLineNo, name);
	}

	inline virtual CRiCoordSysTransform *newRiCoordSysTransform(long aLineNo, const char *name) {
		return new CRiCoordSysTransform(aLineNo, name);
	}

	inline virtual CRiTransformPoints *newRiTransformPoints(long aLineNo, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points) {
		return new CRiTransformPoints(aLineNo, fromspace, tospace, npoints, points);
	}

	inline virtual CRiPolygon *newRiPolygon(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPolygon(aLineNo, decl, curColorSize, nvertices, n, tokens, params);
	}

	inline virtual CRiGeneralPolygon *newRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiGeneralPolygon(aLineNo, decl, curColorSize, nloops, nverts, n, tokens, params);
	}

	inline virtual CRiPointsPolygons *newRiPointsPolygons(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPointsPolygons(aLineNo, decl, curColorSize, npolys, nverts, verts, n, tokens, params);
	}

	inline virtual CRiPointsGeneralPolygons *newRiPointsGeneralPolygons(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPointsGeneralPolygons(aLineNo, decl, curColorSize, npolys, nloops, nverts, verts, n, tokens, params);
	}

	inline virtual CRiPatch *newRiPatch(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPatch(aLineNo, decl, curColorSize, type, n, tokens, params);
	}

	inline virtual CRiPatchMesh *newRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPatchMesh(aLineNo, decl, curColorSize, ustep, vstep, type, nu, uwrap, nv, vwrap, n, tokens, params);
	}

	inline virtual CRiNuPatch *newRiNuPatch(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiNuPatch(aLineNo, decl, curColorSize, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	}

	inline virtual CRiSubdivisionMesh *newRiSubdivisionMesh(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSubdivisionMesh(aLineNo, decl, curColorSize, scheme, nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs, n, tokens, params);
	}

	inline virtual CRiSphere *newRiSphere(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSphere(aLineNo, decl, curColorSize, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiCone *newRiCone(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCone(aLineNo, decl, curColorSize, height, radius, thetamax, n, tokens, params);
	}

	inline virtual CRiCylinder *newRiCylinder(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCylinder(aLineNo, decl, curColorSize, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiHyperboloid *newRiHyperboloid(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiHyperboloid(aLineNo, decl, curColorSize, point1, point2, thetamax, n, tokens, params);
	}

	inline virtual CRiParaboloid *newRiParaboloid(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiParaboloid(aLineNo, decl, curColorSize, rmax, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiDisk *newRiDisk(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisk(aLineNo, decl, curColorSize, height, radius, thetamax, n, tokens, params);
	}

	inline virtual CRiTorus *newRiTorus(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiTorus(aLineNo, decl, curColorSize, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	}

	inline virtual CRiPoints *newRiPoints(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPoints(aLineNo, decl, curColorSize, npts, n, tokens, params);
	}

	inline virtual CRiCurves *newRiCurves(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCurves(aLineNo, decl, curColorSize, ustep, vstep, type, ncurves, nverts, wrap, n, tokens, params);
	}

	inline virtual CRiBlobby *newRiBlobby(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiBlobby(aLineNo, decl, curColorSize, nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	}

	inline virtual CRiProcedural *newRiProcedural(long aLineNo, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc, IRi &ri) {
		return new CRiProcedural(aLineNo, data, bound, subdivfunc, freefunc, ri);
	}

	inline virtual CRiGeometry *newRiGeometry(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiGeometry(aLineNo, decl, curColorSize, name, n, tokens, params);
	}

	inline virtual CRiObjectInstance *newRiObjectInstance(long aLineNo, RtObjectHandle *handle) {
		return new CRiObjectInstance(aLineNo, handle);
	}

	inline virtual CRiMakeTexture *newRiMakeTexture(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeTexture(aLineNo, decl, curColorSize, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeBump *newRiMakeBump(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeBump(aLineNo, decl, curColorSize, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeLatLongEnvironment *newRiMakeLatLongEnvironment(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeLatLongEnvironment(aLineNo, decl, curColorSize, pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeCubeFaceEnvironment *newRiMakeCubeFaceEnvironment(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeCubeFaceEnvironment(aLineNo, decl, curColorSize, px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeShadow *newRiMakeShadow(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeShadow(aLineNo, decl, curColorSize, pic, tex, n, tokens, params);
	}

	inline virtual CRiArchiveRecord *newRiArchiveRecord(long aLineNo, RtToken type, const char *line)
	{
		return new CRiArchiveRecord(aLineNo, type, line);
	}

	inline virtual CRiReadArchive *newRiReadArchive(
		long aLineNo, CDeclarationDictionary &decl, unsigned int curColorSize,
		RtString filename, const IArchiveCallback *callback,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiReadArchive(aLineNo, decl, curColorSize, filename, callback, n, tokens, params);
	}

}; // CRManInterfaceFactory

}

#endif // _RICPP_BASERENDERER_RIMACRO_H
