include ../libmesh.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)

$(OBJPATH)/libmesh.o: $(LIBMESH_LIBMESH_CPP)
	$(CPP) -c $(RELPATH)/libmesh/libmesh.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBMESH_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libmesh/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
