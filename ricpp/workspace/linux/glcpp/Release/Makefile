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

$(OBJPATH)/xwglrc.o: $(GLCPP_XWGRLC_CPP)
	$(CPP) -c $(RELPATH)/glcpp/xwglrc.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/opengl.o: $(GLCPP_OPENGL_CPP)
	$(CPP) -c $(RELPATH)/glcpp/opengl.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/xpixform.o: $(GLCPP_XPIXFORM_CPP)
	$(CPP) -c $(RELPATH)/glcpp/xpixform.cpp -o $@ $(CXXRELEASELIBFLAGS)

$(OBJPATH)/stdafx.o: $(GLCPP_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glcpp/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
