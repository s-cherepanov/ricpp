include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug tools...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release tools...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug tools...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release tools...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release tools...
	$(MAKE) -C Release -f $(MAKEFILE) $@

