@echo off
REM Deployment script for microfluidics control system software, for Windows
REM Prerequisites:
REM   - Qt 5.11
REM   - Visual Studio Build tools
REM   - InnoSetup5 (to create the installer)
REM
REM Usage: update QT_DIR and other relevant paths, and run this script.

set QT_DIR=C:\Qt\5.11.1\msvc2017_64
set QT_BIN=%QT_DIR%\bin
set APP_NAME=ufcs-pc

set PROGRAM_FILES=C:\Program Files (x86)
set VS_BIN=%PROGRAM_FILES%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build
set INNO_EXE=%PROGRAM_FILES%\Inno Setup 5\iscc.exe

set SCRIPT_PATH=%~dp0
set BUILD_DIR=%SCRIPT_PATH%\..\build

echo -------------------------------------------------
echo Checking directories...
echo -------------------------------------------------
IF NOT EXIST "%QT_BIN%\qmake.exe" (
    echo Could not find qmake.
    GOTO EXIT_WITH_ERROR
)
IF NOT EXIST "%VS_BIN%\" (
    echo Could not find Visual Studio Build Tools.
    GOTO EXIT_WITH_ERROR
)
IF NOT EXIST "%INNO_EXE%" (
    echo Could not find InnoSetup.
    GOTO EXIT_WITH_ERROR
)

echo -------------------------------------------------
echo Setting up environment...
echo -------------------------------------------------

call "%VS_BIN%\vcvars64.bat"
set PATH=%QT_BIN%;%PATH%


REM QMake places the makefiles and build directories in the current directory.
REM So the easiest way to change build directories is just to cd to the desired place.
cd %BUILD_DIR%

echo -------------------------------------------------
echo Building application...
echo -------------------------------------------------

"%QT_BIN%\qmake.exe" "%SCRIPT_PATH%\\..\\%APP_NAME%.pro"

nmake release

IF NOT EXIST "%BUILD_DIR%\release\%APP_NAME%.exe" GOTO EXIT_WITH_ERROR
cd release

echo -------------------------------------------------
echo Copying Qt files...
echo -------------------------------------------------

"%QT_BIN%\windeployqt.exe" "%APP_NAME%.exe" "--qmldir" "%SCRIPT_PATH%\..\src\qml"

echo -------------------------------------------------
echo Deleting temporary build files...
echo -------------------------------------------------

del *.obj
del src_qml_*
del moc_*
del qmlcache_loader.cpp
del qrc_res_*

echo -------------------------------------------------
echo Creating installer...
echo -------------------------------------------------
call "%INNO_EXE%" "%SCRIPT_PATH%\create_installer.iss"

GOTO END

:EXIT_WITH_ERROR
echo -------------------------------------------------
echo Build failed :(
echo -------------------------------------------------
GOTO EOF

:END
echo -------------------------------------------------
echo Build finished!
echo -------------------------------------------------

:EOF
