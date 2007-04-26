include ../ricpptest.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install:
	@echo install

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIBS)
	$(CPP) $(OBJ) -o $(RELEASEBIN) $(RELEASELIBS)

../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release

$(OBJPATH)/ricpptest.o: $(RICPPTEST_RICPPTEST_CPP)
	$(CPP) -c $(RELPATH)/ricpptest/ricpptest.cpp -o $@ $(CXXRELEASEFLAGS)

$(OBJPATH)/stdafx.o: $(RICPPTEST_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ricpptest/stdafx.cpp -o $@ $(CXXRELEASEFLAGS)
