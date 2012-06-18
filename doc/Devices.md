

Device traits
========

Device traits:

- Interface: *input* and/or *output*
- Operation: *active* (a.k.a non-blocking) or *passive* (a.k.a blocking)
- Type: *audio*, *midi* or *raw*

Example:
    
    NonBlockingAudioDevice : ActiveAudioInputDevice
    BlockingAudioDevice : PassiveAudioInputDevice


Stream traits
========

Stream traits:

- Interface: *input* and/or *output*
- Type: *audio*, *midi* and/or *raw*