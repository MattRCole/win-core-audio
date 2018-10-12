var m = require('./build/Debug/winCoreAudio.node');

// function callback(array)
// {
//     for(i = 0; i < array.length; i++)
//     {
//         console.log(array[i]);
//     }

//     return
// }


// console.log(winaudio.registerCallback(callback));


// interval = setInterval(function() {
//     console.log('waiting');
// }, 1000)





var deflt = new m.Default();

var speaker = deflt.media.speaker;

console.log(speaker);