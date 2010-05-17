include ../tools.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug.

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)

$(OBJPATH)/logfile.o: $(TOOLS_LOGFILE_CPP)
	$(CPP) -c $(RELPATH)/tools/logfile.cpp -o $@ $(CXXDEBUGLIBFLAGS)
