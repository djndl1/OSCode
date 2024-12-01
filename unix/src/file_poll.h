#ifndef FILE_POLL_H_
#define FILE_POLL_H_

#include "file_desc.h"
#include "timespan.h"

typedef struct {
    file_desc_t fd;
    short events;
} file_desc_poll_req_t;

typedef struct {
    file_desc_t fd;
    short revents;
} file_desc_poll_result_t;

#define dyn_array_element_type file_desc_poll_req_t
#define dyn_array_type_name file_desc_poll_req_array_t
#include "dyn_array.h"

#define dyn_array_element_type file_desc_poll_result_t
#define dyn_array_type_name file_desc_poll_result_array_t
#include "dyn_array.h"

typedef struct {
    int error;
    file_desc_poll_result_array_t results;
} file_desc_array_result_t;


file_desc_array_result_t file_poll(file_desc_poll_req_array_t req, timespan_t timeout);


#endif // FILE_POLL_H_
