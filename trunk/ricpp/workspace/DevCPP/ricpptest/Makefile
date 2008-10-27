include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug ricpptest...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release ricpptest...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug ricpptest...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release ricpptest...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install ricpptest (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

