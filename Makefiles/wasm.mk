SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

_CXX ?= em++
_CC ?= emcc
_AR ?= emar
_LD ?= em++

OBJ_EXT ?= wasm.o
BIN_EXT ?= wasm
HTML_EXT ?= html
JS_EXT ?= js
MJS_EXT ?= mjs

CXX_FLAGS += -s WASM=1
LD_FLAGS += --bind -s WASM=1

-include ${SELF_DIR}/base.mk

HTML ?= ${PRJNAME}.${HTML_EXT}
JS ?= ${PRJNAME}.${JS_EXT}
MJS ?= ${PRJNAME}.${MJS_EXT}

${HTML}: ${OBJ}
	${LD} -o $@ ${LD_FLAGS} $^
${JS}: ${OBJ}
	${LD} -o $@ ${LD_FLAGS} $^
${MJS}: ${OBJ}
	${LD} -o $@ ${LD_FLAGS} $^

.phony: wasm html js mjs
html: ${HTML}
js: ${JS}
mjs: ${MJS}

wasm: bin mjs html