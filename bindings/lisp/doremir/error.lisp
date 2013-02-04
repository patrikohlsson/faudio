(in-package :audio-engine)
(defctype error-severity :int)
(defctype error-interface (:pointer :void))
(define-foreign-type error-type () () (:actual-type :pointer))
(define-parse-method error () (make-instance 'error-type))
(defclass error () ((error-ptr :initarg :error-ptr)))
(defmethod translate-to-foreign (x (type error-type)) (slot-value x 'error-ptr))
(defmethod translate-from-foreign (x (type error-type)) (make-instance 'error :error-ptr x))
(defctype error-callback (:pointer (:pointer :void)))
(defcfun (error-create-simple "doremir_error_create_simple") error (a error-severity) (b string) (c string))
(defcfun (error-severity "doremir_error_severity") error-severity (a error))
(defcfun (error-message "doremir_error_message") string (a error))
(defcfun (error-origin "doremir_error_origin") string (a error))
(defcfun (error-log "doremir_error_log") :void (a ptr) (b error))
(defcfun (error-format "doremir_error_format") string (a :boolean) (b error))
(defcfun (error-check "doremir_error_check") :boolean (a ptr))