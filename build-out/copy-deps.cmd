REM Copy deps for running

mkdir Debug
mkdir Release
mkdir Debug-x64
mkdir Release-x64
copy ..\dependencies\lib_x86_debug\*.dll Debug
copy ..\dependencies\lib_x86_debug\*.pdb Debug
copy ..\dependencies\lib_x86_release\*.dll Release
copy ..\dependencies\lib_x64_debug\*.dll Debug-x64
copy ..\dependencies\lib_x64_debug\*.pdb Debug-x64
copy ..\dependencies\lib_x64_release\*.dll Release-x64
