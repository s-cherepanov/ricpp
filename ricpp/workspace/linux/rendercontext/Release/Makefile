include ../rendercontext.def

.PHONY: all release clean install

all: release
release: $(RELEASEBIN)

clean:
	$(RM) $(OBJ) $(RELEASEBIN)

install: $(RELEASEBIN)
	@echo install

$(RELEASEBIN): $(OBJ)
	$(AR)  $(RELEASEBIN) $(OBJ)

$(OBJPATH)/baseclass.o: $(RENDERCONTEXT_BASECLASS_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/baseclass.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/declarationmap.o: $(RENDERCONTEXT_DECLARATIONMAP_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/declarationmap.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/displaydesc.o: $(RENDERCONTEXT_DISPLAYDESC_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/displaydesc.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/light.o: $(RENDERCONTEXT_LIGHT_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/light.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/matrix3d.o: $(RENDERCONTEXT_MATRIX3D_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/matrix3d.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/optionmap.o: $(RENDERCONTEXT_OPTIONMAP_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/optionmap.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/parameter.o: $(RENDERCONTEXT_PARAMETER_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/parameter.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/parameterdeclaration.o: $(RENDERCONTEXT_PARAMETERDECLARATION_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/parameterdeclaration.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/parametermap.o: $(RENDERCONTEXT_PARAMETERMAP_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/parametermap.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/pathstack.o: $(RENDERCONTEXT_PATHSTACK_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/pathstack.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/renderattributes.o: $(RENDERCONTEXT_RENDERATTRIBUTES_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/renderattributes.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/rendercontext.o: $(RENDERCONTEXT_RENDERCONTEXT_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/rendercontext.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/renderoptions.o: $(RENDERCONTEXT_RENDEROPTIONS_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/renderoptions.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/ricppbinding.o: $(RENDERCONTEXT_RICPPBINDING_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/ricppbinding.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/rimacro.o: $(RENDERCONTEXT_RIMACRO_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/rimacro.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/surfaces.o: $(RENDERCONTEXT_SURFACES_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/surfaces.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/parametric.o: $(RENDERCONTEXT_PARAMETRIC_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/parametric.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/polygons.o: $(RENDERCONTEXT_POLYGONS_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/polygons.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/subdivision.o: $(RENDERCONTEXT_SUBDIVISION_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/subdivision.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/shader.o: $(RENDERCONTEXT_SHADER_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/shader.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/stdafx.o: $(RENDERCONTEXT_STDAFX_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/stdafx.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/trimcurve.o: $(RENDERCONTEXT_TRIMCURVE_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/trimcurve.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/validcontext.o: $(RENDERCONTEXT_VALIDCONTEXT_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/validcontext.cpp -o $@ $(CXXRELEASELIBFLAGS)
$(OBJPATH)/riscene.o: $(RENDERCONTEXT_RISCENE_CPP)
	$(CPP) -c $(RELPATH)/rendercontext/riscene.cpp -o $@ $(CXXRELEASELIBFLAGS)
