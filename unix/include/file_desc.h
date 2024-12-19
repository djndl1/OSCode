#ifndef FILE_DESC_H_
#define FILE_DESC_H_

#include <fcntl.h>
#include <unistd.h>

#include "data_buffer.h"
#include "basis.h"

typedef struct file_desc {
    int fd;
} file_desc;

#define FILE_DESC(fd) ((file_desc) { fd })
#define fd_stdin FILE_DESC(STDIN_FILENO)
#define fd_stdout FILE_DESC(STDOUT_FILENO)
#define fd_stderr FILE_DESC(STDERR_FILENO)

typedef struct file_desc_result {
    int error;
    file_desc fd;
} file_desc_result;

typedef struct file_write_result {
    int error;
    ssize_t written_count;
} file_write_result;

typedef struct file_seek_result {
    int error;
    off_t offset;
} file_seek_result;

typedef struct file_seek_target {
    off_t offset;
    int whence;
} file_seek_target;


#define SEEK_TARGET(off, from) ((file_seek_target){ .offset = off, .whence = from })
#define SEEK_FROM_CUR(off) SEEK_TARGET(off, SEEK_CUR)
#define SEEK_FROM_END(off) SEEK_TARGET(off, SEEK_END)
#define SEEK_FROM_BEGINNING(off) SEEK_TARGET(off, SEEK_SET)

typedef struct {
    int error;
    size_t read_count;
    data_buffer buffer;
} file_read_result;

file_desc_result file_open(const char *pathname, int flags);

file_desc_result file_create(const char *pathname, int flags, mode_t modes);

file_desc_result file_open_at(const file_desc dir, const char *pathanem, int flags);

file_desc_result file_create_at(const file_desc dir, const char *pathanem, int flags);

file_read_result file_read_into(const file_desc self, data_buffer buffer);

file_read_result file_read(const file_desc, size_t);

file_write_result file_write(const file_desc, const data_buffer);

file_write_result file_write_until(const file_desc, const data_buffer, size_t count);

file_seek_result file_seek(const file_desc, const file_seek_target);

error_t file_close(const file_desc);

#endif // FILE_DESC_H_
