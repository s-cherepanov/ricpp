#ifndef _RICPP_RENDERSTATE_RIMACROFACTORY_H
#define _RICPP_RENDERSTATE_RIMACROFACTORY_H


#ifndef _RICPP_RENDERSTATE_RIMACROCONTAINER_H
#include "ricpp/renderstate/rimacrocontainer.h"
#endif // _RICPP_RENDERSTATE_RIMACROCONTAINER_H

#ifndef _RICPP_RENDERSTATE_RIMACROMISC_H
#include "ricpp/renderstate/rimacromisc.h"
#endif // _RICPP_RENDERSTATE_RIMACROMISC_H

#ifndef _RICPP_RENDERSTATE_RIMACROMODES_H
#include "ricpp/renderstate/rimacromodes.h"
#endif // _RICPP_RENDERSTATE_RIMACROMODES_H

#ifndef _RICPP_RENDERSTATE_RIMACROOPTIONS_H
#include "ricpp/renderstate/rimacrooptions.h"
#endif // _RICPP_RENDERSTATE_RIMACROOPTIONS_H

#ifndef _RICPP_RENDERSTATE_RIMACROLIGHTS_H
#include "ricpp/renderstate/rimacrolights.h"
#endif // _RICPP_RENDERSTATE_RIMACROLIGHTS_H

#ifndef _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H
#include "ricpp/renderstate/rimacroattributes.h"
#endif // _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H

#ifndef _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H
#include "ricpp/renderstate/rimacrotransforms.h"
#endif // _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROPRIMS_H
#include "ricpp/renderstate/rimacroprims.h"
#endif // _RICPP_RENDERSTATE_RIMACROPRIMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROEXTERNALS_H
#include "ricpp/renderstate/rimacroexternals.h"
#endif // _RICPP_RENDERSTATE_RIMACROEXTERNALS_H

namespace RiCPP {

///////////////////////////////////////////////////////////////////////////////
/** @brief Interface, return a new instance of a IRi... class.
 *
 *  Maybe not used... Remove?
 */
class CRManInterfaceFactory {
public:
	inline CRManInterfaceFactory() {}
	inline virtual ~CRManInterfaceFactory() {}

	inline virtual CRiErrorHandler *newRiErrorHandler(
		long aLineNo,
		const IErrorHandler &handler)
	{
		return new CRiErrorHandler(aLineNo, handler);
	}

	inline virtual CRiDeclare *newRiDeclare(
		long aLineNo,
		const char *name, 
		const char *declaration)
	{
		return new CRiDeclare(aLineNo, name, declaration);	
	}

	inline virtual CRiResourceBegin *newRiResourceBegin(long aLineNo) {
		return new CRiResourceBegin(aLineNo);
	}

	inline virtual CRiResourceEnd *newRiResourceEnd(long aLineNo) {
		return new CRiResourceEnd(aLineNo);
	}

	inline virtual CRiResource *newRiResource(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken handle,
		RtToken type,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiResource(aLineNo, decl, curColorDescr, handle, type, n, tokens, params);
	}

	inline virtual CRiResource *newRiResource(
		long aLineNo,
		RtToken handle,
		RtToken type,
		const CParameterList &parameters)
	{
		return new CRiResource(aLineNo, handle, type, parameters);
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

	inline virtual CRiObjectBegin *newRiObjectBegin(long aLineNo) {
		return new CRiObjectBegin(aLineNo);
	}

	inline virtual CRiObjectEnd *newRiObjectEnd(long aLineNo) {
		return new CRiObjectEnd(aLineNo);
	}

	inline virtual CRiArchiveBegin *newRiArchiveBegin(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *name,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiArchiveBegin(aLineNo, decl, curColorDescr, name, n, tokens, params);
	}

	inline virtual CRiArchiveBegin *newRiArchiveBegin(
		long aLineNo,
		const char *name,
		const CParameterList &parameters)
	{
		return new CRiArchiveBegin(aLineNo, name, parameters);
	}

	inline virtual CRiArchiveEnd *newRiArchiveEnd(long aLineNo) {
		return new CRiArchiveEnd(aLineNo);
	}

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

	inline virtual CRiArchiveInstance *newRiArchiveInstance(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtArchiveHandle handle, const IArchiveCallback *aCallback,
		RtInt n, RtToken tokens[], RtPointer params[])
	{
		return new CRiArchiveInstance(aLineNo, decl, curColorDescr, handle, aCallback, n, tokens, params);
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

#endif // _RICPP_RENDERSTATE_RIMACROFACTORY_H