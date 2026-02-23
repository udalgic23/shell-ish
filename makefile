CC=gcc

main: shellish-skeleton.c cut.c
	rm -f shellish cut chatroom
	gcc -o cut cut.c
	gcc -o chatroom chatroom.c
	gcc -o shellish shellish-skeleton.c
