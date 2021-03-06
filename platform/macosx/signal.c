
#include <fa/signal.h>
#include <fa/action.h>
#include <fa/midi/message.h>
#include <fa/util.h>

// DLSMusicDevice wrapper
// TODO clean

typedef struct au_context *au_context_t;

au_context_t create_au_context(ptr_t instance, int channels, int frames);
void destroy_au_context(au_context_t context);
ptr_t new_dls_music_device_instance();
void au_prepare(au_context_t context);
void au_send_midi(au_context_t context, int status, int data1, int data2);
void au_render(au_context_t context, double time, double *output);
void au_cleanup(au_context_t context);

void fa_midi_message_decons(fa_midi_message_t midi_message, int *statusCh, int *data1, int *data2);


ptr_t before_(ptr_t x, fa_signal_state_t *state)
{
    au_context_t context = x;
    au_prepare(context);
    return x;
    mark_used(context);
}

ptr_t after_(ptr_t x, fa_signal_state_t *state)
{
    au_context_t context = x;
    au_cleanup(context);
    return x;
    mark_used(context);
}

struct au_context {
    double *outputs;
};

#define kAUVec 32

ptr_t render_(ptr_t x, fa_signal_state_t *state)
{
    au_context_t context = x;

    // context->Time = state->count;
    if (state->count % kAUVec == 0) {
        au_render(context, state->count, NULL);
    }

    state->inputs[32] = context->outputs[kAUVec * 0 + (state->count % kAUVec)];
    state->inputs[33] = context->outputs[kAUVec * 1 + (state->count % kAUVec)];

    return x;
    mark_used(context);
}

ptr_t receive_(ptr_t x, fa_signal_name_t n, fa_signal_message_t msg)
{
    au_context_t context = x;
    assert(fa_midi_message_is_simple(msg));
    // printf("Message: %s\n", unstring(fa_string_show(msg)));

    // TODO pre-allocate
    if (true /*fa_equal(n, string("DLS"))*/) {
        int status, data1, data2;
        fa_midi_message_decons(msg, &status, &data1, &data2);
        au_send_midi(context, status, data1, data2);
    }

    return x;
    mark_used(context);
}

// Pair of signals
pair_t fa_signal_dls()
{
    au_context_t context = create_au_context(new_dls_music_device_instance(), 2, kAUVec);
    // TODO destroy

    fa_signal_custom_processor_t *proc = fa_malloc(sizeof(fa_signal_custom_processor_t));
    proc->before  = before_;
    proc->after   = after_;
    proc->render  = render_;
    proc->receive = receive_;
    proc->data    = context;

    // printf("Sending custom proc %p!\n", proc);

    signal_t left  = fa_signal_input(32);
    signal_t right = fa_signal_input(33);
    signal_t left2 = fa_signal_custom(proc, left);
    return pair(left2, right);
    mark_used(left2);
}
