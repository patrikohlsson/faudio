

(in-package :faudio)

(defvar x nil)
(defvar y nil)
(defvar z nil)
(defvar s nil)
(defvar d nil)
(defvar p nil)


(setf x 1)
(setf y 2)
(setf x "hello")
(setf y "holla")
(setf x 77/88)
(setf y 33/44)
(setf x (export-list '()))
(setf y (export-list '(1 2 3)))

(equal              x y)
(less-than          x y)
(greater-than       x y)
(less-than-equal    x y)
(greater-than-equal x y)
(min                x y)
(max                x y)

(add                x y)
(multiply           x y)
(subtract           x y)
(divide             x y)
(absolute           x)

(copy               x)
(destroy            x)
(string-show        x)
(string-to-string   x)
(string-to-json     x)
(string-from-json "[1,2,3]")
(string-from-json "{\"foo\":null, \"bar\":[1,2,3]}")

; ---------------------------------------------------------------------------------------------------
;
; Fa

(fa-initialize)
(fa-terminate)
(fa-set-log-file "/Users/hans/Library/Logs/Faudio.log")
(fa-set-log-std)

(fa-log-info "What to say?")
(fa-log-warning "Beware!")
(fa-log-error "Rattlesnakes!")
(fa-log-error-from "Rattlesnakes!" "test")


; ---------------------------------------------------------------------------------------------------
;
; Fa.Error

(setf x (error-create-simple 2 "An error" "From.Here"))
(destroy x)

(error-check nil) ; nil
(error-check x)   ; t
(error-log nil (to-error x))

(error-message (to-error x))
(error-severity (to-error x))
(error-origin (to-error x))

; ---------------------------------------------------------------------------------------------------
;
; Fa.Dynamic
;
; Note:
;   Modulo is not smart enough to translate enums to Lisp yet
;   See the module file for the definition

(dynamic-get-type nil)
(dynamic-get-type t)
(dynamic-get-type 123)
(dynamic-get-type 3.14159265)
; (dynamic-get-type 6.02214129d23)
(dynamic-get-type (pair-create 1 2))
(dynamic-get-type (list-empty))
(dynamic-get-type (set-empty))
(dynamic-get-type (map-empty))
(dynamic-get-type "foo")
(dynamic-get-type 1/2)


; ---------------------------------------------------------------------------------------------------
; Data structures
; ---------------------------------------------------------------------------------------------------
;
; Fa.Ratio
;
; faudio ratios are interchangable with Lisp rationals.
; You can pass Lisp rationals to a function expecting fa_ratio_t and vice versa

(setf x (ratio-create 1 2))
(setf y (ratio-create 278 12))
(ratio-destroy x)

(ratio-num x)
(ratio-denom x)

(ratio-add x y)
(ratio-subtract x y)
(ratio-multiply x y)
(ratio-divide x y)
(ratio-succ x)
(ratio-pred x)
(ratio-negate x)
(ratio-recip x)

(ratio-create 1 2)
(ratio-succ (/ 1 2))
(ratio-recip (/ 567 235))

; ---------------------------------------------------------------------------------------------------
;
; Fa.String
;
; faudio strings are interchangable with Lisp strings.
; You can pass Lisp strings to a function expecting fa_string_t and vice versa
;
; Note that LispWorks does not support the full Unicode range (only UCS-2).

(setf x (string-empty))
(setf x (string-single 104))
(setf x (string-repeat 30 (char-int #\a)))
(setf x (string-append "hans " "h�glund"))
(destroy x)

(string-length "h�gtalare")
(code-char (string-char-at 0 "foo"))

(string-matches "a*b*c+" "aaacc")


; ---------------------------------------------------------------------------------------------------
;
; Fa.Pair
;
; faudio pairs are distinct from Lisp conses:
;
;   * faudio pairs print as (1,2), not as '(1 . 2).
;   * You can pass Lisp conses to functions expecting faudio pairs, but not the other way around.
;   * You can use (import-pair) and (export-pair) to convert, see example below.

(setf x (pair-create 1 2))
(setf y (pair-copy x))
(destroy x)

(pair-fst x)
(pair-snd x)
(pair-dup 3)
(pair-swap x)
(pair-assoc (pair-create 1 (pair-create 2 3)))
(pair-unassoc (pair-create (pair-create 1 2) 3))
(pair-snd (from-pointer 'pair (to-pointer (pair-create 1 2))))

; Conversion
(import-pair (pair-create 1 2)))
(export-pair '(1 . 2))

; ---------------------------------------------------------------------------------------------------

; Fa.List

; faudio lists are distinct from Lisp lists:
;
;   * faudio lists print as [1,2,3], not as '(1 2 3).
;   * You can pass Lisp lists to functions expecting faudio lists, but not the other way around.
;   * You can use (import-list) and (export-list) to convert, see example below.

(setf x (list-empty))
(setf x (list-single 0))
(setf x (list-dcons 1 x))
(setf x (list-dcons (random 20) x))
(setf x (list-dtail x))
(setf x (export-list '(1 2 3 4)))
(setf y (list-copy x))
(destroy x)

(list-is-empty x)
(list-is-single x)

(list-length x)
(list-head x)
(list-tail x)
(list-init x)
(list-last x)
(list-append x x)

(list-reverse x)
(list-sort x)
(list-take 5 x)
(list-drop 5 x)
(list-index 2 x)
(list-range 2 3 x)
(list-insert 2 23 x)
(list-remove 2 x)
(list-insert-range 2 (list-single 56) x)
(list-remove-range 2 3 x)

(list-has 1 x)
(list-index-of 1 x)
(list-join (list-empty))
(list-join (list-single (list-single 1)))

(list-find* 'evenp '(1 2 3 4))
(list-find-index* 'evenp '(1 2 3 4))
(list-filter* 'evenp '(1 2 3 4))
(list-map* (lambda (x) (+ 100 x)) '(1 2 3 4))

(find 'evenp '(1 2 3 4))
(find-index 'evenp '(1 2 3 4))
(filter 'evenp '(1 2 3 4))
(map (lambda (x) (+ 100 x)) '(1 2 3 4))
(join
 (list-cons
  (list-single 0)
  (list-single (list-cons 1 (list-single 2)))))


; Mixing faudio and Lisp lists
(list-append '(1 2 3) (list-single 4))
(list-dcons 1 '())
(list-is-empty '())
(list-is-single '(1))

(export-list '(1 2 3))
(import-list (export-list '(1 2 3)))


; ---------------------------------------------------------------------------------------------------

; Fa.Set

(setf x (set-empty))
(setf x (set-single 1))
(setf x (set-dadd 1 x))             ; Set.add does not overwrite equals
(setf x (set-dremove 1 x))          ; Set.set does (for numbers, it doesn't matter)
(setf x (set-dadd (random 20) x))
(setf x (set-dremove (random 20) x))
(setf y (set-copy x))
(destroy x)

(set-size x)
(set-is-empty x)
(set-is-single x)
(set-has 1 x)
(set-get 1 x)

(set-is-subset-of y x)
(set-is-proper-subset-of y x)

(set-sum x y)
(set-intersection x y)
(set-product x y)
(set-difference x y)

(set-to-list x)                   ; Convert to faudio list
(import-list (set-to-list x))     ; Convert to Lisp list

; ---------------------------------------------------------------------------------------------------

; Fa.Map

(setf x (map-empty))
(setf x (map-dadd "name" "hans" x))
(setf x (map-dadd "name" "sven" x))   ; Map.add does not overwrite equals
(setf x (map-dset "name" "sven" x))   ; Map.set does
(setf x (map-dset "skills"
                  (list-single 1) x))
(setf x (map-add-entry
         (pair-create "surname"
                      "h�glund") x))
(setf x (map-remove "name" x))        ; Map.remove removes if present, otherwise does nothing
(setf x (map-remove "skills" x))
(setf y (map-copy x))
(destroy x)

(map-size x)
(map-is-empty x)
(map-is-single x)

(map-has-key "name" x)
(map-has-key "skills" x)
(map-get "name" x)
(map-get "skills" x)

(map-is-submap-of x y)
(map-is-proper-submap-of x y)

(map-to-list x)
(mapcar #'import-pair 
        (import-list 
         (map-to-list x)))


; ---------------------------------------------------------------------------------------------------

; Fa.Buffer
;
; The faudio buffers are simply raw blocks of memory with a known size.

; All memory is allocated outside the control of Lisp and must be freed by destroying 
; the buffer. You can acces individual elements using the (get) function, copy the buffer 
; to the Lisp heap using (buffer-to-array) and copy it back using (array-to-buffer).
;
; Reading or writing outside the range is undefined, but fails with an error message in
; a debug build of the faudio.

(setf x (buffer-create 10))
(setf x (buffer-resize 20 x))
(setf x (buffer-create 1024))
(setf x (buffer-resize 2048 x))
(destroy x)

(cl:print x)

(size x)
(get x 0)
(setf (get x 0) #xff)

(dotimes (i (size x))
  (setf (get x i) (mod i 256)))
(dotimes (i (size x))
  (setf (get x i) 0))

; Typed get
; (buffer-get-int16 x 1)
; (buffer-set-int16 x 1 10)
; (buffer-get-int32 x 1)
; (buffer-set-int32 x 1 10)
; (buffer-get-int64 x 1)
; (buffer-set-int64 x 1 10)
(buffer-get-float x 1)
(buffer-set-float x 1 0.5)
(buffer-get-double x 1)
(buffer-set-double x 1 0.5d0)

; Conversion
(setf a (buffer-to-array x))
(setf x (array-to-buffer a))

; I/O
(setf x (buffer-read-audio* "/Users/hans/Desktop/test.wav"))
(setf x (buffer-read-audio* "does-not-exist.wav"))
(setf x 
      (from-pointer 'buffer (pair-second x)))

; ---------------------------------------------------------------------------------------------------

; Fa.Midi

(setf x (midi #x91 60 127))
(setf x (midi-create-sysex (buffer-create 1024)))
(setf y (midi-copy x))
(destroy x)

(midi-is-simple x)
(midi-status x)
(midi-channel x)
(midi-simple-data x)
(pair-fst (midi-simple-data x))
(pair-snd (midi-simple-data x))

(midi-is-sysex x)
(midi-sysex-data x)


; ---------------------------------------------------------------------------------------------------

; Fa.Time
;
; Use the (time) function to create a time value. All the given components are added, 
; together so (time :minutes 1 :seconds 2) is equivalent to (time :seconds 62).
;
; All components except seconds must be integers. Seconds may be rational or 
; real and are implicitly coerced to rationals. 

(time :minutes 1 :seconds 1/2)
(greater-than (time :minutes 1) (time :seconds 59))
(greater-than (time :minutes 1) (time :seconds 61))
(setf x (time :days 7))
(setf x (time :hours 1 :minutes 1 :seconds 2))
(setf x (time :seconds 3662))
(setf x (time :minutes 4 :seconds 1/5))
(setf x (time :minutes 4 :seconds 33.5))
(setf x (time :milliseconds 5512))
(setf x (time :nanoseconds 1341405))
(setf y (time-copy x))
(setf x (from-pointer 'time (add x y)))
(destroy x)

(time-days x)
(time-hours x)
(time-minutes x)
(time-seconds x)
(time-divisions x)

(time-to-iso x)
(time-to-milliseconds x)
(time-to-seconds x)


; Fa.Clock

(setf x) ; TODO

; Clock from an audio stream
(let* ((s (audio-begin-session))
       (i (audio-default-input s))
       (o (audio-default-output s))
       (st (audio-open-stream* i o (lambda (is) (duplicate (constant 0))))))
  (setf x (audio-stream-clock st)))

(clock-time x)
(clock-milliseconds x)


; ---------------------------------------------------------------------------------------------------
; Devices

; Fa.Device.Audio

(setf s (audio-begin-session))
(error-check s)
(error-log nil (to-error s))
(error-message (to-error s))
(audio-end-session s)

(audio-current-sessions)
(audio-end-all-sessions)

(audio-all s)
(setf x (audio-default-input s))
(setf y (audio-default-output s))

;(error-check nil)

(audio-name x)
(audio-host-name x)
(audio-has-input x)
(audio-has-output x)
(audio-input-channels x)
(audio-output-channels x)
;(audio-input-type x)
;(audio-output-type x)
;(type-channels (audio-input-type x))
;(type-channels (audio-output-type x))
;(type-size-of 1024 (audio-input-type x))

(setf s nil)
(audio-add-status-callback* (lambda ()
  (capi:display-message "Audio setup changed")
  (fa-log-info "Audio setup changed")) s)

(audio-close-stream z)


; ---------------------------------------------------------------------------------------------------

; Fa.Device.Midi

(setf s (midi-begin-session))
(error-check s)
(error-log nil (to-error s))
(error-message (to-error s))
(midi-end-session s)

(midi-current-sessions)
(midi-end-all-sessions)

(midi-all s)
(setf x (midi-default-input s))
(setf y (midi-default-output s))

(midi-name x)
(midi-name y)
(midi-host-name x)
(midi-has-input x)
(midi-has-output x)

; TODO status callback

(setf q (midi-open-stream x))
(setf z (midi-open-stream y))
(midi-close-stream z)
(midi-close-stream q)




; More complete examples:

(progn
  (setf s (midi-begin-session))
  (setf x (midi-default-input s))
  (setf y (midi-default-output s))
  (setf midi-in-stream (midi-open-stream x))
  (setf midi-out-stream (midi-open-stream y))
 )
(cl:print midi-in-stream)
(cl:print midi-out-stream)
(midi-close-stream midi-in-stream)
(midi-close-stream midi-out-stream)
(midi-end-all-sessions)


; Output
(progn
  (midi-schedule (milliseconds 0) (action-send "" (midi #x91 60 127)) midi-out-stream)
  (midi-schedule (milliseconds 100) (action-send "" (midi #x91 63 127)) midi-out-stream)
  (midi-schedule (milliseconds 200) (action-send "" (midi #x91 65 127)) midi-out-stream)
  (midi-schedule (milliseconds 300) (action-send "" (midi #x91 62 127)) midi-out-stream))

; Input
(defvar *msgs* nil)
(push 1 *msgs*)
(progn (mapcar 'cl:print *msgs*) nil)

(midi-add-message-callback* 
 (lambda (time msg)
   (push (cl:list time msg) *msgs*)
   (fa-log-info "Midi message received")
   ) midi-in-stream)

; ---------------------------------------------------------------------------------------------------
; Utility

; Fa.Plot

(setf b (buffer-create 1024))
(dotimes (i (/ (buffer-size b) 8))
  (let* ((i x)
         (y 0.0))
    (buffer-set-double b 0 0.5d1)))

(plot-buffer-double x nil nil)
(plot-buffer-float x nil nil)

; ---------------------------------------------------------------------------------------------------

; Fa.Atomic

(setf x (atomic-create))
(setf y (atomic-copy x))
(destroy x)

(atomic-exchange x 0 1) ; FIXME does not work from Lisp
(atomic-exchange x 1 0)
(atomic-get x)
(atomic-set x 0)

; ---------------------------------------------------------------------------------------------------

; Fa.Atomic.Queue

(setf x (atomic-queue-create))
(destroy x)
(atomic-queue-write x (cl:random 20))
(atomic-queue-read x)

; ---------------------------------------------------------------------------------------------------

; Fa.Atomic.Stack

(setf x (atomic-stack-create))
(destroy x)
(atomic-stack-write x (cl:random 20))
(atomic-stack-read x)


; ---------------------------------------------------------------------------------------------------

; Fa.Thread
;
; We can use an faudio mutex to syncronize threads created by Lisp and vice versa.

(setf x (thread-create* (lambda () 
                          (fa-log-info 
                           "And there's someone in a thread."))))
(thread-join x)
(thread-detach x)

(thread-sleep 3000)

(thread-main)
(thread-current)

(defvar *my-thread* nil)
(cl:print *my-thread*)
(mp:process-send mp:*main-process*
  (setf *my-thread* (thread-current))
 )

(equal 
  (thread-current) (thread-main))

; ---------------------------------------------------------------------------------------------------
; Mutexes

(setf y (thread-create-mutex))
(destroy y)

(thread-lock y)
(thread-try-lock y)
(thread-unlock y)

(thread-holding
 (y)
 (fa-log-info "I have the mutex"))

(thread-create* (lambda ()
  (fa-log-info "I am waiting.")
  (thread-holding (y) 
                  (fa-log-info "Finally, got it!"))))



; ---------------------------------------------------------------------------------------------------

; Signals

(setf x (stime))
(setf x (counter))
(setf x (random))
(setf x (constant 0.5))
(setf x (sin (line 440.0)))
(setf x (+ x 0))
(setf x (* x 2))
(setf x (constant pi))

(signal-print* 10 x)
(signal-run-buffer (cl:* 44100 60) (cl:list) x)
(signal-run-file (cl:* 44100 60) (cl:list) x "/Users/hans/audio/test.wav")
(signal-run-default (lambda (inputs) (duplicate (* 0.2 x))))     ; Don't play too loud


(signal-print* 10 (stime))
(signal-print* 10 (counter))


(signal-print* 10 (signal-input 64))




;;;;;;;;;

(milliseconds 1)
(defun ms (x) (milliseconds x))

; TODO move

(hours 1)
(seconds 1.5)
(minutes 1)
(ms 500)
(ms 2000)

(time-divisions (seconds 7.5))
(time-to-milliseconds (seconds 1/17))
(sign

(signal-run-file* 
 (cl:* 44100 60)
 (* (* (* (stime) 0.1) (signal-input 32)) (sin (line 440)))
 :path "/Users/hans/audio/test.wav"
 :controls (cl:list 
            (pair-create (seconds 1)   (action-set 32 0.5D0))
            (pair-create (seconds 3)   (action-set 32 0.99D0))
            (pair-create (seconds 7.1) (action-set 32 0.0D0))
            (pair-create (seconds 45)  (action-set 32 0.1D0))
            ))

(setf se (audio-begin-session))
(setf i (audio-default-input se))
(setf o (audio-default-output se))
(setf s (audio-open-stream* i o (lambda (_) (cl:list   
                                             (* (input 32) (* 0.1 (sin (line 440))))
                                             ))))
(audio-schedule (seconds 20) (action-set 32 0.5D0) s)
(audio-schedule (seconds 55) (action-set 32 0.1D0) s)
(audio-schedule (seconds 0) (action-set 32 0.0D0) s)
(destroy s)
(destroy se)





(signal-run-default (lambda (inputs) 
  (duplicate 
   (* 0.1 (sin (line 440))))))

;;;;;;;;;;

; Lifting Lisp callbacks to signal level

(defcallback div10 :double ((_ :pointer) (x :double))
  (the double-float x)
  (cl:* 0.1 x))
(defun attenuate (x)
  (signal-lift "(/10)" (callback div10) (cffi:null-pointer) x))
(type-of (attenuate (stime))) ; TODO problem with printing this

; TODO
; RT is much slower than NRT!
; Is this caused by the RT thread acquiring locks?
(signal-run-default (lambda (inputs) 
  (cl:list 
   (attenuate (sin (line 440))))))

(signal-run-file* (cl:* 44100 60)
                  (attenuate (sin (line 220)))
                  :path "/Users/hans/audio/test.wav")
(play-file "/Users/hans/audio/test.wav") ; FIXME mono vs stereo files
;;;;;;;;;;;




;;;;;;;;;;

; Misc

; Read file to buffer
(setf buf (from-pointer 'buffer (pair-second (buffer-read-audio* "/Users/hans/Desktop/Passager.wav"))))
(destroy buf)
(hcl:mark-and-sweep 3)
(fa-terminate)
(fa-initialize)

; Empty buffer
(setf buf (buffer-create (cl:* 88200 8 2)))

; Buffer from signal
(setf buf (signal-run-buffer 44100 (cl:list) (* 0.1 (random))))
(setf buf (signal-run-buffer 44100 (cl:list) (* 0.1 (sin (line 220)))))

(cl:print buf)
;(defcfun (plot-buffer-double "fa_plot_buffer_double") :void (a buffer) (b :pointer) (c :pointer))
(destroy buf)

; Record stereo buffer
(signal-run-default 
 (lambda (inputs)
   (let* ((j (counter))                 ; 0,1,2..
          (left-index (+ (* j 2) 0))    ; 0,2,4..
          (right-index (+ (* j 2) 1))   ; 1,3,5..
          (left (signal-record buf left-index (first inputs)))
          (right (signal-record buf right-index (second inputs))))
     (cl:list 
      (signal-latter left (constant 0)) 
      (signal-latter right (constant 0)))))) ; No output

; Play stereo buffer
(signal-run-default 
 (lambda (inputs)
   (let* ((j (counter))                 ; 0,1,2..
          (left-index (+ (* j 2) 0))    ; 0,2,4..
          (right-index (+ (* j 2) 1))   ; 1,3,5..
          (left (signal-play buf left-index))
          (right (signal-play buf right-index)))
     (cl:list left right)))) ; Stereo output

; Frequency modulation
(signal-run-default (lambda (inputs)
  (cl:list 
   (* (* 0.1 (stime))
    (* (sin (line 70))  (sin (line 550)))) 
   (* (* 0.1 (stime))
    (* (sin (line 141)) (cos (line 550)))))))

; Additive synthesis
(let* ((notes '(100 200 350 475 620))
       (sines (mapcar (lambda (x) (sin (line (cl:* 4 x)))) notes))
       (synth (* 0.05 (reduce (lambda (x y) (+ y x)) (reverse sines)))))
  (cl:print synth)
  (signal-run-default (lambda (_) (duplicate (* 0.1 synth)))))

; (reduce 'cl:+ '() :initial-value 1)

; Control value
(signal-run-default (lambda (inputs) 
  (duplicate 
   (* (input 29) (sin (line 440)))))

                    :stream-callback
                    (lambda (st) 
                      (cl:print st)
                      (audio-schedule (seconds 2) (action-set 29 0.1D0) st)
                      )
                    ; Wait 2 seconds!
                    )

; Echo inputs
;(signal-run-default 
; (lambda (inputs)
;   inputs))

; Sine
(signal-run-default
 (lambda (inputs)
   (duplicate (* 0.2 (sin (line 440))))))

; Echo input
(signal-run-default
 (lambda (inputs)
   (mapcar (lambda (x) (* x 0.1)) inputs)))

; Delay input
(signal-run-default
 (lambda (inputs)
   (mapcar (lambda (x) (* (delay 88200 x) 0.4)) inputs)))


(signal-dls*)

; DLSMusicDevice
(signal-run-default 
 (lambda (inputs) 
   (mapcar 
    (lambda (x) (* 0.5 x)) 
    (signal-dls*)))
 :stream-callback 
 (lambda (stream)
   (cl:print stream)
   (audio-schedule (milliseconds 0)   (action-send "DLS" (midi #x91 60 127)) stream)
   (audio-schedule (milliseconds 100) (action-send "DLS" (midi #x91 63 127)) stream)
   (audio-schedule (milliseconds 200) (action-send "DLS" (midi #x91 65 127)) stream)
   (audio-schedule (milliseconds 300) (action-send "DLS" (midi #x91 62 127)) stream)))

(signal-run-file* 
 (cl:* 44100 5) 
 (car (signal-dls*))
 :controls (cl:list 
            (pair-create (milliseconds 0)   (action-send "DLS" (midi #x91 60 127)))
            (pair-create (milliseconds 100) (action-send "DLS" (midi #x91 63 127)))
            (pair-create (milliseconds 200) (action-send "DLS" (midi #x91 65 127)))
            (pair-create (milliseconds 300) (action-send "DLS" (midi #x91 62 127)))
            ) 
 :path "/Users/hans/audio/test.wav")

; (action-send-name (action-send "DLS" (midi #x90 60 127)))
; (from-pointer 'midi-message (action-send-value (action-send "DLS" (midi #x91 60 127))))

(defun signal-run-many (xs)
  (signal-run-default (lambda (dummy) xs)))

(defun play-file (path)
  (let* ((j (counter))
         (left-index (+ (* j 2) 0))
         (right-index (+ (* j 2) 1))
         (buf (from-pointer 'buffer (pair-second (buffer-read-audio* path))))
         (left (signal-play buf left-index))
         (right (signal-play buf right-index))
         )
    (signal-run-many (cl:list left right))
    (destroy buf)))

(play-file "/Users/hans/Desktop/Passager.wav")
