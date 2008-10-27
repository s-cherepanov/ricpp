include ../libribout.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR) $(RELEASEBIN) $(OBJ)

$(OBJPATH)/libribout.o: $(LIBRIBOUT_LIBRIBOUT_CPP)
	$(CPP) -c $(RELPATH)/libribout/libribout.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/stdafx.o: $(LIBRIBOUT_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/libribout/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
