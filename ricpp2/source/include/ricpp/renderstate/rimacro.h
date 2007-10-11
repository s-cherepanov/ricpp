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

#include <list>

namespace RiCPP {

enum EnumMacroTypes {
	MACROTYPE_UNKNOWN = 0, //!< Macro type is yet unknown
	MACROTYPE_OBJECT,      //!< Macro contains interface calls for an object definition
	MACROTYPE_ARCHIVE      //!< Macro contains interface calls for an rin archive definition
};

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
	 *  @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known).
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
	 *  @param state The renderer backend status.
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.replayRequest(*this);
	}

	/** @brief Replays the interface call.
	 *
	 *  @param ri The renderer backend used for replay.
	 *  @param cb Archive callback.
	 */
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		replay(ri);
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
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri)
	{
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
	 * @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known).
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
	 * @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known).
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
	 * @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known).
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
	 * @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known).
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
 *
 * @todo Replay single frames.
 */
class CRiMacro {
	std::list<CRManInterfaceCall *> m_callList;  //!< List of all interface calls for this macro.
	std::string m_name;         //!< Name of the macro (eg. file name, handle name).
	EnumMacroTypes m_macroType; //!< Type of macro, either object or archive.
	bool m_stopInsertion;       //!< Can be set to false to indicate to stop inserting request into the macro.
public:
	/** @brief Constructor initializes the macro.
	 *
	 *  @param aName Name of the macro, eg. file name.
	 *  @param isObject Macro defines a geometric object.
	 */
	inline CRiMacro(
		RtString aName = "",
		EnumMacroTypes macroType = MACROTYPE_UNKNOWN) :
		m_name(noNullStr(aName)), m_macroType(macroType), m_stopInsertion(false)
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

	/** @brief Replays archive at a back end renderer.
	 *
	 *  @param ri The back end renderer used to replay.
	 */
	void replay(IDoRender &ri);

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

	EnumMacroTypes macroType() const
	{
		return m_macroType;
	}

	void macroType(EnumMacroTypes aMacroType)
	{
		m_macroType = aMacroType;
	}
}; // CRiMacro


class CRiObjectMacro : public CRiMacro {
private:
	RtObjectHandle m_handle;
public:
	inline CRiObjectMacro() : CRiMacro("", MACROTYPE_OBJECT) {}
	inline RtObjectHandle handle() const { return m_handle; }
	inline void handle(RtObjectHandle h) { m_handle = h; }
};

class CRiArchiveMacro : public CRiMacro {
private:
	RtArchiveHandle m_handle;
public:
	inline CRiArchiveMacro() : CRiMacro("", MACROTYPE_ARCHIVE) {}
	inline RtArchiveHandle handle() const { return m_handle; }
	inline void handle(RtArchiveHandle h) { m_handle = h; }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Not suported: Frontend functions
// (REQ_ERROR_HANDLER)
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

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_ERROR_HANDLER; }

	//! Replays the errorhandler statement for a given renderer
	/*! @param ri Instance of a renderer, used to replay the errorhandler statement
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.preErrorHandler(*m_handler);
		ri.doErrorHandler(*m_handler);
	}

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
	RtToken m_name;            //!< Atomized name of the declarated variable
	std::string m_declaration; //!< The declaration, like: varying float[3]
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline static const char *myClassName(void) { return "CRiDeclare"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CRiDeclare::myClassName(); }

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

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	/** @brief Pre-Processes an interface call
	 *
	 *  Pre-Processing normally involves state update of the renderer.
	 *  Is also performed before macro insertion.
	 *
	 *  @param ri The renderer backend used for pre-processing.
	 */
	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preDeclare(name(), declaration(), false);
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
		ri.doDeclare(name(), declaration());
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
		ri.postDeclare(name(), declaration());
	}
}; // CRiDeclare

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

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CRiFrameBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preFrameBegin(m_frameNumber);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doFrameBegin(m_frameNumber);
	}
	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postFrameBegin(m_frameNumber);
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

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string).
	 */
	inline virtual const char *className() const { return CRiFrameEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameEnd(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFrameEnd()
	{
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_END; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preFrameEnd();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doFrameEnd();
	}
	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postFrameEnd();
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
	inline static const char *myClassName(void) { return "CRiWorldBegin"; }
	inline virtual const char *className() const { return CRiWorldBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	inline CRiWorldBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	//! Copy constructor
	/** @param c Object to copy
	 */
	inline CRiWorldBegin(const CRiWorldBegin &c)
	{
		*this = c;
	}

	//! Destructor, frees the resources
	inline virtual ~CRiWorldBegin()
	{
	}

	//! Cloning
	/** @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiWorldBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_BEGIN; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preWorldBegin();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doWorldBegin();
	}
	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postWorldBegin();
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
	inline static const char *myClassName(void) { return "CRiWorldEnd"; }
	inline virtual const char *className() const { return CRiWorldEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	//! Constructor
	/** @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiWorldEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	//! Copy constructor
	/** @param c Object to copy
	 */
	inline CRiWorldEnd(const CRiWorldEnd &c)
	{
		*this = c;
	}

	//! Destructor, frees the resources
	inline virtual ~CRiWorldEnd()
	{
	}

	//! Cloning
	/** @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiWorldEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_END; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preWorldEnd();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doWorldEnd();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postWorldEnd();
	}
}; // CRiWorldEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of an attribute block
 */
class CRiAttributeBegin : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiAttributeBegin"; }
	inline virtual const char *className() const { return CRiAttributeBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	//! Constructor
	/** @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiAttributeBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	//! Copy constructor
	/** @param c Object to copy
	 */
	inline CRiAttributeBegin(const CRiAttributeBegin &c)
	{
		*this = c;
	}

	//! Destructor, frees the resources
	inline virtual ~CRiAttributeBegin()
	{
	}

	//! Cloning
	/** @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiAttributeBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_BEGIN; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preAttributeBegin();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doAttributeBegin();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postAttributeBegin();
	}
}; // CRiAttributeBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of an attribute block
 */
class CRiAttributeEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiAttributeEnd"; }
	inline virtual const char *className() const { return CRiAttributeEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	//! Constructor
	/** @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiAttributeEnd(long aLineNo) : CRManInterfaceCall(aLineNo) {}

	//! Copy constructor
	/** @param c Object to copy
	 */
	inline CRiAttributeEnd(const CRiAttributeEnd &c)
	{
		*this = c;
	}

	//! Destructor, frees the resources
	inline virtual ~CRiAttributeEnd()
	{
	}

	//! Cloning
	/** @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiAttributeEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_END; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preAttributeEnd();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doAttributeEnd();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postAttributeEnd();
	}
}; // CRiAttributeEnd

///////////////////////////////////////////////////////////////////////////////
/** @brief Begin of a transformation block
 */
class CRiTransformBegin : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiTransformBegin"; }
	inline virtual const char *className() const { return CRiTransformBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	//! Constructor
	/** @param aLineNo Line number of a Rib Archive, -1 if there is no such file
	 */
	inline CRiTransformBegin(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	//! Copy constructor
	/** @param c Object to copy
	 */
	inline CRiTransformBegin(const CRiTransformBegin &c)
	{
		*this = c;
	}

	//! Destructor, frees the resources
	inline virtual ~CRiTransformBegin()
	{
	}

	//! Cloning
	/** @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTransformBegin(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_BEGIN; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preTransformBegin();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doTransformBegin();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postTransformBegin();
	}
}; // CRiTransformBegin

///////////////////////////////////////////////////////////////////////////////
/** @brief End of a transformation block
 */
class CRiTransformEnd : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiTransformEnd"; }
	inline virtual const char *className() const { return CRiTransformEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	/** @brief Constructor,
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

	/** @brief Cloning.
	 *
	 *  @return a clone of this object
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTransformEnd(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_END; }

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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preTransformEnd();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doTransformEnd();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postTransformEnd();
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

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiSolidBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSolidBegin(*this);
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preSolidBegin(m_type);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doSolidBegin(m_type);
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postSolidBegin(m_type);
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

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiSolidEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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
	 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file.
	 *  @param anOperation The token of the operation (must be an atomized string, @see CToken).
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

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSolidEnd(*this);
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiSolidEnd()
	{
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_END; }

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


	inline virtual void preProcess(IDoRender &ri)
	{
		ri.preSolidEnd();
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		ri.doSolidEnd();
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		ri.postSolidEnd();
	}
}; // CRiSolidEnd

#if 0
///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiObjectBegin : public CRManInterfaceCall {
protected:
	RtObjectHandle m_handleIdx;

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiObjectBegin"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiObjectBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	inline CRiLightSource(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
	{
		m_handleIdx = illObjectHandle;
	}

	inline CRiObjectBegin(long aLineNo, RtObjectHandle *h) : CRManInterfaceCall(aLineNo)
	{
		assert(h != 0);
		m_handlePtr = h;
	}

	inline CRiObjectBegin(const CRiSolidEnd &c)
	{
		*this = c;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiObjectBegin(*this);
	}

	inline virtual ~CRiObjectBegin()
	{
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_BEGIN; }
	inline virtual void replay(IDoRender &ri) {
		assert(m_handlePtr != 0);
		if ( m_handlePtr != 0 ) {
			*m_handlePtr = ri.preObjectBegin();
			ri.doObjectBegin(*m_handlePtr);
		} else {
			RtObjectHandle h = ri.preObjectBegin();
			ri.doObjectBegin(h);
		}
	}
	inline virtual RtObjectHandle *handlePtr() { return m_handlePtr; }
	inline CRiObjectBegin &operator=(const CRiObjectBegin &c) {
		if ( this == &c )
			return *this;
		
		// ???

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiObjectBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiObjectEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiObjectEnd"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiObjectEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	inline CRiObjectEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}

	inline CRiObjectEnd(const CRiSolidEnd &c)
	{
		*this = c;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiObjectEnd(*this);
	}

	inline virtual ~CRiObjectEnd()
	{
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_END; }
	inline virtual void replay(IDoRender &ri) {
		ri.preObjectEnd();
		ri.doObjectEnd();
	}
	inline CRiObjectEnd &operator=(const CRiObjectEnd &c) {
		if ( this == &c )
			return *this;
		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiObjectEnd
#endif

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiMotionBegin : public CRManInterfaceCall {
	std::vector<RtFloat> m_motionVars; //!< The motion "time stamps"

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMotionBegin"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiMotionBegin::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	inline CRiMotionBegin(long aLineNo=-1) :
		CRManInterfaceCall(aLineNo)
	{
	}

	inline CRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) :
		CRManInterfaceCall(aLineNo)
	{
		motionVars(n, f);
	}

	inline CRiMotionBegin(const CRiSolidEnd &c)
	{
		*this = c;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiMotionBegin(*this);
	}

	inline virtual ~CRiMotionBegin()
	{
	}
	
	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_BEGIN; }

	std::vector<RtFloat> &motionVars()
	{
		return m_motionVars;
	}

	const std::vector<RtFloat> &motionVars() const
	{
		return m_motionVars;
	}

	void motionVars(std::vector<RtFloat> m)
	{
		m_motionVars = m;
	}

	void motionVars(RtInt n, RtFloat *f)
	{
		m_motionVars.resize(0);
		if ( n > 0 ) {
			m_motionVars.resize(n);
			m_motionVars.assign(f, f+n);
		}
	}

	inline virtual void replay(IDoRender &ri)
	{
		ri.preMotionBegin(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
		ri.doMotionBegin(
			static_cast<RtInt>(m_motionVars.size()),
			m_motionVars.empty() ? 0 : &m_motionVars[0]
		);
	}

	inline CRiMotionBegin &operator=(const CRiMotionBegin &c) {
		if ( this == &c )
			return *this;
		motionVars(c.motionVars());
		CRManInterfaceCall::operator=(c);
	 	return *this;
	}
}; // CRiMotionBegin

///////////////////////////////////////////////////////////////////////////////
//! \sa CRManInterfaceCall
class CRiMotionEnd : public CRManInterfaceCall {
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiMotionEnd"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiMotionEnd::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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

	inline CRiMotionEnd(long aLineNo=-1) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_END; }
	inline virtual void replay(IDoRender &ri) {
		ri.preMotionEnd();
		ri.doMotionEnd();
	}
	inline CRiMotionEnd &operator=(const CRiMotionEnd &c) {
		if ( this == &c )
			return *this;
		CRManInterfaceCall::operator=(c);
	 	return *this;
	}
}; // CRiMotionEnd


///////////////////////////////////////////////////////////////////////////////
/** @brief The raster resolution and pixel aspect ratio of the image to be rendered.
 */
class CRiFormat : public CRManInterfaceCall {
	RtInt m_xres,     //!< Width of the image to be rendered.
	      m_yres;     //!< Height of the image to be rendered.
	RtFloat m_aspect; //!< Pixel aspect ratio (width to the height).
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFormat"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiFormat::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known)
	 *  @param name Atomized name of the projection
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

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFormat(*this);
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	/** @brief Replays the content.
	 *
	 * State of the renderer is already checked before this function is
	 * called by CRiMacro::replay().
	 *
	 *  @param ri Back end renderer
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.preFormat(m_xres, m_yres, m_aspect);
		ri.doFormat(m_xres, m_yres, m_aspect);
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

		xres(c.xres());
		yres(c.yres());
		aspect(c.aspect());

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
	RtFloat m_aspect; //!< frame aspect ratio
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFrameAspectRatio"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiFrameAspectRatio::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known)
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

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameAspectRatio(*this);
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	/** @brief Replays the content.
	 *
	 * State of the renderer is already checked before this function is
	 * called by CRiMacro::replay().
	 *
	 *  @param ri Back end renderer.
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.preFrameAspectRatio(m_aspect);
		ri.doFrameAspectRatio(m_aspect);
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

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiScreenWindow::myClassName(); }

	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known)
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
	inline CRiScreenWindow(const CRiFrameAspectRatio &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiScreenWindow()
	{
	}

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiScreenWindow(*this);
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
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

	/** @brief Replays the content.
	 *
	 * State of the renderer is already checked before this function is
	 * called by CRiMacro::replay().
	 *
	 *  @param ri Back end renderer
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.preScreenWindow(m_left, m_right, m_bottom, m_top);
		ri.doScreenWindow(m_left, m_right, m_bottom, m_top);
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

		left(c.left());
		right(c.right());
		bottom(c.bottom());
		top(c.top());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiScreenWindow

///////////////////////////////////////////////////////////////////////////////
/** @brief Give a rectangular cropped region of the image in screen coordinates.
 */
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
		ri.doCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}
	// inline CRiCropWindow &operator=(const CRiCropWindow &) {
	//	return *this;
	// }
}; // CRiCropWindow

///////////////////////////////////////////////////////////////////////////////
//! \sa CVarParamRManInterfaceCall
class CRiProjection : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Atomized projection name (RI_PERSPECTIVE, RI_ORTHOGRAPHIC)

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiProjection"; }

	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline virtual const char *className() const { return CRiProjection::myClassName(); }


	/** @brief Checks if instance belongs to a specific class.
	 *
	 *  @param atomizedClassName Atomized class name (got from a static myClassName() call)
	 *  @return true, if instance belongs to specific class atomizedClassName
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
		return ( atomizedClassName == myClassName() );
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known)
	 *  @param name Atomized name of the projection
	 */
	inline CRiProjection(
		long aLineNo = -1,
		RtToken aName = defProjection)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param name Atomized name of the projection
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiProjection(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params),
		m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if alineNo is initialized to -1 (a line number is not known)
	 * @param name Atomized name of the projection
	 * @param parameters Parsed parameter list.
	 */
	inline CRiProjection(
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
	inline CRiProjection(const CRiProjection &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiProjection()
	{
	}

	/** @brief Cloning.
	 *
	 *  @return A clone of this object.
	 */
	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiProjection(*this);
	}

	/** @brief Gets the interface number of the corresponding RI request.
	 *
	 *  @return Interface number of the corresponding RI request.
	 */
	inline virtual EnumRequests interfaceIdx() const { return REQ_PROJECTION; }

	/** @brief Replays the interface call.
	 *
	 *  @param ri The renderer backend used for replay.
	 *  @param state The renderer backend status.
	 */
	inline virtual void replay(IDoRender &ri)
	{
		ri.preProjection(name(), parameters());
		ri.doProjection(name(), parameters());
	}

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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preClipping(m_hither, m_yon);
		ri.doClipping(m_hither, m_yon);
	}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
		ri.doClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preDepthOfField(m_fstop, m_focallength, m_focaldistance);
		ri.doDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preShutter(m_smin, m_smax);
		ri.doShutter(m_smin, m_smax);
	}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.prePixelVariance(m_variation);
		ri.doPixelVariance(m_variation);
	}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.prePixelSamples(m_xsamples, m_ysamples);
		ri.doPixelSamples(m_xsamples, m_ysamples);
	}
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
	inline virtual void replay(IDoRender &ri) {
		assert(m_function != 0);
		if ( m_function ) {
			ri.prePixelFilter(*m_function, m_xwidth, m_ywidth);
			ri.doPixelFilter(*m_function, m_xwidth, m_ywidth);
		}
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preExposure(m_gain, m_gamma);
		ri.doExposure(m_gain, m_gamma);
	}
	inline CRiExposure &operator=(const CRiExposure &) {
		return *this;
	}
}; // CRiExposure

///////////////////////////////////////////////////////////////////////////////
class CRiImager : public CVarParamRManInterfaceCall {
	RtToken m_name;
public:
	inline static const char *myClassName(void) { return "CRiImager"; }
	inline virtual const char *className() const { return CRiImager::myClassName(); }

	inline CRiImager(
		long aLineNo = -1,
		RtToken aName = defImagerName)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}
	inline CRiImager(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiImager(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IMAGER; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri)
	{
		ri.preImager(m_name, parameters());
		ri.doImager(m_name, parameters());
	}
	inline CRiImager &operator=(const CRiImager &c) {
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preQuantize(m_type.c_str(), m_one, m_qmin, m_qmax, m_ampl);
		ri.doQuantize(m_type.c_str(), m_one, m_qmin, m_qmax, m_ampl);
	}
	inline CRiQuantize &operator=(const CRiQuantize &) {
		return *this;
	}
}; // CRiQuantize

///////////////////////////////////////////////////////////////////////////////
class CRiDisplayChannel : public CVarParamRManInterfaceCall {
protected:
	std::string m_channel;
public:
	inline static const char *myClassName(void) { return "CRiDisplayChannel"; }
	inline virtual const char *className() const { return CRiDisplayChannel::myClassName(); }

	inline CRiDisplayChannel(
		long aLineNo = -1,
		RtString aChannel = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_channel(aChannel)
	{
	}
	inline CRiDisplayChannel(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString channel,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_channel(channel)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiDisplayChannel(
		long aLineNo,
		RtToken aChannel,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_channel(aChannel)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY_CHANNEL; }
	inline RtString channel() const
	{
		return m_channel.c_str();
	}
	inline void channel(RtToken aChannel)
	{
		m_channel = aChannel;
	}
	inline virtual void replay(IDoRender &ri)
	{
		ri.preDisplayChannel(m_channel.c_str(), parameters());
		ri.doDisplayChannel(m_channel.c_str(), parameters());
	}
	inline CRiDisplayChannel &operator=(const CRiDisplayChannel &c) {
		if ( this == &c )
			return *this;

		channel(c.channel());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplayChannel

///////////////////////////////////////////////////////////////////////////////
class CRiDisplay : public CVarParamRManInterfaceCall {
	RtToken m_name, m_type, m_mode;
public:
	inline static const char *myClassName(void) { return "CRiDisplay"; }
	inline virtual const char *className() const { return CRiDisplay::myClassName(); }

	inline CRiDisplay(
		long aLineNo = -1,
		RtToken aName = RI_NULL, RtToken aType = RI_NULL, RtToken aMode = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName), m_type(aType), m_mode(aMode)
	{
	}
	inline CRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName, RtToken aType, RtToken aMode,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName), m_type(aType), m_mode(aMode)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiDisplay(
		long aLineNo,
		RtToken aName, RtToken aType, RtToken aMode,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName), m_type(aType), m_mode(aMode)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline RtToken type() const
	{
		return m_type;
	}
	inline void type(RtToken aType)
	{
		m_type = aType;
	}
	inline RtToken mode() const
	{
		return m_mode;
	}
	inline void mode(RtToken aMode)
	{
		m_mode = aMode;
	}
	inline virtual void replay(IDoRender &ri)
	{
		ri.preDisplay(m_name, m_type, m_mode, parameters());
		ri.doDisplay(m_name, m_type, m_mode, parameters());
	}

	inline CRiDisplay &operator=(const CRiDisplay &c) {
		if ( this == &c )
			return *this;

		name(c.name());
		type(c.type());
		mode(c.mode());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplay

///////////////////////////////////////////////////////////////////////////////
class CRiHider : public CVarParamRManInterfaceCall {
protected:
	RtToken m_type;
public:
	inline static const char *myClassName(void) { return "CRiHider"; }
	inline virtual const char *className() const { return CRiHider::myClassName(); }

	inline CRiHider(
		long aLineNo = -1,
		RtToken aType = defHiderType)
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
	}
	inline CRiHider(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aType,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiHider(
		long aLineNo,
		RtToken aType,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_type(aType)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_HIDER; }
	inline RtToken type() const
	{
		return m_type;
	}
	inline void type(RtToken aType)
	{
		m_type = aType;
	}
	inline virtual void replay(IDoRender &ri)
	{
		ri.preHider(m_type, parameters());
		ri.preHider(m_type, parameters());
	}
	inline CRiHider &operator=(const CRiHider &c) {
		if ( this == &c )
			return *this;

		type(c.type());

		CVarParamRManInterfaceCall::operator=(c);
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
	inline virtual void replay(IDoRender &ri) {
		ri.preColorSamples(
			m_N,
			m_nRGB.empty() ? 0 : &m_nRGB[0],
			m_RGBn.empty() ? 0 : &m_RGBn[0]);
		ri.doColorSamples(
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preRelativeDetail(m_relativedetail);
		ri.doRelativeDetail(m_relativedetail);
	}
	inline CRiRelativeDetail &operator=(const CRiRelativeDetail &) {
		return *this;
	}
}; // CRiRelativeDetail

///////////////////////////////////////////////////////////////////////////////
class CRiOption : public CVarParamRManInterfaceCall {
protected:
	RtToken m_name;
public:
	inline static const char *myClassName(void) { return "CRiOption"; }
	inline virtual const char *className() const { return CRiOption::myClassName(); }

	inline CRiOption(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}
	inline CRiOption(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline CRiOption(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OPTION; }
	inline RtToken name() const
	{
		return m_name;
	}
	inline void name(RtToken aName)
	{
		m_name = aName;
	}
	inline virtual void replay(IDoRender &ri)
	{
		ri.preOption(m_name, parameters());
		ri.doOption(m_name, parameters());
	}
	inline CRiOption &operator=(const CRiOption &c) {
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiOption

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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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

	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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

///////////////////////////////////////////////////////////////////////////////
class CRiIdentity : public CRManInterfaceCall {
public:
	inline static const char *myClassName(void) { return "CRiIdentity"; }
	inline virtual const char *className() const { return CRiIdentity::myClassName(); }

	inline CRiIdentity(long aLineNo) : CRManInterfaceCall(aLineNo) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }
	inline virtual void replay(IDoRender &ri)
	{
		ri.preSphere(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
		ri.doSphere(m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual EnumRequests interfaceIdx() const { return REQ_TORUS; }
	inline virtual void replay(IDoRender &ri)
	{
		ri.preTorus(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
		ri.doTorus(m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt npts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_npts(npts)
	{
		CPointsClasses p(npts);
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preGeometry(m_name.c_str(), parameters());
		ri.doGeometry(m_name.c_str(), parameters());
	}
	inline CRiGeometry &operator=(const CRiGeometry &) {
		return *this;
	}
}; // CRiGeometry

///////////////////////////////////////////////////////////////////////////////
class CRiObjectInstance : public CRManInterfaceCall {
protected:
	RtObjectHandle m_handle;
public:
	inline static const char *myClassName(void) { return "CRiObjectInstance"; }
	inline virtual const char *className() const { return CRiObjectInstance::myClassName(); }

	inline CRiObjectInstance(long aLineNo, RtObjectHandle handle) : CRManInterfaceCall(aLineNo), m_handle(handle)
	{
		assert(handle != 0);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_INSTANCE; }
	inline virtual void replay(IDoRender &ri)
	{
		assert(m_handle != 0);
		if ( m_handle != 0 ) {
			ri.preObjectInstance(m_handle);
			ri.doObjectInstance(m_handle);
		}
	}
	inline virtual RtObjectHandle handle() { return m_handle; }
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
	inline virtual void replay(IDoRender &ri) {
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preArchiveRecord(m_type.c_str(), m_line.c_str());
		ri.doArchiveRecord(m_type.c_str(), m_line.c_str());
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *callback)
	{
		ri.preArchiveRecord(m_type.c_str(), m_line.c_str());
		ri.doArchiveRecord(m_type.c_str(), m_line.c_str());
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
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString filename, const IArchiveCallback *callback,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_filename(filename)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);

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
	inline virtual void replay(IDoRender &ri)
	{
		ri.preReadArchive(m_filename.c_str(), m_callback, parameters());
		ri.doReadArchive(m_filename.c_str(), m_callback, parameters());
	}
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *callback)
	{
		ri.preReadArchive(m_filename.c_str(), callback, parameters());
		ri.doReadArchive(m_filename.c_str(), callback, parameters());
	}
	inline CRiReadArchive &operator=(const CRiReadArchive &) {
		return *this;
	}
}; // CRiReadArchive


///////////////////////////////////////////////////////////////////////////////
//! Interface, return a new instance of a IRi... class.
class CRManInterfaceFactory {
public:
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

	/*
	inline virtual CRiObjectBegin *newRiObjectBegin(long aLineNo, RtObjectHandle *h) {
		return new CRiObjectBegin(aLineNo, h);
	}

	inline virtual CRiObjectEnd *newRiObjectEnd(long aLineNo) {
		return new CRiObjectEnd(aLineNo);
	}
	*/

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
