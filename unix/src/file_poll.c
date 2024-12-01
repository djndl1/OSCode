#include "file_poll.h"
#include "timespan.h"
#include <errno.h>

#define dyn_array_element_type file_desc_poll_req_t
#define dyn_array_type_name file_desc_poll_req_array_t
#include "dyn_array.c"

#define dyn_array_element_type file_desc_poll_result_t
#define dyn_array_type_name file_desc_poll_result_array_t
#include "dyn_array.c"

file_desc_array_result_t file_poll(file_desc_poll_req_array_t req, timespan_t timeout)
{
    return (file_desc_array_result_t) { .error = ENOSYS };
}
