include ../wxTest.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ) $(DEBUGDEPLIBS)
	$(CPP) $(OBJ) -o $(DEBUGBIN) $(DEBUGLIBS)

../../rendererloader/Debug/out/librendererloaderd.a:
	$(MAKE) -C ../../rendererloader -f $(MAKEFILE) debug

$(OBJPATH)/wxTest.o: $(WXTEST_WXTEST_CPP)
	$(CPP) -c $(RELPATH)/wxTest/wxTest.cpp -o $@ $(WXDEBUGFLAGS)

$(OBJPATH)/stdafx.o: $(WXTEST_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/wxTest/stdafx.cpp -o $@ $(WXDEBUGFLAGS)

wxTest_private.res: $(RELPATH)/wxTest/wxTest.rc $(RELPATH)/wxTest/mondrian.ico
	$(WINDRES) -i $(RELPATH)/wxTest/wxTest.rc -I rc -o $@ -O coff --include-dir $(RELPATH)/wxTest --include-dir $(WXWIN)/include

