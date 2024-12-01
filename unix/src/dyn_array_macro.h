#ifndef DYN_ARRAY_MACRO_H_
#define DYN_ARRAY_MACRO_H_


#undef dyn_array_result_type_name
#define dyn_array_result_type_name CONCAT2(dyn_array_type_name, _result_t)
#undef dyn_array_func
#define dyn_array_func(funcname) CONCAT2(dyn_array_type_name, _ ## funcname)

#endif // DYN_ARRAY_MACRO_H_
