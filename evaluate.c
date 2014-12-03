#include <stdio.h>
#include <stdlib.h>

#include "proto.h"

char evaluate(void *circuit)

    switch (TYPE(circuit)) {
    case TYPE_NOT:
        return !(evaluate(UN_INPUT(circuit)));
    case TYPE_OR:
        return ((evaluate(BINOP_INPUT_A(circuit))) || (evaluate(BINOP_INPUT_B(circuit))));
    case TYPE_AND:
        return ((evaluate(BINOP_INPUT_A(circuit))) && (evaluate(BINOP_INPUT_B(circuit))));
    case TYPE_TFLIPFLOP:
        if (TFLIPFLOP_STATE(circuit) == 1)
            TFLIPFLOP_STATE(circuit) = (evaluate(UN_INPUT(circuit)) == 0);
        else
            TFLIPFLOP_STATE(circuit) = (evaluate(UN_INPUT(circuit)) == 1);
        return TFLIPFLOP_STATE(circuit);
    case TYPE_INPUT:
        return INPUT(circuit);
    default:
        fprintf(stderr, "evaluate crash: label should have been replaced\n");
        free_element(circuit);
        exit(-1);
    }
}
