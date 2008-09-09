#ifndef _RICPP_RICPP_RICPP_H
#define _RICPP_RICPP_RICPP_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
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

/** @file ricpp.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Abstract interface for the RenderMan(R) C++ Binding
 *
 @verbatim
       RenderMan(R) is a registered trademark of Pixar
   The RenderMan(R) Interface Procedures and Protocol are:
           Copyright 1988, 1989, 2000, 2005 Pixar
                   All rights Reservered
 @endverbatim
 *
 * ricpp.h means the same to RiCPP as ri.h to the standard C binding.
 */

#ifndef _RICPP_RIBASE_RICPPCONST_H
#include "ricpp/ribase/ricppconst.h"
#endif _RICPP_RIBASE_RICPPCONST_H
 
#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif _RICPP_RIBASE_RICPPTOKENS_H

/** @namespace RiCPP
 *  @brief All of the interface is placed within the RiCPP namespace.
 */
namespace RiCPP {

// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------

/** Forward declaration of the RI (RenderMan Interface)
 */
class IRiRoot;
class IRi;

/** @brief Interface for the error handler (was RtErrorHandler)
 */
class IErrorHandler {
public:
	inline virtual ~IErrorHandler() {}

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the filter function.
	 */
	virtual IErrorHandler *duplicate() const = 0;

	/** @brief The name of the error handler.
	 * @return The name of the error handler as used in RIB files.
	 */
	virtual RtToken name() const = 0;

	/** @brief Handles the error.
	 * @param ri Front end that detected the error
	 * @param code Error code (RIE_...).
	 * @param severity Error severity level (RIE_INFO ... RIE_SEVERE).
	 * @param msg Error message, describing the specific error in detail.
	 */
	virtual RtVoid operator()(IRi &ri, RtInt code, RtInt severity, RtString msg) const = 0;

	/** @brief A singleton error handler.
	 * @return A singleton error handler (static error handler).
	 */
	virtual const IErrorHandler &singleton() const = 0;
};

/** @brief Interface for a pixel filter (super sampling).
 */
class IFilterFunc {
public:
	IFilterFunc();
	virtual ~IFilterFunc();
	
	/** @brief Clone the instance.
	 *  @return A new cloned instance of the filter function.
	 */
	virtual IFilterFunc *duplicate() const = 0;

	/** @brief The name of the filter.
	 *  @return The name of the filter as used in RIB binding.
	 */
	virtual RtToken name() const = 0;

	/** @brief The implementation of the filter function.
	 *
	 *  Each supersample in the rectangular area xwidth, ywidth contributes by its
	 *  weight to the output value of a pixel in the center of this rectangle.
	 *  The filter function is used to calculate the weight of a sample at the x,y position
	 *  relative to the center of the pixel (grid cell).
	 *  See RISPEC3.2 Appendix E, UPS89 Filtering the supersamples, 176ff.
	 *  @param x x-coordinate of the supersample in the image relative to the center, -(xwidth/2) <= x <= (xwidth/2)
	 *  @param y y-coordinate of the supersample in the image relative to the center, -(ywidth/2) <= y <= (ywidth/2)
	 *  @param xwidth Influence in pixels of the filter in horizontal direction
	 *  @param ywidth Influence in pixels of the filter in vertical direction
	 *  @return The weigth of the sample, the final pixel value is the average of the weighted supersamples.
	 *  @see CRiRoot::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
	 */
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const = 0;

	/** @brief A singleton pixel filter.
	 * @return A singleton pixel filter (static pixel filter).
	 */
	virtual const IFilterFunc &singleton() const = 0;
};

/** @brief Data container for Subdiv functions
 *
 *  Inherit from this object to make data for own procedurals.
 */
class ISubdivData {
public:
	ISubdivData();

	/** @brief Destructor to free data.
	 */
	virtual ~ISubdivData();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	virtual ISubdivData *duplicate() const = 0;

	/** @brief Gets the data of the instance.
	 *  @return The data of the instance, not a copy.
	 */
	virtual RtPointer data() = 0;

	/** @brief Gets the read only data of the instance.
	 *  @return The read only data of the instance, not a copy.
	 */
	virtual const RtPointer data() const = 0;

	/** @brief Sets the data of the instance.
	 *  @param da The data for the instance, creates a copy.
	 */
	virtual void data(RtPointer da) = 0;

	/** @brief Frees the data of the instance.
	 */
	virtual void freeData() = 0;
};

/** @brief Interface, subdivision for procedurals changed to include renderer instance.
 */
class ISubdivFunc {
public:
	ISubdivFunc();
	virtual ~ISubdivFunc();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	virtual ISubdivFunc *duplicate() const = 0;

	/** @brief Name of the subdivision function.
	 *  @return The name of the subdivision function as used in RIB binding.
	 */
	virtual RtToken name() const = 0;

	/** @brief Number of (string) arguments used by the function.
	 *  @return The number of (string) arguments used by the function.
	 */
	virtual RtInt numArgs() const = 0;

	/** @brief The subdivision function as operator()().
	 *  @param ri the frontend for interface calls of the subdivision function.
	 *  @param data The data handled by the subdivision function.
	 *  @param detail the current level of detail.
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const = 0;

	/** @brief Creates the data structure needed for subdivision.
	 *  @param data The data handled by the subdivision function.
	 *  @return A data structure containing a copy of @a data.
	 */
	virtual ISubdivData *createSubdivData(RtPointer data) const = 0;

	/** @brief A singleton subdivision/procedural.
	 *  @return A singleton subdivision/procedural (static subdivision/procedural).
	 */
	virtual ISubdivFunc &singleton() = 0;
};

/** @brief Free function for procedurals.
 */
class IFreeFunc {
public:
	IFreeFunc();
	virtual ~IFreeFunc();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	virtual IFreeFunc *duplicate() const = 0;

	/** @brief Name of the free function.
	 *  @return The name of the free function as used in RIB binding.
	 */
	virtual RtToken name() const = 0;

	/** @brief The free function as operator()().
	 * @param data The data handled by the subdivision function that should be freed.
	 */
	virtual RtVoid operator()(RtPointer data) const = 0;

	/** @brief A singleton free.
	 * @return A singleton free (static free).
	 */
	virtual IFreeFunc &singleton() = 0;
};

/** @brief Callback function to handle structural comments in rib files (IRi::readArchive),
 *  changed to include renderer instance and has a stripped signature
 */
class IArchiveCallback {
public:
	IArchiveCallback();
	virtual ~IArchiveCallback();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	virtual IArchiveCallback *duplicate() const = 0;

	/** @brief Name of the callback function
	 */
	virtual RtToken name() const = 0;

	/** @brief The callback function as operator()()
	 *  @param ri the frontend used by the subdivision function that handled the data.
	 *  @param type The type of the comment, either RI_COMMENT or RI_STRUCTURE. RI_VERBATIM
	 *              cannot be handled.
	 *  @param line The (formatted) comment line, smae as the formatted string
	 *              given by IRi::archiveRecordV()
	 */
	virtual RtVoid operator()(IRi &ri, RtToken type, RtString line) const = 0;

	/** @brief Returns a singleton object
	 *  @return Singleton archive callback
	 */
	virtual const IArchiveCallback &singleton() const = 0;
};

// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------

/** @brief RenderMan Interface without the ellipsis (...) calls
 * 
 * Base class for frontend IRiCPP::IRi and backend context IRiCPP::IRiContext,
 * also used for RIB interception. Contains the interface functions
 * @see IRi, ricpp_interface
 */
class IRiRoot {
public:
	IRiRoot();

	/** @brief The virtual destructor of the interface
	 */
	virtual ~IRiRoot();

	/** @defgroup ricpp_interface Ri interface functions
	 *  @brief The interface functions, part 1
	 *  @see IRi
	 *  @{
	 */

	/** @defgroup ricpp_error Ri error handling
	 *  @ingroup ricpp_interface
	 *  @brief User defined error handling
	 *  @{
	 */

	/** @brief Sets an error handler.
	 *
	 *  The error handler (predifined are
	 *  @c errorAbort, @c errorPrint, @c errorIgnore)
	 *  is called if an error occurs. It is a front end function because it
	 *  is not bound to a specific context.
	 *  The ErrorHandler RIB command readed in from a RIB archive can be deactivated
	 *  by the front end to avoid disturbing program logic. The context
	 *  IRiContext itself does not have an IRiRoot::ErrorHandler()
	 *  handler implemented: It throws a RiCPP::ERendererException. The front end
	 *  end catches this exception and calls the user defined error handler.
	 *  However, because the error handler can be written as a RIB command,
	 *  a stub must exist at the back end.
	 *
	 *  @param handler The error handler
	 *  @see ricpp_errorconst, IErrorHandler, ricpp_severity
	 */
	virtual RtVoid errorHandler(const IErrorHandler &handler) = 0;

	/** @brief Gets the last error number occured.
	 */
	virtual RtInt lastError() = 0;
	//@}
	//@}

	/** @defgroup ricpp_other Ri other routines
	 *  @ingroup ricpp_interface
	 *  @brief Declarations, synchronizing and system
	 *  @{
	 */

	/** @brief Declares a token (parameter type)
	 *  @param name name of the token
	 *  @param declaration declaration string of the token
	 *  @return A unique token
	 *  s.a. ARM2000, Parameter Declarations, 64-66,
	 *  s.a. RISPEC3.2, RiDeclare, pp. 13-15,
	 *  s.a. STE2003, Declaring parameter types, 150-152,
	 *  s.a. UPS89, Extending the set of variables, 242-244
	 */
	virtual RtToken declare(RtToken name, RtString declaration) = 0;
 
	//! @brief Synchronize the rendering state [QRM], front end function like errorHandler
	/*! @param name Type of synchronization, e.g. RI_ABORT to abort the rendering
	 */
    virtual RtVoid synchronize(RtToken name) = 0;

	/** @brief Executes an arbitrary system command
	 * @param cmd The command
	 */
	virtual RtVoid system(RtString cmd) = 0;

	/** @brief Controls the backend renderer or the frontend.
	 *
	 *  Control request are not part of the RIB binding. It's a mechanism
	 *  to control the non rendering parts of the renderer, the controls are not
	 *  used for RIB output. For example a control request can be used to switch
	 *  between ascii and binary output of the RIB binding (CRibWriter). This request
	 *  has to be passed to the renderer somehow, but is technically not a RIB request.
	 *  Since the renderers "know" their control requests, control tokens have not
	 *  to be declared. Normally renderers will have no special control requests.
	 *
	 *  Control requests take effect immediatly, they are not stored for defered rendering.
	 *
	 *  @param name Name of the control request
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the control request
	 */
    virtual RtVoid controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Writes RIB version information if needed
	 */
	virtual RtVoid version()=0;
	//@}

	/** @defgroup ricpp_resource Ri Resource handling
	 *  @ingroup ricpp_interface
	 *  @brief Handling of external resources
	 *  @{
	 */

	/** @brief Creates an external resource. Scope inside resourceBegin, resourceEnd
	 *
	 *  resourceV() creates  a resource (input/output, store/reload) @a name of the given @a type
	 *
	 *  Pixars C-binding exposes no return type (RtString name -> name string to identify the handle).
	 *  Can be used to store sets of attributes.
	 *
	 *  External resources maybe cannot be output to RIB (e.g. Memory resources for deffered read archive).
	 *  However, the referenced resource can be used. If an handle is used to cache resources this
	 *  caching hint may be lost in RIB.
	 *
	 *  Resources are removed, if the current mode is ending. However, only the handle
	 *  is removed, not an external resource itself - so you must free the
	 *  memory or close the file if it is not needed any more. Internally handled
	 *  resources like the storage for attributes are appropriatly freed. A resource
	 *  can be overwritten by another one with the same name (warning). The overwritten
	 *  one is not accessibly and will be freed as usual at the blocks end.
	 *
     *  @param handle Name of the resource 
     *  @param type Type of the resource
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list.
	 *  @param params Value pointer for additional parameter list for the resource
	 *  @see resourcebegin(), resourceEnd()
	 */
	virtual RtVoid resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	/** @defgroup ricpp_modes Ri Modes
	 *  @ingroup ricpp_interface
	 *  @brief The modes
	 *  @{
	 */

	/** @brief Creates and initializes a new rendering context
	 *
	 * In a context it is called after instanciation, changed the signature (similar to [QRM])
	 * to simplify the handling of arguments for the renderer.
	 * @param name indicates either a rib file for output or an identifier for a renderer
	 * @param n Number of tokens
	 * @param tokens Tokens for additional parameter list
	 * @param params Value pointer for additional parameter list for the renderer, e.g. RI_FILE outputfile
	 * @return The new context handle
	 */
	virtual RtContextHandle beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
	/** @brief Terminates the current rendering context
	 *
	 * Does cleanup operations, in a context it is called before the context is deleted.
	 */
	virtual RtVoid end(void) = 0;

	//! @brief Starts a new single frame block of an animation sequence
	/*! @param number of the frame
	 */
	virtual RtVoid frameBegin(RtInt number) = 0;

	//! @brief Ends a frame block
	virtual RtVoid frameEnd(void) = 0;

	//! @brief Starts a new world block, the description of the scene geometry
	virtual RtVoid worldBegin(void) = 0;

	//! @brief Ends a world block
	virtual RtVoid worldEnd(void) = 0;

	//! @brief Starts a new attribute block, pushes the current set of attriubes
	virtual RtVoid attributeBegin(void) = 0;

	//! @brief Ends an attribute block, pops the set of attributes
	virtual RtVoid attributeEnd(void) = 0;

	//! @brief Starts a transformation block, pushes the composit transformation matrix
	virtual RtVoid transformBegin(void) = 0;

	//! @brief Ends a transformation block, pops the composit transformation matrix
	virtual RtVoid transformEnd(void) = 0;

	//! @brief Starts a solid block, a CSG solid primitive
	/*! @param type Type of the solid block either
	 *  RI_PRIMITIVE, RI_INTERSECTION, RI_UNION or RI_DIFFERENCE
	 */
    virtual RtVoid solidBegin(RtToken type) = 0;

	//! @brief Ends a solid block
    virtual RtVoid solidEnd(void) = 0;

	/** @brief Starts a new object block for retained models
	 *  
	 *  The object is freed at its neares enclosing frame or world block. It inherits
	 *  the object state, when it is instanciated. Calls inside do not have
	 *  side effects when instanciated - that's different to a rib archive. Nested blocks
	 *  inside have to be balanced. You can declare variables inside object blocks and
	 *  they stay declared after objectEnd(), but are not redeclared by objectInstance().
	 *
	 *  If used inside an archive block, it cannot be used outside this block and is
	 *  freed with this block. Dito for objects in objects.
	 *
	 *  Cannot be nested.
	 *
	 *  The rib writer will have an option to expand macros.
	 *
	 *  @param name A name parameter make sense for RIB string handles, but is not
	 *  standard, maybe I delete it again.
	 *
	 *  @return The handle is used to later reference the object in IRi::objectInstance()
	 */
	virtual RtObjectHandle objectBegin(RtString name) = 0;

	//! @brief Ends an object block, the object can now be created with IRi::objectInstance()
	virtual RtVoid objectEnd(void) = 0;

	//! @brief Instanciate retained models (objectBegin(), objectEnd())
	/*! Creates a new object referenced by handle, the object inherits the
	 *  current state but do not have side effects.
	 * @param handle Reference to an object, returned by objectBegin
	 */
    virtual RtVoid objectInstance(RtObjectHandle handle) = 0;

	//! @brief Starts a motion block
	/*! @param N     Number of samples (length of times)
	 *  @param times Vector of samples
	 */
    virtual RtVoid motionBeginV(RtInt N, RtFloat times[]) = 0;

	//! @brief End of a motion block
    virtual RtVoid motionEnd(void) = 0;

	/** @brief Begins a resource block.
	 *
	 *  Controls the scope of resource()
	 */
	virtual RtVoid resourceBegin(void) = 0;

	/** @brief Ends a resource block.
	 */
	virtual RtVoid resourceEnd(void) = 0;

	/** @brief Begins an inline archive
	 *
	 *  Stores the commands in memory for later use. When @a name is used in
	 *  readArchiveV() and the DelayedReadArchive procedural,
	 *  the stored archive @a name is used.
	 *  Macros are freed at its neares enclosing frame or world block.
	 *  This command does not write a RIB entity or file. The rib writer will
	 *  have an option to expand archivs.
	 *
	 *  Can be nested.
	 *
	 *  @param name Name of the archive
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtArchiveHandle archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Ends an archive
	 *  The commands are not longer stored if the outer most archive block is closed
	 */
	virtual RtVoid archiveEnd(void) = 0;
	//@}

	/** @defgroup ricpp_options Ri Options
	 *  @ingroup ricpp_interface
	 *  @brief Options
	 *  @{
	 */

	//! Sets display resolution
	/*! Sets the hroizontal (xres) and vertical (yres) resolution (in pixels)
	 *  of the image to be rendered.
	 *  @param xres Horizontal resolution (pixel)
	 *  @param yres Vertical resolution (pixel)
	 *  @param aspect pixel aspect ratio pixelwidth/pixelheight
	 */
    virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect) = 0;

	//! Sets the aspect ration of the frame
	/*! Frameaspectratio is the ratio of the width to the height of the desired image.
	 *  @param aspect frame aspect ratio width/height
	 */
    virtual RtVoid frameAspectRatio(RtFloat aspect) = 0;

	//! Sets the coordinates of the screen window (screen coordinates left <= right, bot <= top)
	/*! Defines a rectangle in the image plane that gets mapped to the raster
	 *  coordinate system and that corresponds to the display areea selected.
	 *  @param left screen coordinate of the left window border
	 *  @param right screen coordinates of the right window border
	 *  @param bot screen coordinates of the bottom window border
	 *  @param top screen coordinates of the top window border
	 */
    virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;

	//! Renders only a subrectangle of an image.
	/*! Sets a crop window in normalized device coordinates "NDC", (0,0) is upper left
	 *  @param xmin left side
	 *  @param xmax right side
	 *  @param ymin upper side
	 *  @param ymax lower side
	 */
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;

	//! Sets the type of 3D to 2D projection
	/*! Sets the type of the projection and marks the current coordinate system
	 *  before projection as the camera coordinate system.
	 *  @param name Type od the projection, e.g. RI_PERSPECTIVE, RI_ORTHOGRAPHIC
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the projection, e.g. RI_FOV float
	 */
    virtual RtVoid projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! The hither and yonder clipping plane, orthogonal to the camera view
	/*! @param hither distance of the front clipping plane
	 *  @param yon distance of the back clipping plane
	 */
	virtual RtVoid clipping(RtFloat hither, RtFloat yon) = 0;

	//! Additional clipping plane
	/*! Clip all geometry on the positive side of the plane described
	 *  by a point and a normal
	 *  @param x x coordinate of a point of the plane
	 *  @param y y coordinate of a point of the plane
	 *  @param z z coordinate of a point of the plane
	 *  @param nx x of the normal
	 *  @param ny y of the normal
	 *  @param nz z of the normal
	 */
    virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0; // *New 3.2*

	//! Depth of field
	/*! focaldistance sets the distance along the directionof view at which
	 *  objects will be in focus
	 *  @param fstop
	 *  @param focallength
	 *  @param focaldistance
	 */
    virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;

	//! Shutter times used for motion blur
	/*! Sets the time at which the shutter opens and closes
	 *  @param smin Start time, shutter opens
	 *  @param smax End time, shutter closes
	 */
    virtual RtVoid shutter(RtFloat smin, RtFloat smax) = 0;

	//! Pixel variance for shading
	/*! Sets the amount computed image values are allowed to
	 *  deviate from the true image values
	 *  @param variation Value of the deviation
	 */
	virtual RtVoid pixelVariance(RtFloat variation) = 0;

	//! Samples used for one pixel
	/*! Sets the effective sampling rate in the horizontal and vertical direction
	 *  @param xsamples sampling rate in horizontal direction
	 *  @param ysamples sampling rate in vertical direction
	 */
    virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;

	//! Sets a pixel filter influenced by an area of xwidth*ywidth pixels
	/*! Performs antialiasing by filtering the geometry (or supersampling) and
	 *  then sampling at pixel locations.
	 *  @param function filter function
	 *  @param xwidth size of influence in horizontal direction
	 *  @param ywidth size of influence in vertical direction
	 */
    virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;

	//! Controls the intensity
	/*! Controls the sensitivity and non-linearity of the exposure process.
	 *  Color_out = (Color_in*gain)^1/gamma
	 *  @param gain sensitivity component
	 *  @param gamma non-linearity component
	 */
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma) = 0;

	//! Sets an imager shader
	/*! Selects an imager function programmed in the Shading Language
	 *  @param name Name of the shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the imager
	 */
    virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the quantization parameters and dithering for colors or depth.
	/*! The result value is calculated by ivalue=fvalue*one+ditheramplitude*random()
	 *  @param type The type of quantization RI_RGBA (colors) or RI_Z (depth)
	 *  @param one  Multiply every value by this number
	 *  @param qmin Minimum value, smaller values are clamed
	 *  @param qmax Maximum value, higher values are clamed
	 *  @param ampl Ditheramplitude, random value (between -1.0 and 1.0) added
	 */
	virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;

	/** @brief Defines a new display channel for display()
	 *  @param channel Identifier and type information for the Channel
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameters to declare the type of the channel
	 */
	virtual RtVoid displayChannelV(RtString channel, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Choose a display (or file) and sets the type for the output being rendered.
	/*! @param name Name for the display or filename
	 *  @param type Type of the output, e.g. RI_FRAMEBUFFER, RI_FILE
	 *  @param mode Mode of the output, channels e.g. RI_RGBA, RI_Z, see displayChannelV()
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list (RI_ORIGIN)
	 *  @param params Value pointer for additional parameter list for the display
	 */
    virtual RtVoid displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! @brief Choose the hidden surface algorithm
	/*! @param type Hidden surface algorithm, default types are
	 *              RI_HIDER (hidden surface), RI_PAINT (paint in the order of the definition)
	 *              and the RI_NULL_LIT "null" (can also be RI_NULL, performs no output)
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the hider
	 */
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Controls the number of color components or samples to be used in specific colors
	/*! @param N number of samples
	 *  @param nRGB Nx3 matrix, converts N sample to a 3 component RGB value
	 *  @param RGBn 3xN matrix, converts 3 component RGB value to a N sample
	 */
    virtual RtVoid colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;

	//! Set the relative detail
	/*! Scales the result of all level of detail calculations
	 *  @param relativedetail Factor used for scaling
	 */
    virtual RtVoid relativeDetail(RtFloat relativedetail) = 0;

	//! Sets a implementation-specific option
	/*! @param name Name of the option
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the option
	 */
    virtual RtVoid optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}
	
	/** @defgroup ricpp_lights Ri Lights
	 *  @ingroup ricpp_interface
	 *  @brief Light sources
	 *  @see IRiRoot::illuminate()
	 *  @{
	 */
	
	//! Sets a light source shader
	/*! Creates a non-area light, turns it on, and adds it to the current light source
	 *  list. Standard light sources are
	 *  RI_AMBIENTLIGHT, RI_POINTLIGHT, RI_DISTANTLIGHT and RI_SPOTLIGHT
	 *  @param name Name of the light source
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the light source
	 *  @return A light source handle to reference the light source in IRi::illuminate
	 */
    virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets an area light source shader
	/*! Creates an area light and makes it the current area light source. Each subsequent
	 *  geometric prim itive is added to the list of durfaces that define the area light source.
	 *  @param name Name of the area light source
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list for the area light source
	 *  @return A light source handle to reference the light source in IRi::illuminate
	 */
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}
	
	/** @defgroup ricpp_attributes Ri Attributes
	 *  @ingroup ricpp_interface
	 *  @brief Attributes
	 *  @{
	 */
	
	//! Sets a implementation-specific attribute
	/*! @param name Name of the attribute
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current color
	/*! @param Cs The current color
	 */
	virtual RtVoid color(RtColor Cs) = 0;

	//! Sets the current opacity
	/*! @param Os The current opacity
	 */
	virtual RtVoid opacity(RtColor Os) = 0;

	//! Sets the current surface shader
	/*! @param name Name of the surface shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current atmosphere shader
	/*! @param name Name of the atmosphere shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current interior shader
	/*! @param name Name of the interior shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current exterior shader
	/*! @param name Name of the exterior shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/* @brief Sets the current volume shader.
	 *  @param name Name of the volume shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	// virtual RtVoid volumeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Turns a specific light source on or off
	/*! @param light Handle that references a light source (area or non-area light source)
	 *  @param onoff RI_TRUE turns a light source on, RI_FALSE turns it off.
	 *  @see ricpp_lights
	 */
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) = 0;

	//! Sets the current displacement shader
	/*! @param name Name of the displacement shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	//! Sets the current set of texture coordinates to the values passed as arguments.
	/*! Declares a projection from the unit square [(0,0), (1,0), (0,1), (1,1)] in
	 *  u,v parameter space to quadrilateral [(s1,t1), (s2,t2), (s3,t3), (s4,t4)]
	 *  @param s1 'upper left' texture coordinate for s
	 *  @param t1 'upper left' texture coordinate for t
	 *  @param s2 'upper right' texture coordinate for s
	 *  @param t2 'upper right' texture coordinate for t
	 *  @param s3 'lower left' texture coordinate for s
	 *  @param t3 'lower left' texture coordinate for t
	 *  @param s4 'lower right' texture coordinate for s
	 *  @param t4 'lower right' texture coordinate for t
	 */
	virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;

	//! Sets the current shading rate to size
	/*! Controls the frequency of shading a surface.
	 *  @param size The new current shading rate (area in pixels)
	 *  @see shadingInterpolation()
	 */
    virtual RtVoid shadingRate(RtFloat size) = 0;

	//! Sets the shading interpolation type
	/*! Controls how values are interpolated between shading samples
	 *  (usually across a polygon).
	 *  @param type Type of the shading interpolation, RI_SMOOTH or RI_CONSTANT
	 *  @see shadingRate()
	 */
	virtual RtVoid shadingInterpolation(RtToken type) = 0;

	//! Controls rendering as a matte, can be either on or off.
	/*! @param onoff RI_TRUE subsequent primitives are matte objects,
	 *                   RI_FALSE subsequent primitives are rendered normal.
	 */
    virtual RtVoid matte(RtBoolean onoff) = 0;

	//! Sets the current bounding box to bound
	/*! @param bound The bounding box, subsequent primitives are within this bound
	 */
	virtual RtVoid bound(RtBound bound) = 0;

	//! Sets the current level of detail to the area of the bounding box in raster coordinate system
	/*! @param bound The bounding box to calculate the level of detail
	 */
	virtual RtVoid detail(RtBound bound) = 0;

	//! @brief Sets the current detail range.
	/*! The detail range is taken for subsequent primitives to the next call of
     *  detailRange() or the next attributeEnd() or worldEnd()
	 *  @param minvis model is not used for values smaller than this level of detail (LOD)
	 *  @param lowtran model is most important for this LOD (start), always present for values bigger than this and smaller than uptran
	 *  @param uptran model is most important for this LOD (end), always present for values smaller than this and bigger than lowtran
	 *  @param maxvis model is not used for values bigger than this LOD
	 */
	virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;

	//! @brief Adds the criterion for approximating the surface primitives.
	/*! [QRM] Spec is used here.
	 *  @param type Type of criterion for approximation, RI_TESSELATION, RI_DEVIATION, RI_FLATNESS, RI_MOTIONFACTOR
	 *  @param value metric for the criterion
	 */
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value) = 0;

	//! @brief Sets the type of the geometric representation.
	/*! Used to represent the primitives (added, was QRM)
	 *  @param type Type of representation, RI_POINTS, RI_LINES, RI_PRIMITIVE
	 */
	virtual RtVoid geometricRepresentation(RtToken type) = 0;

	//! @brief Sets the current orientation.
	/*! The orientation can be set expicitly to be either left-handed or right-handed or
	 *  set to the inside or the outside (the side is then rendered if IRi::sides(1))
	 *  @param anOrientation The new orientation,
	 *                     RI_LH, TRi_RH, RI_INSIDE or RI_OUTSIDE
	 *  @see reverseOrientation()
	 */
	virtual RtVoid orientation(RtToken anOrientation) = 0;

	//! @brief Flips current orientation, left-handed becomes right-handed and vice versa.
	/*! @see reverseOrientation()
	 */
	virtual RtVoid reverseOrientation(void) = 0;

	//! @brief Sets the number of sides of a surface being rendered.
    /*! If sides is 2, subsequent surfaces are considerd two-sided and both the inside
	 *  and the outside of a surface will be visible.
	 *  @param nsides Visible sides of a surface (0 <= nsides <= 2)
	 */
	virtual RtVoid sides(RtInt nsides) = 0;

	//! @brief Sets the current basis matrices used by bicubic patches.
	/*! @param ubasis The u-basis matrix
	 *  @param ustep The step size in u direction (bicubic patch meshes)
	 *  @param vbasis The v-basis matrix
	 *  @param vstep The step size in v direction (bicubic patch meshes)
	 *  @see ricpp_steps, ricpp_basis
	 */
    virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;

	/** @brief Sets the current trim curve.
	 *
	 *  The current trim curve consists of NURB curves in homogeneous parameter space,
	 *  and is used to trim IRi::nuPatch().
	 *  @param nloops Number of curve loops (curves with holes)
	 *  @param ncurves Number of curves per loop, 
	 *                 the first curve is the outline, subsequent are holes,
	 *                 size of the array is nloops,
	 *                 the sum of all elements in this array is the number of curves
	 *  @param order   The order of all curves
	 *  @param knot    The concatenated knot vectors of all curves
	 *  @param amin    The minimal parametric value for each curve
	 *  @param amax    The maximal parametric value for each curve
	 *  @param n       The number of control points for each curve
	 *  @param u       The u coordinates of the control points
	 *  @param v       The v coordinates of the control points
	 *  @param w       The w coordinates of the control points
	 */
    virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;
	//@}

	
	/** @defgroup ricpp_transforms Ri Transformations
	 *  @ingroup ricpp_interface
	 *  @brief Transformations
     *
	 * Transformations of the cartesian coordinate system
	 * Transformations modify the composite transformation matrix (CTM),
	 * concatenation is done by premultiplying a matrix to the CTM.
	 * @{
	 */

	/** @brief Sets the CTM to the identity matrix.
	 */
	virtual RtVoid identity(void) = 0;

	/** @brief Sets the CTM to transform
	 *  @param aTransform The new transformation matrix
	 */
	virtual RtVoid transform(RtMatrix aTransform) = 0;

	/** @brief Concatenates transform onto the CTM
	 *  @param aTransform CTM is premultiplied by transform
	 */
	virtual RtVoid concatTransform(RtMatrix aTransform) = 0;

	/** @brief Concatenates a perspective transformation matrix onto the CTM
	 *  @param fov The field of view angle used to calculate the perspective transformation
	 */
	virtual RtVoid perspective(RtFloat fov) = 0;

	/** @brief Concatenates a translation onto the CTM
	 *  @param dx translation in direction x
	 *  @param dy translation in direction y
	 *  @param dz translation in direction z
	 */
	virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;

	/** @brief Concatenates a rotation onto the CTM
	 *
	 *  Rotates angle degrees around the axis vector (dx, dy, dz)
	 *
	 *  @param angle Degrees to rotate
	 *  @param dx x component of the rotation axis
	 *  @param dy y component of the rotation axis
	 *  @param dz z component of the rotation axis
	 */
	virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;

	/** @brief Concatenates a scaling onto the CTM
	 *
	 *  @param dx Scales in x direction
	 *  @param dy Scales in y direction
	 *  @param dz Scales in z direction
	 */
	virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	
	/** @brief Concatenates a skew onto the CTM
	 *
	 * Skews angle degrees on the plane defined by the vectors (dx1, dy1, dz1) and (dx2, dy2, dz2)
	 *
	 * @param angle degrees to skew
	 * @param dx1 x coordinate of the first vector for the plane
	 * @param dy1 y coordinate of the first vector for the plane
	 * @param dz1 z coordinate of the first vector for the plane
	 * @param dx2 x coordinate of the second vector for the plane
	 * @param dy2 y coordinate of the second vector for the plane
	 * @param dz2 z coordinate of the second vector for the plane
	 */
    virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;

	/** @brief Sets a deformation shader
	 *  @param name Name of the deformation shader
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/* @brief Marks the CTM in the scope of a block
	 *
	 * RenderMan 11.5.2, like coordinate system, difference: stacked with attribute block
	 * and above (not xform block)
	 *
	 * @param space Name for the CTM of the coordinate system
	 * @see coordinateSystem()
	 */
	virtual RtVoid scopedCoordinateSystem(RtToken space) = 0;

	/** @brief Marks the CTM
	 *
	 *  Marks the coordinate system defined by the current transformation with the name 'space' and saves it.
	 *
	 *  @param space Name for the CTM, the CTM can be referenced by space later
	 */
	virtual RtVoid coordinateSystem(RtToken space) = 0;

	/** @brief Assigns the matrix previously marked by space (with IRi::coordinateSystem) to the CTM
	 *  @param space Name of the matrix that becomes the new CTM
	 */
	virtual RtVoid coordSysTransform(RtToken space) = 0; /* New 3.2 */

	/** @brief Transform points
	 *
	 *  Transforms an array of points from the coordinate system fromspace to the coordinatesystem tospace.
	 *
	 *  @param fromspace Name of the coordinate system from which points are transformed
	 *  @param tospace Name of the coordinate system to which points are transformed
	 *  @param npoints Number of points in @a points
	 *  @param points The points that are to be transformed, the values are modified
	 *  @return Points, containing the modified points, NULL if an error orccured
	 */
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;
	//@}

	
	/** @defgroup ricpp_prims Ri Graphics primitives
	 *  @ingroup ricpp_interface
	 *  @brief Graphics primitives
	 *  @{
	 */

	/** @defgroup ricpp_polygons Ri Polygons
	 *  @ingroup ricpp_prims
	 *  @brief Polygons
	 *  @{
	 */

	/** @brief Requests a single closed planar convex polygon
	 *
	 *  nvertices is the number of vertices in a single closed planar convex polygon.
	 *  The parameterlist is a list of token-array pairs where each token
	 *  is one of the standard geometric primitive variable or a variable
	 *  which has been defined by RiCPP::IRi::declare(), at least RI_P has to occur.
	 *
	 *  @param nvertices Number of verticces
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a single general concave planar polygon with holes
	 *  @param nloops number of loops defining the general polygon
	 *  @param nverts Number of vertices, the first is the outline, followed by holes
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a mesh of convex planar polygons with shared vertices
	 *
	 *  @param npolys number of polygons
	 *  @param nverts Number of vertices per polygon
	 *  @param verts indices of the vertices
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a mesh of general concave planar polygons with holes and shared vertices
	 *  @param npolys number of polygons
	 *  @param nloops number of loops defining the general polygons
	 *  @param nverts Number of vertices per polygon
	 *  @param verts indices of the vertices
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	/** @defgroup ricpp_patches Ri Patches
	 *  @ingroup ricpp_prims
	 *  @brief Patches
	 *  @{
	 */

	/** @brief Requests a single bilinear or bicubic patch
	 *  @param type Either RI_BILINEAR or RI_BICUBIC
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a bilinear or bicubic patch mesh
	 *  @param type Either RI_BILINEAR or RI_BICUBIC
	 *  @param nu patches in u direction
	 *  @param uwrap periodic (RI_PERIODIC) or not periodic (RI_UNPERIODIC) in u direction
	 *  @param nv patches in v direction
	 *  @param vwrap periodic (RI_PERIODIC) or not periodic (RI_UNPERIODIC) in v direction
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a single tensor product rational or polynomal non-uniform B-spline (NURB) surface patch
	 *  @param nu     Number of knots in u direction
	 *  @param uorder The order of the nu-patch in u direction
	 *  @param uknot  The knot vector for the u direction
	 *  @param umin   Minimal parameter value for u
	 *  @param umax   Maximal parameter value for u
	 *  @param nv     Number of knots in v direction
	 *  @param vorder The order of the nu-patch in u direction
	 *  @param vknot  The knot vector for the v direction
	 *  @param vmin   Minimal parameter value for v
	 *  @param vmax   Maximal parameter value for v
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}


	/** @defgroup ricpp_subdiv Ri Subdivision surfaces
	 *  @ingroup ricpp_prims
	 *  @brief Subdivision Surfaces
	 *  @{
	 */

	/** @brief Request a subdivison surface mesh
	 *  @param scheme Scheme of the subdivion mesh, currently RI_CATMULL_CLARK only
	 *  @param nfaces Number of initial faces
	 *  @param nvertices Number of the vertices per face
	 *  @param vertices Indices of the vertices
	 *  @param ntags Number of tags
	 *  @param tags The tags
	 *  @param nargs Number of the arguments of a tag, number of intargs followed by the number of the floatargs
	 *  @param intargs Integer arguments for the tags
	 *  @param floatargs Float arguments for the tags
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	/** @brief Request a hierarchical subdivison surface mesh.
	 *
	 *  see Application note #41, November 2004
	 *  @param scheme Scheme of the subdivion mesh, currently RI_CATMULL_CLARK only
	 *  @param nfaces Number of initial faces
	 *  @param nvertices Number of the vertices per face
	 *  @param vertices Indices of the vertices
	 *  @param ntags Number of tags
	 *  @param tags The tags
	 *  @param nargs Number of the arguments of a tag, number of intargs followed by the number of the floatargs
	 *  @param intargs Integer arguments for the tags
	 *  @param floatargs Float arguments for the tags
	 *  @param stringargs String arguments for edits
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}


	/** @defgroup ricpp_quadrics Ri Quadrics
	 *  @ingroup ricpp_prims
	 *  @brief Quadrics, created by sweeping a curve around z, thetamax is given in degrees
	 *  @{
	 */

	/** @brief Requests a sphere surface
	 *
	 *  The sphere is created by rotating an arc in the +x,z half plane, with center at O, around the z axis
	 *  @param radius Radius of the sphere
	 *  @param zmin Minimal z value of the sphere
	 *  @param zmax Minimal z value of the sphere
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a conic surface
	 *
	 *  The cone is created by rotating a line that hits the x (or is completely on x) and the z axis in the +x,+z quater around the z axis
	 *  @param height Height of the cone
	 *  @param radius Radius of the cone base
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a cylindric surface
	 *
	 *  The cylinder is created by rotating a line parallel to x axis in the +x,+z quater around the z axis
	 *  @param radius Radius of the cylinder
	 *  @param zmin
	 *  @param zmin Minimal z value of the cylinder
	 *  @param zmax Minimal z value of the cylinder
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a hyperbolic surface
	 *
	 *  The hyperboloid is created by rotating a line around the z axis
	 *
	 *  @param point1 Startpoint of the line
	 *  @param point2 Endpoint of the line
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a parabolic surface
	 *  The hyperboloid is created by rotating a parabolic curve in the +x,+z quater around the z axis
	 *  @param rmax radius at zmax (maximal radius)
	 *  @param zmin minimal z value
	 *  @param zmax maximal z value
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a flat disk, same as a cone with height == 0
	 *
	 *  The disk is created by rotating a line on parallel to the x axsis (on the positive side) that hits the z axis around the z axis
	 *
	 *  @param height translation in z
	 *  @param radius Radius of the disk
	 *  @param thetamax Rotation angle
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Requests a torodial surface
	 *
	 *  Creates first an arc on the +x,z plane using minorrad, phimin, phimax
	 *  with center at (majorad, 0, 0) and then a torus by rotating
	 *  this arc thetmax degrees around z
	 *
	 *  @param majorrad Distance between center of the torus and center of the arc being swwept
	 *  @param minorrad Radius of the arc
	 *  @param phimin startangle of the arc
	 *  @param phimax endangle of the arc
	 *  @param thetamax Rotation angle around z
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	/** @defgroup ricpp_2d Ri Points and curves
	 *  @ingroup ricpp_prims
	 *  @brief Point and curve primitives
	 *  @{
	 */
	
	/** @brief Creates a point cloud
	 *  @param npts Number of points
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */

	/** @brief Creates linear or cubic curves
	 *  @param type Type of the curves RI_LINEAR or RI_CUBIC
	 *  @param ncurves Number of curves
	 *  @param nverts number of the vertices of the curves
	 *  @param wrap RI_PERIODIC (curve wraps) or RI_NONPERIODIC
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}

	/** @defgroup ricpp_blobby Ri Blobby implicit surfaces
	 *  @ingroup ricpp_prims
	 *  @brief Blobby implicit surfaces
	 *  @{
	 */

	/** @brief Creates implicit surfaces
	 *
	 *  Blobby surfaces may be composed of spherical and sausage-like line-segment
	 *  primitives with extremly flexible control over blending. The surface type
	 *  also provides for repulsion to avoid intersection with irregular ground
	 *  planes represented by depth maps, s.a. [RISPEC3.2].
	 *
	 * @param nleaf Number of primitive blobs
	 * @param ncode Size of the code array
	 * @param code Code array, s.a. [RISPEC3.2] for the opcodes
	 * @param nflt Size of the flt array
	 * @param flt The float operands for the code
	 * @param nstr Size of the str array
	 * @param str The string operants for the code
	 * @param n Number of tokens
	 * @param tokens Tokens for additional parameter list
	 * @param params Value pointer for additional parameter list
	 * @see ricpp_blobbyconst, ricpp_blobbyop
	 */
    virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}

	/** @defgroup ricpp_proc Ri Procedural primitives
	 *  @ingroup ricpp_prims
	 *  @brief Procedural primitives
	 *  @{
	 */

	/*! @brief Calls a procedural primitive
	 * @param data
	 * @param bound
	 * @param subdivfunc
	 * @param freefunc
	 */
	virtual RtVoid procedural(RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc) = 0;
	//@}

	/** @defgroup ricpp_general Ri General objects
	 *  @ingroup ricpp_prims
     *  General objects
	 *  @{
	 */

	/** @brief Generates a predefined geometry, RI_TEAPOT is the only predifined one
	 *  @param type type of the geometry (RI_TEAPOT)
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	//@}

	/** @defgroup ricpp_texture Ri texture maps
	 *  @ingroup ricpp_interface
	 *  @brief Map-making, functions to create texture map from (rendered) images
	 *  @{
	 */
 
	/** @brief Creates a texture map.
	 *  @param pic Filename of an image (input).
	 *  @param tex Filename for the texture map (output).
	 *  @param swrap Periodic in s direction.
	 *  @param twrap Periodic in t direction.
	 *  @param filterfunc Pixel filter
	 *  @param swidth Width of the filter in s direction
	 *  @param twidth Width of the filter in t direction
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a bump map
	 *  @param pic Filename of an image (input).
	 *  @param tex Filename for the bump map (output).
	 *  @param swrap Periodic in s direction.
	 *  @param twrap Periodic in t direction.
	 *  @param filterfunc Pixel filter
	 *  @param swidth Width of the filter in s direction
	 *  @param twidth Width of the filter in t direction
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a lattitude/longitude environment map
	 *  @param pic Filename of an image (input).
	 *  @param tex Filename for the environment map (output).
	 *  @param filterfunc Pixel filter
	 *  @param swidth Width of the filter in s direction
	 *  @param twidth Width of the filter in t direction
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a cube face environment map, s.a. [RISPEC3.2]
	 *  @param px Filename input image, viewed from axis: forward +X, Up +Y, Right -Z
	 *  @param nx Filename input image, viewed from axis: forward -X, Up +Y, Right +Z
	 *  @param py Filename input image, viewed from axis: forward +Y, Up -Z, Right +X
	 *  @param ny Filename input image, viewed from axis: forward -Y, Up +Z, Right +X
	 *  @param pz Filename input image, viewed from axis: forward +Z, Up +Y, Right +X
	 *  @param nz Filename input image, viewed from axis: forward -Z, Up +Y, Right -X
	 *  @param tex Filename for the environment map (output).
	 *  @param fov Field of view to create the environment map.
	 *  @param filterfunc Pixel filter
	 *  @param swidth Width of the filter in s direction
	 *  @param twidth Width of the filter in t direction
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a shadow map.
	 *
	 *  @param pic Filename of an image (input).
	 *  @param tex Filename for the shadow map (output).
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/** @brief Creates a 3D texture map.
	 *  Can be used by the texture3D function of the Shading Language.
	 *
	 *  @param nNames Number of poinbt clouds
	 *  @param ptcnames Pointer to filenames of the point clouds
	 *  @param bkmname Filename for the 3d texture map (output).
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
    virtual RtVoid makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	//@}

	/** @defgroup ricpp_archive Ri RIB archives
	 *  @ingroup ricpp_interface
	 *  @brief Archive files
	 *  @{
	 */

	/** @brief Prints into a RIB file
	 *  @param type Type of Record RI_COMMENT (\#text), RI_STRUCTURE (\#\#text), RI_VERBATIM (text)
	 *  @param line Error description
	 */
	virtual RtVoid archiveRecordV(RtToken type, RtString line) = 0; /* Added, e.g. line is the formatted line from archive record */

	/** @brief Reads a RIB archive from a file and 'renders' its content by calling a delegate 
	 *  @param name Name of the RIB file
	 *  @param callback Called for each archive record
	 *  @param n Number of tokens
	 *  @param tokens Tokens for additional parameter list
	 *  @param params Value pointer for additional parameter list
	 */
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0; /* New 3.2 */
	//@}

	/*  @defgroup ricpp_conditional Ri RIB conditional expressions
	 *  @ingroup ricpp_interface
	 *  @brief Conditional expression for RIB
	 *
	 *  Changed the names of the procedures elseif and else because else is a keyword in c/c++
	 *  @{
	 */
	virtual RtVoid ifBegin(RtString expr) = 0;
	virtual RtVoid elseIfBegin(RtString expr) = 0; // was RiElseIf (renamed to match elseBegin()
	virtual RtVoid elseBegin(void) = 0; // was RiElse, else is a keyword ... moan, may be a reason for uppercase method names Else()
	virtual RtVoid ifEnd(void) = 0;
	//@}

	//@}
}; // IRiRoot

// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------

/** @brief RenderMan Interface
 *
 * Part 2 of the as (nearly) pure virtual C++ class, extents IRiRoot with the ellipsis (...) functions
 * with variable parameters.
 * @see IRiRoot, ricpp_filter, ricpp_stdprocs, ricpp_stderrors ricpp_interface
 */
class IRi : public IRiRoot {
public:
	IRi();

	/** @brief The virtual destructor
	 */
	virtual ~IRi();

	/** @defgroup ricpp_filter Ri pixel filters
	 *
	 *  @brief Standard build-in filter functions, part 2
	 *  @see IRiRoot
	 */
	//@{
	virtual const IFilterFunc &boxFilter() const = 0;
	virtual const IFilterFunc &catmullRomFilter() const = 0;
	virtual const IFilterFunc &gaussianFilter() const = 0;
	virtual const IFilterFunc &sincFilter() const = 0;
	virtual const IFilterFunc &triangleFilter() const = 0;
	//@}

	/** @defgroup ricpp_stdprocs Ri standard procedural primitives
	 * @brief Standard build-in procedural primitives
	 */
	//@{
	virtual ISubdivFunc &procDelayedReadArchive() const = 0;
	virtual ISubdivFunc &procRunProgram() const = 0;
	virtual ISubdivFunc &procDynamicLoad() const = 0;
	virtual IFreeFunc &procFree() const = 0;
	//@}

	/** @defgroup ricpp_stderrors Ri error handlers
	 * @brief Standard build-in error handler
	 */
	//@{
	virtual const IErrorHandler &errorAbort() const = 0;
	virtual const IErrorHandler &errorIgnore() const = 0;
	virtual const IErrorHandler &errorPrint() const = 0;
	//@}

	/** @addtogroup ricpp_interface
	 * @{
	 */

	/*  @addtogroup ricpp_resource
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/*  @brief Resource handle
	 *  @see IRiRoot::resourceV()
	 */
	virtual RtVoid resource(RtToken handle, RtToken type, RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @addtogroup ricpp_other
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/** @brief Controls the backend renderer or the frontend.
	 *
	 *  @see IRiRoot::controlV()
	 *
	 *  @param name Name of the control request
	 *  @param token First token of RI_NULL terminated token-value list
	 */
    virtual RtVoid control(RtToken name, RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @defgroup ricpp_contexts Ri context handlers
	 *  @ingroup ricpp_interface
	 *  @{
	 */
	/** @brief Gets the current context
	 *  @return The current handle
	 */
	virtual RtContextHandle getContext(void) = 0;

	/** @brief Context switching, sets a new active context
	 *  @param handle Active context, handle is a context, previously returned by getContext()
	 */
	virtual RtVoid context(RtContextHandle handle) = 0;
	//@}


	/** @addtogroup ricpp_modes
	 *  @ingroup ricpp_interface
	 *  @{
	 */
	/** @brief Starts an archive in memory
	 * @see IRiRoot::archiveBeginV()
	 */
	virtual RtArchiveHandle archiveBegin(RtToken name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Starts a new renderer
	 *
	 * Added a parameterlist (RtToken @a token, ...) and as the return type,
	 * a context handle (similar to [QRM])
	 * @see IRiRoot::beginV()
	 */
	virtual RtContextHandle begin(RtString name, RtToken token = RI_NULL, ...) = 0;

	//! Starts a motion block for a moving primitive
	/*! @param N     Number of RtFloat samples (length of times)
	 *  @see IRiRoot::motionBeginV()
	 */
    virtual RtVoid motionBegin(RtInt N, ...) = 0;
	//@}

	/** @addtogroup ricpp_options
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/** @brief Projection type
	 *  @see IRiRoot::projectionV()
	 */
    virtual RtVoid projection(RtToken name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Imager function
	 *  @see IRiRoot::imagerV()
	 */
    virtual RtVoid imager(RtString name, RtToken token = RI_NULL, ...) = 0;

	/*  @brief Defines a new display channel for display()
	 *  @see displayChannelV()
	 */
	virtual RtVoid displayChannel(RtString channel, RtToken token = RI_NULL, ...) = 0;

	/** @brief Choose a display
	 *  @see IRiRoot::displayV()
	 */
	virtual RtVoid display(RtString name, RtToken type, RtString mode, RtToken token = RI_NULL, ...) = 0;

	/** @brief Choose the hidden surface algorithm
	 *  @see IRiRoot::hiderV()
	 */
	virtual RtVoid hider(RtToken type, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets a implementation-specific option
	 *  @see IRiRoot::optionV()
	 */
    virtual RtVoid option(RtToken name, RtToken token = RI_NULL, ...) = 0;
	//@}
	
	/** @addtogroup ricpp_lights
	 *  @ingroup ricpp_interface
	 *  @{
	 */
	
	/** @brief Sets a light source shader
	 *  @see IRiRoot::lightSourceV()
	 */
    virtual RtLightHandle lightSource(RtString name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets an area light source shader
	 *  @see IRiRoot::arealightSourceV()
	 */
	virtual RtLightHandle areaLightSource(RtString name, RtToken token = RI_NULL, ...) = 0;
	//@}
	

	/** @addtogroup ricpp_attributes
	 *  @ingroup ricpp_interface
	 *  @{
	 */
	
	/** @brief Sets a implementation-specific attribute
	 *  @see IRiRoot::attributeV
	 */
	virtual RtVoid attribute(RtToken name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets the current surface shader
	 *  @see IRiRoot::surfaceV()
	 */
	virtual RtVoid surface(RtString name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets the current atmosphere shader
	 *  @see IRiRoot::atmosphereV()
	 */
    virtual RtVoid atmosphere(RtString name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets the current interior shader
	 *  @see IRiRoot::interiorV()
	 */
	virtual RtVoid interior(RtString name, RtToken token = RI_NULL, ...) = 0;

	/** @brief Sets the current exterior shader
	 *  @see IRiRoot::exteriorV()
	 */
    virtual RtVoid exterior(RtString name, RtToken token = RI_NULL, ...) = 0;

	/* @brief Sets the current volume shader.
	 * @see IRiRoot::volumeV()
	 */
	// virtual RtVoid volume(RtString name, RtToken token = RI_NULL, ...) = 0;

	/** @brief the current displacement shader
	 *  @see IRiRoot::displacementV()
	 */
	virtual RtVoid displacement(RtString name, RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @addtogroup ricpp_transforms
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/** @brief Sets a deformation shader
	 *  @see IRiRoot::deformationV()
	 */
	virtual RtVoid deformation(RtString name, RtToken token = RI_NULL, ...) = 0;
	//@}

	
	/** @addtogroup ricpp_prims
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/** @addtogroup ricpp_polygons
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Requests a single closed planar convex polygon
	 *  @see IRiRoot::polygonV()
	 */
	virtual RtVoid polygon(RtInt nvertices, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a single general concave planar polygon with holes
	 *  @see IRiRoot::generalPolygonV()
	 */
	virtual RtVoid generalPolygon(RtInt nloops, RtInt nverts[], RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a mesh of convex planar polygons with shared vertices
	 *  @see IRiRoot::pointsPolygonsV()
	 */
	virtual RtVoid pointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a mesh of general concave planar polygons with holes and shared vertices
	 *  @see IRiRoot::pointsGeneralPolygonsV()
	 */
	virtual RtVoid pointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @addtogroup ricpp_patches
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Requests a single bilinear or bicubic patch
	 *  @see IRiRoot::patchV()
	 */
    virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a bilinear or bicubic patch mesh
	 *  @see IRiRoot::patchMeshV()
	 */
    virtual RtVoid patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a single tensor product rational or polynomal non-uniform B-spline (NURB) surface patch
	 *  @see IRiRoot::nuPatchV()
	 */
	virtual RtVoid nuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, RtToken token = RI_NULL, ...) = 0;
	//@}


	/** @addtogroup ricpp_subdiv
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Requests a subdivison surface mesh
	 *  @see IRiRoot::subdivisionMeshV()
	 */
    virtual RtVoid subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	/** @brief Requests a hierarchical subdivison surface mesh
	 *  @see IRiRoot::hierarchicalSubdivisionMeshV()
	 */
    virtual RtVoid hierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtToken tokens = RI_NULL, ...) = 0;
	//@}


	/** @addtogroup ricpp_quadrics
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Requests a sphere surface
	 *  @see IRiRoot::sphereV()
	 */
	virtual RtVoid sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a conic surface
	 *  @see IRiRoot::coneV()
	 */
	virtual RtVoid cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a cylindric surface
	 *  @see IRiRoot::cylinderV()
	 */
	virtual RtVoid cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a hyperbolic surface
	 *  @see IRiRoot::hyperboloidV()
	 */
	virtual RtVoid hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a parabolic surface
	 *  @see IRiRoot::paraboloidV()
	 */
	virtual RtVoid paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a flat disk, same as a cone with height == 0
	 *  @see IRiRoot::diskV()
	 */
	virtual RtVoid disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;

	/** @brief Requests a torodial surface
	 *  @see IRiRoot::torusV()
	 */
	virtual RtVoid torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @addtogroup ricpp_2d
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Creates a point cloud
	 *  @see IRiRoot::pointsV()
	 */
	virtual RtVoid points(RtInt npts, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */

	/** @brief Creates linear or cubic curves
	 *  @see IRiRoot::curvesV()
	 */
    virtual RtVoid curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */
	//@}

	/** @addtogroup ricpp_blobby
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Creates implicit surfaces
	 *  @see IRiRoot::blobbyV()
	 */
	virtual RtVoid blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token = RI_NULL, ...) = 0; /* New 3.2 */
	//@}

	/** @addtogroup ricpp_general
	 *  @ingroup ricpp_prims
	 *  @{
	 */

	/** @brief Generates a predefined geometry,
	 *  @see IRiRoot::geometryV()
	 */
    virtual RtVoid geometry(RtToken type, RtToken token = RI_NULL, ...) = 0;
	//@}

	//@}

	/** @addtogroup ricpp_texture
	 *  @ingroup ricpp_interface
	 *  @{
	 */
 
	/** @brief Creates a texture map
	 *  @see IRiRoot::makeTextureV()
	 */
	virtual RtVoid makeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a bump map
	 *  @see IRiRoot::makeBumpV()
	 */
    virtual RtVoid makeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a lattitude/longitude environment map
	 *  @see IRiRoot::makeLatLongEnvironmentV()
	 */
    virtual RtVoid makeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a cube face environment map
	 *  @see IRiRoot::makeCubeFaceEnvironmentV()
	 */
    virtual RtVoid makeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a shadow map
	 *  @see IRiRoot::makeShadowV()
	 */
    virtual RtVoid makeShadow(RtString pic, RtString tex, RtToken token = RI_NULL, ...) = 0;

	/** @brief Creates a 3D texture map.
	 *  @see IRiRoot::makeBrickMapV()
	 */
    virtual RtVoid makeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, RtToken token = RI_NULL, ...) = 0;
	//@}

	/** @addtogroup ricpp_archive
	 *  @ingroup ricpp_interface
	 *  @{
	 */

	/** @brief Prints into a RIB file
	 *  @param type Type of Record RI_COMMENT (\#text), RI_STRUCTURE (\#\#text), RI_VERBATIM (text)
	 *  @param format Format string like for printf()
	 *  @see IRiRoot::archiveRecordV()
	 */
    virtual RtVoid archiveRecord(RtToken type, RtString format, ...) = 0; /* New 3.2 */

	/** @brief Reads a RIB archive from a file and renders its content
	 *  @see IRiRoot::archiveRecordV()
	 */
    virtual RtVoid readArchive(RtString name, const IArchiveCallback *callback, RtToken token = RI_NULL, ...) = 0; /* New 3.2 */
	//@}

	//@}
}; // IRi
} // namespace RiCPP
#endif // _RICPP_RICPP_RICPP_H
