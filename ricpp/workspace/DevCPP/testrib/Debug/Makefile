include ../testrib.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo install

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(CPP) $(OBJ) -o $(DEBUGBIN) $(DEBUGLIBS)

../../rendererloader/Debug/out/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug

../../libribout/Debug/out/libriboutd.a:
	$(MAKE) -C ../../libribout -f $(MAKEFILE) debug

../../ribparser/Debug/out/libribparserd.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) debug

../../rendercontext/Debug/out/librendercontextd.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) debug

$(OBJPATH)/testrib.o: $(TESTRIB_TESTRIB_CPP)
	$(CPP) -c $(RELPATH)/testrib/testrib.cpp -o $@ $(CXXDEBUGFLAGS)

$(OBJPATH)/stdafx.o: $(TESTRIB_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/testrib/stdafx.cpp -o $@ $(CXXDEBUGFLAGS)
