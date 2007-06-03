// passthrough.cpp : Defines the entry point for the DLL application.
//

#include "ricpp/ribfilter/ribfilter.h"
#include <windows.h>

using namespace RiCPP;

/** @brief Export declaration for the included functions
 */
#define EXPORT __declspec ( dllexport )


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" {

/** @fn CRibFilter *newPlugin(unsigned long majorversion, const char *type)
 *  @brief Returns an instance of a new plugin
 *  @param majorversion Major version number of the returnd type
 *  @param type Type must match the type of the plugin
 *  @return Returns a plugin if \a majorversion has the right value
 */
EXPORT
CRibFilter * CDECL newPlugin(unsigned long majorversion, const char *type)
{
	if ( majorversion != CRibFilter::myMajorVersion() || !type || strcmp(type, CRibFilter::myType()) != 0 )
		return NULL;
	return new CRibFilter;
}

/** @fn void deletePlugin(CRibFilter *p)
 *  @brief Deletes a plugin created by newPlugin()
 *  @param p Plugin that is deletetd, must have been constructed by newPlugin()
 */
EXPORT
void CDECL deletePlugin(CRibFilter *p)
{
	if ( p )
		delete p;
}

/** @fn unsigned long majorVersion()
 *  @brief Returns the major version number of the plugin
 *  @return The major version of the plugin
 */
EXPORT
unsigned long CDECL majorVersion()
{
	return CRibFilter::myMajorVersion();
}

/** @fn unsigned long minorVersion()
 *  @brief Returns the minor version number of the plugin
 *  @return The minor version of the plugin
 */
EXPORT
unsigned long CDECL minorVersion()
{
	return CRibFilter::myMinorVersion();
}

/** @fn unsigned long revision()
 *  @brief Returns the revision of the plugin
 *  @return The revision number of the plugin
 */
EXPORT
unsigned long CDECL revision()
{
	return CRibFilter::myRevision();
}

/** @fn RtToken type()
 *  @brief Returns the type of the plugin
 *  @return The renderer type the plugin
 */
EXPORT
const char * CDECL type()
{
	return CRibFilter::myType();
}

/** @fn RtToken name()
 *  @brief Returns the name of the plugin
 *  @return The name of the plugin
 */
EXPORT
const char * CDECL name()
{
	return CRibFilter::myName();
}

}
