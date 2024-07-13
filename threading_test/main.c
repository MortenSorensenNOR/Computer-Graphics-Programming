#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 1048576
int test_array[ARRAY_SIZE];

struct Array_Test_Argument {
    int start, end;
    int* arr;
};

void* array_test_fun(void* arg_p) {
    struct Array_Test_Argument* arg = (struct Array_Test_Argument*)(arg_p); 
    for (int i = arg->start; i < arg->end; ++i) {
        arg->arr[i] = i;
    }
    return NULL;
}

int main() {
    int num_threads = 4; 
    int batch_size = ARRAY_SIZE / num_threads;


    clock_t start = clock();
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    struct Array_Test_Argument* thread_args = (struct Array_Test_Argument*)malloc(num_threads * sizeof(struct Array_Test_Argument));

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].start = batch_size * i;
        thread_args[i].end = batch_size * i + batch_size;
        thread_args[i].arr = test_array;
        pthread_create(&threads[i], NULL, array_test_fun, (void*)&thread_args[i]);
    }
    
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    clock_t end = clock();
    printf("Threads: %f\n", (double)(end - start)/CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        test_array[i] = i;
    }
    end = clock();
    printf("Loop: %f\n", (double)(end - start)/CLOCKS_PER_SEC);

    exit(0);
}
