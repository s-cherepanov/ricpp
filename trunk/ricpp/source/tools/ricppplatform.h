#ifndef RICPP_TOOLS_PLATFORM_H
#define RICPP_TOOLS_PLATFORM_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! \file baseclass.h
 *  \brief Include file with system dependent content
 *  \author Andrew Chapman 
 *          Andreas Pidde (andreas@pidde.de)
 */

//! WINDOWS declarations
#ifdef WIN32
#  ifndef __GNUWIN32__
#    pragma once
#  endif // !__GNUWIN32__

// #  define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here

#  include <windows.h>

// Warnings disable:
// C4097 Synonym for class name,
// C4100 unreferenced formal parameter,
// C4201 non standard nameless struct/union
// C4706 assignment in conditional expression
// #pragma warning ( disable : 4097 4100 4201 4706 )


#  ifndef __GNUWIN32__
//!  Warnings disable for STL in VisualC++
#    pragma warning(disable:4786)
//!  Disable throw() warning in VisualC++
#    pragma warning(disable:4290)
//!  Don't give deprecate warnings for c - string functions
#    pragma warning(disable:4996)
#  endif // !__GNUWIN32__

# ifdef __GNUWIN32__
# endif

// #  define TYPENAME
#endif // WIN32


//! LINUX declarations
#ifdef LINUX
#  include <errno.h> //!< for errno
#  include <dlfcn.h>
#  define WINAPI
#  define CDECL
#  include <locale.h> //!< for setlocale
#  include <string.h> //!< for strcasecmp
#  include <stdarg.h> //!< for snprintf
#  include <stdio.h>  //!< for snprintf
#  include <unistd.h> //!< for getcwd

//! Renaming of some functions
//! {
#  define stricmp strcasecmp
#  define _snprintf  snprintf
#  define _vsnprintf vsnprintf
#  define _getcwd getcwd
#  define TYPENAME typename
#  define _MAX_PATH FILENAME_MAX
#  define HINSTANCE void *
//! }

#define DllImport
#define DllExport
#define __cdecl

//! Gets the last system error stored in errno
/*! \return the value of errno (the last system error)
 */
inline int GetLastError() {
  return errno;
}

//! Loads a dynamic library
/*! \param filename Path to the dynamic library
 *  \return Handle to the dynamic library
 */
inline void * LoadLibrary(const char *filename) {
  return dlopen(filename, RTLD_NOW);
}

//! Closes a dynamic library
/*! \param handle The handle of the dynamic library return by a call of LoadLibrary()
 */
inline void FreeLibrary(void *handle) {
  dlclose(handle);
}

//! Gets the start address of a function in a dynamic library
/*! \param handle The handle of the dynamic library return by a call of LoadLibrary()
 *  \param symbol The name of the element of the library to access
 *  \return Pointer to the function denoted by symbol
 */
inline void * GetProcAddress(void *handle, char *symbol) {
  return dlsym(handle, symbol);
}

//! A pointer to a string
#  define LPSTR char*

//! Logically true
#  define TRUE true

//! Logically false
#  define FALSE false
#endif // LINUX


// Global declarations

// Type for Indices in arrays etc.
typedef unsigned long IndexType;

// Undef min and max (because max is used in <limits> and in helpers.h own versions are supplied)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#endif // RICPP_TOOLS_PLATFORM_H
