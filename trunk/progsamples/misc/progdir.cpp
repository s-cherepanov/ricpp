#include <sys/errno.h>
#include <sys/types.h>

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

// Can be small because space is allocated dynamically if PATH_MAX was to small
#ifndef PATH_MAX
#define PATH_MAX 2
#endif

std::string &getProgDir(std::string &prog)
{
	static std::string path = "";
	static bool isset = false;

	prog = "";

	if ( !isset ) {
		isset = true; // only try one time, path can be empty

		char *buf = new char[PATH_MAX+1];

		pid_t pid;
		char aString[128];
		ssize_t res;

		pid = getpid();

		snprintf(aString, sizeof(aString), "/proc/%d/exe", pid);

		res = readlink(aString, buf, PATH_MAX);
		if ( res == (ssize_t)-1 ) {
			// BSD
			snprintf(aString, sizeof(aString), "/proc/%d/file", pid);
			res = readlink(aString, buf, PATH_MAX);
			if ( res == (ssize_t)-1 ) {
				// Solaris
				snprintf(aString, sizeof(aString), "/proc/%d/path/a.out", pid);
				res = readlink(aString, buf, PATH_MAX);
			}
			if ( res == (ssize_t)-1 ) {
				// Error
				printf ("%s\n", "error 1");
				return prog;
			}
		}

		if ( res > PATH_MAX ) {
			delete buf;
			buf = new char[res+1];
			ssize_t newres = readlink(aString, buf, res);
			if ( newres == (ssize_t)-1 || newres > res ) {
				// error
				printf ("%s\n", "error 2");
				return prog;
			}
			res = newres;
		}
		buf[res]='\0';
		printf ("%s\n", buf);

		path = buf;

		delete buf;
	}

	prog = path;
	return prog;
}

int main(int argc, const char *argv[])
{
	std::string prog;

	printf ("%s\n", getProgDir(prog).c_str());
	return 0;
}
