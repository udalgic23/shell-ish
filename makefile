CC=gcc

main: shellish-skeleton.c cut.c
	rm -f shellish cut chatroom dirsize
	gcc -o cut cut.c
	gcc -o dirsize dirsize.c
	gcc -o chatroom chatroom.c
	gcc -o shellish shellish-skeleton.c
