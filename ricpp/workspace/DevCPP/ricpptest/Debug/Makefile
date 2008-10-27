include ../ricpptest.def

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

$(OBJPATH)/ricpptest.o: $(RICPPTEST_RICPPTEST_CPP)
	$(CPP) -c $(RELPATH)/ricpptest/ricpptest.cpp -o $@ $(CXXDEBUGFLAGS)

$(OBJPATH)/stdafx.o: $(RICPPTEST_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ricpptest/stdafx.cpp -o $@ $(CXXDEBUGFLAGS)
