#include "ricpp/ri/ri.h"
void main(int argc, const char *argv[])
{
	RtPoint points[] = { { 0, 0, 0}, {-.5, .5, 0}, {.5, .5, 0} };
	RtFloat color[]  = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

	/* RiBegin("|aqsis -progress"); */
	/* RiBegin("|rndr -p -t"); */
	RiBegin(RI_NULL);
		RiPixelFilter(RiGaussianFilter, 3, 3),
		RiFrameBegin(1);
			RiDisplay("Polygon", RI_FRAMEBUFFER, RI_RGB, RI_NULL);
			RiTranslate(0, 0, 5);
			RiWorldBegin();
				RiSides(1);
				RiOrientation(RI_LH);
				RiPolygon(3, RI_P, points, RI_CS, color, RI_NULL);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
