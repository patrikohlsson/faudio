(in-package :doremir)
(define-foreign-type device-audio-type () () (:actual-type :pointer))
(define-parse-method device-audio () (make-instance 'device-audio-type))
(defclass device-audio () ((device-audio-ptr :initarg :device-audio-ptr)))
(defmethod translate-to-foreign (x (type device-audio-type)) (slot-value x 'device-audio-ptr))
(defmethod translate-from-foreign (x (type device-audio-type)) (make-instance 'device-audio :device-audio-ptr x))
(define-foreign-type device-midi-type () () (:actual-type :pointer))
(define-parse-method device-midi () (make-instance 'device-midi-type))
(defclass device-midi () ((device-midi-ptr :initarg :device-midi-ptr)))
(defmethod translate-to-foreign (x (type device-midi-type)) (slot-value x 'device-midi-ptr))
(defmethod translate-from-foreign (x (type device-midi-type)) (make-instance 'device-midi :device-midi-ptr x))