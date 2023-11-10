#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define ARGUMENT_ERROR 1
#define SOURCE_FILE_FAILURE 2
#define DEST_FILE_FAILURE 3
#define WRITE_ERROR 4
#define MEMORY_ERROR 5

#define BUF_SIZE 256

int copy_with_libc(const char *dst, const char *src)
{
    int retval = 0;

    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL) {
        perror(src);
        retval = SOURCE_FILE_FAILURE;

        goto ret_point;
    }

    FILE *dst_file = fopen(dst, "wb");
    if (dst_file == NULL) {
        perror(dst);

        retval = DEST_FILE_FAILURE;
        goto close_src;
    }

    char *rec = (char*)malloc(sizeof(char) * BUF_SIZE);
    if (rec == NULL) {
        retval = MEMORY_ERROR;
        goto close_dst;
    }

    size_t bytes_in = 0;
    while ((bytes_in = fread(rec, 1, BUF_SIZE, src_file)) > 0) {
        size_t bytes_out = fwrite(rec, 1, BUF_SIZE, dst_file);
        if (bytes_in != bytes_out) {
            perror("Fatal write error");

            retval = WRITE_ERROR;
            goto buf_cleanup;
        }
    }


buf_cleanup:
    free(rec);
    rec = NULL;
close_dst:
    if (dst_file != NULL) {
        fclose(dst_file);
        dst_file = NULL;
    }
close_src:
    if (src_file != NULL) {
        fclose(src_file);
        src_file = NULL;
    }
ret_point:
    return retval;
}

bool string_ends_with(const char *str, const char *substr)
{
    if (str == NULL || str[0] == '\0') {
        return false;
    }
    if (substr == NULL || substr[0] == '\0') {
        return false;
    }

    size_t str_idx = strlen(str);
    size_t substr_idx = strlen(substr);
    if (str_idx < substr_idx) {
        return false;
    }

    while (str_idx > 0 && substr_idx > 0
           && str[str_idx--] == str[substr_idx--]) {
    }

    if (substr_idx != 0) {
        return false;
    }

    return str[str_idx] == substr[substr_idx];
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stdin, "Usage: cp src dst\n");

        return ARGUMENT_ERROR;
    }

    if (string_ends_with(argv[0], "cpc.exe")) {
        return copy_with_libc(argv[2], argv[1]);
    }

    fprintf(stderr, "Unknown mode: %s\n", argv[0]);
    return ARGUMENT_ERROR;
}
