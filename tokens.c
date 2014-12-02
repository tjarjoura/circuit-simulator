#include <ctype.h>
#include "proto.h"

#define is_illegal(c) ((c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == '.') || (c == '\"') || isdigit(c))
#define is_term(c) ((c == ' ') || (c == '\n') || (c == '\t') || (c == ')') || (c == '('))

static int get_atom(char *bufp, char *token_buf, int n)
{
    int len = 0; 

    while (!is_term(*bufp)) {
        *token_buf++ = *bufp++;
        if (++len >= n)
            return -1; /* token_buf too small! */
    }

    *token_buf = '\0';

    return len;
}

int get_token(char *bufp, char *token_buf, int n, int *retpos) 
{
    int rv; 

    while (isspace(*bufp)) {
        bufp++;
        (*retpos)++;
    }

    if ((*bufp) == '(') {
        token_buf[0] = '(', token_buf[1] = '\0';
        *retpos += 1;
        return TOKEN_LEFTP;

    } else if ((*bufp) == ')') {
        token_buf[0] = ')', token_buf[1] = '\0';
        *retpos += 1; 
        return TOKEN_RIGHTP;
    
    } else if ((*bufp) == '\0') 
        return 0; /* eol */
    
    else if ((rv = get_atom(bufp, token_buf, n)) < 0)
        return -1; /* signal error */

    *retpos += rv;
    return TOKEN_ATOM;
}
