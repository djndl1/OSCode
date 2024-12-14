#ifndef FILE_POLL_H_
#define FILE_POLL_H_

#include "file_desc.h"
#include "timespan.h"

typedef struct {
    file_desc fd;
    short events;
} file_desc_poll_req;

typedef struct {
    file_desc fd;
    short revents;
} file_desc_poll_result;

#define dyn_array_element_type file_desc_poll_req
#define dyn_array_type_name file_desc_poll_req_array
#include "dyn_array.h"

#define dyn_array_element_type file_desc_poll_result
#define dyn_array_type_name file_desc_poll_result_array
#include "dyn_array.h"

typedef struct {
    int error;
    file_desc_poll_result_array results;
} file_desc_array_result;


file_desc_array_result file_poll(file_desc_poll_req_array req, timespan timeout);


#endif // FILE_POLL_H_
