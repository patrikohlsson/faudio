(in-package :audio-engine)
(define-foreign-type priority-queue-type () () (:actual-type :pointer))
(define-parse-method priority-queue () (make-instance 'priority-queue-type))
(defclass priority-queue () ((priority-queue-ptr :initarg :priority-queue-ptr)))
(defmethod translate-to-foreign (x (type priority-queue-type)) (slot-value x 'priority-queue-ptr))
(defmethod translate-from-foreign (x (type priority-queue-type)) (make-instance 'priority-queue :priority-queue-ptr x))
(defcfun (priorityqueue-empty "doremir_priority_queue_empty") priority-queue)
(defcfun (priorityqueue-single "doremir_priority_queue_single") priority-queue (a ptr))
(defcfun (priorityqueue-destroy "doremir_priority_queue_destroy") :void (a priority-queue))
(defcfun (priorityqueue-merge "doremir_priority_queue_merge") :void (a priority-queue) (b priority-queue))
(defcfun (priorityqueue-insert "doremir_priority_queue_insert") :void (a ptr) (b priority-queue))
(defcfun (priorityqueue-peek "doremir_priority_queue_peek") ptr (a priority-queue))
(defcfun (priorityqueue-pop "doremir_priority_queue_pop") ptr (a priority-queue))