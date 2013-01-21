(in-package :doremir)
(defctype processor-info (:pointer :void))
(defctype processor-samples ptr)
(defctype processor-interface (:pointer :void))
(define-foreign-type processor-type () () (:actual-type :pointer))
(define-parse-method processor () (make-instance 'processor-type))
(defclass processor () ((processor-ptr :initarg :processor-ptr)))
(defmethod translate-to-foreign (x (type processor-type)) (slot-value x 'processor-ptr))
(defmethod translate-from-foreign (x (type processor-type)) (make-instance 'processor :processor-ptr x))
(defcfun (processor-unary "doremir_processor_unary") processor (a type) (b type) (c unary) (d ptr))
(defcfun (processor-binary "doremir_processor_binary") processor (a type) (b type) (c type) (d binary) (e ptr))
(defcfun (processor-ternary "doremir_processor_ternary") processor (a type) (b type) (c type) (d type) (e ternary))
(defcfun (processor-identity "doremir_processor_identity") processor (a type))
(defcfun (processor-constant "doremir_processor_constant") processor (a type) (b type) (c ptr))
(defcfun (processor-delay "doremir_processor_delay") processor (a type) (b :int32))
(defcfun (processor-split "doremir_processor_split") processor (a type))
(defcfun (processor-seq "doremir_processor_seq") processor (a processor) (b processor))
(defcfun (processor-par "doremir_processor_par") processor (a processor) (b processor))
(defcfun (processor-loop "doremir_processor_loop") processor (a processor))