include ../glxTest.def

DEBUGDEPLIBS  = ../../libglrenderer/Debug/libglrendererd.a
DEBUGDEPLIBS += ../../ribparser/Debug/libribparserd.a
DEBUGDEPLIBS += ../../rendererloader/Debug/librendererloaderd.a
DEBUGDEPLIBS += ../../rendercontext/Debug/librendercontextd.a
DEBUGDEPLIBS += ../../glcpp/Debug/libglcppd.a

DEBUGLIBS    += $(DEBUGDEPLIBS) -lz
DEBUGLIBS    += -ltiff
DEBUGLIBS    += -ldl
DEBUGLIBS    += -lGL
DEBUGLIBS    += -lGLU

.PHONY: all debug clean install $(DEBUGDEPLIBS)

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(LINK) $(OBJ) $(DEBUGLIBS) -o $(DEBUGBIN)

../../libglrenderer/Debug/libglrendererd.a:
	$(MAKE) -C ../../libglrenderer -f $(MAKEFILE) debug
../../rendererloader/Debug/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug
../../glcpp/Debug/libglcppd.a:
	$(MAKE) -C ../../glcpp -f $(MAKEFILE) debug
../../rendercontext/Debug/librendercontextd.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) debug
../../ribparser/Debug/libribparserd.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) debug

$(OBJPATH)/glxTest.o: $(GLXTEST_GLXTEST_CPP)
	$(CPP) -c $(RELPATH)/glxTest/glxTest.cpp -o $@ $(CXXDEBUGFLAGS)
