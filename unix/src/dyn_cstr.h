#ifndef DYN_CSTR_H_
#define DYN_CSTR_H_

#include "data_buffer.h"
#include "allocator.h"

typedef struct {
    data_buffer_t _data;
    size_t _len;
} dyn_cstr_t;

typedef struct {
    int error;
    dyn_cstr_t str;
} dyn_cstr_result_t;

typedef struct {
    dyn_cstr_t _str;
    char *_p;
} dyn_cstr_iter_t;


static inline char *dyn_cstr_nbts(dyn_cstr_t self)
{
    return ((char*)(self._data.data));
}

#define dyn_cstr_at(self, idx) ((dyn_cstr_nbts(self))[idx])

dyn_cstr_result_t dyn_cstr_from_buffer(data_buffer_t buf, const allocator_t *allocator);

dyn_cstr_result_t dyn_cstr_from(dyn_cstr_t str, const allocator_t *allocator);

dyn_cstr_result_t dyn_cstr_literal(const char *lit, const allocator_t *allocator);

static inline size_t dyn_cstr_len(dyn_cstr_t self)
{
    return self._len;
}

int dyn_cstr_compare(dyn_cstr_t self, dyn_cstr_t other);

static bool dyn_cstr_equal(dyn_cstr_t self, dyn_cstr_t other)
{
    return dyn_cstr_compare(self, other) == 0;
}

void dyn_cstr_destroy(dyn_cstr_t self);

static inline dyn_cstr_iter_t dyn_cstr_iter(const dyn_cstr_t self)
{
    return (dyn_cstr_iter_t) {
        ._str = self,
        ._p = NULL,
    };
}

static inline char dyn_cstr_iter_cur(const dyn_cstr_iter_t self)
{
    return *(self._p);
}

static inline char *dyn_cstr_iter_cur_ref(const dyn_cstr_iter_t self)
{
    return self._p;
}

static inline const char *dyn_cstr_iter_cur_cref(const dyn_cstr_iter_t self)
{
    return self._p;
}

static inline bool dyn_cstr_iter_next(dyn_cstr_iter_t *self)
{
    char *tmp = (char*)self->_str._data.data;
    if (tmp == NULL || *tmp == '\0') {
        return false;
    }

    if (self->_p == NULL) {
        self->_p = (char*)self->_str._data.data;
        return true;
    }

    if (*self->_p != '\0') {
        self->_p++;
        return true;
    }

    return false;
}

#define dyn_cstr_foreach(it, str) \
    for (dyn_cstr_iter_t it = dyn_cstr_iter(str); dyn_cstr_iter_next(&it); )
#endif // DYN_CSTR_H_
