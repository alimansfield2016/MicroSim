_LD = em++

PRELOAD_FILES += Roms/65xx/test.rom

CXX_DEFINES += WASM
CXX_FLAGS += --llvm-opts 3
LD_FLAGS += ${PRELOAD_FILES:%=--preload-file %} #\
			-g4 \
			--source-map-base \/ #\
			-s EXPORT_NAME=MicroSim #\
			-s EXPORT_ES6=1 #\
			-s MODULARIZE=1\

include ./Makefiles/.microsim.mk
include ./Makefiles/wasm.mk