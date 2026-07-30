#include "../include/analyzer.h"
#include <stdio.h>

#define KB 1024

int analyzer_analyze_file(const char *file_path,
                          file_analysis_t *analysis_result) {
        if (file_path == NULL || analysis_result == NULL) return -1;

        FILE *file = fopen(file_path, "rb");
        if (file == NULL) return -1;
        
        // record the file path
        int n = snprintf(analysis_result->file_path,
                         sizeof(analysis_result->file_path),
                         "%s",
                         file_path);
        if (n < 0 || n >= sizeof(analysis_result->file_path)) {
                fprintf(stderr, "File path too long\n");
                fclose(file);
                return -1;
        }

        // find the number of bytes
        long off = ftell(file);
        if (off == -1) {
                perror("ftell");
                fclose(file);
                return -1;
        }
        if (fseek(file, 0, SEEK_END) != 0) {
                perror("fseek");
                fclose(file);
                return -1;
        }
        long size = ftell(file);
        if (size == -1L) {
                perror("ftell");
                fclose(file);
                return -1;
        }
        analysis_result->bytes = size;
        if (fseek(file, 0, off) != 0) {
                perror("fseek");
                fclose(file);
                return -1;
        }
        
        // count number of lines
        size_t lines = 0;
        char buffer[8 * KB];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                for (size_t i = 0; i < bytes; i++) {
                        if (buffer[i] == '\n')
                                lines++;
                }
        }
        if (ferror(file)) {
                fprintf(stderr,
                        "fread: An error occured when reading the file");
                fclose(file);
                return -1;
        }
        if (fclose(file) == EOF) {
                perror("fclose");
                return -1;
        }
        analysis_result->lines = lines;

        return 0;
}
