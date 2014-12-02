#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "proto.h"

struct input *input_vector;
int n_inputs;

void *output;
char final_val; 

char evaluate(void *circuit);
void *parse(char *bufp);

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

