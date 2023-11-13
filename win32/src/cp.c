#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <wchar.h>

#include <windows.h>

#define ARGUMENT_ERROR 1
#define SOURCE_FILE_FAILURE 2
#define DEST_FILE_FAILURE 3
#define WRITE_ERROR 4
#define MEMORY_ERROR 5
#define CHAR_ERROR 6

#define BUF_SIZE 256

/**
 * @returns (transfer)
 * */
wchar_t *char_str_to_wchar(const char *cstr)
{
    if (cstr == NULL) return NULL;

    size_t len = strlen(cstr);
    if (len == 0) return L"";

    wchar_t *wstr = malloc(sizeof(wchar_t) * (len + 1));
    if (wstr == NULL) {
        return NULL;
    }

    size_t converted_wchar_count = mbstowcs(wstr, cstr, len + 1);

    return wstr;
}

int copy_with_win32(const char *dst, const char *src)
{
    int retval = 0;

    wchar_t *wsrc = char_str_to_wchar(src);
    if (wsrc == NULL) {
        retval = CHAR_ERROR;
        goto ret_point;
    }

    wchar_t *wdst = char_str_to_wchar(dst);
    if (wdst == NULL) {
        retval = CHAR_ERROR;
        goto free_wsrc;
    }

	HANDLE hIn = CreateFile(wsrc, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Cannot open input file: 0x%lx\n", GetLastError());

		retval = SOURCE_FILE_FAILURE;
        goto free_wdst;
	}

	HANDLE hOut = CreateFile(wdst, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Cannot open output file: 0x%lx\n", GetLastError());

        retval = DEST_FILE_FAILURE;
        goto close_in;
	}

    CHAR *buffer = (char*)malloc(sizeof(CHAR) * BUF_SIZE);
    if (buffer == NULL) {
        retval = MEMORY_ERROR;
        goto close_out;
    }

    DWORD nIn = 0;
	while (ReadFile(hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
        DWORD nOut = 0;
		WriteFile(hOut, buffer, nIn, &nOut, NULL);
		if (nIn != nOut) {
			fprintf(stderr, "Fatal write error: 0x%lx\n", GetLastError());

            retval = WRITE_ERROR;
            goto close_out;
		}
	}

close_out:
    if (hOut != NULL) {
        CloseHandle (hOut);
        hOut = INVALID_HANDLE_VALUE;
    }
close_in:
    if (hIn != NULL) {
        CloseHandle (hIn);
        hIn = INVALID_HANDLE_VALUE;
    }
free_wdst:
    free(wdst);
    wdst = NULL;
free_wsrc:
    free(wsrc);
    wsrc = NULL;
ret_point:
    return retval;
}

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
        size_t bytes_out = fwrite(rec, 1, bytes_in, dst_file);
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
           && str[--str_idx] == substr[--substr_idx]) {
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
    } else if (string_ends_with(argv[0], "cpw32.exe")) {
        return copy_with_win32(argv[2], argv[1]);
    }

    fprintf(stderr, "Unknown mode: %s\n", argv[0]);
    return ARGUMENT_ERROR;
}
