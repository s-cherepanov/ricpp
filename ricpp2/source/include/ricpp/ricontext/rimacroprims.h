#ifndef _RICPP_RICONTEXT_RIMACROPRIMS_H
#define _RICPP_RICONTEXT_RIMACROPRIMS_H

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
	RtInt m_nVertices; //!< Number of vertices of the polygon must match the number of positions "P".

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 * The default constructor sets the number of vertices to 0.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPolygon(long aLineNo=-1)
		: CPolygonRManInterfaceCall(aLineNo), m_nVertices(0)
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
		: CPolygonRManInterfaceCall(aLineNo), m_nVertices(theNVertices)
	{
		CPolygonClasses p(theNVertices);
		setParams(decl, p, curColorDescr, n, tokens, params);
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
		: CPolygonRManInterfaceCall(aLineNo, theParameters), m_nVertices(theNVertices)
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
		ri.prePolygon(m_nVertices, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPolygon(m_nVertices, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPolygon(m_nVertices, parameters());
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

		CPolygonRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPolygon

///////////////////////////////////////////////////////////////////////////////
/** @brief General Polygon
 */
class CRiGeneralPolygon : public CPolygonRManInterfaceCall {
private:
	std::vector<RtInt> m_nVerts; //!< Number of verts per outline (Number of loops is the size of m_nVerts).
	/** @brief enters the values.
	 *
	 *  @param theNLoops Number of loops
	 *  @param theNVerts Number of vertices/loop, size of \a nverts vector is \a nloops.
	 */
	void enterValues(RtInt theNLoops, RtInt *theNVerts);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
	 *  @param theNVerts Number of vertices/loop, size of \a nverts vector is \a nloops.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	CRiGeneralPolygon(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt theNLoops, RtInt *theNVerts,
		RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param theNLoops Number of loops
	 *  @param theNVerts Number of vertices/loop, size of \a nverts vector is \a nloops.
	 *  @param theParameters Parsed parameter list.
	 */
	CRiGeneralPolygon(
		long aLineNo,
		RtInt theNLoops, RtInt *theNVerts,
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
	 *  @retrun The number of vertices (size is the number of loops).
	 */
	inline const std::vector<RtInt> &nVerts() const
	{
		return m_nVerts;
	}

	/** @brief Gets the number of positions.
	 *
	 *  @retrun The number of positions.
	 */
	inline RtInt numPos() const
	{
		(RtInt)sum(m_nVerts.size(), &(m_nVerts[0]));
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
	inline void set(RtInt theNLoops, RtInt *theNVerts)
	{
		enterValues(theNLoops, theNVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeneralPolygon((RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doGeneralPolygon((RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postGeneralPolygon((RtInt)m_nVerts.size(),
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
	std::vector<RtInt> m_nVerts; //!< Number of vertices per polygon (size is the number of polygons).
	std::vector<RtInt> m_verts;  //!< Indices of the vertices.

	/** @brief enters the values.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNVerts Number of verts per polygon.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	void enterValues(RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts,
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
		RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts,
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
	 *  @retrun The numbers of the vertices (size is the number of polygons).
	 */
	inline const std::vector<RtInt> &nVerts() const
	{
		return m_nVerts;
	}

	/** @brief Gets the indices of the vertices.
	 *
	 *  @retrun The indizes of the vertices.
	 */
	inline const std::vector<RtInt> &verts() const
	{
		return m_verts;
	}

	/** @brief Gets the number of the positions.
	 *
	 *  @retrun The number of the positions.
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
	inline void set(RtInt theNPolys, RtInt *theNVerts, RtInt *theVerts)
	{
		enterValues(theNPolys, theNVerts, theVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsPolygons(
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPointsPolygons(
			(RtInt)m_nVerts.size(),
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPointsPolygons(
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
	std::vector<RtInt> m_nLoops; //!< Loops per polygon (Number of polygons is the size of the vector).
	std::vector<RtInt> m_nVerts; //!< Vertices per loop.
	std::vector<RtInt> m_verts;  //!< Vertex indices.

	/** @brief Enters the values.
	 *
	 *  @param theNPolys Number of polygons.
	 *  @param theNLoops Number of loops per polygon.
	 *  @param theNVerts Number of verts per loop.
	 *  @param theVerts The vertex indices of the polygons.
	 */
	void enterValues(RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts);
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts,
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
		RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts,
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
	 *  @retrun The number of the positions.
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
	inline void set(RtInt theNPolys, RtInt *theNLoops, RtInt *theNVerts, RtInt *theVerts)
	{
		enterValues(theNPolys, theNLoops, theNVerts, theVerts);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePointsGeneralPolygons(
			(RtInt)m_nLoops.size(),
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPointsGeneralPolygons(
			(RtInt)m_nLoops.size(),
			m_nLoops.empty() ? 0 : &m_nLoops[0],
			m_nVerts.empty() ? 0 : &m_nVerts[0],
			m_verts.empty() ? 0 : &m_verts[0],
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPointsGeneralPolygons(
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
	RtToken m_type; //!< Type of the patch, either RI_BILINEAR or RI_BICUBIC (or RI_NULL if not set).

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		: CUVRManInterfaceCall(aLineNo), m_type(aType)
	{
		CPatchClasses p(aType);
		setParams(decl, p, curColorDescr, n, tokens, params);
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
		: CUVRManInterfaceCall(aLineNo), m_type(aType)
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
	 *  @param aName The type of the attribute as atomized string.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatch(m_type, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPatch(m_type, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPatch(m_type, parameters());
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
	RtToken m_type,  //!< Type of the mesh either RI_BILINEAR or RI_BICUBIC.
	        m_uwrap, //!< Wrap in parametric u direction RI_PERIODIC or RI_NONPERIODIC.
	        m_vwrap; //!< Wrap in parametric v direction RI_PERIODIC or RI_NONPERIODIC.
	RtInt   m_nu,    //!< Number of control points in parametric u direction.
	        m_nv;    //!< Number of control points in parametric v direction.
	RtInt   m_ustep, //!< Step m_ustep control points to the next patch in u direction (from the basis).
	        m_vstep; //!< Step m_vstep control points to the next patch in v direction (from the basis).

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		ri.prePatchMesh(m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPatchMesh(m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPatchMesh(m_type, m_nu, m_uwrap, m_nv, m_vwrap, parameters());
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
	RtInt m_nu,     //!< Number of control points in parametric u direction.
	      m_uorder, //!< Order of the polynomial basis for parameters u.
	      m_nv,     //!< Number of control points in parametric v direction.
	      m_vorder; //!< Order of the polynomial basis for parameters v.
	std::vector<RtFloat> m_uknot, //!< Knot vector for the u parameters.
	                     m_vknot; //!< Knot vector for the v parameters.

	RtFloat m_umin, //!< Minimal value for parameter u.
	        m_umax, //!< Maximal value for parameter u.
	        m_vmin, //!< Minimal value for parameter v.
	        m_vmax; //!< Maximal value for parameter v.
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax,
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
		RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax,
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
		RtInt aNU, RtInt aUOrder, const RtFloat *aUKnot, RtFloat aUMin, RtFloat aUMax,
		RtInt aNV, RtInt aVOrder, const RtFloat *aVKnot, RtFloat aVMin, RtFloat aVMax);

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
		ri.preNuPatch(m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doNuPatch(m_nu, m_uorder,
			m_uknot.empty() ? 0 : &m_uknot[0],
			m_umin, m_umax, m_nv, m_vorder,
			m_vknot.empty() ? 0 : &m_vknot[0],
			m_vmin, m_vmax,
			parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postNuPatch(m_nu, m_uorder,
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
	RtToken m_scheme;    //!< The scheme (currently RI_CATMULL_CLARK only).
	RtInt   m_nvertices, //!< The number of vertices (maximum+1 of m_verts).
	        m_nedges;    //!< Number of edges (eq. number of vertices) of the surface.
	std::vector<RtInt>      m_nverts,  //!< Number of vertices for each face (size is the number of faces).
	                        m_verts,   //!< Index for each vertex.
	                        m_nargs,   //!< Number of integer and float args (integer, float) for each tag (size = 2*m_tags.size()).
	                        m_intargs; //!< Integer arguments for tags.
	std::vector<RtFloat>    m_floargs; //!< Float arguments for tags.
	std::vector<RtToken>    m_tags;    //!< Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
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
		long aLineNo,
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[],
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
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[]);

	/** @brief Sets the values of the member variables.
	 *
	 *  @param aScheme  The scheme (currently RI_CATMULL_CLARK only).
	 *  @param aNVerts  Number of vertices for each face (size is the number of faces).
	 *  @param aVerts   Index for each vertex.
	 *  @param aTags    Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...)
	 *  @param aNArgs   Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	 *  @param aIntArgs Integer arguments for tags.
	 *  @param aFloArgs Float arguments for tags.
	 */
	void set(
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
		ri.preSubdivisionMesh(
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
		ri.doSubdivisionMesh(
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
		ri.postSubdivisionMesh(
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

		set(c.scheme(), c.nVerts(), c.verts(), c.tags(), c.nArgs(), c.intArgs(), c.floatArgs()); 

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiSubdivisionMesh


///////////////////////////////////////////////////////////////////////////////
/** @brief Hierarchical subdivision mesh surface.
 */
class CRiHierarchicalSubdivisionMesh : public CGeometryRManInterfaceCall {
private:
	RtToken m_scheme;    //!< The scheme (currently RI_CATMULL_CLARK only).
	RtInt   m_nvertices, //!< The number of vertices (maximum+1 of m_verts).
	        m_nedges;    //!< Number of edges (eq. number of vertices) of the surface.
	std::vector<RtInt>       m_nverts,  //!< Number of vertices for each face (size is the number of faces).
	                         m_verts,   //!< Index for each vertex.
	                         m_nargs,   //!< Number of integer, float and string args (integer, float, string) for each tag (size = 3*m_tags.size()).
	                         m_intargs; //!< Integer arguments for tags.
	std::vector<RtFloat>     m_floargs; //!< Float arguments for tags.
	std::vector<RtToken>     m_strargs; //!< String arguments for tags.
	std::vector<RtToken>     m_tags;    //!< Tags (RI_HOLE, RI_CREASE, RI_CORNER, ...), size is the number of tags.

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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
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
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[], RtToken aStrArgs[],
		RtInt n, RtToken tokens[], RtPointer params[]
		);

	/** @brief Constructor.
	 *
	 *  @param aLineNo  The line number to store, if aLineNo is initialized to -1 (a line number is not known)
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
		long aLineNo,
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[], RtToken aStrArgs[],
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
		RtToken aScheme, RtInt aNFaces, RtInt aNVerts[], RtInt aVerts[],
		RtInt aNTags, RtToken aTags[], RtInt aNArgs[], RtInt aIntArgs[], RtFloat aFloArgs[], RtToken aStrArgs[]);

	/** @brief Sets the values of the member variables.
	 *
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
	const std::vector<RtToken> &stringArgs() const
	{
		return m_strargs;
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
		ri.preHierarchicalSubdivisionMesh(
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strargs.empty() ? 0 : &(m_strargs[0]),
			parameters()
			);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doHierarchicalSubdivisionMesh(
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strargs.empty() ? 0 : &(m_strargs[0]),
			parameters()
			);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postHierarchicalSubdivisionMesh(
			m_scheme, (RtInt)m_nverts.size(),
			m_nverts.empty() ? 0 : &(m_nverts[0]),
			m_verts.empty() ? 0 : &(m_verts[0]),
			(RtInt)m_tags.size(),
			m_tags.empty() ? 0 : &(m_tags[0]),
			m_nargs.empty() ? 0 : &(m_nargs[0]),
			m_intargs.empty() ? 0 : &(m_intargs[0]),
			m_floargs.empty() ? 0 : &(m_floargs[0]),
			m_strargs.empty() ? 0 : &(m_strargs[0]),
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

		set(c.scheme(), c.nVerts(), c.verts(), c.tags(), c.nArgs(), c.intArgs(), c.floatArgs(), c.stringArgs()); 

		CGeometryRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiHierarchicalSubdivisionMesh


///////////////////////////////////////////////////////////////////////////////
/** @brief Sphere.
 */
class CRiSphere : public CUVRManInterfaceCall {
private:
	RtFloat m_radius,   //!< Radius.
	        m_zmin,     //!< Minimum z coordinate (cut pole).
			m_zmax,     //!< Maximum z coordinate (cut pole).
			m_thetamax; //!< Sweep angle.
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
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  The default constructor sets the size of the primitive to 0.
	 *
	 *  @param aLineNo   The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimum z coordinate (cut pole).
     *  @param aZMax     Maximum z coordinate (cut pole).
     *  @param aThetaMax Sweep angle.
	 */
	CRiSphere(long aLineNo=-1,
		RtFloat aRadius = 0, RtFloat aZMin = 0, RtFloat aZMax = 0, RtFloat aThetaMax = 360)
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
     *  @param aZMin     Minimum z coordinate (cut pole).
     *  @param aZMax     Maximum z coordinate (cut pole).
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
	 *  @param decl      Dictonary with the current declarations.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimum z coordinate (cut pole).
     *  @param aZMax     Maximum z coordinate (cut pole).
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
     *  @retval aZMin     Minimum z coordinate (cut pole).
     *  @retval aZMax     Maximum z coordinate (cut pole).
     *  @retval aThetaMax Sweep angle.
	 */
	void get(RtFloat &aRadius, RtFloat &aZMin, RtFloat &aZMax, RtFloat &aThetaMax)
	{
		aRadius = m_radius;
		aZMin = m_zmin;
		aZMax = m_zmax;
		aThetaMax = m_thetamax;
	}

	/** @brief Sets the member variables of a sphere.
	 *
	 *  @param aRadius   Radius.
     *  @param aZMin     Minimum z coordinate (cut pole).
     *  @param aZMax     Maximum z coordinate (cut pole).
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
protected:
	RtFloat m_height,   //!< Height.
	        m_radius,   //!< Radius.
			m_thetamax; //!< Sweep angle.
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
	RtFloat m_radius,   //!< Radius.
	        m_zmin,     //!< Minimal z coordiante.
			m_zmax,     //!< Maximal z coordiante.
			m_thetamax; //!< Sweep angle.
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
	RtPoint m_point1,   //!< First point of the line to sweep to get a hyperbolid.
	        m_point2;   //!< Second point of the line to sweep to get a hyperbolid.
	RtFloat m_thetamax; //!< Sweep angle.
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
	RtFloat m_rmax,     //!< Maximal radius (on zmax).
            m_zmin,     //!< Minimal z coordinate.
            m_zmax,     //!< Maximal z coordinate.
	        m_thetamax; //!< Sweep angle.
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
	RtFloat m_height,   //!< Distance from disk to origin on positive z axis.
	        m_radius,   //!< Radius.
	        m_thetamax; //!< Sweep angle.
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
	RtFloat m_majorrad, //!< Major radius (center of the ring).
	        m_minorrad, //!< Minor radius.
	        m_phimin,   //!< Start angle to sweep the torus body.
	        m_phimax,   //!< End angle to sweep the torus body.
	        m_thetamax; //!< Sweep angle.
public:
	inline static const char *myClassName(void) { return "CRiTorus"; }
	inline virtual const char *className() const { return CRiTorus::myClassName(); }

	inline CRiTorus(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CUVRManInterfaceCall(aLineNo, decl, CQuadricClasses(), curColorDescr, n, tokens, params),
		  m_majorrad(majorrad), m_minorrad(minorrad),
		  m_phimin(phimin), m_phimax(phimax), m_thetamax(thetamax)
	{
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
	inline CRiTorus &operator=(const CRiTorus &c)
	{
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
	RtInt m_npts; //!< Number of points.
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
	RtToken m_name; //!< Name of the geometry (only RI_TEAPOT)
public:
	inline static const char *myClassName(void) { return "CRiGeometry"; }
	inline virtual const char *className() const { return CRiGeometry::myClassName(); }

	inline CRiGeometry(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CGeometryRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preGeometry(m_name, parameters());
		ri.doGeometry(m_name, parameters());
	}
	inline CRiGeometry &operator=(const CRiGeometry &) {
		return *this;
	}
}; // CRiGeometry

}

#endif // _RICPP_RICONTEXT_RIMACROPRIMS_H