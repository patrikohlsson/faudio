
var ffi = require('ffi')

var ae_ = ffi.Library('libae', {
    'doremir_audio_engine_version':             ['pointer', [] ],
    'doremir_audio_engine_version_string':      ['pointer', [] ],
    'doremir_string_to_utf8':                   ['string',  ['pointer'] ],
                                                
    'doremir_audio_engine_initialize':          ['void',    [] ],
    'doremir_audio_engine_terminate':           ['void',    [] ],
                                                
    'doremir_audio_engine_set_log':             ['void',    ['pointer','pointer']],
    'doremir_audio_engine_set_log_std':         ['void',    []],
                                                
                                                
    'doremir_list_empty':                       ['pointer', []],
    'doremir_list_cons':                        ['pointer', ['pointer','pointer']],
    'doremir_list_is_empty':                    ['bool',    ['pointer']],
    'doremir_list_head':                        ['pointer', ['pointer']],
    'doremir_list_tail':                        ['pointer', ['pointer']],
                                                
    'doremir_dynamic_get_type':                 ['int',     ['pointer']],
                                                
    'doremir_pair_create':                      ['pointer', ['pointer', 'pointer']],
    'doremir_pair_fst':                         ['pointer', ['pointer']],
    'doremir_pair_snd':                         ['pointer', ['pointer']],    
                                                
    'doremir_to_bool':                          ['bool',    ['pointer']],
    'doremir_to_int8':                          ['int',     ['pointer']],
    'doremir_to_int16':                         ['int',     ['pointer']],
    'doremir_peek_int32':                       ['int',     ['pointer']], 
    'doremir_peek_int64':                       ['int',     ['pointer']], 
    'doremir_peek_float':                       ['double',  ['pointer']], 
    'doremir_peek_double':                      ['double',  ['pointer']], 
                                                
    'doremir_device_audio_begin_session':       ['pointer', []],
    'doremir_device_audio_end_session':         ['void',    ['pointer']],
    'doremir_device_audio_with_session':        ['void',    ['pointer','pointer','pointer','pointer']],
    'doremir_device_audio_all':                 ['pointer', ['pointer']],
    'doremir_device_audio_default':             ['pointer', ['pointer']],
    'doremir_device_audio_set_status_callback': ['pointer', ['pointer']],
    'doremir_device_audio_name':                ['pointer', ['pointer']],
    'doremir_device_audio_host_name':           ['pointer', ['pointer']],
    'doremir_device_audio_has_input':           ['pointer', ['pointer']],
    'doremir_device_audio_has_output':          ['pointer', ['pointer']],
    'doremir_device_audio_input_type':          ['pointer', ['pointer']],
    'doremir_device_audio_output_type':         ['pointer', ['pointer']],
    'doremir_device_audio_open_stream':         ['pointer', ['pointer','pointer','pointer']],
    'doremir_device_audio_close_stream':        ['void',    ['pointer']],
    'doremir_device_audio_with_stream':         ['void',    ['pointer','pointer','pointer','pointer','pointer','pointer','pointer']],
    
    'doremir_error_check':                      ['bool', ['pointer']],
    'doremir_error_message':                    ['pointer', ['pointer']],
    'doremir_string_show':                      ['pointer', ['pointer']],
})

var bool_ = ae_.doremir_to_bool;
var i8_   = ae_.doremir_to_int8;
var i16_  = ae_.doremir_to_int16;
var i32_  = ae_.doremir_peek_int32;
var i64_  = ae_.doremir_peek_int64;
var f32_  = ae_.doremir_peek_float;
var f64_  = ae_.doremir_peek_double;

var pair_ = function (a) {
    var x = ae_.doremir_pair_fst(as); 
    var y = ae_.doremir_pair_snd(as);
    return [x,y];
}

var list_   = function(as) {
    if (ae_.doremir_list_is_empty(as)) {
        return [];
    } else {
        var b  = ae_.doremir_list_head(as);
        var bs = ae_.doremir_list_tail(as);
        return [dyn_(b)].concat(list_(bs));
    }
}

var string_     = ae_.doremir_string_to_utf8;
var check_      = ae_.doremir_error_check;
var message_    = ae_.doremir_error_message;
var show_       = ae_.doremir_string_show;

var dyn_ = function(a) {
    var t = ae_.doremir_dynamic_get_type(a);
    switch (t) {
        case 0:     return bool_(a);
        case 1:     return i8_(a);
        case 2:     return i16_(a);
        case 3:     return i32_(a);
        case 4:     return i64_(a);
        case 5:     return f32_(a);
        case 6:     return f64_(a);
        case 7:     return pair_(a);
        case 8:     return list_(a);
        // case 9:     return set_(a);
        // case 10:    return map_(a);
        case 11:    return string_(a);
        // case 12:    return ratio_(a);
        default:    throw "dyn_: Missing case";
    }
}         

var thowIfErr = function (a) {
    if (check_(a)) {
        var m = string_(message_(a));
        throw "Error: " + m;
    } else {
        return a;
    }
}

var ae = {
    version                     : function() { return dyn_(
                                    ae_.doremir_audio_engine_version() )},
    versionString               : function() { return string_(
                                    ae_.doremir_audio_engine_version_string() )} ,
    initialize                  : ae_.doremir_audio_engine_initialize,
    terminate                   : ae_.doremir_audio_engine_terminate,
    setLog                      : ae_.doremir_audio_engine_set_log,
    setLogStd                   : ae_.doremir_audio_engine_set_log_std,

    device : {
        audio : {
            beginSession        : function () { return thowIfErr(
                                    ae_.doremir_device_audio_begin_session() )},
            endSession          : ae_.doremir_device_audio_end_session,
            withSession         : ae_.doremir_device_audio_with_session,
            all                 : ae_.doremir_device_audio_all,
            defaults            : ae_.doremir_device_audio_default,
            setStatusCallback   : ae_.doremir_device_audio_set_status_callback,
            name                : ae_.doremir_device_audio_name,
            hostName            : ae_.doremir_device_audio_host_name,
            hasInput            : ae_.doremir_device_audio_has_input,
            hasOutput           : ae_.doremir_device_audio_has_output,
            inputType           : ae_.doremir_device_audio_input_type,
            outputType          : ae_.doremir_device_audio_output_type,
            openStream          : ae_.doremir_device_audio_open_stream,
            closeStream         : ae_.doremir_device_audio_close_stream,
            withStream          : ae_.doremir_device_audio_with_stream,
        }
    },

    string : {
        toUtf8      : ae_.doremir_string_to_utf8,
    }
}
module.exports = ae;
