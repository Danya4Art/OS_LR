program_name = filter
main_obj = main.o

files_obj = files.o
main_c = main.c

files_c = files.c
CFLAGS = -O2 -ftrapv  -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11
COMPILER = gcc

all: $(program_name)

$(program_name): $(main_obj) $(files_obj) 
	$(COMPILER) $(CFLAGS) $(main_obj) $(files_obj) -o $(program_name) -lm

$(files_obj): $(files_c)
	$(COMPILER) -g -c $(files_c) -o $(files_obj)

$(main_obj): $(main_c)
	$(COMPILER) -g -c $(main_c) -o $(main_obj)

clean:
	rm *.o $(program)
