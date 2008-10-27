include ../glcpp.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)
	$(RANLIB) $(DEBUGBIN)

$(OBJPATH)/wglrc.o: $(GLCPP_WGRLC_CPP)
	$(CPP) -c $(RELPATH)/glcpp/wglrc.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/opengl.o: $(GLCPP_OPENGL_CPP)
	$(CPP) -c $(RELPATH)/glcpp/opengl.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/pixform.o: $(GLCPP_PIXFORM_CPP)
	$(CPP) -c $(RELPATH)/glcpp/pixform.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/stdafx.o: $(GLCPP_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glcpp/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
