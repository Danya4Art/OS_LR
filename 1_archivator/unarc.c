#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "header.h"

#define MAX_NAME_LENGTH 100
#define BLOCK_SIZE 512
#define PERMISSION 00666


int unpack_file(int i_file, unsigned long i_size, char *true_name)
{
	char i_block[BLOCK_SIZE];
	
	int n_file = open(true_name, O_CREAT|O_WRONLY, PERMISSION);		//открываем файл
	
	if (n_file == -1)
		printf("Can't open file");
	
	//Заполнение файла
	for (int i = 0; i < i_size; ) {
		//Считываем блок
		int count = read(i_file, i_block, BLOCK_SIZE);
		//Записываем блок
		if ((i + BLOCK_SIZE) > i_size) {
			write(n_file, i_block, i_size-i);	//Дописываем файл
			break;
		} else {
			write(n_file, i_block, BLOCK_SIZE);
			i += BLOCK_SIZE;
		}
	}
	close(n_file);
}


int unarc(char *fname, char *dir, int depth)	//Распаковывает архив
{
	int arc_file = open(fname, O_RDONLY);
	if (arc_file == -1)	{
		printf("Can't open archive!");
		return 1;
	}
	DIR *directory = opendir(dir);
	if (directory == NULL) {
		if (mkdir(dir, S_IRWXU|S_IRWXG|S_IRWXO) == -1) {
			printf("Can't create directory\t%s", dir);
			close(arc_file);
			return 2;
		}
	}
	closedir(directory);
	if (chdir(dir) == -1)
		perror("");
	
	unsigned long i_size;
	int i_depth;
	char i_name[MAX_NAME_LENGTH];
	char a;
	while (read(arc_file, i_name, MAX_NAME_LENGTH) == MAX_NAME_LENGTH) {
		read(arc_file, &i_size, sizeof(long));	//Считывание размера файла
		read(arc_file, &i_depth, sizeof(int));	//Считывание глубины залегания файла
		for (int i = 0; i < BLOCK_SIZE - MAX_NAME_LENGTH - sizeof(int) - sizeof(long); i++) {
			read(arc_file, &a, 1);	//Считывание заголовочного блока полностью
		}
		//Проверяем, является ли считанный файл собственно файлом или архивом (т.е. вложенной папкой)
		if (i_depth == depth+1) {
			unpack_file(arc_file, i_size, ".included_archive");
			unarc(".included_archive", i_name, depth+1);
			remove(".included_archive");
		} else if (i_depth == depth) {
			unpack_file(arc_file, i_size, i_name);
		} else {
			close(arc_file);
			return 3;
		}
	}
	close(arc_file);
	chdir("..");	//Возвращаемся на уровень выше
	return 0;
}
