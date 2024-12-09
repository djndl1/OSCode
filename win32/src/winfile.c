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

    if (!is_handle_valid(fh)) {
        return (file_open_result) {
            .file_handle = fh,
            .status = WIN_ERR(last_error())
        };
    }

    return (file_open_result){
         .file_handle = fh,
         .status = WIN_OK,
    };
}

file_read_result winfile_sync_read(winhandle fh,
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
            .status = WIN_ERR(last_error()),
            .read_count = 0,
    };
}
