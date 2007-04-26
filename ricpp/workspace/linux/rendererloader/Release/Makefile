include ../rendererloader.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)

$(OBJPATH)/stdafx.o: $(RENDERERLOADER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/sysexcept.o: $(RENDERERLOADER_SYSEXCEPT_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/sysexcept.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/ricpp.o: $(RENDERERLOADER_RICPP_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/ricpp.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/rendererloader.o: $(RENDERERLOADER_rendererloader_CPP)
	$(CPP) -c $(RELPATH)/rendererloader/rendererloader.cpp -o $@ $(CXXRELEASELIBFLAGS)
