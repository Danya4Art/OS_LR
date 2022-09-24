all:
	gcc main.c arc.c unarc.c -o archivator.o
sanitized: 
	cppcheck --enable=all --inconclusive --library=posix 
	gcc main.c arc.c unarc.c -o archivator -fsanitize=address
