include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug wxTest...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release wxTest..
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug wxTest...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release wxTest...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install wxTest (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

