#ifndef PARSER_H
#define PARSER_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void StringCheck(char*InputString,int* NameLength,int* CountArg)
{
	int i=0;
	while(InputString[i]!=' '&&InputString[i]!='\0')
	{
		*NameLength+=1;
		i++;
	}
	while(InputString[i]!='\0')
	{
		i++;
		while(InputString[i]!=' '&&InputString[i]!='\0')
		{
			i++;
		}
		*CountArg+=1;
	}	
}

void ArgsLength (int* LengthArgs,char*InputString)
{
	int i=0;
	int j=0;
	while(InputString[i]!=' '&&InputString[i]!='\0')
	{
		i++;
	}
	while(InputString[i]!='\0')
	{
		i++;
		int k=0;
		while(InputString[i]!=' '&&InputString[i]!='\0')
		{
			i++;
			k++;
		
		}
		LengthArgs[j]=k;
		j++;
	}	
}

void ParsingString(char* Name,char** Arg,char*InputString)
{
	int i=0;
	int j=0;
	while(InputString[i]!=' '&&InputString[i]!='\0')
	{
		Name[i]=InputString[i];
		Arg[j][i]=InputString[i];
		i++;
	}
	Name[i]='\0';
	Arg[j][i]='\0';
	j++;
	while(InputString[i]!='\0')
	{
		i++;
		int k=0;
		//Arg[j]=(char*)malloc(255);
		while(InputString[i]!=' '&&InputString[i]!='\0')
		{
			Arg[j][k]=InputString[i];
			k++;
			i++;
		}
		Arg[j][k]='\0';
		j++;
	}
}

void XOR_Crypt(char* Mes, char*Key,char *crypt)
{
	for(int i=0;i<=sympos(Mes);i++)
	{
		crypt[i]=Mes[i]^Key[i%(strlen(Key))];
	}
}

int sympos(char*Mes)
{
	int c;
	for(int i=0;i<255;i++)
	{
		if(Mes[i]!='\0')
		{
			c=i;	
		}
	}
	return c;
}

// void StringCheck(char*InputString,int* NameLength,int* CountArg){ 
//       int i=0; 
//       while(InputString[i]!=' '&&InputString[i]!='\0'){ 
//             *NameLength+=1; 
//             i++; 
//       }
// }


#endif /* FILES_H */