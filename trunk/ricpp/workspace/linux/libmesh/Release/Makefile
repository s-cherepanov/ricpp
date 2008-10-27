include ../libmesh.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)

$(OBJPATH)/libmesh.o: $(LIBMESH_LIBMESH_CPP)
	$(CPP) -c $(RELPATH)/libmesh/libmesh.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBMESH_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libmesh/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
