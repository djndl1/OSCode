#ifndef WINFILE_H_
#define WINFILE_H_

#include "winhandle.h"
#include <data_buffer.h>

typedef struct file_open_request {
    DWORD requested_access;
    DWORD share_mode;
    bool handle_inheritable;
    SECURITY_DESCRIPTOR *security_descriptor;
    DWORD creation_disposition;
    DWORD flags_attributes;

    winhandle template_file;
} file_open_request;

typedef struct file_open_result {
    winstatus status;
    winhandle file_handle;
} file_open_result;

file_open_result winfile_open(const wchar_t *filename, file_open_request request);

typedef struct file_read_result {
    winstatus status;
    DWORD read_count;
} file_read_result;

file_read_result winfile_sync_read_into(winhandle fh,
                                        data_buffer buffer);


#endif // WINFILE_H_
