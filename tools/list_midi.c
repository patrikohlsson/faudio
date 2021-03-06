
#include <fa/fa.h>
#include <fa/util.h>

/*
    This program prints the currently connected MIDI devices.

    The output consists of device declarations separated by blank lines.

    An example device declaration:

        Name:           Network Session 1
        Host:           CoreMIDI
        Input:          true
        Output:         false

    The actual output has no indentation.
 */

fa_midi_session_t print_midi_devices(fa_ptr_t _, midi_session_t session)
{
    // fa_print("Listing MIDI devices:\n", 0);
    // fa_print_ln(string(""));

    fa_for_each(x, fa_midi_all(session)) {
        fa_print("Name: %s\n", fa_string_to_string(fa_midi_name(x)));
        fa_print("Host: %s\n", fa_string_to_string(fa_midi_host_name(x)));
        fa_print("In:   %s\n", fb(fa_midi_has_input(x)));
        fa_print("Out:  %s\n", fb(fa_midi_has_output(x)));
        fa_print_ln(string(""));
    }
    return session;
}

int main(int argc, char const *argv[])
{
    fa_fa_initialize();
    fa_midi_with_session(
        print_midi_devices, NULL,
        fa_error_log, NULL);
    fa_fa_terminate();
}
