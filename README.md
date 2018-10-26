# win-core-audio
Bringing the Windows Core Audio API to NodeJS

A simple, intuitive, and powerful way to interact with audio devices on Windows with only two external package dependancies: `node-gyp` and `nan`!

### The following syntax is a work in progress and is nearly complete
The format may change before intial package publication.

For context:

    var audioController = require('win-core-audio');

set the volume of the system speakers or mic:

    audioController.defaultMedia.speaker.volume = 15;
    audioController.defaultMedia.mic.volume = 15;

make stuff easier on yourself when you write documentation:

    var defaultMedia = audioController.defaultMedia;
    var defaultCommunications = audioController.defaultCommunications;

get the volume of the media speaker/mic:

    var currentVolume = defaultMedia.speaker.volume;

mute communications mic:

    defaultCommunications.mic.isMuted = true;

set a new speaker as the default media speaker:

    var speakers = audioController.speakers;
    var desiredSpeaker;
    speakers.some(speaker => {
        if(speaker.name.includes('Name of speaker I want to be default'))
            { desiredSpeaker = speaker; return true; }
        else
            return false;
    });

    defaultMedia.speaker = desiredSpeaker;

disable speaker/mic:

    defaultCommunications.mic.state = 'disabled';

activate speaker/mic:

    defaultMedia.speaker.state = 'active';

get the current speaker state:

    var currentState = defaultMedia.speaker.state;

do something when the default speaker/mic is changed:

    defaultMedia.speaker.onChange( newSpeaker => {
        //Do stuff
    });

do something when the volume of a speaker changes:

    audioController.speakers[0].onVolumeChange( newVolume => {
        //Do stuff
    });

lock the volume to a given ammount (between 0 and 100):

    desiredSpeaker.lockVolume(42);

unlock the volume:

    desiredSpeaker.unlokVolume();

lock the default media speaker to a given speaker:

    defaultMedia.lock(desiredSpeaker);

unlock the default  speaker/mic:

    defaultCommunications.unlockSpeaker();

### Nitty gritty stuff

It is not guaranteed that this variable:

    var defaultComSpeaker = defaultCommunications.speaker;

will always point to the default communications speaker, rather, it points to the speaker that currently is the default communications speaker,
even if the default communications speaker is changed to a different speaker.

However, this:

    defaultCommunications.speaker

will always point to the current default communications speaker

this:

    audioController.speakers

returns only `'active'` or `'disabled'` speakers, which should suffice for most uses. 

However this:

    audioController.allSpeakers

returns all speakers regardless of state. 90% of the time, this will return the same speakers/mics as the speakers property.

### Nitty gritty stuff about the Windows Core Audio API itself

The possible states for a speaker/mic on windows are as follows: `'active'`, `'disabled'`, `'unplugged'`, or `'not present'`.
A speaker or mic may be `'unplugged'` or `'not present'`, and can still have its state changed. For example: a device can be disabled so that when it is plugged in, it will default to a disabled state.
However, these changes will not become apparent by reading the speaker/mic's state.

A speaker that is not `'active'` may be set as a default speaker/mic, but the change of default device will not occure. The change may occure after the device becomes `'active'`.
