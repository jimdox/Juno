

![alt text](https://github.com/jimdox/black_box/blob/master/juno_engine/res/snapshot-0-1-2d.jpg)


## Project Setup
Sandbox is an example project which has everything you need to get started. Currently, since glfw is not behaving well with gcc + Premake, you will need to install the glfw-dev (or glfw-x11) package on your system. This is an unfortunate extra step, but I am hoping to resolve this for both Windows and Linux soon. Most modern distributions will have glew installed by default, if not, then you will have to install that package also. 


### Status of Loading Custom .obj Files
In order to correctly load a wavefront file, apply 'split edges' along any texture seams for the model, this will duplicate vertices along that seam, as the renderer currently can only process unique vertices per any texture coordinate. Additionally, mark all faces as 'shade smooth', as otherwise blender will generate multiple normals per face, which is not currently supported.


### And Finally
This project is still early in development. Cross-platform support is planned, but I am currently exclusively focusing on Linux platforms until I can get around to fixing the Premake side of things to work with Windows and macOS.

    
