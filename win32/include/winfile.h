#ifndef WINFILE_H_
#define WINFILE_H_

#include "winhandle.h"
#include <data_buffer.h>
#include <stdint.h>

typedef struct file_open_request {
    uint32_t requested_access;
    uint32_t share_mode;
    bool handle_inheritable;
    SECURITY_DESCRIPTOR *security_descriptor;
    uint32_t creation_disposition;
    uint32_t flags_attributes;

    winhandle template_file;
} file_open_request;

typedef struct file_open_result {
    winstatus status;
    winhandle file_handle;
} file_open_result;

file_open_result winfile_open(const wchar_t *filename, file_open_request request);

typedef struct file_read_result {
    winstatus status;
    uint32_t read_count;
} file_read_result;

file_read_result winfile_sync_read_into(winhandle fh,
                                        data_buffer buffer);

typedef struct file_read_result_buffer {
    winstatus status;
    data_buffer buffer;
    uint32_t read_count;
} file_read_result_buffer;

file_read_result_buffer winfile_sync_read_as_buffer(winhandle fh,
                                                    uint32_t count,
                                                    const mem_allocator *allocator);


#endif // WINFILE_H_
