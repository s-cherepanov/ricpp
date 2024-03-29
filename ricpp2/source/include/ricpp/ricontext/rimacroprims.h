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

#ifndef _RICPP_RICONTEXT_POLYGON_H
#include "ricpp/ricontext/polygon.h"
#endif // _RICPP_RICONTEXT_POLYGON_H

namespace RiCPP {

	// ----------------------------------------------------------------------------
	// Graphical primitives
	// ----------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////
	/** @brief Simple polygon.
	 */
	class CRiPolygon : public CVarParamRManInterfaceCall {
	private:
		RtInt m_nVertices; ///< Number of vertices of the polygon must match the number of positions "P".
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.polygonV(nVertices(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
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
	class CRiGeneralPolygon : public CVarParamRManInterfaceCall {
	private:
		CTriangulatedPolygon *m_triangulated; ///< Triangulated polygon
		std::vector<RtInt> m_nVerts; ///< Number of verts per outline (Number of loops is the size of m_nVerts).
		/** @brief enters the values.
		 *
		 *  @param theNLoops Number of loops
		 *  @param theNVerts Number of vertices/loop, size of @a nverts vector is @a nloops.
		 */
		void enterValues(RtInt theNLoops, const RtInt theNVerts[]);
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of vertices to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiGeneralPolygon(long aLineNo = -1)
			: TypeParent(aLineNo)
		{
			m_triangulated = 0;
			aLineNo = aLineNo + 0;
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
			m_triangulated = 0;
			*this = c;
		}

		/** @brief Destructor.
		 */
		inline virtual ~CRiGeneralPolygon()
		{
			if ( m_triangulated )
				delete m_triangulated;
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
			return (RtInt)(m_nVerts.size());
		}

		/** @brief Gets the number of vertices per loop.
		 *
		 *  @return The number of vertices (size is the number of loops).
		 */
		inline const std::vector<RtInt> &nVerts() const
		{
			return m_nVerts;
		}

		inline std::vector<RtInt> &nVerts()
		{
			return m_nVerts;
		}
		
		inline const RtInt *nVertsPtr() const
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
		}

		inline RtInt *nVertsPtr()
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
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

		/** @brief Triangulates the polygon if not already done.
		 *  @param strategy Strategy to use
		 *  @return Indices for triangulated polygon, 0 on error
		 */
		const CTriangulatedPolygon *triangulate(const IPolygonTriangulationStrategy &strategy, bool frontCW=true);

		/** @brief Requests whether polygon is triangulated.
		 *  @return true, if polygon is triangulated; false, otherwise.
		 */
		inline bool triangulated() const { return m_triangulated!=0; }

		inline virtual void process(IRiRoot &ri)
		{
			ri.generalPolygonV(nLoops(), nVertsPtr(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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
			if ( m_triangulated )
				delete m_triangulated;
			m_triangulated = 0;

			TypeParent::operator=(c);
			return *this;
		}

	}; // CRiGeneralPolygon

	
	///////////////////////////////////////////////////////////////////////////////
	/** @brief Simple polygon net.
	 */
	class CRiPointsPolygons : public CVarParamRManInterfaceCall {
	private:
		std::vector<RtInt> m_nVerts; ///< Number of vertices per polygon (size is the number of polygons).
		std::vector<RtInt> m_verts;  ///< Indices of the vertices.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of polygons to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiPointsPolygons(long aLineNo=-1)
			: TypeParent(aLineNo)
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
			return (RtInt)(m_nVerts.size());
		}

		/** @brief Gets the numbers of the vertices.
		 *
		 *  @return The numbers of the vertices (size is the number of polygons).
		 */
		inline const std::vector<RtInt> &nVerts() const
		{
			return m_nVerts;
		}
		
		inline std::vector<RtInt> &nVerts()
		{
			return m_nVerts;
		}
		
		inline const RtInt *nVertsPtr() const
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
		}
		
		inline RtInt *nVertsPtr()
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
		}

		/** @brief Gets the indices of the vertices.
		 *
		 *  @return The indizes of the vertices.
		 */
		inline const std::vector<RtInt> &verts() const
		{
			return m_verts;
		}

		inline std::vector<RtInt> &verts()
		{
			return m_verts;
		}
		
		inline const RtInt *vertsPtr() const
		{
			return m_verts.empty() ? 0 : &m_verts[0];
		}
		
		inline RtInt *vertsPtr()
		{
			return m_verts.empty() ? 0 : &m_verts[0];
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

			assert((RtInt)m_verts.size() == sum(m_nVerts.size(), &(m_nVerts[0])));
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.pointsPolygonsV(nPolys(), nVertsPtr(), vertsPtr(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			ribWriter.putBlank();
			ribWriter.putArray(verts());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPointsPolygons

	
	///////////////////////////////////////////////////////////////////////////////
	/** @brief General polygon net.
	 */
	class CRiPointsGeneralPolygons : public CVarParamRManInterfaceCall {
	private:
		std::vector<RtInt> m_nLoops; ///< Loops per polygon (Number of polygons is the size of the vector).
		std::vector<RtInt> m_nVerts; ///< Vertices per loop.
		std::vector<RtInt> m_verts;  ///< Vertex indices.
		std::vector<CTriangulatedPolygon> m_triangulated; ///< Triangulated polygons
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;

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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of polygons to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiPointsGeneralPolygons(long aLineNo=-1)
			: TypeParent(aLineNo)
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

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiPointsGeneralPolygons(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_GENERAL_POLYGONS; }

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

		inline std::vector<RtInt> &nLoops()
		{
			return m_nLoops;
		}
		
		inline const RtInt *nLoopsPtr() const
		{
			return m_nLoops.empty() ? 0 : &m_nLoops[0];
		}
		
		inline RtInt *nLoopsPtr()
		{
			return m_nLoops.empty() ? 0 : &m_nLoops[0];
		}

		/** @brief Gets the number of vertices per loops.
		 *
		 *  @return The number of vertices per loops.
		 */
		const std::vector<RtInt> &nVerts() const
		{
			return m_nVerts;
		}

		inline std::vector<RtInt> &nVerts()
		{
			return m_nVerts;
		}
		
		inline const RtInt *nVertsPtr() const
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
		}
		
		inline RtInt *nVertsPtr()
		{
			return m_nVerts.empty() ? 0 : &m_nVerts[0];
		}

		/** @brief Gets the indices of the vertices.
		 *
		 *  @return The indices of the vertices.
		 */
		const std::vector<RtInt> &verts() const
		{
			return m_verts;
		}

		
		inline std::vector<RtInt> &verts()
		{
			return m_verts;
		}
		
		inline const RtInt *vertsPtr() const
		{
			return m_verts.empty() ? 0 : &m_verts[0];
		}
		
		inline RtInt *vertsPtr()
		{
			return m_verts.empty() ? 0 : &m_verts[0];
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

			assert((RtInt)m_nVerts.size() == sum(m_nLoops.size(), &(m_nLoops[0])));
			assert((RtInt)m_verts.size() == sum(m_nVerts.size(), &(m_nVerts[0])));
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

		/** @brief Triangulates the polygons if not already done.
		 *  @param strategy Strategy to use
		 *  @return Indirect indices for the triangulated polygons, (size==0) on error
		 */
		const std::vector<CTriangulatedPolygon> &triangulate(const IPolygonTriangulationStrategy &strategy, bool frontCW=true);
		
		/** @brief Requests whether polygons are triangulated.
		 *  @return true, if polygons are triangulated; false, otherwise.
		 */
		inline bool triangulated() const { return m_triangulated.size() > 0; }

		inline virtual void process(IRiRoot &ri)
		{
			ri.pointsGeneralPolygonsV(nPolys(), nLoopsPtr(), nVertsPtr(), vertsPtr(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putArray(nLoops());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			ribWriter.putBlank();
			ribWriter.putArray(verts());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			m_triangulated.clear();
			set(c.nLoops(), c.nVerts(), c.verts());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPointsGeneralPolygons


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Bilinear or bicubic patch
	 */
	class CRiPatch : public CVarParamRManInterfaceCall {
	private:
		RtToken m_type; ///< Type of the patch, either RI_BILINEAR or RI_BICUBIC (or RI_NULL if not set).
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of polygons to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiPatch(long aLineNo=-1)
			: TypeParent(aLineNo)
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
			: TypeParent(aLineNo, decl, CPatchClasses(aType), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters), m_type(aType)
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.patchV(type(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPatch


	///////////////////////////////////////////////////////////////////////////////
	/** @brief a mesh of bilinear or bicubic patches.
	 */
	class CRiPatchMesh : public CVarParamRManInterfaceCall {
	private:
		RtToken m_type,  ///< Type of the mesh either RI_BILINEAR or RI_BICUBIC.
				m_uwrap, ///< Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC.
				m_vwrap; ///< Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
		RtInt   m_nu,    ///< Number of control points in parametric u direction.
				m_nv;    ///< Number of control points in parametric v direction.
		RtInt   m_ustep, ///< Step m_ustep control points to the next patch in u direction (from the basis).
				m_vstep; ///< Step m_vstep control points to the next patch in v direction (from the basis).

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
		
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
		
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of polygons to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		CRiPatchMesh(long aLineNo = -1) :
			TypeParent(aLineNo)
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

		/** @brief Number of patches in parametric u direction.
		 *
		 * @return The number of patches in parametric u direction.
		 * @todo Duplicate code in CPatchMeshClasses::reset()
		 */
		RtInt nuPatches() const
		{
			if ( type() == RI_BICUBIC )
				return uWrap() == RI_PERIODIC ? nu()/uStep() : (nu()-4)/uStep() + 1;
			// type == RI_BILINEAR
			return uWrap() == RI_PERIODIC ? nu() : nu()-1;
		}
		
		/** @brief Number of patches in parametric v direction.
		 *
		 * @return The number of patches in parametric v direction.
		 * @todo Duplicate code in CPatchMeshClasses::reset()
		 */
		RtInt nvPatches() const
		{
			if ( type() == RI_BICUBIC )
				return vWrap() == RI_PERIODIC ? nv()/vStep() : (nv()-4)/vStep() + 1;
			// type == RI_BILINEAR
			return vWrap() == RI_PERIODIC ? nv() : nv()-1;
		}
		
		inline virtual void process(IRiRoot &ri)
		{
			ri.patchMeshV(type(), nu(), uWrap(), nv(), vWrap(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			ribWriter.putBlank();
			ribWriter.putValue(nu());
			ribWriter.putBlank();
			ribWriter.putStringToken(uWrap());
			ribWriter.putBlank();
			ribWriter.putValue(nv());
			ribWriter.putBlank();
			ribWriter.putStringToken(vWrap());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPatchMesh


	///////////////////////////////////////////////////////////////////////////////
	/** @brief NURBS patch.
	 */
	class CRiNuPatch : public CVarParamRManInterfaceCall {
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
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of control points to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiNuPatch(long aLineNo=-1) : TypeParent(aLineNo)
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
		inline RtInt nu() const
		{
			return m_nu;
		}

		/** @brief Gets the number of control points in parametric v direction.
		 *
		 *  @return The number of control points in parametric v direction.
		 */
		inline RtInt nv() const
		{
			return m_nv;
		}

		/** @brief Gets the order of the polynomial basis for parameters u.
		 *
		 *  @return the order of the polynomial basis for parameters u.
		 */
		inline RtInt uOrder() const
		{
			return m_uorder;
		}

		/** @brief Gets the order of the polynomial basis for parameters v.
		 *
		 *  @return The order of the polynomial basis for parameters v.
		 */
		inline RtInt vOrder() const
		{
			return m_vorder;
		}

		/** @brief Gets the minimal value for parameter u.
		 *
		 *  @return The minimal value for parameter u.
		 */
		inline RtFloat uMin() const
		{
			return m_umin;
		}

		/** @brief Gets the maximal value for parameter u.
		 *
		 *  @return The maximal value for parameter u.
		 */
		inline RtFloat uMax() const
		{
			return m_umax;
		}

		/** @brief Gets the minimal value for parameter v.
		 *
		 *  @return The minimal value for parameter v.
		 */
		inline RtFloat vMin() const
		{
			return m_vmin;
		}

		/** @brief Gets the maximal value for parameter v.
		 *
		 *  @return The maximal value for parameter v.
		 */
		inline RtFloat vMax() const
		{
			return m_vmax;
		}

		/** @brief Gets the knot vector for the u parameters.
		 *
		 *  @return The knot vector for the u parameters.
		 */
		inline const std::vector<RtFloat> &uKnot() const
		{
			return m_uknot;
		}

		inline std::vector<RtFloat> &uKnot()
		{
			return m_uknot;
		}

		inline const RtFloat *uKnotPtr() const
		{
			return m_uknot.empty() ? 0 : &m_uknot[0];
		}
		
		inline RtFloat *uKnotPtr()
		{
			return m_uknot.empty() ? 0 : &m_uknot[0];
		}

		/** @brief Gets the knot vector for the v parameters.
		 *
		 *  @return The knot vector for the v parameters.
		 */
		inline const std::vector<RtFloat> &vKnot() const
		{
			return m_vknot;
		}

		inline std::vector<RtFloat> &vKnot()
		{
			return m_uknot;
		}

		inline const RtFloat *vKnotPtr() const
		{
			return m_vknot.empty() ? 0 : &m_vknot[0];
		}
		
		inline RtFloat *vKnotPtr()
		{
			return m_vknot.empty() ? 0 : &m_vknot[0];
		}
		
		inline virtual void process(IRiRoot &ri)
		{
			ri.nuPatchV(nu(), uOrder(), uKnotPtr(), uMin(), uMax(),
						nv(), vOrder(), vKnotPtr(), vMin(), vMax(),
						paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(nu());
			ribWriter.putBlank();
			ribWriter.putValue(uOrder());
			ribWriter.putBlank();
			ribWriter.putArray(uKnot());
			ribWriter.putBlank();
			ribWriter.putValue(uMin());
			ribWriter.putBlank();
			ribWriter.putValue(uMax());
			ribWriter.putBlank();
			ribWriter.putValue(nv());
			ribWriter.putBlank();
			ribWriter.putValue(vOrder());
			ribWriter.putBlank();
			ribWriter.putArray(vKnot());
			ribWriter.putBlank();
			ribWriter.putValue(vMin());
			ribWriter.putBlank();
			ribWriter.putValue(vMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiNuPatch


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Subdivision mesh surface.
	 */
	class CRiSubdivisionMesh : public CVarParamRManInterfaceCall {
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
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of faces to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		CRiSubdivisionMesh(long aLineNo=-1) : TypeParent(aLineNo)
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
		inline RtToken scheme() const
		{
			return m_scheme;
		}

		/** @brief Gets the number of vertices (maximum+1 of m_verts).
		 *
		 * @return The number of vertices (maximum+1 of m_verts).
		 */
		inline RtInt nVertices() const
		{
			return m_nvertices;
		}

		/** @brief Gets the number of edges (eq. number of vertices) of the surface.
		 *
		 * @return The Number of edges (eq. number of vertices) of the surface.
		 */
		inline RtInt nEdges() const
		{
			return m_nedges;
		}

		/** @brief Gets the number of vertices for each face (size is the number of faces).
		 *
		 * @return Number of vertices for each face (size is the number of faces).
		 */
		inline const std::vector<RtInt> &nVerts() const
		{
			return m_nverts;
		}

		inline RtInt nFaces() const
		{
			return static_cast<RtInt>(m_nverts.size());
		}

		inline RtInt nVertsOffs(RtInt faceIdx) const
		{
			assert(faceIdx <= (RtInt)m_nverts.size());
			RtInt result = 0;
			for ( int i = 0; i < faceIdx; ++i )
				result += m_nverts[i];
			return result;
		}

		/** @brief Gets the index for each vertex.
		 *
		 * @return The index for each vertex.
		 */
		inline const std::vector<RtInt> &verts() const
		{
			return m_verts;
		}

		/** @brief Gets the Number of integer and float args.
		 *
		 * @return The Number of integer and float args (integer, float) for each tag (size = 2*m_tags.size()).
		 */
		inline const std::vector<RtInt> &nArgs() const
		{
			return m_nargs;
		}

		/** @brief Gets the integer arguments for tags.
		 *
		 * @return The integer arguments for tags.
		 */
		inline const std::vector<RtInt> &intArgs() const
		{
			return m_intargs;
		}

		/** @brief Gets the float arguments for tags.
		 *
		 * @return The float arguments for tags.
		 */
		inline const std::vector<RtFloat> &floatArgs() const
		{
			return m_floargs;
		}

		/** @brief Gets the tags.
		 *
		 * @return The tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.
		 */
		inline const std::vector<RtToken> &tags() const
		{
			return m_tags;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.subdivisionMeshV(
								m_scheme, (RtInt)m_nverts.size(),
								m_nverts.empty() ? 0 : &(m_nverts[0]),
								m_verts.empty() ? 0 : &(m_verts[0]),
								(RtInt)m_tags.size(),
								m_tags.empty() ? 0 : &(m_tags[0]),
								m_nargs.empty() ? 0 : &(m_nargs[0]),
								m_intargs.empty() ? 0 : &(m_intargs[0]),
								m_floargs.empty() ? 0 : &(m_floargs[0]),
								paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(scheme());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			ribWriter.putBlank();
			ribWriter.putArray(verts());
			ribWriter.putBlank();
			ribWriter.putTokenArray(tags());
			ribWriter.putBlank();
			ribWriter.putArray(nArgs());
			ribWriter.putBlank();
			ribWriter.putArray(intArgs());
			ribWriter.putBlank();
			ribWriter.putArray(floatArgs());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSubdivisionMesh


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Hierarchical subdivision mesh surface.
	 */
	class CRiHierarchicalSubdivisionMesh : public CVarParamRManInterfaceCall {
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
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 * The default constructor sets the number of faces to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		CRiHierarchicalSubdivisionMesh(long aLineNo=-1) : TypeParent(aLineNo)
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

		/** @brief Converts a simple subdivision mesh to a hierarchical one
		 *
		 *  @param c Object to copy.
		 */
		explicit CRiHierarchicalSubdivisionMesh(const CRiSubdivisionMesh &c);

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
		inline RtToken scheme() const
		{
			return m_scheme;
		}

		/** @brief Gets the number of vertices (maximum+1 of m_verts).
		 *
		 * @return The number of vertices (maximum+1 of m_verts).
		 */
		inline RtInt nVertices() const
		{
			return m_nvertices;
		}

		inline RtInt nFaces() const
		{
			return static_cast<RtInt>(m_nverts.size());
		}

		inline RtInt nVertsOffs(RtInt faceIdx) const
		{
			assert(faceIdx <= (RtInt)m_nverts.size());
			RtInt result = 0;
			for ( int i = 0; i < faceIdx; ++i )
				result += m_nverts[i];
			return result;
		}

		/** @brief Gets the number of edges (eq. number of vertices) of the surface.
		 *
		 * @return The Number of edges (eq. number of vertices) of the surface.
		 */
		inline RtInt nEdges() const
		{
			return m_nedges;
		}

		/** @brief Gets the number of vertices for each face (size is the number of faces).
		 *
		 * @return Number of vertices for each face (size is the number of faces).
		 */
		inline const std::vector<RtInt> &nVerts() const
		{
			return m_nverts;
		}

		/** @brief Gets the index for each vertex.
		 *
		 * @return The index for each vertex.
		 */
		inline const std::vector<RtInt> &verts() const
		{
			return m_verts;
		}

		/** @brief Gets the Number of integer and float args.
		 *
		 * @return The Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
		 */
		inline const std::vector<RtInt> &nArgs() const
		{
			return m_nargs;
		}

		/** @brief Gets the integer arguments for tags.
		 *
		 * @return The integer arguments for tags.
		 */
		inline const std::vector<RtInt> &intArgs() const
		{
			return m_intargs;
		}

		/** @brief Gets the float arguments for tags.
		 *
		 * @return The float arguments for tags.
		 */
		inline const std::vector<RtFloat> &floatArgs() const
		{
			return m_floargs;
		}

		/** @brief Gets the string arguments for tags.
		 *
		 * @return The string arguments for tags.
		 */
		inline const std::vector<RtToken> &stringPtrArgs() const
		{
			return m_strptrargs;
		}

		/** @brief Gets the tags.
		 *
		 * @return The tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.
		 */
		inline const std::vector<RtToken> &tags() const
		{
			return m_tags;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.hierarchicalSubdivisionMeshV(
											m_scheme, (RtInt)m_nverts.size(),
											m_nverts.empty() ? 0 : &(m_nverts[0]),
											m_verts.empty() ? 0 : &(m_verts[0]),
											(RtInt)m_tags.size(),
											m_tags.empty() ? 0 : &(m_tags[0]),
											m_nargs.empty() ? 0 : &(m_nargs[0]),
											m_intargs.empty() ? 0 : &(m_intargs[0]),
											m_floargs.empty() ? 0 : &(m_floargs[0]),
											m_strptrargs.empty() ? 0 : &(m_strptrargs[0]),
											paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(scheme());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			ribWriter.putBlank();
			ribWriter.putArray(verts());
			ribWriter.putBlank();
			ribWriter.putTokenArray(tags());
			ribWriter.putBlank();
			ribWriter.putArray(nArgs());
			ribWriter.putBlank();
			ribWriter.putArray(intArgs());
			ribWriter.putBlank();
			ribWriter.putArray(floatArgs());
			ribWriter.putBlank();
			ribWriter.putArray(stringPtrArgs());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiHierarchicalSubdivisionMesh


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Sphere.
	 */
	class CRiSphere : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_radius,   ///< Radius.
				m_zmin,     ///< Minimal z coordinate (cut pole).
				m_zmax,     ///< Maximal z coordinate (cut pole).
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.sphereV(radius(), zMin(), zMax(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(radius());
			ribWriter.putBlank();
			ribWriter.putValue(zMin());
			ribWriter.putBlank();
			ribWriter.putValue(zMax());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiSphere


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Cone.
	 */
	class CRiCone : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_height,   ///< Height.
				m_radius,   ///< Radius.
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.coneV(height(), radius(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(height());
			ribWriter.putBlank();
			ribWriter.putValue(radius());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiCone


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Cylinder.
	 */
	class CRiCylinder : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_radius,   ///< Radius.
				m_zmin,     ///< Minimal z coordiante.
				m_zmax,     ///< Maximal z coordiante.
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.cylinderV(radius(), zMin(), zMax(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(radius());
			ribWriter.putBlank();
			ribWriter.putValue(zMin());
			ribWriter.putBlank();
			ribWriter.putValue(zMax());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiCylinder


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Hyperboloid.
	 */
	class CRiHyperboloid : public CVarParamRManInterfaceCall {
	private:
		RtPoint m_point1,   ///< First point of the line to sweep to get a hyperbolid.
				m_point2;   ///< Second point of the line to sweep to get a hyperbolid.
		RtFloat m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  The default constructor sets the size of the primitive to 0.
		 *
		 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiHyperboloid(long aLineNo = -1)
			: TypeParent(aLineNo)
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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
		RtPoint &point1()
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
		RtPoint &point2()
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.hyperboloidV(point1(), point2(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(point1()[0]);
			ribWriter.putBlank();
			ribWriter.putValue(point1()[1]);
			ribWriter.putBlank();
			ribWriter.putValue(point1()[2]);
			ribWriter.putBlank();
			ribWriter.putValue(point2()[0]);
			ribWriter.putBlank();
			ribWriter.putValue(point2()[1]);
			ribWriter.putBlank();
			ribWriter.putValue(point2()[2]);
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiHyperboloid


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Paraboloid.
	 */
	class CRiParaboloid : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_rmax,     ///< Maximal radius (on zmax).
				m_zmin,     ///< Minimal z coordinate.
				m_zmax,     ///< Maximal z coordinate.
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.paraboloidV(rMax(), zMin(), zMax(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(rMax());
			ribWriter.putBlank();
			ribWriter.putValue(zMin());
			ribWriter.putBlank();
			ribWriter.putValue(zMax());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiParaboloid


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Disk.
	 */
	class CRiDisk : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_height,   ///< Distance from disk to origin on positive z axis.
				m_radius,   ///< Radius.
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.diskV(height(), radius(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(height());
			ribWriter.putBlank();
			ribWriter.putValue(radius());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiDisk


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Torus.
	 */
	class CRiTorus : public CVarParamRManInterfaceCall {
	private:
		RtFloat m_majorrad, ///< Major radius (center of the ring).
				m_minorrad, ///< Minor radius.
				m_phimin,   ///< Start angle to sweep the torus body.
				m_phimax,   ///< End angle to sweep the torus body.
				m_thetamax; ///< Sweep angle.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
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
			: TypeParent(aLineNo),
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
			: TypeParent(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.torusV(majorRad(), minorRad(), phiMin(), phiMax(), thetaMax(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(majorRad());
			ribWriter.putBlank();
			ribWriter.putValue(minorRad());
			ribWriter.putBlank();
			ribWriter.putValue(phiMin());
			ribWriter.putBlank();
			ribWriter.putValue(phiMax());
			ribWriter.putBlank();
			ribWriter.putValue(thetaMax());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiTorus


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Points.
	 */
	class CRiPoints : public CVarParamRManInterfaceCall {
	private:
		RtInt m_npts; ///< Number of points.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  The default constructor sets the number of points to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiPoints(long aLineNo = -1)
			: TypeParent(aLineNo)
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
			: TypeParent(aLineNo, decl, CPointsClasses(aNPts), curColorDescr, n, tokens, params),
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
			: TypeParent(aLineNo, theParameters),
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

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiPoints(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }

		/** @brief Gets the number of points.
		 *
		 *  @return Number of points.
		 */
		inline virtual RtInt nPoints() const
		{
			return m_npts;
		}

		/** @brief Sets the number of points.
		 *
		 *  @param aNPts Number of points.
		 */
		inline virtual void nPoints(RtInt aNPts)
		{
			m_npts = aNPts;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.pointsV(nPoints(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
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
		inline CRiPoints &operator=(const CRiPoints &c)
		{
			if ( this == &c )
				return *this;

			nPoints(c.nPoints());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiPoints


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Curves.
	 */
	class CRiCurves : public CVarParamRManInterfaceCall {
	private:
		RtToken m_type, ///< Type of the curve either RI_LINEAR or RI_CUBIC.
				m_wrap; ///< Wrap at the ends either RI_PERIODIC or RI_NONPERIODIC.
		RtInt   m_step; ///< Step to the next curve segment (by basis, v parameter)
		std::vector<RtInt> m_nverts; ///< Number of vertices for each curve (size == number of curves)
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  The default constructor sets the number of curves to 0.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		CRiCurves(long aLineNo = -1) : TypeParent(aLineNo)
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.curvesV(
					   m_type, (RtInt)m_nverts.size(),
					   m_nverts.empty() ? 0 : &m_nverts[0],
					   m_wrap,
					   paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			ribWriter.putBlank();
			ribWriter.putArray(nVerts());
			ribWriter.putBlank();
			ribWriter.putStringToken(wrap());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiCurves


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Blobby.
	 */
	class CRiBlobby : public CVarParamRManInterfaceCall {
	private:
		RtInt                   m_nleaf;        ///< Number of primitive blobs, has to match the content of m_code.
		std::vector<RtInt>      m_code;         ///< Code array, s.a. [RISPEC3.2] for the opcodes.
		std::vector<RtFloat>    m_flt;          ///< The float operands for the code.
		std::deque<std::string> m_strcontainer; ///< The string operants for the code.
		std::vector<RtString>   m_str;          ///< C-String pointers of m_strcontainer.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  Empty object.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 */
		inline CRiBlobby(long aLineNo = -1) : TypeParent(aLineNo)
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

		inline RtInt nCode() const
		{
			return (RtInt)m_code.size();
		}

		/** @brief Get the float operands for the code.
		 *
		 * @return The float operands for the code.
		 */
		inline const std::vector<RtFloat> &flt() const
		{
			return m_flt;
		}

		inline RtInt nFlt() const
		{
			return (RtInt)m_flt.size();
		}

		/** @brief Get the string operands for the code.
		 *
		 * @return The string operands for the code.
		 */
		inline const std::vector<RtToken> &str() const
		{
			return m_str;
		}

		inline RtInt nStr() const
		{
			return (RtInt)m_str.size();
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

		inline virtual void process(IRiRoot &ri)
		{
			ri.blobbyV(
					   m_nleaf,
					   (RtInt)m_code.size(), m_code.empty() ? 0 : &m_code[0],
					   (RtInt)m_flt.size(), m_flt.empty() ? 0 : &m_flt[0],
					   (RtInt)m_str.size(), m_str.empty() ? 0 : &m_str[0],
					   paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putValue(nLeaf());
			ribWriter.putBlank();
			ribWriter.putArray(code());
			ribWriter.putBlank();
			ribWriter.putArray(flt());
			ribWriter.putBlank();
			ribWriter.putArray(str());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiBlobby


	///////////////////////////////////////////////////////////////////////////////
	/** @brief Special Geometry.
	 */
	class CRiGeometry : public CVarParamRManInterfaceCall {
	private:
		RtToken m_type; ///< Type of the geometry as atomized string.
	protected:
		typedef CVarParamRManInterfaceCall TypeParent;
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
			return TypeParent::isKindOf(atomizedClassName);
		}

		/** @brief Default constructor.
		 *
		 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
		 *  @param aName Name of the geometry as atomized string.
		 */
		inline CRiGeometry(long aLineNo = -1, RtToken aType = RI_NULL)
			: TypeParent(aLineNo), m_type(aType)
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
			RtToken aType,
			RtInt n, RtToken tokens[], RtPointer params[])
			: TypeParent(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params),
			  m_type(aType)
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
			RtToken aType,
			const CParameterList &theParameters)
			: TypeParent(aLineNo, theParameters),
			  m_type(aType)
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

		inline virtual CRManInterfaceCall *duplicate() const
		{
			return new CRiGeometry(*this);
		}

		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }

		/** @brief Gets the name of the attribute as atomized string.
		 *
		 *  @return The name of the attribute as atomized string.
		 */
		inline RtToken type() const
		{
			return m_type;
		}

		/** @brief Sets the name of the attribute as atomized string.
		 *
		 *  @param aName The name of the attribute as atomized string.
		 */
		inline void type(RtToken aType)
		{
			m_type = aType;
		}

		inline virtual void process(IRiRoot &ri)
		{
			ri.geometryV(type(), paramSize(), tokenPtr(), valuePtr());
		}
		
		inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.preProcess(*this);
		}
		
		inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.doProcess(*this);
		}
		
		inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
		{
			ri.postProcess(*this);
		}
		
		inline virtual void writeRIB(CRibElementsWriter &ribWriter, RtInt n=0, const RtToken ignoreTokens[]=0) const
		{
			ribWriter.putRequest(interfaceIdx());
			ribWriter.putBlank();
			ribWriter.putStringToken(type());
			TypeParent::writeRIB(ribWriter, n, ignoreTokens);
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

			type(c.type());

			TypeParent::operator=(c);
			return *this;
		}
	}; // CRiGeometry

}

#endif // _RICPP_RICONTEXT_RIMACROPRIMS_H
