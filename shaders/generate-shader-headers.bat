%VULKAN_SDK%\Bin\glslc.exe shader.vert -o vert.spv
%VULKAN_SDK%\Bin\glslc.exe shader.frag -o frag.spv

"..\bin\debug\bin2cpp.exe" gShaderVertex vert.spv ..\src\poc-engine\rendering\vulkan\shaders\vulkan-shader-vertex.hpp
"..\bin\debug\bin2cpp.exe" gShaderFragment frag.spv ..\src\poc-engine\rendering\vulkan\shaders\vulkan-shader-fragment.hpp

pause
