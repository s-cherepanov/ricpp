include ../ribparser.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)

$(OBJPATH)/stdafx.o: $(RIBPARSER_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/ribparser/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/ribistream.o: $(RIBPARSER_RIBISTREAM_CPP)
	$(CPP) -c $(RELPATH)/ribparser/ribistream.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/ribparser.o: $(RIBPARSER_RIBPARSER_CPP)
	$(CPP) -c $(RELPATH)/ribparser/ribparser.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/paramclasses.o: $(RIBPARSER_PARAMCLASSES_CPP)
	$(CPP) -c $(RELPATH)/ribparser/paramclasses.cpp -o $@ $(CXXRELEASELIBFLAGS)
