#include "parser.h"
#define MAX_STR_LEN 1024
enum {ELEM_SIZE  =  sizeof(Elem_size),  CHAR_SIZE = sizeof(char),   FUNC_SIZE = sizeof(Calculate_elem)};
enum { MAXSTR = 1024};

int count_polis(Token token);

int 
main(void)
{
    Token token;
    char str[MAXSTR + 2];
    int flag;
    printf("Enter the string with the expression.\n");

    fgets(str, MAXSTR, stdin);
    size_t size = strlen(str);
    int bg_flag = 0;
    if ((flag = analyze(str, size, &token, &bg_flag)) != 0) {
        free(token.data);
        return flag;
    }

    if (bg_flag == 1) {
        if (fork() == 0) {
            return count_polis(token);  
        }
        free(token.data);
        return 0;
    }
    
    return count_polis(token);
}

int
count_polis(Token token)
{
    int flag;
    Polis polis;
    if ((flag = parce_start(&polis, &token)) != 0) {
        free(polis.data);
        free(token.data);
        return flag;
    }
    free(token.data);
    void *result = NULL;
    size_t size_rez = 0;
    if ((flag = calculate_polis(&polis, &result, &size_rez, 0)) != 0) {
        free(polis.data);
        free(token.data);
        return flag;
    };
    if (result == NULL) {
        free(polis.data);
        return 0;
    }
    if (*(Calculate_elem *)&((char *)(result))[ELEM_SIZE] == count_output_data) {
            for (int i = ELEM_SIZE + FUNC_SIZE; i < size_rez - 1; ++i) {
                printf("%c", ((char *)(result))[i]);
            }
    } else {
        if (fork() == 0) {
            Polis polis;
            polis.data = result;
            polis.size = size_rez;
            char *arg[MAX_STR_LEN];
            int curr_element = 0;
            char str[MAX_STR_LEN];
            make_arg_array(polis, arg, &curr_element, str);
            if (execvp(arg[0], arg) == -1) {
                perror("Execvp sec");
                exit(1);
            }
        }
        wait(0);
    }
    free(result);
    free(polis.data);
    return 0;
}