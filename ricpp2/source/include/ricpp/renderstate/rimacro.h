#ifndef _RICPP_RENDERSTATE_RIMACRO_H
#define _RICPP_RENDERSTATE_RIMACRO_H

/** @file rimacro.h
 *  @brief Concentrates the declarations of the macro classes to record interface calls.
 *
 *  There is a class for each interface call, the classes can be used to store interface
 *  calls in memory and 'replay' them later. CRiMacro is the class to store a list of macros.
 *  The renderer base class TIntermediateRenderer manages the construction of macros while
 *  loading a RIB file.
 *
 *  @author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RENDERSTATE_RIMACROCONTAINER_H
#include "ricpp/renderstate/rimacrocontainer.h"
#endif // _RICPP_RENDERSTATE_RIMACROCONTAINER_H

#ifndef _RICPP_RENDERSTATE_RIMACROMISC_H
#include "ricpp/renderstate/rimacromisc.h"
#endif // _RICPP_RENDERSTATE_RIMACROMISC_H

#ifndef _RICPP_RENDERSTATE_RIMACROMODES_H
#include "ricpp/renderstate/rimacromodes.h"
#endif // _RICPP_RENDERSTATE_RIMACROMODES_H

#ifndef _RICPP_RENDERSTATE_RIMACROOPTIONS_H
#include "ricpp/renderstate/rimacrooptions.h"
#endif // _RICPP_RENDERSTATE_RIMACROOPTIONS_H

#ifndef _RICPP_RENDERSTATE_RIMACROLIGHTS_H
#include "ricpp/renderstate/rimacrolights.h"
#endif // _RICPP_RENDERSTATE_RIMACROLIGHTS_H

#ifndef _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H
#include "ricpp/renderstate/rimacroattributes.h"
#endif // _RICPP_RENDERSTATE_RIMACROATTRIBUTES_H

#ifndef _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H
#include "ricpp/renderstate/rimacrotransforms.h"
#endif // _RICPP_RENDERSTATE_RIMACROTRANSFORMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROPRIMS_H
#include "ricpp/renderstate/rimacroprims.h"
#endif // _RICPP_RENDERSTATE_RIMACROPRIMS_H

#ifndef _RICPP_RENDERSTATE_RIMACROEXTERNALS_H
#include "ricpp/renderstate/rimacroexternals.h"
#endif // _RICPP_RENDERSTATE_RIMACROEXTERNALS_H

#endif // _RICPP_RENDERSTATE_RIMACRO_H
