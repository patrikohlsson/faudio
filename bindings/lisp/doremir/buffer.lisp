(defctype Doremir.Buffer :pointer)

(defcfun "Doremir.Buffer.create" :Doremir.Buffer (:size))

(defcfun "Doremir.Buffer.copy" :Doremir.Buffer (:Doremir.Buffer))

(defcfun "Doremir.Buffer.resize" :Doremir.Buffer (:size :Doremir.Buffer))

(defcfun "Doremir.Buffer.destroy" :void (:Doremir.Buffer))

(defcfun "Doremir.Buffer.size" :size (:Doremir.Buffer))

(defcfun "Doremir.Buffer.peek" :uint8 (:Doremir.Buffer :size))

(defcfun "Doremir.Buffer.poke" :void (:Doremir.Buffer :size :uint8))

(defcfun "Doremir.Buffer.unsafeAddress" (:pointer :void) (:Doremir.Buffer))