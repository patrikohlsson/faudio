

/** Begin a new session, and retain it for the duration of a call to the given function.

    The given function will be called once after the session has created. The session
    will be ended after the callback function has returned. If an error occurs while
    starting the session, the error callback is invoked in place of the session callback.

    @param callback                     Function to receive the sesssion.
    @param error_callback               Function to receive eventual errors.
    @param error_data, session_data     Data closed over by the callbacks.
 */
void fa_midi_with_session(
    fa_midi_session_callback_t session_callback,
    fa_ptr_t                           session_data,
    fa_error_callback_t                error_callback,
    fa_ptr_t                           error_data
) {}

/** Begin a new midi session.

    @return
        A new session.
    @par Errors
        Fails if the session could not be started.
 */
fa_midi_session_t fa_midi_begin_session() {}

/** End the given session.

    @param session
        Session to end.
 */
void fa_midi_end_session(
    fa_midi_session_t session) {}


// --------------------------------------------------------------------------------

/** Get all active midi devices of the given session.

    @param session   The session.
    @return
        A list of @ref fa_midi_device_t.
 */
fa_list_t fa_midi_all(
    fa_midi_session_t session) {}

/** Get the standard devices of the given session.

    @param session   The session.
    @return
        A pair of @ref fa_midi_device_t representing the default input and
        output device, respectively.
 */
fa_pair_t fa_midi_default(
    fa_midi_session_t session) {}

/** Get the standard input device of the given session.
    @param session   The session.
 */
fa_midi_device_t fa_midi_default_input(
    fa_midi_session_t session) {}

/** Get the standard output device of the given session.
    @param session   The session.
 */
fa_midi_device_t fa_midi_default_output(
    fa_midi_session_t session) {}

/** Return the name of the given device.
    @param device   The device.
 */
fa_string_t fa_midi_name(
    fa_midi_device_t device) {}

/** Return the host name of the given device.
    @param device   The device.
 */
fa_string_t fa_midi_host_name(
    fa_midi_device_t device) {}

/** Return whether the given device has input or not.
    @param device   The device.
 */
bool fa_midi_has_input(fa_midi_device_t) {}

/** Return whether the given device has output or not.
    @param device   The device.
 */
bool fa_midi_has_output(fa_midi_device_t) {}

/** Set a callback to be invoked when a status change is detected on the
    given session (mainly useful for hardware setup changes).

    Note that this function will not modify the devices in a session, you have to
    restart the session to get a fresh snapshot.

    @param device   The device.
    @warning
        On OS X this function must be called from the main thread.
 */
void fa_midi_set_status_callback(
    fa_midi_status_callback_t callback,
    fa_ptr_t                          function,
    fa_midi_session_t         session) {}

// --------------------------------------------------------------------------------

/**
    Open a stream on the given devices.

    @param device   The device.
    @return         A new stream or an error if no stream could be opened.
 */
fa_midi_stream_t
fa_midi_open_stream(
    fa_midi_device_t    device) {}

/**
    Close the given stream.
    @param session
        Stream to close.
 */
void fa_midi_close_stream(
    fa_midi_stream_t stream) {}

/**
    Run a stream on the given devices.

    @param device
        The device.
    @param callback
        Function to receive the stream.
    @param error_callback
        Function to receive eventual errors.
 */
void fa_midi_with_stream(
    fa_midi_device_t                   device,
    fa_midi_stream_callback_t          stream_callback,
    fa_ptr_t                           stream_data,
    fa_error_callback_t                error_callback,
    fa_ptr_t                           error_data
) {}
