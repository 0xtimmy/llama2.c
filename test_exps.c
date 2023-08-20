#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include"win.h"

long time_in_ms() {
    // return time in milliseconds, for benchmarking the model speed
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

void asm_exp2(float *x) {
    int intx = (int)*x - ((*x < 0) ? 1 : 0);
    float decx = *x - (float)intx;

    asm ("shl $23, %1\n\t"
        "add $1065353216, %1\n\t"
        "mov %1, %0"
        : "=r" (*x)
        : "r" (intx)
    );
    *x = *x * (1.0 + decx);
}

int main() {

    int TEST_SIZE = pow(10, 9);
    /*
        over 10^10 iterations
        exp took on average  347.746002ms
        exp2 took on average 47.537998ms
        exp2 is 7.315116x faster than exp
        average deviation from 2^x is -0.159916%
    */

    long SAMPLES = 10;
    long TEST_SIZE = 10000000;
    long RAND_RANGE = 8;
    long average_exp = 0;
    long average_exp2 = 0;
    float average_dev = 0;

    // Precision Testing


    FILE* ptr = fopen("./precision.json", "w");
    fprintf(ptr, "[");
    fclose(ptr);
    for(int i = 0; i < 1000; i++) {
        float x = ((float)rand()) / (RAND_MAX / RAND_RANGE) - (RAND_RANGE / 2);
        float y = x;
        float z = exp2(x);

        asm_exp2(&y);

        ptr = fopen("./precision.json", "a");
        if(i != 0) fprintf(ptr, ", ");
        fprintf(ptr, "[%f, %f, %f]", x, y, z);
        fclose(ptr);
    }

    ptr = fopen("./precision.json", "a");
    fprintf(ptr, "]\n");
    fclose(ptr);

    // Performance Testing

    ptr = fopen("./performance.json", "w");
    fprintf(ptr, "[");
    fclose(ptr);


    for (int i = 0; i < SAMPLES; i++) {
        long exp_start = time_in_ms();
        float* nums = malloc(sizeof(float) * TEST_SIZE);
        float* x = malloc(sizeof(float) * TEST_SIZE);
        for(int i = 0; i < TEST_SIZE; i++) {
            nums[i] = ((float)rand()) / (RAND_MAX / RAND_RANGE) - (RAND_RANGE / 2);
            x[i] = nums[i];
        }

        for(int i = 0; i < TEST_SIZE; i++) {
            expf(nums[i]);
        }
        long exp_end = time_in_ms();
        for(int i = 0; i < TEST_SIZE; i++) {
            asm_exp2(x + i);
        }
        long exp2_end = time_in_ms();
        float dev = 0;
        for(int i = 0; i < TEST_SIZE; i++) {
            float real = exp2(nums[i]);
            dev += (real - x[i])/real;
        }
        fprintf(stderr, "dev = %f", (dev / (float)TEST_SIZE));
        average_dev += (dev / (float)TEST_SIZE);

        long diff_exp = (exp_end - exp_start);
        long diff_exp2 = (exp2_end - exp_end);

        ptr = fopen("./performance.json", "a");
        if(i != 0) fprintf(ptr, ", ");
        fprintf(ptr, "[%ld, %ld]", diff_exp, diff_exp2);
        fclose(ptr);

        free(nums);
        free(x);

        average_exp += diff_exp;
        average_exp2 += diff_exp2;
    }

    fprintf(stderr, "exp took on average  %fms\n", (float)average_exp/(float)SAMPLES);
    fprintf(stderr, "exp2 took on average %fms\n", (float)average_exp2/(float)SAMPLES);
    fprintf(stderr, "exp2 is %fx faster than exp\n", ((float)average_exp)/((float)average_exp2));

    ptr = fopen("./performance.json", "a");
    fprintf(ptr, "]\n");
    fclose(ptr);

    return 0;
}