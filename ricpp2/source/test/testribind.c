#include "ricpp/ri/ri.h"

extern void exit(int rval);

void main(int argc, const char *argv[])
{
	RtPoint points[] = { { 0, 0, 0}, {-.5, .5, 0}, {.5, .5, 0} };
	RtPoint points2[] = { { 0, 0, 0}, { 0, (RtFloat)1.0, 0}, {(RtFloat)1.33333, -1.0, 0}, {(RtFloat)-1.33333, -1, 0} };
	RtFloat color[]  = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	RtFloat Cs1[] = {1, 1, 1};
	RtFloat Cs2[] = {1, 1, 0};
	RtFloat Cs3[] = {1, 0, 0};
	RtBound bound = { -.5, 0, 0, .5, .5, 0 };
	RtInt renderer = 0;
	RtString myVal[2] = {"MyVal0", "MyVal1"};
	
	RiOption("MyOption", "string MyVar", &myVal[0], RI_NULL);
	RiCPPControl("MyControl", "string MyVar2", &myVal[1], RI_NULL);

	switch(renderer) {
		case 1:
			RiBegin("|aqsis -progress");
			break;
		case 2:
			RiBegin("|rndr -p");
			break;
		default:
			RiBegin(RI_NULL);
	}
		/* RiMakeTexture("mytexture.tiff", "mytexture.tx", RI_PERIODIC, RI_PERIODIC, RiSincFilter, (RtFloat)3.0, (RtFloat)3.0, RI_NULL); */
		RiPixelFilter(RiGaussianFilter, 3, 3),
		RiShutter(0, 1);
		RiClipping(0.5, 20.0);
		RiProjection(RI_PERSPECTIVE, RI_NULL);
		RiFrameBegin(1);
			RiDisplay("Polygon", RI_FRAMEBUFFER, RI_RGB, RI_NULL);
			RiTranslate(0, 0, (RtFloat)4.5);
			RiLightSource("pointlight", RI_NULL);
			RiTranslate(0, 0, (RtFloat).5);
			RiWorldBegin();
				RiOrientation(RI_LH);
				RiSides(1);
				RiSurface("matte", RI_NULL);
				RiMotionBegin(3, 0.0, 0.5, 1.0);
					RiColor(Cs1);
					RiColor(Cs2);
					RiColor(Cs3);
				RiMotionEnd();
				RiMotionBegin(3, 0.0, 0.5, 1.0);
					RiRotate(10, 0, 0, 1);
					RiRotate(20, 0, 0, 1);
					RiRotate(30, 0, 0, 1);
				RiMotionEnd();
				RiDetail(bound);
				RiDetailRange(0, 0, 100, 150);
				RiPolygon(3, RI_P, points, RI_NULL);
				RiDetailRange(100, 150, RI_INFINITY, RI_INFINITY);
				RiPolygon(3, RI_P, points, RI_CS, color, RI_NULL);
				RiTransformPoints(RI_SCREEN, RI_RASTER, sizeof(points2)/sizeof(RtPoint), points2);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
	exit(0);
}
