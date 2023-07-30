SRC ?= main.c

TARGET ?= poll

CROSS ?= aarch64-linux-gnu-

${TARGET}:
	${CROSS}gcc ${SRC} -o $@

copy:
	cp ${TARGET} /home/thomas/Test/runninglinuxkernel_5.0/kmodules

clean:
	rm ${TARGET}