#include "files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>


int height = 0;
int width = 0;
char*** PixelArr;
char*** PixelArr2;
int ThreadNum=0;
int A=0;

int main(int argc, char **argv)
{
    if(argc!=3)
    {
        printf("Неверное число аргументов\n");
        return 0;
    }
    FILE* f;
    if((f=fopen(argv[1], "rb"))==NULL)
    {
        printf("Ошибка открытия файла\n");
        return 0;
    }
    for(int i=0;i<strlen(argv[2]);i++)
    {
        if(argv[2][i]>='0'&&argv[2][i]<='9')
        {
            ThreadNum=ThreadNum*10+(argv[2][i]-'0');
        }
        else
        {
            printf("Неверное значение количества потоков\n");
            fclose(f);
            return 0;
        }
    }
    if(ThreadNum==0)
    {
        printf("Неверное значение количества потоков\n");
        fclose(f);
        return 0;
    }
    char check[3];
    fread(check,sizeof(unsigned char),3,f);
    if(strncmp(check,"P6\n",3))
    {
        printf("Неверная структура файла\n");
        fclose(f);
        return 0;
    }
    clock_t start, end;
    fseek(f, 3, SEEK_SET);
    char c = '0';
    fread(&c, sizeof(unsigned char), 1, f);
    while (c != ' '&& c!='\n')
    {
        width = width * 10 + (c - 48);
        fread(&c, sizeof(unsigned char), 1, f);
    }
    fread(&c, sizeof(unsigned char), 1, f);
    while (c != ' ' && c != '\n')
    {
        height = height * 10 + (c - 48);
        fread(&c, sizeof(unsigned char), 1, f);
    }
    PixelArr = (char***)malloc((height + 1) * sizeof(char**));
    for (int i = 0; i <= height; i++)
    {
        PixelArr[i] = (char**)malloc((width) * sizeof(char*));
        for (int j = 0; j < width; j++)
        {
            PixelArr[i][j] = (char*)malloc(3);
        }
    }
    PixelArr2 = (char***)malloc((height + 1) * sizeof(char**));
    for (int i = 0; i <= height; i++)
    {
        PixelArr2[i] = (char**)malloc((width) * sizeof(char*));
        for (int j = 0; j < width; j++)
        {
            PixelArr2[i][j] = (char*)malloc(3);
        }
    }
    if (ThreadNum > height)
    {
        ThreadNum = height;
    }
    fclose(f);
    ReadFile(argv[1]);
    start=clock();
    int res;
    pthread_t a_thread[ThreadNum];
    void* thread_result;
    for (int thread = 0; thread < ThreadNum; thread++)
    {
        res = pthread_create(&(a_thread[thread]), NULL,
        thread_function, (void*)(intptr_t)thread);
        if (res != 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }
while(A!=ThreadNum){}
    CreateFile(&ThreadNum);
    end=clock();
    printf("%lf\n",(end-start)*1.0/ (CLOCKS_PER_SEC));
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            free(PixelArr[i][j]);
        }
        free(PixelArr[i]);
    }
    free(PixelArr);
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            free(PixelArr2[i][j]);
        }
        free(PixelArr2[i]);
    }
    free(PixelArr2);
}
