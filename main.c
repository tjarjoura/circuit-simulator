#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "proto.h"

void *circuit;
char final_val; 

void free_element(void *elem)
{
    if ((TYPE(elem) == TYPE_INPUT) || (TYPE(elem) == TYPE_LABEL))
        return;
    
    if ((TYPE(elem) == TYPE_NOT) || (TYPE(elem) == TYPE_TFLIPFLOP)) {
        free_element(UN_INPUT(elem));
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

void get_circuit(char *bufp)
{
    int rv, i = 0; 
    char token_buf[20];

    rv = get_token(bufp + i, token_buf, 20, &i);
    switch (rv) {
    case TOKEN_LEFTP:
        circuit = parse(bufp + i, &i);
        break;
    case TOKEN_ATOM:
        if ((circuit = get_input(token_buf)) != NULL)
            break;
    default:
        fprintf(stderr, "get_circuit crash: invalid spec\n");
        exit(-1);
    }
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
    
    get_circuit(line_buffer);
    replace_labels(circuit);

    /* evaluate until eof */
    while (fgets(line_buffer, 1024, stdin) != NULL) {
        set_inputs(line_buffer);
        final_val = evaluate(circuit);
    }

    printf("Result: %d\n", final_val);

    free_element(circuit);
    exit(0);
}

