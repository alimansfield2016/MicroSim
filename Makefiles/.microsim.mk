PRJNAME = microsim
LIBS += pthread
OPTIM = 3
CXX_FLAGS += -ggdb
LD_FLAGS += -ggdb

bin: roms

ROMS += Roms/65xx/test.rom
SYMS = ${ROMS:%=%_sym}

.phony: roms ${ROMS}

%.rom: %_sym.rom
	cp $< $@

roms: ${ROMS}