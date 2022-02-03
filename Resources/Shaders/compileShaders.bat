@echo off

set currentDir=%~dp0
set OutDir=%1

rem Compile glsl shaders to SPIR-V
for /r %currentDir% %%f in (*.vert) do D:\dev\Vulkan\Bin\glslc.exe %%f -o %%~dpnf%V.spv
for /r %currentDir% %%f in (*.frag) do D:\dev\Vulkan\Bin\glslc.exe %%f -o %%~dpnf%F.spv

rem Move all .spv files to Output
mkdir %OutDir%Resources\Shaders\
xcopy /y %currentDir%*.spv %OutDir%Resources\Shaders\ /s
for /r %currentDir% %%f in (*.spv) do del %%f