include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug mesh...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release mesh...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug mesh...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release mesh...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install mesh (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

