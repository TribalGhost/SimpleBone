for /f "usebackq delims=" %%i in (`vswhere.exe -prerelease -latest -property installationPath`) do (
set InstallDir=%%i
)
call %InstallDir%\VC\Auxiliary\Build\vcvarsall.bat x64

D:\focus\focus.exe