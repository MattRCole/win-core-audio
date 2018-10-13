var m = require('./build/Release/winCoreAudio.node');

var deflt = new m.Default();

var speaker = deflt.media.speaker;

speaker.volume = 100;

console.log(deflt);