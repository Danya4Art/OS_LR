#ifndef FILES_H
#define FILES_H

#include "files.h"

unsigned char PixelVal(unsigned char A[3][3])
{
    double Gx = (A[2][0] + 2 * A[2][1] + A[2][2]) - (A[0][0] + 2 * A[0][1] + A[0][2]);
    double Gy = (A[0][2] + 2 * A[1][2] + A[2][2]) - (A[0][0] + 2 * A[1][0] + A[2][0]);
    double f = sqrt(Gx * Gx + Gy * Gy);
    return (unsigned char)f;
}

void SobelKer(int StartInd, int EndInd)
{
    unsigned char PixelR[3][3];
    unsigned char PixelG[3][3];
    unsigned char PixelB[3][3];
    for (int j = (StartInd); j <= (EndInd); j++)
    {
        for (int i = 0; i < (width); i++)
        {
            for (int k = 0; k < 3; k++)
            {
                for (int h = 0; h < 3; h++)
                {
                    if (i + h >= (width))
                    {
                        PixelR[k][h] = 0;
                        PixelG[k][h] = 0;
                        PixelB[k][h] = 0;
                    }
                    else
                    {
                        if (j + k >= height)
                        {
                            PixelR[k][h] = 0;
                            PixelG[k][h] = 0;
                            PixelB[k][h] = 0;
                        }
                        else
                        {
                            PixelR[k][h] = PixelArr[j + k][i + h][0];
                            PixelG[k][h] = PixelArr[j + k][i + h][1];
                            PixelB[k][h] = PixelArr[j + k][i + h][2];
                        }
                    }
                }
            }
            PixelArr2[j][i][0] = PixelVal(PixelR);
            PixelArr2[j][i][1] = PixelVal(PixelG);
            PixelArr2[j][i][2] = PixelVal(PixelB);
        }
    }
}

void CreateFile(int* ThreadNum)
{
    char buf[256];
    sprintf(buf, "%d.ppm", *ThreadNum);
    FILE* ff = fopen(buf, "wb");
    memset(buf, '\0', 256);
    sprintf(buf,"P6\n%d%d\n255\n",width,height);
    fwrite(buf, sizeof(unsigned char), strlen(buf), ff);
    for (int i = 0; i <=(height); i++)
    {
        for (int j = 0; j < (width); j++)
        {
            fwrite(&PixelArr2[i][j][0], sizeof(unsigned char), 1, ff);
            fwrite(&PixelArr2[i][j][1], sizeof(unsigned char), 1, ff);
            fwrite(&PixelArr2[i][j][2], sizeof(unsigned char), 1, ff);
        }
    }
    fclose(ff);
}

void ReadFile(char* filename)
{
    FILE* f = fopen(filename, "rb");
    char buf[256];
    memset(buf, '\0', 256);
    sprintf(buf, "P6\n%d %d\n255\n", width, height);
    fseek(f, strlen(buf), SEEK_SET);
    int row_padded = ((width) * 3 + 3) & (~3);
    unsigned char* data = (unsigned char*)malloc(3);
    for (int i = 0; i <= (height); i++)
    {
        for (int j = 0; j < (width); j++)
        {
            fread(data, sizeof(unsigned char), 3, f);
            PixelArr[i][j][0] = data[0];
            PixelArr[i][j][1] = data[1];
            PixelArr[i][j][2] = data[2];
        }
    }
    free(data);
    fclose(f);
}

void* thread_function(void* arg)
{
    int t = (intptr_t)arg;
    int StartInd = t * height / ThreadNum;
    int EndInd;
    if (t == ThreadNum - 1)
    {
        EndInd = height;
    }
    else
    {
        EndInd = (t + 1) * height / ThreadNum;
    }
    SobelKer(StartInd,EndInd);
    A++;
    pthread_exit(NULL);
}

#endif /* FILES_H */