hello: main.c
	gcc -o hello -fsanitize=address -g -lm -ldl -Wall -Wpointer-arith -finline-functions -ffast-math main.c
#	./hello

#First_prog.o: First_prog.c
#	gcc -c -o First_prog.o First_prog.c

clean:
	rm hello *.o
# "workingdir":"${folder:${file}}",
# "commandline":"gcc -g  ${file} -o ${file_base_name} && gdb --interpreter=mi --args ./${file_base_name}",
# "sublimegdb_exec_cmd": "-exec-run",
