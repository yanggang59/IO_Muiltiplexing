SRC ?= main.c

TARGET ?= select

CROSS ?= aarch64-linux-gnu-

${TARGET}:
	${CROSS}gcc ${SRC} -o $@

copy:
	cp ${TARGET} ../../../Test/runninglinuxkernel_5.0/kmodules

clean:
	rm select