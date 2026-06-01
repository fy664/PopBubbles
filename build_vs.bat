@echo off
call "D:\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
cd /d "d:\Desktop\PopBubbles"
if exist build rmdir /s /q build
"D:\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build -G "Visual Studio 18 2026"
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
"D:\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build --config Debug
