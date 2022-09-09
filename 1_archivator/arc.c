#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "header.h"

#define MAX_NAME_LENGTH 100
#define BLOCK_SIZE 512

int add_to_arc(char *temp_name, int arc_file, int depth, char *d_name) {

    struct stat stats;
    unsigned long size;

    int temp_file = open(temp_name, O_RDONLY);
    if (temp_file == -1) {
        printf("Can't open file");
        return 1;
    } 
    lstat(temp_name, &stats);   //считываем статус файла
    size = stats.st_size;
    //printf("Size %lu\t", size);

    if (strcmp("", d_name) == 0)	{
		if (write(arc_file, temp_name, MAX_NAME_LENGTH) != MAX_NAME_LENGTH)	//Запись имени
			perror("");
	}   else
		write(arc_file, d_name, MAX_NAME_LENGTH);	
    write(arc_file, &size, sizeof(long));	//Запись размера файла
	write(arc_file, &depth, sizeof(int));	//Запись глубины залегания файла
	char a = 0;
	for (int i = 0; i < BLOCK_SIZE-MAX_NAME_LENGTH - sizeof(int) - sizeof(long); i++)
		write(arc_file, &a, 1);	
    
    //Блоки данных
	unsigned char block[BLOCK_SIZE];
	int count = read(temp_file, block, BLOCK_SIZE);
	while (count == BLOCK_SIZE) {
		write(arc_file, block, BLOCK_SIZE);
		count = read(temp_file, block, BLOCK_SIZE);
	}
	if (count != 0)	{
		write(arc_file, block, count);
		for (int i = 0; i < BLOCK_SIZE - count; i++)	//Дописываем блок до BLOCK_SIZE байт
			write(arc_file, &a, 1);
	}
	close(temp_file);
	return 0;

}

int arc(char *filename, char *dirname, char depth) {
    // printf("%s", filename);
    // printf("%s", dirname);   
    // printf("%d", depth);

    int arc_file = open(filename, O_CREAT | O_WRONLY, 00666);   //создаём файл только для записи
    if (arc_file == -1) {
        printf("Can't create archive!\n");
        return 1;
    }
    //perror("");

    DIR *dir = opendir(dirname);        //открывет каталог и формирует поток  
    if (dir == NULL) {
        printf("Can't open directory!\n");
        return 2;
    }
    chdir(dirname);                 

    struct dirent *read_dir;
    struct stat statbuf;
    
    read_dir = readdir(dir);        //получаем казатель на структуру
    while (read_dir != NULL) {
        if (strcmp((*read_dir).d_name, ".") != 0 && strcmp((*read_dir).d_name, "..") != 0) {
            //printf("> %s", (*read_dir).d_name);
            lstat((*read_dir).d_name, &statbuf);    //получаем информацию о ссылке
            if (S_ISDIR(statbuf.st_mode)) {         //проверка является ли файл каталогом
                //printf(" - directory.\n");
                arc(".tempdir", (*read_dir).d_name, depth+1);
                add_to_arc(".tempdir", arc_file, depth+1, (*read_dir).d_name);
                remove(".tempdir");
            } else {
                //printf(" - file.\n");
                add_to_arc((*read_dir).d_name, arc_file, depth, "");
            }
        }
        read_dir = readdir(dir);
    }
    chdir("..");
    closedir(dir);
    if (close(arc_file) == -1) {
        printf("Can't close file!");
        return 3;
    }
    return 0;
}
