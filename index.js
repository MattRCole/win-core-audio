var audioController = require('./build/Release/winCoreAudio.node');

var speaker = audioController.default.media.speaker;

// console.log(speaker);

// speaker.onVolumeChange(function(newVol) { console.log(newVol)});

// speaker.onMuteChange(function(muteStatus) { console.log(muteStatus) } );

// speaker.lockVolume(50);

// speaker.unlockVolume();

// speaker.lockMute(false);

// speaker.unlockMute();

speaker.onVolumeChange(function(newVolume) {console.log(newVolume)});

var newVolume = 0;
var mute = false;
var interval = setInterval( function() {
   newVolume = (newVolume + 1) % 101;
   let toMute = newVolume % 5;
   audioController.default.media.speaker.volume = newVolume;
   if(toMute === 0)
   {
      audioController.default.media.speaker.mute = mute;
      mute = !mute;
   }
}, 100);

// console.log('hello')