(in-package :faudio)
(define-foreign-type audio-session-type () () (:actual-type :pointer))
(define-parse-method audio-session () (make-instance 'audio-session-type))
(defclass audio-session () ((audio-session-ptr :initarg :audio-session-ptr)))
(defmethod translate-to-foreign (x (type audio-session-type)) (slot-value x 'audio-session-ptr))
(defmethod translate-from-foreign (x (type audio-session-type)) (make-instance 'audio-session :audio-session-ptr x))
(define-foreign-type audio-device-type () () (:actual-type :pointer))
(define-parse-method audio-device () (make-instance 'audio-device-type))
(defclass audio-device () ((audio-device-ptr :initarg :audio-device-ptr)))
(defmethod translate-to-foreign (x (type audio-device-type)) (slot-value x 'audio-device-ptr))
(defmethod translate-from-foreign (x (type audio-device-type)) (make-instance 'audio-device :audio-device-ptr x))
(define-foreign-type audio-stream-type () () (:actual-type :pointer))
(define-parse-method audio-stream () (make-instance 'audio-stream-type))
(defclass audio-stream () ((audio-stream-ptr :initarg :audio-stream-ptr)))
(defmethod translate-to-foreign (x (type audio-stream-type)) (slot-value x 'audio-stream-ptr))
(defmethod translate-from-foreign (x (type audio-stream-type)) (make-instance 'audio-stream :audio-stream-ptr x))
(defctype audio-session-callback (:pointer (:pointer :void)))
(defctype audio-stream-callback (:pointer (:pointer :void)))
(defctype audio-status-callback nullary)
(defctype audio-proc (:pointer (:pointer :void)))
(defcfun (audio-begin-session "fa_audio_begin_session") audio-session)
(defcfun (audio-end-session "fa_audio_end_session") :void (a audio-session))
(defcfun (audio-with-session "fa_audio_with_session") :void (a audio-session-callback) (b ptr) (c error-callback) (d ptr))
(defcfun (audio-current-sessions "fa_audio_current_sessions") list)
(defcfun (audio-end-all-sessions "fa_audio_end_all_sessions") ptr)
(defcfun (audio-all "fa_audio_all") list (a audio-session))
(defcfun (audio-default "fa_audio_default") pair (a audio-session))
(defcfun (audio-default-input "fa_audio_default_input") audio-device (a audio-session))
(defcfun (audio-default-output "fa_audio_default_output") audio-device (a audio-session))
(defcfun (audio-set-status-callback "fa_audio_set_status_callback") :void (a audio-status-callback) (b ptr) (c audio-session))
(defcfun (audio-session "fa_audio_session") audio-session (a audio-device))
(defcfun (audio-name "fa_audio_name") string (a audio-device))
(defcfun (audio-host-name "fa_audio_host_name") string (a audio-device))
(defcfun (audio-has-input "fa_audio_has_input") :boolean (a audio-device))
(defcfun (audio-has-output "fa_audio_has_output") :boolean (a audio-device))
(defcfun (audio-input-channels "fa_audio_input_channels") :int (a audio-device))
(defcfun (audio-output-channels "fa_audio_output_channels") :int (a audio-device))
(defcfun (audio-open-stream "fa_audio_open_stream") audio-stream (a audio-device) (b audio-device) (c audio-proc) (d ptr))
(defcfun (audio-close-stream "fa_audio_close_stream") :void (a audio-stream))
(defcfun (audio-with-stream "fa_audio_with_stream") :void (a audio-device) (b audio-device) (c audio-proc) (d ptr) (e audio-stream-callback) (f ptr) (g error-callback) (h ptr))
(defcfun (audio-devices "fa_audio_devices") list (a audio-stream))
(defcfun (audio-schedule "fa_audio_schedule") :void (a time) (b action) (c audio-stream))

