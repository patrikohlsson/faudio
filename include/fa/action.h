
#ifndef _FA_ACTION
#define _FA_ACTION

#include <fa.h>
#include <fa/pair.h>
#include <fa/list.h>
#include <fa/signal.h>

/** @addtogroup FaAction
 
    Actions on control values.

    @par Literals

    @par Implements 
    - fa_copy_t
    - fa_destroy_t
    - fa_string_show_t

    @see 

 
    @defgroup Fa Fa
    @{
    @defgroup FaAction Action
    @{
    */


typedef struct _fa_action_t * fa_action_t;


typedef int fa_action_channel_t;


typedef fa_string_t fa_action_name_t;


fa_action_t fa_action_set(fa_action_channel_t, double);


fa_action_t fa_action_accum(fa_action_channel_t,
                            fa_signal_unary_double_t,
                            fa_ptr_t);


fa_action_t fa_action_send(fa_action_name_t, fa_ptr_t);


fa_action_t fa_action_copy(fa_action_t);


void fa_action_destroy(fa_action_t);

/** @}
    @}
    */

#endif // _FA_ACTION

