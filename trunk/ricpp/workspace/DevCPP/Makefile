include ricpp.def
.PHONY: all debug relase clean debugclean releaseclean install

all: debug release
clean: cleandebug cleanrelease

debug:
	@echo Building debug...
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glrenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribout -f $(MAKEFILE) $@
	$(MAKE) -C mesh -f $(MAKEFILE) $@
	$(MAKE) -C ricpptest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
#	$(MAKE) -C wxTest -f $(MAKEFILE) $@

release:
	@echo Building release...
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glrenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribout -f $(MAKEFILE) $@
	$(MAKE) -C mesh -f $(MAKEFILE) $@
	$(MAKE) -C ricpptest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
#	$(MAKE) -C wxTest -f $(MAKEFILE) $@

cleandebug:
	@echo Cleaning debug...
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glrenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribout -f $(MAKEFILE) $@
	$(MAKE) -C mesh -f $(MAKEFILE) $@
	$(MAKE) -C ricpptest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
#	$(MAKE) -C wxTest -f $(MAKEFILE) $@

cleanrelease:
	@echo Cleaning release...
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glrenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribout -f $(MAKEFILE) $@
	$(MAKE) -C mesh -f $(MAKEFILE) $@
	$(MAKE) -C ricpptest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
#	$(MAKE) -C wxTest -f $(MAKEFILE) $@

install:
	@echo Install...
	$(MAKE) -C glcpp -f $(MAKEFILE) $@
	$(MAKE) -C rendererloader -f $(MAKEFILE) $@
	$(MAKE) -C ribparser -f $(MAKEFILE) $@
	$(MAKE) -C rendercontext -f $(MAKEFILE) $@
	$(MAKE) -C libglrenderer -f $(MAKEFILE) $@
	$(MAKE) -C libribout -f $(MAKEFILE) $@
	$(MAKE) -C libmesh -f $(MAKEFILE) $@
	$(MAKE) -C glrenderer -f $(MAKEFILE) $@
	$(MAKE) -C ribout -f $(MAKEFILE) $@
	$(MAKE) -C mesh -f $(MAKEFILE) $@
	$(MAKE) -C ricpptest -f $(MAKEFILE) $@
	$(MAKE) -C testrib -f $(MAKEFILE) $@
#	$(MAKE) -C wxTest -f $(MAKEFILE) $@
