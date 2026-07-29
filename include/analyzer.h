/*
 * This file defines a file analyzer API.
 *
 */

#ifndef __ANALYZER_H__
#define __ANALYZER_H__

#include <limits.h>
#include <stdio.h>

typedef struct file_analysis {
        char file_path[PATH_MAX];
        size_t lines;
        size_t bytes;
} file_analysis_t;

/*
 * This function analysis a given file and puts the result
 * into the given pointer.
 *
 * On success, function returns 0, otherwise returns an error code
 * that specifies the problem.
 */
int analyzer_analyze_file(const char *file_path,
                          file_analysis_t *analysis_result);

#endif
