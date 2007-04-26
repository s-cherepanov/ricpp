include ../libribout.def

.PHONY: all debug clean install

all: debug
debug: $(DEBUGBIN)

clean:
	$(RM) $(OBJ) $(DEBUGBIN)

install:
	@echo No install for debug

$(DEBUGBIN): $(OBJ)
	$(AR) $(DEBUGBIN) $(OBJ)

$(OBJPATH)/libribout.o: $(LIBRIBOUT_LIBRIBOUT_CPP)
	$(CPP) -c $(RELPATH)/libribout/libribout.cpp -o $@ $(CXXDEBUGLIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBRIBOUT_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libribout/stdafx.cpp -o $@ $(CXXDEBUGLIBFLAGS)
