
#ifndef _FA_TIME
#define _FA_TIME

#include <fa.h>
#include <fa/ratio.h>
#include <fa/string.h>

/** @addtogroup FaTime

    Time values.
    
    This type represent a point in time relative to a stream-specific *start time*.

    The represeentation supports small divisions of a second. Values longer than a
    day are supported, but must be represented explicitly as a number of days, since
    weeks, months, years and so on would require a calendar representation.

    To *deconstruct* a time interval into its components, use `fa_time_days`,
    `fa_time_hours` and so on. To *convert* a type into a single numeric value
    such as number of seconds, use `fa_time_to_seconds` and so on. 
    
    For example:
    
    ~~~c    
    fa_time_minutes(hms(0,4,33))    => 4
    fa_time_seconds(hms(0,4,33))    => 33

    fa_time_to_minutes(hms(0,4,33)) => 4*60 + 33 = 273
    ~~~

    @par Literals
    - `hms(0,1,30)`
    - `days(2)`
    - `hours(2)`
    - `minutes(3)`
    - `seconds(4)`
    - `milliseconds(3501)`
    - `microseconds(3500421)`
    - `divisions(1, 3)`
    - `fa_add(minutes(1), milliseconds(100))`
    - `fa_subtract(hours(0), seconds(2))`

    @par Implements
    - fa_equal_t
    - fa_order_t
    - fa_string_show_t
    - fa_copy_t
    - fa_destroy_t
    - fa_dynamic_t
    - fa_number_t

 
    @defgroup Fa Fa
    @{
    @defgroup FaTime Time
    @{
    */

/** The abstract type of times.
*/
typedef struct _fa_time_t * fa_time_t;

/** A number of days.
*/
typedef int32_t fa_time_days_t;

/** A number of hours.
*/
typedef int32_t fa_time_hours_t;

/** A number of minutes.
*/
typedef int32_t fa_time_minutes_t;

/** A number of seconds.
*/
typedef int32_t fa_time_seconds_t;

/** A number of milliseconds.
*/
typedef int64_t fa_time_milliseconds_t;

/** Create a new time value.
    @param days
        Number of days.
    @param hours
        Number of hours.
    @param minutes
        Number of minutes.
    @param seconds
        Number of seconds (destroyed).
    @return
        A new time value.
*/
fa_time_t fa_time_create(fa_time_days_t,
                         fa_time_hours_t,
                         fa_time_minutes_t,
                         fa_ratio_t);

/**
    Copy the given time value.
*/
fa_time_t fa_time_copy(fa_time_t);

/**
    Destroy the given time value.
*/
void fa_time_destroy(fa_time_t);

/**
    Return the number of whole days in this time value.

    @param time
        Time value.
    @return
        Integer representing this time in days modulo one.
*/
int32_t fa_time_days(fa_time_t);

/**
    Return the number of whole hours in this time value.

    @param time
        Time value.
    @return
        Integer representing this time in hours modulo one.
*/
int32_t fa_time_hours(fa_time_t);

/**
    Return the number of whole minutes in this time value.

    @param time
        Time value.
    @return
        Integer representing this time in minutes modulo one.
*/
int32_t fa_time_minutes(fa_time_t);

/**
    Return the number of whole seconds in this time value.

    @param time
        Time value.
    @return
        Integer representing this time in seconds modulo one.
*/
int32_t fa_time_seconds(fa_time_t);

/**
    Return the fractions of a second in this time value.

    @param time
        Time value.
    @return
        Rational number, representing the remainder of time in seconds over one.
*/
fa_ratio_t fa_time_divisions(fa_time_t);

/** Print the time as an ISO 8601 duration.

    The ISO represenation use decimal fractions of a second, and may lose precision. For example
    the duration of 1 min 24 1/3 sec would be represented as `P0000-00-00T00:01:24.3333`.
*/
fa_string_t fa_time_to_iso(fa_time_t);

/** Convert the time to seconds.
    This may lose precision.

    @param time
        Time value.
*/
fa_time_seconds_t fa_time_to_seconds(fa_time_t);

/** Convert the time to milliseconds.
    This may lose precision.

    @param time
        Time value.
*/
fa_time_milliseconds_t fa_time_to_milliseconds(fa_time_t);

/** The system time type.

    This is the same as `time_t` in the standard C header `time.h`.
*/
typedef struct _fa_time_system_t * fa_time_system_t;

/** @}
    @}
    */

#endif // _FA_TIME

