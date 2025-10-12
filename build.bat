@echo off
SETLOCAL

:: --------------------------
:: Config
:: --------------------------
SET APP_NAME=qt_app.exe
SET SOURCE_DIR=%~dp0
SET SOURCE_DIR=%SOURCE_DIR:~0,-1%
SET QT_DIR=C:\Qt\6.9.3\msvc2022_64
SET QML_DIR=%SOURCE_DIR%\qml
SET CONFIG=Release
SET BUILD_DIR=%SOURCE_DIR%\build

:: --------------------------
:: Step 1: Create build folder
:: --------------------------
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: --------------------------
:: Step 2: Run CMake to configure
:: --------------------------
echo Configuring project with CMake...
cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_DIR%"

:: --------------------------
:: Step 3: Build the project
:: --------------------------
echo Building project...
cmake --build "%BUILD_DIR%" --config %CONFIG%

:: --------------------------
:: Step 4: Deploy Qt runtime
:: --------------------------
echo Deploying Qt libraries with windeployqt...
"%QT_DIR%\bin\windeployqt.exe" --release --qmldir "%QML_DIR%" "%BUILD_DIR%\%CONFIG%\%APP_NAME%"

:: --------------------------
:: Done
:: --------------------------
echo.
echo ======================================
echo Build and deployment complete!
echo Executable is at: %BUILD_DIR%\%CONFIG%\%APP_NAME%
echo ======================================

ENDLOCAL
pause
