#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EGENERAL -1

pthread_mutex_t mutex;
int counter = 0;

void *thread_function(void *p_increment)
{
    int increment = *((int *) p_increment);
    printf("%d\n", increment);
    pthread_mutex_lock(&mutex);
    counter += increment;
    pthread_mutex_unlock(&mutex);
    printf("thread[%d] counter: %d\n", increment, counter);
}

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    int thread1_ec, thread2_ec;
    int thread1_increment = 1, thread2_increment = 5;

    thread1_ec = pthread_create(&thread1, NULL, thread_function, (void *) &thread1_increment);
    thread2_ec = pthread_create(&thread2, NULL, thread_function, (void *) &thread2_increment);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("thread1 ec: %d\n", thread1_ec);
    printf("thread1 ec: %d\n", thread2_ec);

    exit(0);
}
