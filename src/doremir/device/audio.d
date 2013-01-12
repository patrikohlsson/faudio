
/** Run a session.
    @param callback 
        Function to receive the sesssion.
 */
void doremir_device_audio_with_session(doremir_device_audio_session_callback_t callback) {}

/** Begin a new session.
    @return 
        A new session, or NULL if no session could be started.
 */
doremir_device_audio_session_t doremir_device_audio_begin_session() {}

/** End the given session.
    @param session 
        Session to end.
 */
void doremir_device_audio_end_session(doremir_device_audio_session_t session) {}

// --------------------------------------------------------------------------------

/** Get all active audio devices of the given session.
    @return
        A list of @ref doremir_device_audio_t.
 */
doremir_list_t doremir_device_audio_all(doremir_device_audio_session_t session) {}

/** Get the standard devices of the given session.
    @return
        A pair of @ref doremir_device_audio_t.
 */
doremir_pair_t doremir_device_audio_default(doremir_device_audio_session_t session) {}

/** Return the name of the given device.
 */
doremir_string_t doremir_device_audio_name(doremir_device_audio_t) {}

/** Return the host name of the given device.
 */
doremir_string_t doremir_device_audio_host_name(doremir_device_audio_t) {}

/** Return the number of channels of the given device.
    @return
        A pair of int32_t, denoting number of inputs and outputs, respectively.
 */
doremir_pair_t doremir_device_audio_channels(doremir_device_audio_t) {}

// --------------------------------------------------------------------------------

/** Run a stream on the given devices.
    @param input
        Input device.
    @param processor
        Processor to run over the devices.
    @param input
        Output device.
    @param callback
        Function to receive the stream.
 */
void doremir_device_audio_with_stream(doremir_device_audio_t                 input,
                                      doremir_processor_any_t                processor,
                                      doremir_device_audio_t                 output,
                                      doremir_device_audio_stream_callback_t callback) {}

/** Open a stream on the given devices.
    @param input
        Input device.
    @param processor
        Processor to run over the devices.
    @param input
        Output device.
    @return
        A new stream or NULL if no stream could be started.
 */
doremir_device_audio_stream_t 
doremir_device_audio_start_stream(doremir_device_audio_t    input,
                                  doremir_processor_any_t   processor,
                                  doremir_device_audio_t    output) {}

/** Close the given stream.
    @param session 
        Stream to close.
 */
void doremir_device_audio_stop_stream(doremir_device_audio_stream_t stream) {}
