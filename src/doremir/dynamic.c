
/*
    DoReMIR Audio Engine
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.
 */

#include <doremir.h>
#include <doremir/dynamic.h>
#include <doremir/util.h>

doremir_dynamic_type_repr_t doremir_dynamic_get_type(doremir_ptr_t a)
{
    assert(doremir_interface(doremir_dynamic_i, a) && "Must implement Show");
    return ((doremir_dynamic_t *) doremir_interface(doremir_dynamic_i, a))->get_type(a);

}