#include "ricpp/ri/ri.h"
void main(int argc, const char *argv[])
{
	RtPoint points[] = { { 0, 0, 0}, {-.5, .5, 0}, {.5, .5, 0} };
	RtFloat color[]  = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	RtFloat Cs1[] = {1.0F, 1.0F, 1.0F};
	RtFloat Cs2[] = {1.0F, 1.0F, 0.0F};
	RtFloat Cs3[] = {1.0F, 0.0F, 0.0F};

	RtInt renderer = 0;
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
		RiPixelFilter(RiGaussianFilter, 3, 3),
		RiShutter(0, 1);
		RiFrameBegin(1);
			RiDisplay("Polygon", RI_FRAMEBUFFER, RI_RGB, RI_NULL);
			RiTranslate(0, 0, 5);
			RiWorldBegin();
				RiSides(1);
				RiOrientation(RI_LH);
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
				RiPolygon(3, RI_P, points, RI_CS, color, RI_NULL);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
