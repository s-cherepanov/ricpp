include ../glcpp.def

.PHONY: all release clean

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install
	
$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)
	$(RANLIB) $(RELEASEBIN)

$(OBJPATH)/wglrc.o: $(GLCPP_WGRLC_CPP)
	$(CPP) -c $(RELPATH)/glcpp/wglrc.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/opengl.o: $(GLCPP_OPENGL_CPP)
	$(CPP) -c $(RELPATH)/glcpp/opengl.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/pixform.o: $(GLCPP_PIXFORM_CPP)
	$(CPP) -c $(RELPATH)/glcpp/pixform.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/stdafx.o: $(GLCPP_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glcpp/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
