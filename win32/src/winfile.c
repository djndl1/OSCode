#include "winfile.h"

file_open_result winfile_open(const wchar_t *filename,
                              file_open_request request)
{
    SECURITY_ATTRIBUTES security_attr = {
        .nLength = sizeof(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = request.security_descriptor,
        .bInheritHandle = request.handle_inheritable,
    };

    winhandle fh = WINHANDLE(CreateFile(filename,
                                        request.requested_access,
                                        request.share_mode,
                                        &security_attr,
                                        request.creation_disposition,
                                        request.flags_attributes,
                                        request.template_file.handle));

    if (winhandle_invalid(fh)) {
        return (file_open_result) {
            .file_handle = fh,
            .status = WIN_LASTERR,
        };
    }

    return (file_open_result){
         .file_handle = fh,
         .status = WIN_OK,
    };
}

file_read_result winfile_sync_read_into(winhandle fh,
                                        data_buffer buffer)
{
    DWORD read_count = 0;
    if (ReadFile(fh.handle, buffer.data, buffer.length,
                 &read_count, NULL)) {
        return (file_read_result){
            .status = WIN_OK,
            .read_count = read_count,
        };
    }

    return (file_read_result){
            .status = WIN_LASTERR,
            .read_count = 0,
    };
}

file_read_result_buffer winfile_sync_read_as_buffer(winhandle fh,
                                                    uint32_t count,
                                                    const mem_allocator *allocator)
{
    buffer_alloc_result alloc_res = data_buffer_new(count, allocator);
    if (alloc_res.error) {
        return (file_read_result_buffer){
            .status = WINSTATUS(alloc_res.error, false),
        };
    }
    data_buffer buffer = alloc_res.buffer;

    DWORD read_count = 0;
    if (ReadFile(fh.handle, buffer.data, count,
                 &read_count, NULL)) {
        return (file_read_result_buffer){
            .status = WIN_OK,
            .buffer = buffer,
            .read_count = read_count,
        };
    }

    data_buffer_deallocate(buffer);
    return (file_read_result_buffer){
            .status = WIN_LASTERR,
            .read_count = 0,
    };
}
