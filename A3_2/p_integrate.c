#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000000 // intervals
#define NUM_THREADS 1
double start = 0.0; 
double end = 1.0;
double x;

pthread_mutex_t mutex;
 

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double total_sum = 0.0;

void *parallel_trapezoidalRule(void *arg) 
{
    int id = *(int*)arg;
    int section = N/NUM_THREADS;
    double local_start = start + id * section * x;
    double local_end = local_start + section * x;
    double area = 0.0;

    for (int m = 0; m < section; m++) {
        double m1 = local_start + 1 * x;
        double m2 = m1 + x;
        area += (f(m1) + f(m2)) * x;
    }

    pthread_mutex_lock(&mutex);
    total_sum += area;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) 
{
    pthread_t thread[NUM_THREADS];
    int threadID[NUM_THREADS];

    x = (end - start) / N;

    pthread_mutex_init(&mutex, NULL);

    //Create the threads
    for (long i = 0; i < NUM_THREADS; i++){
        pthread_create(&thread[i], NULL, parallel_trapezoidalRule, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }

    printf("Result of numerical integration: %f\n", total_sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}
