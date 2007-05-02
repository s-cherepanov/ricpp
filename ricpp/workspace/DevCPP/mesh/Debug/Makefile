include ../mesh.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN) $(DEBUGDEFFILE) $(DEBUGSTATICLIB)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(DLLWRAP) --def ../libmeshdll.def --output-def $(DEBUGDEFFILE) $(DLLWRAPFLAGS) --implib $(DEBUGSTATICLIB) $(OBJ) $(DEBUGLIBS) -o $(DEBUGBIN)

../../libmesh/Debug/out/libmeshd.a:
	$(MAKE) -C ../../libmesh -f $(MAKEFILE) debug
../../rendererloader/Debug/out/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug
../../rendercontext/Debug/out/librendercontextd.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) debug
../../ribparser/Debug/out/libribparserd.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) debug

$(OBJPATH)/mesh.o: $(MESH_MESH_CPP)
	$(CPP) -c $(RELPATH)/mesh/mesh.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D MESH_EXPORTS

$(OBJPATH)/stdafx.o: $(MESH_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/mesh/stdafx.cpp -o $@ $(CXXDEBUGDLLFLAGS) -D MESH_EXPORTS
