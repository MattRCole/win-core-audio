var winaudioComplete

//if (process.env.DEBUG) {
  winaudioComplete= require('./build/Debug/completewinaudio.node')
//} else {
//  winaudioComplete= require('./build/Release/completewinaudio.node')
//}

module.exports = winaudioComplete