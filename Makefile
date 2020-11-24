PRJNAME = microsim

CXX_STANDARD = c++17
OPTIMISATION = 3
WARNINGS += all shadow extra no-unused-parameter #no-unused-variable
ERRORS += 
INCLUDES += 
INC_DIR += ./src
LIB_DIR += 
LIBS += pthread
WASM_PRELOAD_FILES += ./test

SRC_EXT = cpp
OBJ_EXT = o
WOBJ_EXT = wasm.o
BIN_EXT = elf
WASM_EXT = wasm
JS_EXT = js
HTML_EXT = html

SRC_DIR = src
BLD_DIR = build

CXX = g++
CC = gcc
LD = ${CXX}
WCXX = em++
WCC = emcc
WLD = ${WCXX}
MKDIR = mkdir -p
RM = rm -rf

CXX_FLAGS += 	-c\
				${WARNINGS:%=-W%}\
				${OPTIMISATION:%=-O%}\
				${CXX_STANDARD:%=--std=%}\
				${INC_DIR:%=-I%}\
				${INCLUDES:%=-i%}\
				-ggdb
CC_FLAGS +=		-c -ggdb
LD_FLAGS +=		${LIB_DIR:%=-L%} ${LIBS:%=-l%}
EM_FLAGS += 	-s DISABLE_EXCEPTION_CATCHING\
				--bind\
				-s ASYNCIFY=1 

SRC += ${shell find ${SRC_DIR}/ -type f -name '*.${SRC_EXT}'}
OBJ += ${SRC:${SRC_DIR}/%.${SRC_EXT}=${BLD_DIR}/%.${OBJ_EXT}}
WOBJ += ${SRC:${SRC_DIR}/%.${SRC_EXT}=${BLD_DIR}/%.${WOBJ_EXT}}



ifdef BIN_EXT
BIN = ${PRJNAME}.${BIN_EXT}
else
BIN = ${PRJNAME}
endif

WASM = ${PRJNAME}.${WASM_EXT}
JS = ${PRJNAME}.${JS_EXT}
HTML = ${PRJNAME}.${HTML_EXT}

all: ${BIN}

${BLD_DIR}/%.${OBJ_EXT}: ${SRC_DIR}/%.${SRC_EXT}
	${MKDIR} ${@D}
	${CXX} ${CXX_FLAGS} -o $@ $<

${BIN} : ${OBJ}
	${LD} ${LD_FLAGS} -o $@ $^
	

${BLD_DIR}/%.${WOBJ_EXT}: ${SRC_DIR}/%.${SRC_EXT}
	${MKDIR} ${@D}
	${WCXX} -DWASM ${CXX_FLAGS} ${EM_FLAGS} -o $@ $<
${WASM} : ${WOBJ}
	${WLD} ${LD_FLAGS} ${EM_FLAGS} ${WASM_PRELOAD_FILES:%=--preload-file %} -o $@ $^
${HTML} : ${WOBJ}
	${WLD} ${LD_FLAGS} ${EM_FLAGS} ${WASM_PRELOAD_FILES:%=--preload-file %} -o $@ $^
${JS} : ${WOBJ}
	${WLD} ${LD_FLAGS} ${EM_FLAGS} ${WASM_PRELOAD_FILES:%=--preload-file %} -o $@ $^

.phony: test_file
test_file:
	cp test_sym test

.phony: debug_src debug_obj
debug_src:
	@echo "SRC"
	@echo ${SRC}
debug_obj:
	@echo "OBJ"
	@echo ${OBJ}

.phony: bin clean wasm
bin: test_file ${BIN}
wasm: test_file ${WASM} ${JS} ${HTML}

clean:
	${RM} ${PRJNAME}* ${BLD_DIR}

include Makefiles/.microsim.mk