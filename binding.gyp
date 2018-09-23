{
	"targets": [{
		"target_name": "completewinaudio",
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