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

/** @file ribtool.cpp
 *  @author Andreas Pidde (andreas@pidde.de).
 *  @brief RIBtool, a shell tool to do some RIB processing.

Attention: This is only a roadmap (some ideas), a minor number
of options work at the moment! A list of working options can
be found in printUsage(). I will implement the options one
by one to test the behaviour of the RiCPP RIB parsing and
writing capabilities, occasionally.

RIBtool is a little shell tool, which parses, processes and
writes RenderMan(R) Interface Byte streams (RIB files).
For example, jobs for RIBtool can be expanding archives and
converting them from ASCII to binary representation or
vice versa. To execute RIBtool, put the executable together
with the RiCPP dynlibs into the same directory. You can use
symbolic links for RIBtool.

@verbatim
Synopsis: ribtool options files...
@endverbatim

If you run RIBtool, the command line is first searched for an optional
output file given by the option -o filename. The first file found will
be taken, default value is standard output. The command line gets then
read from the beginning left to right ignoring the -o options.
The found options control the way the following RIB files are processed.
The options start with a - (disable) or + (enable) followed
directly by one character, the command, that identifies the operation
that has to be done. Optionally the command can be followed directly
by one or more characters as arguments. The output is the concatenation
of all processed input files.

You have to separate options from filenames with a blank. A single
'-' followed by no character stands for standard input. Options
do not need to be separated, because the - and the + are used as
boundary. Filenames can contain simple wildcards (* and ?).
Files (not stdin and stdout at the moment) could be compressed
with zlib. You can concatenate filenames and the stdin symbol
with a '+', like lightsource.rib+objects.rib+world.rib to parse
them within a single parsing context (to do: "append" parameter of
ReadArchive) to use the handles defined in one file within another.

RIB dynamic procedurals and RIB filters have to be linked against
the RiCPP library to work correctly.

- The option p (postpone), default +p-pf

Option p stands for postponing or defering special output,
e.g. -pa won't postpone archives. The requests between
ArchiveBegin and ArchiveEnd (archive definition) are cached
(not written) and will be written if a ReadArchive will be found.
In contrast, if an archive is postponed with +pa, the archive
definition is written as well as the appropriate ReadArchive
request, but not the contents. Later the renderer, reading the
processed RIB, will have to do the archive processing. If an
archive has been read with -pa, that means the archive definition
was not written, but the instantiation with the appropriate
ReadArchive was found while +pa was set, the archive will be
written anyway. Otherwise an error will occur, because the
definition was not written. The contents of the archive will
be written as well, if ArchiveBegin/ArchiveEnd (archive was written)
was found while +pa was active, but the appropriate ReadArchive
was found with a -pa set. Objects are processed in the same way.

The arguments can be juxtaposed. E.g. -pdafo disables the
defered output for dynamic procedurals, inline archives, RIB
files and objects. The RIB code gets printed when instantiated.
If no argument follows the p, everything is influenced. E.g.
-p has the same effect as -pafodryi or -pafopi. All command
options with arguments are handled in this way.

@verbatim
+p postpones
-p doesn't postpone

a Inline archives (ArchiveBegin, ArchiveEnd, ReadArchive)
f RIB files (ReadArchive)
o Objects (ObjectBegin, ObjectEnd, ObjectInstance)
p Shortcut for all procedurals "dry"
  (Procedural "DelayedReadArchive", "RunProgram", "DynamicLoad")
d delayed read procedural (Procedural "DelayedReadArchive")
r run program procedural (Procedural "RunProgram")
y dynamic procedural (Procedural "DynamicLoad" - dynamic
  procedurals work only if linked with RiCPP)
i If blocks (IfBegin, ElseIf, Else, IfEnd)
@endverbatim

- The option i (inhibit), default +i

The option i can be used to inhibit writing. This can be used
to read declarations that are used for further RIB reading
but shall not occur at output. There are no arguments.
For example: ribtool -i Declarations.rib +i -pdafo Objects.rib
can be a command line used to read Declarations.rib containing
the declarations of a special renderer and write the
expanded contents of Objects.rib to standard out.

@verbatim
-i stops output, only reads and interprets RIB
+i continues writing output
@endverbatim

- The option - (stdin)

A single - (followed by a blank) stands for reading from
standard input. Stdin can not be compressed at the moment.

Example command line to read from stdin and write to stdout:
ribtool - 

@verbatim
- Reading from standard input
@endverbatim

- The option o (output)

The option for file output can used as -o filename or +o filename.
-o won't overwrite existing files, +o will. You can add a number
0-9 for
GZIP compression, 0 is default compression no number stands
for no compression. filename should normally have the .rib
extension. If you use .rib.gz, the file will also be
compressed (with default compression, if no compression
value is given). Stdout as well as stdin can not be
compressed at the moment.

@verbatim
-o[0-9] filename Writes output to file, if file doe not exist
+o[0-9] filename Writes output to file, will overwrite an existing file
@endverbatim

- The option c (cull), default -c

Culls the geometry that is not visible inside the viewing volume.

-pdr+c-cp Culls geometry, procedurals (delayed read, run program)
are executed everytime (-pdr and -cp), but the geometries found
are culled (+c).

-pdr+c Culls geometry and procedurals having their bounding box
outside the viewing volume. Procedurals are not executed because
they are not visible.

@verbatim
+c Cull geometry (Sphere, Cone, Paraboloid, PatchMesh...);
-c Do not cull

p procedurals
g other geometries
@endverbatim

- The option s (simulate), default +s (must change RiCPP to do this)

This option is usefull only for postponed requests (and only affects
these requests therefore). It can happen that read archives influencing
the way the following RIB code is processed (e.g. user defined attributes
can influence the way an if expression evaluates). You can simulate the
processing of otherwise postponed RIB code. However, it can happen that
you don't want to simulate, e.g. because the RIB archive files are
not available, or the procedural is not executable  by RIBtool. The
arguments are the same as for the postpone option.

@verbatim
+s Simulates input
-s Doesn't simulate input

Arguments: see option p
@endverbatim

- The option k (skip), default -k

Skips writing comments, headers and version requests. At the moment,
always the version of the RIB generator (RiCPP ribwriter) is
written, not the version found. That's because the output is
written with the version of the generator, not the one of the
input.

@verbatim
+k Skips
-k Doesn't skip

h Header (structured comments '##' at the beginning of a file)
v Version
1 Uses only the first header and version
  (if no 'version 3.03' follows a header, one is generated)
c Comments (regular comments '#')
s Structured comments other than headers
@endverbatim

- The option f filtername (RIB filter)

Adds or removes a special RIB filter. RiCPP supports special DLLs,
that can be used as RIB filter. RIB filters can be Daisy-Chained.

@verbatim
+f filtername Adds a RIB filter
-f filtername Removes RIB filter
@endverbatim

- The option h (handles), default +h

Controls the way handles are processed (old style numeric or
newer RIB string handles, if found). Numeric handles will be
renumbered.

@verbatim
+h Uses string handles if found, numeric handles
   will stay numeric and will be renumbered.
-h Prints only numeric handles, even for string handles.
   The handles will be renumbered.
@endverbatim

- The option n (indent RIB archives), default +n4

+n indents the written RIB blocks by a number of blanks.
The +n0 and -n options are used to indicate to use no indent.

@verbatim
+n[0-9] Indents RIB output by blanks
-n Does not indent RIB output
@endverbatim

- The option b (binary RIB archives), default -b

Writes either binary (+b) or ASCII (-b) output

@verbatim
+b Binary output
-b ASCII output
@endverbatim

- The option x (Extract frames)

Extracts frames (using positive and negative lists). The
extraction string has the form /n,n-m,n-,-n/, e.g.
+x/-2,7,9-11,15-/-x/20/ extracts 1,2, 7, 9,10,11, 15,16,17,...
but not 20. The frames are written in the sequence they are
found. If only a postive list is given and all frames out of
the list have been found, RIBtool terminates without writing
the requests that may follow the last FrameEnd. The extracted
frames from FrameBegin to FrameEnd, as well as the RIB code
in front of, between, and possibly after the frames will be
written.

Use no x option to let RIBtool write all frames. -x can
also be used for extraction.

@verbatim
+x/list/ Extracts the frames given by the list. If
         the list is empty // or missing, no frames
		 will be written.
-x/list/ writes all frames except the ones
         given by the list, if the list is empty //
		 or missing, all frames will be written
@endverbatim

- The option v (RIB variable extraction while parsing option used) +v

For compatibility the option "rib" "string varsubst" can
be used to enable RIB variables. If you want to disable
this option and variable extraction done later, you can
use -v. Mind +v does not turn on the RIB variable extraction,
this is done only if the varsubst option is found. If RIB
variable extraction is turned off if-expressions are
still evaluated, but within an if-expression a variable ${id}
(curly brackets) is not extracted. However, $id and $(id)
(parenthesis) are not affected and are used for evaluation
indeed.

@verbatim
-v disables handling of varsubst option
+v enables handling of varsubst option
@endverbatim

- The option g (generate header)

For a larger header, a header-only RIB file
can be included.

+g without any further arguments writes simple header info:

@verbatim
##RenderMan RIB-Structure 1.1
version 3.03
@endverbatim

If you included a header, you can disable further
headers/version by the skip option +khv

The version line is written in general only once.

@verbatim
h Header string RIB file: ##RenderMan RIB-Structure 1.1
e Header for entity: ##RenderMan RIB-Structure 1.1 Entity
v Writes current version string of the ribwriter: version 3.03
@endverbatim

- The option m (motion blocks), default +e

Can be used to roll out motion blocks. Geometry is written
in enclosing attribute blocks. Instead of the
motion block with attributes and transformations, the first
command is written. Geometry is written multiple times with
the appropriate options/transforms set. Camera motion causes
the whole content of a world blocks written multiple times within
attribute blocks.

@verbatim
+e Motion blocks are written as usual, the renderer has to perform the motions
-e Motion blocks are rolled out
   t Just in front of geometry a opacity command is used to set
     the opacity/transparency. I used the letter t because of option o is
	 the important output option and typos can cause overwriting
	 of RIB-files unwantingly.
@endverbatim


- The option l (level of detail), default +l

@verbatim
+l Level of detail commands are written
-l The appropriate level of detail geometry is used
   t Uses transparency to blend models
@endverbatim

- The option r (select renderer), default ribwriter

@verbatim
-r               Selects the renderer (no name: ribwriter)
   "name"        name (like "|myrenderer -anstartoption")
+r token "value" Adds renderer specific options, value mus be written within ""
                 s/value{/value}/ for string values, / is the seperator
                 fvalue{,value}*  for float values
                 ivalue(,value)*  for integer values
@endverbatim

- The option e (cache (memory) RIB archives), default -e

Experimental, caches the archive files read by ReadArchive,
can lead wrong behaviour, because variables are evaluated
while parsing. This needs more considerations...

@verbatim
+e Caches RIB archives.
-e Doesn't cache RIB archives.
@endverbatim
*/


#include "ricpp/ricppbridge/ricppbridge.h"

using namespace RiCPP;

CRiCPPBridge ri;    ///< The bridge to the rendering context

RtInt yes = 1;      ///< Used as parmeter for IRi::control() (positive)
RtInt no = 0;       ///< Used as parmeter for IRi::control() (negative)
RtInt special = -1; ///< Used as parmeter for IRi::control() (special behaviour)


/** @brief Prints the usage of RIBtool.
 */
void printUsage()
{
	std::cout << "Parses, processes and outputs RenderMan(R) Interface Byte streams (RIB files)." << std::endl;
	std::cout << "RenderMan(R) is a registered trademark of Pixar." << std::endl;
	std::cout << std::endl;
	std::cout << "usage: ribtool [-o[0-9] outfile -p[afo] +p[afo] - filename]..." << std::endl;
	std::cout << "+o Sets outputfile, standard is stdout, the file will be overwritten" << std::endl;
	std::cout << "   0-9 gzip compression level, 0 is default compression" << std::endl;
	std::cout << "       omit for no compression" << std::endl;
	std::cout << "+p Enables postpone of" << std::endl;
	std::cout << "-p Disables postpone of" << std::endl;
	std::cout << "   a Inline archives" << std::endl;
	std::cout << "   f File (RIB) archives" << std::endl;
	std::cout << "   o RI objects" << std::endl;
	std::cout << "   p Procedurals (experimental)" << std::endl;
	std::cout << "- Reads from stdin" << std::endl;
	std::cout << "filename Reads from filename" << std::endl;
}


/** @brief Prints an error message.
 *  @param msg The message.
 */
void printError(const char *msg)
{
	if ( msg )
		std::cerr << "# *** Error: " << msg << std::endl;
}

/** @brief Option 'p' postpone.
 *  @param aSwitch '+' or '-'
 *  @param argument The argument character.
 */
void postpone(int aSwitch, int argument)
{
	assert ( aSwitch == '-' || aSwitch == '+' );

	RtInt *param = (aSwitch == '-') ? &no : &yes; // '-' means no, '+' means yes
	RtInt *paramFile = (aSwitch == '-') ? &no : &special; // The file itself should be read

	// Process the argument
	switch(argument) {
		case 0: // all
			ri.control("ribwriter", "postpone-inline-archives", param, RI_NULL);
			ri.control("ribwriter", "postpone-file-archives", paramFile, RI_NULL);
			ri.control("ribwriter", "postpone-objects", param, RI_NULL);
			ri.control("ribwriter", "postpone-procedurals", param, RI_NULL);
			break;

		case 'a': // Inline archives (ArchiveBegin, ArchiveEnd, ReadArchive)
			ri.control("ribwriter", "postpone-inline-archives", param, RI_NULL);
			break;

		case 'f': // RIB files (ReadArchive)
			ri.control("ribwriter", "postpone-file-archives", paramFile, RI_NULL);
			break;

		case 'o': // Objects (ObjectBegin, ObjectEnd, ObjectInstance)
			ri.control("ribwriter", "postpone-objects", param, RI_NULL);
			break;

		case 'p': // Procedurals (delayedReadArchive, RunProgramm, DynamicLoad)
			ri.control("ribwriter", "postpone-procedurals", param, RI_NULL);
			break;

		default: // not recognized
		{
			std::string msg = "Sorry, unrecogniced postpone argument ";
			msg += (char)argument;
			printError(msg.c_str());
		}
		break;
	}
}

/** @brief Process command.
 *  @retval i Input/Output of current argument index.
 *  @param argc number of arguments @a argv.
 *  @param argv The arguments that will be parsed.
 */
void command(int &i, int argc, char * const argv[])
{
	assert ( i < argc );

	const char *arg = argv[i];
	size_t len = arg ? strlen(arg) : 0;

	assert ( len > 1 );
	
	int cnt = 0;

	while ( arg[cnt] ) {

		char aSwitch = arg[cnt++];

		assert(aSwitch == '-' || aSwitch == '+' );

		if ( !aSwitch )
			return;

		char aCmd = arg[cnt++];

		switch ( aCmd ) {

			case 'o': // output (ignore)
			{
				if ( isdigit(arg[cnt]) )
					++cnt;
				if ( arg[cnt] ) {
					// filename in same argument
					return;
				}
				// filename in next argument
				++i;

				if ( aSwitch == '-' ) {
					printError("Sorry, -o is not implemented.");
				}
				return;
			}
			break;

			case 'p': // postpone
				{
				bool found = false;
				for ( ; arg[cnt] && arg[cnt] != '-' && arg[cnt] != '+'; ++cnt ) {
					found = true;
					postpone(aSwitch, arg[cnt]);
				}
				if ( !found ) {
					postpone(aSwitch, 0);
				}
			}
			break;

			default: // unknown
			{
				std::string msg = "Sorry, unrecogniced command sequence ";
				msg += arg;
				printError(msg.c_str());
			}
			return;

		}
	}
}

/** @brief The main funtion.
 *
 *  Description of ribtool @see ribtool.cpp
 *
 *  @param argc number of arguments @a argv
 *  @param argv The arbuments that will be parsed
 *  @return 0, if no error occured, 1, otherwise
 */
int main(int argc, char * const argv[])
{
	// CUri absUri, baseUri;
	RtInt compression = 0;

	// No arguments, print usage
	if ( argc < 2 ) {
		printUsage();
		return 1;
	}

	assert(argc > 1);

	bool optfound = false;        // Flag for: An option was found, here -o
	std::string outfilename = ""; // Container for output filename

	// Scan for the output filenname
	for ( int i = 1; i < argc; ++i ) {
		
		const char *arg = argv[i];
		size_t len = arg ? strlen(arg) : 0;

		if ( optfound ) {
			// -o was found
			if ( arg )
				outfilename = arg;
			break;
		}

		if ( len > 1 && arg[0] == '+' ) {
			if ( arg[1] == 'o' ) {
				// option for output found
				optfound = true;
				if ( len > 2 && isdigit(arg[2]) ) {
					compression = arg[2]-'0';
					++arg;
					--len;
					if ( compression == 0 )
						compression = -1;
				}
				if ( len > 2 ) {
					arg += 2;
					outfilename = arg;
					break;
				}
			}
		}
	}

	// Print errors
	ri.errorHandler(ri.errorPrint());
	
	const char *outfile = outfilename.empty() ? RI_NULL : outfilename.c_str();

	// Start the ribwriter - maybe integrate renderers to the options later
	ri.begin("ribwriter", RI_FILE, &outfile, "compress", &compression, RI_NULL );

	// Scan the options from left to right
	std::string filename;
	bool fileWasFound = false; // At least one filename was found

	for ( int i = 1; i < argc; ++i ) {
		const char *arg = argv[i];
		size_t len = arg ? strlen(arg) : 0;

		if ( !len )
			continue;

		// Commands start either with '-' or '+' followed by the command character
		if ( len > 1 && (arg[0]=='-' || arg[0]=='+') ) {
			command(i, argc, argv);
			continue;
		}

		// If it was not a command, a filename is assumed
		fileWasFound = true;

		if ( len == 1 && arg[0]=='-' ) {

			// Reads from standard input
			ri.readArchive(RI_NULL, 0, RI_NULL);

		} else {
			filename = noNullStr(argv[i]);

			if ( filename == outfilename ) {
				printError("Inputfile == outputfile");
				continue;
			}

			// Reads from the file
			ri.readArchive(filename.c_str(), 0, RI_NULL);

		}
	}

	// If no filename was found, process the standard input
	if ( !fileWasFound ) {
		ri.readArchive(RI_NULL, 0, RI_NULL);
	}

	// Closes the context
	ri.end();

	// and ends the program.
	return 0;
}

