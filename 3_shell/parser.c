#ifndef PARSER_H
#define PARSER_H

#include "parser.h"
#define ALLOC_STEP 100
#define MAX_STR_LEN 1024
#define BUFFER_SIZE 4096
#define CHECK_SIZE(checking_size, max_size, data)   if ((checking_size) >= max_size) {\
                                                        max_size += ALLOC_STEP; \
                                                        data = realloc(data, max_size * CHAR_SIZE);\
                                                    } 
#define SAFE_FUNC_CALL(func, error, ...)    if ((int flag = func(__VA_ARGS_)) != 0) {\
                                                fprintf(stderr, error);\
                                                return flag;\
                                            }
#define NEW_ELEMENT(var, index, operation, ch)    SIZE_POINTER(var, index) = CHAR_SIZE;\
                                                    *(Calculate_elem *)&(((char *)var)\
                                                        [index + ELEM_SIZE]) = operation;\
                                                    CHAR_POINTER(var, index + FUNC_SIZE + ELEM_SIZE) = ch;\
                                                    index += CHAR_SIZE + ELEM_SIZE + FUNC_SIZE;
#define FORK_FREE free(first_arg.data);\
                  free(sec_arg.data);\
                  final_stack(stack);\
                  free(pol->data);
#define CLOSE_WRITER close(fd_for_result[1]);\
                     close(fd_for_result[0]);\
                     close(fd_for_pipeline[0]);\
                     dup2(fd_for_pipeline[1], 1);
#define CLOSE_READER close(fd_for_result[0]);\
                     close(fd_for_pipeline[1]);\
                     dup2(fd_for_pipeline[0], 0);\
                     dup2(fd_for_result[1], 1);
#define EXEC(var, fr) char *arg[MAX_STR_LEN];\
                        int curr_element = 0;\
                        char str[MAX_STR_LEN];\
                        make_arg_array(var, arg, &curr_element, str);\
                        fr;\
                        if (execvp(arg[0], arg) == -1) {\
                            perror("Execvp sec");\
                            exit(1);\
                        }
#define PROLOG  Polis first_arg, sec_arg;\
                first_arg.data = malloc(ALLOC_STEP);\
                sec_arg.data = malloc(ALLOC_STEP);\
                take_polis_from_stack(stack, &first_arg);\
                take_polis_from_stack(stack, &sec_arg);

enum {ELEM_SIZE  =  sizeof(Elem_size),  CHAR_SIZE = sizeof(char),   FUNC_SIZE = sizeof(Calculate_elem)};

jmp_buf ebuf;

int analyze(const void *data, const size_t size, Token *tokens, int *bg_flag)
{
    int curr_size = 0, curr_elem_size = 0, curr_external_offset = 0, curr_element_offset = 0;
    Calculate_elem func;
    if ((tokens->data = calloc(ALLOC_STEP, CHAR_SIZE)) == NULL) {
        return 1;
    }
    tokens->size = ALLOC_STEP;
    //0 - (, 1 - ), 2 - |, 3 - ||, 4 - <, 5 - >, 6 - &, 7 - &&, 8 - >>, 9 - commands, 10 - args
    int flag = -1, error_flag = 0, is_ready = 0; 
    char num_string[256];
    char c;
    char old_par = '\0';
    int parentheses_counter = 0;
    for (int i = 0; i < size; ++i) {
        c = ((char*)data)[i];
        if (c == '\n') {
            if (i == 0) {
                printf("Empty string\n");
                return 5;
            }
            break; 
        }
        int old_flag = flag;
        //Defining current charachter
        if (c == ' ') {
            is_ready = 1;
            continue;
        }
        if ((old_par == '&') && (c != '&') && (flag  == 6)) {
            perror("Wrong &");
            return 1;
        }

        if (c == '(') {
            is_ready = 0;
            parentheses_counter += 1;
            switch (flag) {
                case 1: 
                case 4: 
                case 5: 
                case 6:
                case 8: 
                case 9:
                case 10: 
                    perror("Wrong (");
                    return 1;
                default:
                    break; 

            }
            flag = 0;
            CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
            NEW_ELEMENT(tokens->data, curr_size, count_subshell, c);
            curr_element_offset = 0;

        } else if (c == ')') {
            is_ready = 0;
            parentheses_counter -= 1;
            if (parentheses_counter < 0) {
                perror("Wrong parenthesis balanse");
                return 2;
            }
            switch (flag) {
                case 0: 
                case 2: 
                case 3: 
                case 4: 
                case 5: 
                case 6:
                case 7:
                case 8:
                    perror("Wrong )");
                    return 1;
                default:
                    break; 

            }
            flag = 1;
            CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
            NEW_ELEMENT(tokens->data, curr_size, end_subshell, c);
            curr_element_offset = 0;
        }
        else if (c == '&') {
            is_ready = 0;
            switch (flag) {
                case -1:
                case 0: 
                case 3: 
                case 4: 
                case 5:
                case 7:
                case 8:
                    fprintf(stderr, "Wrong %c or %s\n", '&', "&&");
                    return 1;
                default:
                    break; 
            }
            if (flag != 6) {
                *bg_flag = 1;
                flag = 6;
            } else {
                *bg_flag = 0;
                flag = 7;
                CHECK_SIZE(curr_size + 2 * CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                NEW_ELEMENT(tokens->data, curr_size, count_and_or, c);
                SIZE_POINTER(tokens->data, curr_size - FUNC_SIZE - CHAR_SIZE - ELEM_SIZE) += CHAR_SIZE;
                //((Elem_size *)(tokens->data))[curr_size - FUNC_SIZE - CHAR_SIZE - ELEM_SIZE] += CHAR_SIZE;
                ((char *)(tokens->data))[curr_size] = c;
                curr_element_offset = 0;
                curr_size += CHAR_SIZE;
            }
        } else if (c == '|') {
            is_ready = 0;
            switch (flag) {
                case -1:
                case 0: 
                case 3: 
                case 4: 
                case 5:
                case 6:
                case 7:
                case 8:
                    fprintf(stderr, "Wrong %c or %s\n", '|', "||");
                    return 1;
                default:
                    break; 
            }
            if (flag != 2) {
                flag = 2;
                CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                NEW_ELEMENT(tokens->data, curr_size, count_pipeline, c);
                curr_element_offset = CHAR_SIZE + ELEM_SIZE + FUNC_SIZE;
            } else {
                flag = 3;
                CHECK_SIZE(curr_size + CHAR_SIZE, tokens->size, tokens->data);
                *(Calculate_elem *)&(((char *)tokens->data)[curr_size - curr_element_offset + ELEM_SIZE]) = count_and_or;
                SIZE_POINTER(tokens->data, curr_size - curr_element_offset) += CHAR_SIZE;
                //((Elem_size *)(tokens->data))[curr_size - curr_element_offset] += CHAR_SIZE;
                ((char *)(tokens->data))[curr_size] = c;
                curr_element_offset += CHAR_SIZE;
                curr_size += CHAR_SIZE;
            }
        } else if ((c == '<') || (c == '>')) {
            is_ready = 0;
            switch (flag) {
                case -1:
                case 0: 
                case 2: 
                case 3: 
                case 4: 
                case 6:
                case 7:
                case 8:
                    fprintf(stderr, "Wrong %c\n", c);
                    return 1;
                default:
                    break; 

            }
            if (c == '<') {
                flag = 4;
                CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                NEW_ELEMENT(tokens->data, curr_size, count_redirection, c);
            } else if (flag != 5) {
                flag = 5;
                CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                NEW_ELEMENT(tokens->data, curr_size, count_redirection, c);
                curr_element_offset = CHAR_SIZE + ELEM_SIZE + FUNC_SIZE;
            } else {
                flag = 8;
                CHECK_SIZE(curr_size + CHAR_SIZE, tokens->size, tokens->data);
                SIZE_POINTER(tokens->data, curr_size - curr_element_offset) += CHAR_SIZE;
                //((Elem_size *)(tokens->data))[curr_size - curr_element_offset] += CHAR_SIZE;
                ((char *)(tokens->data))[curr_size] = c;
                curr_element_offset += CHAR_SIZE;
                curr_size += CHAR_SIZE;
            }
            old_par = c;
        } else { //commands and args
            if (((is_ready == 1) && (flag == 9)) || (flag == 10)) { //args
                if ((flag != 10) && (flag != 9)) {
                    fprintf(stderr, "Wrong args");
                        return 1;
                }
                if ((flag == 10) && (is_ready == 0)) {
                    is_ready = 0;
                    CHECK_SIZE(curr_size + CHAR_SIZE, tokens->size, tokens->data);
                    SIZE_POINTER(tokens->data, curr_size - curr_element_offset) += CHAR_SIZE;
                    //((Elem_size *)(tokens->data))[curr_size - curr_element_offset] += CHAR_SIZE;
                    ((char *)(tokens->data))[curr_size] = c;
                    curr_element_offset += CHAR_SIZE;
                    curr_size += CHAR_SIZE;
                    old_par = c;
                } else {
                    flag = 10;
                    is_ready = 0;
                    CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                    NEW_ELEMENT(tokens->data, curr_size, count_argument, c);
                    curr_element_offset = CHAR_SIZE + ELEM_SIZE + FUNC_SIZE;
                }

            }
            else {
                switch (flag) {
                    case 1: 
                    case 6: 
                        fprintf(stderr, "Wrong command");
                        return 1;
                    default:
                        break; 
                }
            
                if (flag != 9)  {
                    flag = 9;
                    is_ready = 0;
                    CHECK_SIZE(curr_size + CHAR_SIZE + ELEM_SIZE + FUNC_SIZE, tokens->size, tokens->data);
                    NEW_ELEMENT(tokens->data, curr_size, count_command, c);
                    curr_element_offset = CHAR_SIZE + ELEM_SIZE + FUNC_SIZE;
                    old_par = c;
                } else {
                    is_ready = 0;
                    CHECK_SIZE(curr_size + CHAR_SIZE, tokens->size, tokens->data);
                    SIZE_POINTER(tokens->data, curr_size - curr_element_offset) += CHAR_SIZE;
                    //((Elem_size *)(tokens->data))[curr_size - curr_element_offset] += CHAR_SIZE;
                    ((char *)(tokens->data))[curr_size] = c;
                    curr_element_offset += CHAR_SIZE;
                    curr_size += CHAR_SIZE;
                    old_par = c;
                }
            }
            
        }
    }
    switch(flag) {
        case 0: 
        case 2: 
        case 3: 
        case 4: 
        case 5: 
        case 7:
        case 8:
            fprintf(stderr, "Wrong last operation");
            return 1;
        default:
            break;
    }
    if (parentheses_counter != 0) {
        perror("Wrong parenthesis balanse");
        return 2;
    }
    tokens->data = realloc(tokens->data, curr_size);
    //printf("SIZE %d, %d\n", curr_size, ELEM_SIZE);
    tokens->size = curr_size;
    return 0;
}

int parce_start(Polis *polis, const Token *tokens)
{
    void *elem;
    size_t curr_polis_size = 0, curr_token_size = 0; 
    polis->data = calloc(ALLOC_STEP, CHAR_SIZE);
    polis->size = 100;
    take_next_elem(tokens, &elem, &curr_token_size);
    int i = setjmp(ebuf);
    if (i != 0) {
        return i;
    }
    parce_binary(polis, tokens, &elem, &curr_polis_size, &curr_token_size);
    polis->size = curr_polis_size;
    return 0;
}


int parce_binary(Polis *polis, const Token *tokens, void **elem, size_t *curr_polis_size, size_t *curr_token_size)
{
    parce_symbol(polis, tokens, elem, curr_polis_size, curr_token_size);
    while((FUNC_POINTER(*elem, ELEM_SIZE) == count_and_or) ||
          (FUNC_POINTER(*elem, ELEM_SIZE) == count_pipeline) ||
          (FUNC_POINTER(*elem, ELEM_SIZE) == count_redirection)) {
        void *elem_in_queue = *elem;
        take_next_elem(tokens, elem, curr_token_size);
        parce_symbol(polis, tokens, elem, curr_polis_size, curr_token_size);
        put_elem_to_polis(polis, elem_in_queue, curr_polis_size);
    }
    return 0;
}

int parce_symbol(Polis *polis, const Token *tokens, void **elem, 
             size_t *curr_polis_size, size_t *curr_token_size)
{
    char c = ((char *)*elem)[ELEM_SIZE+FUNC_SIZE];
    if((((char *)(*elem))[ELEM_SIZE+FUNC_SIZE] == '(') 
        && (*(Calculate_elem *)&((char *)(*elem))[ELEM_SIZE] == count_subshell)) {
        put_elem_to_polis(polis, *elem, curr_polis_size);
        take_next_elem(tokens, elem, curr_token_size);
        parce_binary(polis, tokens, elem, curr_polis_size, curr_token_size);
        if(((char * )(*elem))[ELEM_SIZE+FUNC_SIZE] != ')') {
            printf("Wrong left parenthesis\n");
            longjmp(ebuf, 1);
        } else {
            while (1) {
                put_elem_to_polis(polis, *elem, curr_polis_size);
                if (take_next_elem(tokens, elem, curr_token_size) == 1) {
                    break;
                }
                if (FUNC_POINTER(*elem, ELEM_SIZE) != count_argument) {
                    break;
                }
            }
        }
    } else {
            while (1) {
                put_elem_to_polis(polis, *elem, curr_polis_size);
                if (take_next_elem(tokens, elem, curr_token_size) == 1) {
                    break;
                }
                if (FUNC_POINTER(*elem, ELEM_SIZE) != count_argument) {
                    break;
                }
            }
        }
    return 0;
}

int take_next_elem(const Token *tokens, void **elem, size_t *curr_token_size)
{
    if (*curr_token_size > tokens->size - 1) {
        return 1;
    }
    *elem = &((char*)tokens->data)[*curr_token_size];

    *curr_token_size += *(Elem_size *)*elem + FUNC_SIZE + ELEM_SIZE;

    return 0;
}

int put_elem_to_polis (Polis *polis, const void *elem, size_t *curr_polis_size) {

    if ((*curr_polis_size + SIZE_POINTER(elem, 0) + FUNC_SIZE + ELEM_SIZE) 
        >= polis->size) {
        polis->size += ALLOC_STEP; 
        polis->data = realloc(polis->data, polis->size*CHAR_SIZE);
    }

    for (int i = 0; i < SIZE_POINTER(elem, 0) + FUNC_SIZE + ELEM_SIZE; ++i ) {
            CHECK_SIZE(*curr_polis_size, polis->size, polis->data)
        ((char *)polis->data)[*curr_polis_size] = ((char *)elem)[i];
        *curr_polis_size += CHAR_SIZE;
    }
    return 0;
}

int count_pipeline(const void *elem, Elem_size size, Stack **stack, Polis *pol)
{
    PROLOG;
    int fd_for_pipeline[2], fd_for_result[2];
    pipe(fd_for_pipeline);
    pipe(fd_for_result);
    int pid1 = 0, pid2;
    char exit_status; 
    //count command
    if (*(Calculate_elem *)&((char *)(first_arg.data))[ELEM_SIZE] == count_command) {
        if ((pid1 = fork()) == 0) {
            CLOSE_WRITER;
            EXEC(first_arg, FORK_FREE);
        }
    } else if (FUNC_POINTER(first_arg.data, ELEM_SIZE) == count_subshell){
        if ((pid1 = fork()) == 0) {
            CLOSE_WRITER;
            void *res;
            size_t size_res;
            calculate_polis(&first_arg, &res, &size_res, 1);
            FORK_FREE;
            if (*(Calculate_elem *)&((char *)(res))[ELEM_SIZE] == count_output_data) {
                write_to_ch(fd_for_pipeline[1], res, size_res - 1 - ELEM_SIZE - FUNC_SIZE);
            } else {
                if (fork() == 0) {
                    Polis polis;
                    polis.data = res;
                    polis.size = size_res;
                    EXEC(polis, free(res));
                }
                wait(0);
            }
            close(fd_for_pipeline[1]);
            free(res);
            exit(0);
        }
    } else {
            write_to_ch(fd_for_pipeline[1], first_arg.data, first_arg.size - ELEM_SIZE - FUNC_SIZE - 1);
    }
    if (FUNC_POINTER(sec_arg.data, ELEM_SIZE) == count_subshell) {
        if ((pid2 = fork()) == 0) {
            CLOSE_READER;
            void *res;
            size_t size_res;
            calculate_polis(&sec_arg, &res, &size_res, 1);
            FORK_FREE;
            if (*(Calculate_elem *)&((char *)(res))[ELEM_SIZE] == count_output_data) {
                write_to_ch(fd_for_result[1], res, size_res - 1 - ELEM_SIZE - FUNC_SIZE);
            } else {
                if (fork() == 0) {
                    Polis polis;
                    polis.data = res;
                    polis.size = size_res;
                    EXEC(polis, free(res));
                }
                wait(0);
            }
            free(res);
            exit(0);
        }
    } else if ((pid2 = fork()) == 0) {
        CLOSE_READER;
        EXEC(sec_arg, FORK_FREE);
    }
    
    close(fd_for_pipeline[0]);
    close(fd_for_pipeline[1]);
    close(fd_for_result[1]);
    void *rez = malloc(ALLOC_STEP);
    SIZE_POINTER(rez, 0) = CHAR_SIZE;
    FUNC_POINTER(rez, ELEM_SIZE) = count_output_data;
    int curr_size = ELEM_SIZE + FUNC_SIZE, n, max_size = ALLOC_STEP;
    char buf[BUFFER_SIZE];
    while ((n = read(fd_for_result[0], buf, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < n; ++i) {
            if ((curr_size) >= max_size) {
                max_size += ALLOC_STEP;
                rez = realloc(rez, max_size * CHAR_SIZE);
            }
            SIZE_POINTER(rez, 0) += CHAR_SIZE;
            ((char *)(rez))[curr_size] = buf[i];
            curr_size += CHAR_SIZE;
        }
    }
    close(fd_for_result[0]);
    int status1;
    int status2;
    if (pid1 != 0) {
        waitpid(pid1, &status1, 0);
    }
    waitpid(pid2, &status2, 0);
    exit_status = WEXITSTATUS(status2);
    CHECK_SIZE(curr_size, max_size, rez);

    ((char *)(rez))[curr_size] = exit_status;

    stack_push(stack, rez, curr_size + 1);

    free(rez);
    free(first_arg.data);
    free(sec_arg.data);
    return 0;
}

int count_and_or(const void *elem, Elem_size size, Stack **stack, Polis *pol) 
{
    PROLOG;
    int pid1, pid2;
    int flag, flag_is_output_data = 0;
    if (CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == '&') {
        flag = 1;
    } else {
        flag = 2;
    }
    char exit_status; 
    if (*(Calculate_elem *)&((char *)(first_arg.data))[ELEM_SIZE] == count_command) {
        if ((pid1 = fork()) == 0) {
            EXEC(first_arg, FORK_FREE);
        }
    } else if (FUNC_POINTER(first_arg.data, ELEM_SIZE) == count_subshell){
        if ((pid1 = fork()) == 0) {
            void *res;
            size_t size_res;
            calculate_polis(&first_arg, &res, &size_res, 1);
            FORK_FREE;
            if (*(Calculate_elem *)&((char *)(res))[ELEM_SIZE] == count_output_data) {
                flag_is_output_data = 1;
                exit_status = CHAR_POINTER(res, SIZE_POINTER(res, 0) + FUNC_SIZE + ELEM_SIZE - 1);
            } else {
                if (fork() == 0) {
                    EXEC(first_arg, free(res));
                }
                wait(0);
            }
            free(res);
            exit(0);
        }
    } else {
        for (int i = ELEM_SIZE + FUNC_SIZE; i < first_arg.size - 1; ++i) {
            printf("%c", ((char *)(first_arg.data))[i]);
        }
        flag_is_output_data = 1;
        exit_status = CHAR_POINTER(first_arg.data, SIZE_POINTER(first_arg.data, 0) + FUNC_SIZE + ELEM_SIZE - 1);
    }
    if (flag_is_output_data == 0) {
        int status;
        waitpid(pid1, &status, 0);
        exit_status = WEXITSTATUS(status);
    }
    flag_is_output_data = 1;
    if (((exit_status == 0) && (flag == 1)) || ((exit_status != 0) && (flag == 2))){
        flag_is_output_data = 0;
        if (FUNC_POINTER(sec_arg.data, ELEM_SIZE) == count_subshell) {
            if ((pid2 = fork()) == 0) {
                void *res;
                size_t size_res;
                calculate_polis(&sec_arg, &res, &size_res, 1);
                FORK_FREE;
                if (*(Calculate_elem *)&((char *)(res))[ELEM_SIZE] == count_output_data) {
                    for (int i = ELEM_SIZE + FUNC_SIZE; i < size_res - 1; ++i) {
                        printf("%c", ((char *)(res))[i]);
                    }   
                } else {
                    if (fork() == 0) {
                        EXEC(sec_arg, free(res));
                    }
                    wait(0);
                }
                free(res);
                exit(0);
            }
        } else if ((pid2 = fork()) == 0) {
            EXEC(sec_arg, FORK_FREE);
        }
    } else {
        for (int i = ELEM_SIZE + FUNC_SIZE; i < sec_arg.size - 1; ++i) {
            printf("%c", ((char *)(sec_arg.data))[i]);
        }
    }
    if (flag_is_output_data == 0) {
        int status2;
        waitpid(pid2, &status2, 0);
        exit_status = WEXITSTATUS(status2);
    }
    else {
        exit_status = 0;
    }
    void *rez = malloc(ALLOC_STEP*CHAR_SIZE);
    SIZE_POINTER(rez, 0) = CHAR_SIZE;
    *(Calculate_elem *)&(((char *)rez)[ELEM_SIZE]) = count_output_data;
    int curr_size = ELEM_SIZE + FUNC_SIZE;
    ((char *)(rez))[curr_size] = exit_status;
    curr_size += 1;
    stack_push(stack, rez, curr_size);
    free(rez);
    free(first_arg.data);
    free(sec_arg.data);
    return 0;
}


int count_redirection(const void *elem, Elem_size size, Stack **stack, Polis *pol) 
{
    PROLOG;
    char file_name[MAX_STR_LEN];
    int i = 0;
    for (i = 0; i < SIZE_POINTER(sec_arg.data, 0); ++i) {
        file_name[i] = CHAR_POINTER(sec_arg.data, FUNC_SIZE + ELEM_SIZE + i);
    }
    file_name[i] = '\0';
    int fd;
    int flag;// 1 - <, 2 - >, 3 - >>
    if (size == 2) {
        flag = 3;
        if ((fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1) {
            perror("File open");
            return 1;
        };
    } else {
        if (CHAR_POINTER(elem, ELEM_SIZE + FUNC_SIZE) == '<') {
            flag = 1;
            if ((fd = open(file_name, O_RDONLY)) == -1) {
                perror("File open");
                return 1;
            };
        } else {
            flag = 2;
            if ((fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
                perror("File open");
                return 1;
            };
        }
    }

    int pid;
    int fd_result[2];
    pipe(fd_result);
    int pid_flag = 0;
    if (FUNC_POINTER(first_arg.data, ELEM_SIZE) == count_output_data) {
        pid_flag = 1;
        write_to_ch(fd, first_arg.data, first_arg.size - ELEM_SIZE - FUNC_SIZE - 1);
    } else if (FUNC_POINTER(first_arg.data, ELEM_SIZE) == count_subshell) {
        if ((pid = fork()) == 0) {
            if (flag == 1) {
                dup2(fd, 0);
                close(fd_result[0]);
                dup2(fd_result[1], 1);
            } else {

                dup2(fd, 1);
                close(fd_result[0]);
                close(fd_result[1]);
            }
            void *res;
            size_t size_res;
            calculate_polis(&first_arg, &res, &size_res, 1);
            FORK_FREE;
            if (*(Calculate_elem *)&((char *)(res))[ELEM_SIZE] == count_output_data) {
                write_to_ch(fd_result[1], res, size_res);
            } else {
                if (fork() == 0) {
                    Polis polis;
                    polis.data = res;
                    polis.size = size_res;
                    EXEC(polis, free(res));
                }
                wait(0);
            }
            free(res);
            exit(0);
        }
        wait(0);
    }
    else if ((pid = fork()) == 0) {
        if (flag == 1) {
            dup2(fd, 0);
            close(fd_result[0]);
            dup2(fd_result[1], 1);
        } else {

            dup2(fd, 1);
            close(fd_result[0]);
            close(fd_result[1]);
        }
        EXEC(first_arg, FORK_FREE);
    }
    close(fd);
    close(fd_result[1]);
    int exit_status = 0;
    if (pid_flag == 0) {
        int status;
        waitpid(pid, &status, 0);
        exit_status = WEXITSTATUS(status);
    }
    
    if (flag == 1) {
        void *rez = malloc(ALLOC_STEP * CHAR_SIZE);
        SIZE_POINTER(rez, 0) = CHAR_SIZE;
        *(Calculate_elem *)&(((char *)rez)[ELEM_SIZE]) = count_output_data;
        int curr_size = ELEM_SIZE + FUNC_SIZE, n, max_size = ALLOC_STEP;
        char buf[BUFFER_SIZE];
        while ((n = read(fd_result[0], buf, BUFFER_SIZE)) > 0) {
            for (int i = 0; i < n; ++i) {
                CHECK_SIZE(curr_size, max_size, rez);
                SIZE_POINTER(rez, 0) += CHAR_SIZE;
                ((char *)(rez))[curr_size] = buf[i];
                curr_size += CHAR_SIZE;
            }
        }
        close(fd_result[1]);
        ((char *)(rez))[curr_size] = exit_status;
        curr_size += 1;
        stack_push(stack, rez, curr_size);
        free(rez);
    }
    free(first_arg.data);
    free(sec_arg.data);
    return 0;
}

int write_to_ch(int fd, void *buf, size_t size)
{
    size_t curr_size = size, readed = 0;
    do {
        int max_read = BUFFER_SIZE <= curr_size ? BUFFER_SIZE : curr_size;
        int n = write(fd, &CHAR_POINTER(buf, ELEM_SIZE + FUNC_SIZE + readed), curr_size);
        curr_size -= n;
        readed += n;
    } while(curr_size > 0);
    return 0;
}
int make_arg_array(Polis polis, char *arg[], int *curr_element, char str[])
{
    int i = 0;
    size_t curr_size = ELEM_SIZE + FUNC_SIZE, prev_size = 0;
    int curr_string_char = 0, curr_len = 0;
    while (curr_size <= polis.size) {
        for (i = 0; i < SIZE_POINTER(polis.data, prev_size); ++i) {
            str[curr_string_char] = ((char *)(polis.data))[curr_size];
            curr_size += 1;
            curr_string_char += 1;
            curr_len +=1;
        }
        str[curr_string_char] = '\0';
        curr_len += 1;
        curr_string_char += 1;
        arg[*curr_element] = &(str[curr_string_char - curr_len]);
        *curr_element += 1;
        prev_size = curr_size;
        curr_size += ELEM_SIZE + FUNC_SIZE;
        curr_len = 0;
    }
    arg[*curr_element] = NULL;
    return 0;
}

int take_polis_from_stack(Stack **stack, Polis *first_arg)
{
    size_t curr_polis_size = 0, curr_element = 1;
    int flag = 0;
    void *res;
    size_t size;
    first_arg->size = ALLOC_STEP;
    size = (*stack)->size;
    res = malloc(size);
    stack_pop(stack, res, size);
    if (FUNC_POINTER(res, ELEM_SIZE) == count_subshell) {
        put_elem_to_polis(first_arg, res, &curr_polis_size);
        int parenthesis_counter = 1;
        free(res);
        while(*stack != NULL) {
            size = (*stack)->size;
            res = malloc(size);
            stack_pop(stack, res, size);
            if (FUNC_POINTER(res, ELEM_SIZE) == count_subshell) {
                parenthesis_counter += 1;
            }
            if (FUNC_POINTER(res, ELEM_SIZE) == end_subshell) {
                parenthesis_counter -= 1;
                if (parenthesis_counter == 0) {
                    put_elem_to_polis(first_arg, res, &curr_polis_size);
                    free(res);
                    break;
                }
            }
            put_elem_to_polis(first_arg, res, &curr_polis_size);
            free(res);
        }
    } else if (FUNC_POINTER(res, ELEM_SIZE) == count_command) {
        put_elem_to_polis(first_arg, res, &curr_polis_size);
        free(res);
        while(*stack != NULL) {
            size = (*stack)->size;
            res = malloc(size);
            stack_pop(stack, res, size);
            if (FUNC_POINTER(res, ELEM_SIZE) != count_argument) {
                stack_push(stack, res, size);
                free(res);
                break;
            }

            put_elem_to_polis(first_arg, res, &curr_polis_size);
            free(res);
        }
    } else if (FUNC_POINTER(res, ELEM_SIZE) == count_output_data) {
        put_elem_to_polis(first_arg, res, &curr_polis_size);
        free(res);
        
    }
    first_arg->size = curr_polis_size;
    return 0;
}

int count_subshell(const void *elem, Elem_size size, Stack **stack, Polis *pol) 
{
    stack_push_start(stack, elem, size + ELEM_SIZE + FUNC_SIZE);
    return 0;
}

int end_subshell(const void *elem, Elem_size size, Stack **stack, Polis *pol)
{
    stack_push_start(stack, elem, size + ELEM_SIZE + FUNC_SIZE);
    return 0;
}

int count_argument(const void *elem, Elem_size size, Stack **stack, Polis *pol)
{
    stack_push_start(stack, elem, size + ELEM_SIZE + FUNC_SIZE);
    return 0;
}

int count_command(const void *elem, Elem_size size, Stack **stack, Polis *pol) 
{
    stack_push_start(stack, elem, size + ELEM_SIZE + FUNC_SIZE);
    return 0;
}

int count_output_data(const void *elem, Elem_size size, Stack **stack, Polis *pol)
{
    return 0;
}

#endif /* PARSER_H */