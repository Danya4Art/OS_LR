#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

typedef struct Stack{
    void *data;
    struct Stack *next;
    size_t size;
} Stack;

int final_stack (Stack **stack);

int stack_pop(Stack **stack, void *res, size_t size_rez);

int stack_pop_start(Stack **stack, void *res, size_t size_rez);

int stack_push(Stack **stack, const void *data, size_t size);

int stack_push_start(Stack **stack, const void *data, size_t size);