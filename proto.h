#ifndef PROTO_H
#define PROTO_H

#define TYPE_NOT       1
#define TYPE_OR        2
#define TYPE_AND       3
#define TYPE_TFLIPFLOP 4
#define TYPE_INPUT     5
#define TYPE_LABEL     6

#define TYPE(p)            (((struct not *)p)->type)
#define UN_INPUT(p)        (((struct not *)p)->input)
#define BINOP_INPUT_A(p)   (((struct binary_op *)p)->input_a)
#define BINOP_INPUT_B(p)   (((struct binary_op *)p)->input_b)
#define TFLIPFLOP_STATE(p) (((struct tflipflop *)p)->former_state)
#define INPUT(p)           (((struct input *)p)->value)
#define INPUT_LABEL(p)     (((struct input *)p)->label)
#define LABEL_NAME(p)      (((struct label *)p)->label)
#define LABEL_VALUE(p)     (((struct label *)p)->value)

#define TOKEN_LEFTP  1
#define TOKEN_RIGHTP 2
#define TOKEN_ATOM   3

/* tokens.c */
int get_token(char *bufp, char *token_buf, int n, int *retpos);

/* parse.c */
void *parse(char *bufp, int *retpos); 

/* main.c */
extern void *circuit; 
void free_element(void *elem);

/* evaluate.c */
char evaluate(void *circuit);

/* label.c */
struct label *get_label(char *str);
void replace_labels(void *elem);

/* inputs.c */
void get_input_variables(char *buf);
struct input *get_input(char *str);
void set_inputs(char *buf);

/* data structures */
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
    char type;
    void *input;
    char former_state;
};

struct input {
    char type;
    char label[10];
    char value;
};

struct label {
    char type;
    char label[10];
    void *value;
};

#endif
