CC       = g++
LINK     = g++
AR       = ar rv
CPPFLAGS = -D_BOOL -DLINUX -D_DEBUG -D__DEBUG__
CPPFLAGS += -Wall -fexceptions -fverbose-asm -g3
INCLUDES = -I. -I../../ricpp/source
LIBS    += ../../ricpp/workspace/linux/rendercontext/Debug/librendercontextd.a
LIBS    += ../../ricpp/workspace/linux/ribparser/Debug/libribparserd.a
LIBS    += ../../ricpp/workspace/linux/rendererloader/Debug/librendererloaderd.a
LIBS    += -lz
LIBS    += -ldl

OBJS  = testrib.o

BINNAME  = testrib

default: $(BINNAME)

$(BINNAME): $(OBJS)
	@echo "Linking:   "$@
	$(LINK) $(OBJS) $(LIBS) -o $(BINNAME)

.cpp.o:
	@echo "Compiling: "$@
	@$(CC) $(CPPFLAGS) -c $(INCLUDES) -o $@ $<

clean:
	-@rm -f *.o $(BINNAME)
