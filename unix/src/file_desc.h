#ifndef FILE_DESC_H_
#define FILE_DESC_H_

#include <fcntl.h>


typedef struct {
    int fd;
} file_desc_t;

typedef struct {
    int error;
    file_desc_t fd;
} file_desc_result_t;


file_desc_result_t file_open(const char *pathname, int flags);

file_desc_result_t file_create(const char *pathname, int flags, mode_t modes);

file_desc_result_t file_open_at(const file_desc_t dir, const char *pathanem, int flags);

file_desc_result_t file_create_at(const file_desc_t dir, const char *pathanem, int flags);

#endif // FILE_DESC_H_
