include ../tools.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)

$(OBJPATH)/logfile.o: $(TOOLS_LOGFILE_CPP)
	$(CPP) -c $(RELPATH)/tools/logfile.cpp -o $@ $(CXXRELEASELIBFLAGS)
