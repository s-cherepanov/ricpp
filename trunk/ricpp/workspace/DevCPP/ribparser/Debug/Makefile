include ../ribparser.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug.

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)
	$(RANLIB) $(DEBUGBIN)

$(OBJPATH)/stdafx.o: $(RIBPARSER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ribparser/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/ribistream.o: $(RIBPARSER_RIBISTREAM_CPP)
	$(CPP) -c $(RELPATH)/ribparser/ribistream.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/ribparser.o: $(RIBPARSER_RIBPARSER_CPP)
	$(CPP) -c $(RELPATH)/ribparser/ribparser.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/paramclasses.o: $(RIBPARSER_PARAMCLASSES_CPP)
	$(CPP) -c $(RELPATH)/ribparser/paramclasses.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/logfile.o: $(TOOLS_LOGFILE_CPP)
	$(CPP) -c $(RELPATH)/tools/logfile.cpp -o $@ $(CXXDEBUGLIBFLAGS)
