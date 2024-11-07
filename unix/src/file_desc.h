#ifndef FILE_DESC_H_
#define FILE_DESC_H_

#include <fcntl.h>
#include <unistd.h>

#include "data_buffer.h"
#include "util.h"


typedef struct {
    int fd;
} file_desc_t;

#define FILE_DESC(fd) ((file_desc_t) { fd })
#define fd_stdin FILE_DESC(STDIN_FILENO)
#define fd_stdout FILE_DESC(STDOUT_FILENO)
#define fd_stderr FILE_DESC(STDERR_FILENO)

typedef struct {
    int error;
    file_desc_t fd;
} file_desc_result_t;

typedef struct {
    int error;
    ssize_t written_count;
} file_write_result_t;

typedef struct {
    int error;
    off_t offset;
} file_seek_result_t;

typedef struct {
    off_t offset;
    int whence;
} file_seek_target_t;


#define SEEK_TARGET(off, from) ((file_seek_target_t){ .offset = off, .whence = from })
#define SEEK_FROM_CUR(off) SEEK_TARGET(off, SEEK_CUR)
#define SEEK_FROM_END(off) SEEK_TARGET(off, SEEK_END)
#define SEEK_FROM_BEGINNING(off) SEEK_TARGET(off, SEEK_SET)

typedef struct {
    int error;
    size_t read_count;
    data_buffer_t buffer;
} file_read_result_t;

file_desc_result_t file_open(const char *pathname, int flags);

file_desc_result_t file_create(const char *pathname, int flags, mode_t modes);

file_desc_result_t file_open_at(const file_desc_t dir, const char *pathanem, int flags);

file_desc_result_t file_create_at(const file_desc_t dir, const char *pathanem, int flags);

file_read_result_t file_read_into(const file_desc_t self, data_buffer_t buffer);

file_read_result_t file_read(const file_desc_t, size_t);

file_write_result_t file_write(const file_desc_t, const data_buffer_t);

file_write_result_t file_write_until(const file_desc_t, const data_buffer_t, size_t count);

file_seek_result_t file_seek(const file_desc_t, const file_seek_target_t);

error_t file_close(const file_desc_t);

#endif // FILE_DESC_H_
