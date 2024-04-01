#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_DATA 1000

int generate_data(int i) {
    int data = i;
    for (int j = 0; j < 10000; ++j) {
        data += sin(i) * sin(i)* cos(i) * cos(i) * tan(data + j);
    }
    return data;
}

void process_data(int data) {
    double result = data;
    for (int i = 0; i < 10000; ++i) {
        result += sqrt(fabs(sin(data) * cos(data)));
    }
}

void main() {
    double start_time1, end_time1,start_time2, end_time2;
    int i;

    start_time1 = omp_get_wtime();
    //omp_set_nested(2);
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        #pragma omp single
        {   
            float generated;
            //omp_set_num_threads(4);
            //#pragma omp parallel for
            for (i = 0; i < NUM_DATA; ++i) {
                #pragma omp task firstprivate(i)
                {
                    generated = generate_data(i);
                }
                #pragma omp task depend(in: generated)
                {
                    process_data(generated);
                }
            }
        }
    }
    end_time1 = omp_get_wtime();
    printf("With OpenMP pipeline: %f seconds\n", end_time1 - start_time1);

    start_time2 = omp_get_wtime();
    for (i = 0; i < NUM_DATA; ++i) {
        float generated = generate_data(i);
        process_data(generated);
    }
    end_time2 = omp_get_wtime();
    printf("Without pipeline: %f seconds\n", end_time2 - start_time2);
}
