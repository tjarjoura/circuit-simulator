#include <stdio.h>

#include "proto.h"

static struct not *parse_not(char *bufp, int *retpos)
{
    int rv, i = 0;
    char token_buf[20];
    void *input;
    
    rv = get_token(bufp + i, token_buf, 20, &i);
    if (rv == TOKEN_LEFTP) {
        if ((input = parse(bufp + i, &i)) == NULL)
            return NULL;
    
    } else if (rv == TOKEN_ATOM) {
        if ((input = get_input(token_buf)) == NULL)
            input = get_label(token_buf);

    } else {
        fprintf(stderr, "parse_not crash: unexpected token\n");
        return NULL;
    }

    if ((rv = get_token(bufp + i, token_buf, 20, &i)) != TOKEN_RIGHTP) {
        free_element(input);    
        fprintf(stderr, "parse_not crash: unexpected token\n");
        return NULL;
    }
    
    struct not *parsed = malloc(sizeof(struct not));
    parsed->type = TYPE_NOT;
    parsed->input = input; 
    
    *retpos += i;
    return parsed;
}

static struct binary_op *parse_or(char *bufp, int *retpos);
static struct binary_op *parse_and(char *bufp, int *retpos);
static struct tflipflop *parse_tflipflop(char *bufp, int *retpos);
static void *parse_label(char *bufp, int *retpos);

static int type(char *atom)
{
    if (strncmp(str, "not", 4) == 0)
        return TYPE_NOT;
    if (strncmp(str, "or", 3) == 0)
        return TYPE_OR;
    if (strncmp(str, "and", 4) == 0)
        return TYPE_AND;
    if (strncmp(str, "tflipflop", 10) == 0)
        return TYPE_TFLIPFLOP;
    if (get_input(str) != NULL) 
        return TYPE_INPUT;
    else
        return TYPE_LABEL;
}

void *parse(char *bufp, int *retpos)
{
    int rv, type, i = 0;
    char token_buf[20];
    void *parsed;

    /*first grab the first atom */ 
    if ((rv = get_token(bufp + i, token_buf, 20, &i)) != TOKEN_ATOM) {
        fprintf(stderr, "parse crash: expected atom\n");
        return NULL;
    }
    
    type = type(token_buf);
    
    switch (type) {
    case TYPE_NOT:
        parsed = parse_not(bufp + i, &i);
        *retpos += i;
        return parsed;
    case TYPE_OR:
        parsed = parse_or(bufp + i, &i);
        *retpos += i;
        return parsed;
    case TYPE_AND:
        parsed = parse_and(bufp + i, &i);
        *retpos += i;
        return parsed;
    case TYPE_TFLIPFLOP:
        parsed = parse_tflipflop(bufp + i, &i);
        *retpos += i;
        return parsed;
    case TYPE_LABEL:
        parsed = parse_label(bufp + i, &i);
        *retpos += i;
        return parsed;
    default:
        fprintf(stderr, "parse crash: expected gate or label\n");
        exit(-1);
    }
}
