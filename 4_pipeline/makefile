program_name = pipeline
main_obj = main.o

parser_obj = parser.o
main_c = main.c

parser_c = parser.c
CFLAGS = -O2 -ftrapv  -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11
COMPILER = gcc

all: $(program_name)

$(program_name): $(main_obj) $(parser_obj) 
	$(COMPILER) $(CFLAGS) $(main_obj) $(parser_obj) -o $(program_name) -lm

$(parser_obj): $(parser_c)
	$(COMPILER) -g -c $(parser_c) -o $(parser_obj)

$(main_obj): $(main_c)
	$(COMPILER) -g -c $(main_c) -o $(main_obj)

clean:
	rm -rf *.o $(program)

hello: main.c
	gcc -o hello -fsanitize=address $(CFLGS) main.c
#	./hello

#First_prog.o: First_prog.c
#	gcc -c -o First_prog.o First_prog.c

clean:
	rm *.o
