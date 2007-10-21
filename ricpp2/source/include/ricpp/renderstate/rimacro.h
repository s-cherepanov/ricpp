#ifndef _RICPP_RENDERSTATE_RIMACRO_H
#define _RICPP_RENDERSTATE_RIMACRO_H


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

#ifndef _RICPP_BASERENDERER_DORENDER_H
#include "ricpp/baserenderer/dorender.h"
#endif // _RICPP_BASERENDERER_DORENDER_H

#ifndef _RICPP_RICPP_PARAMCLASSES_H
#include "ricpp/ricpp/paramclasses.h"
#endif // _RICPP_RICPP_PARAMCLASSES_H

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/ricpp/errorhandlers.h"
#endif // _RICPP_RICPP_ERRORHANDLERS_H

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/type.h"
#endif // _RICPP_RICPP_TYPES_H

#include <list>

namespace RiCPP {

///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls.
 */
class CRManInterfaceCall {
private:
	long m_lineNo; //!< Place to store the line number of a call in a RIB file, -1 if there is no line number.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Constructor, to set a line number (defaults to -1).
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 */
	inline CRManInterfaceCall(long aLineNo = -1): m_lineNo(aLineNo) {}

	/** @brief Copy constructor
	 *
	 *  @param c Object to copy
	 */
	inline CRManInterfaceCall(const CRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRManInterfaceCall() {}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_UNKNOWN;}

	/** @brief Replays the interface call.
	 *
	 *  @param ri The renderer backend used for replay.
	 *  @param cb Archive callback.
	 */
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.replayRequest(*this, cb);
	}

	/** @brief Asks for the line number.
	 *
	 *  @return The line number, -1 if there is none.
	 */
	inline long lineNo() const { return m_lineNo; }

	/** @brief Sets the line number.
	 *
	 *  @param aLineNo The new line number, -1 if there is none.
	 */
	inline void lineNo(long aLineNo) { m_lineNo=aLineNo; }

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRManInterfaceCall &operator=(const CRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		lineNo(c.lineNo());
		return *this;
	}

	/** @brief Pre-Processes an interface call
	 *
	 *  Pre-Processing normally involves state update of the renderer.
	 *  Is also performed before macro insertion.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Processes an interface call
	 *
	 *  Processing the interface call, e.g. do the rendering. This is
	 *  not called at a macro (object) definition.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for processing.
	 */
	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Post-Processes an interface call
	 *
	 *  Post-Processing the interface call, is called after
	 *  Macro insertion or processing.
	 *
	 *  @param cb Callback interface for comments.
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
	}

	/** @brief Pre-Processes an interface call
	 *
	 *  Pre-Processing normally involves state update of the renderer.
	 *  Is also performed before macro insertion.
	 *
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, 0);
	}

	/** @brief Processes an interface call
	 *
	 *  Processing the interface call, e.g. do the rendering. This is
	 *  not called at a macro (object) definition.
	 *
	 *  @param ri The renderer backend used for processing.
	 */
	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, 0);
	}

	/** @brief Post-Processes an interface call
	 *
	 *  Post-Processing the interface call, is called after
	 *  Macro insertion or processing.
	 *
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, 0);
	}

	/** @brief Test if interface call belongs to a macro definition
	 *
	 *  Macro definitions are not stored themselfes in macros.
	 *  (i.e. ObjectBegin, ObjectEnd, ArchiveBegin, Archive End)
	 *
	 *  @return true, if the request starts or ends a macro definition
	 */
	inline virtual bool isMacroDefinition()
	{
		return false;
	}
}; // CRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls with parameter lists.
 */
class CVarParamRManInterfaceCall : public CRManInterfaceCall {
private:
	CParameterList m_parameters;             //!< Parameters of an interface call.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CVarParamRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CVarParamRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to a kind of class atomizedClassName
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Constructor.
	 *
	 *  Constructs an empty parameter list, with line number, defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CVarParamRManInterfaceCall(long aLineNo=-1) :
		CRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CVarParamRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CRManInterfaceCall(aLineNo)
	{
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param parameters Parsed parameter list.
	 */
	inline CVarParamRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CRManInterfaceCall(aLineNo)
	{
		parameters(theParameters);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CVarParamRManInterfaceCall(const CVarParamRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CVarParamRManInterfaceCall() {}
	
	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CVarParamRManInterfaceCall(*this);
	}

	/** @brief Gets the size of tokens and parameters of the request.
	 *
	 * @return The size of tokens and parameters of the request.
	 */
	inline virtual CParameterList::size_type size() const
	{
		return m_parameters.size();
	}

	/** @brief Gets the tokens of the request.
	 *
	 * For the size of the array @see size(), for the parameter pointers @see getParams().
	 *
	 * @return The tokens of the request.
	 */
	inline RtToken *getTokens()
	{
		return m_parameters.tokenPtr();
	}

	/** @brief Gets the parameter pointers of the request.
	 *
	 * For the size of the array @see size(), for the tokenss @see getTokens().
	 *
	 * @return The parameters of the request.
	 */
	inline RtPointer *getParams()
	{
		return m_parameters.valuePtr();
	}

	/** @brief Gets a writeable reference of the parameter list.
	 *
	 *  @return Writeable reference of the parameter list interface.
	 */
	inline CParameterList &parameters()
	{
		return m_parameters;
	}

	/** @brief Gets a read only reference of the parameter list.
	 *
	 *  @return Read only reference of the parameter list interface.
	 */
	inline const CParameterList &parameters() const
	{
		return m_parameters;
	}

	/** @brief Fills the parameter list.
	 *
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline void setParams(
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		m_parameters.set(p, decl, curColorDescr, n, tokens, params);
	}
	
	/** @brief Fills the parameter list.
	 *
	 *  @param parameters Parsed parameter list.
	 */
	inline void parameters(const CParameterList &theParameters)
	{
		m_parameters = theParameters;
	}

	/** @brief Assignment.
	 *
	 *  @param c CVarParamRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CVarParamRManInterfaceCall &operator=(const CVarParamRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		parameters(c.parameters());
		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // TVarParamInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls for geometry.
 */
class CGeometryRManInterfaceCall : public CVarParamRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CGeometryRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CGeometryRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to a kind of class atomizedClassName
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CGeometryRManInterfaceCall(long aLineNo=-1) : CVarParamRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CGeometryRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CVarParamRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param parameters Parsed parameter list.
	 */
	inline CGeometryRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CGeometryRManInterfaceCall(const CGeometryRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CGeometryRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CGeometryRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CGeometryRManInterfaceCall &CGeometryRManInterfaceCall::operator=(const CGeometryRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CGeometryRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls based on UV Meshes (Splines, Quadrics).
 */
class CUVRManInterfaceCall : public CGeometryRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CUVRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CUVRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store.
	 */
	inline CUVRManInterfaceCall(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CUVRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CGeometryRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param parameters Parsed parameter list.
	 */
	inline CUVRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CGeometryRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CUVRManInterfaceCall(const CUVRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CUVRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CUVRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CUVRManInterfaceCall &CUVRManInterfaceCall::operator=(const CUVRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CUVRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Base class of all interface calls based on Polygon meshes.
 */
class CPolygonRManInterfaceCall : public CGeometryRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CPolygonRManInterfaceCall"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CPolygonRManInterfaceCall::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to specific class atomizedClassName.
	 */
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Checks if instance belongs to a kind of a class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call).
	 *  @return true, if instance belongs to a kind of class atomizedClassName.
	 */
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor. Empty parameter list, with line number defaults to -1.
	 *
	 *  @param aLineNo The line number to store..
	 */
	inline CPolygonRManInterfaceCall(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo) {}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CPolygonRManInterfaceCall(
		long aLineNo,
		CDeclarationDictionary &decl,
		const CParameterClasses &p,
		const CColorDescr &curColorDescr,
		RtInt n, RtToken tokens[], RtPointer params[]) :
		CGeometryRManInterfaceCall(aLineNo, decl, p, curColorDescr, n, tokens, params)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 * @param parameters Parsed parameter list.
	 */
	inline CPolygonRManInterfaceCall(
		long aLineNo,
		const CParameterList &theParameters) :
		CGeometryRManInterfaceCall(aLineNo, theParameters)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CPolygonRManInterfaceCall(const CPolygonRManInterfaceCall &c)
	{
		*this = c;
	}

	/** @brief Duplication.
	 * 
	 *  @return New instance as clone of this instance.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CPolygonRManInterfaceCall(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CPolygonRManInterfaceCall() {}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CPolygonRManInterfaceCall &CPolygonRManInterfaceCall::operator=(const CPolygonRManInterfaceCall &c)
	{
		if ( this == &c )
			return *this;

		CPolygonRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CPolygonRManInterfaceCall


///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container.
 */
class CRiMacro {
public:
	/** @brief Macro types
	 */
	enum EnumMacroTypes {
		MACROTYPE_UNKNOWN = 0, //!< Macro type is yet unknown
		MACROTYPE_OBJECT,      //!< Macro contains interface calls for an object definition
		MACROTYPE_ARCHIVE,     //!< Macro contains interface calls for an rib inline archive definition
		MACROTYPE_FILE         //!< Macro contains interface calls for an rib file archive definition
	};

private:
	std::list<CRManInterfaceCall *> m_callList;  //!< List of all interface calls for this macro.
	std::string m_name;         //!< Name of the macro (eg. file name, handle name).
	EnumMacroTypes m_macroType; //!< Type of macro, either object or archive.
	bool m_stopInsertion;       //!< Can be set to false to indicate to stop inserting request into the macro.
	bool m_isClosed;            //!< Set to true (close) if macro definition ended.
	bool m_postpone;            //!< Set to true if macro should be postponed (e.g. in RIB oputput).

public:

	/** @brief Constructor initializes the macro.
	 *
	 *  @param aName Name of the macro, eg. file name.
	 *  @param isObject Macro defines a geometric object.
	 */
	inline CRiMacro(
		RtString aName = 0,
		EnumMacroTypes macroType = MACROTYPE_UNKNOWN) :
		m_name(noNullStr(aName)), m_macroType(macroType), m_stopInsertion(false), m_isClosed(false), m_postpone(true)
	{
	}

	/** @brief Destructor, frees resources.
	 */
	virtual ~CRiMacro();

	/** @brief Gets the name of the macro.
	 *  @return The name of the macro.
	 */
	inline RtString name() const { return m_name.c_str(); }

	/** @brief Sets the name of the macro.
	 *
	 *  @param name The new name for the macro.
	 */
	inline void name(RtString aName)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Replays archive with callback for RIB archives at a back end renderer.
	 *
	 *  @param ri The back end renderer used to replay.
	 *  @param callback Used for archive callbacks (esp. for IRi::readArchive()).
	 */
	void replay(IDoRender &ri, const IArchiveCallback *callback);

	/** @brief Adds an interface call to the macro.
	 *
	 *  @param c Instance of an interface call.
	 *  @return true, if added and false, if not (because c == 0).
	 */
	bool add(CRManInterfaceCall *c);

	/** @brief Gets the validity of the macro.
	 *
	 *  @return true, macro is valid and request objects can be inserted.
	 */
	bool stopInsertion() const
	{
		return m_stopInsertion;
	}

	/** @brief Sets the validity of the macro.
	 *
	 *  @param isValid false, to indicate stopping further insertion of request
	 */
	void stopInsertion(bool stopIt)
	{
		m_stopInsertion = stopIt;
	}

	/** @brief Gets the open/closed state of a macro.
	 *
	 * A macro is closed, if all request are read and an End (Like IRi::objectEnd() ) was called.
	 * A closed macro can be instanciated.
	 *
	 *  @return true, if the macro is closed
	 */
	bool isClosed() const
	{
		return m_isClosed;
	}
	
	/** @brief Opens a macro for writing (adding requests).
	 *
	 * A macro is opened after created. @see isClosed()
	 */
	void open()
	{
		m_isClosed = false;
	}

	/** @brief Closes a macro.
	 *
	 * A macro is closed by the framework after called an End-Request. @see isClosed()
	 */
	void close()
	{
		m_isClosed = true;
	}

	/** @brief Postpone a macro instanciation (hint only).
	 *
	 * postpone() gives a hint, that the instanciation should be postponed. E.g.
	 * indicates, that a RIB writer should print a ReadArchive request instead
	 * of reading the macro and print the content. Also possible for ObjectInstance.
	 *
	 * @return true, if a macro instanciation is postponed (e.g. by a RIB writer).
	 */
	bool postpone() const
	{
		return m_postpone;
	}
	
	/** @brief Sets the postpone state.
	 *
	 * @see postpone()
	 *
	 * @param doPostpone true, if a macro instanciation should be postponed (e.g. by a RIB writer).
	 */
	void postpone(bool doPostpone)
	{
		m_postpone = doPostpone;
	}

	/** @brief Gets the type of the macro.
	 *
	 * Gets the type of the macro (EnumMacroTypes) for some checking.
	 *
	 * @return Type of the macro (EnumMacroTypes object, inline archive or archive file).
	 */
	EnumMacroTypes macroType() const
	{
		return m_macroType;
	}

	/** @brief Sets the type of the macro.
	 *
	 * @param aMacroType Type of the macro (EnumMacroTypes object, inline archive or archive file).
	 */
	void macroType(EnumMacroTypes aMacroType)
	{
		m_macroType = aMacroType;
	}
}; // CRiMacro


///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container for objects.
 */
class CRiObjectMacro : public CRiMacro {
private:
	RtObjectHandle m_handle; //!< Associated handle to identify the macro.
public:
	/** @brief Creates a macro container for an object (retained geometry).
	 *
	 * The type for the object macro is allways MACROTYPE_OBJECT
	 */
	inline CRiObjectMacro() : CRiMacro(0, MACROTYPE_OBJECT) { m_handle = illObjectHandle; }

	/** @brief Gets the associated handle of the object.
	 *
	 * @return The associated handle of the object.
	 */
	inline RtObjectHandle handle() const { return m_handle; }

	/** @brief Sets the associated handle of the object.
	 *
	 * @param h The associated handle of the object.
	 */
	inline void handle(RtObjectHandle h) { m_handle = h; }
};

///////////////////////////////////////////////////////////////////////////////
/** @brief Macro container for archives (file or inline).
 */
class CRiArchiveMacro : public CRiMacro {
private:
	RtArchiveHandle m_handle; //!< Associated handle to identify the macro.
public:
	/** @brief Creates a macro container for a RIB archive.
	 *
	 * @param name Filename or archive name
	 * @param aMacroType Type of the macro, will be MACROTYPE_ARCHIVE or MACROTYPE_FILE
	 */
	inline CRiArchiveMacro(const char *name = 0, EnumMacroTypes aMacroType=MACROTYPE_ARCHIVE) : CRiMacro(name, aMacroType)
	{
		m_handle = illArchiveHandle; 
 	}

	/** @brief Gets the associated handle of the archive.
	 *
	 * @return The associated handle of the archive.
	 */
	inline RtArchiveHandle handle() const { return m_handle; }

	/** @brief Sets the associated handle of the archive.
	 *
	 * @param h The associated handle of the archive.
	 */
	inline void handle(RtArchiveHandle h) { m_handle = h; }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Not suported: Frontend functions
// (REQ_ERROR_HANDLER - is a frontend funktion but has a class CRiErrorHandler
//  because Errorhandler is also a RIB request)
// REQ_BEGIN
// REQ_END
// REQ_CONTEXT
// REQ_GET_CONTEXT

///////////////////////////////////////////////////////////////////////////////
/** @brief Stores the content of a IRi::errorHandler() call.
 *
 *  Stores the setting of an error handler.
 */
class CRiErrorHandler : public CRManInterfaceCall {
private:
	IErrorHandler *m_handler; //!< Pointer to the error handler function

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRiErrorHandler"; }

	inline virtual const char *className() const { return CRiErrorHandler::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor, default handler is a CPrintErrorHandler.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
	{
		m_handler = new CPrintErrorHandler();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiErrorHandler", __LINE__, __FILE__);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts,
	 *                 -1 if there is no file.
	 *  @param handler Pointer to the error handler function that should be used.
	 *  @except ExceptRiCPPError Could not construct a handler.
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

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler(const CRiErrorHandler &c)
	{
		*this = c;
	}

	/** @brief Destructor, frees the resources.
	 */
	inline virtual ~CRiErrorHandler()
	{
		if ( m_handler )
			delete m_handler;
	}

	/** @brief Cloning.
	 *
	 *  @return a clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiErrorHandler(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ERROR_HANDLER; }

	/** @brief Gets the handler.
	 *
	 *  @return The currently assigned handler.
	 */
	inline const IErrorHandler &handler() const
	{
		assert(m_handler != 0);
		return *m_handler;
	}

	/** @brief Assigns a new handler.
	 *
	 *  Old handler is destroyed. If the new handler cannot be constructed, an
	 *  ExceptRiCPPError is thrown.
	 *
	 *  @param handler A clone of the handler will be created and assigned.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline void handler(const IErrorHandler &handler)
	{
		if ( m_handler )
			delete m_handler;
		m_handler = handler.duplicate();
		if ( !m_handler )
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"CRiErrorHandler::handler()", __LINE__, __FILE__);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cbi)
	{
		ri.preErrorHandler(*m_handler);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doErrorHandler(*m_handler);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postErrorHandler(*m_handler);
	}

	/** @brief Assignment.
	 *
	 *  @param c Object to assign.
	 *  @return A reference to this object.
	 *  @except ExceptRiCPPError Could not construct a handler.
	 */
	inline CRiErrorHandler &CRiErrorHandler::operator=(const CRiErrorHandler &c)
	{
		if ( this == &c )
			return *this;

		handler(c.handler());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiErrorHandler


///////////////////////////////////////////////////////////////////////////////
/** @brief Stores the content of a IRi::declare() call.
 *
 * A declaration of an interface variable is stored in an instance of this class.
 * The declaration is only stored here, not checked.
 */
class CRiDeclare : public CRManInterfaceCall {
private:
	RtToken m_name;            //!< Atomized name of the declarated variable
	std::string m_declaration; //!< The declaration, like: varying float[3]
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRiDeclare"; }

	inline virtual const char *className() const { return CRiDeclare::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @param name The atomized name of the declared variable.
	 *  @param declaration The declaration string.
	 */
	inline CRiDeclare(
		long aLineNo = -1,
		RtToken aName = 0, 
		RtString aDeclaration = 0)
		: CRManInterfaceCall(aLineNo),
		  m_name(aName),
		  m_declaration(noNullStr(aDeclaration))
	{}
	
	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDeclare(const CRiDeclare &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDeclare() {}

	/** @brief Cloning.
	 *
	 *  @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDeclare(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DECLARE; }

	/** @brief Gets the token of the name iof the declaration.
	 *
	 *  @return Token of the name of the declaration.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the token of the name iof the declaration.
	 *
	 *  @param name Token of the name of the declaration.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	/** @brief Gets the declaration string.
	 *
	 *  @return The declaration string.
	 */
	inline RtString declaration() const
	{
		return m_declaration.c_str();
	}

	/** @brief Sets (does not check) the declaration string.
	 *
	 *  @param aDeclaration The declaration string.
	 */
	inline void declaration(RtString aDeclaration)
	{
		m_declaration = noNullStr(aDeclaration);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cbi)
	{
		ri.preDeclare(name(), declaration(), false);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDeclare(name(), declaration());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDeclare(name(), declaration());
	}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CRiDeclare &CRiDeclare::operator=(const CRiDeclare &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());
		declaration(c.declaration());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDeclare


// ----------------------------------------------------------------------------
// Modes
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of a resource block
 */
class CRiResourceBegin : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiResourceBegin"; }

	inline virtual const char *className() const { return CRiResourceBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiResourceBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiResourceBegin(const CRiResourceBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiResourceBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiResourceBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE_BEGIN; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preResourceBegin();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doResourceBegin();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postResourceBegin();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiResourceBegin &CRiResourceBegin::operator=(const CRiResourceBegin &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiResourceBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of a resource block
 */
class CRiResourceEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiResourceEnd"; }

	inline virtual const char *className() const { return CRiResourceEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiResourceEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiResourceEnd(const CRiResourceEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiResourceEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiResourceEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preResourceEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doResourceEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postResourceEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiResourceEnd &CRiResourceEnd::operator=(const CRiResourceEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiResourceEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Resource handling
 */
class CRiResource : public CVarParamRManInterfaceCall {
private:
	std::string m_handle; //!< Handle id of a resource
	std::string m_type;   //!< Type of a resource

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiResource"; }

	inline virtual const char *className() const { return CRiResource::myClassName(); }


	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHandle Atomized handle (id) of the resource
	 *  @param aType Atomized type of the resource
	 */
	inline CRiResource(
		long aLineNo = -1,
		RtToken aHandle = 0,
		RtToken aType = 0
		)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_handle = noNullStr(aHandle);
		m_type = noNullStr(aType);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aHandle Atomized handle (id) of the resource
	 *  @param aType Atomized type of the resource
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiResource(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aHandle,
		RtToken aType,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_handle = aHandle;
		m_type = aType;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHandle Atomized handle (id) of the resource
	 *  @param aType Atomized type of the resource
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiResource(
		long aLineNo,
		RtToken aHandle,
		RtToken aType,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_handle = aHandle;
		m_type = aType;
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiResource(const CRiResource &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiResource()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiResource(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE; }

	/** @brief Gets the handle of the resource.
	 *
	 *  @return The atomized handle of the resource.
	 */
	inline RtString handle() const
	{
		return m_handle.c_str();
	}

	/** @brief Sets the handle of the resource.
	 *
	 *  @param An atomized handle of a resource.
	 */
	inline void handle(RtString aHandle)
	{
		m_handle = noNullStr(aHandle);
	}

	/** @brief Gets the type of the resource.
	 *
	 *  @return The atomized type of the resource.
	 */
	inline RtString type() const
	{
		return m_type.c_str();
	}

	/** @brief Sets the type of the resource.
	 *
	 *  @param An atomized type of a resource.
	 */
	inline void type(RtString aType)
	{
		m_type = noNullStr(aType);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preResource(handle(), type(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doResource(handle(), type(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postResource(handle(), type(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiResource &operator=(const CRiResource &c)
	{
		if ( this == &c )
			return *this;

		handle(c.handle());
		type(c.type());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiResource

///////////////////////////////////////////////////////////////////////////////
/** @brief Starts a frame block.
 */
class CRiFrameBegin : public CRManInterfaceCall {
	RtInt m_frameNumber; //!< A number to identify the frame.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRiFrameBegin"; }

	inline virtual const char *className() const { return CRiFrameBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 *  @param number A frame number.
	 */
	inline CRiFrameBegin(long aLineNo=-1, RtInt number=0)
		: CRManInterfaceCall(aLineNo), m_frameNumber(number)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiFrameBegin(const CRiFrameBegin &c)
	{
		*this = c;
	}

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameBegin(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFrameBegin()
	{
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_BEGIN; }

	/** @brief Gets the frame number.
	 *
	 *  @return The frame number.
	 */
	inline RtInt frameNumber() const
	{
		return m_frameNumber;
	}

	/** @brief Sets a frame number.
	 *
	 *  @param number A frame number.
	 */
	inline void frameNumber(RtInt number)
	{
		m_frameNumber = number;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFrameBegin(m_frameNumber);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFrameBegin(m_frameNumber);
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFrameBegin(m_frameNumber);
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiFrameBegin &CRiFrameBegin::operator=(const CRiFrameBegin &c)
	{
		if ( this == &c )
			return *this;

		frameNumber(c.frameNumber());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFrameBegin


///////////////////////////////////////////////////////////////////////////////
/** @brief Ends a frame block.
 */
class CRiFrameEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRiFrameEnd"; }

	inline virtual const char *className() const { return CRiFrameEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
	 */
	inline CRiFrameEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor
	 *
	 *  @param c Object to copy
	 */
	inline CRiFrameEnd(const CRiFrameEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFrameEnd()
	{
	}

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFrameEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFrameEnd();
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFrameEnd();
	}

	/** @brief Assignment.
	 *
	 * @param c Object to assign.
	 * @return A reference to this object.
	 */
	inline CRiFrameEnd &CRiFrameEnd::operator=(const CRiFrameEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFrameEnd


///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of a world block.
 *
 *  \sa CRManInterfaceCall
 *  \sa IRi::worldBegin()
 */
class CRiWorldBegin : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiWorldBegin"; }

	inline virtual const char *className() const { return CRiWorldBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiWorldBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiWorldBegin(const CRiWorldBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiWorldBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiWorldBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_BEGIN; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preWorldBegin();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doWorldBegin();
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postWorldBegin();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiWorldBegin &CRiWorldBegin::operator=(const CRiWorldBegin &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiWorldBegin


///////////////////////////////////////////////////////////////////////////////
/** @brief End of a world block.
 *
 *  \sa CRManInterfaceCall
 *  \sa IRi::worldEnd()
 */
class CRiWorldEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiWorldEnd"; }

	inline virtual const char *className() const { return CRiWorldEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiWorldEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiWorldEnd(const CRiWorldEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiWorldEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiWorldEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preWorldEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doWorldEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postWorldEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiWorldEnd &CRiWorldEnd::operator=(const CRiWorldEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiWorldEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of an attribute block
 */
class CRiAttributeBegin : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiAttributeBegin"; }

	inline virtual const char *className() const { return CRiAttributeBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiAttributeBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiAttributeBegin(const CRiAttributeBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiAttributeBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiAttributeBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_BEGIN; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAttributeBegin();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doAttributeBegin();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postAttributeBegin();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiAttributeBegin &CRiAttributeBegin::operator=(const CRiAttributeBegin &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAttributeBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of an attribute block
 */
class CRiAttributeEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiAttributeEnd"; }

	inline virtual const char *className() const { return CRiAttributeEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiAttributeEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiAttributeEnd(const CRiAttributeEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiAttributeEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiAttributeEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAttributeEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doAttributeEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postAttributeEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiAttributeEnd &CRiAttributeEnd::operator=(const CRiAttributeEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAttributeEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of a transformation block
 */
class CRiTransformBegin : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiTransformBegin"; }

	inline virtual const char *className() const { return CRiTransformBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiTransformBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiTransformBegin(const CRiTransformBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiTransformBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTransformBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_BEGIN; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransformBegin();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTransformBegin();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTransformBegin();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiTransformBegin &CRiTransformBegin::operator=(const CRiTransformBegin &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTransformBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of a transformation block
 */
class CRiTransformEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiTransformEnd"; }

	inline virtual const char *className() const { return CRiTransformEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiTransformEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy
	 */
	inline CRiTransformEnd(const CRiTransformEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiTransformEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTransformEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransformEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTransformEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTransformEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiTransformEnd &CRiTransformEnd::operator=(const CRiTransformEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTransformEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of a solid block.
 */
class CRiSolidBegin : public CRManInterfaceCall {
private:
	RtToken m_type; //!< Token that indicates the solid operation (like RI_UNION), must be atomized (@see CToken)

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiSolidBegin"; }

	inline virtual const char *className() const { return CRiSolidBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file.
	 *  @param anOperation The token of the operation (must be an atomized string, @see CToken).
	 */
	inline CRiSolidBegin(long aLineNo=-1, RtToken aType=RI_PRIMITIVE) : CRManInterfaceCall(aLineNo), m_type(aType) { }

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiSolidBegin(const CRiSolidBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiSolidBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSolidBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_BEGIN; }

	/** @brief Gets the solid operation.
	 * @return The token of the operation (must be an atomized string, @see CToken)
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets solid operation.
	 * @param anOperation The token of the operation (must be an atomized string, @see CToken)
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSolidBegin(m_type);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSolidBegin(m_type);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSolidBegin(m_type);
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiSolidBegin &CRiSolidBegin::operator=(const CRiSolidBegin &c)
	{
		if ( this == &c )
			return *this;

		type(c.type());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSolidBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of a solid block.
 */
class CRiSolidEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiSolidEnd"; }

	inline virtual const char *className() const { return CRiSolidEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file.
	 */
	inline CRiSolidEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiSolidEnd(const CRiSolidEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiSolidEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSolidEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSolidEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSolidEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSolidEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiSolidEnd &CRiSolidEnd::operator=(const CRiSolidEnd &c)
	{
		if ( this == &c )
			return *this;

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSolidEnd


///////////////////////////////////////////////////////////////////////////////
/** @brief Starts an object block of retained geometry.
 *
 *  CRiObjectBegin requests are used to control macro definitions and
 *  instances are not stored at a macro themselves.
 */
class CRiObjectBegin : public CRManInterfaceCall {
private:
	RtObjectHandle m_handle; //!< Handle used to identify the object

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void)
	{
		return "CRiObjectBegin";
	}

	inline virtual const char *className() const
	{
		return CRiObjectBegin::myClassName();
	}
	
	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}
	
	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiObjectBegin(long aLineNo = -1) : CRManInterfaceCall(aLineNo)
	{
		m_handle = illObjectHandle;
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiObjectBegin(const CRiSolidEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiObjectBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiObjectBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_BEGIN; }

	/** Gets the handle of the object.
	 *
	 * @return Handle of the object.
	 */
	inline virtual RtObjectHandle handle() const { return m_handle; }

	/** Sets the handle of the object.
	 *
	 * The handle is generated by a IDoRender::preObjectBegin() implementation
	 * using a TemplHandleStack.
	 *
	 * @param aHandle Handle of the object.
	 */
	inline virtual void handle(RtObjectHandle aHandle) { m_handle = aHandle; }
	
	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		handle(ri.preObjectBegin());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doObjectBegin(handle());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postObjectBegin(handle());
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiObjectBegin &operator=(const CRiObjectBegin &c)
	{
		if ( this == &c )
			return *this;
		
		handle(c.handle());

		CRManInterfaceCall::operator=(c);
		return *this;
	}

	inline virtual bool isMacroDefinition()
	{
		return true;
	}
}; // CRiObjectBegin


///////////////////////////////////////////////////////////////////////////////
/** @brief Ends an object block of retained geometry.
 *
 *  CRiObjectEnd requests are used to control macro definitions and
 *  instances are not stored at a macro themselves.
 */
class CRiObjectEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiObjectEnd"; }

	inline virtual const char *className() const { return CRiObjectEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiObjectEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiObjectEnd(const CRiObjectEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiObjectEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiObjectEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preObjectEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doObjectEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postObjectEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiObjectEnd &operator=(const CRiObjectEnd &c)
	{
		if ( this == &c )
			return *this;
		CRManInterfaceCall::operator=(c);
		return *this;
	}

	inline virtual bool isMacroDefinition()
	{
		return true;
	}
}; // CRiObjectEnd


///////////////////////////////////////////////////////////////////////////////
/** @brief Instanciate an object.
 */
class CRiObjectInstance : public CRManInterfaceCall {
private:
	RtObjectHandle m_handle; //!< Object handle
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiObjectInstance"; }

	inline virtual const char *className() const { return CRiObjectInstance::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHandle Handle to identify the object
	 */
	inline CRiObjectInstance(
		long aLineNo=-1,
		RtObjectHandle aHandle=illObjectHandle)
		: CRManInterfaceCall(aLineNo), m_handle(aHandle)
	{
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiObjectInstance(const CRiObjectInstance &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiObjectInstance() {}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiObjectInstance(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_INSTANCE; }

	/** @brief Gets the object handle.
	 */
	inline virtual RtObjectHandle handle() const { return m_handle; }

	/** @brief Sets the object handle.
	 *
	 *  @param h An object handle to identify the object.
	 */
	inline virtual void handle(RtObjectHandle h) { m_handle = h; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preObjectInstance(handle());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doObjectInstance(handle());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postObjectInstance(handle());
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiObjectInstance &operator=(const CRiObjectInstance &c)
	{
		if ( this == &c )
			return *this;

		handle(c.handle());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiObjectInstance


///////////////////////////////////////////////////////////////////////////////
/** @brief Begins the block of an archive.
 *
 *  CRiArchiveBegin requests are used to control macro definitions and
 *  instances are not stored at a macro themselves.
 */
class CRiArchiveBegin : public CVarParamRManInterfaceCall {
private:
	RtArchiveHandle m_handle; //!< Associated archive handle to identify the object (constructed by IDoRender::preArchiveBegin()).
	std::string m_name;       //!< Name of the archive, used to generate the handle.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiArchiveBegin"; }

	inline virtual const char *className() const { return CRiArchiveBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
	 */
	inline CRiArchiveBegin(long aLineNo = -1, const char *aName = 0)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_handle = illObjectHandle;
		m_name = noNullStr(aName);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiArchiveBegin(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
	 *  @param theParameters Parameter values of the request.
	 */
	inline CRiArchiveBegin(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiArchiveBegin(const CRiArchiveBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiArchiveBegin()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiArchiveBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_BEGIN; }

	/** Gets the handle of the archive.
	 *
	 * @return Handle of the archive.
	 */
	inline virtual RtArchiveHandle handle() const { return m_handle; }

	/** Sets the handle of the archive.
	 *
	 * The handle is generated by a IDoRender::preArchiveBegin() implementation
	 * using a TemplHandleStack.
	 *
	 * @param aHandle Handle of the archive.
	 */
	inline virtual void handle(RtArchiveHandle aHandle) { m_handle = aHandle; }
	
	/** Gets the name of the archive.
	 *
	 * @return Name of the archive.
	 */
	inline virtual RtString name() const { return m_name.c_str(); }

	/** Sets the name of the archive.
	 *
	 * The name is set by the constructor and usually not changed. The name
	 * is used to create the handle. An archive can overwrite another one with the same name.
	 *
	 * @param aName Name of the archive.
	 */
	inline virtual void name(const char *aName) { m_name = noNullStr(aName); }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		handle(ri.preArchiveBegin(name(), parameters()));
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doArchiveBegin(handle(), name(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postArchiveBegin(handle(), name(), parameters());
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiArchiveBegin &operator=(const CRiArchiveBegin &c)
	{
		if ( this == &c )
			return *this;
		
		handle(c.handle());
		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}

	inline virtual bool isMacroDefinition()
	{
		return true;
	}
}; // CRiArchiveBegin


///////////////////////////////////////////////////////////////////////////////
/** @brief Ends the block of an archive.
 *
 *  CRiArchiveEnd requests are used to control macro definitions and
 *  instances are not stored at a macro themselves.
 */
class CRiArchiveEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiArchiveEnd"; }

	inline virtual const char *className() const { return CRiArchiveEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiArchiveEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiArchiveEnd(const CRiArchiveEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiArchiveEnd()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiArchiveEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preArchiveEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doArchiveEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postArchiveEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiArchiveEnd &operator=(const CRiArchiveEnd &c)
	{
		if ( this == &c )
			return *this;
		CRManInterfaceCall::operator=(c);
		return *this;
	}

	inline virtual bool isMacroDefinition()
	{
		return true;
	}
}; // CRiArchiveEnd


///////////////////////////////////////////////////////////////////////////////
/** @brief Start of a motion block.
 *
 *  @todo Count the time stamps within a motion block at the render state.
 */
class CRiMotionBegin : public CRManInterfaceCall {
private:
	std::vector<RtFloat> m_motionVars; //!< The motion "time stamps"

public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMotionBegin"; }

	inline virtual const char *className() const { return CRiMotionBegin::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMotionBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  Start of a motion block. The motion block contains n requests. Each requests is associated with a
	 *  time sample using the vector f of "time stamps".
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
	 *  @param n Number of "time stamps".
	 *  @param Vector of "time stamps", size is @a n.
	 */
	inline CRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) :
		CRManInterfaceCall(aLineNo)
	{
		set(n, f);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMotionBegin(const CRiMotionBegin &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiMotionBegin()
	{
	}
	
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMotionBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_BEGIN; }

	/** Gets a reference to the vector with the motion vars.
	 *
	 * @return A reference to the vector with the motion vars.
	 */
	inline std::vector<RtFloat> &motionVars()
	{
		return m_motionVars;
	}

	/** Gets a read-only reference to the vector with the motion vars.
	 *
	 * @return A read-only reference to the vector with the motion vars.
	 */
	inline const std::vector<RtFloat> &motionVars() const
	{
		return m_motionVars;
	}

	/** Gets a copy of the vector with the motion vars.
	 *
	 * @retval A reference to a vector to retrieve the motion vars.
	 */
	inline void get(std::vector<RtFloat> &m) const
	{
		m = m_motionVars;
	}

	/** Sets the vector with the motion vars.
	 *
	 * @param A reference to a vector with motion vars.
	 */
	inline void set(const std::vector<RtFloat> m)
	{
		m_motionVars = m;
	}

	/** Sets the vector with the motion vars.
	 *
	 *  @param n Number of "time stamps".
	 *  @param Vector of "time stamps", size is @a n.
	 */
	inline void set(RtInt n, const RtFloat *f)
	{
		m_motionVars.resize(0);
		if ( n > 0 ) {
			m_motionVars.resize(n);
			m_motionVars.assign(f, f+n);
		}
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMotionBegin(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doMotionBegin(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postMotionBegin(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiMotionBegin &operator=(const CRiMotionBegin &c)
	{
		if ( this == &c )
			return *this;

		set(c.motionVars());

		CRManInterfaceCall::operator=(c);
	 	return *this;
	}
}; // CRiMotionBegin


///////////////////////////////////////////////////////////////////////////////
/** @brief End of a motion block.
 */
class CRiMotionEnd : public CRManInterfaceCall {
public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMotionEnd"; }

	inline virtual const char *className() const { return CRiMotionEnd::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiMotionEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiMotionEnd(const CRiMotionEnd &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiMotionEnd()
	{
	}
	
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMotionEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_END; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMotionEnd();
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doMotionEnd();
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postMotionEnd();
	}

	/** @brief Assignment
	 *
	 * @param c Object to assign
	 * @return A reference to this object
	 */
	inline CRiMotionEnd &operator=(const CRiMotionEnd &c)
	{
		if ( this == &c )
			return *this;
		CRManInterfaceCall::operator=(c);
	 	return *this;
	}
}; // CRiMotionEnd


// ----------------------------------------------------------------------------
// Options
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief The raster resolution and pixel aspect ratio of the image to be rendered.
 */
class CRiFormat : public CRManInterfaceCall {
private:
	RtInt m_xres,     //!< Width of the image to be rendered.
	      m_yres;     //!< Height of the image to be rendered.
	RtFloat m_aspect; //!< Pixel aspect ratio (width to the height).

public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFormat"; }

	inline virtual const char *className() const { return CRiFormat::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline CRiFormat(
		long aLineNo = -1,
		RtInt aXRes = defXResolution,
		RtInt aYRes = defYResolution,
		RtFloat anAspect = defPixelAspectRatio) :
		CRManInterfaceCall(aLineNo), m_xres(aXRes), m_yres(aYRes), m_aspect(anAspect)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiFormat(const CRiFormat &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFormat()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFormat(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FORMAT; }

	/** @brief Gets the x (horizontal) resolution of the image.
	 *
	 *  @return The x (horizontal) resolution of the image to be rendered.
	 */
	inline RtInt xres() const
	{
		return m_xres;
	}

	/** @brief Sets the x (horizontal) resolution of the image.
	 *
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 */
	inline void xres(RtInt aXRes)
	{
		m_xres = aXRes;
	}

	/** @brief Gets the y (vertical) resolution of the image.
	 *
	 *  @return The y (vertical) resolution of the image to be rendered.
	 */
	inline RtInt yres() const
	{
		return m_yres;
	}

	/** @brief Sets the y (vertical) resolution of the image.
	 *
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 */
	inline void yres(RtInt aYRes)
	{
		m_yres = aYRes;
	}

	/** @brief Gets the pixel aspect ratio of the image.
	 *
	 * The pixel aspect ration matches a physical display.
	 * The viewport aspect ratio is calculated by (xres() * aspect())/yres()
	 *
	 *  @return The pixel aspect ratio of the image to be rendered.
	 */
	inline RtFloat aspect() const
	{
		return m_aspect;
	}

	/** @brief Sets the pixel aspect ratio of the image.
	 *
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void aspect(RtFloat anAspect)
	{
		m_aspect = anAspect;
	}

	/** @brief Gets the values of the screen format.
	 *
	 *  @retval aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @retval aYRes The y (vertical) resolution of the image to be rendered.
	 *  @retval anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void set(
		RtInt &aXRes,
		RtInt &aYRes,
		RtFloat &anAspect) const
	{
		aXRes = m_xres;
		aYRes = m_yres;
		anAspect = m_aspect;
	}

	/** @brief Sets the values of the screen format.
	 *
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void set(
		RtInt aXRes,
		RtInt aYRes,
		RtFloat anAspect)
	{
		m_xres = aXRes;
		m_yres = aYRes;
		m_aspect = anAspect;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFormat(m_xres, m_yres, m_aspect);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFormat(m_xres, m_yres, m_aspect);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFormat(m_xres, m_yres, m_aspect);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiFormat &operator=(const CRiFormat &c)
	{
		if ( this == &c )
			return *this;

		set(c.xres(), c.yres(), c.aspect());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFormat


///////////////////////////////////////////////////////////////////////////////
/** @brief Frame aspect ratio of the image to be rendered
 *
 * If the frame aspect ration do not match the setting made by calling a IRi::display()
 * and IRi::format(), die image should be aligned with the upper left corner (origin)
 * of the image so that the longer edge of the image fills the appropriate edge of the display.
 *
 * If a IRi::frameAspectRatio() is not called, the frame aspect is calculated by the settings
 * made by IRi::format().
 */
class CRiFrameAspectRatio : public CRManInterfaceCall {
private:
	RtFloat m_aspect; //!< frame aspect ratio

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFrameAspectRatio"; }

	inline virtual const char *className() const { return CRiFrameAspectRatio::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param anAspect The frame aspect ratio of the image to be rendered.
	 */
	inline CRiFrameAspectRatio(
		long aLineNo = -1,
		RtFloat aspect = defFrameAspectRatio) :
		CRManInterfaceCall(aLineNo), m_aspect(aspect)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiFrameAspectRatio(const CRiFrameAspectRatio &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFrameAspectRatio()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameAspectRatio(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_ASPECT_RATIO; }
	
	/** @brief Gets the frame aspect ratio of the image.
	 *
	 *  @return The frame aspect ratio of the image to be rendered.
	 */
	inline RtFloat aspect() const
	{
		return m_aspect;
	}

	/** @brief Sets the frame aspect ratio of the image.
	 *
	 *  @param anAspect The frame aspect ratio of the image to be rendered.
	 */
	inline void aspect(RtFloat anAspect)
	{
		m_aspect = anAspect;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFrameAspectRatio(m_aspect);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFrameAspectRatio(m_aspect);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFrameAspectRatio(m_aspect);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiFrameAspectRatio &operator=(const CRiFrameAspectRatio &c)
	{
		if ( this == &c )
			return *this;

		aspect(c.aspect());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFrameAspectRatio

///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the size of the screen window (in screen coordinates)
 *
 *  Has to match the frame aspect ratio (frameaspect = abs(right-left)/abs(top-bot))
 *  to avoid distortion at the displayed image. If the
 *  IRi::screenWindow() is nor called, the values default to
 *  frameaspectratio >= 1: (-frameaspectratio, frameaspectratio, -1, 1), and
 *  frameaspectratio < 1: (-1, 1, -1/frameaspectratio, 1/frameaspectratio).
 *  For perspective projection the smaller range will be centered at the displayed
 *  image.
 */
class CRiScreenWindow : public CRManInterfaceCall {
private:
	RtFloat m_left,   //!< coordinate of the left screen window edge
	        m_right,  //!< coordinate of the right screen window edge
	        m_bottom, //!< coordinate of the bottom screen window edge
	        m_top;    //!< coordinate of the top screen window edge

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiScreenWindow"; }

	inline virtual const char *className() const { return CRiScreenWindow::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aLeft The left edge of the screen window (screen coordinates)
	 *  @param aRight The right edge of the screen window (screen coordinates)
	 *  @param aBot The bottom edge of the screen window (screen coordinates)
	 *  @param aTop The top edge of the screen window (screen coordinates)
	 */
	inline CRiScreenWindow(
		long aLineNo = -1,
		RtFloat aLeft = defScreenWindowLeft,
		RtFloat aRight = defScreenWindowRight,
		RtFloat aBottom = defScreenWindowBottom,
		RtFloat aTop = defScreenWindowTop ) :
		CRManInterfaceCall(aLineNo),
		m_left(aLeft), m_right(aRight), m_bottom(aBottom), m_top(aTop)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiScreenWindow(const CRiScreenWindow &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiScreenWindow()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiScreenWindow(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SCREEN_WINDOW; }

	/** @brief Gets the left edge.
	 *
	 *  @return The left edge of the screen window (screen coordinates).
	 */
	inline RtFloat left() const
	{
		return m_left;
	}

	/** @brief Sets the left edge.
	 *
	 *  @param aLeft The left edge of the screen window (screen coordinates).
	 */
	inline void left(RtFloat aLeft)
	{
		m_left = aLeft;
	}

	/** @brief Gets the left edge.
	 *
	 *  @return The right edge of the screen window (screen coordinates).
	 */
	inline RtFloat right() const
	{
		return m_right;
	}

	/** @brief Sets the right edge.
	 *
	 *  @param aRight The right edge of the screen window (screen coordinates).
	 */
	inline void right(RtFloat aRight)
	{
		m_right = aRight;
	}

	/** @brief Gets the bottom edge.
	 *
	 *  @return The bottom edge of the screen window (screen coordinates).
	 */
	inline RtFloat bottom() const
	{
		return m_bottom;
	}

	/** @brief Sets The bottom edge.
	 *
	 *  @param aBot The bottom edge of the screen window (screen coordinates).
	 */
	inline void bottom(RtFloat aBottom)
	{
		m_bottom = aBottom;
	}

	/** @brief Gets the top edge.
	 *
	 *  @return The top edge of the screen window (screen coordinates).
	 */
	inline RtFloat top() const
	{
		return m_top;
	}

	/** @brief Sets The top edge.
	 *
	 *  @param aTop The top edge of the screen window (screen coordinates).
	 */
	inline void top(RtFloat aTop)
	{
		m_top = aTop;
	}

	/** @brief Gets the values of the screen window.
	 *
	 *  @retval aLeft The left edge of the screen window (screen coordinates)
	 *  @retval aRight The right edge of the screen window (screen coordinates)
	 *  @retval aBot The bottom edge of the screen window (screen coordinates)
	 *  @retval aTop The top edge of the screen window (screen coordinates)
	 */
	inline void get(
		RtFloat &aLeft,
		RtFloat &aRight,
		RtFloat &aBottom,
		RtFloat &aTop) const
	{
		aLeft = m_left;
		aRight = m_right;
		aBottom = m_bottom;
		aTop = m_top;
	}

	/** @brief Sets the values of the screen window.
	 *
	 *  @param aLeft The left edge of the screen window (screen coordinates)
	 *  @param aRight The right edge of the screen window (screen coordinates)
	 *  @param aBot The bottom edge of the screen window (screen coordinates)
	 *  @param aTop The top edge of the screen window (screen coordinates)
	 */
	inline void set(
		RtFloat aLeft,
		RtFloat aRight,
		RtFloat aBottom,
		RtFloat aTop)
	{
		m_left = aLeft;
		m_right = aRight;
		m_bottom = aBottom;
		m_top = aTop;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiScreenWindow &operator=(const CRiScreenWindow &c)
	{
		if ( this == &c )
			return *this;

		set(c.left(), c.right(), c.bottom(), c.top());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiScreenWindow


///////////////////////////////////////////////////////////////////////////////
/** @brief Give a rectangular cropped region of the image in screen coordinates.
 */
class CRiCropWindow : public CRManInterfaceCall {
private:
	RtFloat m_xmin,  //!< Crop window minimal x
	        m_xmax,  //!< Crop window maximal x
	        m_ymin,  //!< Crop window minimal y
	        m_ymax;  //!< Crop window maximal y

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiCropWindow"; }

	inline virtual const char *className() const { return CRiCropWindow::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXmin The left edge of the crop window (screen coordinates)
	 *  @param aXmax The right edge of the crop window (screen coordinates)
	 *  @param aYmin The bottom edge of the crop window (screen coordinates)
	 *  @param aYmax The top edge of the crop window (screen coordinates)
	 */
	inline CRiCropWindow(
		long aLineNo = -1,
		RtFloat aXmin = defCropWindowLeft,
		RtFloat aXmax = defCropWindowRight,
		RtFloat aYmin = defScreenWindowBottom,
		RtFloat aYmax = defScreenWindowTop )
		: CRManInterfaceCall(aLineNo), m_xmin(aXmin), m_xmax(aXmax), m_ymin(aYmin), m_ymax(aYmax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiCropWindow(const CRiCropWindow &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiCropWindow()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiCropWindow(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CROP_WINDOW; }


	/** @brief Gets the left edge.
	 *
	 *  @return The left edge of the crop window (screen coordinates).
	 */
	inline RtFloat xmin() const
	{
		return m_xmin;
	}

	/** @brief Sets the left edge.
	 *
	 *  @param aXmin The left edge of the crop window (screen coordinates).
	 */
	inline void xmin(RtFloat aXmin)
	{
		m_xmin = aXmin;
	}

	/** @brief Gets the left edge.
	 *
	 *  @return The right edge of the crop window (screen coordinates).
	 */
	inline RtFloat xmax() const
	{
		return m_xmax;
	}

	/** @brief Sets the right edge.
	 *
	 *  @param aXmax The right edge of the crop window (screen coordinates).
	 */
	inline void xmax(RtFloat aXmax)
	{
		m_xmax = aXmax;
	}

	/** @brief Gets the bottom edge.
	 *
	 *  @return The bottom edge of the crop window (screen coordinates).
	 */
	inline RtFloat ymin() const
	{
		return m_ymin;
	}

	/** @brief Sets The bottom edge.
	 *
	 *  @param aYmin The bottom edge of the crop window (screen coordinates).
	 */
	inline void ymin(RtFloat aYmin)
	{
		m_ymin = aYmin;
	}

	/** @brief Gets the top edge.
	 *
	 *  @return The top edge of the crop window (screen coordinates).
	 */
	inline RtFloat ymax() const
	{
		return m_ymax;
	}

	/** @brief Sets The top edge.
	 *
	 *  @param aYmax The top edge of the crop window (screen coordinates).
	 */
	inline void ymax(RtFloat aYmax)
	{
		m_ymax = aYmax;
	}

	/** @brief Gets the values of a crop window.
	 *
	 *  @retval aXmin The left edge of the crop window (screen coordinates)
	 *  @retval aXmax The right edge of the crop window (screen coordinates)
	 *  @retval aYmin The bottom edge of the crop window (screen coordinates)
	 *  @retval aYmax The top edge of the crop window (screen coordinates)
	 */
	inline void set(
		RtFloat &aXmin,
		RtFloat &aXmax,
		RtFloat &aYmin,
		RtFloat &aYmax) const
	{
		aXmin = m_xmin;
		aXmax = m_xmax;
		aYmin = m_ymin;
		aYmax = m_ymax;
	}

	/** @brief Sets the values of a crop window.
	 *
	 *  @param aXmin The left edge of the crop window (screen coordinates)
	 *  @param aXmax The right edge of the crop window (screen coordinates)
	 *  @param aYmin The bottom edge of the crop window (screen coordinates)
	 *  @param aYmax The top edge of the crop window (screen coordinates)
	 */
	inline void set(
		RtFloat aXmin,
		RtFloat aXmax,
		RtFloat aYmin,
		RtFloat aYmax)
	{
		m_xmin = aXmin;
		m_xmax = aXmax;
		m_ymin = aYmin;
		m_ymax = aYmax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline CRiCropWindow &operator=(const CRiCropWindow &c)
	{
		if ( this == &c )
			return *this;

		set(c.xmin(), c.xmax(), c.ymin(), c.ymax());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiCropWindow


///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the projection type of the camera.
 */
class CRiProjection : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Atomized projection name (RI_PERSPECTIVE, RI_ORTHOGRAPHIC)

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiProjection"; }

	inline virtual const char *className() const { return CRiProjection::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param name Atomized name of the projection
	 */
	inline CRiProjection(
		long aLineNo = -1,
		RtToken aName = defProjection)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Atomized name of the projection
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiProjection(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 * @param aName Atomized name of the projection
	 * @param parameters Parsed parameter list.
	 */
	inline CRiProjection(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_name = aName;
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiProjection(const CRiProjection &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiProjection()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiProjection(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PROJECTION; }

	/** @brief Gets the name of the projection.
	 *
	 *  @return The atomized name of the projection.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the projection.
	 *
	 *  @param An atomized name of a projection.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preProjection(name(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doProjection(name(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postProjection(name(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiProjection &operator=(const CRiProjection &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiProjection


///////////////////////////////////////////////////////////////////////////////
/** @brief The near/far camera clipping planes.
 */
class CRiClipping : public CRManInterfaceCall {
private:
	RtFloat m_hither, //!< Near clipping plane
	        m_yon;    //!< Far clipping plane

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiClipping"; }

	inline virtual const char *className() const { return CRiClipping::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param hither Near clipping plane
	 *  @param yon Far clipping plane
	 */
	inline CRiClipping(long aLineNo=-1, RtFloat aHither=defNearClip, RtFloat aYon=defFarClip)
		: CRManInterfaceCall(aLineNo), m_hither(aHither), m_yon(aYon)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiClipping(const CRiClipping &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiClipping()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiClipping(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING; }

	/** @brief Gets the near clipping plane for the camera.
	 *
	 *  @return The near clipping plane for the camera.
	 */
	inline RtFloat hither() const
	{
		return m_hither;
	}

	/** @brief Sets the near clipping plane for the camera.
	 *
	 *  @param aHither Near clipping plane for the camera.
	 */
	inline void hither(RtFloat aHither)
	{
		m_hither = aHither;
	}

	/** @brief Gets the far clipping plane for the camera.
	 *
	 *  @return The far clipping plane for the camera.
	 */
	inline RtFloat yon() const
	{
		return m_yon;
	}

	/** @brief Sets the far clipping plane for the camera.
	 *
	 *  @param aYon Far clipping plane for the camera.
	 */
	inline void yon(RtFloat aYon)
	{
		m_yon = aYon;
	}

	/** @brief Gets the values of the camera clipping planes.
	 *
	 *  @retval hither Near clipping plane
	 *  @retval yon Far clipping plane
	 */
	inline void get(RtFloat &aHither, RtFloat &aYon) const
	{
		aHither = m_hither;
		aYon = m_yon;
	}

	/** @brief Sets the values of the camera clipping planes.
	 *
	 *  @param hither Near clipping plane
	 *  @param yon Far clipping plane
	 */
	inline void set(RtFloat aHither, RtFloat aYon)
	{
		m_hither = aHither;
		m_yon = aYon;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preClipping(m_hither, m_yon);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doClipping(m_hither, m_yon);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postClipping(m_hither, m_yon);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiClipping &operator=(const CRiClipping &c)
	{
		if ( this == &c )
			return *this;

		set(c.hither(), c.yon());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiClipping

///////////////////////////////////////////////////////////////////////////////
/** @brief Additional clipping plane.
 */
class CRiClippingPlane : public CRManInterfaceCall {
private:
	RtFloat m_x,  //!< Point of an additional clipping plane (x coordinate).
	        m_y,  //!< Point of an additional clipping plane (y coordinate).
	        m_z,  //!< Point of an additional clipping plane (z coordinate).
	        m_nx, //!< Normal of an additional clipping plane (x coordinate).
	        m_ny, //!< Normal of an additional clipping plane (y coordinate).
	        m_nz; //!< Normal of an additional clipping plane (z coordinate).

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiClippingPlane"; }

	inline virtual const char *className() const { return CRiClippingPlane::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param x x component of a point at the clipping plane.
	 *  @param y y component of a point at the clipping plane.
	 *  @param z z component of a point at the clipping plane.
	 *  @param nx x component of the normal vector of the clipping plane.
	 *  @param ny y component of the normal vector of the clipping plane.
	 *  @param nz z component of the normal vector of the clipping plane.
	 */
	inline CRiClippingPlane(
		long aLineNo = -1,
		RtFloat aX = 0,
		RtFloat aY = 0,
		RtFloat aZ = 0,
		RtFloat aNx = 0,
		RtFloat aNy = 0,
		RtFloat aNz = -1) :
	CRManInterfaceCall(aLineNo), m_x(aX), m_y(aY), m_z(aZ), m_nx(aNx), m_ny(aNy), m_nz(aNz)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiClippingPlane(const CRiClippingPlane &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiClippingPlane()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiClippingPlane(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING_PLANE; }

	/** @brief Gets the x component of a point at the clipping plane.
	 *
	 *  @return The x component of a point at the clipping plane.
	 */
	inline RtFloat x() const
	{
		return m_x;
	}

	/** @brief Sets the x component of a point at the clipping plane.
	 *
	 *  @param aX The x component of a point at the clipping plane.
	 */
	inline void x(RtFloat aX)
	{
		m_x = aX;
	}

	/** @brief Gets the y component of a point at the clipping plane.
	 *
	 *  @return The y component of a point at the clipping plane.
	 */
	inline RtFloat y() const
	{
		return m_y;
	}

	/** @brief Sets the y component of a point at the clipping plane.
	 *
	 *  @param aY The y component of a point at the clipping plane.
	 */
	inline void y(RtFloat aY)
	{
		m_y = aY;
	}

	/** @brief Gets the z component of a point at the clipping plane.
	 *
	 *  @return The z component of a point at the clipping plane.
	 */
	inline RtFloat z() const
	{
		return m_z;
	}

	/** @brief Sets the z component of a point at the clipping plane.
	 *
	 *  @param aZ The z component of a point at the clipping plane.
	 */
	inline void z(RtFloat aZ)
	{
		m_z = aZ;
	}

	/** @brief Gets the components of a point at the clipping plane.
	 *
	 *  @retval p The components of a point at the clipping plane.
	 */
	inline void getPoint(RtPoint &p) const
	{
		p[0] = m_x;
		p[1] = m_y;
		p[2] = m_z;
	}

	/** @brief Sets the components of a point at the clipping plane.
	 *
	 *  @param p The components of a point at the clipping plane.
	 */
	inline void setPoint(const RtPoint &p)
	{
		m_x = p[0];
		m_y = p[1];
		m_z = p[2];
	}

	/** @brief Gets the x component of the normal vector of the clipping plane.
	 *
	 *  @return The x component of the normal vector of the clipping plane.
	 */
	inline RtFloat nx() const
	{
		return m_nx;
	}

	/** @brief Sets the x component of the normal vector of the clipping plane.
	 *
	 *  @param aNx The x component of the normal vector the clipping plane.
	 */
	inline void nx(RtFloat aNx)
	{
		m_nx = aNx;
	}

	/** @brief Gets the y component of the normal vector of the clipping plane.
	 *
	 *  @return The y component of the normal vector of the clipping plane.
	 */
	inline RtFloat ny() const
	{
		return m_ny;
	}

	/** @brief Sets the y component of the normal vector of the clipping plane.
	 *
	 *  @param aNy The y component of the normal vector the clipping plane.
	 */
	inline void ny(RtFloat aNy)
	{
		m_ny = aNy;
	}

	/** @brief Gets the z component of the normal vector of the clipping plane.
	 *
	 *  @return The z component of the normal vector of the clipping plane.
	 */
	inline RtFloat nz() const
	{
		return m_nz;
	}

	/** @brief Sets the z component of the normal vector of the clipping plane.
	 *
	 *  @param aNz The z component of the normal vector the clipping plane.
	 */
	inline void nz(RtFloat aNz)
	{
		m_nz = aNz;
	}

	/** @brief Gets the components of the normal of the clipping plane.
	 *
	 *  @retval p The components of the normal of the clipping plane.
	 */
	inline void getNormal(RtPoint &p) const
	{
		p[0] = m_nx;
		p[1] = m_ny;
		p[2] = m_nz;
	}

	/** @brief Sets the components of the normal of the clipping plane.
	 *
	 *  @param p The components of the normal of the clipping plane.
	 */
	inline void setNormal(const RtPoint &p)
	{
		m_nx = p[0];
		m_ny = p[1];
		m_nz = p[2];
	}

	/** @brief Gets the values of the clipping plane
	 *
	 *  @retval x x component of a point at the clipping plane.
	 *  @retval y y component of a point at the clipping plane.
	 *  @retval z z component of a point at the clipping plane.
	 *  @retval nx x component of the normal vector of the clipping plane.
	 *  @retval ny y component of the normal vector of the clipping plane.
	 *  @retval nz z component of the normal vector of the clipping plane.
	 */
	inline void get(
		RtFloat &aX,
		RtFloat &aY,
		RtFloat &aZ,
		RtFloat &aNx,
		RtFloat &aNy,
		RtFloat &aNz) const
	{
		aX = m_x;
		aY = m_y;
		aZ = m_z;
		aNx = m_nx;
		aNy = m_ny;
		aNz =m_nz;
	}

	/** @brief Sets the values of the clipping plane
	 *
	 *  @param x x component of a point at the clipping plane.
	 *  @param y y component of a point at the clipping plane.
	 *  @param z z component of a point at the clipping plane.
	 *  @param nx x component of the normal vector of the clipping plane.
	 *  @param ny y component of the normal vector of the clipping plane.
	 *  @param nz z component of the normal vector of the clipping plane.
	 */
	inline void set(
		RtFloat aX,
		RtFloat aY,
		RtFloat aZ,
		RtFloat aNx,
		RtFloat aNy,
		RtFloat aNz)
	{
		m_x = aX;
		m_y = aY;
		m_z = aZ;
		m_nx = aNx;
		m_ny = aNy;
		m_nz = aNz;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiClippingPlane &operator=(const CRiClippingPlane &c)
	{
		if ( this == &c )
			return *this;

		set(c.x(), c.y(), c.z(), c.nx(), c.ny(), c.nz());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiClippingPlane


///////////////////////////////////////////////////////////////////////////////
/** @brief Depth of field of the camera projection.
 */
class CRiDepthOfField : public CRManInterfaceCall {
private:
	RtFloat
		m_fstop,         //!< Aperture number to calculate the lens diameter.
		m_focallength,   //!< Focal length of the camera.
		m_focaldistance; //!< Distance along the cameras z axis in which objects will be in focus.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDepthOfField"; }

	inline virtual const char *className() const { return CRiDepthOfField::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aFstop Aperture number to calculate the lens diameter.
	 *  @param aFocallength Focal length of the camera.
	 *  @param aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline CRiDepthOfField(
		long aLineNo = -1,
		RtFloat aFstop = defFstop,
		RtFloat aFocallength = defFocalLength,
		RtFloat aFocaldistance = defFocalDistance)
		: CRManInterfaceCall(aLineNo),
		  m_fstop(aFstop), m_focallength(aFocallength), m_focaldistance(aFocaldistance)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDepthOfField(const CRiDepthOfField &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDepthOfField()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDepthOfField(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DEPTH_OF_FIELD; }

	/** @brief Gets the aperture number.
	 *
	 *  @return The aperture number.
	 */
	inline RtFloat fstop() const
	{
		return m_fstop;
	}

	/** @brief Sets the aperture number.
	 *
	 *  @param aFstop The aperture number.
	 */
	inline void fstop(RtFloat aFstop)
	{
		m_fstop = aFstop;
	}

	/** @brief Gets the focal length.
	 *
	 *  @return The focal length.
	 */
	inline RtFloat focallength() const
	{
		return m_focallength;
	}

	/** @brief Sets the focal length.
	 *
	 *  @param  aFocallength The focal length.
	 */
	inline void focallength(RtFloat aFocallength)
	{
		m_focallength = aFocallength;
	}

	/** @brief Gets the focal distance.
	 *
	 *  @return The focal distance.
	 */
	inline RtFloat focaldistance() const
	{
		return m_focaldistance;
	}

	/** @brief Sets the focal distance.
	 *
	 *  @param  aFocallength The focal distance.
	 */
	inline void focaldistance(RtFloat aFocaldistance)
	{
		m_focaldistance = aFocaldistance;
	}

	/** @brief Gets the values of the depth of field option.
	 *
	 *  @retval aFstop Aperture number to calculate the lens diameter.
	 *  @retval aFocallength Focal length of the camera.
	 *  @retval aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline void get(
		RtFloat &aFstop,
		RtFloat &aFocallength,
		RtFloat &aFocaldistance) const
	{
		aFstop = m_fstop;
		aFocallength = m_focallength;
		aFocaldistance = m_focaldistance;
	}

	/** @brief Sets the values for the depth of field option.
	 *
	 *  @param aFstop Aperture number to calculate the lens diameter.
	 *  @param aFocallength Focal length of the camera.
	 *  @param aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline void set(
		RtFloat aFstop,
		RtFloat aFocallength,
		RtFloat aFocaldistance)
	{
		m_fstop = aFstop;
		m_focallength = aFocallength;
		m_focaldistance = aFocaldistance;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDepthOfField &operator=(const CRiDepthOfField &c)
	{
		if ( this == &c )
			return *this;

		set(c.fstop(), c.focallength(), c.focaldistance());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDepthOfField


///////////////////////////////////////////////////////////////////////////////
/** @brief The camera shuter open/close times
 */
class CRiShutter : public CRManInterfaceCall {
private:
	RtFloat
		m_smin, //!< Time when shutter opens.
		m_smax; //!< Time when shutter closes.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiShutter"; }

	inline virtual const char *className() const { return CRiShutter::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param smin Time when shutter opens.
	 *  @param smax Time when shutter closes.
	 */
	inline CRiShutter(
		long aLineNo = -1,
		RtFloat smin = defShutterOpen,
		RtFloat smax = defShutterClose)
		: CRManInterfaceCall(aLineNo),
		  m_smin(smin), m_smax(smax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiShutter(const CRiShutter &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiShutter()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiShutter(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SHUTTER; }

	/** @brief Gets the time at which the shutter opens.
	 *
	 *  @return The time at which the shutter opens.
	 */
	inline RtFloat shutterOpen() const
	{
		return m_smin;
	}

	/** @brief Sets the time at which the shutter opens.
	 *
	 *  @param smin The time at which the shutter opens.
	 */
	inline void shutterOpen(RtFloat smin)
	{
		m_smin = smin;
	}

	/** @brief Gets the time at which the shutter closes.
	 *
	 *  @return The time at which the shutter closes.
	 */
	inline RtFloat shutterClose() const
	{
		return m_smax;
	}

	/** @brief Sets the time at which the shutter closes.
	 *
	 *  @param smax The time at which the shutter closes.
	 */
	inline void shutterClose(RtFloat smax)
	{
		m_smax = smax;
	}

	/** @brief Gets the shutter times.
	 *
	 *  @retval smin Time when shutter opens.
	 *  @retval smax Time when shutter closes.
	 */
	inline void get(
		RtFloat &smin,
		RtFloat &smax) const
	{
		smin = m_smin;
		smax = m_smax;
	}

	/** @brief Sets the shutter times.
	 *
	 *  @param smin Time when shutter opens.
	 *  @param smax Time when shutter closes.
	 */
	inline void set(
		RtFloat smin,
		RtFloat smax)
	{
		m_smin = smin;
		m_smax = smax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShutter(m_smin, m_smax);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doShutter(m_smin, m_smax);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postShutter(m_smin, m_smax);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiShutter &operator=(const CRiShutter &c)
	{
		if ( this == &c )
			return *this;

		set(c.shutterOpen(), c.shutterClose());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiShutter


///////////////////////////////////////////////////////////////////////////////
/** @brief Estimated variance of the computed pixel value from the true pixel value.
 */
class CRiPixelVariance : public CRManInterfaceCall {
private:
	RtFloat m_variation; //!< Estimated variance of the computed pixel value from the true pixel value.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelVariance"; }

	inline virtual const char *className() const { return CRiPixelVariance::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aVariation Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline CRiPixelVariance(
		long aLineNo = -1,
		RtFloat aVariation = defPixelVariance)
		: CRManInterfaceCall(aLineNo),
		  m_variation(aVariation)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelVariance(const CRiPixelVariance &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPixelVariance()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelVariance(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_VARIANCE; }

	/** @brief Gets the pixel color variation.
	 *
	 *  @return Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline RtFloat variation() const
	{
		return m_variation;
	}

	/** @brief Sets the pixel color variation.
	 *
	 *  @param aVariation Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline void variation(RtFloat aVariation)
	{
		m_variation = aVariation;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePixelVariance(m_variation);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPixelVariance(m_variation);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPixelVariance(m_variation);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelVariance &operator=(const CRiPixelVariance &c)
	{
		if ( this == &c )
			return *this;

		variation(c.variation());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelVariance


///////////////////////////////////////////////////////////////////////////////
/** @brief Samples per pixel
 */
class CRiPixelSamples : public CRManInterfaceCall {
private:
	RtFloat
		m_xsamples, //!< Samples per pixel in horizontal direction.
		m_ysamples; //!< Samples per pixel in vertical direction.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelSamples"; }

	inline virtual const char *className() const { return CRiPixelSamples::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXsamples Samples per pixel in horizontal direction.
	 *  @param aYsamples Samples per pixel in vertical direction.
	 */
	inline CRiPixelSamples(
		long aLineNo = -1,
		RtFloat aXsamples = defXSamples,
		RtFloat aYsamples = defYSamples)
		: CRManInterfaceCall(aLineNo),
		  m_xsamples(aXsamples), m_ysamples(aYsamples)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelSamples(const CRiPixelSamples &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPixelSamples()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelSamples(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_SAMPLES; }

	/** @brief Gets the samples per pixel in horizontal direction.
	 *
	 *  @return The samples per pixel in horizontal direction.
	 */
	inline RtFloat xsamples() const
	{
		return m_xsamples;
	}

	/** @brief Sets the samples per pixel in horizontal direction.
	 *
	 *  @param aXsamples The samples per pixel in horizontal direction.
	 */
	inline void xsamples(RtFloat aXsamples)
	{
		m_xsamples = aXsamples;
	}

	/** @brief Sets the samples per pixel in vertical direction.
	 *
	 *  @return The samples per pixel in verical direction.
	 */
	inline RtFloat ysamples() const
	{
		return m_ysamples;
	}

	/** @brief Gets the samples per pixel in vertical direction.
	 *
	 *  @param aYsamples The samples per pixel in verical direction.
	 */
	inline void ysamples(RtFloat aYsamples)
	{
		m_ysamples = aYsamples;
	}

	/** @brief Gets the samples per pixel
	 *
	 *  @retval aXsamples Samples per pixel in horizontal direction.
	 *  @retval aYsamples Samples per pixel in vertical direction.
	 */
	inline void set(
		RtFloat &aXsamples,
		RtFloat &aYsamples) const
	{
		aXsamples = m_xsamples;
		aYsamples = m_ysamples;
	}

	/** @brief Sets the samples per pixel
	 *
	 *  @param aXsamples Samples per pixel in horizontal direction.
	 *  @param aYsamples Samples per pixel in vertical direction.
	 */
	inline void set(
		RtFloat aXsamples,
		RtFloat aYsamples)
	{
		m_xsamples = aXsamples;
		m_ysamples = aYsamples;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePixelSamples(m_xsamples, m_ysamples);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPixelSamples(m_xsamples, m_ysamples);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPixelSamples(m_xsamples, m_ysamples);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelSamples &operator=(const CRiPixelSamples &c)
	{
		if ( this == &c )
			return *this;

		set(c.xsamples(), c.ysamples());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelSamples

///////////////////////////////////////////////////////////////////////////////
/** @brief Pixel anti-aliasing filter.
 */
class CRiPixelFilter : public CRManInterfaceCall {
private:
	IFilterFunc *m_function; //!< Filter function.
	RtFloat m_xwidth,        //!< Horizontal influence in pixels.
	        m_ywidth;        //!< Vertical influence in pixels.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelFilter"; }

	inline virtual const char *className() const { return CRiPixelFilter::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  The default is a gaussian filter with the influence of defXFilterWidth x defYFilterWidth pixels.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPixelFilter(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
	{
		m_function = new CGaussianFilter();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiPixelFilter, m_function=CGaussianFilter", __LINE__, __FILE__);
		}
		m_xwidth = defXFilterWidth;
		m_ywidth = defYFilterWidth;
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aFunction Filter function, is duplicated.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline CRiPixelFilter(
		long aLineNo,
		const IFilterFunc &aFunction,
		RtFloat aXwidth, RtFloat aYwidth)
		: CRManInterfaceCall(aLineNo),
		  m_xwidth(aXwidth), m_ywidth(aYwidth)
	{
		m_function = aFunction.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiPixelFilter, m_function", __LINE__, __FILE__);
		}
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelFilter(const CRiPixelFilter &c)
	{
		*this = c;
	}

	/** @brief Destructor, frees resources.
	 */
	inline virtual ~CRiPixelFilter()
	{
		if ( m_function )
			delete m_function;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelFilter(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_FILTER; }

	/** @brief Gets a reference of the filter function.
	 *
	 *  @return Reference of the filter function.
	 */
	inline const IFilterFunc &function() const
	{
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"filterfunction is undefined", __LINE__, __FILE__);
		}
		return *m_function;
	}

	/** @brief Sets a new filter function (@a aFunction is duplicated).
	 *
	 *  @param aFunction Reference of a filter function.
	 */
	inline void function(const IFilterFunc &aFunction)
	{
		m_function = aFunction.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"pixel filter function could not be set", __LINE__, __FILE__);
		}
	}

	/** @brief Gets the horizontal influence in pixels.
	 *
	 *  @return The horizontal influence in pixels.
	 */
	inline RtFloat xwidth() const
	{
		return m_xwidth;
	}

	/** @brief Sets the horizontal influence in pixels.
	 *
	 *  @param aXWidth The horizontal influence in pixels.
	 */
	inline void xwidth(RtFloat aXWidth)
	{
		m_xwidth = aXWidth;
	}

	/** @brief Gets the vertical influence in pixels.
	 *
	 *  @return The vertical influence in pixels.
	 */
	inline RtFloat ywidth() const
	{
		return m_ywidth;
	}

	/** @brief Sets the vertical influence in pixels.
	 *
	 *  @param aYWidth The vertical influence in pixels.
	 */
	inline void ywidth(RtFloat aYWidth)
	{
		m_xwidth = aYWidth;
	}

	/** @brief Gets the pixel filter.
	 *
	 *  @param aFunction Pointer to a Filter function, gets a reference of the current function.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline void get(
		const IFilterFunc **function,
		RtFloat &aXwidth, RtFloat &aYwidth) const
	{
		if ( function )
			*function = m_function;

		aXwidth = m_xwidth;
		aYwidth = m_ywidth;
	}

	/** @brief Sets the pixel filter.
	 *
	 *  @param aFunction Filter function, is duplicated.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline void set(
		const IFilterFunc &function,
		RtFloat aXwidth, RtFloat aYwidth)
	{
		if ( m_function )
			delete m_function;

		m_function = function.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Set pixelfilter, m_function", __LINE__, __FILE__);
		}
		m_xwidth = aXwidth;
		m_ywidth = aYwidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.prePixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.doPixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.postPixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelFilter &operator=(const CRiPixelFilter &c)
	{
		if ( this == &c )
			return *this;

		set(c.function(), c.xwidth(), c.ywidth());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelFilter

///////////////////////////////////////////////////////////////////////////////
/** @brief Gamma correction.
 */
class CRiExposure : public CRManInterfaceCall {
private:
	RtFloat m_gain,  //!< Gain of the brightness correction.
	        m_gamma; //!< Gamma value of the brightness correction.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiExposure"; }

	inline virtual const char *className() const { return CRiExposure::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aGain Gain value.
	 *  @param aGamma Gamma value.
	 */
	inline CRiExposure(
		long aLineNo = -1,
		RtFloat aGain = defGain,
		RtFloat aGamma = defGamma)
		: CRManInterfaceCall(aLineNo),
		   m_gain(aGain), m_gamma(aGamma)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiExposure(const CRiExposure &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiExposure()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiExposure(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_EXPOSURE; }

	/** @brief Gets the gain value for the brightness correction.
	 *
	 *  @return Gain value.
	 */
	inline RtFloat gain() const
	{
		return m_gain;
	}

	/** @brief Sets the gain value for the brightness correction.
	 *
	 *  @param aGain Gain value.
	 */
	inline void gain(RtFloat aGain)
	{
		m_gain = aGain;
	}

	/** @brief Gets the gamma value for the brightness correction.
	 *
	 *  @return Gamma value.
	 */
	inline RtFloat gamma() const
	{
		return m_gamma;
	}

	/** @brief Sets the gamma value for the brightness correction.
	 *
	 *  @param aGamma Gamma value.
	 */
	inline void gamma(RtFloat aGamma)
	{
		m_gamma = aGamma;
	}

	/** @brief Gets the values for the brightness correction.
	 *
	 *  @retval aGain Gain value.
	 *  @retval aGamma Gamma value.
	 */
	inline void get(
		RtFloat &aGain,
		RtFloat &aGamma)
	{
		m_gain = aGain;
		m_gamma = aGamma;
	}

	/** @brief Sets the values for the brightness correction.
	 *
	 *  @param aGain Gain value.
	 *  @param aGamma Gamma value.
	 */
	inline void set(
		RtFloat aGain,
		RtFloat aGamma)
	{
		m_gain = aGain;
		m_gamma = aGamma;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preExposure(m_gain, m_gamma);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doExposure(m_gain, m_gamma);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postExposure(m_gain, m_gamma);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiExposure &operator=(const CRiExposure &c)
	{
		if ( this == &c )
			return *this;

		set(c.gain(), c.gamma());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiExposure

///////////////////////////////////////////////////////////////////////////////
/** @brief The imager shader.
 */
class CRiImager : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Atomized name of the imager shader.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiImager"; }

	inline virtual const char *className() const { return CRiImager::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName The name of the imager.
	 */
	inline CRiImager(
		long aLineNo = -1,
		RtToken aName = defImagerName)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_name = aName;
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Atomized name of the imager.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiImager(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Atomized name of the imager.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiImager(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_name = aName;
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiImager(const CRiImager &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiImager()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiImager(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_IMAGER; }

	/** @brief Gets the name of the imager.
	 *
	 *  @return Atomized name of the imager.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the imager.
	 *
	 *  @param aName Atomized name of the imager.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preImager(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doImager(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postImager(m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiImager &operator=(const CRiImager &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiImager

///////////////////////////////////////////////////////////////////////////////
/** @brief Quantize the color or depth values by dithering.
 */
class CRiQuantize : public CRManInterfaceCall {
private:
	RtToken m_type;     //!< Atomized type of quantization (RI_RGBA or RI_Z).
	RtInt m_one,        //!< The corresponding resulting integer value for RtFloat 1.0.
	      m_qmin,       //!< Lower clamp for the dither value (absoule).
	      m_qmax;       //!< Higher clamp for the dither value (absoule).
	RtFloat m_ditheramplitude; //!< Amplitude (+/-) for the dither offset.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiQuantize"; }

	inline virtual const char *className() const { return CRiQuantize::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of quantization (RI_RGBA or RI_Z).
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @param aQmin Lower clamp for the dither value (absoule).
	 *  @param aQmax Higher clamp for the dither value (absoule).
	 *  @param anAmpl Amplitude (+/-) for the dither offset.
	 */
	inline CRiQuantize(
		long aLineNo = -1,
		RtToken aType = RI_RGBA,
		RtInt aOne = defOneRGBA,
		RtInt aQmin = defMinRGBA,
		RtInt aQmax = defMaxRGBA,
		RtFloat aDitherAmplitude = defDitherAmplitudeRGBA)
		: CRManInterfaceCall(aLineNo),
		m_type(aType), m_one(aOne), m_qmin(aQmin), m_qmax(aQmax), m_ditheramplitude(aDitherAmplitude)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiQuantize(const CRiQuantize &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiQuantize()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiQuantize(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_QUANTIZE; }

	/** @brief Gets the quantization type.
	 *
	 * @return Quantization type.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets the quantization type.
	 *
	 * @param aType Atomized quantization type.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	/** @brief Gets the corresponding resulting integer value for RtFloat 1.0.
	 *
	 *  @return The corresponding resulting integer value for RtFloat 1.0.
	 */
	inline RtInt one() const
	{
		return m_one;
	}

	/** @brief Sets the corresponding resulting integer value for RtFloat 1.0.
	 *
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 */
	inline void one(RtInt aOne)
	{
		m_one = aOne;
	}

	/** @brief Gets the lower clamp for the dither value (absoule).
	 *
	 *  @return Lower clamp for the dither value (absoule).
	 */
	inline RtInt qmin() const
	{
		return m_qmin;
	}

	/** @brief Sets the lower clamp for the dither value (absoule).
	 *
	 *  @param aQMin Lower clamp for the dither value (absoule).
	 */
	inline void qmin(RtInt aQMin)
	{
		m_qmin = aQMin;
	}

	/** @brief Gets the higher clamp for the dither value (absoule).
	 *
	 *  @return Higher clamp for the dither value (absoule).
	 */
	inline RtInt qmax() const
	{
		return m_qmax;
	}

	/** @brief Sets the higher clamp for the dither value (absoule).
	 *
	 *  @param aQMax Higher clamp for the dither value (absoule).
	 */
	inline void qmax(RtInt aQMax)
	{
		m_qmax = aQMax;
	}

	/** @brief Gets the dither amplitude.
	 *
	 *  @return The dither amplitude.
	 */
	inline RtFloat ditheramplitude() const
	{
		return m_ditheramplitude;
	}

	/** @brief Sets the dither amplitude.
	 *
	 *  @param aDitherAmplitude The dither amplitude.
	 */
	inline void ditheramplitude(RtFloat aDitherAmplitude)
	{
		m_ditheramplitude = aDitherAmplitude;
	}

	/** @brief Gets the values of the quantizer
	 *
	 *  @retval aType The type of quantization (RI_RGBA or RI_Z).
	 *  @retval aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @retval aQmin Lower clamp for the dither value (absoule).
	 *  @retval aQmax Higher clamp for the dither value (absoule).
	 *  @retval aDitherAmplitude Amplitude (+/-) for the dither offset.
	 */
	inline void get(
		RtToken &aType,
		RtInt &aOne,
		RtInt &aQmin,
		RtInt &aQmax,
		RtFloat &aDitherAmplitude) const
	{
		aType = m_type;
		aOne = m_one;
		aQmin = m_qmin;
		aQmax = m_qmax;
		aDitherAmplitude = m_ditheramplitude;
	}

	/** @brief Sets the values of the quantizer
	 *
	 *  @param aType The type of quantization (RI_RGBA or RI_Z).
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @param aQmin Lower clamp for the dither value (absoule).
	 *  @param aQmax Higher clamp for the dither value (absoule).
	 *  @param aDitherAmplitude Amplitude (+/-) for the dither offset.
	 */
	inline void set(
		RtToken aType,
		RtInt aOne,
		RtInt aQmin,
		RtInt aQmax,
		RtFloat aDitherAmplitude)
	{
		m_type = aType;
		m_one = aOne;
		m_qmin = aQmin;
		m_qmax = aQmax;
		m_ditheramplitude = aDitherAmplitude;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiQuantize &operator=(const CRiQuantize &c)
	{
		if ( this == &c )
			return *this;

		set(c.type(), c.one(), c.qmin(), c.qmax(), c.ditheramplitude());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiQuantize

///////////////////////////////////////////////////////////////////////////////
/** @brief New display channel for output
 */
class CRiDisplayChannel : public CVarParamRManInterfaceCall {
private:
	std::string m_channel; //!< Name of the display channel

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDisplayChannel"; }

	inline virtual const char *className() const { return CRiDisplayChannel::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aChannel Name of the display channel.
	 */
	inline CRiDisplayChannel(
		long aLineNo = -1,
		RtString aChannel = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_channel = noNullStr(aChannel);
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aChannel Name of the display channel.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiDisplayChannel(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aChannel,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_channel = noNullStr(aChannel);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aChannel Name of the display channel.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiDisplayChannel(
		long aLineNo,
		RtToken aChannel,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_channel = noNullStr(aChannel);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDisplayChannel(const CRiDisplayChannel &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDisplayChannel()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDisplayChannel(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY_CHANNEL; }

	/** @brief Gets the name of the display channel.
	 *
	 *  @return The name of the display channel
	 */
	inline RtString channel() const
	{
		return m_channel.c_str();
	}

	/** @brief Sets the name of the display channel.
	 *
	 *  @param aChannel The name of the display channel
	 */
	inline void channel(RtToken aChannel)
	{
		m_channel = noNullStr(aChannel);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplayChannel(m_channel.c_str(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisplayChannel(m_channel.c_str(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisplayChannel(m_channel.c_str(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDisplayChannel &operator=(const CRiDisplayChannel &c)
	{
		if ( this == &c )
			return *this;

		channel(c.channel());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplayChannel

///////////////////////////////////////////////////////////////////////////////
/** @brief Set an output display description.
 */
class CRiDisplay : public CVarParamRManInterfaceCall {
private:
	std::string m_name; //!< Name of the display.
	RtToken m_type, //!< Atomized type of the display.
	        m_mode; //!< Atomized mode of the output.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDisplay"; }

	inline virtual const char *className() const { return CRiDisplay::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 */
	inline CRiDisplay(
		long aLineNo = -1,
		RtString aName = RI_NULL, RtToken aType = RI_NULL, RtToken aMode = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo),
		m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aName, RtToken aType, RtToken aMode,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo),
		  m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiDisplay(
		long aLineNo,
		RtString aName, RtToken aType, RtToken aMode,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters),
		  m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDisplay(const CRiDisplay &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDisplay()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDisplay(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY; }

	/** @brief Gets the name of the display.
	 *
	 *  @return The name of the display.
	 */
	inline RtString name() const
	{
		return m_name.c_str();
	}

	/** @brief Sets the name of the display.
	 *
	 *  @param aName The name of the display.
	 */
	inline void name(RtString aName)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Gets the type of the display.
	 *
	 *  @return Atomized type of the display.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Dets the type of the display.
	 *
	 *  @param aType Atomized type of the display.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	/** @brief Gets the mode of the output.
	 *
	 *  @return Atomized mode of the output.
	 */
	inline RtToken mode() const
	{
		return m_mode;
	}

	/** @brief Sets the mode of the output.
	 *
	 *  @param aMode Atomized mode of the output.
	 */
	inline void mode(RtToken aMode)
	{
		m_mode = aMode;
	}

	/** @brief Gets the display values.
	 *
	 *  @retval aName Name of the display.
	 *  @retval aType Atomized type of the display.
	 *  @retval aMode Atomized mode of the output.
	 */
	inline void get(RtString *aName, RtToken &aType, RtToken &aMode) const
	{
		if ( aName )
			*aName = m_name.c_str();
		aType = m_type;
		aMode = m_mode;
	}

	/** @brief Sets the display values.
	 *
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 */
	inline void set(RtString aName, RtToken aType, RtToken aMode)
	{
		m_name = noNullStr(aName);
		m_type = aType;
		m_mode = aMode;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDisplay &operator=(const CRiDisplay &c)
	{
		if ( this == &c )
			return *this;

		set(c.name(), c.type(), c.mode());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplay

///////////////////////////////////////////////////////////////////////////////
/** @brief The hidden surface algorithm.
 */
class CRiHider : public CVarParamRManInterfaceCall {
private:
	RtToken m_type; //!< Atomized type of the hidden surface algorithm.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiHider"; }

	inline virtual const char *className() const { return CRiHider::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of the hidden surface algorithm.
	 */
	inline CRiHider(
		long aLineNo = -1,
		RtToken aType = defHiderType)
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aType Atomized type of the hidden surface algorithm.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiHider(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aType,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of the hidden surface algorithm.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiHider(
		long aLineNo,
		RtToken aType,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_type(aType)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiHider(const CRiHider &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiHider()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiHider(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_HIDER; }

	/** @brief Gets the type of the hidden surface algorithm.
	 *
	 *  @return Atomized type of the hidden surface algorithm.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets the type of the hidden surface algorithm.
	 *
	 *  @param aType Atomized type of the hidden surface algorithm.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preHider(m_type, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doHider(m_type, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postHider(m_type, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiHider &operator=(const CRiHider &c)
	{
		if ( this == &c )
			return *this;

		type(c.type());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiHider

///////////////////////////////////////////////////////////////////////////////
/** @brief the samples per color and color transformation to/from RGB.
 */
class CRiColorSamples : public CRManInterfaceCall {
private:
	CColorDescr m_colorDecr; //!< Stored color descriptor.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiColorSamples"; }

	inline virtual const char *className() const { return CRiColorSamples::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  Per Default no color transformation is done (transformation is the identity).
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiColorSamples(long aLineNo=-1)
		: CRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param N Number of color samples (>0)
	 *  @param nRGB Transformation matrix from a color to a RGB value
	 *  @param RGBn Transformation matrix from a RGB value to a color
	 */
	inline CRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn)
		: CRManInterfaceCall(aLineNo), m_colorDecr(N, nRGB, RGBn)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiColorSamples(const CRiColorSamples &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiColorSamples()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiColorSamples(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR_SAMPLES; }

	/** @brief Gets the number color samples.
	 *
	 * @return The number of color samples.
	 */
	inline RtInt colorSamples() const
	{
		return m_colorDecr.colorSamples();
	}

	/** @brief Gets the number color samples and transformation matrices.
	 *
	 * @see CColorDescr::getColorSamples()
	 *
	 * @retval nColorSamples The number of color samples.
	 * @retval nRGB Matrix color to RGB.
	 * @retval RGBn Matrix RGB to color.
	 */
	inline void get(RtInt nColorSamples, std::vector<RtFloat> &nRGB, std::vector<RtFloat> &RGBn) const
	{
		m_colorDecr.getColorSamples(nColorSamples, nRGB, RGBn);
	}

	/** @brief Sets the number color samples and transformation matrices.
	 *
	 * @see CColorDescr::colorSamples()
	 *
	 * @param nColorSamples The number of color samples (>0).
	 * @param nRGB Matrix color to RGB (should not be 0).
	 * @param RGBn Matrix RGB to color (should not be 0).
	 */
	inline RtVoid set(RtInt nColorSamples, RtFloat *nRGB, RtFloat *RGBn)
	{
		m_colorDecr.colorSamples(nColorSamples, nRGB, RGBn);
	}

	/** @brief Gets the color descriptor, read-only.
	 *
	 *  @return The read-only color descriptor.
	 */
	inline const CColorDescr &colorDescr() const
	{
		return m_colorDecr;
	}

	/** @brief Gets the color descriptor.
	 *
	 *  @return The color descriptor.
	 */
	inline CColorDescr &colorDescr()
	{
		return m_colorDecr;
	}

	/** @brief Sets the color descriptor.
	 *
	 *  @param aColorDescr A color descriptor.
	 */
	inline void colorDescr(const CColorDescr &aColorDescr)
	{
		m_colorDecr = aColorDescr;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.preColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.doColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.postColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiColorSamples &operator=(const CRiColorSamples &c)
	{
		if ( this == &c )
			return *this;

		colorDescr(c.colorDescr());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiColorSamples

///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the relative detail scale.
 */
class CRiRelativeDetail : public CRManInterfaceCall {
private:
	RtFloat m_relativedetail; //!< The relative detail scale.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiRelativeDetail"; }

	inline virtual const char *className() const { return CRiRelativeDetail::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRelativedetail A relative detail scale.
	 */
	inline CRiRelativeDetail(
		long aLineNo = -1,
		RtFloat aRelativedetail = defRelativeDetail)
		: CRManInterfaceCall(aLineNo), m_relativedetail(aRelativedetail)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiRelativeDetail(const CRiRelativeDetail &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiRelativeDetail()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiRelativeDetail(*this);
	}

	inline EnumRequests interfaceIdx() const { return REQ_RELATIVE_DETAIL; }

	/** @brief Gets the relative detail scale.
	 *
	 *  @return The relative detail scale.
	 */
	inline virtual RtFloat relativeDetail() const
	{
		return m_relativedetail;
	}

	/** @brief Sets the relative detail scale.
	 *
	 *  @return The relative detail scale.
	 */
	inline void relativeDetail(RtFloat aRelativedetail)
	{
		m_relativedetail = aRelativedetail;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preRelativeDetail(m_relativedetail);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doRelativeDetail(m_relativedetail);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postRelativeDetail(m_relativedetail);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiRelativeDetail &operator=(const CRiRelativeDetail &c) {
		if ( this == &c )
			return *this;

		relativeDetail(c.relativeDetail());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiRelativeDetail

///////////////////////////////////////////////////////////////////////////////
/** @brief User defined option.
 */
class CRiOption : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Name of the option as atomized string.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiOption"; }

	inline virtual const char *className() const { return CRiOption::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the option as atomized string.
	 */
	inline CRiOption(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the option as atomized string.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiOption(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the option as atomized string.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiOption(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiOption(const CRiOption &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiOption()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiOption(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OPTION; }

	/** @brief Gets the name of the option as atomized string.
	 *
	 *  @return The name of the option as atomized string.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the option as atomized string.
	 *
	 *  @param aName The name of the option as atomized string.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preOption(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doOption(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postOption(m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiOption &operator=(const CRiOption &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiOption


// ----------------------------------------------------------------------------
// Light sources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handleIdx;
public:
	inline static const char *myClassName(void) { return "CRiLightSource"; }
	inline virtual const char *className() const { return CRiLightSource::myClassName(); }

	inline CRiLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
	}
	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo()), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		CParameterClasses p;
		setParams(state.dict(), p, state.options().colorDescr(), n, tokens, params);
	}
	inline CRiLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_LIGHT_SOURCE; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		RtLightHandle h = ri.preLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, h);
		ri.doLightSource(h, m_name, parameters());
	}
	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }
	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }
	inline CRiLightSource &operator=(const CRiLightSource &c) {
		if ( this == &c )
			return *this;

		name(c.name());
		handleIdx(c.handleIdx());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiAreaLightSource : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
	RtLightHandle m_handleIdx;
public:
	inline static const char *myClassName(void) { return "CRiAreaLightSource"; }
	inline virtual const char *className() const { return CRiAreaLightSource::myClassName(); }

	inline CRiAreaLightSource(
		long aLineNo = -1,
		RtToken aName=RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		m_handleIdx = illLightHandle;
	}
	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(state.lineNo()), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
		CParameterClasses p;
		setParams(state.dict(), p, state.options().colorDescr(), n, tokens, params);
	}
	inline CRiAreaLightSource(
		CRenderState &state,
		RtToken aName, 
		const CParameterList &theParameters)
		: CVarParamRManInterfaceCall(state.lineNo(), theParameters), m_name(aName)
	{
		m_handleIdx = state.lights().newLightHandleIdx();
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_AREA_LIGHT_SOURCE; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		RtLightHandle h = ri.preAreaLightSource(m_name, parameters());
		ri.renderState()->lights().setHandle(m_handleIdx, h);
		ri.doAreaLightSource(h, m_name, parameters());
	}
	inline virtual RtLightHandle handleIdx() const { return m_handleIdx; }
	inline virtual void handleIdx(RtLightHandle anIndex) { m_handleIdx = anIndex; }
	inline CRiAreaLightSource &operator=(const CRiAreaLightSource &c) {
		if ( this == &c )
			return *this;

		name(c.name());
		handleIdx(c.handleIdx());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiAreaLightSource

///////////////////////////////////////////////////////////////////////////////
class CRiIlluminate : public CRManInterfaceCall {
protected:
	RtLightHandle m_handleIdx;
	RtBoolean m_onoff;
public:
	inline static const char *myClassName(void) { return "CRiIlluminate"; }
	inline virtual const char *className() const { return CRiIlluminate::myClassName(); }

	inline CRiIlluminate(long aLineNo, RtLightHandle handleIdx, RtBoolean onoff) :
		CRManInterfaceCall(aLineNo), m_handleIdx(handleIdx), m_onoff(onoff)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ILLUMINATE; }

	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIlluminate(m_handleIdx, m_onoff);
		ri.doIlluminate(m_handleIdx, m_onoff);
	}
	inline virtual RtLightHandle handleIdx() { return m_handleIdx; }
	inline CRiIlluminate &operator=(const CRiIlluminate &)
	{
		return *this;
	}
}; // CRiIlluminate

// ----------------------------------------------------------------------------
// Attributes
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiAttribute : public CVarParamRManInterfaceCall {
protected:
	std::string m_name;
public:
	inline static const char *myClassName(void) { return "CRiAttribute"; }
	inline virtual const char *className() const { return CRiAttribute::myClassName(); }

	inline CRiAttribute(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiAttribute(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAttribute(m_name.c_str(), parameters());
		ri.doAttribute(m_name.c_str(), parameters());
	}
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

	inline CRiColor(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		RtInt i;
		for ( i=0; i<curColorDescr.colorSamples(); ++i )
			m_color.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preColor(m_color.empty() ? 0 : &m_color[0]);
		ri.doColor(m_color.empty() ? 0 : &m_color[0]);
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

	inline CRiOpacity(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) : CRManInterfaceCall(aLineNo) {
		RtInt i;
		for ( i=0; i<curColorDescr.colorSamples(); ++i )
			m_opacity.push_back(Cs[i]);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OPACITY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
		ri.doOpacity(m_opacity.empty() ? 0 : &m_opacity[0]);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiSurface(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SURFACE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSurface(m_name.c_str(), parameters());
		ri.doSurface(m_name.c_str(), parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiAtmosphere(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ATMOSPHERE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preAtmosphere(m_name.c_str(), parameters());
		ri.doAtmosphere(m_name.c_str(), parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiInterior(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_INTERIOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preInterior(m_name.c_str(), parameters());
		ri.doInterior(m_name.c_str(), parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiExterior(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_EXTERIOR; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preExterior(m_name.c_str(), parameters());
		ri.doExterior(m_name.c_str(), parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiDisplacement(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLACEMENT; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplacement(m_name.c_str(), parameters());
		ri.doDisplacement(m_name.c_str(), parameters());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
		ri.doTextureCoordinates(m_s1, m_t1, m_s2, m_t2, m_s3, m_t3, m_s4, m_t4);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShadingRate(m_size);
		ri.doShadingRate(m_size);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShadingInterpolation(m_type.c_str());
		ri.doShadingInterpolation(m_type.c_str());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMatte(m_onoff);
		ri.doMatte(m_onoff);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBound(m_bound);
		ri.doBound(m_bound);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDetail(m_bound);
		ri.doDetail(m_bound);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
		ri.doDetailRange(m_minvis, m_lowtran, m_uptran, m_maxvis);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometricApproximation(m_type.c_str(), m_value);
		ri.doGeometricApproximation(m_type.c_str(), m_value);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometricRepresentation(m_type.c_str());
		ri.doGeometricRepresentation(m_type.c_str());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preOrientation(m_orientation.c_str());
		ri.doOrientation(m_orientation.c_str());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preReverseOrientation();
		ri.doReverseOrientation();
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSides(m_nsides);
		ri.doSides(m_nsides);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
		ri.doBasis(m_ubasis, m_ustep, m_vbasis, m_vstep);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preTrimCurve(m_trimCurve.m_nLoops,
			m_trimCurve.m_nCurves.empty() ? 0 : &m_trimCurve.m_nCurves[0],
			m_trimCurve.m_order.empty() ? 0 : &m_trimCurve.m_order[0],
			m_trimCurve.m_knots.empty() ? 0 : &m_trimCurve.m_knots[0],
			m_trimCurve.m_min.empty() ? 0 : &m_trimCurve.m_min[0],
			m_trimCurve.m_max.empty() ? 0 : &m_trimCurve.m_max[0],
			m_trimCurve.m_n.empty() ? 0 : &m_trimCurve.m_n[0],
			m_trimCurve.m_u.empty() ? 0 : &m_trimCurve.m_u[0],
			m_trimCurve.m_v.empty() ? 0 : &m_trimCurve.m_v[0],
			m_trimCurve.m_w.empty() ? 0 : &m_trimCurve.m_w[0]);
		ri.doTrimCurve(m_trimCurve.m_nLoops,
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


// ----------------------------------------------------------------------------
// Transformations and coordinate systems
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiIdentity : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiIdentity"; }
	inline virtual const char *className() const { return CRiIdentity::myClassName(); }

	inline CRiIdentity(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preIdentity();
		ri.doIdentity();
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTransform(m_transform);
		ri.doTransform(m_transform);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preConcatTransform(m_transform);
		ri.doConcatTransform(m_transform);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePerspective(m_fov);
		ri.doPerspective(m_fov);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTranslate(m_dx, m_dy, m_dz);
		ri.doTranslate(m_dx, m_dy, m_dz);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preRotate(m_angle, m_dx, m_dy, m_dz);
		ri.doRotate(m_angle, m_dx, m_dy, m_dz);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preScale(m_dx, m_dy, m_dz);
		ri.doScale(m_dx, m_dy, m_dz);
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSkew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2);
		ri.doSkew(m_angle, m_dx1, m_dy1, m_dz1, m_dx2, m_dy2, m_dz2);
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DEFORMATION; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDeformation(m_name.c_str(), parameters());
		ri.doDeformation(m_name.c_str(), parameters());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCoordinateSystem(m_name.c_str());
		ri.doCoordinateSystem(m_name.c_str());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCoordSysTransform(m_name.c_str());
		ri.doCoordSysTransform(m_name.c_str());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
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


// ----------------------------------------------------------------------------
// Graphical primitives
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiPolygon : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nVertices;
public:
	inline static const char *myClassName(void) { return "CRiPolygon"; }
	inline virtual const char *className() const { return CRiPolygon::myClassName(); }

	inline CRiPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CPolygonRManInterfaceCall(aLineNo), m_nVertices(nvertices)
	{
		CPolygonClasses p(nvertices);
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiPolygon(
		long aLineNo,
		RtInt nvertices,
		const CParameterList &theParameters
		)
		: CPolygonRManInterfaceCall(aLineNo, theParameters)
	{
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_POLYGON; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePolygon(m_nVertices, parameters());
		ri.doPolygon(m_nVertices, parameters());
	}
	inline CRiPolygon &operator=(const CRiPolygon &) {
		return *this;
	}
}; // CRiPolygon

///////////////////////////////////////////////////////////////////////////////
class CRiGeneralPolygon : public CPolygonRManInterfaceCall {
protected:
	RtInt m_nLoops;
	std::vector<RtInt> m_nVerts;
	void enterValues(RtInt nloops, RtInt *nverts);
public:
	inline static const char *myClassName(void) { return "CRiGeneralPolygon"; }
	inline virtual const char *className() const { return CRiGeneralPolygon::myClassName(); }

	CRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	CRiGeneralPolygon(
		long aLineNo,
		RtInt nloops, RtInt *nverts,
		const CParameterList &theParameters);

	inline virtual EnumRequests interfaceIdx() const { return REQ_GENERAL_POLYGON; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preGeneralPolygon(m_nLoops,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
		ri.doGeneralPolygon(m_nLoops,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
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
	void enterValues(RtInt npolys, RtInt *nverts, RtInt *verts);
public:
	inline static const char *myClassName(void) { return "CRiPointsPolygons"; }
	inline virtual const char *className() const { return CRiPointsPolygons::myClassName(); }

	CRiPointsPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	CRiPointsPolygons(
		long aLineNo,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		const CParameterList &theParameters);
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsPolygons(
			m_nPolys,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
		ri.doPointsPolygons(
			m_nPolys,
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}
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
	void enterValues(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts);
public:
	inline static const char *myClassName(void) { return "CRiPointsGeneralPolygons"; }
	inline virtual const char *className() const { return CRiPointsGeneralPolygons::myClassName(); }

	CRiPointsGeneralPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[]);
	CRiPointsGeneralPolygons(
		long aLineNo,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		const CParameterList &theParameters);
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsGeneralPolygons(
			m_nPolys,
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
		ri.prePointsGeneralPolygons(
			m_nPolys,
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo), m_type(type)
	{
		CPatchClasses p(type);
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiPatch(
		long aLineNo,
		RtToken type,
		const CParameterList &theParameters)
		: CUVRManInterfaceCall(aLineNo), m_type(type)
	{
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatch(m_type.c_str(), parameters());
		ri.doPatch(m_type.c_str(), parameters());
	}
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
	void enterValues(RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap);
public:
	inline static const char *myClassName(void) { return "CRiPatchMesh"; }
	inline virtual const char *className() const { return CRiPatchMesh::myClassName(); }

	CRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	CRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		const CParameterList &theParameters);

	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatchMesh(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), parameters());
		ri.doPatchMesh(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preNuPatch(m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
		ri.doNuPatch(m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}
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

	CRiSubdivisionMesh(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);
	inline virtual EnumRequests interfaceIdx() const { return REQ_SUBDIVISION_MESH; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSubdivisionMesh(
			m_scheme.c_str(), m_nfaces,
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			m_ntags,
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			parameters()
			);

		ri.doSubdivisionMesh(
			m_scheme.c_str(), m_nfaces,
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			m_ntags,
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			parameters()
			);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	inline CRiSphere(
		long aLineNo,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
	}

	inline CRiSphere(const CRiSphere &c)
	{
		*this = c;
	}

	inline virtual ~CRiSphere() {}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSphere(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSphere(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSphere(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSphere(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline CRiSphere &operator=(const CRiSphere &c) {
		if ( this == &c )
			return *this;
		m_radius = c.m_radius;
		m_zmin = c.m_zmin;
		m_zmax = c.m_zmax;
		m_thetamax = c.m_thetamax;
		CUVRManInterfaceCall::operator=(c);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_CONE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCone(m_height, m_radius, m_thetamax, parameters());
		ri.doCone(m_height, m_radius, m_thetamax, parameters());
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(radius), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCylinder(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
		ri.doCylinder(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
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
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_HYPERBOLOID; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preHyperboloid(m_point1, m_point2, m_thetamax, parameters());
		ri.doHyperboloid(m_point1, m_point2, m_thetamax, parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_rmax(rmax), m_zmin(zmin), m_zmax(zmax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_PARABOLOID; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preParaboloid(m_rmax, m_zmin, m_zmax, m_thetamax, parameters());
		ri.doParaboloid(m_rmax, m_zmin, m_zmax, m_thetamax, parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_height(height), m_radius(radius), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISK; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		ri.preDisk(m_height, m_radius, m_thetamax, parameters());
		ri.doDisk(m_height, m_radius, m_thetamax, parameters());
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo),
		  m_majorrad(majorrad), m_minorrad(minorrad),
		  m_phimin(phimin), m_phimax(phimax), m_thetamax(thetamax)
	{
		CQuadricClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiTorus(
		long aLineNo,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_majorrad(majorrad), m_minorrad(minorrad),
		  m_phimin(phimin), m_phimax(phimax), m_thetamax(thetamax)
	{
	}

	inline CRiTorus(const CRiTorus &c)
	{
		*this = c;
	}

	inline virtual ~CRiTorus() {}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTorus(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TORUS; }
	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTorus(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTorus(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTorus(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}
	inline CRiTorus &operator=(const CRiTorus &c) {
		if ( this == &c )
			return *this;
		m_majorrad = c.m_majorrad;
		m_minorrad = c.m_minorrad;
		m_phimin = c.m_phimin;
		m_phimax = c.m_phimax;
		m_thetamax = c.m_thetamax;
		CUVRManInterfaceCall::operator=(c);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_npts(npts)
	{
		CPointsClasses p(npts);
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePoints(m_npts, parameters());
		ri.doPoints(m_npts, parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_CURVES; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCurves(m_type.c_str(), m_ncurves,
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap.c_str(),
			parameters());
		ri.doCurves(m_type.c_str(), m_ncurves,
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap.c_str(),
			parameters());
	}
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

	CRiBlobby(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[]);
	inline virtual EnumRequests interfaceIdx() const { return REQ_BLOBBY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBlobby(m_nleaf,
			m_ncode, m_code.empty() ? 0 : &m_code[0],
			m_nflt, m_flt.empty() ? 0 : &m_flt[0],
			m_nstr, m_str.empty() ? 0 : &m_str[0],
			parameters());
		ri.doBlobby(m_nleaf,
			m_ncode, m_code.empty() ? 0 : &m_code[0],
			m_nflt, m_flt.empty() ? 0 : &m_flt[0],
			m_nstr, m_str.empty() ? 0 : &m_str[0],
			parameters());
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		assert(m_subdivfunc != 0);
		assert(m_freefunc != 0);
		if ( m_subdivfunc != 0 && m_freefunc != 0 )
		ri.preProcedural(m_str.empty() ? 0 :
			m_str.empty() ? 0 : (RtPointer)&m_str[0],
			m_bound, *m_subdivfunc, *m_freefunc);
		ri.doProcedural(m_str.empty() ? 0 :
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometry(m_name.c_str(), parameters());
		ri.doGeometry(m_name.c_str(), parameters());
	}
	inline CRiGeometry &operator=(const CRiGeometry &) {
		return *this;
	}
}; // CRiGeometry

// ----------------------------------------------------------------------------
// External resources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiArchiveInstance : public CVarParamRManInterfaceCall {
protected:
	RtArchiveHandle m_handle;
	IArchiveCallback *m_callback;
public:
	inline static const char *myClassName(void) { return "CRiArchiveInstance"; }
	inline virtual const char *className() const { return CRiArchiveInstance::myClassName(); }

	inline CRiArchiveInstance(long aLineNo=-1, RtObjectHandle aHandle=illArchiveHandle,	const IArchiveCallback *aCallback = 0) : CVarParamRManInterfaceCall(aLineNo), m_handle(aHandle)
	{
		m_callback = aCallback ? aCallback->duplicate() : 0;
	}

	inline CRiArchiveInstance(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtObjectHandle aHandle,	const IArchiveCallback *aCallback,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_handle = aHandle;
		m_callback = aCallback ? aCallback->duplicate() : 0;
	}

	inline CRiArchiveInstance(
		long aLineNo,
		RtObjectHandle aHandle, const IArchiveCallback *aCallback,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_handle = aHandle;
		m_callback = aCallback ? aCallback->duplicate() : 0;
	}

	inline CRiArchiveInstance(const CRiArchiveInstance &c)
	{
		*this = c;
	}

	inline virtual ~CRiArchiveInstance()
	{
		if ( m_callback ) {
			delete m_callback;
		}
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiArchiveInstance(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_INSTANCE; }
	
	inline virtual RtObjectHandle handle() const { return m_handle; }
	
	inline virtual void handle(RtObjectHandle h) { m_handle = h; }

	inline const IArchiveCallback *callback() const
	{
		return m_callback;
	}
	
	inline void callback(const IArchiveCallback *cb)
	{
		if ( m_callback )
			delete m_callback;
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preArchiveInstance(handle(), m_callback, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doArchiveInstance(handle(), m_callback, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postArchiveInstance(handle(), m_callback, parameters());
	}
	
	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, m_callback);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, m_callback);
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, m_callback);
	}

	inline CRiArchiveInstance &operator=(const CRiArchiveInstance &c) {
		if ( this == &c )
			return *this;

		handle(c.handle());
		callback(c.callback());
		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiArchiveInstance

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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_px(px), m_nx(nx), m_py(py), m_ny(ny), m_pz(pz), m_nz(nz), m_tex(tex), m_fov(fov), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_SHADOW; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
		ri.doMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
	}
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
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preArchiveRecord(m_type.c_str(), m_line.c_str());
		ri.doArchiveRecord(m_type.c_str(), m_line.c_str());
		if ( cb ) 
			(*cb)(ri, m_type.c_str(), m_line.c_str());
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aFilename, const IArchiveCallback *cb,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_filename(noNullStr(aFilename))
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);

		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline virtual ~CRiReadArchive()
	{
		if ( m_callback )
			delete m_callback;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
	
	inline const IArchiveCallback *callback() const
	{
		return m_callback;
	}
	inline void callback(const IArchiveCallback *cb)
	{
		if ( m_callback )
			delete m_callback;
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline RtString filename() const
	{
		return m_filename.c_str();
	}
	inline void filename(RtString aFilename)
	{
		m_filename = noNullStr(aFilename);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, m_callback);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, m_callback);
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, m_callback);
	}

	inline CRiReadArchive &operator=(const CRiReadArchive &c) {
		if ( this == &c )
			return *this;

		filename(c.filename());
		callback(c.callback());
		
		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiReadArchive


///////////////////////////////////////////////////////////////////////////////
//! Interface, return a new instance of a IRi... class.
class CRManInterfaceFactory {
public:
	inline CRManInterfaceFactory() {}
	inline virtual ~CRManInterfaceFactory() {}

	inline virtual CRiErrorHandler *newRiErrorHandler(
		long aLineNo,
		const IErrorHandler &handler)
	{
		return new CRiErrorHandler(aLineNo, handler);
	}

	inline virtual CRiDeclare *newRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
	{
		return new CRiDeclare(aLineNo, name, declaration);	
	}

	inline virtual CRiResourceBegin *newRiResourceBegin(long aLineNo) {
		return new CRiResourceBegin(aLineNo);
	}

	inline virtual CRiResourceEnd *newRiResourceEnd(long aLineNo) {
		return new CRiResourceEnd(aLineNo);
	}

	inline virtual CRiResource *newRiResource(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken handle,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiResource(aLineNo, decl, curColorDescr, handle, type, n, tokens, params);
	}

	inline virtual CRiResource *newRiResource(
		long aLineNo,
		RtToken handle,
		RtToken type,
		const CParameterList &parameters)
	{
		return new CRiResource(aLineNo, handle, type, parameters);
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

	inline virtual CRiAttributeBegin *newRiAttributeBegin(long aLineNo) {
		return new CRiAttributeBegin(aLineNo);
	}

	inline virtual CRiAttributeEnd *newRiAttributeEnd(long aLineNo) {
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

	inline virtual CRiObjectBegin *newRiObjectBegin(long aLineNo) {
		return new CRiObjectBegin(aLineNo);
	}

	inline virtual CRiObjectEnd *newRiObjectEnd(long aLineNo) {
		return new CRiObjectEnd(aLineNo);
	}

	inline virtual CRiArchiveBegin *newRiArchiveBegin(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiArchiveBegin(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiArchiveBegin *newRiArchiveBegin(
		long aLineNo,
		const char *name,
		const CParameterList &parameters)
	{
		return new CRiArchiveBegin(aLineNo, name, parameters);
	}

	inline virtual CRiArchiveEnd *newRiArchiveEnd(long aLineNo) {
		return new CRiArchiveEnd(aLineNo);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiProjection(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiProjection *newRiProjection(
		long aLineNo,
		const char *name,
		const CParameterList &parameters)
	{
		return new CRiProjection(aLineNo, name, parameters);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiImager(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiImager *newRiImager(
		long aLineNo,
		const char *name,
		const CParameterList &parameters)
	{
		return new CRiImager(aLineNo, name, parameters);
	}

	inline virtual CRiQuantize *newRiQuantize(long aLineNo, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
		return new CRiQuantize(aLineNo, type, one, qmin, qmax, ampl);
	}

	inline virtual CRiDisplayChannel *newRiDisplayChannel(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken channel,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisplayChannel(aLineNo, decl, curColorDescr, channel, n, tokens, params);
	}

	inline virtual CRiDisplayChannel *newRiDisplayChannel(
		long aLineNo,
		RtToken channel,
		const CParameterList &parameters)
	{
		return new CRiDisplayChannel(aLineNo, channel, parameters);
	}

	inline virtual CRiDisplay *newRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken name, RtToken type, RtToken mode,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisplay(aLineNo, decl, curColorDescr, name, type, mode, n, tokens, params);
	}

	inline virtual CRiDisplay *newRiDisplay(
		long aLineNo,
		RtToken name, RtToken type, RtToken mode,
		const CParameterList &parameters)
	{
		return new CRiDisplay(aLineNo, name, type, mode, parameters);
	}

	inline virtual CRiHider *newRiHider(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiHider(aLineNo, decl, curColorDescr, type, n, tokens, params);
	}

	inline virtual CRiHider *newRiHider(
		long aLineNo,
		const char *type,
		const CParameterList &parameters)
	{
		return new CRiHider(aLineNo, type, parameters);
	}

	inline virtual CRiColorSamples *newRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
		return new CRiColorSamples(aLineNo, N, nRGB, RGBn);
	}

	inline virtual CRiRelativeDetail *newRiRelativeDetail(long aLineNo, RtFloat relativedetail) {
		return new CRiRelativeDetail(aLineNo, relativedetail);
	}

	inline virtual CRiOption *newRiOption(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiOption(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiOption *newRiOption(
		long aLineNo,
		const char *name,
		const CParameterList &parameters)
	{
		return new CRiOption(aLineNo, name, parameters);
	}

	inline virtual CRiLightSource *newRiLightSource(
		CRenderState &state, const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiLightSource(state, name, n, tokens, params);
	}

	inline virtual CRiLightSource *newRiLightSource(
		CRenderState &state, const char *name,
		const CParameterList &parameters)
	{
		return new CRiLightSource(state, name, parameters);
	}

	inline virtual CRiAreaLightSource *newRiAreaLightSource(
		CRenderState &state, const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAreaLightSource(state, name, n, tokens, params);
	}

	inline virtual CRiAreaLightSource *newRiAreaLightSource(
		CRenderState &state, const char *name,
		const CParameterList &parameters)
	{
		return new CRiAreaLightSource(state, name, parameters);
	}

	inline virtual CRiIlluminate *newRiIlluminate(long aLineNo, RtLightHandle handleIdx, RtBoolean onoff) {
		return new CRiIlluminate(aLineNo, handleIdx, onoff);
	}

	inline virtual CRiAttribute *newRiAttribute(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAttribute(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiColor *newRiColor(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) {
		return new CRiColor(aLineNo, curColorDescr, Cs);
	}

	inline virtual CRiOpacity *newRiOpacity(long aLineNo, const CColorDescr &curColorDescr, RtColor Cs) {
		return new CRiOpacity(aLineNo, curColorDescr, Cs);
	}

	inline virtual CRiSurface *newRiSurface(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSurface(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiAtmosphere *newRiAtmosphere(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiAtmosphere(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiInterior *newRiInterior(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiInterior(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiExterior *newRiExterior(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiExterior(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiDisplacement *newRiDisplacement(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisplacement(aLineNo, decl, curColorDescr, name, n, tokens, params);
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

	inline virtual CRiGeometricApproximation *newRiGeometricApproximation(long aLineNo, RtToken type, RtFloat value) {
		return new CRiGeometricApproximation(aLineNo, type, value);
	}

	inline virtual CRiGeometricRepresentation *newRiGeometricRepresentation(long aLineNo, RtToken type) {
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

	inline virtual CRiTrimCurve *newRiTrimCurve(long aLineNo, RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDeformation(aLineNo, decl, curColorDescr, name, n, tokens, params);
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPolygon(aLineNo, decl, curColorDescr, nvertices, n, tokens, params);
	}

	inline virtual CRiGeneralPolygon *newRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nloops, RtInt *nverts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiGeneralPolygon(aLineNo, decl, curColorDescr, nloops, nverts, n, tokens, params);
	}

	inline virtual CRiPointsPolygons *newRiPointsPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPointsPolygons(aLineNo, decl, curColorDescr, npolys, nverts, verts, n, tokens, params);
	}

	inline virtual CRiPointsGeneralPolygons *newRiPointsGeneralPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPointsGeneralPolygons(aLineNo, decl, curColorDescr, npolys, nloops, nverts, verts, n, tokens, params);
	}

	inline virtual CRiPatch *newRiPatch(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPatch(aLineNo, decl, curColorDescr, type, n, tokens, params);
	}

	inline virtual CRiPatchMesh *newRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPatchMesh(aLineNo, decl, curColorDescr, ustep, vstep, type, nu, uwrap, nv, vwrap, n, tokens, params);
	}

	inline virtual CRiNuPatch *newRiNuPatch(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiNuPatch(aLineNo, decl, curColorDescr, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	}

	inline virtual CRiSubdivisionMesh *newRiSubdivisionMesh(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken scheme, RtInt nfaces, RtInt nverts[], RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floargs[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSubdivisionMesh(aLineNo, decl, curColorDescr, scheme, nfaces, nverts, verts, ntags, tags, nargs, intargs, floargs, n, tokens, params);
	}

	inline virtual CRiSphere *newRiSphere(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiSphere(aLineNo, decl, curColorDescr, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiCone *newRiCone(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCone(aLineNo, decl, curColorDescr, height, radius, thetamax, n, tokens, params);
	}

	inline virtual CRiCylinder *newRiCylinder(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCylinder(aLineNo, decl, curColorDescr, radius, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiHyperboloid *newRiHyperboloid(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiHyperboloid(aLineNo, decl, curColorDescr, point1, point2, thetamax, n, tokens, params);
	}

	inline virtual CRiParaboloid *newRiParaboloid(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiParaboloid(aLineNo, decl, curColorDescr, rmax, zmin, zmax, thetamax, n, tokens, params);
	}

	inline virtual CRiDisk *newRiDisk(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiDisk(aLineNo, decl, curColorDescr, height, radius, thetamax, n, tokens, params);
	}

	inline virtual CRiTorus *newRiTorus(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiTorus(aLineNo, decl, curColorDescr, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	}

	inline virtual CRiPoints *newRiPoints(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiPoints(aLineNo, decl, curColorDescr, npts, n, tokens, params);
	}

	inline virtual CRiCurves *newRiCurves(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt ustep, RtInt vstep, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiCurves(aLineNo, decl, curColorDescr, ustep, vstep, type, ncurves, nverts, wrap, n, tokens, params);
	}

	inline virtual CRiBlobby *newRiBlobby(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[],
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiBlobby(aLineNo, decl, curColorDescr, nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	}

	inline virtual CRiProcedural *newRiProcedural(long aLineNo, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc, IRi &ri) {
		return new CRiProcedural(aLineNo, data, bound, subdivfunc, freefunc, ri);
	}

	inline virtual CRiGeometry *newRiGeometry(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiGeometry(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiObjectInstance *newRiObjectInstance(long aLineNo, RtObjectHandle handle) {
		return new CRiObjectInstance(aLineNo, handle);
	}

	inline virtual CRiArchiveInstance *newRiArchiveInstance(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtArchiveHandle handle, const IArchiveCallback *aCallback,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiArchiveInstance(aLineNo, decl, curColorDescr, handle, aCallback, n, tokens, params);
	}

	inline virtual CRiMakeTexture *newRiMakeTexture(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeTexture(aLineNo, decl, curColorDescr, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeBump *newRiMakeBump(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeBump(aLineNo, decl, curColorDescr, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeLatLongEnvironment *newRiMakeLatLongEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeLatLongEnvironment(aLineNo, decl, curColorDescr, pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeCubeFaceEnvironment *newRiMakeCubeFaceEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeCubeFaceEnvironment(aLineNo, decl, curColorDescr, px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	}

	inline virtual CRiMakeShadow *newRiMakeShadow(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiMakeShadow(aLineNo, decl, curColorDescr, pic, tex, n, tokens, params);
	}

	inline virtual CRiArchiveRecord *newRiArchiveRecord(long aLineNo, RtToken type, const char *line)
	{
		return new CRiArchiveRecord(aLineNo, type, line);
	}

	inline virtual CRiReadArchive *newRiReadArchive(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString filename, const IArchiveCallback *callback,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiReadArchive(aLineNo, decl, curColorDescr, filename, callback, n, tokens, params);
	}

}; // CRManInterfaceFactory

}

#endif // _RICPP_RENDERSTATE_RIMACRO_H
