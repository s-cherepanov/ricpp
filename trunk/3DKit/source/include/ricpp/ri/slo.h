#ifndef _RICPP_RI_SLO_H
#define _RICPP_RI_SLO_H

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

typedef enum {
	SLO_TYPE_UNKNOWN,
	SLO_TYPE_POINT,
	SLO_TYPE_COLOR,
	SLO_TYPE_SCALAR,
	SLO_TYPE_STRING,
/* The following types are primarily used for shaders */
	SLO_TYPE_SURFACE,
	SLO_TYPE_LIGHT,
	SLO_TYPE_DISPLACEMENT,
	SLO_TYPE_INTERIOR,
	SLO_TYPE_EXTERIOR,
	SLO_TYPE_ATMOSPHERE,
	SLO_TYPE_DEFORMATION,
	SLO_TYPE_IMAGER
} SLO_TYPE;

typedef enum {
	SLO_STOR_UNKNOWN,
	SLO_STORE_CONSTANT,
	SLO_STOR_VARIABLE,
	SLO_STOR_TEMPORARY,
	SLO_STOR_PARAMETER,
	SLO_STOR_GSTATE
} SLO_STORAGE;

typedef enum {
	SLO_DETAIL_UNKNOWN,
	SLO_DETAIL_VARYING,
	SLO_DETAIL_UNIFORM
} SLO_DETAIL;


#define _SHADECON_
typedef struct {
	float xval;
	float yval;
	float zval;
} POINT;
typedef float SCALAR;

typedef struct slovissymdef {
	char *svd_name;
	SLO_TYPE svd_type;
	SLO_STORAGE svd_storage;
	SLO_DETAIL vd_detail;
	char *svd_spacename;
	union {
		POINT *pointval;
		SCALAR *scalarval;
		char *stringval;
	} svd_default;
	union svd_defaultvalu {
		POINT svd_pointval;
		SCALAR svd_scalarval;
	} svd_defaultval;
	unsigned svd_valisvalid : 1;
} SLO_VISSYMDEF;

#define NULL_SLOVISSYMDEF ((SLO_VISSYMDEF *)0)

#endif
