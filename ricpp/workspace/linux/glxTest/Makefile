include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug glxTest...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release glxTest...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug glxTest...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release glxTest...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release glxTest...
	$(MAKE) -C Release -f $(MAKEFILE) $@

