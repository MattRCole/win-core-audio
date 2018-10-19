var m = require('./build/Debug/winCoreAudio.node');

var deflt = new m.Default();

var mic = deflt.media.mic;

console.log(mic);

var speaker = deflt.media.speaker;

console.log(speaker);

// speaker.onVolumeChange(function(newVol) { console.log(newVol)});

// speaker.onMuteChange(function(muteStatus) { console.log(muteStatus) } );

speaker.lockVolume(50);

// speaker.unlockVolume();

speaker.lockMute(false);

// speaker.unlockMute();

var interval = setInterval( function() {console.log('waiting');}, 1000);

console.log('hello')