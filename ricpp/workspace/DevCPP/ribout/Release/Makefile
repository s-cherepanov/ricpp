include ../ribout.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN) $(RELEASEDEFFILE) $(RELEASESTATICLIB)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(DLLWRAP) --def ../libriboutdll.def --output-def $(RELEASEDEFFILE) $(DLLWRAPFLAGS) --implib $(RELEASESTATICLIB) $(OBJ) $(RELEASELIBS) -o $(RELEASEBIN)

../../libribout/Release/out/libribout.a:
	$(MAKE) -C ../../libribout -f $(MAKEFILE) release
../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release
../../rendercontext/Release/out/librendercontext.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) release
../../libsurfaces/Release/out/libsurfaces.a:
	$(MAKE) -C ../../libsurfaces -f $(MAKEFILE) release
../../ribparser/Release/out/libribparser.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) release

$(OBJPATH)/ribout.o: $(RIBOUT_RIBOUT_CPP)
	$(CPP) -c $(RELPATH)/ribout/ribout.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D RIBOUT_EXPORTS

$(OBJPATH)/stdafx.o: $(RIBOUT_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ribout/stdafx.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D RIBOUT_EXPORTS
