include ../testrib.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(CPP) $(OBJ) -o $(RELEASEBIN) $(RELEASELIBS)

../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release

../../libribout/Release/out/libribout.a:
	$(MAKE) -C ../../libribout -f $(MAKEFILE) release

../../ribparser/Release/out/libribparser.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) release

../../libsurfaces/Release/out/libsurfaces.a:
	$(MAKE) -C ../../libsurfaces -f $(MAKEFILE) release

../../rendercontext/Release/out/librendercontext.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) release

$(OBJPATH)/testrib.o: $(TESTRIB_TESTRIB_CPP)
	$(CPP) -c $(RELPATH)/testrib/testrib.cpp -o $@ $(CXXRELEASEFLAGS)

$(OBJPATH)/stdafx.o: $(TESTRIB_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/testrib/stdafx.cpp -o $@ $(CXXRELEASEFLAGS)
