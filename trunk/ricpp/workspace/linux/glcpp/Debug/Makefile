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

$(OBJPATH)/xwglrc.o: $(GLCPP_XWGRLC_CPP)
	$(CPP) -c $(RELPATH)/glcpp/xwglrc.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/opengl.o: $(GLCPP_OPENGL_CPP)
	$(CPP) -c $(RELPATH)/glcpp/opengl.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/xpixform.o: $(GLCPP_XPIXFORM_CPP)
	$(CPP) -c $(RELPATH)/glcpp/xpixform.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/stdafx.o: $(GLCPP_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/glcpp/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
