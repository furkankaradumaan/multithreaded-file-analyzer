/*
 * This file defines a function for a pthread
 * that analyzes a given file and returns its result.
 */

#ifndef __THREAD_H__
#define __THREAD_H__

#include <limits.h>
#include <stdio.h>
#include "analyzer.h"

typedef struct worker_args {
        char file_path[PATH_MAX];
        file_analysis_t analysis;
        int status;
} worker_args_t;

/*
 * This function receives a thread_context object
 * and returns file_analysis_t object.
 *
 * The return code of anlayze function is
 * stored with 'status' attribute.
 */
void *thread_entry(void *args);

#endif
