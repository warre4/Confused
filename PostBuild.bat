@echo off

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

rem Copy nessecary files to the output directory

rem Engine .dll
copy /y %OutDir%..\%EngineName%\* %OutDir%

rem Third party files
if !%Configuration%! == !Debug! copy /y %ThirdPartyDir%vld\* %OutDir%

copy /y %ThirdPartyDir%SDL2\lib\* %OutDir%
copy /y %ThirdPartyDir%glfw-3.3.6\lib\* %OutDir%
copy /y %ThirdPartyDir%vulkan\* %OutDir%