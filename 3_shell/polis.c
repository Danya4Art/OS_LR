#include "polis.h"

#define SAFE(call) do { \
                    flag = call; \
                    if (flag) { \
                    goto FINALLY; \
                    }\
                }\
                while(0)

enum {ELEM_SIZE  =  sizeof(Elem_size),  CHAR_SIZE = sizeof(char),   FUNC_SIZE = sizeof(Calculate_elem)};

int
calculate_polis(Polis *pol, void **result, size_t *size_rez, int parenthesis_flag)
{
    Stack *stack = NULL;
    int flag;
    int parenthesis_counter = 0;
    for (size_t current_size = 0; current_size < pol->size; 
         current_size += ELEM_SIZE + SIZE_POINTER(pol->data, current_size)
                       + FUNC_SIZE) {
        void *elem = &((char*)pol->data)[current_size];
        if (((CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == '(') || (CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == ')')) 
            && (parenthesis_flag == 1) && ((current_size == 0) ||((current_size + ELEM_SIZE + ((char *)pol->data)[current_size] 
                       + FUNC_SIZE) >= (pol->size - 1)))) {
            continue;
        }
        if (CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == '(') {
            parenthesis_counter += 1;
        }
        if (CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == ')') {
            parenthesis_counter -= 1;
        }
        Calculate_elem func = *(Calculate_elem *)&((char *)elem)[ELEM_SIZE];
        if (parenthesis_counter == 0) {
            SAFE(func(elem, SIZE_POINTER(pol->data, current_size), &stack, pol));

        }
        else {
            stack_push_start(&stack, elem, SIZE_POINTER(pol->data, current_size) + ELEM_SIZE + FUNC_SIZE);
        }
    }

    int size;
    *result = malloc(1);

    *size_rez = 0;
    if (stack == NULL) {
        free(*result);
        *result = NULL;
    }
    while (stack != NULL) {

        size = (stack)->size * CHAR_SIZE;

        *size_rez += size;
        *result = realloc(*result, *size_rez);
        stack_pop(&stack, &((*result)[*size_rez - size]), size);
    }
    return 0;

FINALLY: 
    final_stack(&stack);
    return flag;
}