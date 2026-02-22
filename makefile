CC=gcc

main: shellish-skeleton.c cut.c
	rm -f shellish cut
	gcc -o cut cut.c
	gcc -o shellish shellish-skeleton.c
