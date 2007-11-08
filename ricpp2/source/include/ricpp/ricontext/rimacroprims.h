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
	 *  @retrun The sum of the number of the positions.
	 */
	inline RtInt numPos() const
	{
		return (RtInt)tmax(m_verts.size(), &(m_verts[0]));
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
	 *  @retrun The sum of the number of the positions.
	 */
	inline RtInt numPos() const
	{
		return (RtInt)tmax(m_verts.size(), &(m_verts[0]));
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
	 *  @param aType Type of the patch.
	 */
	inline CRiPatch(long aLineNo=-1, RtToken aType=RI_NULL)
		: CUVRManInterfaceCall(aLineNo), m_type(aType)
	{
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
		long aLineNo,
		RtInt ustep, RtInt vstep, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		const CParameterList &theParameters);

	inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePatchMesh(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), parameters());
		ri.doPatchMesh(m_type.c_str(), m_nu, m_uwrap.c_str(), m_nv, m_vwrap.c_str(), parameters());
	}
	inline CRiPatchMesh &operator=(const CRiPatchMesh &c)
	{
		if ( this == &c )
			return *this;

		CUVRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPatchMesh

///////////////////////////////////////////////////////////////////////////////
class CRiNuPatch : public CGeometryRManInterfaceCall {
protected:
	RtInt m_nu, m_uorder, m_nv, m_vorder;
	std::vector<RtFloat> m_uknot, m_vknot;
	RtFloat m_umin, m_umax, m_vmin, m_vmax;
	void enterValues(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax);
public:
	inline static const char *myClassName(void) { return "CRiNuPatch"; }
	inline virtual const char *className() const { return CRiNuPatch::myClassName(); }

	CRiNuPatch(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[]);
	CRiNuPatch(
		long aLineNo,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		const CParameterList &theParameters);
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
	inline CRiNuPatch &operator=(const CRiNuPatch &c)
	{
		if ( this == &c )
			return *this;

		CGeometryRManInterfaceCall::operator=(c);
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

	inline CRiSphere &operator=(const CRiSphere &c)
	{
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

}

#endif // _RICPP_RICONTEXT_RIMACROPRIMS_H