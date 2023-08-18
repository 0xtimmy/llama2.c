#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>

long time_in_ms() {
    // return time in milliseconds, for benchmarking the model speed
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

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

int main() {

    int TEST_SIZE = 1000000;

    long exp_start = time_in_ms();
    float* nums = malloc(sizeof(float) * TEST_SIZE);
    for(int i = 0; i < TEST_SIZE; i++) {
        nums[i] = ((float)rand()) / 32767.0;
    }

    for(int i = 0; i < TEST_SIZE; i++) {
        expf(nums[i]);
    }
    long exp_end = time_in_ms();
    for(int i = 0; i < TEST_SIZE; i++) {
        asm_exp2(nums + i);
    }
    long exp2_end = time_in_ms();

    fprintf(stdout, "exp took on average  %ldms\n", (exp_end - exp_start));
    fprintf(stdout, "exp2 took on average %ldms\n", (exp2_end - exp_end));

    return 0;
}