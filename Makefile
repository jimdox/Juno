# GNU Make workspace makefile autogenerated by Premake

.NOTPARALLEL:

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  bx_Engine_config = debug
endif
ifeq ($(config),release)
  bx_Engine_config = release
endif

PROJECTS := bx_Engine

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

bx_Engine:
ifneq (,$(bx_Engine_config))
	@echo "==== Building bx_Engine ($(bx_Engine_config)) ===="
	@${MAKE} --no-print-directory -C bx_Engine -f Makefile config=$(bx_Engine_config)
endif

clean:
	@${MAKE} --no-print-directory -C bx_Engine -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   bx_Engine"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"