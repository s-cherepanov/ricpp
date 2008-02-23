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
	virtual RtVoid version();

	virtual RtVoid preArchiveEnd(CRiArchiveEnd &obj);
	virtual RtVoid preObjectEnd(CRiObjectEnd &obj);
	virtual RtVoid preReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params);

	virtual RtVoid doControl(CRiControl &obj, RtToken name, const CParameterList &params);
	virtual RtVoid doReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params);
	virtual RtVoid doObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle);
	virtual RtVoid doProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc);

	///////////////////////////////////

	virtual RtVoid postSynchronize(CRiSynchronize &obj, RtToken name);

	virtual RtVoid postErrorHandler(CRiErrorHandler &obj,const IErrorHandler &handler);
	virtual RtVoid postDeclare(CRiDeclare &obj,RtToken name, RtString declaration);
    // virtual RtVoid postControl(CRiControl &obj,RtToken name, const CParameterList &params);

	virtual RtVoid postSystem(CRiSystem &obj, RtString cmd);

	virtual RtVoid postBegin(CRiBegin &obj, RtString name, const CParameterList &params);
	virtual RtVoid postEnd(CRiEnd &obj);

	virtual RtVoid postFrameBegin(CRiFrameBegin &obj, RtInt number);
	virtual RtVoid postFrameEnd(CRiFrameEnd &obj);

	virtual RtVoid postWorldBegin(CRiWorldBegin &obj);
	virtual RtVoid postWorldEnd(CRiWorldEnd &obj);

	virtual RtVoid postAttributeBegin(CRiAttributeBegin &obj);
	virtual RtVoid postAttributeEnd(CRiAttributeEnd &obj);

	virtual RtVoid postTransformBegin(CRiTransformBegin &obj);
	virtual RtVoid postTransformEnd(CRiTransformEnd &obj);

    virtual RtVoid postSolidBegin(CRiSolidBegin &obj, RtToken type);
    virtual RtVoid postSolidEnd(CRiSolidEnd &obj);

	virtual RtVoid postObjectBegin(CRiObjectBegin &obj, RtString name);
	virtual RtVoid postObjectEnd(CRiObjectEnd &obj);
    virtual RtVoid postObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle);

    virtual RtVoid postMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]);
    virtual RtVoid postMotionEnd(CRiMotionEnd &obj);

	virtual RtVoid postResourceBegin(CRiResourceBegin &obj);
	virtual RtVoid postResourceEnd(CRiResourceEnd &obj);
	virtual RtVoid postResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params);

	virtual RtVoid postArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params);
	virtual RtVoid postArchiveEnd(CRiArchiveEnd &objArchiveEnd);

    virtual RtVoid postFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid postFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect);

	virtual RtVoid postScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid postCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	virtual RtVoid postProjection(CRiProjection &obj, RtToken name, const CParameterList &params);
	virtual RtVoid postClipping(CRiClipping &obj, RtFloat hither, RtFloat yon);
    virtual RtVoid postClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
    virtual RtVoid postDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
    virtual RtVoid postShutter(CRiShutter &obj, RtFloat smin, RtFloat smax);
	virtual RtVoid postPixelVariance(CRiPixelVariance &obj, RtFloat variation);
    virtual RtVoid postPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples);
    virtual RtVoid postPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
    virtual RtVoid postExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma);
    virtual RtVoid postImager(CRiImager &obj, RtString name, const CParameterList &params);
	virtual RtVoid postQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
    virtual RtVoid postDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params);
    virtual RtVoid postDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params);
    virtual RtVoid postHider(CRiHider &obj, RtToken type, const CParameterList &params);
    virtual RtVoid postColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]);
    virtual RtVoid postRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail);
    virtual RtVoid postOption(CRiOption &obj, RtToken name, const CParameterList &params);
	
    virtual RtVoid postLightSource(CRiLightSource &obj, RtString name, const CParameterList &params);
	virtual RtVoid postAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params);
	
    virtual RtVoid postAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params);
	virtual RtVoid postColor(CRiColor &obj, RtColor Cs);
	virtual RtVoid postOpacity(CRiOpacity &obj, RtColor Os);
    virtual RtVoid postSurface(CRiSurface &obj, RtString name, const CParameterList &params);
    virtual RtVoid postAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params);
    virtual RtVoid postInterior(CRiInterior &obj, RtString name, const CParameterList &params);
	virtual RtVoid postExterior(CRiExterior &obj, RtString name, const CParameterList &params);
	virtual RtVoid postIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff);
    virtual RtVoid postDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params);
	virtual RtVoid postTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
    virtual RtVoid postShadingRate(CRiShadingRate &obj, RtFloat size);
	virtual RtVoid postShadingInterpolation(CRiShadingInterpolation &obj, RtToken type);
    virtual RtVoid postMatte(CRiMatte &obj, RtBoolean onoff);
	virtual RtVoid postBound(CRiBound &obj, RtBound bound);
	virtual RtVoid postDetail(CRiDetail &obj, RtBound bound);
	virtual RtVoid postDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid postGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value);
	virtual RtVoid postGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type);
	virtual RtVoid postOrientation(CRiOrientation &obj, RtToken anOrientation);
	virtual RtVoid postReverseOrientation(CRiReverseOrientation &obj);
	virtual RtVoid postSides(CRiSides &obj, RtInt nsides);
    virtual RtVoid postBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid postTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

	virtual RtVoid postIdentity(CRiIdentity &obj);
	virtual RtVoid postTransform(CRiTransform &obj, RtMatrix aTransform);
	virtual RtVoid postConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform);
	virtual RtVoid postPerspective(CRiPerspective &obj, RtFloat fov);
	virtual RtVoid postTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid postRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid postScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid postSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
	virtual RtVoid postDeformation(CRiDeformation &obj, RtString name, const CParameterList &params);
	virtual RtVoid postScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space);
	virtual RtVoid postCoordinateSystem(CRiCoordinateSystem &obj, RtToken space);
	virtual RtVoid postCoordSysTransform(CRiCoordSysTransform &obj, RtToken space);
	// virtual RtVoid postTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

    virtual RtVoid postPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params);
	virtual RtVoid postGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params);
	virtual RtVoid postPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params);
    virtual RtVoid postPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params);

	virtual RtVoid postPatch(CRiPatch &obj, RtToken type, const CParameterList &params);
	virtual RtVoid postPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params);
    virtual RtVoid postNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params);

	virtual RtVoid postSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params);
	virtual RtVoid postHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params);

	virtual RtVoid postSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params);
    virtual RtVoid postTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params);

    virtual RtVoid postPoints(CRiPoints &obj, RtInt npts, const CParameterList &params);
    virtual RtVoid postCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params);

	virtual RtVoid postBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params);

	virtual RtVoid postProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc);

	virtual RtVoid postGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params);

	virtual RtVoid postMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params);
    virtual RtVoid postMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params);
    virtual RtVoid postMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params);

	virtual RtVoid postArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line);
	virtual RtVoid postReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params);

	virtual RtVoid postIfBegin(CRiIfBegin &obj, RtString expr);
	virtual RtVoid postElseIfBegin(CRiElseIfBegin &obj, RtString expr);
	virtual RtVoid postElseBegin(CRiElseBegin &obj);
	virtual RtVoid postIfEnd(CRiIfEnd &obj);
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
