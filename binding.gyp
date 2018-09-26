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
			"src/CtoJS.cc",
         "src/WinAPIWrap.cpp",
		]
	}]
}