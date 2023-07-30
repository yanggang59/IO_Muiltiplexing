SRC ?= main.c

TARGET ?= poll

CROSS ?= aarch64-linux-gnu-

${TARGET}:
	${CROSS}gcc ${SRC} -o $@

clean:
	rm ${TARGET}