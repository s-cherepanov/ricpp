#ifndef _RICPP_RICONTEXT_RIMACROPRIMS_H
#define _RICPP_RICONTEXT_RIMACROPRIMS_H

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

/** @file rimacroprims.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The macro classes for primitives, @see rimacro.h
 */

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Graphical primitives
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief Simple polygon.
 */
class CRiPolygon : public CPolygonRManInterfaceCall {
private:
	RtInt m_nVertices; ///< Number of vertices of the polygon must match the number of positions "P".
protected:
	typedef CPolygonRManInterfaceCall TypeParent;
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPolygon"; }
	inline virtual const char *className() const { return CRiPolygon::myClassName(); }

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
	 * The default constructor sets the number of vertices to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPolygon(long aLineNo=-1)
		: TypeParent(aLineNo), m_nVertices(0)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param theNVertices Number of vertices of the polygon
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt theNVertices,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TypeParent(aLineNo, decl, CPolygonClasses(theNVertices), curColorDescr, n, tokens, params),
		  m_nVertices(theNVertices)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param theNVertices Number of vertices of the polygon
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiPolygon(
		long aLineNo,
		RtInt theNVertices,
		const CParameterList &theParameters
		)
		: TypeParent(aLineNo, theParameters), m_nVertices(theNVertices)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPolygon(const CRiPolygon &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPolygon()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPolygon(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_POLYGON; }

	/** @brief Gets the number of vertices.
	 *
	 *  @return The number of vertices.
	 */
	inline RtInt nVertices() const
	{
		return m_nVertices;
	}

	/** @brief Sets the number of vertices.
	 *
	 *  @param theNVertices The number of vertices.
	 */
	inline void nVertices(RtInt theNVertices)
	{
		m_nVertices = theNVertices;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePolygon(*this, m_nVertices, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPolygon(*this, m_nVertices, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPolygon(*this, m_nVertices, parameters());
	}

	inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
	{
		ribWriter.putRequest(interfaceIdx());
		TypeParent::writeRIB(ribWriter, n, ignoreTokens);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPolygon &operator=(const CRiPolygon &c)
	{
		if ( this == &c )
			return *this;

		nVertices(c.nVertices());

		TypeParent::operator=(c);
		return *this;
	}
}; // CRiPolygon


///////////////////////////////////////////////////////////////////////////////
/** @brief General Polygon
 */
class CRiGeneralPolygon : public CPolygonRManInterfaceCall {
private:
	std::vector<RtInt> m_nVerts; ///< Number of verts per outline (Number of loops is the size of m_nVerts).
	/** @brief enters the values.
	 *
	 *  @param theNLoops Number of loops
	 *  @param theNVerts Number of vertices/loop, size of @a nverts vector is @a nloops.
	 */
	void enterValues(RtInt theNLoops, const RtInt theNVerts[]);
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiGeneralPolygon"; }
	inline virtual const char *className() const { return CRiGeneralPolygon::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CPolygonRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of vertices to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiGeneralPolygon(long aLineNo=-1)
		: CPolygonRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param theNLoops Number of loops
	 *  @param theNVerts Number of vertices/loop, size of @a nverts vector is @a nloops.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	CRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt theNLoops, const RtInt theNVerts[],
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param theNLoops Number of loops
	 *  @param theNVerts Number of vertices/loop, size of @a nverts vector is @a nloops.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiGeneralPolygon(
		long aLineNo,
		RtInt theNLoops, const RtInt theNVerts[],
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiGeneralPolygon(const CRiGeneralPolygon &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiGeneralPolygon()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiGeneralPolygon(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_GENERAL_POLYGON; }

	/** @brief Gets the number of loops.
	 *
	 *  @return The number of loops.
	 */
	inline RtInt nLoops() const
	{
		return (RtInt)(m_nVerts.size());;
	}

	/** @brief Gets the number of vertices per loop.
	 *
	 *  @return The number of vertices (size is the number of loops).
	 */
	inline const std::vector<RtInt> &nVerts() const
	{
		return m_nVerts;
	}

	/** @brief Gets the number of positions.
	 *
	 *  @return The number of positions.
	 */
	inline RtInt numPos() const
	{
		return (RtInt)sum(m_nVerts.size(), &(m_nVerts[0]));
	}

	/** @brief Dets the number of vertices and loops.
	 *
	 *  @param theNVerts The number of vertices (size is the number of loops).
	 */
	inline void nVerts(const std::vector<RtInt> &theNVerts)
	{
		m_nVerts = theNVerts;
	}

	/** @brief Sets the number of loops and vertices.
	 *
	 *  @param theNLoops The number of loops.
	 *  @param theNVerts The number of vertices.
	 */
	inline void set(RtInt theNLoops, const RtInt theNVerts[])
	{
		enterValues(theNLoops, theNVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeneralPolygon(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doGeneralPolygon(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postGeneralPolygon(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiGeneralPolygon &operator=(const CRiGeneralPolygon &c)
	{
		if ( this == &c )
			return *this;

		nVerts(c.nVerts());

		CPolygonRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiGeneralPolygon

///////////////////////////////////////////////////////////////////////////////
/** @brief Simple polygon net.
 */
class CRiPointsPolygons : public CPolygonRManInterfaceCall {
private:
	std::vector<RtInt> m_nVerts; ///< Number of vertices per polygon (size is the number of polygons).
	std::vector<RtInt> m_verts;  ///< Indices of the vertices.

	/** @brief enters the values.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNVerts Number of verts per polygon.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	void enterValues(RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[]);
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPointsPolygons"; }
	inline virtual const char *className() const { return CRiPointsPolygons::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CPolygonRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of polygons to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPointsPolygons(long aLineNo=-1)
		: CPolygonRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param theNPolys Number of polygons.
	 *  @param theNVerts Number of verts per polygon.
	 *  @param theVerts The verts (indices) of the polygons.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	CRiPointsPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[],
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param theNPolys Number of polygons.
	 *  @param theNVerts Number of verts per polygon.
	 *  @param theVerts The verts (indices) of the polygons.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiPointsPolygons(
		long aLineNo,
		RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[],
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPointsPolygons(const CRiPointsPolygons &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPointsPolygons()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPointsPolygons(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }

	/** @brief Gets the number of loops.
	 *
	 *  @return The number of loops.
	 */
	inline RtInt nPolys() const
	{
		return (RtInt)(m_nVerts.size());;
	}

	/** @brief Gets the numbers of the vertices.
	 *
	 *  @return The numbers of the vertices (size is the number of polygons).
	 */
	inline const std::vector<RtInt> &nVerts() const
	{
		return m_nVerts;
	}

	/** @brief Gets the indices of the vertices.
	 *
	 *  @return The indizes of the vertices.
	 */
	inline const std::vector<RtInt> &verts() const
	{
		return m_verts;
	}

	/** @brief Gets the number of the positions.
	 *
	 *  @return The number of the positions.
	 */
	inline RtInt numPos() const
	{
		if ( m_verts.size() == 0 )
			return 0;
		return 1+(RtInt)tmax(m_verts.size(), &(m_verts[0]));
	}

	/** @brief Sets the vertex indices.
	 *
	 *  @param theNVerts Number of vertices per polygon (size is the number of polygons).
	 *  @param theVerts Indices of the vertices.
	 */
	inline void set(const std::vector<RtInt> &theNVerts, const std::vector<RtInt> &theVerts)
	{
		m_nVerts = theNVerts;
		m_verts = theVerts;

		assert(m_verts.size() == sum(m_nVerts.size(), &(m_nVerts[0])));
	}

	/** @brief Sets the vertex indices.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNVerts Number of verts per polygon.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	inline void set(RtInt theNPolys, const RtInt theNVerts[], const RtInt theVerts[])
	{
		enterValues(theNPolys, theNVerts, theVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsPolygons(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPointsPolygons(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPointsPolygons(*this, 
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPointsPolygons &operator=(const CRiPointsPolygons &c)
	{
		if ( this == &c )
			return *this;

		set(c.nVerts(), c.verts());

		CPolygonRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPointsPolygons

///////////////////////////////////////////////////////////////////////////////
/** @brief General polygon net.
 */
class CRiPointsGeneralPolygons : public CPolygonRManInterfaceCall {
private:
	std::vector<RtInt> m_nLoops; ///< Loops per polygon (Number of polygons is the size of the vector).
	std::vector<RtInt> m_nVerts; ///< Vertices per loop.
	std::vector<RtInt> m_verts;  ///< Vertex indices.

	/** @brief Enters the values.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	void enterValues(RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[]);
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPointsGeneralPolygons"; }
	inline virtual const char *className() const { return CRiPointsGeneralPolygons::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CPolygonRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of polygons to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPointsGeneralPolygons(long aLineNo=-1)
		: CPolygonRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param theNPolys Number of polygons.
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	CRiPointsGeneralPolygons(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[],
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param theNPolys Number of polygons.
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiPointsGeneralPolygons(
		long aLineNo,
		RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[],
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPointsGeneralPolygons(const CRiPointsGeneralPolygons &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPointsGeneralPolygons()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPointsGeneralPolygons(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }

	/** @brief Gets the number of the positions.
	 *
	 *  @return The number of the positions.
	 */
	inline RtInt numPos() const
	{
		if ( m_verts.size() == 0 )
			return 0;
		return 1+(RtInt)tmax(m_verts.size(), &(m_verts[0]));
	}

	/** @brief Gets the number of polygons.
	 *
	 *  @return The number of polygons.
	 */
	RtInt nPolys() const
	{
		return (RtInt)m_nLoops.size();
	}

	/** @brief Gets the number of loops per polygon.
	 *
	 *  @return The number of loops per polygon.
	 */
	const std::vector<RtInt> &nLoops() const
	{
		return m_nLoops;
	}

	/** @brief Gets the number of vertices per loops.
	 *
	 *  @return The number of vertices per loops.
	 */
	const std::vector<RtInt> &nVerts() const
	{
		return m_nVerts;
	}

	/** @brief Gets the indices of the vertices.
	 *
	 *  @return The indices of the vertices.
	 */
	const std::vector<RtInt> &verts() const
	{
		return m_verts;
	}

	/** @brief Sets the vertex indices.
	 *
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	inline void set(
		const std::vector<RtInt> &theNLoops,
		const std::vector<RtInt> &theNVerts,
		const std::vector<RtInt> &theVerts)
	{
		m_nLoops = theNLoops;
		m_nVerts = theNVerts;
		m_verts  = theVerts;

		assert(m_nVerts.size() == sum(m_nLoops.size(), &(m_nLoops[0])));
		assert(m_verts.size() == sum(m_nVerts.size(), &(m_nVerts[0])));
	}

	/** @brief Sets the vertex indices.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	inline void set(RtInt theNPolys, const RtInt theNLoops[], const RtInt theNVerts[], const RtInt theVerts[])
	{
		enterValues(theNPolys, theNLoops, theNVerts, theVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsGeneralPolygons(*this, 
			(RtInt)m_nLoops.size(),
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPointsGeneralPolygons(*this, 
			(RtInt)m_nLoops.size(),
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPointsGeneralPolygons(*this, 
			(RtInt)m_nLoops.size(),
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPointsGeneralPolygons &operator=(const CRiPointsGeneralPolygons &c)
	{
		if ( this == &c )
			return *this;

		set(c.nLoops(), c.nVerts(), c.verts());

		CPolygonRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPointsGeneralPolygons


///////////////////////////////////////////////////////////////////////////////
/** @brief Bilinear or bicubic patch
 */
class CRiPatch : public CUVRManInterfaceCall {
private:
	RtToken m_type; ///< Type of the patch, either RI_BILINEAR or RI_BICUBIC (or RI_NULL if not set).

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPatch"; }
	inline virtual const char *className() const { return CRiPatch::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of polygons to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPatch(long aLineNo=-1)
		: CUVRManInterfaceCall(aLineNo)
	{
		m_type = RI_NULL;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aType Type of the patch.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiPatch(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aType,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CPatchClasses(aType), curColorDescr, n, tokens, params),
		  m_type(aType)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Type of the patch.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiPatch(
		long aLineNo,
		RtToken aType,
		const CParameterList &theParameters)
		: CUVRManInterfaceCall(aLineNo, theParameters), m_type(aType)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPatch(const CRiPatch &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPatch()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPatch(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }

	/** @brief Gets the type of the attribute as atomized string.
	 *
	 *  @return The type of the attribute as atomized string.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets the type of the attribute as atomized string.
	 *
	 *  @param aType The type of the attribute as atomized string.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatch(*this, m_type, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPatch(*this, m_type, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPatch(*this, m_type, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPatch &operator=(const CRiPatch &c)
	{
		if ( this == &c )
			return *this;

		type(c.type());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPatch


///////////////////////////////////////////////////////////////////////////////
/** @brief a mesh of bilinear or bicubic patches.
 */
class CRiPatchMesh : public CUVRManInterfaceCall {
private:
	RtToken m_type,  ///< Type of the mesh either RI_BILINEAR or RI_BICUBIC.
	        m_uwrap, ///< Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC.
	        m_vwrap; ///< Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
	RtInt   m_nu,    ///< Number of control points in parametric u direction.
	        m_nv;    ///< Number of control points in parametric v direction.
	RtInt   m_ustep, ///< Step m_ustep control points to the next patch in u direction (from the basis).
	        m_vstep; ///< Step m_vstep control points to the next patch in v direction (from the basis).

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPatchMesh"; }
	inline virtual const char *className() const { return CRiPatchMesh::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of polygons to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	CRiPatchMesh(long aLineNo = -1) :
		CUVRManInterfaceCall(aLineNo)
	{
		m_type = RI_NULL;
		m_uwrap = RI_NULL;
		m_vwrap = RI_NULL;
		m_nu = 0;
		m_nv = 0;
		m_ustep = 0;
		m_vstep = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl     Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aUStep   Number of control points to the next patch in u direction (from the basis).
	 *  @param aVStep   Number of control points to the next patch in v direction (from the basis).
	 *  @param aType    Type of the mesh either RI_BILINEAR or RI_BICUBIC.
	 *  @param aNu      Number of control points in u direction.
	 *  @param aUWrap   Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC
	 *  @param aNv      Number of control points in v direction.
	 *  @param aVWrap   Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
	 *  @param n        Number of parameters (size of @a tokens, @a params).
	 *  @param tokens   Tokens of the request.
	 *  @param params   Parameter values of the request.
	 */
	CRiPatchMesh(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt aUStep, RtInt aVStep,
		RtToken aType,
		RtInt aNu, RtToken aUWrap,
		RtInt aNv, RtToken aVWrap,
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aUStep   Number of control points to the next patch in u direction (from the basis).
	 *  @param aVStep   Number of control points to the next patch in v direction (from the basis).
	 *  @param aType    Type of the mesh either RI_BILINEAR or RI_BICUBIC.
	 *  @param aNu      Number of control points in u direction.
	 *  @param aUWrap   Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC
	 *  @param aNv      Number of control points in v direction.
	 *  @param aVWrap   Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiPatchMesh(
		long aLineNo,
		RtInt aUStep, RtInt aVStep,
		RtToken aType,
		RtInt aNu, RtToken aUWrap,
		RtInt aNv, RtToken aVWrap,
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPatchMesh(const CRiPatchMesh &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPatchMesh()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPatchMesh(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH_MESH; }

	/** @brief Get the type of the mesh.
	 *
	 * @return The type of the mesh, either RI_BILINEAR or RI_BICUBIC.
	 */
	RtToken type() const
	{
		return m_type;
	}

	/** @brief Wrap of the mesh in u direction.
	 *
	 * @return The wrap of the mesh in u direction.
	 */
	RtToken uWrap() const
	{
		return m_uwrap;
	}

	/** @brief Wrap of the mesh in v direction.
	 *
	 * @return The wrap of the mesh in v direction.
	 */
	RtToken vWrap() const
	{
		return m_vwrap;
	}
	
	/** @brief Number of control points in parametric u direction.
	 *
	 * @return The number of control points in parametric u direction.
	 */
	RtInt nu() const
	{
		return m_nu;
	}

	/** @brief Number of control points in parametric v direction.
	 *
	 * @return The number of control points in parametric v direction.
	 */
	RtInt nv() const
	{
		return m_nv;
	}

	/** @brief Number of control points to the next patch in u direction (from the basis).
	 *
	 * @return The number of control points to the next patch in u direction (from the basis).
	 */
	RtInt uStep() const
	{
		return m_ustep;
	}

	/** @brief Number of control points to the next patch in v direction (from the basis).
	 *
	 * @return The number of control points to the next patch in v direction (from the basis).
	 */
	RtInt vStep() const
	{
		return m_vstep;
	}

	/** @brief Enter the element variable using the interface parameters.
	 *
	 *  @param aUStep Number of control points to the next patch in u direction.
	 *  @param aVStep Number of control points to the next patch in v direction.
	 *  @param aType  Type of the mesh either RI_BILINEAR or RI_BICUBIC.
	 *  @param aNu    Number of control points in u direction.
	 *  @param aUWrap Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC
	 *  @param aNv    Number of control points in v direction.
	 *  @param aVWrap Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
	 */
	void set(
		RtInt aUStep, RtInt aVStep,
		RtToken aType,
		RtInt aNu, RtToken aUWrap,
		RtInt aNv, RtToken aVWrap);

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatchMesh(*this, m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPatchMesh(*this, m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPatchMesh(*this, m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPatchMesh &operator=(const CRiPatchMesh &c)
	{
		if ( this == &c )
			return *this;

		set(c.uStep(), c.vStep(), c.type(), c.nu(), c.uWrap(), c.nv(), c.vWrap());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPatchMesh


///////////////////////////////////////////////////////////////////////////////
/** @brief NURBS patch.
 */
class CRiNuPatch : public CGeometryRManInterfaceCall {
private:
	RtInt m_nu,     ///< Number of control points in parametric u direction.
	      m_uorder, ///< Order of the polynomial basis for parameters u.
	      m_nv,     ///< Number of control points in parametric v direction.
	      m_vorder; ///< Order of the polynomial basis for parameters v.
	std::vector<RtFloat> m_uknot, ///< Knot vector for the u parameters.
	                     m_vknot; ///< Knot vector for the v parameters.

	RtFloat m_umin, ///< Minimal value for parameter u.
	        m_umax, ///< Maximal value for parameter u.
	        m_vmin, ///< Minimal value for parameter v.
	        m_vmax; ///< Maximal value for parameter v.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiNuPatch"; }
	inline virtual const char *className() const { return CRiNuPatch::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of control points to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	CRiNuPatch(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo)
	{
		m_nu = m_uorder = 0;
		m_nv = m_vorder = 0;
		m_umin = m_umax = 0;
		m_vmin = m_vmax = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl     Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aNU      Number of control points in parametric u direction.
	 *  @param aUOrder  Order of the polynomial basis for parameters u.
	 *  @param aUKnot   Knot vector for the u parameters.
	 *  @param aUMin    Minimal value for parameter u.
	 *  @param aUMax    Maximal value for parameter u.
	 *  @param aNV      Number of control points in parametric v direction.
	 *  @param aVOrder  Order of the polynomial basis for parameters v.
	 *  @param aVKnot   Knot vector for the v parameters.
	 *  @param aVMin    Minimal value for parameter v.
	 *  @param aVMax    Maximal value for parameter v.
	 *  @param n        Number of parameters (size of @a tokens, @a params).
	 *  @param tokens   Tokens of the request.
	 *  @param params   Parameter values of the request.
	 */
	CRiNuPatch(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax,
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aNU      Number of control points in parametric u direction.
	 *  @param aUOrder  Order of the polynomial basis for parameters u.
	 *  @param aUKnot   Knot vector for the u parameters.
	 *  @param aUMin    Minimal value for parameter u.
	 *  @param aUMax    Maximal value for parameter u.
	 *  @param aNV      Number of control points in parametric v direction.
	 *  @param aVOrder  Order of the polynomial basis for parameters v.
	 *  @param aVKnot   Knot vector for the v parameters.
	 *  @param aVMin    Minimal value for parameter v.
	 *  @param aVMax    Maximal value for parameter v.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiNuPatch(
		long aLineNo,
		RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax,
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiNuPatch(const CRiNuPatch &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiNuPatch()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiNuPatch(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_NU_PATCH; }

	/** @brief Gets the number of segments for the NURBS patch.
	 *
	 *  @return The number of segments for the NURBS patch.
	 */
	inline virtual RtInt segments() const { return (1+m_nu-m_uorder)*(1+m_nv-m_vorder); }

	/** @brief Enter the values into member variables.
	 *
	 *  @param aNU Number of control points in parametric u direction.
	 *  @param aUOrder Order of the polynomial basis for parameters u.
	 *  @param aUKnot Knot vector for the u parameters.
	 *  @param aUMin Minimal value for parameter u.
	 *  @param aUMax Maximal value for parameter u.
	 *  @param aNV Number of control points in parametric v direction.
	 *  @param aVOrder Order of the polynomial basis for parameters v.
	 *  @param aVKnot Knot vector for the v parameters.
	 *  @param aVMin Minimal value for parameter v.
	 *  @param aVMax Maximal value for parameter v.
	 */
	void set(
		RtInt aNU, RtInt aUOrder, const RtFloat aUKnot[], RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat aVKnot[], RtFloat aVMin, RtFloat aVMax);

	/** @brief Enter the values into member variables.
	 *
	 *  @param aNU Number of control points in parametric u direction.
	 *  @param aUOrder Order of the polynomial basis for parameters u.
	 *  @param aUKnot Knot vector for the u parameters.
	 *  @param aUMin Minimal value for parameter u.
	 *  @param aUMax Maximal value for parameter u.
	 *  @param aNV Number of control points in parametric v direction.
	 *  @param aVOrder Order of the polynomial basis for parameters v.
	 *  @param aVKnot Knot vector for the v parameters.
	 *  @param aVMin Minimal value for parameter v.
	 *  @param aVMax Maximal value for parameter v.
	 */
	void set(
		RtInt aNU, RtInt aUOrder, const std::vector<RtFloat> &aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const std::vector<RtFloat> &aVKnot, RtFloat aVMin, RtFloat aVMax);

	/** @brief Gets the number of control points in parametric u direction.
	 *
	 *  @return The number of control points in parametric u direction.
	 */
	RtInt nu() const
	{
		return m_nu;
	}

	/** @brief Gets the number of control points in parametric v direction.
	 *
	 *  @return The number of control points in parametric v direction.
	 */
	RtInt nv() const
	{
		return m_nv;
	}

	/** @brief Gets the order of the polynomial basis for parameters u.
	 *
	 *  @return the order of the polynomial basis for parameters u.
	 */
	RtInt uOrder() const
	{
		return m_uorder;
	}

	/** @brief Gets the order of the polynomial basis for parameters v.
	 *
	 *  @return The order of the polynomial basis for parameters v.
	 */
	RtInt vOrder() const
	{
		return m_vorder;
	}

	/** @brief Gets the minimal value for parameter u.
	 *
	 *  @return The minimal value for parameter u.
	 */
	RtFloat uMin() const
	{
		return m_umin;
	}

	/** @brief Gets the maximal value for parameter u.
	 *
	 *  @return The maximal value for parameter u.
	 */
	RtFloat uMax() const
	{
		return m_umax;
	}

	/** @brief Gets the minimal value for parameter v.
	 *
	 *  @return The minimal value for parameter v.
	 */
	RtFloat vMin() const
	{
		return m_vmin;
	}

	/** @brief Gets the maximal value for parameter v.
	 *
	 *  @return The maximal value for parameter v.
	 */
	RtFloat vMax() const
	{
		return m_vmax;
	}

	/** @brief Gets the knot vector for the u parameters.
	 *
	 *  @return The knot vector for the u parameters.
	 */
	const std::vector<RtFloat> &uKnot() const
	{
		return m_uknot;
	}

	/** @brief Gets the knot vector for the v parameters.
	 *
	 *  @return The knot vector for the v parameters.
	 */
	const std::vector<RtFloat> &vKnot() const
	{
		return m_vknot;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preNuPatch(*this, 
			m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doNuPatch(*this, 
			m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postNuPatch(*this, 
			m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiNuPatch &operator=(const CRiNuPatch &c)
	{
		if ( this == &c )
			return *this;

		set(c.nu(), c.uOrder(), c.uKnot(), c.uMin(), c.uMax(), c.nv(), c.vOrder(), c.vKnot(), c.vMin(), c.vMax());

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiNuPatch


///////////////////////////////////////////////////////////////////////////////
/** @brief Subdivision mesh surface.
 */
class CRiSubdivisionMesh : public CGeometryRManInterfaceCall {
private:
	RtToken m_scheme;    ///< The scheme (currently RI_CATMULL_CLARK only).
	RtInt   m_nvertices, ///< The number of vertices (maximum+1 of m_verts).
	        m_nedges;    ///< Number of edges (eq. number of vertices) of the surface.
	std::vector<RtInt>      m_nverts,  ///< Number of vertices for each face (size is the number of faces).
	                        m_verts,   ///< Index for each vertex.
	                        m_nargs,   ///< Number of integer and float args (integer, float) for each tag (size = 2*m_tags.size()).
	                        m_intargs; ///< Integer arguments for tags.
	std::vector<RtFloat>    m_floargs; ///< Float arguments for tags.
	std::vector<RtToken>    m_tags;    ///< Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiSubdivisionMesh"; }
	inline virtual const char *className() const { return CRiSubdivisionMesh::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of faces to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	CRiSubdivisionMesh(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo)
	{
		m_scheme = RI_NULL;
		m_nvertices = 0;
		m_nedges = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl     Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param n        Number of parameters (size of @a tokens, @a params).
	 *  @param tokens   Tokens of the request.
	 *  @param params   Parameter values of the request.
	 */
	CRiSubdivisionMesh(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param tokenMap Current tokens.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiSubdivisionMesh(
		long aLineNo, CTokenMap *tokenMap, 
		RtToken aScheme, RtInt aNFaces, RtInt const aNVerts[], RtInt const aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[],
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiSubdivisionMesh(const CRiSubdivisionMesh &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiSubdivisionMesh()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSubdivisionMesh(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SUBDIVISION_MESH; }
	
	/** @brief Sets the values of the member variables.
	 *
	 *  @param tokenMap For tags.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 */
	void set(
		CTokenMap *tokenMap, 
		RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[]);

	/** @brief Sets the values of the member variables.
	 *
	 *  @param tokenMap  For tags.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 */
	void set(
		CTokenMap *tokenMap, 
		RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
		const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
		const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs);

	/** @brief The scheme (currently RI_CATMULL_CLARK only).
	 *
	 *  @return The scheme (currently RI_CATMULL_CLARK only).
	 */
	RtToken scheme() const
	{
		return m_scheme;
	}

	/** @brief Gets the number of vertices (maximum+1 of m_verts).
	 *
	 * @return The number of vertices (maximum+1 of m_verts).
	 */
	RtInt nVertices() const
	{
		return m_nvertices;
	}

	/** @brief Gets the number of edges (eq. number of vertices) of the surface.
	 *
	 * @return The Number of edges (eq. number of vertices) of the surface.
	 */
	RtInt nEdges() const
	{
		return m_nedges;
	}

	/** @brief Gets the number of vertices for each face (size is the number of faces).
	 *
	 * @return Number of vertices for each face (size is the number of faces).
	 */
	const std::vector<RtInt> &nVerts() const
	{
		return m_nverts;
	}

	/** @brief Gets the index for each vertex.
	 *
	 * @return The index for each vertex.
	 */
	const std::vector<RtInt> &verts() const
	{
		return m_verts;
	}

	/** @brief Gets the Number of integer and float args.
	 *
	 * @return The Number of integer and float args (integer, float) for each tag (size = 2*m_tags.size()).
	 */
	const std::vector<RtInt> &nArgs() const
	{
		return m_nargs;
	}

	/** @brief Gets the integer arguments for tags.
	 *
	 * @return The integer arguments for tags.
	 */
	const std::vector<RtInt> &intArgs() const
	{
		return m_intargs;
	}

	/** @brief Gets the float arguments for tags.
	 *
	 * @return The float arguments for tags.
	 */
	const std::vector<RtFloat> &floatArgs() const
	{
		return m_floargs;
	}

	/** @brief Gets the tags.
	 *
	 * @return The tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.
	 */
	const std::vector<RtToken> &tags() const
	{
		return m_tags;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			parameters()
			);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			parameters()
			);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			parameters()
			);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiSubdivisionMesh &operator=(const CRiSubdivisionMesh &c)
	{
		if ( this == &c )
			return *this;

		set(0, c.scheme(), c.nVerts(), c.verts(), c.tags(), c.nArgs(), c.intArgs(), c.floatArgs()); 

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSubdivisionMesh


///////////////////////////////////////////////////////////////////////////////
/** @brief Hierarchical subdivision mesh surface.
 */
class CRiHierarchicalSubdivisionMesh : public CGeometryRManInterfaceCall {
private:
	RtToken m_scheme;    ///< The scheme (currently RI_CATMULL_CLARK only).
	RtInt   m_nvertices, ///< The number of vertices (maximum+1 of m_verts).
	        m_nedges;    ///< Number of edges (eq. number of vertices) of the surface.
	std::vector<RtInt>       m_nverts,  ///< Number of vertices for each face (size is the number of faces).
	                         m_verts,   ///< Index for each vertex.
	                         m_nargs,   ///< Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	                         m_intargs; ///< Integer arguments for tags.
	std::vector<RtFloat>     m_floargs; ///< Float arguments for tags.
	std::vector<std::string> m_strargs; ///< String arguments for tags.
	std::vector<RtToken>     m_strptrargs; ///< String arguments for tags.
	std::vector<RtToken>     m_tags;    ///< Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiHierarchicalSubdivisionMesh"; }
	inline virtual const char *className() const { return CRiHierarchicalSubdivisionMesh::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of faces to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	CRiHierarchicalSubdivisionMesh(long aLineNo=-1) : CGeometryRManInterfaceCall(aLineNo)
	{
		m_scheme = RI_NULL;
		m_nvertices = 0;
		m_nedges = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl     Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param aStrArgs String arguments for tags.
	 *  @param n        Number of parameters (size of @a tokens, @a params).
	 *  @param tokens   Tokens of the request.
	 *  @param params   Parameter values of the request.
	 */
	CRiHierarchicalSubdivisionMesh(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[], const RtToken aStrArgs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param tokenMap Current toklens (for tags)
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param aStrArgs String arguments for tags.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiHierarchicalSubdivisionMesh(
		long aLineNo, CTokenMap *tokenMap, 
		RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[], const RtToken aStrArgs[],
		const CParameterList &theParameters);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiHierarchicalSubdivisionMesh(const CRiHierarchicalSubdivisionMesh &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiHierarchicalSubdivisionMesh()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiHierarchicalSubdivisionMesh(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_HIERARCHICAL_SUBDIVISION_MESH; }
	
	/** @brief Sets the values of the member variables.
	 *
	 *  @param tokenMap Current tokensfor the tags.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNFaces  Number of faces
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aNTags   Number of tags.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param aStrArgs String arguments for tags.
	 */
	void set(
		CTokenMap *tokenMap, 
		RtToken aScheme, RtInt aNFaces, const RtInt aNVerts[], const RtInt aVerts[],
		RtInt aNTags, const RtToken aTags[], const RtInt aNArgs[], const RtInt aIntArgs[], const RtFloat aFloArgs[], const RtToken aStrArgs[]);

	/** @brief Sets the values of the member variables.
	 *
	 *  @param tokenMap Current tokens for the tags.
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 *  @param aStrArgs String arguments for tags.
	 */
	void set(
		CTokenMap *tokenMap, 
		RtToken aScheme, const std::vector<RtInt> &aNVerts, const std::vector<RtInt> &aVerts,
		const std::vector<RtToken> &aTags, const std::vector<RtInt> &aNArgs,
		const std::vector<RtInt> &aIntArgs, const std::vector<RtFloat> &aFloArgs, const std::vector<RtToken> &aStrArgs);

	/** @brief The scheme (currently RI_CATMULL_CLARK only).
	 *
	 *  @return The scheme (currently RI_CATMULL_CLARK only).
	 */
	RtToken scheme() const
	{
		return m_scheme;
	}

	/** @brief Gets the number of vertices (maximum+1 of m_verts).
	 *
	 * @return The number of vertices (maximum+1 of m_verts).
	 */
	RtInt nVertices() const
	{
		return m_nvertices;
	}

	/** @brief Gets the number of edges (eq. number of vertices) of the surface.
	 *
	 * @return The Number of edges (eq. number of vertices) of the surface.
	 */
	RtInt nEdges() const
	{
		return m_nedges;
	}

	/** @brief Gets the number of vertices for each face (size is the number of faces).
	 *
	 * @return Number of vertices for each face (size is the number of faces).
	 */
	const std::vector<RtInt> &nVerts() const
	{
		return m_nverts;
	}

	/** @brief Gets the index for each vertex.
	 *
	 * @return The index for each vertex.
	 */
	const std::vector<RtInt> &verts() const
	{
		return m_verts;
	}

	/** @brief Gets the Number of integer and float args.
	 *
	 * @return The Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 */
	const std::vector<RtInt> &nArgs() const
	{
		return m_nargs;
	}

	/** @brief Gets the integer arguments for tags.
	 *
	 * @return The integer arguments for tags.
	 */
	const std::vector<RtInt> &intArgs() const
	{
		return m_intargs;
	}

	/** @brief Gets the float arguments for tags.
	 *
	 * @return The float arguments for tags.
	 */
	const std::vector<RtFloat> &floatArgs() const
	{
		return m_floargs;
	}

	/** @brief Gets the string arguments for tags.
	 *
	 * @return The string arguments for tags.
	 */
	const std::vector<RtToken> &stringPtrArgs() const
	{
		return m_strptrargs;
	}

	/** @brief Gets the tags.
	 *
	 * @return The tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.
	 */
	const std::vector<RtToken> &tags() const
	{
		return m_tags;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preHierarchicalSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strptrargs.empty() ? 0 : &(m_strptrargs[0]),
			parameters()
			);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doHierarchicalSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strptrargs.empty() ? 0 : &(m_strptrargs[0]),
			parameters()
			);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postHierarchicalSubdivisionMesh(*this, 
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strptrargs.empty() ? 0 : &(m_strptrargs[0]),
			parameters()
			);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiHierarchicalSubdivisionMesh &operator=(const CRiHierarchicalSubdivisionMesh &c)
	{
		if ( this == &c )
			return *this;

		set(0, c.scheme(), c.nVerts(), c.verts(), c.tags(), c.nArgs(), c.intArgs(), c.floatArgs(), c.stringPtrArgs()); 

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiHierarchicalSubdivisionMesh


///////////////////////////////////////////////////////////////////////////////
/** @brief Sphere.
 */
class CRiSphere : public CUVRManInterfaceCall {
private:
	RtFloat m_radius,   ///< Radius.
	        m_zmin,     ///< Minimal z coordinate (cut pole).
			m_zmax,     ///< Maximal z coordinate (cut pole).
			m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiSphere"; }
	inline virtual const char *className() const { return CRiSphere::myClassName(); }


	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 */
	CRiSphere(long aLineNo=-1,
		RtFloat aRadius = 0, RtFloat aZMin = 0, RtFloat aZMax = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiSphere(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiSphere(
		long aLineNo,
		RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiSphere(const CRiSphere &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiSphere()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiSphere(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }

	/** @brief Gets the radius of the sphere.
	 *
	 *  @return The radius of the sphere.
	 */
	RtFloat radius() const
	{
		return m_radius;
	}

	/** @brief Gets the radius of the sphere.
	 *
	 *  @param aRadius The radius of the sphere.
	 */
	void radius(RtFloat aRadius)
	{
		m_radius = aRadius;
	}

	/** @brief Gets the minimum z coordinate (cut pole).
	 *
	 * @return The minimum z coordinate (cut pole).
	 */
	RtFloat zMin() const
	{
		return m_zmin;
	}

	/** @brief Sets the minimum z coordinate (cut pole).
	 *
	 * @param aZMin The minimum z coordinate (cut pole).
	 */
	void zMin(RtFloat aZMin)
	{
		m_zmin = aZMin;
	}

	/** @brief Gets the maximum z coordinate (cut pole).
	 *
	 * @return The minimum z coordinate (cut pole).
	 */
	RtFloat zMax() const
	{
		return m_zmax;
	}

	/** @brief Sets the maximum z coordinate (cut pole).
	 *
	 * @param aZMax The maximum z coordinate (cut pole).
	 */
	void zMax(RtFloat aZMax)
	{
		m_zmax = aZMax;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a sphere.
	 *
	 *  @retval aRadius   Radius.
     *  @retval aZMin     Minimal z coordinate (cut pole).
     *  @retval aZMax     Maximal z coordinate (cut pole).
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aRadius, RtFloat &aZMin, RtFloat &aZMax, RtFloat &aThetaMax) const
	{
		aRadius = m_radius;
		aZMin = m_zmin;
		aZMax = m_zmax;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a sphere.
	 *
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax)
	{
		m_radius = aRadius;
		m_zmin = aZMin;
		m_zmax = aZMax;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preSphere(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doSphere(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postSphere(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiSphere &operator=(const CRiSphere &c)
	{
		if ( this == &c )
			return *this;

		set(c.radius(), c.zMin(), c.zMax(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSphere


///////////////////////////////////////////////////////////////////////////////
/** @brief Cone.
 */
class CRiCone : public CUVRManInterfaceCall {
private:
	RtFloat m_height,   ///< Height.
	        m_radius,   ///< Radius.
			m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiCone"; }
	inline virtual const char *className() const { return CRiCone::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHeight   Height.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 */
	inline CRiCone(
		long aLineNo = -1, RtFloat aHeight = 0, RtFloat aRadius = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aHeight   Height.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiCone(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHeight   Height.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiCone(
		long aLineNo,
		RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiCone(const CRiCone &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiCone()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiCone(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CONE; }

	/** @brief Gets the height of the cone.
	 *
	 *  @return The height of the cone.
	 */
	RtFloat height() const
	{
		return m_height;
	}

	/** @brief Gets the height of the cone.
	 *
	 *  @param aHeight The height of the cone.
	 */
	void height(RtFloat aHeight)
	{
		m_height = aHeight;
	}

	/** @brief Gets the radius of the cone.
	 *
	 *  @return The radius of the cone.
	 */
	RtFloat radius() const
	{
		return m_radius;
	}

	/** @brief Gets the radius of the cone.
	 *
	 *  @param aRadius The radius of the cone.
	 */
	void radius(RtFloat aRadius)
	{
		m_radius = aRadius;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a cone.
	 *
	 *  @param aHeight   Height.
	 *  @retval aRadius   Radius.
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aHeight, RtFloat &aRadius, RtFloat &aThetaMax) const
	{
		aHeight = m_height;
		aRadius = m_radius;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a cone.
	 *
	 *  @param aHeight   Height.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax)
	{
		m_height = aHeight;
		m_radius = aRadius;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCone(*this, m_height, m_radius, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doCone(*this, m_height, m_radius, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postCone(*this, m_height, m_radius, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiCone &operator=(const CRiCone &c) {
		if ( this == &c )
			return *this;

		set(c.height(), c.radius(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiCone


///////////////////////////////////////////////////////////////////////////////
/** @brief Cylinder.
 */
class CRiCylinder : public CUVRManInterfaceCall {
private:
	RtFloat m_radius,   ///< Radius.
	        m_zmin,     ///< Minimal z coordiante.
			m_zmax,     ///< Maximal z coordiante.
			m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiCylinder"; }
	inline virtual const char *className() const { return CRiCylinder::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 */
	inline CRiCylinder(
		long aLineNo = -1, RtFloat aRadius = 0, RtFloat aZMin = 0, RtFloat aZMax = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiCylinder(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiCylinder(
		long aLineNo,
		RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_radius(aRadius), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiCylinder(const CRiCylinder &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiCylinder()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiCylinder(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CYLINDER; }

	/** @brief Gets the radius of the cylinder.
	 *
	 *  @return The radius of the cylinder.
	 */
	RtFloat radius() const
	{
		return m_radius;
	}

	/** @brief Gets the radius of the cylinder.
	 *
	 *  @param aRadius The radius of the cylinder.
	 */
	void radius(RtFloat aRadius)
	{
		m_radius = aRadius;
	}

	/** @brief Gets the minimum z coordinate.
	 *
	 * @return The minimum z coordinate.
	 */
	RtFloat zMin() const
	{
		return m_zmin;
	}

	/** @brief Sets the minimum z coordinate.
	 *
	 * @param aZMin The minimum z coordinate.
	 */
	void zMin(RtFloat aZMin)
	{
		m_zmin = aZMin;
	}

	/** @brief Gets the maximum z coordinate.
	 *
	 * @return The minimum z coordinate.
	 */
	RtFloat zMax() const
	{
		return m_zmax;
	}

	/** @brief Sets the maximum z coordinate.
	 *
	 * @param aZMax The maximum z coordinate.
	 */
	void zMax(RtFloat aZMax)
	{
		m_zmax = aZMax;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a cylinder.
	 *
	 *  @retval aRadius   Radius.
     *  @retval aZMin     Minimal z coordinate (cut pole).
     *  @retval aZMax     Maximal z coordinate (cut pole).
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aRadius, RtFloat &aZMin, RtFloat &aZMax, RtFloat &aThetaMax) const
	{
		aRadius = m_radius;
		aZMin = m_zmin;
		aZMax = m_zmax;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a cylinder.
	 *
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimal z coordinate (cut pole).
     *  @param aZMax     Maximal z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aRadius, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax)
	{
		m_radius = aRadius;
		m_zmin = aZMin;
		m_zmax = aZMax;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCylinder(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doCylinder(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postCylinder(*this, m_radius, m_zmin, m_zmax, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiCylinder &operator=(const CRiCylinder &c)
	{
		if ( this == &c )
			return *this;

		set(c.radius(), c.zMin(), c.zMax(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiCylinder


///////////////////////////////////////////////////////////////////////////////
/** @brief Hyperboloid.
 */
class CRiHyperboloid : public CUVRManInterfaceCall {
private:
	RtPoint m_point1,   ///< First point of the line to sweep to get a hyperbolid.
	        m_point2;   ///< Second point of the line to sweep to get a hyperbolid.
	RtFloat m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiHyperboloid"; }
	inline virtual const char *className() const { return CRiHyperboloid::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiHyperboloid(long aLineNo = -1)
		: CUVRManInterfaceCall(aLineNo)
	{
		m_point1[0] = m_point1[1] = m_point1[2] = 0;
		m_point2[0] = m_point2[1] = m_point2[2] = 0;
		m_thetamax = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aPoint1   First point of the line to sweep to get a hyperbolid.
	 *  @param aPoint2   Second point of the line to sweep to get a hyperbolid.
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiHyperboloid(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const RtPoint aPoint1, const RtPoint aPoint2, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_thetamax(aThetaMax)
	{
		for ( int i = 0; i < 3; ++i ) {
			m_point1[i] = aPoint1[i];
			m_point2[i] = aPoint2[i];
		}
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aPoint1   First point of the line to sweep to get a hyperbolid.
	 *  @param aPoint2   Second point of the line to sweep to get a hyperbolid.
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiHyperboloid(
		long aLineNo,
		const RtPoint aPoint1, const RtPoint aPoint2, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_thetamax(aThetaMax)
	{
		for ( int i = 0; i < 3; ++i ) {
			m_point1[i] = aPoint1[i];
			m_point2[i] = aPoint2[i];
		}
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiHyperboloid(const CRiHyperboloid &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiHyperboloid()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiHyperboloid(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_HYPERBOLOID; }

	/** @brief Gets the first point of the line to sweep.
	 *
	 *  @retval aPoint1 First point of the line to sweep to get a hyperbolid.
	 */
	void getPoint1(RtPoint aPoint1) const
	{
		for ( int i = 0; i < 3; ++i ) {
			aPoint1[i] = m_point1[i];
		}
	}

	/** @brief Gets the first point of the line to sweep.
	 *
	 *  @return First point of the line to sweep to get a hyperbolid.
	 */
	const RtPoint &point1() const
	{
		return m_point1;
	}

	/** @brief Sets the first point of the line to sweep.
	 *
	 *  @param aPoint1 First point of the line to sweep to get a hyperbolid.
	 */
	void point1(const RtPoint aPoint1)
	{
		for ( int i = 0; i < 3; ++i ) {
			m_point1[i] = aPoint1[i];
		}
	}

	/** @brief Gets the second point of the line to sweep.
	 *
	 *  @retval aPoint2 Second point of the line to sweep to get a hyperbolid.
	 */
	void getPoint2(RtPoint aPoint2) const
	{
		for ( int i = 0; i < 3; ++i ) {
			aPoint2[i] = m_point2[i];
		}
	}

	/** @brief Gets the second point of the line to sweep.
	 *
	 *  @return Second point of the line to sweep to get a hyperbolid.
	 */
	const RtPoint &point2() const
	{
		return m_point2;
	}

	/** @brief Sets the second point of the line to sweep.
	 *
	 *  @param aPoint2 Second point of the line to sweep to get a hyperbolid.
	 */
	void point2(const RtPoint aPoint2)
	{
		for ( int i = 0; i < 3; ++i ) {
			m_point2[i] = aPoint2[i];
		}
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a cylinder.
	 *
	 *  @retval aPoint1   First point of the line to sweep to get a hyperbolid.
	 *  @retval aPoint2   Second point of the line to sweep to get a hyperbolid.
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtPoint aPoint1, RtPoint aPoint2, RtFloat &aThetaMax) const
	{
		for ( int i = 0; i < 3; ++i ) {
			aPoint1[i] = m_point1[i];
			aPoint2[i] = m_point2[i];
		}
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a cylinder.
	 *
	 *  @param aPoint1   First point of the line to sweep to get a hyperbolid.
	 *  @param aPoint2   Second point of the line to sweep to get a hyperbolid.
     *  @param aThetaMax Sweep angle.
	 */
	void set(const RtPoint aPoint1, const RtPoint aPoint2, RtFloat aThetaMax)
	{
		for ( int i = 0; i < 3; ++i ) {
			m_point1[i] = aPoint1[i];
			m_point2[i] = aPoint2[i];
		}
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preHyperboloid(*this, m_point1, m_point2, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doHyperboloid(*this, m_point1, m_point2, m_thetamax, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postHyperboloid(*this, m_point1, m_point2, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiHyperboloid &operator=(const CRiHyperboloid &c)
	{
		if ( this == &c )
			return *this;

		set(c.point1(), c.point2(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiHyperboloid


///////////////////////////////////////////////////////////////////////////////
/** @brief Paraboloid.
 */
class CRiParaboloid : public CUVRManInterfaceCall {
private:
	RtFloat m_rmax,     ///< Maximal radius (on zmax).
            m_zmin,     ///< Minimal z coordinate.
            m_zmax,     ///< Maximal z coordinate.
	        m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiParaboloid"; }
	inline virtual const char *className() const { return CRiParaboloid::myClassName(); }

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRMax     Maximal radius.
     *  @param aZMin     Minimal z coordinate.
     *  @param aZMax     Maximal z coordinate.
     *  @param aThetaMax Sweep angle.
	 */
	inline CRiParaboloid(
		long aLineNo = -1,
		RtFloat aRMax = 0, RtFloat aZMin = 0, RtFloat aZMax = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_rmax(aRMax), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aRMax     Maximal radius.
     *  @param aZMin     Minimal z coordinate.
     *  @param aZMax     Maximal z coordinate.
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiParaboloid(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aRMax, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_rmax(aRMax), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRMax     Maximal radius.
     *  @param aZMin     Minimal z coordinate.
     *  @param aZMax     Maximal z coordinate.
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiParaboloid(
		long aLineNo,
		RtFloat aRMax, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_rmax(aRMax), m_zmin(aZMin), m_zmax(aZMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiParaboloid(const CRiParaboloid &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiParaboloid()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiParaboloid(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PARABOLOID; }

	/** @brief Gets the maximal radius of the paraboloid.
	 *
	 *  @return The maximal radius of the paraboloid.
	 */
	RtFloat rMax() const
	{
		return m_rmax;
	}

	/** @brief Gets the maximal radius of the paraboloid.
	 *
	 *  @param aRMax The maximal radius of the paraboloid.
	 */
	void rMax(RtFloat aRMax)
	{
		m_rmax = aRMax;
	}

	/** @brief Gets the minimum z coordinate.
	 *
	 * @return The minimum z coordinate.
	 */
	RtFloat zMin() const
	{
		return m_zmin;
	}

	/** @brief Sets the minimum z coordinate.
	 *
	 * @param aZMin The minimum z coordinate.
	 */
	void zMin(RtFloat aZMin)
	{
		m_zmin = aZMin;
	}

	/** @brief Gets the maximum z coordinate.
	 *
	 * @return The minimum z coordinate.
	 */
	RtFloat zMax() const
	{
		return m_zmax;
	}

	/** @brief Sets the maximum z coordinate.
	 *
	 * @param aZMax The maximum z coordinate.
	 */
	void zMax(RtFloat aZMax)
	{
		m_zmax = aZMax;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a paraboloid.
	 *
	 *  @retval aRMax     Maximal radius.
     *  @retval aZMin     Minimal z coordinate.
     *  @retval aZMax     Maximal z coordinate.
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aRMax, RtFloat &aZMin, RtFloat &aZMax, RtFloat &aThetaMax) const
	{
		aRMax = m_rmax;
		aZMin = m_zmin;
		aZMax = m_zmax;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a paraboloid.
	 *
	 *  @param aRMax     Maximal Radius.
     *  @param aZMin     Minimal z coordinate.
     *  @param aZMax     Maximal z coordinate.
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aRMax, RtFloat aZMin, RtFloat aZMax, RtFloat aThetaMax)
	{
		m_rmax = aRMax;
		m_zmin = aZMin;
		m_zmax = aZMax;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preParaboloid(*this, m_rmax, m_zmin, m_zmax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doParaboloid(*this, m_rmax, m_zmin, m_zmax, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postParaboloid(*this, m_rmax, m_zmin, m_zmax, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiParaboloid &operator=(const CRiParaboloid &c)
	{
		if ( this == &c )
			return *this;

		set(c.rMax(), c.zMin(), c.zMax(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiParaboloid


///////////////////////////////////////////////////////////////////////////////
/** @brief Disk.
 */
class CRiDisk : public CUVRManInterfaceCall {
private:
	RtFloat m_height,   ///< Distance from disk to origin on positive z axis.
	        m_radius,   ///< Radius.
	        m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDisk"; }
	inline virtual const char *className() const { return CRiDisk::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHeight   Distance (z) from origin.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 */
	inline CRiDisk(
		long aLineNo = -1, RtFloat aHeight = 0, RtFloat aRadius = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aHeight   Distance (z) from origin.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiDisk(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aHeight   Distance (z) from origin.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiDisk(
		long aLineNo,
		RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_height(aHeight), m_radius(aRadius), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDisk(const CRiDisk &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDisk()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDisk(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DISK; }

	/** @brief Gets the distance (z) from origin of the disk.
	 *
	 *  @return The distance (z) from origin of the disk.
	 */
	RtFloat height() const
	{
		return m_height;
	}

	/** @brief Gets the distance (z) from origin of the disk.
	 *
	 *  @param aHeight The distance (z) from origin of the disk.
	 */
	void height(RtFloat aHeight)
	{
		m_height = aHeight;
	}

	/** @brief Gets the radius of the disk.
	 *
	 *  @return The radius of the disk.
	 */
	RtFloat radius() const
	{
		return m_radius;
	}

	/** @brief Gets the radius of the disk.
	 *
	 *  @param aRadius The radius of the disk.
	 */
	void radius(RtFloat aRadius)
	{
		m_radius = aRadius;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a disk.
	 *
     *  @param aHeight   Distance (z) from origin.
	 *  @retval aRadius   Radius.
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aHeight, RtFloat &aRadius, RtFloat &aThetaMax) const
	{
		aHeight = m_height;
		aRadius = m_radius;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a disk.
	 *
     *  @param aHeight   Distance (z) from origin.
	 *  @param aRadius   Radius.
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aHeight, RtFloat aRadius, RtFloat aThetaMax)
	{
		m_height = aHeight;
		m_radius = aRadius;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisk(*this, m_height, m_radius, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisk(*this, m_height, m_radius, m_thetamax, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisk(*this, m_height, m_radius, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDisk &operator=(const CRiDisk &c)
	{
		if ( this == &c )
			return *this;

		set(c.height(), c.radius(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisk


///////////////////////////////////////////////////////////////////////////////
/** @brief Torus.
 */
class CRiTorus : public CUVRManInterfaceCall {
private:
	RtFloat m_majorrad, ///< Major radius (center of the ring).
	        m_minorrad, ///< Minor radius.
	        m_phimin,   ///< Start angle to sweep the torus body.
	        m_phimax,   ///< End angle to sweep the torus body.
	        m_thetamax; ///< Sweep angle.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiTorus"; }
	inline virtual const char *className() const { return CRiTorus::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CUVRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aMajorRad Major radius (center of the ring).
	 *  @param aMinorRad Minor radius.
	 *  @param aPhiMin   Start angle to sweep the torus body.
	 *  @param aPhiMax   End angle to sweep the torus body.
     *  @param aThetaMax Sweep angle.
	 */
	inline CRiTorus(
		long aLineNo = -1,
		RtFloat aMajorRad = 0, RtFloat aMinorRad = 0, RtFloat aPhiMin = 0, RtFloat aPhiMax = 0, RtFloat aThetaMax = 0)
		: CUVRManInterfaceCall(aLineNo),
		  m_majorrad(aMajorRad), m_minorrad(aMinorRad),
		  m_phimin(aPhiMin), m_phimax(aPhiMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aMajorRad Major radius (center of the ring).
	 *  @param aMinorRad Minor radius.
	 *  @param aPhiMin   Start angle to sweep the torus body.
	 *  @param aPhiMax   End angle to sweep the torus body.
     *  @param aThetaMax Sweep angle.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiTorus(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat aMajorRad, RtFloat aMinorRad, RtFloat aPhiMin, RtFloat aPhiMax, RtFloat aThetaMax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_majorrad(aMajorRad), m_minorrad(aMinorRad),
		  m_phimin(aPhiMin), m_phimax(aPhiMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aMajorRad Major radius (center of the ring).
	 *  @param aMinorRad Minor radius.
	 *  @param aPhiMin   Start angle to sweep the torus body.
	 *  @param aPhiMax   End angle to sweep the torus body.
     *  @param aThetaMax Sweep angle.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiTorus(
		long aLineNo,
		RtFloat aMajorRad, RtFloat aMinorRad, RtFloat aPhiMin, RtFloat aPhiMax, RtFloat aThetaMax,
		const CParameterList &theParameters
		)
		: CUVRManInterfaceCall(aLineNo, theParameters),
		  m_majorrad(aMajorRad), m_minorrad(aMinorRad),
		  m_phimin(aPhiMin), m_phimax(aPhiMax), m_thetamax(aThetaMax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiTorus(const CRiTorus &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiTorus()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiTorus(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_TORUS; }

	/** @brief Gets the major radius (center of the ring).
	 *
	 *  @return The major radius (center of the ring).
	 */
	inline virtual RtFloat majorRad() const
	{
		return m_majorrad;
	}

	/** @brief Sets the major radius (center of the ring).
	 *
	 *  @param aMajorRad The major radius (center of the ring).
	 */
	inline virtual void majorRad(RtFloat aMajorRad)
	{
		m_majorrad = aMajorRad;
	}

	/** @brief Gets the minor radius.
	 *
	 *  @return The minor radius.
	 */
	inline virtual RtFloat minorRad() const
	{
		return m_minorrad;
	}

	/** @brief Sets the minor radius.
	 *
	 *  @param aMinorRad The minor radius.
	 */
	inline virtual void minorRad(RtFloat aMinorRad)
	{
		m_minorrad = aMinorRad;
	}

	/** @brief Gets the start angle to sweep the torus body.
	 *
	 *  @return Start angle to sweep the torus body.
	 */
	inline virtual RtFloat phiMin() const
	{
		return m_phimin;
	}

	/** @brief Sets the start angle to sweep the torus body.
	 *
	 *  @param aPhiMin Start angle to sweep the torus body.
	 */
	inline virtual void phiMin(RtFloat aPhiMin)
	{
		m_phimin = aPhiMin;
	}

	/** @brief Gets the end angle to sweep the torus body.
	 *
	 *  @return End angle to sweep the torus body.
	 */
	inline virtual RtFloat phiMax() const
	{
		return m_phimax;
	}

	/** @brief Sets the end angle to sweep the torus body.
	 *
	 *  @param aPhiMax End angle to sweep the torus body.
	 */
	inline virtual void phiMax(RtFloat aPhiMax)
	{
		m_phimax = aPhiMax;
	}

	/** @brief Gets the sweep angle.
	 *
	 * @return The sweep angle.
	 */
	RtFloat thetaMax() const
	{
		return m_thetamax;
	}

	/** @brief Sets the sweep angle.
	 *
	 * @param aThetaMax The sweep angle.
	 */
	void thetaMax(RtFloat aThetaMax)
	{
		m_thetamax = aThetaMax;
	}

	/** @brief Gets the member variables of a disk.
	 *
	 *  @retval aMajorRad Major radius (center of the ring).
	 *  @retval aMinorRad Minor radius.
	 *  @retval aPhiMin   Start angle to sweep the torus body.
	 *  @retval aPhiMax   End angle to sweep the torus body.
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aMajorRad, RtFloat &aMinorRad, RtFloat &aPhiMin, RtFloat &aPhiMax, RtFloat &aThetaMax) const
	{
		aMajorRad = m_majorrad;
	    aMinorRad = m_minorrad;
	    aPhiMin = m_phimin;
	    aPhiMax = m_phimax;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a disk.
	 *
	 *  @param aMajorRad Major radius (center of the ring).
	 *  @param aMinorRad Minor radius.
	 *  @param aPhiMin   Start angle to sweep the torus body.
	 *  @param aPhiMax   End angle to sweep the torus body.
     *  @param aThetaMax Sweep angle.
	 */
	void set(RtFloat aMajorRad, RtFloat aMinorRad, RtFloat aPhiMin, RtFloat aPhiMax, RtFloat aThetaMax)
	{
		m_majorrad = aMajorRad;
	    m_minorrad = aMinorRad;
	    m_phimin = aPhiMin;
	    m_phimax = aPhiMax;
		m_thetamax = aThetaMax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preTorus(*this, m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doTorus(*this, m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postTorus(*this, m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiTorus &operator=(const CRiTorus &c)
	{
		if ( this == &c )
			return *this;

		set(c.majorRad(), c.minorRad(), c.phiMin(), c.phiMax(), c.thetaMax());

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiTorus


///////////////////////////////////////////////////////////////////////////////
/** @brief Points.
 */
class CRiPoints : public CGeometryRManInterfaceCall {
private:
	RtInt m_npts; ///< Number of points.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPoints"; }
	inline virtual const char *className() const { return CRiPoints::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the number of points to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPoints(long aLineNo = -1)
		: CGeometryRManInterfaceCall(aLineNo)
	{
		m_npts = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
     *  @param aNPts     Number of points.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	inline CRiPoints(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt aNPts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo, decl, CPointsClasses(aNPts), curColorDescr, n, tokens, params),
		  m_npts(aNPts)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
     *  @param aNPts     Number of points.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiPoints(
		long aLineNo,
		RtInt aNPts,
		const CParameterList &theParameters
		)
		: CGeometryRManInterfaceCall(aLineNo, theParameters),
		  m_npts(aNPts)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPoints(const CRiPoints &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPoints()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPoints(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }

	/** @brief Gets the number of points.
	 *
	 *  @return Number of points.
	 */
	inline virtual RtInt nPts() const
	{
		return m_npts;
	}

	/** @brief Sets the number of points.
	 *
	 *  @param aNPts Number of points.
	 */
	inline virtual void nPts(RtInt aNPts)
	{
		m_npts = aNPts;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePoints(*this, m_npts, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPoints(*this, m_npts, parameters());
	}
	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPoints(*this, m_npts, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPoints &operator=(const CRiPoints &c)
	{
		if ( this == &c )
			return *this;

		nPts(c.nPts());

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPoints


///////////////////////////////////////////////////////////////////////////////
/** @brief Curves.
 */
class CRiCurves : public CGeometryRManInterfaceCall {
private:
	RtToken m_type, ///< Type of the curve either RI_LINEAR or RI_CUBIC.
	        m_wrap; ///< Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	RtInt   m_step; ///< Step to the next curve segment (by basis, v parameter)
	std::vector<RtInt> m_nverts; ///< Number of vertices for each curve (size == number of curves)
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiCurves"; }
	inline virtual const char *className() const { return CRiCurves::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the number of curves to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	CRiCurves(long aLineNo = -1) : CGeometryRManInterfaceCall(aLineNo)
	{
		m_type = RI_NULL,
		m_wrap = RI_NULL;
		m_step = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aStep    Step to the next curve segment (by basis, v parameter)
	 *  @param aType     Type of the curve either RI_LINEAR or RI_CUBIC.
	 *  @param aNCurves  Number of curves
	 *  @param aNVerts   Number of vertices for each curve (size == @a NCurves)
	 *  @param aWrap     Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	 *  @param n         Number of parameters (size of @a tokens, @a params).
	 *  @param tokens    Tokens of the request.
	 *  @param params    Parameter values of the request.
	 */
	CRiCurves(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt aStep, RtToken aType, RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap,
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aStep    Step to the next curve segment (by basis, v parameter)
	 *  @param aType     Type of the curve either RI_LINEAR or RI_CUBIC.
	 *  @param aNCurves  Number of curves
	 *  @param aNVerts   Number of vertices for each curve (size == @a NCurves)
	 *  @param aWrap     Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiCurves(
		long aLineNo,
		RtInt aStep, RtToken aType, RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap,
		const CParameterList &theParameters
		);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiCurves(const CRiCurves &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiCurves()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiCurves(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CURVES; }


	/** @brief Gets the type of the curves.
	 *
	 * @return The type of the mesh.
	 */
	RtToken type() const
	{
		return m_type;
	}

	/** @brief Gets the wrap of the curves.
	 *
	 * @return The wrap of the curves.
	 */
	RtToken wrap() const
	{
		return m_wrap;
	}

	/** @brief Gets the indices of the control points.
	 *
	 * @return The indices of the control points.
	 */
	RtInt nCurves() const
	{
		return (RtInt)m_nverts.size();
	}

	/** @brief Gets the number of the vertices for each curve.
	 *
	 * @return The number of the vertices for each curve.
	 */
	const std::vector<RtInt> &nVerts() const
	{
		return m_nverts;
	}

	/** @brief Number of control points to the next patch (from the basis).
	 *
	 * @return The number of control points to the next patch (from the basis).
	 */
	RtInt step() const
	{
		return m_step;
	}

	/** @brief Sets the values of member variables.
	 *
	 *  @param aStep    Step to the next curve segment (by basis, v parameter)
	 *  @param aType     Type of the curve either RI_LINEAR or RI_CUBIC.
	 *  @param aNCurves  Number of curves
	 *  @param aNVerts   Number of vertices for each curve (size == @a NCurves)
	 *  @param aWrap     Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	 */
	void set(RtInt aStep, RtToken aType, RtInt aNCurves, const RtInt aNVerts[], RtToken aWrap);

	/** @brief Sets the values of member variables.
	 *
	 *  @param aStep    Step to the next curve segment (by basis, v parameter)
	 *  @param aType     Type of the curve either RI_LINEAR or RI_CUBIC.
	 *  @param aNVerts   Number of vertices for each curve (number of curves == size)
	 *  @param aWrap     Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	 */
	void set(RtInt aStep, RtToken aType, const std::vector<RtInt> &aNVerts, RtToken aWrap);

	/** @brief Gets the values of member variables.
	 *
	 *  @retval aStep    Step to the next curve segment (by basis, v parameter)
	 *  @retval aType     Type of the curve either RI_LINEAR or RI_CUBIC.
	 *  @retval aNVerts   Number of vertices for each curve (number of curves == size)
	 *  @retval aWrap     Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
	 */
	void get(RtInt &aStep, RtToken &aType, std::vector<RtInt> &aNVerts, RtToken &aWrap);

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCurves(*this, 
			m_type, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap,
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doCurves(*this, 
			m_type, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap,
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postCurves(*this, 
			m_type, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &m_nverts[0],
			m_wrap,
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiCurves &operator=(const CRiCurves &c)
	{
		if ( this == &c )
			return *this;

		set(c.step(), c.type(), c.nVerts(), c.wrap());

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiCurves


///////////////////////////////////////////////////////////////////////////////
/** @brief Blobby.
 */
class CRiBlobby : public CGeometryRManInterfaceCall {
private:
	RtInt                   m_nleaf;        ///< Number of primitive blobs, has to match the content of m_code.
	std::vector<RtInt>      m_code;         ///< Code array, s.a. [RISPEC3.2] for the opcodes.
	std::vector<RtFloat>    m_flt;          ///< The float operands for the code.
	std::deque<std::string> m_strcontainer; ///< The string operants for the code.
	std::vector<RtString>   m_str;          ///< C-String pointers of m_strcontainer.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiBlobby"; }
	inline virtual const char *className() const { return CRiBlobby::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  Empty object.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiBlobby(long aLineNo = -1) : CGeometryRManInterfaceCall(aLineNo)
	{
		m_nleaf = 0;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aNLeaf  Number of primitive blobs.
	 *  @param aNCode  Size of the code array.
	 *  @param aCode   Code array, s.a. [RISPEC3.2] for the opcodes.
	 *  @param aNFlt   Size of the flt array.
	 *  @param aFlt    The float operands for the code.
	 *  @param aNStr   Size of the str array.
	 *  @param aStr    The string operants for the code.
	 *  @param n       Number of parameters (size of @a tokens, @a params).
	 *  @param tokens  Tokens of the request.
	 *  @param params  Parameter values of the request.
	 */
	CRiBlobby(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
		RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[],
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aNLeaf  Number of primitive blobs.
	 *  @param aNCode  Size of the code array.
	 *  @param aCode   Code array, s.a. [RISPEC3.2] for the opcodes.
	 *  @param aNFlt   Size of the flt array.
	 *  @param aFlt    The float operands for the code.
	 *  @param aNStr   Size of the str array.
	 *  @param aStr    The string operants for the code.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiBlobby(long aLineNo,
		RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
		RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[],
		const CParameterList &theParameters
		);

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiBlobby(const CRiBlobby &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiBlobby()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiBlobby(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_BLOBBY; }

	/** @brief Get the number of primitive blobs, has to match the content of m_code.
	 *
	 * @return The number of primitive blobs, has to match the content of m_code.
	 */
	inline RtInt nLeaf() const
	{
		return m_nleaf;
	}

	/** @brief Get the code array, s.a. [RISPEC3.2] for the opcodes.
	 *
	 * @return The code array, s.a. [RISPEC3.2] for the opcodes.
	 */
	inline const std::vector<RtInt> &code() const
	{
		return m_code;
	}

	/** @brief Get the float operands for the code.
	 *
	 * @return The float operands for the code.
	 */
	inline const std::vector<RtFloat> &flt() const
	{
		return m_flt;
	}

	/** @brief Get the string operands for the code.
	 *
	 * @return The string operands for the code.
	 */
	inline const std::vector<RtToken> &str() const
	{
		return m_str;
	}

	/** @brief Sets the member variables.
	 *
	 *  @param aNLeaf  Number of primitive blobs.
	 *  @param aNCode  Size of the code array.
	 *  @param aCode   Code array, s.a. [RISPEC3.2] for the opcodes.
	 *  @param aNFlt   Size of the flt array.
	 *  @param aFlt    The float operands for the code.
	 *  @param aNStr   Size of the str array.
	 *  @param aStr    The string operants for the code.
	 */
	void set(
		RtInt aNLeaf, RtInt aNCode, const RtInt aCode[],
		RtInt aNFlt, const RtFloat aFlt[], RtInt aNStr, const RtString aStr[]);

	/** @brief Sets the member variables.
	 *
	 *  @param aNLeaf  Number of primitive blobs.
	 *  @param aCode   Code array, s.a. [RISPEC3.2] for the opcodes.
	 *  @param aFlt    The float operands for the code.
	 *  @param aStr    The string operants for the code.
	 */
	void set(
		RtInt aNLeaf, const std::vector<RtInt> &aCode,
		const std::vector<RtFloat> &aFlt, const std::vector<RtString> &aStr);

	/** @brief Gets the member variables.
	 *
	 *  @retval aNLeaf  Number of primitive blobs.
	 *  @retval aCode   Code array, s.a. [RISPEC3.2] for the opcodes.
	 *  @retval aFlt    The float operands for the code.
	 *  @retval aStr    The string operants for the code.
	 */
	void get(
		RtInt &aNLeaf, std::vector<RtInt> &aCode,
		std::vector<RtFloat> &aFlt, std::vector<RtString> &aStr) const;

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preBlobby(*this, 
			m_nleaf,
			(RtInt)m_code.size(), m_code.empty() ? 0 : &m_code[0],
			(RtInt)m_flt.size(), m_flt.empty() ? 0 : &m_flt[0],
			(RtInt)m_str.size(), m_str.empty() ? 0 : &m_str[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doBlobby(*this, 
			m_nleaf,
			(RtInt)m_code.size(), m_code.empty() ? 0 : &m_code[0],
			(RtInt)m_flt.size(), m_flt.empty() ? 0 : &m_flt[0],
			(RtInt)m_str.size(), m_str.empty() ? 0 : &m_str[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postBlobby(*this, 
			m_nleaf,
			(RtInt)m_code.size(), m_code.empty() ? 0 : &m_code[0],
			(RtInt)m_flt.size(), m_flt.empty() ? 0 : &m_flt[0],
			(RtInt)m_str.size(), m_str.empty() ? 0 : &m_str[0],
			parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiBlobby &operator=(const CRiBlobby &c)
	{
		if ( this == &c )
			return *this;

		set(c.nLeaf(), c.code(), c.flt(), c.str());

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiBlobby


///////////////////////////////////////////////////////////////////////////////
/** @brief Special Geometry.
 */
class CRiGeometry : public CGeometryRManInterfaceCall {
private:
	RtToken m_name; ///< Name of the geometry as atomized string.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiGeometry"; }
	inline virtual const char *className() const { return CRiGeometry::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CGeometryRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the geometry as atomized string.
	 */
	inline CRiGeometry(long aLineNo = -1, RtToken aName = RI_NULL)
		: CGeometryRManInterfaceCall(aLineNo), m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param decl    Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the geometry as atomized string.
	 *  @param n       Number of parameters (size of @a tokens, @a params).
	 *  @param tokens  Tokens of the request.
	 *  @param params  Parameter values of the request.
	 */
	inline CRiGeometry(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params),
		  m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the geometry as atomized string.
	 *  @param theParameters Parsed parameter list.
	 */
	inline CRiGeometry(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters)
		: CGeometryRManInterfaceCall(aLineNo, theParameters),
		  m_name(aName)
	{
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiGeometry(const CRiGeometry &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiGeometry()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiGeometry(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }

	/** @brief Gets the name of the attribute as atomized string.
	 *
	 *  @return The name of the attribute as atomized string.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the attribute as atomized string.
	 *
	 *  @param aName The name of the attribute as atomized string.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometry(*this, m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doGeometry(*this, m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postGeometry(*this, m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiGeometry &operator=(const CRiGeometry &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiGeometry

}

#endif // _RICPP_RICONTEXT_RIMACROPRIMS_H
