@echo off
setlocal

set SHADERC=shadercDebug.exe
set SRC_DIR=..\res\shaders_src
set OUT_DIR=..\res\shaders
set INCLUDE_DIR=..\libs\include\BGFX\shader_include

if not exist %OUT_DIR% mkdir %OUT_DIR%

echo --- Starting Shader Compilation ---

for %%f in (%SRC_DIR%\vs_*.sc) do (
    echo Compiling Vertex Shader: %%~nxf
    %SHADERC% -f %%f -o %OUT_DIR%\%%~nf.bin --type vertex --platform windows --profile s_5_0 -i %INCLUDE_DIR%
)

for %%f in (%SRC_DIR%\fs_*.sc) do (
    echo Compiling Fragment Shader: %%~nxf
    %SHADERC% -f %%f -o %OUT_DIR%\%%~nf.bin --type fragment --platform windows --profile s_5_0 -i %INCLUDE_DIR%
)

echo --- Compilation Finished ---
pause
