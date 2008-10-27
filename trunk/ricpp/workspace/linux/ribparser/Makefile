include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug ribparser...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release ribparser...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug ribparser...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release ribparser...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release ribparser...
	$(MAKE) -C Release -f $(MAKEFILE) $@

