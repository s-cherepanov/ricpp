include ../glrenderer.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN) $(DEBUGDEFFILE) $(DEBUGSTATICLIB)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(DLLWRAP) --def ../libglrendererdll.def --output-def $(DEBUGDEFFILE) $(DLLWRAPFLAGS) --implib $(DEBUGSTATICLIB) $(OBJ) $(DEBUGLIBS) -o $(DEBUGBIN)

../../libglrenderer/Debug/out/libglrendererd.a:
	$(MAKE) -C ../../libglrenderer -f $(MAKEFILE) debug
../../rendererloader/Debug/out/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug
../../glcpp/Debug/out/libglcppd.a:
	$(MAKE) -C ../../glcpp -f $(MAKEFILE) debug
../../rendercontext/Debug/out/librendercontextd.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) debug
../../ribparser/Debug/out/libribparserd.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) debug
	
$(OBJPATH)/glrenderer.o: $(GLRENDERER_GLRENDERER_CPP)
	$(CPP) -c $(RELPATH)/glrenderer/glrenderer.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D GLRENDERER_EXPORTS

$(OBJPATH)/stdafx.o: $(GLRENDERER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glrenderer/stdafx.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D GLRENDERER_EXPORTS
