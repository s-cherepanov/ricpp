include ../libglrenderer.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)

$(OBJPATH)/glprims.o: $(LIBGLRENDERER_GLPRIMS_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/glprims.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/libglrenderer.o: $(LIBGLRENDERER_LIBGLRENDERER_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/libglrenderer.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBGLRENDERER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
