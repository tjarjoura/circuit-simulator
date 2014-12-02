#include <string.h>

#include "proto.h"

#define MAX_LABELS 30 

struct label labels[MAX_LABELS];
int n_labels = 0;

struct label *get_label(char *str) 
{
    int i;

    for (i = 0; i < n_labels; i++)
        if (strncmp(str, labels[i].label, 10) == 0)
            return &labels[i];

    /* create new label if not found */
    n_labels++;
    strncpy(labels[i].name, str 10);
    labels[i].type = TYPE_LABEL; 
    labels[i].value = NULL;

    return &labels[i];
}
