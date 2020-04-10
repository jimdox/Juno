

![alt text](https://github.com/jimdox/black_box/blob/master/juno_engine/res/juno-s.jpg)


## Project Setup
Sandbox is an example project which has everything you need to get started with Juno. Currently, since I glfw is not yet a git submodule, you will need to install the glfw-dev (glfw-x11) package on your system. This is an unfortunate extra step, but I am hoping to resolve this soon. Most modern distributions will have glew installed by default, if not, then that will also need to be installed in order to run. 


### On the Status of Loading Custom .obj Files
In order to correctly load a wavefront file, apply 'split edges' along any UV texture seams, this will duplicate vertices along that seam, as the renderer currently can only process unique vertices per any texture coordinate. Additionally, mark all faces as 'shade smooth', as otherwise blender will generate multiple normals per face, which is not currently supported.


### And Finally
This project is still early in development. Cross-platform support is planned, but currently I am exclusively targeting Linux platforms until I can get around to restructing the build system side of things to also work with windows and macOS.

    