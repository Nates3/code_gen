@echo off

if not exist ..\build mkdir ..\build
pushd ..\build
set libs=kernel32.lib winmm.lib
set exe_name=code_gen
set opts=-Od -Z7 -nologo -c -FC -Fo:%exe_name% -TC
set linker_opts=-DEBUG:FULL -nologo

cl %opts% ..\code\codegen_main.c
link.exe %linker_opts% %exe_name%.obj %libs%
popd