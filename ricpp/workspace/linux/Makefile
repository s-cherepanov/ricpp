include ricpp.def
.PHONY: all debug relase clean debugclean releaseclean install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug...
	$(MAKE) -C tools -f $(MAKEFILE) $@
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glxTest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@

release:
	@echo Building release...
	$(MAKE) -C tools -f $(MAKEFILE) $@
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glxTest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@

cleandebug:
	@echo Cleaning debug...
	$(MAKE) -C tools -f $(MAKEFILE) $@
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glxTest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@

cleanrelease:
	@echo Cleaning release...
	$(MAKE) -C tools -f $(MAKEFILE) $@
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glxTest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@

install:
	@echo Install...
	$(MAKE) -C tools -f $(MAKEFILE) $@
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glxTest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
