include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug glcpp...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release glcpp...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug glcpp...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release glcpp...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install release gllcpp ...
	$(MAKE) -C Release -f $(MAKEFILE) $@

