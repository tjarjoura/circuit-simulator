#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "proto.h"

#define MAX_INPUTS 14

struct input input_vector[MAX_INPUTS];
int n_inputs = 0;

void get_input_variables(char *bufp)
{
    int i, j = 0, in_word = 0;

    for (i = 0; bufp[i] != '\0'; i++) {
        if (isspace(bufp[i]) && in_word) { /* done copying variable name */
           in_word = 0;
           input_vector[n_inputs].type = TYPE_INPUT;
           input_vector[n_inputs++].label[j] = '\0';
           j = 0;

        } else if (!isspace(bufp[i]) && !in_word) 
           in_word = 1;

        if (in_word)
            input_vector[n_inputs].label[j++] = bufp[i]; 
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

void set_inputs(char *buf)
{
    int i, j = 0;

    for (i = 0; buf[i] != '\0'; i++)
        if (!isspace(buf[i])) {
            if (j >= n_inputs) {
                fprintf(stderr, "set_inputs crash(): too many input values given\n");
                free_element(circuit);
                exit(-1);
            }
            input_vector[j++].value = buf[i] - '0';
        }
}
