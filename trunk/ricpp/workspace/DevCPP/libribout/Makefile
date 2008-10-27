include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug libribout...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release libribout...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug libribout...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release libribout...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install libribout (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

