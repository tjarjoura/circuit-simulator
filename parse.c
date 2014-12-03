#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proto.h"

static void *parse_un(char *bufp, int *retpos, int t_flipflop)
{
    int rv, i = 0;
    char token_buf[20];
    void *input, *parsed;
   
    /* get input */ 
    rv = get_token(bufp + i, token_buf, 20, &i);
    if (rv == TOKEN_LEFTP) {
        if ((input = parse(bufp + i, &i)) == NULL)
            return NULL;
    
    } else if (rv == TOKEN_ATOM) {
        if ((input = get_input(token_buf)) == NULL)
            input = get_label(token_buf);

    } else {
        fprintf(stderr, "parse_un crash: unexpected token\n");
        return NULL;
    }

    /* check for terminating right parenthesis */
    if ((rv = get_token(bufp + i, token_buf, 20, &i)) != TOKEN_RIGHTP) {
        free_element(input);    
        fprintf(stderr, "parse_un crash: unexpected token\n");
        return NULL;
    }
  
    /* set up data structure */ 
    parsed = t_flipflop ? malloc(sizeof(struct tflipflop)) : malloc(sizeof(struct not));
    
    TYPE(parsed) = t_flipflop ? TYPE_TFLIPFLOP : TYPE_NOT;
    UN_INPUT(parsed) = input;
    if(t_flipflop)
       TFLIPFLOP_STATE(parsed) = 0;
    
    *retpos += i;
    return parsed;
}

static struct binary_op *parse_bin(char *bufp, int *retpos, int type) 
{
    int rv, i = 0;
    char token_buf[20];
    void *input_a, *input_b, *parsed;

    /* set first input */
    rv = get_token(bufp + i, token_buf, 20, &i); 
    if (rv == TOKEN_LEFTP) {
        if ((input_a = parse(bufp + i, &i)) == NULL)
            return NULL;
    
    } else if (rv == TOKEN_ATOM) {
        if ((input_a = get_input(token_buf)) == NULL)
            input_a = get_label(token_buf);

    } else {
        fprintf(stderr, "parse_bin crash: unexpected token\n");
        return NULL;
    }
   
    /* set second input */
    rv = get_token(bufp + i, token_buf, 20, &i); 
    if (rv == TOKEN_LEFTP) {
        if ((input_b = parse(bufp + i, &i)) == NULL) {
            free_element(input_a);
            return NULL;
        }
    } else if (rv == TOKEN_ATOM) {
        if ((input_b = get_input(token_buf)) == NULL)
            input_b = get_label(token_buf);

    } else {
        free_element(input_a);
        fprintf(stderr, "parse_bin crash: unexpected token\n");
        return NULL;
    }

    /* check for terminating right parenthesis */
    if ((rv = get_token(bufp + i, token_buf, 20, &i)) != TOKEN_RIGHTP) {
        free_element(input_a);
        free_element(input_b);
        fprintf(stderr, "parse_bin crash: unexpected token\n");
        return NULL;
    }

    /* set up data structure */
    parsed = malloc(sizeof(struct binary_op));

    TYPE(parsed) = type;
    BINOP_INPUT_A(parsed) = input_a;
    BINOP_INPUT_B(parsed) = input_b;

    *retpos += i;
    return parsed;
}

static void *parse_label(char *bufp, int *retpos, char *label)
{
    struct label *l = get_label(label);
    void *parsed;
    int rv, i = 0;
    char token_buf[20];
    
    rv = get_token(bufp + i, token_buf, 20, &i); 
    if (rv == TOKEN_LEFTP) {
        if ((parsed = parse(bufp + i, &i)) == NULL)
            return NULL;
    
    } else if (rv == TOKEN_ATOM) {
        if ((parsed = get_input(token_buf)) == NULL)
            parsed = get_label(token_buf);

    } else {
        fprintf(stderr, "parse_bin crash: unexpected token\n");
        return NULL;
    }

    l->value = parsed;
    *retpos += i;
    return parsed;
}

static int get_type(char *atom)
{
    if (strncmp(atom, "not", 4) == 0)
        return TYPE_NOT;
    if (strncmp(atom, "or", 3) == 0)
        return TYPE_OR;
    if (strncmp(atom, "and", 4) == 0)
        return TYPE_AND;
    if (strncmp(atom, "tflipflop", 10) == 0)
        return TYPE_TFLIPFLOP;
    if (get_input(atom) != NULL) 
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
    
    type = get_type(token_buf);
    
    switch (type) {
    case TYPE_NOT:
        parsed = parse_un(bufp + i, &i, 0);
        *retpos += i;
        return parsed;
    case TYPE_OR:
        parsed = parse_bin(bufp + i, &i, TYPE_OR);
        *retpos += i;
        return parsed;
    case TYPE_AND:
        parsed = parse_bin(bufp + i, &i, TYPE_AND);
        *retpos += i;
        return parsed;
    case TYPE_TFLIPFLOP:
        parsed = parse_un(bufp + i, &i, 1);
        *retpos += i;
        return parsed;
    case TYPE_LABEL:
        parsed = parse_label(bufp + i, &i, token_buf);
        *retpos += i;
        return parsed;
    default:
        fprintf(stderr, "parse crash: expected gate or label\n");
        exit(-1);
    }
}
