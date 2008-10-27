include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug rendercontext...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release rendecontext...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug rendercontext
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release rendercontext
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install rendercontext (release)..
	$(MAKE) -C Release -f $(MAKEFILE) $@

