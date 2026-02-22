CC=gcc

main: shellish-skeleton.c
	rm -f shellish
	gcc -o shellish shellish-skeleton.c
