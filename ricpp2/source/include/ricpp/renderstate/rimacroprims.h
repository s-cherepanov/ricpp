#ifndef _RICPP_RENDERSTATE_RIMACROPRIMS_H
#define _RICPP_RENDERSTATE_RIMACROPRIMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROBASE_H
#include "ricpp/renderstate/rimacrobase.h"
#endif // _RICPP_RENDERSTATE_RIMACROBASE_H

namespace RiCPP {

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

}

#endif // _RICPP_RENDERSTATE_RIMACROPRIMS_H