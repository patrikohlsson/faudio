
MODULO=modulo --lisp-package=faudio

all:	c
bindings: lisp

c:
	mkdir -p include/fa
	mkdir -p include/fa/atomic
	mkdir -p include/fa/device
	mkdir -p include/fa/midi
	mkdir -p include/fa/pair
	mkdir -p include/fa/processor
	mkdir -p include/fa/processor/external
	mkdir -p include/fa/string
	mkdir -p include/fa/system
	mkdir -p include/fa/thread
	$(MODULO) -Mmodules --language=c    <modules/Fa.module    			>include/fa.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Fa.module    			>include/fa/fa.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Atomic.module    		>include/fa/atomic.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Atomic/Queue.module     	>include/fa/atomic/queue.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Atomic/Stack.module     	>include/fa/atomic/stack.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Atomic/RingBuffer.module	>include/fa/atomic/ring_buffer.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/System.module    		>include/fa/system.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/System/Directory.module 	>include/fa/system/directory.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Dynamic.module    		>include/fa/dynamic.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Thread.module    		>include/fa/thread.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Std.module    			>include/fa/std.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/String.module    		>include/fa/string.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Error.module    		>include/fa/error.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Buffer.module    		>include/fa/buffer.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Ratio.module    		>include/fa/ratio.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/List.module    			>include/fa/list.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Set.module    			>include/fa/set.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Pair.module    			>include/fa/pair.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Pair/Left.module    		>include/fa/pair/left.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Plot.module    			>include/fa/plot.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Map.module    			>include/fa/map.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Graph.module    		>include/fa/graph.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/PriorityQueue.module		>include/fa/priority_queue.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Midi/Message.module    		>include/fa/midi/message.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Time.module    			>include/fa/time.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Clock.module    		>include/fa/clock.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Action.module  			>include/fa/action.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Signal.module  			>include/fa/signal.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Audio.module    		>include/fa/audio.h && \
	$(MODULO) -Mmodules --language=c    <modules/Fa/Midi.module    			>include/fa/midi.h

lisp: fix-lisp
gen-lisp:
	mkdir -p bindings/lisp/fa
	mkdir -p bindings/lisp/fa/atomic
	mkdir -p bindings/lisp/fa/device
	mkdir -p bindings/lisp/fa/midi
	mkdir -p bindings/lisp/fa/processor
	mkdir -p bindings/lisp/fa/pair
	mkdir -p bindings/lisp/fa/processor/external
	mkdir -p bindings/lisp/fa/string
	mkdir -p bindings/lisp/fa/system
	mkdir -p bindings/lisp/fa/thread
	$(MODULO) -Mmodules --language=lisp <modules/Fa.module    			>bindings/lisp/fa.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Fa.module    			>bindings/lisp/fa/fa.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Atomic.module    		>bindings/lisp/fa/atomic.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Atomic/Queue.module     	>bindings/lisp/fa/atomic/queue.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Atomic/Stack.module     	>bindings/lisp/fa/atomic/stack.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Atomic/RingBuffer.module	>bindings/lisp/fa/atomic/ring-buffer.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Thread.module    		>bindings/lisp/fa/thread.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/System.module    		>bindings/lisp/fa/system.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/System/Directory.module 	>bindings/lisp/fa/system/directory.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Dynamic.module    		>bindings/lisp/fa/dynamic.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Std.module    			>bindings/lisp/fa/std.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/String.module    		>bindings/lisp/fa/string.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Error.module    		>bindings/lisp/fa/error.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Buffer.module    		>bindings/lisp/fa/buffer.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Ratio.module    		>bindings/lisp/fa/ratio.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/List.module    			>bindings/lisp/fa/list.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Set.module    			>bindings/lisp/fa/set.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Pair.module    			>bindings/lisp/fa/pair.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Pair/Left.module    		>bindings/lisp/fa/pair/left.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Plot.module    			>bindings/lisp/fa/plot.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Map.module    			>bindings/lisp/fa/map.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Graph.module    		>bindings/lisp/fa/graph.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/PriorityQueue.module		>bindings/lisp/fa/priority-queue.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Midi/Message.module    		>bindings/lisp/fa/midi/message.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Time.module    			>bindings/lisp/fa/time.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Clock.module			>bindings/lisp/fa/clock.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Action.module  			>bindings/lisp/fa/action.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Signal.module  			>bindings/lisp/fa/signal.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Audio.module    		>bindings/lisp/fa/audio.lisp && \
	$(MODULO) -Mmodules --language=lisp <modules/Fa/Midi.module    			>bindings/lisp/fa/midi.lisp

fix-lisp: gen-lisp
	perl -i -pe 's/\(defctype ptr/; (defctype ptr/g' bindings/lisp/fa.lisp

	perl -i -pe 's/\(defcfun \(signal-lift \"fa_signal_lift\"\) signal \(a string\) \(b signal-unary-double\) \(c ptr\) \(d signal\)\)/(defcfun (signal-lift "fa_signal_lift") signal (a string) (b :pointer) (c :pointer) (d signal))/g' bindings/lisp/fa/signal.lisp
	perl -i -pe 's/\(defcfun \(signal-lift2 \"fa_signal_lift2\"\) signal \(a string\) \(b signal-binary-double\) \(c ptr\) \(d signal\) \(e signal\)\)/(defcfun (signal-lift2 "fa_signal_lift2") signal (a string) (b :pointer) (c :pointer) (d signal) (e signal))/g' bindings/lisp/fa/signal.lisp
	perl -i -pe 's/\(defcfun \(signal-loop \"fa_signal_loop\"\) signal \(a signal-unary-signal\) \(b ptr\)\)/(defcfun (signal-loop "fa_signal_loop") signal (a :pointer) (b :pointer))/g' bindings/lisp/fa/signal.lisp

# perl -i -pe 's/A/B/g' bindings/lisp/fa.lisp

clean-lisp:
	find bindings -name '*.lisp' | xargs rm -v;