# Ocean Simulation

Implementation of Gerstner waves from Nvidia GPU Gems Chapter 1 and Fournier Paper. Using Opengl and C++. The images used 64 waves, the breakers uses 32 waves.

![alt text](https://i.imgur.com/TZbiIwa.png "Calm Sunset")

![alt text](https://i.imgur.com/pwJzSbs.png "Calm Noon")

![alt text](https://i.imgur.com/F6Oj2rl.png "Breaker")

## Features
+ Blinn - Phong Shading
+ Gamma Correction
+ Normal Mapping
+ Reflection (Environtment and Fresnel)
+ Foam or Breaker Generation (Limited Application)

## Libraries 
+ GLFW (https://www.glfw.org/)
+ GLM (https://github.com/g-truc/glm)
+ GLAD (https://glad.dav1d.de/)
+ STB_IMAGE (https://github.com/nothings/stb/blob/master/stb_image.h)

## References
+ Waves: https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
+ Fresnel: http://filmicworlds.com/blog/everything-has-fresnel/
+ Opengl: https://learnopengl.com/Introduction
+ Fournier & Tessendorf Papers
