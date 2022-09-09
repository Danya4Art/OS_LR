
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<string.h> 
 
void XOR_Crypt(char* Mes, char*Key,char* crypt); 
void ParsingString(char* Name,char** Arg,char*InputString); 
void StringCheck(char*InputString,int* NameLength,int* CountArg); 
void ArgsLength (int* LengthArgs,char*InputString); 
int sympos(char*Mes); 
 
int main(int argc,char**argv){ 
    if(argc!=3){ 
          printf("Wrong number of parameters"); 
          return 0; 
    } 
    int Pipes[2]; 
    char Mes[255]; 
    char key[255]; 
    char crypt[255]; 
    memset(Mes, '\0', 255); 
    memset(key, '\0', 255); 
    memset(crypt, '\0', 255); 
    
    int NameLengthFst=0; 
    int CountArgFst=0; 
    StringCheck(argv[1],&NameLengthFst,&CountArgFst); 
    int* LengthArgs=(int *)malloc(CountArgFst*sizeof(int)); 
    ArgsLength(LengthArgs,argv[1]); 
    char *Mes_Name=(char*)malloc(NameLengthFst+1); 
    memset(Mes_Name, '\0', NameLengthFst); 
    char *Arg_Mes=(char*)malloc(CountArgFst+2); 
    Arg_Mes[0]=(char*)malloc(NameLengthFst+1); 
    memset(Arg_Mes[0], '\0', NameLengthFst); 
    for(int i=1;i<CountArgFst+1;i++){ 
        Arg_Mes[i]=(char*)malloc(LengthArgs[i-1]+1); 
        memset(Arg_Mes[i], '\0', LengthArgs[i-1]+1); 
    } 
    Arg_Mes[CountArgFst+1]=(char *)0; 
    
    int NameLengthSec=0; 
    int CountArgSec=0; 
    StringCheck(argv[2],&NameLengthSec,&CountArgSec); 
    int* LengthArgsSec=(int *)malloc(CountArgSec*sizeof(int)); 
    ArgsLength(LengthArgsSec,argv[2]); 
    char *key_Name=(char*)malloc(NameLengthSec+1); 
    memset(key_Name, '\0', NameLengthSec); 
    char* Arg_Key=(char*)malloc(CountArgSec+2); 
    Arg_Key[0]=(char*)malloc(NameLengthSec+1); 
    memset(Arg_Key[0], '\0', NameLengthSec); 
    for(int i=1;i<CountArgSec+1;i++){ 
        Arg_Key[i]=(char*)malloc(LengthArgsSec[i-1]+1); 
        memset(Arg_Key[i], '\0', LengthArgsSec[i-1]); 
    } 
    Arg_Key[CountArgSec+1]=(char *)0; 
    ParsingString(Mes_Name,Arg_Mes,argv[1]); 
    ParsingString(key_Name,Arg_Key,argv[2]); 
    if(pipe(Pipes)==-1){ 
        printf("Pipe error"); 
        return 0; 
    } 
    int fork_result=fork(); 
    if (fork_result == -1) { 
        fprintf(stderr, "Fork failure"); 
        return 0; 
    } 
    if(fork_result==0){ 
        close(1); 
        dup(Pipes[1]); 
        if(execvp(Mes_Name,Arg_Mes)==-1){ 
            //printf("_Error_"); 
            write(Pipes[1],"_Error_",7); 
        } 
    } 
    else 
    { 
        read(Pipes[0],Mes,255); 
        if(strcmp("_Error_",Mes)==0){ 
            printf("Failed process: %s",argv[1]); 
            free(Mes_Name); 
            free(key_Name); 
            for(int i=0;i<CountArgFst+2;i++){ 
                free(Arg_Mes[i]); 
            } 
            for(int i=0;i<CountArgSec+2;i++){ 
                free(Arg_Key[i]); 
            } 
            free(Arg_Mes); 
            free(Arg_Key); 
            free(LengthArgs); 
            free(LengthArgsSec); 
            return 0; 
        } 
        fork_result=fork(); 
        if (fork_result == -1) { 
            fprintf(stderr, "Fork failure"); 
            free(Mes_Name); 
            free(key_Name); 
            for(int i=0;i<CountArgFst+2;i++){ 
                free(Arg_Mes[i]); 
            } 
            for(int i=0;i<CountArgSec+2;i++){ 
                free(Arg_Key[i]); 
            } 
            free(Arg_Mes); 
            free(Arg_Key); 
            free(LengthArgs); 
            free(LengthArgsSec); 
            return 0; 
        } 
        if(fork_result==0){ 
            close(1); 
            dup(Pipes[1]);  
            if(execvp(key_Name,Arg_Key)==-1){ 
                write(Pipes[1],"_Error_",7); 
            } 
        } 
    } 
    read(Pipes[0],key,255); 
    if(strcmp("_Error_",key)==0){ 
        printf("Failed process: %s",argv[2]); 
        free(Mes_Name); 
        free(key_Name); 
        for(int i=0;i<CountArgFst+2;i++){ 
              free(Arg_Mes[i]); 
        } 
        for(int i=0;i<CountArgSec+2;i++)
        { 
              free(Arg_Key[i]); 
        } 
        free(Arg_Mes); 
        free(Arg_Key); 
        free(LengthArgs); 
        free(LengthArgsSec); 
        return 0; 
    } 
    
    printf("\nMes: %s\nKey:%s\n",Mes,key); 
    XOR_Crypt(Mes,key,crypt); 
    printf("crypt: %s\n",crypt); 
    memset(Mes, '\0', 255); 
    XOR_Crypt(crypt,key,Mes); 
    printf("crypt: %s\n",Mes); 
    free(Mes_Name); 
    free(key_Name); 
    for(int i=0;i<CountArgFst+2;i++){ 
          free(Arg_Mes[i]); 
    } 
    for(int i=0;i<CountArgSec+2;i++){ 
          free(Arg_Key[i]); 
    } 
    free(Arg_Mes); 
    free(Arg_Key); 
    free(LengthArgs); 
    free(LengthArgsSec); 
    return 0; 
} 

void StringCheck(char*InputString,int* NameLength,int* CountArg){ 
      int i=0; 
      while(InputString[i]!=' '&&InputString[i]!='\0'){ 
            *NameLength+=1; 
            i++; 
      }
}
