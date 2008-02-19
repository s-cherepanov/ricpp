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
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Shell tool to do some rib processing.

@verbatim
Synopsis: ribtool options files...
@endverbatim

This is only a roadmap, only a minor number of options work at the moment.
I will implement the options one by one to test the behaviour of the
RiCPP RIB parsing and writing capabilities. Put executable together
with dynlibs into the same directory, you can use symlinks for ribtool.

Reads, interprets and processes binary and ASCII RIB files
and prints the result to standard output or an output file.
Jobs for processing can be for example expanding archives and
converting from ASCII to binary representation. The command
line is first searched for an optional output file
-o filename (standard is stdout, first file found is taken).
It is than read from the beginning left to right ignoring th -o.
Options control the way the following RIB files are processed.
The options start with - (disable) or + (enable) followed
directly by an character that identifies the operation,
optionaly followed directly by single characters as arguments. The
output is the concatenation of all input files.

You have to seperate options from filenames and the '-' (for standard
input). Options need not to be seperated, because the - and the +
are used as boundary. filenames can contain simple
wildcards (* and ?). Files (not stdin and stdout at the moment) can
be compressed with zlib.

Rib dynamic procedurals and ribfilters have to be compiled using the RiCPP
library.

The option p (postpone), default +p-pf

Option p for postpone to defer output, e.g. -pa won't postpone
archives. The requests between ArchiveBegin and ArchiveEnd
(archive definition) are cached (not written) and put out if
a ReadArchive is found. If an archive is posponed with +pa, the
archive definition is written as well as a ReadArchive request
for this archive, so later the renderer will have to do the archive
processing. If an archive was read with -pa, that means the
archive definition is not written, but the instanciation with
ReadArchive was found while +pa is set, the archive is written
anyway. It is also written if found +pa ArchiveBegin ArchiveEnd
(archive is written) +pa ReadArchive. Objects are processed the same
way.

The arguments can be juxtaposed. E.g. -pdafo disables the
defered output for dynamic procedurals, inline archives, RIB
files and objects. They are printed when they are instanciated.
If no arguments follow the p, everything is influenced. E.g.
-p is the same as -pafodryi or -pafopi All options with
arguments are handled this way.

@verbatim
+p postpones
-p doesn't postpone

a Inline archives (ArchiveBegin, ArchiveEnd, ReadArchive)
f RIB files (ReadArchive)
o Objects (ObjectBegin, ObjectEnd, ObjectInstance)
p Shortcut for all procedurals dry (Procedural "DelayedRead", "Run", "Dynamic")
d delayed read procedural (Procedural "DelayedRead")
r run program procedural (Procedural "Run")
y dynamic procedural (Procedural "Dynamic" works for RiCPP dynamic procedurals only)
i If blocks (IfBegin, ElseIf, Else, IfEnd)
@endverbatim

The option i (inhibit), default +i

The option i can be used to inhibit output. That can be used to read declarations
that are used for further RIB reading but shall not occur at output. There are no arguments.
E.g. -i Declarations.rib +i -pdafo Objects.rib

@verbatim
-i stops output, only reads and interprets RIB
+i continues output
@endverbatim

The option - (stdin)

A single - (followed by a blank) stands for reading from standard input. Stdin can not be
compressed at the moment (todo).

Read from stdin, write to stdout: ribtool - 

@verbatim
- Reading from standard input
@endverbatim

The option o (output)

Only as usual as -o filename. You can add a number 0-9 for ZIP compression,
0 is default compression no number for no compression. filename should
normally have the .rib extension. If you use .rib.gz, the
file will also be compressed (with default compression, if no compression
value is given). Stdout can not be
compressed at the moment (todo).

@verbatim
-o[0-9] filename Print output to file
@endverbatim

The option c (cull), default -c

Culls the geometry that is not visible in the viewing volume.

-pdr+c-cp Culls geometry, procedurals (delayed read, run program)
are executed and the geometries found are culled.

-pdr+c Culls geometry, procedurals having their bounding box
outside the viewing volume are not executed.

@verbatim
+c Cull geometry (Sphere, cone, ...);
-c Do not cull

p procedurals
g other geometries
@endverbatim

The option s (simulate), default +s (must change RiCPP to do this)

This option is usefull (and only affects therefore) only for
posponed requests. It can happen the read archives influencing
the way the following rib code is processed (e.g. user
defined attributes can influence the way an if expression
evaluates). You can simulate the
processing of otherwise posponed RIB code. However, it can happen
that you don't want to simulate
e.g. the RIB archive files are not available, the procedural is not
runable by RiCPP. The Arguments are the same as for the postpone option.

@verbatim
+s Simulates input
-s Doesn't simulate input

Arguments see option p
@endverbatim

The option k (skip), default -k

Skip comments, headers and version. At the moment, allways the version
of the RIB generator (RiCPP ribwriter) is written, not the version found.
That's because the output is written with the version of
the generator, not of the input (this behaviour will maybe be changed)

@verbatim
+k Skips
-k Doesn't skip

h Header (structured comments at the beginning of a file)
v Version
1 Uses only the first header and version (if no version 3.03 follows, one is generated)
c Comments (regular comments)
s Structured comments other than header
@endverbatim

The option f filtername (ribfilter)

Adds or removes a special ribfilter. RiCPP supports special DLL's that
can be used as RIB filter. Ribfilters can be Daisy-Chained.

@verbatim
+f filtername Adds a ribfilter
-f filtername Removes ribfilter
@endverbatim

The option h (handles), default +h

Controlls the way handles are processed (oldstyle numeric or Rib string handles if found).
Numeric handles will be renumbered.

@verbatim
+h Use string handles if found, numeric handles
   will stay numeric and get renumbered.
-h Print only numeric handles, even for string handles.
   The handles get renumbered.
@endverbatim

The option n (indent rib archives), default +n4

Indents the output Rib blocks by a number of blanks +n0 and -n is
for no indent.

@verbatim
+n[0-9] Indents Rib output by Blanks
-n Does not indent Rib output
@endverbatim

The option b (binary RIB archives), default -b

Writes either binary (+b) or ASCII (-b) output

@verbatim
+b Binary output
-b Ascii outpu
@endverbatim

The option m (cache (memory) rib archives), default -m

Experimental, caches the archive files read by ReadArchive,
can lead to errors. Needs more considerations...

@verbatim
+m Caches Rib archives.
-m Doesn't cache Rib archives.
@endverbatim
*/


#include "ricpp/ricppbridge/ricppbridge.h"

using namespace RiCPP;

CRiCPPBridge ri;

RtInt yes = 1;
RtInt no = 0;
RtInt special = -1;


/** @brief Prints the usage of ribtool.
 *  @todo print the usage
 */
void printUsage()
{
	std::cout << "Parses, processes and outputs RenderMan(R) Interface Bytstreams (RIB files)." << std::endl;
	std::cout << "RenderMan(R) is a registered trademark of Pixar." << std::endl;
	std::cout << std::endl;
	std::cout << "usage: ribtool [-o[0-9] outfile -p[afo] +p[afo] - filename]..." << std::endl;
	std::cout << "-o Sets outputfile, standard is stdout" << std::endl;
	std::cout << "   0-9 gzip compression level, 0 is default compression" << std::endl;
	std::cout << "       omit for no compression" << std::endl;
	std::cout << "+p Enables postpone of" << std::endl;
	std::cout << "-p Disables postpone of" << std::endl;
	std::cout << "   a Inline archives" << std::endl;
	std::cout << "   f File (RIB) archives" << std::endl;
	std::cout << "   o RI objects" << std::endl;
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

/** @brief Option 'p' postpone
 *  @param aSwitch '+' or '-'
 *  @param argument The argument character
 */
void postpone(int aSwitch, int argument)
{
	RtInt *param = (aSwitch == '-') ? &no : &yes;
	RtInt *paramFile = (aSwitch == '-') ? &no : &special; // The file itself should be read

	switch(argument) {
		case 0: // all
			ri.control("ribwriter", "postpone-inline-archives", param, RI_NULL);
			ri.control("ribwriter", "postpone-file-archives", paramFile, RI_NULL);
			ri.control("ribwriter", "postpone-objects", param, RI_NULL);
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

		default: {
			std::string msg = "Sorry, unrecogniced postpone argument ";
			msg += (char)argument;
			printError(msg.c_str());
		}
		break;
	}
}

/** @brief Process command.
 *  @retval i Input/Output of current argument index
 *  @param argc number of arguments @a argv
 *  @param argv The arguments that will be parsed
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

			case 'o': {
				// output (ignore)
				if ( isdigit(arg[cnt]) )
					++cnt;
				if ( arg[cnt] ) {
					// filename in same argument
					return;
				}
				// filename in next argument
				++i;
				return;
			}
			break;

			case 'p': {
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

			default: {
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
	RtInt compression = 0;

	if ( argc < 2 ) {
		printUsage();
		return 1;
	}

	// argc > 1

	bool ofound = false;
	std::string outfilename = "";

	for ( int i = 1; i < argc; ++i ) {
		const char *arg = argv[i];
		size_t len = arg ? strlen(arg) : 0;
		if ( ofound ) {
			if ( arg )
				outfilename = arg;
			break;
		}
		if ( len > 1 && arg[0] == '-' ) {
			if ( arg[1] == 'o' ) {
				ofound = true;
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

	ri.errorHandler(ri.errorPrint());
	CFilepathConverter::convertToInternal(outfilename);
	CFilepathConverter::convertToURL(outfilename);
	const char *outfile = outfilename.empty() ? RI_NULL : outfilename.c_str();
	ri.begin("ribwriter", RI_FILE, &outfile, "compress", &compression, RI_NULL );

	std::string filename;
	bool fileWasFound = false;
	for ( int i = 1; i < argc; ++i ) {
		const char *arg = argv[i];
		size_t len = arg ? strlen(arg) : 0;

		if ( !len )
			continue;

		if ( len > 1 && (arg[0]=='-' || arg[0]=='+') ) {
			command(i, argc, argv);
			continue;
		}

		fileWasFound = true;

		if ( len == 1 && arg[0]=='-' ) {

			ri.readArchive(RI_NULL, 0, RI_NULL);

		} else {

			filename = noNullStr(argv[i]);
			if ( filename == outfilename ) {
				printError("Inputfile == outputfile");
				continue;
			}
			CFilepathConverter::convertToInternal(filename);
			CFilepathConverter::convertToURL(filename);
			ri.readArchive(filename.c_str(), 0, RI_NULL);

		}
	}

	if ( !fileWasFound ) {
		ri.readArchive(RI_NULL, 0, RI_NULL);
	}

	ri.end();
	return 0;
}

