
#ifndef _FA_SIGNAL
#define _FA_SIGNAL

#include <fa.h>
#include <fa/pair.h>
#include <fa/type.h>
#include <fa/time.h>
#include <fa/buffer.h>

/** @addtogroup FaSignal

    @addtogroup FaSignal
    
    Provides signals.
    
 
    @defgroup Fa Fa
    @{
    @defgroup FaSignal Signal
    @{
    */


typedef struct _fa_signal_t * fa_signal_t;

/** Like fa_unary_t, but speficied on signals.
    
*/
typedef fa_signal_t (* fa_signal_unary_signal_t)(fa_ptr_t,
                                                 fa_signal_t);

/** Like fa_unary_t, but speficied on doubles.
    
*/
typedef double (* fa_signal_unary_double_t)(fa_ptr_t, double);

/** Like fa_binary_t, but speficied on doubles.
    
*/
typedef double (* fa_signal_binary_double_t)(fa_ptr_t,
                                             double,
                                             double);

/**
    This primitive signal represents elapsed time in seconds.
    
    @par Semantic $$y(t) = t$$.
*/
fa_signal_t fa_signal_time();

/**
    This primitive signal represents a random number in the range -1 to 1.
    This can be used as a white-noise generator.
    
    @par Semantic $$y(t) = random(-1,1)$$.
*/
fa_signal_t fa_signal_random();

/**
    This primitive signal represents a constant value.
    
    @par Semantic $$y(c)(t) = c$$.
*/
fa_signal_t fa_signal_constant(double);

/**
    This primitive signal lifts a unary function to signal level.
    
    @par Semantic $$y(f,a)(t) = f(a(t))$$.
*/
fa_signal_t fa_signal_lift(fa_string_t,
                           fa_signal_unary_double_t,
                           fa_ptr_t,
                           fa_signal_t);

/**
    This primitive signal lifts a binary function to signal level.
    
    @par Semantic $$y(f,a,b)(t) = f(a(t), b(t))$$.
*/
fa_signal_t fa_signal_lift2(fa_string_t,
                            fa_signal_binary_double_t,
                            fa_ptr_t,
                            fa_signal_t,
                            fa_signal_t);

/**
    The loop primitive provides feedback. The given signal function receives
    a signal which is the same as the output of the function with an implicit
    one-sample delay.
    
    @par Semantic $$y(f)(t) = f(y(t-1))$$.
*/
fa_signal_t fa_signal_loop(fa_signal_unary_signal_t, fa_ptr_t);

/**
    The delay primitive provides time shifting.
    
    @par Semantic $$y(n,a)(t) = a(t-n)$$.
*/
fa_signal_t fa_signal_delay(int, fa_signal_t);


fa_signal_t fa_signal_input(int);


fa_signal_t fa_signal_output(int, int, fa_signal_t);


void fa_signal_run(int, fa_signal_t, double *);


fa_ptr_t fa_signal_run_file(int, fa_signal_t, fa_string_t);


fa_buffer_t fa_signal_run_buffer(int, fa_signal_t);


void fa_signal_print(int, fa_signal_t);


fa_signal_t fa_signal_add(fa_signal_t, fa_signal_t);


fa_signal_t fa_signal_multiply(fa_signal_t, fa_signal_t);


fa_signal_t fa_signal_sin(fa_signal_t);


fa_pair_t fa_signal_to_tree(fa_signal_t);


fa_string_t fa_signal_draw_tree(fa_pair_t);


fa_signal_t fa_signal_simplify(fa_signal_t);


fa_signal_t fa_signal_latter(fa_signal_t, fa_signal_t);


fa_signal_t fa_signal_former(fa_signal_t, fa_signal_t);


fa_signal_t fa_signal_impulse();


fa_signal_t fa_signal_line(double);


fa_signal_t fa_signal_low_pass(fa_signal_t,
                               fa_signal_t,
                               fa_signal_t,
                               fa_signal_t,
                               fa_signal_t);


fa_signal_t fa_signal_biquad(fa_signal_t,
                             fa_signal_t,
                             fa_signal_t,
                             fa_signal_t,
                             fa_signal_t,
                             fa_signal_t);

/** @}
    @}
    */

#endif // _FA_SIGNAL

