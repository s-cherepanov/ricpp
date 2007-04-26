include ricpp.def
.PHONY: all debug relase clean debugclean releaseclean install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug...
	$(MAKE) -C baserenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribwriter -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ricpp -f $(MAKEFILE) $@
	$(MAKE) -C test -f $(MAKEFILE) $@

release:
	@echo Building release...
	$(MAKE) -C baserenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribwriter -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ricpp -f $(MAKEFILE) $@
	$(MAKE) -C test -f $(MAKEFILE) $@

cleandebug:
	@echo Cleaning debug...
	$(MAKE) -C baserenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribwriter -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ricpp -f $(MAKEFILE) $@
	$(MAKE) -C test -f $(MAKEFILE) $@

cleanrelease:
	@echo Cleaning release...
	$(MAKE) -C baserenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribwriter -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ricpp -f $(MAKEFILE) $@
	$(MAKE) -C test -f $(MAKEFILE) $@

install:
	@echo Install...
	$(MAKE) -C baserenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribwriter -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ricpp -f $(MAKEFILE) $@
	$(MAKE) -C test -f $(MAKEFILE) $@

