#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "proto.h"

struct input *input_vector;
int n_inputs;

struct label *labels;
int n_labels; 

void *output;
char final_val; 

char evaluate(void *circuit);

void free_element(void *elem)
{
    if ((TYPE(elem) == TYPE_INPUT) || (TYPE(elem) == TYPE_LABEL))
        return;
    
    if ((TYPE(elem) == TYPE_NOT) || (TYPE(elem) == TYPE_TFLIPFLOP)) {
        free_element(NOT_INPUT(elem));
        free(elem);
        return;
    } 
        
    if ((TYPE(elem) == TYPE_OR) || (TYPE(elem) == TYPE_AND)) {
        free_element(BINOP_INPUT_A(elem));
        free_element(BINOP_INPUT_B(elem));
        free(elem);
        return;
    }
}

void get_output(char *bufp)
{
    int rv, i = 0; 
    char token_buf[20];

    rv = get_token(bufp + i, token_buf, 20, &i);
    switch (rv) {
    case TOKEN_LEFTP:
        output = parse(bufp + i, &i);
        break;
    case TOKEN_ATOM:
        if ((output = get_input(token_buf)) != NULL)
            break;
    default:
        fprintf(stderr, "get_output crash: invalid spec\n");
        exit(-1);
    }

    replace_labels(output);
}

/* TODO: RECURSE */
void replace_labels(void *elem)
{
    switch(TYPE(elem)) {
    case TYPE_NOT:
    case TYPE_TFLIPFLOP:
        while (TYPE(UN_INPUT(elem)) == TYPE_LABEL) {
            if (LABEL_VALUE(UN_INPUT(elem)) == NULL) {
                fprintf(stderr, "replace_labels crash: undefined label\n");
                free_element(output);
                exit(-1);
            } 
            UN_INPUT(elem) = LABEL_VALUE(UN_INPUT(elem)); 
        }

        replace_labels(UN_INPUT(elem));
        return;
    
    case TYPE_OR:
    case TYPE_AND:
        while (TYPE(BINOP_INPUT_A(elem)) == TYPE_LABEL) {
            if (LABEL_VALUE(BINOP_INPUT_A(elem)) == NULL) {
                fprintf(stderr, "replace_labels crash: undefined label\n");
                free_element(output);
                exit(-1);
            } 
            BINOP_INPUT_A(elem) = LABEL_VALUE(BINOP_INPUT_A(elem));
        }

        while (TYPE(BINOP_INPUT_B(elem)) == TYPE_LABEL) {
            if (LABEL_VALUE(BINOP_INPUT_B(elem)) == NULL) {
                fprintf(stderr, "replace_labels crash: undefined label\n");
                free_element(output);
                exit(-1);
            }
            BINOP_INPUT_B(elem) = LABEL_VALUE(BINOP_INPUT_B(elem));
        }
        return;
}

void get_input_variables(char *bufp)
{
    int i, j = 0, k, n = 0, in_word = 0;

    /* count how much memory we need to allocate */
    for (i = 0; bufp[i] != '\0'; i++) {
        if (!isspace(bufp[i]) && !in_word) {
            n++;
            in_word = 1;
        } else if (isspace(bufp[i]) && in_word)
            in_word = 0;
    }

    input_vector = malloc(sizeof(struct input) * n); 
    n_inputs = n;

    in_word = k = 0;
    for (i = 0; bufp[i] != '\0'; i++) {
        if (isspace(bufp[i]) && in_word) { /* done copying variable name */
           in_word = 0;
           input_vector[j].type = TYPE_INPUT;
           input_vector[j++].name[k] = '\0';
           k = 0;

        } else if (!isspace(bufp[i]) && !in_word) 
           in_word = 1;

        if (in_word)
            input_vector[j].name[k++] = bufp[i]; 
    }
}

struct input *get_input(char *label)
{
    int i;

    for (i = 0; i < n_inputs; i++)
        if (strncmp(label, input_vector[i].label, 10) == 0)
            return &input_vector[i];
    
    return NULL;
}

int main(int argc, char **argv)
{
    char line_buffer[1024];

    /* get input variables */
    if (fgets(line_buffer, 1024, stdin) == NULL) {
        fprintf(stderr, "fgets: crash\n");
        exit(-1);
    }
    
    get_input_variables(line_buffer);

    /* get circuit spec */ 
    if (fgets(line_buffer, 1024, stdin) == NULL) {
        fprintf(stderr, "fgets: crash\n");
        exit(-1);
    }
    
    get_output(line_buffer);

    /* evaluate until eof */
    while (fgets(line_buffer, 1024, stdin) != NULL) {
        set_output(line_buffer);
        final_val = evaluate();
    }

    printf("Result: %d\n", final_val);

    /* INSERT CLEANUP */
    free(input_vector);
    exit(0);
}

