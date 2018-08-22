var winaudio = require('./');

function callback(array)
{
    for(i = 0; i < array.length; i++)
    {
        console.log(array[i]);
    }

    return
}


console.log(winaudio.registerCallback(callback));


interval = setInterval(function() {
    console.log('waiting');
}, 1000)