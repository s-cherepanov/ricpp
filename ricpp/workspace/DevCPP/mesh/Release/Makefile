include ../mesh.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN) $(RELEASEDEFFILE) $(RELEASESTATICLIB)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(DLLWRAP) --def ../libmeshdll.def --output-def $(RELEASEDEFFILE) $(DLLWRAPFLAGS) --implib $(RELEASESTATICLIB) $(OBJ) $(RELEASELIBS) -o $(RELEASEBIN)

../../libmesh/Release/out/libmesh.a:
	$(MAKE) -C ../../libmesh -f $(MAKEFILE) release
../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release
../../rendercontext/Release/out/librendercontext.a:
	$(MAKE) -C ../../rendercontext -f $(MAKEFILE) release
../../ribparser/Release/out/libribparser.a:
	$(MAKE) -C ../../ribparser -f $(MAKEFILE) release

$(OBJPATH)/mesh.o: $(MESH_MESH_CPP)
	$(CPP) -c $(RELPATH)/mesh/mesh.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D MESH_EXPORTS

$(OBJPATH)/stdafx.o: $(MESH_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/mesh/stdafx.cpp -o $@ $(CXXRELEASEDLLFLAGS) -D MESH_EXPORTS
