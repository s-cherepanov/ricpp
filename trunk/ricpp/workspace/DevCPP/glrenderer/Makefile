include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug glrenderer...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release glrenderer...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug glrenderer...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release glrenderer
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install glrenderer (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

