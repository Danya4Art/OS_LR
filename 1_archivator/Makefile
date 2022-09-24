all:
	gcc main.c arc.c unarc.c -o archivator
sanitized: 
	cppcheck --enable=all --inconclusive --library=posix 
	gcc main.c arc.c unarc.c -o archivator -fsanitize=address
