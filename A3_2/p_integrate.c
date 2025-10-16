#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define N 1000000000 // intervals
#define NUM_THREADS 1
double a = 0.0; // start of interval 
double b = 1.0; // end of interval
double h; // variable to hold the width of the subintervals

pthread_mutex_t mutex;
 

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double total_sum = 0.0;

void *parallel_trapezoidalRule(void *arg) 
{
    long id = (long)arg;
    int intervalsPerThread = N/NUM_THREADS;
    double local_start = a + id * intervalsPerThread * h;
    double local_end = local_start + intervalsPerThread * h;
    double area = 0.0;

    area += (f(local_start) + f(local_end)) / 2.0;
    for (int i = 1; i < intervalsPerThread - 1; i ++) {
        area += f(local_start + i * h);
    }

    area *= h;

    pthread_mutex_lock(&mutex);
    total_sum += area;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    // threads
    pthread_t threadID[NUM_THREADS];

    h = (b - a) / N;

    pthread_mutex_init(&mutex, NULL);

    //Create the threads
    for (long i = 0; i < NUM_THREADS; i++){
        pthread_create(&threadID[i], NULL, parallel_trapezoidalRule, (void *)i);
    }

    for (long i = 0; i < NUM_THREADS; i++){
        pthread_join(threadID[i], NULL);
    }

    printf("Result of numerical integration: %f\n", total_sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}