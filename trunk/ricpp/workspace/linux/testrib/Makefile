include ../ricpp.def

.PHONY: all debug release clean cleandebug cleanrelease install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug testrib...
	$(MAKE) -C Debug -f $(MAKEFILE) $@
release:
	@echo Building release testrib...
	$(MAKE) -C Release -f $(MAKEFILE) $@
cleandebug:
	@echo Cleaning debug testrib...
	$(MAKE) -C Debug -f $(MAKEFILE) clean
cleanrelease:
	@echo Cleaning release testrib...
	$(MAKE) -C Release -f $(MAKEFILE) clean
install:
	@echo Install testrib (release)...
	$(MAKE) -C Release -f $(MAKEFILE) $@

