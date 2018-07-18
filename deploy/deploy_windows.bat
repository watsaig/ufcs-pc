@echo off

REM Deployment script for microfluidics control system software, for Windows

set QT_DIR=C:\Qt\5.11.1\msvc2017_64
set QT_BIN=%QT_DIR%\bin
set APP_NAME=ufcs-pc

REM Not sure if the next few lines are necessary
set PROGRAM_FILES=C:\Program Files (x86)
set VS_BIN=%PROGRAM_FILES%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build
call "%VS_BIN%\vcvars64.bat"

REM Ensure that the correct version of Qt is used for windeployqt imports
set PATH=%QT_BIN%;%PATH%

set SCRIPT_PATH=%~dp0
set BUILD_DIR=%SCRIPT_PATH%\..\build

REM rmdir /S /Q %BUILD_DIR%


REM QMake places the makefiles and build directories in the current directory.
REM So the easiest way to change build directories is just to cd to the desired place.
cd %BUILD_DIR%

"%QT_BIN%\qmake.exe" "%SCRIPT_PATH%\\..\\%APP_NAME%.pro"

nmake release

IF NOT EXIST "%BUILD_DIR%\release\%APP_NAME%.exe" GOTO EXIT_WITH_ERROR
cd release

"%QT_BIN%\windeployqt.exe" "%APP_NAME%.exe" "--qmldir" "%SCRIPT_PATH%\..\src\qml"


echo Deleting temporary build files...

del *.obj
del src_qml_*
del moc_*
del qmlcache_loader.cpp
del qrc_res_*


GOTO END

:EXIT_WITH_ERROR
echo "Build failed."
GOTO EOF

:END
echo Build finished.

:EOF
