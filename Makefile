# GNU Make workspace makefile autogenerated by Premake

.NOTPARALLEL:

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  bx_engine_config = debug
endif
ifeq ($(config),release)
  bx_engine_config = release
endif

PROJECTS := bx_engine

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

bx_engine:
ifneq (,$(bx_engine_config))
	@echo "==== Building bx_engine ($(bx_engine_config)) ===="
	@${MAKE} --no-print-directory -C bx_engine -f Makefile config=$(bx_engine_config)
endif

clean:
	@${MAKE} --no-print-directory -C bx_engine -f Makefile clean

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
	@echo "   bx_engine"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"