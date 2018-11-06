CC = gcc
FLAGS = -g
LDFLAGS = -lrt
BIN = send receive limit

all: send receive limit

send:
	${CC} ${FLAGS} ${LDFLAGS} send.c -o $@

receive:
	${CC} ${FLAGS} ${LDFLAGS} receive.c -o $@

limit:
	${CC} ${FLAGS} ${LDFLAGS} limit.c -o $@

clean:
	rm -f ${BIN}
