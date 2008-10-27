#ifndef _RICPP_3DKIT_GNU3D_H
#define _RICPP_3DKIT_GNU3D_H

/*
 *  RiCPP 3dkit based havily on 3dkit and gnu3dkit. It is very experimental. The author tries to get
 *  the (Gnu)3dkit object structure working together with RiCPP. It will be first compiled for Apple's Cocoa and if that works
 *  for GnuStep. The main differences between the 3dkit and ricpp3dkit headers are some adoptions for Cocoa and using name
 *  scheme of G3DKit. 
 *  
 *  3dKit is Copyright (c) 1991, 1992 by NeXT Computer, Inc. All rights reserved.
 *  gnu3dkit is Copyright (c) Philippe C.D. Robert
 *  Adapted for RiCPP Andreas Pidde
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */

#ifndef _RICPP_RI_RI_H
#include <ricpp/ri/ri.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    G3D_Perspective = 0,
    G3D_Orthographic
} G3DProjectionType;

typedef enum {
    G3D_AmbientLight = 0,
    G3D_PointLight,
    G3D_DistantLight,
    G3D_SpotLight
} G3DLightType;

typedef enum {
    G3D_AllAxes = 0,
    G3D_XAxis,
    G3D_YAxis,
    G3D_ZAxis,
    G3D_XYAxes,
    G3D_XZAxes,
    G3D_YZAxes
} G3DAxis;

typedef enum {
	G3D_HiddenRendering = 0,
	G3D_InOrderRendering,
	G3D_NoRendering
} G3DHider;

typedef enum {
	G3D_PointCloud = 0,
	G3D_WireFrame,
	G3D_ShadedWireFrame,
	G3D_FacetedSolids,
	G3D_SmoothSolids
} G3DSurfaceType;

/*
 *  Macros for quick structure copies.  Turns into small code when
 *  compiled with -O.
 */

#define G3D_CopyMatrix( s_, d_ )  do { \
		    struct _cms { RtMatrix _m; }; \
		    *((struct _cms *)(d_)) = *((struct _cms *)(s_)); \
		    } while (0)
#define G3D_CopyPoint( s_, d_ )  do { \
		    struct _cps { RtPoint _p; }; \
		    *((struct _cps *)(d_)) = *((struct _cps *)(s_)); \
		    } while (0)
#define G3D_CopyBound( s_, d_ )  do { \
		    struct _cps { RtBound _b; }; \
		    *((struct _cps *)(d_)) = *((struct _cps *)(s_)); \
		    } while (0)
#define G3D_ConvertBoundToPoints( b_, p_ )	do { \
		    (p_)[0][0] = (b_)[0]; \
		    (p_)[0][1] = (b_)[2]; \
		    (p_)[0][2] = (b_)[4]; \
		    (p_)[1][0] = (b_)[1]; \
		    (p_)[1][1] = (b_)[3]; \
		    (p_)[1][2] = (b_)[5]; \
		    } while (0)
#define G3D_ConvertPointsToBound( p_, b_ )	do { \
		    (b_)[0] = (p_)[0][0]; \
		    (b_)[1] = (p_)[1][0]; \
		    (b_)[2] = (p_)[0][1]; \
		    (b_)[3] = (p_)[1][1]; \
		    (b_)[4] = (p_)[0][2]; \
		    (b_)[5] = (p_)[1][2]; \
		    } while (0)
/*
 *  Single-value macros for accessing point coords.
 */
#define G3D_XComp( _p )	((_p)[0])
#define G3D_YComp( _p )	((_p)[1])
#define G3D_ZComp( _p )	((_p)[2])
#define G3D_WComp( _p )	((_p)[3])

void G3DMult3DPoint( RtPoint p, RtMatrix m, RtPoint result );
void G3DMult3DPoints( RtPoint *p, int count, RtMatrix m, RtPoint *result );
void G3DIntersectLinePlane( RtPoint *L, RtPoint pnorm, RtPoint ppoint,
			RtPoint *psect );
void G3DMultiplyMatrix( RtMatrix a, RtMatrix b, RtMatrix result );
float G3DInvertMatrix( RtMatrix m, RtMatrix inverse_m );

/* Various and sundry useful globals */
extern NSString *G3DRIBPboardType;
extern const RtPoint G3DOrigin;
extern const RtMatrix G3DIdentityMatrix;


#ifdef __cplusplus
}
#endif
#endif // _RICPP_3DKIT_GNU3D3D_H
