@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ---------------------------------------------------------------------------
rem make_submission_windows.bat
rem Creates correction_code\Projet\submission\submission.zip under Windows.
rem Uses only native Windows tools: cmd.exe and PowerShell Compress-Archive.
rem
rem Expected location:
rem   correction_code\Projet\submission\make_submission_windows.bat
rem ---------------------------------------------------------------------------

rem Variables
set "SUB_DIR=submission"
set "SUB_FILE=submission.zip"
set "FILES_EXT=cpp"
set "SRC_DIR=src"
set "INC_DIR=include"
set "OBJ_DIR=obj"
set "TARGET=main"
set "TMP_DIR=.submission_tmp"

rem We move to parent directory: correction_code\Projet
cd /d "%~dp0\.." >nul 2>nul
if errorlevel 1 (
    echo error: cannot move to project directory.
    exit /b 1
)

rem We check the existence of directories
if not exist "%SRC_DIR%\" (
    echo error: source directory %SRC_DIR% not found
)

if not exist "%INC_DIR%\" (
    echo error: include directory %INC_DIR% not found
)

if not exist "%OBJ_DIR%\" (
    echo error: objects directory %OBJ_DIR% not found
)

if not exist "%SUB_DIR%\" (
    echo error: submission directory %SUB_DIR% not found
    exit /b 1
)

rem We delete objects and executable manually, without make clean
if exist "%TARGET%" (
    del /f /q "%TARGET%" >nul 2>nul
)

if exist "%TARGET%.exe" (
    del /f /q "%TARGET%.exe" >nul 2>nul
)

if exist "%OBJ_DIR%\" (
    del /f /q "%OBJ_DIR%\*.o" >nul 2>nul
    del /f /q "%OBJ_DIR%\*.obj" >nul 2>nul
)

rem We delete previous archive and temporary directory
if exist "%SUB_DIR%\%SUB_FILE%" (
    del /f /q "%SUB_DIR%\%SUB_FILE%" >nul 2>nul
)

if exist "%TMP_DIR%\" (
    rmdir /s /q "%TMP_DIR%" >nul 2>nul
)

rem We create a temporary submission tree
mkdir "%TMP_DIR%" >nul 2>nul
mkdir "%TMP_DIR%\%SRC_DIR%" >nul 2>nul
mkdir "%TMP_DIR%\%INC_DIR%" >nul 2>nul
mkdir "%TMP_DIR%\%OBJ_DIR%" >nul 2>nul

rem We copy source files
if exist "%SRC_DIR%\*.%FILES_EXT%" (
    copy /y "%SRC_DIR%\*.%FILES_EXT%" "%TMP_DIR%\%SRC_DIR%\" >nul
)

rem We copy header files
if exist "%INC_DIR%\*.h" (
    copy /y "%INC_DIR%\*.h" "%TMP_DIR%\%INC_DIR%\" >nul
)

rem We copy Makefile, Makefile.common, Makefile.depend, etc.
for %%F in (Makefile*) do (
    if exist "%%F" (
        copy /y "%%F" "%TMP_DIR%\" >nul
    )
)

rem Compress-Archive usually ignores truly empty directories.
rem This harmless placeholder guarantees that obj/ exists in the ZIP.
type nul > "%TMP_DIR%\%OBJ_DIR%\.keep"

rem We create the zip archive of the submission
<nul set /p dummy="+ Creating submission archive : "

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$ErrorActionPreference = 'Stop'; Compress-Archive -Path '%TMP_DIR%\*' -DestinationPath '%SUB_DIR%\%SUB_FILE%' -Force" >nul 2>nul

if errorlevel 1 (
    echo error
    echo error: failed to create %SUB_DIR%\%SUB_FILE%
    rmdir /s /q "%TMP_DIR%" >nul 2>nul
    exit /b 1
)

if not exist "%SUB_DIR%\%SUB_FILE%" (
    echo error
    echo error: archive was not created.
    rmdir /s /q "%TMP_DIR%" >nul 2>nul
    exit /b 1
)

rem We delete the temporary submission tree
rmdir /s /q "%TMP_DIR%" >nul 2>nul

echo ok
exit /b 0
