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
			"src/DeviceController.cc",
         "src/AudioDevice.cc"
         "src/WinAPIWrap.cpp",
		]
	}]
}