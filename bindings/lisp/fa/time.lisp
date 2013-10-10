(in-package :faudio)
(define-foreign-type time-type () () (:actual-type :pointer))
(define-parse-method time () (make-instance 'time-type))
(defclass time () ((time-ptr :initarg :time-ptr)))
(defmethod translate-to-foreign (x (type time-type)) (slot-value x 'time-ptr))
(defmethod translate-from-foreign (x (type time-type)) (make-instance 'time :time-ptr x))
(defctype time-seconds :int32)
(defctype time-milliseconds :int64)
(defcfun (time-create "fa_time_create") time (a :int32) (b :int32) (c :int32) (d ratio))
(defcfun (time-copy "fa_time_copy") time (a time))
(defcfun (time-destroy "fa_time_destroy") :void (a time))
(defcfun (time-days "fa_time_days") :int32 (a time))
(defcfun (time-hours "fa_time_hours") :int32 (a time))
(defcfun (time-minutes "fa_time_minutes") :int32 (a time))
(defcfun (time-seconds "fa_time_seconds") :int32 (a time))
(defcfun (time-divisions "fa_time_divisions") ratio (a time))
(defcfun (time-to-iso "fa_time_to_iso") string (a time))
(defcfun (time-to-seconds "fa_time_to_seconds") time-seconds (a time))
(defcfun (time-to-milliseconds "fa_time_to_milliseconds") time-milliseconds (a time))
(define-foreign-type time-system-type () () (:actual-type :pointer))
(define-parse-method time-system () (make-instance 'time-system-type))
(defclass time-system () ((time-system-ptr :initarg :time-system-ptr)))
(defmethod translate-to-foreign (x (type time-system-type)) (slot-value x 'time-system-ptr))
(defmethod translate-from-foreign (x (type time-system-type)) (make-instance 'time-system :time-system-ptr x))

