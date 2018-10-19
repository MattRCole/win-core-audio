{
	"targets": [{
		"target_name": "winCoreAudio",
		"include_dirs" : [
			"src",
			"<!(node -e \"require('nan')\")"
		],
		"sources": [
			"src/index.cc",
			"src/asyncCBclass.cpp",
			"src/AudioDeviceController.cc",
         "src/AudioDevice.cc",
         "src/WinAPIWrap.cpp",
         "src/common.cpp",
         "src/CallbackNotificationManager.cc",
		]
	}]
}