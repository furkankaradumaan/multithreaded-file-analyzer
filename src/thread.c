#include "../include/thread.h"
#include "../include/analyzer.h"
#include <pthread.h>

void *thread_entry(void *args) {
        worker_args_t *worker_args = (worker_args_t *)args;
        printf("[Worker %lu] Started: %s\n", pthread_self(), worker_args->file_path);

        worker_args->status =
                analyzer_analyze_file(worker_args->file_path,
                                      &worker_args->analysis);

        printf("[Worker %lu] Finished: %s\n", pthread_self(), worker_args->file_path);
        
        return NULL;
}
