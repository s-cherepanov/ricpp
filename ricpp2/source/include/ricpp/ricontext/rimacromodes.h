#ifndef _RICPP_RICONTEXT_RIMACROMODES_H
#define _RICPP_RICONTEXT_RIMACROMODES_H

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

/** @file rimacromodes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for modes, @see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Modes
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begins the context
	 *
	 * CRiBegin is not used as a macro, but used for state control. 
	 *
	 */
	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begins a context.
	 */
	class CRiBegin : public CVarParamRManInterfaceCall {
	private:
		std::string m_name; ///< Name of the backend as string.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;

	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiBegin"; }

		inline virtual const char *className() const { return CRiBegin::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the backedn.
		 */
		inline CRiBegin(
			long aLineNo = -1,
			RtString aName = RI_NULL)
			: TypeParent(aLineNo), m_name(noNullStr(aName))
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aName Name of the backend.
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiBegin(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtString aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, RI_BEGIN, aName, decl, CParameterClasses(), curColorDescr, n, tokens, params), m_name(noNullStr(aName))
		{
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the backend.
		 *  @param theParameters Parsed parameter list.
		 */
		inline CRiBegin(
			long aLineNo,
			RtString aName,
			const CParameterList &theParameters
			)
			: TypeParent(aLineNo, theParameters), m_name(noNullStr(aName))
		{
		}


		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiBegin(const CRiBegin &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiBegin()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiBegin(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_BEGIN; }

		/** @brief Gets the name of the backend.
		 *
		 *  @return The name of the backend.
		 */
		inline RtString name() const
		{
			return m_name.c_str();
		}

		/** @brief Sets the name of the backend.
		 *
		 *  @param aName The name of the backend.
		 */
		inline void name(RtString aName)
		{
			m_name = noNullStr(aName);
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preBegin(*this, m_name.c_str(), parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doBegin(*this, m_name.c_str(), parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postBegin(*this, m_name.c_str(), parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Normally not called because Begin is not a RIB command
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		/** @brief Assignment.
		 *
		 *  @param c CRManInterfaceCall to assign
		 *  @return A reference to this object.
		 */
		inline CRiBegin &operator=(const CRiBegin &c)
		{
			if ( this == &c )
				return *this;

			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Ends the context
	 *
	 * CRiEnd is not used as a macro, but used for state control. 
	 *
	 */
	class CRiEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiEnd"; }

		inline virtual const char *className() const { return CRiEnd::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy
		 */
		inline CRiEnd(const CRiEnd &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiEnd()
		{
		}

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiEnd(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_END; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Normally not called because Begin is not a RIB command
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiEnd &CRiEnd::operator=(const CRiEnd &c)
		{
			if ( this == &c )
				return *this;

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiEnd


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begin of a resource block
	 */
	class CRiResourceBegin : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiResourceBegin(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preResourceBegin(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doResourceBegin(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postResourceBegin(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiResourceBegin

	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of a resource block
	 */
	class CRiResourceEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiResourceEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preResourceEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doResourceEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postResourceEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiResourceEnd

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Starts a frame block.
	 */
	class CRiFrameBegin : public CRManInterfaceCall {
		RtInt m_frameNumber; ///< A number to identify the frame.
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 *  @param number A frame number.
		 */
		inline CRiFrameBegin(long aLineNo=-1, RtInt number=0)
			: TypeParent(aLineNo), m_frameNumber(number)
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
			ri.preFrameBegin(*this, m_frameNumber);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doFrameBegin(*this, m_frameNumber);
		}
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postFrameBegin(*this, m_frameNumber);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(frameNumber());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiFrameBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Ends a frame block.
	 */
	class CRiFrameEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number at a rib file where the declare statement starts, -1 if there is no file.
		 */
		inline CRiFrameEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preFrameEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doFrameEnd(*this);
		}
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postFrameEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiFrameEnd


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begin of a world block.
	 *
	 *  @see CRManInterfaceCall
	 *  @see IRi::worldBegin()
	 */
	class CRiWorldBegin : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiWorldBegin(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preWorldBegin(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doWorldBegin(*this);
		}
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postWorldBegin(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiWorldBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of a world block.
	 *
	 *  @see CRManInterfaceCall
	 *  @see IRi::worldEnd()
	 */
	class CRiWorldEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiWorldEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preWorldEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doWorldEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postWorldEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiWorldEnd

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begin of an attribute block
	 */
	class CRiAttributeBegin : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiAttributeBegin(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preAttributeBegin(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doAttributeBegin(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postAttributeBegin(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiAttributeBegin

	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of an attribute block
	 */
	class CRiAttributeEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiAttributeEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preAttributeEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doAttributeEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postAttributeEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiAttributeEnd

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begin of a transformation block
	 */
	class CRiTransformBegin : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiTransformBegin(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preTransformBegin(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doTransformBegin(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postTransformBegin(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTransformBegin

	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of a transformation block
	 */
	class CRiTransformEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file
		 */
		inline CRiTransformEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preTransformEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doTransformEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postTransformEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTransformEnd

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begin of a solid block.
	 */
	class CRiSolidBegin : public CRManInterfaceCall {
	private:
		RtToken m_type; ///< Token that indicates the solid operation (like RI_UNION), must be atomized (@see CToken)
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file.
		 *  @param aType The token of the operation (must be an atomized string, @see CToken).
		 */
		inline CRiSolidBegin(long aLineNo=-1, RtToken aType=RI_PRIMITIVE) : TypeParent(aLineNo), m_type(aType) { }

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
		 * @param aType The token of the operation (must be an atomized string, @see CToken)
		 */
		inline void type(RtToken aType)
		{
			m_type = aType;
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preSolidBegin(*this, m_type);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doSolidBegin(*this, m_type);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postSolidBegin(*this, m_type);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSolidBegin

	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of a solid block.
	 */
	class CRiSolidEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo Line number of a Rib Archive, -1 if there is no such file.
		 */
		inline CRiSolidEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preSolidEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doSolidEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postSolidEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
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
	protected:
		typedef CRManInterfaceCall TypeParent;
	private:
		RtObjectHandle m_handle; ///< Handle used to identify the object
		RtToken m_name;          ///< Name of the object, used to generate the handle.
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aHandle An object handle
		 *  @param aName Name may be a RIB string handle
		 */
		inline CRiObjectBegin(
			long aLineNo = -1,
			RtObjectHandle aHandle = illObjectHandle,
			RtToken aName = RI_NULL)
			: TypeParent(aLineNo)
		{
			m_handle = aHandle;
			m_name = aName;
		}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiObjectBegin(const CRiObjectBegin &c)
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
		
		/** Gets the name of the object.
		 *
		 * @return Name of the object.
		 */
		inline virtual RtToken name() const { return m_name; }

		/** Sets the name of the object.
		 *
		 * The name is set by the constructor and usually not changed. The name
		 * is used to create the handle. An object can overwrite another one with the same name.
		 *
		 * @param aName Name of the object.
		 */
		inline virtual void name(RtToken aName) { m_name = aName; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preObjectBegin(*this, name());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doObjectBegin(*this, name());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postObjectBegin(*this, name());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, const char *aName, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(aName);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, unsigned long handleNo, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			// Special handling by rib writer
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(handleNo);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			name(c.name());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiObjectBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Ends an object block of retained geometry.
	 *
	 *  CRiObjectEnd requests are used to control macro definitions and
	 *  instances are not stored at a macro themselves.
	 */
	class CRiObjectEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiObjectEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preObjectEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doObjectEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postObjectEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiObjectEnd


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Begins the block of an archive.
	 *
	 *  CRiArchiveBegin requests are used to control macro definitions and
	 *  instances are not stored at a macro themselves.
	 */
	class CRiArchiveBegin : public CVarParamRManInterfaceCall {
	private:
		RtArchiveHandle m_handle; ///< Associated archive handle to identify the object (constructed by IDoRender::preArchiveBegin()).
		std::string m_name;       ///< Name of the archive, used to generate the handle.
	protected:
		typedef CRManInterfaceCall TypeParent;
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
		 *  @param aHandle Given archive handle.
		 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
		 */
		inline CRiArchiveBegin(long aLineNo = -1, RtArchiveHandle aHandle=illArchiveHandle, const char *aName = 0)
			: CVarParamRManInterfaceCall(aLineNo)
		{
			m_handle = aHandle;
			m_name = noNullStr(aName);
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param decl Dictonary with the current declarations.
		 *  @param curColorDescr Current color descriptor.
		 *  @param aHandle Given archive handle.
		 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
		 *  @param n Number of parameters (size of @a tokens, @a params).
		 *  @param tokens Tokens of the request.
		 *  @param params Parameter values of the request.
		 */
		inline CRiArchiveBegin(
			long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
			RtArchiveHandle aHandle, RtToken aName,
			RtInt n, RtToken tokens[], RtPointer params[])
			: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
		{
			m_handle = aHandle;
			m_name = noNullStr(aName);
		}

		/** @brief Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aHandle Given archive handle.
		 *  @param aName Name of the archive. If empty, a name will generated by the framework (TemplHandleStack).
		 *  @param theParameters Parameter values of the request.
		 */
		inline CRiArchiveBegin(
			long aLineNo,
			RtArchiveHandle aHandle, RtToken aName,
			const CParameterList &theParameters)
			: CVarParamRManInterfaceCall(aLineNo, theParameters)
		{
			m_handle = aHandle;
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
			ri.preArchiveBegin(*this, name(), parameters());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doArchiveBegin(*this, name(), parameters());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postArchiveBegin(*this, name(), parameters());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(name());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
	}; // CRiArchiveBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Ends the block of an archive.
	 *
	 *  CRiArchiveEnd requests are used to control macro definitions and
	 *  instances are not stored at a macro themselves.
	 */
	class CRiArchiveEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiArchiveEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preArchiveEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doArchiveEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postArchiveEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiArchiveEnd

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Start of a motion block.
	 *
	 *  @todo Count the time stamps within a motion block at the render state.
	 */
	class CRiMotionBegin : public CRManInterfaceCall {
	private:
		std::vector<RtFloat> m_motionVars; ///< The motion "time stamps"
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiMotionBegin(long aLineNo=-1) : TypeParent(aLineNo)
		{
		}

		/** @brief Constructor.
		 *
		 *  Start of a motion block. The motion block contains n requests. Each requests is associated with a
		 *  time sample using the vector f of "time stamps".
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known).
		 *  @param n Number of "time stamps".
		 *  @param f Vector of "time stamps", size is @a n.
		 */
		inline CRiMotionBegin(long aLineNo, RtInt n, RtFloat *f) :
			TypeParent(aLineNo)
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
		 * @retval m A reference to a vector to retrieve the motion vars.
		 */
		inline void get(std::vector<RtFloat> &m) const
		{
			m = m_motionVars;
		}

		/** Sets the vector with the motion vars.
		 *
		 * @param m A reference to a vector with motion vars.
		 */
		inline void set(const std::vector<RtFloat> m)
		{
			m_motionVars = m;
		}

		/** Sets the vector with the motion vars.
		 *
		 *  @param n Number of "time stamps".
		 *  @param f Vector of "time stamps", size is @a n.
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
			ri.preMotionBegin(*this, 
				static_cast<RtInt>(m_motionVars.size()),
				m_motionVars.empty() ? 0 : &m_motionVars[0]
			);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doMotionBegin(*this, 
				static_cast<RtInt>(m_motionVars.size()),
				m_motionVars.empty() ? 0 : &m_motionVars[0]
			);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postMotionBegin(*this, 
				static_cast<RtInt>(m_motionVars.size()),
				m_motionVars.empty() ? 0 : &m_motionVars[0]
			);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(static_cast<RtInt>(m_motionVars.size()),
							   m_motionVars.empty() ? 0 : &m_motionVars[0]);
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiMotionBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief End of a motion block.
	 */
	class CRiMotionEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiMotionEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

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
			ri.preMotionEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doMotionEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postMotionEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiMotionEnd


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Start of an if block.
	 */
	class CRiIfBegin : public CRManInterfaceCall {
		std::string m_exprStr; ///< The expression of the if block
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets the name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiIfBegin"; }

		inline virtual const char *className() const { return CRiIfBegin::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param anExprStr The expression of the if block.
		 */
		inline CRiIfBegin(long aLineNo=-1, RtString anExprStr=0) : TypeParent(aLineNo), m_exprStr(noNullStr(anExprStr)) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiIfBegin(const CRiIfBegin &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiIfBegin()
		{
		}
		
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiIfBegin(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_IF_BEGIN; }

		/** @brief Gets the string with the expression.
		 *
		 *  @return The expression of the if block.
		 */
		inline RtString exprStr() const
		{
			return m_exprStr.c_str();
		}

		/** @brief Sets the string with the expression.
		 *
		 *  @param anExprStr The expression of the if block.
		 */
		inline void exprStr(RtString anExprStr)
		{
			m_exprStr = noNullStr(anExprStr);
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preIfBegin(*this, exprStr());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doIfBegin(*this, exprStr());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postIfBegin(*this, exprStr());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putString(exprStr());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiIfBegin &operator=(const CRiIfBegin &c)
		{
			if ( this == &c )
				return *this;
			exprStr(c.exprStr());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiIfBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Start of the else if part of an if block.
	 */
	class CRiElseIfBegin : public CRManInterfaceCall {
		std::string m_exprStr; ///< The expression of the if block
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets the name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiElseIfBegin"; }

		inline virtual const char *className() const { return CRiElseIfBegin::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param anExprStr The expression of the if block.
		 */
		inline CRiElseIfBegin(long aLineNo=-1, const char *anExprStr=0) : TypeParent(aLineNo), m_exprStr(noNullStr(anExprStr)) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiElseIfBegin(const CRiElseIfBegin &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiElseIfBegin()
		{
		}
		
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiElseIfBegin(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ELSE_IF; }

		/** @brief Gets the string with the expression.
		 *
		 *  @return The expression of the if block.
		 */
		inline const char *exprStr() const
		{
			return m_exprStr.c_str();
		}

		/** @brief Sets the string with the expression.
		 *
		 *  @param anExprStr The expression of the if block.
		 */
		inline void exprStr(const char *anExprStr)
		{
			m_exprStr = noNullStr(anExprStr);
		}

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preElseIfBegin(*this, exprStr());
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doElseIfBegin(*this, exprStr());
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postElseIfBegin(*this, exprStr());
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putString(exprStr());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiElseIfBegin &operator=(const CRiElseIfBegin &c)
		{
			if ( this == &c )
				return *this;
			exprStr(c.exprStr());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiElseIfBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Start of the else part of an if block.
	 */
	class CRiElseBegin : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets the name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiElseBegin"; }

		inline virtual const char *className() const { return CRiElseBegin::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiElseBegin(long aLineNo=-1) : TypeParent(aLineNo) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiElseBegin(const CRiElseBegin &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiElseBegin()
		{
		}
		
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiElseBegin(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_ELSE; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preElseBegin(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doElseBegin(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postElseBegin(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiElseBegin &operator=(const CRiElseBegin &c)
		{
			if ( this == &c )
				return *this;

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiElseBegin


	///////////////////////////////////////////////////////////////////////////////
	/** @brief End an if block.
	 */
	class CRiIfEnd : public CRManInterfaceCall {
	protected:
		typedef CRManInterfaceCall TypeParent;
	public:
		/** @brief Gets the name for the class.
		 *
		 *  @return The name of the class (can be used as atomized string)
		 */
		inline static const char *myClassName(void) { return "CRiIfEnd"; }

		inline virtual const char *className() const { return CRiIfEnd::myClassName(); }

		inline virtual bool isA(const char *atomizedClassName) const
		{
			return ( atomizedClassName == myClassName() );
		}

		inline virtual bool isKindOf(const char *atomizedClassName) const
		{
			if ( atomizedClassName == myClassName() )
				return true;
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default Constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiIfEnd(long aLineNo=-1) : TypeParent(aLineNo) {}

		/** @brief Copy constructor.
		 *
		 *  @param c Object to copy.
		 */
		inline CRiIfEnd(const CRiIfEnd &c)
		{
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiIfEnd()
		{
		}
		
		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiIfEnd(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_IF_END; }

		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preIfEnd(*this);
		}

		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doIfEnd(*this);
		}

		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postIfEnd(*this);
		}

		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
		}
		
		/** @brief Assignment
		 *
		 * @param c Object to assign
		 * @return A reference to this object
		 */
		inline CRiIfEnd &operator=(const CRiIfEnd &c)
		{
			if ( this == &c )
				return *this;

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiIfEnd

}
#endif // // _RICPP_RICONTEXT_RIMACROMODES_H
