#ifndef STACK_H
#define STACK_H

#include "stack.h"

int final_stack (Stack **stack) {
    while (*stack != NULL) {
        Stack *old_node = *stack;
        *stack = (*stack)->next;
        free(old_node->data);
        free(old_node);
    }
    return 0;
}

int stack_pop(Stack **stack, void *res, size_t size_rez)
{
    Stack *node;
    node = *stack;
    *stack = (*stack)->next;
    for (int i = 0; i < size_rez; ++i) {
        ((char *)res)[i] = ((char *)(node->data))[i];
    }
    free(node->data);
    free(node);
    return 0;
}

int stack_pop_start(Stack **stack, void *res, size_t size_rez)
{
    Stack *node;
    Stack *node2;
    node2 = *stack;
    node = node2;
    int counter = 0;
    if (node2 == NULL) {
        return 1;
    }
    while(node2->next != NULL) {
        counter += 1;
        node = node2;
        node2 = node2->next;
    }
    if (counter == 0) {
        *stack = NULL;
    }
    node->next = NULL;
    for (int i = 0; i < size_rez; ++i) {
        ((char *)res)[i] = ((char *)(node2->data))[i];
    }
    free(node2->data);
    free(node2);
    return 0;
}

int stack_push(Stack **stack, const void *data, size_t size) 
{
    
    Stack *new_node;
    if ((new_node = malloc(sizeof(Stack))) == NULL) {
        printf("Memory error");
        exit(3);
    }
    new_node->next = *stack;
    if ((new_node->data = malloc(size)) == NULL) {
        printf("Memory error");
        exit(3);
    }
    for (int i = 0; i < size; ++i) {
        ((char *)(new_node->data))[i] = ((char *)data)[i];
    };
    new_node->size = size;
    *stack= new_node;
    return 0;
    
}

int stack_push_start(Stack **stack, const void *data, size_t size) 
{
    
    Stack *new_node;
    new_node = *stack;
    if (new_node != NULL) {
        while(new_node->next != NULL) {
            new_node = new_node->next;
        }
        if ((new_node->next = malloc(sizeof(Stack))) == NULL) {
            printf("Memory error");
            exit(3);
        }
        if ((new_node->next->data = malloc(size)) == NULL) {
            printf("Memory error");
            exit(3);
        }
        new_node->next->next = NULL;
        for (int i = 0; i < size; ++i) {
            ((char *)(new_node->next->data))[i] = ((char *)data)[i];
        };
        new_node->next->size = size;
    }
    else {
        if ((new_node = malloc(sizeof(Stack))) == NULL) {
            printf("Memory error");
            exit(3);
        }
        new_node->next = NULL;
        if ((new_node->data = malloc(size)) == NULL) {
            printf("Memory error");
            exit(3);
        }
        for (int i = 0; i < size; ++i) {
            ((char *)(new_node->data))[i] = ((char *)data)[i];
        };
        new_node->size = size;
        *stack = new_node;
    }
    return 0;
}

#endif /* STACK_H */