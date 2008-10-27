include ../libglrenderer.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)
	$(RANLIB) $(RELEASEBIN)

$(OBJPATH)/glprims.o: $(LIBGLRENDERER_GLPRIMS_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/glprims.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/libglrenderer.o: $(LIBGLRENDERER_LIBGLRENDERER_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/libglrenderer.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBGLRENDERER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libglrenderer/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
