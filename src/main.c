#include "../include/thread.h"
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

int count_files(const char *dir_path);

int main(int argc, char *argv[]) {
        if (argc != 2) {
                fprintf(stderr, "Usage: ./main <dir_path>\n");
                return EXIT_FAILURE;
        }

        char *dir_path = argv[1];
        
        // find number of files in directory.
        int file_count = count_files(dir_path);
        if (file_count == -1) {
                return EXIT_FAILURE;
        } else if (file_count == 0) {
                return EXIT_SUCCESS;
        }

        pthread_t *threads = malloc(sizeof(pthread_t) * file_count);
        worker_args_t *thread_args = malloc(sizeof(worker_args_t) * file_count);
        if (threads == NULL || thread_args == NULL) {
                perror("malloc");
                return EXIT_FAILURE;
        }

        DIR *dir = opendir(dir_path);
        if (dir == NULL) {
                perror("opendir");
                return EXIT_FAILURE;
        }
        
        struct dirent *ent;
        int idx = 0;
        errno = 0;
        while (idx < file_count && (ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_REG) {
                        int n = snprintf(thread_args[idx].file_path,
                                        sizeof(thread_args[idx].file_path),
                                        "%s/%s",
                                        dir_path, ent->d_name);
                        if (n < 0 || n >= PATH_MAX) {
                                fprintf(stderr, "File path is too long\n");
                                continue;
                        }
                        
                        pthread_create(&threads[idx], NULL, thread_entry, &thread_args[idx]);

                        idx++;
                }
        }
        if (errno != 0) {
                perror("readdir");
                closedir(dir);
                return EXIT_FAILURE;
        }
        if (closedir(dir) == -1) {
                perror("closedir");
                return EXIT_FAILURE;
        }

        // wait all threads to complete their jobs
        for (int i = 0; i < file_count; i++) {
                pthread_join(threads[i], NULL);
        }

        // print results
        printf("------------------------------\n");
        printf("%-*s\t%-*s\t%-*s\tStatus\n",
                50, "File", 20, "Lines", 20, "Bytes");
        printf("------------------------------\n");
        for (int i = 0; i < file_count; i++) {
                worker_args_t args = thread_args[i];
                printf("%-*s\t", 50, args.file_path);
                if (args.status != 0) {
                        printf("%-*s\t%-*s\tError\n", 20, "-", 20, "-");
                } else {
                        printf("%-*zu\t%-*zu\tOK\n",
                               20, args.analysis.lines, 20, args.analysis.bytes);
                }
        }
        
        free(threads);
        free(thread_args);
        thread_args = NULL;
        threads = NULL;

        return EXIT_SUCCESS;
}

int count_files(const char *dir_path) {
        DIR *dir = opendir(dir_path);
        if (dir == NULL) {
                perror("opendir");
                return -1;
        }
        
        int counter = 0;
        struct dirent *ent;
        errno = 0;
        while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_REG) {
                        counter++;
                }
        }
        if (errno != 0) {
                perror("readdir");
                closedir(dir);
                return -1;
        }

        if (closedir(dir) == -1) {
                perror("closedir");
                return -1;
        }

        return counter;
}
