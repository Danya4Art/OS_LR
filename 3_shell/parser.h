#include "polis.h"
#include <setjmp.h>
#include <string.h>
//size is the number of elements in data array.
//*data is the array of bytes, each element representet by: size_of_element|bytes_of_data
typedef struct {
    size_t size;
    void *data;
} Token;

//Diving input data into tokens
int analyze(const void *data, const size_t size, Token *tokens, int *var_flag);

//Conversion to the Polish notation
int parce_start(Polis *polis, const Token *tokens);

int parce_binary(Polis *polis, const Token *tokens, void **elem, 
              size_t *curr_polis_size, size_t *curr_token_size);

int parce_symbol(Polis *polis, const Token *tokens, void **elem, 
                 size_t *curr_polis_size, size_t *curr_token_size);

int take_next_elem(const Token *tokens, void **elem, size_t *curr_size);

int put_elem_to_polis (Polis *polis, const void *elem, size_t *curr_size);


//Operations
int count_pipeline(const void *elem, Elem_size size, Stack **stack, Polis *pol); // |
int count_and_or(const void *elem, Elem_size size, Stack **stack, Polis *pol); // &&, ||
int count_subshell(const void *elem, Elem_size size, Stack **stack, Polis *pol); // (
int end_subshell(const void *elem, Elem_size size, Stack **stack, Polis *pol); // )
int count_redirection(const void *elem, Elem_size size, Stack **stack, Polis *pol); // < > >>
int count_command(const void *elem, Elem_size size, Stack **stack, Polis *pol); // input/otput files and commands
int count_argument(const void *elem, Elem_size size, Stack **stack, Polis *pol);
int count_output_data(const void *elem, Elem_size size, Stack **stack, Polis *pol);

//Work functions
int take_polis_from_stack(Stack **stack, Polis *first_arg);
int make_arg_array(Polis polis, char *arg[], int *curr_element, char str[]);
int write_to_ch(int fd, void *buf, size_t size);