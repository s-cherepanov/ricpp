include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug libglrenderer...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release libglrenderer...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug libglrenderer...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release libglrenderer...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release libglrenderer...
	$(MAKE) -C Release -f $(MAKEFILE) $@

