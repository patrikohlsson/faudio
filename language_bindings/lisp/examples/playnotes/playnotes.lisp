
(require          :asdf)
(asdf:load-system :audio-engine)
(in-package       :audio-engine)

(defconstant +sfpath+ "/Users/hans/Documents/Kod/doremir/modus/app/resources/soundfonts/sound.sf2")

(defun send-audio ()
  (let ((opts (default-send-options)))
    (setf (kind opts) :audio) opts))

(defun playnotes ()
  (with-library
    (handler-case
      (let* ((output (default-audio-output-device))
             (plugin (load-dls-music-device))
             (processor (make-processor plugin))
             ; (processor (load-fluidsynth +sfpath+))
             (stream (open-device-stream 
                       :audio-output output 
                       :audio-processor processor)))
        (format *standard-output* "Starting audio...~%")
        (format *standard-output* "Playing notes on ~s~%" (name processor))
        (start stream)
        (send-later stream 0    '(#xc0 11)     (send-audio)) ; Change program to marimba
        (send-later stream 200  '(#x90 60 100) (send-audio)) ; Schedule note onsets
        (send-later stream 400  '(#x90 62 100) (send-audio))
        (send-later stream 600  '(#x90 63 100) (send-audio))
        (send-later stream 4000 '(#x90 60 0)   (send-audio)) ; Schedule note offsets
        (send-later stream 4000 '(#x90 62 0)   (send-audio))
        (send-later stream 4000 '(#x90 63 0)   (send-audio))
        (sleep 5)
        (stop stream))
      (audio-error (e) (print (message e)))))) 

#-deliver (playnotes)