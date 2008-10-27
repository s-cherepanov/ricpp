include ../ribout.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN) $(DEBUGDEFFILE) $(DEBUGSTATICLIB)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(DLLWRAP) --def ../libriboutdll.def --output-def $(DEBUGDEFFILE) $(DLLWRAPFLAGS) --implib $(DEBUGSTATICLIB) $(OBJ) $(DEBUGLIBS) -o $(DEBUGBIN)

../../libribout/Debug/out/libriboutd.a:
	$(MAKE) -C ../../libribout -f $(MAKEFILE) debug
../../rendererloader/Debug/out/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug
../../rendercontext/Debug/out/librendercontextd.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) debug
../../libsurfaces/Debug/out/libsurfacesd.a:
	$(MAKE) -C ../../libsurfaces -f $(MAKEFILE) debug
../../ribparser/Debug/out/libribparserd.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) debug

$(OBJPATH)/ribout.o: $(RIBOUT_RIBOUT_CPP)
	$(CPP) -c $(RELPATH)/ribout/ribout.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D RIBOUT_EXPORTS

$(OBJPATH)/stdafx.o: $(RIBOUT_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ribout/stdafx.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D RIBOUT_EXPORTS
