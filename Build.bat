@echo off

set StartTime=%time%

cls
mkdir CodeGenerationbuild
pushd CodeGenerationbuild

set CodeGenBuildStartTime=%time%

::cl /w /Zi /FC /MD /Gm- ..\windows_code\d_codegeneration.c /link /INCREMENTAL:no /libpath:..\lib /out:CodeGeneration.exe /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup

set CodeGenBuildEndTime=%time%

set CodeGenStartTime=%time%

::CodeGeneration.exe >NUL

set CodeGenEndTime=%time%

popd

cls
mkdir build
pushd build

set build_parameters=/Zi /utf-8 /MD /Od /FC /we4020 /we4024 /we4133 /we4477 /we4047 /we4716 /we4002 /we4457 /we4715 /we4456 /we4459 /we4553 /we4142 /I ..\game_code

for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "datestamp=%YYYY%%MM%%DD%" & set "timestamp=%HH%%Min%%Sec%"
set "fullstamp=%YYYY%-%MM%-%DD%_%HH%-%Min%-%Sec%"

:: that is a lot of libs bro
type DONTREADIT > Lock.tmp 
del *.pdb

set DLLBuildStartTime=%time%

cl  %build_parameters% ..\windows_code\d_windows_game.c raylib.lib -LD /link /INCREMENTAL:no /PDB:DGame_%timestamp%.pdb /dll raylib.lib user32.lib opengl32.lib winmm.lib gdi32.lib shell32.lib ole32.lib comdlg32.lib ws2_32.lib /libpath:..\lib /out:DGame.dll /export:game_load

set DLLBuildEndTime=%time%

del Lock.tmp

set MainBuildStartTime=%time%

cl %build_parameters% ..\windows_code\d_windows_main.c /link /INCREMENTAL:no raylib.lib user32.lib opengl32.lib kernel32.lib winmm.lib gdi32.lib shell32.lib ole32.lib comdlg32.lib /libpath:..\lib /out:DAPP.exe /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup

mkdir ..\build2\
mkdir ..\build2\Game\
mkdir ..\build_temp\
robocopy ..\build2\Game\ ..\build_temp\ /mir /r:0 /w:0
robocopy ..\build\ ..\build2\ /mir /r:0 /w:0
robocopy ..\build_temp\ ..\build2\Game\
rmdir /s /q ..\build_temp\
rename ..\build2\DAPP.exe DAPP_2.exe

set MainBuildEndTime=%time%

del *.rdi
del *.exp
del *.obj
del *.lib

popd
:: i don't wanna use batch anymore

@echo Code Generation build Start:%CodeGenBuildStartTime%  End:%CodeGenBuildEndTime%
@echo Code Generation Start:%CodeGenStartTime%  End:%CodeGenEndTime%
@echo DLL Build Start:%DLLBuildStartTime%  End:%DLLBuildEndTime%
@echo Main Build Start:%MainBuildStartTime%  End:%MainBuildEndTime%
@echo Start:%StartTime%  End:%time%