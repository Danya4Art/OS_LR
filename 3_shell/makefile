program_name = my_shell
main_obj = main.o

parser_obj = parser.o
stack_obj = stack.o
polis_obj = polis.o
main_c = main.c

parser_c = parser.c
stack_c = stack.c
polis_c = polis.c
CFLAGS = -O2 -ftrapv  -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11
COMPILER = gcc

all: $(program_name)

$(program_name): $(main_obj) $(parser_obj) $(stack_obj) $(polis_obj)
	$(COMPILER) $(CFLAGS) $(main_obj) $(parser_obj) $(stack_obj) $(polis_obj) -o $(program_name) -lm

$(stack_obj): $(stack_c)
	$(COMPILER) -g -c $(stack_c) -o $(stack_obj)

$(polis_obj): $(polis_c)
	$(COMPILER) -g -c $(polis_c) -o $(polis_obj)

$(parser_obj): $(parser_c)
	$(COMPILER) -g -c $(parser_c) -o $(parser_obj)

$(main_obj): $(main_c)
	$(COMPILER) -g -c $(main_c) -o $(main_obj)

clean:
	rm -rf *.o $(program)
