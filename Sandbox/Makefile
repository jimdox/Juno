# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug)
  RESCOMP = windres
  TARGETDIR = ../Bin/Debug
  TARGET = $(TARGETDIR)/Sandbox
  OBJDIR = ../Bin/Int/Debug/Debug/Sandbox
  PCH = ../Juno/Include/jnpch.h
  GCH = $(OBJDIR)/$(notdir $(PCH)).gch
  DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLAD -D_X11 -DJUNO_PLATFORM_LINUX
  INCLUDES += -I../Juno -I../Juno/Include -I../Juno/Src -I../Juno/Vendor/glad/include -I../Juno/Vendor/glfw/include -IVendor/vulkan/x86_64/include -IVendor/vulkan/x86_64/lib -I../Juno/Vendor/glm -I../Juno/Vendor/imgui -I../Juno/Vendor/stb -I../Juno/Vendor/jpl -I../Juno/Vendor/spdlog/include
  FORCE_INCLUDE += -include $(OBJDIR)/$(notdir $(PCH))
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -std=c++17
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../Bin/Debug/libJuno.a ../Bin/Debug/libGLFW.a ../Bin/Debug/libGLAD.a -lvulkan -lX11 -ldl -lpthread
  LDDEPS += ../Bin/Debug/libJuno.a ../Bin/Debug/libGLFW.a ../Bin/Debug/libGLAD.a
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release)
  RESCOMP = windres
  TARGETDIR = ../Bin/Release
  TARGET = $(TARGETDIR)/Sandbox
  OBJDIR = ../Bin/Int/Release/Release/Sandbox
  PCH = ../Juno/Include/jnpch.h
  GCH = $(OBJDIR)/$(notdir $(PCH)).gch
  DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLAD -D_X11 -DJUNO_PLATFORM_LINUX
  INCLUDES += -I../Juno -I../Juno/Include -I../Juno/Src -I../Juno/Vendor/glad/include -I../Juno/Vendor/glfw/include -IVendor/vulkan/x86_64/include -IVendor/vulkan/x86_64/lib -I../Juno/Vendor/glm -I../Juno/Vendor/imgui -I../Juno/Vendor/stb -I../Juno/Vendor/jpl -I../Juno/Vendor/spdlog/include
  FORCE_INCLUDE += -include $(OBJDIR)/$(notdir $(PCH))
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -std=c++17
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../Bin/Release/libJuno.a ../Bin/Release/libGLFW.a ../Bin/Release/libGLAD.a -lvulkan -lX11 -ldl -lpthread
  LDDEPS += ../Bin/Release/libJuno.a ../Bin/Release/libGLFW.a ../Bin/Release/libGLAD.a
  ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

OBJECTS := \
	$(OBJDIR)/jnpch.o \
	$(OBJDIR)/SandboxProgram.o \

RESOURCES := \

CUSTOMFILES := \

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

$(TARGET): $(GCH) ${CUSTOMFILES} $(OBJECTS) $(LDDEPS) $(RESOURCES) | $(TARGETDIR)
	@echo Linking Sandbox
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(CUSTOMFILES): | $(OBJDIR)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Sandbox
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) $(PCH) | $(OBJDIR)
$(GCH): $(PCH) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
else
$(OBJECTS): | $(OBJDIR)
endif

$(OBJDIR)/jnpch.o: ../Juno/Include/jnpch.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SandboxProgram.o: Src/SandboxProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif