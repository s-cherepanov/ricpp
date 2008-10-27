#import "MyOpenGLView.h"
#import "MyNSObjectController.h"

#import "ricpp/ri/ri.h"
#import <stdio.h>

// For OpenGL Test drawAnObject() only (ri hides OpenGL)
/*
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
*/

#ifdef _DEBUG
// #define _TRACE
#endif

static const int startScreen = 10; // 0-maxScreens
static const int maxScreens = 23;

static const float defPivotDepth = 2;

static RtFloat opacity_25[] = {0.25,  0.25, 0.25};
static RtFloat opacity_50[] = {0.5,  0.5, 0.5};
static RtFloat opacity_75[] = {0.75,  0.75, 0.75};
static RtFloat greenish[] = {0.5,  1, 0.5};
static RtFloat redish[] = {1,  0.5, 0.5};
static RtFloat blueish[] = {0.5,  0.5, 1};

void testTeapot()
{
    RiAttributeBegin();
	RiColor(blueish);
	RiTranslate(0, -.2, 2.5);
	RiScale(0.3F, 0.3F, 0.3F);
	RiRotate(-135, 1, 0, 0);
	RiSides(2);
	RiGeometry(RI_TEAPOT, RI_NULL);
    RiAttributeEnd();
}

void testSimpleNuPatch()
{

    RtFloat uknot[] = {0.0F, 0.0F, 1.0F, 1.0F};
    RtFloat vknot[] = {0.0F, 0.0F, 1.0F, 1.0F};
	static RtFloat bilinearPatchLHW[] = {
		-.9F, .9F,0,1,   .9F,.9F,0,1,
		-.9F,-.9F,0,1,   .9F,-.9F,0,1
	};
	/*
	static RtFloat bilinearPatchLH[] = {
		-.9F, .9F,0,   .9F,.9F,0,
		-.9F,-.9F,0,   .9F,-.9F,0
	};
	*/
	
    RiAttributeBegin();
	RiColor(redish);
	RiSides(2);
	RiTranslate(0, 0, 2.5);
	RiNuPatch(2, 2, uknot, 0.0F, 1.0F, 2, 2, vknot, 0.0F, 1.0F, RI_PW, bilinearPatchLHW, RI_NULL);
    RiAttributeEnd();
}

void testNuPatch()
{
    static RtFloat vaseMesh[] = { // Data copied from BMRT
        0.0F, 0.0F, 0.0F, 0.869825F, 0.0F, 0.0F, 0.0F, 0.902369F, 0.0F, 0.0F, 0.0F, 1.0F,
		0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F,
		0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F,
		0.0F, 0.0F, 0.0F, 0.804738F, 0.0F, 0.0F, 0.0F, 0.869825F,
        2.1269F, 3.79357F, 0.0F, 0.869825F, 1.17851F, 4.51184F, 0.0F, 0.902369F, 0.0F, 5.0F, 0.0F, 1.0F, -3.53553F, 3.53553F, 0.0F, 0.707107F, -5.0F, 3.06152e-016F, 0.0F, 1.0F, -3.53553F, -3.53553F, 0.0F,
        0.707107F, -6.12303e-016F, -5.0F, 0.0F, 1.0F, 3.53553F, -3.53553F, 0.0F, 0.707107F, 5.0F, -9.18455e-016F, 0.0F, 1.0F, 4.02369F, 2.35702F, 0.0F, 0.804738F, 2.1269F, 3.79357F, 0.0F, 0.869825F,
        12.1872F, 21.7372F, 0.0F, 4.9841F, 6.75287F, 25.8529F, 0.0F, 5.17057F, 0.0F, 28.65F, 0.0F, 5.73F, -20.2586F, 20.2586F, 0.0F, 4.05172F, -28.65F, 1.75425e-015F, 0.0F, 5.73F, -20.2586F,
        -20.2586F, 0.0F, 4.05172F, -3.5085e-015F, -28.65F, 0.0F, 5.73F, 20.2586F, -20.2586F, 0.0F, 4.05172F, 28.65F, -5.26275e-015F, 0.0F, 5.73F, 23.0557F, 13.5057F, 0.0F, 4.61115F, 12.1872F,
        21.7372F, 0.0F, 4.9841F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 1.17851F, 4.51184F, 0.270711F, 0.902369F, 0.0F, 5.0F, 0.3F, 1.0F, -3.53553F, 3.53553F, 0.212132F, 0.707107F, -5.0F,
        3.06152e-016F, 0.3F, 1.0F, -3.53553F, -3.53553F, 0.212132F, 0.707107F, -6.12303e-016F, -5.0F, 0.3F, 1.0F, 3.53553F, -3.53553F, 0.212132F, 0.707107F, 5.0F, -9.18455e-016F, 0.3F, 1.0F,
        4.02369F, 2.35702F, 0.241421F, 0.804738F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 3.53553F, 13.5355F, 0.812132F, 2.70711F, 0.0F,
        15.0F, 0.9F, 3.0F, -10.6066F, 10.6066F, 0.636396F, 2.12132F, -15.0F, 9.18455e-016F, 0.9F, 3.0F, -10.6066F, -10.6066F, 0.636396F, 2.12132F, -1.83691e-015F, -15.0F, 0.9F, 3.0F, 10.6066F,
        -10.6066F, 0.636396F, 2.12132F, 15.0F, -2.75536e-015F, 0.9F, 3.0F, 12.0711F, 7.07107F, 0.724264F, 2.41421F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 2.72244F, 4.85577F,
        2.0006F, 0.869825F, 1.50849F, 5.77516F, 2.07545F, 0.902369F, 0.0F, 6.4F, 2.3F, 1.0F, -4.52548F, 4.52548F, 1.62635F, 0.707107F, -6.4F, 3.91874e-016F, 2.3F, 1.0F, -4.52548F, -4.52548F,
        1.62635F, 0.707107F, -7.83748e-016F, -6.4F, 2.3F, 1.0F, 4.52548F, -4.52548F, 1.62635F, 0.707107F, 6.4F, -1.17562e-015F, 2.3F, 1.0F, 5.15032F, 3.01699F, 1.8509F, 0.804738F,
        2.72244F, 4.85577F, 2.0006F, 0.869825F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 1.64992F, 6.31658F, 4.51184F, 0.902369F, 0.0F, 7.0F, 5.0F, 1.0F, -4.94975F, 4.94975F, 3.53553F,
        0.707107F, -7.0F, 4.28612e-016F, 5.0F, 1.0F, -4.94975F, -4.94975F, 3.53553F, 0.707107F, -8.57224e-016F, -7.0F, 5.0F, 1.0F, 4.94975F, -4.94975F, 3.53553F, 0.707107F, 7.0F, -1.28584e-015F, 5.0F,
        1.0F, 5.63316F, 3.29983F, 4.02369F, 0.804738F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.41421F, 5.41421F, 7.21895F, 0.902369F, 0.0F, 6.0F, 8.0F,
        1.0F, -4.24264F, 4.24264F, 5.65685F, 0.707107F, -6.0F, 3.67382e-016F, 8.0F, 1.0F, -4.24264F, -4.24264F, 5.65685F, 0.707107F, -7.34764e-016F, -6.0F, 8.0F, 1.0F, 4.24264F, -4.24264F, 5.65685F,
        0.707107F, 6.0F, -1.10215e-015F, 8.0F, 1.0F, 4.82843F, 2.82843F, 6.4379F, 0.804738F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.27614F, 2.27614F, 8.69825F, 0.869825F, 0.707107F,
        2.70711F, 9.02369F, 0.902369F, 0.0F, 3.0F, 10.0F, 1.0F, -2.12132F, 2.12132F, 7.07107F, 0.707107F, -3.0F, 1.83691e-016F, 10.0F, 1.0F, -2.12132F, -2.12132F, 7.07107F, 0.707107F,
        -3.67382e-016F, -3.0F, 10.0F, 1.0F, 2.12132F, -2.12132F, 7.07107F, 0.707107F, 3.0F, -5.51073e-016F, 10.0F, 1.0F, 2.41421F, 1.41421F, 8.04738F, 0.804738F, 1.27614F, 2.27614F, 8.69825F,
        0.869825F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 0.707107F, 2.70711F, 10.8284F, 0.902369F, 0.0F, 3.0F, 12.0F, 1.0F, -2.12132F, 2.12132F, 8.48528F, 0.707107F, -3.0F, 1.83691e-016F,
        12.0F, 1.0F, -2.12132F, -2.12132F, 8.48528F, 0.707107F, -3.67382e-016F, -3.0F, 12.0F, 1.0F, 2.12132F, -2.12132F, 8.48528F, 0.707107F, 3.0F, -5.51073e-016F, 12.0F, 1.0F, 2.41421F, 1.41421F,
        9.65685F, 0.804738F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 0.801388F, 3.06805F, 10.8284F, 0.902369F, 0.0F, 3.4F, 12.0F, 1.0F, -2.40416F,
        2.40416F, 8.48528F, 0.707107F, -3.4F, 2.08183e-016F, 12.0F, 1.0F, -2.40416F, -2.40416F, 8.48528F, 0.707107F, -4.16366e-016F, -3.4F, 12.0F, 1.0F, 2.40416F, -2.40416F, 8.48528F,
        0.707107F, 3.4F, -6.24549e-016F, 12.0F, 1.0F, 2.73611F, 1.60278F, 9.65685F, 0.804738F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 11.3077F, 0.869825F,
        0.801388F, 3.06805F, 11.7308F, 0.902369F, 0.0F, 3.4F, 13.0F, 1.0F, -2.40416F, 2.40416F, 9.19239F, 0.707107F, -3.4F, 2.08183e-016F, 13.0F, 1.0F, -2.40416F, -2.40416F, 9.19239F, 0.707107F,
        -4.16366e-016F, -3.4F, 13.0F, 1.0F, 2.40416F, -2.40416F, 9.19239F, 0.707107F, 3.4F, -6.24549e-016F, 13.0F, 1.0F, 2.73611F, 1.60278F, 10.4616F, 0.804738F, 1.44629F, 2.57963F,
        11.3077F, 0.869825F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 0.659966F, 2.52663F, 11.7308F, 0.902369F, 0.0F, 2.8F, 13.0F, 1.0F, -1.9799F, 1.9799F, 9.19239F, 0.707107F, -2.8F,
        1.71445e-016F, 13.0F, 1.0F, -1.9799F, -1.9799F, 9.19239F, 0.707107F, -3.4289e-016F, -2.8F, 13.0F, 1.0F, 1.9799F, -1.9799F, 9.19239F, 0.707107F, 2.8F, -5.14335e-016F, 13.0F, 1.0F, 2.25327F,
        1.31993F, 10.4616F, 0.804738F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 0.659966F, 2.52663F, 10.8284F, 0.902369F, 0.0F, 2.8F, 12.0F, 1.0F,
        -1.9799F, 1.9799F, 8.48528F, 0.707107F, -2.8F, 1.71445e-016F, 12.0F, 1.0F, -1.9799F, -1.9799F, 8.48528F, 0.707107F, -3.4289e-016F, -2.8F, 12.0F, 1.0F, 1.9799F, -1.9799F, 8.48528F,
        0.707107F, 2.8F, -5.14335e-016F, 12.0F, 1.0F, 2.25327F, 1.31993F, 9.65685F, 0.804738F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 1.14853F, 2.04853F, 8.78523F, 0.869825F,
        0.636396F, 2.4364F, 9.11393F, 0.902369F, 0.0F, 2.7F, 10.1F, 1.0F, -1.90919F, 1.90919F, 7.14178F, 0.707107F, -2.7F, 1.65322e-016F, 10.1F, 1.0F, -1.90919F, -1.90919F, 7.14178F,
        0.707107F, -3.30644e-016F, -2.7F, 10.1F, 1.0F, 1.90919F, -1.90919F, 7.14178F, 0.707107F, 2.7F, -4.95966e-016F, 10.1F, 1.0F, 2.17279F, 1.27279F, 8.12785F, 0.804738F, 1.14853F,
        2.04853F, 8.78523F, 0.869825F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 1.31993F, 5.05327F, 7.21895F, 0.902369F, 0.0F, 5.6F, 8.0F, 1.0F, -3.9598F, 3.9598F, 5.65685F, 0.707107F, -5.6F,
        3.4289e-016F, 8.0F, 1.0F, -3.9598F, -3.9598F, 5.65685F, 0.707107F, -6.8578e-016F, -5.6F, 8.0F, 1.0F, 3.9598F, -3.9598F, 5.65685F, 0.707107F, 5.6F, -1.02867e-015F, 8.0F, 1.0F, 4.50653F,
        2.63987F, 6.4379F, 0.804738F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 1.55563F, 5.95564F, 4.51184F, 0.902369F, 0.0F, 6.6F, 5.0F, 1.0F,
        -4.6669F, 4.6669F, 3.53553F, 0.707107F, -6.6F, 4.0412e-016F, 5.0F, 1.0F, -4.6669F, -4.6669F, 3.53553F, 0.707107F, -8.0824e-016F, -6.6F, 5.0F, 1.0F, 4.6669F, -4.6669F, 3.53553F,
        0.707107F, 6.6F, -1.21236e-015F, 5.0F, 1.0F, 5.31127F, 3.11127F, 4.02369F, 0.804738F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 2.59482F, 4.62816F, 2.43551F, 0.869825F,
        1.43778F, 5.50445F, 2.52663F, 0.902369F, 0.0F, 6.1F, 2.8F, 1.0F, -4.31335F, 4.31335F, 1.9799F, 0.707107F, -6.1F, 3.73505e-016F, 2.8F, 1.0F, -4.31335F, -4.31335F, 1.9799F, 0.707107F,
        -7.4701e-016F, -6.1F, 2.8F, 1.0F, 4.31335F, -4.31335F, 1.9799F, 0.707107F, 6.1F, -1.12051e-015F, 2.8F, 1.0F, 4.9089F, 2.87557F, 2.25327F, 0.804738F, 2.59482F, 4.62816F, 2.43551F,
        0.869825F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 1.1078F, 4.24113F, 0.360948F, 0.902369F, 0.0F, 4.7F, 0.4F, 1.0F, -3.3234F, 3.3234F, 0.282843F, 0.707107F, -4.7F,
        2.87782e-016F, 0.4F, 1.0F, -3.3234F, -3.3234F, 0.282843F, 0.707107F, -5.75565e-016F, -4.7F, 0.4F, 1.0F, 3.3234F, -3.3234F, 0.282843F, 0.707107F, 4.7F, -8.63347e-016F, 0.4F, 1.0F,
        3.78227F, 2.2156F, 0.321895F, 0.804738F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 0.0F, 0.0F, 0.173965F, 0.869825F, 0.0F, 0.0F, 0.180474F, 0.902369F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F,
        0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.160948F, 0.804738F, 0.0F, 0.0F, 0.173965F, 0.869825F
    };
	
    RtFloat uknot[] = {0.0F, 0.0F, 0.0F, 0.0833333F, 0.0833333F, 0.333333F, 0.333333F, 0.583333F, 0.583333F, 0.833333F, 0.833333F, 1.0F, 1.0F, 1.0F};
    RtFloat vknot[] = {0.0F, 0.0F, 0.0F, 0.0555556F, 0.111111F, 0.166667F, 0.222222F, 0.277778F, 0.333333F, 0.388889F, 0.444444F, 0.5F, 0.555556F, 0.611111F, 0.666667F, 0.722222F, 0.777778F, 0.833333F, 0.888889F, 0.944444F, 1.0F, 1.0F, 1.0F};
	
    RiAttributeBegin();
	RiColor(redish);
	RiSides(1);
	RiTranslate(0, -.3, 2);
	RiScale(.1F, .1F, .1F);
	RiRotate(-135, 1, 0, 0);
	RiNuPatch(11, 3, uknot, 0.0F, 1.0F, 20, 3, vknot, 0.0F, 1.0F, RI_PW, vaseMesh, RI_NULL);
    RiAttributeEnd();
}


void testBicubicPatch()
{
	static RtFloat bicubicPatchLH[] = {
		  0,   0,0,   .3F,   0,0,       .6F,   0,0,      .9F,   0,0,
		  0,-.3F,0,   .3F,-.3F,-.6F,    .6F,-.3F,-.6F,   .9F,-.3F,0,
		  0,-.6F,0,   .3F,-.6F,-.6F,    .6F,-.6F,-.6F,   .9F,-.6F,0,
		  0,-.9F,0,   .3F,-.9F,0,       .6F,-.9F,0,      .9F,-.9F,0
	};

	static RtFloat bilinearPatchLH[] = {
		0,   0,0,   .9F,   0,0,
		0,-.9F,0,   .9F,-.9F,0
	};
	
	static RtFloat bicubicPatchRH[] = {
		0,-.9F,0,   .3F,-.9F,0,       .6F,-.9F,0,      .9F,-.9F,0,
		0,-.6F,0,   .3F,-.6F,-.6F,    .6F,-.6F,-.6F,   .9F,-.6F,0,
		0,-.3F,0,   .3F,-.3F,-.6F,    .6F,-.3F,-.6F,   .9F,-.3F,0,
		0,   0,0,   .3F,   0,0,       .6F,   0,0,      .9F,   0,0
	};

	static RtFloat bilinearPatchRH[] = {
		0,-.9F,0,   .9F,-.9F,0,
		0,   0,0,   .9F,   0,0
	};

	RiAttributeBegin();

	RiTranslate(0, 0, 2);
	RiRotate(45, 1, 0, 0);
	RiColor(blueish);
	RiSides(1);
	RiBasis(RiBezierBasis, RI_BEZIERSTEP, RiBezierBasis, RI_BEZIERSTEP);

	RiOrientation(RI_LH);
	
	RiTransformBegin();
	RiTranslate(-.95F,.95F,0);
	RiPatch(RI_BICUBIC, RI_P, bicubicPatchLH, RI_NULL);
	RiTransformEnd();

	RiTransformBegin();
	RiTranslate(.05F,.95F,0);
	RiPatch(RI_BILINEAR, RI_P, bilinearPatchLH, RI_NULL);
	RiTransformEnd();

	RiOrientation(RI_RH);
	
	RiTransformBegin();
	RiTranslate(.05F,-.05F,0);
	RiPatch(RI_BICUBIC, RI_P, bicubicPatchRH, RI_NULL);
	RiTransformEnd();

	RiTransformBegin();
	RiTranslate(-.95F,-.05F,0);
	RiPatch(RI_BILINEAR, RI_P, bilinearPatchRH, RI_NULL);
	RiTransformEnd();

	RiAttributeEnd();
}

void testBicubicMesh()
{
	static RtFloat pinMesh[] = { // Pin data from RanderMan companion
		0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
		0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
		0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F, 0.0F, 0.0F, 1.5F,
		0.0703F, 0.0F, 1.5F, 0.0703F, 0.038826F, 1.5F, 0.038826F, 0.0703F, 1.5F, 0.0F, 0.0703F, 1.5F,
		-0.038826F, 0.0703F, 1.5F, -0.0703F, 0.038826F, 1.5F, -0.0703F, 0.0F, 1.5F, -0.0703F, -0.038826F, 1.5F,
		-0.038826F, -0.0703F, 1.5F, 0.0F, -0.0703F, 1.5F, 0.038826F, -0.0703F, 1.5F, 0.0703F, -0.038826F, 1.5F,
		0.1273F, 0.0F, 1.4293F, 0.1273F, 0.070306F, 1.4293F, 0.070306F, 0.1273F, 1.4293F, 0.0F, 0.1273F, 1.4293F,
		-0.070306F, 0.1273F, 1.4293F, -0.1273F, 0.070306F, 1.4293F, -0.1273F, 0.0F, 1.4293F, -0.1273F, -0.070306F, 1.4293F,
		-0.070306F, -0.1273F, 1.4293F, 0.0F, -0.1273F, 1.4293F, 0.070306F, -0.1273F, 1.4293F, 0.1273F, -0.070306F, 1.4293F,
		0.1273F, 0.0F, 1.3727F, 0.1273F, 0.070306F, 1.3727F, 0.070306F, 0.1273F, 1.3727F, 0.0F, 0.1273F, 1.3727F,
		-0.070306F, 0.1273F, 1.3727F, -0.1273F, 0.070306F, 1.3727F, -0.1273F, 0.0F, 1.3727F, -0.1273F, -0.070306F, 1.3727F,
		-0.070306F, -0.1273F, 1.3727F, 0.0F, -0.1273F, 1.3727F, 0.070306F, -0.1273F, 1.3727F, 0.1273F, -0.070306F, 1.3727F,
		0.1273F, 0.0F, 1.23F, 0.1273F, 0.070306F, 1.23F, 0.070306F, 0.1273F, 1.23F, 0.0F, 0.1273F, 1.23F,
		-0.070306F, 0.1273F, 1.23F, -0.1273F, 0.070306F, 1.23F, -0.1273F, 0.0F, 1.23F, -0.1273F, -0.070306F, 1.23F,
		-0.070306F, -0.1273F, 1.23F, 0.0F, -0.1273F, 1.23F, 0.070306F, -0.1273F, 1.23F, 0.1273F, -0.070306F, 1.23F,
		0.0899F, 0.0F, 1.16F, 0.0899F, 0.04965F, 1.16F, 0.04965F, 0.0899F, 1.16F, 0.0F, 0.0899F, 1.16F,
		-0.04965F, 0.0899F, 1.16F, -0.0899F, 0.04965F, 1.16F, -0.0899F, 0.0F, 1.16F, -0.0899F, -0.04965F, 1.16F,
		-0.04965F, -0.0899F, 1.16F, 0.0F, -0.0899F, 1.16F, 0.04965F, -0.0899F, 1.16F, 0.0899F, -0.04965F, 1.16F,
		0.0899F, 0.0F, 1.0F, 0.0899F, 0.04965F, 1.0F, 0.04965F, 0.0899F, 1.0F, 0.0F, 0.0899F, 1.0F,
		-0.04965F, 0.0899F, 1.0F, -0.0899F, 0.04965F, 1.0F, -0.0899F, 0.0F, 1.0F, -0.0899F, -0.04965F, 1.0F,
		-0.04965F, -0.0899F, 1.0F, 0.0F, -0.0899F, 1.0F, 0.04965F, -0.0899F, 1.0F, 0.0899F, -0.04965F, 1.0F,
		0.0899F, 0.0F, 0.75F, 0.0899F, 0.04965F, 0.75F, 0.04965F, 0.0899F, 0.75F, 0.0F, 0.0899F, 0.75F,
		-0.04965F, 0.0899F, 0.75F, -0.0899F, 0.04965F, 0.75F, -0.0899F, 0.0F, 0.75F, -0.0899F, -0.04965F, 0.75F,
		-0.04965F, -0.0899F, 0.75F, 0.0F, -0.0899F, 0.75F, 0.04965F, -0.0899F, 0.75F, 0.0899F, -0.04965F, 0.75F,
		0.41F, 0.0F, 0.678F, 0.41F, 0.226437F, 0.678F, 0.226437F, 0.41F, 0.678F, 0.0F, 0.41F, 0.678F,
		-0.226437F, 0.41F, 0.678F, -0.41F, 0.226437F, 0.678F, -0.41F, 0.0F, 0.678F, -0.41F, -0.226437F, 0.678F,
		-0.226437F, -0.41F, 0.678F, 0.0F, -0.41F, 0.678F, 0.226437F, -0.41F, 0.678F, 0.41F, -0.226437F, 0.678F,
		0.125F, 0.0F, 0.0F, 0.125F, 0.069036F, 0.0F, 0.069036F, 0.125F, 0.0F, 0.0F, 0.125F, 0.0F,
		-0.069036F, 0.125F, 0.0F, -0.125F, 0.069036F, 0.0F, -0.125F, 0.0F, 0.0F, -0.125F, -0.069036F, 0.0F,
		-0.069036F, -0.125F, 0.0F, 0.0F, -0.125F, 0.0F, 0.069036F, -0.125F, 0.0F, 0.125F, -0.069036F, 0.0F
	};


	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_RH);
	//	RiOpacity(opacity_75);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, -.3, 2);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(-30, 1, 0, 0);
	// RiRotate(45, 0, 0, 1);
	RiBasis(RiBezierBasis, RI_BEZIERSTEP, RiBezierBasis, RI_BEZIERSTEP);
	RiPatchMesh(RI_BICUBIC, 12, RI_PERIODIC, 10, RI_NONPERIODIC, RI_P, pinMesh, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testBilinPatchMesh()
{
    static RtFloat patchMesh [5*5*3] = {
        -1.0F,  1.0F, 0.0F,  -0.5F,  1.0F, 0.0F,   0.0F,  1.0F, 0.8F,  0.5F,  1.0F, 0.0F,  1.0F,  1.0F, 0.0F,
        -1.0F,  0.5F, 0.0F,  -0.5F,  0.5F, 0.1F,   0.0F,  0.5F, 0.4F,  0.5F,  0.5F, 0.1F,  1.0F,  0.5F, 0.0F,
        -1.0F,  0.0F, 0.0F,  -0.5F,  0.0F, 0.2F,   0.0F,  0.0F, 0.2F,  0.5F,  0.0F, 0.2F,  1.0F,  0.0F, 0.0F,
        -1.0F, -0.5F, 0.0F,  -0.5F, -0.5F, 0.4F,   0.0F, -0.5F, 0.1F,  0.5F, -0.5F, 0.4F,  1.0F, -0.5F, 0.0F,
        -1.0F, -1.0F, 0.0F,  -0.5F, -1.0F, 0.8F,   0.0F, -1.0F, 0.0F,  0.5F, -1.0F, 0.8F,  1.0F, -1.0F, 0.0F
    };
	
	static RtFloat colors [3*16] = {
		0.0F, 0.0F, 1.0F,
		0.0F, 0.2F, 0.8F,
		0.0F, 0.4F, 0.4F,
		0.0F, 0.8F, 0.2F,
		
		0.0F, 1.0F, 0.0F,
		0.2F, 0.8F, 0.0F,
		0.4F, 0.4F, 0.0F,
		0.8F, 0.2F, 0.0F,
		
		1.0F, 0.0F, 0.0F,
		0.8F, 0.0F, 0.2F,
		0.4F, 0.0F, 0.4F,
		0.2F, 0.0F, 0.8F,
		
		1.0F, 1.0F, 1.0F,
		0.8F, 0.8F, 0.8F,
		0.4F, 0.4F, 0.4F,
		0.2F, 0.2F, 0.2F
	};

	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	//	RiOpacity(opacity_75);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(30, 0, 1, 0);
	// RiRotate(45, 0, 0, 1);
	RiPatchMesh(RI_BILINEAR, 5, RI_NONPERIODIC, 5, RI_NONPERIODIC, RI_P, patchMesh, "uniform color Cs", colors, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testBilinPatch()
{
	RtFloat pos[] = {
		-0.65,  0.65, .1,
		 0.65,  0.65, -.3,
		-0.65, -0.65, -.1,
		 0.65, -0.65, .3
	};
	
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
//	RiOpacity(opacity_75);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(30, 1, 0, 0);
	// RiRotate(45, 0, 0, 1);
	RiPatch(RI_BILINEAR, RI_P, pos, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testCone()
{
	const RtFloat rad = 0.6;
	const RtFloat height = 1.6;
	RtFloat colors[] = {
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	// RiOpacity(opacity_25);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, -height/4.0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(-45, 1, 0, 0);
	RiCone(height, rad, 360, RI_CS, colors, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testCylinder()
{
	const RtFloat rad = 0.6;
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	RiOpacity(opacity_75);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(35, 1, 0, 0);
	RiCylinder(rad, -rad, rad, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testDisk()
{
	const RtFloat rad = 0.8;
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	// RiOpacity(opacity_25);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(-115, 1, 0, 0);
	RiDisk(0, rad, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testHyperboloid()
{
	RtPoint point1 = {-0.5, 0.5, -0.6};
	RtPoint point2 = { 0.5, 0.5,  0.6};
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	// RiOpacity(opacity_75);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(-115, 1, 0, 0);
	RiHyperboloid(point1, point2, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testParaboloid()
{
	const RtFloat rad = 0.8;
	RiAttributeBegin();
	RiSides(2);
	// RiOpacity(opacity_25);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 1.1);
	// RiOrientation(RI_OUTSIDE);
	// RiTranslate(0, rad, 1.0+rad);
	// RiTranslate(0, -rad, 1.0+rad);
	// RiTranslate(0, 0, 1);
	// RiRotate(180, 0, 1, 0);
	// RiRotate(-90, 1, 0, 0);
	RiRotate(45, 0, 1, 0);
	RiTranslate(-1.0, 0, 0);
	RiScale(1.0, 1.0, 1.0);
	RiParaboloid(rad, 0, rad*2.0, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testSphere()
{
	const RtFloat rad = 0.8;
	RiAttributeBegin();
	RiSides(1);
	RiOrientation(RI_OUTSIDE);
	// RiOpacity(opacity_25);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 2.1);
	RiRotate(45, 0, 1, 0);
	RiScale(1.0, -1.0, 1.0);
	RiSphere(rad, -rad, rad, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testTorus()
{
	const RtFloat majorrad = 0.4;
	const RtFloat minorrad = 0.2;
	const RtFloat phimin = 0;
	const RtFloat phimax = 360;

	RiAttributeBegin();
	RiSides(1);
	RiOrientation(RI_OUTSIDE);
	// RiOpacity(opacity_25);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, 0, 1.1);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(45, 1, 0, 0);
	// RiRotate(-65, 0, 0, 1);
	RiTorus(majorrad, minorrad, phimin, phimax, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testPoly10()
{
	RtFloat p[] = {
		// outer
		-.5, -.5, 1, //  1
		-.5,  .5, 1, //  2
		.5,  .5, 1, //  3
		.5, -.5, 1, //  4
		0
	};
	RtInt nloops[] = {
		1
	};
	RtInt nverts[] = {
		4
	};
	RtInt verts[] = {
		0, 1, 2, 3
	};

	// Disable warnings (variable not used)
	opacity_25[0] = opacity_25[0];
	opacity_50[0] = opacity_50[0];
	opacity_75[0] = opacity_75[0];

	RiAttributeBegin();
	RiSides(2);
	
	RiAttributeBegin();
	RiRotate(15.0, 0, 0, 1);
	RiOpacity(opacity_50);
	RiColor(greenish);

	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
	RiPolygon(4, RI_P, &p, RI_NULL);
	// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
	// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
	
#   ifdef _TRACE
	{
		RtInt i;
		RiTransformPoints(RI_CURRENT, RI_RASTER, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
		for ( i = 0; i < 4; ++i ) {
			printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
		}
		printf("Inverse\n");
		RiTransformPoints(RI_RASTER, RI_CURRENT, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
		for ( i = 0; i < 4; ++i ) {
			printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
		}
	}
#   endif
	RiAttributeEnd();
	
	RiAttributeBegin();
	RiTranslate(0, 0, 0.2);
	// RiOpacity(opacity_50);
	RiColor(redish);

	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
	// RiPolygon(4, RI_P, &p, RI_NULL);
	RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
	// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
	
	RiAttributeEnd();

	RiAttributeBegin();
	RiRotate(-15.0, 0, 0, 1);
	RiTranslate(0, 0, 0.3);
	RiOpacity(opacity_75);
	RiColor(blueish);

	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
	// RiPolygon(4, RI_P, &p, RI_NULL);
	// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
	RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
	
	RiAttributeEnd();

	RiAttributeEnd();
}

void testPoly9()
{
	RtFloat p[] = {
		// outer
		0, 0, 1, //  1
		4, 0, 1, //  2
		4, 4, 1, //  3
		0, 4, 1, //  4
		
		//  5
		//  6
		2, 0, 1, //  7
		4, 0, 1, //  8
		4, 4, 1, //  9
		2, 4, 1, // 10
		
		// 11
		// 12
		
		1, 1, 1, // 13
		2, 1, 1, // 14
		2, 3, 1, // 15
		1, 3, 1, // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiAttributeBegin();
	RiOrientation(RI_RH);
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
	RiAttributeEnd();
}

void testPoly8()
{
	// orientation is rh on purpose
	RtFloat p[] = {
		// outer
		0,   1, 1, //  1
		2,   4, 1, //  2
		4,   1, 1, //  3
		2.5, 1, 1, //  4
		3,   0, 1, //  5
		1,   0, 1, //  6
		1.5, 1, 1, //  7
		2.5, 1, 1, //  8
		2,   2, 1, //  9
		1.5, 1, 1, // 10
		
		0
	};
	
	RtInt nverts[] = {
		10
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly7()
{
	RtFloat p[] = {
		// outer
		0,   0,   1, //  1
		
		0,   2,   1, //  2
		1,   1.5, 1, //  3
		1,   2.5, 1, //  4
		0,   2,   1, //  5
		
		0,   5,   1, //  6
		3,   5,   1, //  7
		
		3,   3,   1, //  8
		2,   3.5, 1, //  9
		2,   2.5, 1, // 10
		3,   3,   1, // 11
		
		3,   2,   1, // 12
		2,   2,   1, // 13
		2,   1,   1, // 14
		3,   1,   1, // 15
		3,   2,   1, // 16
		
		5,   2,   1, // 17
		4.5, 1,   1, // 18
		5.5, 1,   1, // 19
		5,   2,   1, // 20
		
		7,   2,   1, // 21
		7,   0,   1, // 22
		
		6,   0,   1, // 23
		6.5, 0.5, 1, // 24
		5.5, 0.5, 1, // 25
		6,   0,   1, // 26
		
		0
	};
	
	RtInt nverts[] = {
		26
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly6()
{
	RtFloat p[] = {
		// outer
		0, 0, 1, //  1
		0, 5, 1, //  2
		3, 5, 1, //  3
		3, 2, 1, //  4
		7, 2, 1, //  5
		7, 0, 1, //  6
		
		//  7
		//  8
		
		2, 2.5, 1, //  9
		2, 3.5, 1, // 10
		3, 3,   1, // 11
		
		// 12
		// 13
		
		2, 1, 1, // 14
		2, 2, 1, // 15
		3, 2, 1, // 16
		3, 1, 1, // 17
		
		// 18
		// 19
		
		4.5, 1, 1, // 20
		5.5, 1, 1, // 21
		5,   2, 1, // 22
		
		0
	};
	
	RtInt nverts[] = {
		6,
		3,
		4,
		3
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly5()
{
	RtFloat p[] = {
		// outer
		-.5, -.5, 1, //  1
		-.5,   0, 1, //  2
		-.5,  .5, 1, //  3
		 .5,  .5, 1, //  4
		 .5, -.5, 1, //  5
		 0
	};
	
	RtInt nverts[] = {
		5
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0];
			p[n+i*3+1] = p[n+i*3+1];
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly4()
{
	RtFloat p[] = {
		// outer
		0, 0, 1, //  1
		0, 6, 1, //  2
		6, 6, 1, //  3
		6, 0, 1, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 1,  //  7
		3, 1, 1,  //  8
		3, 3, 1,  //  9
		1, 3, 1,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		3, 3, 1,  // 13
		5, 3, 1,  // 14
		5, 5, 1,  // 15
		3, 5, 1,  // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly3()
{
	RtFloat p[] = {
		// outer
		0, 0, 1, //  1
		0, 6, 1, //  2
		7, 6, 1, //  3
		7, 0, 1, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 1,  //  7
		3, 1, 1,  //  8
		3, 3, 1,  //  9
		1, 3, 1,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		4, 3, 1,  // 13
		6, 3, 1,  // 14
		6, 5, 1,  // 15
		4, 5, 1,  // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly2()
{
	RtFloat p[] = {
		// outer
		0, 0, 1, //  1
		0, 7, 1, //  2
		7, 7, 1, //  3
		7, 0, 1, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 1,  //  7
		3, 1, 1,  //  8
		3, 3, 1,  //  9
		1, 3, 1,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		4, 1, 1,  // 13
		6, 1, 1,  // 14
		6, 3, 1,  // 15
		4, 3, 1,  // 16
		
		//  17
		//  18
		
		// inner (hole)
		4, 4, 1,  // 19
		6, 4, 1,  // 20
		6, 6, 1,  // 21
		4, 6, 1,  // 22
		
		//  23
		//  24
		
		// inner (hole)
		1, 4, 1,  //  7
		3, 4, 1,  //  8
		3, 6, 1,  //  9
		1, 6, 1,  // 10
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly1()
{
	RtFloat p[] = {
		// outer
		1, 1, 1, // 0
		1, 8, 1, // 1
		4, 8, 1, // 2
		5, 6, 1, // 3
		6, 8, 1, // 4
		7, 8, 1, // 5
		
		// inner (hole)
		2, 5,   1,  // 6
		2, 6,   1,  // 7
		3, 5.5, 1,  // 8
		
		// inner (hole)
		2, 3,   1,  //  9
		2, 4,   1,  // 10
		3, 3.5, 1,  // 11
		
		0
	};
	
	RtInt nverts[] = {
		6,
		3,
		3
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly()
{
	// O'Rourke's poly 1.1
	// orientation is rh on purpose
	RtFloat p[] = {
		// outer
		0,   0, 1, //  0
		10,  7, 1, //  1
		12,  3, 1, //  2
		20,  8, 1, //  3
		13, 17, 1, //  4
		10, 12, 1, //  5
		12, 14, 1, //  6
		14,  9, 1, //  7
		8,  10, 1, //  8
		6,  14, 1, //  9
		10, 15, 1, // 10
		7,  18, 1, // 11
		0,  16, 1, // 12
		1,  13, 1, // 13
		3,  15, 1, // 14
		5,   8, 1, // 15
		-2,  9, 1, // 16
		5,   5, 1  // 17
	};
	
	RtInt nverts[] = {
		18
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[0]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 18.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 18.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2];
		}
		n += nverts[j]*3;
	}
	
	RiReverseOrientation();
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
	RiReverseOrientation();
}

/*
static void drawAnObject ()
{
	
	// Opaque
	glColor4f(0.0f, 0.35f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.1, 0.5, -0.15);
		glVertex3f( -0.1, -0.3, -0.15);
		glVertex3f( 0.3, -0.3 , 0.3);
	}
	glEnd();
	
	// Transparent from far to near
	
	glColor4f(1.0f, 0.85f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(  0, 0.5, -0.1);
		glVertex3f( -0.5, -0.3, -0.1);
		glVertex3f( 0.5, -0.3 , 0.1);
	}
	glEnd();
	
	glColor4f(0.0f, 0.0f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.3, 0.5, -0.3);
		glVertex3f( 0.1, -0.3, -0.3);
		glVertex3f( 0.5, -0.3 , 0.15);
	}
	glEnd();
}
*/

@implementation MyOpenGLView
-(id) init
{
	/* NSLog(@"init"); */
	counter = startScreen;
	[self resetCamera];
	return self;
}

-(id) resetCamera
{
	/* NSLog(@"resetCamera"); */
	sphi = stheta = 0;
	sdepth = 0;
	pivotDepth = defPivotDepth;
	return self;
}

-(void) awakeFromNib
{
	/* NSLog(@"awakeFromNib"); */
	counter = startScreen;
	[self resetCamera];
}

-(void) drawRect: (NSRect) bounds
{
	/* NSLog(@"drawRect"); */
	char *matrixName[] = {"pre-camera"};
	RiIdentity();
	
	RiTranslate(0.0F,0.0F,sdepth); // Move back and forth
	RiTranslate(0, 0, pivotDepth); // Move back to previous pos
	
	// Rotation at Pivot (here)
	RiRotate(-stheta, 1.0, 0.0, 0.0); // Rotate x
	RiRotate(-sphi, 0.0, 1.0, 0.0); // Rotate y
	
	RiTranslate(0, 0, -pivotDepth); // Move to a pivot
	RiCPPControl("state", "string store-transform", matrixName, RI_NULL); // Candidate for RiResource
	RiIdentity();
	
	char *screenAction[2] = {"clear", "finish"};
#ifdef _DEBUG
	RtInt noYes[2] = {0, 1};
#endif
	
	if ( counter < 0 ) 
		counter = maxScreens;
	else if ( counter > maxScreens )
		counter = 0;
	
	RtInt width = (RtInt)bounds.size.width;
	RtInt height = (RtInt)bounds.size.height;
	RtFloat pixelAspect = (RtFloat)1.0;
	
	RtInt origin[2] = {0, 0};
	
#ifdef _DEBUG
	RiAttribute("glrenderer", "draw-normals", &noYes[1], RI_NULL);
#endif
	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
	RiFrameBegin(counter);

	// Set the display spec
	if ( [(MyNSObjectController *)panelController displayEnabled] ) {
		origin[0] = (RtFloat)[(MyNSObjectController *)panelController originX];
		origin[1] = (RtFloat)[(MyNSObjectController *)panelController originY];
#ifdef _TRACE
		printf("originX %f, originY %f\n", origin[0], origin[1]);
#endif
		RiDisplay("OpenGL Demo", RI_FRAMEBUFFER, RI_RGBA,
				  RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect,
				  RI_ORIGIN, origin, RI_NULL);
		RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect, RI_ORIGIN, origin, RI_NULL);
	} else {
		[(MyNSObjectController *)panelController displayOriginX:origin[0] originY:origin[1]];
		RiDisplay("OpenGL Demo", RI_FRAMEBUFFER, RI_RGBA,
				  RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect,
				  RI_ORIGIN, origin, RI_NULL);
		RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect, RI_ORIGIN, origin, RI_NULL);
	}
	
	// Commented out, origin do not influences display size
	/*
	width -= origin[0]; if ( width < 0 ) width = 0;
	height -= origin[1]; if ( height < 0 ) height = 0;
	*/
	
#ifdef _TRACE
	printf("Bounds width %f, height %f\n", bounds.size.width, bounds.size.height);
#endif
	
	// Raster format
	if ( [(MyNSObjectController *)panelController formatEnabled] ) {
		width = (RtInt)[(MyNSObjectController *)panelController formatWidth];
		height = (RtInt)[(MyNSObjectController *)panelController formatHeight];
		pixelAspect = (RtFloat)[(MyNSObjectController *)panelController formatPixelAspect];
		RiFormat(width, height, pixelAspect);
	} else {
		[(MyNSObjectController *)panelController formatWidth:width height:height pixelAspect:pixelAspect];
		// default: frame size = display size
		// RiFormat(width, height, pixelAspect);
	}
	
	// Frame aspect ratio
	RtFloat frameAspect = (RtFloat)width*pixelAspect/(RtFloat)height;
	if ( [(MyNSObjectController *)panelController frameAspectEnabled] ) {
		frameAspect = (RtFloat)[(MyNSObjectController *)panelController frameAspect];
		RiFrameAspectRatio((RtFloat)frameAspect);
	} else {
		[(MyNSObjectController *)panelController frameAspect:frameAspect];
		// That's the default
		// RiFrameAspectRatio(frameAspect);
	}

	// Screen Window
	RtFloat screenLeft, screenRight, screenBottom, screenTop;
	if ( frameAspect < 1 ) {
		screenLeft = -1.0;
		screenRight = 1.0;
		screenBottom = 1.0/-frameAspect;
		screenTop = 1.0/frameAspect;
	} else {
		screenLeft = -frameAspect;
		screenRight = frameAspect;
		screenBottom = -1.0;
		screenTop = 1.0;
	}

	if ( [(MyNSObjectController *)panelController screenEnabled] ) {
		screenLeft = (RtFloat)[(MyNSObjectController *)panelController screenLeft];
		screenRight = (RtFloat)[(MyNSObjectController *)panelController screenRight];
		screenBottom = (RtFloat)[(MyNSObjectController *)panelController screenBottom];
		screenTop = (RtFloat)[(MyNSObjectController *)panelController screenTop];
		RiScreenWindow(screenLeft, screenRight, screenBottom, screenTop);
	} else {
		[(MyNSObjectController *)panelController screenLeft:screenLeft right:screenRight bottom:screenBottom top:screenTop];
		// That's the default
		// RiScreenWindow(screenLeft, screenRight, screenBottom, screenTop);
	}
	
	// Clipping
	// RtFloat hither = RI_EPSILON, yonder = RI_INFINITY;
	RtFloat hither = (RtFloat)0.4, yonder = 500;
	if ( [(MyNSObjectController *)panelController clippingEnabled] ) {
		hither = (RtFloat)[(MyNSObjectController *)panelController clippingHither];
		yonder = (RtFloat)[(MyNSObjectController *)panelController clippingYonder];
		RiClipping(hither, yonder);
	} else {
		[(MyNSObjectController *)panelController clippingHither:hither yonder:yonder];
		RiClipping(hither, yonder);
	}

	// Projection (here: defaults set by controller - maybe preferable)
	if ( [(MyNSObjectController *)panelController projectionEnabled] ) {
		RtToken proj = [(MyNSObjectController *)panelController projectionType];
		if ( proj == RI_PERSPECTIVE ) {
			if ( [(MyNSObjectController *)panelController fovEnabled] ) {
				RtFloat fov = [(MyNSObjectController *)panelController fov];
				RiProjection(RI_PERSPECTIVE, RI_FOV, &fov, RI_NULL);
			} else {
				// Default fov is 90 degree
				RiProjection(RI_PERSPECTIVE, RI_NULL);
			}
		} else if ( proj == RI_ORTHOGRAPHIC ) {
			RiProjection(RI_ORTHOGRAPHIC, RI_NULL);
		} else {
			// RI_NULL projection
			RiProjection(RI_NULL, RI_NULL);
		}
	} else {
		// Defaults automatic, orthographic projection
		// RiProjection(RI_ORTHOGRAPHIC, RI_NULL);
	}

	// Crop
	RtFloat xmin=0.0, xmax=1.0, ymin=0.0, ymax=1.0;
	if ( [(MyNSObjectController *)panelController cropEnabled] ) {
		xmin = (RtFloat)[(MyNSObjectController *)panelController cropLeft];
		xmax = (RtFloat)[(MyNSObjectController *)panelController cropRight];
		ymax = (RtFloat)[(MyNSObjectController *)panelController cropBottom];
		ymin = (RtFloat)[(MyNSObjectController *)panelController cropTop];
		RiCropWindow(xmin, xmax, ymin, ymax);
	} else {
		[(MyNSObjectController *)panelController cropLeft:xmin right:xmax bottom:ymax top:ymin];
		// That's the default
		// RiCropWindow(xmin, xmax, ymin, ymax);
	}
	
	// World Block
	RiWorldBegin();
	RiSides(1);
	RiColor(greenish);

	switch ( counter ) {
		case 0:
			testPoly(); // Simple concave no holes
			break;
		case 1:
			testPoly1(); // Simple concave with holes
			break;
		case 2:
			testPoly2(); // Simple concave with holes (window)
			break;
		case 3:
			testPoly3(); // Simple concave with holes (two rectangles)
			break;
		case 4:
			testPoly4(); // Simple concave with holes (touching two rectangles)
			break;
		case 5:
			testPoly5(); // Simple convex (vertex in a line)
			break;
		case 6:
			testPoly6(); // Simple concave with holes (holes touch boundary)
			break;
		case 7:
			testPoly7(); // Simple concave no holes (boundary vertices form holes)
			break;
		case 8:
			testPoly8(); // Simple concave no holes (boundary vertices form holes and isles)
			break;
		case 9:
			testPoly9(); // Simple convex with holes (holes to boundary)
			break;
		case 10:
			testPoly10(); // Simple squares with different opacity
			break;
		case 11: // Cone
			testCone();
			break;
		case 12: // Cylinder
			testCylinder();
			break;
		case 13: // Disk
			testDisk();
			break;
		case 14: // Hyperboloid
			testHyperboloid();
			break;
		case 15: // Paraboloid
			testParaboloid();
			break;
		case 16: // Sphere
			testSphere();
			break;
		case 17: // Torus
			testTorus();
			break;
		case 18: // Bilinear Patch
			testBilinPatch();
			break;
		case 19: // Bilinear Patch Mesh
			testBilinPatchMesh();
			break;
		case 20: // Bicubic Patch Mesh (pin)
			testBicubicMesh();
			break;
		case 21: // Bicubic Patch
			testBicubicPatch();
			break;
		case 22: // NuPatch (vase)
			testNuPatch();
			break;
		case 23: // Teapot
			testTeapot();
			break;
		default:
			// drawAnObject(); // GL test
			break;
	}
	
	RiWorldEnd();
	RiFrameEnd();
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
}

- (IBAction)nextPoly:(id)sender
{
	/* NSLog(@"nextPoly"); */
	++counter;
	[self resetCamera];
	[self display];
}

- (IBAction)prevPoly:(id)sender
{
	/* NSLog(@"nextPoly"); */
	--counter;
	[self resetCamera];
	[self display];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	// NSLog(@"rightMouseUp");
	[self resetCamera];
	[self display];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	// NSLog(@"mouseDragged");
	if ( !theEvent )
		return;
	sphi += [theEvent deltaX] / 4.0;
	stheta += [theEvent deltaY] / 4.0;
	[self display];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	// NSLog(@"scrollWheel");
	if ( !theEvent )
		return;
	sdepth += [theEvent deltaY] / 10.0;
	[self display];
}

@end
