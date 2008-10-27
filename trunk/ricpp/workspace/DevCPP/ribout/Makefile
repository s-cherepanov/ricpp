include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug ribout...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release ribout...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug ribout...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release ribout...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install ribout (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

