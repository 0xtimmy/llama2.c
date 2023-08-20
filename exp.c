#include<stdio.h>
#include "win.h"

void printbin(char *x, int truesize) {

    for(int j = truesize-1; j >= 0; j--) {
        for(int i = 7; i >= 0; i--) {
            fprintf(stdout, "%d", (*(x+j) & (1 << i)) >> i );
        }
    }
}

// adds 1 and 4 and outputs 5
/*
    asm ("add %1, %2\n\t"
        "mov %2, %0"
        : "=r" (out)
        : "r" (1), "r" (4)
    );
*/

void asm_exp2(float *x) {
    float out;
    int intx = (int)*x;
    float decx = *x - (float)intx;

    asm ("shl $23, %1\n\t" //shift x 23 digits (into the exp of a floating point)
        "add $1065353216, %1\n\t" // add the shifted x into a fp representation of 1
        "mov %1, %0"
        : "=r" (*x)
        : "r" (intx)
    );

    *x = *x * (1.0f + decx);
}

float* c_exp2(float *x) {
    int intx = (int)*x;
    float base = 1.0f;
    fprintf(stdout, "sizeof(x) = %d\n", sizeof(*x));
    fprintf(stdout, "x = %f\n", *x);
    fprintf(stdout, "(int)x = %d\n", (int)*x);

    fprintf(stdout, "(in binary) x = ");
    printbin((char *)x, sizeof(*x));
    fprintf(stdout, "\n");

    fprintf(stdout, "base = %f\n", base);
    fprintf(stdout, "(in binary) base = ");
    printbin((char *)&base, sizeof(base));
    fprintf(stdout, "\n\n");

    fprintf(stdout, "(in binary) (int)x = %d = ", intx);
    printbin((char *)&intx, sizeof(intx));
    fprintf(stdout, "\n\n");

    int intxshft = intx << 23;

    fprintf(stdout, "(in binary) (int)x << 23 = ");
    printbin((char *)&(intxshft), sizeof(intxshft));
    fprintf(stdout, "\n\n");

    *((int *)x) = *((int *)&base) + intxshft;

    fprintf(stdout, "y = %f\n", *x);
    fprintf(stdout, "(in binary) y = ");
    printbin((char *)x, sizeof(*x));
    fprintf(stdout, "\n\n");

    return x;
}

void main() {
    float x = -4.569379;

    c_exp2(&x);

    /*
    fprintf(stdout, "x = %f = ", x);
    printbin((char *)&x, sizeof(x));
    fprintf(stdout, "\n");
    asm_exp2(&x);
    fprintf(stdout, "exp2(x) = %f = ", x);
    printbin((char *)&x, sizeof(x));
    fprintf(stdout, "\n");
    */

    return 0;
}