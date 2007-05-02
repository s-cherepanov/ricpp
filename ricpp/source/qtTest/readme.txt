qtTest
------

qtTest is a quick and dirty 'hack' (from Tutorial 8)
which can be compiled with qt 2.3.0. You must first obtain
Qt 2.3.0 (e.g the non profit version)
The QTDIR environment variable must be set.

Qt 2.3.0: qtTest  Workspace for VC 6 only.
Qt 3.2.3: qt3Test Workspace for VC 6 and VC 7 only. (Qt 3.2.3 Evaluation)

There is a warning in debug mode because there is no debug
library for qt included in the non profit version:

MSVCRTD.lib(cinitexe.obj) : warning LNK4098: defaultlib "msvcrt.lib"
conflicts with use of other libs; use /NODEFAULTLIB:library
