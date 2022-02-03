@echo off

setlocal enabledelayedexpansion

rem Variables
set Configuration=%1
set SolutionDir=%2
set OutDir=%3
set EngineName=%4
set ThirdPartyDir=%SolutionDir%\thirdParty\

echo Configuration: %Configuration%
echo SolutionDir: %SolutionDir%
echo OutDir: %OutDir%
echo EngineName: %EngineName%

rem Copy Engine .dll
copy /y %OutDir%..\%EngineName%\* %OutDir%

rem Copy Third party files
if !%Configuration%! == !Debug! copy /y %ThirdPartyDir%vld\* %OutDir%

copy /y %ThirdPartyDir%SDL2\lib\* %OutDir%
copy /y %ThirdPartyDir%glfw-3.3.6\lib\* %OutDir%
copy /y %ThirdPartyDir%vulkan\* %OutDir%

rem Copy Resources
xcopy /y %SolutionDir%Resources %OutDir%Resources\ /e /exclude:%SolutionDir%ExcludeResources.txt

rem Compile glsl shaders to SPIR-V (and move them to output directory)
%SolutionDir%Resources\Shaders\compileShaders.bat %OutDir%