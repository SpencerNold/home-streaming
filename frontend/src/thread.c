#include "thread.h"

#include <pthread.h>

void PROC_StartNewThread(void* (*func)(void*)) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, func, NULL);
}