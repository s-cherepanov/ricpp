include ../glxTest.def

RELEASEDEPLIBS  = ../../libglrenderer/Release/libglrenderer.a
RELEASEDEPLIBS += ../../ribparser/Release/libribparser.a
RELEASEDEPLIBS += ../../rendererloader/Release/librendererloader.a
RELEASEDEPLIBS += ../../rendercontext/Release/librendercontext.a
RELEASEDEPLIBS += ../../glcpp/Release/libglcpp.a

RELEASELIBS    += $(RELEASEDEPLIBS) -lz
RELEASELIBS    += -ltiff
RELEASELIBS    += -ldl
RELEASELIBS    += -lGL
RELEASELIBS    += -lGLU

.PHONY: all release clean install $(RELEASEDEPLIBS)

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(LINK) $(OBJ) $(RELEASELIBS) -o $(RELEASEBIN)

../../libglrenderer/Release/libglrenderer.a:
	$(MAKE) -C ../../libglrenderer -f $(MAKEFILE) release
../../rendererloader/Release/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release
../../glcpp/Release/libglcpp.a:
	$(MAKE) -C ../../glcpp -f $(MAKEFILE) release
../../rendercontext/Release/librendercontext.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) release
../../ribparser/Release/libribparser.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) release

$(OBJPATH)/glxTest.o: $(GLXTEST_GLXTEST_CPP)
	$(CPP) -c $(RELPATH)/glxTest/glxTest.cpp -o $@ $(CXXRELEASEFLAGS)
