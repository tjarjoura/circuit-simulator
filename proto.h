#ifndef PROTO_H
#define PROTO_H

#define TYPE_NOT       1
#define TYPE_OR        2
#define TYPE_AND       3
#define TYPE_TFLIPFLOP 4
#define TYPE_INPUT     5
#define TYPE_LABEL     6

#define KIND(p)            (((not *)p)->kind)
#define NOT_INPUT(p)       (((not *)p)->input)
#define BINOP_INPUT_A(p)   (((binary_op *)p)->input_a)
#define BINOP_INPUT_B(p)   (((binary_op *)p)->input_b)
#define TFLIPFLOP_INPUT(p) (((binary_op *)p)->input_a)
#define TFLIPFLOP_STATE(p) (((binary_op *)p)->former_state)
#define INPUT(p)           (((input *)p)->value)
#define INPUT_LABEL(p)     (((input *)p)->label)
#define LABEL_NAME(p)      (((label *)p)->label)
#define LABEL_VALUE(p)     (((label *)p)->value)

#define TOKEN_LEFTP  1
#define TOKEN_RIGHTP 2
#define TOKEN_ATOM   3

int get_token(char *bufp, char *token_buf, int n, int *retpos);

struct not {
    char type;
    void *input;
};

struct binary_op {
    char type;
    void *input_a;
    void *input_b;
};

struct tflipflop {
    char kind;
    void *input;
    char former_state;
};

struct input {
    char kind;
    char label[10];
    char value;
};

struct label {
    char kind;
    char *label;
    void *value;
};

#endif
