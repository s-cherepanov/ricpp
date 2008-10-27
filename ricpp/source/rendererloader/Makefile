CC       = g++
LINK     = g++
AR       = ar rv
CPPFLAGS = -D_BOOL -DLINUX
CPPFLAGS += -Wall
INCLUDES = -I. -I..

OBJS  = rendererloader.o ricpp.o sysexcept.o

BINNAME  = librendererloader.a

default: $(BINNAME)

$(BINNAME): $(OBJS)
	@echo "Linking:   "$@
	@$(AR) $(BINNAME) $(OBJS)

.cpp.o:
	@echo "Compiling: "$@
	@$(CC) $(CPPFLAGS) -c $(INCLUDES) -o $@ $<

clean:
	-@rm -f *.o $(BINNAME)
