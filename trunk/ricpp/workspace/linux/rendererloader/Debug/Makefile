include ../rendererloader.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug.

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)

$(OBJPATH)/stdafx.o: $(RENDERERLOADER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/sysexcept.o: $(RENDERERLOADER_SYSEXCEPT_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/sysexcept.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/ricpp.o: $(RENDERERLOADER_RICPP_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/ricpp.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/rendererloader.o: $(RENDERERLOADER_rendererloader_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/rendererloader.cpp -o $@ $(CXXDEBUGLIBFLAGS)

