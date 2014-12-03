#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proto.h"

#define MAX_LABELS 30 

struct label labels[MAX_LABELS];
int n_labels = 0;

void replace_labels(void *elem)
{
    switch (TYPE(elem)) {
    case TYPE_NOT:
    case TYPE_TFLIPFLOP:
        while (TYPE(UN_INPUT(elem)) == TYPE_LABEL) {
            if (LABEL_VALUE(UN_INPUT(elem)) == NULL) {
                fprintf(stderr, "replace_labels crash: undefined label\n");
                free_element(circuit);
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
                free_element(circuit);
                exit(-1);
            } 
            BINOP_INPUT_A(elem) = LABEL_VALUE(BINOP_INPUT_A(elem));
        }

        while (TYPE(BINOP_INPUT_B(elem)) == TYPE_LABEL) {
            if (LABEL_VALUE(BINOP_INPUT_B(elem)) == NULL) {
                fprintf(stderr, "replace_labels crash: undefined label\n");
                free_element(circuit);
                exit(-1);
            }
            BINOP_INPUT_B(elem) = LABEL_VALUE(BINOP_INPUT_B(elem));
        }
         
        replace_labels(BINOP_INPUT_A(elem));
        replace_labels(BINOP_INPUT_B(elem));
        return;
    }
}

struct label *get_label(char *str) 
{
    int i;

    for (i = 0; i < n_labels; i++)
        if (strncmp(str, labels[i].label, 10) == 0)
            return &labels[i];

    /* create new label if not found */
    n_labels++;
    strncpy(labels[i].label, str, 10);
    labels[i].type = TYPE_LABEL; 
    labels[i].value = NULL;

    return &labels[i];
}
