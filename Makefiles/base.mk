PRJNAME ?= program

SRC_DIR ?= src
BLD_DIR ?= build
OBJ_DIR ?= .
DEP_DIR ?= .

CXX_SRC_EXT ?= cpp
C_SRC_EXT ?= c
OBJ_EXT ?= o
DEP_EXT ?= d
BIN_EXT ?= elf
LIB_EXT ?= a
SHARED_EXT ?= so

CXX_SRC += ${shell find ${SRC_DIR}/ -type f -name '*.${CXX_SRC_EXT}'}
C_SRC += ${shell find ${SRC_DIR}/ -type f -name '*.${C_SRC_EXT}'}
OBJ += ${CXX_SRC:${SRC_DIR}/%.${CXX_SRC_EXT}=${BLD_DIR}/${OBJ_DIR}/%.${OBJ_EXT}}
OBJ += ${C_SRC:${SRC_DIR}/%.${C_SRC_EXT}=${BLD_DIR}/${OBJ_DIR}/%.${OBJ_EXT}}
DEP += ${OBJ:${BLD_DIR}/${OBJ_DIR}/%.${OBJ_EXT}=${BLD_DIR}/${DEP_DIR}/%.${DEP_EXT}}
BIN ?= ${PRJNAME}.${BIN_EXT}
LIB ?= ${PRJNAME}.${LIB_EXT}
SHARED ?= ${PRJNAME}.${SHARED_EXT}

CXX_STD ?= c++17
INCLUDE_DIRS += ${SRC_DIR}
CXX_ERRORS += return-type


CXX_FLAGS += 	${CXX_STD:%=--std=%}\
				${CXX_DEFINES:%=-D%}\
				${INCLUDE_DIRS:%=-I%}\
				${INCLUDES:%=-i%}\
				${CXX_WARNINGS:%=-W%}\
				${CXX_ERRORS:%=-Werror=%}\
				${OPTIM:%=-O%}
LD_FLAGS += 	${LIB_DIRS:%=-L%} \
				${LIBS:%=-l%}\
				${OPTIM:%=-O%}

_CXX ?= g++
_CC ?= gcc
_AR ?= ar
_LD ?= ld
_NM ?= nm
_SIZE ?= size

CXX = ${TOOLCHAIN_PREFIX}${_CXX}
CC = ${TOOLCHAIN_PREFIX}${_CC}
AR = ${TOOLCHAIN_PREFIX}${_AR}
LD = ${TOOLCHAIN_PREFIX}${_LD}
NM = ${TOOLCHAIN_PREFIX}${_NM}
SIZE = ${TOOLCHAIN_PREFIX}${_SIZE}

MKDIR ?= mkdir -p

_RM ?= rm -rf
RM = ${_RM}

#-------------------------------------------------------------------------------------------#
#																							#
#									Debug Options											#
#																							#
#-------------------------------------------------------------------------------------------#

DBG += debug debug_src debug_obj debug_cxx_flags debug_c_flags debug_ld_flags debug_tools

.phony: debug debug_src debug_obj debug_dep debug_cxx_flags debug_c_flags debug_ld_flags debug_tools
debug:
	@echo "Project: " ${PRJNAME}
	@echo "Project Root Directory: " ${ROOT_DIR}
	@echo "Source Directory: " ${SRC_DIR}
	@echo "Other debug options: " ${DBG}
debug_src:
	@echo "Source Directory: " ${SRC_DIR}
	@echo "Sources: " ${SRC}
debug_obj:
	@echo "Build Directory: " ${BLD_DIR}
	@echo "Object Directory: " ${OBJ_DIR}
	@echo "Objects: " ${OBJ}
debug_dep:
	@echo "Build Directory: " ${BLD_DIR}
	@echo "Dependencies Directory: " ${DEP_DIR}
	@echo "Dependencies: " ${DEP}

debug_cxx_flags:
	@echo "CXX flags: " ${CXX_FLAGS}
debug_c_flags:
	@echo "C flags: " ${C_FLAGS}
debug_ld_flags:
	@echo "LD flags: " ${LD_FLAGS}

debug_tools:
	@echo "C Compiler: " ${CC}
	@echo "C++ Compiler: " ${CXX}
	@echo "Static Archiver : " ${AR}
	@echo "Linker: " ${LD}


#-------------------------------------------------------------------------------------------#
#																							#
#									Compilation 											#
#																							#
#-------------------------------------------------------------------------------------------#

# Compile .o file from .cpp file
${BLD_DIR}/${OBJ_DIR}/%.${OBJ_EXT}: ${SRC_DIR}/%.${CXX_SRC_EXT}
	${MKDIR} ${@D}
	${CXX} -c -o $@ $< ${CXX_FLAGS}
# Compile .o file from .c file
${BLD_DIR}/${OBJ_DIR}/%.${OBJ_EXT}: ${SRC_DIR}/%.${C_SRC_EXT}
	${MKDIR} ${@D}
	${CC} -c -o $@ $< ${C_FLAGS}

# Compile .d file from .cpp file
${BLD_DIR}/${DEP_DIR}/%.${DEP_EXT}: ${SRC_DIR}/%.${CXX_SRC_EXT}
	${MKDIR} ${@D}
	${CXX} -c -M -o $@ $< ${CXX_FLAGS}
# Compile .d file from .c file
${BLD_DIR}/${DEP_DIR}/%.${DEP_EXT}: ${SRC_DIR}/%.${C_SRC_EXT}
	${MKDIR} ${@D}
	${CC} -c -M -o $@ $< ${C_FLAGS}

# Compile .elf file from .o files
${BIN}: ${OBJ}
	${LD} -o $@ ${LD_FLAGS} $^


.phony: obj dep bin lib shared
obj: ${OBJ}
dep: ${DEP}
bin: ${BIN}
lib: ${LIB}
shared: ${SHARED}

.phony: clean
clean:
	${RM} ${PRJNAME}.* ${BLD_DIR}

include ${DEP}