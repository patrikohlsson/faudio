
#ifndef _FA_PLOT
#define _FA_PLOT

#include <fa.h>
#include <fa/std.h>
#include <fa/buffer.h>

/** @addtogroup FaPlot
 
    Provides basic plotting.
    
    There are currently two backends:

    - [CorePlot](http://code.google.com/p/core-plot/)
      - Only works on OS X.
      - Handles time-varying data (such as incoming audio signals etc).
      - Requires the third-party library `CorePlot.framework` to be in the framework path.
    - [gnuplot](http://gnuplot.sourceforge.net/)                        
      - Only handles static input.
      - Signals are rendered at time zero.
      - Requires the `gnuplot` executable to be in the path.
    
    @warning
        The CorePlot backend starts a Cocoa event loop which will not work if
        another GUI is already running. The functionality is intended for internal 
        testing only.
 
    @defgroup Fa Fa
    @{
    @defgroup FaPlot Plot
    @{
    */

/**
    @typedef fa_plot_function_t
    
    Callback to be invoked by the plotter.
    
    @param context  Context reference.
    @param index    Plot index, from \f$0\f$ to \f$4\f$. 
    @param time     Time in seconds, starting at \f$0\f$. 
    @param x        Value of x, from \f$-1\f$ to \f$1\f$. 
*/
typedef double (* fa_plot_function_t)(fa_ptr_t,
                                      int,
                                      double,
                                      double);


void fa_plot_use_gnu();


void fa_plot_continous(fa_plot_function_t,
                       fa_ptr_t,
                       fa_nullary_t,
                       fa_ptr_t);


void fa_plot_buffer_float(fa_buffer_t, fa_nullary_t, fa_ptr_t);


void fa_plot_buffer_double(fa_buffer_t, fa_nullary_t, fa_ptr_t);

/** @}
    @}
    */

#endif // _FA_PLOT

