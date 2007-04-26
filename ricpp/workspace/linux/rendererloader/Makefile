include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug rendererloader...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release rendererloader...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug rendererloader...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release rendererloader...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release rendererloader...
	$(MAKE) -C Release -f $(MAKEFILE) $@
