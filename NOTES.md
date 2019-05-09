# Notes

## Initial
- Setup build system with emcscripten (Don't forget to setup environment)
- Call `emconfigure cmake ..` to setup
- Call `emmake make` to build
- All build files go into the web directory in the build folder 

## 5/6/2019
- Added imgui by copying source and headers into the external directory.
- Added index array or GL_ELEMENT_ARRAY_BUFFER
- Added color to the shader.
Next: Create a shader class.

## 5/8/2019
- Was working on a deploy script for deploying to a server
- Found out that WebGL2 is still not supported by a lot of browsers (Safari)
  and I don't really want to write old OpenGL 1 code that is basically obsolete.
  Might have to pivot over to using some available framework like three.js. Still
  want to target the web and still want to create 3D visuals.
