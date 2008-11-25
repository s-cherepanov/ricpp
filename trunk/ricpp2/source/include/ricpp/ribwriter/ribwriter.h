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
 *
 *  @todo Move the concrete ribwriting code into the CRi classes.
 */
class CRibWriter : public CBaseRenderer, public IRequestNotification {
protected:
	typedef CBaseRenderer TypeParent;
	
private:
	CRibElementsWriter *m_writer;
	CFrontStreambuf *m_buffer;
	std::string m_cmd;
	std::string m_nativepath;

	RtToken RI_RIBWRITER;

	RtToken RI_COMPRESS;
	
	RtToken RI_QUAL_POSTPONE_PROCEDURALS;
	RtToken RI_QUAL_POSTPONE_OBJECTS;
	RtToken RI_QUAL_POSTPONE_FILE_ARCHIVES;
	RtToken RI_QUAL_POSTPONE_INLINE_ARCHIVES;
	RtToken RI_QUAL_SKIP_HEADERS;
	RtToken RI_QUAL_SKIP_VERSION;
	RtToken RI_QUAL_INDENT;
	RtToken RI_QUAL_INDENT_STRING;
	RtToken RI_QUAL_SUPPRESS_OUTPUT;
	RtToken RI_QUAL_BINARY_OUTPUT;

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

	using TypeParent::doProcess;
	using TypeParent::preProcess;
	using TypeParent::postProcess;

	virtual RtVoid preProcess(CRiArchiveEnd &obj);
	virtual RtVoid preProcess(CRiObjectEnd &obj);
	virtual RtVoid preProcess(CRiReadArchive &obj);

	///////////////////////////////////

	virtual RtVoid doProcess(CRiControl &obj);
	virtual RtVoid doProcess(CRiReadArchive &obj);
	virtual RtVoid doProcess(CRiObjectInstance &obj);
	virtual RtVoid doProcess(CRiProcedural &obj);
	inline virtual RtVoid doProcess(CRiGeometry &obj) {}

	///////////////////////////////////
	
	virtual RtVoid postProcess(CRiSynchronize &obj);
	
	virtual RtVoid postProcess(CRiErrorHandler &obj);
	virtual RtVoid postProcess(CRiDeclare &obj);
    // virtual RtVoid postProcess(CRiControl &obj); // Not used

	virtual RtVoid postProcess(CRiSystem &obj);
	
	virtual RtVoid postProcess(CRiBegin &obj);
	virtual RtVoid postProcess(CRiEnd &obj);
	
	virtual RtVoid postProcess(CRiFrameBegin &obj);
	virtual RtVoid postProcess(CRiFrameEnd &obj);
	
	virtual RtVoid postProcess(CRiWorldBegin &obj);
	virtual RtVoid postProcess(CRiWorldEnd &obj);
	
	virtual RtVoid postProcess(CRiAttributeBegin &obj);
	virtual RtVoid postProcess(CRiAttributeEnd &obj);
	
	virtual RtVoid postProcess(CRiTransformBegin &obj);
	virtual RtVoid postProcess(CRiTransformEnd &obj);
	
    virtual RtVoid postProcess(CRiSolidBegin &obj);
    virtual RtVoid postProcess(CRiSolidEnd &obj);
	
	virtual RtVoid postProcess(CRiObjectBegin &obj);
	virtual RtVoid postProcess(CRiObjectEnd &obj);
    virtual RtVoid postProcess(CRiObjectInstance &obj);
	
    virtual RtVoid postProcess(CRiMotionBegin &obj);
    virtual RtVoid postProcess(CRiMotionEnd &obj);
	
	virtual RtVoid postProcess(CRiResourceBegin &obj);
	virtual RtVoid postProcess(CRiResourceEnd &obj);
	virtual RtVoid postProcess(CRiResource &obj);
	
	virtual RtVoid postProcess(CRiArchiveBegin &obj);
	virtual RtVoid postProcess(CRiArchiveEnd &objArchiveEnd);
	
    virtual RtVoid postProcess(CRiFormat &obj);
    virtual RtVoid postProcess(CRiFrameAspectRatio &obj);
	
	virtual RtVoid postProcess(CRiScreenWindow &obj);
    virtual RtVoid postProcess(CRiCropWindow &obj);
	virtual RtVoid postProcess(CRiProjection &obj);
	virtual RtVoid postProcess(CRiClipping &obj);
    virtual RtVoid postProcess(CRiClippingPlane &obj);
    virtual RtVoid postProcess(CRiDepthOfField &obj);
    virtual RtVoid postProcess(CRiShutter &obj);
	virtual RtVoid postProcess(CRiPixelVariance &obj);
    virtual RtVoid postProcess(CRiPixelSamples &obj);
    virtual RtVoid postProcess(CRiPixelFilter &obj);
    virtual RtVoid postProcess(CRiExposure &obj);
    virtual RtVoid postProcess(CRiImager &obj);
	virtual RtVoid postProcess(CRiQuantize &obj);
    virtual RtVoid postProcess(CRiDisplayChannel &obj);
    virtual RtVoid postProcess(CRiDisplay &obj);
    virtual RtVoid postProcess(CRiHider &obj);
    virtual RtVoid postProcess(CRiColorSamples &obj);
    virtual RtVoid postProcess(CRiRelativeDetail &obj);
    virtual RtVoid postProcess(CRiOption &obj);
	
    virtual RtVoid postProcess(CRiLightSource &obj);
	virtual RtVoid postProcess(CRiAreaLightSource &obj);
	
    virtual RtVoid postProcess(CRiAttribute &obj);
	virtual RtVoid postProcess(CRiColor &obj);
	virtual RtVoid postProcess(CRiOpacity &obj);
    virtual RtVoid postProcess(CRiSurface &obj);
    virtual RtVoid postProcess(CRiAtmosphere &obj);
    virtual RtVoid postProcess(CRiInterior &obj);
	virtual RtVoid postProcess(CRiExterior &obj);
	virtual RtVoid postProcess(CRiIlluminate &obj);
    virtual RtVoid postProcess(CRiDisplacement &obj);
	virtual RtVoid postProcess(CRiTextureCoordinates &obj);
    virtual RtVoid postProcess(CRiShadingRate &obj);
	virtual RtVoid postProcess(CRiShadingInterpolation &obj);
    virtual RtVoid postProcess(CRiMatte &obj);
	virtual RtVoid postProcess(CRiBound &obj);
	virtual RtVoid postProcess(CRiDetail &obj);
	virtual RtVoid postProcess(CRiDetailRange &obj);
    virtual RtVoid postProcess(CRiGeometricApproximation &obj);
	virtual RtVoid postProcess(CRiGeometricRepresentation &obj);
	virtual RtVoid postProcess(CRiOrientation &obj);
	virtual RtVoid postProcess(CRiReverseOrientation &obj);
	virtual RtVoid postProcess(CRiSides &obj);
    virtual RtVoid postProcess(CRiBasis &obj);
    virtual RtVoid postProcess(CRiTrimCurve &obj);
	
	virtual RtVoid postProcess(CRiIdentity &obj);
	virtual RtVoid postProcess(CRiTransform &obj);
	virtual RtVoid postProcess(CRiConcatTransform &obj);
	virtual RtVoid postProcess(CRiPerspective &obj);
	virtual RtVoid postProcess(CRiTranslate &obj);
	virtual RtVoid postProcess(CRiRotate &obj);
	virtual RtVoid postProcess(CRiScale &obj);
    virtual RtVoid postProcess(CRiSkew &obj);
	virtual RtVoid postProcess(CRiDeformation &obj);
	virtual RtVoid postProcess(CRiScopedCoordinateSystem &obj);
	virtual RtVoid postProcess(CRiCoordinateSystem &obj);
	virtual RtVoid postProcess(CRiCoordSysTransform &obj);
	// virtual RtVoid postProcess(CRiTransformPoints &obj); // Not used

    virtual RtVoid postProcess(CRiPolygon &obj);
	virtual RtVoid postProcess(CRiGeneralPolygon &obj);
	virtual RtVoid postProcess(CRiPointsPolygons &obj);
    virtual RtVoid postProcess(CRiPointsGeneralPolygons &obj);
	
	virtual RtVoid postProcess(CRiPatch &obj);
	virtual RtVoid postProcess(CRiPatchMesh &obj);
    virtual RtVoid postProcess(CRiNuPatch &obj);
	
	virtual RtVoid postProcess(CRiSubdivisionMesh &obj);
	virtual RtVoid postProcess(CRiHierarchicalSubdivisionMesh &obj);
	
	virtual RtVoid postProcess(CRiSphere &obj);
    virtual RtVoid postProcess(CRiCone &obj);
    virtual RtVoid postProcess(CRiCylinder &obj);
    virtual RtVoid postProcess(CRiHyperboloid &obj);
    virtual RtVoid postProcess(CRiParaboloid &obj);
    virtual RtVoid postProcess(CRiDisk &obj);
    virtual RtVoid postProcess(CRiTorus &obj);
	
    virtual RtVoid postProcess(CRiPoints &obj);
    virtual RtVoid postProcess(CRiCurves &obj);
	
	virtual RtVoid postProcess(CRiBlobby &obj);
	
	virtual RtVoid postProcess(CRiProcedural &obj);
	
	virtual RtVoid postProcess(CRiGeometry &obj);
	
	virtual RtVoid postProcess(CRiMakeTexture &obj);
    virtual RtVoid postProcess(CRiMakeBump &obj);
    virtual RtVoid postProcess(CRiMakeLatLongEnvironment &obj);
    virtual RtVoid postProcess(CRiMakeCubeFaceEnvironment &obj);
    virtual RtVoid postProcess(CRiMakeShadow &obj);
    virtual RtVoid postProcess(CRiMakeBrickMap &obj);
	
	virtual RtVoid postProcess(CRiArchiveRecord &obj);
	virtual RtVoid postProcess(CRiReadArchive &obj);
	
	virtual RtVoid postProcess(CRiIfBegin &obj);
	virtual RtVoid postProcess(CRiElseIfBegin &obj);
	virtual RtVoid postProcess(CRiElseBegin &obj);
	virtual RtVoid postProcess(CRiIfEnd &obj);
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
