
/*
    faudio

    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.

 */

#include <fa/audio.h>

#include <fa/atomic.h>
#include <fa/atomic/queue.h>
#include <fa/pair/left.h>
#include <fa/priority_queue.h>
#include <fa/signal.h>
#include <fa/thread.h>
#include <fa/util.h>
#include <fa/time.h>

#include <portaudio.h>

/*
    Notes:
        * Name mixup. What to call incoming/outcoming to make it unambigous.
            * to_audio, from_audio or something?
        * FIXME endSession must check for running streams, or we get consistency errors
            * Either abort streams or refuse to close session
 */


typedef fa_audio_proc_t             proc_t;
typedef fa_audio_device_t           device_t;
typedef fa_audio_stream_t           stream_t;
typedef fa_audio_session_t          session_t;
typedef fa_audio_stream_callback_t  stream_callback_t;
typedef fa_audio_session_callback_t session_callback_t;
typedef fa_audio_status_callback_t  status_callback_t;
typedef fa_signal_custom_processor_t   *custom_proc_t;

typedef PaDeviceIndex native_index_t;
typedef PaStream     *native_stream_t;

#define kMaxSignals 8

// TODO formalize better
struct _state_t {
    double *inputs;
    void   *_;
    int     count;
    double  rate;
    // ...
};
typedef struct _state_t *state_t;
list_t fa_signal_get_procs(fa_signal_t signal2);
state_t new_state();
void add_custom_proc(custom_proc_t proc, state_t state);
void delete_state(state_t state);
void inc_state(state_t state);
void run_actions(priority_queue_t controls2, state_t state);
void run_custom_procs(int when, state_t state);
double step(signal_t signal, state_t state);
fa_signal_t fa_signal_simplify(fa_signal_t signal2);


struct _fa_audio_session_t {

    impl_t              impl;               // Dispatcher
    system_time_t       acquired;           // Time of acquisition (not used at the moment)

    list_t              devices;            // Cached device list

    device_t            def_input;          // Default devices, both possibly null
    device_t            def_output;         // If present, these are also in the above list
};

struct _fa_audio_device_t {

    impl_t              impl;               // Dispatcher
    native_index_t      index;              // Native device index
    session_t           session;            // Underlying session

    string_t            name;               // Cached names
    string_t            host_name;

    // bool                muted;           // Not used at the moment
    // double              volume;
};

struct _fa_audio_stream_t {

    impl_t              impl;               // Dispatcher
    native_stream_t     native;             // Native stream
    device_t            input, output;

    unsigned            signal_count;       // Number of signals (same as number of outputs)
    signal_t            signals[kMaxSignals];
    state_t             state;              // DSP state

    unsigned            input_channels, output_channels;
    double              sample_rate;
    long                max_buffer_size;
    int32_t             sample_count;       // Monotonically increasing sample count

    atomic_queue_t      in_controls;        // Controls for scheduling, (AtomicQueue (Time, (Channel, Ptr)))
    priority_queue_t    controls;           // Scheduled controls (Time, (Channel, Ptr))
};

static mutex_t   pa_mutex;
static bool      pa_status;
static session_t current_session;

error_t audio_device_error(string_t msg);
error_t audio_device_error_with(string_t msg, int error);
ptr_t audio_session_impl(fa_id_t interface);
ptr_t audio_device_impl(fa_id_t interface);
ptr_t audio_stream_impl(fa_id_t interface);
inline static session_t new_session();
inline static void session_init_devices(session_t session);
inline static void delete_session(session_t session);
inline static device_t new_device(session_t session, native_index_t index);
inline static void delete_device(device_t device);
inline static stream_t new_stream(device_t input, device_t output, double sample_rate, long max_buffer_size);
inline static void delete_stream(stream_t stream);

void before_processing(stream_t stream);
void after_processing(stream_t stream);
void during_processing(stream_t stream, unsigned count, float **input, float **output);

static int native_audio_callback(const void *input_ptr,
                                 void *output_ptr,
                                 unsigned long frame_count,
                                 const PaStreamCallbackTimeInfo *time_info,
                                 PaStreamCallbackFlags flags,
                                 void *data);

static void native_finished_callback(void *data);

// --------------------------------------------------------------------------------

inline static session_t new_session()
{
    session_t session = fa_new(audio_session);
    session->impl = &audio_session_impl;
    return session;
}

inline static void session_init_devices(session_t session)
{
    native_index_t count;
    list_t         devices;

    count   = Pa_GetDeviceCount();
    devices = fa_list_empty();

    for (size_t i = 0; i < count; ++i) {
        device_t device = new_device(session, i);

        if (device) {
            devices = fa_list_dcons(device, devices);
        }
    }

    session->devices      = fa_list_dreverse(devices);
    session->def_input    = new_device(session, Pa_GetDefaultInputDevice());
    session->def_output   = new_device(session, Pa_GetDefaultOutputDevice());
}

inline static void delete_session(session_t session)
{
    // TODO free device list
    fa_delete(session);
}

inline static device_t new_device(session_t session, native_index_t index)
{
    if (index == paNoDevice) {
        return NULL;
    }

    device_t device = fa_new(audio_device);
    device->impl    = &audio_device_impl;

    const PaDeviceInfo  *info      = Pa_GetDeviceInfo(index);
    const PaHostApiInfo *host_info = Pa_GetHostApiInfo(info->hostApi);

    device->index       = index;
    device->session     = session;
    device->name        = string((char *) info->name);      // const cast
    device->host_name   = string((char *) host_info->name);
    // device->muted       = false;
    // device->volume      = 1.0;

    return device;
}

inline static void delete_device(device_t device)
{
    fa_destroy(device->name);
    fa_destroy(device->host_name);
    fa_delete(device);
}

inline static stream_t new_stream(device_t input, device_t output, double sample_rate, long max_buffer_size)
{
    stream_t stream         = fa_new(audio_stream);

    stream->impl            = &audio_stream_impl;

    stream->input           = input;
    stream->output          = output;
    stream->input_channels  = fa_audio_input_channels(input);
    stream->output_channels = fa_audio_output_channels(output);

    stream->sample_rate     = sample_rate;
    stream->max_buffer_size = max_buffer_size;

    stream->signal_count    = 0;
    stream->sample_count    = 0;

    stream->in_controls     = atomic_queue();
    stream->controls        = priority_queue();

    return stream;
}

inline static void delete_stream(stream_t stream)
{
    // fa_destroy(stream->incoming);
    // fa_destroy(stream->outgoing);
    fa_delete(stream);
}


// --------------------------------------------------------------------------------

void fa_audio_initialize()
{
    pa_mutex        = fa_thread_create_mutex();
    pa_status       = false;
    current_session = NULL;
}

void fa_audio_terminate()
{
    fa_thread_destroy_mutex(pa_mutex);
}

// --------------------------------------------------------------------------------

session_t fa_audio_begin_session()
{
    if (!pa_mutex) {
        assert(false && "Module not initalized");
    }

    inform(string("Initializing real-time audio session"));

    fa_thread_lock(pa_mutex);
    {
        if (pa_status) {
            fa_thread_unlock(pa_mutex);
            return (session_t) audio_device_error(string("Overlapping real-time audio sessions"));
        } else {
            Pa_Initialize();
            pa_status = true;
            fa_thread_unlock(pa_mutex);

            session_t session = new_session();
            session_init_devices(session);

            current_session = session;
            return session;
        }
    }
}

void fa_audio_end_session(session_t session)
{
    if (!pa_mutex) {
        assert(false && "Module not initalized");
    }

    inform(string("Terminating real-time audio session"));

    fa_thread_lock(pa_mutex);
    {
        if (pa_status) {
            Pa_Terminate();
            pa_status = false;
        }

        current_session = NULL;
    }
    fa_thread_unlock(pa_mutex);
    delete_session(session);
}

void fa_audio_with_session(session_callback_t session_callback,
                           ptr_t                           session_data,
                           error_callback_t                error_callback,
                           ptr_t                           error_data
                          )
{
    session_t session = fa_audio_begin_session();

    if (fa_check(session)) {
        error_callback(error_data, (error_t) session);
    } else {
        session_callback(session_data, session);
    }

    fa_audio_end_session(session);
}

fa_list_t fa_audio_current_sessions()
{
    if (!current_session) {
        return empty();
    } else {
        return list(current_session);
    }
}

fa_ptr_t fa_audio_end_all_sessions()
{
    fa_dfor_each(x, fa_audio_current_sessions()) {
        fa_audio_end_session(x);
    }
    return NULL;
}

fa_list_t fa_audio_all(session_t session)
{
    return fa_copy(session->devices);
}

#define fail_if_no_input(type) \
    if (!session->def_input) { \
        return (type) audio_device_error(string("No input device available")); \
    }
#define fail_if_no_output(type) \
    if (!session->def_output) { \
        return (type) audio_device_error(string("No output device available")); \
    }

fa_pair_t fa_audio_default(session_t session)
{       
    fail_if_no_input(fa_pair_t);
    fail_if_no_output(fa_pair_t);
    return pair(session->def_input, session->def_output);
}

device_t fa_audio_default_input(session_t session)
{
    fail_if_no_input(device_t);
    return session->def_input;
}

device_t fa_audio_default_output(session_t session)
{
    fail_if_no_output(device_t);
    return session->def_output;
}

session_t fa_audio_session(device_t device)
{
    return device->session;
}

fa_string_t fa_audio_name(device_t device)
{
    return fa_copy(device->name);
}

fa_string_t fa_audio_host_name(device_t device)
{
    return fa_copy(device->host_name);
}

int fa_audio_input_channels(device_t device)
{
    const PaDeviceInfo *info = Pa_GetDeviceInfo(device->index);
    return info->maxInputChannels;
}

int fa_audio_output_channels(device_t device)
{
    const PaDeviceInfo *info = Pa_GetDeviceInfo(device->index);
    return info->maxOutputChannels;
}

bool fa_audio_has_input(device_t device)
{
    return fa_audio_input_channels(device) != 0;
}
bool fa_audio_has_output(device_t device)
{
    return fa_audio_output_channels(device) != 0;
}


void add_audio_status_listener(status_callback_t function, ptr_t data);

void fa_audio_add_status_callback(status_callback_t function,
                                  ptr_t             data,
                                  session_t         session)
{
    assert(session && "Not a real session");

    // See platform/*/device.c
    add_audio_status_listener(function, data);
}

// --------------------------------------------------------------------------------

#define kOutputOffset 0
#define kInputOffset 8
#define kControlOffset 16

void audio_inform_opening(device_t input, ptr_t proc, device_t output)
{
    inform(string("Opening real-time audio stream"));
    inform(string_dappend(string("    Input:  "), input ? fa_string_show(input) : string("-")));
    inform(string_dappend(string("    Output: "), output ? fa_string_show(output) : string("-")));
}

// TODO change sample rate
// TODO use unspec vector size if we can determine max
stream_t fa_audio_open_stream(device_t input,
                              device_t output,
                              proc_t proc,
                              ptr_t proc_data
                             )
{
    PaError         status;
    unsigned long   buffer_size = 16;
    double          sample_rate = 44100;

    if (!input && !output) {
        return (stream_t) audio_device_error_with(
            string("Can not open a stream with no devices"), 0);
    }

    stream_t        stream = new_stream(input, output, sample_rate, buffer_size);

    // TODO number of inputs
    list_t all_inputs = list(fa_signal_input(kInputOffset + 0), fa_signal_input(kInputOffset + 1));

    list_t all_signals = all_inputs;

    if (proc) {
        all_signals = proc(proc_data, all_inputs);
    }

    stream->signal_count        = fa_list_length(all_signals);

    for (int i = 0; i < stream->signal_count; ++i) {
        stream->signals[i] = fa_list_index(i, all_signals);
    }

    audio_inform_opening(input, all_signals, output);
    {
        PaStreamParameters inp = {
            .suggestedLatency           = 0,
            .hostApiSpecificStreamInfo  = NULL,
            .device                     = (input ? input->index : 0),
            .sampleFormat               = (paFloat32 | paNonInterleaved),
            .channelCount               = stream->input_channels
        };

        PaStreamParameters outp = {
            .suggestedLatency           = 0,
            .hostApiSpecificStreamInfo  = NULL,
            .channelCount               = stream->output_channels,
            .sampleFormat               = (paFloat32 | paNonInterleaved),
            .device                     = (output ? output->index : 0)
        };

        const PaStreamParameters       *in       = input ? &inp : NULL;
        const PaStreamParameters       *out      = output ? &outp : NULL;
        PaStreamFlags                   flags    = paNoFlag;
        PaStreamCallback               *callback = native_audio_callback;
        ptr_t                           data     = stream;

        status = Pa_OpenStream(&stream->native, in, out, sample_rate, buffer_size, flags, callback, data);

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }

        status = Pa_SetStreamFinishedCallback(stream->native, native_finished_callback);

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }
    }
    {
        before_processing(stream);

        status = Pa_StartStream(stream->native);

        if (status != paNoError) {
            return (stream_t) audio_device_error_with(string("Could not start stream"), status);
        }
    }

    return stream;
}

void fa_audio_close_stream(stream_t stream)
{
    inform(string("Closing real-time audio stream"));

    // Note that after_processing will be called from native_finished_callback
    Pa_CloseStream(stream->native);
    delete_stream(stream);
}

void fa_audio_with_stream(device_t            input,
                          device_t            output,
                          proc_t              proc,
                          ptr_t               proc_data,
                          stream_callback_t   stream_callback,
                          ptr_t               stream_data,
                          error_callback_t    error_callback,
                          ptr_t               error_data)
{
    stream_t stream = fa_audio_open_stream(input, output, proc, proc_data);

    if (fa_check(stream)) {
        error_callback(error_data, (error_t) stream);
    } else {
        stream_callback(stream_data, stream);
    }

    fa_audio_close_stream(stream);
}

list_t fa_audio_devices(fa_audio_stream_t stream)
{
    if (fa_equal(stream->input, stream->output)) {
        return list(stream->input);
    } else {
        return list(stream->input, stream->output);
    }
}

fa_clock_t fa_audio_stream_clock(fa_audio_stream_t stream)
{
    return (fa_clock_t) stream;
}

void fa_audio_add_message_callback(fa_audio_message_callback_t function,
                                   fa_ptr_t data,
                                   fa_audio_stream_t stream)
{
    // TODO
}

void fa_audio_schedule(fa_time_t time,
                       fa_action_t action,
                       fa_audio_stream_t stream)
{
    pair_left_t pair = pair_left(time, action);
    fa_atomic_queue_write(stream->in_controls, pair);
}



// --------------------------------------------------------------------------------

#define VALS inputs
#define MERGED_SIGNAL signals[0]


void before_processing(stream_t stream)
{
    stream->state      = new_state();

    signal_t merged = fa_signal_constant(0);

    for (int c = 0; c < stream->signal_count; ++c) {
        signal_t withOutput = fa_signal_output(0, kOutputOffset + c, stream->signals[c]);
        merged = fa_signal_former(merged, withOutput); // Could use any combinator here
    }

    fa_for_each(x, fa_signal_get_procs(merged)) {
        // printf("Adding custom proc %p!\n", x);
        add_custom_proc(x, stream->state);
    }
    stream->MERGED_SIGNAL = fa_signal_simplify(merged);
    fa_print_ln(stream->MERGED_SIGNAL);
    // TODO optimize
    // TODO verify

    run_custom_procs(0, stream->state);
}

void after_processing(stream_t stream)
{
    run_custom_procs(2, stream->state);
    delete_state(stream->state);
}

void during_processing(stream_t stream, unsigned count, float **input, float **output)
{
    ptr_t val;

    while ((val = fa_atomic_queue_read(stream->in_controls))) {
        fa_priority_queue_insert(fa_pair_left_from_pair(val), stream->controls);
    }

    for (int i = 0; i < count; ++ i) {

        // Note: This could be done outside sample loop
        //       which would be faster but less exact
        run_actions(stream->controls, stream->state);
        run_custom_procs(1, stream->state);

        for (int c = 0; c < stream->signal_count; ++c) {
            stream->state->VALS[c + kInputOffset] = input[c][i];
        }

        step(stream->MERGED_SIGNAL, stream->state);

        // TODO run inserts
        // We must know about all current inserts and pass the state
        // so that the buffers can be updated

        for (int c = 0; c < stream->signal_count; ++c) {
            output[c][i] = stream->state->VALS[c + kOutputOffset];
        }

        inc_state(stream->state);
    }

    stream->sample_count += count; // TODO atomic incr
}



/* The callbacks */

int native_audio_callback(const void                       *input,
                          void                             *output,
                          unsigned long                     count,
                          const PaStreamCallbackTimeInfo   *time_info,
                          PaStreamCallbackFlags             flags,
                          void                             *data)
{
    during_processing(data, count, (float **) input, (float **) output);
    return paContinue;
}

void native_finished_callback(void *data)
{
    after_processing(data);
}


// --------------------------------------------------------------------------------

bool audio_session_equal(ptr_t a, ptr_t b)
{
    return a == b;
}

fa_string_t audio_session_show(ptr_t a)
{
    string_t str = string("<AudioSession ");
    str = string_dappend(str, fa_string_format_integral("%p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_session_destroy(ptr_t a)
{
    fa_audio_end_session(a);
}

ptr_t audio_session_impl(fa_id_t interface)
{
    static fa_equal_t audio_session_equal_impl
        = { audio_session_equal };
    static fa_string_show_t audio_session_show_impl
        = { audio_session_show };
    static fa_destroy_t audio_session_destroy_impl
        = { audio_session_destroy };

    switch (interface) {
    case fa_equal_i:
        return &audio_session_equal_impl;

    case fa_string_show_i:
        return &audio_session_show_impl;

    case fa_destroy_i:
        return &audio_session_destroy_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

bool audio_device_equal(ptr_t a, ptr_t b)
{
    device_t device1 = (device_t) a;
    device_t device2 = (device_t) b;
    // TODO check that session is valid
    return device1->index == device2->index;
}

fa_string_t audio_device_show(ptr_t a)
{
    device_t device = (device_t) a;

    string_t str = string("<AudioDevice ");
    str = string_dappend(str, fa_audio_host_name(device));
    str = string_dappend(str, string(" "));
    str = string_dappend(str, fa_audio_name(device));
    str = string_dappend(str, string(">"));
    return str;
}

ptr_t audio_device_impl(fa_id_t interface)
{
    static fa_equal_t audio_device_equal_impl
        = { audio_device_equal };
    static fa_string_show_t audio_device_show_impl
        = { audio_device_show };

    switch (interface) {
    case fa_equal_i:
        return &audio_device_equal_impl;

    case fa_string_show_i:
        return &audio_device_show_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

fa_string_t audio_stream_show(ptr_t a)
{
    string_t str = string("<AudioStream ");
    str = string_dappend(str, fa_string_format_integral(" %p", (long) a));
    str = string_dappend(str, string(">"));
    return str;
}

void audio_stream_destroy(ptr_t a)
{
    fa_audio_close_stream(a);
}

int64_t audio_stream_milliseconds(ptr_t a)
{
    stream_t stream = (stream_t) a;
    double c = (double) stream->state->count;
    double r = (double) stream->state->rate;
    return ((int64_t)(c / r * 1000));
}

fa_time_t audio_stream_time(ptr_t a)
{
    int64_t ms = audio_stream_milliseconds(a);
    return fa_milliseconds(ms);
}


ptr_t audio_stream_impl(fa_id_t interface)
{
    static fa_string_show_t audio_stream_show_impl
        = { audio_stream_show };
    static fa_clock_interface_t audio_stream_clock_impl
        = { audio_stream_time, audio_stream_milliseconds };
    static fa_destroy_t audio_stream_destroy_impl
        = { audio_stream_destroy };

    switch (interface) {

    case fa_clock_interface_i:
        return &audio_stream_clock_impl;

    case fa_string_show_i:
        return &audio_stream_show_impl;

    case fa_destroy_i:
        return &audio_stream_destroy_impl;

    default:
        return NULL;
    }
}


// --------------------------------------------------------------------------------

void fa_fa_log_error_from(fa_string_t msg, fa_string_t origin);

error_t audio_device_error(string_t msg)
{
    return fa_error_create_simple(error,
                                  msg,
                                  string("Doremir.Device.Audio"));
}

error_t audio_device_error_with(string_t msg, int code)
{
    return fa_error_create_simple(error,
                                  string_dappend(msg, format_integral(" (error code %d)", code)),
                                  string("Doremir.Device.Audio"));
}

void audio_device_fatal(string_t msg, int code)
{
    fa_fa_log_error_from(
        string_dappend(msg, format_integral(" (error code %d)", code)),
        string("Doremir.Device.Audio"));

    fa_fa_log_error(string("Terminating Fa"));
    exit(error);
}

