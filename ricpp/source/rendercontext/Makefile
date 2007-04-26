CC       = g++
LINK     = g++
AR       = ar rv
CPPFLAGS = -D_BOOL -DLINUX
CPPFLAGS += -Wall
INCLUDES = -I. -I..

OBJS  = baseclass.o light.o parameterdeclaration.o pathstack.o renderoptions.o shader.o
OBJS += declarationmap.o matrix3d.o parametermap.o renderattributes.o ricppbinding.o
OBJS += subdiv.o displaydesc.o optionmap.o parameter.o rendercontext.o rimacro.o
OBJS += validcontext.o riscene.o trimcurve.o

BINNAME  = librendercontext.a

default: $(BINNAME)

$(BINNAME): $(OBJS)
	@echo "Linking:   "$@
	@$(AR) $(BINNAME) $(OBJS)

.cpp.o:
	@echo "Compiling: "$@
	@$(CC) $(CPPFLAGS) -c $(INCLUDES) -o $@ $<

clean:
	-@rm -f *.o $(BINNAME)
