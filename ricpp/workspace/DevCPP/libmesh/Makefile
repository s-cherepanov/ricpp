include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug libmesh...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release libmesh...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug libmesh...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release libmesh...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install libmesh (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

