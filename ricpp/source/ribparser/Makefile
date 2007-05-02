CC       = g++
LINK     = g++
AR       = ar rv
CPPFLAGS = -D_BOOL -DLINUX
CPPFLAGS += -Wall
INCLUDES = -I. -I..

OBJS  = ribistream.o  ribparser.o

BINNAME  = libribparser.a

default: $(BINNAME)

$(BINNAME): $(OBJS)
	@echo "Linking:   "$@
	@$(AR) $(BINNAME) $(OBJS)

.cpp.o:
	@echo "Compiling: "$@
	@$(CC) $(CPPFLAGS) -c $(INCLUDES) -o $@ $<

clean:
	-@rm -f *.o $(BINNAME)
