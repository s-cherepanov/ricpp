#include "ricpp/ri/ri.h"
void main(int argc, const char *argv[])
{
	RiBegin(RI_NULL);
		RiPixelFilter (RiGaussianFilter, 3, 3),
		RiFrameBegin(1);
			RiWorldBegin();
				RiOrientation(RI_LH);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
