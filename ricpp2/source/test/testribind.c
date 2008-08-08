#include "ricpp/ri/ri.h"

extern void exit(int rval);

int main(int argc, const char *argv[])
{
	RtPoint points[] = { { 0, 0, 0}, {-.5, .5, 0}, {.5, .5, 0} };
	RtPoint points2[] = { { 0, 0, 0}, { 0, 1.0F, 0}, {1.33333F, -1.0F, 0}, {-1.33333F, -1.0F, 0} };
	RtFloat color[]  = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	RtFloat Cs1[] = {1, 1, 1};
	RtFloat Cs2[] = {1, 1, 0};
	RtFloat Cs3[] = {1, 0, 0};
	RtBound bound = { -.5F, 0, 0, .5F, .5F, 0 };
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

		/* Using gcc-4.0.1 I got a warning for Ri-functions that float is used instead of double due to prototype,
		 * even if I pass explictely float. Due to a message in fr.comp.lang.c "Complexe avex fonction réelle" Jan 23, 2008
		 * this warning (gcc-4.2.3) is not issued in gcc-4.3.0 anymore, so I disabled the "Prototype conversion" option temporarily.
		 */
		RiPixelFilter(RiGaussianFilter, 3.0f, 3.0f),
		RiShutter(0.0F, 1.0F);
		RiClipping(0.5F, 20.0F);
		RiProjection(RI_PERSPECTIVE, RI_NULL);
		RiFrameBegin(1);
			RiDisplay("Polygon", RI_FRAMEBUFFER, RI_RGB, RI_NULL);
			RiTranslate(0.0F, 0.0F, 4.5F);
			RiLightSource("pointlight", RI_NULL);
			RiTranslate(0.0F, 0.0F, .5F);
			RiWorldBegin();
				RiOrientation(RI_LH);
				RiSides(1);
				RiSurface("matte", RI_NULL);
				RiMotionBegin(3, 0.0F, 0.5F, 1.0F);
					RiColor(Cs1);
					RiColor(Cs2);
					RiColor(Cs3);
				RiMotionEnd();
				RiMotionBegin(3, 0.0F, 0.5F, 1.0F);
					RiRotate(10.0F, 0.0F, 0.0F, 1.0F);
					RiRotate(20.0F, 0.0F, 0.0F, 1.0F);
					RiRotate(30.0F, 0.0F, 0.0F, 1.0F);
				RiMotionEnd();
				RiDetail(bound);
				RiDetailRange(0.0F, 0.0F, 100.0F, 150.0F);
				RiPolygon(3, RI_P, points, RI_NULL);
				RiDetailRange(100.0F, 150.0F, RI_INFINITY, RI_INFINITY);
				RiPolygon(3, RI_P, points, RI_CS, color, RI_NULL);
				RiTransformPoints(RI_SCREEN, RI_RASTER, sizeof(points2)/sizeof(RtPoint), points2);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
	exit(0);
	
	return 0;
}
