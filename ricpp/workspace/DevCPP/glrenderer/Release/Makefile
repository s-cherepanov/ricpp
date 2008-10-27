include ../glrenderer.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN) $(RELEASEDEFFILE) $(RELEASESTATICLIB)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(DLLWRAP) --def ../libglrendererdll.def --output-def $(RELEASEDEFFILE) $(DLLWRAPFLAGS) --implib $(RELEASESTATICLIB) $(OBJ) $(RELEASELIBS) -o $(RELEASEBIN)

../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release
../../glcpp/Release/out/libglcpp.a:
	$(MAKE) -C ../../glcpp -f $(MAKEFILE) release
../../rendercontext/Release/out/librendercontext.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) release
../../ribparser/Release/out/libribparser.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) release

$(OBJPATH)/glrenderer.o: $(GLRENDERER_GLRENDERER_CPP)
	$(CPP) -c $(RELPATH)/glrenderer/glrenderer.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D GLRENDERER_EXPORTS

$(OBJPATH)/stdafx.o: $(GLRENDERER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glrenderer/stdafx.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D GLRENDERER_EXPORTS
