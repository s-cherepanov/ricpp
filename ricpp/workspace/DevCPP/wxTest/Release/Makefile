include ../wxTest.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install:
	@echo No install for release

$(RELEASEBIN): $(OBJ) $(RELEASEDEPLIB)
	$(CPP) $(OBJ) -o $(RELEASEBIN) $(RELEASELIBS)

../../rendererloader/Release/out/librendererloader.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) release

$(OBJPATH)/wxTest.o: $(WXTEST_WXTEST_CPP)
	$(CPP) -c $(RELPATH)/wxTest/wxTest.cpp -o $@ $(WXRELEASEFLAGS)

$(OBJPATH)/stdafx.o: $(WXTEST_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/wxTest/stdafx.cpp -o $@ $(WXRELEASEFLAGS)

wxTest_private.res: $(RELPATH)/wxTest/wxTest.rc $(RELPATH)/wxTest/mondrian.ico
	$(WINDRES) -i $(RELPATH)/wxTest/wxTest.rc -I rc -o $@ -O coff --include-dir $(RELPATH)/wxTest --include-dir $(WXWIN)/include

