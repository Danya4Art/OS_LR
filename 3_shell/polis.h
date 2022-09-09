#include "stack.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE_POINTER(var, index) *(Elem_size *)&((char *)(var))[index]
#define FUNC_POINTER(var, index) *(Calculate_elem *)&((char *)(var))[index]
#define CHAR_POINTER(var, index) ((char *)var)[index]


typedef struct 
{
    size_t size;
    void *data;
} Polis;

typedef long long Elem_size;

typedef int (*Calculate_elem)(const void *elem, Elem_size size, Stack **stack, Polis *pol);

int calculate_polis(Polis *pol, void **result, size_t *size_rez, int parenthesis_flag);
