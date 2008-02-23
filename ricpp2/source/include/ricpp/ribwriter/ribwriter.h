#ifndef _RICPP_RIBWRITER_RIBWRITER_H
#define _RICPP_RIBWRITER_RIBWRITER_H

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

/** @file ribwriter.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a context to write RIB streams
 */

#ifndef _RICPP_BASERENDERER_BASERENDERER_H
#include "ricpp/baserenderer/baserenderer.h"
#endif // _RICPP_BASERENDERER_BASERENDERER_H

#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H
#include "ricpp/ricontext/contextcreator.h"
#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H

#ifndef _RICPP_RICONTEXT_RIBELEMENTS_H
#include "ricpp/ricontext/ribelements.h"
#endif // _RICPP_RICONTEXT_RIBELEMENTS_H

namespace RiCPP {

#define RIBWRITER_NAME "ribwriter"
#define RIBWRITERCREATOR_NAME "ribwritercreator"

/** @brief Rendering context that writes a RIB stream
 *
 *  This class implements the output part of the RIB binding
 *  (see [RISPEC3.2], appendix C.2), RIB can be parsed using CRibParser.
 */
class CRibWriter : public CBaseRenderer, public IRequestNotification {
private:
	CRibElementsWriter *m_writer;
	TemplFrontStreambuf<char> *m_buffer;
	std::string m_cmd;
	std::string m_nativepath;

	RtToken RI_COMPRESS;
	RtToken RI_POSTPONE_PROCEDURALS;
	RtToken RI_POSTPONE_OBJECTS;
	RtToken RI_POSTPONE_FILE_ARCHIVES;
	RtToken RI_POSTPONE_INLINE_ARCHIVES;
	RtToken RI_SKIP_HEADERS;
	RtToken RI_SKIP_VERSION;
	RtToken RI_INDENT;
	RtToken RI_INDENT_STRING;
	RtToken RI_SUPPRESS_OUTPUT;
	RtToken RI_BINARY_OUTPUT;

	RtToken RI_RIBWRITER;

	bool m_suppressOutput;
	bool m_controlSuppressOutput;
	std::vector<bool> m_suppressOutputVector;

	// Control

	int m_postponeProcedural;
	int m_postponeObject;
	int m_postponeFileArchive;
	int m_postponeInlineArchive;
	bool m_header; ///< True, till first request called (in archives and main)
	int m_skipHeader;
	int m_skipVersion; ///< skips version request: 1: skip version, 0: write version, -1 write only once (default)
	bool m_execute;
	bool m_doReadArchive;
	bool m_indent;
	std::string m_indentString;

	bool m_binary;

	bool willExecuteMacro(RtString name);


	unsigned long m_nestingDepth; ///< Depth of begin/end blocks

	inline unsigned long nestingDepth() const
	{
		return m_nestingDepth;
	}

	inline void nestingDepth(unsigned long aDepth)
	{
		m_nestingDepth = aDepth;
	}
	
	inline unsigned long incNestingDepth()
	{
		return ++m_nestingDepth;
	}

	inline unsigned long decNestingDepth()
	{
		return --m_nestingDepth;
	}
protected:
	virtual void defaultDeclarations();
	bool testValid() const;
	bool postTestValid() const;
	void writePrefix(bool isArchiveRecord=false);
	void writeParameterList(const CParameterList &params, RtInt n=0, RtToken ignoreTokens[]=0);

public:

	CRibWriter();
	virtual ~CRibWriter();

	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	static RtToken myRendererType();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual RtToken rendererType() const { return myRendererType(); }

	virtual void requestWritten(EnumRequests aRequest);

public:
	virtual RtVoid doControl(RtToken name, const CParameterList &params);
	virtual RtVoid version();

	/*
	inline virtual RtVoid doAbort(void) { CBaseRenderer::doAbort(); }
	inline virtual RtVoid doActivate(void) { CBaseRenderer::doActivate(); }
	inline virtual RtVoid doDeactivate(void) { CBaseRenderer::doDeactivate(); }
	*/

	virtual RtVoid postErrorHandler(const IErrorHandler &handler);

	virtual RtVoid postDeclare(RtToken name, RtString declaration);
	virtual RtVoid postSynchronize(RtToken name);
	virtual RtVoid postSystem(RtString cmd);
	virtual RtVoid postResource(RtToken handle, RtToken type, const CParameterList &params);

	virtual RtVoid postBegin(RtString name, const CParameterList &params);
	virtual RtVoid postEnd(void);

	virtual RtVoid postFrameBegin(RtInt number);
	virtual RtVoid postFrameEnd(void);

	virtual RtVoid postWorldBegin(void);
	virtual RtVoid postWorldEnd(void);

	virtual RtVoid postAttributeBegin(void);
	virtual RtVoid postAttributeEnd(void);

	virtual RtVoid postTransformBegin(void);
	virtual RtVoid postTransformEnd(void);

    virtual RtVoid postSolidBegin(RtToken type);
    virtual RtVoid postSolidEnd(void);

	virtual RtVoid postObjectBegin(RtObjectHandle h, RtString name);

	virtual RtVoid preObjectEnd(void);
	virtual RtVoid postObjectEnd(void);

	virtual RtVoid doObjectInstance(RtObjectHandle handle);
	virtual RtVoid postObjectInstance(RtObjectHandle handle);

    virtual RtVoid postMotionBegin(RtInt N, RtFloat times[]);
    virtual RtVoid postMotionEnd(void);

	virtual RtVoid postResourceBegin(void);
	virtual RtVoid postResourceEnd(void);

	virtual RtVoid postArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params);
	
	virtual RtVoid preArchiveEnd(void);
	virtual RtVoid postArchiveEnd(void);

    virtual RtVoid postFormat(RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid postFrameAspectRatio(RtFloat aspect);
	virtual RtVoid postScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid postCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
    virtual RtVoid postProjection(RtToken name, const CParameterList &params);
	virtual RtVoid postClipping(RtFloat hither, RtFloat yon);
    virtual RtVoid postClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
    virtual RtVoid postDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
    virtual RtVoid postShutter(RtFloat smin, RtFloat smax);
	virtual RtVoid postPixelVariance(RtFloat variation);
    virtual RtVoid postPixelSamples(RtFloat xsamples, RtFloat ysamples);
    virtual RtVoid postPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
    virtual RtVoid postExposure(RtFloat gain, RtFloat gamma);
    virtual RtVoid postImager(RtString name, const CParameterList &params);
	virtual RtVoid postQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
	virtual RtVoid postDisplayChannel(RtString channel, const CParameterList &params);
    virtual RtVoid postDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params);
    virtual RtVoid postHider(RtToken type, const CParameterList &params);
    virtual RtVoid postColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]);
    virtual RtVoid postRelativeDetail(RtFloat relativedetail);
    virtual RtVoid postOption(RtToken name, const CParameterList &params);
	
    virtual RtVoid postLightSource(RtLightHandle h, RtString name, const CParameterList &params);
	virtual RtVoid postAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params);

    virtual RtVoid postAttribute(RtToken name, const CParameterList &params);
	virtual RtVoid postColor(RtColor Cs);
	virtual RtVoid postOpacity(RtColor Os);
    virtual RtVoid postSurface(RtString name, const CParameterList &params);
    virtual RtVoid postAtmosphere(RtString name, const CParameterList &params);
    virtual RtVoid postInterior(RtString name, const CParameterList &params);
	virtual RtVoid postExterior(RtString name, const CParameterList &params);
	virtual RtVoid postIlluminate(RtLightHandle light, RtBoolean onoff);
    virtual RtVoid postDisplacement(RtString name, const CParameterList &params);
	virtual RtVoid postTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
    virtual RtVoid postShadingRate(RtFloat size);
	virtual RtVoid postShadingInterpolation(RtToken type);
    virtual RtVoid postMatte(RtBoolean onoff);
	virtual RtVoid postBound(RtBound bound);
	virtual RtVoid postDetail(RtBound bound);
	virtual RtVoid postDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid postGeometricApproximation(RtToken type, RtFloat value);
	virtual RtVoid postGeometricRepresentation(RtToken type);
	virtual RtVoid postOrientation(RtToken anOrientation);
	virtual RtVoid postReverseOrientation(void);
	virtual RtVoid postSides(RtInt nsides);
    virtual RtVoid postBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid postTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

	virtual RtVoid postIdentity(void);
	virtual RtVoid postTransform(RtMatrix aTransform);
	virtual RtVoid postConcatTransform(RtMatrix aTransform);
	virtual RtVoid postPerspective(RtFloat fov);
	virtual RtVoid postTranslate(RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid postRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid postScale(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid postSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
	virtual RtVoid postDeformation(RtString name, const CParameterList &params);
	virtual RtVoid postScopedCoordinateSystem(RtToken space);
	virtual RtVoid postCoordinateSystem(RtToken space);
	virtual RtVoid postCoordSysTransform(RtToken space);

    virtual RtVoid postPolygon(RtInt nvertices, const CParameterList &params);
	virtual RtVoid postGeneralPolygon(RtInt nloops, RtInt nverts[], const CParameterList &params);
	virtual RtVoid postPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params);
    virtual RtVoid postPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params);

	virtual RtVoid postPatch(RtToken type, const CParameterList &params);
	virtual RtVoid postPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params);
    virtual RtVoid postNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params);

	virtual RtVoid postSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params);
	virtual RtVoid postHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params);

	virtual RtVoid postSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params);

    virtual RtVoid postPoints(RtInt npts, const CParameterList &params);
    virtual RtVoid postCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params);

	virtual RtVoid postBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params);

	virtual RtVoid doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc);
	virtual RtVoid postProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc);

	virtual RtVoid postGeometry(RtToken type, const CParameterList &params);

	virtual RtVoid postMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeShadow(RtString pic, RtString tex, const CParameterList &params);
	virtual RtVoid postMakeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params);

	virtual RtVoid postArchiveRecord(RtToken type, RtString line);

	virtual RtVoid preReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params);
	virtual RtVoid doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params);
	virtual RtVoid postReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params);

	virtual RtVoid postIfBegin(RtString expr);
	virtual RtVoid postElseIfBegin(RtString expr);
	virtual RtVoid postElseBegin(void);
	virtual RtVoid postIfEnd(void);
};

/** @brief Creator for the CRibWriter rendering context.
 */
class CRibWriterCreator : public CContextCreator {
protected:
	inline virtual IRiContext *getNewContext() { return new CRibWriter; }

public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	inline virtual unsigned long majorContextVersion() const { return CRibWriter::myMajorVersion(); }
	inline virtual unsigned long minorContextVersion() const { return CRibWriter::myMinorVersion(); }
	inline virtual unsigned long contextRevision() const { return CRibWriter::myRevision(); }
	inline virtual RtToken contextName() const { return CRibWriter::myName(); }
	inline virtual RtToken contextType() const { return CRibWriter::myType(); }
	inline virtual RtToken rendererType() const { return CRibWriter::myRendererType(); }
};

} // namespace RiCPP

#endif // _RICPP_RIBWRITER_RIBWRITER_H
