#include "ricpp/ri/ri.h"
void main(int argc, const char *argv[])
{
	RiBegin(RI_NULL);
		RiFrameBegin(1);
			RiWorldBegin();
				RiOrientation(RI_LH);
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}
